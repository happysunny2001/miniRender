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

#include <string.h>
#include <assert.h>

#include "cwMathUtil.h"
#include "cwMatrix4X4.h"
#include "cwVector3D.h"
#include "cwVector4D.h"
#include "cwPoint4D.h"
#include "cwQuaternion.h"
#include "cwPlane.h"

#include <sstream>
using namespace std;

NS_MINIR_BEGIN

cwMatrix4X4 cwMatrix4X4::identityMatrix(1.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 1.0f);

cwMatrix4X4::cwMatrix4X4()
{
    identity();
}

cwMatrix4X4::cwMatrix4X4(float mm11, float mm12, float mm13,
                         float mm21, float mm22, float mm23,
                         float mm31, float mm32, float mm33):
m11(mm11), m12(mm12), m13(mm13), m14(0),
m21(mm21), m22(mm22), m23(mm23), m24(0),
m31(mm31), m32(mm32), m33(mm33), m34(0),
m41(0), m42(0), m43(0), m44(1.0f)
{
    
}

cwMatrix4X4::cwMatrix4X4(float mm11, float mm12, float mm13, float mm14,
                         float mm21, float mm22, float mm23, float mm24,
                         float mm31, float mm32, float mm33, float mm34,
                         float mm41, float mm42, float mm43, float mm44):
m11(mm11), m12(mm12), m13(mm13), m14(mm14),
m21(mm21), m22(mm22), m23(mm23), m24(mm24),
m31(mm31), m32(mm32), m33(mm33), m34(mm34),
m41(mm41), m42(mm42), m43(mm43), m44(mm44)
{
    
}

cwMatrix4X4::cwMatrix4X4(const cwMatrix4X4& mm)
{
    memcpy(m, mm.m, sizeof(m));
}

void cwMatrix4X4::identity()
{
    m11 = 1.0f; m12 = 0;    m13 = 0;    m14 = 0;
    m21 = 0;    m22 = 1.0f; m23 = 0;    m24 = 0;
    m31 = 0;    m32 = 0;    m33 = 1.0f; m34 = 0;
    m41 = 0;    m42 = 0;    m43 = 0;    m44 = 1.0f;
}

void cwMatrix4X4::setRotation(float yaw, float pitch, float roll)
{
    float ys, yc;
    float ps, pc;
    float rs, rc;
    
	cwMathUtil::cwSinCos(&ys, &yc, yaw);
	cwMathUtil::cwSinCos(&ps, &pc, pitch);
	cwMathUtil::cwSinCos(&rs, &rc, roll);
    
    //heading => yaw, pitch => pitch, bank => roll
    /***********************************************************************************************************************
     | cos(yaw)cos(roll)+sin(yaw)sin(pitch)sin(roll),  sin(roll)cos(pitch), -sin(yaw)cos(roll)+cos(yaw)sin(pitch)sin(roll) |
     |-cos(yaw)sin(roll)+sin(yaw)sin(pitch)cos(roll),  cos(roll)cos(pitch),  sin(roll)sin(yaw)+cos(yaw)sin(pitch)cos(roll) |
     | sin(yaw)cos(pitch)                           , -sin(pitch)         ,  cos(yaw)cos(pitch)                            |
     ***********************************************************************************************************************/
    
    m11 = yc*rc + ys*ps*rs;  m12 = rs*pc; m13 = -ys*rc + yc*ps*rs;
    m21 = -yc*rs + ys*ps*rc; m22 = rc*pc; m23 = rs*ys + yc*ps*rc;
    m31 = ys*pc;             m32 = -ps;   m33 = yc*pc;
}

void cwMatrix4X4::setRotation(const cwVector3D& v)
{
    setRotation(v.y, v.x, v.z);
}

void cwMatrix4X4::setRotation(const cwVector4D& v)
{
    setRotation(v.y, v.x, v.z);
}

