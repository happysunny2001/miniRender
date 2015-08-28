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

#ifndef __CW_RENDER_TEXTURE_H__
#define __CW_RENDER_TEXTURE_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Base/cwUtils.h"
#include "Ref/cwRef.h"
#include "Math/cwMath.h"
#include "cwTexture.h"

NS_MINIR_BEGIN

class cwRenderTexture : public cwTexture
{
public:
	virtual CWBOOL init(CWFLOAT fWidth, CWFLOAT fHeight);

	virtual CWVOID beginResize();
	virtual CWBOOL onResize(CWBOOL bForce = false);
	
	virtual CWHANDLE getRenderHandle() const;
	virtual CWHANDLE getHandle() const override;
	virtual CWHANDLE getWritablehandle() const;

	virtual CWVOID binding();
	virtual CWVOID beginDraw(CWBOOL bClearColor = CWTRUE, CWBOOL bClearDepth = CWTRUE, CWBOOL bClearStencil = CWTRUE);
	virtual CWVOID endDraw();

	inline CWVOID setClearColor(const cwVector4D& color) { m_nClearColor = color; }
	inline const cwVector4D& getClearColor() const { return m_nClearColor; }

	inline eRenderTextureType getType() const { return m_eType; }

protected:
	CWFLOAT m_fWidth;
	CWFLOAT m_fHeight;
	cwVector4D m_nClearColor;

	eRenderTextureType m_eType;

};

NS_MINIR_END

#endif
