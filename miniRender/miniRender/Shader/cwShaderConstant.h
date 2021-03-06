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

#ifndef _CW_SHADERCONSTANT_H_
#define _CW_SHADERCONSTANT_H_

#include "Base/cwMacros.h"

NS_MINIR_BEGIN

#define CW_SIG_POSITION "POSITION"
#define CW_SIG_NORMAL   "NORMAL"
#define CW_SIG_TEXCOORD "TEXCOORD"
#define CW_SIG_COLOR    "COLOR"

#define CW_SHADER_MAT_WORLDVIEWPROJ        "gMatWorldViewProj"
#define CW_SHADER_MAT_VIEWPROJ             "gMatViewProj"
#define CW_SHADER_MAT_VIEW				   "gMatView"
#define CW_SHADER_MAT_PROJ				   "gMatProj"
#define CW_SHADER_POS_EYE_WORLD            "gEyePosWorld"
#define CW_SHADER_CAMERA_NEAR_FAR_Z        "gCameraNearFarZ"
#define CW_SHADER_MAT_WORLD                "gMatWorld"
#define CW_SHADER_MAT_WORLD_INV_TRANS      "gMatWorldInvTranspose"

#define CW_SHADER_MATERIAL                 "gMaterial"
#define CW_SHADER_DIFFUSE_TEXTURE          "gDiffuseTexture"
#define CW_SHADER_DIFF_TEX_TRANS           "gDiffTexTransform"

#define CW_SHADER_DIRECTIONAL_LIGHT        "gDirectionalLight"
#define CW_SHADER_DIRECTIONAL_LIGHT_ENABLE "gEnableDirectionalLight"

#define CW_SHADER_POINT_LIGHTS             "gPointLights"
#define CW_SHADER_POINT_LIGHTS_CNT         "gPointLightCnt"

#define CW_SHADER_DIRECTIONAL_LIGHT_CNT    "gDirectionalLightCount"
#define CW_SHADER_POINT_LIGHT              "gPointLight"
#define CW_SHADER_POINT_LIGHT_CNT          "gPointLightCount"
#define CW_SHADER_SPOT_LIGHT			   "gSpotLight"
#define CW_SHADER_SPOT_LIGHT_CNT		   "gSpotLightCount"
#define CW_SHADER_EYE_POSW                 "gEyePosWorld"
#define CW_SHADER_FOG_START                "gFogStart"
#define CW_SHADER_FOG_RANGE                "gFogRange"
#define CW_SHADER_FOG_COLOR                "gFogColor"
#define CW_SHADER_SKY_CUBE_MAP             "gSkyCubeMap"
#define CW_SHADER_REFLECT_CUBE_MAP         "gReflectCubeMap"
#define CW_SHADER_REFLECT_FACTOR           "fReflectFactor"
#define CW_SHADER_DYNAMIC_REFLECT_CUBE_MAP "gDynamicReflectCubeMap"

typedef enum {
	//eDefShaderColor = 0,
	eDefShaderLighting = 0,
	//eDefShaderLightingTex,
	eDefShaderTex,
	//eDefShaderPosTexColor,
	eDefShaderSkyDome,
	//eDefShaderNormalMap,
	eDefShaderMax
}eDefShaderID;

NS_MINIR_END

#endif