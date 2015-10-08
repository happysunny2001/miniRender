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

#include "cwD3D11BufferWritable.h"
#include "Device/cwDevice.h"
#include "Repertory/cwRepertory.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11BufferWritable* cwD3D11BufferWritable::create(
CWVOID* pData,
CWUINT uSize,
eAccessFlag uCpuFlag,
CWUINT structureByteStride,
CWUINT offset,
CWBOOL bAppend)
{
	cwD3D11BufferWritable* pBuffer = new cwD3D11BufferWritable();
	if (pBuffer && pBuffer->init(pData, uSize, eBufferUsageDefault, eBufferBindWritable, uCpuFlag, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, structureByteStride, offset, bAppend)) {
		pBuffer->autorelease();
		return pBuffer;
	}

	CW_SAFE_DELETE(pBuffer);
	return nullptr;
}

cwD3D11BufferWritable::cwD3D11BufferWritable() : 
m_pUnorderedResource(NULL)
{

}

cwD3D11BufferWritable::~cwD3D11BufferWritable()
{
	CW_RELEASE_COM(m_pUnorderedResource);
	m_pUnorderedResource = NULL;
}

CWBOOL cwD3D11BufferWritable::init(
	CWVOID* pData,
	CWUINT uSize,
	eBufferUsage usage,
	eBufferBindFlag bindFlag,
	eAccessFlag uCpuFlag,
	CWUINT miscFlag,
	CWUINT structureByteStride,
	CWUINT offset,
	CWBOOL bAppend)
{
	if (!cwD3D11Buffer::init(pData, uSize, usage, bindFlag, uCpuFlag, miscFlag, structureByteStride, offset)) return CWFALSE;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedDesc;
	unorderedDesc.Format = DXGI_FORMAT_UNKNOWN;
	unorderedDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	unorderedDesc.Buffer.FirstElement = 0;
	if (bAppend)
		unorderedDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
	else
		unorderedDesc.Buffer.Flags = 0;
	unorderedDesc.Buffer.NumElements = m_iElementCnt;

	CW_HR(pD3D11Device->getD3D11Device()->CreateUnorderedAccessView(m_pD3D11Buffer, &unorderedDesc, &m_pUnorderedResource));

	return CWTRUE;
}

CWHANDLE cwD3D11BufferWritable::getShaderHandle()
{
	return m_pUnorderedResource;
}

NS_MINIR_END

#endif
