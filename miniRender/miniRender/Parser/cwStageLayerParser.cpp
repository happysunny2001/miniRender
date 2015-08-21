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

#include "cwStageLayerParser.h"
#include "Repertory/cwRepertory.h"
#include "Render/ProcessingUnit/cwPUStageLayer.h"
#include "cwParserManager.h"
//#include "cwBlendParser.h"
//#include "cwStencilParser.h"
#include "cwPUStageLayerParser.h"

NS_MINIR_BEGIN

cwStageLayerParser* cwStageLayerParser::create()
{
	cwStageLayerParser* pParser = new cwStageLayerParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwStageLayerParser::cwStageLayerParser()
{
	m_nMapParser["Layer"] = CW_CALLBACK_2(cwStageLayerParser::parseAttribute, this);
	m_nMapParser["PU"]    = CW_CALLBACK_2(cwStageLayerParser::parsePU, this);
	//m_nMapParser["Blend"]   = CW_CALLBACK_2(cwStageLayerParser::parseBlend, this);
	//m_nMapParser["Stencil"] = CW_CALLBACK_2(cwStageLayerParser::parseStencil, this);
}

cwStageLayer* cwStageLayerParser::parse(tinyxml2::XMLElement* pStageLayerData)
{
	if (!pStageLayerData) return nullptr;

	cwStageLayer* pStageLayer = cwStageLayer::create();
	if (!pStageLayer) return nullptr;

	parseElement(pStageLayer, pStageLayerData);

	tinyxml2::XMLElement* pChildNode = pStageLayerData->FirstChildElement();
	while (pChildNode) {
		parseElement(pStageLayer, pChildNode);
		pChildNode = pChildNode->NextSiblingElement();
	}

	return pStageLayer;
}

CWVOID cwStageLayerParser::parseElement(cwStageLayer* pStageLayer, tinyxml2::XMLElement* pStageLayerData)
{
	const char* pcName = pStageLayerData->Name();
	if (m_nMapParser.find(pcName) != m_nMapParser.end()) {
		m_nMapParser[pcName](pStageLayer, pStageLayerData);
	}
}

CWVOID cwStageLayerParser::parseAttribute(cwStageLayer* pStageLayer, tinyxml2::XMLElement* pStageLayerData)
{
	const CWCHAR* pcFilter = pStageLayerData->Attribute("EntityType");
	if (pcFilter) {
		if (strncmp(pcFilter, "Entity", 6) == 0) {
			pStageLayer->setFliterType(eStageLayerFliterEntity);
		}
		else if (strncmp(pcFilter, "Stage", 5) == 0) {
			pStageLayer->setFliterType(eStageLayerFliterStage);
		}
		else if (strncmp(pcFilter, "Mirror", 6) == 0) {
			pStageLayer->setFliterType(eStageLayerFliterMirror);
		}
	}
}

CWVOID cwStageLayerParser::parsePU(cwStageLayer* pStageLayer, tinyxml2::XMLElement* pStageLayerData)
{
	if (!pStageLayer || !pStageLayerData) return;

	cwPUStageLayerParser* pPUParser = static_cast<cwPUStageLayerParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserStageLayerPU));
	if (!pPUParser) return;

	cwVector<cwPUStageLayer*> vecPU = pPUParser->parse(pStageLayerData);
	pStageLayer->setPUList(vecPU);
}

//CWVOID cwStageLayerParser::parseBlend(cwStageLayer* pStageLayer, tinyxml2::XMLElement* pStageLayerData)
//{
//	cwBlendParser* pBlendParser = static_cast<cwBlendParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserBlend));
//	if (!pBlendParser) return;
//
//	cwBlend* pBlend = pBlendParser->parse(pStageLayerData);
//	if (pBlend) {
//		pStageLayer->setBlend(pBlend);
//	}
//}
//
//CWVOID cwStageLayerParser::parseStencil(cwStageLayer* pStageLayer, tinyxml2::XMLElement* pStageLayerData)
//{
//	cwStencilParser* pStencilParser = static_cast<cwStencilParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserStencil));
//	if (!pStencilParser) return;
//
//	cwStencil* pStencil = pStencilParser->parse(pStageLayerData);
//	if (pStencil) {
//		pStageLayer->setStencil(pStencil);
//	}
//}

NS_MINIR_END
