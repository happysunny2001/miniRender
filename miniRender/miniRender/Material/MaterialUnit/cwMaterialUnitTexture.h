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

#ifndef __CW_MATERIAL_UNIT_TEXTURE_H__
#define __CW_MATERIAL_UNIT_TEXTURE_H__

#include "Base/cwMacros.h"
#include "cwMaterialUnit.h"

NS_MINIR_BEGIN

class cwTexture;

class cwMaterialUnitTexture : public cwMaterialUnit
{
public:
	static cwMaterialUnitTexture* create(const CWSTRING& strTexture, const CWSTRING& strParamName, CWBOOL bThreading=CWFALSE);
	static cwMaterialUnitTexture* create(cwTexture* pTexture, const CWSTRING& strParamName, CWBOOL bThreading = CWFALSE);
	static cwMaterialUnitTexture* createCube(const CWSTRING& strTexture, const CWSTRING& strParamName, CWBOOL bThreading = CWFALSE);

	cwMaterialUnitTexture();
	virtual ~cwMaterialUnitTexture();

	virtual CWBOOL init(const CWSTRING& strTexture, const CWSTRING& strParamName);
	virtual CWBOOL init(cwTexture* pTexture, const CWSTRING& strParamName);
	virtual CWBOOL initCube(const CWSTRING& strTexture, const CWSTRING& strParamName);

	CWVOID setTexture(cwTexture* pTexture);
	inline cwTexture* getTexture() const { return m_pTexture; }

	virtual CWVOID config(cwEffect* pEffect) override;

protected:
	cwTexture* m_pTexture;

};

NS_MINIR_END

#endif
