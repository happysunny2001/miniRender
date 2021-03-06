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

#include <math.h>
#include <assert.h>

#include "cwQuaternion.h"
#include "cwVector3D.h"
#include "cwMatrix4X4.h"

NS_MINIR_BEGIN

cwQuaternion cwQuaternion::identityQuaternion(1.0f, 0.0f, 0.0f, 0.0f);

void cwQuaternion::normalize()
{
    float len = this->length();
    if(len > 0) {
        float lenDiv = 1.0f / len;
        w *= lenDiv;
        x *= lenDiv;
        y *= lenDiv;
        z *= lenDiv;
    }
    else {
        assert(false);
    }
}

void cwQuaternion::setRotationX(float fTheta)
{
    w = cosf(fTheta*0.5f);
    x = sinf(fTheta*0.5f);
    y = z = 0;
}

void cwQuaternion::setRotationY(float fTheta)
{
    w = cosf(fTheta*0.5f);
    y = sinf(fTheta*0.5f);
    x = z = 0;
}

void cwQuaternion::setRotationZ(float fTheta)
{
    w = cosf(fTheta*0.5f);
    z = sinf(fTheta*0.5f);
    x = y = 0;
}

void cwQuaternion::setRotation(const cwVector3D& n, float fTheta)
{
    float s = sinf(fTheta*0.5f);
    
    w = cosf(fTheta*0.5f);
    x = n.x*s;
    y = n.y*s;
    z = n.z*s;
}

float cwQuaternion::length()
{
    return sqrtf(w*w + x*x + y*y + z*z);
}

cwQuaternion cwQuaternion::conjugate() const
{
    cwQuaternion r;
    
    r.w = w;
    r.x = -x;
    r.y = -y;
    r.z = -z;
    
    return r;
}

cwQuaternion cwQuaternion::operator * (const cwQuaternion& q) const
{
    cwQuaternion r;
    
    r.w = w*q.w - x*q.x - y*q.y - z*q.z;
    r.x = w*q.x + x*q.w + y*q.z - z*q.y;
    r.y = w*q.y + y*q.w + z*q.x - x*q.z;
    r.z = w*q.z + z*q.w + x*q.y - y*q.x;
    
    return r;
}

cwQuaternion& cwQuaternion::operator *= (const cwQuaternion& q)
{
    *this = *this * q;
    return *this;
}

cwQuaternion cwQuaternion::slerp(const cwQuaternion& q, float t) const
{
    cwQuaternion r;
    this->slerp(q, t, r);
    return r;
}

void cwQuaternion::slerp(const cwQuaternion& q, float t, cwQuaternion& r) const
{
    if(t <= 0) {
        r = *this;
        return;
    }
    
    if(t >= 1.0f) {
        r = q;
        return;
    }
    
    float cosTheta = this->dot(q);
    float qw = q.w;
    float qx = q.x;
    float qy = q.y;
    float qz = q.z;
    float k0, k1;
    
    if(cosTheta < 0.0f) {
        qw = -qw;
        qx = -qx;
        qy = -qy;
        qz = -qz;
        cosTheta = -cosTheta;
    }
    
    //夹角趋于0，use LERP
    if(cosTheta > 0.99999f) {
        k0 = 1.0f - t;
        k1 = t;
    }
    else {
        float sinTheta = sqrtf(1.0f - cosTheta*cosTheta);
        //the angle between this and q
        float theta = atan2f(sinTheta, cosTheta);
        float sinThetaInv = 1.0f / sinTheta;
        
        k0 = sinf((1.0f-t)*theta) * sinThetaInv;
        k1 = sinf(t*theta) * sinThetaInv;
    }
    
    r.w = w*k0 + qw*k1;
    r.x = x*k0 + qx*k1;
    r.y = y*k0 + qy*k1;
    r.z = z*k0 + qz*k1;
}

cwQuaternion cwQuaternion::lerp(const cwQuaternion& q, float t) const
{
    cwQuaternion r;
    this->lerp(q, t, r);
    return r;
}

