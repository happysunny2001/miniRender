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

#ifndef __CW_ORTHO_CAMERA_H__
#define __CW_ORTHO_CAMERA_H__

#include "Base/cwMacros.h"
#include "cwCamera.h"

NS_MINIR_BEGIN

class cwOrthoCamera : public cwCamera
{
public:
	static cwOrthoCamera* create();

	cwOrthoCamera();

	virtual CWBOOL init() override;

	virtual CWVOID updateCamera(CWFLOAT fPosX, CWFLOAT fPosY, CWFLOAT fPosZ) override;
	virtual CWVOID updateProjMatrixFov(CWFLOAT fFov, CWFLOAT fAspect, CWFLOAT fNearZ, CWFLOAT fFarZ) override;
	virtual CWVOID updateProjMatrix(CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fNearZ, CWFLOAT fFarZ) override;
	virtual CWVOID updateProjMatrix(CWFLOAT fLeft, CWFLOAT fRight, CWFLOAT fTop, CWFLOAT fBottom, CWFLOAT fNearZ, CWFLOAT fFarZ) override;

protected:

};

NS_MINIR_END

#endif
