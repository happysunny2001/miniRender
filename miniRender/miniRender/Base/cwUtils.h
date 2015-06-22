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

NS_MINI_BEGIN

typedef enum {
	eRenderStateSolid,
	eRenderStateWire,
	eRenderStateNoCull,
	eRenderStateCW,   //clock wise cull
}eRenderState;

typedef enum {
	eBlendStateNone   = 1,
	eBlendStateActive = 2,
}eBlendState;

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
	eColorWriteEnableNone  = 0,
	eColorWriteEnableRed   = 1,
	eColorWriteEnableGreen = 2,
	eColorWriteEnableBlue  = 4,
	eColorWriteEnableAlpha = 8,
	eColorWriteEnableAll = (eColorWriteEnableRed | eColorWriteEnableGreen | eColorWriteEnableBlue | eColorWriteEnableAlpha),
}eColorWriteEnable;

typedef enum {
	eBlendFactorZero           = D3D11_BLEND_ZERO,
	eBlendFactorOne            = D3D11_BLEND_ONE,
	eBlendFactorSrcColor       = D3D11_BLEND_SRC_COLOR,
	eBlendFactorInvSrcColor    = D3D11_BLEND_INV_SRC_COLOR,
	eBlendFactorSrcAlpha       = D3D11_BLEND_SRC_ALPHA,
	eBlendFactorInvSrcAlpha    = D3D11_BLEND_INV_SRC_ALPHA,
	eBlendFactorDestAlpha      = D3D11_BLEND_DEST_ALPHA,
	eBlendFactorInvDestAlpha   = D3D11_BLEND_INV_DEST_ALPHA,
	eBlendFactorDestColor      = D3D11_BLEND_DEST_COLOR,
	eBlendFactorInvDestColor   = D3D11_BLEND_INV_DEST_COLOR,
	eBlendFactorSrcAlphaSat    = D3D11_BLEND_SRC_ALPHA_SAT,
	eBlendFactorBlendFactor    = D3D11_BLEND_BLEND_FACTOR,
	eBlendFactorInvBlendFactor = D3D11_BLEND_INV_BLEND_FACTOR,
	eBlendFactorSrc1Color      = D3D11_BLEND_SRC1_COLOR,
	eBlendFactorInvSrc1Color   = D3D11_BLEND_INV_SRC1_COLOR,
	eBlendFactorSrc1Alpha      = D3D11_BLEND_SRC1_ALPHA,
	eBlendFactorInvSrc1Alpha   = D3D11_BLEND_INV_SRC1_ALPHA,
}eBlendFactor;

typedef enum {
	eBlendOpAdd         = D3D11_BLEND_OP_ADD,
	eBlendOpSubtract    = D3D11_BLEND_OP_SUBTRACT,
	eBlendOpRevSubtract = D3D11_BLEND_OP_REV_SUBTRACT,
	eBlendOpMin         = D3D11_BLEND_OP_MIN,
	eBlendOpMax         = D3D11_BLEND_OP_MAX,
}eBlendOp;

typedef enum {
	eStencilOpKeep    = D3D11_STENCIL_OP_KEEP,
	eStencilOpZero    = D3D11_STENCIL_OP_ZERO,
	eStencilOpReplace = D3D11_STENCIL_OP_REPLACE,
	eStencilOpIncrSat = D3D11_STENCIL_OP_INCR_SAT,
	eStencilOpDecrSat = D3D11_STENCIL_OP_DECR_SAT,
	eStencilOpInvert  = D3D11_STENCIL_OP_INVERT,
	eStencilOpIncr    = D3D11_STENCIL_OP_INCR,
	eStencilOpDecr    = D3D11_STENCIL_OP_DECR,
}eStencilOp;

typedef enum {
	eComparisonNever        = D3D11_COMPARISON_NEVER,
	eComparisonLess         = D3D11_COMPARISON_LESS,
	eComparisonEqual        = D3D11_COMPARISON_EQUAL,
	eComparisonLessEqual    = D3D11_COMPARISON_LESS_EQUAL,
	eComparisonGreater      = D3D11_COMPARISON_GREATER,
	eComparisonNotEqual     = D3D11_COMPARISON_NOT_EQUAL,
	eComparisonGreaterEqual = D3D11_COMPARISON_GREATER_EQUAL,
	eComparisonAlways       = D3D11_COMPARISON_ALWAYS,
}eComparison;

typedef enum {
	eDepthWriteMaskZero = D3D11_DEPTH_WRITE_MASK_ZERO,
	eDepthWriteMaskAll  = D3D11_DEPTH_WRITE_MASK_ALL,
}eDepthWriteMask;

typedef D3D11_PRIMITIVE_TOPOLOGY CW_PRIMITIVE_TOPOLOGY;
typedef D3D11_BUFFER_DESC CW_BUFFER_DESC;
typedef ID3D11Buffer      CW_BUFFER;
typedef D3D11_USAGE       CW_BUFFER_USAGE;

#define CW_CONTROL MK_CONTROL
#define CW_LBUTTON MK_LBUTTON
#define CW_MBUTTON MK_MBUTTON
#define CW_RBUTTON MK_RBUTTON
#define CW_SHIFT MK_SHIFT
#define CW_UINT_MAX 0xFFFFFFFF

NS_MINI_END

#endif