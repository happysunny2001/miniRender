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

#ifndef __CW_CAMERA_H__
#define __CW_CAMERA_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "Ref/cwRef.h"

NS_MINIR_BEGIN

class CW_DLL cwCamera : public cwRef
{
public:
	static cwCamera* create();
	static cwCamera* create(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ);

	cwCamera();
	virtual ~cwCamera();

	virtual CWBOOL init();
	virtual CWBOOL init(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ);

	virtual CWVOID lookAt(const cwVector3D& pos, const cwVector3D& target, const cwVector3D& up);
	virtual CWVOID updateCamera(CWFLOAT fPosX, CWFLOAT fPosY, CWFLOAT fPosZ);
	virtual CWVOID updateProjMatrixFov(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ);
	virtual CWVOID updateProjMatrix(CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fNearZ, CWFLOAT fFarZ);

	virtual CWVOID walk(CWFLOAT fDist);
	virtual CWVOID strafe(CWFLOAT fDist);

	//rotate the camera a radian on y axis
	virtual CWVOID yaw(CWFLOAT fRadian);
	//rotate the camera a radian on x axis
	virtual CWVOID pitch(CWFLOAT fRadian);
	//rotate the camera a radian on z axis
	virtual CWVOID roll(CWFLOAT fRadian);

	inline const cwMatrix4X4& getViewMatrix() const { return m_nViewMatrix; }
	inline const cwMatrix4X4& getProjMatrix() const { return m_nProjMatrix; }
	inline const cwMatrix4X4& getViewProjMatrix() const { return m_nViewProjMatrix; }
	inline const cwVector3D& getPos() const { return m_nPos; }

	inline const CWSTRING& getName() const { return m_strName; }
	inline CWVOID setName(const CWSTRING& strName) { m_strName = strName; }

	inline CWFLOAT getFovY() const { return m_fFovY; }
	inline CWFLOAT getAspect() const { return m_fAspect; }
	inline CWFLOAT getNearZ() const { return m_fNearZ; }
	inline CWFLOAT getFarZ() const { return m_fFarZ; }

	inline const cwFrustum& getFrustum() const { return m_nFrustum; }

protected:
	CWVOID updateViewMatrix();

protected:
	cwMatrix4X4 m_nViewMatrix;
	cwMatrix4X4 m_nProjMatrix;
	cwMatrix4X4 m_nViewProjMatrix;

	cwVector3D m_nPos;
	cwVector3D m_nLook;
	cwVector3D m_nUp;
	cwVector3D m_nRight;

	CWFLOAT m_fFovY;
	CWFLOAT m_fAspect;
	CWFLOAT m_fNearZ;
	CWFLOAT m_fFarZ;

	cwFrustum m_nFrustum;

	CWSTRING m_strName;

};

NS_MINIR_END

#endif