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

#ifndef __CW_MATERIAL_UNIT_H__
#define __CW_MATERIAL_UNIT_H__

#include "Base/cwMacros.h"
#include "Ref/cwRef.h"

NS_MINIR_BEGIN

class cwMaterial;
class cwEffect;

class cwMaterialUnit : public cwRef
{
public:
	cwMaterialUnit();
	virtual ~cwMaterialUnit();

	virtual CWBOOL init();

	inline CWVOID setParentMaterial(cwMaterial* pMaterial) { m_pParentMaterial = pMaterial; }
	inline cwMaterial* getParentMaterial() const { return m_pParentMaterial; }

	inline const CWSTRING& getShaderParam() const { return m_nStrShaderParam; }
	inline CWVOID setShaderParam(const CWSTRING& strName) { m_nStrShaderParam = strName; }

	virtual CWVOID config(cwEffect* pEffect) = 0;

protected:
	cwMaterial* m_pParentMaterial;
	CWSTRING m_nStrShaderParam;

};

NS_MINIR_END

#endif // !__CW_MATERIAL_UNIT_H__

