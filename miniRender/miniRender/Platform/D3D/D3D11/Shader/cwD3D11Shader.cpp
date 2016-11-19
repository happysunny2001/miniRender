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

#include "cwD3D11Shader.h"
#include "Shader/cwShaderManager.h"
#include "Base/cwStringConvert.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Texture/cwTexture.h"
//#include "Texture/cwRenderTexture.h"
#include "Buffer/cwBuffer.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "cwD3D11ShaderInclude.h"

#include <iostream>

#ifdef _CW_D3D11_

NS_MINIR_BEGIN

static cwD3D11ShaderInclude shaderInclude;

cwD3D11Shader* cwD3D11Shader::create(const std::string& strShaderFile)
{
	cwD3D11Shader* pShader = new cwD3D11Shader();
	if (pShader && pShader->init(strShaderFile)) {
		pShader->autorelease();
		return pShader;
	}

	CW_SAFE_DELETE(pShader);
	return nullptr;
}

cwD3D11Shader* cwD3D11Shader::createThreadSafe(const CWSTRING& strShaderFile)
{
	cwD3D11Shader* pShader = new cwD3D11Shader();
	if (pShader && pShader->init(strShaderFile)) {
		return pShader;
	}

	CW_SAFE_DELETE(pShader);
	return nullptr;
}

cwD3D11Shader* cwD3D11Shader::create(const CWCHAR* pcSourceData, CWUINT64 uSize)
{
	cwD3D11Shader* pShader = new cwD3D11Shader();
	if (pShader && pShader->init(pcSourceData, uSize)) {
		pShader->autorelease();
		return pShader;
	}

	CW_SAFE_DELETE(pShader);
	return nullptr;
}

cwD3D11Shader* cwD3D11Shader::createThreadSafe(const CWCHAR* pcSourceData, CWUINT64 uSize)
{
	cwD3D11Shader* pShader = new cwD3D11Shader();
	if (pShader && pShader->init(pcSourceData, uSize)) {
		return pShader;
	}

	CW_SAFE_DELETE(pShader);
	return nullptr;
}

cwD3D11Shader::cwD3D11Shader()
{

}

cwD3D11Shader::~cwD3D11Shader()
{
	CW_RELEASE_COM(m_pEffect);
}

CWBOOL cwD3D11Shader::init(const std::string& strShaderFile)
{
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);

	UINT compileFlag = 0;
#if defined(CW_DEBUG)
	compileFlag |= D3D10_SHADER_DEBUG;
	compileFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	CWWSTRING wstrFileName = cwStringConvert::convertToWideChar(strShaderFile);
	ID3D10Blob* compiledShader = NULL;
	ID3D10Blob* compiledMsg = NULL;
	HRESULT hr = D3DX11CompileFromFile(
		wstrFileName.c_str(),
		NULL,
		NULL,
		NULL,
		CW_SHADER_VERSION,
		compileFlag,
		0,
		NULL,
		&compiledShader,
		&compiledMsg,
		NULL);

	if (compiledMsg != NULL) {
		MessageBoxA(NULL, (char*)compiledMsg->GetBufferPointer(), "Error", MB_OK);
		CW_RELEASE_COM(compiledMsg);
		return CWFALSE;
	}

	if (FAILED(hr)) {
		DXTrace(__FILE__, __LINE__, hr, L"D3DX11CompileFromFile", true);
		return CWFALSE;
	}

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	CW_HR(D3DX11CreateEffectFromMemory(
		compiledShader->GetBufferPointer(),
		compiledShader->GetBufferSize(),
		0,
		pD3D11Device->getD3D11Device(),
		&m_pEffect));

	CW_RELEASE_COM(compiledShader);

	if (!saveTech()) return CWFALSE;
	if (!saveVariable()) return CWFALSE;

	for (auto itTech = m_vecTech.begin(); itTech != m_vecTech.end(); ++itTech){
		if (!savePass((*itTech))) return CWFALSE;
	}

	return CWTRUE;
}

