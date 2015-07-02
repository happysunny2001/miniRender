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

#include "cwShaderManager.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/cwFileSystem.h"

NS_MINIR_BEGIN

//cwShaderManager* cwShaderManager::create()
//{
//	cwShaderManager* pManager = new cwShaderManager();
//	if (pManager && pManager->init()) {
//		pManager->autorelease();
//		return pManager;
//	}
//
//	CW_SAFE_DELETE(pManager);
//	return nullptr;
//}

cwShaderManager::cwShaderManager()
{

}

cwShaderManager::~cwShaderManager()
{

}

cwShader* cwShaderManager::loadShader(const CWSTRING& strFile)
{
	auto itFind = m_nMapShader.find(strFile);
	if (itFind != m_nMapShader.end()) return itFind->second;

	cwShader* pShader = cwRepertory::getInstance().getDevice()->createShader(strFile);
	if (pShader) {
		m_nMapShader.insert(strFile, pShader);
		return pShader;
	}

	return nullptr;
}

cwShader* cwShaderManager::getShader(const CWSTRING& strFile)
{
	auto itFind = m_nMapShader.find(strFile);
	if (itFind != m_nMapShader.end()) return itFind->second;
	return nullptr;
}

cwShader* cwShaderManager::getDefShader(CWUINT iKey)
{
	auto itFind = m_nMapDefShader.find(iKey);
	if (itFind != m_nMapDefShader.end()) return itFind->second;
	return nullptr;
}

bool cwShaderManager::init()
{
	return true;
}

NS_MINIR_END
