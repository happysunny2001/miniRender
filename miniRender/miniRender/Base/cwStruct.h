/*
Copyright © 2015-2016 Ziwei Wang (happy.sunny.2001@163.com)

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
#include "Generator/cwGeometryGenerator.h"

NS_MINIR_BEGIN

struct cwVertexPos
{
	cwVector3D pos;

	cwVertexPos() {}
	cwVertexPos(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
	}

	cwVertexPos& operator =(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		return *this;
	}
};

//vertex structure
struct cwVertexPosColor
{
	cwVector3D pos;
	cwVector4D color;

	cwVertexPosColor() {}
	cwVertexPosColor(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
	}

	cwVertexPosColor& operator =(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		return *this;
	}
};

struct cwVertexPosNormal
{
	cwVector3D pos;
	cwVector3D normal;

	cwVertexPosNormal() {}
	cwVertexPosNormal(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		normal = vertex.normal;
	}

	cwVertexPosNormal& operator =(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		normal = vertex.normal;
		return *this;
	}
};

struct cwVertexPosNormalTex
{
	cwVector3D pos;
	cwVector3D normal;
	cwVector2D tex;

	cwVertexPosNormalTex() {}
	cwVertexPosNormalTex(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		normal = vertex.normal;
		tex = vertex.tex;
	}

	cwVertexPosNormalTex& operator =(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		normal = vertex.normal;
		tex = vertex.tex;
		return *this;
	}
};

struct cwVertexPosNormalTexTan
{
	cwVector3D pos;
	cwVector3D normal;
	cwVector2D tex;
	cwVector3D tan;

	cwVertexPosNormalTexTan() {}
	cwVertexPosNormalTexTan(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		normal = vertex.normal;
		tex = vertex.tex;
		tan = vertex.tangentU;
	}

	cwVertexPosNormalTexTan& operator =(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		normal = vertex.normal;
		tex = vertex.tex;
		tan = vertex.tangentU;
		return *this;
	}
};

struct cwVertexPosTex
{
	cwVector3D pos;
	cwVector2D tex;

	cwVertexPosTex() {}
	cwVertexPosTex(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		tex = vertex.tex;
	}

	cwVertexPosTex& operator =(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		tex = vertex.tex;
		return *this;
	}
};

struct cwVertexPosTexColor
{
	cwVector3D pos;
	cwVector2D tex;
	cwVector4D color;

	cwVertexPosTexColor() {}
	cwVertexPosTexColor(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		tex = vertex.tex;
	}

	cwVertexPosTexColor& operator =(const cwGeometryGenerator::cwVertex& vertex) {
		pos = vertex.pos;
		tex = vertex.tex;
		return *this;
	}
};

typedef struct CW_BUFFER_DESC
{
	CWUINT ByteWidth;
	CWUINT Usage;
	CWUINT BindFlags;
	CWUINT CPUAccessFlags;
	CWUINT MiscFlags;
	CWUINT StructureByteStride;
}CW_BUFFER_DESC;

typedef struct CW_TEXTURE_DESC
{
	CWUINT iWidth;
	CWUINT iHeight;
	CWUINT iMipLevels;
	CWUINT iArraySize;
	eFormat nFormat;
	CWBOOL bMultiSampling;
	CWUINT iMultiSamplingCount;
	CWUINT iMultiSamplingQuality;
	eBufferUsage nUsage;
	CWUINT iBindFlags;
	CWUINT iCpuAccessFlags;
	CWUINT iMiscFlags;
}CW_TEXTURE_DESC;

typedef struct CW_DEPTH_STENCIL_VIEW_DESC
{
	CWUINT iFlags;
	eFormat nFormat;
	eDepthStencilViewDimension nDSVDimension;
	CWUINT iMipSlice;
}CW_DEPTH_STENCIL_VIEW_DESC;

typedef struct CW_SHADER_RESOURCE_VIEW_DESC
{
	eFormat nFormat;
	eShaderResourceViewDimension nSRVDimension;
	CWUINT iMostDetailedMip;
	CWUINT iMipLevels;
}CW_SHADER_RESOURCE_VIEW_DESC;

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