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

#include "cwEventManager.h"
#include "cwEvent.h"
#include "cwEventListener.h"

#include <algorithm>

NS_MINIR_BEGIN

static bool eventListenerComp(cwEventListener* left, cwEventListener* right)
{
	return left->getPriority() < right->getPriority();
}

cwEventManager* cwEventManager::create()
{
	cwEventManager* pManager = new cwEventManager();
	if (pManager && pManager->init()) {
		pManager->autorelease();
		return pManager;
	}

	CW_SAFE_DELETE(pManager);
	return nullptr;
}

cwEventManager::cwEventManager() 
{

}

cwEventManager::~cwEventManager()
{

}

bool cwEventManager::init()
{
	m_bDirty = false;
	return true;
}

void cwEventManager::addEvent(cwEvent* pEvent)
{
	if (!pEvent) return;

	std::lock_guard<std::mutex> lg(m_nEventMutex);
	m_nVecAppendEvent.pushBack(pEvent);
}

void cwEventManager::removeEvent(cwEvent* pEvent)
{
	if (!pEvent) return;
	pEvent->setEventState(EventStateDrop);
}

bool cwEventManager::addListener(cwEventListener* pListener)
{
	if (!pListener) return false;
	return addListener(pListener, CW_EVENT_DEFAULT_PRIORITY, false);
}

bool cwEventManager::addListener(cwEventListener* pListener, CWINT iPriority, bool swallow)
{
	if (!pListener) return false;
	if (m_nVecListener.contains(pListener)) return false;

	pListener->setPriority(iPriority);
	if (swallow)
		pListener->setBehaviour(EventListenerBehaviourSwallow);
	else
		pListener->setBehaviour(EventListenerBehaviourNormal);

	m_bDirty = true;

	{
		std::lock_guard<std::mutex> lg(m_nListenerMutex);
		m_nVecAppendListener.pushBack(pListener);
	}
	
	return true;
}

void cwEventManager::removeListener(cwEventListener* pListener)
{
	if (!pListener) return;
	if (m_nVecListener.contains(pListener)) {
		pListener->setState(EventListenerStateDrop);
		m_bDirty = true;
	} 
}

void cwEventManager::dispatchEvent()
{
	if (m_nVecEvent.empty() || m_nVecListener.empty()) {
		clear();
		append();
		return;
	}

	if (m_bDirty) {
		//sort listener
		std::sort(m_nVecListener.begin(), m_nVecListener.end(), eventListenerComp);
		m_bDirty = false;
	}

	for (auto itListener = m_nVecListener.begin(); itListener != m_nVecListener.end(); ++itListener) {
		if ((*itListener)->getState() == EventListenerStateAlive) {
			for (auto itEvent = m_nVecEvent.begin(); itEvent != m_nVecEvent.end(); ++itEvent) {
				(*itListener)->onEvent(*itEvent);
			}
		}
	}

	clear();
	append();
}

void cwEventManager::clear()
{
	cwVector<cwEventListener*> tmpVecListener;

	for (auto it = m_nVecListener.begin(); it != m_nVecListener.end(); ++it) {
		if ((*it)->getState() == EventListenerStateDrop)
			tmpVecListener.pushBack(*it);
	}

	if (!tmpVecListener.empty()) {
		for (auto it = tmpVecListener.begin(); it != tmpVecListener.end(); ++it) {
			m_nVecListener.erase(*it);
		}
	}

	tmpVecListener.clear();
	m_nVecEvent.clear();
}

void cwEventManager::append()
{
	if (true) {
		std::lock_guard<std::mutex> lg(m_nEventMutex);
		if (!m_nVecAppendEvent.empty())
			m_nVecEvent.pushBack(m_nVecAppendEvent);
		m_nVecAppendEvent.clear();
	}

	if (true) {
		std::lock_guard<std::mutex> lg(m_nListenerMutex);
		if (!m_nVecAppendListener.empty())
			m_nVecListener.pushBack(m_nVecAppendListener);
		m_nVecAppendListener.clear();
	}
}

NS_MINIR_END

