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

#include "cwEffectParser.h"
#include "Repertory/cwRepertory.h"
#include "Base/cwStringConvert.h"
#include "Math/cwMath.h"
#include "Shader/cwShaderManager.h"
#include "effect/cwEffectParameter.h"
#include "effect/cwEffectFloatParameter.h"
#include "effect/cwEffectFloatArrayParameter.h"
#include "effect/cwEffectMatrixParameter.h"
#include "effect/cwEffectTextureParameter.h"
#include "Render/cwRenderer.h"
#include "Render/cwStage.h"
#include "Engine/cwEngine.h"

NS_MINIR_BEGIN

cwEffectParser* cwEffectParser::create()
{
	cwEffectParser* pParser = new cwEffectParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwEffectParser::cwEffectParser()
{

}

cwEffect* cwEffectParser::parse(tinyxml2::XMLElement* pEffectElement)
{
	if (!pEffectElement) return nullptr;

	cwEffect* pEffect = cwEffect::create();
	if (!pEffect) return nullptr;

	parseAttribute(pEffect, pEffectElement);

	tinyxml2::XMLElement* pParamElement = pEffectElement->FirstChildElement("Param");
	while (pParamElement){
		const char* pcType = pParamElement->Attribute("Type");
		if (!pcType) continue;

		cwEffectParameter* pEffectParam = nullptr;
		if (strlen(pcType) == 5 && strncmp(pcType, "Float", 5) == 0) {
			pEffectParam = parseFloatParameter(pParamElement);
		}
		else if (strlen(pcType) == 10 && strncmp(pcType, "FloatArray", 10) == 0) {
			pEffectParam = parseFloatArrayParameter(pParamElement);
		}
		else if (strlen(pcType) == 7 && strncmp(pcType, "Texture", 7) == 0) {
			pEffectParam = parseTextureParameter(pParamElement);
		}
		else if (strlen(pcType) == 6 && strncmp(pcType, "Matrix", 6) == 0) {
			pEffectParam = parseMatrixParameter(pParamElement);
		}

		if (pEffectParam) {
			pEffect->addParameter(pEffectParam);
		}

		pParamElement = pParamElement->NextSiblingElement("Param");
	}

	return pEffect;
}

CWVOID cwEffectParser::parseAttribute(cwEffect* pEffect, tinyxml2::XMLElement* pEffectElement)
{
	if (!pEffectElement || !pEffect) return;

	const char* pcShaderType = pEffectElement->Attribute("ShaderType");
	const char* pcShader = pEffectElement->Attribute("Shader");
	const char* pcTech = pEffectElement->Attribute("Tech");
	const char* pcName = pEffectElement->Attribute("Name");

	if (pcName) {
		pEffect->setName(pcName);
	}

	if (strncmp(pcShaderType, "default", 7) == 0) {
		CWUINT iShaderIndex = atoi(pcShader);
		eDefShaderID eShaderID = static_cast<eDefShaderID>(iShaderIndex);
		cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eShaderID);

		if (pShader) {
			pEffect->setShader(pShader);
			pEffect->setTech(pcTech);
		}
	}
	else if (strncmp(pcShaderType, "specific", 8) == 0) {
		cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getShader(pcShader);

		if (pShader) {
			pEffect->setShader(pShader);
			pEffect->setTech(pcTech);
		}
	}
}

cwEffectParameter* cwEffectParser::parseFloatParameter(tinyxml2::XMLElement* pParameterElement)
{
	if (!pParameterElement) return nullptr;

	const char* pcName = pParameterElement->Attribute("Name");
	if (pcName) return nullptr;

	tinyxml2::XMLElement* pDataElement = pParameterElement->FirstChildElement("Data");
	if (pDataElement) {
		const char* pcData = pDataElement->GetText();
		if (pcData) {
			CWFLOAT fValue = CWFLOAT(atof(pcData));
			cwEffectFloatParameter* pEffectParameter = cwEffectFloatParameter::create();
			pEffectParameter->setParameterName(pcName);
			pEffectParameter->m_fValue = fValue;
			return pEffectParameter;
		}
	}

	return nullptr;
}

