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

#ifndef __CW_D3D11_SHADER_H__
#define __CW_D3D11_SHADER_H__

#ifdef _CW_D3D11_

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "Shader/cwShader.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

#define CW_SHADER_VERSION "fx_5_0"

class CW_DLL cwD3D11Shader : public cwShader
{
public:
	static cwD3D11Shader* create(const CWSTRING& strShaderFile);

	cwD3D11Shader();
	virtual ~cwD3D11Shader();

	virtual CWBOOL init(const CWSTRING& strShaderFile) override;

	virtual CWBOOL hasVariable(const string& strVariable) override;
	virtual CWVOID setVariableData(const string& strVariable, void* pData, CWUINT offset, CWUINT iSize) override;
	virtual CWVOID setVariableData(const string& strVariable, CWUINT index, void* pData, CWUINT offset, CWUINT iSize) override;
	virtual CWVOID setVariableMatrix(const string& strVariable, CWFLOAT* pData) override;
	virtual CWVOID setVariableInt(const string& strVariable, CWINT value) override;
	virtual CWVOID setVariableFloat(const string& strVariable, CWFLOAT value) override;
	virtual CWVOID setVariableFloatArray(const string& strVariable, CWFLOAT* pData, CWUINT count) override;
	virtual CWVOID setVariableTexture(const string& strVariable, cwTexture* pTexture) override;
	virtual CWVOID setVariableTextureWritable(const CWSTRING& strVariable, cwRenderTexture* pTexture) override;

	virtual CWBOOL hasVariable(eShaderParamIndex eParam) override;
	virtual CWVOID setVariableData(eShaderParamIndex eParam, CWVOID* pData, CWUINT offset, CWUINT iSize) override;
	virtual CWVOID setVariableData(eShaderParamIndex eParam, CWUINT index, CWVOID* pData, CWUINT offset, CWUINT iSize) override;
	virtual CWVOID setVariableMatrix(eShaderParamIndex eParam, CWFLOAT* pData) override;
	virtual CWVOID setVariableInt(eShaderParamIndex eParam, CWINT value) override;
	virtual CWVOID setVariableFloat(eShaderParamIndex eParam, CWFLOAT value) override;
	virtual CWVOID setVariableFloatArray(eShaderParamIndex eParam, CWFLOAT* pData, CWUINT count) override;
	virtual CWVOID setVariableTexture(eShaderParamIndex eParam, cwTexture* pTexture) override;
	virtual CWVOID setVariableTextureWritable(eShaderParamIndex eParam, cwRenderTexture* pTexture) override;

	virtual CWVOID apply(CWUINT techIndex, CWUINT passIndex) override;

	ID3DX11Effect* getEffect() { return m_pEffect; }
	ID3DX11EffectTechnique* getTechnique(CWUINT index);
	ID3DX11EffectTechnique* getTechnique(const string& strTechName);
	ID3DX11EffectPass* getPass(CWUINT techIndex, CWUINT passIndex);
	ID3DX11EffectPass* getPass(const string& strTechName, const string& strPassName);
	ID3DX11EffectPass* getPass(CWUINT techIndex, const string& strPassName);
	ID3DX11EffectPass* getPass(const string& strTechName, CWUINT passIndex);

protected:
	CWBOOL saveTech();
	CWBOOL savePass(ID3DX11EffectTechnique* pTech);
	CWBOOL saveVariable();

protected:
	ID3DX11Effect* m_pEffect;
	vector<ID3DX11EffectTechnique*> m_vecTech;
	unordered_map<string, ID3DX11EffectTechnique*> m_mapTech;

	unordered_map<ID3DX11EffectTechnique*, vector<ID3DX11EffectPass*> > m_mapIndexPass;
	unordered_map<ID3DX11EffectTechnique*, unordered_map<string, ID3DX11EffectPass*> > m_mapNamePass;

	//shader variable map
	unordered_map<string, ID3DX11EffectVariable*> m_mapVariable;
	ID3DX11EffectVariable* m_pShaderParam[eShaderParamMax];

};

NS_MINIR_END

#endif

#endif
