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

#ifndef __CW_LOAD_BATCH_H__
#define __CW_LOAD_BATCH_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Ref/cwRef.h"

#include <functional>
#include <vector>

NS_MINIR_BEGIN

typedef enum {
	eResourceTypeNone = 0,
	eResourceTypeTexture2D = 1,
	eResourceTypeTextureCube,
	eResourceTypeShader,
}eResourceType;

class cwResourceInfo
{
public:
	eResourceType m_eType;
	CWSTRING m_nStrName;

};

class cwLoadBatch : public cwRef
{
public:
	typedef std::function<CWVOID(cwLoadBatch*)> LoadCallbackFunc;

public:
	static cwLoadBatch* create();

	cwLoadBatch();
	virtual ~cwLoadBatch();

	CWVOID addResource(cwResourceInfo& resInfo);
	CWVOID addTexture2D(const CWSTRING& strName);
	CWVOID addTextureCube(const CWSTRING& strName);
	CWVOID addShader(const CWSTRING& strName);

	std::vector<cwResourceInfo>::iterator begin() { return m_nVecResource.begin(); }
	std::vector<cwResourceInfo>::iterator end() { return m_nVecResource.end(); }

	CWVOID onOverCallback();

protected:
	CWBOOL checkResourceExist(cwResourceInfo& resInfo);

protected:
	std::vector<cwResourceInfo> m_nVecResource;

public:
	LoadCallbackFunc onLoadOver;

};

NS_MINIR_END

#endif
