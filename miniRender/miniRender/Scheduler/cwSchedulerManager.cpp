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

#include "cwSchedulerManager.h"
#include "Base/cwFuncNode.h"

#include "uthash.h"

NS_MINIR_BEGIN

typedef cwFuncNode<cwScheduleProperty, CWVOID, CWFLOAT> FUNCNODE;

struct scheduleHashNode
{
	cwScheduleInterface* key;
	FUNCNODE* funNode;
	CWBOOL remove;
	UT_hash_handle hh;
};

cwSchedulerManager* cwSchedulerManager::create()
{
	cwSchedulerManager* pScheduleManager = new cwSchedulerManager();
	if (pScheduleManager && pScheduleManager->init()) {
		pScheduleManager->autorelease();
		return pScheduleManager;
	}

	CW_SAFE_DELETE(pScheduleManager);
	return nullptr;
}

cwSchedulerManager::cwSchedulerManager():
m_pHashData(NULL),
m_bUpdateLock(false)
{

}

cwSchedulerManager::~cwSchedulerManager()
{
	clearAll();
	m_pHashData = NULL;
}

bool cwSchedulerManager::init()
{
	return true;
}

bool cwSchedulerManager::schedule(SCHEDULE_UPDATE pFunc, cwScheduleInterface* pCaller, CWINT iPriority, CWFLOAT fFreq, CWFLOAT fWaiting, CWBOOL bOnce)
{
	if (!pFunc || !pCaller) return false;

	struct scheduleHashNode* f = NULL;
	HASH_FIND_PTR(m_pHashData, &pCaller, f);

	if (f) {
		//already added the func, update the schedule node property
		return true;
	}

	FUNCNODE* funNode = new FUNCNODE(std::bind(pFunc, pCaller, std::placeholders::_1));
	if (!funNode) return false;
	cwScheduleProperty property;
	property.bOnce = bOnce;
	property.fFreq = fFreq;
	property.iPriority = iPriority;
	funNode->setData(property);

	struct scheduleHashNode* hashNode = (struct scheduleHashNode*)malloc(sizeof(struct scheduleHashNode));
	if (!hashNode) {
		CW_SAFE_DELETE(funNode);
		return false;
	}

	hashNode->key = pCaller;
	hashNode->funNode = funNode;
	hashNode->remove = false;

	if (!m_bUpdateLock)
		HASH_ADD_PTR(m_pHashData, key, hashNode);
	else
		m_nVecTmpNode.push_back(hashNode);

	return true;
}

bool cwSchedulerManager::remove(SCHEDULE_UPDATE pFunc, cwScheduleInterface* pCaller)
{
	if (!pFunc || !pCaller) return false;

	struct scheduleHashNode* f = NULL;
	HASH_FIND_PTR(m_pHashData, &pCaller, f);
	if (f) {
		if (m_bUpdateLock)
			f->remove = true;
		else
			clear(f);
	}

	return true;
}

void cwSchedulerManager::clear()
{
	struct scheduleHashNode *s, *tmp;
	HASH_ITER(hh, m_pHashData, s, tmp) {
		if (s && s->remove) {
			clear(s);
		}
	}
}

void cwSchedulerManager::clear(struct scheduleHashNode* s)
{
	HASH_DEL(m_pHashData, s);
	CW_SAFE_DELETE(s->funNode);
	CW_SAFE_FREE(s);
}

void cwSchedulerManager::clearAll()
{
	struct scheduleHashNode *s, *tmp;
	HASH_ITER(hh, m_pHashData, s, tmp) {
		if (s) {
			clear(s);
		}
	}
}

void cwSchedulerManager::appendNode()
{
	if (!m_nVecTmpNode.empty()) {
		for (auto it = m_nVecTmpNode.begin(); it != m_nVecTmpNode.end(); ++it) {
			HASH_ADD_PTR(m_pHashData, key, (*it));
		}

		m_nVecTmpNode.clear();
	}
}

void cwSchedulerManager::update(float dt)
{
	m_bUpdateLock = true;

	struct scheduleHashNode *s, *tmp;
	HASH_ITER(hh, m_pHashData, s, tmp) {
		if (s && !s->remove) {
			(*(s->funNode))(dt);
			if (s->funNode->data().bOnce) {
				s->remove = true;
			}
		}
	}

	clear();
	appendNode();

	m_bUpdateLock = false;
}

NS_MINIR_END

