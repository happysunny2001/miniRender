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

#ifndef __CW_TOUCH_EVENT_LISTENER_H__
#define __CW_TOUCH_EVENT_LISTENER_H__

#include "Base/cwMacros.h"
#include "Platform/cwPlatform.h"
#include "cwEventListener.h"
#include "cwTouchEvent.h"

#include <functional>

NS_MINIR_BEGIN

class cwTouchEventListener : public cwEventListener
{
public:
	typedef std::function<void(cwTouch* pTouch)> TouchDownFunc;
	typedef std::function<void(cwTouch* pTouch)> TouchUpFunc;
	typedef std::function<void(cwTouch* pTouch)> TouchMovingFunc;

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
	typedef std::function<void(cwTouch* pTouch)> TouchWheelFunc;
#endif

public:
	static cwTouchEventListener* create();

	cwTouchEventListener();
	virtual ~cwTouchEventListener();

	virtual bool init();

protected:
	virtual void onEvent(cwEvent* pEvent) override;

public:
	TouchDownFunc onTouchDown;
	TouchUpFunc onTouchUp;
	TouchMovingFunc onTouchMoving;

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
	TouchWheelFunc onTouchWheel;
#endif

};

NS_MINIR_END

#endif
