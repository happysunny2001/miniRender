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

#ifndef __CW_STAGE_LAYER_H__
#define __CW_STAGE_LAYER_H__

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "Base/cwVector.h"
#include "Ref/cwRef.h"
#include "Render/cwRenderPipeline.h"

#include <unordered_map>
#include <vector>

NS_MINIR_BEGIN

class cwShader;
class cwRenderNode;
class cwEffect;
class cwBlend;
class cwStencil;
class cwPUStageLayer;
class cwPUBatch;
class cwCamera;

class cwStageLayer : public cwRef
{
public:
	static cwStageLayer* create();

	~cwStageLayer();

	const CWSTRING& getName() const { return m_nStrName; }
	CWVOID setName(const CWSTRING& strName) { m_nStrName = strName; }

	eStageLayerFliterType getFliterType() const { return m_eFilterType; }
	CWVOID setFliterType(eStageLayerFliterType eType) { m_eFilterType = eType; }

	CWVOID reset();
	CWVOID begin(std::vector<cwRenderNode*>* vecEntities);
	CWVOID begin(cwRenderNode* pNode);
	virtual CWVOID render();
	CWVOID end();

	CWVOID setPUList(cwVector<cwPUStageLayer*>& vecPU);
	CWVOID addPU(cwPUStageLayer* pPUStageLayer);
	cwPUStageLayer* getPU(ePURenderType eType);

	CWVOID setBatchPUList(cwVector<cwPUBatch*>& vecPU);
	CWVOID batchPUUpdate(cwRenderBatch* pBatch);

	CWVOID setCamera(cwCamera* pCamera);
	CWVOID setCamera(const CWSTRING& strCamName);
	inline CWVOID setCameraName(const CWSTRING& str) { m_nCameraName = str; }
	inline const CWSTRING& getCameraName() const { return m_nCameraName; }
	inline cwCamera* getCamera() const { return m_pCamera; }

	inline CWVOID setRenderState(eRenderState eState) { m_eRenderState = eState; }

	inline std::unordered_map<cwShader*, cwRenderPipeline*>& getRenderPipeline() { return m_nMapPipeline; }
	inline const CWSTRING& getLayerType() const { return m_nLayerType; }

protected:
	cwStageLayer();

protected:
	cwRenderPipeline* getPipeline(cwRenderNode* pEntity);
	cwRenderPipeline* getUnusePipeline(cwShader* pShader);

	virtual CWVOID addEntities(std::vector<cwRenderNode*>* vecEntities);
	virtual CWVOID addEntities(cwRenderNode* pNode);

protected:
	//stage layer name
	CWSTRING m_nStrName;
	eStageLayerFliterType m_eFilterType;
	CWBOOL m_bTransparent;
	CWSTRING m_nLayerType;

	//render camera
	CWSTRING m_nCameraName;
	cwCamera* m_pCamera;

	cwVector<cwPUStageLayer*> m_nVecPU;
	cwVector<cwPUBatch*> m_nVecPUBatch;

	eRenderState m_eRenderState;
	eRenderState m_eOldRenderState;

	cwRenderPipeline m_nPipeline[CW_STAGE_PIPELINE_SIZE];
	CWUINT m_iPipeLineIndex;

	std::unordered_map<cwShader*, cwRenderPipeline*> m_nMapPipeline;

};

NS_MINIR_END

#endif
