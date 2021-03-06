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

#include "cwResourceLoader.h"
#include "cwLoadBatch.h"
#include "cwLoadResult.h"
#include "cwRemoveBatch.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShaderManager.h"
#include "Texture/cwTextureManager.h"
//#include "Parser/cwParserManager.h"
#include "Parser/cwResourceConfParser.h"
#include "Platform/cwFileSystem.h"
#include "cwStreaming.h"
#include "Base/cwLog.h"

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

cwResourceLoader::cwResourceLoader() : 
m_iThreadState(0)
{

}

cwResourceLoader::~cwResourceLoader()
{

}

CWBOOL cwResourceLoader::init()
{
	loadConfigure();

	std::thread t(loadingProcessThread, this);
	t.detach();

	return CWTRUE;
}

CWVOID cwResourceLoader::loadConfigure()
{
	cwResourceConfParser* pParser = cwResourceConfParser::create();
	CWSTRING strFilePath = cwRepertory::getInstance().getFileSystem()->getFullFilePath("Configure/resource.xml");
	m_nMapResLocation = pParser->parse(strFilePath);
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
	if (pBatch) {
		std::unique_lock<std::mutex> lock(m_nMutex);
		m_nQueueBatch.push(pBatch);
		CW_SAFE_RETAIN(pBatch);
		m_nCondNotEmpty.notify_one();
	}
}

CWVOID cwResourceLoader::loadAsync(cwStreaming* pObjStreaming)
{
	if (pObjStreaming) {
		pObjStreaming->streamPrepare();
		
		cwLoadBatch* pBatch = pObjStreaming->getLoadBatch();
		if (pBatch) {
			loadAsync(pBatch);
		}

		cwRef* pRef = dynamic_cast<cwRef*>(pObjStreaming);
		CW_SAFE_RETAIN(pRef);
	}
}

CWBOOL cwResourceLoader::loadSync(cwLoadBatch* pBatch)
{
	if (pBatch) {
		cwLoadResult* pResult = load(pBatch);
		if (!pResult) return CWFALSE;

		pResult->distribute();
		pResult->release();
	}

	return true;
}

cwLoadResult* cwResourceLoader::load(cwLoadBatch* pBatch)
{
	cwLoadResult* pResult = cwLoadResult::create();
	pResult->setLoadBatch(pBatch);

	if (!pBatch->isStreamCancelled())
		pResult->load();

	return pResult;
}

CWVOID cwResourceLoader::remove(cwRemoveBatch* pBatch)
{
	if (pBatch) {
		for (auto it = pBatch->begin(); it != pBatch->end(); ++it) {
			remove(*it);
		}
	}
}

CWVOID cwResourceLoader::remove(cwResourceInfo& resInfo)
{
	switch (resInfo.m_eType) {
	case eResourceTypeShader:
		cwRepertory::getInstance().getShaderManager()->removeShader(resInfo.m_nStrName);
		break;
	case eResourceTypeTexture2D:
	case eResourceTypeTextureCube:
		cwRepertory::getInstance().getTextureManager()->removeTexture(resInfo.m_nStrName);
		break;
	}
}

CWBOOL cwResourceLoader::update(float dt)
{
	if (g_aResult == 0)
		return CWFALSE;

	cwLoadResult* pResult = nullptr;

	std::unique_lock<std::mutex> lockResult(m_nMutexResult);
	if (!m_nQueueResult.empty()){
		pResult = m_nQueueResult.front();
		m_nQueueResult.pop();
	}

	g_aResult = (unsigned int)(m_nQueueResult.size());

	lockResult.unlock();

	if (!pResult) return CWFALSE;

	cwStreaming*p = pResult->getLoadBatch()->m_pObjStreaming;
	cwRef* pRef = dynamic_cast<cwRef*>(p);

	if (pResult->getLoadBatch()) {
		if (pResult->getLoadBatch()->isStreamCancelled()) {
			cwLog::print("load result cancel!\n");
			pResult->getLoadBatch()->streamRelease();
			pResult->release();
			CW_SAFE_RELEASE_NULL(pRef);

			return CWTRUE;
		}
	}

	pResult->distribute();
	pResult->release();
	CW_SAFE_RELEASE_NULL(pRef);

	return CWTRUE;
}

CWSTRING cwResourceLoader::getFullPath(const CWSTRING& strFileName)
{
	cwFileSystem* fs = cwRepertory::getInstance().getFileSystem();
	const CWSTRING& strWorkingPath = fs->getWokringPath();

	for (auto it = m_nMapResLocation.begin(); it != m_nMapResLocation.end(); ++it) {
		for (auto itPath = it->second.begin(); itPath != it->second.end(); ++itPath) {
			CWSTRING strFullPath = strWorkingPath + "/" + *itPath + strFileName;
			if (fs->isFileExist(strFullPath)) {
				return strFullPath;
			}
		}
	}

	return "";
}

