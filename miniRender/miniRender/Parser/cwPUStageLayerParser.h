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

#ifndef __CW_PU_STAGE_LAYER_PARSER_H__
#define __CW_PU_STAGE_LAYER_PARSER_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Base/cwVector.h"
#include "Ref/cwRef.h"
#include "tinyxml2.h"

#include <unordered_map>

NS_MINIR_BEGIN

class cwPUStageLayer;
class cwPUStageLayerBlend;
class cwPUStageLayerStencil;
class cwPUStageLayerEffect;

class cwPUStageLayerParser : public cwRef
{
public:
	static cwPUStageLayerParser* create();

	cwPUStageLayerParser();

	cwVector<cwPUStageLayer*> parse(tinyxml2::XMLElement* pPUElement);

protected:
	cwPUStageLayerBlend* parsePUBlend(tinyxml2::XMLElement* pBlendElement);
	cwPUStageLayerStencil* parsePUStencil(tinyxml2::XMLElement* pStencilElement);
	cwPUStageLayerEffect* parsePUEffect(tinyxml2::XMLElement* pEffectElement);

protected:
	std::unordered_map <CWSTRING, std::function<cwPUStageLayer*(tinyxml2::XMLElement*)>> m_nMapParser;

};

NS_MINIR_END

#endif
