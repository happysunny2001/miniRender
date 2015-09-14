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

#ifndef __CW_EVENT_DEFINE_H__
#define __CW_EVENT_DEFINE_H__

#include "Base/cwMacros.h"
#include "Math/cwMath.h"

NS_MINIR_BEGIN

typedef enum _EventType
{
	EventTypeNone = 0,
	EventTypeMouse,
	EventTypeKeyboard,
}EventType;

typedef enum _EventState
{
	EventStateNone = 0,
	EventStateAlive,
	EventStateDrop,
}EventState;

typedef enum _EventListenerState
{
	EventListenerStateNone = 0,
	EventListenerStateAlive,
	EventListenerStateDrop,
}EventListenerState;

typedef enum _EventListenerBehaviour
{
	EventListenerBehaviourNone = 0,
	EventListenerBehaviourNormal,
	EventListenerBehaviourSwallow,
}EventListenerBehaviour;

typedef enum _TouchType
{
	TouchTypeNone = 0,
	TouchTypeDown,
	TouchTypeUp,
	TouchTypeMoving,
	TouchTypeWheel,
}TouchType;

typedef enum _TouchButton
{
	TouchButtonNone = 0,
	TouchButtonLeft,
	TouchButtonRight,
	TouchButtonMiddle,
}TouchButton;

typedef enum _KeyCode
{
	KeyNone = 0,
	Back,
	Tab,
	Clear,
	Enter,
	Shift,
	Control,
	Alt,
	Pause,
	Caps,
	ESC,
	Space,
	PageUp,
	PageDown,
	End,
	Home,
	Left,
	Up,
	Right,
	Down,
	Select,
	Print,
	Execute,
	PrintScreen,
	Insert,
	Delete,
	Help,
	Zero,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	LWin,
	RWin,
	Num0,
	Num1,
	Num2,
	Num3,
	Num4,
	Num5,
	Num6,
	Num7,
	Num8,
	Num9,
	Add,
	Separator,
	Sub,
	Decimal,
	Div,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,
	NumLock,
	Scroll,
	LShift,
	RShift,
	LControl,
	RControl,
	LMenu,
	RMenu,
}KeyCode;

typedef enum _KeyState
{
	StateNone = 0,
	KeyDown = 1,
	keyUp = 2,
}KeyState;

#define CW_EVENT_DEFAULT_PRIORITY 0

NS_MINIR_END

#endif
