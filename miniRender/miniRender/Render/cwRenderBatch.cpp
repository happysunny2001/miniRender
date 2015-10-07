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

#include "cwRenderBatch.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Effect/cwEffect.h"
#include "Device/cwDevice.h"
#include "cwRenderer.h"

NS_MINIR_BEGIN

cwRenderBatch::cwRenderBatch()
{
	reset();
}

cwRenderBatch::~cwRenderBatch()
{
	reset();
}

CWVOID cwRenderBatch::reset()
{
	m_pEffect = nullptr;
	m_pEntity = nullptr;
}

CWVOID cwRenderBatch::render()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	repertory.getDevice()->setBlend(m_pBlend);
	repertory.getDevice()->setStencil(m_pStencil);
	repertory.getEngine()->getRenderer()->render(this);
}

NS_MINIR_END
