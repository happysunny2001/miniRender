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

#include "cwPUStageLayerParser.h"
#include "Repertory/cwRepertory.h"
#include "cwParserManager.h"
#include "cwBlendParser.h"
#include "cwStencilParser.h"
#include "cwEffectParser.h"
#include "Render/ProcessingUnit/cwPUStageLayerBlend.h"
#include "Render/ProcessingUnit/cwPUStageLayerStencil.h"
#include "Render/ProcessingUnit/cwPUStageLayerEffect.h"

NS_MINIR_BEGIN

cwPUStageLayerParser* cwPUStageLayerParser::create()
{
	cwPUStageLayerParser* pParser = new cwPUStageLayerParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwPUStageLayerParser::cwPUStageLayerParser()
{
	m_nMapParser["Blend"]   = CW_CALLBACK_1(cwPUStageLayerParser::parsePUBlend, this);
	m_nMapParser["Stencil"] = CW_CALLBACK_1(cwPUStageLayerParser::parsePUStencil, this);
	m_nMapParser["Effect"]  = CW_CALLBACK_1(cwPUStageLayerParser::parsePUEffect, this);
}

cwVector<cwPUStageLayer*> cwPUStageLayerParser::parse(tinyxml2::XMLElement* pPUElement)
{
	cwVector<cwPUStageLayer*> vecPU;
	if (!pPUElement) return vecPU;

	tinyxml2::XMLElement* pChildElement = pPUElement->FirstChildElement();
	while (pChildElement) {
		if (m_nMapParser.find(pChildElement->Name()) != m_nMapParser.end()) {
			cwPUStageLayer* pPU = m_nMapParser[pChildElement->Name()](pChildElement);
			if (pPU)
				vecPU.pushBack(pPU);
		}

		pChildElement = pChildElement->NextSiblingElement();
	}

	return vecPU;
}

cwPUStageLayerBlend* cwPUStageLayerParser::parsePUBlend(tinyxml2::XMLElement* pBlendElement)
{
	cwBlendParser* pParser = static_cast<cwBlendParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserBlend));
	if (!pParser) return nullptr;

	cwBlend* pBlend = pParser->parse(pBlendElement);
	if (!pBlend) return nullptr;

	cwPUStageLayerBlend* pPUBlend = cwPUStageLayerBlend::create();
	if (!pPUBlend) return nullptr;
	pPUBlend->setBlend(pBlend);

	return pPUBlend;
}

cwPUStageLayerStencil* cwPUStageLayerParser::parsePUStencil(tinyxml2::XMLElement* pStencilElement)
{
	cwStencilParser* pParser = static_cast<cwStencilParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserStencil));
	if (!pParser) return nullptr;

	cwStencil* pStencil = pParser->parse(pStencilElement);
	if (!pStencil) return nullptr;

	cwPUStageLayerStencil* pPUStencil = cwPUStageLayerStencil::create();
	if (!pPUStencil) return nullptr;
	pPUStencil->setStencil(pStencil);

	return pPUStencil;
}

cwPUStageLayerEffect* cwPUStageLayerParser::parsePUEffect(tinyxml2::XMLElement* pEffectElement)
{
	if (!pEffectElement) return nullptr;
	cwEffectParser* pParser = static_cast<cwEffectParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserEffect));
	if (!pParser) return nullptr;

	cwEffect* pEffect = pParser->parse(pEffectElement);
	if (!pEffect) return nullptr;

	cwPUStageLayerEffect* pPUEffect = cwPUStageLayerEffect::create();
	if (!pPUEffect) return nullptr;
	pPUEffect->setEffect(pEffect);

	return pPUEffect;
}

NS_MINIR_END
