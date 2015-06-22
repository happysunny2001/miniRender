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

#include "cwD3D11Device.h"
#include "Base/cwColor.h"
#include "cwEffects.h"
#include "Layouts/cwLayouts.h"
#include "Layouts/cwD3D11Layouts.h"
#include "Buffer/cwBuffer.h"
#include "cwVertexBuffer.h"
#include "cwIndexBuffer.h"
#include "cwRenderObject.h"
#include "cwShaderConstant.h"
#include "cwCamera.h"
#include "cwEntity.h"
#include "cwMaterial.h"
#include "cwCommon.h"
#include "cwTexture.h"
#include "cwD3D11Texture.h"
#include "cwD3D11Blend.h"
#include "cwStencil.h"
#include "cwD3D11Stencil.h"

#include <assert.h>
#include <xnamath.h>

NS_MINI_BEGIN

cwD3D11Device::cwD3D11Device(HWND hWnd, CWUINT width, CWUINT height) :
cwRenderDevice(hWnd, width, height),
m_pD3D11Device(NULL),
m_pD3D11DeviceContext(NULL),
m_pDxgiSwapChain(NULL),
m_pD3D11RenderTarget(NULL),
m_pDepthStencilBuffer(NULL),
m_pDepthStencilView(NULL),
m_pSolidRenderState(NULL),
m_pWireRenderState(NULL),
m_pNoCullRenderState(NULL),
m_pCullCWRenderState(NULL),
m_pMaterialDefault(nullptr)
{
}

cwD3D11Device::~cwD3D11Device()
{
	CW_RELEASE_COM(m_pD3D11RenderTarget);
	CW_RELEASE_COM(m_pDepthStencilView);
	CW_RELEASE_COM(m_pDepthStencilBuffer);
	CW_RELEASE_COM(m_pDxgiSwapChain);
	CW_RELEASE_COM(m_pD3D11Device);
	CW_RELEASE_COM(m_pD3D11DeviceContext);
	CW_RELEASE_COM(m_pSolidRenderState);
	CW_RELEASE_COM(m_pWireRenderState);
	CW_RELEASE_COM(m_pNoCullRenderState);
	CW_RELEASE_COM(m_pCullCWRenderState);
	CW_SAFE_RELEASE_NULL(m_pMaterialDefault);
}

bool cwD3D11Device::initDevice()
{
	//create d3d11 device and d3d11 device context
	UINT devFlags = 0;
#if CW_DEBUG
	devFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;
	CW_HR(D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 
		devFlags, 
		NULL, 
		0, 
		D3D11_SDK_VERSION,
		&m_pD3D11Device,
		&featureLevel,
		&m_pD3D11DeviceContext
		));

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(NULL, L"error create d3d11 device!", L"error", MB_OK);
		return false;
	}

	//check MSAA 4x support
	CW_HR(m_pD3D11Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_uiM4xMsaaQuality));
	assert(m_uiM4xMsaaQuality > 0);

	//create swap buffer
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = m_uiClientWidth;
	sd.BufferDesc.Height = m_uiClientHeight;
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
	sd.OutputWindow = m_hWnd;
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

	resize(m_uiClientWidth, m_uiClientHeight);

	m_pMaterialDefault = cwMaterial::create(
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f));
	CW_SAFE_RETAIN(m_pMaterialDefault);

	return true;
}

void cwD3D11Device::createRenderTarget()
{
	ID3D11Texture2D* backBuffer = NULL;
	CW_HR(m_pDxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));
	CW_HR(m_pD3D11Device->CreateRenderTargetView(backBuffer, NULL, &m_pD3D11RenderTarget));
	CW_RELEASE_COM(backBuffer);
}

void cwD3D11Device::createDepthStencil()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width  = m_uiClientWidth;
	texDesc.Height = m_uiClientHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (m_bEnableMsaa4x) {
		texDesc.SampleDesc.Count = 4;
		texDesc.SampleDesc.Quality = m_uiM4xMsaaQuality - 1;
	}
	else {
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
	}
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	CW_HR(m_pD3D11Device->CreateTexture2D(&texDesc, NULL, &m_pDepthStencilBuffer));
	CW_HR(m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView));
}

