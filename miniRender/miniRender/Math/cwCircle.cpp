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

#include "cwCircle.h"
#include "cwMathUtil.h"
//#include "cwIntersection.h"
#include "cwRay.h"
#include "cwPlane.h"
#include "cwAABB.h"

NS_MINIR_BEGIN

cwCircle::cwCircle()
{

}

cwCircle::cwCircle(const cwPoint3D& o, float r):
m_nOrigin(o),
m_fR(r)
{

}

cwCircle::cwCircle(const cwCircle& c):
m_nOrigin(c.m_nOrigin),
m_fR(c.m_fR)
{

}

cwCircle::~cwCircle()
{
    
}

cwVector3D cwCircle::closestPoint(const cwPoint3D& p) const
{
    cwVector3D d = m_nOrigin - p;
	if (d.squareLength() < cwMathUtil::cwFloatEpsilon) return m_nOrigin;
    float dLen = d.length();
    
    return p + (dLen-m_fR)*(d/dLen);
}

float cwCircle::closestPoint(const cwPoint3D& p, cwVector3D& ret) const
{
    cwVector3D d = m_nOrigin - p;
	if (d.squareLength() < cwMathUtil::cwFloatEpsilon) {
        ret = m_nOrigin;
        return 0;
    }
    
    float dLen = d.length();
    ret = p + (dLen-m_fR)*(d/dLen);
    return dLen-m_fR;
}

//int cwCircle::intersection(const cwShape& other) const
//{
//    switch (other.m_eType) {
//        case eShapeRay:
//            return cwIntersectionRayCircle(static_cast<const cwRay&>(other), *this);
//        case eShapeCircle:
//            return cwIntersectionCircleCircle(*this, static_cast<const cwCircle&>(other));
//        case eShapePlane:
//            return cwIntersectionPlaneCircle(static_cast<const cwPlane&>(other), *this);
//        case eShapeAABB:
//            return cwIntersectionCircleAABB(*this, static_cast<const cwAABB&>(other));
//        default:
//            return false;
//    }
//    
//    return false;
//}

int cwCircle::intersection(const cwRay& ray) const
{
	cwVector3D e = this->m_nOrigin - ray.m_nOrigin;
	float a = e.dot(ray.m_nDir);
	float f = this->m_fR*this->m_fR - e.dot(e) + a*a;
	if (f < 0) return 0;

	float t = a - sqrt(f);
	if (t < 0 || t > ray.m_fT) return 0;

	return 1;
}

int cwCircle::intersection(const cwCircle& circle) const
{
	cwVector2D d = circle.m_nOrigin - this->m_nOrigin;
	if (d.squareLength() <= (this->m_fR + circle.m_fR)*(this->m_fR + circle.m_fR)) return true;
	return false;
}

int cwCircle::intersection(const cwPlane& plane) const
{
	float d = plane.m_nNormal.dot(this->m_nOrigin) - plane.m_fD;

	if (d > this->m_fR) return 1;
	if (d < -this->m_fR) return -1;
	return 0;
}

int cwCircle::intersection(const cwAABB& aabb) const
{
	cwVector3D p = aabb.closestPoint(this->m_nOrigin);
	cwVector3D v = p - this->m_nOrigin;
	if (v.squareLength() <= this->m_fR*this->m_fR) return 1;
	return 0;
}

NS_MINIR_END













