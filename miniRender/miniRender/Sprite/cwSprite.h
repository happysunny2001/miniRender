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

#ifndef __CW_SPRITE_H__
#define __CW_SPRITE_H__

#include "Base/cwMacros.h"
#include "Base/cwStruct.h"
#include "Math/cwMath.h"
#include "cwRenderNode2D.h"

NS_MINIR_BEGIN

class cwTexture;

class CW_DLL cwSprite : public cwRenderNode2D
{
public:
	static cwSprite* create();
	static cwSprite* create(const std::string& strFile);

	cwSprite();
	virtual ~cwSprite();

	virtual CWBOOL init() override;
	virtual CWBOOL init(const std::string& strFile);

	inline const cwVertexPosTexColor* getVertexBuffer() const { return m_pVertexBuffer; }
	inline CWUINT getVertexCnt() const { return 6; }
	inline cwTexture* getTexture() { return m_pTexture; }

	virtual CWVOID refreshTransform() override;

protected:
	CWBOOL loadTexture(const std::string& strFile);
	CWBOOL buildVertexBuffer();
	CWVOID initVertexBuffer(cwVertexPosTexColor* pVertexBuffer);
	CWVOID initVertexBuffer(cwVector4D* pVertexBuffer);

protected:
	cwTexture* m_pTexture;
	cwVector4D m_nColor;
	cwVertexPosTexColor* m_pVertexBuffer;

};

NS_MINIR_END

#endif
