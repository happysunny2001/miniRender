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

#ifndef __CW_INDEX_BUFFER_H__
#define __CW_INDEX_BUFFER_H__

#ifdef _CW_D3D11_

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Buffer/cwBuffer.h"

NS_MINIR_BEGIN

class CW_DLL cwD3D11IndexBuffer : public cwBuffer
{
public:
	static cwD3D11IndexBuffer* create(
		CWUINT uSize,
		eBufferUsage usage = eBufferUsageImmutable,
		CWUINT uCpuFlag = 0,
		CWUINT miscFlag = 0,
		CWUINT structureByteStride = 0);

	cwD3D11IndexBuffer();
	virtual ~cwD3D11IndexBuffer();

	virtual bool init(
		CWUINT uSize,
		eBufferUsage usage,
		CWUINT bindFlag,
		CWUINT uCpuFlag,
		CWUINT miscFlag,
		CWUINT structureByteStride) override;

	virtual void set(cwDevice* pDevice);
	
protected:

};

NS_MINIR_END

#endif

#endif