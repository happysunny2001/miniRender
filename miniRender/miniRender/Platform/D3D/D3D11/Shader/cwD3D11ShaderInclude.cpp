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

#include "cwD3D11ShaderInclude.h"
#include "Resource/cwResourceLoader.h"
#include "Platform/cwFileSystem.h"
#include "Repertory/cwRepertory.h"

NS_MINIR_BEGIN

HRESULT __stdcall cwD3D11ShaderInclude::Open(
			D3D10_INCLUDE_TYPE IncludeType, 
			LPCSTR pFileName, 
			LPCVOID pParentData, 
			LPCVOID *ppData, 
			UINT *pBytes)
{
	cwRepertory& repertory = cwRepertory::getInstance();

	CWSTRING strFullFilePath = repertory.getResourceLoader()->getShaderFullPath(pFileName);
	cwData* pData = repertory.getFileSystem()->getFileData(strFullFilePath);
	if (pData && pData->m_pData && pData->m_uSize > 0) {
		CWBYTE* pCopyData = new CWBYTE[pData->m_uSize];
		if (pCopyData) {
			memcpy(pCopyData, pData->m_pData, sizeof(CWBYTE)*pData->m_uSize);
			*ppData = (LPCVOID)pCopyData;
			*pBytes = pData->m_uSize;
		}
		else {
			ppData = NULL;
			*pBytes = 0;
		}

		delete pData;
	}
	else {
		ppData = NULL;
		*pBytes = 0;
	}

	return S_OK;
}

HRESULT __stdcall cwD3D11ShaderInclude::Close(LPCVOID pData)
{
	CWBYTE* pByteData = (CWBYTE*)pData;
	delete[] pByteData;
	return S_OK;
}

NS_MINIR_END

#endif
