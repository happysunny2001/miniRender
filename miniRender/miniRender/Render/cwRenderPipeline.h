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

#ifndef __CW_RENDER_PIPELINE_H__
#define __CW_RENDER_PIPELINE_H__

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "cwRenderBatch.h"

#include <vector>

NS_MINIR_BEGIN

class cwEntity;
class cwEffect;
class cwShader;

class cwRenderPipeline 
{
public:
	cwRenderPipeline();
	~cwRenderPipeline();
	
	CWVOID reset();
	CWBOOL addEntity(cwEntity* pEntity);
	CWBOOL addEntity(cwEntity* pEntity, cwEffect* pEffect);
	CWBOOL full();

	inline CWVOID setShader(cwShader* pShader) { m_pShader = pShader; }
	inline cwShader* getShader() const { return m_pShader; }

	CWVOID render();

protected:
	cwRenderBatch* getNextAvailableBatch();

protected:
	std::vector<cwRenderBatch> m_nVecBatch;
	CWUINT m_iBatchIndex;

	cwShader* m_pShader;

};

NS_MINIR_END

#endif