CWBOOL cwD3D11Shader::init(const CWCHAR* pcSourceData, CWUINT64 uSize)
{
	if (!pcSourceData) return CWFALSE;

	UINT compileFlag = 0;
#if defined(CW_DEBUG)
	compileFlag |= D3D10_SHADER_DEBUG;
	compileFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = NULL;
	ID3D10Blob* compiledMsg = NULL;

	HRESULT hr = D3DX11CompileFromMemory(
		pcSourceData, 
		uSize, 
		NULL, 
		NULL, 
		&shaderInclude,
		NULL, 
		CW_SHADER_VERSION, 
		compileFlag, 
		0, 
		NULL,
		&compiledShader, 
		&compiledMsg, 
		NULL);

	if (compiledMsg != NULL) {
		MessageBoxA(NULL, (char*)compiledMsg->GetBufferPointer(), "Error", MB_OK);
		CW_RELEASE_COM(compiledMsg);
		return CWFALSE;
	}

	if (FAILED(hr)) {
		DXTrace(__FILE__, __LINE__, hr, L"D3DX11CompileFromFile", true);
		return CWFALSE;
	}

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	CW_HR(D3DX11CreateEffectFromMemory(
		compiledShader->GetBufferPointer(),
		compiledShader->GetBufferSize(),
		0,
		pD3D11Device->getD3D11Device(),
		&m_pEffect));

	CW_RELEASE_COM(compiledShader);

	if (!saveTech()) return CWFALSE;
	if (!saveVariable()) return CWFALSE;

	for (auto itTech = m_vecTech.begin(); itTech != m_vecTech.end(); ++itTech){
		if (!savePass((*itTech))) return CWFALSE;
	}

	return CWTRUE;
}

CWBOOL cwD3D11Shader::saveTech()
{
	D3DX11_EFFECT_DESC effDesc;
	CW_HR(m_pEffect->GetDesc(&effDesc));

	for (CWUINT index = 0; index < effDesc.Techniques; ++index) {
		ID3DX11EffectTechnique* pTech = m_pEffect->GetTechniqueByIndex(index);
		if (!pTech) {
			return CWFALSE;
		}
		D3DX11_TECHNIQUE_DESC techDesc;
		CW_HR(pTech->GetDesc(&techDesc));

		m_vecTech.push_back(pTech);
		m_mapTech[techDesc.Name] = pTech;
	}

	return CWTRUE;
}

CWBOOL cwD3D11Shader::savePass(ID3DX11EffectTechnique* pTech)
{
	if (!pTech) return CWFALSE;

	D3DX11_TECHNIQUE_DESC techDesc;
	CW_HR(pTech->GetDesc(&techDesc));

	vector<ID3DX11EffectPass*> vecPass;
	unordered_map<string, ID3DX11EffectPass*> mapPass;

	for (CWUINT index = 0; index < techDesc.Passes; ++index) {
		ID3DX11EffectPass* pPass = pTech->GetPassByIndex(index);
		if (!pPass) return CWFALSE;

		D3DX11_PASS_DESC passDesc;
		CW_HR(pPass->GetDesc(&passDesc));

		vecPass.push_back(pPass);
		mapPass[passDesc.Name] = pPass;
	}

	m_mapIndexPass[pTech] = vecPass;
	m_mapNamePass[pTech] = mapPass;

	return CWTRUE;
}

CWBOOL cwD3D11Shader::saveVariable()
{
	D3DX11_EFFECT_DESC effDesc;
	CW_HR(m_pEffect->GetDesc(&effDesc));

	for (CWUINT i = 0; i < effDesc.GlobalVariables; ++i) {
		ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByIndex(i);
		if (!pVariable) continue;
		D3DX11_EFFECT_VARIABLE_DESC desc;
		pVariable->GetDesc(&desc);

		m_mapVariable[desc.Name] = pVariable;
		//OutputDebugStringA(desc.Name);
		//OutputDebugStringA("\n");
	}

	cwShaderManager* pShaderManager = cwRepertory::getInstance().getShaderManager();
	memset(m_pShaderParam, 0, sizeof(ID3DX11EffectVariable*)*eShaderParamMax);
	for (int i = (int)eShaderParamWorldViewProj; i < (int)eShaderParamMax; ++i) {
		if (this->hasVariable(pShaderManager->getShaderParamString((eShaderParamIndex)i))) {
			m_pShaderParam[i] = m_mapVariable[pShaderManager->getShaderParamString((eShaderParamIndex)i)];
		}
	}

	return CWTRUE;
}

