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
#include "Render/cwRenderer.h"

#include <vector>

NS_MINIR_BEGIN

class cwSprite;
class cwRenderNode;
class cwDynamicRenderObject;
class cwCamera;
class cwEffect;
class cwStencil;

class cwSpriteManager : public cwRef
{
public:
	CWVOID begin();
	CWVOID render();
	CWVOID end();

protected:
	static cwSpriteManager* create();

	cwSpriteManager();
	virtual ~cwSpriteManager();

	CWBOOL init();
	CWBOOL buildRenderObjects();
	CWBOOL buildEffect();

	CWVOID renderBatch(cwSprite* pSprite);

	CWVOID refreshSprite();
	CWVOID addRefreshNode(cwRenderNode* pNode);

	friend class cwRenderer;

protected:
	std::vector<cwRenderNode*> m_nVecSprites;
	CWUINT m_uVertexCnt;
	cwVertexPosTexColor m_nVertexBuffer[1000];
	cwDynamicRenderObject* m_pRenderObjects;

	cwRenderNode* m_pRootSprite;
	std::vector<cwRenderNode*> m_nDirtyNodes;

	cwCamera* m_pCurrCamera;
	cwEffect* m_pDefEffect;

};

NS_MINIR_END

#endif
