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

#include "cwPUBatchShaderParamMatrix.h"
#include "Camera/cwCamera.h"
#include "Engine/cwEngine.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShader.h"

NS_MINIR_BEGIN

cwPUBatchShaderParamMatrix* cwPUBatchShaderParamMatrix::create()
{
	cwPUBatchShaderParamMatrix* pPUBatch = new cwPUBatchShaderParamMatrix();
	if (pPUBatch) {
		pPUBatch->autorelease();
	}

	return pPUBatch;
}

cwPUBatchShaderParamMatrix::cwPUBatchShaderParamMatrix() :
m_pMatrix(nullptr)
{
	m_eType = eShaderParamTypeMatrix;
}

cwPUBatchShaderParamMatrix::~cwPUBatchShaderParamMatrix()
{
	CW_SAFE_DELETE(m_pMatrix);
}

CWVOID cwPUBatchShaderParamMatrix::setMatrixData(const cwMatrix4X4& mat)
{
	CW_SAFE_DELETE(m_pMatrix);
	m_pMatrix = new cwMatrix4X4(mat);
}

const cwMatrix4X4* cwPUBatchShaderParamMatrix::getMatrix() const
{
	if (m_eSource == eShaderParamSourceData)
		return m_pMatrix;

	if (m_nEngineParamType == "Camera") {
		return getCameraMatrix();
	}

	return nullptr;
}

const cwMatrix4X4* cwPUBatchShaderParamMatrix::getCameraMatrix() const
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera(m_nEngineParamName);
	if (pCamera) {
		if (m_nEngineParamAttr == "ViewMatrix")
			return &pCamera->getViewMatrix();
		else if (m_nEngineParamAttr == "ProjMatrix")
			return &pCamera->getProjMatrix();
		else if (m_nEngineParamAttr == "ViewProjMatrix")
			return &pCamera->getViewProjMatrix();
		else if (m_nEngineParamAttr == "InvertViewProjMatrix")
			return &pCamera->getInvertViewProjMatrix();
	}

	return nullptr;
}

CWVOID cwPUBatchShaderParamMatrix::updateShaderParam()
{
	const cwMatrix4X4* pMatreix = getMatrix();
	if (pMatreix && m_pShader && !m_nShaderParamName.empty()) {
		m_pShader->setVariableMatrix(m_nShaderParamName, *pMatreix);
	}
}

NS_MINIR_END
