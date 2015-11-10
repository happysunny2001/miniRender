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

#ifndef __CW_MATERIAL_UNIT_MATRIX_H__
#define __CW_MATERIAL_UNIT_MATRIX_H__

#include "Base/cwMacros.h"
#include "Math/cwMath.h"
#include "cwMaterialUnit.h"

NS_MINIR_BEGIN

class cwMaterialUnitMatrix : public cwMaterialUnit
{
public:
	static cwMaterialUnitMatrix* create();

	cwMaterialUnitMatrix();
	virtual ~cwMaterialUnitMatrix();

	virtual CWBOOL init() override;
	virtual CWVOID config(cwEffect* pEffect) override;

	CWVOID move(CWFLOAT x, CWFLOAT y);
	CWVOID move(const cwVector2D& pos);
	CWVOID scale(CWFLOAT x, CWFLOAT y);
	CWVOID scale(const cwVector2D& scale);

protected:
	CWVOID updateMatrix();

protected:
	cwMatrix4X4 m_nMat;
	cwVector2D m_nDiffTextureTrans;
	cwVector2D m_nDiffTextureScale;

};

NS_MINIR_END

#endif
