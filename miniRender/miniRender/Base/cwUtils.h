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
	eFormatUnknown = 0,
	eFormatR32g32b32a32Typeless,
	eFormatR32g32b32a32Float,
	eFormatR32g32b32a32Uint,
	eFormatR32g32b32a32Sint,
	eFormatR32g32b32Typeless,
	eFormatR32g32b32Float,
	eFormatR32g32b32Uint,
	eFormatR32g32b32Sint,
	eFormatR16g16b16a16Typeless,
	eFormatR16g16b16a16Float,
	eFormatR16g16b16a16Unorm,
	eFormatR16g16b16a16Uint,
	eFormatR16g16b16a16Snorm,
	eFormatR16g16b16a16Sint,
	eFormatR32g32Typeless,
	eFormatR32g32Float,
	eFormatR32g32Uint,
	eFormatR32g32Sint,
	eFormatR32g8x24Typeless,
	eFormatD32FloatS8x24Uint,
	eFormatR32FloatX8x24Typeless,
	eFormatX32TypelessG8x24Uint,
	eFormatR10g10b10a2Typeless,
	eFormatR10g10b10a2Unorm,
	eFormatR10g10b10a2Uint,
	eFormatR11g11b10Float,
	eFormatR8g8b8a8Typeless,
	eFormatR8g8b8a8Unorm,
	eFormatR8g8b8a8UnormSrgb,
	eFormatR8g8b8a8Uint,
	eFormatR8g8b8a8Snorm,
	eFormatR8g8b8a8Sint,
	eFormatR16g16Typeless,
	eFormatR16g16Float,
	eFormatR16g16Unorm,
	eFormatR16g16Uint,
	eFormatR16g16Snorm,
	eFormatR16g16Sint,
	eFormatR32Typeless,
	eFormatd32Float,
	eFormatR32Float,
	eFormatR32Uint,
	eFormatR32Sint,
	eFormatR24g8Typeless,
	eFormatd24UnormS8Uint,
	eFormatR24UnormX8Typeless,
	eFormatx24TypelessG8Uint,
	eFormatR8g8Typeless,
	eFormatR8g8Unorm,
	eFormatR8g8Uint,
	eFormatR8g8Snorm,
	eFormatR8g8Sint,
	eFormatR16Typeless,
	eFormatR16Float,
	eFormatd16Unorm,
	eFormatR16Unorm,
	eFormatR16Uint,
	eFormatR16Snorm,
	eFormatR16Sint,
	eFormatR8Typeless,
	eFormatR8Unorm,
	eFormatR8Uint,
	eFormatR8Snorm,
	eFormatR8Sint,
	eFormatA8Unorm,
	eFormatR1Unorm,
	eFormatR9g9b9e5Sharedexp,
	eFormatR8g8_B8g8Unorm,
	eFormatG8r8G8b8Unorm,
	eFormatBc1Typeless,
	eFormatBc1Unorm,
	eFormatBc1UnormSrgb,
	eFormatBc2Typeless,
	eFormatBc2Unorm,
	eFormatBc2UnormSrgb,
	eFormatBc3Typeless,
	eFormatBc3Unorm,
	eFormatBc3UnormSrgb,
	eFormatBc4Typeless,
	eFormatBc4Unorm,
	eFormatBc4Snorm,
	eFormatBc5Typeless,
	eFormatBc5Unorm,
	eFormatBc5Snorm,
	eFormatB5g6r5Unorm,
	eFormatB5g5r5a1Unorm,
	eFormatB8g8r8a8Unorm,
	eFormatB8g8r8x8Unorm,
	eFormatR10g10b10XrBiasA2Unorm,
	eFormatB8g8r8a8Typeless,
	eFormatB8g8r8a8UnormSrgb,
	eFormatB8g8r8x8Typeless,
	eFormatB8g8r8x8UnormSrgb,
	eFormatBc6hTypeless,
	eFormatBc6hUf16,
	eFormatBc6hSf16,
	eFormatBc7Typeless,
	eFormatBc7Unorm,
	eFormatBc7UnormSrgb,
	eFormatForceUint,
	eFormatMaxCount,
}eFormat;

typedef enum {
	eClassificationPerVertex = 0,
	eClassificationPerInstance,
	eClassificationMaxCount,
}eClassification;

typedef enum {
	eBufferUsageDefault = 0,
	eBufferUsageImmutable,
	eBufferUsageDynamic,
	eBufferUsageStaging,
	eBufferUsageMaxCount,
}eBufferUsage;

typedef enum {
	eBufferBindNone = 0,
	eBufferBindVertex,
	eBufferBindIndex,
	eBufferBindConstant,
	eBufferBindShader,
	eBufferBindSteam,
	eBufferBindRenderTarget,
	eBufferBindDepthStencil,
	eBufferBindWritable,
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
	eRenderTextureTexture,
	eRenderTextureTarget,
	eRenderTextureShader,
	eRenderTextureWritable,
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
	eShaderParamViewProj,
	eShaderParamWorld,
	eShaderParamWorldInvTrans,
	eShaderParamMaterial, 
	eShaderParamDirectionalLight,
	eShaderParamDirectionalLightCnt,
	eShaderParamPointLight,
	eShaderParamPointLightCnt,
	eShaderParamSpotLight,
	eShaderParamSpotLightCnt,
	eShaderParamEyePosWorld,
	eShaderParamTexture0,
	eShaderParamTexture1,
	eShaderParamTexture2,
	eShaderParamTexture3,
	eShaderParamTexture4,
	eShaderParamTexture5,
	eShaderParamTexture6,
	eShaderParamTexture7,
	eShaderParamFogStart,
	eShaderParamFogRange,
	eShaderParamFogColor,
	eShaderParamSkyCubeMap,
	eShaderParamReflectCubeMap,
	eShaderParamReflectFactor,
	eShaderParamMax,
}eShaderParamIndex;

typedef enum {
	eRenderTypeNode = 1,
	eRenderTypeEntity,
	eRenderTypeBatchEntity,
	eRenderTypePrimitiveEntity,
	eRenderTypeScene,
	eRenderTypeMirror,
	eRenderTypeNode2D,
	eRenderTypeSprite,

	eRenderTypeUserBegin = 10000,
}eRenderType;

typedef enum {
	eParserStage = 0,
	eParserRenderer,
	eParserEntity,
	eParserBlend,
	eParserStencil,
	eParserStageLayer,
	eParserStageLayerPU,
	eParserTexture,
	eParserEffect,
	eParserRenderState,
	eParserTypeMax
}eParserType;

typedef enum {
	eStageLayerFliterEntity = 1,
	eStageLayerFliterMirror,
	eStageLayerFliterStage,
}eStageLayerFliterType;

typedef enum {
	ePUNone = 0,
	ePUStageLayerBlend = 1,
	ePUStageLayerStencil,
	ePUStageLayerWorldTrans,
	ePUStageLayerLightTrans,
	ePUStageLayerEffect,
}ePURenderType;

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
