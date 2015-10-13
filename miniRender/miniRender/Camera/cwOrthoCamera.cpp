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

#include "cwOrthoCamera.h"

NS_MINIR_BEGIN

cwOrthoCamera* cwOrthoCamera::create()
{
	cwOrthoCamera* pCamera = new cwOrthoCamera();
	if (pCamera && pCamera->init()) {
		pCamera->autorelease();
		return pCamera;
	}

	CW_SAFE_DELETE(pCamera);
	return nullptr;
}

cwOrthoCamera::cwOrthoCamera()
{

}

CWBOOL cwOrthoCamera::init()
{
	if (!cwCamera::init()) return CWFALSE;

	m_nPos = cwVector3D(0.0f, 0.0f, -10.0f);
	updateViewMatrix();

	return CWTRUE;
}

CWVOID cwOrthoCamera::updateCamera(CWFLOAT fPosX, CWFLOAT fPosY, CWFLOAT fPosZ)
{

}

CWVOID cwOrthoCamera::updateProjMatrixFov(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ)
{
	m_fFovY   = fFov;
	m_fAspect = fAspect;
	m_fNearZ  = fNearZ;
	m_fFarZ   = fFarZ;

	m_nProjMatrix.orthoFov(m_fFovY, m_fAspect, m_fNearZ, m_fFarZ);
	m_nViewProjMatrix = m_nViewMatrix*m_nProjMatrix;

	m_nFrustum.refresh(this);
}

CWVOID cwOrthoCamera::updateProjMatrix(CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fNearZ, CWFLOAT fFarZ)
{
	m_fNearZ  = fNearZ;
	m_fFarZ   = fFarZ;
	m_fAspect = fWidth / fHeight;

	m_nProjMatrix.ortho(fWidth, fHeight, m_fNearZ, m_fFarZ);
	m_nViewProjMatrix = m_nViewMatrix*m_nProjMatrix;

	m_nFrustum.refresh(this);
}

NS_MINIR_END
