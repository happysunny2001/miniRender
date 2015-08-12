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

#ifndef __STENCIL_DEMO_SCENE_H__
#define __STENCIL_DEMO_SCENE_H__

#include "cwMiniRender.h"
NS_USING_MINIR;

class StencilDemoScene : public cwScene
{
public:
	static StencilDemoScene* create();

	StencilDemoScene();
	virtual ~StencilDemoScene();

	virtual CWBOOL init() override;

	virtual CWVOID onTouchDown(cwTouch* pTouch) override;
	virtual CWVOID onTouchUp(cwTouch* pTouch) override;
	virtual CWVOID onTouchMoving(cwTouch* pTouch) override;

protected:
	CWVOID buildEntity();
	CWVOID buildGround();
	CWVOID buildCar();
	CWVOID buildLights();

	cwStaticRenderObject* createRenderObj(cwGeometryGenerator::cwMeshData& mesh);

protected:
	CWFLOAT m_fLastX;
	CWFLOAT m_fLastY;
	CWFLOAT m_fTheta;
	CWFLOAT m_fPhi;
	CWFLOAT m_fRadius;
	CWBOOL m_bTouchDown;

};

#endif
