/*
Copyright © 2015-2016 Ziwei Wang

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

#include "cwRenderTargetParser.h"
#include "Texture/cwTextureManager.h"
#include "Repertory/cwRepertory.h"
#include "cwParserManager.h"

NS_MINIR_BEGIN

cwRenderTargetParser* cwRenderTargetParser::create()
{
	cwRenderTargetParser* pParser = new cwRenderTargetParser();
	if (pParser) {
		pParser->autorelease();
	}

	return pParser;
}

cwRenderTargetParser::cwRenderTargetParser()
{

}

cwRenderTexture* cwRenderTargetParser::parse(tinyxml2::XMLElement* pDomRenderTarget)
{
	if (!pDomRenderTarget) return nullptr;

	const char* pcType = pDomRenderTarget->Attribute("Type");
	if (!pcType) return nullptr;

	if (CW_STRING_MATCH(pcType, "backbuffer")) {
		return this->parseBackbuffer(pDomRenderTarget);
	}
	else if (CW_STRING_MATCH(pcType, "texture")) {
		return this->parseTexture(pDomRenderTarget);
	}
	else if (CW_STRING_MATCH(pcType, "ShadowMap")) {
		return this->parseShadowMap(pDomRenderTarget);
	}
	else if (CW_STRING_MATCH(pcType, "Multi")) {
		return this->parseMultiRenderTarget(pDomRenderTarget);
	}

	return nullptr;
}

cwRenderTexture* cwRenderTargetParser::parseBackbuffer(tinyxml2::XMLElement* pDomRenderTarget)
{
	return nullptr;
}

cwRenderTexture* cwRenderTargetParser::parseTexture(tinyxml2::XMLElement* pDomRenderTarget)
{
	cwRenderTexture* pRenderTexture = nullptr;
	cwTextureManager* pTextureManager = cwRepertory::getInstance().getTextureManager();

	CWFLOAT fWidth = pDomRenderTarget->FloatAttribute("Width");
	CWFLOAT fHeight = pDomRenderTarget->FloatAttribute("Height");
	const char* pcWritable = pDomRenderTarget->Attribute("Writable");

	if (pcWritable) {
		if (cwRepertory::getInstance().getParserManager()->getBool(pcWritable))
			pRenderTexture = pTextureManager->createRenderTexture(fWidth, fHeight, eRenderTextureWritable);
		else
			pRenderTexture = pTextureManager->createRenderTexture(fWidth, fHeight);
	}
	else {
		pRenderTexture = pTextureManager->createRenderTexture(fWidth, fHeight);
	}

	return pRenderTexture;
}

cwRenderTexture* cwRenderTargetParser::parseShadowMap(tinyxml2::XMLElement* pDomRenderTarget)
{
	cwTextureManager* pTextureManager = cwRepertory::getInstance().getTextureManager();

	CWFLOAT fWidth = pDomRenderTarget->FloatAttribute("Width");
	CWFLOAT fHeight = pDomRenderTarget->FloatAttribute("Height");

	return pTextureManager->createRenderTexture(fWidth, fHeight, eRenderTextureShadowMap);
}

cwRenderTexture* cwRenderTargetParser::parseMultiRenderTarget(tinyxml2::XMLElement* pDomRenderTarget)
{
	cwTextureManager* pTextureManager = cwRepertory::getInstance().getTextureManager();
	cwParserManager* pParserManager = cwRepertory::getInstance().getParserManager();

	CWFLOAT fWidth = pDomRenderTarget->FloatAttribute("Width");
	CWFLOAT fHeight = pDomRenderTarget->FloatAttribute("Height");

	cwRenderTexture* pRenderTexture = pTextureManager->createRenderTexture(fWidth, fHeight, eRenderTextureMultiTarget);
	if (!pRenderTexture) return nullptr;

	const char* pcDepthResource = pDomRenderTarget->Attribute("DepthResource");
	if (pcDepthResource) {
		CWBOOL bDepthRes = pParserManager->getBool(pcDepthResource);
		if (bDepthRes)
			pRenderTexture->enableDepthBufferShaderResource();
	}

	CWUINT iGBufCnt = 0;
	tinyxml2::XMLElement* pGBufElement = pDomRenderTarget->FirstChildElement("G-Buffer");
	while (pGBufElement){
		iGBufCnt++;
		pGBufElement = pGBufElement->NextSiblingElement("G-Buffer");
	}

	if (iGBufCnt == 0) return nullptr;

	pRenderTexture->setRenderTargetCount(iGBufCnt);

	CWUINT index = 0;
	pGBufElement = pDomRenderTarget->FirstChildElement("G-Buffer");
	while (pGBufElement){
		const char* pcFormat = pGBufElement->Attribute("Format");
		eFormat format = pParserManager->getFormatType(pcFormat);
		if (format == eFormatUnknown) continue;

		pRenderTexture->setRenderTarget(format, index);
		index++;

		pGBufElement = pGBufElement->NextSiblingElement("G-Buffer");
	}

	return pRenderTexture;
}

NS_MINIR_END
