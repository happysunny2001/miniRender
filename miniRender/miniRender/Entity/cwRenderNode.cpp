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

#include "cwRenderNode.h"
#include "Event/cwEventListener.h"
#include "Event/cwEventManager.h"
#include "Event/cwEventDefine.h"
#include "Effect/cwEffect.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Blend/cwBlend.h"
#include "Stencil/cwStencil.h"
#include "cwScene.h"

#include <stack>

NS_MINIR_BEGIN

cwRenderNode* cwRenderNode::create()
{
	cwRenderNode* pNode = new cwRenderNode();
	if (pNode && pNode->init()) {
		pNode->autorelease();
		return pNode;
	}
	CW_SAFE_DELETE(pNode);
	return nullptr;
}

cwRenderNode::cwRenderNode() :
m_pParent(nullptr),
m_bVisible(CWTRUE),
m_nPos(cwVector3D::ZERO),
m_nRot(cwVector3D::ZERO),
m_nScale(1.0f, 1.0f ,1.0f),
m_bTransDirty(CWFALSE),
m_pEffect(nullptr),
m_pBlend(nullptr),
m_pStencil(nullptr)
{
	m_eType = eSceneObjectNode;
}

cwRenderNode::~cwRenderNode()
{
	CW_SAFE_RELEASE_NULL(m_pParent);
	CW_SAFE_RELEASE_NULL(m_pEffect);
	CW_SAFE_RELEASE_NULL(m_pBlend);
	CW_SAFE_RELEASE_NULL(m_pStencil);
	clearChildren();
	clearEventListener();
}

CWBOOL cwRenderNode::init()
{
	return CWTRUE;
}

CWVOID cwRenderNode::clearChildren()
{
	for (auto it = m_nVecChildren.begin(); it != m_nVecChildren.end(); ++it) {
		(*it)->setParent(nullptr);
	}

	m_nVecChildren.clear();
}

CWVOID cwRenderNode::setParent(cwRenderNode* pNode)
{
	m_pParent = pNode;
}

CWBOOL cwRenderNode::addChild(cwRenderNode* pNode)
{
	if (pNode && !pNode->getParent()) {
		if (!m_nVecChildren.contains(pNode)) {
			m_nVecChildren.pushBack(pNode);
			pNode->setParent(this);

			//cwRepertory::getInstance().getEngine()->insertSpatialNode(pNode);
			//cwRepertory::getInstance().getEngine()->refreshSpatialNode(pNode);
			insertSpatialNode(pNode);
			return CWTRUE;
		}
	}

	return CWFALSE;
}

CWBOOL cwRenderNode::removeChild(cwRenderNode* pNode)
{
	if (pNode && pNode->getParent() == this) {
		pNode->setParent(nullptr);
		//cwRepertory::getInstance().getEngine()->removeSpatialNode(pNode);
		removeSpatialNode(pNode);
		m_nVecChildren.erase(pNode);
		return CWTRUE;
	}

	return CWFALSE;
}

CWBOOL cwRenderNode::removeFromParent()
{
	if (m_pParent) {
		return m_pParent->removeChild(this);
	}

	return CWFALSE;
}

CWVOID cwRenderNode::insertSpatialNode(cwRenderNode* pNode)
{
	cwRepertory::getInstance().getEngine()->insertSpatialNode(pNode);
	cwRepertory::getInstance().getEngine()->refreshSpatialNode(pNode);
}

CWVOID cwRenderNode::removeSpatialNode(cwRenderNode* pNode)
{
	cwRepertory::getInstance().getEngine()->removeSpatialNode(pNode);
}

CWVOID cwRenderNode::refreshSpatialNode()
{
	if (m_pParent)
		cwRepertory::getInstance().getEngine()->refreshSpatialNode(this);
}

//CWVOID cwRenderNode::removeChildren()
//{
//	for (auto it = m_nVecChildren.begin(); it != m_nVecChildren.end(); ++it) {
//		(*it)->setParent(nullptr);
//	}
//
//	m_nVecChildren.clear();
//}

