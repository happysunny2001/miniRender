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

#ifndef __CWRAY_H__
#define __CWRAY_H__

//#include "cwShape.h"
#include "cwVector3D.h"
#include "../Base/cwMacros.h"

NS_MINIR_BEGIN

class cwCircle;
class cwPlane;
class cwAABB;

//ray:R = p0 + td  (t [0, ∞) )
//line: L = p0 + td  (t [0, 1.0] )
class CW_DLL cwRay 
{
public:
    cwRay();
    cwRay(const cwPoint3D& nPos, const cwVector3D& v);
    cwRay(const cwPoint3D& nPos, const cwVector3D& v, float t);
    cwRay(const cwRay& r);
    ~cwRay();
    
    //get closest point
    cwVector3D closestPoint(const cwPoint3D& p) const;
    float closestPoint(const cwPoint3D& p, cwVector3D& ret) const;
    
    //intersection test
    //int intersection(const cwShape& other) const;
	int intersection(const cwRay& ray) const;
	int intersection(const cwCircle& circle) const;
	int intersection(const cwPlane& plane) const;
	int intersection(const cwAABB& aabb) const;
    
public:
    cwPoint3D m_nOrigin;
    cwVector3D m_nDir;
    float m_fT;
    
};

NS_MINIR_END

#endif 
