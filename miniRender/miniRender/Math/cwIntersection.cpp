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

#include "cwIntersection.h"
#include "cwRay.h"
#include "cwCircle.h"
#include "cwPlane.h"
#include "cwAABB.h"
#include "cwMathUtil.h"

NS_MINI_BEGIN

bool cwIntersectionRayRay(const cwRay& r1, const cwRay& r2)
{
    cwVector3D d = r1.m_nDir.cross(r2.m_nDir);
    float dSquLen = d.squareLength();
    //lines are parallel
	if (dSquLen < cwMathUtil::cwFloatEpsilon) return false;
    
    float dSquLenInv = 1.0f / dSquLen;
    cwVector3D v = (r2.m_nOrigin-r1.m_nOrigin);
    
    float t1 = v.cross(r2.m_nDir).dot(d) * dSquLenInv;
    float t2 = v.cross(r1.m_nDir).dot(d) * dSquLenInv;
    
    if(t1 < 0 || t2 < 0 || t1 > r1.m_fT || t2 > r2.m_fT) return false;
    
    cwPoint3D p1 = r1.m_nOrigin + t1*r1.m_nDir;
    cwPoint3D p2 = r2.m_nOrigin + t2*r2.m_nDir;
    
    //two line maybe skew, we examine the distance between p1 and p2
	if ((p2 - p1).squareLength() < cwMathUtil::cwFloatEpsilon) return true;
    
    return false;
}

bool cwIntersectionRayCircle(const cwRay& ray, const cwCircle& circle)
{
    cwVector3D e = circle.m_nOrigin - ray.m_nOrigin;
    float a = e.dot(ray.m_nDir);
    float f = circle.m_fR*circle.m_fR - e.dot(e) + a*a;
    if(f < 0) return false;
    
    float t = a - sqrt(f);
    if(t < 0 || t > ray.m_fT) return false;
    
    return true;
}

bool cwIntersectionRayPlane(const cwRay& ray, const cwPlane& plane)
{
    float f = ray.m_nDir.dot(plane.m_nNormal);
	if (f < cwMathUtil::cwFloatEpsilon) return false;
    
    float t = (plane.m_fD - ray.m_nOrigin.dot(plane.m_nNormal)) / f;
    if(t < 0 || t > ray.m_fT) return false;
    return true;
}

bool cwIntersectionRayAABB(const cwRay& ray, const cwAABB& aabb)
{
    cwVector3D rayDelta = ray.m_nDir*(ray.m_fT*0.5f);
    bool inside = true;
    
    float xt, xn;
    if(ray.m_nOrigin.x < aabb.m_nMin.x) {
        xt = aabb.m_nMin.x - ray.m_nOrigin.x;
        if(xt > rayDelta.x) return false;
        xt /= rayDelta.x;
        inside = false;
        xn = -1.0f;
    }
    else if(ray.m_nOrigin.x > aabb.m_nMax.x) {
        xt = aabb.m_nMax.x - ray.m_nOrigin.x;
        if(xt < rayDelta.x) return false;
        xt /= rayDelta.x;
        inside = false;
        xn = 1.0f;
    }
    else {
        xt = -1.0f;
    }
    
    float yt, yn;
    if(ray.m_nOrigin.y < aabb.m_nMin.y) {
        yt = aabb.m_nMin.y - ray.m_nOrigin.y;
        if(yt > rayDelta.y) return false;
        yt /= rayDelta.y;
        inside = false;
        yn = -1.0f;
    }
    else if(ray.m_nOrigin.y > aabb.m_nMax.y) {
        yt = aabb.m_nMax.y - ray.m_nOrigin.y;
        if(yt < rayDelta.y) return false;
        yt /= rayDelta.y;
        inside = false;
        yn = 1.0f;
    }
    else {
        yt = -1.0f;
    }
    
    float zt, zn;
    if(ray.m_nOrigin.z < aabb.m_nMin.z) {
        zt = aabb.m_nMin.z - ray.m_nOrigin.z;
        if(zt > rayDelta.z) return false;
        zt /= rayDelta.z;
        inside = false;
        zn = -1.0f;
    }
    else if(ray.m_nOrigin.z > aabb.m_nMax.z) {
        zt = aabb.m_nMax.z - ray.m_nOrigin.z;
        if(zt < rayDelta.z) return false;
        zt /= rayDelta.z;
        inside = false;
        zn = 1.0f;
    }
    else {
        zt = -1.0f;
    }
    
    if(inside) {
        return false;
    }
    
    int which = 0;
    float t = xt;
    if(yt > t) {
        which = 1;
        t = yt;
    }
    
    if(zt > t) {
        which = 2;
        t = zt;
    }
    
    switch (which) {
        case 0:
        {
            float y = ray.m_nOrigin.y + rayDelta.y*t;
            if(y < aabb.m_nMin.y || y > aabb.m_nMax.y) return false;
            float z = ray.m_nOrigin.z + rayDelta.z*t;
            if(z < aabb.m_nMin.z || z > aabb.m_nMax.z) return false;
        }
            break;
        case 1:
        {
            float x = ray.m_nOrigin.x + rayDelta.x*t;
            if(x < aabb.m_nMin.x || x > aabb.m_nMax.x) return false;
            float z = ray.m_nOrigin.z + rayDelta.z*t;
            if(z < aabb.m_nMin.z || z > aabb.m_nMax.z) return false;
        }
            break;
        case 2:
        {
            float x = ray.m_nOrigin.x + rayDelta.x*t;
            if(x < aabb.m_nMin.x || x > aabb.m_nMax.x) return false;
            float y = ray.m_nOrigin.y + rayDelta.y*t;
            if(y < aabb.m_nMin.y || y > aabb.m_nMax.y) return false;
        }
            break;
    }
    
    return true;
}

