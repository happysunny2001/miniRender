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
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"
#include "Base/cwColor.h"
#include "Base/cwLog.h"
#include "Repertory/cwGlobalParameter.h"

NS_MINIR_BEGIN

cwOctree::sOctreeNode::sOctreeNode()
{
	m_pParent = nullptr;
	memset(m_pChildren, 0, sizeof(sOctreeNode*)* 8);
	m_nBox.zero();
}

cwOctree::sOctreeNode::~sOctreeNode()
{
	m_pParent = nullptr;
	m_nListObjs.clear();

	for (CWUINT i = 0; i < 8; ++i) {
		if (m_pChildren[i]) {
			CW_SAFE_DELETE(m_pChildren[i]);
		}
	}
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
	initData.m_uDepth = cwGlobalParameter::OctreeDefaultDepth;
	initData.m_nMaxSpace = cwGlobalParameter::OctreeDefaultSize;

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

//CWBOOL cwOctree::build(cwScene* pScene)
//{
//	if (!pScene) return CWFALSE;
//
//	insert(pScene);
//
//	return CWTRUE;
//}

CWBOOL cwOctree::insertNode(cwRenderNode* pNode)
{
	if (pNode && m_pRoot) {
		pNode->transform();
		pNode->refreshTransform();
		pNode->refreshBoundingBox();

		insertNode(pNode, m_pRoot, 0);
		return CWTRUE;
	}

	return CWFALSE;
}

CWBOOL cwOctree::insertNode(cwRenderNode* pNode, sOctreeNode* pOctreeNode, CWUINT uDepth)
{
	if (uDepth == m_uMaxDepth) {
		pOctreeNode->m_nListObjs.push_back(pNode);
		return CWTRUE;
	}

	if (pOctreeNode->m_nBox.contained(pNode->getBoundingBox())) {
		cwAABB childBox[8];
		getChildrenBoundingBox(pOctreeNode, childBox);
		CWBOOL bInner = CWFALSE;

		const cwAABB& nodeAabb = pNode->getBoundingBox();

		for (CWUINT i = 0; i < 8; ++i) {
			if (childBox[i].contained(nodeAabb)) {
				bInner = CWTRUE;
				if (pOctreeNode->m_pChildren[i]) {
					insertNode(pNode, pOctreeNode->m_pChildren[i], uDepth + 1);
				}
				else {
					pOctreeNode->m_pChildren[i] = getUnuseOctreeNode();
					if (pOctreeNode->m_pChildren[i]) {
						pOctreeNode->m_pChildren[i]->m_pParent = pOctreeNode;
						pOctreeNode->m_pChildren[i]->m_nBox = childBox[i];
						insertNode(pNode, pOctreeNode->m_pChildren[i], uDepth + 1);
					}
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

	return CWTRUE;
}

CWBOOL cwOctree::insertNodes()
{
	for (auto pNode : m_nVecAppend) {
		insertNode(pNode);
	}

	m_nVecAppend.clear();

	return CWTRUE;
}

CWBOOL cwOctree::insert(cwRenderNode* pNode)
{
	if (pNode) {
		if (!m_nVecAppend.contains(pNode)) {
			m_nVecAppend.pushBack(pNode);
			return CWTRUE;
		}
	}

	return CWFALSE;
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

	std::vector<cwRenderNode*> vecStack;
	vecStack.reserve(10);
	vecStack.push_back(pNode);

	while (!vecStack.empty()) {
		cwRenderNode* pLast = vecStack.back();
		vecStack.pop_back();

		auto it = m_nSetRemove.find(pLast);
		if (it == m_nSetRemove.end()) {
			auto pairRet = m_nSetRemove.insert(pLast);
			if (pairRet.second)
				CW_SAFE_RETAIN(pLast);
		}

		cwVector<cwRenderNode*>& nVecChildren = pLast->getChildren();
		if (!nVecChildren.empty()) {
			for (auto pNode : nVecChildren) {
				if (pNode)
					vecStack.push_back(pNode);
			}
		}
	}

	return CWTRUE;
}

CWBOOL cwOctree::removeNode(cwRenderNode* pNode)
{
	sOctreeNode* pInnerOctreeNode = getTreeNodeBelong(pNode);
	if (pInnerOctreeNode) {
		auto it = std::find(pInnerOctreeNode->m_nListObjs.begin(), pInnerOctreeNode->m_nListObjs.end(), pNode);
		if (it != pInnerOctreeNode->m_nListObjs.end()) {
			pInnerOctreeNode->m_nListObjs.erase(it);

			checkOctreeNodeEmpty(pInnerOctreeNode);
		}
	}

	return CWTRUE;
}

CWBOOL cwOctree::removeNodes()
{
	for (auto pNode : m_nSetRemove) {
		removeNode(pNode);
		CW_SAFE_RELEASE(pNode);
	}

	m_nSetRemove.clear();
	return CWTRUE;
}

cwOctree::sOctreeNode* cwOctree::getTreeNodeBelong(cwRenderNode* pNode)
{
	if (!pNode) return nullptr;
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
	cwLog::print("cwOctree::getTreeNodeBelongRude, bad!\n");
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

CWVOID cwOctree::intersection(const cwFrustum& frustum, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	intersection(m_pRoot, frustum, vecRet, bVisible);
}

CWVOID cwOctree::intersection(sOctreeNode* pOctreeNode, const cwFrustum& frustum, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	if (!pOctreeNode) return;
	int iRet = frustum.intersection(pOctreeNode->m_nBox);
	if (!frustum.isCollide(iRet)) return;

	for (auto pNode : pOctreeNode->m_nListObjs) {
		if (pNode->getVisible() == bVisible) {
			int iRet = frustum.intersection(pNode->getBoundingBox());
			if (frustum.isCollide(iRet)) vecRet.pushBack(pNode);
		}
	}

	for (CWUINT i = 0; i < 8; ++i) {
		if (pOctreeNode->m_pChildren[i])
			intersection(pOctreeNode->m_pChildren[i], frustum, vecRet, bVisible);
	}
}

CWVOID cwOctree::intersection(const cwAABB& aabb, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	intersection(m_pRoot, aabb, vecRet, bVisible);
}

CWVOID cwOctree::intersection(sOctreeNode* pOctreeNode, const cwAABB& aabb, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	if (!pOctreeNode) return;
	if (!pOctreeNode->m_nBox.intersection(aabb)) return;

	for (auto pNode : pOctreeNode->m_nListObjs) {
		if (pNode->getVisible() == bVisible) {
			if (aabb.intersection(pNode->getBoundingBox()))
				vecRet.pushBack(pNode);
		}
	}

	for (CWUINT i = 0; i < 8; ++i) {
		if (pOctreeNode->m_pChildren[i])
			intersection(pOctreeNode->m_pChildren[i], aabb, vecRet, bVisible);
	}
}

CWVOID cwOctree::intersection(const cwCircle& circle, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	intersection(m_pRoot, circle, vecRet, bVisible);
}

CWVOID cwOctree::intersection(sOctreeNode* pOctreeNode, const cwCircle& circle, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	if (!pOctreeNode) return;
	if (!pOctreeNode->m_nBox.intersection(circle)) return;

	for (auto pNode : pOctreeNode->m_nListObjs) {
		if (pNode->getVisible() == bVisible) {
			if (circle.intersection(pNode->getBoundingBox()))
				vecRet.pushBack(pNode);
		}
	}

	for (CWUINT i = 0; i < 8; ++i) {
		if (pOctreeNode->m_pChildren[i])
			intersection(pOctreeNode->m_pChildren[i], circle, vecRet, bVisible);
	}
}

CWVOID cwOctree::intersection(const cwRay& ray, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	intersection(m_pRoot, ray, vecRet, bVisible);
}

CWVOID cwOctree::intersection(sOctreeNode* pOctreeNode, const cwRay& ray, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	if (!pOctreeNode) return;
	if (!pOctreeNode->m_nBox.intersection(ray)) return;

	for (auto pNode : pOctreeNode->m_nListObjs) {
		if (pNode->getVisible() == bVisible) {
			if (pNode->getBoundingBox().intersection(ray))
				vecRet.pushBack(pNode);
		}
	}

	for (CWUINT i = 0; i < 8; ++i) {
		if (pOctreeNode->m_pChildren[i])
			intersection(pOctreeNode->m_pChildren[i], ray, vecRet, bVisible);
	}
}

CWVOID cwOctree::refresh(cwRenderNode* pNode)
{
	if (pNode) {
		std::vector<cwRenderNode*> vecStack;
		vecStack.reserve(10);
		vecStack.push_back(pNode);

		while (!vecStack.empty()) {
			cwRenderNode* pLast = vecStack.back();
			vecStack.pop_back();

			auto it = m_nSetRefreshNode.find(pLast);
			if (it == m_nSetRefreshNode.end()) {
				auto pairRet = m_nSetRefreshNode.insert(pLast);
				if (pairRet.second)
					CW_SAFE_RETAIN(pLast);
			}

			cwVector<cwRenderNode*>& nVecChildren = pLast->getChildren();
			if (!nVecChildren.empty()) {
				for (auto pChildNode : nVecChildren) {
					if (pChildNode)
						vecStack.push_back(pChildNode);
				}
			}
		}
	}
}

CWVOID cwOctree::update()
{
	if (!m_pRoot) return;
	insertNodes();

	if (!m_nSetRefreshNode.empty()) {
		std::unordered_map<cwRenderNode*, sOctreeNode*> mapNodeTree;
		std::unordered_set<sOctreeNode*> setOctreeNode;

		for (auto pNode : m_nSetRefreshNode) {
			if (pNode) {
				if (mapNodeTree.find(pNode) == mapNodeTree.end()) {
					sOctreeNode* pOctreeNode = getTreeNodeBelong(pNode);
					if (pOctreeNode) {
						mapNodeTree[pNode] = pOctreeNode;
						setOctreeNode.insert(pOctreeNode);
					}
				}
			}
		}

		for (auto nodeData : mapNodeTree) {
			nodeData.first->transform();
		}

		for (auto nodeData : mapNodeTree) {
			nodeData.first->refreshTransform();
			nodeData.first->refreshBoundingBox();

			auto it = std::find(nodeData.second->m_nListObjs.begin(), nodeData.second->m_nListObjs.end(), nodeData.first);
			if (it != nodeData.second->m_nListObjs.end()) {
				nodeData.second->m_nListObjs.erase(it);
			}

			//sOctreeNode* pOctreeNode = nodeData.second;
			//if (nodeData.second->m_nBox.contained(nodeData.first->getBoundingBox())) continue;

			insertNode(nodeData.first, m_pRoot, 0);
		}

		for (auto pOctreeNode : setOctreeNode) {
			checkOctreeNodeEmpty(pOctreeNode);
		}

		for (auto pNode : m_nSetRefreshNode) {
			CW_SAFE_RELEASE(pNode);
		}
		m_nSetRefreshNode.clear();
	}
	
	removeNodes();
}

CWVOID cwOctree::getRenderNodeChild(cwRenderNode* pNode, std::unordered_map<cwRenderNode*, sOctreeNode*>& mapNode)
{
	if (!pNode) return;

	for (auto pChildNode : pNode->getChildren()) {
		if (pChildNode) {
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

CWBOOL cwOctree::isLeafNode(sOctreeNode* pOctreeNode)
{
	for (CWUINT i = 0; i < 8; ++i)
	if (pOctreeNode->m_pChildren[i]) return CWFALSE;

	return CWTRUE;
}

CWVOID cwOctree::saveUnuseOctreeNode(sOctreeNode* pOctreeNode)
{
	if (pOctreeNode) {
		pOctreeNode->m_pParent = nullptr;
		memset(pOctreeNode->m_pChildren, 0, sizeof(sOctreeNode*)* 8);
		pOctreeNode->m_nBox.zero();
		m_nQueueUnuseOctreeNode.push(pOctreeNode);
	}
}

CWVOID cwOctree::removeOctreeNode(sOctreeNode* pOctreeNode)
{
	if (pOctreeNode) {
		//root node? 
		if (!pOctreeNode->m_pParent) return;
		for (CWUINT i = 0; i < 8; ++i) {
			if (pOctreeNode->m_pParent->m_pChildren[i] == pOctreeNode) {
				pOctreeNode->m_pParent->m_pChildren[i] = nullptr;
				saveUnuseOctreeNode(pOctreeNode);
				break;
			}
		}
	}
}

CWVOID cwOctree::checkOctreeNodeEmpty(sOctreeNode* pOctreeNode)
{
	if (pOctreeNode) {
		if (pOctreeNode->m_nListObjs.empty() && isLeafNode(pOctreeNode)) {
			sOctreeNode* pOctreeParent = pOctreeNode->m_pParent;
			removeOctreeNode(pOctreeNode);
			checkOctreeNodeEmpty(pOctreeParent);
		}
	}
}

CWVOID cwOctree::clear()
{
	clearOctreeNode(m_pRoot);
	m_nVecAppend.clear();

	for (auto pNode : m_nSetRefreshNode) {
		CW_SAFE_RELEASE(pNode);
	}
	m_nSetRefreshNode.clear();
	
	for (auto pNode : m_nSetRemove) {
		CW_SAFE_RELEASE(pNode);
	}
	m_nSetRemove.clear();
}

CWVOID cwOctree::clearOctreeNode(sOctreeNode* pOctreeNode)
{
	if (pOctreeNode) {
		pOctreeNode->m_nListObjs.clear();
		for (auto pChildNode : pOctreeNode->m_pChildren) {
			clearOctreeNode(pChildNode);
		}
	}
}

CWVOID cwOctree::renderPrimitiveFrame()
{
	if (m_pRoot)
		renderOctreeNodePrimitiveFrame(m_pRoot);
}

CWVOID cwOctree::renderOctreeNodePrimitiveFrame(sOctreeNode* pOctreeNode)
{
	
	for (CWUINT i = 0; i < 8; ++i)  {
		if (pOctreeNode->m_pChildren[i])
			renderOctreeNodePrimitiveFrame(pOctreeNode->m_pChildren[i]);
	}

	if (pOctreeNode->m_nListObjs.empty())
		cwRepertory::getInstance().getEngine()->getRenderer()->renderPrimitive(pOctreeNode->m_nBox);
	else
		cwRepertory::getInstance().getEngine()->getRenderer()->renderPrimitive(pOctreeNode->m_nBox, cwColor::blue);
}

const cwAABB& cwOctree::getBoundingBox()
{
	if (m_pRoot)
		return m_pRoot->m_nBox;
	return cwGlobalParameter::OctreeDefaultSize;
}

CWUINT cwOctree::getObjCnt()
{
	return getObjCnt(m_pRoot);
}

CWUINT cwOctree::getObjCnt(sOctreeNode* pNode)
{
	if (!pNode) return 0;
	if (isLeafNode(pNode)) return pNode->m_nListObjs.size();

	CWUINT c = pNode->m_nListObjs.size();

	for (CWUINT i = 0; i < 8; ++i)  {
		if (pNode->m_pChildren[i])
			c += getObjCnt(pNode->m_pChildren[i]);
	}

	return c;
}

NS_MINIR_END