void cwD3D11Device::resize(CWUINT width, CWUINT height)
{
	CW_RELEASE_COM(m_pD3D11RenderTarget);
	CW_RELEASE_COM(m_pDepthStencilView);
	CW_RELEASE_COM(m_pDepthStencilBuffer);

	m_uiClientWidth  = width;
	m_uiClientHeight = height;

	CW_HR(m_pDxgiSwapChain->ResizeBuffers(1, m_uiClientWidth, m_uiClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	createRenderTarget();
	createDepthStencil();
	m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pD3D11RenderTarget, m_pDepthStencilView);

	createViewPort();
}

void cwD3D11Device::resize()
{
	resize(m_uiClientWidth, m_uiClientHeight);
}

void cwD3D11Device::setSize(CWUINT width, CWUINT height)
{
	m_uiClientWidth  = width;
	m_uiClientHeight = height;
}

void cwD3D11Device::setClearColor(const cwVector4D& fvColor)
{
	m_fvClearColor = fvColor;
}

void cwD3D11Device::createViewPort()
{
	D3D11_VIEWPORT viewPort;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = (float)m_uiClientWidth;
	viewPort.Height = (float)m_uiClientHeight;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1.0f;
	m_pD3D11DeviceContext->RSSetViewports(1, &viewPort);
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

void cwD3D11Device::beginDraw()
{
	m_pD3D11DeviceContext->ClearRenderTargetView(m_pD3D11RenderTarget, (const CWFLOAT*)&m_fvClearColor);
	m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void cwD3D11Device::endDraw()
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
	m_pD3D11DeviceContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(topology));
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

void cwD3D11Device::DrawIndexed(CWUINT indexCnt, CWUINT startIndex, CWINT baseVertex)
{
	m_pD3D11DeviceContext->DrawIndexed(indexCnt, startIndex, baseVertex);
}

cwEffects* cwD3D11Device::createEffect(const string& strName)
{
	return cwEffects::create(strName, this);
}

cwVertexBuffer* cwD3D11Device::createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt)
{
	cwVertexBuffer* pVertexBuffer = cwVertexBuffer::create(uStride*uCnt);
	if (!pVertexBuffer) return NULL;

	CW_BUFFER_DESC& desc = pVertexBuffer->getBufferDesc();
	CW_BUFFER* pBuffer = pVertexBuffer->getBuffer();

	if (pData) {
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = pData;
		CW_HR(m_pD3D11Device->CreateBuffer(&desc, &subData, &pBuffer));
	}
	else {
		CW_HR(m_pD3D11Device->CreateBuffer(&desc, 0, &pBuffer));
	}

	pVertexBuffer->setBuffer(pBuffer);
	pVertexBuffer->setStride(uStride);

	return pVertexBuffer;
}

cwVertexBuffer* cwD3D11Device::createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, D3D11_USAGE usage, CWUINT cpuFlag)
{
	cwVertexBuffer* pVertexBuffer = cwVertexBuffer::create(uStride*uCnt, usage, cpuFlag);
	if (!pVertexBuffer) return NULL;
	
	CW_BUFFER_DESC& desc = pVertexBuffer->getBufferDesc();
	CW_BUFFER* pBuffer = pVertexBuffer->getBuffer();

	if (pData) {
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = pData;
		CW_HR(m_pD3D11Device->CreateBuffer(&desc, &subData, &pBuffer));
	}
	else {
		CW_HR(m_pD3D11Device->CreateBuffer(&desc, 0, &pBuffer));
	}

	pVertexBuffer->setBuffer(pBuffer);
	pVertexBuffer->setStride(uStride);

	return pVertexBuffer;
}

cwIndexBuffer* cwD3D11Device::createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt)
{
	cwIndexBuffer* pIndexBuffer = cwIndexBuffer::create(uStride*uCnt);
	if (!pIndexBuffer) return NULL;
	
	CW_BUFFER_DESC& desc = pIndexBuffer->getBufferDesc();
	CW_BUFFER* pBuffer = pIndexBuffer->getBuffer();

	if (pData) {
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = pData;
		CW_HR(m_pD3D11Device->CreateBuffer(&desc, &subData, &pBuffer));
	}
	else {
		CW_HR(m_pD3D11Device->CreateBuffer(&desc, 0, &pBuffer));
	}

	pIndexBuffer->setBuffer(pBuffer);

	return pIndexBuffer;
}

cwBlend* cwD3D11Device::createBlend(
	bool bEnable, eBlendFactor srcBlend, eBlendFactor dstBlend, eBlendOp blendOp,
	eBlendFactor srcBlendAlpha, eBlendFactor dstBlendAlpha, eBlendOp blendOpAlpha,
	eColorWriteEnable renderWriteMask)
{
	cwD3D11Blend* pBlendOp = cwD3D11Blend::create(
		bEnable, srcBlend, dstBlend, blendOp, 
		srcBlendAlpha, dstBlendAlpha, blendOpAlpha, 
		renderWriteMask);
	return pBlendOp;
}

