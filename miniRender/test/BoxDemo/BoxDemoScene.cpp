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
m_pTestCamera(nullptr),
m_pBoxRenderObj(nullptr),
m_pEntityClicked(nullptr)
{

}

BoxDemoScene::~BoxDemoScene()
{
	CW_SAFE_RELEASE_NULL(m_pEntityAxis);
	CW_SAFE_RELEASE_NULL(m_pTestCamera);
	CW_SAFE_RELEASE_NULL(m_pBoxRenderObj);
	m_pEntityClicked = nullptr;
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
	if (m_bTouchDown && pTouch) {
		/*cwRay ray = cwRepertory::getInstance().getEngine()->getRenderer()->getPickingRayWorld(pTouch);

		for (auto pEntity : m_nVecCollideEntities) {
			if (ray.intersection(pEntity->getBoundingBox())) {
				m_pEntityClicked = pEntity;
				break;
			}
		}*/

		m_pEntityClicked = cwRepertory::getInstance().getEngine()->getScreenClickNode(pTouch);
	}

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
		if (m_nVecEntities.size() > 0) {
			cwEntity* pLastEntity = m_nVecEntities.at(m_nVecEntities.size() - 1);
			pLastEntity->removeFromParent();

			if (m_pEntityClicked == pLastEntity) {
				m_pEntityClicked = nullptr;
			}

			m_nVecEntities.popBack();
		}
	}
	else if (isKeyDown(KeyCode::I)) {
		createRandomEntity();
	}

	static CWFLOAT fRadian = 0;
	static CWFLOAT fSpeed = cwMathUtil::cwPI*0.1f;
	if (fRadian >= 2.0f*cwMathUtil::cwPI) {
		fRadian -= 2.0f*cwMathUtil::cwPI;
	}

	fRadian += fSpeed*dt;
	cwMatrix4X4 mat;
	mat.setRotation(fRadian, 0, 0);
	m_nTestRay.m_nDir = cwVector3D(0, 0, 1.0f) * mat;
	m_nTestRay.m_nDir.normalize();

	m_pTestCamera->yaw(dt*cwMathUtil::cwPI*0.1f);
	m_pEntity01->rotate(0, dt*cwMathUtil::cwPI*0.03f, 0);

	//for (auto pEntity : m_nVecCollideEntities) {
	//	if (m_nTestRay.intersection(pEntity->getBoundingBox()))
	//		repertory.getEngine()->getRenderer()->renderPrimitive(pEntity->getBoundingBox(), cwColor::blue);
	//	else
	//		repertory.getEngine()->getRenderer()->renderPrimitive(pEntity->getBoundingBox(), cwColor::sliver);
	//}

	if (m_pEntityClicked) {
		repertory.getEngine()->getRenderer()->renderPrimitive(m_pEntityClicked->getBoundingBox(), cwColor::blue);
	}

	repertory.getEngine()->getRenderer()->renderPrimitive(m_nTestRay);
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

	m_pEntity01 = buildEntity();
	m_pEntity01->setPosition(cwVector3D(3.0, 3.0, 2.0));
	this->addChild(m_pEntity01);

	m_nVecCollideEntities.pushBack(m_pEntity01);

	//cwVector3D v(0, 0, 10.0f);
	//CWFLOAT fRadian = 0;
	//CWFLOAT fStep = cwMathUtil::cwPI*2.0f*0.1f;

	//for (CWUINT i = 0; i < 10; ++i) {
	//	cwEntity* pEntity = buildEntity();
	//	if (!pEntity) continue;

	//	cwMatrix4X4 mat;
	//	mat.setRotation(fRadian, 0, 0);

	//	pEntity->setPosition(v*mat);
	//	m_pEntity01->addChild(pEntity);

	//	m_nVecCollideEntities.pushBack(pEntity);

	//	fRadian += fStep;
	//}
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

	if (m_pEntity01) {
		m_pEntity01->addChild(pEntity);
		m_nVecEntities.pushBack(pEntity);
	}
}

void BoxDemoScene::buildCamera()
{
	m_pTestCamera = cwCamera::create();
	CW_SAFE_RETAIN(m_pTestCamera);

	m_pTestCamera->yaw(-0.5f*cwMathUtil::cwPI);
	m_pTestCamera->walk(-20);

	m_pTestCamera->updateProjMatrixFov(0.25f*cwMathUtil::cwPI, 800.0f / 600.0f, 1.0f, 30.0f);

	m_nTestRay.m_nOrigin.set(6.0f, 3.0f, -3.0f);
	m_nTestRay.m_nDir.set(0, 0, 1.0f);
	m_nTestRay.m_fT = 1000.0f;
}
