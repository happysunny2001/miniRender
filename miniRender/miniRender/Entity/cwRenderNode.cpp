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

NS_MINI_BEGIN

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
m_bVisible(true)
{

}

cwRenderNode::~cwRenderNode()
{
	CW_SAFE_RELEASE_NULL(m_pParent);
	clearChildren();
}

bool cwRenderNode::init()
{
	return true;
}

void cwRenderNode::clearChildren()
{
	for (auto it = m_nVecChildren.begin(); it != m_nVecChildren.end(); ++it) {
		(*it)->setParent(nullptr);
	}

	m_nVecChildren.clear();
}

void cwRenderNode::setParent(cwRenderNode* pNode)
{
	CW_SAFE_RELEASE_NULL(m_pParent);
	m_pParent = pNode;
	CW_SAFE_RETAIN(m_pParent);
}

void cwRenderNode::addChild(cwRenderNode* pNode)
{
	if (!pNode) return;
	if (m_nVecChildren.contains(pNode)) return;
	m_nVecChildren.pushBack(pNode);
}

void cwRenderNode::removeChild(cwRenderNode* pNode)
{
	if (!pNode) return;
	pNode->setParent(nullptr);
	m_nVecChildren.erase(pNode);
}

void cwRenderNode::removeChildren()
{
	clearChildren();
}

void cwRenderNode::setPosition(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nPos.x = x;
	m_nPos.y = y;
	m_nPos.z = z;
}

void cwRenderNode::setPosition(const cwVector3D& v)
{
	m_nPos.x = v.x;
	m_nPos.y = v.y;
	m_nPos.z = v.z;
}

void cwRenderNode::move(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nPos.x += x;
	m_nPos.y += y;
	m_nPos.z += z;
}

void cwRenderNode::move(const cwVector3D& v)
{
	m_nPos.x += v.x;
	m_nPos.y += v.y;
	m_nPos.z += v.z;
}

void cwRenderNode::setRotation(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nRot.x = x;
	m_nRot.y = y;
	m_nRot.z = z;
}

void cwRenderNode::setRotation(const cwVector3D& v)
{
	m_nRot.x = v.x;
	m_nRot.y = v.y;
	m_nRot.z = v.z;
}

void cwRenderNode::rotate(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nRot.x += x;
	m_nRot.y += y;
	m_nRot.z += z;
}

void cwRenderNode::rotate(const cwVector3D& v)
{
	m_nRot.x += v.x;
	m_nRot.y += v.y;
	m_nRot.z += v.z;
}

void cwRenderNode::setScale(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nScale.x = x;
	m_nScale.y = y;
	m_nScale.z = z;
}

void cwRenderNode::setScale(const cwVector3D& v)
{
	m_nScale.x = v.x;
	m_nScale.y = v.y;
	m_nScale.z = v.z;
}

void cwRenderNode::scale(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nScale.x += x;
	m_nScale.y += y;
	m_nScale.z += z;
}

void cwRenderNode::scale(const cwVector3D& v)
{
	m_nScale.x += v.x;
	m_nScale.y += v.y;
	m_nScale.z += v.z;
}

void cwRenderNode::setVisible(bool b)
{
	m_bVisible = b;
}

void cwRenderNode::transform()
{
	cwMatrix4X4 matTranslate, matScale, matRot;
	matTranslate.setTranslation(m_nPos);
	matScale.setScale(m_nScale);
	matRot.setRotation(m_nRot);
	m_nTrans = matScale * matRot * matTranslate;

	if (m_pParent) {
		m_nTrans = m_nTrans * m_pParent->getTransformMatrix();
	}
}

void cwRenderNode::update(CWFLOAT dt)
{

}

void cwRenderNode::render()
{
	if (m_bVisible) {
		this->transform();
		this->renderSelf();
		this->renderChildren();
	}
}

void cwRenderNode::renderChildren()
{
	for (auto it = m_nVecChildren.begin(); it != m_nVecChildren.end(); ++it) {
		(*it)->render();
	}
}

void cwRenderNode::renderSelf()
{

}

NS_MINI_END


