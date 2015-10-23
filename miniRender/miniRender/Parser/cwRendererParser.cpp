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

#include "cwRendererParser.h"
#include "cwStageParser.h"
#include "cwParserManager.h"
#include "Render/Stage/cwStage.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "tinyxml2.h"

NS_MINIR_BEGIN

cwRendererParser* cwRendererParser::create()
{
	cwRendererParser* pParser = new cwRendererParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwRenderer* cwRendererParser::parse(const CWSTRING& strFileName)
{
	cwStageParser* pStageParser = static_cast<cwStageParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserStage));
	if (!pStageParser) return nullptr;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(strFileName.c_str());
	if (err != tinyxml2::XML_NO_ERROR) {
		return nullptr;
	}

	tinyxml2::XMLElement* pRendererElement = doc.FirstChildElement("Renderer");
	if (!pRendererElement) return nullptr;

	cwRenderer* pRenderer = cwRenderer::create();
	if (!pRenderer) return nullptr;

	tinyxml2::XMLElement* pStageListNode = pRendererElement->FirstChildElement("StageList");
	if (pStageListNode) {
		tinyxml2::XMLElement* pStageNode = pStageListNode->FirstChildElement("Stage");
		while (pStageNode) {
			cwStage* pStage = pStageParser->parse(pStageNode);
			if (pStage) {
				pRenderer->addStage(pStage);
			}

			pStageNode = pStageNode->NextSiblingElement("Stage");
		}
	}

	tinyxml2::XMLElement* pViewPortNode = pRendererElement->FirstChildElement("ViewPort");
	parseViewPort(pRenderer, pViewPortNode);

	tinyxml2::XMLElement* pCameraNode = pRendererElement->FirstChildElement("Camera");
	parseCamera(pRenderer, pCameraNode);

	pRenderer->setFullPath(strFileName);

	return pRenderer;
}

CWVOID cwRendererParser::parseViewPort(cwRenderer* pRenderer, tinyxml2::XMLElement* pViewPortData)
{
	if (!pRenderer || !pViewPortData) return;

	CWFLOAT fTopLeftX = pViewPortData->FloatAttribute("TopLeftX");
	CWFLOAT fTopLeftY = pViewPortData->FloatAttribute("TopLeftY");
	CWFLOAT fWidth = pViewPortData->FloatAttribute("Width");
	CWFLOAT fHeight = pViewPortData->FloatAttribute("Height");
	CWFLOAT fMinDepth = pViewPortData->FloatAttribute("MinDepth");
	CWFLOAT fMaxDepth = pViewPortData->FloatAttribute("MaxDepth");

	pRenderer->createViewPort(fTopLeftX, fTopLeftY, fWidth, fHeight, fMinDepth, fMaxDepth);
}

CWVOID cwRendererParser::parseCamera(cwRenderer* pRenderer, tinyxml2::XMLElement* pCameraData)
{
	if (!pRenderer || !pCameraData) return;

	const CWCHAR* pcCameraName = pCameraData->Attribute("Name");
	if (pcCameraName)
		pRenderer->setRendererCamera(cwRepertory::getInstance().getEngine()->getCamera(pcCameraName));
	else
		pRenderer->setRendererCamera(cwRepertory::getInstance().getEngine()->getCamera("Default"));
}

CWVOID cwRendererParser::deferParse(cwRenderer* pRenderer)
{
	if (!pRenderer) return;

	const CWSTRING& strFullPath = pRenderer->getFullPath();
	if (strFullPath.empty()) return;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(strFullPath.c_str());
	if (err != tinyxml2::XML_NO_ERROR) {
		return;
	}

	tinyxml2::XMLElement* pRendererElement = doc.FirstChildElement("Renderer");
	if (!pRendererElement) return;

	cwStageParser* pStageParser = static_cast<cwStageParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserStage));
	if (!pStageParser) return;

	tinyxml2::XMLElement* pStageListElement = pRendererElement->FirstChildElement("StageList");
	if (pStageListElement) {
		tinyxml2::XMLElement* pStageElement = pStageListElement->FirstChildElement("Stage");

		while (pStageElement) {
			const CWCHAR* pcStageName = pStageElement->Attribute("Name");

			cwStage* pStage = pRenderer->getStage(pcStageName);
			if (pStage) {
				pStageParser->deferParse(pStage, pStageElement);
			}

			pStageElement = pStageElement->NextSiblingElement("Stage");
		}
	}
}

NS_MINIR_END
