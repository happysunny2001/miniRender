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

#ifdef _CW_D3D11_

#include "cwInputElementDescManager.h"
#include "Shader/cwShaderConstant.h"

NS_MINIR_BEGIN

cwInputElementDesc* cwInputElementDesc::create(int iElementCnt)
{
	cwInputElementDesc* pDesc = new cwInputElementDesc();
	if (pDesc && pDesc->init(iElementCnt)) {
		pDesc->autorelease();
		return pDesc;
	}

	CW_SAFE_DELETE(pDesc);
	return nullptr;
}

cwInputElementDesc::cwInputElementDesc()
{

}

cwInputElementDesc::~cwInputElementDesc()
{
	CW_SAFE_DELETE_ARRAY(m_pElementList);
	m_iElementCnt = 0;
}

bool cwInputElementDesc::init(int iElementCnt)
{
	m_pElementList = new D3D11_INPUT_ELEMENT_DESC[iElementCnt];
	if (!m_pElementList) return false;
	m_iElementCnt = iElementCnt;

	return true;
}

void cwInputElementDesc::addElementDesc(const CWCHAR* name, DXGI_FORMAT format, CWUINT offset, int index)
{
	m_pElementList[index] = { name, 0, format, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

NS_MINIR_END

#endif
