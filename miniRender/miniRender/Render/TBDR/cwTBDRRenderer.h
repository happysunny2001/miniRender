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

#ifndef __CW_TBDR_RENDERER_H__
#define __CW_TBDR_RENDERER_H__

#include "Base/cwMacros.h"
#include "../cwRenderer.h"
#include "Light/cwPointLight.h"

#include <vector>

NS_MINIR_BEGIN

class cwStage;
class cwTexture;
class cwMultiRTTexture;
class cwEffect;
class cwGPEffect;
class cwEntity;
class cwBuffer;

class cwTBDRRenderer : public cwRenderer
{
public:
#define MAX_LIGHTS 400
	static cwTBDRRenderer* create();

	cwTBDRRenderer();
	virtual ~cwTBDRRenderer();

	virtual CWBOOL init() override;
	virtual CWVOID configLight() override;

protected:
	virtual CWVOID perFrameConfig() override;

protected:
	CWBOOL buildGBuffer();
	CWBOOL buildDepthStencil();
	CWBOOL buildLitTexture();
	CWBOOL buildFinalRenderTarget();

	CWBOOL buildEntity();
	CWBOOL buildEffect();

	CWBOOL buildClearStage();
	CWBOOL buildRenderScreenStage();
	CWBOOL buildTBDRCoreStage();
	CWBOOL buildResultStage();
	CWBOOL buildSpriteStage(CWBOOL bEnable);

	CWBOOL buildPointLightBuffer();

protected:
	cwMultiRTTexture* m_pGBuffer;
	cwTexture* m_pDepthStencil;
	cwTexture* m_pLitTexture;
	cwTexture* m_pFinalRenderTarget;

	cwEntity* m_pClearScreenQuad;
	cwEffect* m_pEffectClearGBuffer;

	cwEntity* m_pCoreScreenQuad;
	cwGPEffect* m_pEffectTBDRCore;
	
	cwEntity* m_pResultScreenQuad;
	cwEffect* m_pEffectResult;

	cwStage* m_pTBDRCoreStage;
	cwBuffer* m_pPointLightBuffer;
	vector<cwPointLight::PointLight_Struct> m_nVecViewSpacePointLights;

};

NS_MINIR_END

#endif
