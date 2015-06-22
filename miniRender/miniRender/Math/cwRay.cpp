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
#include <float.h>
#include "cwMathUtil.h"
#include "cwCircle.h"
#include "cwPlane.h"
#include "cwAABB.h"
#include "cwIntersection.h"

NS_MINI_BEGIN

cwRay::cwRay() :
m_fT(FLT_MAX)
{
    m_eType = eShapeRay;
}

cwRay::cwRay(const cwPoint3D& nPos, const cwVector3D& v) :
m_nOrigin(nPos),
m_nDir(v),
m_fT(FLT_MAX)
{
    m_eType = eShapeRay;
}

cwRay::cwRay(const cwPoint3D& nPos, const cwVector3D& v, float t) :
m_nOrigin(nPos),
m_nDir(v),
m_fT(t)
{
    m_eType = eShapeRay;
}

cwRay::cwRay(const cwRay& r) :
m_nOrigin(r.m_nOrigin),
m_nDir(r.m_nDir),
m_fT(r.m_fT)
{
    m_eType = eShapeRay;
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

bool cwRay::intersection(const cwShape& other) const
{
    switch (other.m_eType) {
        case eShapeRay:
            return cwIntersectionRayRay(*this, static_cast<const cwRay&>(other));
        case eShapeCircle:
            return cwIntersectionRayCircle(*this, static_cast<const cwCircle&>(other));
        case eShapePlane:
            return cwIntersectionRayPlane(*this, static_cast<const cwPlane&>(other));
        case eShapeAABB:
            return cwIntersectionRayAABB(*this, static_cast<const cwAABB&>(other));
        default:
            return false;
    }
    
    return false;
}

NS_MINI_END