CWBOOL cwD3D11Shader::hasVariable(const string& strVariable)
{
	if (m_mapVariable.find(strVariable) != m_mapVariable.end()) return CWTRUE;
	return CWFALSE;
}

CWVOID cwD3D11Shader::setVariableData(const string& strVariable, CWVOID* pData, CWUINT offset, CWUINT iSize)
{
	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()) {
		ID3DX11EffectVariable* pVariable = itVariable->second;
		pVariable->SetRawValue(pData, offset, iSize);
	}
}

CWVOID cwD3D11Shader::setVariableData(const string& strVariable, CWUINT index, CWVOID* pData, CWUINT offset, CWUINT iSize)
{
	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()) {
		ID3DX11EffectVariable* pVariable = itVariable->second;
		ID3DX11EffectVariable* pVariElement = pVariable->GetElement(index);
		if (pVariElement) {
			pVariElement->SetRawValue(pData, offset, iSize);
		}
	}
}

CWVOID cwD3D11Shader::setVariableMatrix(const string& strVariable, CWFLOAT* pData)
{
	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()) {
		ID3DX11EffectVariable* pVariable = itVariable->second;
		pVariable->AsMatrix()->SetMatrix(pData);
	}
}

CWVOID cwD3D11Shader::setVariableMatrix(const CWSTRING& strVariable, const cwMatrix4X4& mat)
{
	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()) {
		ID3DX11EffectVariable* pVariable = itVariable->second;
		//const CWFLOAT* f = &(mat.m11);
		//pVariable->AsMatrix()->SetMatrix(const_cast<CWFLOAT*>(f));
		pVariable->AsMatrix()->SetMatrix(const_cast<float*>(mat.getBuffer()));
	}
}

CWVOID cwD3D11Shader::setVariableInt(const string& strVariable, CWINT value)
{
	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()) {
		ID3DX11EffectVariable* pVariable = itVariable->second;
		CW_HR(pVariable->AsScalar()->SetInt(value));
	}
}

CWVOID cwD3D11Shader::setVariableFloat(const string& strVariable, CWFLOAT value)
{
	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()) {
		ID3DX11EffectVariable* pVariable = itVariable->second;
		CW_HR(pVariable->AsScalar()->SetFloat(value));
	}
}

CWVOID cwD3D11Shader::setVariableFloatArray(const string& strVariable, CWFLOAT* pData, CWUINT count)
{
	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()) {
		ID3DX11EffectVariable* pVariable = itVariable->second;
		//auto v = pVariable->AsScalar();
		CW_HR(pVariable->SetRawValue(pData, 0, sizeof(CWFLOAT)*count));
		//CW_HR(v->SetFloatArray(pData, 0, count));
	}
}

CWVOID cwD3D11Shader::setVariableTexture(const string& strVariable, const cwTexture* pTexture)
{
	if (!pTexture) return;
	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()) {
		ID3D11ShaderResourceView* pShaderRes = reinterpret_cast<ID3D11ShaderResourceView*>(pTexture->getHandle());
		if (pShaderRes) {
			ID3DX11EffectVariable* pVariable = itVariable->second;
			CW_HR(pVariable->AsShaderResource()->SetResource(pShaderRes));
		}
	}
}

CWVOID cwD3D11Shader::setVariableTextureWritable(const CWSTRING& strVariable, const cwTexture* pTexture)
{
	if (!pTexture) return;
	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()) {
		ID3D11UnorderedAccessView* pShaderRes = reinterpret_cast<ID3D11UnorderedAccessView*>(pTexture->getWritablehandle());
		if (pShaderRes) {
			ID3DX11EffectVariable* pVariable = itVariable->second;
			CW_HR(pVariable->AsUnorderedAccessView()->SetUnorderedAccessView(pShaderRes));
		}
	}
}

//CWVOID cwD3D11Shader::setVariableTextureWritable(const CWSTRING& strVariable, const cwTexture* pTexture)
//{
//	if (!pTexture) return;
//	CWHANDLE pHandle = pTexture->getWritablehandle();
//	if (!pHandle) return;
//	auto itVariable = m_mapVariable.find(strVariable);
//	if (itVariable != m_mapVariable.end()) {
//		ID3D11UnorderedAccessView* pShaderRes = reinterpret_cast<ID3D11UnorderedAccessView*>(pHandle);
//		if (pShaderRes) {
//			ID3DX11EffectVariable* pVariable = itVariable->second;
//			CW_HR(pVariable->AsUnorderedAccessView()->SetUnorderedAccessView(pShaderRes));
//		}
//	}
//}

