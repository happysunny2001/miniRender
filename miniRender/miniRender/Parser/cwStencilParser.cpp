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

#include "cwStencilParser.h"
#include "cwParserManager.h"
#include "Base/cwStruct.h"
#include "Base/cwCommon.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

cwStencilParser* cwStencilParser::create()
{
	cwStencilParser* pParser = new cwStencilParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwStencilParser::cwStencilParser()
{
	m_nMapDepthWriteMask["Zero"] = eDepthWriteMaskZero;
	m_nMapDepthWriteMask["All"]  = eDepthWriteMaskAll;

	m_nMapComparison["Never"]        = eComparisonNever;
	m_nMapComparison["Less"]         = eComparisonLess;
	m_nMapComparison["Equal"]        = eComparisonEqual;
	m_nMapComparison["LessEqual"]    = eComparisonLessEqual;
	m_nMapComparison["Greater"]      = eComparisonGreater;
	m_nMapComparison["NotEqual"]     = eComparisonNotEqual;
	m_nMapComparison["GreaterEqual"] = eComparisonGreaterEqual;
	m_nMapComparison["Always"]       = eComparisonAlways;

	m_nMapStencilOp["Keep"]    = eStencilOpKeep;
	m_nMapStencilOp["Zero"]    = eStencilOpZero;
	m_nMapStencilOp["Replace"] = eStencilOpReplace;
	m_nMapStencilOp["IncrSat"] = eStencilOpIncrSat;
	m_nMapStencilOp["DecrSat"] = eStencilOpDecrSat;
	m_nMapStencilOp["Invert"]  = eStencilOpInvert;
	m_nMapStencilOp["Incr"]    = eStencilOpIncr;
	m_nMapStencilOp["Decr"]    = eStencilOpDecr;
}

cwStencil* cwStencilParser::parse(tinyxml2::XMLElement* pStencilData)
{
	if (!pStencilData) return nullptr;

	StencilData stencilData;
	memset(&stencilData, 0, sizeof(StencilData));

	tinyxml2::XMLElement* pDepthElement = pStencilData->FirstChildElement("DepthBuffer");
	if (pDepthElement) {
		const char* pcEnable = pDepthElement->Attribute("Enable");
		if (pcEnable)
			stencilData.depthEnable = cwRepertory::getInstance().getParserManager()->getBool(pcEnable);

		const char* pcWriteMask = pDepthElement->Attribute("WriteMask");
		if (pcWriteMask && m_nMapDepthWriteMask.find(pcWriteMask) != m_nMapDepthWriteMask.end())
			stencilData.depthWriteMask = m_nMapDepthWriteMask[pcWriteMask];

		const char* pcFunc = pDepthElement->Attribute("Func");
		if (pcFunc && m_nMapComparison.find(pcFunc) != m_nMapComparison.end())
			stencilData.depthFunc = m_nMapComparison[pcFunc];
	}

	tinyxml2::XMLElement* pStencilElement = pStencilData->FirstChildElement("StencilBuffer");
	if (pStencilElement) {
		const char* pcEnable = pStencilElement->Attribute("Enable");
		if (pcEnable)
			stencilData.stencilEnable = cwRepertory::getInstance().getParserManager()->getBool(pcEnable);

		const char* pcReadMask = pStencilElement->Attribute("ReadMask");
		CWBYTE cReadMask = cwCommon::parseHexStringByte(pcReadMask);
		stencilData.stencilReadMask = cReadMask;

		const char* pcWriteMask = pStencilElement->Attribute("WriteMask");
		CWBYTE cWriteMask = cwCommon::parseHexStringByte(pcWriteMask);
		stencilData.stencilWriteMask = cWriteMask;
	}

	tinyxml2::XMLElement* pFrontElement = pStencilData->FirstChildElement("Front");
	if (pFrontElement) {
		const char* pcFailed = pFrontElement->Attribute("Failed");
		if (pcFailed && m_nMapStencilOp.find(pcFailed) != m_nMapStencilOp.end())
			stencilData.frontStencilFailOp = m_nMapStencilOp[pcFailed];

		const char* pcDepthFailed = pFrontElement->Attribute("DepthFailed");
		if (pcDepthFailed && m_nMapStencilOp.find(pcDepthFailed) != m_nMapStencilOp.end())
			stencilData.frontStencilDepthFailOp = m_nMapStencilOp[pcDepthFailed];

		const char* pcPass = pFrontElement->Attribute("Pass");
		if (pcPass && m_nMapStencilOp.find(pcPass) != m_nMapStencilOp.end())
			stencilData.frontStencilPassOp = m_nMapStencilOp[pcPass];

		const char* pcFunc = pFrontElement->Attribute("Func");
		if (pcFunc && m_nMapComparison.find(pcFunc) != m_nMapComparison.end())
			stencilData.frontStencilFunc = m_nMapComparison[pcFunc];
	}

	tinyxml2::XMLElement* pBackElement = pStencilData->FirstChildElement("Back");
	if (pBackElement) {
		const char* pcFailed = pBackElement->Attribute("Failed");
		if (pcFailed && m_nMapStencilOp.find(pcFailed) != m_nMapStencilOp.end())
			stencilData.backStencilFailOp = m_nMapStencilOp[pcFailed];

		const char* pcDepthFailed = pBackElement->Attribute("DepthFailed");
		if (pcDepthFailed && m_nMapStencilOp.find(pcDepthFailed) != m_nMapStencilOp.end())
			stencilData.backStencilDepthFailOp = m_nMapStencilOp[pcDepthFailed];

		const char* pcPass = pBackElement->Attribute("Pass");
		if (pcPass && m_nMapStencilOp.find(pcPass) != m_nMapStencilOp.end())
			stencilData.backStencilPassOp = m_nMapStencilOp[pcPass];

		const char* pcFunc = pBackElement->Attribute("Func");
		if (pcFunc && m_nMapComparison.find(pcFunc) != m_nMapComparison.end())
			stencilData.backStencilFunc = m_nMapComparison[pcFunc];
	}

	cwStencil* pStencil = cwRepertory::getInstance().getDevice()->createStencil(stencilData);
	CWUINT iValue = 0;
	const char* pcValue = pStencilData->Attribute("Value");
	if (pcValue) {
		iValue = atoi(pcValue);
	}
	pStencil->setStencilRef(iValue);

	return pStencil;
}

NS_MINIR_END
