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

#ifndef __CW_STAGE_H__
#define __CW_STAGE_H__

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "Base/cwVector.h"
#include "Base/cwMap.h"
#include "ViewPort/cwViewPort.h"
#include "Texture/cwRenderTexture.h"
#include "Parser/cwStageParser.h"
#include "Render/cwRenderPipeline.h"
#include "Ref/cwRef.h"

#include <vector>
#include <unordered_map>

NS_MINIR_BEGIN

class cwCamera;
class cwEffect;
class cwShader;
class cwStageLayer;
class cwRenderGenerator;
class cwRenderNode;
class cwEntity;

class CW_DLL cwStage : public cwRef
{
public:
	static cwStage* create();

	~cwStage();

	inline CWBOOL getEnable() const { return m_bEnable; }
	inline eStageType getType() const { return m_eType; }
	inline const CWSTRING& getName() const { return m_strName; }

	inline const CWSTRING& getCameraName() const { return m_strCameraName; }
	inline CWVOID setCameraName(const CWSTRING& strCameraName) { m_strCameraName = strCameraName; }
	inline cwCamera* getCamera() const { return m_pCamera; }
	CWVOID setCamera(cwCamera* pCamera);

	//inline cwViewPort* getViewPort() const { return m_pViewPort; }
	
	CWVOID addStageTexture(const CWSTRING& strName, cwTexture* pTexture);
	cwTexture* getStageTexture(const CWSTRING& strName);
	CWBOOL removeStageTexture(const CWSTRING& strName);
	CWBOOL removeStageTexture(cwTexture* pTexture);

	inline CWVOID setIsClearColor(CWBOOL bClear) { m_bClearColor = bClear; }
	inline CWVOID setIsClearDepth(CWBOOL bClear) { m_bClearDepth = bClear; }
	inline CWVOID setIsClearStencil(CWBOOL bClear) { m_bClearStencil = bClear; }

	virtual CWVOID reset();
	virtual CWVOID begin();
	virtual CWVOID render();
	virtual CWVOID end();

	CWVOID addStageEntity(cwEntity* pEntity);
	CWVOID addStageLayer(cwStageLayer* pLayer);
	CWUINT getStageLayerCount() const;
	cwStageLayer* getStageLayer(CWUINT index);
	cwStageLayer* getStageLayer(const CWSTRING& strName);

	CWVOID clearStageLayer(CWBOOL bClear=CWTRUE);
	CWVOID clearStageGenerator();
	CWVOID clearStageEntity();

	CWVOID setName(const CWSTRING& strName) { m_strName = strName; }
	//CWVOID setType(eStageType eType) { m_eType = eType; }
	CWVOID setEnable(CWBOOL bEnable) { m_bEnable = bEnable; }

	inline cwTexture* getRenderTexture() const { return m_pRenderTarget; }
	virtual CWVOID setRenderTexture(cwTexture* pRenderTexture);
	inline cwTexture* getDepthStencil() const { return m_pDepthStencil; }
	virtual CWVOID setDepthStencil(cwTexture* pDepthStencil);
	CWVOID setRefreshRenderTarget(CWBOOL bRefresh);

	CWVOID addRenderGenerator(cwRenderGenerator* pGenerator);
	
protected:
	cwStage();
	std::vector<cwRenderNode*>* getRenderEntities(cwStageLayer* pStageLayer);

protected:
	CWBOOL m_bEnable;
	eStageType m_eType;
	CWSTRING m_strName;

	CWSTRING m_strCameraName;
	cwCamera* m_pCamera;
	
	cwTexture* m_pRenderTarget;
	cwTexture* m_pDepthStencil;
	CWBOOL m_bRefreshRenderTarget;
	CWBOOL m_bClearColor;
	CWBOOL m_bClearDepth;
	CWBOOL m_bClearStencil;

	std::vector<cwRenderNode*> m_nVecRenderNodes;
	std::vector<cwRenderNode*> m_nVecStageEntities;

	cwVector<cwRenderGenerator*> m_nVecGenerator;
	cwMap<CWSTRING, cwTexture*> m_nMapStageTextures;
	cwVector<cwStageLayer*> m_nVecLayer;

};

NS_MINIR_END

#endif
