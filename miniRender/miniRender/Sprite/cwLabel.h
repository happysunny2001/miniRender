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

#ifndef __CW_LABEL_H__
#define __CW_LABEL_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "cwSprite.h"

NS_MINIR_BEGIN

class cwLabel : public cwSprite
{
public:
	static cwLabel* create(const CWSTRING& strText, const CWSTRING& strFontTexture, CWCHAR cStartChar, CWUINT uCharWidth);

	cwLabel();
	virtual ~cwLabel();

	virtual CWBOOL init(const CWSTRING& strText, const CWSTRING& strFontTexture, CWCHAR cStartChar, CWUINT uCharWidth);
	virtual CWUINT getVertexCnt() const override;

	virtual CWVOID setString(const CWSTRING& strText);

protected:
	virtual CWBOOL buildVertexBuffer() override;
	virtual CWVOID refreshVertexBuffer();
	virtual CWVOID transformBuffer() override;

protected:
	CWSTRING m_nStrText;
	CWUINT m_uMaxCharCnt;
	CWUINT m_uVertexSize;
	CWCHAR m_cStartChar;
	CWUINT m_uCharWidth;

};

NS_MINIR_END

#endif
