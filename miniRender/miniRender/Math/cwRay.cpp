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

#include "cwRay.h"
#include "cwMathUtil.h"
#include "cwCircle.h"
#include "cwPlane.h"
#include "cwAABB.h"
//#include "cwIntersection.h"

#include <float.h>

NS_MINIR_BEGIN

cwRay::cwRay() :
m_fT(FLT_MAX)
{

}

cwRay::cwRay(const cwPoint3D& nPos, const cwVector3D& v) :
m_nOrigin(nPos),
m_nDir(v),
m_fT(FLT_MAX)
{

}

cwRay::cwRay(const cwPoint3D& nPos, const cwVector3D& v, float t) :
m_nOrigin(nPos),
m_nDir(v),
m_fT(t)
{

}

cwRay::cwRay(const cwRay& r) :
m_nOrigin(r.m_nOrigin),
m_nDir(r.m_nDir),
m_fT(r.m_fT)
{

}

cwRay::~cwRay()
{
    
}

cwVector3D cwRay::closestPoint(const cwPoint3D& p) const
{
    return m_nOrigin + ((p-m_nOrigin).dot(m_nDir))*m_nDir;
}

float cwRay::closestPoint(const cwPoint3D& p, cwVector3D& ret) const
{
    float t = (p-m_nOrigin).dot(m_nDir);
    ret = m_nOrigin + t*m_nDir;
    return t;
}

//int cwRay::intersection(const cwShape& other) const
//{
//    switch (other.m_eType) {
//        case eShapeRay:
//            return cwIntersectionRayRay(*this, static_cast<const cwRay&>(other));
//        case eShapeCircle:
//            return cwIntersectionRayCircle(*this, static_cast<const cwCircle&>(other));
//        case eShapePlane:
//            return cwIntersectionRayPlane(*this, static_cast<const cwPlane&>(other));
//        case eShapeAABB:
//            return cwIntersectionRayAABB(*this, static_cast<const cwAABB&>(other));
//        default:
//            return false;
//    }
//    
//    return false;
//}

int cwRay::intersection(const cwRay& ray) const
{
	cwVector3D d = this->m_nDir.cross(ray.m_nDir);
	float dSquLen = d.squareLength();
	//lines are parallel
	if (dSquLen < cwMathUtil::cwFloatEpsilon) return 0;

	float dSquLenInv = 1.0f / dSquLen;
	cwVector3D v = (ray.m_nOrigin - this->m_nOrigin);

	float t1 = v.cross(ray.m_nDir).dot(d) * dSquLenInv;
	float t2 = v.cross(this->m_nDir).dot(d) * dSquLenInv;

	if (t1 < 0 || t2 < 0 || t1 > this->m_fT || t2 > ray.m_fT) return 0;

	cwPoint3D p1 = this->m_nOrigin + t1*this->m_nDir;
	cwPoint3D p2 = ray.m_nOrigin + t2*ray.m_nDir;

	//two line maybe skew, we examine the distance between p1 and p2
	if ((p2 - p1).squareLength() < cwMathUtil::cwFloatEpsilon) return 1;

	return 0;
}

int cwRay::intersection(const cwCircle& circle) const
{
	return circle.intersection(*this);
}

int cwRay::intersection(const cwPlane& plane) const
{
	return plane.intersection(*this);
}

int cwRay::intersection(const cwAABB& aabb) const
{
	return aabb.intersection(*this);
}

NS_MINIR_END







