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

#include "cwPlane.h"
#include "cwRay.h"
#include "cwCircle.h"
#include "cwAABB.h"
//#include "cwIntersection.h"
#include "cwMathUtil.h"

NS_MINIR_BEGIN

cwPlane::cwPlane():
m_fD(0)
{

}

cwPlane::cwPlane(const cwVector3D& n, float d):
m_nNormal(n),
m_fD(d)
{

}

cwPlane::cwPlane(const cwPoint3D& p1, const cwPoint3D& p2, const cwPoint3D& p3)
{
    cwVector3D e1 = p2 - p1;
    cwVector3D e2 = p3 - p1;
    
    m_nNormal = e1.cross(e2);
    m_nNormal.normalize();
    
    m_fD = m_nNormal.dot(p1);
}

cwPlane::cwPlane(const cwPlane& p):
m_nNormal(p.m_nNormal),
m_fD(p.m_fD)
{

}

cwPlane::~cwPlane()
{
    
}

cwVector3D cwPlane::closestPoint(const cwPoint3D& p) const
{
    return p + (m_fD - p.dot(m_nNormal))*m_nNormal;
}

float cwPlane::closestPoint(const cwPoint3D& p, cwVector3D& ret) const
{
    float f = m_fD - p.dot(m_nNormal);
    ret = p + f*m_nNormal;
    return f;
}

void cwPlane::update(const cwMatrix4X4& mat)
{
	cwVector3D vecTrans = mat.getTranslation();
	m_nNormal *= mat;
	m_nNormal.normalize();
	m_fD = m_nNormal.dot(vecTrans);
}

void cwPlane::normalize()
{
	float fDivLen = 1.0f / m_nNormal.length();
	m_nNormal *= fDivLen;
	m_fD *= fDivLen;
}

//int cwPlane::intersection(const cwShape& other) const
//{
//    switch (other.m_eType) {
//        case eShapeRay:
//            return cwIntersectionRayPlane(static_cast<const cwRay&>(other), *this);
//        case eShapePlane:
//            return cwIntersectionPlanePlane(*this, static_cast<const cwPlane&>(other));
//        case eShapeCircle:
//            return cwIntersectionPlaneCircle(*this, static_cast<const cwCircle&>(other));
//        case eShapeAABB:
//            return cwIntersectionPlaneAABB(*this, static_cast<const cwAABB&>(other));
//        default:
//            return false;
//    }
//    return false;
//}

int cwPlane::intersection(const cwRay& ray) const
{
	float f = ray.m_nDir.dot(this->m_nNormal);
	if (f < cwMathUtil::cwFloatEpsilon) return 0;

	float t = (this->m_fD - ray.m_nOrigin.dot(this->m_nNormal)) / f;
	if (t < 0 || t > ray.m_fT) return 0;
	return 1;
}

int cwPlane::intersection(const cwPlane& plane) const
{
	if (fabsf(fabsf(this->m_nNormal.dot(plane.m_nNormal)) - 1.0f) < cwMathUtil::cwFloatEpsilon) return 0;
	return 1;
}

int cwPlane::intersection(const cwCircle& circle) const
{
	return circle.intersection(*this);
}

int cwPlane::intersection(const cwAABB& aabb) const
{
	return aabb.intersection(*this);
}

NS_MINIR_END









