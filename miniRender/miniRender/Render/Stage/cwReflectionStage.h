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

#ifndef __CW_REFLECTION_STAGE_H_
#define __CW_REFLECTION_STAGE_H_

#include "Base/cwMacros.h"
#include "cwStage.h"

#include <vector>

NS_MINIR_BEGIN

class cwCubeTexture;
class cwRenderTexture;
class cwViewPort;
class cwCamera;

class CW_DLL cwReflectionStage : public cwStage
{
public:
	struct sStageRef{
		cwStage* m_pStage;
		CWBOOL m_bReplaceRenderTarget;

		cwCamera* m_pSavedCamera;
		cwTexture* m_pSavedRenderTarget;

		sStageRef() : m_pStage(nullptr), m_bReplaceRenderTarget(CWTRUE), m_pSavedCamera(nullptr), m_pSavedRenderTarget(nullptr) {}
		sStageRef(cwStage* pStage) : m_pStage(pStage), m_bReplaceRenderTarget(CWTRUE), m_pSavedCamera(nullptr), m_pSavedRenderTarget(nullptr) {}
		sStageRef(cwStage* pStage, CWBOOL bReplace) : m_pStage(pStage), m_bReplaceRenderTarget(bReplace), m_pSavedCamera(nullptr), m_pSavedRenderTarget(nullptr) {}
	};

public:
	static cwReflectionStage* create();

	virtual ~cwReflectionStage();

	virtual CWVOID reset() override;
	virtual CWVOID begin() override;
	virtual CWVOID render() override;
	virtual CWVOID end() override;

	CWVOID updateCamera(const cwVector3D& pos);
	CWVOID addStage(cwStage* pStage);
	CWVOID addStage(cwStage* pStage, CWBOOL bReplaceRenderTarget);

protected:
	cwReflectionStage();

	CWVOID buildCameras();

protected:
	cwCubeTexture* m_pCubeTexture;
	cwViewPort* m_pViewport;
	cwCamera* m_nCameras[6];

	std::vector<sStageRef> m_nVecStageRef;

	cwViewPort* m_pPrevViewPort;
	//std::vector<cwCamera*> m_nVecStageCameras;
	//std::vector<cwTexture*> m_nVecStageRenderTexture;

};

NS_MINIR_END

#endif
