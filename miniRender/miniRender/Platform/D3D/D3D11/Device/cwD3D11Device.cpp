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
#include "Platform/D3D/D3D11/Buffer/cwD3D11VertexBuffer.h"
#include "Platform/D3D/D3D11/Buffer/cwD3D11IndexBuffer.h"
#include "Platform/D3D/D3D11/Stencil/cwD3D11Stencil.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11Texture.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11RenderTarget.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11RenderTexture.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11RenderTextureMultiThread.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11TextureArray.h"
#include "Platform/D3D/D3D11/Blend/cwD3D11Blend.h"
#include "Platform/D3D/D3D11/Shader/cwD3D11Shader.h"
#include "Platform/D3D/D3D11/ViewPort/cwD3D11ViewPort.h"

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
	clearPixelShaderResource();
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

void cwD3D11Device::drawIndexed(CWUINT indexCnt, CWUINT startIndex, CWINT baseVertex)
{
	m_pD3D11DeviceContext->DrawIndexed(indexCnt, startIndex, baseVertex);
}

cwShader* cwD3D11Device::createShader(const string& strFileName)
{
	return cwD3D11Shader::create(strFileName);
}

cwBuffer* cwD3D11Device::createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt)
{
	cwD3D11VertexBuffer* pVertexBuffer = cwD3D11VertexBuffer::create(uStride*uCnt, uStride);
	if (!pVertexBuffer) return NULL;

	CW_BUFFER_DESC& desc = pVertexBuffer->getBufferDesc();
	D3D11_BUFFER_DESC d3dDesc;
	memcpy(&d3dDesc, &desc, sizeof(CW_BUFFER_DESC));
	ID3D11Buffer* pBuffer = static_cast<ID3D11Buffer*>(pVertexBuffer->getBuffer());

	if (pData) {
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = pData;
		CW_HR(m_pD3D11Device->CreateBuffer(&d3dDesc, &subData, &pBuffer));
	}
	else {
		CW_HR(m_pD3D11Device->CreateBuffer(&d3dDesc, 0, &pBuffer));
	}

	pVertexBuffer->setBuffer(pBuffer);
	pVertexBuffer->setStride(uStride);

	return pVertexBuffer;
}

cwBuffer* cwD3D11Device::createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, eBufferUsage usage, CWUINT cpuFlag)
{
	cwD3D11VertexBuffer* pVertexBuffer = cwD3D11VertexBuffer::create(uStride*uCnt, usage, cpuFlag);
	if (!pVertexBuffer) return NULL;
	
	CW_BUFFER_DESC& desc = pVertexBuffer->getBufferDesc();
	D3D11_BUFFER_DESC d3dDesc;
	memcpy(&d3dDesc, &desc, sizeof(CW_BUFFER_DESC));
	ID3D11Buffer* pBuffer = static_cast<ID3D11Buffer*>(pVertexBuffer->getBuffer());

	if (pData) {
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = pData;
		CW_HR(m_pD3D11Device->CreateBuffer(&d3dDesc, &subData, &pBuffer));
	}
	else {
		CW_HR(m_pD3D11Device->CreateBuffer(&d3dDesc, 0, &pBuffer));
	}

	pVertexBuffer->setBuffer(pBuffer);
	pVertexBuffer->setStride(uStride);

	return pVertexBuffer;
}

cwBuffer* cwD3D11Device::createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt)
{
	cwD3D11IndexBuffer* pIndexBuffer = cwD3D11IndexBuffer::create(uStride*uCnt, uStride);
	if (!pIndexBuffer) return NULL;
	
	CW_BUFFER_DESC& desc = pIndexBuffer->getBufferDesc();
	D3D11_BUFFER_DESC d3dDesc;
	memcpy(&d3dDesc, &desc, sizeof(CW_BUFFER_DESC));
	ID3D11Buffer* pBuffer = static_cast<ID3D11Buffer*>(pIndexBuffer->getBuffer());

	if (pData) {
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = pData;
		CW_HR(m_pD3D11Device->CreateBuffer(&d3dDesc, &subData, &pBuffer));
	}
	else {
		CW_HR(m_pD3D11Device->CreateBuffer(&d3dDesc, 0, &pBuffer));
	}

	pIndexBuffer->setBuffer(pBuffer);

	return pIndexBuffer;
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
		ID3D11Buffer* pBuffer = static_cast<ID3D11Buffer*>(pVertexBuffer->getBuffer());
		m_pD3D11DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);
	}
}

