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

#ifndef __CW_SHADOW_MAP_STAGE_LAYER_H__
#define __CW_SHADOW_MAP_STAGE_LAYER_H__

#include "Base/cwMacros.h"
#include "Render/Stage/cwStageLayer.h"
#include "Math/cwAABB.h"

NS_MINIR_BEGIN

class cwOrthoCamera;

class cwShadowMapStageLayer : public cwStageLayer
{
public:
	CW_CREATE_FUNC(cwShadowMapStageLayer);

	cwShadowMapStageLayer();
	virtual ~cwShadowMapStageLayer();

	virtual CWVOID setDepthStencil(cwTexture* pTexture) override;
	inline const cwMatrix4X4& getShadowTransform() const { return m_nMatShadowTransform; }

	virtual CWVOID begin() override;
	virtual CWVOID render() override;
	virtual CWVOID end() override;

protected:
	virtual CWBOOL init() override;
	virtual CWVOID batchSceneRenderNodes() override;

	CWBOOL buildLightSourceCamera();
	CWVOID refreshLightSourceCamera();
	CWVOID refreshShadowCastState();

	virtual CWVOID preFrameConfig(cwShader* pShader) override;

protected:
	cwAABB m_nAabbShadowSpace;
	cwOrthoCamera* m_pShadowCamera;

	cwMatrix4X4 m_nMatTextureSpaceTrans;
	cwMatrix4X4 m_nMatShadowTransform;

	CWBOOL m_bHasLightingShadow;

};

NS_MINIR_END

#endif
