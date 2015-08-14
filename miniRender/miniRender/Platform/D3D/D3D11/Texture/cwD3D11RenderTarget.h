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

#ifndef __CW_D3D11_RENDER_TARGET_H_
#define __CW_D3D11_RENDER_TARGET_H_

#ifdef _CW_D3D11_

#include "Base/cwMacros.h"
#include "Texture/cwRenderTexture.h"

NS_MINIR_BEGIN

class cwD3D11RenderTarget : public cwRenderTexture
{
public:
	static cwD3D11RenderTarget* create();

	cwD3D11RenderTarget();
	virtual ~cwD3D11RenderTarget();

	virtual bool init(CWFLOAT fWidth, CWFLOAT fHeight) override;

	virtual void beginResize() override;
	virtual bool onResize(bool bForce=false) override;

	virtual CWHANDLE getRenderTargetPtr() override;
	virtual CWHANDLE getTexturePtr() override;
	virtual CWHANDLE getTextureMultiThreadPtr() override;

	virtual CWVOID binding() override;
	virtual CWVOID beginDraw(CWBOOL bClearColor = CWTRUE, CWBOOL bClearDepth = CWTRUE, CWBOOL bClearStencil = CWTRUE) override;
	virtual CWVOID endDraw() override;

	virtual CWVOID buildDepthStencilBuffer(CWUINT iWidth, CWUINT iHeight);

protected:
	ID3D11RenderTargetView* m_pRenderTarget;

	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;

};

NS_MINIR_END

#endif // end _CW_D3D11_

#endif // end __CW_D3D11_RENDER_TARGET_H_
