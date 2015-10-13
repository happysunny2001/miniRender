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

#ifdef _CW_D3D11_

#include "cwD3D11Device.h"
#include "Base/cwColor.h"
#include "Base/cwValueMap.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderConstant.h"
#include "Layouts/cwLayouts.h"
#include "Buffer/cwBuffer.h"
#include "RenderObject/cwRenderObject.h"
#include "Camera/cwCamera.h"
#include "Entity/cwEntity.h"
#include "Material/cwMaterial.h"
#include "Texture/cwTexture.h"
#include "Texture/cwRenderTexture.h"
#include "Texture/cwTextureManager.h"
#include "Effect/cwEffect.h"
#include "Stencil/cwStencil.h"
#include "Engine/cwEngine.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"
#include "Platform/D3D/D3D11/Layouts/cwD3D11Layouts.h"
#include "Platform/D3D/D3D11/Buffer/cwD3D11Buffer.h"
#include "Platform/D3D/D3D11/Buffer/cwD3D11BufferShader.h"
#include "Platform/D3D/D3D11/Buffer/cwD3D11BufferWritable.h"
#include "Platform/D3D/D3D11/Stencil/cwD3D11Stencil.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11Texture.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11RenderTarget.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11RenderTexture.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11RenderTextureWritable.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11TextureArray.h"
#include "Platform/D3D/D3D11/Blend/cwD3D11Blend.h"
#include "Platform/D3D/D3D11/Shader/cwD3D11Shader.h"
#include "Platform/D3D/D3D11/ViewPort/cwD3D11ViewPort.h"
#include "Platform/D3D/D3D11/Entity/cwD3D11BatchEntity.h"

#include <assert.h>
#include <xnamath.h>

NS_MINIR_BEGIN

CWINT cwD3D11Device::blendFactor[eBlendFactorMaxCount];
CWINT cwD3D11Device::blendOp[eBlendOpMaxCount];

CWINT cwD3D11Device::stencilOp[eStencilOpMaxCount];
CWINT cwD3D11Device::comparisonType[eComparisonMaxCount];
CWINT cwD3D11Device::depthWriteMask[eDepthWriteMaskMaxCount];

CWUINT cwD3D11Device::accessFlag[eAccessFlagMaxCount];
CWUINT cwD3D11Device::bufferBindFlag[eBufferBindMaxCount];
CWUINT cwD3D11Device::bufferUsage[eBufferUsageMaxCount];

CWUINT cwD3D11Device::primitiveType[ePrimitiveTypeMaxCount];
CWUINT cwD3D11Device::formatType[eFormatMaxCount];
D3D11_INPUT_CLASSIFICATION cwD3D11Device::classificationType[eClassificationMaxCount];

cwD3D11Device::cwD3D11Device() :
m_pD3D11Device(NULL),
m_pD3D11DeviceContext(NULL),
m_pDxgiSwapChain(NULL),
m_pSolidRenderState(NULL),
m_pWireRenderState(NULL),
m_pNoCullRenderState(NULL),
m_pCullCWRenderState(NULL),
#if CW_DEBUG
m_pD3D11Debug(NULL),
#endif
m_pMaterialDefault(nullptr)
{
	initBlendBaseData();
	initStencilBaseData();
	initAccessFlagData();
	initBufferBindFlagData();
	initBufferUsageData();
	initPrimitiveTypeData();
	initFormatTypeData();
	initClassification();
}

cwD3D11Device::~cwD3D11Device()
{
	CW_RELEASE_COM(m_pDxgiSwapChain);
	CW_RELEASE_COM(m_pD3D11Device);
	CW_RELEASE_COM(m_pD3D11DeviceContext);
	CW_RELEASE_COM(m_pSolidRenderState);
	CW_RELEASE_COM(m_pWireRenderState);
	CW_RELEASE_COM(m_pNoCullRenderState);
	CW_RELEASE_COM(m_pCullCWRenderState);
	CW_SAFE_RELEASE_NULL(m_pMaterialDefault);
	CW_SAFE_RELEASE_NULL(m_pRenderTargetBkBuffer);
	CW_SAFE_RELEASE_NULL(m_pCurrRenderTarget);

#if CW_DEBUG
	if (m_pD3D11Debug) {
		m_pD3D11Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		m_pD3D11Debug = NULL;
	}
#endif
}

bool cwD3D11Device::initDevice()
{
	//create d3d11 device and d3d11 device context
	UINT devFlags = 0;
#if CW_DEBUG
	devFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL inFeatureLevel[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = S_OK;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
		D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];

		hr = D3D11CreateDevice(
			NULL,
			driverType,
			NULL,
			devFlags,
			inFeatureLevel,
			3,
			D3D11_SDK_VERSION,
			&m_pD3D11Device,
			&featureLevel,
			&m_pD3D11DeviceContext
			);

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr)) {
		DXTrace(__FILE__, __LINE__, hr, L"D3D11CreateDevice error", true);
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(NULL, L"error create d3d11 device!", L"error", MB_OK);
		return false;
	}

	//check MSAA 4x support
	CW_HR(m_pD3D11Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_uiM4xMsaaQuality));
	assert(m_uiM4xMsaaQuality > 0);

#if CW_DEBUG
	CW_HR(m_pD3D11Device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_pD3D11Debug)));
