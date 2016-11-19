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

#ifndef __CW_SCENE_H__
#define __CW_SCENE_H__

#include "Base/cwMacros.h"
#include "Base/cwVector.h"
#include "Light/cwLight.h"
#include "Light/cwDirectionalLight.h"
#include "Light/cwPointLight.h"
#include "Light/cwSpotLight.h"
#include "cwRenderNode.h"

NS_MINIR_BEGIN

class cwEntity;
class cwCamera;
class cwSkyDome;
class cwRenderNode2D;

class CW_DLL cwScene : public cwRenderNode
{
public:
	static cwScene* create();

	cwScene();
	virtual ~cwScene();

	virtual CWBOOL init() override;

	virtual CWBOOL createSkyDome(const std::string& strSkyTexture);

	virtual CWVOID addDirectionalLight(cwDirectionalLight* pLight);
	virtual CWVOID addPointLight(cwPointLight* pLight);
	virtual CWVOID addSpotLight(cwSpotLight* pLight);

	virtual CWVOID removeDirectionalLight();
	virtual CWVOID removePointLight(cwPointLight* pLight);
	virtual CWVOID removeSpotLight(cwSpotLight* pLight);

	cwDirectionalLight* getDirectionalLight() const;
	const cwVector<cwPointLight*>& getPointLights() const;
	const cwVector<cwSpotLight*>& getSpotLights() const;
	inline cwSkyDome* getSkyDome() { return m_pSkyDome; }

	CWVOID addChild2D(cwRenderNode2D* pNode2D);
	CWVOID removeChild2D(cwRenderNode2D* pNode2D);
	std::vector<cwRenderNode2D*>& getRenderNodes2D();

	//CWVOID getRenderNode(eSceneObjectType eType, std::vector<cwRenderNode*>& vecNodes);

protected:
	virtual CWBOOL buildRootNode2D();
	CWVOID refreshDirtyNode2D(cwRenderNode2D* pNode2D);
	CWVOID refreshNode2D();

protected:
	//cwVector<cwDirectionalLight*> m_nVecDirectionalLights;
	cwDirectionalLight* m_pDirectionalLight;
	cwVector<cwPointLight*> m_nVecPointLights;
	cwVector<cwSpotLight*> m_nVecSpotLights;
	cwSkyDome* m_pSkyDome;

	cwRenderNode2D* m_pRootNode2D;
	std::vector<cwRenderNode2D*> m_nVecNode2DStack;
	std::vector<cwRenderNode2D*> m_nVecDirty2DStack;
	std::vector<cwRenderNode2D*> m_nVecRefresh2DStack;
	std::vector<cwRenderNode2D*> m_nVecRender2DQueue;

};

NS_MINIR_END

#endif
