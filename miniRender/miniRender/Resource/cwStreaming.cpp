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

#include "cwStreaming.h"
#include "cwLoadBatch.h"
#include "Base/cwLog.h"
#include "Repertory/cwRepertory.h"
#include "Resource/cwResourceLoader.h"

NS_MINIR_BEGIN

cwStreaming::cwStreaming() : 
m_pLoadBatch(nullptr),
m_eState(eStreamStateOffline)
{

}

cwStreaming::~cwStreaming()
{
	CW_SAFE_RELEASE_NULL(m_pLoadBatch);
	cwLog::print("cwStreaming::~cwStreaming.\n");
}

CWVOID cwStreaming::streamPrepare()
{
	m_eState = eStreamStateStreaming;

	CW_SAFE_RELEASE_NULL(m_pLoadBatch);
	m_pLoadBatch = cwLoadBatch::create();
	CW_SAFE_RETAIN(m_pLoadBatch);
	if (m_pLoadBatch)
		m_pLoadBatch->m_pObjStreaming = this;
}

CWVOID cwStreaming::streamBegin()
{
	
}

CWVOID cwStreaming::streamEnd()
{
	m_eState = eStreamStateOnline;
}

CWVOID cwStreaming::streamClean()
{
	CW_SAFE_RELEASE_NULL(m_pLoadBatch);
}

CWVOID cwStreaming::streamRelease()
{
	if (!canRelease()) {
		streamCancel();
		return;
	}

	if (m_eState == eStreamStateOnline) {
		cwRemoveBatch* pRemoveBatch = this->buildRemoveBatch();
		if (pRemoveBatch) {
			cwRepertory::getInstance().getResourceLoader()->remove(pRemoveBatch);
		}
	}

	m_eState = eStreamStateOffline;
}

CWVOID cwStreaming::streamFailed()
{
	m_eState = eStreamStateOffline;
}

CWVOID cwStreaming::streamCancel()
{
	m_eState = eStreamStateCancel;
}

cwRemoveBatch* cwStreaming::buildRemoveBatch()
{
	return nullptr;
}

CWBOOL cwStreaming::canRelease()
{
	if (m_eState == eStreamStateStreaming) return CWFALSE;
	return CWTRUE;
}

NS_MINIR_END
