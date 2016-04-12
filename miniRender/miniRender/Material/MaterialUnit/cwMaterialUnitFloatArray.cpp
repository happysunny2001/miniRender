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

#include "cwMaterialUnitFloatArray.h"
#include "Shader/cwShader.h"
#include "effect/cwEffect.h"

NS_MINIR_BEGIN

cwMaterialUnitFloatArray* cwMaterialUnitFloatArray::create(const CWFLOAT* pFloats, CWUINT iSize, CWBOOL bThreading)
{
	cwMaterialUnitFloatArray* pMatUnit = new cwMaterialUnitFloatArray();
	if (pMatUnit && pMatUnit->init(pFloats, iSize)) {
		if (!bThreading)
			pMatUnit->autorelease();
		return pMatUnit;
	}

	CW_SAFE_DELETE(pMatUnit);
	return nullptr;
}

cwMaterialUnitFloatArray* cwMaterialUnitFloatArray::create(CWUINT iSize, CWBOOL bThreading)
{
	cwMaterialUnitFloatArray* pMatUnit = new cwMaterialUnitFloatArray();
	if (pMatUnit && pMatUnit->init(iSize)) {
		if (!bThreading)
			pMatUnit->autorelease();
		return pMatUnit;
	}

	CW_SAFE_DELETE(pMatUnit);
	return nullptr;
}

cwMaterialUnitFloatArray::cwMaterialUnitFloatArray() : 
m_pFloats(nullptr)
{

}

cwMaterialUnitFloatArray::~cwMaterialUnitFloatArray()
{
	CW_SAFE_DELETE_ARRAY(m_pFloats);
}

CWBOOL cwMaterialUnitFloatArray::init(const CWFLOAT* pFloats, CWUINT iSize)
{
	if (!cwMaterialUnit::init()) return CWFALSE;

	if (pFloats && iSize > 0) {
		m_pFloats = new CWFLOAT[iSize];
		memcpy(m_pFloats, pFloats, sizeof(CWFLOAT)*iSize);
		m_iSize = iSize;
	}

	return CWTRUE;
}

CWBOOL cwMaterialUnitFloatArray::init(CWUINT iSize)
{
	if (!cwMaterialUnit::init()) return CWFALSE;

	if (iSize > 0) {
		m_pFloats = new CWFLOAT[iSize];
		memset(m_pFloats, 0, sizeof(CWFLOAT)*iSize);
		m_iSize = iSize;
	}

	return CWTRUE;
}

CWVOID cwMaterialUnitFloatArray::setFloatArray(const CWFLOAT* pFloats, CWUINT iSize)
{
	if (pFloats && iSize > 0) {
		CW_SAFE_DELETE_ARRAY(m_pFloats);

		m_pFloats = new CWFLOAT[iSize];
		memcpy(m_pFloats, pFloats, sizeof(CWFLOAT)*iSize);
		m_iSize = iSize;
	}
}

CWVOID cwMaterialUnitFloatArray::refresh(const CWFLOAT* pFloats)
{
	if (pFloats && m_pFloats) {
		memcpy(m_pFloats, pFloats, sizeof(CWFLOAT)*m_iSize);
	}
}

CWVOID cwMaterialUnitFloatArray::config(cwEffect* pEffect)
{
	cwShader* pShader = pEffect->getShader();
	if (pShader && m_pFloats && !m_nStrShaderParam.empty()) {
		pShader->setVariableData(m_nStrShaderParam, (CWVOID*)m_pFloats, 0, sizeof(CWFLOAT)*m_iSize);
	}
}

NS_MINIR_END
