﻿/*
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
#include "Render/cwStage.h"
#include "Device/cwDevice.h"
#include "Repertory/cwRepertory.h"
#include "ViewPort/cwViewPort.h"
#include "Texture/cwRenderTexture.h"
#include "Texture/cwTextureManager.h"

NS_MINIR_BEGIN

cwStage* cwStageParser::parse(tinyxml2::XMLElement* pStageData)
{
	if (!pStageData) return nullptr;

	cwStage* pStage = new cwStage();
	if (!pStage) return nullptr;

	parseAttribute(pStage, pStageData);
	parseCamera(pStage, pStageData);
	parseViewPort(pStage, pStageData);
	parseRenderTarget(pStage, pStageData);

	return pStage;
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
	

	const CWCHAR* pcEnable = pStageData->Attribute("Enable");
	if (pcEnable) {
		if (strncmp(pcEnable, "true", 4) == 0) {
			pStage->setEnable(true);
		}
		else if (strncmp(pcEnable, "false", 5) == 0) {
			pStage->setEnable(false);
		}
	}
}

CWVOID cwStageParser::parseCamera(cwStage* pStage, tinyxml2::XMLElement* pStageData)
{
	tinyxml2::XMLElement* pCameraData = pStageData->FirstChildElement("Camera");
	if (!pCameraData) return;

	const CWCHAR* pcCameraName = pCameraData->Attribute("Name");
	if (pcCameraName && strncmp(pcCameraName, "default", 7) != 0) {
		//set camera object
	}
}

CWVOID cwStageParser::parseViewPort(cwStage* pStage, tinyxml2::XMLElement* pStageData)
{
	tinyxml2::XMLElement* pViewPortData = pStageData->FirstChildElement("ViewPort");
	if (!pViewPortData) return;

	const CWCHAR* pcViewPortName = pViewPortData->Attribute("Name");
	if (pcViewPortName && strncmp(pcViewPortName, "default", 7) == 0) return;

	CWFLOAT fTopLeftX = pViewPortData->FloatAttribute("TopLeftX");
	CWFLOAT fTopLeftY = pViewPortData->FloatAttribute("TopLeftY");
	CWFLOAT fWidth  = pViewPortData->FloatAttribute("Width");
	CWFLOAT fHeight = pViewPortData->FloatAttribute("Height");
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

CWVOID cwStageParser::parseRenderTarget(cwStage* pStage, tinyxml2::XMLElement* pStageData)
{
	tinyxml2::XMLElement* pRenderTarget = pStageData->FirstChildElement("RenderTarget");
	if (!pRenderTarget) return;

	const CWCHAR* pcType = pRenderTarget->Attribute("type");
	if (!pcType) return;

	if (strncmp(pcType, "backbuffer", 10) == 0)
		return;
	else if (strncmp(pcType, "texture", 7) == 0) {
		CWFLOAT fWidth = pRenderTarget->FloatAttribute("Width");
		CWFLOAT fHeight = pRenderTarget->FloatAttribute("Height");

		cwRenderTexture* pRenderTexture = cwRepertory::getInstance().getTextureManager()->createRenderTexture(fWidth, fHeight);
		if (pRenderTexture) {
			pStage->setRenderTexture(pRenderTexture);
		}
	}
}

NS_MINIR_END