#endif

	CWUINT winWidth  = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);
	HWND hWnd = static_cast<HWND>(cwRepertory::getInstance().getPtr(gValueWinHandle));

	//create swap buffer
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = winWidth;
	sd.BufferDesc.Height = winHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator   = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	if (m_bEnableMsaa4x) {
		sd.SampleDesc.Quality = m_uiM4xMsaaQuality - 1;
		sd.SampleDesc.Count = 4;
	}
	else {
		sd.SampleDesc.Quality = 0;
		sd.SampleDesc.Count = 1;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = NULL;
	CW_HR(m_pD3D11Device->QueryInterface(__uuidof(IDXGIDevice),  (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = NULL;
	CW_HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = NULL;
	CW_HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	CW_HR(dxgiFactory->CreateSwapChain(m_pD3D11Device, &sd, &m_pDxgiSwapChain));

	CW_RELEASE_COM(dxgiDevice);
	CW_RELEASE_COM(dxgiAdapter);
	CW_RELEASE_COM(dxgiFactory);

	createRenderState();
	createDefaultRenderTarget();
	createDefaultViewPort();
	createDefaultStencil();

	m_pMaterialDefault = cwMaterial::create(
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f));
	CW_SAFE_RETAIN(m_pMaterialDefault);

	return true;
}

void cwD3D11Device::resize(CWUINT width, CWUINT height)
{
	cwRepertory::getInstance().getTextureManager()->beginResize();
	CW_HR(m_pDxgiSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	cwRepertory::getInstance().getTextureManager()->onResize();

	createDefaultViewPort();

	m_bRefreshRenderTarget = true;
}

void cwD3D11Device::resize()
{
	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	resize(winWidth, winHeight);
}

void cwD3D11Device::setClearColor(const cwVector4D& fvColor)
{
	m_fvClearColor = fvColor;
}

cwViewPort* cwD3D11Device::createViewPort(CWFLOAT fTopLeftX, CWFLOAT fTopLeftY, CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fMinDepth, CWFLOAT fMaxDepth)
{
	return cwD3D11ViewPort::create(fTopLeftX, fTopLeftY, fWidth, fHeight, fMinDepth, fMaxDepth);
}

void cwD3D11Device::createRenderState()
{
	D3D11_RASTERIZER_DESC solidDesc;
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
 	solidDesc.FrontCounterClockwise = FALSE;
 	solidDesc.DepthClipEnable = TRUE;
	solidDesc.DepthBias = 0;
	solidDesc.SlopeScaledDepthBias = 0.0f;
	solidDesc.DepthBiasClamp = 0.0f;
	solidDesc.ScissorEnable = FALSE;
	solidDesc.MultisampleEnable = FALSE;
	solidDesc.AntialiasedLineEnable = FALSE;

	CW_HR(m_pD3D11Device->CreateRasterizerState(&solidDesc, &m_pSolidRenderState));

	D3D11_RASTERIZER_DESC wireDesc;
	wireDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireDesc.CullMode = D3D11_CULL_BACK;
	wireDesc.FrontCounterClockwise = FALSE;
	wireDesc.DepthClipEnable = TRUE;
	wireDesc.DepthBias = 0;
	wireDesc.SlopeScaledDepthBias = 0.0f;
	wireDesc.DepthBiasClamp = 0.0f;
	wireDesc.ScissorEnable = FALSE;
	wireDesc.MultisampleEnable = FALSE;
	wireDesc.AntialiasedLineEnable = FALSE;

	CW_HR(m_pD3D11Device->CreateRasterizerState(&wireDesc, &m_pWireRenderState));

	D3D11_RASTERIZER_DESC noCullDesc;
	memset(&noCullDesc, 0, sizeof(noCullDesc));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = FALSE;
	noCullDesc.DepthClipEnable = TRUE;

	CW_HR(m_pD3D11Device->CreateRasterizerState(&noCullDesc, &m_pNoCullRenderState));

	D3D11_RASTERIZER_DESC cwCull;
	memset(&cwCull, 0, sizeof(cwCull));
	cwCull.FillMode = D3D11_FILL_SOLID;
	cwCull.CullMode = D3D11_CULL_BACK;
	cwCull.FrontCounterClockwise = true;
	cwCull.DepthClipEnable = true;

	CW_HR(m_pD3D11Device->CreateRasterizerState(&cwCull, &m_pCullCWRenderState));

	m_pD3D11DeviceContext->RSSetState(m_pSolidRenderState);
	m_eRenderState = eRenderStateSolid;
}

void cwD3D11Device::beginDraw(CWBOOL bClearColor, CWBOOL bClearDepth, CWBOOL bClearStencil)
{
	if (m_bRefreshRenderTarget && m_pCurrRenderTarget) {
		m_pCurrRenderTarget->binding();
	}

	if (m_bRefreshViewPort && m_pCurrViewPort) {
		m_pCurrViewPort->binding();
	}

	m_pCurrRenderTarget->beginDraw(bClearColor, bClearDepth, bClearStencil);
}

void cwD3D11Device::endDraw()
{
	m_pCurrRenderTarget->endDraw();
	this->clearShaderResource();
}

void cwD3D11Device::swap()
{
	CW_HR(m_pDxgiSwapChain->Present(0, 0));
}

void cwD3D11Device::setInputLayout(cwLayouts* pInputLayout)
{
	if (pInputLayout)
		m_pD3D11DeviceContext->IASetInputLayout(static_cast<ID3D11InputLayout*>(pInputLayout->getHandle()));
}

void cwD3D11Device::setPrimitiveTopology(ePrimitiveType topology)
{
	m_pD3D11DeviceContext->IASetPrimitiveTopology(cwD3D11Device::getPrimitiveType(topology));
}

void cwD3D11Device::setRenderState(eRenderState e)
{
	if (m_eRenderState == e) return;
	if (e == eRenderStateSolid)
		m_pD3D11DeviceContext->RSSetState(m_pSolidRenderState);
	else if (e == eRenderStateWire)
		m_pD3D11DeviceContext->RSSetState(m_pWireRenderState);
	else if (e == eRenderStateNoCull)
		m_pD3D11DeviceContext->RSSetState(m_pNoCullRenderState);
	else if (e == eRenderStateCW)
		m_pD3D11DeviceContext->RSSetState(m_pCullCWRenderState);

	m_eRenderState = e;
}

cwShader* cwD3D11Device::createShader(const string& strFileName)
{
	return cwD3D11Shader::create(strFileName);
}

cwBuffer* cwD3D11Device::createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt)
{
	cwD3D11Buffer* pVertexBuffer = cwD3D11Buffer::create(pData, uStride*uCnt, eBufferUsageImmutable, eBufferBindVertex, eAccessFlagNone, 0, uStride);
	if (pVertexBuffer) return pVertexBuffer;

	return nullptr;
}

cwBuffer* cwD3D11Device::createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, eBufferUsage usage, eAccessFlag cpuFlag)
{
	cwD3D11Buffer* pVertexBuffer = cwD3D11Buffer::create(pData, uStride*uCnt, usage, eBufferBindVertex, cpuFlag, 0, uStride);
	if (pVertexBuffer) return pVertexBuffer;

	return nullptr;
}

cwBuffer* cwD3D11Device::createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt)
{
	cwD3D11Buffer* pIndexBuffer = cwD3D11Buffer::create(pData, uStride*uCnt, eBufferUsageImmutable, eBufferBindIndex, eAccessFlagNone, 0, uStride);
	if (pIndexBuffer) return pIndexBuffer;

	return nullptr;
}

