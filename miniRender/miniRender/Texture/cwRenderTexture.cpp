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

#include "cwRenderTexture.h"
#include "Base/cwColor.h"

NS_MINIR_BEGIN

cwRenderTexture::cwRenderTexture() :
m_bResizeable(CWTRUE)
//m_bMultiRenderTarget(CWFALSE)
{

}

CWBOOL cwRenderTexture::init(CWFLOAT fWidth, CWFLOAT fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_nClearColor = cwColor::black;

	return onResize(CWTRUE);
}

CWBOOL cwRenderTexture::onResize(CWBOOL bForce)
{
	return CWTRUE;
}

CWHANDLE cwRenderTexture::getRenderHandle() const
{
	return NULL;
}

CWHANDLE cwRenderTexture::getHandle() const
{
	return NULL;
}

CWHANDLE cwRenderTexture::getWritablehandle() const
{
	return NULL;
}

CWVOID cwRenderTexture::beginResize()
{

}

CWVOID cwRenderTexture::setRenderTargetCount(CWUINT iCount)
{

}

CWVOID cwRenderTexture::setRenderTarget(eFormat format, CWUINT index)
{

}

CWHANDLE cwRenderTexture::getRenderTargetHandle(CWUINT index) const
{
	return NULL;
}

CWHANDLE cwRenderTexture::getShaderResourceHandle(CWUINT index) const
{
	return NULL;
}

cwTexture* cwRenderTexture::getRenderTexture(CWINT index) const
{
	return nullptr;
}

CWVOID cwRenderTexture::enableDepthBufferShaderResource()
{

}

NS_MINIR_END