void cwMatrix4X4::setRotation(const cwVector3D& n, float fTheta)
{
    float s, c;
	cwMathUtil::cwSinCos(&s, &c, fTheta);
    
    m11 = n.x*n.x*(1-c) + c;
    m12 = n.x*n.y*(1-c) + n.z*s;
    m13 = n.x*n.z*(1-c) - n.y*s;
    
    m21 = n.x*n.y*(1-c) - n.z*s;
    m22 = n.y*n.y*(1-c) + c;
    m23 = n.y*n.z*(1-c) + n.x*s;
    
    m31 = n.x*n.z*(1-c) + n.y*s;
    m32 = n.y*n.z*(1-c) - n.x*s;
    m33 = n.z*n.z*(1-c) + c;
}

void cwMatrix4X4::setLocalToWorld(const cwVector3D& pos, const cwVector3D& rot)
{
    setRotation(rot);
    setTranslation(pos);
}

void cwMatrix4X4::setRotation(int iAxis, float fTheta)
{
    float s, c;
	cwMathUtil::cwSinCos(&s, &c, fTheta);
    
    switch (iAxis) {
        case 1:
            m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
            m21 = 0.0f; m22 = c;    m23 = s;
            m31 = 0.0f; m32 = -s;   m33 = c;
            break;
        case 2:
            m11 = c;    m12 = 0.0f; m13 = -s;
            m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
            m31 = s;    m32 = 0.0f; m33 = c;
            break;
        case 3:
            m11 = c;    m12 = s;    m13 = 0.0f;
            m21 = -s;   m22 = c;    m23 = 0.0f;
            m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
            break;
        default:
            break;
    }
}

void cwMatrix4X4::setScale(float x, float y, float z)
{
    m11 = x;
    m22 = y;
    m33 = z;
}

void cwMatrix4X4::setScale(const cwVector3D& v)
{
    m11 = v.x;
    m22 = v.y;
    m33 = v.z;
}

void cwMatrix4X4::setScale(const cwVector3D& n, float k)
{
    float k1 = k-1.0f;
    float k1x = k1*n.x;
    float k1y = k1*n.y;
    float k1z = k1*n.z;
    
    m11 = 1.0f + k1x*n.x;
    m22 = 1.0f + k1y*n.y;
    m33 = 1.0f + k1z*n.z;
    
    m12 = m21 = k1x*n.y;
    m13 = m31 = k1x*n.z;
    m23 = m32 = k1y*n.z;
}

void cwMatrix4X4::setShear(int iAxis, float s, float t)
{
    switch (iAxis) {
        case 1:
            m11 = 1.0f; m12 = s;    m13 = t;
            m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
            m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
            break;
        case 2:
            m11 = 1.0f; m12 = 1.0f; m13 = 1.0f;
            m21 = s;    m22 = 1.0f; m23 = t;
            m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
            break;
        case 3:
            m11 = 1.0f; m12 = 1.0f; m13 = 1.0f;
            m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
            m31 = s;    m32 = t;    m33 = 1.0f;
            break;
        default:
            break;
    }
}

void cwMatrix4X4::setOrthoProj(const cwVector3D& n)
{
    m11 = 1.0f - n.x*n.x;
    m22 = 1.0f - n.y*n.y;
    m33 = 1.0f - n.z*n.z;
    
    m12 = m21 = -n.x*n.y;
    m13 = m31 = -n.x*n.z;
    m23 = m32 = -n.y*n.z;
}

float cwMatrix4X4::determinant() const
{
    return m11*(m22*m33 - m23*m32) + m12*(m23*m31 - m21*m33) + m13*(m21*m32 - m22*m31);
}

bool cwMatrix4X4::inverseExist() const
{
	if (fabs(this->determinant()) > 0.000001f) return true;
	return false;
}

