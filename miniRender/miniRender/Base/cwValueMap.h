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

#ifndef _CW_VALUEMAP_H_
#define _CW_VALUEMAP_H_

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include <string>
using namespace std;

NS_MINIR_BEGIN

const CWSTRING gValueWinHandle = "winHandle";
const CWSTRING gValueWinWidth  = "winWidth";
const CWSTRING gValueWinHeight = "winHeight";
const CWSTRING gValueNearZ = "nearZ";
const CWSTRING gValueFarZ  = "farZ";
const CWSTRING gValueFov   = "fov";

struct CW_DLL cwValueMap
{
	union {
		CWFLOAT fData;
		CWVOID* pData;
		CWUINT iData;
	};

	eValueType type;

	cwValueMap() : iData(0), type(eValueTypeNone) {};
	cwValueMap(CWFLOAT f) : fData(f), type(eValueTypeFloat){}
	cwValueMap(CWVOID* p) : pData(p), type(eValueTypePtr) {}
	cwValueMap(CWUINT i) : iData(i), type(eValueTypeUint) {}

};

NS_MINIR_END

#endif