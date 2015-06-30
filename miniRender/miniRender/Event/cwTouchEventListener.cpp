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

#include "cwTouchEventListener.h"
#include "cwEvent.h"
#include "cwTouchEvent.h"
#include "cwEventDefine.h"

NS_MINIR_BEGIN

cwTouchEventListener* cwTouchEventListener::create()
{
	cwTouchEventListener* pListener = new cwTouchEventListener();
	if (pListener && pListener->init()) {
		pListener->autorelease();
		return pListener;
	}

	CW_SAFE_DELETE(pListener);
	return nullptr;
}

cwTouchEventListener::cwTouchEventListener()
{

}

cwTouchEventListener::~cwTouchEventListener()
{

}

bool cwTouchEventListener::init()
{
	onTouchDown = nullptr;
	onTouchUp = nullptr;
	onTouchMoving = nullptr;

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
	onTouchWheel = nullptr;
#endif

	return true;
}

void cwTouchEventListener::onEvent(cwEvent* pEvent)
{
	if (!pEvent) return;
	if (pEvent->getEventType() != EventTypeMouse) return;
	if (pEvent->getEventState() != EventStateAlive) return;

	cwTouchEvent* pTouchEvent = static_cast<cwTouchEvent*>(pEvent);

	switch (pTouchEvent->getTouchType())
	{
	case TouchTypeDown:
		if (onTouchDown) {
			onTouchDown(pTouchEvent->getTouch());
		}
		break;
	case TouchTypeUp:
		if (onTouchUp) {
			onTouchUp(pTouchEvent->getTouch());
		}
		break;
	case TouchTypeMoving:
		if (onTouchMoving) {
			onTouchMoving(pTouchEvent->getTouch());
		}
		break;
#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
	case TouchTypeWheel:
		if (onTouchWheel) {
			onTouchWheel(pTouchEvent->getTouch());
		}
		break;
#endif
	default:
		break;
	}

	onBehaviour(pEvent);
}

NS_MINIR_END
