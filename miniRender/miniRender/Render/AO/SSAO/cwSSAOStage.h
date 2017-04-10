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

#ifndef __CW_SSAO_STAGE_H__
#define __CW_SSAO_STAGE_H__

#include "Base/cwMacros.h"
#include "Render/Stage/cwStage.h"

NS_MINIR_BEGIN

class cwTexture;
class cwEntity;
class cwSprite;

class cwSSAOStage : public cwStage
{
public:
	CW_CREATE_FUNC(cwSSAOStage);

	cwSSAOStage();
	virtual ~cwSSAOStage();

	virtual CWVOID bindingResultParameter(cwShader* pShader) override;
	virtual CWVOID showResult(const cwVector2D& pos, const cwVector2D& scale) override;

protected:
	virtual CWBOOL init() override;
	virtual CWBOOL buildEntity();
	virtual CWBOOL buildRenderTexture() override;

	virtual CWBOOL buildSSAOLayer();
	virtual CWBOOL buildBlurTexture();
	virtual CWBOOL buildHorizBlurLayer();
	virtual CWBOOL buildVertBlurLayer();

	virtual CWVOID showResult() override;

protected:
	cwTexture* m_pBlurTexture;
	cwEntity* m_pScreenQuad;

	cwSprite* m_pSSAOSprite;

};

NS_MINIR_END

#endif
