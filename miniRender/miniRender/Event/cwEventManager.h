﻿/*
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

#ifndef __CW_EVENT_MANAGER_H__
#define __CW_EVENT_MANAGER_H__

#include "Base/cwMacros.h"
#include "Ref/cwRef.h"
#include "Repertory/cwRepertory.h"
#include "Base/cwVector.h"

#include <mutex>

NS_MINIR_BEGIN

class cwEventListener;
class cwEvent;

class cwEventManager : public cwRef
{
public:
	void addEvent(cwEvent* pEvent);
	void removeEvent(cwEvent* pEvent);

	bool addListener(cwEventListener* pListener);
	bool addListener(cwEventListener* pListener, CWINT iPriority, bool swallow);
	void removeListener(cwEventListener* pListener);

	void dispatchEvent();

protected:
	static cwEventManager* create();

	cwEventManager();
	virtual ~cwEventManager();

	bool init();
	void clear();

protected:
	cwVector<cwEventListener*> m_nVecListener;
	cwVector<cwEvent*> m_nVecEvent;

	bool m_bDirty;
	std::mutex m_nEventMutex;
	std::mutex m_nListenerMutex;
	
	friend class cwRepertory;
};

NS_MINIR_END

#endif
