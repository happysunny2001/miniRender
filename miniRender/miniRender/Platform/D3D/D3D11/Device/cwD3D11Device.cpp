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
#include "Stencil/cwStencil.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"
#include "Platform/D3D/D3D11/Layouts/cwD3D11Layouts.h"
#include "Platform/D3D/D3D11/Buffer/cwD3D11VertexBuffer.h"
#include "Platform/D3D/D3D11/Buffer/cwD3D11IndexBuffer.h"
#include "Platform/D3D/D3D11/Stencil/cwD3D11Stencil.h"
#include "Platform/D3D/D3D11/Texture/cwD3D11Texture.h"
#include "Platform/D3D/D3D11/Blend/cwD3D11Blend.h"
#include "Platform/D3D/D3D11/Shader/cwD3D11Shader.h"

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
m_pD3D11RenderTarget(NULL),
m_pDepthStencilBuffer(NULL),
m_pDepthStencilView(NULL),
m_pSolidRenderState(NULL),
m_pWireRenderState(NULL),
m_pNoCullRenderState(NULL),
m_pCullCWRenderState(NULL),
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

	resize(winWidth, winHeight);

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
	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = winWidth;
	texDesc.Height = winHeight;
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

	//m_uiClientWidth  = width;
	//m_uiClientHeight = height;

	CW_HR(m_pDxgiSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	createRenderTarget();
	createDepthStencil();
	m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pD3D11RenderTarget, m_pDepthStencilView);

	createViewPort();
}

void cwD3D11Device::resize()
{
	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	resize(winWidth, winHeight);
}

//void cwD3D11Device::setSize(CWUINT width, CWUINT height)
//{
//	m_uiClientWidth  = width;
//	m_uiClientHeight = height;
//}

void cwD3D11Device::setClearColor(const cwVector4D& fvColor)
{
	m_fvClearColor = fvColor;
}

