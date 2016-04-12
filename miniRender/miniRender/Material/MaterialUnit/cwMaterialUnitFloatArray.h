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

#ifndef __CW_MATERIAL_UNIT_FLOAT_ARRAY_H__
#define __CW_MATERIAL_UNIT_FLOAT_ARRAY_H__

#include "Base/cwMacros.h"
#include "Math/cwMath.h"
#include "cwMaterialUnit.h"

NS_MINIR_BEGIN

class cwMaterialUnitFloatArray : public cwMaterialUnit
{
public:
	static cwMaterialUnitFloatArray* create(const CWFLOAT* pFloats, CWUINT iSize, CWBOOL bThreading=CWFALSE);
	static cwMaterialUnitFloatArray* create(CWUINT iSize, CWBOOL bThreading = CWFALSE);

	cwMaterialUnitFloatArray();
	virtual ~cwMaterialUnitFloatArray();

	virtual CWBOOL init(const CWFLOAT* pFloats, CWUINT iSize);
	virtual CWBOOL init(CWUINT iSize);

	CWVOID setFloatArray(const CWFLOAT* pFloats, CWUINT iSize);
	CWVOID refresh(const CWFLOAT* pFloats);

	virtual CWVOID config(cwEffect* pEffect) override;

protected:
	CWFLOAT* m_pFloats;
	CWUINT m_iSize;

};

NS_MINIR_END

#endif
