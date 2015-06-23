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

#include "Base/cwMacros.h"
#include "Shader/cwShader.h"

#ifdef _CW_D3D11_

NS_MINI_BEGIN

class cwD3D11Shader : public cwShader
{
public:
	static cwD3D11Shader* create(const std::string& strShaderFile);

	cwD3D11Shader();
	virtual ~cwD3D11Shader();

	virtual bool init(const std::string& strShaderFile) override;

	virtual void setVariableData(const string& strVariable, void* pData, CWUINT offset, CWUINT iSize) override;
	virtual void setVariableData(const string& strVariable, CWUINT index, void* pData, CWUINT offset, CWUINT iSize) override;
	virtual void setVariableMatrix(const string& strVariable, CWFLOAT* pData) override;
	virtual void setVariableInt(const string& strVariable, CWINT value) override;
	virtual void setVariableFloat(const string& strVariable, CWFLOAT value) override;
	virtual void setVariableFloatArray(const string& strVariable, CWFLOAT* pData, CWUINT count) override;
	virtual void setVariableTexture(const string& strVariable, cwTexture* pTexture) override;

	virtual void apply(CWUINT techIndex, CWUINT passIndex) override;

protected:
	bool saveTech();
	bool savePass(ID3DX11EffectTechnique* pTech);
	bool saveVariable();

	ID3DX11Effect* getEffect() { return m_pEffect; }
	ID3DX11EffectTechnique* getTechnique(CWUINT index);
	ID3DX11EffectTechnique* getTechnique(const string& strTechName);
	ID3DX11EffectPass* getPass(CWUINT techIndex, CWUINT passIndex);
	ID3DX11EffectPass* getPass(const string& strTechName, const string& strPassName);
	ID3DX11EffectPass* getPass(CWUINT techIndex, const string& strPassName);
	ID3DX11EffectPass* getPass(const string& strTechName, CWUINT passIndex);

protected:
	ID3DX11Effect* m_pEffect;
	vector<ID3DX11EffectTechnique*> m_vecTech;
	unordered_map<string, ID3DX11EffectTechnique*> m_mapTech;

	unordered_map<ID3DX11EffectTechnique*, vector<ID3DX11EffectPass*> > m_mapIndexPass;
	unordered_map<ID3DX11EffectTechnique*, unordered_map<string, ID3DX11EffectPass*> > m_mapNamePass;

	unordered_map<string, ID3DX11EffectVariable*> m_mapVariable;

};

NS_MINI_END

#endif
