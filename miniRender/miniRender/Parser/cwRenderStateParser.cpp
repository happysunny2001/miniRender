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

#include "cwRenderStateParser.h"

NS_MINIR_BEGIN

cwRenderStateParser* cwRenderStateParser::create()
{
	cwRenderStateParser* pParser = new cwRenderStateParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwRenderStateParser::cwRenderStateParser()
{

}

CWBOOL cwRenderStateParser::isBuildIn(tinyxml2::XMLElement* pRenderStateElement)
{
	if (!pRenderStateElement) return CWFALSE;

	const char* pcType = pRenderStateElement->Attribute("Type");
	if (!pcType) return CWFALSE;

	if (CW_STRING_MATCH(pcType, "BuildIn")) return CWTRUE;
	return CWFALSE;
}

eRenderState cwRenderStateParser::parseBuildIn(tinyxml2::XMLElement* pRenderStateElement)
{
	if (!pRenderStateElement) return eRenderStateNone;

	const char* pcName = pRenderStateElement->Attribute("Name");
	if (!pcName) return eRenderStateNone;

	if (CW_STRING_MATCH(pcName, "Solid")) return eRenderStateSolid;
	else if (CW_STRING_MATCH(pcName, "Wire")) return eRenderStateWire;
	else if (CW_STRING_MATCH(pcName, "NoCull")) return eRenderStateNoCull;
	else if (CW_STRING_MATCH(pcName, "CW")) return eRenderStateCW;

	return eRenderStateNone;
}

NS_MINIR_END
