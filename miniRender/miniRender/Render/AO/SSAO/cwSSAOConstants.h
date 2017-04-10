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

#ifndef __CW_SSAO_CONSTANTS_H__
#define __CW_SSAO_CONSTANTS_H__

#define CW_SSAO_SHADER_FILE "SSAOTechnique.hlsl"
#define CW_SSAO_SHADER_TECH "TechSSAO"
#define CW_SSAO_SHADER_HORIZ_BLUR_TECH "TechSSAOHorizBlur"
#define CW_SSAO_SHADER_VERT_BLUR_TECH  "TechSSAOVertBlur"
#define CW_SSAO_LAYER_NAME "SSAOStageLayer"
#define CW_SSAO_HORIZ_BLUR_LAYER_NAME "SSAOHorizBlurStageLayer"
#define CW_SSAO_VERT_BLUR_LAYER_NAME "SSAOVertBlurStageLayer"
#define CW_SSAO_STAGE_NAME "SSAOStage"
#define CW_SSAO_PARAM_TEXTURE "gSSAOTexture"

#endif