void cwQuaternion::lerp(const cwQuaternion& q, float t, cwQuaternion& r) const
{
    if(t <= 0) {
        r = *this;
        return;
    }
    
    if(t >= 1.0f) {
        r = q;
        return;
    }
    
    r.w = (1.0f-t)*w + t*q.w;
    r.x = (1.0f-t)*x + t*q.x;
    r.y = (1.0f-t)*y + t*q.y;
    r.z = (1.0f-t)*z + t*q.z;
}

cwMatrix4X4 cwQuaternion::convertToMatrix() const
{
    cwMatrix4X4 m;
    this->convertToMatrix(m);
    return m;
}

/**********************************************************
 | 1-2yy-2zz   2xy+2wz     2xz-2wy   |
 | 2xy-2wz     1-2xx-2zz   2yz+2wx   |
 | 2xz+2wy     2yz-2wx     1-2xx-2yy |
 **********************************************************/
void cwQuaternion::convertToMatrix(cwMatrix4X4& m) const
{
    float w2 = 2.0f*w;
    float x2 = 2.0f*x;
    float y2 = 2.0f*y;
    float z2 = 2.0f*z;
    
    m.m11 = 1.0f - y2*y - z2*z;
    m.m12 = x2*y + w2*z;
    m.m13 = x2*z - w2*y;
    
    m.m21 = x2*y - w2*z;
    m.m22 = 1.0f - x2*x - z2*z;
    m.m23 = y2*z + w2*x;
    
    m.m31 = x2*z + w2*y;
    m.m32 = y2*z - w2*x;
    m.m33 = 1.0f - x2*x - y2*y;
}

/**************************************************************************************
 w = sqrt( m11+m22+m33+1)/2 | x = (m23-m32)/4w | y = (m31-m13)/4w | z = (m12-m21)/4w
 x = sqrt( m11-m22-m33+1)/2 | w = (m23-m32)/4x | y = (m12+m21)/4x | z = (m31+m13)/4x
 y = sqrt(-m11+m22-m33+1)/2 | w = (m31-m13)/4y | x = (m12+m21)/4y | z = (m23+m32)/4y
 z = sqrt(-m11-m22+m33+1)/2 | w = (m12-m21)/4z | x = (m31+m13)/4z | y = (m23+m32)/4z
 **************************************************************************************/
void cwQuaternion::fromMatrix(const cwMatrix4X4& m)
{
    float wSquareRoot =  m.m11 + m.m22 + m.m33;
    float xSquareRoot =  m.m11 - m.m22 - m.m33;
    float ySquareRoot = -m.m11 + m.m22 - m.m33;
    float zSquareRoot = -m.m11 - m.m22 + m.m33;
    
    //find biggest square root
    int iBigIndex = 0;
    float bigSquareRoot = wSquareRoot;
    if(xSquareRoot > bigSquareRoot) {
        bigSquareRoot = xSquareRoot;
        iBigIndex = 1;
    }
    
    if(ySquareRoot > bigSquareRoot) {
        bigSquareRoot = ySquareRoot;
        iBigIndex = 2;
    }
    
    if(zSquareRoot > bigSquareRoot) {
        bigSquareRoot = zSquareRoot;
        iBigIndex = 3;
    }
    
    float bigValue = sqrtf(bigSquareRoot + 1.0f) * 0.5f;
    float invValue = bigValue * 0.25f;
    
    switch (iBigIndex) {
        case 0:
            w = bigValue;
            x = (m.m23 - m.m32) * invValue;
            y = (m.m31 - m.m13) * invValue;
            z = (m.m12 - m.m21) * invValue;
            break;
        case 1:
            w = (m.m23 - m.m32) * invValue;
            x = bigValue;
            y = (m.m12 + m.m21) * invValue;
            z = (m.m31 + m.m13) * invValue;
            break;
        case 2:
            w = (m.m31 - m.m13) * invValue;
            x = (m.m12 + m.m21) * invValue;
            y = bigValue;
            z = (m.m23 + m.m32) * invValue;
            break;
        case 3:
            w = (m.m12 - m.m21) * invValue;
            x = (m.m31 + m.m13) * invValue;
            y = (m.m23 + m.m32) * invValue;
            break;
        default:
            break;
    }
}

NS_MINIR_END














