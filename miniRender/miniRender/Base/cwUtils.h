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

#ifndef _CW_UTILS_H_
#define _CW_UTILS_H_

#ifdef _CW_D3D11_

#pragma warning(disable:4005)
//#include <d3d11.h>
//#include <d3dx11effect.h>
//#include <d3dx11async.h>
//#include <DxErr.h>
//#include <xnamath.h>

#include <string>
#include <vector>
#include <map>
using namespace std;

#include "cwMacros.h"
#include "cwBasicType.h"

NS_MINIR_BEGIN

typedef enum {
	eRenderStateNone = 0,
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
	ceEleDescPosTex, //position + color + texture
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
	eStageTypeNormal = 1,
	eStageTypeSpecific,
}eStageType;

typedef enum {
	ePrimitiveTypePointList = 0,
	ePrimitiveTypeLineList,
	ePrimitiveTypeLineStrip,
	ePrimitiveTypeTriangleList,
	ePrimitiveTypeTriangleStrip,
	ePrimitiveTypeLineListAdj,
	ePrimitiveTypeLineStripAdj,
	ePrimitiveTypeTriangleListAdj,
	ePrimitiveTypeTriangleStripAdj,
	ePrimitiveTypePatchList1,
	ePrimitiveTypePatchList2,
	ePrimitiveTypePatchList3,
	ePrimitiveTypePatchList4,
	ePrimitiveTypePatchList5,
	ePrimitiveTypePatchList6,
	ePrimitiveTypePatchList7,
	ePrimitiveTypePatchList8,
	ePrimitiveTypePatchList9,
	ePrimitiveTypePatchList10,
	ePrimitiveTypePatchList11,
	ePrimitiveTypePatchList12,
	ePrimitiveTypePatchList13,
	ePrimitiveTypePatchList14,
	ePrimitiveTypePatchList15,
	ePrimitiveTypePatchList16,
	ePrimitiveTypePatchList17,
	ePrimitiveTypePatchList18,
	ePrimitiveTypePatchList19,
	ePrimitiveTypePatchList20,
	ePrimitiveTypePatchList21,
	ePrimitiveTypePatchList22,
	ePrimitiveTypePatchList23,
	ePrimitiveTypePatchList24,
	ePrimitiveTypePatchList25,
	ePrimitiveTypePatchList26,
	ePrimitiveTypePatchList27,
	ePrimitiveTypePatchList28,
	ePrimitiveTypePatchList29,
	ePrimitiveTypePatchList30,
	ePrimitiveTypePatchList31,
	ePrimitiveTypePatchList32,
	ePrimitiveTypeMaxCount,
}ePrimitiveType;

typedef enum {
	eBufferUsageDefault = 0,
	eBufferUsageImmutable,
	eBufferUsageDynamic,
	eBufferUsageStaging,
	eBufferUsageMaxCount,
}eBufferUsage;

typedef enum {
	eBufferBindVertex = 0,
	eBufferBindIndex,
	eBufferBindConstant,
	eBufferBindShader,
	eBufferBindSteam,
	eBufferBindRenderTarget,
	eBufferBindDepthStencil,
	eBufferBindUnorderedAccess,
	eBufferBindMaxCount,
}eBufferBindFlag;

typedef enum {
	eAccessFlagNone = 0,
	eAccessFlagRead,
	eAccessFlagWrite,
	eAccessFlagMaxCount,
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

typedef enum {
	eRenderTextureNone = 0,
	eRenderTextureTarget = 1,
	eRenderTextureShader = 2,
	eRenderTextureMultiThread = 3,
}eRenderTextureType;

typedef enum {
	eColorWriteEnableNone = 0,
	eColorWriteEnableRed = 1,
	eColorWriteEnableGreen = 2,
	eColorWriteEnableBlue = 4,
	eColorWriteEnableAlpha = 8,
	eColorWriteEnableAll = (eColorWriteEnableRed | eColorWriteEnableGreen | eColorWriteEnableBlue | eColorWriteEnableAlpha),
}eColorWriteEnable;

typedef enum {
	eShaderParamWorldViewProj = 0,
	eShaderParamWorld         = 1,
	eShaderParamWorldInvTrans = 2,
	eShaderParamMaterial      = 3, 
	eShaderParamLight         = 4,
	eShaderParamLightCnt      = 5,
	eShaderParamEyePosWorld   = 6,
	eShaderParamTexture0      = 7,
	eShaderParamTexture1      = 8,
	eShaderParamTexture2      = 9,
	eShaderParamTexture3      = 10,
	eShaderParamTexture4      = 11,
	eShaderParamTexture5      = 12,
	eShaderParamTexture6      = 13,
	eShaderParamTexture7      = 14,
	eShaderParamFogStart      = 15,
	eShaderParamFogRange      = 16,
	eShaderParamFogColor      = 17,
	eShaderParamMax,
}eShaderParamIndex;

typedef enum {
	eSceneObjectNode = 1,
	eSceneObjectEntity,
	eSceneObjectScene,
	eSceneObjectMirror,
}eSceneObjectType;

typedef enum {
	eParserStage = 0,
	eParserRenderer,
	eParserEntity,
	eParserBlend,
	eParserStencil,
	eParserStageLayer,
	eParserTypeMax
}eParserType;

typedef enum {
	eStageLayerFliterEntity = 1,
	eStageLayerFliterMirror,
	eStageLayerFliterStage,
}eStageLayerFliterType;

#define CW_CONTROL MK_CONTROL
#define CW_LBUTTON MK_LBUTTON
#define CW_MBUTTON MK_MBUTTON
#define CW_RBUTTON MK_RBUTTON
#define CW_SHIFT MK_SHIFT
#define CW_UINT_MAX 0xFFFFFFFF

#define CW_PIPELINE_BATCH_SIZE 1024
#define CW_STAGE_PIPELINE_SIZE 16

NS_MINIR_END

#endif

#endif
