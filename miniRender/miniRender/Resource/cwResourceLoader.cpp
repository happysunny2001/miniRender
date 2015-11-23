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

#include "cwResourceLoader.h"
#include "cwLoadBatch.h"
#include "cwLoadResult.h"

#include <thread>
#include <atomic>

NS_MINIR_BEGIN

std::atomic<unsigned int> g_aResult(0);

CWVOID loadingProcessThread(cwResourceLoader*);

cwResourceLoader* cwResourceLoader::create()
{
	cwResourceLoader* pResourceLoader = new cwResourceLoader();
	if (pResourceLoader && pResourceLoader->init()) {
		pResourceLoader->autorelease();
		return pResourceLoader;
	}

	CW_SAFE_DELETE(pResourceLoader);
	return nullptr;
}

cwResourceLoader::cwResourceLoader()
{

}

cwResourceLoader::~cwResourceLoader()
{

}

CWBOOL cwResourceLoader::init()
{
	std::thread t(loadingProcessThread, this);
	t.detach();

	return CWTRUE;
}

CWBOOL cwResourceLoader::batchEmpty()
{
	return m_nQueueBatch.empty();
}

CWVOID cwResourceLoader::popBatch()
{
	if (m_nQueueBatch.empty()) return;

	cwLoadBatch* pFirst = m_nQueueBatch.front();
	CW_SAFE_RELEASE(pFirst);
	m_nQueueBatch.pop();
}

cwLoadBatch* cwResourceLoader::firstBatch()
{
	return m_nQueueBatch.front();
}

CWVOID cwResourceLoader::loadAsync(cwLoadBatch* pBatch)
{
	std::unique_lock<std::mutex> lock(m_nMutex);
	m_nQueueBatch.push(pBatch);
	CW_SAFE_RETAIN(pBatch);
	m_nCondNotEmpty.notify_all();
}

CWBOOL cwResourceLoader::loadSync(cwLoadBatch* pBatch)
{
	cwLoadResult* pResult = load(pBatch);
	if (!pResult) return CWFALSE;

	pResult->distribute();
	pResult->release();

	return true;
}

cwLoadResult* cwResourceLoader::load(cwLoadBatch* pBatch)
{
	cwLoadResult* pResult = cwLoadResult::create();
	pResult->setLoadBatch(pBatch);
	pResult->load();

	return pResult;
}

CWBOOL cwResourceLoader::update(float dt)
{
	if (g_aResult == 0)
		return false;

	cwLoadResult* pResult = nullptr;

	std::unique_lock<std::mutex> lockResult(m_nMutexResult);
	if (!m_nQueueResult.empty()){
		pResult = m_nQueueResult.front();
		m_nQueueResult.pop();
	}

	g_aResult = (unsigned int)(m_nQueueResult.size());

	lockResult.unlock();

	if (!pResult) return false;

	pResult->distribute();
	pResult->release();

	return true;
}

CWVOID loadingProcessThread(cwResourceLoader* pLoader)
{
	while (true) {
		std::unique_lock<std::mutex> lock(pLoader->m_nMutex);
		while (pLoader->batchEmpty()) {
			pLoader->m_nCondNotEmpty.wait(lock);
		}

		cwLoadBatch* pCurrBatch = pLoader->firstBatch();
		pCurrBatch->retain();
		pLoader->popBatch();

		lock.unlock();

		cwLoadResult* pResult = pLoader->load(pCurrBatch);
		if (pResult) {
			std::unique_lock<std::mutex> lockResult(pLoader->m_nMutexResult);

			pLoader->m_nQueueResult.push(pResult);
			g_aResult = (unsigned int)(pLoader->m_nQueueResult.size());

			lockResult.unlock();
		}
	}
}

NS_MINIR_END
