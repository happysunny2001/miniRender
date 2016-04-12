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

#ifndef __CW_PRIMITIVE_NODE2D_H__
#define __CW_PRIMITIVE_NODE2D_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Base/cwUtils.h"
#include "Base/cwStruct.h"
#include "RenderObject/cwDynamicRenderObject.h"
#include "Sprite/cwRenderNode2D.h"
#include "cwPrimitive2D.h"

#include <vector>

NS_MINIR_BEGIN

class cwPrimitiveNode2D : public cwRenderNode2D
{
public:
	static CWUINT uMaxVertexCnt;

public:
	static cwPrimitiveNode2D* create(CWBOOL bThreading=CWFALSE);

	cwPrimitiveNode2D();
	virtual ~cwPrimitiveNode2D();

	virtual CWBOOL init() override;
	virtual CWVOID render(cwCamera* pCamera) override;

	CWVOID drawLine(const cwVector2D& start, const cwVector2D& end, const cwVector4D& color);
	CWVOID drawQuad(
		const cwVector2D& p0,
		const cwVector2D& p1,
		const cwVector2D& p2,
		const cwVector2D& p3,
		const cwVector4D& color, CWBOOL bFill);

protected:
	CWVOID buildRenderBuffer();
	CWVOID buildEffect();

protected:
	cwVertexPosColor* m_pFrameVertexBuffer;
	CWUINT m_uFrameVertexCnt;
	cwDynamicRenderObject* m_pFrameRenderObjects;

	cwVertexPosColor* m_pSolidVertexBuffer;
	CWUINT m_uSolidVertexCnt;
	cwDynamicRenderObject* m_pSolidRenderObjects;

};

NS_MINIR_END

#endif

