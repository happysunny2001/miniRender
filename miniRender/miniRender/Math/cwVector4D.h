/*
Copyright © 2015-2016 Ziwei Wang

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

#ifndef __CWVECTOR4D_H__
#define __CWVECTOR4D_H__

#include "cwVector3D.h"
#include "../Base/cwMacros.h"

NS_MINIR_BEGIN

class CW_DLL cwVector4D : public cwVector3D
{
public:
    cwVector4D() : w(0) {}
    cwVector4D(float xx, float yy, float zz) : cwVector3D(xx, yy, zz), w(0) {}
	cwVector4D(float xx, float yy, float zz, float ww) : cwVector3D(xx, yy, zz), w(ww) {}
	cwVector4D(const cwVector3D& v) : cwVector3D(v), w(0) {}
	cwVector4D(const cwVector3D& v, float ww) : cwVector3D(v), w(ww) {}
    cwVector4D(const cwVector4D& v) : cwVector3D(v.x, v.y, v.z), w(v.w) {}
    
    void zero() {
        x = y = z = w = 0;
    }
    
    cwVector4D& operator = (const cwVector4D& v) {
        x = v.x;
        y = v.y;
        z = v.z;
		w = v.w;
        return *this;
    }
    
    cwVector4D operator - () const {
        return cwVector4D(-x, -y, -z);
    }
    
    cwVector4D operator + (const cwVector4D& v) const {
        return cwVector4D(x+v.x, y+v.y, z+v.z);
    }
    
    cwVector4D operator - (const cwVector4D& v) const {
        return cwVector4D(x-v.x, y-v.y, z-v.z);
    }

	void set(float fx, float fy, float fz, float fw) {
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}
    
    void add(const cwVector4D& v, cwVector4D& ret) const {
        ret.x = x + v.x;
        ret.y = y + v.y;
        ret.z = z + v.z;
    }
    
    void sub(const cwVector4D& v, cwVector4D& ret) const {
        ret.x = x - v.x;
        ret.y = y - v.y;
        ret.z = z - v.z;
    }
    
    cwVector4D& operator += (const cwVector4D& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    
    cwVector4D& operator -= (const cwVector4D& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    
    cwVector4D operator * (float f) const {
        return cwVector4D(x*f, y*f, z*f);
    }
    
    cwVector4D operator * (const cwVector4D& v) const {
        return cwVector4D(x*v.x, y*v.y, z*v.z);
    }
    
    cwVector4D operator / (float f) const {
        float div = 1.0f / f;
        return cwVector4D(x*div, y*div, z*div);
    }
    
    cwVector4D& operator *= (float f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }
    
    cwVector4D& operator *= (const cwVector4D& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    
    cwVector4D& operator /= (float f) {
        float div = 1.0f / f;
        x *= div;
        y *= div;
        z *= div;
        return *this;
    }
    
    float dot(const cwVector4D& v) const {
        return x*v.x + y*v.y + z*v.z;
    }
    
    float length() const {
        return sqrtf(x*x + y*y + z*z);
    }
    
    float squareLength() const {
        return x*x + y*y + z*z;
    }
    
    float length(const cwVector4D& v) const {
        return sqrtf((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y) + (z-v.z)*(z-v.z));
    }
    
    float squareLength(const cwVector4D& v) const {
        return (x-v.x)*(x-v.x) + (y-v.y)*(y-v.y) + (z-v.z)*(z-v.z);
    }
    
    void normalize() {
        float slen = x*x + y*y + z*z;
        if(slen > 0) {
            float lenDiv = 1.0f / sqrtf(slen);
            x *= lenDiv;
            y *= lenDiv;
            z *= lenDiv;
        }
    }
    
public:
    const static cwVector4D ZERO;
    
public:
    float w;
    
};

inline cwVector4D operator * (float f, const cwVector4D& v)
{
    return cwVector4D(f*v.x, f*v.y, f*v.z);
}

NS_MINIR_END

#endif /* defined(__CWVECTOR4D_H__) */
