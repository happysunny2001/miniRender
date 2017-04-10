/*
Copyright © 2015-2016 Ziwei Wang

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "cwSSAOStageLayer.h"
#include "Base/cwStruct.h"
#include "Generator/cwRenderObjectGenerator.hpp"
#include "Entity/cwEntity.h"
#include "Math/cwMatrix4X4.h"
#include "Math/cwMathUtil.h"
#include "Camera/cwCamera.h"
#include "Shader/cwShader.h"
#include "Texture/cwTexture.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "effect/cwEffect.h"
#include "effect/Parameter/cwEffectTextureParameter.h"
#include "effect/Parameter/cwEffectFloatVectorArrayParameter.h"

NS_MINIR_BEGIN

cwSSAOStageLayer::cwSSAOStageLayer():
m_pRandVectorTexture(nullptr)
{

}

cwSSAOStageLayer::~cwSSAOStageLayer()
{
	CW_SAFE_RELEASE_NULL(m_pRandVectorTexture);
}

CWBOOL cwSSAOStageLayer::init()
{
	if (!cwStageLayer::init()) return CWFALSE;
	if (!buildRandVector()) return CWFALSE;
	if (!buildRandVectorTexture()) return CWFALSE;

	m_nFilterType = eStageLayerFliterSelf;

	return CWTRUE;
}

CWVOID cwSSAOStageLayer::setUniformEffect(cwEffect* pEffect)
{
	cwStageLayer::setUniformEffect(pEffect);

	if (m_pUniformEffect) {
		cwEffectTextureParameter* pTexParameter = cwEffectTextureParameter::create();
		pTexParameter->setParameterName("gSSAORandVecMap");
		pTexParameter->setTexture(m_pRandVectorTexture);
		m_pUniformEffect->addParameter(pTexParameter);

		cwEffectFloatVectorArrayParameter* pVecParameter = cwEffectFloatVectorArrayParameter::create();
		pVecParameter->setParameterName("gOffsetVectors");
		pVecParameter->setVectorArray(m_nRandVector, 14);
		m_pUniformEffect->addParameter(pVecParameter);
	}
}

CWVOID cwSSAOStageLayer::preFrameConfig(cwShader* pShader)
{
	static const cwMatrix4X4 matTextureTrans(
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f);

	cwStageLayer::preFrameConfig(pShader);
	if (pShader) {
		if (m_pCamera) {
			cwMatrix4X4 matProjTex = m_pCamera->getProjMatrix()*matTextureTrans;
			pShader->setVariableMatrix("gViewToTexSpace", matProjTex);
		}
	}
}

CWBOOL cwSSAOStageLayer::buildRandVector()
{
	//8 corners
	m_nRandVector[0].set(+1.0f, +1.0f, +1.0f, 0.0f);
	m_nRandVector[1].set(-1.0f, +1.0f, +1.0f, 0.0f);
	m_nRandVector[2].set(-1.0f, +1.0f, -1.0f, 0.0f);
	m_nRandVector[3].set(+1.0f, +1.0f, -1.0f, 0.0f);

	m_nRandVector[4].set(+1.0f, -1.0f, +1.0f, 0.0f);
	m_nRandVector[5].set(-1.0f, -1.0f, +1.0f, 0.0f);
	m_nRandVector[6].set(-1.0f, -1.0f, -1.0f, 0.0f);
	m_nRandVector[7].set(+1.0f, -1.0f, -1.0f, 0.0f);

	//6 faces
	m_nRandVector[8].set(+1.0f,  0.0f,  0.0f, 0.0f);
	m_nRandVector[9].set(-1.0f,  0.0f,  0.0f, 0.0f);

	m_nRandVector[10].set(0.0f, 0.0f, +1.0f, 0.0f);
	m_nRandVector[11].set(0.0f, 0.0f, -1.0f, 0.0f);

	m_nRandVector[12].set(0.0f, +1.0f, 0.0f, 0.0f);
	m_nRandVector[13].set(0.0f, -1.0f, 0.0f, 0.0f);

	for (CWUINT i = 0; i < 14; ++i) {
		m_nRandVector[i].normalize();
	}

	return CWTRUE;
}

CWBOOL cwSSAOStageLayer::buildRandVectorTexture()
{
#define CW_SSAO_RAND_TEXTURE_SIZE 256
	cwVector4D* vecRand = new cwVector4D[CW_SSAO_RAND_TEXTURE_SIZE*CW_SSAO_RAND_TEXTURE_SIZE];
	if (!vecRand) return CWFALSE;

	for (CWUINT i = 0; i < CW_SSAO_RAND_TEXTURE_SIZE; ++i) {
		for (CWUINT j = 0; j < CW_SSAO_RAND_TEXTURE_SIZE; ++j) {
			vecRand[i * CW_SSAO_RAND_TEXTURE_SIZE + j].set(
				cwMathUtil::randFloat(), 
				cwMathUtil::randFloat(), 
				cwMathUtil::randFloat(), 
				0.0f);
		}
	}

	cwDevice* pDevice = cwRepertory::getInstance().getDevice();
	m_pRandVectorTexture = pDevice->createTexture(
		vecRand, 
		CW_SSAO_RAND_TEXTURE_SIZE, 
		CW_SSAO_RAND_TEXTURE_SIZE, 
		sizeof(cwVector4D), 
		eFormatR8g8b8a8Unorm);
	CW_SAFE_RETAIN(m_pRandVectorTexture);

	CW_SAFE_DELETE_ARRAY(vecRand);

	return CWTRUE;
}

NS_MINIR_END
