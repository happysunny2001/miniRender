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

#ifndef __CWINTERSECTION_H__
#define __CWINTERSECTION_H__

#include "../Base/cwMacros.h"

NS_MINI_BEGIN

class cwRay;
class cwCircle;
class cwPlane;
class cwAABB;

bool cwIntersectionRayRay(const cwRay& r1, const cwRay& r2);
bool cwIntersectionRayCircle(const cwRay& ray, const cwCircle& circle);
bool cwIntersectionRayPlane(const cwRay& ray, const cwPlane& plane);
bool cwIntersectionRayAABB(const cwRay& ray, const cwAABB& aabb);

bool cwIntersectionPlanePlane(const cwPlane& p1, const cwPlane& p2);
bool cwIntersectionPlaneCircle(const cwPlane& plane, const cwCircle& circle);
bool cwIntersectionPlaneAABB(const cwPlane& plane, const cwAABB& aabb);

bool cwIntersectionCircleCircle(const cwCircle& c1, const cwCircle& c2);
bool cwIntersectionCircleAABB(const cwCircle& circle, const cwAABB& aabb);

bool cwIntersectionAABBAABB(const cwAABB& a1, const cwAABB& a2);

NS_MINI_END

#endif /* defined(__testGL31__cwIntersection__) */
