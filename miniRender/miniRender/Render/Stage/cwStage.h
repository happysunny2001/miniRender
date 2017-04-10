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
#include "Math/cwVector2D.h"
#include "cwPipelineNode.h"

NS_MINIR_BEGIN

class cwStageLayer;
class cwCamera;
class cwTexture;
class cwShader;
class cwSprite;
class cwEffect;

class cwStage : public cwPipelineNode
{
public:
	static cwStage* create();
	static cwStage* create(cwTexture* pRenderTexture);

	cwStage();
	virtual ~cwStage();

	cwPipelineNode* getChildPipelineNode(const CWSTRING& name);
	CWVOID addChildPipelineNode(cwPipelineNode* pPipelineNode);
	CWVOID removeChildPipelineNode(cwPipelineNode* pPipelineNode);

	inline cwTexture* getRenderTarget() const { return m_pResultRenderTexture; }
	virtual cwTexture* getRenderResult() const;

	virtual CWVOID doRender() override;

	virtual CWVOID showResult(const cwVector2D& pos, const cwVector2D& scale);

	inline CWBOOL isShowResult() const { return m_bShowResult; }
	inline CWVOID setShowResult(CWBOOL b) { m_bShowResult = b; }

protected:
	virtual CWBOOL init();
	virtual CWBOOL init(cwTexture* pRenderTexture);
	virtual CWBOOL buildRenderTexture();
	virtual CWVOID showResult();

	virtual CWVOID begin();
	virtual CWVOID render();
	virtual CWVOID end();

	cwSprite* createResultSprite(cwTexture* pTexture, cwEffect* pEffect);

protected:
	cwVector<cwPipelineNode*> m_nVecStageLayers;

	cwTexture* m_pResultRenderTexture;

	CWBOOL m_bShowResult;
	cwVector2D m_nResultPos;
	cwVector2D m_nResultScale;
	
};

NS_MINIR_END

#endif