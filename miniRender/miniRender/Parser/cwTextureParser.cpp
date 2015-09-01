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

#include "cwTextureParser.h"
#include "Base/cwStringConvert.h"
#include "Repertory/cwRepertory.h"
#include "Texture/cwTextureManager.h"
#include "Texture/cwRenderTexture.h"
#include "Render/cwRenderer.h"
#include "Render/cwStage.h"
#include "Engine/cwEngine.h"
#include "Parser/cwParserManager.h"

NS_MINIR_BEGIN

cwTextureParser* cwTextureParser::create()
{
	cwTextureParser* pParser = new cwTextureParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwTextureParser::cwTextureParser()
{

}

cwTextureParser::~cwTextureParser()
{

}

cwTexture* cwTextureParser::parse(tinyxml2::XMLElement* pTextureData)
{
	if (!pTextureData) return nullptr;

	const char* pcType = pTextureData->Attribute("Type");
	if (!pcType) return nullptr;

	cwTexture* pTexture = nullptr;

	if (strncmp(pcType, "File", 4) == 0) {
		pTexture = parseTextureFromFile(pTextureData);
	}
	else if (strncmp(pcType, "Stage", 5) == 0) {
		pTexture = parseTextureStage(pTextureData);
	}
	else if (strncmp(pcType, "StageRenderTarget", 17) == 0) {
		pTexture = parseTextureStageRenderTarget(pTextureData);
	}
	else if (strncmp(pcType, "Create", 6) == 0) {
		pTexture = parseTextureCreate(pTextureData);
	}

	return pTexture;
}

cwTexture* cwTextureParser::parseTextureFromFile(tinyxml2::XMLElement* pTextureData)
{
	if (!pTextureData) return nullptr;

	const char* pcLocation = pTextureData->Attribute("Location");
	if (pcLocation) {
		cwTexture* pTexture = cwRepertory::getInstance().getTextureManager()->getTexture(pcLocation);
		if (!pTexture) return nullptr;

		return pTexture;
	}

	return nullptr;
}

cwTexture* cwTextureParser::parseTextureStage(tinyxml2::XMLElement* pTextureData)
{
	if (!pTextureData) return nullptr;

	const char* pcLocation = pTextureData->Attribute("Location");
	if (!pcLocation) return nullptr;

	CWSTRING strLocation(pcLocation);
	std::vector<CWSTRING> vecPath;
	cwStringConvert::split(strLocation, "/", vecPath);
	if (vecPath.size() < 2) return nullptr;

	cwStage* pOtherStage = cwRepertory::getInstance().getEngine()->getRenderer()->getStage(vecPath[0]);
	if (!pOtherStage) return nullptr;

	return pOtherStage->getStageTexture(vecPath[1]);
}

cwTexture* cwTextureParser::parseTextureStageRenderTarget(tinyxml2::XMLElement* pTextureData)
{
	if (!pTextureData) return nullptr;

	const char* pcLocation = pTextureData->Attribute("Location");
	if (pcLocation) {
		cwStage* pOtherStage = cwRepertory::getInstance().getEngine()->getRenderer()->getStage(pcLocation);
		if (pOtherStage) {
			return pOtherStage->getRenderTexture();
		}
	}

	return nullptr;
}

cwTexture* cwTextureParser::parseTextureCreate(tinyxml2::XMLElement* pTextureData)
{
	if (!pTextureData) return nullptr;

	CWFLOAT fWidth  = pTextureData->FloatAttribute("Width");
	CWFLOAT fHeight = pTextureData->FloatAttribute("Height");

	CWBOOL bWritable = CWFALSE;
	const char* pcWritable = pTextureData->Attribute("Writable");
	if (pcWritable) {
		bWritable = cwRepertory::getInstance().getParserManager()->getBool(pcWritable);
	}

	if (!bWritable) {
		return cwRepertory::getInstance().getTextureManager()->createRenderTexture(fWidth, fHeight);
	}
	else {
		return cwRepertory::getInstance().getTextureManager()->createRenderTexture(fWidth, fHeight, eRenderTextureWritable);
	}

	return nullptr;
}

NS_MINIR_END
