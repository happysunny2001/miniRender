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

#include "cwBlendParser.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

cwBlendParser* cwBlendParser::create()
{
	cwBlendParser* pBlendParser = new cwBlendParser();
	if (pBlendParser) {
		pBlendParser->autorelease();
		return pBlendParser;
	}

	return nullptr;
}

cwBlendParser::cwBlendParser()
{
	m_nMapBlendFactor["Zero"]           = eBlendFactorZero;
	m_nMapBlendFactor["One"]            = eBlendFactorOne;
	m_nMapBlendFactor["SrcColor"]       = eBlendFactorSrcColor;
	m_nMapBlendFactor["InvSrcColor"]    = eBlendFactorInvSrcColor;
	m_nMapBlendFactor["SrcAlpha"]       = eBlendFactorSrcAlpha;
	m_nMapBlendFactor["InvSrcAlpha"]    = eBlendFactorInvSrcAlpha;
	m_nMapBlendFactor["DestAlpha"]      = eBlendFactorDestAlpha;
	m_nMapBlendFactor["InvDestAlpha"]   = eBlendFactorInvDestAlpha;
	m_nMapBlendFactor["DestColor"]      = eBlendFactorDestColor;
	m_nMapBlendFactor["InvDestColor"]   = eBlendFactorInvDestColor;
	m_nMapBlendFactor["SrcAlphaSat"]    = eBlendFactorSrcAlphaSat;
	m_nMapBlendFactor["BlendFactor"]    = eBlendFactorBlendFactor;
	m_nMapBlendFactor["InvBlendFactor"] = eBlendFactorInvBlendFactor;
	m_nMapBlendFactor["Src1Color"]      = eBlendFactorSrc1Color;
	m_nMapBlendFactor["InvSrc1Color"]   = eBlendFactorInvSrc1Color;
	m_nMapBlendFactor["Src1Alpha"]      = eBlendFactorSrc1Alpha;
	m_nMapBlendFactor["InvSrc1Alpha"]   = eBlendFactorInvSrc1Alpha;

	m_nMapBlendOp["Add"]    = eBlendOpAdd;
	m_nMapBlendOp["Sub"]    = eBlendOpSubtract;
	m_nMapBlendOp["RevSub"] = eBlendOpRevSubtract;
	m_nMapBlendOp["Min"]    = eBlendOpMin;
	m_nMapBlendOp["Max"]    = eBlendOpMax;
}

cwBlend* cwBlendParser::parse(tinyxml2::XMLElement* pBlendData)
{
	if (!pBlendData) return nullptr;

	BlendData blendData;
	tinyxml2::XMLElement* pRGBElement = pBlendData->FirstChildElement("RGB");
	if (pRGBElement) {
		const char* pcSrc = pRGBElement->Attribute("Src");
		if (pcSrc && m_nMapBlendFactor.find(pcSrc) != m_nMapBlendFactor.end())
			blendData.srcBlend = m_nMapBlendFactor[pcSrc];

		const char* pcDest = pRGBElement->Attribute("Dest");
		if (pcDest && m_nMapBlendFactor.find(pcDest) != m_nMapBlendFactor.end())
			blendData.dstBlend = m_nMapBlendFactor[pcDest];

		const char* pcOper = pRGBElement->Attribute("Oper");
		if (pcOper && m_nMapBlendOp.find(pcOper) != m_nMapBlendOp.end())
			blendData.blendOp = m_nMapBlendOp[pcOper];
	}

	tinyxml2::XMLElement* pAlphaElement = pBlendData->FirstChildElement("Alpha");
	if (pAlphaElement) {
		const char* pcSrc = pAlphaElement->Attribute("Src");
		if (pcSrc && m_nMapBlendFactor.find(pcSrc) != m_nMapBlendFactor.end())
			blendData.srcBlendAlpha = m_nMapBlendFactor[pcSrc];

		const char* pcDest = pAlphaElement->Attribute("Dest");
		if (pcDest && m_nMapBlendFactor.find(pcDest) != m_nMapBlendFactor.end())
			blendData.dstBlendAlpha = m_nMapBlendFactor[pcDest];

		const char* pcOper = pAlphaElement->Attribute("Oper");
		if (pcOper && m_nMapBlendOp.find(pcOper) != m_nMapBlendOp.end())
			blendData.blendOpAlpha = m_nMapBlendOp[pcOper];
	}

	return cwRepertory::getInstance().getDevice()->createBlend(blendData);
}

NS_MINIR_END