void cwD3D11Device::setIndexBuffer(cwBuffer* pIndexBuffer)
{
	if (pIndexBuffer) {
		m_pD3D11DeviceContext->IASetIndexBuffer(static_cast<ID3D11Buffer*>(pIndexBuffer->getBuffer()), DXGI_FORMAT_R32_UINT, 0);
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

	ID3D11BlendState* pD3D11BlendState = static_cast<ID3D11BlendState*>(const_cast<CWVOID*>(pBlendOper->getBlendHandlePtr()));
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

	ID3D11DepthStencilState* pState = static_cast<ID3D11DepthStencilState*>(const_cast<CWVOID*>(pStencil->getStencilHandlePtr()));
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
	case eRenderTextureMultiThread:
		return cwD3D11RenderTextureMultiThread::create(fWidth, fHeight);
	}

	return nullptr;
}

cwTexture* cwD3D11Device::createTextureArray(const std::vector<CWSTRING>& vecFiles)
{
	return cwD3D11TextureArray::create(vecFiles);
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

	if (pShader->hasVariable(eShaderParamWorldViewProj)) {
		const cwMatrix4X4& matViewProj = pCamera->getViewProjMatrix();
		cwMatrix4X4 worldViewProj = trans*matViewProj;
		pShader->setVariableMatrix(eShaderParamWorldViewProj, reinterpret_cast<CWFLOAT*>(&worldViewProj));
		pShader->setVariableMatrix(eShaderParamViewProj, (CWFLOAT*)(&matViewProj));
	}

	const cwVector3D& pos = pCamera->getPos();
	pShader->setVariableData(eShaderParamEyePosWorld, (CWVOID*)&pos, 0, sizeof(cwVector3D));
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

CWVOID cwD3D11Device::clearPixelShaderResource()
{
	ID3D11ShaderResourceView* pSrvs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { 0 };
	m_pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSrvs);
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
	accessFlag[eAccessFlagNone] = 0;
	accessFlag[eAccessFlagRead] = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
	accessFlag[eAccessFlagWrite] = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
}

void cwD3D11Device::initBufferBindFlagData()
{
	bufferBindFlag[eBufferBindVertex] = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	bufferBindFlag[eBufferBindIndex] = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	bufferBindFlag[eBufferBindConstant] = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	bufferBindFlag[eBufferBindShader] = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	bufferBindFlag[eBufferBindSteam] = D3D11_BIND_FLAG::D3D11_BIND_STREAM_OUTPUT;
	bufferBindFlag[eBufferBindRenderTarget] = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	bufferBindFlag[eBufferBindDepthStencil] = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	bufferBindFlag[eBufferBindUnorderedAccess] = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
}

void cwD3D11Device::initBufferUsageData()
{
	bufferUsage[eBufferUsageDefault] = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	bufferUsage[eBufferUsageImmutable] = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
	bufferUsage[eBufferUsageDynamic] = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	bufferUsage[eBufferUsageStaging] = D3D11_USAGE::D3D11_USAGE_STAGING;
}

void cwD3D11Device::initPrimitiveTypeData()
{
	primitiveType[ePrimitiveTypePointList] = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	primitiveType[ePrimitiveTypeLineList] = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	primitiveType[ePrimitiveTypeLineStrip] = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	primitiveType[ePrimitiveTypeTriangleList] = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	primitiveType[ePrimitiveTypeTriangleStrip] = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	primitiveType[ePrimitiveTypeLineListAdj] = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
	primitiveType[ePrimitiveTypeLineStripAdj] = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
	primitiveType[ePrimitiveTypeTriangleListAdj] = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
	primitiveType[ePrimitiveTypeTriangleStripAdj] = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
	primitiveType[ePrimitiveTypePatchList1] = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList2] = D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList3] = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList4] = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList5] = D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList6] = D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList7] = D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList8] = D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList9] = D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList10] = D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList11] = D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList12] = D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList13] = D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList14] = D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList15] = D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList16] = D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList17] = D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList18] = D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList19] = D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList20] = D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList21] = D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList22] = D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList23] = D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList24] = D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList25] = D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList26] = D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList27] = D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList28] = D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList29] = D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList30] = D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList31] = D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
	primitiveType[ePrimitiveTypePatchList32] = D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
}

NS_MINIR_END

#endif
