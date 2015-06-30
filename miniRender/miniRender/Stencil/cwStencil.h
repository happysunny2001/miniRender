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

#ifndef __CW_STENCIL_H__
#define __CW_STENCIL_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Ref/cwRef.h"

NS_MINIR_BEGIN

class CW_DLL cwStencil : public cwRef
{
public:
	cwStencil() :m_uStencilRef(0){}
	virtual ~cwStencil() {}

	virtual bool init(
		bool bDepthEnable, eDepthWriteMask depthWriteMask, eComparison depthFunc,
		bool bStencilEnable, CWBYTE uReadMask, CWBYTE uWriteMask,
		eStencilOp frontFailOp, eStencilOp frontDepthFailOp, eStencilOp frontPassOp, eComparison frontFunc,
		eStencilOp backFailOp, eStencilOp backDepthFailOp, eStencilOp backPassOp, eComparison backFunc) = 0;

	virtual CWHANDLE getStencilHandlePtr() const { return nullptr; }
	
	CWUINT getStencilRef() const { return m_uStencilRef; }
	CWVOID setStencilRef(CWUINT ref) { m_uStencilRef = ref; }

protected:
	CWUINT m_uStencilRef;

};

NS_MINIR_END

#endif
