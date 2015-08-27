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
#include "ViewPort/cwViewPort.h"
#include "Texture/cwRenderTexture.h"
#include "Parser/cwStageParser.h"
#include "cwRenderPipeline.h"
#include "Ref/cwRef.h"

#include <unordered_map>

NS_MINIR_BEGIN

class cwCamera;
class cwEffect;
class cwShader;
class cwStageLayer;
class cwRenderGenerator;

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

	inline cwViewPort* getViewPort() const { return m_pViewPort; }
	inline cwRenderTexture* getRenderTexture() const { return m_pRenderTarget; }

	inline CWVOID setIsClearColor(CWBOOL bClear) { m_bClearColor = bClear; }
	inline CWVOID setIsClearDepth(CWBOOL bClear) { m_bClearDepth = bClear; }
	inline CWVOID setIsClearStencil(CWBOOL bClear) { m_bClearStencil = bClear; }

	CWVOID reset();
	CWVOID begin();
	CWVOID render();
	CWVOID end();

	CWVOID addStageEntity(cwEntity* pEntity);
	CWVOID addStageLayer(cwStageLayer* pLayer);
	CWUINT getStageLayerCount() const;
	cwStageLayer* getStageLayer(CWUINT index);
	CWVOID clearStageLayer(CWBOOL bClear=CWTRUE);
	CWVOID clearStageGenerator();
	CWVOID clearStageEntity();

	CWVOID setName(const CWSTRING& strName) { m_strName = strName; }
	CWVOID setType(eStageType eType) { m_eType = eType; }
	CWVOID setEnable(CWBOOL bEnable) { m_bEnable = bEnable; }
	CWVOID setViewPort(cwViewPort* pView);
	CWVOID setRenderTexture(cwRenderTexture* pRenderTexture);
	CWVOID setRefreshRenderTarget(CWBOOL bRefresh);
	CWVOID addRenderGenerator(cwRenderGenerator* pGenerator);
	
protected:
	cwStage();

protected:
	CWBOOL m_bEnable;
	eStageType m_eType;
	CWSTRING m_strName;

	CWSTRING m_strCameraName;
	cwCamera* m_pCamera;
	
	cwViewPort* m_pViewPort;
	cwRenderTexture* m_pRenderTarget;
	CWBOOL m_bRefreshRenderTarget;
	CWBOOL m_bClearColor;
	CWBOOL m_bClearDepth;
	CWBOOL m_bClearStencil;

	cwVector<cwRenderGenerator*> m_nVecGenerator;

	cwVector<cwEntity*> m_nVecStageEntities;
	cwVector<cwStageLayer*> m_nVecLayer;

};

NS_MINIR_END

#endif
