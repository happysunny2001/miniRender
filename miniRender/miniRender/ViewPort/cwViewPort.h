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

#ifndef __CW_VIEW_PORT_H__
#define __CW_VIEW_PORT_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Ref/cwRef.h"

NS_MINIR_BEGIN

class cwViewPort : public cwRef
{
public:
	static cwViewPort* create(
		CWFLOAT fTopLeftX, CWFLOAT fTopLeftY, 
		CWFLOAT fWidth, CWFLOAT fHeight, 
		CWFLOAT fMinDepth, CWFLOAT fMaxDepth);

	virtual CWBOOL init(
		CWFLOAT fTopLeftX, CWFLOAT fTopLeftY,
		CWFLOAT fWidth, CWFLOAT fHeight,
		CWFLOAT fMinDepth, CWFLOAT fMaxDepth);

	virtual CWVOID refresh(
		CWFLOAT fTopLeftX, CWFLOAT fTopLeftY,
		CWFLOAT fWidth, CWFLOAT fHeight,
		CWFLOAT fMinDepth, CWFLOAT fMaxDepth);

	virtual CWVOID binding();

	inline CWFLOAT getTopLeftX() const { return m_fTopLeftX; }
	inline CWFLOAT getTopLeftY() const { return m_fTopLeftY; }
	inline CWFLOAT getWidth() const  { return m_fWidth; }
	inline CWFLOAT getHeight() const { return m_fHeight; }
	inline CWFLOAT getMinDepth() const { return m_fMinDepth; }
	inline CWFLOAT getMaxDepth() const { return m_fMaxDepth; }

protected:
	CWFLOAT m_fTopLeftX;
	CWFLOAT m_fTopLeftY;
	CWFLOAT m_fWidth;
	CWFLOAT m_fHeight;
	CWFLOAT m_fMinDepth;
	CWFLOAT m_fMaxDepth;

};

NS_MINIR_END

#endif
