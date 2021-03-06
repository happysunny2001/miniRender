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

#ifndef __CW_REPERTORY_H__
#define __CW_REPERTORY_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Base/cwValueMap.h"
#include <unordered_map>
using namespace std;

NS_MINIR_BEGIN

class cwDevice;
class cwShaderManager;
class cwLayoutsManager;
class cwAutoReleasePool;
class cwTextureManager;
class cwFileSystem;
class cwCamera;
class cwEngine;
class cwGeometryGenerator;
class cwEventManager;
class cwSchedulerManager;
//class cwParserManager;
class cwResourceLoader;
class cwSpatialFactory;
class cwEffectManager;
class cwObjectMonitor;
class cwRendererFactory;

class CW_DLL cwRepertory
{
public:
	static cwRepertory& getInstance();

	virtual CWVOID initAll();
	virtual CWVOID releaseAll();
	virtual CWVOID addValue(const string& strName, const cwValueMap& value);
	CWFLOAT getFloat(const string& strName);
	CWUINT  getUInt(const string& strName);
	CWVOID* getPtr(const string& strName);

	inline cwDevice* getDevice() { return m_pDevice; }
	inline cwShaderManager* getShaderManager() { return m_pShaderManager; }
	inline cwLayoutsManager* getLayoutManager() { return m_pLayoutManager; }
	inline cwAutoReleasePool* getAutoReleasePool() { return m_pAutoReleasePool; }
	inline cwTextureManager* getTextureManager() { return m_pTextureManager; }
	inline cwFileSystem* getFileSystem() { return m_pFileSystem; }
	inline cwEngine* getEngine() { return m_pEngine; }
	inline cwGeometryGenerator* getGeoGenerator() { return m_pGeoGenerator; }
	inline cwEventManager* getEventManager() { return m_pEventManager; }
	inline cwSchedulerManager* getSchedulerManager() { return m_pSchedulerManager; }
	//inline cwParserManager* getParserManager() { return m_pParserManager; }
	inline cwResourceLoader* getResourceLoader() { return m_pResourceLoader; }
	inline cwSpatialFactory* getSpatialFactory() { return m_pSpatialFactory; }
	inline cwEffectManager* getEffectManager() { return m_pEffectManager; }
	inline cwObjectMonitor* getObjectMonitor() { return m_pObjectMonitor; }
	inline cwRendererFactory* getRendererFactory() { return m_pRendererFactory; }

	virtual CWVOID refreshWindowTitle(const CWSTRING& strTitle) {}

protected:
	cwRepertory();
	cwRepertory(cwRepertory const&) {}
	cwRepertory& operator=(cwRepertory const&) {}
	virtual ~cwRepertory();

	virtual CWBOOL specialInit();
	
protected:
	cwDevice* m_pDevice;
	cwShaderManager* m_pShaderManager;
	cwLayoutsManager* m_pLayoutManager;
	cwAutoReleasePool* m_pAutoReleasePool;
	cwTextureManager* m_pTextureManager;
	cwFileSystem* m_pFileSystem;
	cwEngine* m_pEngine;
	cwGeometryGenerator* m_pGeoGenerator;
	cwEventManager* m_pEventManager;
	cwSchedulerManager* m_pSchedulerManager;
	//cwParserManager* m_pParserManager;
	cwResourceLoader* m_pResourceLoader;
	cwSpatialFactory* m_pSpatialFactory;
	cwEffectManager* m_pEffectManager;
	cwObjectMonitor* m_pObjectMonitor;
	cwRendererFactory* m_pRendererFactory;

	unordered_map<string, cwValueMap> m_mapData;

};

NS_MINIR_END

#endif
