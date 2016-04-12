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
#include "Ref/cwAutoReleasePool.h"
#include "Camera/cwCamera.h"
#include "Platform/cwFileSystem.h"
#include "Engine/cwEngine.h"
#include "Generator/cwGeometryGenerator.h"
#include "Event/cwEventManager.h"
#include "Scheduler/cwSchedulerManager.h"
#include "Parser/cwParserManager.h"
#include "Math/cwMath.h"
#include "Resource/cwResourceLoader.h"
#include "SpatialSorting/cwSpatialFactory.h"
#include "effect/cwEffectManager.h"

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
m_pShaderManager(nullptr),
m_pLayoutManager(nullptr),
m_pTextureManager(nullptr),
m_pFileSystem(nullptr),
m_pEngine(nullptr),
m_pGeoGenerator(nullptr),
m_pEventManager(nullptr),
m_pSchedulerManager(nullptr),
m_pParserManager(nullptr),
m_pResourceLoader(nullptr),
m_pSpatialFactory(nullptr),
m_pEffectManager(nullptr)
{
	m_pAutoReleasePool = new cwAutoReleasePool();
}

cwRepertory::~cwRepertory()
{
	CW_SAFE_RELEASE_NULL(m_pShaderManager);
	CW_SAFE_RELEASE_NULL(m_pLayoutManager);
	CW_SAFE_DELETE(m_pAutoReleasePool);
	CW_SAFE_RELEASE_NULL(m_pTextureManager);
	CW_SAFE_RELEASE_NULL(m_pFileSystem);
	CW_SAFE_RELEASE_NULL(m_pEngine);
	CW_SAFE_RELEASE_NULL(m_pGeoGenerator);
	CW_SAFE_RELEASE_NULL(m_pEventManager);
	CW_SAFE_RELEASE_NULL(m_pSchedulerManager);
	CW_SAFE_RELEASE_NULL(m_pParserManager);
	CW_SAFE_RELEASE_NULL(m_pEffectManager);
	CW_SAFE_DELETE(m_pSpatialFactory);
	CW_SAFE_DELETE(m_pDevice);
}

void cwRepertory::initAll()
{
//	m_pLog = new cwLog();

	this->addValue(gValueWinWidth, cwValueMap(CWUINT(800)));
	this->addValue(gValueWinHeight, cwValueMap(CWUINT(600)));

	addValue(gValueNearZ, cwValueMap(1.0f));
	addValue(gValueFarZ, cwValueMap(1000.0f));
	addValue(gValueFov, cwValueMap(0.25f*cwMathUtil::cwPI));

	m_pSpatialFactory = new cwSpatialFactory();

	m_pParserManager = cwParserManager::create();
	CW_SAFE_RETAIN(m_pParserManager);

	m_pFileSystem = cwFileSystem::create();
	CW_SAFE_RETAIN(m_pFileSystem);

	m_pResourceLoader = cwResourceLoader::create();
	CW_SAFE_RETAIN(m_pResourceLoader);

	m_pEventManager = cwEventManager::create();
	CW_SAFE_RETAIN(m_pEventManager);

	m_pGeoGenerator = cwGeometryGenerator::create();
	CW_SAFE_RETAIN(m_pGeoGenerator);

	m_pSchedulerManager = cwSchedulerManager::create();
	CW_SAFE_RETAIN(m_pSchedulerManager);

	m_pTextureManager = cwTextureManager::create();
	CW_SAFE_RETAIN(m_pTextureManager);

	specialInit();

	m_pEffectManager = cwEffectManager::create();
	CW_SAFE_RETAIN(m_pEffectManager);

	m_pEngine = cwEngine::create();
	CW_SAFE_RETAIN(m_pEngine);
	
//	m_pEngine->deferParseRenderer();
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
