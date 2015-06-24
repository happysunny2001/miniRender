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

#include "cwD3D11Layouts.h"
#include "Shader/cwShader.h"
#include "cwInputElementDescManager.h"

NS_MINI_BEGIN

cwD3D11Layouts* cwD3D11Layouts::create(cwInputElementDesc* pElementDesc, cwD3D11Shader* pShader)
{
	cwD3D11Layouts* pLayout = new cwD3D11Layouts();
	if (pLayout && pLayout->init(pElementDesc, pShader)) {
		pLayout->autorelease();
		return pLayout;
	}

	CW_SAFE_DELETE(pLayout);
	return NULL;
}

cwD3D11Layouts::cwD3D11Layouts() :
m_pInputLayout(NULL)
{
}

cwD3D11Layouts::~cwD3D11Layouts()
{
	CW_RELEASE_COM(m_pInputLayout);
}

bool cwD3D11Layouts::init(cwInputElementDesc* pElementDesc, cwD3D11Shader* pShader)
{
	if (!pElementDesc || !pShader) return false;
	D3DX11_PASS_DESC passDesc;
	pShader->getPass(0, 0)->GetDesc(&passDesc);

	ID3D11Device* pDevice = NULL;
	pShader->getEffect()->GetDevice(&pDevice);
	CW_HR(pDevice->CreateInputLayout(
		pElementDesc->getElementDesc(),
		pElementDesc->getDescCnt(),
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize,
		&m_pInputLayout));

	return true;
}

NS_MINI_END

#endif


