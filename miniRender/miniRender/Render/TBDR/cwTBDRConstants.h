/*
Copyright © 2015-2016 Ziwei Wang

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

#ifndef __CW_TBDR_CONSTANTS_H__
#define __CW_TBDR_CONSTANTS_H__

#define CW_MAX_LIGHTS 1024
#define CW_ENABLE_MSAA 1
#define CW_MSAA_SAMPLES 4

#define CW_TBDR_SHADER_FILE        "TBDR/TBDRTechnique.hlsl"
#define CW_TBDR_CORE_SHADER_FILE   "TBDR/TBDRCoreTechnique.hlsl"
#define CW_TBDR_SHADER_TECH_CLEAR  "GBufferClear"
#define CW_TBDR_SHADER_TECH_BUILD  "GBufferBuild"
#define CW_TBDR_SHADER_TECH_CORE   "TBDRCore"
#define CW_TBDR_SHADER_TECH_RESULT "ScreenQuad"

#define CW_TBDR_SHADER_PARAM_NORMAL_TEXTURE   "gNormalTexture"
#define CW_TBDR_SHADER_PARAM_DIFFUSE_TEXTURE  "gDiffuseTexture"
#define CW_TBDR_SHADER_PARAM_SPECULAR_TEXTURE "gSpecularTexture"
#define CW_TBDR_SHADER_PARAM_DEPTH_TEXTURE    "gDepthTexture"
#define CW_TBDR_SHADER_PARAM_FRAME_BUFFER     "gLitFrameBuffer"
#define CW_TBDR_SHADER_PARAM_LIT_FRAME_BUFFER "gLitResultFrameBuffer"
#define CW_TBDR_SHADER_PARAM_FRAME_DIM        "gFrameBufferDimensions"

#define CW_TBDR_CLEAR_LAYER_NAME  "ClearLayer"
#define CW_TBDR_RENDER_LAYER_NAME "RenderLayer"
#define CW_TBDR_CORE_LAYER_NAME   "TBDRCoreLayer"
#define CW_TBDR_RESULT_LAYER_NAME "ResultLayer"

#endif
