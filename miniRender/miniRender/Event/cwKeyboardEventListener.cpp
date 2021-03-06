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

#include "cwKeyboardEventListener.h"

NS_MINIR_BEGIN

cwKeyboardEventListener* cwKeyboardEventListener::create()
{
	cwKeyboardEventListener* pEventListener = new cwKeyboardEventListener();
	if (pEventListener && pEventListener->init()) {
		pEventListener->autorelease();
		return pEventListener;
	}

	CW_SAFE_DELETE(pEventListener);
	return nullptr;
}

cwKeyboardEventListener::cwKeyboardEventListener()
{

}

cwKeyboardEventListener::~cwKeyboardEventListener()
{

}

CWBOOL cwKeyboardEventListener::init()
{
	onKeyDown = nullptr;
	onKeyUp = nullptr;

	return CWTRUE;
}

void cwKeyboardEventListener::onEvent(cwEvent* pEvent)
{
	if (pEvent->getEventType() != EventTypeKeyboard) return;

	cwKeyboardEvent* pKeyEvent = static_cast<cwKeyboardEvent*>(pEvent);

	switch (pKeyEvent->getKeyState())
	{
	case KeyState::KeyDown:
		if (onKeyDown) {
			onKeyDown(pKeyEvent->getKeyboard());
		}
		break;
	case KeyState::keyUp:
		if (onKeyUp) {
			onKeyUp(pKeyEvent->getKeyboard());
		}
		break;
	default:
		break;
	}

	onBehaviour(pEvent);
}

NS_MINIR_END