void cwD3D11Device::createViewPort()
{
	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	D3D11_VIEWPORT viewPort;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = (float)winWidth;
	viewPort.Height = (float)winHeight;
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

void cwD3D11Device::DrawIndexed(CWUINT indexCnt, CWUINT startIndex, CWINT baseVertex)
{
	m_pD3D11DeviceContext->DrawIndexed(indexCnt, startIndex, baseVertex);
}

cwShader* cwD3D11Device::createShader(const string& strFileName)
{
	return cwD3D11Shader::create(strFileName);
}

cwBuffer* cwD3D11Device::createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt)
{
	cwD3D11VertexBuffer* pVertexBuffer = cwD3D11VertexBuffer::create(uStride*uCnt);
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
	cwD3D11IndexBuffer* pIndexBuffer = cwD3D11IndexBuffer::create(uStride*uCnt);
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

cwBlend* cwD3D11Device::createBlend(const BlendData& blendData)
{
	return cwD3D11Blend::create(blendData);
}

cwStencil* cwD3D11Device::createStencil(const StencilData& stencliData)
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
	ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(pBuffer->getBuffer());
	CW_HR(m_pD3D11DeviceContext->Map(pD3D11Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	CW_RES_LOCK_DATA lockData;
	lockData.pData = mappedData.pData;
	lockData.DepthPitch = mappedData.DepthPitch;
	lockData.RowPitch = mappedData.RowPitch;

	return lockData;
}

void cwD3D11Device::unlockBuffer(cwBuffer* pBuffer)
{
	ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(pBuffer->getBuffer());
	m_pD3D11DeviceContext->Unmap(pD3D11Buffer, 0);
}

cwTexture* cwD3D11Device::createTexture(const string& strFileName)
{
	cwTexture* pTexture = cwD3D11Texture::create(strFileName);
	return pTexture;
}

void cwD3D11Device::render(cwRenderObject* pRenderObj, const cwVector3D& worldPos, cwShader* pShader, cwCamera* pCamera)
{
	if (!pRenderObj || !pShader || !pCamera) return;

	cwMatrix4X4 matWorld;
	matWorld.setTranslation(worldPos);

	if (pShader->hasVariable(CW_SHADER_MAT_WORLD)) {
		pShader->setVariableMatrix(CW_SHADER_MAT_WORLD, reinterpret_cast<float*>(&matWorld));
	}

	if (pShader->hasVariable(CW_SHADER_MAT_WORLD_INV_TRANS)) {
		cwMatrix4X4 matWorldInvTrans = matWorld.inverse().transpose();
		pShader->setVariableMatrix(CW_SHADER_MAT_WORLD_INV_TRANS, reinterpret_cast<float*>(&matWorldInvTrans));
	}

	if (pShader->hasVariable(CW_SHADER_MATERIAL)) {
		pShader->setVariableData(CW_SHADER_MATERIAL, m_pMaterialDefault->getColorData(), 0, m_pMaterialDefault->getColorDataSize());
	}

	if (pShader->hasVariable(CW_SHADER_EYE_POSW)) {
		const cwVector3D& pos = pCamera->getPos();
		pShader->setVariableData(CW_SHADER_EYE_POSW, (CWVOID*)&pos, 0, sizeof(cwVector3D));
	}

	if (pShader->hasVariable(CW_SHADER_MAT_WORLDVIEWPROJ)) {
		cwMatrix4X4 matViewProj = pCamera->getViewProjMatrix();
		cwMatrix4X4 worldViewProj = matWorld*matViewProj;
		pShader->setVariableMatrix(CW_SHADER_MAT_WORLDVIEWPROJ, reinterpret_cast<CWFLOAT*>(worldViewProj.getBuffer()));
	}

	pRenderObj->preRender();

	this->setInputLayout(pRenderObj->getInputLayout());
	this->setPrimitiveTopology(pRenderObj->getPrimitiveTopology());
	this->setVertexBuffer(pRenderObj->getVertexBuffer());
	this->setIndexBuffer(pRenderObj->getIndexBuffer());

	pShader->apply(0, 0);
	this->DrawIndexed(pRenderObj->getIndexBuffer()->getIndexCount(), 0, 0);
}

void cwD3D11Device::render(cwEntity* pEntity, cwCamera* pCamera)
{
	if (!pEntity || !pCamera) return;

	cwMaterial* pMaterial = pEntity->getMaterial();
	assert(pMaterial != nullptr);
	pMaterial->configEffect();

	cwShader* pShader = pMaterial->getShader();
	assert(pShader != nullptr);
	setShaderWorldTrans(pShader, pEntity->getTransformMatrix(), pCamera);

	cwRenderObject* pRenderObj = pEntity->getRenderObj();
	assert(pRenderObj != nullptr);
	draw(pShader, pMaterial->getTechName(), pRenderObj);
}

void cwD3D11Device::setShaderWorldTrans(cwShader* pShader, const cwMatrix4X4& trans, cwCamera* pCamera)
{
	if (!pShader) return;

	if (pShader->hasVariable(CW_SHADER_MAT_WORLD)) {
		pShader->setVariableMatrix(CW_SHADER_MAT_WORLD, (CWFLOAT*)(&trans));
	}

	if (pShader->hasVariable(CW_SHADER_MAT_WORLD_INV_TRANS)) {
		if (trans.inverseExist()) {
			cwMatrix4X4 matWorldInvTrans = trans.inverse().transpose();
			pShader->setVariableMatrix(CW_SHADER_MAT_WORLD_INV_TRANS, reinterpret_cast<CWFLOAT*>(&matWorldInvTrans));
		}
		else {
			cwMatrix4X4& M = cwMatrix4X4::identityMatrix;
			pShader->setVariableMatrix(CW_SHADER_MAT_WORLD_INV_TRANS, reinterpret_cast<CWFLOAT*>(&M));
		}
	}

	if (pShader->hasVariable(CW_SHADER_MAT_WORLDVIEWPROJ)) {
		cwMatrix4X4 matViewProj = pCamera->getViewProjMatrix();
		cwMatrix4X4 worldViewProj = trans*matViewProj;
		pShader->setVariableMatrix(CW_SHADER_MAT_WORLDVIEWPROJ, reinterpret_cast<CWFLOAT*>(&worldViewProj));
	}

	if (pShader->hasVariable(CW_SHADER_EYE_POSW)) {
		cwVector3D pos = pCamera->getPos();
		pShader->setVariableData(CW_SHADER_EYE_POSW, (CWVOID*)&pos, 0, sizeof(cwVector3D));
	}
}

void cwD3D11Device::setDiffuseTrans(cwShader* pShader, const cwMatrix4X4& trans)
{
	if (pShader && pShader->hasVariable(CW_SHADER_DIFF_TEX_TRANS)) {
		pShader->setVariableMatrix(CW_SHADER_DIFF_TEX_TRANS, (CWFLOAT*)(&trans));
	}
}

void cwD3D11Device::draw(cwShader* pShader, const string& strTech, cwRenderObject* pRenderObj)
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

	for (CWUINT i = 0; i < techDesc.Passes; ++i) {
		this->setVertexBuffer(pRenderObj->getVertexBuffer());
		this->setIndexBuffer(pRenderObj->getIndexBuffer());

		pTech->GetPassByIndex(i)->Apply(0, m_pD3D11DeviceContext);
		this->DrawIndexed(pRenderObj->getIndexBuffer()->getIndexCount(), 0, 0);
	}
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
