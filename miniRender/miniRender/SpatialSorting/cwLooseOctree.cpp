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

#include "cwLooseOctree.h"

NS_MINIR_BEGIN

cwLooseOctree* cwLooseOctree::create()
{
	cwLooseOctree* pOctree = new cwLooseOctree();
	if (pOctree && pOctree->init()) {
		pOctree->autorelease();
		return pOctree;
	}

	CW_SAFE_DELETE(pOctree);
	return nullptr;
}

cwLooseOctree* cwLooseOctree::create(const sOctreeInit& octreeInit)
{
	cwLooseOctree* pOctree = new cwLooseOctree();
	if (pOctree && pOctree->init(octreeInit)) {
		pOctree->autorelease();
		return pOctree;
	}

	CW_SAFE_DELETE(pOctree);
	return nullptr;
}

CWVOID cwLooseOctree::getChildrenBoundingBox(sOctreeNode* pOctreeNode, cwAABB* pAabb)
{
	if (!pOctreeNode || !pAabb) return;

	const cwVector3D& minPos = pOctreeNode->m_nBox.m_nMin;
	const cwVector3D& maxPos = pOctreeNode->m_nBox.m_nMax;
	cwVector3D center = pOctreeNode->m_nBox.center();
	float fOffset = (maxPos.x - minPos.x)*0.25f;
	cwVector3D offset(fOffset, fOffset, fOffset);

	pAabb[eOctreeTopFarLeft].m_nMin.set(minPos.x - fOffset, center.y - fOffset, center.z - fOffset);
	pAabb[eOctreeTopFarLeft].m_nMax.set(center.x + fOffset, maxPos.y + fOffset, maxPos.z + fOffset);

	pAabb[eOctreeTopFarRight].m_nMin = center - offset;
	pAabb[eOctreeTopFarRight].m_nMax = maxPos + offset;

	pAabb[eOctreeTopNearLeft].m_nMin.set(minPos.x - fOffset, center.y - fOffset, minPos.z - fOffset);
	pAabb[eOctreeTopNearLeft].m_nMax.set(center.x + fOffset, maxPos.y + fOffset, center.z + fOffset);

	pAabb[eOctreeTopNearRight].m_nMin.set(center.x - fOffset, center.y - fOffset, minPos.z - fOffset);
	pAabb[eOctreeTopNearRight].m_nMax.set(maxPos.x + fOffset, maxPos.y + fOffset, center.z + fOffset);

	pAabb[eOctreeBottomFarLeft].m_nMin.set(minPos.x - fOffset, minPos.y - fOffset, center.z - fOffset);
	pAabb[eOctreeBottomFarLeft].m_nMax.set(center.x + fOffset, center.y + fOffset, maxPos.z + fOffset);

	pAabb[eOctreeBottomFarRight].m_nMin.set(center.x - fOffset, minPos.y - fOffset, center.z - fOffset);
	pAabb[eOctreeBottomFarRight].m_nMax.set(maxPos.x + fOffset, center.y + fOffset, maxPos.z + fOffset);

	pAabb[eOctreeBottomNearLeft].m_nMin = minPos - offset;
	pAabb[eOctreeBottomNearLeft].m_nMax = center + offset;

	pAabb[eOctreeBottomNearRight].m_nMin.set(center.x - fOffset, minPos.y - fOffset, minPos.z - fOffset);
	pAabb[eOctreeBottomNearRight].m_nMax.set(maxPos.x + fOffset, center.y + fOffset, center.z + fOffset);
}

NS_MINIR_END
