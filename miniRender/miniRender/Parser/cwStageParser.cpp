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

#include "cwStageParser.h"
#include "cwEntityParser.h"
#include "cwStageLayerParser.h"
#include "cwParserManager.h"
#include "Render/cwStage.h"
#include "Render/cwStageLayer.h"
#include "Device/cwDevice.h"
#include "Repertory/cwRepertory.h"
#include "ViewPort/cwViewPort.h"
#include "Texture/cwRenderTexture.h"
#include "Texture/cwTextureManager.h"
#include "Engine/cwEngine.h"

NS_MINIR_BEGIN

cwStageParser* cwStageParser::create()
{
	cwStageParser* pParser = new cwStageParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwStageParser::cwStageParser()
{
	m_nMapParser["Stage"]        = CW_CALLBACK_2(cwStageParser::parseAttribute, this);
	m_nMapParser["Camera"]       = CW_CALLBACK_2(cwStageParser::parseCamera, this);
	m_nMapParser["ViewPort"]     = CW_CALLBACK_2(cwStageParser::parseViewPort, this);
	m_nMapParser["RenderTarget"] = CW_CALLBACK_2(cwStageParser::parseRenderTarget, this);
	m_nMapParser["Layer"]        = CW_CALLBACK_2(cwStageParser::parseLayer, this);
}

cwStage* cwStageParser::parse(tinyxml2::XMLElement* pStageData)
{
	if (!pStageData) return nullptr;

	cwStage* pStage = new cwStage();
	if (!pStage) return nullptr;

	parseElement(pStage, pStageData);

	tinyxml2::XMLElement* pChildNode = pStageData->FirstChildElement();
	while (pChildNode) {
		parseElement(pStage, pChildNode);
		pChildNode = pChildNode->NextSiblingElement();
	}

	return pStage;
}

CWVOID cwStageParser::parseElement(cwStage* pStage, tinyxml2::XMLElement* pStageData)
{
	const char* pcName = pStageData->Name();
	if (m_nMapParser.find(pcName) != m_nMapParser.end()) {
		m_nMapParser[pcName](pStage, pStageData);
	}
}

CWVOID cwStageParser::parseAttribute(cwStage* pStage, tinyxml2::XMLElement* pStageData)
{
	const char* pcName = pStageData->Attribute("Name");
	if (pcName) {
		pStage->setName(pcName);
	}

	const CWCHAR* pcType = pStageData->Attribute("Type");
	if (pcType) {
		CWSTRING strType = pcType;
		if (strncmp(pcType, "normal", 6) == 0) {
			pStage->setType(eStageTypeNormal);
		}
		else if (strncmp(pcType, "specific", 8) == 0) {
			pStage->setType(eStageTypeSpecific);
		}
	}
	

	const char* pcEnable = pStageData->Attribute("Enable");
	pStage->setEnable(cwRepertory::getInstance().getParserManager()->getBool(pcEnable));
}

CWVOID cwStageParser::parseCamera(cwStage* pStage, tinyxml2::XMLElement* pCameraData)
{
	const CWCHAR* pcCameraName = pCameraData->Attribute("Name");
	pStage->setCameraName(pcCameraName);
	//if (pcCameraName && strncmp(pcCameraName, "default", 7) != 0) {
	//	//set camera object
	//}
}

CWVOID cwStageParser::parseViewPort(cwStage* pStage, tinyxml2::XMLElement* pViewPortData)
{
	const CWCHAR* pcViewPortName = pViewPortData->Attribute("Name");
	if (pcViewPortName && strncmp(pcViewPortName, "default", 7) == 0) return;

	CWFLOAT fTopLeftX = pViewPortData->FloatAttribute("TopLeftX");
	CWFLOAT fTopLeftY = pViewPortData->FloatAttribute("TopLeftY");
	CWFLOAT fWidth    = pViewPortData->FloatAttribute("Width");
	CWFLOAT fHeight   = pViewPortData->FloatAttribute("Height");
	CWFLOAT fMinDepth = pViewPortData->FloatAttribute("MinDepth");
	CWFLOAT fMaxDepth = pViewPortData->FloatAttribute("MaxDepth");

	if (fWidth < 0) {
		fWidth = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinWidth));
	}

	if (fHeight < 0) {
		fHeight = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinHeight));
	}

	cwViewPort* pViewPort = cwRepertory::getInstance().getDevice()->createViewPort(fTopLeftX, fTopLeftY, fWidth, fHeight, fMinDepth, fMaxDepth);
	if (!pViewPort) return;

	pStage->setViewPort(pViewPort);
}

CWVOID cwStageParser::parseRenderTarget(cwStage* pStage, tinyxml2::XMLElement* pRenderTarget)
{
	const CWCHAR* pcType = pRenderTarget->Attribute("Type");
	if (!pcType) return;

	if (strncmp(pcType, "backbuffer", 10) == 0) {

	}
	else if (strncmp(pcType, "texture", 7) == 0) {
		CWFLOAT fWidth = pRenderTarget->FloatAttribute("Width");
		CWFLOAT fHeight = pRenderTarget->FloatAttribute("Height");

		cwRenderTexture* pRenderTexture = cwRepertory::getInstance().getTextureManager()->createRenderTexture(fWidth, fHeight);
		if (pRenderTexture) {
			pStage->setRenderTexture(pRenderTexture);
		}
	}

	cwParserManager* pParserManager = cwRepertory::getInstance().getParserManager();

	const char* pcIsClearColor = pRenderTarget->Attribute("IsClearColor");
	if (pcIsClearColor)
		pStage->setIsClearColor(pParserManager->getBool(pcIsClearColor));

	const char* pcIsClearDepth = pRenderTarget->Attribute("IsClearDepth");
	if (pcIsClearDepth)
		pStage->setIsClearDepth(pParserManager->getBool(pcIsClearDepth));

	const char* pcIsClearStencil = pRenderTarget->Attribute("IsClearStencil");
	if (pcIsClearStencil)
		pStage->setIsClearStencil(pParserManager->getBool(pcIsClearStencil));
}

CWVOID cwStageParser::parseLayer(cwStage* pStage, tinyxml2::XMLElement* pLayerData)
{
	cwStageLayerParser* pStageLayerParser = static_cast<cwStageLayerParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserStageLayer));
	if (!pStageLayerParser) return;

	cwStageLayer* pStageLayer = pStageLayerParser->parse(pLayerData);
	if (pStageLayer)
		pStage->addStageLayer(pStageLayer);
}

CWVOID cwStageParser::deferParse(cwStage* pStage, tinyxml2::XMLElement* pStageElement)
{
	if (!pStageElement || !pStage) return;

	tinyxml2::XMLElement* pEntityListElement = pStageElement->FirstChildElement("EntityList");
	if (pEntityListElement) {
		cwEntityParser* pEntityParser = static_cast<cwEntityParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserEntity));

		if (pEntityParser) {
			tinyxml2::XMLElement* pEntityElement = pEntityListElement->FirstChildElement("Entity");
			while (pEntityElement) {
				cwEntity* pEntity = pEntityParser->parse(pEntityElement);
				if (pEntity) {
					pStage->addStageEntity(pEntity);
				}

				pEntityElement = pEntityElement->NextSiblingElement("Entity");
			}
		}
	}

	//add default stage layer
	if (pStage->getStageLayerCount() == 0) {
		cwStageLayer* pStageLayer = new cwStageLayer();
		pStage->addStageLayer(pStageLayer);
	}

	pStage->setCamera(cwRepertory::getInstance().getEngine()->getCamera(pStage->getCameraName()));
}

NS_MINIR_END
