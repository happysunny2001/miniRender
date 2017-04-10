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

#include "TBDRScene.h"

TBDRScene* TBDRScene::create()
{
	TBDRScene* pScene = new TBDRScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

TBDRScene::TBDRScene() :
m_pPlaneObject(nullptr),
m_pSphereObject(nullptr),
m_pEntityPlane(nullptr),
m_pEntitySphere(nullptr),
m_pBoxObject(nullptr),
m_pEntityBox01(nullptr),
m_pEntityBox02(nullptr),
m_pEntityBox03(nullptr)
{

}

TBDRScene::~TBDRScene()
{
	CW_SAFE_RELEASE_NULL(m_pPlaneObject);
	CW_SAFE_RELEASE_NULL(m_pSphereObject);
	CW_SAFE_RELEASE_NULL(m_pEntityPlane);
	CW_SAFE_RELEASE_NULL(m_pEntitySphere);
	CW_SAFE_RELEASE_NULL(m_pBoxObject);
	CW_SAFE_RELEASE_NULL(m_pEntityBox01);
	CW_SAFE_RELEASE_NULL(m_pEntityBox02);
	CW_SAFE_RELEASE_NULL(m_pEntityBox03);
}

CWBOOL TBDRScene::init()
{
	if (!cwBaseScene::init()) return CWFALSE;

	buildRenderObject();
	buildEntity();
	buildLight();

	this->initScene();

	return CWTRUE;
}

CWVOID TBDRScene::update(CWFLOAT dt)
{
	//if (isKeyDown(KeyCode::X)) {
	//	cwRenderer* pRender = cwRepertory::getInstance().getEngine()->getRenderer();
	//	if (pRender) {
	//		cwStage* pDebugStage = pRender->getStage("DebugStage");
	//		cwStage* pMLAAStage = pRender->getStage("MLAAStage");

	//		if(pDebugStage) pDebugStage->setEnable(CWTRUE);
	//		if(pMLAAStage) pMLAAStage->setEnable(CWFALSE);
	//	}
	//}
	//else if (isKeyDown(KeyCode::M)) {
	//	cwRenderer* pRender = cwRepertory::getInstance().getEngine()->getRenderer();
	//	if (pRender) {
	//		cwStage* pDebugStage = pRender->getStage("DebugStage");
	//		cwStage* pMLAAStage = pRender->getStage("MLAAStage");

	//		if (pDebugStage) pDebugStage->setEnable(CWFALSE);
	//		if (pMLAAStage) pMLAAStage->setEnable(CWTRUE);
	//	}
	//}

	//movingLights(dt);
	//movingDirectionalLight(dt);
}

CWVOID TBDRScene::movingLights(CWFLOAT dt)
{
	CWFLOAT angle = cwMathUtil::cwPI*0.02f*dt;
	cwMatrix4X4 matRot;
	matRot.setRotation(angle, 0, 0);

	auto iSize = m_nVecPointLights.size();

	for (auto i = 0; i < iSize; ++i) {
		const cwVector3D& newPos = m_nVecPointLights.at(i)->getPosition();
		m_nVecPointLights.at(i)->setPosition(newPos*matRot);
	}
}

CWVOID TBDRScene::movingDirectionalLight(CWFLOAT dt)
{
	if (m_pDirectionalLight) {
		static CWFLOAT fSpeed = cwMathUtil::cwPI*0.01f;
		CWFLOAT fAngle = fSpeed*dt;

		cwMatrix4X4 matRot;
		matRot.setRotation(cwVector3D(0, fAngle, 0));

		cwVector4D dir = m_pDirectionalLight->getDirection();
		dir *= matRot;
		m_pDirectionalLight->setDirection(dir);
	}
}

CWVOID TBDRScene::buildRenderObject()
{
	cwRenderObjectGenerator<cwVertexPosNormal> roGenerator;

	m_pPlaneObject = roGenerator.generateGrid(200, 200, 40, 40, "PosNormal");
	CW_SAFE_RETAIN(m_pPlaneObject);

	m_pBoxObject = roGenerator.generateBox("PosNormal");
	CW_SAFE_RETAIN(m_pBoxObject);

	m_pSphereObject = roGenerator.generateSphere(1.0, 20, 20, "PosNormal");
	CW_SAFE_RETAIN(m_pSphereObject);
}

CWVOID TBDRScene::buildEntity()
{
	cwMaterial* pMaterial = cwMaterial::create();
	pMaterial->setDiffuse(cwVector4D(0.8f, 0.8f, 0.8f, 1.0f));
	pMaterial->setSpecular(cwVector4D(1.0f, 1.0f, 1.0f, 8.0f));

	cwVector4D startPos(-93.0f, 2.0f, 93.0f, 1.0f);
	for (CWUINT x = 0; x < 10; ++x) {
		for (CWUINT z = 0; z < 10; ++z) {
			cwEntity* pEntity = cwEntity::create();
			pEntity->setRenderObject(m_pBoxObject);
			pEntity->setMaterial(pMaterial);

			if (x == 5 && z == 5)
				pEntity->setScale(4.0f, 34.0f, 4.0f);
			else
				pEntity->setScale(4.0f, 4.0f, 4.0f);

			pEntity->setPosition(startPos);
			this->addChild(pEntity);

			startPos.z -= 17.1f;
		}

		startPos.x += 17.1f;
		startPos.z = 93.0f;
	}

	m_pEntityPlane = cwEntity::create();
	m_pEntityPlane->setRenderObject(m_pPlaneObject);
	m_pEntityPlane->setCastShadow(CWFALSE);
	CW_SAFE_RETAIN(m_pEntityPlane);

/*	m_pEntitySphere = cwEntity::create();
	m_pEntitySphere->setRenderObject(m_pSphereObject);
	m_pEntitySphere->setEffect(pEffect);
	m_pEntitySphere->setMaterial(pMaterial);
	CW_SAFE_RETAIN(m_pEntitySphere);

	m_pEntityBox01 = cwEntity::create();
	m_pEntityBox01->setRenderObject(m_pBoxObject);
	m_pEntityBox01->setEffect(pEffect);
	m_pEntityBox01->setMaterial(pMaterial);
	CW_SAFE_RETAIN(m_pEntityBox01);

	m_pEntityBox02 = cwEntity::create();
	m_pEntityBox02->setRenderObject(m_pBoxObject);
	m_pEntityBox02->setEffect(pEffect);
	m_pEntityBox02->setMaterial(pMaterial);
	CW_SAFE_RETAIN(m_pEntityBox02);

	m_pEntityBox03 = cwEntity::create();
	m_pEntityBox03->setRenderObject(m_pBoxObject);
	m_pEntityBox03->setEffect(pEffect);
	m_pEntityBox03->setMaterial(pMaterial);
	CW_SAFE_RETAIN(m_pEntityBox03);*/
}

CWVOID TBDRScene::buildLight()
{
	cwVector3D startPos(-100.0f, 6.0f, 100.0f);
	CWFLOAT step = 20.0f;

	for (CWUINT x = 0; x < 10; ++x) {
		for (CWUINT z = 0; z < 10; ++z) {
			CWFLOAT r = cwMathUtil::randFloat()*0.1f;
			CWFLOAT g = cwMathUtil::randFloat()*0.1f;
			CWFLOAT b = cwMathUtil::randFloat()*0.1f;

			CWFLOAT radius = 10.0f + 6.0f;// *cwMathUtil::randFloat();

			if (r >= g && r >= b) r *= 2.0f;
			if (g >= r && r >= b) g *= 2.0f;
			if (b >= r && b >= g) b *= 2.0f;

			cwPointLight* pLightPoint = cwPointLight::create(
				startPos,
				cwVector4D(0.1f+r, 0.1f+g, 0.1f+b, 1.0f),
				2.0f, 2.0f+radius);
			this->addPointLight(pLightPoint);

			startPos.z -= step;
		}

		startPos.x += step;
		startPos.z = 95.0f;
	}

	cwVector3D dir(1.0, -1.0, 1.0);
	dir.normalize();
	cwDirectionalLight* pDirLight = cwDirectionalLight::create(cwVector4D(dir), cwVector3D(0.2f, 0.2f, 0.2f));
	pDirLight->setCastShadow(CWTRUE);
	this->addDirectionalLight(pDirLight);

	//cwVector4D startPos(-40.0f, 5.0f, 40.0f, 1.0f);
	//for (CWUINT y = 0; y < 4; ++y) {
	//	for (CWUINT x = 0; x < 4; ++x) {
	//		for (CWUINT z = 0; z < 8; ++z) {
	//			CWFLOAT r = 0.5f + cwMathUtil::randFloat()*0.5f;
	//			CWFLOAT g = 0.5f + cwMathUtil::randFloat()*0.5f;
	//			CWFLOAT b = 0.5f + cwMathUtil::randFloat()*0.5f;

	//			cwPointLight* pLightPoint = cwPointLight::create(
	//				startPos,
	//				cwVector4D::ZERO,
	//				cwVector4D(r, g, b, 1.0f),
	//				cwVector4D(1.0f, 1.0f, 1.0f, 8.0f),
	//				cwVector4D(0.0f, 0.1f, 0.0f, 10.0f));
	//			this->addPointLight(pLightPoint);

	//			startPos.z -= 10.0f;
	//		}

	//		startPos.x += 20.0f;
	//		startPos.z = 40.0f;
	//	}

	//	startPos.x = -40.0f;
	//	startPos.y += 10.0f;
	//	startPos.z = 40.0f;
	//}
}

CWVOID TBDRScene::initScene()
{
	m_pEntityPlane->setPosition(0, 0, 0);
	this->addChild(m_pEntityPlane);

/*	m_pEntityBox01->setScale(4.0f, 4.0f, 4.0f);
	m_pEntityBox01->setPosition(0, 2.0f, 0);
	this->addChild(m_pEntityBox01);

	m_pEntityBox02->setScale(4.0f, 16.0f, 4.0f);
	m_pEntityBox02->setRotation(0, cwMathUtil::angleRadian(30), 0);
	m_pEntityBox02->setPosition(6.0, 8.0f, 0);
	this->addChild(m_pEntityBox02);

	m_pEntityBox03->setScale(4.0f, 4.0f, 4.0f);
	m_pEntityBox03->setRotation(0, -cwMathUtil::angleRadian(30), 0);
	m_pEntityBox03->setPosition(-6.0, 2.0f, -5.0);
	this->addChild(m_pEntityBox03);

	m_pEntitySphere->setScale(4.0f, 4.0f, 4.0f);
	m_pEntitySphere->setPosition(0, 10.0f, -10.0f);
	this->addChild(m_pEntitySphere);*/
}
