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

#include "cwTouchEvent.h"

NS_MINIR_BEGIN

cwTouch* cwTouch::create(const cwVector2D& pos)
{
	cwTouch* pTouch = new cwTouch();
	if (pTouch && pTouch->init(pos)) {
		pTouch->autorelease();
		return pTouch;
	}

	CW_SAFE_DELETE(pTouch);
	return nullptr;
}

cwTouch::cwTouch()
{
	
}

cwTouch::~cwTouch()
{

}

bool cwTouch::init(const cwVector2D& pos)
{
	m_nScreenPos = pos;
	return true;
}

cwTouchEvent* cwTouchEvent::create(TouchType type, const cwVector2D& pos)
{
	cwTouchEvent* pTouchEvent = new cwTouchEvent();
	if (pTouchEvent && pTouchEvent->init(type, pos)) {
		pTouchEvent->autorelease();
		return pTouchEvent;
	}

	CW_SAFE_DELETE(pTouchEvent);
	return nullptr;
}

cwTouchEvent::cwTouchEvent():
m_eTouchType(TouchTypeNone),
m_pTouch(nullptr)
{
	m_eType = EventTypeMouse;
}

cwTouchEvent::~cwTouchEvent()
{
	CW_SAFE_RELEASE_NULL(m_pTouch);
}

bool cwTouchEvent::init(TouchType type, const cwVector2D& pos)
{
	m_eTouchType = type;
	m_pTouch = cwTouch::create(pos);
	CW_SAFE_RETAIN(m_pTouch);

	return true;
}

NS_MINIR_END
