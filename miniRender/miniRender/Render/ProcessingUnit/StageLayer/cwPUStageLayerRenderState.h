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

#ifndef __CW_PU_STAGE_LAYER_RENDER_STATE_H__
#define __CW_PU_STAGE_LAYER_RENDER_STATE_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "cwPUStageLayer.h"

NS_MINIR_BEGIN

class cwPUStageLayerRenderState : public cwPUStageLayer
{
public:
	static cwPUStageLayerRenderState* create();

	cwPUStageLayerRenderState();
	virtual ~cwPUStageLayerRenderState();

	virtual CWVOID begin() override;
	virtual CWVOID end() override;

	inline CWVOID setRenderState(eRenderState eState) { m_eRenderState = eState; }

protected:
	eRenderState m_eRenderState;

};

NS_MINIR_END

#endif
