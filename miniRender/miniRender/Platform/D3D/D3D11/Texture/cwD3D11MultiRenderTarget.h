/*
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

#ifndef __CW_D3D11_MULTI_RENDER_TARGET_H__
#define __CW_D3D11_MULTI_RENDER_TARGET_H__

#ifdef _CW_D3D11_

#include "Base/cwMacros.h"
#include "Texture/cwRenderTexture.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"
#include "Base/cwUtils.h"
#include "Base/cwVector.h"

NS_MINIR_BEGIN

class cwD3D11Texture;

class cwD3D11MultiRenderTarget : public cwRenderTexture
{
public:
	static cwD3D11MultiRenderTarget* create(CWFLOAT fWidth, CWFLOAT fHeight, CWBOOL bThreading = CWFALSE);

	cwD3D11MultiRenderTarget();
	virtual ~cwD3D11MultiRenderTarget();

	virtual CWBOOL init(CWFLOAT fWidth, CWFLOAT fHeight) override;

	virtual CWVOID beginResize() override;
	virtual CWBOOL onResize(CWBOOL bForce = CWFALSE) override;

	virtual CWVOID binding() override;
	virtual CWVOID beginDraw(CWBOOL bClearColor = CWTRUE, CWBOOL bClearDepth = CWTRUE, CWBOOL bClearStencil = CWTRUE) override;
	virtual CWVOID endDraw() override;

	virtual CWVOID setRenderTargetCount(CWUINT iCount) override;
	virtual CWVOID setRenderTarget(eFormat format, CWUINT index) override;
	virtual CWHANDLE getRenderTargetHandle(CWUINT index) const override;
	virtual CWHANDLE getShaderResourceHandle(CWUINT index) const override;
	virtual cwTexture* getRenderTexture(CWINT index) const override;
	virtual CWVOID enableDepthBufferShaderResource() override;

protected:
	CWVOID buildDepthStencilBuffer();
	CWVOID releaseAll();

protected:
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11ShaderResourceView* m_pDepthShaderResource;
	CWBOOL m_bDepthShaderResource;
	cwD3D11Texture* m_pDepthTexture;

	CWUINT m_iGBufCount;
	ID3D11Texture2D* m_ppRenderTargetTextures[4];
	ID3D11RenderTargetView* m_ppRenderTargets[4];
	ID3D11ShaderResourceView* m_ppShaderResources[4];
	cwVector<cwD3D11Texture*> m_nVecTextures;

};

NS_MINIR_END

#endif

#endif
