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

#ifndef __CW_REPERTORY_H__
#define __CW_REPERTORY_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Base/cwValueMap.h"
#include <unordered_map>
using namespace std;

NS_MINI_BEGIN

class cwDevice;
class cwLog;
class cwEffectManager;
class cwLayoutsManager;
class cwAutoReleasePool;
class cwCamera;

class CW_DLL cwRepertory
{
public:
	static cwRepertory& getInstance();

	virtual void initAll();
	virtual void addValue(const string& strName, const cwValueMap& value);
	CWFLOAT getFloat(const string& strName);
	CWUINT  getUInt(const string& strName);
	CWVOID* getPtr(const string& strName);

	virtual cwDevice* getDevice();
	virtual cwLog* getLog();
	virtual cwEffectManager* getEffectManager();
	virtual cwLayoutsManager* getLayoutManager();
	virtual cwAutoReleasePool* getAutoReleasePool();

	virtual void refreshWindowTitle(const CWWCHAR* pcTitle) {}

	cwCamera* getCurrentCamera() { return m_pCurrentCamera; }
	void setCurrentCamera(cwCamera* pCam);

protected:
	cwRepertory();
	cwRepertory(cwRepertory const&) {}
	cwRepertory& operator=(cwRepertory const&) {}
	virtual ~cwRepertory();
	
protected:
	cwDevice* m_pDevice;
	cwEffectManager* m_pEffectManager;
	cwLayoutsManager* m_pLayoutManager;
	cwAutoReleasePool* m_pAutoReleasePool;
	cwLog* m_pLog;
	cwCamera* m_pCurrentCamera;

	unordered_map<string, cwValueMap> m_mapData;

};

NS_MINI_END

#endif