cwMatrix4X4 cwMatrix4X4::inverse() const
{
	cwMatrix4X4 m;
    float det = this->determinant();
	if (fabs(det) > 0.000001f) {
		float detInv = 1.0f / det;

		m.m11 = (m22*m33 - m23*m32) * detInv;
		m.m12 = (m13*m32 - m12*m33) * detInv;
		m.m13 = (m12*m23 - m13*m22) * detInv;

		m.m21 = (m23*m31 - m21*m33) * detInv;
		m.m22 = (m11*m33 - m13*m31) * detInv;
		m.m23 = (m13*m21 - m11*m23) * detInv;

		m.m31 = (m21*m32 - m22*m31) * detInv;
		m.m32 = (m12*m31 - m11*m32) * detInv;
		m.m33 = (m11*m22 - m12*m21) * detInv;

		m.m41 = -(m41*m.m11 + m42*m.m21 + m43*m.m31);
		m.m42 = -(m41*m.m12 + m42*m.m22 + m43*m.m32);
		m.m43 = -(m41*m.m13 + m42*m.m23 + m43*m.m33);
	}
    
    return m;
}

void cwMatrix4X4::fromQuaternion(const cwQuaternion& q)
{
    q.convertToMatrix(*this);
}

void cwMatrix4X4::lookAt(const cwVector3D& pos, const cwVector3D& up, const cwVector3D& target)
{
	cwVector3D vDir = target - pos;
	vDir.normalize();

	cwVector3D vRight = up.cross(vDir);
	vRight.normalize();

	cwVector3D vUp = vDir.cross(vRight);

	cwMatrix4X4 mRot(
		vRight.x, vUp.x, vDir.x, 0,
		vRight.y, vUp.y, vDir.y, 0,
		vRight.z, vUp.z, vDir.z, 0,
		0, 0, 0, 1.0f);

	cwMatrix4X4 mTran;
	mTran.m41 = -pos.x; mTran.m42 = -pos.y; mTran.m43 = -pos.z;

	*this = mTran*mRot;
}

void cwMatrix4X4::lookAt(const cwVector3D& pos, const cwVector3D& right, const cwVector3D& up, const cwVector3D& view)
{
	cwVector3D trans = -pos;

	m11 = right.x;
	m21 = right.y;
	m31 = right.z;
	m41 = trans.dot(right);

	m12 = up.x;
	m22 = up.y;
	m32 = up.z;
	m42 = trans.dot(up);

	m13 = view.x;
	m23 = view.y;
	m33 = view.z;
	m43 = trans.dot(view);

	m14 = 0;
	m24 = 0;
	m34 = 0;
	m44 = 1.0f;
}

void cwMatrix4X4::perspectiveFov(float fFovY, float fAspect, float fNearZ, float fFarZ)
{
	float yScale = 1.0f / tanf(fFovY*0.5f);
	float xScale = yScale / fAspect;
	float zf = fFarZ / (fFarZ - fNearZ);

	m11 = xScale; m12 = 0;      m13 = 0;          m14 = 0;
	m21 = 0;      m22 = yScale; m23 = 0;          m24 = 0;
	m31 = 0;      m32 = 0;      m33 = zf;         m34 = 1.0f;
	m41 = 0;      m42 = 0;      m43 = -fNearZ*zf; m44 = 0;
}

void cwMatrix4X4::perspective(float fWidth, float fHeight, float fNearZ, float fFarZ)
{
	float zf = fFarZ / (fFarZ - fNearZ);

	m11 = 2.0f*fNearZ/fWidth; m12 = 0;                   m13 = 0;          m14 = 0;
	m21 = 0;                  m22 = 2.0f*fNearZ/fHeight; m23 = 0;          m24 = 0;
	m31 = 0;                  m32 = 0;                   m33 = zf;         m34 = 1.0f;
	m41 = 0;                  m42 = 0;                   m43 = -fNearZ*zf; m44 = 0;
}

