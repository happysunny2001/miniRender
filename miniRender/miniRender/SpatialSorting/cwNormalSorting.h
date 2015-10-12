﻿/*
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

#ifndef __CW_NORMAL_SORTING_H__
#define __CW_NORMAL_SORTING_H__

#include "Base/cwBasicType.h"
#include "Base/cwMacros.h"
#include "cwSpatial.h"

#include <list>
#include <unordered_set>

NS_MINIR_BEGIN

class CW_DLL cwNormalSorting : public cwSpatial
{
public:
	static cwNormalSorting* create();

	cwNormalSorting();
	virtual ~cwNormalSorting();

	virtual CWBOOL init();

	//virtual CWBOOL build(cwScene*) override;

	virtual CWVOID intersection(const cwFrustum&, cwVector<cwRenderNode*>&, eSceneObjectType, CWBOOL) override;
	virtual CWVOID intersection(const cwAABB&, cwVector<cwRenderNode*>&, eSceneObjectType, CWBOOL) override;
	virtual CWVOID intersection(const cwCircle&, cwVector<cwRenderNode*>&, eSceneObjectType, CWBOOL) override;

	virtual CWBOOL insert(cwRenderNode*) override;
	virtual CWBOOL remove(cwRenderNode*) override;

	virtual CWVOID update() override;
	virtual CWVOID refresh(cwRenderNode*) override;

	virtual CWVOID clear() override;
	virtual CWVOID renderPrimitiveFrame() override;

	virtual const cwAABB& getBoundingBox() override;

protected:
	CWBOOL insertNode(cwRenderNode*);
	CWBOOL insertNodes();

	CWBOOL removeNodes();

protected:
	std::list<cwRenderNode*> m_nListNodes;

	std::unordered_set<cwRenderNode*> m_nSetRefreshNode;

	cwVector<cwRenderNode*> m_nVecAppend;
	std::unordered_set<cwRenderNode*> m_nSetRemove;

	cwAABB m_nBoundingBox;

};

NS_MINIR_END

#endif