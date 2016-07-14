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

#ifndef __CW_PU_BATCH_SHADOW_MAP_H__
#define __CW_PU_BATCH_SHADOW_MAP_H__

#include "Base/cwMacros.h"
#include "cwPUBatch.h"
#include "Math/cwMatrix4X4.h"

NS_MINIR_BEGIN

class cwShader;
class cwShadowMapStage;

class cwPUBatchShadowMap : public cwPUBatch
{
public:
	static cwPUBatchShadowMap* create();
	
	cwPUBatchShadowMap();
	virtual ~cwPUBatchShadowMap();

	virtual CWVOID reset() override;
	virtual CWVOID updateBatch(cwRenderBatch* pBatch) override;
	virtual CWVOID bindingStage(cwStage* pStage);

	CWVOID updateShaderTextureParam();
	CWVOID updateShaderTransParam(cwRenderBatch* pBatch);
	
	inline CWVOID setTextureParameterName(const CWSTRING& strName) { m_nShadowMapTextureName = strName; }
	inline CWVOID setTransParameterName(const CWSTRING& strName) { m_nShadowMapTransName = strName; }

protected:
	cwShader* m_pShader;

	CWSTRING m_nShadowMapTextureName;
	CWSTRING m_nShadowMapTransName;

	cwShadowMapStage* m_pShadowStage;

};

NS_MINIR_END

#endif
