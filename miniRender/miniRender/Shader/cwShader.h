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
#include "Ref/cwRef.h"
#include "cwShaderConstant.h"

#include <unordered_map>

NS_MINI_BEGIN

class cwTexture;

class CW_DLL cwShader : public cwRef
{
public:
	cwShader();
	virtual ~cwShader();

	virtual bool init(const CWSTRING& strShaderFile) = 0;

	virtual bool hasVariable(const string& strVariable) = 0;
	virtual void setVariableData(const string& strVariable, void* pData, CWUINT offset, CWUINT iSize) = 0;
	virtual void setVariableData(const string& strVariable, CWUINT index, void* pData, CWUINT offset, CWUINT iSize) = 0;
	virtual void setVariableMatrix(const string& strVariable, CWFLOAT* pData) = 0;
	virtual void setVariableInt(const string& strVariable, CWINT value) = 0;
	virtual void setVariableFloat(const string& strVariable, CWFLOAT value) = 0;
	virtual void setVariableFloatArray(const string& strVariable, CWFLOAT* pData, CWUINT count) = 0;
	virtual void setVariableTexture(const string& strVariable, cwTexture* pTexture) = 0;

	virtual void apply(CWUINT techIndex, CWUINT passIndex) = 0;

protected:

};

NS_MINI_END

#endif