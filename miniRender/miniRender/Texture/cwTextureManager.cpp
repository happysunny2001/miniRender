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

#include "cwTextureManager.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/cwFileSystem.h"
#include "Resource/cwResourceLoader.h"

NS_MINIR_BEGIN

cwTextureManager* cwTextureManager::create()
{
	cwTextureManager* pManager = new cwTextureManager();
	if (pManager) {
		pManager->autorelease();
		return pManager;
	}

	return nullptr;
}

cwTextureManager::~cwTextureManager()
{
	m_vecRenderTexture.clear();
}

cwTexture* cwTextureManager::createTexture(const CWSTRING& strName)
{
	cwTexture* pTex = isExist(strName);
	if (pTex) {
		return pTex;
	}

	cwRepertory& repertory = cwRepertory::getInstance();
	cwData* pData = repertory.getResourceLoader()->getTextureData(strName);
	if (pData) {
		pTex = repertory.getDevice()->createTexture(pData->m_pData, pData->m_uSize);
		if (pTex) {
			pTex->setName(strName);
			appendTexture(pTex);
		}
		delete pData;
	}

	return pTex;
}

cwTexture* cwTextureManager::createTextureThreadSafe(const CWSTRING& strName)
{
	cwTexture* pTex = isExist(strName);
	if (pTex) {
		return pTex;
	}

	cwRepertory& repertory = cwRepertory::getInstance();
	cwData* pData = repertory.getResourceLoader()->getTextureData(strName);
	if (pData) {
		pTex = repertory.getDevice()->createTextureThreadSafe(pData->m_pData, pData->m_uSize); // ref count:1
		if (pTex) {
			pTex->setName(strName);
			appendTexture(pTex); // ref count: 2
			CW_SAFE_RELEASE(pTex);  //ref count: 1
		}
		delete pData;
	}

	return pTex;
}

cwTexture* cwTextureManager::createTexture(const CWSTRING& strName, CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format)
{
	cwTexture* pTex = isExist(strName);
	if (pTex) {
		return pTex;
	}

	cwRepertory& repertory = cwRepertory::getInstance();
	pTex = repertory.getDevice()->createTexture(pData, iWidth, iHeight, iElementSize, format);
	if (pTex) {
		pTex->setName(strName);
		appendTexture(pTex);
	}

	return pTex;
}

cwTexture* cwTextureManager::createTextureThreadSafe(const CWSTRING& strName, CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format)
{
	cwTexture* pTex = isExist(strName);
	if (pTex) {
		return pTex;
	}

	cwRepertory& repertory = cwRepertory::getInstance();
	pTex = repertory.getDevice()->createTextureThreadSafe(pData, iWidth, iHeight, iElementSize, format); //ref count: 1
	if (pTex) {
		pTex->setName(strName);
		appendTexture(pTex); // ref count:2
		CW_SAFE_RELEASE(pTex); //ref count: 1
	}

	return pTex;
}

cwTexture* cwTextureManager::createCubeTexture(const CWSTRING& strName)
{
	cwTexture* pTex = isExist(strName);
	if (pTex) {
		return pTex;
	}

	cwRepertory& repertory = cwRepertory::getInstance();
	cwData* pData = repertory.getResourceLoader()->getTextureData(strName);
	if (pData) {
		pTex = repertory.getDevice()->createCubeTexture(pData->m_pData, pData->m_uSize);
		if (pTex) {
			pTex->setName(strName);
			appendTexture(pTex);
		}

		delete pData;
	}

	return pTex;
}

cwTexture* cwTextureManager::createCubeTextureThreadSafe(const CWSTRING& strName)
{
	cwTexture* pTex = isExist(strName);
	if (pTex) {
		return pTex;
	}

	cwRepertory& repertory = cwRepertory::getInstance();
	cwData* pData = repertory.getResourceLoader()->getTextureData(strName);
	if (pData) {
		pTex = repertory.getDevice()->createCubeTextureThreadSafe(pData->m_pData, pData->m_uSize); //ref count: 1
		if (pTex) {
			pTex->setName(strName);
			appendTexture(pTex); // ref count: 2
			CW_SAFE_RELEASE(pTex); // ref count: 1
		}

		delete pData;
	}

	return pTex;
}

cwTexture* cwTextureManager::createCubeTexture(CWUINT iSize)
{
	return cwRepertory::getInstance().getDevice()->createCubeTexture(iSize);
}

cwTexture* cwTextureManager::createTextureArray(const std::vector<CWSTRING>& vecFiles)
{
	if (vecFiles.empty()) return nullptr;

	cwTexture* pTex = isExist(vecFiles[0]);
	if (pTex) {
		return pTex;
	}

	pTex = cwRepertory::getInstance().getDevice()->createTextureArray(vecFiles);
	if (pTex) {
		pTex->setName(vecFiles[0]);
		appendTexture(pTex);
		return pTex;
	}

	return nullptr;
}

cwRenderTexture* cwTextureManager::createRenderTexture(float fWidth, float fHeight, eRenderTextureType eType)
{
	cwRenderTexture* pRenderTex = cwRepertory::getInstance().getDevice()->createRenderTexture(fWidth, fHeight, eType);
	if (pRenderTex) {
		m_vecRenderTexture.pushBack(pRenderTex);
	}

	return pRenderTex;
}

cwTexture* cwTextureManager::getTexture(const string& strName)
{
	cwTexture* pTex = isExist(strName);
	if (pTex) {
		return pTex;
	}

	return createTexture(strName);
}

cwTexture* cwTextureManager::getCubeTexture(const CWSTRING& strName)
{
	cwTexture* pTex = isExist(strName);
	if (pTex) {
		return pTex;
	}

	return createCubeTexture(strName);
}

cwTexture* cwTextureManager::getTextureArray(const std::vector<CWSTRING>& vecFiles)
{
	if (vecFiles.empty()) return nullptr;

	cwTexture* pTex = isExist(vecFiles[0]);
	if (pTex) {
		return pTex;
	}

	return createTextureArray(vecFiles);
}

cwTexture* cwTextureManager::isExist(const CWSTRING& strName)
{
	std::unique_lock<std::mutex> lock(m_nMutex);

	auto itFind = m_mapTexture.find(strName);
	if (itFind != m_mapTexture.end()) return itFind->second;

	return nullptr;
}

CWVOID cwTextureManager::appendTexture(cwTexture* pTex)
{
	if (pTex) {
		std::unique_lock<std::mutex> lock(m_nMutex);
		m_mapTexture.insert(pTex->getName(), pTex);
	}
}

void cwTextureManager::removeTexture(const string& strName)
{
	std::unique_lock<std::mutex> lock(m_nMutex);

	auto it = m_mapTexture.find(strName);
	if (it == m_mapTexture.end()) return;

	if (1 == it->second->retainCount())
		m_mapTexture.erase(it);
}

CWVOID cwTextureManager::removeTexture(cwTexture* pTex)
{
	if (pTex) {
		removeTexture(pTex->getName());
	}
}

void cwTextureManager::removeRenderTexture(cwRenderTexture* pTex)
{
	if (pTex) {
		m_vecRenderTexture.erase(pTex);
	}
}

void cwTextureManager::beginResize()
{
	for (auto pRenderTex : m_vecRenderTexture) {
		pRenderTex->beginResize();
	}
}

void cwTextureManager::onResize()
{
	for (auto pRenderTex : m_vecRenderTexture) {
		pRenderTex->onResize();
	}
}

NS_MINIR_END
