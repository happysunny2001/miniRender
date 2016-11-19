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

#ifndef __CW_TBDR_CORE_STAGE_LAYER_H__
#define __CW_TBDR_CORE_STAGE_LAYER_H__

#include "Base/cwMacros.h"
#include "Render/Stage/cwStageLayer.h"
#include "Light/cwDirectionalLight.h"
#include "Light/cwPointLight.h"
#include "Buffer/cwBuffer.h"
#include "cwTBDRConstants.h"

NS_MINIR_BEGIN

class cwTBDRCoreStageLayer : public cwStageLayer
{
public:
	CW_CREATE_FUNC(cwTBDRCoreStageLayer);

	cwTBDRCoreStageLayer();
	virtual ~cwTBDRCoreStageLayer();

	virtual CWVOID configDirectionalLight(cwScene* pScene, cwShader* pShader) override;
	virtual CWVOID configPointLight(cwScene* pScene, cwShader* pShader) override;

protected:
	virtual CWBOOL init() override;

	virtual CWVOID preFrameConfig(cwShader* pShader) override;

	CWBOOL buildDirectionalLight();
	CWBOOL buildPointLightBuffer();

protected:
	vector<cwPointLight::PointLight_Struct> m_nVecViewSpacePointLights;
	cwDirectionalLight* m_pViewSpaceDirectionalLight;

	cwBuffer* m_pPointLightBuffer;

};

NS_MINIR_END

#endif
