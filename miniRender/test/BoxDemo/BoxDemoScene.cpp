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
m_pEntityBox01(nullptr),
m_pEntityBox02(nullptr)
{

}

BoxDemoScene::~BoxDemoScene()
{
	CW_SAFE_RELEASE_NULL(m_pEntityAxis);
	CW_SAFE_RELEASE_NULL(m_pEntityBox01);
	CW_SAFE_RELEASE_NULL(m_pEntityBox02);
}

bool BoxDemoScene::init()
{
	if (!cwScene::init()) return false;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown   = CW_CALLBACK_1(BoxDemoScene::onTouchDown, this);
	pTouchListener->onTouchUp     = CW_CALLBACK_1(BoxDemoScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(BoxDemoScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	cwKeyboardEventListener* pKeyListener = cwKeyboardEventListener::create();
	pKeyListener->onKeyDown = CW_CALLBACK_1(BoxDemoScene::onKeyDown, this);
	pKeyListener->onKeyUp   = CW_CALLBACK_1(BoxDemoScene::onKeyUp, this);
	this->addEventListener(pKeyListener);

	this->schedulerUpdate();

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

	if (m_pEntityBox01) {
		m_pEntityBox01->rotate(0, dt*cwMathUtil::cwPI*0.1f, 0);
	}

	repertory.getEngine()->getRenderer()->renderPrimitive(m_pEntityBox01->getBoundingBox(), cwColor::sliver);
	repertory.getEngine()->getRenderer()->renderPrimitive(m_pEntityBox02->getBoundingBox(), cwColor::sliver);
	repertory.getEngine()->getRenderer()->renderPrimitive(m_pEntityBox01->getGroupBoundingBox(), cwColor::sliver);
}

void BoxDemoScene::buildEntity()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateBox(mesh);

	vector<cwVertexPosColor> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].color = cwVector4D(1.0f, 1.0f, 1.0f, 0.5f);
	}

	cwRenderObject *pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosColor), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosColor");

	cwShader* pShader = repertory.getShaderManager()->getDefShader(eDefShaderColor);
	cwMaterial* pMaterial = cwMaterial::create();
	pMaterial->m_nMatData.m_nDiffuse.w = 0.5f;
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	m_pEntityBox01 = cwEntity::create();
	m_pEntityBox01->setMaterial(pMaterial);
	m_pEntityBox01->setRenderObject(pRenderObj);
	m_pEntityBox01->setEffect(pEffect);
	CW_SAFE_RETAIN(m_pEntityBox01);

	m_pEntityBox02 = cwEntity::create();
	m_pEntityBox02->setMaterial(pMaterial);
	m_pEntityBox02->setRenderObject(pRenderObj);
	m_pEntityBox02->setEffect(pEffect);
	CW_SAFE_RETAIN(m_pEntityBox02);
}

void BoxDemoScene::buildAxis()
{
	m_pEntityAxis = cwRepertory::getInstance().getGeoGenerator()->generateCoordinateAxisEntity(10.0f);
	CW_SAFE_RETAIN(m_pEntityAxis);
}

void BoxDemoScene::buildScene()
{
	buildEntity();
	buildAxis();

	m_pEntityBox01->setPosition(cwVector3D(2.0f, 1.0f, 0.0f));
	this->addChild(m_pEntityBox01);

	m_pEntityBox02->setPosition(cwVector3D(-2.0f, 1.0f, -3.0f));
	m_pEntityBox01->addChild(m_pEntityBox02);
	this->addChild(m_pEntityAxis);

	cwRenderNode* pZeroNode = cwRenderNode::create();
	this->addChild(pZeroNode);

//	m_pEntityBox01->rotate(0, cwMathUtil::cwPI*0.1f, 0);
}
