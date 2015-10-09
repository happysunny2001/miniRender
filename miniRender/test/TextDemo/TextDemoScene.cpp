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

#include "TextDemoScene.h"

TextDemoScene* TextDemoScene::create()
{
	TextDemoScene* pScene = new TextDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

TextDemoScene::TextDemoScene()
{

}

TextDemoScene::~TextDemoScene()
{

}

CWBOOL TextDemoScene::init()
{
	if (!cwScene::init()) return CWFALSE;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown = CW_CALLBACK_1(TextDemoScene::onTouchDown, this);
	pTouchListener->onTouchUp = CW_CALLBACK_1(TextDemoScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(TextDemoScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	this->schedulerUpdate();
	m_bTouchDown = false;

	buildText();

	return CWTRUE;
}

CWVOID TextDemoScene::onTouchDown(cwTouch* pTouch)
{
	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;

	m_bTouchDown = true;
}

CWVOID TextDemoScene::onTouchUp(cwTouch* pTouch)
{
	m_bTouchDown = false;
}

CWVOID TextDemoScene::onTouchMoving(cwTouch* pTouch)
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

CWVOID TextDemoScene::update(CWFLOAT dt)
{

}

CWVOID TextDemoScene::buildText()
{
	cwText* pText = cwText::create("1234567890", "Textures/NO_pvp_B99.png", '0', 40);
	if (pText) {
		pText->setPosition(cwVector3D::ZERO);
		this->addChild(pText);
	}
}
