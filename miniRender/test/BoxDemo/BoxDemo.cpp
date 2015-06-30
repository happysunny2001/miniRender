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

#include "BoxDemo.h"

BoxDemo::BoxDemo():
m_pEntityAxis(nullptr),
m_pEntityBox01(nullptr),
m_pEntityBox02(nullptr),
m_pScene(nullptr)
{

}

BoxDemo::~BoxDemo()
{
	CW_SAFE_RELEASE_NULL(m_pEntityAxis);
	CW_SAFE_RELEASE_NULL(m_pEntityBox01);
	CW_SAFE_RELEASE_NULL(m_pEntityBox02);
	CW_SAFE_RELEASE_NULL(m_pScene);
}

void BoxDemo::gameBegin()
{
	cwRepertory::getInstance().getEngine()->getDefaultCamera()->updateCamera(0, 2.0f, -20.0f);
	buildScene();
}

void BoxDemo::gameEnd()
{

}

void BoxDemo::gameBeginBackGround()
{

}

void BoxDemo::gameEndBackGround()
{

}

void BoxDemo::buildEntity()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateBox(mesh);

	vector<cwVertexPosColor> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].color = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);
	}

	cwRenderObject *pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosColor), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), ceEleDescPosColor);

	cwShader* pShader = repertory.getShaderManager()->getDefShader(CW_SHADER_DEF_COLOR);
	cwMaterial* pMaterial = cwMaterial::create();
	pMaterial->setShader(pShader);

	m_pEntityBox01 = cwEntity::create();
	m_pEntityBox01->setMaterial(pMaterial);
	m_pEntityBox01->setRenderObject(pRenderObj);
	m_pEntityBox01->setPosition(cwVector3D(2.0f, 1.0f, 0.0f));
	CW_SAFE_RETAIN(m_pEntityBox01);

	m_pEntityBox02 = cwEntity::create();
	m_pEntityBox02->setMaterial(pMaterial);
	m_pEntityBox02->setRenderObject(pRenderObj);
	m_pEntityBox02->setPosition(cwVector3D(-2.0f, 1.0f, -3.0f));
	CW_SAFE_RETAIN(m_pEntityBox02);
}

void BoxDemo::buildAxis()
{
	m_pEntityAxis = cwRepertory::getInstance().getGeoGenerator()->generateCoordinateAxisEntity(10.0f);
	CW_SAFE_RETAIN(m_pEntityAxis);
}

void BoxDemo::buildScene()
{
	buildEntity();
	buildAxis();

	m_pScene = cwScene::create();
	CW_SAFE_RETAIN(m_pScene);

	m_pScene->addChild(m_pEntityBox01);
	m_pEntityBox01->addChild(m_pEntityBox02);
//	m_pScene->addChild(m_pEntityBox02);
	m_pScene->addChild(m_pEntityAxis);

	cwRepertory::getInstance().getEngine()->setScene(m_pScene);
}
