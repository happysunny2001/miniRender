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

#ifndef __CW_MATERIAL_UNIT_REFLECT_H__
#define __CW_MATERIAL_UNIT_REFLECT_H__

#include "Base/cwMacros.h"
#include "cwMaterialUnit.h"
#include "cwMaterialUnitTexture.h"

NS_MINIR_BEGIN

class cwTexture;

class cwMaterialUnitDynamicReflect : public cwMaterialUnitTexture
{
public:
	static cwMaterialUnitDynamicReflect* create(const CWSTRING& strTexture, CWFLOAT fFactor = 0.5f);
	static cwMaterialUnitDynamicReflect* create();

	cwMaterialUnitDynamicReflect();
	virtual ~cwMaterialUnitDynamicReflect();

	virtual CWBOOL init(const CWSTRING& strTexture, CWFLOAT fFactor);
	virtual CWBOOL init();

	virtual CWVOID config(cwEffect* pEffect) override;

	inline CWVOID setReflectionFactor(CWFLOAT f) { m_fReflectFactor = f; }
	inline CWFLOAT getReflectionFactor() const { return m_fReflectFactor; }

	inline const CWSTRING& getFactorParamName() const { return m_nStrShaderFactorParam; }
	inline CWVOID setFactorParamName(const CWSTRING& strName) { m_nStrShaderFactorParam = strName; }

protected:
	CWFLOAT m_fReflectFactor;
	CWSTRING m_nStrShaderFactorParam;

};

NS_MINIR_END

#endif
