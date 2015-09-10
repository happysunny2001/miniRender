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

#ifndef __CW_PU_STAGE_LAYER_LIGHT_TRANS_H__
#define __CW_PU_STAGE_LAYER_LIGHT_TRANS_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "Light/cwLight.h"
#include "Light/cwDirectionalLight.h"
#include "Light/cwPointLight.h"
#include "Light/cwSpotLight.h"
#include "cwPUStageLayer.h"

#include <unordered_map>

NS_MINIR_BEGIN

class cwScene;

class cwPUStageLayerLightTrans : public cwPUStageLayer
{
public:
	static cwPUStageLayerLightTrans* create();

	cwPUStageLayerLightTrans();
	virtual ~cwPUStageLayerLightTrans();

	virtual CWVOID begin() override;
	virtual CWVOID end() override;

	inline CWVOID setLightTrans(const cwMatrix4X4& mat) { m_nMatLightTrans = mat; }

protected:
	CWVOID saveDirectionalLight(cwScene* pScene);
	CWVOID savePointLight(cwScene* pScene);
	CWVOID saveSpotLight(cwScene* pScene);

	CWVOID restoreDirectionalLight(cwScene* pScene);
	CWVOID restorePointLight(cwScene* pScene);
	CWVOID restoreSpotLight(cwScene* pScene);

protected:
	cwMatrix4X4 m_nMatLightTrans;

	std::unordered_map<cwDirectionalLight*, cwVector4D> m_nMapDirectional;
	std::unordered_map<cwPointLight*, cwVector4D> m_nMapPoint;
	std::unordered_map<cwSpotLight*, cwVector4D> m_nMapSpot;
	std::unordered_map<cwSpotLight*, cwVector4D> m_nMapSpotDir;

};

NS_MINIR_END

#endif

