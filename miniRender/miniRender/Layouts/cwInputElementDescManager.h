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

#ifndef _cwInputElementDescManager_h_
#define _cwInputElementDescManager_h_

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "cwRef.h"
#include "Base/cwMap.h"

NS_MINI_BEGIN

class CW_DLL cwInputElementDesc : public cwRef
{
public:
	static cwInputElementDesc* create(int iElementCnt);

	cwInputElementDesc();
	virtual ~cwInputElementDesc();

	virtual bool init(int iElementCnt);

	void addElementDesc(const CWCHAR* name, DXGI_FORMAT format, CWUINT offset, int index);

	D3D11_INPUT_ELEMENT_DESC* getElementDesc() { return m_pElementList; }
	CWUINT getDescCnt() const { return m_iElementCnt; }

public:
	D3D11_INPUT_ELEMENT_DESC* m_pElementList;
	CWUINT m_iElementCnt;

};

class cwInputElementDescManager
{
public:
	static cwInputElementDescManager& getInstance() {
		static cwInputElementDescManager manager;
		return manager;
	}

	cwInputElementDesc* getElement(ceElementDesc e);

private:
	cwInputElementDescManager();
	cwInputElementDescManager(cwInputElementDescManager const&) {}
	cwInputElementDescManager& operator=(cwInputElementDescManager const&) {}
	~cwInputElementDescManager();

	void buildDescMap();

private:
	cwMap<ceElementDesc, cwInputElementDesc*> m_mapDesc;

};

NS_MINI_END

#endif