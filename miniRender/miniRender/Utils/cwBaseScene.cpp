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

#include "cwBaseScene.h"
#include "Event/cwTouchEventListener.h"
#include "Event/cwKeyboardEventListener.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Camera/cwCamera.h"

NS_MINIR_BEGIN

cwBaseScene::cwBaseScene()
{

}

cwBaseScene::~cwBaseScene()
{

}

CWBOOL cwBaseScene::init()
{
	if (!cwScene::init()) return CWFALSE;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown = CW_CALLBACK_1(cwBaseScene::onTouchDown, this);
	pTouchListener->onTouchUp = CW_CALLBACK_1(cwBaseScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(cwBaseScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	cwKeyboardEventListener* pKeyListener = cwKeyboardEventListener::create();
	pKeyListener->onKeyDown = CW_CALLBACK_1(cwBaseScene::onKeyDown, this);
	this->addEventListener(pKeyListener);

	this->schedulerUpdate();

	m_bTouchDown = false;

	return CWTRUE;
}

CWVOID cwBaseScene::onTouchDown(cwTouch* pTouch)
{
	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;

	m_bTouchDown = true;
}

CWVOID cwBaseScene::onTouchUp(cwTouch* pTouch)
{
	m_bTouchDown = false;
}

CWVOID cwBaseScene::onTouchMoving(cwTouch* pTouch)
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

CWVOID cwBaseScene::update(CWFLOAT dt)
{

}

CWVOID cwBaseScene::onKeyDown(cwKeyboard* pKey)
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

NS_MINIR_END
