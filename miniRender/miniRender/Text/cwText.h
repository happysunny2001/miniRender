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

#ifndef __CW_TEXT_H__
#define __CW_TEXT_H__

#include "Base/cwBasicType.h"
#include "Base/cwMacros.h"
#include "Base/cwStruct.h"
#include "Ref/cwRef.h"
#include "Entity/cwRenderNode.h"
#include "Entity/cwEntity.h"

NS_MINIR_BEGIN

class cwRenderObject;
class cwTexture;
class cwDynamicRenderObject;

class cwText : public cwEntity
{
public:
	static cwText* create(const CWSTRING& strText, const CWSTRING& strFontTexture, CWCHAR cStartChar, CWUINT uCharWidth);

	virtual ~cwText();

	virtual CWBOOL init(const CWSTRING& strText, const CWSTRING& strFontTexture, CWCHAR cStartChar, CWUINT uCharWidth);

protected:
	cwText();

	CWBOOL buildMaterial(const CWSTRING& strFontTexture);
	CWBOOL buildVertexData();
	CWBOOL refreshText();

protected:
	CWSTRING m_nStrText;
	CWUINT m_uMaxCharCnt;
	//cwTexture* m_pFontTexture;

	cwVertexPosTex* m_pVertexData;
	CWUINT* m_pIndexData;

	CWCHAR m_cStartChar;
	CWUINT m_uCharWidth;

};

NS_MINIR_END

#endif