void cwMatrix4X4::orthoFov(float fFovY, float fAspect, float fNearZ, float fFarZ)
{
	float h = 2.0f * fNearZ*tanf(fFovY*0.5f);
	float w = h * fAspect;
	float zf = 1.0f / (fFarZ - fNearZ);
	
	m11 = 2.0f / w; m12 = 0;        m13 = 0;          m14 = 0;
	m21 = 0;        m22 = 2.0f / h; m23 = 0;          m24 = 0;
	m31 = 0;        m32 = 0;		m33 = zf;         m34 = 0;
	m41 = 0;        m42 = 0;        m43 = -fNearZ*zf; m44 = 1.0f;
}

void cwMatrix4X4::ortho(float fWidth, float fHeight, float fNearZ, float fFarZ)
{
	float zf = 1.0f / (fFarZ - fNearZ);

	m11 = 2.0f / fWidth; m12 = 0;              m13 = 0;          m14 = 0;
	m21 = 0;             m22 = 2.0f / fHeight; m23 = 0;          m24 = 0;
	m31 = 0;             m32 = 0;		       m33 = zf;         m34 = 0;
	m41 = 0;             m42 = 0;              m43 = -fNearZ*zf; m44 = 1.0f;
}

void cwMatrix4X4::reflect(const cwPlane& plane)
{
	const cwVector3D& n = plane.m_nNormal;
	float d = -plane.m_fD;

	m11 = 1.0f - 2.0f*n.x*n.x; m12 = -2.0f*n.x*n.y;       m13 = -2.0f*n.x*n.z;       m14 = 0;
	m21 = -2.0f*n.x*n.y;       m22 = 1.0f - 2.0f*n.y*n.y; m23 = -2.0f*n.y*n.z;       m24 = 0;
	m31 = -2.0f*n.x*n.z;       m32 = -2.0f*n.y*n.z;       m33 = 1.0f - 2.0f*n.z*n.z; m34 = 0;
	m41 = -2.0f*d*n.x;         m42 = -2.0f*d*n.y;         m43 = -2.0f*d*n.z;         m44 = 1.0f;
}

void cwMatrix4X4::shadow(const cwPlane& plane, const cwVector4D& vLight)
{
	float NL = plane.m_nNormal.dot(vLight);
	const cwVector4D& L = vLight;
	const cwVector3D& N = plane.m_nNormal;
	float d = plane.m_fD;

	m11 = NL + d*L.w - L.x*N.x; m12 = -L.y*N.x;             m13 = -L.z*N.x;             m14 = -L.w*N.x;
	m21 = -L.x*N.y;             m22 = NL + d*L.w - L.y*N.y; m23 = -L.z*N.y;             m24 = -L.w*N.y;
	m31 = -L.x*N.z;             m32 = -L.y*N.z;             m33 = NL + d*L.w - L.z*N.z; m34 = -L.w*N.z;
	m41 = -L.x*d;               m42 = -L.y*d;               m43 = -L.z*d;               m44 = NL;
}

string cwMatrix4X4::desc() const
{
	stringstream ss;
	ss << m11 << "," << m12 << "," << m13 << "," << m14 << "\n";
	ss << m21 << "," << m22 << "," << m23 << "," << m24 << "\n";
	ss << m31 << "," << m32 << "," << m33 << "," << m34 << "\n";
	ss << m41 << "," << m42 << "," << m43 << "," << m44 << "\n";

	return ss.str();
}

cwVector3D operator * (const cwVector3D& v, const cwMatrix4X4& m)
{
    return cwVector3D(v.x*m.m11 + v.y*m.m21 + v.z*m.m31,
                      v.x*m.m12 + v.y*m.m22 + v.z*m.m32,
                      v.x*m.m13 + v.y*m.m23 + v.z*m.m33);
}

cwVector4D operator * (const cwVector4D& v, const cwMatrix4X4& m)
{
    return cwVector4D(v.x*m.m11 + v.y*m.m21 + v.z*m.m31 + v.w*m.m41,
                      v.x*m.m12 + v.y*m.m22 + v.z*m.m32 + v.w*m.m42,
                      v.x*m.m13 + v.y*m.m23 + v.z*m.m33 + v.w*m.m43);
}

