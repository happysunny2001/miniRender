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

#ifndef __CW_STAGE_PARSER_H__
#define __CW_STAGE_PARSER_H__

#include "Base/cwMacros.h"
#include "Ref/cwRef.h"
#include "tinyxml2.h"

#include <unordered_map>
#include <functional>

NS_MINIR_BEGIN

class cwStage;
class cwStageLayer;

class cwStageParser : public cwRef
{
public:
	static cwStageParser* create();

	cwStageParser();
	
	cwStage* parse(tinyxml2::XMLElement* pStageData);
	CWVOID deferParse(cwStage* pStage, tinyxml2::XMLElement* pStageElement);

protected:
	CWVOID parseElement(cwStage* pStage, tinyxml2::XMLElement* pStageData);
	CWVOID parseAttribute(cwStage* pStage, tinyxml2::XMLElement* pStageData);
	CWVOID parseCamera(cwStage* pStage, tinyxml2::XMLElement* pStageData);
	CWVOID parseRenderTarget(cwStage* pStage, tinyxml2::XMLElement* pStageData);
	CWVOID parseLayer(cwStage* pStage, tinyxml2::XMLElement* pStageData);

	CWVOID parseTextureList(cwStage* pStage, tinyxml2::XMLElement* pStageElement);
	CWVOID parseEntityList(cwStage* pStage, tinyxml2::XMLElement* pStageElement);

	cwStage* createStage(tinyxml2::XMLElement* pStageData);

protected:
	std::unordered_map<CWSTRING, std::function<CWVOID(cwStage*, tinyxml2::XMLElement*)>> m_nMapParser;
	std::unordered_map<cwStageLayer*, tinyxml2::XMLElement*> m_nMapStageLayer;

};

NS_MINIR_END

#endif
