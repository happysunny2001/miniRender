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

#ifndef __CW_RENDER_BATCH_H__
#define __CW_RENDER_BATCH_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"

NS_MINIR_BEGIN

class cwEffect;
class cwEntity;
class cwBlend;
class cwStencil;

class cwRenderBatch
{
public:
	cwRenderBatch();
	~cwRenderBatch();

	CWVOID reset();
	CWVOID render();

public:
	cwEffect* m_pEffect;
	cwBlend* m_pBlend;
	cwStencil* m_pStencil;
	cwEntity* m_pEntity;
	CWSTRING m_nStrTech;
	cwMatrix4X4 m_nWorldTrans;

};

NS_MINIR_END

#endif
