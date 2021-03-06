﻿/*
Copyright © 2015-2016 Ziwei Wang

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

#ifndef __CW_D3D11_RT_TEXTURE_H__
#define __CW_D3D11_RT_TEXTURE_H__

#ifdef _CW_D3D11_

#include "Base/cwMacros.h"
#include "Base/cwStruct.h"
#include "cwD3D11Texture.h"

NS_MINIR_BEGIN

class cwD3D11RTTexture : public cwD3D11Texture
{
public:
	static cwD3D11RTTexture* create(CWBOOL bThreadSafe=CWFALSE);
	static cwD3D11RTTexture* create(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWBOOL bShaderUsage = CWFALSE, CWBOOL bThreadSafe = CWFALSE);
	static cwD3D11RTTexture* create(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWUINT iMSAASamples, CWBOOL bShaderUsage = CWFALSE, CWBOOL bThreadSafe = CWFALSE);

	cwD3D11RTTexture();
	virtual ~cwD3D11RTTexture();

	virtual CWHANDLE getRenderHandle() const override;
	virtual std::vector<CWHANDLE>* getRenderHandleArray() const override;
	virtual const CW_TEXTURE_DESC* getTextureDesc() const override;

protected:
	virtual CWBOOL init();
	virtual CWBOOL init(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWBOOL bShaderUsage);
	virtual CWBOOL init(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWUINT iMSAASamples, CWBOOL bShaderUsage);

	CWBOOL buildRenderTargetBackBuffer();
	CWBOOL buildRenderTarget();

protected:
	ID3D11RenderTargetView* m_pRenderTargetView;
	CW_TEXTURE_DESC m_nTextureDesc;

};

NS_MINIR_END

#endif

#endif