cwEffectParameter* cwEffectParser::parseFloatArrayParameter(tinyxml2::XMLElement* pParameterElement)
{
	if (!pParameterElement) return nullptr;

	const char* pcName = pParameterElement->Attribute("Name");
	if (pcName) return nullptr;

	tinyxml2::XMLElement* pDataElement = pParameterElement->FirstChildElement("Data");
	if (pDataElement) {
		const char* pcData = pDataElement->GetText();
		CWSTRING strData(pcData);
		strData = cwStringConvert::replace(strData, "\n", "");
		std::vector<CWSTRING> vecFloat;
		cwStringConvert::split(strData, ";", vecFloat);
		if (vecFloat.empty()) return nullptr;

		cwEffectFloatArrayParameter* pEffectFloatArrayParam = cwEffectFloatArrayParameter::create();
		if (!pEffectFloatArrayParam) return nullptr;

		CWFLOAT* fFloatArray = new CWFLOAT[vecFloat.size()];
		if (fFloatArray) {
			CWUINT i = 0;
			for (auto pStr : vecFloat) {
				fFloatArray[i++] = CWFLOAT(atof(pStr.c_str()));
			}
			
			pEffectFloatArrayParam->setParameterName(pcName);
			pEffectFloatArrayParam->m_pFloatArray = fFloatArray;
			pEffectFloatArrayParam->m_iCnt = CWUINT(vecFloat.size());

			return pEffectFloatArrayParam;
		}
	}

	return nullptr;
}

cwEffectParameter* cwEffectParser::parseMatrixParameter(tinyxml2::XMLElement* pParameterElement)
{
	if (!pParameterElement) return nullptr;

	const char* pcName = pParameterElement->Attribute("Name");
	if (pcName) return nullptr;

	tinyxml2::XMLElement* pDataElement = pParameterElement->FirstChildElement("Data");
	if (pDataElement) {
		const char* pcData = pDataElement->GetText();
		CWSTRING strData(pcData);
		strData = cwStringConvert::replace(strData, "\n", "");
		std::vector<CWSTRING> vecFloat;
		cwStringConvert::split(strData, ";", vecFloat);
		if (vecFloat.empty()) return nullptr;

		cwEffectMatrixParameter* pEffectMatrixParam = cwEffectMatrixParameter::create();
		if (!pEffectMatrixParam) return nullptr;

		cwMatrix4X4 mat;
		mat.m11 = CWFLOAT(atof(vecFloat[0].c_str()));
		mat.m12 = CWFLOAT(atof(vecFloat[1].c_str()));
		mat.m13 = CWFLOAT(atof(vecFloat[2].c_str()));
		mat.m14 = CWFLOAT(atof(vecFloat[3].c_str()));

		mat.m21 = CWFLOAT(atof(vecFloat[4].c_str()));
		mat.m22 = CWFLOAT(atof(vecFloat[5].c_str()));
		mat.m23 = CWFLOAT(atof(vecFloat[6].c_str()));
		mat.m24 = CWFLOAT(atof(vecFloat[7].c_str()));

		mat.m31 = CWFLOAT(atof(vecFloat[8].c_str()));
		mat.m32 = CWFLOAT(atof(vecFloat[9].c_str()));
		mat.m33 = CWFLOAT(atof(vecFloat[10].c_str()));
		mat.m34 = CWFLOAT(atof(vecFloat[11].c_str()));

		mat.m41 = CWFLOAT(atof(vecFloat[12].c_str()));
		mat.m42 = CWFLOAT(atof(vecFloat[13].c_str()));
		mat.m43 = CWFLOAT(atof(vecFloat[14].c_str()));
		mat.m44 = CWFLOAT(atof(vecFloat[15].c_str()));

		pEffectMatrixParam->setMatrix(mat);
		pEffectMatrixParam->setParameterName(pcName);

		return pEffectMatrixParam;
	}

	return nullptr;
}

cwEffectParameter* cwEffectParser::parseTextureParameter(tinyxml2::XMLElement* pParameterElement)
{
	if (!pParameterElement) return nullptr;

	const char* pcName = pParameterElement->Attribute("Name");
	if (pcName) return nullptr;

	tinyxml2::XMLElement* pDataElement = pParameterElement->FirstChildElement("Data");
	if (pDataElement) {
		const char* pcData = pDataElement->GetText();
		std::vector<CWSTRING> vecPath;
		CWSTRING strData(pcData);
		cwStringConvert::split(strData, "/", vecPath);
		if (vecPath.size() < 2) return nullptr;

		cwRenderer* pRenderer = cwRepertory::getInstance().getEngine()->getRenderer();
		cwStage* pStage = pRenderer->getStage(vecPath[0]);
		if (pStage) {
			cwTexture* pTexture = pStage->getStageTexture(vecPath[1]);
			if (pTexture) {
				cwEffectTextureParameter* pEffectTextureParam = cwEffectTextureParameter::create();
				if (pEffectTextureParam) {
					pEffectTextureParam->setParameterName(pcName);
					pEffectTextureParam->setTexture(pTexture);
					return pEffectTextureParam;
				}
			}
		}
	}

	return nullptr;
}

NS_MINIR_END
