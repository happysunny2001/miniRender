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

#include "cwTimer.h"

#include <windows.h>

NS_MINI_BEGIN

cwTimer::cwTimer():
m_iPrevTimeCounter(0),
m_iCurrTimeCounter(0),
m_dSecondPerCount(0),
m_dDeltaTime(0),
m_iFPS(0),
m_dFPSTimeCounter(0),
m_iFPSCounter(0)
{
	CWINT64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	m_dSecondPerCount = 1.0 / (CWDOUBLE)countsPerSecond;
}

cwTimer::~cwTimer()
{

}

CWFLOAT cwTimer::deltaTime() const
{
	return (float)m_dDeltaTime;
}

CWUINT cwTimer::fps() const
{
	return m_iFPS;
}

void cwTimer::reset()
{
	CWINT64 currCounter;
	QueryPerformanceCounter((LARGE_INTEGER*)&currCounter);
	m_iPrevTimeCounter = currCounter;
	m_iCurrTimeCounter = currCounter;
	m_dDeltaTime = 0;
}

void cwTimer::tick()
{
	CWINT64 currCounter;
	QueryPerformanceCounter((LARGE_INTEGER*)&currCounter);

	m_iCurrTimeCounter = currCounter;
	m_dDeltaTime = (m_iCurrTimeCounter - m_iPrevTimeCounter) * m_dSecondPerCount;
	if (m_dDeltaTime < 0) m_dDeltaTime = 0;

	m_iPrevTimeCounter = m_iCurrTimeCounter;

	m_dFPSTimeCounter += m_dDeltaTime;
	m_iFPSCounter++;
	if (m_dFPSTimeCounter >= 1.0) {
		m_iFPS = m_iFPSCounter;
		m_iFPSCounter = 0;
		m_dFPSTimeCounter = 0;
	}
}

NS_MINI_END