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

#define CW_EVENT_DEFAULT_PRIORITY 0

NS_MINIR_END

#endif
