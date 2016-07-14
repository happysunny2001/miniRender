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

#ifndef __CW_MULTI_RT_TEXTURE_H__
#define __CW_MULTI_RT_TEXTURE_H__

#include "Base/cwMacros.h"
#include "Base/cwVector.h"
#include "cwTexture.h"

NS_MINIR_BEGIN

class cwMultiRTTexture : public cwTexture
{
public:
	static cwMultiRTTexture* create(CWFLOAT fWidth, CWFLOAT fHeight, CWBOOL bThreadSafe = CWFALSE);

	cwMultiRTTexture();
	virtual ~cwMultiRTTexture();

	virtual CWHANDLE getHandle() const override;
	virtual std::vector<CWHANDLE>* getRenderHandleArray() const override;

	virtual CWHANDLE getHandle(CWUINT index) const;
	virtual cwTexture* getTexture(CWUINT index) const;
	virtual CWBOOL addRTTexture(eFormat format, CWBOOL bThreadSafe = CWFALSE);

protected:
	virtual CWBOOL init(CWFLOAT fWidth, CWFLOAT fHeight);

protected:
	cwVector<cwTexture*> m_nVecRTTextures;

};

NS_MINIR_END

#endif
