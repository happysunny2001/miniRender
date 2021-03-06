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

#ifndef __CW_MIRROR_H__
#define __CW_MIRROR_H__

#include "Base/cwMacros.h"
#include "Math/cwMath.h"
#include "cwEntity.h"

NS_MINIR_BEGIN

class cwBlend;
class cwStencil;
class cwStage;

class CW_DLL cwMirror : public cwEntity
{
public:
	static cwMirror* create();

	cwMirror();
	virtual ~cwMirror();

	virtual CWBOOL init() override;
	//virtual CWVOID transform() override;
	virtual CWVOID refreshTransform() override;

	virtual CWVOID setReflectPlane(const cwPlane& plane);
	inline const cwMatrix4X4& getReclectMatrix() const { return m_nMatReflect; }

protected:
	CWVOID updatePlane();

protected:
	cwPlane m_nReflectPlane;
	cwMatrix4X4 m_nMatReflect;

};

NS_MINIR_END

#endif
