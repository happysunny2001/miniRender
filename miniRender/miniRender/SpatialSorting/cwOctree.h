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

#ifndef __CW_OCTREE_H__
#define __CW_OCTREE_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "cwSpatial.h"

#include <list>

NS_MINIR_BEGIN

class cwOctree : public cwSpatial
{
public:
	static const CWUINT MAX_DEPTH;
	static const CWUINT m_uDefaultDepth;
	static const cwAABB m_nDefaultSize;

	enum eOctreeNode
	{
		eOctreeTopFarLeft = 0,
		eOctreeTopFarRight,
		eOctreeTopNearLeft,
		eOctreeTopNearRight,
		eOctreeBottomFarLeft,
		eOctreeBottomFarRight,
		eOctreeBottomNearLeft,
		eOctreeBottomNearRight,
	};

	struct sOctreeNode
	{
		sOctreeNode* m_pParent;
		sOctreeNode* m_pChildren[8];
		cwAABB m_nBox;
		std::list<cwRenderNode*> m_nListObjs;

		sOctreeNode();
		~sOctreeNode();
	};

	struct sOctreeInit
	{
		CWUINT m_uDepth;
		cwAABB m_nMaxSpace;
	};

public:
	static cwOctree* create();
	static cwOctree* create(const sOctreeInit&);

	cwOctree();
	virtual ~cwOctree();

	virtual CWBOOL init();
	virtual CWBOOL init(const sOctreeInit&);

	virtual CWBOOL build(cwScene*) override;

	virtual CWVOID intersection(const cwFrustum&, std::vector<cwRenderNode*>&) override;
	virtual CWVOID intersection(const cwAABB&, std::vector<cwRenderNode*>&) override;
	virtual CWVOID intersection(const cwCircle&, std::vector<cwRenderNode*>&) override;

	virtual CWBOOL insert(cwRenderNode*) override;
	virtual CWBOOL remove(cwRenderNode*) override;

	virtual CWVOID update() override;

	inline CWUINT getMaxDepth() const { return m_uMaxDepth; }

protected:
	CWBOOL insert(cwRenderNode*, sOctreeNode*);
	CWVOID getChildrenBoundingBox(sOctreeNode*, cwAABB*);

protected:
	sOctreeNode* m_pRoot;
	CWUINT m_uMaxDepth;

};

NS_MINIR_END

#endif
