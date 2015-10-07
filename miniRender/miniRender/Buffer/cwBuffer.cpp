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

#include "cwBuffer.h"

NS_MINIR_BEGIN

cwBuffer::cwBuffer():
m_iElementCnt(0),
m_nStride(0),
m_nOffset(0),
m_uBufferSize(0)
{

}

cwBuffer::~cwBuffer()
{
	
}

CWBOOL cwBuffer::init(
	CWVOID* pData,
	CWUINT uSize,
	eBufferUsage usage,
	eBufferBindFlag bindFlag,
	eAccessFlag uCpuFlag,
	CWUINT miscFlag,
	CWUINT structureByteStride)
{
	if (structureByteStride > 0)
		m_iElementCnt = uSize / structureByteStride;

	m_uBufferSize = uSize;
	m_nStride = structureByteStride;
	m_nUsage = usage;
	m_nAccessFlag = uCpuFlag;

	return true;
}

CWHANDLE cwBuffer::getHandle()
{
	return NULL;
}

CWHANDLE cwBuffer::getShaderHandle()
{
	return NULL;
}

CWVOID cwBuffer::refresh(CWVOID* pData)
{

}

CWVOID cwBuffer::refresh(CWVOID* pData, CWUINT uSize)
{

}

CWVOID cwBuffer::copyFrom(cwBuffer* pBuffer)
{

}

CWVOID cwBuffer::copyTo(CWVOID* pData)
{

}

NS_MINIR_END