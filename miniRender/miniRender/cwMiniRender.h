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

#include "Platform/cwPlatform.h"

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Base/cwMap.h"
#include "Base/cwVector.h"
#include "Base/cwValueMap.h"
#include "Base/cwColor.h"
#include "Base/cwLog.h"
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
#include "Material/MaterialUnit/cwMaterialUnit.h"
#include "Material/MaterialUnit/cwMaterialUnitTexture.h"
#include "Material/MaterialUnit/cwMaterialUnitDynamicReflect.h"
#include "Material/MaterialUnit/cwMaterialUnitMatrix.h"
#include "Light/cwLight.h"
#include "Light/cwDirectionalLight.h"
#include "Light/cwPointLight.h"
#include "Light/cwSpotLight.h"
#include "Entity/cwEntity.h"
#include "Entity/cwScene.h"
#include "Entity/cwMirror.h"
#include "Entity/cwBatchEntity.h"
#include "Entity/cwSkyDome.h"
#include "Entity/cwDynamicReflectionEntity.h"
#include "Engine/cwEngine.h"
#include "Repertory/cwRepertory.h"
#include "Repertory/cwGlobalParameter.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Texture/cwRenderTexture.h"
#include "Texture/cwCubeTexture.h"
#include "Effect/cwEffect.h"
#include "Effect/cwEffectParameter.h"
#include "Effect/cwEffectFloatParameter.h"
#include "Effect/cwEffectMatrixParameter.h"
#include "Effect/cwEffectTextureParameter.h"
#include "Blend/cwBlend.h"
#include "Stencil/cwStencil.h"
#include "Platform/cwFileSystem.h"
#include "Math/cwMath.h"
#include "Event/cwEvent.h"
#include "Event/cwEventDefine.h"
#include "Event/cwEventListener.h"
#include "Event/cwEventManager.h"
#include "Event/cwTouchEvent.h"
#include "Event/cwTouchEventListener.h"
#include "Event/cwKeyboardEvent.h"
#include "Event/cwKeyboardEventListener.h"
#include "Render/cwRenderer.h"
#include "SpatialSorting/cwSpatial.h"
#include "SpatialSorting/cwSpatialFactory.h"
#include "Text/cwText.h"
#include "Sprite/cwRenderNode2D.h"
#include "Sprite/cwSprite.h"
#include "Sprite/cwLabel.h"
#include "Sprite/Primitive/cwPrimitiveNode2D.h"
#include "Utils/cwBaseScene.h"
#include "Resource/cwLoadBatch.h"
#include "Resource/cwResourceLoader.h"
#include "Terrain/cwTerrain.h"
#include "Terrain/cwTerrainTile.h"

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
#include "Platform/Windows/cwWinApplication.h"
#endif

#endif