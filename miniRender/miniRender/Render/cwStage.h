﻿/*
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
#include "Ref/cwRef.h"
#include "ViewPort/cwViewPort.h"
#include "Texture/cwRenderTexture.h"
#include "Parser/cwStageParser.h"

NS_MINIR_BEGIN

class cwCamera;

class CW_DLL cwStage : public cwRef
{
public:
	static cwStage* create();

	cwStage();
	virtual ~cwStage();

	virtual CWBOOL init();

	inline CWBOOL getEnable() const { return m_bEnable; }
	inline eStageType getType() const { return m_eType; }
	inline const CWSTRING& getName() const { return m_strName; }

	inline const CWSTRING& getCameraName() const { return m_strCameraName; }
	inline cwCamera* getCamera() const { return m_pCamera; }

	inline cwViewPort* getViewPort() const { return m_pViewPort; }
	inline cwRenderTexture* getRenderTexture() const { return m_pRenderTarget; }

protected:
	CWVOID setName(const CWSTRING& strName) { m_strName = strName; }
	CWVOID setType(eStageType eType) { m_eType = eType; }
	CWVOID setEnable(CWBOOL bEnable) { m_bEnable = bEnable; }
	CWVOID setViewPort(cwViewPort* pView);
	CWVOID setRenderTexture(cwRenderTexture* pRenderTexture);

	friend class cwStageParser;

protected:
	CWBOOL m_bEnable;
	eStageType m_eType;
	CWSTRING m_strName;

	CWSTRING m_strCameraName;
	cwCamera* m_pCamera;
	
	cwViewPort* m_pViewPort;
	cwRenderTexture* m_pRenderTarget;

};

NS_MINIR_END

#endif
