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

#include "cwRenderNode2D.h"

NS_MINIR_BEGIN

cwRenderNode2D* cwRenderNode2D::create()
{
	cwRenderNode2D* pNode = new cwRenderNode2D();
	if (pNode && pNode->init()) {
		pNode->autorelease();
		return pNode;
	}

	CW_SAFE_DELETE(pNode);
	return nullptr;
}

cwRenderNode2D::cwRenderNode2D():
m_uRenderOrder(0)
{
	m_eRenderType = eRenderTypeNode2D;
}

cwRenderNode2D::~cwRenderNode2D()
{

}

CWBOOL cwRenderNode2D::init()
{
	if (!cwRenderNode::init()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwRenderNode2D::addChild(cwRenderNode* pNode)
{
	return CWFALSE;
}

CWBOOL cwRenderNode2D::addChild(cwRenderNode2D* pNode)
{
	if (pNode && !pNode->getParent()) {
		if (!m_nVecChildren.contains(pNode)) {

			CWUINT64 iSize = m_nVecChildren.size();
			CWUINT64 index = 0;
			CWBOOL bAdded = CWFALSE;
			for (; index < iSize; ++index) {
				cwRenderNode2D* pNode2D = static_cast<cwRenderNode2D*>(m_nVecChildren.at(index));
				if (pNode->getRenderOrder() <= pNode2D->getRenderOrder()) {
					m_nVecChildren.insert(index, pNode2D);
					bAdded = CWTRUE;
					break;
				}
			}

			if (!bAdded) {
				m_nVecChildren.pushBack(pNode);
			}
			return CWTRUE;
		}
	}

	return CWFALSE;
}

CWVOID cwRenderNode2D::refreshBoundingBox()
{
	cwAABB nAabb = m_nBoundingBox;
	m_nBoundingBox.update(nAabb, m_nTrans);
}

CWVOID cwRenderNode2D::setPosition(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	cwRenderNode::setPosition(x, y, 0.0f);
}

CWVOID cwRenderNode2D::setPosition(const cwVector3D& v)
{
	cwRenderNode::setPosition(v.x, v.y, 0.0f);
}

CWVOID cwRenderNode2D::move(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	cwRenderNode::move(x, y, 0.0f);
}

CWVOID cwRenderNode2D::move(const cwVector3D& v)
{
	cwRenderNode2D::move(v.x, v.y, 0.0f);
}

CWVOID cwRenderNode2D::setPosition(CWFLOAT x, CWFLOAT y)
{
	cwRenderNode::setPosition(x, y, 0.0f);
}

CWVOID cwRenderNode2D::move(CWFLOAT x, CWFLOAT y)
{
	cwRenderNode::move(x, y, 0.0f);
}

CWVOID cwRenderNode2D::setRotation(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	cwRenderNode::setRotation(0.0f, 0.0f, z);
}

CWVOID cwRenderNode2D::setRotation(const cwVector3D& v)
{
	cwRenderNode::setRotation(0.0f, 0.0f, v.z);
}

CWVOID cwRenderNode2D::rotate(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	cwRenderNode::rotate(0.0f, 0.0f, z);
}

CWVOID cwRenderNode2D::rotate(const cwVector3D& v)
{
	cwRenderNode::rotate(0.0f, 0.0f, v.z);
}

CWVOID cwRenderNode2D::setRotation(CWFLOAT f)
{
	cwRenderNode::setRotation(0.0f, 0.0f, f);
}

CWVOID cwRenderNode2D::rotate(CWFLOAT f)
{
	cwRenderNode::rotate(0.0f, 0.0f, f);
}

CWVOID cwRenderNode2D::setScale(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	cwRenderNode::setScale(x, y, 0.0f);
}

CWVOID cwRenderNode2D::setScale(const cwVector3D& v)
{
	cwRenderNode::setScale(v.x, v.y, 0.0f);
}

CWVOID cwRenderNode2D::scale(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	cwRenderNode::scale(x, y, 0.0f);
}

CWVOID cwRenderNode2D::scale(const cwVector3D& v)
{
	cwRenderNode::scale(v.x, v.y, 0.0f);
}

CWVOID cwRenderNode2D::setScale(CWFLOAT x, CWFLOAT y)
{
	cwRenderNode::setScale(x, y, 0.0f);
}

CWVOID cwRenderNode2D::scale(CWFLOAT x, CWFLOAT y)
{
	cwRenderNode::scale(x, y, 0.0f);
}

CWVOID cwRenderNode2D::setRenderOrder(CWINT uOrder)
{
	m_uRenderOrder = uOrder;
}

CWVOID cwRenderNode2D::insertSpatialNode(cwRenderNode* pNode)
{

}

CWVOID cwRenderNode2D::removeSpatialNode(cwRenderNode* pNode)
{

}

CWVOID cwRenderNode2D::refreshSpatialNode()
{

}

CWVOID cwRenderNode2D::render(cwCamera* pCamera)
{

}

NS_MINIR_END
