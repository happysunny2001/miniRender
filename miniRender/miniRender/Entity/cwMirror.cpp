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

#include "cwMirror.h"
#include "Base/cwStruct.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Blend/cwBlend.h"
#include "Stencil/cwStencil.h"
#include "Engine/cwEngine.h"
#include "Render/cwStageLayer.h"
#include "Render/cwStage.h"
#include "Render/cwRenderer.h"
#include "Render/cwRenderer.h"

NS_MINIR_BEGIN

cwMirror* cwMirror::create()
{
	cwMirror* pMirror = new cwMirror();
	if (pMirror && pMirror->init()) {
		pMirror->autorelease();
		return pMirror;
	}

	CW_SAFE_DELETE(pMirror);
	return nullptr;
}

cwMirror::cwMirror():
m_pBlendTransparent(nullptr),
m_pStencilDrawRef(nullptr),
m_pStage(nullptr)
{

}

cwMirror::~cwMirror()
{
	CW_SAFE_RELEASE_NULL(m_pBlendTransparent);
	CW_SAFE_RELEASE_NULL(m_pStencilDrawRef);
	CW_SAFE_DELETE(m_pStage);
}

CWBOOL cwMirror::init()
{
	if (!cwEntity::init()) return CWFALSE;

	buildBlend();
	buildStencil();
	buildStage();

	m_eType = eSceneObjectMirror;

	return CWTRUE;
}

CWVOID cwMirror::buildBlend()
{
	BlendData blendNotDraw;
	blendNotDraw.bEnable       = CWFALSE;
	blendNotDraw.srcBlend      = eBlendFactorOne;
	blendNotDraw.dstBlend      = eBlendFactorZero;
	blendNotDraw.blendOp       = eBlendOpAdd;
	blendNotDraw.srcBlendAlpha = eBlendFactorOne;
	blendNotDraw.dstBlendAlpha = eBlendFactorZero;
	blendNotDraw.blendOpAlpha  = eBlendOpAdd;
	blendNotDraw.uColorEnable  = eColorWriteEnableNone;

	cwBlend* pBlendNotDraw = cwRepertory::getInstance().getDevice()->createBlend(blendNotDraw);
	this->setBlend(pBlendNotDraw);

	BlendData blendTrans;
	blendTrans.bEnable = CWTRUE;
	blendTrans.srcBlend = eBlendFactorSrcAlpha;
	blendTrans.dstBlend = eBlendFactorInvSrcAlpha;
	blendTrans.blendOp = eBlendOpAdd;
	blendTrans.srcBlendAlpha = eBlendFactorOne;
	blendTrans.dstBlendAlpha = eBlendFactorZero;
	blendTrans.blendOpAlpha = eBlendOpAdd;
	blendTrans.uColorEnable = eColorWriteEnableAll;

	m_pBlendTransparent = cwRepertory::getInstance().getDevice()->createBlend(blendTrans);
	CW_SAFE_RETAIN(m_pBlendTransparent);
}

CWVOID cwMirror::buildStencil()
{
	StencilData stencilMask;
	stencilMask.depthEnable      = CWTRUE;
	stencilMask.depthWriteMask   = eDepthWriteMaskZero;
	stencilMask.depthFunc        = eComparisonLess;
	stencilMask.stencilEnable    = CWTRUE;
	stencilMask.stencilReadMask  = 0xff;
	stencilMask.stencilWriteMask = 0xff;

	stencilMask.frontStencilFailOp      = eStencilOpKeep;
	stencilMask.frontStencilDepthFailOp = eStencilOpKeep;
	stencilMask.frontStencilPassOp      = eStencilOpReplace;
	stencilMask.frontStencilFunc        = eComparisonAlways;

	stencilMask.backStencilFailOp      = eStencilOpKeep;
	stencilMask.backStencilDepthFailOp = eStencilOpKeep;
	stencilMask.backStencilPassOp      = eStencilOpReplace;
	stencilMask.backStencilFunc        = eComparisonAlways;

	cwStencil* pStencilMask = cwRepertory::getInstance().getDevice()->createStencil(stencilMask);
	pStencilMask->setStencilRef(1);
	this->setStencil(pStencilMask);

	StencilData stencilDrawRef;
	stencilDrawRef.depthEnable      = CWTRUE;
	stencilDrawRef.depthWriteMask   = eDepthWriteMaskAll;
	stencilDrawRef.depthFunc        = eComparisonLess;
	stencilDrawRef.stencilEnable    = CWTRUE;
	stencilDrawRef.stencilReadMask  = 0xff;
	stencilDrawRef.stencilWriteMask = 0xff;

	stencilDrawRef.frontStencilFailOp      = eStencilOpKeep;
	stencilDrawRef.frontStencilDepthFailOp = eStencilOpKeep;
	stencilDrawRef.frontStencilPassOp      = eStencilOpKeep;
	stencilDrawRef.frontStencilFunc        = eComparisonEqual;

	stencilDrawRef.backStencilFailOp      = eStencilOpKeep;
	stencilDrawRef.backStencilDepthFailOp = eStencilOpKeep;
	stencilDrawRef.backStencilPassOp      = eStencilOpKeep;
	stencilDrawRef.backStencilFunc        = eComparisonEqual;
	m_pStencilDrawRef = cwRepertory::getInstance().getDevice()->createStencil(stencilDrawRef);
	m_pStencilDrawRef->setStencilRef(1);
	CW_SAFE_RETAIN(m_pStencilDrawRef);
}

CWVOID cwMirror::setReflectPlane(const cwPlane& plane)
{
	m_nReflectPlane = plane;
	m_nMatReflect.reflect(m_nReflectPlane);

	if (m_pStage) {
		cwStageLayer* pStageLayer = m_pStage->getStageLayer(0);
		if (pStageLayer) {
			pStageLayer->setWorldTrans(m_nMatReflect);
		}
	}
}

CWVOID cwMirror::buildStage()
{
	m_pStage = new cwStage();
	cwStageLayer* pStageLayerEntity = new cwStageLayer();
	cwStageLayer* pStageLayerMirror = new cwStageLayer();

	if (!m_pStage) return;

	m_pStage->setRefreshRenderTarget(CWFALSE);
	m_pStage->setName("MirrorStage");
	m_pStage->setCamera(cwRepertory::getInstance().getEngine()->getDefaultCamera());

	pStageLayerEntity->setFliterType(eStageLayerFliterEntity);
	pStageLayerEntity->setStencil(m_pStencilDrawRef);
	pStageLayerEntity->setWorldTrans(m_nMatReflect);
	pStageLayerEntity->setRenderState(eRenderStateCW);
	m_pStage->addStageLayer(pStageLayerEntity);

	pStageLayerMirror->setFliterType(eStageLayerFliterMirror);
	pStageLayerMirror->setBlend(m_pBlendTransparent);
	m_pStage->addStageLayer(pStageLayerMirror);
}

CWVOID cwMirror::render()
{
	cwStage* pCurrStage = cwRepertory::getInstance().getEngine()->getRenderer()->getCurrRenderStage();
	if (m_pStage != pCurrStage)
		cwRepertory::getInstance().getEngine()->getRenderer()->addStageRealTime(m_pStage);
}

NS_MINIR_END
