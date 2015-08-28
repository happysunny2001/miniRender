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

#ifndef _CW_BLEND_OPER_H_
#define _CW_BLEND_OPER_H_

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Base/cwStruct.h"
#include "Ref/cwRef.h"

NS_MINIR_BEGIN

class CW_DLL cwBlend : public cwRef
{
public:
	cwBlend();
	virtual ~cwBlend();

	virtual bool init(const cwBlendData& blendData) = 0;

	virtual CWHANDLE getHandle() const { return nullptr; }

	virtual const CWFLOAT* getBlendFactor() { return m_fblendFactor; }
	virtual void setblendFactor(const CWFLOAT* factors);

	virtual const CWUINT getSampleMask() const { return m_uSampleMask; }
	virtual void setSampleMask(CWUINT u) { m_uSampleMask = u; }

protected:
	CWFLOAT m_fblendFactor[4];
	CWUINT m_uSampleMask;

};

NS_MINIR_END

#endif