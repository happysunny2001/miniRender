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

#ifndef __CW_PRIMITIVE_ENTITY_H__
#define __CW_PRIMITIVE_ENTITY_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Base/cwStruct.h"
#include "Math/cwMath.h"
#include "Camera/cwCamera.h"
#include "cwEntity.h"

NS_MINIR_BEGIN

class cwPrimitiveEntity : public cwEntity
{
public:
	static CWUINT uMaxVertexCnt;

public:
	static cwPrimitiveEntity* create();

	cwPrimitiveEntity();
	virtual ~cwPrimitiveEntity();

	virtual CWBOOL init() override;
	virtual CWVOID render(cwRenderBatch* pRenderBatch) override;

	CWVOID addPrimitive(const cwAABB& aabb);
	CWVOID addPrimitive(const cwAABB& aabb, const cwVector4D& color);

	CWVOID addPrimitive(cwCamera* pCamera);
	CWVOID addPrimitive(cwCamera* pCamera, const cwVector4D& color);

protected:
	CWVOID buildMaterial();
	CWVOID buildRenderObject();
	CWVOID buildEffect();

protected:
	cwVertexPosColor* m_pVertexData;
	CWUINT m_uVertexCnt;

};

NS_MINIR_END

#endif