cwBuffer* cwD3D11Device::createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, eBufferUsage usage, eAccessFlag cpuFlag)
{
	cwD3D11Buffer* pIndexBuffer = cwD3D11Buffer::create(pData, uStride*uCnt, usage, eBufferBindIndex, cpuFlag, 0, uStride);
	if (pIndexBuffer) return pIndexBuffer;

	return nullptr;
}

cwBuffer* cwD3D11Device::createShaderBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, CWBOOL bWritable, CWBOOL bAppend)
{
	if (!bWritable) {
		cwD3D11BufferShader* pShaderBuffer = cwD3D11BufferShader::create(pData, uStride*uCnt, eAccessFlagNone, uStride);
		if (pShaderBuffer)
			return pShaderBuffer;
	}

	cwD3D11BufferWritable* pShaderBuffer = cwD3D11BufferWritable::create(pData, uStride*uCnt, eAccessFlagNone, uStride, bAppend);
	if (pShaderBuffer)
		return pShaderBuffer;

	return nullptr;
}

cwBuffer* cwD3D11Device::createBufferOutput(CWUINT uStride, CWUINT uCnt)
{
	cwD3D11Buffer* pBuffer = cwD3D11Buffer::create(
		NULL, 
		uStride*uCnt, 
		eBufferUsageStaging, 
		eBufferBindNone, 
		eAccessFlagRead, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, 
		uStride);
	return pBuffer ? pBuffer : nullptr;
}

cwBuffer* cwD3D11Device::createBuffer(CWUINT uCnt, eBufferUsage usage, eBufferBindFlag bindFlag, eAccessFlag uCpuFlag, CWUINT miscFlag, CWUINT uStride)
{
	cwD3D11Buffer* pBuffer = cwD3D11Buffer::create(NULL, uStride*uCnt, usage, bindFlag, uCpuFlag, miscFlag, uStride);
	if (pBuffer) return pBuffer;
	return nullptr;
}

cwBlend* cwD3D11Device::createBlend(const cwBlendData& blendData)
{
	return cwD3D11Blend::create(blendData);
}

cwStencil* cwD3D11Device::createStencil(const cwStencilData& stencliData)
{
	return cwD3D11Stencil::create(stencliData);
}

void cwD3D11Device::setVertexBuffer(cwBuffer* pVertexBuffer)
{
	if (pVertexBuffer) {
		CWUINT stride = pVertexBuffer->getStride();
		CWUINT offset = pVertexBuffer->getOffset();
		ID3D11Buffer* pBuffer = static_cast<ID3D11Buffer*>(pVertexBuffer->getHandle());
		m_pD3D11DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);
	}
}

