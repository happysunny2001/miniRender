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

#ifndef _cwD3D11Device_h_
#define _cwD3D11Device_h_

#ifdef _CW_D3D11_

#include "Base/cwUtils.h"
#include "Device/cwDevice.h"

NS_MINI_BEGIN

class cwLayouts;
class cwShader;
class cwD3D11VertexBuffer;
class cwD3D11IndexBuffer;
class cwBuffer;
class cwMaterial;

class cwD3D11Device : public cwDevice
{
public:
	cwD3D11Device(HWND hWnd, CWUINT width, CWUINT height);
	virtual ~cwD3D11Device();

	virtual bool initDevice();
	virtual void resize(CWUINT width, CWUINT height);
	virtual void resize();

	virtual void setSize(CWUINT width, CWUINT height);
	
	virtual void createRenderTarget();
	virtual void createDepthStencil();
	virtual void createViewPort();
	virtual void createRenderState();

	virtual void beginDraw();
	virtual void endDraw();

	virtual void setInputLayout(cwLayouts* pInputLayout);
	virtual void setPrimitiveTopology(ePrimitiveType topology);
	virtual void setClearColor(const cwVector4D& fvColor);
	virtual void setRenderState(eRenderState e);
	virtual void DrawIndexed(CWUINT indexCnt, CWUINT startIndex, CWINT baseVertex);

 	virtual CWVOID* getDevice() { return m_pD3D11Device; }
 	virtual CWVOID* getDeviceContext() { return m_pD3D11DeviceContext; }

	virtual cwShader* createShader(const string& strFileName) override;
	virtual cwBuffer* createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) override;
	virtual cwBuffer* createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, eBufferUsage usage, CWUINT cpuFlag) override;
	virtual cwBuffer* createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) override;
	virtual cwBlend* createBlend(
		bool bEnable, eBlendFactor srcBlend, eBlendFactor dstBlend, eBlendOp blendOp,
		eBlendFactor srcBlendAlpha, eBlendFactor dstBlendAlpha, eBlendOp blendOpAlpha,
		eColorWriteEnable renderWriteMask);
	virtual cwStencil* createStencil(
		bool bDepthEnable, eDepthWriteMask depthWriteMask, eComparison depthFunc,
		bool bStencilEnable, CWBYTE uReadMask, CWBYTE uWriteMask,
		eStencilOp frontFailOp, eStencilOp frontDepthFailOp, eStencilOp frontPassOp, eComparison frontFunc,
		eStencilOp backFailOp, eStencilOp backDepthFailOp, eStencilOp backPassOp, eComparison backFunc);

	virtual void setVertexBuffer(cwBuffer* pVertexBuffer) override;
	virtual void setIndexBuffer(cwBuffer* pIndexBuffer) override;
	virtual void setBlend(const cwBlend* pBlendOper);
	virtual void setStencil(const cwStencil* pStencil);

	virtual CW_RES_LOCK_DATA lockBuffer(cwBuffer* pBuffer);
	virtual void unlockBuffer(cwBuffer* pBuffer);

	virtual cwTexture* createTexture(const string& strFileName);

	virtual void render(cwRenderObject* pRenderObj, const cwVector3D& worldPos, cwShader* pShader, cwCamera* pCamera) override;
	virtual void render(cwEntity* pEntity, cwCamera* pCamera) override;

	virtual void setEffectWorldTrans(cwShader* pShader, const cwMatrix4X4& trans, cwCamera* pCamera) override;
	virtual void setDiffuseTrans(cwShader* pShader, const cwMatrix4X4& trans) override;
	virtual void draw(cwShader* pShader, const string& strTech, cwRenderObject* pRenderObj) override;

protected:
	ID3D11Device* m_pD3D11Device;
	ID3D11DeviceContext* m_pD3D11DeviceContext;
	IDXGISwapChain* m_pDxgiSwapChain;
	ID3D11RenderTargetView* m_pD3D11RenderTarget;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;
	CWUINT m_uiM4xMsaaQuality;

	ID3D11RasterizerState* m_pSolidRenderState;
	ID3D11RasterizerState* m_pWireRenderState;
	ID3D11RasterizerState* m_pNoCullRenderState;
	ID3D11RasterizerState* m_pCullCWRenderState; // render state for clock wise cull mode

	cwMaterial* m_pMaterialDefault;

};

NS_MINI_END

#endif

#endif