bool cwIntersectionPlanePlane(const cwPlane& p1, const cwPlane& p2)
{
	if (fabsf(fabsf(p1.m_nNormal.dot(p2.m_nNormal)) - 1.0f) < cwMathUtil::cwFloatEpsilon) return false;
    return true;
}

bool cwIntersectionPlaneCircle(const cwPlane& plane, const cwCircle& circle)
{
    float d = plane.m_nNormal.dot(circle.m_nOrigin) - plane.m_fD;
    
    if(d < -circle.m_fR || d > circle.m_fR) return false;
    return true;
}

bool cwIntersectionPlaneAABB(const cwPlane& plane, const cwAABB& aabb)
{
    float minD, maxD;
    
    if(plane.m_nNormal.x > 0) {
        minD = plane.m_nNormal.x*aabb.m_nMin.x;
        maxD = plane.m_nNormal.x*aabb.m_nMax.x;
    }
    else {
        minD = plane.m_nNormal.x*aabb.m_nMax.x;
        maxD = plane.m_nNormal.x*aabb.m_nMin.x;
    }
    
    if (plane.m_nNormal.y > 0) {
        minD += plane.m_nNormal.y*aabb.m_nMin.y;
        maxD += plane.m_nNormal.y*aabb.m_nMax.y;
    }
    else {
        minD += plane.m_nNormal.y*aabb.m_nMax.y;
        maxD += plane.m_nNormal.y*aabb.m_nMin.y;
    }
    
    if(plane.m_nNormal.z > 0) {
        minD += plane.m_nNormal.z*aabb.m_nMin.z;
        maxD += plane.m_nNormal.z*aabb.m_nMax.z;
    }
    else {
        minD += plane.m_nNormal.z*aabb.m_nMax.z;
        maxD += plane.m_nNormal.z*aabb.m_nMin.z;
    }
    
    if(minD > plane.m_fD || maxD < plane.m_fD) return false;
    return true;
}

bool cwIntersectionCircleCircle(const cwCircle& c1, const cwCircle& c2)
{
    cwVector2D d = c2.m_nOrigin - c1.m_nOrigin;
    if(d.squareLength() <= (c1.m_fR+c2.m_fR)*(c1.m_fR+c2.m_fR)) return true;
    return false;
}

bool cwIntersectionCircleAABB(const cwCircle& circle, const cwAABB& aabb)
{
    cwVector3D p = aabb.closestPoint(circle.m_nOrigin);
    cwVector3D v = p - circle.m_nOrigin;
    if(v.squareLength() <= circle.m_fR*circle.m_fR) return true;
    return false;
}

bool cwIntersectionAABBAABB(const cwAABB& a1, const cwAABB& a2)
{
    if(a1.m_nMin.x >= a2.m_nMax.x) return false;
    if(a1.m_nMax.x <= a2.m_nMin.x) return false;
    if(a1.m_nMin.y >= a2.m_nMax.y) return false;
    if(a1.m_nMax.y <= a2.m_nMin.y) return false;
    if(a1.m_nMin.z >= a2.m_nMax.z) return false;
    if(a1.m_nMax.z <= a2.m_nMin.z) return false;
    
    return true;
}

NS_MINI_END





















