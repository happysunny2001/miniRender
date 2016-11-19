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

#ifndef __CWVECTOR3D_H__
#define __CWVECTOR3D_H__

#include "cwVector2D.h"
#include "../Base/cwMacros.h"

NS_MINIR_BEGIN

class cwVector4D;

class CW_DLL cwVector3D : public cwVector2D
{
public:
    cwVector3D() {}
    cwVector3D(float xx, float yy, float zz) : cwVector2D(xx, yy), z(zz) {}
    cwVector3D(const cwVector3D& v) : cwVector2D(v.x, v.y), z(v.z) {}
	cwVector3D(const cwVector4D& v);
    
    void zero() {
        x = y = z = 0;
    }
    
    cwVector3D& operator = (const cwVector3D& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

	cwVector3D& operator = (const cwVector4D& v);
    cwVector3D operator - () const {
        return cwVector3D(-x, -y, -z);
    }
    
    cwVector3D operator + (const cwVector3D& v) const {
        return cwVector3D(x+v.x, y+v.y, z+v.z);
    }
    
    cwVector3D operator - (const cwVector3D& v) const {
        return cwVector3D(x-v.x, y-v.y, z-v.z);
    }
    
    void add(const cwVector3D& v, cwVector3D& ret) const {
        ret.x = x + v.x;
        ret.y = y + v.y;
        ret.z = z + v.z;
    }
    
    void sub(const cwVector3D& v, cwVector3D& ret) const {
        ret.x = x - v.x;
        ret.y = y - v.y;
        ret.z = z - v.z;
    }
    
    cwVector3D& operator += (const cwVector3D& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    
    cwVector3D& operator -= (const cwVector3D& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    
    cwVector3D operator * (float f) const {
        return cwVector3D(x*f, y*f, z*f);
    }
    
    cwVector3D operator * (const cwVector3D& v) const {
        return cwVector3D(x*v.x, y*v.y, z*v.z);
    }
    
    cwVector3D operator / (float f) const {
        float div = 1.0f / f;
        return cwVector3D(x*div, y*div, z*div);
    }
    
    cwVector3D& operator *= (float f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }
    
    cwVector3D& operator *= (const cwVector3D& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    
    cwVector3D& operator /= (float f) {
        float div = 1.0f / f;
        x *= div;
        y *= div;
        z *= div;
        return *this;
    }

	void set(float fx, float fy, float fz) {
		x = fx;
		y = fy;
		z = fz;
	}
    
    float dot(const cwVector3D& v) const {
        return x*v.x + y*v.y + z*v.z;
    }
    
    cwVector3D cross(const cwVector3D& v) const {
        return cwVector3D(y*v.z - z*v.y,
                          z*v.x - x*v.z,
                          x*v.y - y*v.x);
    }
    
    float length() const {
        return sqrtf(x*x + y*y + z*z);
    }
    
    float squareLength() const {
        return x*x + y*y + z*z;
    }
    
    float length(const cwVector3D& v) const {
        return sqrtf((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y) + (z-v.z)*(z-v.z));
    }
    
    float squareLength(const cwVector3D& v) const {
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
    const static cwVector3D ZERO;
    
public:
    float z;
    
};

inline cwVector3D operator * (float f, const cwVector3D& v)
{
    return cwVector3D(f*v.x, f*v.y, f*v.z);
}

typedef cwVector3D cwPoint3D;

NS_MINIR_END

#endif /* defined(__CWVECTOR3D_H__) */
