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

#include "cwText.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Repertory/cwRepertory.h"
#include "RenderObject/cwDynamicRenderObject.h"
#include "Buffer/cwBuffer.h"
#include "Material/cwMaterial.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderManager.h"
#include "effect/cwEffect.h"

NS_MINIR_BEGIN

cwText* cwText::create(const CWSTRING& strText, const CWSTRING& strFontTexture, CWCHAR cStartChar, CWUINT uCharWidth)
{
	cwText* pText = new cwText();
	if (pText && pText->init(strText, strFontTexture, cStartChar, uCharWidth)) {
		pText->autorelease();
		return pText;
	}

	CW_SAFE_DELETE(pText);
	return nullptr;
}

cwText::cwText():
m_pVertexData(nullptr),
m_pIndexData(nullptr)
{

}

cwText::~cwText()
{
	CW_SAFE_DELETE(m_pVertexData);
	CW_SAFE_DELETE(m_pIndexData);
}

CWBOOL cwText::init(const CWSTRING& strText, const CWSTRING& strFontTexture, CWCHAR cStartChar, CWUINT uCharWidth)
{
	if (!cwRenderNode::init()) return CWFALSE;
	
	m_nStrText = strText;
	m_cStartChar = cStartChar;
	m_uCharWidth = uCharWidth;

	if (!buildMaterial(strFontTexture)) return CWFALSE;
	if (!buildVertexData()) return CWFALSE;
	if (!refreshText()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwText::buildMaterial(const CWSTRING& strFontTexture)
{
	m_pMaterial = cwMaterial::create();
	if (!m_pMaterial) return CWFALSE;
	CW_SAFE_RETAIN(m_pMaterial);

	cwRepertory& repertory = cwRepertory::getInstance();
	cwTexture* pTexFont = repertory.getTextureManager()->getTexture(strFontTexture);
	if (!pTexFont) return CWFALSE;
	m_pMaterial->setDiffuseTexture(pTexFont);

	cwShader* pShader = repertory.getShaderManager()->getDefShader(eDefShaderTex);
	cwEffect* pEffect = cwEffect::create();
	if (!pEffect) return CWFALSE;
	pEffect->setShader(pShader);

	this->setEffect(pEffect);

	return CWTRUE;
}

CWBOOL cwText::buildVertexData()
{
#define CW_DEF_TEXT_LEN 10

	m_uMaxCharCnt = CW_DEF_TEXT_LEN;
	if (!m_nStrText.empty()) {
		m_uMaxCharCnt = (CWUINT)(m_nStrText.size()) * 2;
	}

	buildNativeBuffer();

	m_pRenderObj = cwDynamicRenderObject::create(
		ePrimitiveTypeTriangleList, m_pVertexData, sizeof(cwVertexPosTex), m_uMaxCharCnt * 4,
		m_pIndexData, m_uMaxCharCnt * 6, "PosTex");
	if (!m_pRenderObj) return CWFALSE;
	CW_SAFE_RETAIN(m_pRenderObj);

	return CWTRUE;
}

CWBOOL cwText::buildNativeBuffer()
{
	CW_SAFE_DELETE_ARRAY(m_pVertexData);
	CW_SAFE_DELETE_ARRAY(m_pIndexData);

	m_pVertexData = new cwVertexPosTex[m_uMaxCharCnt * 4];
	if (!m_pVertexData) return CWFALSE;

	m_pIndexData = new CWUINT[m_uMaxCharCnt * 6];
	if (!m_pIndexData) return CWFALSE;

	memset(m_pIndexData, 0, sizeof(CWUINT)*m_uMaxCharCnt * 6);

	CWFLOAT fTextureHeight = m_pMaterial->getDiffuseTexture()->getHeight();
	CWFLOAT fStartX = 0;
	for (CWUINT i = 0; i < m_uMaxCharCnt; ++i) {
		m_pVertexData[4 * i + 0].pos.set(fStartX, 0, 0);
		m_pVertexData[4 * i + 0].tex.set(0, 1.0f);

		m_pVertexData[4 * i + 1].pos.set(fStartX, fTextureHeight, 0);
		m_pVertexData[4 * i + 1].tex.set(0, 0.0f);

		m_pVertexData[4 * i + 2].pos.set(fStartX + m_uCharWidth, 0, 0);
		m_pVertexData[4 * i + 2].tex.set(0, 1.0f);

		m_pVertexData[4 * i + 3].pos.set(fStartX + m_uCharWidth, fTextureHeight, 0);
		m_pVertexData[4 * i + 3].tex.set(0, 0);

		m_pIndexData[6 * i + 0] = 4 * i + 0;
		m_pIndexData[6 * i + 1] = 4 * i + 1;
		m_pIndexData[6 * i + 2] = 4 * i + 2;

		m_pIndexData[6 * i + 3] = 4 * i + 1;
		m_pIndexData[6 * i + 4] = 4 * i + 3;
		m_pIndexData[6 * i + 5] = 4 * i + 2;

		fStartX += m_uCharWidth;
	}

	return CWTRUE;
}

CWBOOL cwText::refreshText()
{
	if (m_nStrText.empty()) return CWFALSE;
	CWUINT uStrlen = (CWUINT)(m_nStrText.size());
	if (uStrlen > m_uMaxCharCnt) return CWFALSE;

	CWFLOAT fTextureWidth = m_pMaterial->getDiffuseTexture()->getWidth();
	CWFLOAT fUStep = 1.0f / (fTextureWidth/(CWFLOAT)m_uCharWidth);

	for (CWUINT i = 0; i < uStrlen; ++i) {
		CWUINT uCharOffset = m_nStrText[i] - m_cStartChar;
		CWFLOAT fStartU = (CWFLOAT)uCharOffset*fUStep;

		m_pVertexData[4 * i + 0].tex.x = fStartU;
		m_pVertexData[4 * i + 1].tex.x = fStartU;
		m_pVertexData[4 * i + 2].tex.x = fStartU + fUStep;
		m_pVertexData[4 * i + 3].tex.x = fStartU + fUStep;
	}

	m_pRenderObj->updateVertexData(m_pVertexData, uStrlen * 4);
	m_pRenderObj->getIndexBuffer()->setElementCount(uStrlen * 6);

	return CWTRUE;
}

NS_MINIR_END
