/*
Copyright © 2015 Ziwei Wang

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

#include "cwSkyDome.h"
#include "Base/cwBasicType.h"
#include "Base/cwStruct.h"
#include "Generator/cwGeometryGenerator.h"
#include "Repertory/cwRepertory.h"
#include "RenderObject/cwStaticRenderObject.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Shader/cwShaderConstant.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderManager.h"
#include "effect/cwEffect.h"
#include "effect/cwEffectTextureParameter.h"

NS_MINIR_BEGIN

cwSkyDome* cwSkyDome::create(const CWSTRING& strSkyTexture)
{
	cwSkyDome* pSkyDome = new cwSkyDome();
	if (pSkyDome && pSkyDome->init(strSkyTexture)) {
		pSkyDome->autorelease();
		return pSkyDome;
	}

	CW_SAFE_DELETE(pSkyDome);
	return nullptr;
}

cwSkyDome::cwSkyDome():
m_pTexSkyCubMap(nullptr)
{

}

cwSkyDome::~cwSkyDome()
{
	CW_SAFE_RELEASE_NULL(m_pTexSkyCubMap);
}

CWBOOL cwSkyDome::init(const CWSTRING& strSkyTexture)
{
	if (!cwEntity::init()) return CWFALSE;

	buildSkyRenderObject();
	buildSkyEffect(strSkyTexture);

	return CWTRUE;
}

CWVOID cwSkyDome::buildSkyRenderObject()
{
	cwGeometryGenerator::cwMeshData mesh;
	cwRepertory::getInstance().getGeoGenerator()->generateSphere(5000.0f, 30, 30, mesh);

	vector<cwVertexPos> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
	}

	m_pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPos), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "Pos");
	CW_SAFE_RETAIN(m_pRenderObj);
}

CWVOID cwSkyDome::buildSkyEffect(const CWSTRING& strSkyTexture)
{
	m_pTexSkyCubMap = cwRepertory::getInstance().getTextureManager()->getCubeTexture(strSkyTexture);
	CW_SAFE_RETAIN(m_pTexSkyCubMap);

	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderSkyDome);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	cwEffectTextureParameter* pEffectTextureParam = cwEffectTextureParameter::create();
	pEffectTextureParam->setParameterName(CW_SHADER_SKY_CUBE_MAP);
	pEffectTextureParam->setTexture(m_pTexSkyCubMap);
	pEffect->addParameter(pEffectTextureParam);

	this->setEffect(pEffect);
}

CWVOID cwSkyDome::refreshSpatialNode()
{

}

NS_MINIR_END
