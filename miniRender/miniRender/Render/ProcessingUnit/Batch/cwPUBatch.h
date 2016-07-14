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

#ifndef __CW_PU_BATCH_H__
#define __CW_PU_BATCH_H__

#include "Base/cwMacros.h"
#include "Ref/cwRef.h"
#include "Render/cwRenderBatch.h"

NS_MINIR_BEGIN

class cwStage;
class cwStageLayer;

class cwPUBatch : public cwRef
{
public:
	cwPUBatch() : m_pStageLayer(nullptr) {}
	virtual ~cwPUBatch() {
		m_pStageLayer = nullptr;
	}

	virtual CWVOID reset() = 0;
	virtual CWVOID updateBatch(cwRenderBatch* pBatch) = 0;

	inline CWVOID setStageLayer(cwStageLayer* pStageLayer) { m_pStageLayer = pStageLayer; }
	inline cwStageLayer* getStageLayer() const { return m_pStageLayer; }

	inline const CWSTRING& getName() const { return m_nName; }
	inline CWVOID setName(const CWSTRING& name) { m_nName = name; }

protected:
	CWSTRING m_nName;
	cwStageLayer* m_pStageLayer;

};

NS_MINIR_END

#endif
