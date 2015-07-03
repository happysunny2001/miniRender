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

#include "cwRepertory.h"
#include "Device/cwDevice.h"
#include "Shader/cwShaderManager.h"
#include "Layouts/cwLayoutsManager.h"
#include "Texture/cwTextureManager.h"
//#include "cwLog.h"
#include "Ref/cwAutoReleasePool.h"
#include "Camera/cwCamera.h"
#include "Platform/cwFileSystem.h"
#include "Engine/cwEngine.h"
#include "Generator/cwGeometryGenerator.h"
#include "Event/cwEventManager.h"
#include "Scheduler/cwSchedulerManager.h"

#ifdef _CW_D3D11_
#include "Platform/D3D/D3D11/Repertory/cwD3D11Repertory.h"
#endif

NS_MINIR_BEGIN

cwRepertory& cwRepertory::getInstance()
{
#ifdef _CW_D3D11_
	static cwD3D11Repertory rep;
#endif
	return rep;
}

cwRepertory::cwRepertory():
m_pDevice(nullptr),
//m_pLog(nullptr),
m_pShaderManager(nullptr),
m_pLayoutManager(nullptr),
m_pTextureManager(nullptr),
m_pFileSystem(nullptr),
m_pEngine(nullptr),
m_pGeoGenerator(nullptr),
m_pEventManager(nullptr),
m_pSchedulerManager(nullptr)
{
	m_pAutoReleasePool = new cwAutoReleasePool();
}

cwRepertory::~cwRepertory()
{
	CW_SAFE_RELEASE_NULL(m_pShaderManager);
	CW_SAFE_RELEASE_NULL(m_pLayoutManager);
	CW_SAFE_DELETE(m_pDevice);
	//CW_SAFE_DELETE(m_pLog);
	CW_SAFE_DELETE(m_pAutoReleasePool);
	CW_SAFE_RELEASE_NULL(m_pTextureManager);
	CW_SAFE_RELEASE_NULL(m_pFileSystem);
	CW_SAFE_RELEASE_NULL(m_pEngine);
	CW_SAFE_RELEASE_NULL(m_pGeoGenerator);
	CW_SAFE_RELEASE_NULL(m_pEventManager);
	CW_SAFE_RELEASE_NULL(m_pSchedulerManager);
}

cwDevice* cwRepertory::getDevice()
{
	return m_pDevice;
}

//cwLog* cwRepertory::getLog()
//{
//	return m_pLog;
//}

cwShaderManager* cwRepertory::getShaderManager()
{
	return m_pShaderManager;
}

cwLayoutsManager* cwRepertory::getLayoutManager()
{
	return m_pLayoutManager;
}

cwAutoReleasePool* cwRepertory::getAutoReleasePool()
{
	return m_pAutoReleasePool;
}

cwTextureManager* cwRepertory::getTextureManager()
{
	return m_pTextureManager;
}

cwFileSystem* cwRepertory::getFileSystem()
{
	return m_pFileSystem;
}

cwEngine* cwRepertory::getEngine()
{
	return m_pEngine;
}

cwGeometryGenerator* cwRepertory::getGeoGenerator()
{
	return m_pGeoGenerator;
}

cwEventManager* cwRepertory::getEventManager()
{
	return m_pEventManager;
}

cwSchedulerManager* cwRepertory::getSchedulerManager()
{
	return m_pSchedulerManager;
}

void cwRepertory::initAll()
{
//	m_pLog = new cwLog();

	addValue(gValueNearZ, cwValueMap(1.0f));
	addValue(gValueFarZ, cwValueMap(1000.0f));
	addValue(gValueFov, cwValueMap(0.25f*3.14159f));

	m_pEventManager = cwEventManager::create();
	CW_SAFE_RETAIN(m_pEventManager);

	m_pFileSystem = cwFileSystem::create();
	CW_SAFE_RETAIN(m_pFileSystem);

	m_pGeoGenerator = cwGeometryGenerator::create();
	CW_SAFE_RETAIN(m_pGeoGenerator);

	m_pSchedulerManager = cwSchedulerManager::create();
	CW_SAFE_RETAIN(m_pSchedulerManager);

	specialInit();

	m_pTextureManager = cwTextureManager::create();
	CW_SAFE_RETAIN(m_pTextureManager);

	m_pEngine = cwEngine::create();
	CW_SAFE_RETAIN(m_pEngine);
}

bool cwRepertory::specialInit()
{
	return true;
}

void cwRepertory::addValue(const string& strName, const cwValueMap& value)
{
	m_mapData[strName] = value;
}

CWFLOAT cwRepertory::getFloat(const string& strName)
{
	if (m_mapData.find(strName) == m_mapData.end()) return FLT_MAX;
	const cwValueMap& value = m_mapData[strName];
	if (value.type != eValueTypeFloat) return FLT_MAX;
	return value.fData;
}

CWUINT cwRepertory::getUInt(const string& strName)
{
	if (m_mapData.find(strName) == m_mapData.end()) return CW_UINT_MAX;
	const cwValueMap& value = m_mapData[strName];
	if (value.type != eValueTypeUint) return CW_UINT_MAX;
	return value.iData;
}

CWVOID* cwRepertory::getPtr(const string& strName)
{
	if (m_mapData.find(strName) == m_mapData.end()) return nullptr;
	const cwValueMap& value = m_mapData[strName];
	if (value.type != eValueTypePtr) return nullptr;
	return value.pData;
}

NS_MINIR_END
