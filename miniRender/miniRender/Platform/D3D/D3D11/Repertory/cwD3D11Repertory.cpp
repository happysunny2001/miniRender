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
#include "Shader/cwShaderManager.h"
#include "Layouts/cwLayoutsManager.h"
//#include "cwWinMain.h"

#include <assert.h>

NS_MINI_BEGIN

cwD3D11Repertory::cwD3D11Repertory()
{

}

cwD3D11Repertory::~cwD3D11Repertory()
{

}

void cwD3D11Repertory::initAll()
{
	cwRepertory::initAll();

	CWVOID* phWnd     = this->getPtr(gValueWinHandle);
	CWUINT iWinWidth  = this->getUInt(gValueWinWidth);
	CWUINT iWinHeight = this->getUInt(gValueWinHeight);

	assert(phWnd != nullptr);
	assert(iWinWidth != CW_UINT_MAX);
	assert(iWinHeight != CW_UINT_MAX);

	m_pDevice = new cwD3D11Device(reinterpret_cast<HWND>(phWnd), iWinWidth, iWinHeight);
	assert(m_pDevice != nullptr);
	bool b = m_pDevice->initDevice();
	assert(b);

	m_pShaderManager = cwShaderManager::create();
	assert(m_pShaderManager != nullptr);
	CW_SAFE_RETAIN(m_pShaderManager);

	m_pLayoutManager = new cwLayoutsManager();
	assert(m_pLayoutManager != nullptr);
	m_pLayoutManager->init();
}

void cwD3D11Repertory::refreshWindowTitle(const CWSTRING& strTitle)
{
	//if (!pcTitle) return;

	//CWVOID* phWnd = this->getPtr(gValueWinHandle);
	//if (!phWnd) return;

	//HWND hWnd = reinterpret_cast<HWND>(phWnd);
	//SetWindowText(hWnd, pcTitle);
}

NS_MINI_END
