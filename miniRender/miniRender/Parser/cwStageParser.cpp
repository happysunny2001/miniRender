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
#include "cwTextureParser.h"
#include "cwRenderTargetParser.h"
#include "Render/Stage/cwStage.h"
#include "Render/Stage/cwSpriteStage.h"
#include "Render/Stage/cwPrimitiveStage.h"
#include "Render/Stage/cwSkyDomeStage.h"
#include "Render/Stage/Layer/cwStageLayer.h"
#include "Render/cwRenderer.h"
#include "Device/cwDevice.h"
#include "Repertory/cwRepertory.h"
#include "ViewPort/cwViewPort.h"
#include "Texture/cwRenderTexture.h"
#include "Texture/cwTextureManager.h"
#include "Engine/cwEngine.h"
#include "Render/Generator/cwRenderGenerator.h"
#include "Render/Stage/cwReflectionStage.h"
#include "Render/Stage/Shadow/cwShadowMapStage.h"

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
	m_nMapParser["RenderTarget"] = CW_CALLBACK_2(cwStageParser::parseRenderTarget, this);
	m_nMapParser["Layer"]        = CW_CALLBACK_2(cwStageParser::parseLayer, this);
}

cwStage* cwStageParser::createStage(tinyxml2::XMLElement* pStageData)
{
	if (!pStageData) return cwStage::create();

	const CWCHAR* pcType = pStageData->Attribute("Type");
	if (pcType) {
		if (strncmp(pcType, "Normal", 6) == 0) {
			return cwStage::create();
		}
		else if (strncmp(pcType, "Sprite", 8) == 0) {
			return cwSpriteStage::create();
		}
		else if (strncmp(pcType, "Primitive", 9) == 0) {
			return cwPrimitiveStage::create();
		}
		else if (strncmp(pcType, "SkyDome", 7) == 0) {
			return cwSkyDomeStage::create();
		}
		else if (strncmp(pcType, "Reflection", 10) == 0) {
			return cwReflectionStage::create();
		}
		else if (CW_STRING_MATCH(pcType, "ShadowMap")) {
			return cwShadowMapStage::create();
		}
	}

	return nullptr;
}

cwStage* cwStageParser::parse(tinyxml2::XMLElement* pStageData)
{
	if (!pStageData) return nullptr;

	cwStage* pStage = createStage(pStageData);
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

	//const CWCHAR* pcType = pStageData->Attribute("Type");
	//if (pcType) {
	//	CWSTRING strType = pcType;
	//	if (strncmp(pcType, "normal", 6) == 0) {
	//		pStage->setType(eStageTypeNormal);
	//	}
	//	else if (strncmp(pcType, "specific", 8) == 0) {
	//		pStage->setType(eStageTypeSpecific);
	//	}
	//}

	const char* pcEnable = pStageData->Attribute("Enable");
	pStage->setEnable(cwRepertory::getInstance().getParserManager()->getBool(pcEnable));

	const char* pcGenerator = pStageData->Attribute("Generator");
	if (pcGenerator) {
		cwRenderGenerator* pGenerator = cwRenderGenerator::createRenderGenerator(pcGenerator);
		if (pGenerator) {
			pStage->addRenderGenerator(pGenerator);
		}
	}
}

CWVOID cwStageParser::parseCamera(cwStage* pStage, tinyxml2::XMLElement* pCameraData)
{
	const CWCHAR* pcCameraName = pCameraData->Attribute("Name");
	if (pcCameraName)
		pStage->setCameraName(pcCameraName);
}

CWVOID cwStageParser::parseRenderTarget(cwStage* pStage, tinyxml2::XMLElement* pRenderTarget)
{
	if (!pRenderTarget || !pStage) return;

	const CWCHAR* pcType = pRenderTarget->Attribute("Type");
	if (!pcType) return;

	cwParserManager* pParserManager = cwRepertory::getInstance().getParserManager();
	cwRenderTargetParser* pParser = static_cast<cwRenderTargetParser*>(pParserManager->getParser(eParserRenderTarget));
	cwRenderTexture* pRenderTexture = pParser->parse(pRenderTarget);

	if (pRenderTexture) {
		pStage->setRenderTexture(pRenderTexture);
	}

	const char* pcIsClearColor = pRenderTarget->Attribute("IsClearColor");
	if (pcIsClearColor)
		pStage->setIsClearColor(pParserManager->getBool(pcIsClearColor));

	const char* pcIsClearDepth = pRenderTarget->Attribute("IsClearDepth");
	if (pcIsClearDepth)
		pStage->setIsClearDepth(pParserManager->getBool(pcIsClearDepth));

	const char* pcIsClearStencil = pRenderTarget->Attribute("IsClearStencil");
	if (pcIsClearStencil)
		pStage->setIsClearStencil(pParserManager->getBool(pcIsClearStencil));

	pStage->setRefreshRenderTarget(CWTRUE);
}

