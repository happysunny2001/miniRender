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

#ifndef __CW_MINI_RENDER_H__
#define __CW_MINI_RENDER_H__

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Base/cwMap.h"
#include "Base/cwVector.h"
#include "Base/cwValueMap.h"
#include "Base/cwColor.h"
#include "Ref/cwRef.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderManager.h"
#include "Shader/cwShaderConstant.h"
#include "Layouts/cwLayouts.h"
#include "Camera/cwCamera.h"
#include "Timer/cwTimer.h"
#include "Device/cwDevice.h"
#include "RenderObject/cwRenderObject.h"
#include "RenderObject/cwStaticRenderObject.h"
#include "RenderObject/cwDynamicRenderObject.h"
#include "Generator/cwGeometryGenerator.h"
#include "Material/cwMaterial.h"
#include "Light/cwLight.h"
#include "Entity/cwEntity.h"
#include "Entity/cwScene.h"
// #include "cwReflectEntity.h"
#include "Repertory/cwRepertory.h"
// #include "cwLog.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Blend/cwBlend.h"
// #include "cwFog.h"
#include "Stencil/cwStencil.h"
#include "Platform/cwFileSystem.h"
// #include "cwPlaneShadow.h"

#include "Math/cwMath.h"

#endif