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

#ifndef __CWMATHUTIL_H__
#define __CWMATHUTIL_H__

#include <math.h>
#include "../Base/cwMacros.h"
#include "../Base/cwBasicType.h"

NS_MINIR_BEGIN

class CW_DLL cwMathUtil
{
public:
	static const CWFLOAT cwPI;
	static const CWFLOAT cwFloatEpsilon;
	static const CWFLOAT cwPIx2;
	static const CWFLOAT cwAngleInv180;
	static const CWFLOAT cwPiInv;

public:
	static void cwSinCos(CWFLOAT* fSin, CWFLOAT* fCos, CWFLOAT fTheta)
	{
		*fSin = sinf(fTheta);
		*fCos = cosf(fTheta);
	}

	//return angle form (1.0,0) and (x, ), in radian
	static CWFLOAT angleFrom(CWFLOAT x, CWFLOAT y);
	static CWFLOAT angleRadian(CWFLOAT angle);
	static CWFLOAT angleDegree(CWFLOAT angle);
};

NS_MINIR_END

#endif
