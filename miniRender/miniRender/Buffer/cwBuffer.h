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

#ifndef __CW_BUFFER_H__
#define __CW_BUFFER_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Base/cwStruct.h"
#include "Ref/cwRef.h"

NS_MINIR_BEGIN

class cwDevice;

class CW_DLL cwBuffer : public cwRef
{
public:
	cwBuffer();
	virtual ~cwBuffer();

	virtual CWBOOL init(
		CWVOID* pData,
		CWUINT uSize,
		eBufferUsage usage,
		eBufferBindFlag bindFlag,
		eAccessFlag uCpuFlag,
		CWUINT miscFlag,
		CWUINT structureByteStride,
		CWUINT offset);

	virtual CWHANDLE getHandle();
	virtual CWHANDLE getShaderHandle();

	virtual CWVOID refresh(CWVOID* pData);
	virtual CWVOID refresh(CWVOID* pData, CWUINT uSize);
	virtual CWVOID copyFrom(cwBuffer* pBuffer);
	virtual CWVOID copyTo(CWVOID* pData);
	virtual CWBOOL rebuild(
		CWVOID* pData,
		CWUINT uSize,
		eBufferUsage usage,
		eBufferBindFlag bindFlag,
		eAccessFlag uCpuFlag,
		CWUINT miscFlag,
		CWUINT structureByteStride,
		CWUINT offset);

	inline CWUINT getSize() const { return m_uBufferSize; }
	inline eAccessFlag getAcessFlag() const { return m_nAccessFlag; }
	inline eBufferUsage getUsage() const { return m_nUsage; }
	inline CWUINT getStride() const { return m_nStride; }
	inline CWUINT getOffset() const { return m_nOffset; }
	inline CWVOID setOffset(CWUINT uOffset) { m_nOffset = uOffset; }
	//inline CWUINT getElementCount() const { return m_iElementCnt; }
	
	inline CWVOID setElementCount(CWUINT u) { m_iElementCnt = u; }

protected:
	eAccessFlag m_nAccessFlag;
	eBufferUsage m_nUsage;
	eBufferBindFlag m_nBindingFlag;
	CWUINT m_nStride;
	CWUINT m_nOffset;
	CWUINT m_nMiscFlag;
	CWUINT m_iElementCnt;
	CWUINT m_uBufferSize;

};

NS_MINIR_END

#endif