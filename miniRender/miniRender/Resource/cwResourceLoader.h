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

#ifndef __CW_RESOURCE_LOADER_H__
#define __CW_RESOURCE_LOADER_H__

#include "Base/cwMacros.h"
#include "Ref/cwRef.h"
#include "Repertory/cwRepertory.h"
#include "Platform/cwFileSystem.h"

#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include <functional>

NS_MINIR_BEGIN

class cwLoadBatch;
class cwLoadResult;
class cwResourceInfo;
class cwRemoveBatch;
class cwStreaming;

class cwResourceLoader : public cwRef
{
public:
	virtual ~cwResourceLoader();

	CWVOID loadAsync(cwLoadBatch* pBatch);
	CWVOID loadAsync(cwStreaming* pObjStreaming);
	CWBOOL loadSync(cwLoadBatch* pBatch);

	CWVOID remove(cwRemoveBatch* pBatch);

	CWBOOL update(float dt);

	CWSTRING getFullPath(const CWSTRING& strFileName);
	CWSTRING getTextureFullPath(const CWSTRING& strFileName);
	CWSTRING getShaderFullPath(const CWSTRING& strFileName);

	cwData* getShaderData(const CWSTRING& strFileName);
	cwData* getTextureData(const CWSTRING& strFileName);
	cwData* getFileData(const CWSTRING& strFileName);

	inline int getThreadState() { return m_iThreadState; }
	inline void setThreadState(int iState) { m_iThreadState = iState; }

private:
	static cwResourceLoader* create();

	cwResourceLoader();
	CWBOOL init();
	CWVOID loadConfigure();

	CWBOOL batchEmpty();

	CWVOID popBatch();
	cwLoadBatch* firstBatch();

	cwLoadResult* load(cwLoadBatch* pBatch);

	CWVOID remove(cwResourceInfo& resInfo);
	CWVOID exit();

	friend class cwRepertory;
	friend CWVOID loadingProcessThread(cwResourceLoader*);

protected:
	std::unordered_map<CWSTRING, std::vector<CWSTRING>> m_nMapResLocation;

	std::queue<cwLoadBatch*> m_nQueueBatch;
	std::queue<cwLoadResult*> m_nQueueResult;

	std::mutex m_nMutex;
	std::mutex m_nMutexResult;
	std::condition_variable m_nCondNotEmpty;
	int m_iThreadState;

};

NS_MINIR_END

#endif