cwStencil* cwD3D11Device::createStencil(
	bool bDepthEnable, eDepthWriteMask depthWriteMask, eComparison depthFunc,
	bool bStencilEnable, CWBYTE uReadMask, CWBYTE uWriteMask,
	eStencilOp frontFailOp, eStencilOp frontDepthFailOp, eStencilOp frontPassOp, eComparison frontFunc,
	eStencilOp backFailOp, eStencilOp backDepthFailOp, eStencilOp backPassOp, eComparison backFunc)
{
	cwD3D11Stencil* pStencil = cwD3D11Stencil::create(
		bDepthEnable, depthWriteMask, depthFunc, 
		bStencilEnable, uReadMask, uWriteMask,
		frontFailOp, frontDepthFailOp, frontPassOp, 
		frontFunc, backFailOp, backDepthFailOp, backPassOp, backFunc);
	return pStencil;
}

void cwD3D11Device::setVertexBuffer(cwVertexBuffer* pVertexBuffer)
{
	if (pVertexBuffer) {
		CWUINT stride = pVertexBuffer->getStride();
		CWUINT offset = pVertexBuffer->getOffset();
		CW_BUFFER* pBuffer = pVertexBuffer->getBuffer();
		m_pD3D11DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);
	}
}

void cwD3D11Device::setIndexBuffer(cwIndexBuffer* pIndexBuffer)
{
	if (pIndexBuffer) {
		m_pD3D11DeviceContext->IASetIndexBuffer(pIndexBuffer->getBuffer(), DXGI_FORMAT_R32_UINT, 0);
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
		m_pD3D11DeviceContext->OMSetDepthStencilState(0, 0);
		return;
	}

	ID3D11DepthStencilState* pState = static_cast<ID3D11DepthStencilState*>(const_cast<CWVOID*>(pStencil->getStencilHandlePtr()));
	if (!pState) return;
	m_pD3D11DeviceContext->OMSetDepthStencilState(pState, pStencil->getStencilRef());
}

