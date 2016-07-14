﻿/*
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

NS_MINIR_BEGIN

#define CW_RENDERER_LIST_POOL_SIZE 128

class cwStage;
class cwCamera;
class cwRenderBatch;
class cwShader;
class cwEffect;
class cwPrimitiveEntity;
class cwSpriteManager;
class cwViewPort;
class cwTouch;

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

	virtual CWVOID setCurrCamera(cwCamera* pCamera);
	virtual CWVOID setRendererCamera(cwCamera* pCamera);
	virtual CWVOID setCurrShader(cwShader* pShader);

	virtual CWVOID createViewPort(CWFLOAT fTopLeftX, CWFLOAT fTopLeftY, CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fMinDepth, CWFLOAT fMaxDepth);

	virtual CWVOID addStage(cwStage* pStage);
	virtual CWVOID addStageRealTime(cwStage* pStage);

	virtual cwStage* getStage(const CWSTRING& strName);
	inline cwStage* getCurrRenderStage() const { return m_pCurrRenderStage; }
	inline cwCamera* getCurrCamera() const { return m_pCurrCamera; }
	inline cwCamera* getRendererCamera() const { return m_pRendererCamera; }
	virtual cwRay getPickingRayWorld(CWFLOAT fPosX, CWFLOAT fPosY);
	virtual cwRay getPickingRayWorld(cwTouch* pTouch);

	const CWSTRING& getFullPath() const { return m_strFullPath; }
	CWVOID setFullPath(const CWSTRING& strPath) { m_strFullPath = strPath; }

	virtual CWVOID begin();
	virtual CWVOID render();
	virtual CWVOID end();
	virtual CWVOID resize();

	virtual CWVOID render(cwRenderBatch* pBatch);
	virtual CWVOID configLight();

	CWVOID renderPrimitive(const cwAABB& aabb);
	CWVOID renderPrimitive(const cwAABB& aabb, const cwVector4D& color);

	CWVOID renderPrimitive(const cwRay& ray);
	CWVOID renderPrimitive(const cwRay& ray, CWFLOAT fLen, const cwVector4D& color);

	CWVOID renderPrimitive(cwCamera* pCamera);
	CWVOID renderPrimitive(cwCamera* pCamera, const cwVector4D& color);

	CWVOID renderPrimitiveEntity();

protected:
	virtual CWVOID render(cwStage* pStage);
	virtual CWVOID batchConfig(cwRenderBatch* pBatch);
	virtual CWVOID perFrameConfig();
	virtual CWVOID configDirectionalLight();
	virtual CWVOID configPointLight();
	virtual CWVOID configSpotLight();

	CWVOID buildPrimitiveEntity();

	sRendererListNode* getAvaiableListNode();
	sRendererListNode* buildStageList();

protected:
	CWSTRING m_strFullPath;

	cwVector<cwStage*> m_nVecStage;
	cwStage* m_pCurrRenderStage;

	cwPrimitiveEntity* m_pPrimitiveEntity;
	cwRenderBatch* m_pPrimitiveBatch;

	sRendererListNode m_nListNodePool[CW_RENDERER_LIST_POOL_SIZE];
	CWUINT m_iListPoolIndex;
	sRendererListNode* m_pRenderListHead;

	cwViewPort* m_pViewPort;
	CWFLOAT m_fViewPortTopLeftX;
	CWFLOAT m_fViewPortTopLeftY;
	CWFLOAT m_fViewPortWidth;
	CWFLOAT m_fViewPortHeight;
	CWFLOAT m_fViewPortMinDepth;
	CWFLOAT m_fViewPortMaxDepth;

	cwCamera* m_pRendererCamera;

	cwCamera* m_pCurrCamera;
	cwShader* m_pCurrShader;

};

NS_MINIR_END

#endif
