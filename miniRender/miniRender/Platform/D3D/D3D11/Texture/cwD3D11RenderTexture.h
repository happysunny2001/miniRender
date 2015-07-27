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

#ifndef __CW_D3D11_RENDER_TEXTURE_H__
#define __CW_D3D11_RENDER_TEXTURE_H__

#ifdef _CW_D3D11_

#include "Base/cwMacros.h"
#include "Texture/cwRenderTexture.h"

NS_MINIR_BEGIN

class cwD3D11RenderTexture : public cwRenderTexture
{
public:
	static cwD3D11RenderTexture* create(CWUINT iWidth, CWUINT iHeight);

	cwD3D11RenderTexture();
	virtual ~cwD3D11RenderTexture();

	virtual bool init(CWUINT iWidth, CWUINT iHeight) override;
	virtual bool onResize(CWUINT iWidth, CWUINT iHeight) override;

protected:
	ID3D11ShaderResourceView* m_pShaderResource;
	ID3D11RenderTargetView* m_pRenderTarget;

};

NS_MINIR_END

#endif // end _CW_D3D11_

#endif // end __CW_D3D11_RENDER_TEXTURE_H__
