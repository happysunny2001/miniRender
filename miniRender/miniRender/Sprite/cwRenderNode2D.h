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

#ifndef __CW_RENDER_NODE_2D_H__
#define __CW_RENDER_NODE_2D_H__

#include "Base/cwMacros.h"
#include "Entity/cwRenderNode.h"

NS_MINIR_BEGIN

class CW_DLL cwRenderNode2D : public cwRenderNode
{
public:
	static cwRenderNode2D* create();

	cwRenderNode2D();
	virtual ~cwRenderNode2D();

	virtual CWBOOL init() override;

	virtual CWVOID setPosition(CWFLOAT x, CWFLOAT y);
	virtual CWVOID move(CWFLOAT x, CWFLOAT y);
	virtual CWVOID setPosition(CWFLOAT x, CWFLOAT y, CWFLOAT z) override;
	virtual CWVOID move(CWFLOAT x, CWFLOAT y, CWFLOAT z) override;
	virtual CWVOID move(const cwVector3D& v) override;
	virtual CWVOID setPosition(const cwVector3D& v) override;

	virtual CWVOID setRotation(CWFLOAT f);
	virtual CWVOID rotate(CWFLOAT f);
	virtual CWVOID setRotation(CWFLOAT x, CWFLOAT y, CWFLOAT z) override;
	virtual CWVOID setRotation(const cwVector3D& v) override;
	virtual CWVOID rotate(CWFLOAT x, CWFLOAT y, CWFLOAT z) override;
	virtual CWVOID rotate(const cwVector3D& v) override;

	virtual CWVOID setScale(CWFLOAT x, CWFLOAT y);
	virtual CWVOID scale(CWFLOAT x, CWFLOAT y);
	virtual CWVOID setScale(CWFLOAT x, CWFLOAT y, CWFLOAT z) override;
	virtual CWVOID setScale(const cwVector3D& v) override;
	virtual CWVOID scale(CWFLOAT x, CWFLOAT y, CWFLOAT z) override;
	virtual CWVOID scale(const cwVector3D& v) override;

	virtual CWVOID refreshBoundingBox() override;

	inline CWINT getRenderOrder() const { return m_uRenderOrder; }
	virtual CWVOID setRenderOrder(CWINT uOrder);
	
	virtual CWVOID render(cwCamera* pCamera);

protected:
	virtual CWVOID insertSpatialNode(cwRenderNode* pNode) override;
	virtual CWVOID removeSpatialNode(cwRenderNode* pNode) override;
	virtual CWVOID refreshSpatialNode() override;

protected:
	CWINT m_uRenderOrder;

};

NS_MINIR_END

#endif
