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

#include "BoxDemoScene.h"

#include <sstream>

BoxDemoScene* BoxDemoScene::create()
{
	BoxDemoScene* pScene = new BoxDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

BoxDemoScene::BoxDemoScene():
m_bTouchDown(CWFALSE),
m_pEntityAxis(nullptr),
//m_pEntityBox01(nullptr),
//m_pEntityBox02(nullptr),
m_pTestCamera(nullptr),
m_pBoxRenderObj(nullptr)
{

}

BoxDemoScene::~BoxDemoScene()
{
	CW_SAFE_RELEASE_NULL(m_pEntityAxis);
	//CW_SAFE_RELEASE_NULL(m_pEntityBox01);
	//CW_SAFE_RELEASE_NULL(m_pEntityBox02);
	CW_SAFE_RELEASE_NULL(m_pTestCamera);
	CW_SAFE_RELEASE_NULL(m_pBoxRenderObj);
}

bool BoxDemoScene::init()
{
	if (!cwScene::init()) return false;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown   = CW_CALLBACK_1(BoxDemoScene::onTouchDown, this);
	pTouchListener->onTouchUp     = CW_CALLBACK_1(BoxDemoScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(BoxDemoScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	//cwKeyboardEventListener* pKeyListener = cwKeyboardEventListener::create();
	//pKeyListener->onKeyDown = CW_CALLBACK_1(BoxDemoScene::onKeyDown, this);
	//pKeyListener->onKeyUp   = CW_CALLBACK_1(BoxDemoScene::onKeyUp, this);
	//this->addEventListener(pKeyListener);

	this->schedulerUpdate();

	buildCamera();
	buildScene();

	return true;
}

void BoxDemoScene::onTouchDown(cwTouch* pTouch)
{
	wstringstream wss;
	wss << pTouch->getScreenPos().x << "," << pTouch->getScreenPos().y << "\n";
	OutputDebugString(wss.str().c_str());

	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;

	m_bTouchDown = true;
}

void BoxDemoScene::onTouchUp(cwTouch* pTouch)
{
	m_bTouchDown = false;
}

void BoxDemoScene::onTouchMoving(cwTouch* pTouch)
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

void BoxDemoScene::update(CWFLOAT dt)
{
	cwRepertory& repertory = cwRepertory::getInstance();

	if (isKeyDown(KeyCode::A)) {
		repertory.getEngine()->getDefaultCamera()->strafe(-10 * dt);
	}
	else if (isKeyDown(KeyCode::D)) {
		repertory.getEngine()->getDefaultCamera()->strafe(10 * dt);
	}

	if (isKeyDown(KeyCode::W)) {
		repertory.getEngine()->getDefaultCamera()->walk(10 * dt);
	}
	else if (isKeyDown(KeyCode::S)) {
		repertory.getEngine()->getDefaultCamera()->walk(-10 * dt);
	}
	else if (isKeyDown(KeyCode::P)) {
		if (m_nVecEntities.size() > 1) {
			cwEntity* pLastEntity = m_nVecEntities.at(m_nVecEntities.size() - 1);
			pLastEntity->removeFromParent();
			m_nVecEntities.popBack();
		}
	}
	else if (isKeyDown(KeyCode::I)) {
		createRandomEntity();
	}

	m_pTestCamera->yaw(dt*cwMathUtil::cwPI*0.1f);

	if (m_nVecEntities.size() >= 1) {
		m_nVecEntities.at(0)->rotate(0, dt*cwMathUtil::cwPI*0.03f, 0);
	}

	//repertory.getEngine()->getRenderer()->renderPrimitive(m_pEntityBox01->getBoundingBox(), cwColor::sliver);
	//repertory.getEngine()->getRenderer()->renderPrimitive(m_pEntityBox02->getBoundingBox(), cwColor::sliver);
	//repertory.getEngine()->getRenderer()->renderPrimitive(m_pEntityBox01->getGroupBoundingBox(), cwColor::sliver);
	repertory.getEngine()->getSpatial()->renderPrimitiveFrame();
	repertory.getEngine()->getRenderer()->renderPrimitive(m_pTestCamera);

	//for (auto pEntity : m_nVecEntities)
	//	pEntity->setVisible(CWTRUE);

	//cwVector<cwRenderNode*> vecNode;
	//repertory.getEngine()->getSpatial()->intersection(m_pTestCamera->getFrustum(), vecNode, eSceneObjectEntity, CWTRUE);

	//for (auto pEntity : m_nVecEntities)
	//	pEntity->setVisible(CWFALSE);

	//for (auto pNode : vecNode) {
	//	pNode->setVisible(CWTRUE);
	//}
}

void BoxDemoScene::buildRenderObject()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateBox(mesh);

	vector<cwVertexPosColor> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].color = cwVector4D(1.0f, 1.0f, 1.0f, 0.5f);
	}

	m_pBoxRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosColor), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosColor");
	CW_SAFE_RETAIN(m_pBoxRenderObj);
}

