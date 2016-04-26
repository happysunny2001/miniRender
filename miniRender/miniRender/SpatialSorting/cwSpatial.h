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

#ifndef __CW_SPATIAL_H__
#define __CW_SPATIAL_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Base/cwStruct.h"
#include "Base/cwVector.h"
#include "Ref/cwRef.h"

#include <vector>

NS_MINIR_BEGIN

class cwScene;
class cwFrustum;
class cwRenderNode;
class cwAABB;
class cwCircle;
class cwRay;

class CW_DLL cwSpatial : public cwRef
{
public:
	//virtual CWBOOL build(cwScene*) = 0;
	
	virtual CWVOID intersection(const cwFrustum&, cwVector<cwRenderNode*>&, CWBOOL) = 0;
	virtual CWVOID intersection(const cwAABB&, cwVector<cwRenderNode*>&, CWBOOL) = 0;
	virtual CWVOID intersection(const cwCircle&, cwVector<cwRenderNode*>&, CWBOOL) = 0;
	virtual CWVOID intersection(const cwRay&, cwVector<cwRenderNode*>&, CWBOOL) = 0;

	virtual CWBOOL insert(cwRenderNode*) = 0;
	virtual CWBOOL remove(cwRenderNode*) = 0;

	virtual CWVOID update() = 0;
	virtual CWVOID refresh(cwRenderNode*) = 0;

	virtual CWVOID clear() = 0;
	virtual CWVOID renderPrimitiveFrame() = 0;

	virtual const cwAABB& getBoundingBox() = 0;
	virtual CWUINT getObjCnt() = 0;

protected:

};

NS_MINIR_END

#endif