CWVOID cwD3D11Shader::setVariableBuffer(const CWSTRING& strVariable, cwBuffer* pBuffer)
{
	if (!pBuffer) return;

	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()){
		ID3D11ShaderResourceView* pShaderRes = reinterpret_cast<ID3D11ShaderResourceView*>(pBuffer->getShaderHandle());
		if (pShaderRes) {
			ID3DX11EffectVariable* pVariable = itVariable->second;
			CW_HR(pVariable->AsShaderResource()->SetResource(pShaderRes));
		}
	}
}

CWVOID cwD3D11Shader::setVariableBufferWritable(const CWSTRING& strVariable, cwBuffer* pBuffer)
{
	if (!pBuffer) return;

	auto itVariable = m_mapVariable.find(strVariable);
	if (itVariable != m_mapVariable.end()){
		ID3D11UnorderedAccessView* pShaderRes = reinterpret_cast<ID3D11UnorderedAccessView*>(pBuffer->getWritableHandle());
		if (pShaderRes) {
			ID3DX11EffectVariable* pVariable = itVariable->second;
			CW_HR(pVariable->AsUnorderedAccessView()->SetUnorderedAccessView(pShaderRes));
		}
	}
}

CWBOOL cwD3D11Shader::hasVariable(eShaderParamIndex eParam)
{
	return m_pShaderParam[eParam] != nullptr;
}

CWVOID cwD3D11Shader::setVariableData(eShaderParamIndex eParam, CWVOID* pData, CWUINT offset, CWUINT iSize)
{
	if (m_pShaderParam[eParam])
		m_pShaderParam[eParam]->SetRawValue(pData, offset, iSize);
}

CWVOID cwD3D11Shader::setVariableData(eShaderParamIndex eParam, CWUINT index, CWVOID* pData, CWUINT offset, CWUINT iSize)
{
	if (m_pShaderParam[eParam]) {
		ID3DX11EffectVariable* pVariElement = m_pShaderParam[eParam]->GetElement(index);
		if (pVariElement) {
			pVariElement->SetRawValue(pData, offset, iSize);
		}
	}
}

CWVOID cwD3D11Shader::setVariableMatrix(eShaderParamIndex eParam, CWFLOAT* pData)
{
	if (m_pShaderParam[eParam])
		m_pShaderParam[eParam]->AsMatrix()->SetMatrix(pData);
}

CWVOID cwD3D11Shader::setVariableMatrix(eShaderParamIndex eParam, const cwMatrix4X4& mat)
{
	if (m_pShaderParam[eParam])
		m_pShaderParam[eParam]->AsMatrix()->SetMatrix(const_cast<float*>(mat.getBuffer()));
}

CWVOID cwD3D11Shader::setVariableInt(eShaderParamIndex eParam, CWINT value)
{
	if (m_pShaderParam[eParam])
		CW_HR(m_pShaderParam[eParam]->AsScalar()->SetInt(value));
}

CWVOID cwD3D11Shader::setVariableFloat(eShaderParamIndex eParam, CWFLOAT value)
{
	if (m_pShaderParam[eParam])
		CW_HR(m_pShaderParam[eParam]->AsScalar()->SetFloat(value));
}

CWVOID cwD3D11Shader::setVariableFloatArray(eShaderParamIndex eParam, CWFLOAT* pData, CWUINT count)
{
	if (m_pShaderParam[eParam]) {
		CW_HR(m_pShaderParam[eParam]->SetRawValue(pData, 0, sizeof(CWFLOAT)*count));
		//CW_HR(m_pShaderParam[eParam]->AsScalar()->SetFloatArray(pData, 0, count));
	}
}

CWVOID cwD3D11Shader::setVariableTexture(eShaderParamIndex eParam, const cwTexture* pTexture)
{
	if (pTexture && m_pShaderParam[eParam]) {
		ID3D11ShaderResourceView* pShaderRes = reinterpret_cast<ID3D11ShaderResourceView*>(pTexture->getHandle());
		if (pShaderRes) {
			CW_HR(m_pShaderParam[eParam]->AsShaderResource()->SetResource(pShaderRes));
		}
	}
}

