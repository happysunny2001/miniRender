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

#ifndef __CW_ENGINE_H__
#define __CW_ENGINE_H__

//engine core class
//for main loop
//global data

#include "Base/cwMacros.h"
#include "Base/cwVector.h"
#include "Base/cwMap.h"
#include "Repertory/cwRepertory.h"
#include "Ref/cwRef.h"

#include <unordered_map>

NS_MINIR_BEGIN

class cwScene;
class cwCamera;
class cwShader;
class cwEntity;
class cwEffect;
class cwRenderer;
class cwSpatial;
class cwRenderNode;
class cwRenderNode2D;
class cwFrameRateLabel;
class cwTouch;

class CW_DLL cwEngine : public cwRef
{
public:
	struct sVisibleNodesResult
	{
		cwCamera* m_pCamera;
		cwVector<cwRenderNode*>* m_pVecVisibleNodes;
	};

public:
	CWVOID setScene(cwScene* pScene);
	CWVOID replaceScene(cwScene* pScene);
	cwScene* getCurrScene() { return m_pCurrScene; }

	cwSpatial* getSpatial() const { return m_pSpatial; }
	CWBOOL insertSpatialNode(cwRenderNode*);
	CWBOOL removeSpatialNode(cwRenderNode*);
	CWVOID refreshSpatialNode(cwRenderNode*);

	CWVOID mainLoop(CWFLOAT dt);

	cwCamera* getDefaultCamera();
	cwCamera* getCamera(const CWSTRING& strName);
	CWBOOL removeCamera(cwCamera* pCamera);
	CWVOID addCamera(cwCamera* pCamera);

	inline cwFrameRateLabel* getFrameRateLabel() { return m_pLabelFrameRate; }
	inline CWBOOL getFrameEnable() const { return m_bShowFrame; }
	inline CWVOID setFrameEnable(CWBOOL b) { m_bShowFrame = b; }

	inline cwRenderer* getRenderer() const { return m_pRenderer; }
	CWVOID setRenderer(cwRenderer* pRenderer);
	virtual CWVOID loadRenderer(const CWSTRING& strConfFile);

	CWVOID resize();

	virtual CWVOID getVisibleNodes(cwCamera*, eRenderType, std::vector<cwRenderNode*>&);
	virtual cwRenderNode* getScreenClickNode(cwTouch* pTouch);

protected:
	static cwEngine* create();

	cwEngine();
	virtual ~cwEngine();

	virtual CWBOOL init();
	virtual CWVOID buildDefaultCamera();

	virtual CWVOID render();
	CWVOID clearVisibleNodes();
	cwVector<cwRenderNode*>* getEmptyNodeList();

	CWVOID buildFrameRateLabel();
	CWVOID refreshFrameRate(CWFLOAT dt);
	CWVOID extractRenderNodes(sVisibleNodesResult&, std::vector<cwRenderNode*>&, eRenderType);

	friend class cwRenderer;
	friend class cwRepertory;

protected:
	cwScene* m_pCurrScene;
	cwSpatial* m_pSpatial;
	cwRenderer* m_pRenderer;

	cwCamera* m_pDefaultCamera;
	cwMap<CWSTRING, cwCamera*> m_nMapCameras;

	CWUINT m_uNodeVectorCounter;
	std::vector<cwVector<cwRenderNode*>> m_nVecVisiableNodes;
	std::vector<sVisibleNodesResult> m_nVisibleResult;

	cwFrameRateLabel* m_pLabelFrameRate;
	CWBOOL m_bShowFrame;

};

NS_MINIR_END

#endif