CWVOID cwD3D11Device::setVertexBuffer(cwBuffer** pBuffers, CWUINT uCnt)
{
	ID3D11Buffer* arrD3D11Buffer[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = { NULL };
	CWUINT arrStride[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};
	CWUINT arrOffset[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};

	for (CWUINT i = 0; i < uCnt; ++i) {
		arrD3D11Buffer[i] = static_cast<ID3D11Buffer*>(pBuffers[i]->getHandle());
		arrStride[i] = pBuffers[i]->getStride();
		arrOffset[i] = pBuffers[i]->getOffset();
	}

	m_pD3D11DeviceContext->IASetVertexBuffers(0, uCnt, arrD3D11Buffer, arrStride, arrOffset);
}

void cwD3D11Device::setIndexBuffer(cwBuffer* pIndexBuffer)
{
	if (pIndexBuffer) {
		m_pD3D11DeviceContext->IASetIndexBuffer(static_cast<ID3D11Buffer*>(pIndexBuffer->getHandle()), DXGI_FORMAT_R32_UINT, 0);
	}
}

void cwD3D11Device::setBlend(const cwBlend* pBlendOper)
{
	if (!pBlendOper) {
		//restore blend state
		//now blend state is default
		if (!m_pBlendState) return;

		CWFLOAT f[4] = { 0, 0, 0, 0 };
		m_pD3D11DeviceContext->OMSetBlendState(0, f, 0xFFFFFFFF);
		m_pBlendState = nullptr;

		return;
	}

	cwD3D11Blend* pD3DBlendOp = static_cast<cwD3D11Blend*>(const_cast<cwBlend*>(pBlendOper));
	if (m_pBlendState == pD3DBlendOp) return;

	ID3D11BlendState* pD3D11BlendState = static_cast<ID3D11BlendState*>(pBlendOper->getHandle());
	if (!pD3D11BlendState) return;

	CWFLOAT f[4] = { 0, 0, 0, 0 };
	m_pD3D11DeviceContext->OMSetBlendState(
		pD3D11BlendState,
		pD3DBlendOp->getBlendFactor(),
		pD3DBlendOp->getSampleMask());

	m_pBlendState = pD3DBlendOp;
}

void cwD3D11Device::setStencil(const cwStencil* pStencil)
{
	if (!pStencil) {
		if (!m_pStencil) return;
		m_pD3D11DeviceContext->OMSetDepthStencilState(0, 0);
		m_pStencil = nullptr;
		return;
	}

	ID3D11DepthStencilState* pState = static_cast<ID3D11DepthStencilState*>(pStencil->getHandle());
	if (!pState) return;
	m_pD3D11DeviceContext->OMSetDepthStencilState(pState, pStencil->getStencilRef());

	m_pStencil = const_cast<cwStencil*>(pStencil);
}

cwTexture* cwD3D11Device::createTexture(const CWSTRING& strFileName)
{
	cwTexture* pTexture = cwD3D11Texture::create(strFileName);
	return pTexture;
}

cwRenderTexture* cwD3D11Device::createRenderTexture(float fWidth, float fHeight, eRenderTextureType eType)
{
	switch (eType)
	{
	case eRenderTextureTarget:
		return cwD3D11RenderTarget::create();
	case eRenderTextureShader:
		return cwD3D11RenderTexture::create(fWidth, fHeight);
	case eRenderTextureWritable:
		return cwD3D11RenderTextureWritable::create(fWidth, fHeight);
	}

	return nullptr;
}

cwTexture* cwD3D11Device::createTextureArray(const std::vector<CWSTRING>& vecFiles)
{
	return cwD3D11TextureArray::create(vecFiles);
}

cwBatchEntity* cwD3D11Device::createBatchEntity()
{
	return cwD3D11BatchEntity::create();
}

void cwD3D11Device::setShaderWorldTrans(cwShader* pShader, const cwMatrix4X4& trans, cwCamera* pCamera)
{
	if (!pShader) return;

	pShader->setVariableMatrix(eShaderParamWorld, (CWFLOAT*)(&trans));

	if (pShader->hasVariable(eShaderParamWorldInvTrans)) {
		if (trans.inverseExist()) {
			cwMatrix4X4 matWorldInvTrans = trans.inverse().transpose();
			pShader->setVariableMatrix(eShaderParamWorldInvTrans, reinterpret_cast<CWFLOAT*>(&matWorldInvTrans));
		}
		else {
			cwMatrix4X4& M = cwMatrix4X4::identityMatrix;
			pShader->setVariableMatrix(eShaderParamWorldInvTrans, reinterpret_cast<CWFLOAT*>(&M));
		}
	}

	if (pCamera) {
		if (pShader->hasVariable(eShaderParamWorldViewProj)) {
			const cwMatrix4X4& matViewProj = pCamera->getViewProjMatrix();
			cwMatrix4X4 worldViewProj = trans*matViewProj;
			pShader->setVariableMatrix(eShaderParamWorldViewProj, reinterpret_cast<CWFLOAT*>(&worldViewProj));
			pShader->setVariableMatrix(eShaderParamViewProj, (CWFLOAT*)(&matViewProj));
		}

		const cwVector3D& pos = pCamera->getPos();
		pShader->setVariableData(eShaderParamEyePosWorld, (CWVOID*)&pos, 0, sizeof(cwVector3D));
	}
}

void cwD3D11Device::render(cwRenderObject* pRenderObj, const cwVector3D& worldPos, cwShader* pShader, cwCamera* pCamera)
{
	if (!pRenderObj || !pShader || !pCamera) return;

	cwMatrix4X4 matWorld;
	matWorld.setTranslation(worldPos);

	setShaderWorldTrans(pShader, matWorld, pCamera);

	pRenderObj->preRender();

	this->setInputLayout(pRenderObj->getInputLayout());
	this->setPrimitiveTopology(pRenderObj->getPrimitiveTopology());
	this->setVertexBuffer(pRenderObj->getVertexBuffer());

	if (pRenderObj->getIndexBuffer()) {
		this->setIndexBuffer(pRenderObj->getIndexBuffer());

		pShader->apply(0, 0);
		m_pD3D11DeviceContext->DrawIndexed(pRenderObj->getIndexBuffer()->getElementCount(), 0, 0);
	}
	else {
		pShader->apply(0, 0);
		m_pD3D11DeviceContext->Draw(pRenderObj->getVertexBuffer()->getElementCount(), 0);
	}
}

void cwD3D11Device::draw(cwShader* pShader, const CWSTRING& strTech, cwRenderObject* pRenderObj)
{
	this->setInputLayout(pRenderObj->getInputLayout());
	this->setPrimitiveTopology(pRenderObj->getPrimitiveTopology());

	pRenderObj->preRender();

	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique* pTech = nullptr;

	cwD3D11Shader* pD3D11Shader = static_cast<cwD3D11Shader*>(pShader);

	if (strTech.empty()) {
		pTech = pD3D11Shader->getTechnique(0);
	}
	else {
		pTech = pD3D11Shader->getTechnique(strTech);
	}

	if (!pTech) return;
	pTech->GetDesc(&techDesc);

	if (pRenderObj->getIndexBuffer()) {
		for (CWUINT i = 0; i < techDesc.Passes; ++i) {
			this->setVertexBuffer(pRenderObj->getVertexBuffer());
			this->setIndexBuffer(pRenderObj->getIndexBuffer());

			pTech->GetPassByIndex(i)->Apply(0, m_pD3D11DeviceContext);
			m_pD3D11DeviceContext->DrawIndexed(pRenderObj->getIndexBuffer()->getElementCount(), 0, 0);
		}
	}
	else {
		for (CWUINT i = 0; i < techDesc.Passes; ++i) {
			this->setVertexBuffer(pRenderObj->getVertexBuffer());

			pTech->GetPassByIndex(i)->Apply(0, m_pD3D11DeviceContext);
			m_pD3D11DeviceContext->Draw(pRenderObj->getVertexBuffer()->getElementCount(), 0);
		}
	}
}

CWVOID cwD3D11Device::draw(cwShader* pShader, const CWSTRING& strTech, std::vector<cwRenderObject*>& vecRenderObject, CWUINT uCnt)
{
	cwRenderObject* pMeshRenderObj = vecRenderObject[0];

	this->setInputLayout(pMeshRenderObj->getInputLayout());
	this->setPrimitiveTopology(pMeshRenderObj->getPrimitiveTopology());

	cwBuffer* arrBuffer[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {NULL};
	CWUINT iSize = (CWUINT)vecRenderObject.size();
	iSize = iSize < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ? iSize : D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;
	for (CWUINT i = 0; i < iSize; ++i) {
		vecRenderObject[i]->preRender();
		arrBuffer[i] = vecRenderObject[i]->getVertexBuffer();
	}

	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique* pTech = nullptr;

	cwD3D11Shader* pD3D11Shader = static_cast<cwD3D11Shader*>(pShader);

	if (strTech.empty()) {
		pTech = pD3D11Shader->getTechnique(0);
	}
	else {
		pTech = pD3D11Shader->getTechnique(strTech);
	}

	if (!pTech) return;
	pTech->GetDesc(&techDesc);

	if (pMeshRenderObj->getIndexBuffer()) {
		for (CWUINT i = 0; i < techDesc.Passes; ++i) {
			this->setVertexBuffer(arrBuffer, iSize);
			this->setIndexBuffer(pMeshRenderObj->getIndexBuffer());

			pTech->GetPassByIndex(i)->Apply(0, m_pD3D11DeviceContext);
			m_pD3D11DeviceContext->DrawIndexedInstanced(pMeshRenderObj->getIndexBuffer()->getElementCount(), uCnt, 0, 0, 0);
		}
	}
	else {
		for (CWUINT i = 0; i < techDesc.Passes; ++i) {
			this->setVertexBuffer(arrBuffer, iSize);

			pTech->GetPassByIndex(i)->Apply(0, m_pD3D11DeviceContext);
			m_pD3D11DeviceContext->DrawInstanced(pMeshRenderObj->getVertexBuffer()->getElementCount(), uCnt, 0, 0);
		}
	}
}

CWVOID cwD3D11Device::drawGP(cwShader* pShader, const CWSTRING& strTech, cwGPInfo* pGPInfo)
{
	if (!pShader || !pGPInfo) return;

	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique* pTech = nullptr;

	cwD3D11Shader* pD3D11Shader = static_cast<cwD3D11Shader*>(pShader);

	if (strTech.empty()) {
		pTech = pD3D11Shader->getTechnique(0);
	}
	else {
		pTech = pD3D11Shader->getTechnique(strTech);
	}

	if (!pTech) return;
	pTech->GetDesc(&techDesc);

	for (CWUINT i = 0; i < techDesc.Passes; ++i) {
		pTech->GetPassByIndex(i)->Apply(0, m_pD3D11DeviceContext);
		m_pD3D11DeviceContext->Dispatch(pGPInfo->groupX, pGPInfo->groupY, pGPInfo->groupZ);
	}

	ID3D11ShaderResourceView* nullSRV[1] = { 0 };
	m_pD3D11DeviceContext->CSSetShaderResources(0, 1, nullSRV);
}

CWVOID cwD3D11Device::clearShaderResource()
{
	ID3D11ShaderResourceView* pSrvs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { 0 };
	m_pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSrvs);

	ID3D11ShaderResourceView* nullSRV[1] = { 0 };
	m_pD3D11DeviceContext->CSSetShaderResources(0, 1, nullSRV);

	ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
	m_pD3D11DeviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);

	m_pD3D11DeviceContext->CSSetShader(0, 0, 0);
}

