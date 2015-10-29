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

#ifndef __CW_SPRITE_MANAGER_H__
#define __CW_SPRITE_MANAGER_H__

#include "Base/cwMacros.h"
#include "Base/cwStruct.h"
#include "Ref/cwRef.h"
#include "Engine/cwEngine.h"

#include <vector>
#include <unordered_set>

NS_MINIR_BEGIN

class cwSprite;
class cwRenderNode2D;
class cwDynamicRenderObject;
class cwCamera;
class cwEffect;
class cwBlend;

class CW_DLL cwSpriteManager : public cwRef
{
public:
	CWVOID begin();
	CWVOID render();
	CWVOID end();

	CWVOID addNode(cwRenderNode2D* pNode);
	CWVOID removeNode(cwRenderNode2D* pNode);

protected:
	static cwSpriteManager* create();

	cwSpriteManager();
	virtual ~cwSpriteManager();

	CWBOOL init();
	CWBOOL buildRenderObjects();
	CWBOOL buildEffect();
	CWBOOL buildBlend();

	CWVOID renderBatch(cwSprite* pSprite);

	CWVOID refreshSprite();
	CWVOID addRefreshNode(cwRenderNode2D* pNode);

	CWVOID renderNode();

	friend class cwEngine;

protected:
	CWUINT m_uVertexCnt;
	cwVertexPosTexColor m_nVertexBuffer[1000];
	cwDynamicRenderObject* m_pRenderObjects;

	cwRenderNode2D* m_pRootSprite;
	std::vector<cwRenderNode2D*> m_nDirtyNodes;

	cwCamera* m_pCurrCamera;
	cwCamera* m_pOrthoCamera;
	cwEffect* m_pDefEffect;
	cwBlend* m_pAlphaBlend;

};

NS_MINIR_END

#endif
