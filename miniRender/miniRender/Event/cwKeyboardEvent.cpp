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

#include "cwKeyboardEvent.h"

NS_MINIR_BEGIN

cwKeyboard* cwKeyboard::create(KeyCode code)
{
	cwKeyboard* pKeyboard = new cwKeyboard();
	if (pKeyboard && pKeyboard->init(code)) {
		pKeyboard->autorelease();
		return pKeyboard;
	}

	CW_SAFE_DELETE(pKeyboard);
	return nullptr;
}

cwKeyboard::cwKeyboard():
m_eKeyCode(KeyCode::KeyNone)
{

}

cwKeyboard::~cwKeyboard()
{

}

CWBOOL cwKeyboard::init(KeyCode code)
{
	m_eKeyCode = code;
	return CWTRUE;
}

cwKeyboardEvent* cwKeyboardEvent::create(KeyCode code, KeyState state)
{
	cwKeyboardEvent* pEvent = new cwKeyboardEvent();
	if (pEvent && pEvent->init(code, state)) {
		pEvent->autorelease();
		return pEvent;
	}

	CW_SAFE_DELETE(pEvent);
	return nullptr;
}

cwKeyboardEvent::cwKeyboardEvent():
m_pKeyboard(nullptr),
m_eKeyState(KeyState::StateNone)
{
	m_eType = EventTypeKeyboard;
}

cwKeyboardEvent::~cwKeyboardEvent()
{
	CW_SAFE_RELEASE_NULL(m_pKeyboard);
}

CWBOOL cwKeyboardEvent::init(KeyCode code, KeyState state)
{
	m_eKeyState = state;

	m_pKeyboard = cwKeyboard::create(code);
	CW_SAFE_RETAIN(m_pKeyboard);

	return CWTRUE;
}

NS_MINIR_END
