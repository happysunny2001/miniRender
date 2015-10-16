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

#include <sstream>

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

TextDemoScene::TextDemoScene():
m_pSpriteCenter(nullptr),
m_pSpriteLeft(nullptr),
m_pSpriteRight(nullptr),
m_pLabel(nullptr),
m_fNumber(0)
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

	cwKeyboardEventListener* pKeyListener = cwKeyboardEventListener::create();
	pKeyListener->onKeyDown = CW_CALLBACK_1(TextDemoScene::onKeyDown, this);
	this->addEventListener(pKeyListener);

	this->schedulerUpdate();

	m_bTouchDown = false;

	buildText();
	buildSprite();

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

CWVOID TextDemoScene::onKeyDown(cwKeyboard* pKey)
{
	if (pKey) {
		if (pKey->getKeyCode() == KeyCode::W) {
			cwRepertory::getInstance().getEngine()->getDefaultCamera()->walk(1.0f);
		}
		else if (pKey->getKeyCode() == KeyCode::S) {
			cwRepertory::getInstance().getEngine()->getDefaultCamera()->walk(-1.0f);
		}
		else if (pKey->getKeyCode() == KeyCode::A) {
			cwRepertory::getInstance().getEngine()->getDefaultCamera()->strafe(-1.0f);
		}
		else if (pKey->getKeyCode() == KeyCode::D) {
			cwRepertory::getInstance().getEngine()->getDefaultCamera()->strafe(1.0f);
		}
	}
}

CWVOID TextDemoScene::update(CWFLOAT dt)
{
	if (m_pSpriteCenter) {
		m_pSpriteCenter->rotate(dt*cwMathUtil::cwPI*0.1f);
	}

	if (m_pSpriteLeft) {
		m_pSpriteLeft->rotate(dt*cwMathUtil::cwPI*0.2f);
	}

	if (m_pSpriteRight) {
		m_pSpriteRight->rotate(-dt*cwMathUtil::cwPI*0.2f);
	}

	if (m_pLabel) {
		m_fNumber += dt;
		if (m_fNumber >= 1.0f) {
			stringstream ss;
			ss << (CWUINT)(m_fNumber);
			m_pLabel->setString(ss.str());
		}
	}
}

CWVOID TextDemoScene::buildText()
{
	cwText* pText = cwText::create("1234567890", "Textures/font_test.png", '0', 40);
	if (pText) {
		pText->setPosition(cwVector3D::ZERO);
		this->addChild(pText);
	}
}

CWVOID TextDemoScene::buildSprite()
{
	m_pSpriteCenter = cwSprite::create("Textures/cc.png");
	m_pSpriteCenter->setPosition(0, 0);
	cwRepertory::getInstance().getEngine()->addNode2D(m_pSpriteCenter);
	m_pSpriteCenter->setTag(100);

	m_pSpriteLeft = cwSprite::create("Textures/cc.png");
	m_pSpriteLeft->setPosition(-100, 0);
	m_pSpriteCenter->addChild(m_pSpriteLeft);
	m_pSpriteLeft->setTag(101);

	m_pSpriteRight = cwSprite::create("Textures/cc.png");
	m_pSpriteRight->setPosition(100, 0);
	m_pSpriteCenter->addChild(m_pSpriteRight);
	m_pSpriteRight->setTag(102);

	cwSprite* pSpLeftChild01 = cwSprite::create("Textures/cc.png");
	pSpLeftChild01->setPosition(-20, 80);
	m_pSpriteLeft->addChild(pSpLeftChild01);
	pSpLeftChild01->setTag(103);

	cwSprite* pSpRightChild01 = cwSprite::create("Textures/cc.png");
	pSpRightChild01->setPosition(20, -80);
	m_pSpriteRight->addChild(pSpRightChild01);
	pSpRightChild01->setTag(104);

	m_pLabel = cwLabel::create("0", "Textures/frame_text.png", '0', 10);
	m_pLabel->setPosition(-100, -100);
	cwRepertory::getInstance().getEngine()->addNode2D(m_pLabel);
	m_pLabel->setTag(200);
}
