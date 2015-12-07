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
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

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

	virtual CWBOOL initDevice() override;
	virtual CWVOID resize(CWUINT width, CWUINT height) override;
	virtual CWVOID resize() override;
	
	virtual cwViewPort* createViewPort(CWFLOAT fTopLeftX, CWFLOAT fTopLeftY, CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fMinDepth = 0.0f, CWFLOAT fMaxDepth = 1.0f) override;
	virtual CWVOID createRenderState() override;

	virtual CWVOID beginDraw(CWBOOL bClearColor, CWBOOL bClearDepth, CWBOOL bClearStencil) override;
	virtual CWVOID endDraw() override;
	virtual CWVOID swap() override;

	virtual CWVOID setInputLayout(cwLayouts* pInputLayout);
	virtual CWVOID setPrimitiveTopology(ePrimitiveType topology);
	virtual CWVOID setClearColor(const cwVector4D& fvColor);
	virtual CWVOID setRenderState(eRenderState e);

	virtual cwShader* createShader(const string& strFileName) override;
	virtual cwShader* createShaderThreadSafe(const string& strFileName) override;
	virtual cwShader* createShader(const CWCHAR* pcSourceData, CWUINT64 uSize) override;
	virtual cwShader* createShaderThreadSafe(const CWCHAR* pcSourceData, CWUINT64 uSize) override;

	virtual cwBuffer* createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) override;
	virtual cwBuffer* createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, eBufferUsage usage, eAccessFlag cpuFlag) override;
	virtual cwBuffer* createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) override;
	virtual cwBuffer* createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, eBufferUsage usage, eAccessFlag cpuFlag) override;
	virtual cwBuffer* createShaderBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, CWBOOL bWritable = CWFALSE, CWBOOL bAppend = CWFALSE) override;
	virtual cwBuffer* createBufferOutput(CWUINT uStride, CWUINT uCnt) override;
	virtual cwBuffer* createBuffer(CWUINT uCnt, eBufferUsage usage, eBufferBindFlag bindFlag, eAccessFlag uCpuFlag, CWUINT miscFlag, CWUINT uStride) override;

	virtual cwBlend* createBlend(const cwBlendData& blendData) override;
	virtual cwStencil* createStencil(const cwStencilData& stencliData) override;

	virtual cwTexture* createTexture(const CWSTRING& strFileName) override;
	virtual cwTexture* createTextureThreadSafe(const CWSTRING& strFileName) override;
	virtual cwTexture* createTexture(CWVOID* pData, CWUINT64 uSize) override;
	virtual cwTexture* createTextureThreadSafe(CWVOID* pData, CWUINT64 uSize) override;

	virtual cwTexture* createCubeTexture(const CWSTRING& strFileName) override;
	virtual cwTexture* createCubeTextureThreadSafe(const CWSTRING& strFileName) override;
	virtual cwCubeTexture* createCubeTexture(CWUINT iSize) override;
	virtual cwTexture* createCubeTexture(CWVOID* pData, CWUINT64 uSize) override;
	virtual cwTexture* createCubeTextureThreadSafe(CWVOID* pData, CWUINT64 uSize) override;

	virtual cwRenderTexture* createRenderTexture(CWFLOAT fWidth, CWFLOAT fHeight, eRenderTextureType eType = eRenderTextureShader) override;
	virtual cwTexture* createTextureArray(const std::vector<CWSTRING>& vecFiles) override;

	virtual cwBatchEntity* createBatchEntity() override;

	virtual CWVOID setVertexBuffer(cwBuffer* pVertexBuffer) override;
	virtual CWVOID setVertexBuffer(cwBuffer** pBuffers, CWUINT uCnt) override;
	virtual CWVOID setIndexBuffer(cwBuffer* pIndexBuffer) override;
	virtual CWVOID setBlend(const cwBlend* pBlendOper);
	virtual CWVOID setStencil(const cwStencil* pStencil);
	virtual CWVOID setShaderWorldTrans(cwShader* pShader, const cwMatrix4X4& trans, cwCamera* pCamera) override;

	virtual CWVOID clearShaderResource() override;

	virtual CWVOID render(cwRenderObject* pRenderObj, const cwVector3D& worldPos, cwShader* pShader, cwCamera* pCamera) override;
	virtual CWVOID draw(cwShader* pShader, const CWSTRING& strTech, cwRenderObject* pRenderObj) override;
	virtual CWVOID draw(cwShader* pShader, const CWSTRING& strTech, std::vector<cwRenderObject*>& vecRenderObject, CWUINT uCnt) override;
	virtual CWVOID drawGP(cwShader* pShader, const CWSTRING& strTech, cwGPInfo* pGPInfo) override;

public:
	IDXGISwapChain* getSwapChain() { return m_pDxgiSwapChain; }
	ID3D11Device* getD3D11Device() { return m_pD3D11Device; }
	ID3D11DeviceContext* getD3D11DeviceContext() { return m_pD3D11DeviceContext; }
	inline CWUINT getM4xMassQuality() const { return m_uiM4xMsaaQuality; }

protected:
	cwD3D11Device();

	friend class cwD3D11Repertory;

private:
	CWVOID initBlendBaseData();
	CWVOID initStencilBaseData();
	CWVOID initAccessFlagData();
	CWVOID initBufferBindFlagData();
	CWVOID initBufferUsageData();
	CWVOID initPrimitiveTypeData();
	CWVOID initFormatTypeData();
	CWVOID initClassification();

protected:
	ID3D11Device* m_pD3D11Device;
	ID3D11DeviceContext* m_pD3D11DeviceContext;
	IDXGISwapChain* m_pDxgiSwapChain;
#if CW_DEBUG
	ID3D11Debug* m_pD3D11Debug;
#endif
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
	static CWUINT formatType[eFormatMaxCount];
	static D3D11_INPUT_CLASSIFICATION classificationType[eClassificationMaxCount];

	static inline D3D11_BLEND getBlendFactor(eBlendFactor index) { return static_cast<D3D11_BLEND>(blendFactor[index]); }
	static inline D3D11_BLEND_OP getBlendOp(eBlendOp index) { return static_cast<D3D11_BLEND_OP>(blendOp[index]); }

	static inline D3D11_STENCIL_OP getStencilOp(eStencilOp index) { return static_cast<D3D11_STENCIL_OP>(stencilOp[index]); }
	static inline D3D11_COMPARISON_FUNC getComparison(eComparison index) { return static_cast<D3D11_COMPARISON_FUNC>(comparisonType[index]); }
	static inline D3D11_DEPTH_WRITE_MASK getDepthWriteMask(eDepthWriteMask index) { return static_cast<D3D11_DEPTH_WRITE_MASK>(depthWriteMask[index]); }

	static inline CWUINT getAccessFlag(eAccessFlag index) { return accessFlag[index]; }
	static inline D3D11_BIND_FLAG getBufferBindFlag(eBufferBindFlag index) { return static_cast<D3D11_BIND_FLAG>(bufferBindFlag[index]); }
	static inline D3D11_USAGE getBufferUsage(eBufferUsage index) { return static_cast<D3D11_USAGE>(bufferUsage[index]); }

	static inline D3D11_PRIMITIVE_TOPOLOGY getPrimitiveType(ePrimitiveType index) { return static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveType[index]); }
	static inline DXGI_FORMAT getFormatType(eFormat index) { return static_cast<DXGI_FORMAT>(formatType[index]); }
	static D3D11_INPUT_CLASSIFICATION getClassificationType(eClassification index) { return classificationType[index]; }

};

NS_MINIR_END

#endif

#endif