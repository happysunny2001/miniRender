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

#ifndef __CW_STAGE_MIRROR_GENERATOR_H__
#define __CW_STAGE_MIRROR_GENERATOR_H__

#include "Base/cwMacros.h"
#include "Base/cwVector.h"
#include "cwRenderGenerator.h"

#include <vector>

NS_MINIR_BEGIN

#define CW_MIRROR_CNT 10

class cwStage;
class cwStageLayer;
class cwStencil;
class cwBlend;

class cwStageMirrorGenerator : public cwRenderGenerator
{
public:
	static cwStageMirrorGenerator* create();

	virtual ~cwStageMirrorGenerator();
	virtual CWVOID generate() override;

protected:
	cwStageMirrorGenerator();

	CWVOID buildStage();
	CWVOID buildStencil();
	CWVOID buildBlend();

protected:
	cwBlend* m_pBlendTransparent;
	cwStencil* m_pStencilDrawRef;

	cwStage* m_pStage;
	cwStageLayer* m_pMirrorStageLayer;
	cwVector<cwStageLayer*> m_nVecEntityStageLayer;

};

NS_MINIR_END

#endif