cwPoint4D operator * (const cwPoint4D& v, const cwMatrix4X4& m)
{
    return cwPoint4D(v.x*m.m11 + v.y*m.m21 + v.z*m.m31 + v.w*m.m41,
                     v.x*m.m12 + v.y*m.m22 + v.z*m.m32 + v.w*m.m42,
                     v.x*m.m13 + v.y*m.m23 + v.z*m.m33 + v.w*m.m43);
}

cwMatrix4X4 operator * (const cwMatrix4X4& a, const cwMatrix4X4& b)
{
    cwMatrix4X4 r;
    
    r.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31 + a.m14*b.m41;
    r.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32 + a.m14*b.m42;
    r.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33 + a.m14*b.m43;
    r.m14 = a.m11*b.m14 + a.m12*b.m24 + a.m13*b.m34 + a.m14*b.m44;
    
    r.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31 + a.m24*b.m41;
    r.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32 + a.m24*b.m42;
    r.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33 + a.m24*b.m43;
    r.m24 = a.m21*b.m14 + a.m22*b.m24 + a.m23*b.m34 + a.m24*b.m44;
    
    r.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31 + a.m34*b.m41;
    r.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32 + a.m34*b.m42;
    r.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33 + a.m34*b.m43;
    r.m34 = a.m31*b.m14 + a.m32*b.m24 + a.m33*b.m34 + a.m34*b.m44;
    
    r.m41 = a.m41*b.m11 + a.m42*b.m21 + a.m43*b.m31 + a.m44*b.m41;
    r.m42 = a.m41*b.m12 + a.m42*b.m22 + a.m43*b.m32 + a.m44*b.m42;
    r.m43 = a.m41*b.m13 + a.m42*b.m23 + a.m43*b.m33 + a.m44*b.m43;
    r.m44 = a.m41*b.m14 + a.m42*b.m24 + a.m43*b.m34 + a.m44*b.m44;
    
    return r;
}

cwVector3D& operator *= (cwVector3D& v, const cwMatrix4X4& m)
{
    float f[3];
    
    f[0] = v.x*m.m11 + v.y*m.m21 + v.z*m.m31;
    f[1] = v.x*m.m12 + v.y*m.m22 + v.z*m.m32;
    f[2] = v.x*m.m13 + v.y*m.m23 + v.z*m.m33;
    
    v.x = f[0];
    v.y = f[1];
    v.z = f[2];
    
    return v;
}

cwVector4D& operator *= (cwVector4D& v, const cwMatrix4X4& m)
{
    float f[3];
    
    f[0] = v.x*m.m11 + v.y*m.m21 + v.z*m.m31 + v.w*m.m41;
    f[1] = v.x*m.m12 + v.y*m.m22 + v.z*m.m32 + v.w*m.m42;
    f[2] = v.x*m.m13 + v.y*m.m23 + v.z*m.m33 + v.w*m.m43;
    
    v.x = f[0];
    v.y = f[1];
    v.z = f[2];
    
    return v;
}

cwPoint4D& operator *= (cwPoint4D& v, const cwMatrix4X4& m)
{
    float f[3];
    
    f[0] = v.x*m.m11 + v.y*m.m21 + v.z*m.m31 + v.w*m.m41;
    f[1] = v.x*m.m12 + v.y*m.m22 + v.z*m.m32 + v.w*m.m42;
    f[2] = v.x*m.m13 + v.y*m.m23 + v.z*m.m33 + v.w*m.m43;
    
    v.x = f[0];
    v.y = f[1];
    v.z = f[2];
    
    return v;
}

cwMatrix4X4& operator *= (cwMatrix4X4& a, const cwMatrix4X4& b)
{
    a = a * b;
    return a;
}

NS_MINIR_END











