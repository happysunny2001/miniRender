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

#include "cwOctree.h"
#include "Entity/cwRenderNode.h"

NS_MINIR_BEGIN

const CWUINT cwOctree::MAX_DEPTH = 8;
const CWUINT cwOctree::m_uDefaultDepth = 6;
const cwAABB cwOctree::m_nDefaultSize(cwPoint3D(-1000.0f, -1000.0f, -1000.0f), cwPoint3D(1000.0f, 1000.0f, 1000.0f));

cwOctree::sOctreeNode::sOctreeNode()
{
	m_pParent = nullptr;
	memset(m_pChildren, 0, sizeof(sOctreeNode*)* 8);
	m_nBox.zero();
}

cwOctree::sOctreeNode::~sOctreeNode()
{
	m_pParent = nullptr;
	for (CWUINT i = 0; i < 8; ++i) {
		if (m_pChildren[i]) {
			CW_SAFE_DELETE(m_pChildren[i]);
		}
	}

	m_nListObjs.clear();
}

cwOctree* cwOctree::create()
{
	cwOctree* pOctree = new cwOctree();
	if (pOctree && pOctree->init()) {
		pOctree->autorelease();
		return pOctree;
	}

	CW_SAFE_DELETE(pOctree);
	return nullptr;
}

cwOctree* cwOctree::create(const sOctreeInit& octreeInit)
{
	cwOctree* pOctree = new cwOctree();
	if (pOctree && pOctree->init(octreeInit)) {
		pOctree->autorelease();
		return pOctree;
	}

	CW_SAFE_DELETE(pOctree);
	return nullptr;
}

cwOctree::cwOctree():
m_pRoot(nullptr)
{

}

cwOctree::~cwOctree()
{
	CW_SAFE_DELETE(m_pRoot);
}

CWBOOL cwOctree::init()
{
	sOctreeInit initData;
	initData.m_uDepth = MAX_DEPTH;
	initData.m_nMaxSpace = m_nDefaultSize;

	return this->init(initData);
}

CWBOOL cwOctree::init(const cwOctree::sOctreeInit& initData)
{
	m_pRoot = new cwOctree::sOctreeNode();
	if (!m_pRoot) return CWFALSE;

	m_pRoot->m_nBox = initData.m_nMaxSpace;
	m_uMaxDepth = initData.m_uDepth;

	return CWTRUE;
}

CWBOOL cwOctree::build(cwScene* pScene)
{
	if (!pScene) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwOctree::insert(cwRenderNode* pNode)
{
	if (!pNode) return;
	if (!m_pRoot) return;

	if (pNode->getType() == eSceneObjectEntity) {
		if (m_pRoot)
	}
}

CWBOOL cwOctree::insert(cwRenderNode* pNode, sOctreeNode* pOctreeNode)
{
	if (!pOctreeNode || !pNode) return CWFALSE;

	if (pOctreeNode->m_nBox.contained(pNode->getBoundingBox())) {

	}
}

CWVOID cwOctree::getChildrenBoundingBox(sOctreeNode* pOctreeNode, cwAABB* pAabb)
{
	if (!pOctreeNode || !pAabb) return;

	const cwVector3D& minPos = pOctreeNode->m_nBox.m_nMin;
	const cwVector3D& maxPos = pOctreeNode->m_nBox.m_nMax;
	cwVector3D center = pOctreeNode->m_nBox.center();

	pAabb[eOctreeTopFarLeft].m_nMin.set(minPos.x, center.y, center.z);
	pAabb[eOctreeTopFarLeft].m_nMax.set(center.x, maxPos.y, maxPos.z);

	pAabb[eOctreeTopFarRight].m_nMin = center;
	pAabb[eOctreeTopFarRight].m_nMax = maxPos;

	pAabb[eOctreeTopNearLeft].m_nMin.set(minPos.x, center.y, minPos.z);
	pAabb[eOctreeTopNearLeft].m_nMax.set(center.x, maxPos.y, center.z);

	pAabb[eOctreeTopNearRight].m_nMin.set(center.x, center.y, minPos.z);
	pAabb[eOctreeTopNearRight].m_nMax.set(maxPos.x, maxPos.y, center.z);

	pAabb[eOctreeBottomFarLeft].m_nMin.set(minPos.x, minPos.y, center.z);
	pAabb[eOctreeBottomFarLeft].m_nMax.set(center.x, center.y, maxPos.z);

	pAabb[eOctreeBottomFarRight].m_nMin.set(center.x, minPos.y, center.z);
	pAabb[eOctreeBottomFarRight].m_nMax.set(maxPos.x, center.y, maxPos.z);

	pAabb[eOctreeBottomNearLeft].m_nMin = minPos;
	pAabb[eOctreeBottomNearLeft].m_nMax = center;

	pAabb[eOctreeBottomNearRight].m_nMin.set(center.x, minPos.y, minPos.z);
	pAabb[eOctreeBottomNearRight].m_nMax.set(maxPos.x, center.y, center.z);
}

NS_MINIR_END
