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

#include "cwPerformanceTimer.h"
#include "Base/cwLog.h"

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
#include <windows.h>
#endif

NS_MINIR_BEGIN

cwPerformanceTimer::cwPerformanceTimer()
{
#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
	LARGE_INTEGER tmp;
	QueryPerformanceFrequency(&tmp);
	m_dFreq = 1.0/(CWDOUBLE)(tmp.QuadPart);
#endif
}

cwPerformanceTimer::~cwPerformanceTimer()
{

}

CWVOID cwPerformanceTimer::begin()
{
#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
	QueryPerformanceCounter((LARGE_INTEGER*)&m_iBegin);
#endif
}

CWDOUBLE cwPerformanceTimer::end()
{
#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
	QueryPerformanceCounter((LARGE_INTEGER*)&m_iEnd);
	return (m_iEnd - m_iBegin)*m_dFreq;
#endif

	return 0;
}

CWVOID cwPerformanceTimer::print(const CWCHAR* pcType)
{
	CWDOUBLE f = end();
	const CWCHAR* pcT = pcType ? pcType : "";
	cwLog::print("[%s]time:%f\n", pcT, f);
}

NS_MINIR_END
