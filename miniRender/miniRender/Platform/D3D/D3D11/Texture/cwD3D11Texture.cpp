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

#include "cwD3D11Texture.h"
#include "Base/cwStringConvert.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11Texture* cwD3D11Texture::create(const string& strFileName)
{
	cwD3D11Texture* pTexture = new cwD3D11Texture();
	if (pTexture && pTexture->init(strFileName)) {
		pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11Texture::cwD3D11Texture() :
m_pShaderResource(nullptr)
{

}

cwD3D11Texture::~cwD3D11Texture()
{
	CW_RELEASE_COM(m_pShaderResource);
}

bool cwD3D11Texture::init(const string& strFileName)
{
	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	wstring wstrName = cwStringConvert::convertToWideChar(strFileName);

	CW_HR(D3DX11CreateShaderResourceViewFromFile(
		pD3D11Device->getD3D11Device(),
		wstrName.c_str(),
		NULL,
		NULL, 
		&m_pShaderResource, 
		NULL));
	return true;
}

CWHANDLE cwD3D11Texture::getTexturePtr()
{
	return (CWHANDLE)m_pShaderResource;
}

NS_MINIR_END

#endif