CWVOID cwStageParser::parseLayer(cwStage* pStage, tinyxml2::XMLElement* pLayerData)
{
	cwStageLayerParser* pStageLayerParser = static_cast<cwStageLayerParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserStageLayer));
	if (!pStageLayerParser) return;

	cwStageLayer* pStageLayer = pStageLayerParser->parse(pLayerData);
	if (pStageLayer) {
		pStage->addStageLayer(pStageLayer);
		//m_nMapStageLayer[pStageLayer] = pLayerData;
	}
}

CWVOID cwStageParser::deferParse(cwStage* pStage, tinyxml2::XMLElement* pStageElement)
{
	if (!pStageElement || !pStage) return;

	//parse texture
	parseTextureList(pStage, pStageElement);

	//parse stage entity
	parseEntityList(pStage, pStageElement);

	//add default stage layer
	if (pStage->getStageLayerCount() == 0) {
		cwStageLayer* pStageLayer = cwStageLayer::create();
		pStage->addStageLayer(pStageLayer);
	}
	else {
		cwStageLayerParser* pStageLayerParser = static_cast<cwStageLayerParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserStageLayer));
		if (!pStageLayerParser) return;

		tinyxml2::XMLElement* pStageLayerElement = pStageElement->FirstChildElement("Layer");
		while (pStageLayerElement) {
			const char* pcName = pStageLayerElement->Attribute("Name");
			if (pcName && strlen(pcName) > 0) {
				cwStageLayer* pStageLayer = pStage->getStageLayer(pcName);
				if (pStageLayer) {
					pStageLayerParser->deferParse(pStageLayer, pStageLayerElement);
				}
			}
			pStageLayerElement = pStageLayerElement->NextSiblingElement("Layer");
		}
	}

	const CWSTRING& strCameraName = pStage->getCameraName();
	if (strCameraName.empty())
		pStage->setCamera(cwRepertory::getInstance().getEngine()->getRenderer()->getRendererCamera());
	else
		pStage->setCamera(cwRepertory::getInstance().getEngine()->getCamera(pStage->getCameraName()));

	if (pStage->getType() == eStageTypeReflection) {
		parseReflectionStage(static_cast<cwReflectionStage*>(pStage), pStageElement);
	}

	//m_nMapStageLayer.clear();
}

CWVOID cwStageParser::parseEntityList(cwStage* pStage, tinyxml2::XMLElement* pStageElement)
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
}

CWVOID cwStageParser::parseTextureList(cwStage* pStage, tinyxml2::XMLElement* pStageElement)
{
	if (!pStageElement || !pStage) return;
	tinyxml2::XMLElement* pTextureListElement = pStageElement->FirstChildElement("TextureList");
	if (!pTextureListElement) return;

	cwTextureParser* pTextureParser = static_cast<cwTextureParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserTexture));
	if (!pTextureParser) return;

	tinyxml2::XMLElement* pTextureElement = pTextureListElement->FirstChildElement("Texture");
	while (pTextureElement) {
		const char* pcName = pTextureElement->Attribute("Name");
		if (pcName && strlen(pcName) > 0) {
			cwTexture* pTexture = pTextureParser->parse(pTextureElement);
			if (pTexture) {
				pTexture->setName(pcName);
				pStage->addStageTexture(pcName, pTexture);
			}
		}

		pTextureElement = pTextureElement->NextSiblingElement("Texture");
	}
}

CWVOID cwStageParser::parseReflectionStage(cwReflectionStage* pStage, tinyxml2::XMLElement* pStageElement)
{
	if (!pStageElement || !pStage) return;

	cwParserManager* pParserManager = cwRepertory::getInstance().getParserManager();

	tinyxml2::XMLElement* pStageListElement = pStageElement->FirstChildElement("StageList");
	if (!pStageListElement) return;

	tinyxml2::XMLElement* pStageDataElement = pStageListElement->FirstChildElement("StageReference");
	while (pStageDataElement) {
		const char* pcName = pStageDataElement->Attribute("Name");
		const char* pcReplaceRT = pStageDataElement->Attribute("ReplaceRenderTarget");

		cwStage* pOtherStage = cwRepertory::getInstance().getEngine()->getRenderer()->getStage(pcName);
		if (pOtherStage) {
			pStage->addStage(pOtherStage, pParserManager->getBool(pcReplaceRT));
		}

		pStageDataElement = pStageDataElement->NextSiblingElement("StageReference");
	}
}

NS_MINIR_END
