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

#ifndef __CWSHAPE_H__
#define __CWSHAPE_H__

#include "cwVector3D.h"
#include "../Base/cwMacros.h"

NS_MINIR_BEGIN

enum eShapeType
{
    eShapeNone,
    eShapeLine,
    eShapeCircle,
    eShapeRay,
    eShapeAABB,
    eShapePlane,
};

class cwRay;
class cwCircle;
class cwPlane;
class cwAABB;
class cwMatrix4X4;

class CW_DLL cwShape
{
public:
    cwShape();
    virtual ~cwShape();
    
    virtual cwVector3D closestPoint(const cwPoint3D& p) const = 0;
    virtual float closestPoint(const cwPoint3D& p, cwVector3D& ret) const = 0;
    
    virtual int intersection(const cwShape& other) const = 0;
    
public:
    eShapeType m_eType;
    
};

NS_MINIR_END

#endif 