void cwD3D11Device::initBlendBaseData()
{
	blendFactor[eBlendFactorZero]           = D3D11_BLEND_ZERO;
	blendFactor[eBlendFactorOne]            = D3D11_BLEND_ONE;
	blendFactor[eBlendFactorSrcColor]       = D3D11_BLEND_SRC_COLOR;
	blendFactor[eBlendFactorInvSrcColor]    = D3D11_BLEND_INV_SRC_COLOR;
	blendFactor[eBlendFactorSrcAlpha]       = D3D11_BLEND_SRC_ALPHA;
	blendFactor[eBlendFactorInvSrcAlpha]    = D3D11_BLEND_INV_SRC_ALPHA;
	blendFactor[eBlendFactorDestAlpha]      = D3D11_BLEND_DEST_ALPHA;
	blendFactor[eBlendFactorInvDestAlpha]   = D3D11_BLEND_INV_DEST_ALPHA;
	blendFactor[eBlendFactorDestColor]      = D3D11_BLEND_DEST_COLOR;
	blendFactor[eBlendFactorInvDestColor]   = D3D11_BLEND_INV_DEST_COLOR;
	blendFactor[eBlendFactorSrcAlphaSat]    = D3D11_BLEND_SRC_ALPHA_SAT;
	blendFactor[eBlendFactorBlendFactor]    = D3D11_BLEND_BLEND_FACTOR;
	blendFactor[eBlendFactorInvBlendFactor] = D3D11_BLEND_INV_BLEND_FACTOR;
	blendFactor[eBlendFactorSrc1Color]      = D3D11_BLEND_SRC1_COLOR;
	blendFactor[eBlendFactorInvSrc1Color]   = D3D11_BLEND_INV_SRC1_COLOR;
	blendFactor[eBlendFactorSrc1Alpha]      = D3D11_BLEND_SRC1_ALPHA;
	blendFactor[eBlendFactorInvSrc1Alpha]   = D3D11_BLEND_INV_SRC1_ALPHA;

	blendOp[eBlendOpAdd]         = D3D11_BLEND_OP_ADD;
	blendOp[eBlendOpSubtract]    = D3D11_BLEND_OP_SUBTRACT;
	blendOp[eBlendOpRevSubtract] = D3D11_BLEND_OP_REV_SUBTRACT;
	blendOp[eBlendOpMin]         = D3D11_BLEND_OP_MIN;
	blendOp[eBlendOpMax]         = D3D11_BLEND_OP_MAX;
}

