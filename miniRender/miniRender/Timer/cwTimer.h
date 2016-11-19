/*
Copyright © 2015-2016 Ziwei Wang

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

#ifndef __CW_TIMER_H__
#define __CW_TIMER_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"

NS_MINIR_BEGIN

class cwTimer
{
public:
	cwTimer();
	virtual ~cwTimer();

	CWFLOAT deltaTime() const;
	CWUINT fps() const;

	void reset();
	void tick();

protected:
	CWINT64 m_iPrevTimeCounter;
	CWINT64 m_iCurrTimeCounter;
	CWDOUBLE m_dSecondPerCount;
	CWDOUBLE m_dDeltaTime;

	CWDOUBLE m_dFPSTimeCounter;
	CWUINT m_iFPSCounter;
	CWUINT m_iFPS;

};

NS_MINIR_END

#endif
