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

#include "cwD3D11Repertory.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/Shader/cwD3D11ShaderManager.h"
#include "Platform/D3D/D3D11/Layouts/cwD3D11LayoutsManager.h"
#include "Texture/cwTextureManager.h"
#include "Engine/cwEngine.h"
#include "Base/cwValueMap.h"
//#include "cwWinMain.h"

#include <assert.h>

NS_MINIR_BEGIN

cwD3D11Repertory::cwD3D11Repertory()
{

}

cwD3D11Repertory::~cwD3D11Repertory()
{
}

bool cwD3D11Repertory::specialInit()
{
	this->addValue(gValueWinWidth,  cwValueMap(CWUINT(800)));
	this->addValue(gValueWinHeight, cwValueMap(CWUINT(600)));

	m_pDevice = new cwD3D11Device();
	assert(m_pDevice != nullptr);
	bool b = m_pDevice->initDevice();
	assert(b);

	m_pShaderManager = cwD3D11ShaderManager::create();
	CW_SAFE_RETAIN(m_pShaderManager);

	m_pLayoutManager = cwD3D11LayoutsManager::create();
	assert(m_pLayoutManager != nullptr);
	CW_SAFE_RETAIN(m_pLayoutManager);

	return true;
}

void cwD3D11Repertory::refreshWindowTitle(const CWSTRING& strTitle)
{
	//if (!pcTitle) return;

	//CWVOID* phWnd = this->getPtr(gValueWinHandle);
	//if (!phWnd) return;

	//HWND hWnd = reinterpret_cast<HWND>(phWnd);
	//SetWindowText(hWnd, pcTitle);
}

NS_MINIR_END