CW_RES_LOCK_DATA cwD3D11Device::lockBuffer(cwBuffer* pBuffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	CW_HR(m_pD3D11DeviceContext->Map(pBuffer->getBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	CW_RES_LOCK_DATA lockData;
	lockData.pData = mappedData.pData;
	lockData.DepthPitch = mappedData.DepthPitch;
	lockData.RowPitch = mappedData.RowPitch;

	return lockData;
}

void cwD3D11Device::unlockBuffer(cwBuffer* pBuffer)
{
	m_pD3D11DeviceContext->Unmap(pBuffer->getBuffer(), 0);
}

cwTexture* cwD3D11Device::createTexture(const string& strFileName)
{
	cwTexture* pTexture = cwD3D11Texture::create(strFileName);
	CWAssert(pTexture != nullptr, "create texture error!");

	return pTexture;
}

void cwD3D11Device::render(cwRenderObject* pRenderObj, const cwVector3D& worldPos, cwEffects* pEffect, cwCamera* pCamera)
{
	if (!pRenderObj || !pEffect || !pCamera) return;

	cwMatrix4X4 matWorld;
	matWorld.setTranslation(worldPos);

	if (pEffect->hasVariable(CW_EFFECT_MAT_WORLD)) {
		pEffect->setVariableMatrix(CW_EFFECT_MAT_WORLD, reinterpret_cast<float*>(&matWorld));
	}

	if (pEffect->hasVariable(CW_EFFECT_MAT_WORLD_INV_TRANS)) {
		cwMatrix4X4 matWorldInvTrans = matWorld.inverse().transpose();
		pEffect->setVariableMatrix(CW_EFFECT_MAT_WORLD_INV_TRANS, reinterpret_cast<float*>(&matWorldInvTrans));
	}

	if (pEffect->hasVariable(CW_EFFECT_MATERIAL)) {
		pEffect->setVariableData(CW_EFFECT_MATERIAL, m_pMaterialDefault->getColorData(), 0, m_pMaterialDefault->getColorDataSize());
	}

	if (pEffect->hasVariable(CW_EFFECT_EYE_POSW)) {
		const cwVector3D& pos = pCamera->getPos();
		pEffect->setVariableData(CW_EFFECT_EYE_POSW, (CWVOID*)&pos, 0, sizeof(cwVector3D));
	}

	if (pEffect->hasVariable(CW_EFFECT_MAT_WORLDVIEWPROJ)) {
		cwMatrix4X4 matViewProj = pCamera->getViewProjMatrix();
		cwMatrix4X4 worldViewProj = matWorld*matViewProj;
		pEffect->setVariableMatrix(CW_EFFECT_MAT_WORLDVIEWPROJ, reinterpret_cast<CWFLOAT*>(worldViewProj.getBuffer()));
	}

	this->setInputLayout(pRenderObj->getInputLayout());
	this->setPrimitiveTopology(pRenderObj->getPrimitiveTopology());

	pRenderObj->preRender();
	pRenderObj->getVertexBuffer()->set(this);
	pRenderObj->getIndexBuffer()->set(this);

	pEffect->apply(0, 0);
	this->DrawIndexed(pRenderObj->getIndexBuffer()->getIndexCount(), 0, 0);
}

void cwD3D11Device::render(cwEntity* pEntity, cwCamera* pCamera)
{
	if (!pEntity || !pCamera) return;

	cwMaterial* pMaterial = pEntity->getMaterial();
	CWAssert(pMaterial != nullptr, "get material from entity error!");
	pMaterial->configEffect();

	cwEffects* pEffect = pMaterial->getEffect();
	CWAssert(pEffect != nullptr, "get effect from material error!");

	pEntity->transform();
	setEffectWorldTrans(pEffect, pEntity->getWorldTrans(), pCamera);
	const cwMatrix4X4& diffuseTrans = pEntity->getDiffuseTrans();
	if (pEffect->hasVariable(CW_EFFECT_DIFF_TEX_TRANS)) {
		pEffect->setVariableMatrix(CW_EFFECT_DIFF_TEX_TRANS, (CWFLOAT*)(&diffuseTrans));
	}

	cwRenderObject* pRenderObj = pEntity->getRenderObj();
	CWAssert(pRenderObj != nullptr, "get render object from entity error!");

	draw(pEffect, pMaterial->getTechName(), pRenderObj);
}

void cwD3D11Device::setEffectWorldTrans(cwEffects* pEffect, const cwMatrix4X4& trans, cwCamera* pCamera)
{
	if (!pEffect) return;

	if (pEffect->hasVariable(CW_EFFECT_MAT_WORLD)) {
		pEffect->setVariableMatrix(CW_EFFECT_MAT_WORLD, (CWFLOAT*)(&trans));
	}

	if (pEffect->hasVariable(CW_EFFECT_MAT_WORLD_INV_TRANS)) {
		if (trans.inverseExist()) {
			cwMatrix4X4 matWorldInvTrans = trans.inverse().transpose();
			pEffect->setVariableMatrix(CW_EFFECT_MAT_WORLD_INV_TRANS, reinterpret_cast<CWFLOAT*>(&matWorldInvTrans));
		}
		else {
			cwMatrix4X4& M = cwMatrix4X4::identityMatrix;
			pEffect->setVariableMatrix(CW_EFFECT_MAT_WORLD_INV_TRANS, reinterpret_cast<CWFLOAT*>(&M));
		}
	}

	if (pEffect->hasVariable(CW_EFFECT_MAT_WORLDVIEWPROJ)) {
		cwMatrix4X4 matViewProj = pCamera->getViewProjMatrix();
		cwMatrix4X4 worldViewProj = trans*matViewProj;
		pEffect->setVariableMatrix(CW_EFFECT_MAT_WORLDVIEWPROJ, reinterpret_cast<CWFLOAT*>(&worldViewProj));
	}

	if (pEffect->hasVariable(CW_EFFECT_EYE_POSW)) {
		cwVector3D pos = pCamera->getPos();
		pEffect->setVariableData(CW_EFFECT_EYE_POSW, (CWVOID*)&pos, 0, sizeof(cwVector3D));
	}
}

void cwD3D11Device::setDiffuseTrans(cwEffects* pEffect, const cwMatrix4X4& trans)
{
	if (pEffect && pEffect->hasVariable(CW_EFFECT_DIFF_TEX_TRANS)) {
		pEffect->setVariableMatrix(CW_EFFECT_DIFF_TEX_TRANS, (CWFLOAT*)(&trans));
	}
}

void cwD3D11Device::draw(cwEffects* pEffect, const string& strTech, cwRenderObject* pRenderObj)
{
	this->setInputLayout(pRenderObj->getInputLayout());
	this->setPrimitiveTopology(pRenderObj->getPrimitiveTopology());

	pRenderObj->preRender();

	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique* pTech = nullptr;

	if (strTech.empty()) {
		pTech = pEffect->getTechnique(0);
	}
	else {
		pTech = pEffect->getTechnique(strTech);
	}

	if (!pTech) return;
	pTech->GetDesc(&techDesc);

	for (CWUINT i = 0; i < techDesc.Passes; ++i) {
		this->setVertexBuffer(pRenderObj->getVertexBuffer());
		this->setIndexBuffer(pRenderObj->getIndexBuffer());

		pTech->GetPassByIndex(i)->Apply(0, m_pD3D11DeviceContext);
		this->DrawIndexed(pRenderObj->getIndexBuffer()->getIndexCount(), 0, 0);
	}
}

NS_MINI_END