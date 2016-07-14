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

#ifndef __CW_OBJECT_MONITOR_H__
#define __CW_OBJECT_MONITOR_H__

#include "Base/cwMacros.h"
#include "cwRef.h"
#include "Repertory/cwRepertory.h"

#include <unordered_map>
#include <mutex>
using namespace std;

NS_MINIR_BEGIN

class cwObjectMonitor
{
public:
	~cwObjectMonitor();

	CWVOID addRef(cwRef* pRef);
	CWVOID removeRef(cwRef* pRef);
	CWVOID dump();

protected:
	cwObjectMonitor();

	friend class cwRepertory;

protected:
	unordered_map<cwRef*, CWBOOL> m_nMapRefObject;
	std::mutex m_nMapMutex;
};

NS_MINIR_END

#endif
