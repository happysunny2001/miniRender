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
	static cwBuffer* create(
		CWUINT uSize,
		CWUINT usage,
		CWUINT bindFlag,
		CWUINT uCpuFlag,
		CWUINT miscFlag,
		CWUINT structureByteStride);

	cwBuffer();
	virtual ~cwBuffer();

	virtual bool init(
		CWUINT uSize,
		CWUINT usage,
		CWUINT bindFlag,
		CWUINT uCpuFlag,
		CWUINT miscFlag,
		CWUINT structureByteStride);

	inline CW_BUFFER_DESC& getBufferDesc() { return m_nBuffDesc; }
	inline CWVOID* getBuffer() { return m_pDRenderBuffer; }
	inline void setBuffer(CWVOID* pBuffer) { m_pDRenderBuffer = pBuffer; }

	CWUINT getSize() { return m_nBuffDesc.ByteWidth; }

	inline void setStride(CWUINT uStride) { m_nStride = uStride; }
	inline CWUINT getStride() const { return m_nStride; }
	inline CWUINT getOffset() const { return m_nOffset; }

	const CWUINT getIndexCount() const { return m_iIndexCnt; }

protected:
	CW_BUFFER_DESC m_nBuffDesc;
	CWVOID* m_pDRenderBuffer;

	CWUINT m_nStride;
	CWUINT m_nOffset;
	CWUINT m_iIndexCnt;

};

NS_MINIR_END

#endif