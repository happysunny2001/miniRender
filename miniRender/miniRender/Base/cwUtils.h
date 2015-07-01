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

#ifndef _CW_UTILS_H_
#define _CW_UTILS_H_

#ifdef _CW_D3D11_

#pragma warning(disable:4005)
#include <d3d11.h>
#include <d3dx11effect.h>
#include <d3dx11async.h>
#include <DxErr.h>
#include <xnamath.h>

#include <string>
#include <vector>
#include <map>
using namespace std;

#include "cwMacros.h"
#include "cwBasicType.h"

NS_MINIR_BEGIN

typedef enum {
	eRenderStateSolid,
	eRenderStateWire,
	eRenderStateNoCull,
	eRenderStateCW,   //clock wise cull
}eRenderState;

//vertex layout
typedef enum{
	ceEleDescPosColor, //position + color
	ceEleDescPosNormal, //position + normal
	ceEleDescPosNormalTex, //position + normal + texture
}ceElementDesc;

typedef enum {
	eLightTypeNone        = 0,
	eLightTypeDirectional = 1,
	eLightTypePoint       = 2,
	eLightTypeSpot        = 3,
}eLightType;

typedef enum {
	eValueTypeNone  = 0,
	eValueTypeFloat = 1,
	eValueTypeUint  = 2,
	eValueTypePtr   = 3,
}eValueType;

typedef enum {
	eLogTypeDebug    = 1,
	eLogTypeError    = 2,
	eLogTypeCritical = 3,
}eLogType;

typedef enum {
	ePrimitiveTypePointList        = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	ePrimitiveTypeLineList         = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	ePrimitiveTypeLineStrip        = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	ePrimitiveTypeTriangleList     = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	ePrimitiveTypeTriangleStrip    = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	ePrimitiveTypeLineListAdj      = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
	ePrimitiveTypeLineStripAdj     = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
	ePrimitiveTypeTriangleListAdj  = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
	ePrimitiveTypeTriangleStripAdj = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
	ePrimitiveTypePatchList1       = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList2       = D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList3       = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList4       = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList5       = D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList6       = D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList7       = D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList8       = D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList9       = D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList10      = D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList11      = D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList12      = D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList13      = D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList14      = D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList15      = D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList16      = D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList17      = D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList18      = D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList19      = D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList20      = D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList21      = D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList22      = D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList23      = D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList24      = D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList25      = D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList26      = D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList27      = D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList28      = D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList29      = D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList30      = D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList31      = D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST,
	ePrimitiveTypePatchList32      = D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST,
}ePrimitiveType;

typedef enum {
	eBufferUsageDefault   = D3D11_USAGE::D3D11_USAGE_DEFAULT,
	eBufferUsageImmutable = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
	eBufferUsageDynamic   = D3D11_USAGE::D3D11_USAGE_DYNAMIC,
	eBufferUsageStaging   = D3D11_USAGE::D3D11_USAGE_STAGING,
}eBufferUsage;

typedef enum {
	eBufferBindVertex          = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER,
	eBufferBindIndex           = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER,
	eBufferBindConstant        = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER,
	eBufferBindShader          = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE,
	eBufferBindSteam           = D3D11_BIND_FLAG::D3D11_BIND_STREAM_OUTPUT,
	eBufferBindRenderTarget    = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET,
	eBufferBindDepthStencil    = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL,
	eBufferBindUnorderedAccess = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS,
}eBufferBindFlag;

typedef enum {
	eAccessFlagRead  = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ,
	eAccessFlagWrite = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE,
}eAccessFlag;

typedef enum {
	eBlendFactorZero = 0,
	eBlendFactorOne,
	eBlendFactorSrcColor,
	eBlendFactorInvSrcColor,
	eBlendFactorSrcAlpha,
	eBlendFactorInvSrcAlpha,
	eBlendFactorDestAlpha,
	eBlendFactorInvDestAlpha,
	eBlendFactorDestColor,
	eBlendFactorInvDestColor,
	eBlendFactorSrcAlphaSat,
	eBlendFactorBlendFactor,
	eBlendFactorInvBlendFactor,
	eBlendFactorSrc1Color,
	eBlendFactorInvSrc1Color,
	eBlendFactorSrc1Alpha,
	eBlendFactorInvSrc1Alpha,
	eBlendFactorMaxCount,
}eBlendFactor;

typedef enum {
	eBlendOpAdd = 0,
	eBlendOpSubtract,
	eBlendOpRevSubtract,
	eBlendOpMin,
	eBlendOpMax,
	eBlendOpMaxCount
}eBlendOp;

typedef enum {
	eStencilOpKeep = 0,
	eStencilOpZero,
	eStencilOpReplace,
	eStencilOpIncrSat,
	eStencilOpDecrSat,
	eStencilOpInvert,
	eStencilOpIncr,
	eStencilOpDecr,
	eStencilOpMaxCount,
}eStencilOp;

typedef enum {
	eComparisonNever = 0,
	eComparisonLess,
	eComparisonEqual,
	eComparisonLessEqual,
	eComparisonGreater,
	eComparisonNotEqual,
	eComparisonGreaterEqual,
	eComparisonAlways,
	eComparisonMaxCount,
}eComparison;

typedef enum {
	eDepthWriteMaskZero = 0,
	eDepthWriteMaskAll,
	eDepthWriteMaskMaxCount,
}eDepthWriteMask;

#define CW_CONTROL MK_CONTROL
#define CW_LBUTTON MK_LBUTTON
#define CW_MBUTTON MK_MBUTTON
#define CW_RBUTTON MK_RBUTTON
#define CW_SHIFT MK_SHIFT
#define CW_UINT_MAX 0xFFFFFFFF

NS_MINIR_END

#endif

#endif
