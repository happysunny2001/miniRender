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

#ifndef __CWMATRIX4X4_H__
#define __CWMATRIX4X4_H__

#include "cwVector3D.h"
#include "cwVector4D.h"
#include "Base/cwMacros.h"

#include <string>

NS_MINIR_BEGIN

class cwPoint4D;
class cwQuaternion;
class cwPlane;

class CW_DLL cwMatrix4X4
{
public:
    cwMatrix4X4();
    
    cwMatrix4X4(float mm11, float mm12, float mm13,
                float mm21, float mm22, float mm23,
                float mm31, float mm32, float mm33);
    
    cwMatrix4X4(float mm11, float mm12, float mm13, float mm14,
                float mm21, float mm22, float mm23, float mm24,
                float mm31, float mm32, float mm33, float mm34,
                float mm41, float mm42, float mm43, float mm44);
    
    cwMatrix4X4(const cwMatrix4X4& mm);

	void set(float mm11, float mm12, float mm13,
			 float mm21, float mm22, float mm23,
			 float mm31, float mm32, float mm33);

	void set(float mm11, float mm12, float mm13, float mm14,
			 float mm21, float mm22, float mm23, float mm24,
			 float mm31, float mm32, float mm33, float mm34,
			 float mm41, float mm42, float mm43, float mm44);
    
    //单位矩阵
    void identity();
    
    //清空平移
    inline void zeroTranslation() { m41 = m42 = m43 = 0; }
    //设置平移
    inline void setTranslation(float x, float y, float z) { m41 = x; m42 = y; m43 = z; }
    inline void setTranslation(const cwVector3D& v) { m41 = v.x; m42 = v.y; m43 = v.z; }
    inline void setTranslation(const cwVector4D& v) { m41 = v.x; m42 = v.y; m43 = v.z; }
    inline cwVector3D getTranslation() const { return cwVector3D(m41, m42, m43); }
    
    //旋转(不影响平移)，参数为弧度值
    //yaw:绕y轴旋转, pitch:绕x轴旋转, roll:绕z轴旋转
    void setRotation(float yaw, float pitch, float roll);
    void setRotation(const cwVector3D& v);
    void setRotation(const cwVector4D& v);
    //绕轴(x,y,z)旋转
    //iAxis: 1:x, 2:y, 3:z
    void setRotation(int iAxis, float fTheta);
    //绕任意轴旋转
    void setRotation(const cwVector3D& n, float fTheta);
    
    //创建 物体-》世界 变换矩阵
    void setLocalToWorld(const cwVector3D& pos, const cwVector3D& rot);
    
    //缩放
    void setScale(float x, float y, float z);
    void setScale(const cwVector3D& v);
    //沿任意轴缩放
    void setScale(const cwVector3D& n, float k);
    
    //切变
    //iAxis: 1:x切变y,z, 2:y切变x,z, 3:z切变x,y
    void setShear(int iAxis, float s, float t);
    
    //正交投影
    //n为投影平面法线，平面必须过原点
    void setOrthoProj(const cwVector3D& n);
    
    //转置矩阵
    cwMatrix4X4 transpose() {
        return cwMatrix4X4(m11, m21, m31, m41,
                           m12, m22, m32, m42,
                           m13, m23, m33, m43,
                           m14, m24, m34, m44);
    }
    
    //计算矩阵行列式
    float determinant() const;
	//逆矩阵是否存在
	bool inverseExist() const;
    //计算逆矩阵
    cwMatrix4X4 inverse() const;
    
    //从四元数创建旋转矩阵
    void fromQuaternion(const cwQuaternion& q);

	//创建相机矩阵
	void lookAt(const cwVector3D& pos, const cwVector3D& up, const cwVector3D& target);
	void lookAt(const cwVector3D& pos, const cwVector3D& right, const cwVector3D& up, const cwVector3D& view);
	//创建投影矩阵
	void perspectiveFov(float fFovY, float fAspect, float fNearZ, float fFarZ);
	void perspective(float fWidth, float fHeight, float fNearZ, float fFarZ);
	void orthoFov(float fFovY, float fAspect, float fNearZ, float fFarZ);
	void ortho(float fWidth, float fHeight, float fNearZ, float fFarZ);
	void ortho(float fLeft, float fRight, float fTop, float fBottom, float fNear, float fFar);
	//创建反射矩阵
	void reflect(const cwPlane& plane);
	//创建阴影矩阵
	//plane:显示阴影平面
	//vLight:光源信息。如果vLight.w=0，则是平行光；如果vLight.w=1，则是点光源
	void shadow(const cwPlane& plane, const cwVector4D& vLight);

	const float* getBuffer() const { return &(m[0][0]); }

	std::string desc() const;
    
public:
    static cwMatrix4X4 identityMatrix;
    
public:
    union {
        struct {
            float m11, m12, m13, m14;
            float m21, m22, m23, m24;
            float m31, m32, m33, m34;
            float m41, m42, m43, m44;
        };
        
        float m[4][4];
    };
};

cwVector3D operator * (const cwVector3D& v, const cwMatrix4X4& m);
cwVector4D operator * (const cwVector4D& v, const cwMatrix4X4& m);
cwPoint4D operator * (const cwPoint4D& v, const cwMatrix4X4& m);
cwMatrix4X4 operator * (const cwMatrix4X4& a, const cwMatrix4X4& b);

cwVector3D& operator *= (cwVector3D& v, const cwMatrix4X4& m);
cwVector4D& operator *= (cwVector4D& v, const cwMatrix4X4& m);
cwPoint4D& operator *= (cwPoint4D& v, const cwMatrix4X4& m);
cwMatrix4X4& operator *= (cwMatrix4X4& a, const cwMatrix4X4& b);

NS_MINIR_END

#endif /* defined(__CWMATRIX4X4_H__) */
