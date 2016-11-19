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

#ifndef __CW_TBDR_STAGE_H__
#define __CW_TBDR_STAGE_H__

#include "Base/cwMacros.h"
#include "Render/Stage/cwStage.h"
#include "cwTBDRConstants.h"

NS_MINIR_BEGIN

class cwTexture;
class cwMultiRTTexture;
class cwEntity;
class cwBuffer;
class cwEffectParameter;

class cwTBDRStage : public cwStage
{
public:
	CW_CREATE_FUNC(cwTBDRStage);

	cwTBDRStage();
	virtual ~cwTBDRStage();

protected:
	virtual CWBOOL init() override;

	virtual CWBOOL buildParam();
	virtual CWBOOL buildGBuffer();
	virtual CWBOOL buildDepthStencil();
	virtual CWBOOL buildLitTexture();
	//virtual CWBOOL buildFinalRenderTarget();
	virtual CWBOOL buildEntity();

	virtual CWBOOL buildClearStageLayer();
	virtual CWBOOL buildRenderScreenStageLayer();
	virtual CWBOOL buildTBDRCoreStageLayer();
	virtual CWBOOL buildResultStageLayer();

protected:
	typedef struct sLitBuffer
	{
		CWFLOAT r;
		CWFLOAT g;
		CWFLOAT b;
		CWFLOAT a;
	};

protected:
	cwMultiRTTexture* m_pGBuffer;
	cwTexture* m_pDepthStencil;
#if CW_ENABLE_MSAA
	cwBuffer* m_pLitTexture;
#else
	cwTexture* m_pLitTexture;
#endif
	//cwTexture* m_pFinalRenderTarget;

	cwEntity* m_pScreenQuad;
	cwEffectParameter* m_pParamFrameDim;

};

NS_MINIR_END

#endif
