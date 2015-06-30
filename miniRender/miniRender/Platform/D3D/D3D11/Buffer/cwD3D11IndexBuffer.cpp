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

#include "cwD3D11IndexBuffer.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

cwD3D11IndexBuffer* cwD3D11IndexBuffer::create(
	CWUINT uSize,
	eBufferUsage usage,
	CWUINT uCpuFlag,
	CWUINT miscFlag,
	CWUINT structureByteStride)
{
	cwD3D11IndexBuffer* pBuffer = new cwD3D11IndexBuffer();
	if (pBuffer && pBuffer->init(uSize, usage, eBufferBindIndex, uCpuFlag, miscFlag, structureByteStride)) {
		pBuffer->autorelease();
		return pBuffer;
	}

	CW_SAFE_DELETE(pBuffer);
	return nullptr;
}

cwD3D11IndexBuffer::cwD3D11IndexBuffer()
{
	m_iIndexCnt = 0;
}

cwD3D11IndexBuffer::~cwD3D11IndexBuffer()
{
	ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(m_pDRenderBuffer);
	CW_RELEASE_COM(pD3D11Buffer);
	m_pDRenderBuffer = nullptr;
}

bool cwD3D11IndexBuffer::init(
	CWUINT uSize,
	eBufferUsage usage,
	CWUINT bindFlag,
	CWUINT uCpuFlag,
	CWUINT miscFlag,
	CWUINT structureByteStride)
{
	if (!cwBuffer::init(uSize, usage, bindFlag, uCpuFlag, miscFlag, structureByteStride)) return false;

	m_iIndexCnt = uSize / sizeof(CWUINT);

	return true;
}

void cwD3D11IndexBuffer::set(cwDevice* pDevice)
{
	if (pDevice) {
		pDevice->setIndexBuffer(this);
	}
}

NS_MINIR_END
