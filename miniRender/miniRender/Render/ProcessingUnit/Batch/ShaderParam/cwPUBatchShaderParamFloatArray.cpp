/*
Copyright © 2015-2016 Ziwei Wang

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

#include "cwPUBatchShaderParamFloatArray.h"
#include "Shader/cwShader.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Camera/cwCamera.h"

NS_MINIR_BEGIN

cwPUBatchShaderParamFloatArray* cwPUBatchShaderParamFloatArray::create()
{
	cwPUBatchShaderParamFloatArray* pPUBatch = new cwPUBatchShaderParamFloatArray();
	if (pPUBatch) {
		pPUBatch->autorelease();
	}

	return pPUBatch;
}

cwPUBatchShaderParamFloatArray::cwPUBatchShaderParamFloatArray() :
m_pfValues(nullptr),
m_iCnt(0)
{
	m_eType = eShaderParamTypeFloatArray;
}

cwPUBatchShaderParamFloatArray::~cwPUBatchShaderParamFloatArray()
{
	CW_SAFE_DELETE_ARRAY(m_pfValues);
	m_iCnt = 0;
}

CWVOID cwPUBatchShaderParamFloatArray::setValues(CWFLOAT* pfValues, CWUINT iCnt)
{
	if (pfValues && iCnt > 0) {
		CW_SAFE_DELETE_ARRAY(m_pfValues);
		m_pfValues = new CWFLOAT[iCnt];
		memcpy(m_pfValues, pfValues, sizeof(CWFLOAT)*iCnt);
		m_iCnt = iCnt;
	}
}

CWVOID cwPUBatchShaderParamFloatArray::updateShaderParam()
{
	if (m_eSource == eShaderParamSourceData) {
		if (m_pShader && m_pfValues && !m_nShaderParamName.empty()) {
			m_pShader->setVariableFloatArray(m_nShaderParamName, m_pfValues, m_iCnt);
		}
	}
	else {
		CWUINT iCnt = 0;
		CWFLOAT* pf = this->getEngineFloatArray(iCnt);
		if (pf) {
			m_pShader->setVariableFloatArray(m_nShaderParamName, pf, iCnt);
		}
	}
}

CWFLOAT* cwPUBatchShaderParamFloatArray::getEngineFloatArray(CWUINT& iCnt)
{
	iCnt = 0;
	if (m_nEngineParamType == "Camera") {
		cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera(m_nEngineParamName);
		if (pCamera) {
			if (m_nEngineParamAttr == "Position") {
				iCnt = 3;
				const cwVector3D& pos = pCamera->getPos();
				return (CWFLOAT*)&pos;
			}
		}
	}

	return nullptr;
}

NS_MINIR_END
