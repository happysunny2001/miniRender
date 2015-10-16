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

#include "cwLabel.h"
#include "Texture/cwTexture.h"

NS_MINIR_BEGIN

cwLabel* cwLabel::create(const CWSTRING& strText, const CWSTRING& strFontTexture, CWCHAR cStartChar, CWUINT uCharWidth)
{
	cwLabel* pLabel = new cwLabel();
	if (pLabel && pLabel->init(strText, strFontTexture, cStartChar, uCharWidth)) {
		pLabel->autorelease();
		return pLabel;
	}

	CW_SAFE_DELETE(pLabel);
	return nullptr;
}

cwLabel::cwLabel():
m_uVertexSize(0),
m_uMaxCharCnt(0),
m_cStartChar(0),
m_uCharWidth(0)
{

}

cwLabel::~cwLabel()
{

}

CWBOOL cwLabel::init(const CWSTRING& strText, const CWSTRING& strFontTexture, CWCHAR cStartChar, CWUINT uCharWidth)
{
	if (!cwRenderNode2D::init()) return CWFALSE;

	m_nStrText = strText;
	m_cStartChar = cStartChar;
	m_uCharWidth = uCharWidth;

	if (!loadTexture(strFontTexture)) return CWFALSE;
	if (!buildVertexBuffer()) return CWFALSE;

	return CWTRUE;
}

CWUINT cwLabel::getVertexCnt() const
{
	return (CWUINT)(m_nStrText.size()) * 6;
}

CWBOOL cwLabel::buildVertexBuffer()
{
	if (m_nStrText.empty()) {
		m_uMaxCharCnt = 5;
	}
	else {
		m_uMaxCharCnt = (CWUINT)((CWFLOAT)(m_nStrText.size())*1.5f + 1.0f);
	}

	m_uVertexSize = m_uMaxCharCnt * 6;
	m_pVertexBuffer = new cwVertexPosTexColor[m_uVertexSize];
	if (!m_pVertexBuffer) return CWFALSE;

	refreshVertexBuffer();

	return CWTRUE;
}

CWVOID cwLabel::refreshVertexBuffer()
{
	CWUINT iChatSize = (CWUINT)(m_nStrText.size());
	if (iChatSize == 0) return;

	CWFLOAT fHalfTextureHeight = m_pTexture->getHeight()*0.5f;
	CWFLOAT fTextureWidth = m_pTexture->getWidth();
	CWFLOAT fUStep = 1.0f / (fTextureWidth / (CWFLOAT)m_uCharWidth);
	CWFLOAT fStartX = -((iChatSize*m_uCharWidth)*0.5f);

	for (CWUINT i = 0; i < iChatSize; ++i) {
		CWUINT uCharOffset = m_nStrText[i] - m_cStartChar;
		CWFLOAT fStartU = (CWFLOAT)uCharOffset*fUStep;

		m_pVertexBuffer[6 * i + 0].pos.set(fStartX, -fHalfTextureHeight, 0.0f);
		m_pVertexBuffer[6 * i + 0].tex.set(fStartU, 1.0f);
		m_pVertexBuffer[6 * i + 0].color = m_nColor;

		m_pVertexBuffer[6 * i + 1].pos.set(fStartX, fHalfTextureHeight, 0.0f);
		m_pVertexBuffer[6 * i + 1].tex.set(fStartU, 0.0f);
		m_pVertexBuffer[6 * i + 1].color = m_nColor;

		m_pVertexBuffer[6 * i + 2].pos.set(fStartX+m_uCharWidth, -fHalfTextureHeight, 0.0f);
		m_pVertexBuffer[6 * i + 2].tex.set(fStartU + fUStep, 1.0f);
		m_pVertexBuffer[6 * i + 2].color = m_nColor;

		m_pVertexBuffer[6 * i + 3].pos.set(fStartX, fHalfTextureHeight, 0.0f);
		m_pVertexBuffer[6 * i + 3].tex.set(fStartU, 0.0f);
		m_pVertexBuffer[6 * i + 3].color = m_nColor;

		m_pVertexBuffer[6 * i + 4].pos.set(fStartX + m_uCharWidth, fHalfTextureHeight, 0.0f);
		m_pVertexBuffer[6 * i + 4].tex.set(fStartU + fUStep, 0.0f);
		m_pVertexBuffer[6 * i + 4].color = m_nColor;

		m_pVertexBuffer[6 * i + 5].pos.set(fStartX + m_uCharWidth, -fHalfTextureHeight, 0.0f);
		m_pVertexBuffer[6 * i + 5].tex.set(fStartU + fUStep, 1.0f);
		m_pVertexBuffer[6 * i + 5].color = m_nColor;

		fStartX += m_uCharWidth;
	}

	m_nBoundingBox.m_nMin = m_pVertexBuffer[0].pos;
	m_nBoundingBox.m_nMax = m_pVertexBuffer[iChatSize * 6 - 2].pos;
}

CWVOID cwLabel::transformBuffer()
{
	refreshVertexBuffer();

	CWUINT iChatSize = (CWUINT)(m_nStrText.size());
	if (iChatSize == 0) return;

	cwVector4D pos;
	for (CWUINT i = 0; i < iChatSize * 6; ++i) {
		pos.set(m_pVertexBuffer[i].pos.x, m_pVertexBuffer[i].pos.y, 0, 1.0f);
		m_pVertexBuffer[i].pos = pos * m_nTrans;
	}

	m_nBoundingBox.m_nMin = m_pVertexBuffer[0].pos;
	m_nBoundingBox.m_nMax = m_pVertexBuffer[iChatSize * 6 - 2].pos;
}

CWVOID cwLabel::setString(const CWSTRING& strText)
{
	if (strText == m_nStrText) return;

	CWUINT uSize = (CWUINT)(strText.size());
	if (uSize > m_uMaxCharCnt) {
		m_uMaxCharCnt = (CWUINT)(uSize*1.5f + 1.0f);

		CW_SAFE_DELETE(m_pVertexBuffer);
		m_uVertexSize = m_uMaxCharCnt * 6;
		m_pVertexBuffer = new cwVertexPosTexColor[m_uVertexSize];
		if (!m_pVertexBuffer) return;
	}

	m_nStrText = strText;
	transformBuffer();
}

NS_MINIR_END
