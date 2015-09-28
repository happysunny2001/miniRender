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

#include "cwFrustum.h"
#include "cwMatrix4X4.h"
#include "Camera/cwCamera.h"

NS_MINIR_BEGIN

cwFrustum::cwFrustum()
{

}

cwFrustum::~cwFrustum()
{

}

void cwFrustum::refresh(cwCamera* pCamera)
{
	if (pCamera) {
		const cwMatrix4X4& matViewProj = pCamera->getViewProjMatrix();

		m_nPlanes[LeftPlane].m_nNormal.x = matViewProj.m14 + matViewProj.m11;
		m_nPlanes[LeftPlane].m_nNormal.y = matViewProj.m24 + matViewProj.m21;
		m_nPlanes[LeftPlane].m_nNormal.z = matViewProj.m34 + matViewProj.m31;
		m_nPlanes[LeftPlane].m_fD = -(matViewProj.m44 + matViewProj.m41);

		m_nPlanes[RightPlane].m_nNormal.x = matViewProj.m14 - matViewProj.m11;
		m_nPlanes[RightPlane].m_nNormal.y = matViewProj.m24 - matViewProj.m21;
		m_nPlanes[RightPlane].m_nNormal.z = matViewProj.m34 - matViewProj.m31;
		m_nPlanes[RightPlane].m_fD = -(matViewProj.m44 - matViewProj.m41);

		m_nPlanes[BottomPlane].m_nNormal.x = matViewProj.m14 + matViewProj.m12;
		m_nPlanes[BottomPlane].m_nNormal.y = matViewProj.m24 + matViewProj.m22;
		m_nPlanes[BottomPlane].m_nNormal.z = matViewProj.m34 + matViewProj.m32;
		m_nPlanes[BottomPlane].m_fD = -(matViewProj.m44 + matViewProj.m42);

		m_nPlanes[TopPlane].m_nNormal.x = matViewProj.m14 - matViewProj.m12;
		m_nPlanes[TopPlane].m_nNormal.y = matViewProj.m24 - matViewProj.m22;
		m_nPlanes[TopPlane].m_nNormal.z = matViewProj.m34 - matViewProj.m32;
		m_nPlanes[TopPlane].m_fD = -(matViewProj.m44 - matViewProj.m42);

		m_nPlanes[NearPlane].m_nNormal.x = matViewProj.m13;
		m_nPlanes[NearPlane].m_nNormal.y = matViewProj.m23;
		m_nPlanes[NearPlane].m_nNormal.z = matViewProj.m33;
		m_nPlanes[NearPlane].m_fD = -matViewProj.m43;

		m_nPlanes[FarPlane].m_nNormal.x = matViewProj.m14 - matViewProj.m13;
		m_nPlanes[FarPlane].m_nNormal.y = matViewProj.m24 - matViewProj.m23;
		m_nPlanes[FarPlane].m_nNormal.z = matViewProj.m34 - matViewProj.m33;
		m_nPlanes[FarPlane].m_fD = -(matViewProj.m44 - matViewProj.m43);

		m_nPlanes[LeftPlane].normalize();
		m_nPlanes[RightPlane].normalize();
		m_nPlanes[BottomPlane].normalize();
		m_nPlanes[TopPlane].normalize();
		m_nPlanes[NearPlane].normalize();
		m_nPlanes[FarPlane].normalize();
	}
}

int cwFrustum::intersection(const cwAABB& aabb) const
{
	int ret = 0;
	for (int i = LeftPlane; i <= FarPlane; ++i) {
		int iRet = aabb.intersection(m_nPlanes[i]);
		if (iRet < 0) return ret;
		ret |= 1 << i;
		ret |= iRet << (8 + i);
	}

	return ret;
}

int cwFrustum::intersection(const cwCircle& circle) const
{
	int ret = 0;
	for (int i = LeftPlane; i <= FarPlane; ++i) {
		int iRet = circle.intersection(m_nPlanes[i]);
		if (iRet < 0) return ret;
		ret |= 1 << i;
		ret |= iRet << (8 + i);
	}

	return ret;
}

bool cwFrustum::isCollide(int iRet) const
{
	return (iRet & 0x003F) == 0x003F;
}

NS_MINIR_END
