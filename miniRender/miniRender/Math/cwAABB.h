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

#ifndef __CWAABB_H__
#define __CWAABB_H__

#include "cwShape.h"
#include "cwVector3D.h"
#include "cwMatrix4X4.h"
#include "../Base/cwMacros.h"

NS_MINIR_BEGIN

class CW_DLL cwAABB : public cwShape
{
public:
    cwAABB();
    cwAABB(const cwPoint3D& ptMin, const cwPoint3D& ptMax);
    cwAABB(cwPoint3D* p, int n);
    cwAABB(const cwAABB& box);
    virtual ~cwAABB();
    
    cwVector3D corner(int index) const;
    
    void empty();
    void add(const cwPoint3D& p);
    void add(const cwAABB& box);
    void update(cwPoint3D* p, int n);
	void update(const cwAABB& box, const cwMatrix4X4& m);
    
    cwVector3D size() const;
    cwVector3D center() const;
    cwVector3D radius() const;
    
    virtual cwVector3D closestPoint(const cwPoint3D& p) const;
    virtual float closestPoint(const cwPoint3D& p, cwVector3D& ret) const;
    
    virtual bool intersection(const cwShape& other) const;
    
public:
    cwPoint3D m_nMin;
    cwPoint3D m_nMax;
    
};

NS_MINIR_END

#endif /* defined(__testGL31__cwAABB__) */
