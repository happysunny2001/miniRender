﻿/*
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

#ifndef __CW_LOAD_RESULT_H__
#define __CW_LOAD_RESULT_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Base/cwVector.h"
#include "Ref/cwRef.h"
#include "Texture/cwTexture.h"
#include "Shader/cwShader.h"
#include "cwLoadBatch.h"

#include <vector>

NS_MINIR_BEGIN

class cwLoadBatch;

class cwLoadResult : public cwRef
{
public:
	static cwLoadResult* create();

	cwLoadResult();
	virtual ~cwLoadResult();

	CWVOID setLoadBatch(cwLoadBatch* pBatch);
	inline cwLoadBatch* getLoadBatch() { return m_pLoadBatch; }

	CWVOID load();
	CWVOID distribute();

protected:
	CWVOID loadTexture2D(const cwResourceInfo& resInfo);
	CWVOID loadTextureCubeMap(const cwResourceInfo& resInfo);
	CWVOID loadShader(const cwResourceInfo& resInfo);

protected:
	cwLoadBatch* m_pLoadBatch;

};

NS_MINIR_END

#endif
