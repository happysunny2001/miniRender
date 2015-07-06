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

#include "LoadTxtDemoScene.h"

LoadTxtDemoScene* LoadTxtDemoScene::create()
{
	LoadTxtDemoScene* pScene = new LoadTxtDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

LoadTxtDemoScene::LoadTxtDemoScene():
m_pCar(nullptr)
{

}

LoadTxtDemoScene::~LoadTxtDemoScene()
{
	CW_SAFE_RELEASE_NULL(m_pCar);
}

CWBOOL LoadTxtDemoScene::init()
{
	if (!cwScene::init()) return CWFALSE;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown = CW_CALLBACK_1(LoadTxtDemoScene::onTouchDown, this);
	pTouchListener->onTouchUp = CW_CALLBACK_1(LoadTxtDemoScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(LoadTxtDemoScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	m_fTheta = 0.1f;
	m_fPhi = -cwMathUtil::cwPIx2 / 8.0f;
	m_fRadius = 20.0f;
	m_bTouchDown = false;

	buildEntity();
	buildLight();

	return CWTRUE;
}

CWVOID LoadTxtDemoScene::buildEntity()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->loadTxt(repertory.getFileSystem()->getFullFilePath("Models/car.txt"), mesh);

	vector<cwVertexPosNormal> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].normal = mesh.nVertex[i].normal;
	}

	cwRenderObject *pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormal), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), ceEleDescPosNormal);

	cwShader* pShader = repertory.getShaderManager()->getDefShader(CW_SHADER_DEF_LIGHTING);
	cwMaterial* pMaterial = cwMaterial::create();
	pMaterial->setShader(pShader);

	m_pCar = cwEntity::create();
	m_pCar->setMaterial(pMaterial);
	m_pCar->setRenderObject(pRenderObj);
	m_pCar->setPosition(cwVector3D::ZERO);
	CW_SAFE_RETAIN(m_pCar);

	this->addChild(m_pCar);
}

CWVOID LoadTxtDemoScene::buildLight()
{
	cwLight* pLightDirectional = cwLight::createDirectionalLight(
		cwVector3D(0, -1.0, 0),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f));
	this->addLight(pLightDirectional);

	cwLight* pLightPoint = cwLight::createPointLight(
		cwVector3D(30.0f, 80.0f, 0),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector3D(0.0f, 0.03f, 0.0f), 1000.0f);
	this->addLight(pLightPoint);

	cwLight* pLightSpot = cwLight::createSpotLight(
		cwVector3D(-20.0f, 50.0f, 0),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector3D(0.0f, 0.1f, 0.0f), 1000.0f,
		cwVector3D(0, -1.0, 0), 10.0f);
	this->addLight(pLightSpot);
}

CWVOID LoadTxtDemoScene::onTouchDown(cwTouch* pTouch)
{
	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;

	m_bTouchDown = true;
}

CWVOID LoadTxtDemoScene::onTouchUp(cwTouch* pTouch)
{
	m_bTouchDown = false;
}

CWVOID LoadTxtDemoScene::onTouchMoving(cwTouch* pTouch)
{
	if (m_bTouchDown) {
		CWFLOAT dx = XMConvertToRadians(pTouch->getScreenPos().x - m_fLastX);
		CWFLOAT dy = XMConvertToRadians(pTouch->getScreenPos().y - m_fLastY);

		m_fTheta -= dx;
		m_fPhi += dy;
		m_fPhi = min(max(0.1f, m_fPhi), cwMathUtil::cwPI - 0.1f);

		float x = m_fRadius*sinf(m_fPhi)*cosf(m_fTheta);
		float z = m_fRadius*sinf(m_fPhi)*sinf(m_fTheta);
		float y = m_fRadius*cosf(m_fPhi);

		cwRepertory::getInstance().getEngine()->getDefaultCamera()->updateCamera(x, y, z);
	}

	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;
}
