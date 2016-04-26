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

#include "cwNormalSorting.h"
#include "Entity/cwRenderNode.h"
#include "Math/cwMath.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"

NS_MINIR_BEGIN

cwNormalSorting* cwNormalSorting::create()
{
	cwNormalSorting* pSpatial = new cwNormalSorting();
	if (pSpatial && pSpatial->init()) {
		pSpatial->autorelease();
		return pSpatial;
	}

	CW_SAFE_DELETE(pSpatial);
	return nullptr;
}

cwNormalSorting::cwNormalSorting()
{

}

cwNormalSorting::~cwNormalSorting()
{

}

CWBOOL cwNormalSorting::init()
{
	return CWTRUE;
}

CWBOOL cwNormalSorting::insert(cwRenderNode* pNode)
{
	if (pNode) {
		if (!m_nVecAppend.contains(pNode)) {
			m_nVecAppend.pushBack(pNode);
			return CWTRUE;
		}
	}

	return CWFALSE;
}

CWBOOL cwNormalSorting::insertNode(cwRenderNode* pNode)
{
	if (!pNode) {
		pNode->transform();
		pNode->refreshTransform();
		pNode->refreshBoundingBox();

		m_nListNodes.push_back(pNode);

		return CWTRUE;
	}

	return CWFALSE;
}

CWBOOL cwNormalSorting::insertNodes()
{
	for (auto pNode : m_nVecAppend) {
		insertNode(pNode);
	}

	m_nVecAppend.clear();

	return CWTRUE;
}

CWBOOL cwNormalSorting::remove(cwRenderNode* pNode)
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

CWBOOL cwNormalSorting::removeNodes()
{
	for (auto pNode : m_nSetRemove) {
		auto it = std::find(m_nListNodes.begin(), m_nListNodes.end(), pNode);
		if (it != m_nListNodes.end()) {
			m_nListNodes.erase(it);
		}
		CW_SAFE_RELEASE(pNode);
	}

	m_nSetRemove.clear();
	return CWTRUE;
}

CWVOID cwNormalSorting::refresh(cwRenderNode* pNode)
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

CWVOID cwNormalSorting::update()
{
	insertNodes();

	if (!m_nSetRefreshNode.empty()) {
		for (auto pNode : m_nSetRefreshNode) {
			pNode->transform();
		}

		for (auto pNode : m_nSetRefreshNode) {
			pNode->refreshTransform();
			pNode->refreshBoundingBox();

			CW_SAFE_RELEASE(pNode);
		}
		m_nSetRefreshNode.clear();
	}

	removeNodes();
}

CWVOID cwNormalSorting::clear()
{
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

const cwAABB& cwNormalSorting::getBoundingBox()
{
	m_nBoundingBox.zero();
	for (auto it = m_nListNodes.begin(); it != m_nListNodes.end(); ++it) {
		m_nBoundingBox.add((*it)->getBoundingBox());
	}

	return m_nBoundingBox;
}

CWVOID cwNormalSorting::renderPrimitiveFrame()
{
	cwRepertory::getInstance().getEngine()->getRenderer()->renderPrimitive(getBoundingBox());
}

CWVOID cwNormalSorting::intersection(const cwFrustum& frustum, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	for (auto pNode : m_nListNodes) {
		if (pNode->getVisible() == bVisible) {
			int iRet = frustum.intersection(pNode->getBoundingBox());
			if (frustum.isCollide(iRet)) vecRet.pushBack(pNode);
		}
	}
}

CWVOID cwNormalSorting::intersection(const cwAABB& aabb, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	for (auto pNode : m_nListNodes) {
		if (pNode->getVisible() == bVisible) {
			if(aabb.intersection(pNode->getBoundingBox()))
				vecRet.pushBack(pNode);
		}
	}
}

CWVOID cwNormalSorting::intersection(const cwCircle& circle, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	for (auto pNode : m_nListNodes) {
		if (pNode->getVisible() == bVisible) {
			if (circle.intersection(pNode->getBoundingBox()))
				vecRet.pushBack(pNode);
		}
	}
}

CWVOID cwNormalSorting::intersection(const cwRay& ray, cwVector<cwRenderNode*>& vecRet, CWBOOL bVisible)
{
	for (auto pNode : m_nListNodes) {
		if (pNode->getVisible() == bVisible) {
			if (pNode->getBoundingBox().intersection(ray)) {
				vecRet.pushBack(pNode);
			}
		}
	}
}

CWUINT cwNormalSorting::getObjCnt()
{
	return m_nListNodes.size();
}

NS_MINIR_END
