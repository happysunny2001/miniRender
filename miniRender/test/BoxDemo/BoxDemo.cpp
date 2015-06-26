﻿/*
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

#include "BoxDemo.h"


BoxDemo::BoxDemo(HINSTANCE hInstance, int iShowCmd):
cwWinMain(hInstance, iShowCmd),
m_pRenderObj(nullptr)
{

}

BoxDemo::~BoxDemo()
{
	CW_SAFE_RELEASE_NULL(m_pRenderObj);
}

void BoxDemo::initAll()
{
	cwWinMain::initAll();
	cwRepertory::getInstance().setCurrentCamera(m_pCamera);

	buildEntity();
}

void BoxDemo::buildEntity()
{
	cwGeometryGenerator::cwMeshData mesh;
	cwGeometryGenerator::getInstance().generateBox(mesh);

	vector<cwVertexPosColor> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].color = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);
	}

	m_pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosColor), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), ceEleDescPosColor);
	CW_SAFE_RETAIN(m_pRenderObj);

	m_pShader = cwRepertory::getInstance().getShaderManager()->getShader("G:\\Source\\Git\\miniRender\\miniRender\\x64\\Debug\\effect\\D3D11\\color.fx");
}

void BoxDemo::draw()
{
	cwRepertory::getInstance().getDevice()->render(m_pRenderObj, cwVector3D::ZERO, m_pShader, m_pCamera);
}