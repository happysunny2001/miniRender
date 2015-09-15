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

#ifndef __CW_BATCH_ENTITY_H__
#define __CW_BATCH_ENTITY_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "cwRenderNode.h"
#include "cwEntity.h"

NS_MINIR_BEGIN

class cwBatchEntity : public cwEntity
{
public:
	static cwBatchEntity* create();

	cwBatchEntity();
	virtual ~cwBatchEntity();

	virtual CWBOOL init() override;

protected:
	cwRenderObject* m_pInstanceObject;
	CWUINT m_uInstanceMaxCount;
	CWUINT m_uInstanceCount;

};

NS_MINIR_END

#endif
