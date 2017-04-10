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

#ifndef __CW_RENDERER_H__
#define __CW_RENDERER_H__

#include "Base/cwMacros.h"
#include "Ref/cwRef.h"
#include "Base/cwVector.h"
#include "Math/cwMath.h"
#include "Stage/cwPipelineNode.h"

NS_MINIR_BEGIN

#define CW_RENDERER_LIST_POOL_SIZE 128

class cwStage;
class cwCamera;
class cwRenderBatch;
class cwShader;
class cwEffect;
class cwViewPort;
class cwTouch;
class cwTexture;

class cwRenderer : public cwRef
{
public:
	struct sRendererListNode
	{
		cwStage* m_pStage;
		struct sRendererListNode* prev;
		struct sRendererListNode* next;
	};

public:
	static cwRenderer* create();

	cwRenderer();
	virtual ~cwRenderer();

	virtual CWBOOL init();

	virtual CWVOID addStage(cwStage* pStage);
	virtual CWVOID addStageRealTime(cwStage* pStage);

	virtual cwStage* getStage(const CWSTRING& strName);
	inline cwStage* getCurrRenderStage() const { return m_pCurrRenderStage; }
	virtual cwRay getPickingRayWorld(CWFLOAT fPosX, CWFLOAT fPosY);
	virtual cwRay getPickingRayWorld(cwTouch* pTouch);

	const CWSTRING& getFullPath() const { return m_strFullPath; }
	CWVOID setFullPath(const CWSTRING& strPath) { m_strFullPath = strPath; }

	virtual CWVOID begin();
	virtual CWVOID render();
	virtual CWVOID end();
	virtual CWVOID resize();

protected:
	virtual CWBOOL buildFinalRenderTarget();
	virtual CWBOOL buildFinalStage();
	virtual CWBOOL build2DStage();

	sRendererListNode* getAvaiableListNode();
	sRendererListNode* buildStageList();

protected:
	CWSTRING m_strFullPath;

	cwVector<cwStage*> m_nVecStage;
	cwStage* m_pCurrRenderStage;
	cwStage* m_pFinalStage;
	cwStage* m_p2DStage;
	cwTexture* m_pTexFinalRenderTarget;

	sRendererListNode m_nListNodePool[CW_RENDERER_LIST_POOL_SIZE];
	CWUINT m_iListPoolIndex;
	sRendererListNode* m_pRenderListHead;

};

NS_MINIR_END

#endif
