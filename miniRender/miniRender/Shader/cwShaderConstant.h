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

#ifndef _CW_SHADERCONSTANT_H_
#define _CW_SHADERCONSTANT_H_

#include "Base/cwMacros.h"

NS_MINIR_BEGIN

#define CW_SIG_POSITION "POSITION"
#define CW_SIG_NORMAL   "NORMAL"
#define CW_SIG_TEXCOORD "TEXCOORD"
#define CW_SIG_COLOR    "COLOR"

#define CW_SHADER_MAT_WORLDVIEWPROJ   "gMatWorldViewProj"
#define CW_SHADER_MAT_WORLD           "gMatWorld"
#define CW_SHADER_MAT_WORLD_INV_TRANS "gMatWorldInvTranspose"
#define CW_SHADER_MATERIAL            "gMaterial"
#define CW_SHADER_LIGHT               "gLightList"
#define CW_SHADER_LIGHT_CNT           "gLightCount"
#define CW_SHADER_EYE_POSW            "gEyePosWorld"
#define CW_SHADER_TEXTURE0            "gTexture0"
#define CW_SHADER_TEXTURE1            "gTexture1"
#define CW_SHADER_TEXTURE2            "gTexture2"
#define CW_SHADER_TEXTURE3            "gTexture3"
#define CW_SHADER_TEXTURE4            "gTexture4"
#define CW_SHADER_TEXTURE5            "gTexture5"
#define CW_SHADER_TEXTURE6            "gTexture6"
#define CW_SHADER_TEXTURE7            "gTexture7"
#define CW_SHADER_DIFF_TEX_TRANS      "gDiffTexTransform"
#define CW_SHADER_FOG_START           "gFogStart"
#define CW_SHADER_FOG_RANGE           "gFogRange"
#define CW_SHADER_FOG_COLOR           "gFogColor"

typedef enum {
	eDefShaderColor = 0,
	eDefShaderLighting,
	eDefShaderLightingTex,
	eDefShaderTex,
	eDefShaderMax
}eDefShaderID;

NS_MINIR_END

#endif