CWSTRING cwResourceLoader::getTextureFullPath(const CWSTRING& strFileName)
{
	cwFileSystem* fs = cwRepertory::getInstance().getFileSystem();
	const CWSTRING& strWorkingPath = fs->getWokringPath();
	std::vector<CWSTRING>& vecTexturePath = m_nMapResLocation["Texture"];

	for (auto it = vecTexturePath.begin(); it != vecTexturePath.end(); ++it) {
		CWSTRING strFullPath = strWorkingPath + "/" + *it + strFileName;
		if (fs->isFileExist(strFullPath)) {
			return strFullPath;
		}
	}

	return "";
}

CWSTRING cwResourceLoader::getShaderFullPath(const CWSTRING& strFileName)
{
	cwFileSystem* fs = cwRepertory::getInstance().getFileSystem();
	const CWSTRING& strWorkingPath = fs->getWokringPath();
	std::vector<CWSTRING>& vecShaderPath = m_nMapResLocation["Shader"];

	for (auto it = vecShaderPath.begin(); it != vecShaderPath.end(); ++it) {
		CWSTRING strFullPath = strWorkingPath + "/" + *it + strFileName;
		if (fs->isFileExist(strFullPath)) {
			return strFullPath;
		}
	}

	return "";
}

cwData* cwResourceLoader::getShaderData(const CWSTRING& strFileName)
{
	if (strFileName.empty()) return nullptr;
	CWSTRING strFullPath = getShaderFullPath(strFileName);
	if (strFileName.empty()) return nullptr;

	return cwRepertory::getInstance().getFileSystem()->getFileData(strFullPath);
}

cwData* cwResourceLoader::getTextureData(const CWSTRING& strFileName)
{
	if (strFileName.empty()) return nullptr;
	CWSTRING strFullPath = getTextureFullPath(strFileName);
	if (strFileName.empty()) return nullptr;

	return cwRepertory::getInstance().getFileSystem()->getFileData(strFullPath);
}

cwData* cwResourceLoader::getFileData(const CWSTRING& strFileName)
{
	if (strFileName.empty()) return nullptr;
	CWSTRING strFullPath = getFullPath(strFileName);
	if (strFileName.empty()) return nullptr;

	return cwRepertory::getInstance().getFileSystem()->getFileData(strFullPath);
}

CWVOID cwResourceLoader::exit()
{
	this->m_iThreadState = 1;
	this->m_nCondNotEmpty.notify_one();

	while (true) {
		if (this->m_iThreadState == 2)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	while (!m_nQueueBatch.empty()) {
		cwLoadBatch* pFirst = m_nQueueBatch.front();
		m_nQueueBatch.pop();
		pFirst->loadFailed();
		CW_SAFE_RELEASE(pFirst);
	}

	while (!m_nQueueResult.empty()) {
		cwLoadResult* pResult = m_nQueueResult.front();
		m_nQueueResult.pop();

		if (pResult) {
			pResult->distribute();
			pResult->release();
		}
	}
}

CWVOID loadingProcessThread(cwResourceLoader* pLoader)
{
	while (true) {
		std::unique_lock<std::mutex> lock(pLoader->m_nMutex);
		while (pLoader->batchEmpty() && pLoader->getThreadState() == 0) {
			pLoader->m_nCondNotEmpty.wait(lock);
		}

		if (pLoader->getThreadState() != 0) {
			lock.unlock();
			pLoader->setThreadState(2);
			break;
		}

		cwLoadBatch* pCurrBatch = pLoader->firstBatch();
		CW_SAFE_RETAIN(pCurrBatch);
		pLoader->popBatch();

		lock.unlock();

		//if (pCurrBatch->isStreamCancelled()) {
		//	cwLog::print("batch cancel!\n");
		//	if (pCurrBatch->m_pObjStreaming)
		//		pCurrBatch->m_pObjStreaming->streamClean();
		//	pCurrBatch->releaseSteamObject();
		//	CW_SAFE_RELEASE_NULL(pCurrBatch);
		//	continue;
		//}

		if (pCurrBatch->isStreamCancelled()) {
			cwLog::print("batch cancel!\n");
		}

		cwLoadResult* pResult = pLoader->load(pCurrBatch);
		CW_SAFE_RELEASE_NULL(pCurrBatch);
		if (pResult) {
			std::unique_lock<std::mutex> lockResult(pLoader->m_nMutexResult);

			pLoader->m_nQueueResult.push(pResult);
			g_aResult = (unsigned int)(pLoader->m_nQueueResult.size());

			lockResult.unlock();
		}
	}

	cwLog::print("resource loader thread end.\n");
}

NS_MINIR_END
