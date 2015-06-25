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

#ifdef _CW_D3D11_
#include <d3d11.h>
#include <d3dx11effect.h>
#endif

NS_MINI_BEGIN

#define CW_SHADER_VERSION "fx_5_0"

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
#define CW_SHADER_DIFFUSE_TEXTURE     "gDiffuseTexture"
#define CW_SHADER_DIFF_TEX_TRANS      "gDiffTexTransform"
#define CW_SHADER_FOG_START           "gFogStart"
#define CW_SHADER_FOG_RANGE           "gFogRange"
#define CW_SHADER_FOG_COLOR           "gFogColor"

NS_MINI_END

#endif