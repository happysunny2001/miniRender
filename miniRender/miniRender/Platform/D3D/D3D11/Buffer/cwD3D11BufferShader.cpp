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

#include "cwD3D11BufferShader.h"
#include "Device/cwDevice.h"
#include "Repertory/cwRepertory.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11BufferShader* cwD3D11BufferShader::create(
CWVOID* pData,
CWUINT uSize,
eAccessFlag uCpuFlag,
CWUINT structureByteStride)
{
	cwD3D11BufferShader* pBuffer = new cwD3D11BufferShader();
	if (pBuffer && pBuffer->init(pData, uSize, eBufferUsageDefault, eBufferBindShader, uCpuFlag, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, structureByteStride)) {
		pBuffer->autorelease();
		return pBuffer;
	}

	CW_SAFE_DELETE(pBuffer);
	return nullptr;
}

cwD3D11BufferShader::cwD3D11BufferShader():
m_pShaderResource(NULL)
{

}

cwD3D11BufferShader::~cwD3D11BufferShader()
{
	CW_RELEASE_COM(m_pShaderResource);
	m_pShaderResource = NULL;
}

CWBOOL cwD3D11BufferShader::init(
	CWVOID* pData,
	CWUINT uSize,
	eBufferUsage usage,
	eBufferBindFlag bindFlag,
	eAccessFlag uCpuFlag,
	CWUINT miscFlag,
	CWUINT structureByteStride)
{
	if (!cwD3D11Buffer::init(pData, uSize, usage, bindFlag, uCpuFlag, miscFlag, structureByteStride)) return CWFALSE;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	shaderDesc.Format = DXGI_FORMAT_UNKNOWN;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	shaderDesc.BufferEx.FirstElement = 0;
	shaderDesc.BufferEx.Flags = 0;
	shaderDesc.BufferEx.NumElements = m_iElementCnt;

	CW_HR(pD3D11Device->getD3D11Device()->CreateShaderResourceView(m_pD3D11Buffer, &shaderDesc, &m_pShaderResource));

	return CWTRUE;
}

CWHANDLE cwD3D11BufferShader::getShaderHandle()
{
	return m_pShaderResource;
}

NS_MINIR_END

#endif


