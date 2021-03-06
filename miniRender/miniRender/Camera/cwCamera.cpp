﻿/*
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

#include "cwCamera.h"

NS_MINIR_BEGIN

cwCamera* cwCamera::create()
{
	cwCamera* pCamera = new cwCamera();
	if (pCamera && pCamera->init()) {
		pCamera->autorelease();
		return pCamera;
	}

	CW_SAFE_DELETE(pCamera);
	return nullptr;
}

cwCamera* cwCamera::create(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ)
{
	cwCamera* pCamera = new cwCamera();
	if (pCamera && pCamera->init(fFov, fAspect, fNearZ, fFarZ)) {
		pCamera->autorelease();
		return pCamera;
	}

	CW_SAFE_DELETE(pCamera);
	return nullptr;
}

cwCamera::cwCamera()
{
	m_nPos   = cwVector3D(0, 0, -1.0f);
	m_nLook  = cwVector3D(0, 0, 1.0f);
	m_nUp    = cwVector3D(0, 1.0f, 0);
	m_nRight = cwVector3D(1.0f, 0, 0);

	this->updateViewMatrix();
}

cwCamera::~cwCamera()
{

}

CWBOOL cwCamera::init()
{
	return CWTRUE;
}

CWBOOL cwCamera::init(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ)
{
	updateProjMatrixFov(fFov, fAspect, fNearZ, fFarZ);
	return CWTRUE;
}

CWVOID cwCamera::lookAt(const cwVector3D& pos, const cwVector3D& target, const cwVector3D& up)
{
	m_nLook = target - pos;
	m_nLook.normalize();
	m_nRight = up.cross(m_nLook);
	m_nRight.normalize();
	m_nUp = m_nLook.cross(m_nRight);
	
	m_nPos = pos;

	updateViewMatrix();
}

CWVOID cwCamera::walk(CWFLOAT fDist)
{
	m_nPos += m_nLook*fDist;
	updateViewMatrix();
}

CWVOID cwCamera::strafe(CWFLOAT fDist)
{
	m_nPos += m_nRight*fDist;
	updateViewMatrix();
}

CWVOID cwCamera::setPos(const cwVector3D& pos)
{
	m_nPos = pos;
	updateViewMatrix();
}

CWVOID cwCamera::yaw(CWFLOAT fRadian)
{
	cwMatrix4X4 matRot;
	matRot.setRotation(m_nUp, fRadian);

	m_nRight *= matRot;
	m_nLook *= matRot;

	m_nRight.normalize();
	m_nLook.normalize();

	updateViewMatrix();
}

CWVOID cwCamera::pitch(CWFLOAT fRadian)
{
	cwMatrix4X4 matRot;
	matRot.setRotation(m_nRight, fRadian);

	m_nUp *= matRot;
	m_nLook *= matRot;

	m_nUp.normalize();
	m_nLook.normalize();

	updateViewMatrix();
}

CWVOID cwCamera::roll(CWFLOAT fRadian)
{
	cwMatrix4X4 matRot;
	matRot.setRotation(m_nLook, fRadian);

	m_nUp *= matRot;
	m_nRight *= matRot;

	m_nUp.normalize();
	m_nRight.normalize();

	updateViewMatrix();
}

CWVOID cwCamera::updateViewMatrix()
{
	m_nViewMatrix.lookAt(m_nPos, m_nRight, m_nUp, m_nLook);
	m_nViewProjMatrix = m_nViewMatrix*m_nProjMatrix;

	if (m_nViewProjMatrix.inverseExist()) {
		m_nInvertViewProjMatrix = m_nViewProjMatrix.inverse();
	}

	if (m_nProjMatrix.inverseExist()) {
		m_nInvertProjMatrix = m_nProjMatrix.inverse();
	}

	if (m_nViewMatrix.inverseExist()) {
		m_nInvertViewMatrix = m_nViewMatrix.inverse();
	}

	m_nFrustum.refresh(this);
}

CWVOID cwCamera::updateCamera(CWFLOAT fPosX, CWFLOAT fPosY, CWFLOAT fPosZ)
{
	m_nPos = cwVector3D(fPosX, fPosY, fPosZ);
	updateViewMatrix();
}

CWVOID cwCamera::updateProjMatrixFov(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ)
{
	m_fFovY   = fFov;
	m_fAspect = fAspect;
	m_fNearZ  = fNearZ;
	m_fFarZ   = fFarZ;

	m_nProjMatrix.perspectiveFov(m_fFovY, m_fAspect, m_fNearZ, m_fFarZ);
	m_nViewProjMatrix = m_nViewMatrix*m_nProjMatrix;

	m_nFrustum.refresh(this);
}

CWVOID cwCamera::updateProjMatrix(CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fNearZ, CWFLOAT fFarZ)
{
	m_fNearZ  = fNearZ;
	m_fFarZ   = fFarZ;
	m_fAspect = fWidth / fHeight;

	m_nProjMatrix.perspective(fWidth, fHeight, m_fNearZ, m_fFarZ);
	m_nViewProjMatrix = m_nViewMatrix*m_nProjMatrix;

	m_nFrustum.refresh(this);
}

CWVOID cwCamera::updateProjMatrix(CWFLOAT fLeft, CWFLOAT fRight, CWFLOAT fTop, CWFLOAT fBottom, CWFLOAT fNearZ, CWFLOAT fFarZ)
{
	m_fNearZ = fNearZ;
	m_fFarZ = fFarZ;
	m_fAspect = (fRight-fLeft) / (fTop-fBottom);

	m_nProjMatrix.perspective((fRight - fLeft), (fTop - fBottom), m_fNearZ, m_fFarZ);
	m_nViewProjMatrix = m_nViewMatrix*m_nProjMatrix;

	m_nFrustum.refresh(this);
}

NS_MINIR_END