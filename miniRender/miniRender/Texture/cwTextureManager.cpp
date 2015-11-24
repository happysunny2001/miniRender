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
	CWSTRING strFullPath = cwRepertory::getInstance().getFileSystem()->getFullFilePath(strName);
	cwTexture* pTex = cwRepertory::getInstance().getDevice()->createTexture(strFullPath);
	if (pTex) {
		pTex->setName(strName);
		return pTex;
	}

	return nullptr;
}

cwTexture* cwTextureManager::createTextureThreadSafe(const CWSTRING& strName)
{
	CWSTRING strFullPath = cwRepertory::getInstance().getFileSystem()->getFullFilePath(strName);
	cwTexture* pTex = cwRepertory::getInstance().getDevice()->createTextureThreadSafe(strFullPath);
	if (pTex) {
		pTex->setName(strName);
		return pTex;
	}

	return nullptr;
}

cwTexture* cwTextureManager::createCubeTexture(const CWSTRING& strName)
{
	CWSTRING strFullPath = cwRepertory::getInstance().getFileSystem()->getFullFilePath(strName);
	cwTexture* pTex = cwRepertory::getInstance().getDevice()->createCubeTexture(strFullPath);
	if (pTex) {
		pTex->setName(strName);
		return pTex;
	}

	return nullptr;
}

cwTexture* cwTextureManager::createCubeTextureThreadSafe(const CWSTRING& strName)
{
	CWSTRING strFullPath = cwRepertory::getInstance().getFileSystem()->getFullFilePath(strName);
	cwTexture* pTex = cwRepertory::getInstance().getDevice()->createCubeTextureThreadSafe(strFullPath);
	if (pTex) {
		pTex->setName(strName);
		return pTex;
	}

	return nullptr;
}

cwTexture* cwTextureManager::createCubeTexture(CWUINT iSize)
{
	return cwRepertory::getInstance().getDevice()->createCubeTexture(iSize);
}

cwTexture* cwTextureManager::createTextureArray(const std::vector<CWSTRING>& vecFiles)
{
	if (vecFiles.empty()) return nullptr;

	std::vector<CWSTRING> vecFullPath(vecFiles.size());
	auto pFileSystem = cwRepertory::getInstance().getFileSystem();

	for (CWUINT i = 0; i < vecFiles.size(); ++i) {
		vecFullPath[i] = pFileSystem->getFullFilePath(vecFiles[i]);
	}

	cwTexture* pTex = cwRepertory::getInstance().getDevice()->createTextureArray(vecFullPath);
	if (pTex) {
		pTex->setName(vecFiles[0]);
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
	auto itFind = m_mapTexture.find(strName);
	if (itFind != m_mapTexture.end()) return itFind->second;

	cwTexture* pTex = createTexture(strName);
	if (pTex) {
		m_mapTexture.insert(strName, pTex);
		return pTex;
	}

	return nullptr;
}

cwTexture* cwTextureManager::getCubeTexture(const CWSTRING& strName)
{
	auto itFind = m_mapTexture.find(strName);
	if (itFind != m_mapTexture.end()) return itFind->second;

	cwTexture* pTex = createCubeTexture(strName);
	if (pTex) {
		m_mapTexture.insert(strName, pTex);
		return pTex;
	}

	return nullptr;
}

cwTexture* cwTextureManager::getTextureArray(const std::vector<CWSTRING>& vecFiles)
{
	if (vecFiles.empty()) return nullptr;

	auto itFind = m_mapTexture.find(vecFiles[0]);
	if (itFind != m_mapTexture.end()) return itFind->second;

	cwTexture* pTex = createTextureArray(vecFiles);
	if (pTex) {
		m_mapTexture.insert(vecFiles[0], pTex);
		return pTex;
	}

	return nullptr;
}

CWBOOL cwTextureManager::isExist(const CWSTRING& strName)
{
	if (m_mapTexture.find(strName) == m_mapTexture.end()) return CWFALSE;
	return CWTRUE;
}

CWVOID cwTextureManager::appendTexture(cwTexture* pTex)
{
	if (pTex) {
		m_mapTexture.insert(pTex->getName(), pTex);
	}
}

void cwTextureManager::removeTexture(const string& strName)
{
	m_mapTexture.erase(strName);
}

CWVOID cwTextureManager::removeTexture(cwTexture* pTex)
{
	if (pTex) {
		m_mapTexture.erase(pTex->getName());
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