void cwD3D11Device::initStencilBaseData()
{
	stencilOp[eStencilOpKeep]    = D3D11_STENCIL_OP_KEEP;
	stencilOp[eStencilOpZero]    = D3D11_STENCIL_OP_ZERO;
	stencilOp[eStencilOpReplace] = D3D11_STENCIL_OP_REPLACE;
	stencilOp[eStencilOpIncrSat] = D3D11_STENCIL_OP_INCR_SAT;
	stencilOp[eStencilOpDecrSat] = D3D11_STENCIL_OP_DECR_SAT;
	stencilOp[eStencilOpInvert]  = D3D11_STENCIL_OP_INVERT;
	stencilOp[eStencilOpIncr]    = D3D11_STENCIL_OP_INCR;
	stencilOp[eStencilOpDecr]    = D3D11_STENCIL_OP_DECR;

	comparisonType[eComparisonNever]        = D3D11_COMPARISON_NEVER;
	comparisonType[eComparisonLess]         = D3D11_COMPARISON_LESS;
	comparisonType[eComparisonEqual]        = D3D11_COMPARISON_EQUAL;
	comparisonType[eComparisonLessEqual]    = D3D11_COMPARISON_LESS_EQUAL;
	comparisonType[eComparisonGreater]      = D3D11_COMPARISON_GREATER;
	comparisonType[eComparisonNotEqual]     = D3D11_COMPARISON_NOT_EQUAL;
	comparisonType[eComparisonGreaterEqual] = D3D11_COMPARISON_GREATER_EQUAL;
	comparisonType[eComparisonAlways]       = D3D11_COMPARISON_ALWAYS;

	depthWriteMask[eDepthWriteMaskZero] = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthWriteMask[eDepthWriteMaskAll]  = D3D11_DEPTH_WRITE_MASK_ALL;
}

void cwD3D11Device::initAccessFlagData()
{
	accessFlag[eAccessFlagNone]  = 0;
	accessFlag[eAccessFlagRead]  = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
	accessFlag[eAccessFlagWrite] = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
}

void cwD3D11Device::initBufferBindFlagData()
{
	bufferBindFlag[eBufferBindNone]         = 0;
	bufferBindFlag[eBufferBindVertex]       = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	bufferBindFlag[eBufferBindIndex]        = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	bufferBindFlag[eBufferBindConstant]     = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	bufferBindFlag[eBufferBindShader]       = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	bufferBindFlag[eBufferBindSteam]        = D3D11_BIND_FLAG::D3D11_BIND_STREAM_OUTPUT;
	bufferBindFlag[eBufferBindRenderTarget] = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	bufferBindFlag[eBufferBindDepthStencil] = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	bufferBindFlag[eBufferBindWritable]     = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
}

void cwD3D11Device::initBufferUsageData()
{
	bufferUsage[eBufferUsageDefault]   = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	bufferUsage[eBufferUsageImmutable] = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
	bufferUsage[eBufferUsageDynamic]   = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	bufferUsage[eBufferUsageStaging]   = D3D11_USAGE::D3D11_USAGE_STAGING;
}

void cwD3D11Device::initPrimitiveTypeData()
{
	primitiveType[ePrimitiveTypePointList]        = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	primitiveType[ePrimitiveTypeLineList]         = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	primitiveType[ePrimitiveTypeLineStrip]        = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	primitiveType[ePrimitiveTypeTriangleList]     = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	primitiveType[ePrimitiveTypeTriangleStrip]    = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	primitiveType[ePrimitiveTypeLineListAdj]      = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
	primitiveType[ePrimitiveTypeLineStripAdj]     = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
	primitiveType[ePrimitiveTypeTriangleListAdj]  = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
	primitiveType[ePrimitiveTypeTriangleStripAdj] = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
	primitiveType[ePrimitiveTypePatchList1]       = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList2]       = D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList3]       = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList4]       = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList5]       = D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList6]       = D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList7]       = D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList8]       = D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList9]       = D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList10]      = D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList11]      = D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList12]      = D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList13]      = D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList14]      = D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList15]      = D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList16]      = D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList17]      = D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList18]      = D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList19]      = D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList20]      = D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList21]      = D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList22]      = D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList23]      = D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList24]      = D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList25]      = D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList26]      = D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList27]      = D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList28]      = D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList29]      = D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList30]      = D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList31]      = D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList32]      = D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
}

