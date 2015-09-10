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

#ifndef __CW_STRUCT_H__
#define __CW_STRUCT_H__

#include "cwMacros.h"
#include "cwBasicType.h"
#include "cwUtils.h"
#include "Math/cwMath.h"

NS_MINIR_BEGIN

//vertex structure
typedef struct 
{
	cwVector3D pos;
	cwVector4D color;
}cwVertexPosColor;

typedef struct 
{
	cwVector3D pos;
	cwVector3D normal;
}cwVertexPosNormal;

typedef struct 
{
	cwVector3D pos;
	cwVector3D normal;
	cwVector2D tex;
}cwVertexPosNormalTex;

typedef struct  
{
	cwVector3D pos;
	cwVector2D tex;
}cwVertexPosTex;

typedef struct CW_BUFFER_DESC
{
	CWUINT ByteWidth;
	CWUINT Usage;
	CWUINT BindFlags;
	CWUINT CPUAccessFlags;
	CWUINT MiscFlags;
	CWUINT StructureByteStride;
}CW_BUFFER_DESC;

typedef struct cwBlendData
{
	CWBOOL bAlphaToCoverage;
	CWBOOL bIndependentBlend;
	CWBOOL bEnable;
	CWBYTE uColorEnable;
	eBlendFactor srcBlend;
	eBlendFactor dstBlend;
	eBlendOp blendOp;
	eBlendFactor srcBlendAlpha;
	eBlendFactor dstBlendAlpha;
	eBlendOp blendOpAlpha;

	cwBlendData() {
		bAlphaToCoverage = CWFALSE;
		bIndependentBlend = CWFALSE;
		bEnable = CWTRUE;
		uColorEnable = eColorWriteEnableAll;
		srcBlend = eBlendFactorOne;
		dstBlend = eBlendFactorZero;
		blendOp  = eBlendOpAdd;
		srcBlendAlpha = eBlendFactorOne;
		dstBlendAlpha = eBlendFactorZero;
		blendOpAlpha  = eBlendOpAdd;
	}

	cwBlendData(eBlendFactor src, eBlendFactor dst) {
		srcBlend = src;
		dstBlend = dst;
		blendOp  = eBlendOpAdd;
		srcBlendAlpha = src;
		dstBlendAlpha = dst;
		blendOpAlpha  = eBlendOpAdd;
	}

	cwBlendData(eBlendFactor src, eBlendFactor dst, eBlendOp op) {
		srcBlend = src;
		dstBlend = dst;
		blendOp  = op;
		srcBlendAlpha = src;
		dstBlendAlpha = dst;
		blendOpAlpha  = op;
	}

	cwBlendData(eBlendFactor src, eBlendFactor dst, eBlendOp op, eBlendFactor srcAlpha, eBlendFactor dstAlpha, eBlendOp opAlpha) {
		srcBlend = src;
		dstBlend = dst;
		blendOp  = op;
		srcBlendAlpha = srcAlpha;
		dstBlendAlpha = dstAlpha;
		blendOpAlpha  = opAlpha;
	}

}cwBlendData;

typedef struct cwStencilData
{
	CWBOOL depthEnable;
	eDepthWriteMask depthWriteMask;
	eComparison depthFunc;

	CWBOOL stencilEnable;
	CWBYTE stencilReadMask;
	CWBYTE stencilWriteMask;

	eStencilOp frontStencilFailOp;
	eStencilOp frontStencilDepthFailOp;
	eStencilOp frontStencilPassOp;
	eComparison frontStencilFunc;

	eStencilOp backStencilFailOp;
	eStencilOp backStencilDepthFailOp;
	eStencilOp backStencilPassOp;
	eComparison backStencilFunc;
}cwStencilData;

typedef struct cwGPInfo
{
	CWUINT groupX;
	CWUINT groupY;
	CWUINT groupZ;

	cwGPInfo() : groupX(0), groupY(0), groupZ(0)
	{
	}
}cwGPInfo;

NS_MINIR_END

#endif