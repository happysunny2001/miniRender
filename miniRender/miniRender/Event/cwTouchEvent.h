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

#ifndef __CW_TOUCH_EVENT_H__
#define __CW_TOUCH_EVENT_H__

#include "Base/cwMacros.h"
#include "Math/cwMath.h"
#include "Platform/cwPlatform.h"
#include "cwEvent.h"
#include "cwEventDefine.h"

NS_MINIR_BEGIN

class cwTouch : public cwRef
{
public:
	static cwTouch* create(const cwVector2D& pos);

	cwTouch();
	virtual ~cwTouch();

	virtual bool init(const cwVector2D& pos);

	const cwVector2D& getScreenPos() const { return m_nScreenPos; }

protected:
	cwVector2D m_nScreenPos;

};

class cwTouchEvent : public cwEvent
{
public:
	static cwTouchEvent* create(TouchType type, const cwVector2D& pos);

	cwTouchEvent();
	virtual ~cwTouchEvent();

	virtual bool init(TouchType type, const cwVector2D& pos);

	TouchType getTouchType() const { return m_eTouchType; }
	cwTouch* getTouch() { return m_pTouch; }

protected:
	TouchType m_eTouchType;
	cwTouch* m_pTouch;

};

NS_MINIR_END

#endif
