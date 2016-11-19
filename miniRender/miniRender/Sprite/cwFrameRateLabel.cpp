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

#include "cwFrameRateLabel.h"
#include <sstream>

NS_MINIR_BEGIN

cwFrameRateLabel* cwFrameRateLabel::create(const CWSTRING& strText, const CWSTRING& strFontTexture, CWCHAR cStartChar, CWUINT uCharWidth)
{
	cwFrameRateLabel* pLabel = new cwFrameRateLabel();
	if (pLabel && pLabel->init(strText, strFontTexture, cStartChar, uCharWidth)) {
		pLabel->autorelease();
		return pLabel;
	}

	CW_SAFE_DELETE(pLabel);
	return nullptr;
}

cwFrameRateLabel::cwFrameRateLabel() :
m_uFrameRate(0),
m_uFrameCounter(0),
m_fFrameCounter(0)
{

}

cwFrameRateLabel::~cwFrameRateLabel()
{

}

CWVOID cwFrameRateLabel::update(CWFLOAT dt)
{
	m_uFrameCounter++;
	m_fFrameCounter += dt;
	if (m_fFrameCounter >= 1.0f) {
		m_fFrameCounter = 0;
		m_uFrameRate = m_uFrameCounter;
		m_uFrameCounter = 0;

		stringstream ss;
		ss << m_uFrameRate;
		this->setString(ss.str());
	}
}

NS_MINIR_END
