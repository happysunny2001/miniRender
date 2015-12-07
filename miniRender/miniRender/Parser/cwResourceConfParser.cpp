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

#include "cwResourceConfParser.h"
#include "Base/cwStringConvert.h"

NS_MINIR_BEGIN

cwResourceConfParser* cwResourceConfParser::create()
{
	cwResourceConfParser* pParser = new cwResourceConfParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwResourceConfParser::cwResourceConfParser()
{

}

std::unordered_map<CWSTRING, std::vector<CWSTRING>> cwResourceConfParser::parse(const CWSTRING& strFileName)
{
	std::unordered_map<CWSTRING, std::vector<CWSTRING>> mapRet;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(strFileName.c_str());

	if (err != tinyxml2::XML_NO_ERROR) {
		return mapRet;
	}

	tinyxml2::XMLElement* pResourceElement = doc.FirstChildElement("Resource");
	if (!pResourceElement) return mapRet;

	tinyxml2::XMLElement* pChildElement = pResourceElement->FirstChildElement();
	while (pChildElement) {
		const char* pcText = pChildElement->GetText();
		if (pcText) {
			std::vector<CWSTRING> vecTexturePath;
			CWSTRING strPaths = pcText;
			cwStringConvert::split(strPaths, "\n", vecTexturePath);

			std::vector<CWSTRING> vecTexturePathNode;
			for (auto it = vecTexturePath.begin(); it != vecTexturePath.end(); ++it) {
				cwStringConvert::trim(*it, '\t');
				cwStringConvert::trim(*it, '\r');

				if (!it->empty())
					vecTexturePathNode.push_back(*it);
			}

			mapRet.insert(std::make_pair(pChildElement->Name(), vecTexturePathNode));
		}

		pChildElement = pChildElement->NextSiblingElement();
	}

	//tinyxml2::XMLElement* pTextureElement = pResourceElement->FirstChildElement("Texture");
	//if (pTextureElement) {
	//	const char* pcText = pTextureElement->GetText();
	//	if (pcText) {
	//		std::vector<CWSTRING> vecTexturePath;
	//		CWSTRING strPaths = pcText;
	//		cwStringConvert::split(strPaths, "\n", vecTexturePath);

	//		std::vector<CWSTRING> vecTexturePathNode;
	//		for (auto it = vecTexturePath.begin(); it != vecTexturePath.end(); ++it) {
	//			cwStringConvert::trim(*it, '\t');
	//			cwStringConvert::trim(*it, '\r');

	//			if (!it->empty())
	//				vecTexturePathNode.push_back(*it);
	//		}

	//		mapRet.insert(std::make_pair("Texture", vecTexturePathNode));
	//	}
	//}

	//tinyxml2::XMLElement* pShaderElement = pResourceElement->FirstChildElement("Shader");
	//if (pShaderElement) {
	//	const char* pcText = pShaderElement->GetText();
	//	if (pcText) {
	//		std::vector<CWSTRING> vecShaderPath;
	//		CWSTRING strPaths = pcText;
	//		cwStringConvert::split(strPaths, "\n", vecShaderPath);

	//		std::vector<CWSTRING> vecShaderPathNode;
	//		for (auto it = vecShaderPath.begin(); it != vecShaderPath.end(); ++it) {
	//			cwStringConvert::trim(*it, '\t');
	//			cwStringConvert::trim(*it, '\r');

	//			if (!it->empty())
	//				vecShaderPathNode.push_back(*it);
	//		}

	//		mapRet.insert(std::make_pair("Shader", vecShaderPathNode));
	//	}
	//}

	return mapRet;
}

NS_MINIR_END