CWVOID cwD3D11Device::initFormatTypeData()
{
	formatType[eFormatUnknown]                = DXGI_FORMAT_UNKNOWN;
	formatType[eFormatR32g32b32a32Typeless]   = DXGI_FORMAT_R32G32B32A32_TYPELESS;
	formatType[eFormatR32g32b32a32Float]      = DXGI_FORMAT_R32G32B32A32_FLOAT;
	formatType[eFormatR32g32b32a32Uint]       = DXGI_FORMAT_R32G32B32A32_UINT;
	formatType[eFormatR32g32b32a32Sint]       = DXGI_FORMAT_R32G32B32A32_SINT;
	formatType[eFormatR32g32b32Typeless]      = DXGI_FORMAT_R32G32B32_TYPELESS;
	formatType[eFormatR32g32b32Float]         = DXGI_FORMAT_R32G32B32_FLOAT;
	formatType[eFormatR32g32b32Uint]          = DXGI_FORMAT_R32G32B32_UINT;
	formatType[eFormatR32g32b32Sint]          = DXGI_FORMAT_R32G32B32_SINT;
	formatType[eFormatR16g16b16a16Typeless]   = DXGI_FORMAT_R16G16B16A16_TYPELESS;
	formatType[eFormatR16g16b16a16Float]      = DXGI_FORMAT_R16G16B16A16_FLOAT;
	formatType[eFormatR16g16b16a16Unorm]      = DXGI_FORMAT_R16G16B16A16_UNORM;
	formatType[eFormatR16g16b16a16Uint]       = DXGI_FORMAT_R16G16B16A16_UINT;
	formatType[eFormatR16g16b16a16Snorm]      = DXGI_FORMAT_R16G16B16A16_SNORM;
	formatType[eFormatR16g16b16a16Sint]       = DXGI_FORMAT_R16G16B16A16_SINT;
	formatType[eFormatR32g32Typeless]         = DXGI_FORMAT_R32G32_TYPELESS;
	formatType[eFormatR32g32Float]            = DXGI_FORMAT_R32G32_FLOAT;
	formatType[eFormatR32g32Uint]             = DXGI_FORMAT_R32G32_UINT;
	formatType[eFormatR32g32Sint]             = DXGI_FORMAT_R32G32_SINT;
	formatType[eFormatR32g8x24Typeless]       = DXGI_FORMAT_R32G8X24_TYPELESS;
	formatType[eFormatD32FloatS8x24Uint]      = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	formatType[eFormatR32FloatX8x24Typeless]  = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
	formatType[eFormatX32TypelessG8x24Uint]   = DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
	formatType[eFormatR10g10b10a2Typeless]    = DXGI_FORMAT_R10G10B10A2_TYPELESS;
	formatType[eFormatR10g10b10a2Unorm]       = DXGI_FORMAT_R10G10B10A2_UNORM;
	formatType[eFormatR10g10b10a2Uint]        = DXGI_FORMAT_R10G10B10A2_UINT;
	formatType[eFormatR11g11b10Float]         = DXGI_FORMAT_R11G11B10_FLOAT;
	formatType[eFormatR8g8b8a8Typeless]       = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	formatType[eFormatR8g8b8a8Unorm]          = DXGI_FORMAT_R8G8B8A8_UNORM;
	formatType[eFormatR8g8b8a8UnormSrgb]      = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	formatType[eFormatR8g8b8a8Uint]           = DXGI_FORMAT_R8G8B8A8_UINT;
	formatType[eFormatR8g8b8a8Snorm]          = DXGI_FORMAT_R8G8B8A8_SNORM;
	formatType[eFormatR8g8b8a8Sint]           = DXGI_FORMAT_R8G8B8A8_SINT;
	formatType[eFormatR16g16Typeless]         = DXGI_FORMAT_R16G16_TYPELESS;
	formatType[eFormatR16g16Float]            = DXGI_FORMAT_R16G16_FLOAT;
	formatType[eFormatR16g16Unorm]            = DXGI_FORMAT_R16G16_UNORM;
	formatType[eFormatR16g16Uint]             = DXGI_FORMAT_R16G16_UINT;
	formatType[eFormatR16g16Snorm]            = DXGI_FORMAT_R16G16_SNORM;
	formatType[eFormatR16g16Sint]             = DXGI_FORMAT_R16G16_SINT;
	formatType[eFormatR32Typeless]            = DXGI_FORMAT_R32_TYPELESS;
	formatType[eFormatd32Float]               = DXGI_FORMAT_D32_FLOAT;
	formatType[eFormatR32Float]               = DXGI_FORMAT_R32_FLOAT;
	formatType[eFormatR32Uint]                = DXGI_FORMAT_R32_UINT;
	formatType[eFormatR32Sint]                = DXGI_FORMAT_R32_SINT;
	formatType[eFormatR24g8Typeless]          = DXGI_FORMAT_R24G8_TYPELESS;
	formatType[eFormatd24UnormS8Uint]         = DXGI_FORMAT_D24_UNORM_S8_UINT;
	formatType[eFormatR24UnormX8Typeless]     = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	formatType[eFormatx24TypelessG8Uint]      = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	formatType[eFormatR8g8Typeless]           = DXGI_FORMAT_R8G8_TYPELESS;
	formatType[eFormatR8g8Unorm]              = DXGI_FORMAT_R8G8_UNORM;
	formatType[eFormatR8g8Uint]               = DXGI_FORMAT_R8G8_UINT;
	formatType[eFormatR8g8Snorm]              = DXGI_FORMAT_R8G8_SNORM;
	formatType[eFormatR8g8Sint]               = DXGI_FORMAT_R8G8_SINT;
	formatType[eFormatR16Typeless]            = DXGI_FORMAT_R16_TYPELESS;
	formatType[eFormatR16Float]               = DXGI_FORMAT_R16_FLOAT;
	formatType[eFormatd16Unorm]               = DXGI_FORMAT_D16_UNORM;
	formatType[eFormatR16Unorm]               = DXGI_FORMAT_R16_UNORM;
	formatType[eFormatR16Uint]                = DXGI_FORMAT_R16_UINT;
	formatType[eFormatR16Snorm]               = DXGI_FORMAT_R16_SNORM;
	formatType[eFormatR16Sint]                = DXGI_FORMAT_R16_SINT;
	formatType[eFormatR8Typeless]             = DXGI_FORMAT_R8_TYPELESS;
	formatType[eFormatR8Unorm]                = DXGI_FORMAT_R8_UNORM;
	formatType[eFormatR8Uint]                 = DXGI_FORMAT_R8_UINT;
	formatType[eFormatR8Snorm]                = DXGI_FORMAT_R8_SNORM;
	formatType[eFormatR8Sint]                 = DXGI_FORMAT_R8_SINT;
	formatType[eFormatA8Unorm]                = DXGI_FORMAT_A8_UNORM;
	formatType[eFormatR1Unorm]                = DXGI_FORMAT_R1_UNORM;
	formatType[eFormatR9g9b9e5Sharedexp]      = DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
	formatType[eFormatR8g8_B8g8Unorm]         = DXGI_FORMAT_R8G8_B8G8_UNORM;
	formatType[eFormatG8r8G8b8Unorm]          = DXGI_FORMAT_G8R8_G8B8_UNORM;
	formatType[eFormatBc1Typeless]            = DXGI_FORMAT_BC1_TYPELESS;
	formatType[eFormatBc1Unorm]               = DXGI_FORMAT_BC1_UNORM;
	formatType[eFormatBc1UnormSrgb]           = DXGI_FORMAT_BC1_UNORM_SRGB;
	formatType[eFormatBc2Typeless]            = DXGI_FORMAT_BC2_TYPELESS;
	formatType[eFormatBc2Unorm]               = DXGI_FORMAT_BC2_UNORM;
	formatType[eFormatBc2UnormSrgb]           = DXGI_FORMAT_BC2_UNORM_SRGB;
	formatType[eFormatBc3Typeless]            = DXGI_FORMAT_BC3_TYPELESS;
	formatType[eFormatBc3Unorm]               = DXGI_FORMAT_BC3_UNORM;
	formatType[eFormatBc3UnormSrgb]           = DXGI_FORMAT_BC3_UNORM_SRGB;
	formatType[eFormatBc4Typeless]            = DXGI_FORMAT_BC4_TYPELESS;
	formatType[eFormatBc4Unorm]               = DXGI_FORMAT_BC4_UNORM;
	formatType[eFormatBc4Snorm]               = DXGI_FORMAT_BC4_SNORM;
	formatType[eFormatBc5Typeless]            = DXGI_FORMAT_BC5_TYPELESS;
	formatType[eFormatBc5Unorm]               = DXGI_FORMAT_BC5_UNORM;
	formatType[eFormatBc5Snorm]               = DXGI_FORMAT_BC5_SNORM;
	formatType[eFormatB5g6r5Unorm]            = DXGI_FORMAT_B5G6R5_UNORM;
	formatType[eFormatB5g5r5a1Unorm]          = DXGI_FORMAT_B5G5R5A1_UNORM;
	formatType[eFormatB8g8r8a8Unorm]          = DXGI_FORMAT_B8G8R8A8_UNORM;
	formatType[eFormatB8g8r8x8Unorm]          = DXGI_FORMAT_B8G8R8X8_UNORM;
	formatType[eFormatR10g10b10XrBiasA2Unorm] = DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
	formatType[eFormatB8g8r8a8Typeless]       = DXGI_FORMAT_B8G8R8A8_TYPELESS;
	formatType[eFormatB8g8r8a8UnormSrgb]      = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	formatType[eFormatB8g8r8x8Typeless]       = DXGI_FORMAT_B8G8R8X8_TYPELESS;
	formatType[eFormatB8g8r8x8UnormSrgb]      = DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
	formatType[eFormatBc6hTypeless]           = DXGI_FORMAT_BC6H_TYPELESS;
	formatType[eFormatBc6hUf16]               = DXGI_FORMAT_BC6H_UF16;
	formatType[eFormatBc6hSf16]               = DXGI_FORMAT_BC6H_SF16;
	formatType[eFormatBc7Typeless]            = DXGI_FORMAT_BC7_TYPELESS;
	formatType[eFormatBc7Unorm]               = DXGI_FORMAT_BC7_UNORM;
	formatType[eFormatBc7UnormSrgb]           = DXGI_FORMAT_BC7_UNORM_SRGB;
	formatType[eFormatForceUint]              = DXGI_FORMAT_FORCE_UINT;
}

CWVOID cwD3D11Device::initClassification()
{
	classificationType[eClassificationPerVertex]   = D3D11_INPUT_PER_VERTEX_DATA;
	classificationType[eClassificationPerInstance] = D3D11_INPUT_PER_INSTANCE_DATA;
}

NS_MINIR_END

#endif
