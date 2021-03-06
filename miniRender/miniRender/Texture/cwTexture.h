﻿/*
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

#ifndef __CW_TEXTURE_H__
#define __CW_TEXTURE_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Base/cwStruct.h"
#include "Ref/cwRef.h"

#include <vector>

NS_MINIR_BEGIN

class CW_DLL cwTexture : public cwRef
{
public:
	cwTexture();
	virtual ~cwTexture() {}

	virtual CWHANDLE getRenderHandle() const;
	virtual CWHANDLE getHandle() const = 0;
	virtual CWHANDLE getWritablehandle() const;

	virtual std::vector<CWHANDLE>* getRenderHandleArray() const;

	virtual const CWSTRING& getName() const;
	inline CWVOID setName(const CWSTRING& name) { m_nStrName = name; }

	virtual CWVOID binding();
	virtual CWVOID beginDraw(CWBOOL bClearColor = CWTRUE, CWBOOL bClearDepth = CWTRUE, CWBOOL bClearStencil = CWTRUE);
	virtual CWVOID endDraw();
	virtual CWVOID generateMips();

	//inline eRenderTextureType getType() const { return m_eType; }

	inline CWFLOAT getWidth() const { return m_fWidth; }
	inline CWFLOAT getHeight() const { return m_fHeight; }
	inline eFormat getFormat() const { return m_eTextureFormat; }
	virtual const CW_TEXTURE_DESC* getTextureDesc() const;

protected:
	CWSTRING m_nStrName;
	//eRenderTextureType m_eType;

	CWFLOAT m_fWidth;
	CWFLOAT m_fHeight;
	eFormat m_eTextureFormat;

};

NS_MINIR_END

#endif
