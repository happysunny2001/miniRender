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
	m_nPos    = cwVector3D(0, 0, -1.0f);
	m_nTarget = cwVector3D(0, 0, 0);
	m_nUp     = cwVector3D(0, 1.0f, 0);

	this->updateViewMatrix();
}

cwCamera::~cwCamera()
{

}

bool cwCamera::init()
{
	return true;
}

bool cwCamera::init(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ)
{
	updateProjMatrix(fFov, fAspect, fNearZ, fFarZ);
	return true;
}

void cwCamera::updateViewMatrix()
{
	m_nViewMatrix.lookAt(m_nPos, m_nUp, m_nTarget);
}

cwMatrix4X4 cwCamera::getViewProjMatrix() const
{
	return m_nViewMatrix*m_nProjMatrix;
}

void cwCamera::updateCamera(CWFLOAT fPosX, CWFLOAT fPosY, CWFLOAT fPosZ)
{
	m_nPos = cwVector3D(fPosX, fPosY, fPosZ);
	updateViewMatrix();
}

void cwCamera::updateProjMatrix(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ)
{
	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNearZ = fNearZ;
	m_fFarZ = fFarZ;

	m_nProjMatrix.perspectiveFov(m_fFov, m_fAspect, m_fNearZ, m_fFarZ);
}

NS_MINIR_END