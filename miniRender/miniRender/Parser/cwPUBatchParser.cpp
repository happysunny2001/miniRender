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

#include "cwPUBatchParser.h"
#include "Base/cwStringConvert.h"
#include "Math/cwMatrix4X4.h"
#include "Render/ProcessingUnit/Batch/cwPUBatch.h"
#include "Render/ProcessingUnit/Batch/cwPUBatchShadowMap.h"
#include "Render/ProcessingUnit/Batch/ShaderParam/cwPUBatchShaderParam.h"
#include "Render/ProcessingUnit/Batch/ShaderParam/cwPUBatchShaderParamFloat.h"
#include "Render/ProcessingUnit/Batch/ShaderParam/cwPUBatchShaderParamFloatArray.h"
#include "Render/ProcessingUnit/Batch/ShaderParam/cwPUBatchShaderParamMatrix.h"
#include "Render/ProcessingUnit/Batch/ShaderParam/cwPUBatchShaderParamTexture.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"
#include "cwParserManager.h"

NS_MINIR_BEGIN

cwPUBatchParser* cwPUBatchParser::create()
{
	cwPUBatchParser* pParser = new cwPUBatchParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwPUBatchParser::cwPUBatchParser()
{
	m_nMapParser["ShadowMap"]   = CW_CALLBACK_1(cwPUBatchParser::parsePUShadowMap, this);
	m_nMapParser["ShaderParam"] = CW_CALLBACK_1(cwPUBatchParser::parsePUShaderParam, this);
}

cwVector<cwPUBatch*> cwPUBatchParser::parse(tinyxml2::XMLElement* pPUElement)
{
	cwVector<cwPUBatch*> vecPU;
	if (!pPUElement) return vecPU;

	tinyxml2::XMLElement* pChildElement = pPUElement->FirstChildElement();
	while (pChildElement) {
		if (m_nMapParser.find(pChildElement->Name()) != m_nMapParser.end()) {
			cwPUBatch* pPU = m_nMapParser[pChildElement->Name()](pChildElement);
			if (pPU) {
				const char* pcPUName = pChildElement->Attribute("Name");
				if (pcPUName) {
					pPU->setName(pcPUName);
				}

				vecPU.pushBack(pPU);
			}
		}

		pChildElement = pChildElement->NextSiblingElement();
	}

	return vecPU;
}

cwPUBatchShadowMap* cwPUBatchParser::parsePUShadowMap(tinyxml2::XMLElement* pShadowMapElement)
{
	if (!pShadowMapElement) return nullptr;

	const char* pcStageName = pShadowMapElement->Attribute("StageName");
	if (!pcStageName) return nullptr;
	
	cwStage* pShadowStage = cwRepertory::getInstance().getEngine()->getRenderer()->getStage(pcStageName);
	if (!pShadowStage) return nullptr;

	cwPUBatchShadowMap* pPUBatchShadowMap = cwPUBatchShadowMap::create();
	if (!pPUBatchShadowMap) return nullptr;
	pPUBatchShadowMap->bindingStage(pShadowStage);

	tinyxml2::XMLElement* pChildElement = pShadowMapElement->FirstChildElement();
	while (pChildElement) {
		const char* pcElementName = pChildElement->Name();
		if (CW_STRING_MATCH(pcElementName, "Param")) {
			const char* pcType = pChildElement->Attribute("Type");
			const char* pcName = pChildElement->Attribute("Name");

			if (pcType && pcName) {
				if (CW_STRING_MATCH(pcType, "Texture")) {
					pPUBatchShadowMap->setTextureParameterName(pcName);
				}
				else if (CW_STRING_MATCH(pcType, "Transform")) {
					pPUBatchShadowMap->setTransParameterName(pcName);
				}
			}
		}

		pChildElement = pChildElement->NextSiblingElement();
	}

	return pPUBatchShadowMap;
}

cwPUBatchShaderParam* cwPUBatchParser::parsePUShaderParam(tinyxml2::XMLElement* pParamElement)
{
	if (!pParamElement) return nullptr;

	cwPUBatchShaderParam* pPUBatch = nullptr;
	const char* pcType = pParamElement->Attribute("Type");
	if (!pcType) return nullptr;

	cwParserManager* pParseManager = cwRepertory::getInstance().getParserManager();
	eShaderParamType eType = pParseManager->getShaderParamType(pcType);
	switch (eType)
	{
	case eShaderParamTypeFloat:
		pPUBatch = this->parsePUShaderParamFloat(pParamElement);
		break;
	case eShaderParamTypeFloatArray:
		pPUBatch = this->parsePUShaderParamFloatArray(pParamElement);
		break;
	case eShaderParamTypeMatrix:
		pPUBatch = this->parsePUShaderParamMatrix(pParamElement);
		break;
	case eShaderParamTypeTexture:
		pPUBatch = this->parsePUShaderParamTexture(pParamElement);
		break;
	default:
		break;
	}

	if (!pPUBatch) return nullptr;

	const char* pcParamName = pParamElement->Attribute("Name");
	if (!pcParamName) return nullptr;
	pPUBatch->setShaderParamName(pcParamName);

	const char* pcSource = pParamElement->Attribute("Source");
	eShaderParamSource eSource = pParseManager->getShaderParamSource(pcSource);
	pPUBatch->setSourceType(eSource);

	const char* pcFreq = pParamElement->Attribute("Freq");
	eShaderParamFreq eFreq = pParseManager->getShaderParamFreq(pcFreq);
	pPUBatch->setFreq(eFreq);

	return pPUBatch;
}

cwPUBatchShaderParam* cwPUBatchParser::parsePUShaderParamFloat(tinyxml2::XMLElement* pParamElement)
{
	tinyxml2::XMLElement* pDataElement = pParamElement->FirstChildElement("Data");
	if (!pDataElement) return nullptr;

	const char* pcText = pDataElement->GetText();
	if (!pcText) return nullptr;

	cwPUBatchShaderParamFloat* pPUBatch = cwPUBatchShaderParamFloat::create();
	if (!pPUBatch) return nullptr;

	pPUBatch->setValue(static_cast<CWFLOAT>(atof(pcText)));
	return pPUBatch;
}

cwPUBatchShaderParam* cwPUBatchParser::parsePUShaderParamFloatArray(tinyxml2::XMLElement* pParamElement)
{
	const char* pcSource = pParamElement->Attribute("Source");
	if (!pcSource) return nullptr;
	eShaderParamSource eSource = cwRepertory::getInstance().getParserManager()->getShaderParamSource(pcSource);

	if (eSource == eShaderParamSourceData) {
		tinyxml2::XMLElement* pDataElement = pParamElement->FirstChildElement("Data");
		if (!pDataElement) return nullptr;

		const char* pcText = pDataElement->GetText();
		if (!pcText) return nullptr;

		CWSTRING strData(pcText);
		strData = cwStringConvert::replace(strData, "\n", "");
		std::vector<CWSTRING> vecFloat;
		cwStringConvert::split(strData, ";", vecFloat);
		if (vecFloat.empty()) return nullptr;

		cwPUBatchShaderParamFloatArray* pPUBatch = cwPUBatchShaderParamFloatArray::create();
		if (!pPUBatch) return nullptr;

		CWFLOAT* fFloatArray = new CWFLOAT[vecFloat.size()];
		if (fFloatArray) {
			CWUINT i = 0;
			for (auto pStr : vecFloat) {
				fFloatArray[i++] = CWFLOAT(atof(pStr.c_str()));
			}

			pPUBatch->setValues(fFloatArray, static_cast<CWUINT>(vecFloat.size()));
			CW_SAFE_DELETE_ARRAY(fFloatArray);
		}

		return pPUBatch;
	}
	else {
		tinyxml2::XMLElement* pEngineElement = pParamElement->FirstChildElement("Engine");
		if (!pEngineElement) return nullptr;

		const char* pcType = pEngineElement->Attribute("Type");
		const char* pcName = pEngineElement->Attribute("Name");
		const char* pcAttr = pEngineElement->Attribute("Attr");

		if (!pcType || !pcName || !pcAttr) return nullptr;

		cwPUBatchShaderParamFloatArray* pPUBatch = cwPUBatchShaderParamFloatArray::create();
		if (!pPUBatch) return nullptr;

		if (pPUBatch) {
			pPUBatch->setEngineParamType(pcType);
			pPUBatch->setEngineParamName(pcName);
			pPUBatch->setEngineParamAttr(pcAttr);

			return pPUBatch;
		}
	}

	return nullptr;
}

cwPUBatchShaderParam* cwPUBatchParser::parsePUShaderParamMatrix(tinyxml2::XMLElement* pParamElement)
{
	const char* pcSource = pParamElement->Attribute("Source");
	if (!pcSource) return nullptr;
	eShaderParamSource eSource = cwRepertory::getInstance().getParserManager()->getShaderParamSource(pcSource);

	if (eSource == eShaderParamSourceData) {
		tinyxml2::XMLElement* pDataElement = pParamElement->FirstChildElement("Data");
		if (!pDataElement) return nullptr;
		const char* pcData = pDataElement->GetText();
		if (!pcData) return nullptr;

		cwMatrix4X4 mat = cwRepertory::getInstance().getParserManager()->parseMatrixFromString(pcData);

		cwPUBatchShaderParamMatrix* pPUBatch = cwPUBatchShaderParamMatrix::create();
		if (pPUBatch) {
			pPUBatch->setMatrixData(mat);
			return pPUBatch;
		}
	}
	else {
		tinyxml2::XMLElement* pEngineElement = pParamElement->FirstChildElement("Engine");
		if (!pEngineElement) return nullptr;

		const char* pcType = pEngineElement->Attribute("Type");
		const char* pcName = pEngineElement->Attribute("Name");
		const char* pcAttr = pEngineElement->Attribute("Attr");

		if (!pcType || !pcName || !pcAttr) return nullptr;

		cwPUBatchShaderParamMatrix* pPUBatch = cwPUBatchShaderParamMatrix::create();

		if (pPUBatch) {
			pPUBatch->setEngineParamType(pcType);
			pPUBatch->setEngineParamName(pcName);
			pPUBatch->setEngineParamAttr(pcAttr);

			return pPUBatch;
		}
	}

	return nullptr;
}

cwPUBatchShaderParam* cwPUBatchParser::parsePUShaderParamTexture(tinyxml2::XMLElement* pParamElement)
{
	const char* pcSource = pParamElement->Attribute("Source");
	if (!pcSource) return nullptr;
	eShaderParamSource eSource = cwRepertory::getInstance().getParserManager()->getShaderParamSource(pcSource);

	if (eSource == eShaderParamSourceData) {
		tinyxml2::XMLElement* pDataElement = pParamElement->FirstChildElement("Data");
		if (!pDataElement) return nullptr;
		const char* pcData = pDataElement->GetText();
		if (!pcData) return nullptr;

		cwPUBatchShaderParamTexture* pPUBatch = cwPUBatchShaderParamTexture::create();
		if (pPUBatch) {
			pPUBatch->setTextureName(pcData);
			return pPUBatch;
		}
	}
	else {
		tinyxml2::XMLElement* pEngineElement = pParamElement->FirstChildElement("Engine");
		if (!pEngineElement) return nullptr;

		const char* pcType = pEngineElement->Attribute("Type");
		const char* pcName = pEngineElement->Attribute("Name");
		const char* pcAttr = pEngineElement->Attribute("Attr");

		if (!pcType || !pcName || !pcAttr) return nullptr;

		cwPUBatchShaderParamTexture* pPUBatch = cwPUBatchShaderParamTexture::create();

		if (pPUBatch) {
			pPUBatch->setEngineParamType(pcType);
			pPUBatch->setEngineParamName(pcName);
			pPUBatch->setEngineParamAttr(pcAttr);

			return pPUBatch;
		}
	}

	return nullptr;
}

NS_MINIR_END
