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

#include "TerrainDemoScene.h"

#include <sstream>

TerrainDemoScene* TerrainDemoScene::create()
{
	TerrainDemoScene* pScene = new TerrainDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

TerrainDemoScene::TerrainDemoScene() : 
m_pTerrain(nullptr),
m_nCurrKeyCode(KeyCode::KeyNone)
{

}

TerrainDemoScene::~TerrainDemoScene()
{
	CW_SAFE_RELEASE_NULL(m_pTerrain);
}

CWBOOL TerrainDemoScene::init()
{
	if (!cwBaseScene::init()) return CWFALSE;

	buildTerrain();
	buildLight();
	buildLabel();

	//this->createSkyDome("snowcube1024.dds");

	return CWTRUE;
}

CWVOID TerrainDemoScene::update(CWFLOAT dt)
{
	updateCamera(dt);

	cwRepertory& repertory = cwRepertory::getInstance();

	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getDefaultCamera();
	const cwVector3D& pos = pCamera->getPos();

	std::stringstream ss;
	ss << (int)pos.x;
	m_pLblPosX->setString(ss.str());

	ss.str("");
	ss << (int)pos.y;
	m_pLblPosY->setString(ss.str());

	ss.str("");
	ss << (int)pos.z;
	m_pLblPosZ->setString(ss.str());

	//cwTerrainTile* pTile = m_pTerrain->getTerrainTile();
	//int iSect = pCamera->getFrustum().intersection(pTile->getBoundingBox());
	//if (pCamera->getFrustum().isCollide(iSect)) {
	//	repertory.getEngine()->getRenderer()->renderPrimitive(pTile->getBoundingBox(), cwColor::blue);
	//}
	//else {
	//	repertory.getEngine()->getRenderer()->renderPrimitive(pTile->getBoundingBox(), cwColor::white);
	//}

	//CWFLOAT y = m_pTerrain->getHeight(pos);
	//pCamera->setPos(cwVector3D(pos.x, y + 15.0f, pos.z));

	//std::stringstream ss;
	//ss << (int)pos.x;
	//m_pLblPosX->setString(ss.str());

	//ss.str("");
	//ss << pos.y;
	//m_pLblPosY->setString(ss.str());

	//ss.str("");
	//ss << (int)pos.z;
	//m_pLblPosZ->setString(ss.str());
}

CWVOID TerrainDemoScene::buildTerrain()
{
	m_pTerrain = cwRepertory::getInstance().getDevice()->createTerrain("terrain.xml");
	CW_SAFE_RETAIN(m_pTerrain);
	this->addChild(m_pTerrain);
}

CWVOID TerrainDemoScene::buildLight()
{
	cwDirectionalLight* pLightDirectional01 = cwDirectionalLight::create(
		cwVector4D(0.707f, 0.707f, 0.0f, 0.0f),
		cwVector4D(0.3f, 0.3f, 0.3f, 1.0f),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f),
		cwVector4D(0.8f, 0.8f, 0.7f, 1.0f));
	this->addDirectionalLight(pLightDirectional01);

	cwDirectionalLight* pLightDirectional02 = cwDirectionalLight::create(
		cwVector4D(0.57735f, -0.57735f, 0.57735f, 0),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f));
	this->addDirectionalLight(pLightDirectional02);

	cwDirectionalLight* pLightDirectional03 = cwDirectionalLight::create(
		cwVector4D(-0.57735f, -0.57735f, -0.57735f, 0),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f));
	this->addDirectionalLight(pLightDirectional03);
}

CWVOID TerrainDemoScene::buildLabel()
{
	m_pLblPosX = cwLabel::create("0", "frame_text.png", '0', 10);
	m_pLblPosX->setPosition(-100, -100);
	cwRepertory::getInstance().getEngine()->addNode2D(m_pLblPosX);
	m_pLblPosX->setTag(200);

	m_pLblPosY = cwLabel::create("0", "frame_text.png", '0', 10);
	m_pLblPosY->setPosition(-100, -130);
	cwRepertory::getInstance().getEngine()->addNode2D(m_pLblPosY);
	m_pLblPosY->setTag(201);

	m_pLblPosZ = cwLabel::create("0", "frame_text.png", '0', 10);
	m_pLblPosZ->setPosition(-100, -160);
	cwRepertory::getInstance().getEngine()->addNode2D(m_pLblPosZ);
	m_pLblPosZ->setTag(202);
}

CWVOID TerrainDemoScene::onKeyDown(cwKeyboard* pKey)
{
	m_nCurrKeyCode = pKey->getKeyCode();
}

CWVOID TerrainDemoScene::updateCamera(CWFLOAT dt)
{
	static CWFLOAT fRotSpeed = cwMathUtil::cwPI;
	static CWFLOAT fMovSpeed = 200.0f;

	if (m_nCurrKeyCode == KeyCode::Left) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->yaw(-fRotSpeed*dt);
	}
	else if(m_nCurrKeyCode == KeyCode::Right) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->yaw(fRotSpeed*dt);
	}
	else if (m_nCurrKeyCode == KeyCode::Up) {
		cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getDefaultCamera();
		cwVector3D endPos = m_pTerrain->getMovedPosition(pCamera->getPos(), pCamera->getLookDir(), fMovSpeed*dt);
		endPos.y += 5.0f;
		pCamera->setPos(endPos);
	}
	else if (m_nCurrKeyCode == KeyCode::Down) {
		cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getDefaultCamera();
		cwVector3D endPos = m_pTerrain->getMovedPosition(pCamera->getPos(), -pCamera->getLookDir(), fMovSpeed*dt);
		endPos.y += 5.0f;
		pCamera->setPos(endPos);
	}

	m_nCurrKeyCode = KeyCode::KeyNone;
}
