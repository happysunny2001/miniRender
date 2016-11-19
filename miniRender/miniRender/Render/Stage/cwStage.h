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

#ifndef __CW_STAGE_H__
#define __CW_STAGE_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Base/cwVector.h"
#include "Ref/cwRef.h"
#include "Math/cwVector2D.h"

NS_MINIR_BEGIN

class cwStageLayer;
class cwCamera;
class cwTexture;
class cwShader;

class cwStage : public cwRef
{
public:
	static cwStage* create();
	static cwStage* create(cwTexture* pRenderTexture);

	cwStage();
	virtual ~cwStage();

	inline const CWSTRING& name() const { return m_nName; }
	inline CWVOID setName(const CWSTRING& name) { m_nName = name; }

	cwStageLayer* getStageLayer(const CWSTRING& name);
	CWVOID addStageLayer(cwStageLayer* pStageLayer);
	CWVOID removeStageLayer(cwStageLayer* pStageLayer);

	inline cwCamera* getCurrUsingCamera() const { return m_pCurrUsingCamera; }
	inline cwTexture* getRenderTarget() const { return m_pResultRenderTexture; }
	virtual cwTexture* getRenderResult() const;
	inline CWBOOL enable() const { return m_bEnable; }
	inline CWVOID setEnable(CWBOOL b) { m_bEnable = b; }

	virtual CWVOID begin();
	virtual CWVOID render();
	virtual CWVOID end();

	virtual CWVOID bindingResultParameter(cwShader* pShader);
	virtual CWVOID showResult(const cwVector2D& pos, const cwVector2D& scale);

protected:
	virtual CWBOOL init();
	virtual CWBOOL init(cwTexture* pRenderTexture);
	virtual CWBOOL buildRenderTexture();

protected:
	CWSTRING m_nName;
	CWBOOL m_bEnable;

	cwCamera* m_pCurrUsingCamera;
	cwVector<cwStageLayer*> m_nVecStageLayers;

	cwTexture* m_pResultRenderTexture;
	
};

NS_MINIR_END

#endif