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
#include "Base/cwVector.h"
#include "Math/cwMath.h"
#include "SpatialSorting/cwSpatial.h"

#include <list>
#include <queue>
#include <unordered_set>
#include <unordered_map>

NS_MINIR_BEGIN

class CW_DLL cwOctree : public cwSpatial
{
public:
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

		sOctreeInit() {}
		sOctreeInit(CWUINT uDepth, const cwAABB& space) : m_uDepth(uDepth), m_nMaxSpace(space) {}
	};

public:
	static cwOctree* create();
	static cwOctree* create(const sOctreeInit&);

	cwOctree();
	virtual ~cwOctree();

	virtual CWBOOL init();
	virtual CWBOOL init(const sOctreeInit&);

	//virtual CWBOOL build(cwScene*) override;

	virtual CWVOID intersection(const cwFrustum&, cwVector<cwRenderNode*>&, CWBOOL) override;
	virtual CWVOID intersection(const cwAABB&, cwVector<cwRenderNode*>&, CWBOOL) override;
	virtual CWVOID intersection(const cwCircle&, cwVector<cwRenderNode*>&, CWBOOL) override;
	virtual CWVOID intersection(const cwRay&, cwVector<cwRenderNode*>&, CWBOOL) override;

	virtual CWBOOL insert(cwRenderNode*) override;
	virtual CWBOOL remove(cwRenderNode*) override;

	virtual CWVOID update() override;
	virtual CWVOID refresh(cwRenderNode*) override;

	virtual CWVOID clear() override;
	virtual CWVOID renderPrimitiveFrame() override;

	virtual const cwAABB& getBoundingBox() override;

	inline CWUINT getMaxDepth() const { return m_uMaxDepth; }

protected:
	CWBOOL insertNode(cwRenderNode*);
	CWBOOL insertNode(cwRenderNode*, sOctreeNode*, CWUINT uDepth);
	CWBOOL insertNodes();

	CWBOOL removeNode(cwRenderNode*);
	CWBOOL removeNodes();

	CWVOID clearOctreeNode(sOctreeNode*);
	
	virtual CWVOID getChildrenBoundingBox(sOctreeNode*, cwAABB*);
	sOctreeNode* getTreeNodeBelong(cwRenderNode*);
	sOctreeNode* getTreeNodeBelong(cwRenderNode*, sOctreeNode*);
	sOctreeNode* getTreeNodeBelongRude(cwRenderNode*, sOctreeNode*);

	CWVOID intersection(sOctreeNode*, const cwFrustum&, cwVector<cwRenderNode*>&, CWBOOL);
	CWVOID intersection(sOctreeNode*, const cwAABB&, cwVector<cwRenderNode*>&, CWBOOL);
	CWVOID intersection(sOctreeNode*, const cwCircle&, cwVector<cwRenderNode*>&, CWBOOL);
	CWVOID intersection(sOctreeNode*, const cwRay&, cwVector<cwRenderNode*>&, CWBOOL);

	CWVOID getRenderNodeChild(cwRenderNode*, std::unordered_map<cwRenderNode*, sOctreeNode*>&);
	CWVOID updateRenderNode(cwRenderNode*, sOctreeNode*);

	CWBOOL isLeafNode(sOctreeNode*);
	sOctreeNode* getUnuseOctreeNode();
	CWVOID saveUnuseOctreeNode(sOctreeNode*);
	CWVOID removeOctreeNode(sOctreeNode*);
	CWVOID checkOctreeNodeEmpty(sOctreeNode*);

	CWVOID renderOctreeNodePrimitiveFrame(sOctreeNode*);

protected:
	sOctreeNode* m_pRoot;
	CWUINT m_uMaxDepth;

	std::unordered_set<cwRenderNode*> m_nSetRefreshNode;
	std::queue<sOctreeNode*> m_nQueueUnuseOctreeNode;

	cwVector<cwRenderNode*> m_nVecAppend;
	std::unordered_set<cwRenderNode*> m_nSetRemove;

};

NS_MINIR_END

#endif
