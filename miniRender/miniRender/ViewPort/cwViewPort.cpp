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

#include "cwViewPort.h"

NS_MINIR_BEGIN

cwViewPort* cwViewPort::create(
	CWFLOAT fTopLeftX, CWFLOAT fTopLeftY,
	CWFLOAT fWidth, CWFLOAT fHeight,
	CWFLOAT fMinDepth, CWFLOAT fMaxDepth)
{
	cwViewPort* pViewPort = new cwViewPort();
	if (pViewPort && pViewPort->init(fTopLeftX, fTopLeftY, fWidth, fHeight, fMinDepth, fMaxDepth)) {
		pViewPort->autorelease();
		return pViewPort;
	}

	CW_SAFE_DELETE(pViewPort);
	return nullptr;
}

CWBOOL cwViewPort::init(
	CWFLOAT fTopLeftX, CWFLOAT fTopLeftY,
	CWFLOAT fWidth, CWFLOAT fHeight,
	CWFLOAT fMinDepth, CWFLOAT fMaxDepth)
{
	m_fTopLeftX = fTopLeftX;
	m_fTopLeftY = fTopLeftY;
	m_fWidth  = fWidth;
	m_fHeight = fHeight;
	m_fMinDepth = fMinDepth;
	m_fMaxDepth = fMaxDepth;

	return true;
}

CWVOID cwViewPort::refresh(
	CWFLOAT fTopLeftX, CWFLOAT fTopLeftY,
	CWFLOAT fWidth, CWFLOAT fHeight,
	CWFLOAT fMinDepth, CWFLOAT fMaxDepth)
{
	m_fTopLeftX = fTopLeftX;
	m_fTopLeftY = fTopLeftY;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fMinDepth = fMinDepth;
	m_fMaxDepth = fMaxDepth;
}

CWVOID cwViewPort::binding()
{

}

NS_MINIR_END