CWVOID cwRenderNode::setPosition(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nPos.x = x;
	m_nPos.y = y;
	m_nPos.z = z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::setPosition(const cwVector3D& v)
{
	m_nPos.x = v.x;
	m_nPos.y = v.y;
	m_nPos.z = v.z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::move(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nPos.x += x;
	m_nPos.y += y;
	m_nPos.z += z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::move(const cwVector3D& v)
{
	m_nPos.x += v.x;
	m_nPos.y += v.y;
	m_nPos.z += v.z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::setRotation(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nRot.x = x;
	m_nRot.y = y;
	m_nRot.z = z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::setRotation(const cwVector3D& v)
{
	m_nRot.x = v.x;
	m_nRot.y = v.y;
	m_nRot.z = v.z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::rotate(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nRot.x += x;
	m_nRot.y += y;
	m_nRot.z += z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::rotate(const cwVector3D& v)
{
	m_nRot.x += v.x;
	m_nRot.y += v.y;
	m_nRot.z += v.z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::setScale(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nScale.x = x;
	m_nScale.y = y;
	m_nScale.z = z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::setScale(const cwVector3D& v)
{
	m_nScale.x = v.x;
	m_nScale.y = v.y;
	m_nScale.z = v.z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::scale(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nScale.x += x;
	m_nScale.y += y;
	m_nScale.z += z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::scale(const cwVector3D& v)
{
	m_nScale.x += v.x;
	m_nScale.y += v.y;
	m_nScale.z += v.z;

	if (!m_bTransDirty) {
		refreshSpatialNode();
		m_bTransDirty = CWTRUE;
	}
}

CWVOID cwRenderNode::setBlend(cwBlend* pBlend)
{
	if (m_pBlend == pBlend) return;

	CW_SAFE_RETAIN(pBlend);
	CW_SAFE_RELEASE_NULL(m_pBlend);
	m_pBlend = pBlend;
}

CWVOID cwRenderNode::setStencil(cwStencil* pStencil)
{
	if (m_pStencil == pStencil) return;

	CW_SAFE_RETAIN(pStencil);
	CW_SAFE_RELEASE_NULL(m_pStencil);
	m_pStencil = pStencil;
}

CWVOID cwRenderNode::setVisible(bool b)
{
	m_bVisible = b;
}

CWVOID cwRenderNode::addEventListener(cwEventListener* pListener)
{
	addEventListener(pListener, CW_EVENT_DEFAULT_PRIORITY, CWFALSE);
}

CWVOID cwRenderNode::addEventListener(cwEventListener* pListener, CWINT iPriority, CWBOOL swallow)
{
	if (cwRepertory::getInstance().getEventManager()->addListener(pListener, iPriority, swallow)) {
		m_nVecEventListener.pushBack(pListener);
	}
}

CWVOID cwRenderNode::removeEventListerner(cwEventListener* pListener, CWBOOL bClean)
{
	cwRepertory::getInstance().getEventManager()->removeListener(pListener);
	if (bClean)
		m_nVecEventListener.erase(pListener);
}

CWVOID cwRenderNode::transform()
{
	if (m_bTransDirty) {
		cwMatrix4X4 matTranslate, matScale, matRot;
		matTranslate.setTranslation(m_nPos);
		matScale.setScale(m_nScale);
		matRot.setRotation(m_nRot);
		m_nLocalTrans = matScale * matRot * matTranslate;

		m_bTransDirty = CWFALSE;
	}
}

CWVOID cwRenderNode::refreshTransform()
{
	m_nTrans = m_nLocalTrans;

	cwRenderNode* pParent = m_pParent;
	while (pParent) {
		m_nTrans *= pParent->getLocalTransMatrix();
		pParent = pParent->getParent();
	}
}

CWVOID cwRenderNode::refreshBoundingBox()
{
	m_nBoundingBox.m_nMin = m_nBoundingBox.m_nMax = m_nPos;
}

cwAABB cwRenderNode::getGroupBoundingBox()
{
	if (m_nVecChildren.empty()) {
		return m_nBoundingBox;
	}

	m_nGroupBoundingBox.zero();

	std::vector<cwRenderNode*> vecStack;
	vecStack.reserve(10);
	vecStack.push_back(this);

	while (!vecStack.empty()) {
		cwRenderNode* pLast = vecStack.back();
		m_nGroupBoundingBox.add(pLast->getBoundingBox());

		vecStack.pop_back();

		cwVector<cwRenderNode*>& nVecChildren = pLast->getChildren();
		if (!nVecChildren.empty()) {
			for (auto pNode : nVecChildren) {
				vecStack.push_back(pNode);
			}
		}
	}

	return m_nGroupBoundingBox;
}

CWVOID cwRenderNode::update(CWFLOAT dt)
{

}

CWVOID cwRenderNode::render()
{

}

CWVOID cwRenderNode::render(cwRenderBatch* pRenderBatch)
{

}

CWVOID cwRenderNode::clearEventListener()
{
	for (auto it = m_nVecEventListener.begin(); it != m_nVecEventListener.end(); ++it) {
		removeEventListerner(*it, CWFALSE);
	}

	m_nVecEventListener.clear();
}

CWVOID cwRenderNode::setEffect(cwEffect* pEffect)
{
	if (pEffect == m_pEffect) return;
	CW_SAFE_RETAIN(pEffect);
	CW_SAFE_RELEASE_NULL(m_pEffect);
	m_pEffect = pEffect;
}

NS_MINIR_END


