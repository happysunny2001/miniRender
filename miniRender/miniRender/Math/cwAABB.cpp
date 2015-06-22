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

#include "cwAABB.h"
#include <float.h>
#include "cwIntersection.h"
#include "cwRay.h"
#include "cwPlane.h"
#include "cwCircle.h"

NS_MINI_BEGIN

cwAABB::cwAABB():
m_nMin(cwPoint3D::ZERO),
m_nMax(cwPoint3D::ZERO)
{
    m_eType = eShapeAABB;
}

cwAABB::cwAABB(const cwPoint3D& ptMin, const cwPoint3D& ptMax):
m_nMin(ptMin),
m_nMax(ptMax)
{
    m_eType = eShapeAABB;
}
cwAABB::cwAABB(cwPoint3D* p, int n)
{
    m_eType = eShapeAABB;
    compute(p, n);
}

cwAABB::cwAABB(const cwAABB& box):
m_nMin(box.m_nMin),
m_nMax(box.m_nMax)
{
    m_eType = eShapeAABB;
}

cwAABB::~cwAABB()
{

}

///////////////////////////////////////////////////////////////
//
//      6                  7
//       ------------------
//      /|                /|
//     / |               / |
//    /  |              /  |
//   /   |             /   |
// 2/----------------/ 3   |
// |     |            |    |
// |     |            |    |        |
// |     |------------|--- |      +y|
// |    / 4           |    /5       |
// |   /              |   /         |   / +z
// |  /               |  /          |  /
// | /                | /           | /
// |/                 |/            |/
// -------------------              |--------------- +x
// 0                  1
///////////////////////////////////////////////////////////////
cwVector3D cwAABB::corner(int index) const
{
    return cwVector3D((index&1)?m_nMax.x:m_nMin.x,
                      (index&2)?m_nMax.y:m_nMin.y,
                      (index&4)?m_nMax.z:m_nMin.z);
}

cwVector3D cwAABB::size() const
{
    return m_nMax - m_nMin;
}

cwVector3D cwAABB::center() const
{
    return (m_nMin + m_nMax) * 0.5f;
}

cwVector3D cwAABB::radius() const
{
    return (m_nMax - m_nMin)*0.5f;
}

void cwAABB::empty()
{
    m_nMin.x = m_nMin.y = m_nMin.z = FLT_MAX;
    m_nMax.x = m_nMax.y = m_nMax.z = -FLT_MAX;
}

void cwAABB::add(const cwPoint3D& p)
{
    if(p.x < m_nMin.x) m_nMin.x = p.x;
    if(p.x > m_nMax.x) m_nMax.x = p.x;
    if(p.y < m_nMin.y) m_nMin.y = p.y;
    if(p.y > m_nMax.y) m_nMax.y = p.y;
    if(p.z < m_nMin.z) m_nMin.z = p.z;
    if(p.z > m_nMax.z) m_nMax.z = p.z;
}

void cwAABB::add(const cwAABB& box)
{
    if(box.m_nMin.x < m_nMin.x) m_nMin.x = box.m_nMin.x;
    if(box.m_nMax.x > m_nMax.x) m_nMax.x = box.m_nMax.x;
    if(box.m_nMin.y < m_nMin.y) m_nMin.y = box.m_nMin.y;
    if(box.m_nMax.y > m_nMax.y) m_nMax.y = box.m_nMax.y;
    if(box.m_nMin.z < m_nMin.z) m_nMin.z = box.m_nMin.z;
    if(box.m_nMax.z > m_nMax.z) m_nMax.z = box.m_nMax.z;
}

void cwAABB::compute(cwPoint3D* p, int n)
{
    if(!p) return;
    this->empty();
    
    for(int i = 0; i < n; ++i) {
        this->add(p[i]);
    }
}

