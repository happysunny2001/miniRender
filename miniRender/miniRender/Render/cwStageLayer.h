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

#ifndef __CW_STAGE_LAYER_H__
#define __CW_STAGE_LAYER_H__

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "cwRenderPipeline.h"

#include <unordered_map>
#include <vector>

NS_MINIR_BEGIN

class cwShader;
class cwEntity;
class cwEffect;

class cwStageLayer
{
public:
	cwStageLayer();
	~cwStageLayer();

	eStageLayerType getType() const { return m_eType; }
	CWVOID setType(eStageLayerType eType) { m_eType = eType; }

	CWVOID reset();
	CWVOID begin(std::vector<cwEntity*>& vecEntities, cwEffect* pEffect);
	CWVOID render();
	CWVOID end();

protected:
	cwRenderPipeline* getPipeline(cwEntity* pEntity);

	CWVOID addEntities(std::vector<cwEntity*>& vecEntities);
	CWVOID addEntities(std::vector<cwEntity*>& vecEntities, cwEffect* pEffect);

protected:
	eStageLayerType m_eType;

	cwRenderPipeline m_nPipeline[CW_STAGE_PIPELINE_SIZE];
	CWUINT m_iPipeLineIndex;

	std::unordered_map<cwShader*, cwRenderPipeline*> m_nMapPipeline;

};

NS_MINIR_END

#endif
