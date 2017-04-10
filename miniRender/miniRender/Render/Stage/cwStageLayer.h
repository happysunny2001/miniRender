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
#include "Base/cwBasicType.h"
#include "Base/cwUtils.h"
#include "Base/cwVector.h"
#include "cwCommandBatch.h"
#include "cwPipelineNode.h"

#include <unordered_map>
#include <vector>

NS_MINIR_BEGIN

#define CW_COMMAND_BATCH_MAX 16

class cwCamera;
class cwTexture;
class cwRenderNode;
class cwEffect;
class cwViewPort;
class cwScene;
class cwStencil;
class cwBlend;
class cwStage;

class cwStageLayer : public cwPipelineNode
{
public:
	static cwStageLayer* create();
	
	cwStageLayer();
	virtual ~cwStageLayer();

	inline CWUINT getFilterType() const { return m_nFilterType; }
	inline CWVOID setFilterType(eStageLayerFliterType type) { m_nFilterType = type; }
	inline CWBOOL isSetFilterType(eStageLayerFliterType type) const  { return (m_nFilterType&type) != eStageLayerFliterNone; }
	inline CWVOID setFilterTypeBit(eStageLayerFliterType type) { m_nFilterType |= type; }
	inline CWVOID unsetFilterTypeBit(eStageLayerFliterType type) { m_nFilterType &= ~type; }

	inline cwCamera* getCamera() const { return m_pCamera; }
	CWVOID setCamera(cwCamera* pCamera);

	inline cwTexture* getRenderTarget() const { return m_pRenderTarget; }
	virtual CWVOID setRenderTarget(cwTexture* pTexture);

	inline cwTexture* getDepthStencil() const { return m_pDepthStencil; }
	virtual CWVOID setDepthStencil(cwTexture* pTexture);

	inline CWBOOL isClearColor() const { return m_bIsClearColor; }
	inline CWVOID setIsClearColor(CWBOOL b) { m_bIsClearColor = b; }

	inline CWBOOL isClearDepth() const { return m_bIsClearDepth; }
	inline CWVOID setIsClearDepth(CWBOOL b) { m_bIsClearDepth = b; }

	inline CWBOOL isClearStencil() const { return m_bIsClearStencil; }
	inline CWVOID setIsClearStencil(CWBOOL b) { m_bIsClearStencil = b; }

	inline CWBOOL isRefreshRenderTarget() const { return m_bIsRefreshRenderTarget; }
	inline CWVOID setIsRefreshRenderTarget(CWBOOL b) { m_bIsRefreshRenderTarget = b; }

	CWVOID addSelfRenderNode(cwRenderNode* pRenderNode);
	CWVOID removeSelfRenderNode(cwRenderNode* pRenderNode);

	virtual CWVOID addOuterPipeline(cwPipelineNode* pPipelineNode) override;
	virtual CWVOID setUniformEffect(cwEffect* pEffect);

	virtual CWVOID doRender() override;

	virtual CWBOOL buildViewPort(CWFLOAT fLeft, CWFLOAT fTop, CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fMinDepth, CWFLOAT fMaxDepth);

protected:
	virtual CWBOOL init();
	virtual CWBOOL buildViewPort();
	virtual CWBOOL buildStencil();
	virtual CWBOOL buildBlend();

	virtual CWVOID begin();
	virtual CWVOID render();
	virtual CWVOID end();

	virtual CWVOID reset();
	virtual CWVOID bindingRenderTarget();

	virtual CWVOID preFrameConfig(cwShader* pShader);
	virtual CWVOID configPipelineNodeParam(cwShader* pShader);
	virtual CWVOID configLight(cwScene* pScene, cwShader* pShader);
	virtual CWVOID configDirectionalLight(cwScene* pScene, cwShader* pShader);
	virtual CWVOID configPointLight(cwScene* pScene, cwShader* pShader);
	virtual CWVOID configSpotLight(cwScene* pScene, cwShader* pShader);

	virtual CWVOID batchRenderNodes();
	virtual CWVOID batchSelfRenderNodes();
	virtual CWVOID batchSceneRenderNodes();
	virtual CWVOID batchScene2DNodes();

	CWBOOL addRenderNode(cwRenderNode* pNode);
	CWBOOL addRenderNodeWithNodesEffect(cwRenderNode* pNode);
	CWBOOL addRenderNodeWithUniformEffect(cwRenderNode* pNode);

	cwCommandBatch* getFreeCommandBatch();
	cwCommandBatch* getCommandBatch(cwShader* pKeyShader);

protected:
	CWUINT m_nFilterType;
	cwCamera* m_pCamera;
	cwViewPort* m_pViewPort;

	cwTexture* m_pRenderTarget;
	cwTexture* m_pDepthStencil;

	CWBOOL m_bIsClearColor;
	CWBOOL m_bIsClearDepth;
	CWBOOL m_bIsClearStencil;
	CWBOOL m_bIsRefreshRenderTarget;

	cwStencil* m_pStencil;
	cwBlend* m_pBlend;

	cwVector<cwRenderNode*> m_nVecSelfNode;
	std::vector<cwRenderNode*> m_nVecSceneRenderNodes;

	//the effect for all render objects, if NULL, use objects' effect
	cwEffect* m_pUniformEffect;
	cwCommandBatch m_nCommandBatch[CW_COMMAND_BATCH_MAX];
	std::unordered_map<cwShader*, std::vector<cwCommandBatch*>> m_nMapCommandBatch;
	CWUINT m_iCommandBatchIndex;
	cwCommandBatch* m_pLastUsingBatch;

	cwVector<cwPipelineNode*> m_nVecOuterPipelineNode;

};

NS_MINIR_END

#endif