CWVOID cwD3D11Shader::setVariableTextureWritable(eShaderParamIndex eParam, const cwTexture* pTexture)
{
	if (pTexture && m_pShaderParam[eParam]) {
		ID3D11UnorderedAccessView* pShaderRes = reinterpret_cast<ID3D11UnorderedAccessView*>(pTexture->getWritablehandle());
		if (pShaderRes) {
			CW_HR(m_pShaderParam[eParam]->AsUnorderedAccessView()->SetUnorderedAccessView(pShaderRes));
		}
	}
}

CWVOID cwD3D11Shader::setVariableBuffer(eShaderParamIndex eParam, cwBuffer* pBuffer)
{
	if (pBuffer && m_pShaderParam[eParam]) {
		ID3D11ShaderResourceView* pShaderRes = reinterpret_cast<ID3D11ShaderResourceView*>(pBuffer->getShaderHandle());
		if (pShaderRes) {
			CW_HR(m_pShaderParam[eParam]->AsShaderResource()->SetResource(pShaderRes));
		}
	}
}

CWVOID cwD3D11Shader::setVariableBufferWritable(eShaderParamIndex eParam, cwBuffer* pBuffer)
{
	if (pBuffer && m_pShaderParam[eParam]) {
		ID3D11UnorderedAccessView* pShaderRes = reinterpret_cast<ID3D11UnorderedAccessView*>(pBuffer->getShaderHandle());
		if (pShaderRes) {
			CW_HR(m_pShaderParam[eParam]->AsUnorderedAccessView()->SetUnorderedAccessView(pShaderRes));
		}
	}
}

void cwD3D11Shader::apply(CWUINT techIndex, CWUINT passIndex)
{
	ID3DX11EffectPass* pPass = getPass(techIndex, passIndex);
	if (pPass) {
		cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
		pPass->Apply(0, pD3D11Device->getD3D11DeviceContext());
	}
}

ID3DX11EffectTechnique* cwD3D11Shader::getTechnique(CWUINT index)
{
	if (index >= m_vecTech.size()) return nullptr;
	return m_vecTech[index];
}

ID3DX11EffectTechnique* cwD3D11Shader::getTechnique(const string& strTechName)
{
	if (m_mapTech.find(strTechName) == m_mapTech.end()) return nullptr;
	return m_mapTech[strTechName];
}

ID3DX11EffectPass* cwD3D11Shader::getPass(CWUINT techIndex, CWUINT passIndex)
{
	ID3DX11EffectTechnique* pTech = getTechnique(techIndex);
	if (!pTech) return nullptr;

	vector<ID3DX11EffectPass*>& vecPassRef = m_mapIndexPass[pTech];
	if (passIndex >= vecPassRef.size()) return nullptr;
	return vecPassRef[passIndex];
}

ID3DX11EffectPass* cwD3D11Shader::getPass(const string& strTechName, const string& strPassName)
{
	ID3DX11EffectTechnique* pTech = getTechnique(strTechName);
	if (!pTech) return nullptr;

	unordered_map<string, ID3DX11EffectPass*>& mapPassRef = m_mapNamePass[pTech];
	if (mapPassRef.find(strPassName) == mapPassRef.end()) return nullptr;
	return mapPassRef[strPassName];
}

ID3DX11EffectPass* cwD3D11Shader::getPass(CWUINT techIndex, const string& strPassName)
{
	ID3DX11EffectTechnique* pTech = getTechnique(techIndex);
	if (!pTech) return nullptr;

	unordered_map<string, ID3DX11EffectPass*>& mapPassRef = m_mapNamePass[pTech];
	if (mapPassRef.find(strPassName) == mapPassRef.end()) return nullptr;
	return mapPassRef[strPassName];
}

ID3DX11EffectPass* cwD3D11Shader::getPass(const string& strTechName, CWUINT passIndex)
{
	ID3DX11EffectTechnique* pTech = getTechnique(strTechName);
	if (!pTech) return nullptr;

	vector<ID3DX11EffectPass*>& vecPassRef = m_mapIndexPass[pTech];
	if (passIndex >= vecPassRef.size()) return nullptr;
	return vecPassRef[passIndex];
}

NS_MINIR_END

#endif







