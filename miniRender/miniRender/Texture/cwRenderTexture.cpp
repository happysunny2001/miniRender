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

#include "cwRenderTexture.h"

NS_MINIR_BEGIN

bool cwRenderTexture::init(CWFLOAT fWidth, CWFLOAT fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	return onResize(true);
}

bool cwRenderTexture::onResize(bool bForce)
{
	return true;
}

CWHANDLE cwRenderTexture::getRenderTargetPtr()
{
	return NULL;
}

CWHANDLE cwRenderTexture::getResourcePtr()
{
	return NULL;
}

CWHANDLE cwRenderTexture::getResourceMultiThreadPtr()
{
	return NULL;
}

void cwRenderTexture::beginResize()
{

}

NS_MINIR_END
