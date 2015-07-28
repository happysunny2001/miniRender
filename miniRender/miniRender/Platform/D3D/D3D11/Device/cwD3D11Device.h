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

#ifndef __CW_D3D11_DEVICE_H__
#define __CW_D3D11_DEVICE_H__

#ifdef _CW_D3D11_

#include "Base/cwUtils.h"
#include "Device/cwDevice.h"
#include "Platform/D3D/D3D11/Repertory/cwD3D11Repertory.h"

NS_MINIR_BEGIN

class cwLayouts;
class cwShader;
class cwD3D11VertexBuffer;
class cwD3D11IndexBuffer;
class cwBuffer;
class cwMaterial;

class CW_DLL cwD3D11Device : public cwDevice
{
public:
	virtual ~cwD3D11Device();

	virtual bool initDevice() override;
	virtual void resize(CWUINT width, CWUINT height) override;
	virtual void resize() override;
	
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

	virtual cwBlend* createBlend(const BlendData& blendData) override;
	virtual cwStencil* createStencil(const StencilData& stencliData) override;

	virtual void setVertexBuffer(cwBuffer* pVertexBuffer) override;
	virtual void setIndexBuffer(cwBuffer* pIndexBuffer) override;
	virtual void setBlend(const cwBlend* pBlendOper);
	virtual void setStencil(const cwStencil* pStencil);

	virtual CW_RES_LOCK_DATA lockBuffer(cwBuffer* pBuffer);
	virtual void unlockBuffer(cwBuffer* pBuffer);

	virtual cwTexture* createTexture(const string& strFileName) override;
	virtual cwRenderTexture* createRenderTexture(float fWidth, float fHeight, eRenderTextureType eType = eRenderTextureShader) override;

	virtual void render(cwRenderObject* pRenderObj, const cwVector3D& worldPos, cwShader* pShader, cwCamera* pCamera) override;
	virtual void render(cwEntity* pEntity, cwCamera* pCamera) override;

	virtual void setShaderWorldTrans(cwShader* pShader, const cwMatrix4X4& trans, cwCamera* pCamera) override;
	virtual void setDiffuseTrans(cwShader* pShader, const cwMatrix4X4& trans) override;
	virtual void draw(cwShader* pShader, const string& strTech, cwRenderObject* pRenderObj) override;

public:
	IDXGISwapChain* getSwapChain() { return m_pDxgiSwapChain; }
	ID3D11Device* getD3D11Device() { return m_pD3D11Device; }
	ID3D11DeviceContext* getD3D11DeviceContext() { return m_pD3D11DeviceContext; }
	inline CWUINT getM4xMassQuality() const { return m_uiM4xMsaaQuality; }

protected:
	cwD3D11Device();

	friend class cwD3D11Repertory;

private:
	void initBlendBaseData();
	void initStencilBaseData();
	void initAccessFlagData();
	void initBufferBindFlagData();
	void initBufferUsageData();
	void initPrimitiveTypeData();

protected:
	ID3D11Device* m_pD3D11Device;
	ID3D11DeviceContext* m_pD3D11DeviceContext;
	IDXGISwapChain* m_pDxgiSwapChain;
	CWUINT m_uiM4xMsaaQuality;

	ID3D11RasterizerState* m_pSolidRenderState;
	ID3D11RasterizerState* m_pWireRenderState;
	ID3D11RasterizerState* m_pNoCullRenderState;
	ID3D11RasterizerState* m_pCullCWRenderState; // render state for clock wise cull mode

	cwMaterial* m_pMaterialDefault;

public:
	static CWINT blendFactor[eBlendFactorMaxCount];
	static CWINT blendOp[eBlendOpMaxCount];
	static CWINT stencilOp[eStencilOpMaxCount];
	static CWINT comparisonType[eComparisonMaxCount];
	static CWINT depthWriteMask[eDepthWriteMaskMaxCount];
	static CWUINT accessFlag[eAccessFlagMaxCount];
	static CWUINT bufferBindFlag[eBufferBindMaxCount];
	static CWUINT bufferUsage[eBufferUsageMaxCount];
	static CWUINT primitiveType[ePrimitiveTypeMaxCount];

	static inline D3D11_BLEND getBlendFactor(eBlendFactor index) { return static_cast<D3D11_BLEND>(blendFactor[index]); }
	static inline D3D11_BLEND_OP getBlendOp(eBlendOp index) { return static_cast<D3D11_BLEND_OP>(blendOp[index]); }

	static inline D3D11_STENCIL_OP getStencilOp(eStencilOp index) { return static_cast<D3D11_STENCIL_OP>(stencilOp[index]); }
	static inline D3D11_COMPARISON_FUNC getComparison(eComparison index) { return static_cast<D3D11_COMPARISON_FUNC>(comparisonType[index]); }
	static inline D3D11_DEPTH_WRITE_MASK getDepthWriteMask(eDepthWriteMask index) { return static_cast<D3D11_DEPTH_WRITE_MASK>(depthWriteMask[index]); }

	static inline CWUINT getAccessFlag(eAccessFlag index) { return accessFlag[index]; }
	static inline D3D11_BIND_FLAG getBufferBindFlag(eBufferBindFlag index) { return static_cast<D3D11_BIND_FLAG>(bufferBindFlag[index]); }
	static inline D3D11_USAGE getBufferUsage(eBufferUsage index) { return static_cast<D3D11_USAGE>(bufferUsage[index]); }

	static inline D3D11_PRIMITIVE_TOPOLOGY getPrimitiveType(ePrimitiveType index) { return static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveType[index]); }

};

NS_MINIR_END

#endif

#endif