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

#ifndef __CWCIRCLE_H__
#define __CWCIRCLE_H__

#include "cwShape.h"
#include "cwVector3D.h"
#include "../Base/cwMacros.h"

NS_MINIR_BEGIN

class CW_DLL cwCircle : public cwShape
{
public:
    cwCircle();
    cwCircle(const cwPoint3D& o, float r);
    cwCircle(const cwCircle& c);
    virtual ~cwCircle();
    
    virtual cwVector3D closestPoint(const cwPoint3D& p) const;
    virtual float closestPoint(const cwPoint3D& p, cwVector3D& ret) const;
    
    virtual int intersection(const cwShape& other) const;
    
public:
    cwPoint3D m_nOrigin;
    float m_fR;
    
};

NS_MINIR_END

#endif /* defined(__testGL31__cwCircle__) */
