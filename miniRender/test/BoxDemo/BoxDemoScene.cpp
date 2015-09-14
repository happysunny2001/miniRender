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
m_bTouchDown(CWFALSE)
{

}

BoxDemoScene::~BoxDemoScene()
{

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
	if (isKeyDown(KeyCode::A)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->strafe(-10 * dt);
	}
	else if (isKeyDown(KeyCode::D)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->strafe(10 * dt);
	}

	if (isKeyDown(KeyCode::W)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->walk(10 * dt);
	}
	else if (isKeyDown(KeyCode::S)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->walk(-10 * dt);
	}
}
