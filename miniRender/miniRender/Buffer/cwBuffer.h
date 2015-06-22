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
#include "cwRef.h"

NS_MINI_BEGIN

class cwRenderDevice;

class CW_DLL cwBuffer : public cwRef
{
public:
	static cwBuffer* create(
		CWUINT uSize,
		CW_BUFFER_USAGE usage,
		CWUINT bindFlag,
		CWUINT uCpuFlag,
		CWUINT miscFlag,
		CWUINT structureByteStride);

	cwBuffer();
	virtual ~cwBuffer();

	virtual bool init(
		CWUINT uSize,
		CW_BUFFER_USAGE usage,
		CWUINT bindFlag,
		CWUINT uCpuFlag,
		CWUINT miscFlag,
		CWUINT structureByteStride);

	inline CW_BUFFER_DESC& getBufferDesc() { return m_nBuffDesc; }
	inline CW_BUFFER* getBuffer() { return m_pDRenderBuffer; }
	inline void setBuffer(CW_BUFFER* pBuffer) { m_pDRenderBuffer = pBuffer; }

	virtual void set(cwRenderDevice* pDevice) {}
	CWUINT getSize() { return m_nBuffDesc.ByteWidth; }

protected:
	CW_BUFFER_DESC m_nBuffDesc;
	CW_BUFFER* m_pDRenderBuffer;

};

NS_MINI_END

#endif