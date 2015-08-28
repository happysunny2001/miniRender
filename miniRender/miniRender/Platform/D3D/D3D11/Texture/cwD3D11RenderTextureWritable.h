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

#ifndef __CW_D3D11_RENDER_TEXTURE_WRITABLE_H__
#define __CW_D3D11_RENDER_TEXTURE_WRITABLE_H__

#ifdef _CW_D3D11_

#include "Base/cwMacros.h"
#include "cwD3D11RenderTexture.h"

NS_MINIR_BEGIN

class cwD3D11RenderTextureWritable : public cwD3D11RenderTexture
{
public:
	static cwD3D11RenderTextureWritable* create(CWFLOAT fWidth, CWFLOAT fHeight);

	cwD3D11RenderTextureWritable();
	virtual ~cwD3D11RenderTextureWritable();

	virtual bool init(CWFLOAT fWidth, CWFLOAT fHeight) override;
	virtual bool onResize(bool bForce = false) override;

	virtual CWHANDLE getWritablehandle() const override;

protected:
	ID3D11UnorderedAccessView* m_pUnorderedAccessView;

};

NS_MINIR_END

#endif //_CW_D3D11_

#endif //__CW_D3D11_RENDER_TEXTURE_MULTI_THREAD_H__


