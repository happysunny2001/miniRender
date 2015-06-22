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
#include "cwRef.h"

NS_MINI_BEGIN

class CW_DLL cwCamera : public cwRef
{
public:
	static cwCamera* create();
	static cwCamera* create(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ);

	cwCamera();
	virtual ~cwCamera();

	virtual bool init();
	virtual bool init(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ);

	virtual void updateViewMatrix(CWFLOAT fPosX, CWFLOAT fPosY, CWFLOAT fPosZ);
	virtual void updateProjMatrix(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ);

	const cwMatrix4X4& getViewMatrix() { return m_nViewMatrix; }
	const cwMatrix4X4& getProjMatrix() { return m_nProjMatrix; }
	cwMatrix4X4 getViewProjMatrix() const;

	const cwVector3D& getPos() { return m_nPos; }

protected:
	void storeViewMatrix();

protected:
	cwMatrix4X4 m_nViewMatrix;
	cwMatrix4X4 m_nProjMatrix;

	cwVector3D m_nPos;
	cwVector3D m_nTarget;
	cwVector3D m_nUp;

	CWFLOAT m_fFov;
	CWFLOAT m_fAspect;
	CWFLOAT m_fNearZ;
	CWFLOAT m_fFarZ;

};

NS_MINI_END

#endif