void cwAABB::compute(const cwAABB& box, const cwMatrix4X4& m)
{
    m_nMin = m_nMax = m.getTranslation();
    
    if(m.m11 > 0) {
        m_nMin.x += box.m_nMin.x*m.m11;
        m_nMax.x += box.m_nMax.x*m.m11;
    }
    else {
        m_nMin.x += box.m_nMax.x*m.m11;
        m_nMax.x += box.m_nMin.x*m.m11;
    }
    
    if(m.m12 > 0) {
        m_nMin.y += box.m_nMin.x*m.m12;
        m_nMax.y += box.m_nMax.x*m.m12;
    }
    else {
        m_nMin.y += box.m_nMax.x*m.m12;
        m_nMax.y += box.m_nMin.x*m.m12;
    }
    
    if(m.m13 > 0) {
        m_nMin.z += box.m_nMin.x*m.m13;
        m_nMax.z += box.m_nMax.x*m.m13;
    }
    else {
        m_nMin.z += box.m_nMax.x*m.m13;
        m_nMax.z += box.m_nMin.x*m.m13;
    }
    
    if(m.m21 > 0) {
        m_nMin.x += box.m_nMin.y*m.m21;
        m_nMax.x += box.m_nMax.y*m.m21;
    }
    else {
        m_nMin.x += box.m_nMax.y*m.m21;
        m_nMax.x += box.m_nMin.y*m.m21;
    }
    
    if(m.m22 > 0) {
        m_nMin.y += box.m_nMin.y*m.m22;
        m_nMax.y += box.m_nMax.y*m.m22;
    }
    else {
        m_nMin.y += box.m_nMax.y*m.m22;
        m_nMax.y += box.m_nMin.y*m.m22;
    }
    
    if(m.m23 > 0) {
        m_nMin.z += box.m_nMin.y*m.m23;
        m_nMax.z += box.m_nMax.y*m.m23;
    }
    else {
        m_nMin.z += box.m_nMax.y*m.m23;
        m_nMax.z += box.m_nMin.y*m.m23;
    }
    
    if(m.m31 > 0) {
        m_nMin.x += box.m_nMin.z*m.m31;
        m_nMax.x += box.m_nMax.z*m.m31;
    }
    else {
        m_nMin.x += box.m_nMax.z*m.m31;
        m_nMax.x += box.m_nMin.z*m.m31;
    }
    
    if(m.m32 > 0) {
        m_nMin.y += box.m_nMin.z*m.m32;
        m_nMax.y += box.m_nMax.z*m.m32;
    }
    else {
        m_nMin.y += box.m_nMax.z*m.m32;
        m_nMax.y += box.m_nMin.z*m.m32;
    }
    
    if(m.m33 > 0) {
        m_nMin.z += box.m_nMin.z*m.m33;
        m_nMax.z += box.m_nMax.z*m.m33;
    }
    else {
        m_nMin.z += box.m_nMax.z*m.m33;
        m_nMax.z += box.m_nMin.z*m.m33;
    }
}

cwVector3D cwAABB::closestPoint(const cwPoint3D& p) const
{
    cwVector3D r(p);
    
    if(p.x < m_nMin.x)
        r.x = m_nMin.x;
    else if(p.x > m_nMax.x)
        r.x = m_nMax.x;
    
    if(p.y < m_nMin.y)
        r.y = m_nMin.y;
    else if(p.y > m_nMax.y)
        r.y = m_nMax.y;
    
    if(p.z < m_nMin.z)
        r.z = m_nMin.z;
    else if(p.z > m_nMax.z)
        r.z = m_nMax.z;
    
    return r;
}

float cwAABB::closestPoint(const cwPoint3D& p, cwVector3D& ret) const
{
    ret = p;
    
    if(p.x < m_nMin.x)
        ret.x = m_nMin.x;
    else if(p.x > m_nMax.x)
        ret.x = m_nMax.x;
    
    if(p.y < m_nMin.y)
        ret.y = m_nMin.y;
    else if(p.y > m_nMax.y)
        ret.y = m_nMax.y;
    
    if(p.z < m_nMin.z)
        ret.z = m_nMin.z;
    else if(p.z > m_nMax.z)
        ret.z = m_nMax.z;
    
    return 0;
}

bool cwAABB::intersection(const cwShape& other) const
{
    switch (other.m_eType) {
        case eShapeRay:
            return cwIntersectionRayAABB(static_cast<const cwRay&>(other), *this);
        case eShapePlane:
            return cwIntersectionPlaneAABB(static_cast<const cwPlane&>(other), *this);
        case eShapeCircle:
            return cwIntersectionCircleAABB(static_cast<const cwCircle&>(other), *this);
        case eShapeAABB:
            return cwIntersectionAABBAABB(static_cast<const cwAABB&>(other), *this);
        default:
            return false;
    }
    
    return false;
}

NS_MINI_END









