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

#ifndef __CW_KEYBOARD_EVENT_H__
#define __CW_KEYBOARD_EVENT_H__

#include "Base/cwMacros.h"
#include "Math/cwMath.h"
#include "Platform/cwPlatform.h"
#include "cwEvent.h"
#include "cwEventDefine.h"

NS_MINIR_BEGIN

class cwKeyboard : public cwRef
{
public:
	static cwKeyboard* create(KeyCode code);

	cwKeyboard();
	virtual ~cwKeyboard();

	virtual CWBOOL init(KeyCode code);

	KeyCode getKeyCode() const { return m_eKeyCode; }

protected:
	KeyCode m_eKeyCode;
};

class cwKeyboardEvent : public cwEvent
{
public:
	static cwKeyboardEvent* create(KeyCode code, KeyState state);

	cwKeyboardEvent();
	virtual ~cwKeyboardEvent();

	virtual CWBOOL init(KeyCode code, KeyState state);
	
	cwKeyboard* getKeyboard() { return m_pKeyboard; }
	KeyState getKeyState() const { return m_eKeyState; }

protected:
	KeyState m_eKeyState;
	cwKeyboard* m_pKeyboard;

};

NS_MINIR_END

#endif
