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

#ifndef __CWQUATERNION_H__
#define __CWQUATERNION_H__

#include "../Base/cwMacros.h"

NS_MINIR_BEGIN

class cwVector3D;
class cwMatrix4X4;

class CW_DLL cwQuaternion
{
public:
    cwQuaternion() : w(1.0f), x(0), y(0), z(0) {}
    cwQuaternion(float ww, float xx, float yy, float zz) : w(ww), x(xx), y(yy), z(zz) {}
    cwQuaternion(const cwQuaternion& q) : w(q.w), x(q.x), y(q.y), z(q.z) {}
    
    void identity() {
        w = 1.0f; x = y = z = 0.0f;
    }
    
    void normalize();
    
    //绕x,y,z轴旋转
    void setRotationX(float fTheta);
    void setRotationY(float fTheta);
    void setRotationZ(float fTheta);
    //绕任意轴n旋转
    void setRotation(const cwVector3D& n, float fTheta);
    
    float length();
    float dot(const cwQuaternion& q) const {
        return w*q.w + x*q.x + y*q.y + z*q.z;
    }
    
    //SLERP
    cwQuaternion slerp(const cwQuaternion& q, float t) const;
    void slerp(const cwQuaternion& q, float t, cwQuaternion& r) const;
    //LERP
    cwQuaternion lerp(const cwQuaternion& q, float t) const;
    void lerp(const cwQuaternion& q, float t, cwQuaternion& r) const;
    
    //返回共轭四元数
    cwQuaternion conjugate() const;
    
    //转化为旋转矩阵
    cwMatrix4X4 convertToMatrix() const;
    void convertToMatrix(cwMatrix4X4& m) const;
    
    //旋转矩阵创建四元数
    void fromMatrix(const cwMatrix4X4& m);
    
    //四元数叉积
    cwQuaternion operator * (const cwQuaternion& q) const;
    cwQuaternion& operator *= (const cwQuaternion& q);
    
public:
    static cwQuaternion identityQuaternion;
    
public:
    float w, x, y, z;
};

NS_MINIR_END

#endif /* defined(__CWQUATERNION_H__) */
