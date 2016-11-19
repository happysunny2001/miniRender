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

#ifndef __CW_ENTITY_H__
#define __CW_ENTITY_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "cwRenderNode.h"

NS_MINIR_BEGIN

class cwRenderObject;
class cwRenderDevice;
class cwEffect;
class cwCamera;
class cwMaterial;
class cwLight;
class cwBlend;
class cwStencil;
class cwRenderBatch;

class CW_DLL cwEntity : public cwRenderNode
{
public:
	static cwEntity* create();

	cwEntity();
	virtual ~cwEntity();

	virtual CWBOOL init() override;
	virtual CWVOID refreshBoundingBox() override;

	virtual CWVOID setRenderObject(cwRenderObject* pRenderObj);
	inline cwRenderObject* getRenderObj() { return m_pRenderObj; }

	virtual CWVOID setMaterial(cwMaterial* pMaterial);
	inline cwMaterial* getMaterial() { return m_pMaterial; }
	
	virtual CWVOID render() override;
	//virtual CWVOID render(cwRenderBatch* pRenderBatch) override;
	virtual CWVOID render(cwEffect* pEffect) override;

protected:
	virtual CWVOID buildMaterial();

protected:
	cwMaterial* m_pMaterial;
	cwRenderObject* m_pRenderObj;

};

NS_MINIR_END

#endif