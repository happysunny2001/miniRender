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

#include "cwD3D11Repertory.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/Shader/cwD3D11ShaderManager.h"
#include "Platform/D3D/D3D11/Layouts/cwD3D11LayoutsManager.h"
#include "Texture/cwTextureManager.h"
#include "Engine/cwEngine.h"
#include "Base/cwValueMap.h"
#include "effect/cwEffectManager.h"
#include "Scheduler/cwSchedulerManager.h"
#include "Generator/cwGeometryGenerator.h"
#include "Event/cwEventManager.h"
#include "Platform/cwFileSystem.h"
#include "Parser/cwParserManager.h"
#include "SpatialSorting/cwSpatialFactory.h"
#include "Resource/cwResourceLoader.h"

#include <assert.h>

NS_MINIR_BEGIN

cwD3D11Repertory::cwD3D11Repertory()
{

}

cwD3D11Repertory::~cwD3D11Repertory()
{

}

CWBOOL cwD3D11Repertory::specialInit()
{
	m_pDevice = new cwD3D11Device();
	assert(m_pDevice != nullptr);
	bool b = m_pDevice->initDevice();
	assert(b);

	m_pShaderManager = cwD3D11ShaderManager::create();
	CW_SAFE_RETAIN(m_pShaderManager);
	m_pShaderManager->loadDefaultShader();

	m_pLayoutManager = cwD3D11LayoutsManager::create();
	assert(m_pLayoutManager != nullptr);
	CW_SAFE_RETAIN(m_pLayoutManager);

	return CWTRUE;
}

CWVOID cwD3D11Repertory::releaseAll()
{
	cwRepertory::releaseAll();

	CW_SAFE_RELEASE_NULL(m_pResourceLoader);
	
	CW_SAFE_RELEASE_NULL(m_pEngine);
	CW_SAFE_RELEASE_NULL(m_pEventManager);
	CW_SAFE_RELEASE_NULL(m_pSchedulerManager);

	CW_SAFE_RELEASE_NULL(m_pEffectManager);
	CW_SAFE_RELEASE_NULL(m_pTextureManager);
	CW_SAFE_RELEASE_NULL(m_pLayoutManager);
	CW_SAFE_RELEASE_NULL(m_pShaderManager);
	CW_SAFE_DELETE(m_pDevice);

	CW_SAFE_RELEASE_NULL(m_pGeoGenerator);
	CW_SAFE_RELEASE_NULL(m_pFileSystem);
	CW_SAFE_RELEASE_NULL(m_pParserManager);
	CW_SAFE_DELETE(m_pSpatialFactory);
}

CWVOID cwD3D11Repertory::refreshWindowTitle(const CWSTRING& strTitle)
{
	//if (!pcTitle) return;

	//CWVOID* phWnd = this->getPtr(gValueWinHandle);
	//if (!phWnd) return;

	//HWND hWnd = reinterpret_cast<HWND>(phWnd);
	//SetWindowText(hWnd, pcTitle);
}

NS_MINIR_END

#endif
