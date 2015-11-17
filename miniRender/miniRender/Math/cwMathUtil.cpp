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

#include "cwMathUtil.h"

NS_MINIR_BEGIN

const CWFLOAT cwMathUtil::cwPI           = 3.14159f;
const CWFLOAT cwMathUtil::cwFloatEpsilon = 0.00001f;
const CWFLOAT cwMathUtil::cwPIx2         = 6.2831852f;
const CWFLOAT cwMathUtil::cwAngleInv180  = 1.0f / 180.0f;
const CWFLOAT cwMathUtil::cwPiInv        = 1.0f / cwMathUtil::cwPI;
const CWFLOAT cwMathUtil::cwFloatMax     = FLT_MAX;

CWFLOAT cwMathUtil::angleFrom(CWFLOAT x, CWFLOAT y)
{
	//Quadrant I, IV
	if (x >= 0) {
		float f = atanf(y / x);
		if (f < 0)
			f += cwMathUtil::cwPIx2;
		return f;
	}

	//Quadrant II, III
	return atanf(y / x) + cwMathUtil::cwPI;

// 	CWFLOAT angle = atan2f(y, x);
// 	if (angle < 0) angle += cwMath::pix2;
// 	return angle;
}

CWFLOAT cwMathUtil::angleRadian(CWFLOAT angle)
{
	return angle * cwMathUtil::cwAngleInv180 * cwMathUtil::cwPI;
}

CWFLOAT cwMathUtil::angleDegree(CWFLOAT angle)
{
	return angle* cwMathUtil::cwPiInv * 180.0f;
}

//shameless copy from directx's XMConvertFloatToHalf
//only for directx
CWHALF cwMathUtil::convertFloatToHalf(CWFLOAT f)
{
	CWUINT Result;

	CWUINT IValue = ((CWUINT *)(&f))[0];
	CWUINT Sign = (IValue & 0x80000000U) >> 16U;
	IValue = IValue & 0x7FFFFFFFU;      // Hack off the sign

	if (IValue > 0x47FFEFFFU)
	{
		// The number is too large to be represented as a half.  Saturate to infinity.
		Result = 0x7FFFU;
	}
	else
	{
		if (IValue < 0x38800000U)
		{
			// The number is too small to be represented as a normalized half.
			// Convert it to a denormalized value.
			CWUINT Shift = 113U - (IValue >> 23U);
			IValue = (0x800000U | (IValue & 0x7FFFFFU)) >> Shift;
		}
		else
		{
			// Rebias the exponent to represent the value as a normalized half.
			IValue += 0xC8000000U;
		}

		Result = ((IValue + 0x0FFFU + ((IValue >> 13U) & 1U)) >> 13U) & 0x7FFFU;
	}
	return (CWHALF)(Result | Sign);
}

NS_MINIR_END

