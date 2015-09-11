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

#ifndef __CWVECTOR2D_H__
#define __CWVECTOR2D_H__

#include <math.h>
#include "../Base/cwMacros.h"

NS_MINIR_BEGIN
  
class CW_DLL cwVector2D
{
public:
    cwVector2D() {}
    cwVector2D(float xx, float yy) : x(xx), y(yy) {}
    cwVector2D(const cwVector2D& v) : x(v.x), y(v.y) {}
    
    void zero() {
        x = y = 0;
    }
    
    cwVector2D& operator = (const cwVector2D& v) {
        x = v.x;
        y = v.y;
        return *this;
    }
    
    cwVector2D operator - () const {
        return cwVector2D(-x, -y);
    }
    
    cwVector2D operator + (const cwVector2D& v) const {
        return cwVector2D(x+v.x, y+v.y);
    }
    
    cwVector2D operator - (const cwVector2D& v) const {
        return cwVector2D(x-v.x, y-v.y);
    }
    
    void add(const cwVector2D& v, cwVector2D& ret) const {
        ret.x = x + v.x;
        ret.y = y + v.y;
    }
    
    void sub(const cwVector2D& v, cwVector2D& ret) const {
        ret.x = x - v.x;
        ret.y = y - v.y;
    }
    
    cwVector2D& operator += (const cwVector2D& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    
    cwVector2D& operator -= (const cwVector2D& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    
    cwVector2D operator * (float f) const {
        return cwVector2D(x*f, y*f);
    }
    
    cwVector2D operator * (const cwVector2D& v) const {
        return cwVector2D(x*v.x, y*v.y);
    }
    
    cwVector2D operator / (float f) const {
        float div = 1.0f / f;
        return cwVector2D(x*div, y*div);
    }
    
    cwVector2D& operator *= (float f) {
        x *= f;
        y *= f;
        return *this;
    }
    
    cwVector2D& operator *= (const cwVector2D& v) {
        x *= v.x;
        y *= v.y;
        return *this;
    }
    
    cwVector2D& operator /= (float f) {
        float div = 1.0f / f;
        x *= div;
        y *= div;
        return *this;
    }

	void set(float fx, float fy) {
		x = fx;
		y = fy;
	}
    
    float dot(const cwVector2D& v) const {
        return x*v.x+y*v.y;
    }
    
    float length() const {
        return sqrtf(x*x+y*y);
    }
    
    float squareLength() const {
        return x*x+y*y;
    }
    
    float length(const cwVector2D& v) const {
        return sqrtf((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y));
    }
    
    float squareLength(const cwVector2D& v) const {
        return (x-v.x)*(x-v.x) + (y-v.y)*(y-v.y);
    }
    
    void normalize() {
        float slen = x*x + y*y;
        if(slen > 0) {
            float lenDiv = 1.0f / sqrtf(slen);
            x *= lenDiv;
            y *= lenDiv;
        }
    }
    
public:
    static cwVector2D ZERO;
    
public:
    float x, y;
    
};

inline cwVector2D operator * (float f, const cwVector2D& v)
{
    return cwVector2D(f*v.x, f*v.y);
}

NS_MINIR_END

#endif /* defined(__CWVECTOR2D_H__) */
