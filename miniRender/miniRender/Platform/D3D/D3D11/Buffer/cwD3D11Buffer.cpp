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

#include "cwD3D11Buffer.h"
#include "Device/cwDevice.h"
#include "Repertory/cwRepertory.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11Buffer* cwD3D11Buffer::create(
CWVOID* pData,
CWUINT uSize,
eBufferUsage usage,
eBufferBindFlag bindFlag,
eAccessFlag uCpuFlag,
CWUINT miscFlag,
CWUINT structureByteStride)
{
	cwD3D11Buffer* pBuffer = new cwD3D11Buffer();
	if (pBuffer && pBuffer->init(pData, uSize, usage, bindFlag, uCpuFlag, miscFlag, structureByteStride)) {
		pBuffer->autorelease();
		return pBuffer;
	}

	CW_SAFE_DELETE(pBuffer);
	return nullptr;
}

cwD3D11Buffer::cwD3D11Buffer() : 
m_pD3D11Buffer(NULL)
{

}

cwD3D11Buffer::~cwD3D11Buffer()
{
	CW_RELEASE_COM(m_pD3D11Buffer);
	m_pD3D11Buffer = NULL;
}

CWBOOL cwD3D11Buffer::init(
	CWVOID* pData,
	CWUINT uSize,
	eBufferUsage usage,
	eBufferBindFlag bindFlag,
	eAccessFlag uCpuFlag,
	CWUINT miscFlag,
	CWUINT structureByteStride)
{
	if (!cwBuffer::init(pData, uSize, usage, bindFlag, uCpuFlag, miscFlag, structureByteStride)) return CWFALSE;

	CWUINT uD3D11CpuFlag = cwD3D11Device::getAccessFlag(uCpuFlag);
	D3D11_USAGE uD3D11Usage = cwD3D11Device::getBufferUsage(usage);
	CWUINT uBindFlag = cwD3D11Device::getBufferBindFlag(bindFlag);

	D3D11_BUFFER_DESC d3dDesc;
	d3dDesc.BindFlags = uBindFlag;
	d3dDesc.ByteWidth = uSize;
	d3dDesc.CPUAccessFlags = uD3D11CpuFlag;
	d3dDesc.MiscFlags = miscFlag;
	d3dDesc.StructureByteStride = structureByteStride;
	d3dDesc.Usage = uD3D11Usage;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (pData) {
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = pData;
		CW_HR(pD3D11Device->getD3D11Device()->CreateBuffer(&d3dDesc, &subData, &m_pD3D11Buffer));
	}
	else {
		CW_HR(pD3D11Device->getD3D11Device()->CreateBuffer(&d3dDesc, 0, &m_pD3D11Buffer));
	}

	return CWTRUE;
}

CWHANDLE cwD3D11Buffer::getHandle()
{
	return m_pD3D11Buffer;
}

CWVOID cwD3D11Buffer::refresh(CWVOID* pData)
{
	if (!pData) return;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	D3D11_MAPPED_SUBRESOURCE mappedData;
	CW_HR(pD3D11Device->getD3D11DeviceContext()->Map(m_pD3D11Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	memcpy(mappedData.pData, pData, this->getSize());

	pD3D11Device->getD3D11DeviceContext()->Unmap(m_pD3D11Buffer, 0);
}

CWVOID cwD3D11Buffer::copyFrom(cwBuffer* pBuffer)
{
	if (!pBuffer) return;
	if (m_nUsage != eBufferUsageStaging) return;
	if (m_nAccessFlag != eAccessFlagRead) return;
	if (this->getSize() != pBuffer->getSize()) return;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	pD3D11Device->getD3D11DeviceContext()->CopyResource(m_pD3D11Buffer, static_cast<ID3D11Resource*>(pBuffer->getHandle()));
}

CWVOID cwD3D11Buffer::copyTo(CWVOID* pData)
{
	if (!pData) return;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	D3D11_MAPPED_SUBRESOURCE mappedData;
	CW_HR(pD3D11Device->getD3D11DeviceContext()->Map(m_pD3D11Buffer, 0, D3D11_MAP_READ, 0, &mappedData));
	memcpy(pData, mappedData.pData, this->getSize());
	pD3D11Device->getD3D11DeviceContext()->Unmap(m_pD3D11Buffer, 0);
}

NS_MINIR_END

#endif
