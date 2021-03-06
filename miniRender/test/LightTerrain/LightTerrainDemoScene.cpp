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

#include "LightTerrainDemoScene.h"

LightTerrainDemoScene* LightTerrainDemoScene::create()
{
	LightTerrainDemoScene* pScene = new LightTerrainDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

LightTerrainDemoScene::LightTerrainDemoScene():
m_pPointLightEntity(nullptr),
m_pSpotLightEntity(nullptr)
{

}

LightTerrainDemoScene::~LightTerrainDemoScene()
{
	CW_SAFE_RELEASE_NULL(m_pPointLightEntity);
	CW_SAFE_RELEASE_NULL(m_pSpotLightEntity);
}

bool LightTerrainDemoScene::init()
{
	if (!cwScene::init()) return CWFALSE;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown = CW_CALLBACK_1(LightTerrainDemoScene::onTouchDown, this);
	pTouchListener->onTouchUp = CW_CALLBACK_1(LightTerrainDemoScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(LightTerrainDemoScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	this->schedulerUpdate();

	m_bTouchDown = false;

	buildEntity();
	buildLight();

	return CWTRUE;
}

CWVOID LightTerrainDemoScene::onTouchDown(cwTouch* pTouch)
{
	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;

	m_bTouchDown = true;
}

CWVOID LightTerrainDemoScene::onTouchUp(cwTouch* pTouch)
{
	m_bTouchDown = false;
}

CWVOID LightTerrainDemoScene::onTouchMoving(cwTouch* pTouch)
{
	if (m_bTouchDown) {
		CWFLOAT dx = cwMathUtil::angleRadian(pTouch->getScreenPos().x - m_fLastX);
		CWFLOAT dy = cwMathUtil::angleRadian(pTouch->getScreenPos().y - m_fLastY);

		cwRepertory::getInstance().getEngine()->getDefaultCamera()->yaw(dx);
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->pitch(-dy);
	}

	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;
}

CWVOID LightTerrainDemoScene::update(CWFLOAT dt)
{
	if (isKeyDown(KeyCode::A)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->strafe(-50 * dt);
	}
	else if (isKeyDown(KeyCode::D)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->strafe(50 * dt);
	}

	if (isKeyDown(KeyCode::W)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->walk(50 * dt);
	}
	else if (isKeyDown(KeyCode::S)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->walk(-50 * dt);
	}
}

CWVOID LightTerrainDemoScene::buildEntity()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateGrid(200.0f, 200.0f, 80, 80, mesh);

	vector<cwVertexPosNormalTex> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].pos.y = repertory.getGeoGenerator()->getTerrainHeight(vecVertex[i].pos.x, vecVertex[i].pos.z);
		vecVertex[i].normal = repertory.getGeoGenerator()->getTerrainNormal(vecVertex[i].pos.x, vecVertex[i].pos.z);
		vecVertex[i].tex = mesh.nVertex[i].tex;
	}

	cwRenderObject* pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormalTex), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormalTex");

	cwTexture* pTexTerrain = cwRepertory::getInstance().getTextureManager()->getTexture("grass.dds");
	cwShader* pShader = repertory.getShaderManager()->getDefShader(eDefShaderLightingTex);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	cwMaterial* pMaterial = cwMaterial::create();
	if (pTexTerrain) {
		pMaterial->setDiffuseTexture(pTexTerrain);
	}

	cwEntity* pTerrain = cwEntity::create();
	pTerrain->setMaterial(pMaterial);
	pTerrain->setEffect(pEffect);
	pTerrain->setRenderObject(pRenderObj);
	pTerrain->setPosition(cwVector3D::ZERO);
	this->addChild(pTerrain);
}

CWVOID LightTerrainDemoScene::buildLightEntity()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateSphere(3.0f, 10, 10, mesh);

	vector<cwVertexPosColor> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].color = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);
	}

	cwRenderObject* pObjLight = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosColor), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosColor");

	cwShader* pShader = repertory.getShaderManager()->getDefShader(eDefShaderColor);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	cwMaterial* pMaterial = cwMaterial::create();

	m_pPointLightEntity = cwEntity::create();
	m_pPointLightEntity->setMaterial(pMaterial);
	m_pPointLightEntity->setEffect(pEffect);
	m_pPointLightEntity->setRenderObject(pObjLight);
	m_pPointLightEntity->setPosition(cwVector3D::ZERO);
	this->addChild(m_pPointLightEntity);

	m_pSpotLightEntity = cwEntity::create();
	m_pSpotLightEntity->setMaterial(pMaterial);
	m_pSpotLightEntity->setEffect(pEffect);
	m_pSpotLightEntity->setRenderObject(pObjLight);
	m_pSpotLightEntity->setPosition(cwVector3D::ZERO);
	this->addChild(m_pSpotLightEntity);
}

CWVOID LightTerrainDemoScene::buildLight()
{
	cwDirectionalLight* pLightDirectional = cwDirectionalLight::create(
		cwVector4D(0, -1.0, 0, 0),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f));
	this->addDirectionalLight(pLightDirectional);

	cwPointLight* pLightPoint = cwPointLight::create(
		cwVector4D(50.0f, 100.0f, 0, 0),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.0f, 0.1f, 0.0f, 1000.0f));
	this->addPointLight(pLightPoint);

	cwSpotLight* pLightSpot = cwSpotLight::create(
		cwVector4D(-50.0f, 100.0f, 0, 0),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.0f, 0.1f, 0.0f, 1000.0f),
		cwVector4D(0, -1.0, 0, 48.0f));
	this->addSpotLight(pLightSpot);
}
