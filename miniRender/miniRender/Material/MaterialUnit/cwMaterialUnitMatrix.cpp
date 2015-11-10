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

#include "cwMaterialUnitMatrix.h"
#include "Shader/cwShader.h"
#include "effect/cwEffect.h"

NS_MINIR_BEGIN

cwMaterialUnitMatrix* cwMaterialUnitMatrix::create()
{
	cwMaterialUnitMatrix* pMUMatrix = new cwMaterialUnitMatrix();
	if (pMUMatrix && pMUMatrix->init()) {
		pMUMatrix->autorelease();
		return pMUMatrix;
	}

	CW_SAFE_DELETE(pMUMatrix);
	return nullptr;
}

cwMaterialUnitMatrix::cwMaterialUnitMatrix()
{
	m_nMat.identity();
}

cwMaterialUnitMatrix::~cwMaterialUnitMatrix()
{

}

CWBOOL cwMaterialUnitMatrix::init()
{
	if (!cwMaterialUnit::init()) return CWFALSE;

	m_nMat.identity();
	m_nDiffTextureTrans = cwVector2D(0.0f, 0.0f);
	m_nDiffTextureScale = cwVector2D(1.0f, 1.0f);

	return CWTRUE;
}

CWVOID cwMaterialUnitMatrix::config(cwEffect* pEffect)
{
	cwShader* pShader = pEffect->getShader();
	if (pShader && !m_nStrShaderParam.empty()) {
		pShader->setVariableMatrix(m_nStrShaderParam, &(m_nMat.m11));
	}
}

CWVOID cwMaterialUnitMatrix::move(CWFLOAT x, CWFLOAT y)
{
	m_nDiffTextureTrans.x += x;
	m_nDiffTextureTrans.y += y;
	updateMatrix();
}

CWVOID cwMaterialUnitMatrix::move(const cwVector2D& pos)
{
	m_nDiffTextureTrans.x += pos.x;
	m_nDiffTextureTrans.y += pos.y;
	updateMatrix();
}

CWVOID cwMaterialUnitMatrix::scale(CWFLOAT x, CWFLOAT y)
{
	m_nDiffTextureScale.x += x;
	m_nDiffTextureScale.y += y;
	updateMatrix();
}

CWVOID cwMaterialUnitMatrix::scale(const cwVector2D& scale)
{
	m_nDiffTextureScale.x += scale.x;
	m_nDiffTextureScale.y += scale.y;
	updateMatrix();
}

CWVOID cwMaterialUnitMatrix::updateMatrix()
{
	m_nMat.m11 = m_nDiffTextureScale.x; m_nMat.m12 = 0;                     m_nMat.m13 = 0;    m_nMat.m14 = 0;
	m_nMat.m21 = 0;                     m_nMat.m22 = m_nDiffTextureScale.y; m_nMat.m23 = 0;    m_nMat.m24 = 0;
	m_nMat.m31 = 0;                     m_nMat.m32 = 0;                     m_nMat.m33 = 1.0f; m_nMat.m34 = 0;
	m_nMat.m41 = m_nDiffTextureTrans.x; m_nMat.m42 = m_nDiffTextureTrans.y; m_nMat.m43 = 0;    m_nMat.m44 = 1.0f;
}

NS_MINIR_END
