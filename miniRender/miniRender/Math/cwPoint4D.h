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

#ifndef __CWPOINT4D_H__
#define __CWPOINT4D_H__

#include "cwVector4D.h"
#include "../Base/cwMacros.h"

NS_MINI_BEGIN

class CW_DLL cwPoint4D : public cwVector4D
{
public:
    cwPoint4D() {
        w = 1.0f;
    }
    
    cwPoint4D(float xx, float yy, float zz) : cwVector4D(xx, yy, zz) {
        w = 1.0f;
    }
    
    cwPoint4D(const cwPoint4D& v) : cwVector4D(v.x, v.y, v.z) {
		w = v.w;
    }
    
    cwPoint4D(const cwVector4D& v) : cwVector4D(v.x, v.y, v.z) {
		w = v.w;
    }
    
public:
    static cwPoint4D ZERO;
    
};

NS_MINI_END

#endif /* defined(__CWPOINT4D_H__) */