cwEntity* BoxDemoScene::buildEntity()
{
	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderColor);
	cwMaterial* pMaterial = cwMaterial::create();
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	cwEntity* pEntity = cwEntity::create();
	pEntity->setMaterial(pMaterial);
	pEntity->setRenderObject(m_pBoxRenderObj);
	pEntity->setEffect(pEffect);

	return pEntity;
}

void BoxDemoScene::buildAxis()
{
	m_pEntityAxis = cwRepertory::getInstance().getGeoGenerator()->generateCoordinateAxisEntity(10.0f);
	CW_SAFE_RETAIN(m_pEntityAxis);
	this->addChild(m_pEntityAxis);
}

void BoxDemoScene::buildScene()
{
	buildRenderObject();
	buildAxis();

	cwEntity* pEntityBox01 = buildEntity();
	pEntityBox01->setPosition(cwVector3D(3.0, 3.0, 2.0));
	this->addChild(pEntityBox01);
	m_nVecEntities.pushBack(pEntityBox01);

	//cwEntity* pEntityBox02 = buildEntity();
	//pEntityBox02->setPosition(cwVector3D(-5.0f, 1.0f, -3.0f));
	//pEntityBox01->addChild(pEntityBox02);
	//m_nVecEntities.pushBack(pEntityBox02);

//	m_pEntityBox01->rotate(0, cwMathUtil::cwPI*0.1f, 0);
}

CWVOID BoxDemoScene::createRandomEntity()
{
	const cwAABB& worldSpace = cwRepertory::getInstance().getEngine()->getSpatial()->getBoundingBox();
	cwEntity* pEntity = buildEntity();
	if (!pEntity) return;

	cwPoint3D pos;
	pos.x = worldSpace.m_nMin.x + rand() % CWUINT((worldSpace.m_nMax.x - worldSpace.m_nMin.x)*0.8f);
	pos.y = worldSpace.m_nMin.y + rand() % CWUINT((worldSpace.m_nMax.y - worldSpace.m_nMin.y)*0.8f);
	pos.z = worldSpace.m_nMin.z + rand() % CWUINT((worldSpace.m_nMax.z - worldSpace.m_nMin.z)*0.8f);

	pEntity->setPosition(pos);

	if (m_nVecEntities.size() >= 1) {
		m_nVecEntities.at(0)->addChild(pEntity);
		m_nVecEntities.pushBack(pEntity);
	}
}

void BoxDemoScene::buildCamera()
{
	m_pTestCamera = cwCamera::create();
	CW_SAFE_RETAIN(m_pTestCamera);

	m_pTestCamera->yaw(-0.5f*cwMathUtil::cwPI);
	m_pTestCamera->walk(-20);

	m_pTestCamera->updateProjMatrix(0.25f*cwMathUtil::cwPI, 800.0f / 600.0f, 1.0f, 30.0f);
}
