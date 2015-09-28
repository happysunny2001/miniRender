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
#include "Entity/cwScene.h"

#include <unordered_map>

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

	for (CWUINT i = 0; i < m_nQueueUnuseOctreeNode.size(); ++i) {
		sOctreeNode* pOctreeNode = m_nQueueUnuseOctreeNode.front();
		CW_SAFE_DELETE(pOctreeNode);
		m_nQueueUnuseOctreeNode.pop();
	}
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

	insert(pScene);

	return CWTRUE;
}

CWBOOL cwOctree::insert(cwRenderNode* pNode)
{
	if (!pNode) return CWFALSE;
	if (!m_pRoot) return CWFALSE;

	if (pNode->getType() == eSceneObjectEntity) {
		insert(pNode, m_pRoot, 0);
	}
	else {
		for (auto pChild : pNode->getChildren()) {
			insert(pChild);
		}
	}

	return CWTRUE;
}

CWBOOL cwOctree::insert(cwRenderNode* pNode, sOctreeNode* pOctreeNode, CWUINT uDepth)
{
	if (!pOctreeNode || !pNode) return CWFALSE;

	if (uDepth == m_uMaxDepth) {
		pOctreeNode->m_nListObjs.push_back(pNode);
	}

	if (pOctreeNode->m_nBox.contained(pNode->getBoundingBox())) {
		cwAABB childBox[8];
		getChildrenBoundingBox(pOctreeNode, childBox);
		CWBOOL bInner = CWFALSE;

		for (CWUINT i = 0; i < 8; ++i) {
			if (childBox[i].contained(pNode->getBoundingBox())) {
				bInner = CWTRUE;
				if (pOctreeNode->m_pChildren[i]) {
					insert(pNode, pOctreeNode->m_pChildren[i], uDepth++);
				}
				else {
					pOctreeNode->m_pChildren[i] = getUnuseOctreeNode();
					pOctreeNode->m_pChildren[i]->m_pParent = pOctreeNode;
					pOctreeNode->m_pChildren[i]->m_nBox = childBox[i];
					insert(pNode, pOctreeNode->m_pChildren[i], uDepth++);
				}

				if (bInner) {
					break;
				}
			}
		}

		if (!bInner) {
			pOctreeNode->m_nListObjs.push_back(pNode);
		}
	}
	else {
		pOctreeNode->m_nListObjs.push_back(pNode);
	}

	for (auto pChild : pNode->getChildren()) {
		insert(pChild);
	}

	return CWTRUE;
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

CWBOOL cwOctree::remove(cwRenderNode* pNode)
{
	if (!pNode) return CWFALSE;
	if (!m_pRoot) return CWFALSE;

	sOctreeNode* pInnerOctreeNode = getTreeNodeBelong(pNode);
	if (pInnerOctreeNode) {
		auto it = std::find(pInnerOctreeNode->m_nListObjs.begin(), pInnerOctreeNode->m_nListObjs.end(), pNode);
		if (it != pInnerOctreeNode->m_nListObjs.end()) {
			pInnerOctreeNode->m_nListObjs.erase(it);
		}
	}

	for (auto pChild : pNode->getChildren()) {
		remove(pChild);
	}

	return CWFALSE;
}

cwOctree::sOctreeNode* cwOctree::getTreeNodeBelong(cwRenderNode* pNode)
{
	if (!pNode) return nullptr;
	if (pNode->getType() != eSceneObjectEntity) return nullptr;
	if (!m_pRoot) return nullptr;

	sOctreeNode* pInnerOctreeNode = getTreeNodeBelong(pNode, m_pRoot);
	if (pInnerOctreeNode) return pInnerOctreeNode;

	//is bad
	return getTreeNodeBelongRude(pNode, m_pRoot);
}

cwOctree::sOctreeNode* cwOctree::getTreeNodeBelong(cwRenderNode* pNode, sOctreeNode* pOctreeNode)
{
	if (!pOctreeNode) return nullptr;
	if (pOctreeNode->m_nBox.contained(pNode->getBoundingBox())) {
		for (CWUINT i = 0; i < 8; ++i) {
			if (pOctreeNode->m_pChildren[i] && pOctreeNode->m_pChildren[i]->m_nBox.contained(pNode->getBoundingBox())) {
				sOctreeNode* pInnerOctreeNode = getTreeNodeBelong(pNode, pOctreeNode->m_pChildren[i]);
				if (pInnerOctreeNode) return pInnerOctreeNode;
			}
		}
	}

	for (auto pInnerNode : pOctreeNode->m_nListObjs) {
		if (pInnerNode == pNode) return pOctreeNode;
	}

	return nullptr;
}

cwOctree::sOctreeNode* cwOctree::getTreeNodeBelongRude(cwRenderNode* pNode, sOctreeNode* pOctreeNode)
{
	for (auto pInnerNode : pOctreeNode->m_nListObjs) {
		if (pInnerNode == pNode) return pOctreeNode;
	}

	for (CWUINT i = 0; i < 8; ++i) {
		if (pOctreeNode->m_pChildren[i]) {
			sOctreeNode* pInnerOctreeNode = getTreeNodeBelongRude(pNode, pOctreeNode->m_pChildren[i]);
			if (pInnerOctreeNode) return pInnerOctreeNode;
		}
	}

	return nullptr;
}

CWVOID cwOctree::intersection(const cwFrustum& frustum, std::vector<cwRenderNode*>& vecRet)
{
	intersection(m_pRoot, frustum, vecRet);
}

CWVOID cwOctree::intersection(sOctreeNode* pOctreeNode, const cwFrustum& frustum, std::vector<cwRenderNode*>& vecRet)
{
	if (!pOctreeNode) return;
	int iRet = frustum.intersection(pOctreeNode->m_nBox);
	if (!frustum.isCollide(iRet)) return;

	for (auto pNode : pOctreeNode->m_nListObjs) {
		int iRet = frustum.intersection(pNode->getBoundingBox());
		if (frustum.isCollide(iRet)) vecRet.push_back(pNode);
	}

	for (CWUINT i = 0; i < 8; ++i) {
		if (pOctreeNode->m_pChildren[i])
			intersection(pOctreeNode->m_pChildren[i], frustum, vecRet);
	}
}

CWVOID cwOctree::intersection(const cwAABB& aabb, std::vector<cwRenderNode*>& vecRet)
{
	intersection(m_pRoot, aabb, vecRet);
}

CWVOID cwOctree::intersection(sOctreeNode* pOctreeNode, const cwAABB& aabb, std::vector<cwRenderNode*>& vecRet)
{
	if (!pOctreeNode) return;
	if (!pOctreeNode->m_nBox.intersection(aabb)) return;

	for (auto pNode : pOctreeNode->m_nListObjs) {
		if (aabb.intersection(pNode->getBoundingBox()))
			vecRet.push_back(pNode);
	}

	for (CWUINT i = 0; i < 8; ++i) {
		if (pOctreeNode->m_pChildren[i])
			intersection(pOctreeNode->m_pChildren[i], aabb, vecRet);
	}
}

CWVOID cwOctree::intersection(const cwCircle& circle, std::vector<cwRenderNode*>& vecRet)
{
	intersection(m_pRoot, circle, vecRet);
}

CWVOID cwOctree::intersection(sOctreeNode* pOctreeNode, const cwCircle& circle, std::vector<cwRenderNode*>& vecRet)
{
	if (!pOctreeNode) return;
	if (!pOctreeNode->m_nBox.intersection(circle)) return;

	for (auto pNode : pOctreeNode->m_nListObjs) {
		if (circle.intersection(pNode->getBoundingBox()))
			vecRet.push_back(pNode);
	}

	for (CWUINT i = 0; i < 8; ++i) {
		if (pOctreeNode->m_pChildren[i])
			intersection(pOctreeNode->m_pChildren[i], circle, vecRet);
	}
}

CWVOID cwOctree::refresh(cwRenderNode* pNode)
{
	if (pNode) {
		m_nSetRefreshNode.insert(pNode);
	}
}

CWVOID cwOctree::update()
{
	if (m_nSetRefreshNode.empty()) return;
	
	std::unordered_map<cwRenderNode*, sOctreeNode*> m_nMapNodeTree;
	for (auto pNode : m_nSetRefreshNode) {
		if (pNode && pNode->getType() == eSceneObjectEntity) {
			if (m_nMapNodeTree.find(pNode) == m_nMapNodeTree.end()) {
				sOctreeNode* pOctreeNode = getTreeNodeBelong(pNode);
				if (pOctreeNode) {
					m_nMapNodeTree[pNode] = pOctreeNode;
				}
			}
		}

		getRenderNodeChild(pNode, m_nMapNodeTree);
	}

	for (auto nodeData : m_nMapNodeTree) {
		nodeData.first->transform();
	}

	for (auto nodeData : m_nMapNodeTree) {
		nodeData.first->refreshTransform();
		nodeData.first->refreshBoundingBox();
	}

	for (auto nodeData : m_nMapNodeTree) {
		nodeData.first->refreshGroupBoundingBox();
		if (nodeData.second->m_nBox.contained(nodeData.first->getBoundingBox())) continue;

		auto it = std::find(nodeData.second->m_nListObjs.begin(), nodeData.second->m_nListObjs.end(), nodeData.first);
		if (it != nodeData.second->m_nListObjs.end()) {
			nodeData.second->m_nListObjs.erase(it);
		}

		insert(nodeData.first, m_pRoot, 0);
	}

	for (auto nodeData : m_nMapNodeTree) {
		if (nodeData.second->m_nListObjs.empty()) {
			removeOctreeNode(nodeData.second);
		}
	}

	m_nSetRefreshNode.clear();
}

CWVOID cwOctree::getRenderNodeChild(cwRenderNode* pNode, std::unordered_map<cwRenderNode*, sOctreeNode*>& mapNode)
{
	if (!pNode) return;

	for (auto pChildNode : pNode->getChildren()) {
		if (pChildNode && pChildNode->getType() == eSceneObjectEntity) {
			if (mapNode.find(pChildNode) == mapNode.end()) {
				sOctreeNode* pOctreeNode = getTreeNodeBelong(pChildNode);
				if (pOctreeNode) {
					mapNode[pChildNode] = pOctreeNode;
				}
			}
		}

		getRenderNodeChild(pChildNode, mapNode);
	}
}

cwOctree::sOctreeNode* cwOctree::getUnuseOctreeNode()
{
	if (m_nQueueUnuseOctreeNode.empty()) return new sOctreeNode();

	sOctreeNode* pOctreeNode = m_nQueueUnuseOctreeNode.front();
	m_nQueueUnuseOctreeNode.pop();
	return pOctreeNode;
}

CWVOID cwOctree::saveUnuseOctreeNode(sOctreeNode* pOctreeNode)
{
	if (pOctreeNode) {
		m_nQueueUnuseOctreeNode.push(pOctreeNode);
	}
}

CWVOID cwOctree::removeOctreeNode(sOctreeNode* pOctreeNode)
{
	if (pOctreeNode) {
		//root node? 
		if (!pOctreeNode->m_pParent) return;
		for (CWUINT i = 0; i < 8; ++i) {
			if (pOctreeNode->m_pChildren[i] == pOctreeNode) {
				pOctreeNode->m_pChildren[i] = nullptr;
				pOctreeNode->m_pParent = nullptr;
				saveUnuseOctreeNode(pOctreeNode);
			}
		}
	}
}

NS_MINIR_END
