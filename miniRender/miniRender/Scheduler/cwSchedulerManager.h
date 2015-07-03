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

#ifndef __CW_SCHEDULER_MANAGER_H__
#define __CW_SCHEDULER_MANAGER_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Ref/cwRef.h"
#include "Repertory/cwRepertory.h"
#include "cwScheduleInterface.h"

NS_MINIR_BEGIN

struct scheduleHashNode;

class cwScheduleProperty
{
	CWBOOL bOnce;
	CWINT iPriority;
	CWFLOAT fFreq;
};

class cwSchedulerManager : public cwRef
{
public:
	virtual ~cwSchedulerManager();

	virtual bool init();

	virtual bool schedule(SCHEDULE_UPDATE pFunc, cwScheduleInterface* pCaller, CWINT iPriority, CWFLOAT fFreq, CWFLOAT fWaiting, CWBOOL bOnce);
	virtual bool remove(SCHEDULE_UPDATE pFunc, cwScheduleInterface* pCaller);

	virtual void update(float dt);

protected:
	static cwSchedulerManager* create();
	cwSchedulerManager();

	virtual void clear();
	virtual void clear(struct scheduleHashNode* s);
	virtual void appendNode();
	virtual void clearAll();

	friend class cwRepertory;

protected:
	struct scheduleHashNode* m_pHashData;
	std::vector<struct scheduleHashNode*> m_nVecTmpNode;

	bool m_bUpdateLock;

};

NS_MINIR_END

#endif
