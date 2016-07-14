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

#include "cwObjectMonitor.h"
#include "Base/cwLog.h"
#include <iostream>

NS_MINIR_BEGIN

cwObjectMonitor::cwObjectMonitor()
{

}

cwObjectMonitor::~cwObjectMonitor()
{

}

CWVOID cwObjectMonitor::addRef(cwRef* pRef)
{
	std::unique_lock<std::mutex> lg(m_nMapMutex);
	m_nMapRefObject[pRef] = CWTRUE;
}

CWVOID cwObjectMonitor::removeRef(cwRef* pRef)
{
	std::unique_lock<std::mutex> lg(m_nMapMutex);
	m_nMapRefObject.erase(pRef);
}

CWVOID cwObjectMonitor::dump()
{
	cwLog::print("=========================");
	std::unique_lock<std::mutex> lg(m_nMapMutex);
	CWUINT index = 0;
	for (auto k : m_nMapRefObject) {
		auto t = typeid(*(k.first)).name();
		cwLog::print("index %d:%d, %s\n", index++, k.first->retainCount(), t);
	}
}

NS_MINIR_END
