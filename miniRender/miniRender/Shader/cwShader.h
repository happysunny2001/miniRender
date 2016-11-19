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

#ifndef __CW_SHADER_H__
#define __CW_SHADER_H__

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "Ref/cwRef.h"
#include "cwShaderConstant.h"

#include <unordered_map>

NS_MINIR_BEGIN

class cwTexture;
//class cwRenderTexture;
class cwBuffer;
class cwMatrix4X4;

class CW_DLL cwShader : public cwRef
{
public:
	cwShader();
	virtual ~cwShader();

	virtual CWBOOL init(const CWSTRING& strShaderFile) = 0;
	virtual CWBOOL init(const CWCHAR* pcSourceData, CWUINT64 uSize) = 0;

	virtual CWBOOL hasVariable(const CWSTRING& strVariable) = 0;
	virtual CWVOID setVariableData(const CWSTRING& strVariable, CWVOID* pData, CWUINT offset, CWUINT iSize) = 0;
	virtual CWVOID setVariableData(const CWSTRING& strVariable, CWUINT index, CWVOID* pData, CWUINT offset, CWUINT iSize) = 0;
	virtual CWVOID setVariableMatrix(const CWSTRING& strVariable, CWFLOAT* pData) = 0;
	virtual CWVOID setVariableMatrix(const CWSTRING& strVariable, const cwMatrix4X4& mat) = 0;
	virtual CWVOID setVariableInt(const CWSTRING& strVariable, CWINT value) = 0;
	virtual CWVOID setVariableFloat(const CWSTRING& strVariable, CWFLOAT value) = 0;
	virtual CWVOID setVariableFloatArray(const CWSTRING& strVariable, CWFLOAT* pData, CWUINT count) = 0;
	virtual CWVOID setVariableTexture(const CWSTRING& strVariable, const cwTexture* pTexture) = 0;
	virtual CWVOID setVariableTextureWritable(const CWSTRING& strVariable, const cwTexture* pTexture) = 0;
	virtual CWVOID setVariableBuffer(const CWSTRING& strVariable, cwBuffer* pBuffer) = 0;
	virtual CWVOID setVariableBufferWritable(const CWSTRING& strVariable, cwBuffer* pBuffer) = 0;

	virtual CWBOOL hasVariable(eShaderParamIndex eParam) = 0;
	virtual CWVOID setVariableData(eShaderParamIndex eParam, CWVOID* pData, CWUINT offset, CWUINT iSize) = 0;
	virtual CWVOID setVariableData(eShaderParamIndex eParam, CWUINT index, CWVOID* pData, CWUINT offset, CWUINT iSize) = 0;
	virtual CWVOID setVariableMatrix(eShaderParamIndex eParam, CWFLOAT* pData) = 0;
	virtual CWVOID setVariableMatrix(eShaderParamIndex eParam, const cwMatrix4X4& mat) = 0;
	virtual CWVOID setVariableInt(eShaderParamIndex eParam, CWINT value) = 0;
	virtual CWVOID setVariableFloat(eShaderParamIndex eParam, CWFLOAT value) = 0;
	virtual CWVOID setVariableFloatArray(eShaderParamIndex eParam, CWFLOAT* pData, CWUINT count) = 0;
	virtual CWVOID setVariableTexture(eShaderParamIndex eParam, const cwTexture* pTexture) = 0;
	virtual CWVOID setVariableTextureWritable(eShaderParamIndex eParam, const cwTexture* pTexture) = 0;
	virtual CWVOID setVariableBuffer(eShaderParamIndex eParam, cwBuffer* pBuffer) = 0;
	virtual CWVOID setVariableBufferWritable(eShaderParamIndex eParam, cwBuffer* pBuffer) = 0;

	virtual CWVOID apply(CWUINT techIndex, CWUINT passIndex) = 0;

	inline CWVOID setName(const CWSTRING& strName) { m_nStrName = strName; }
	inline const CWSTRING& getName() const { return m_nStrName; }

protected:
	CWSTRING m_nStrName;

};

NS_MINIR_END

#endif
