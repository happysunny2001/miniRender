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

#include "cwParserManager.h"
#include "cwStageParser.h"
#include "cwRendererParser.h"
#include "cwEntityParser.h"
#include "cwBlendParser.h"
#include "cwStencilParser.h"
#include "cwStageLayerParser.h"
#include "cwPUStageLayerParser.h"
#include "cwTextureParser.h"
#include "cwEffectParser.h"
#include "cwRenderStateParser.h"

NS_MINIR_BEGIN

cwParserManager* cwParserManager::create()
{
	cwParserManager* pManager = new cwParserManager();
	if (pManager && pManager->init()) {
		pManager->autorelease();
		return pManager;
	}

	CW_SAFE_DELETE(pManager);
	return nullptr;
}

cwParserManager::cwParserManager()
{
	for (int i = 0; i < eParserTypeMax; ++i) {
		m_nArrParser[i] = nullptr;
	}

	m_nMapBool["True"]  = CWTRUE;
	m_nMapBool["False"] = CWFALSE;

	m_nMapColorEnable["None"]  = eColorWriteEnableNone;
	m_nMapColorEnable["Red"]   = eColorWriteEnableRed;
	m_nMapColorEnable["Green"] = eColorWriteEnableGreen;
	m_nMapColorEnable["Blue"]  = eColorWriteEnableBlue;
	m_nMapColorEnable["Alpha"] = eColorWriteEnableAlpha;
	m_nMapColorEnable["All"]   = eColorWriteEnableAll;

	m_nMapClassification["Vertex"]   = eClassificationPerVertex;
	m_nMapClassification["Instance"] = eClassificationPerInstance;

	initFormat();
}

cwParserManager::~cwParserManager()
{
	for (int i = 0; i < eParserTypeMax; ++i) {
		CW_SAFE_RELEASE_NULL(m_nArrParser[i]);
	}
}

CWBOOL cwParserManager::init()
{
	m_nArrParser[eParserStage] = cwStageParser::create();
	CW_SAFE_RETAIN(m_nArrParser[eParserStage]);

	m_nArrParser[eParserRenderer] = cwRendererParser::create();
	CW_SAFE_RETAIN(m_nArrParser[eParserRenderer]);

	m_nArrParser[eParserEntity] = cwEntityParser::create();
	CW_SAFE_RETAIN(m_nArrParser[eParserEntity]);

	m_nArrParser[eParserBlend] = cwBlendParser::create();
	CW_SAFE_RETAIN(m_nArrParser[eParserBlend]);

	m_nArrParser[eParserStencil] = cwStencilParser::create();
	CW_SAFE_RETAIN(m_nArrParser[eParserStencil]);

	m_nArrParser[eParserStageLayer] = cwStageLayerParser::create();
	CW_SAFE_RETAIN(m_nArrParser[eParserStageLayer]);

	m_nArrParser[eParserStageLayerPU] = cwPUStageLayerParser::create();
	CW_SAFE_RETAIN(m_nArrParser[eParserStageLayerPU]);

	m_nArrParser[eParserTexture] = cwTextureParser::create();
	CW_SAFE_RETAIN(m_nArrParser[eParserTexture]);

	m_nArrParser[eParserEffect] = cwEffectParser::create();
	CW_SAFE_RETAIN(m_nArrParser[eParserEffect]);

	m_nArrParser[eParserRenderState] = cwRenderStateParser::create();
	CW_SAFE_RETAIN(m_nArrParser[eParserRenderState]);

	return CWTRUE;
}

CWBOOL cwParserManager::getBool(const CWSTRING& strBool)
{
	if (m_nMapBool.find(strBool) != m_nMapBool.end())
		return m_nMapBool[strBool];

	return CWFALSE;
}

eColorWriteEnable cwParserManager::getColorEnable(const CWSTRING& strColor)
{
	if (m_nMapColorEnable.find(strColor) != m_nMapColorEnable.end())
		return m_nMapColorEnable[strColor];
	return eColorWriteEnableNone;
}

eFormat cwParserManager::getFormatType(const CWSTRING& strFormat) const
{
	auto it = m_nMapFormat.find(strFormat);
	if (it == m_nMapFormat.end()) return eFormatUnknown;
	return it->second;
}

eClassification cwParserManager::getClassificationType(const CWSTRING& strClass) const
{
	auto it = m_nMapClassification.find(strClass);
	if (it == m_nMapClassification.end()) return eClassificationPerVertex;
	return it->second;
}

CWVOID cwParserManager::initFormat()
{
	m_nMapFormat["Unknown"]                = eFormatUnknown;
	m_nMapFormat["R32g32b32a32Typeless"]   = eFormatR32g32b32a32Typeless;
	m_nMapFormat["R32g32b32a32Float"]      = eFormatR32g32b32a32Float;
	m_nMapFormat["R32g32b32a32Uint"]       = eFormatR32g32b32a32Uint;
	m_nMapFormat["R32g32b32a32Sint"]       = eFormatR32g32b32a32Sint;
	m_nMapFormat["R32g32b32Typeless"]      = eFormatR32g32b32Typeless;
	m_nMapFormat["R32g32b32Float"]         = eFormatR32g32b32Float;
	m_nMapFormat["R32g32b32Uint"]          = eFormatR32g32b32Uint;
	m_nMapFormat["R32g32b32Sint"]          = eFormatR32g32b32Sint;
	m_nMapFormat["R16g16b16a16Typeless"]   = eFormatR16g16b16a16Typeless;
	m_nMapFormat["R16g16b16a16Float"]      = eFormatR16g16b16a16Float;
	m_nMapFormat["R16g16b16a16Unorm"]      = eFormatR16g16b16a16Unorm;
	m_nMapFormat["R16g16b16a16Uint"]       = eFormatR16g16b16a16Uint;
	m_nMapFormat["R16g16b16a16Snorm"]      = eFormatR16g16b16a16Snorm;
	m_nMapFormat["R16g16b16a16Sint"]       = eFormatR16g16b16a16Sint;
	m_nMapFormat["R32g32Typeless"]         = eFormatR32g32Typeless;
	m_nMapFormat["R32g32Float"]            = eFormatR32g32Float;
	m_nMapFormat["R32g32Uint"]             = eFormatR32g32Uint;
	m_nMapFormat["R32g32Sint"]             = eFormatR32g32Sint;
	m_nMapFormat["R32g8x24Typeless"]       = eFormatR32g8x24Typeless;
	m_nMapFormat["D32FloatS8x24Uint"]      = eFormatD32FloatS8x24Uint;
	m_nMapFormat["R32FloatX8x24Typeless"]  = eFormatR32FloatX8x24Typeless;
	m_nMapFormat["X32TypelessG8x24Uint"]   = eFormatX32TypelessG8x24Uint;
	m_nMapFormat["R10g10b10a2Typeless"]    = eFormatR10g10b10a2Typeless;
	m_nMapFormat["R10g10b10a2Unorm"]       = eFormatR10g10b10a2Unorm;
	m_nMapFormat["R10g10b10a2Uint"]        = eFormatR10g10b10a2Uint;
	m_nMapFormat["R11g11b10Float"]         = eFormatR11g11b10Float;
	m_nMapFormat["R8g8b8a8Typeless"]       = eFormatR8g8b8a8Typeless;
	m_nMapFormat["R8g8b8a8Unorm"]          = eFormatR8g8b8a8Unorm;
	m_nMapFormat["R8g8b8a8UnormSrgb"]      = eFormatR8g8b8a8UnormSrgb;
	m_nMapFormat["R8g8b8a8Uint"]           = eFormatR8g8b8a8Uint;
	m_nMapFormat["R8g8b8a8Snorm"]          = eFormatR8g8b8a8Snorm;
	m_nMapFormat["R8g8b8a8Sint"]           = eFormatR8g8b8a8Sint;
	m_nMapFormat["R16g16Typeless"]         = eFormatR16g16Typeless;
	m_nMapFormat["R16g16Float"]            = eFormatR16g16Float;
	m_nMapFormat["R16g16Unorm"]            = eFormatR16g16Unorm;
	m_nMapFormat["R16g16Uint"]             = eFormatR16g16Uint;
	m_nMapFormat["R16g16Snorm"]            = eFormatR16g16Snorm;
	m_nMapFormat["R16g16Sint"]             = eFormatR16g16Sint;
	m_nMapFormat["R32Typeless"]            = eFormatR32Typeless;
	m_nMapFormat["d32Float"]               = eFormatd32Float;
	m_nMapFormat["R32Float"]               = eFormatR32Float;
	m_nMapFormat["R32Uint"]                = eFormatR32Uint;
	m_nMapFormat["R32Sint"]                = eFormatR32Sint;
	m_nMapFormat["R24g8Typeless"]          = eFormatR24g8Typeless;
	m_nMapFormat["d24UnormS8Uint"]         = eFormatd24UnormS8Uint;
	m_nMapFormat["R24UnormX8Typeless"]     = eFormatR24UnormX8Typeless;
	m_nMapFormat["x24TypelessG8Uint"]      = eFormatx24TypelessG8Uint;
	m_nMapFormat["R8g8Typeless"]           = eFormatR8g8Typeless;
	m_nMapFormat["R8g8Unorm"]              = eFormatR8g8Unorm;
	m_nMapFormat["R8g8Uint"]               = eFormatR8g8Uint;
	m_nMapFormat["R8g8Snorm"]              = eFormatR8g8Snorm;
	m_nMapFormat["R8g8Sint"]               = eFormatR8g8Sint;
	m_nMapFormat["R16Typeless"]            = eFormatR16Typeless;
	m_nMapFormat["R16Float"]               = eFormatR16Float;
	m_nMapFormat["d16Unorm"]               = eFormatd16Unorm;
	m_nMapFormat["R16Unorm"]               = eFormatR16Unorm;
	m_nMapFormat["R16Uint"]                = eFormatR16Uint;
	m_nMapFormat["R16Snorm"]               = eFormatR16Snorm;
	m_nMapFormat["R16Sint"]                = eFormatR16Sint;
	m_nMapFormat["R8Typeless"]             = eFormatR8Typeless;
	m_nMapFormat["R8Unorm"]                = eFormatR8Unorm;
	m_nMapFormat["R8Uint"]                 = eFormatR8Uint;
	m_nMapFormat["R8Snorm"]                = eFormatR8Snorm;
	m_nMapFormat["R8Sint"]                 = eFormatR8Sint;
	m_nMapFormat["A8Unorm"]                = eFormatA8Unorm;
	m_nMapFormat["R1Unorm"]                = eFormatR1Unorm;
	m_nMapFormat["R9g9b9e5Sharedexp"]      = eFormatR9g9b9e5Sharedexp;
	m_nMapFormat["R8g8_B8g8Unorm"]         = eFormatR8g8_B8g8Unorm;
	m_nMapFormat["G8r8G8b8Unorm"]          = eFormatG8r8G8b8Unorm;
	m_nMapFormat["Bc1Typeless"]            = eFormatBc1Typeless;
	m_nMapFormat["Bc1Unorm"]               = eFormatBc1Unorm;
	m_nMapFormat["Bc1UnormSrgb"]           = eFormatBc1UnormSrgb;
	m_nMapFormat["Bc2Typeless"]            = eFormatBc2Typeless;
	m_nMapFormat["Bc2Unorm"]               = eFormatBc2Unorm;
	m_nMapFormat["Bc2UnormSrgb"]           = eFormatBc2UnormSrgb;
	m_nMapFormat["Bc3Typeless"]            = eFormatBc3Typeless;
	m_nMapFormat["Bc3Unorm"]               = eFormatBc3Unorm;
	m_nMapFormat["Bc3UnormSrgb"]           = eFormatBc3UnormSrgb;
	m_nMapFormat["Bc4Typeless"]            = eFormatBc4Typeless;
	m_nMapFormat["Bc4Unorm"]               = eFormatBc4Unorm;
	m_nMapFormat["Bc4Snorm"]               = eFormatBc4Snorm;
	m_nMapFormat["Bc5Typeless"]            = eFormatBc5Typeless;
	m_nMapFormat["Bc5Unorm"]               = eFormatBc5Unorm;
	m_nMapFormat["Bc5Snorm"]               = eFormatBc5Snorm;
	m_nMapFormat["B5g6r5Unorm"]            = eFormatB5g6r5Unorm;
	m_nMapFormat["B5g5r5a1Unorm"]          = eFormatB5g5r5a1Unorm;
	m_nMapFormat["B8g8r8a8Unorm"]          = eFormatB8g8r8a8Unorm;
	m_nMapFormat["B8g8r8x8Unorm"]          = eFormatB8g8r8x8Unorm;
	m_nMapFormat["R10g10b10XrBiasA2Unorm"] = eFormatR10g10b10XrBiasA2Unorm;
	m_nMapFormat["B8g8r8a8Typeless"]       = eFormatB8g8r8a8Typeless;
	m_nMapFormat["B8g8r8a8UnormSrgb"]      = eFormatB8g8r8a8UnormSrgb;
	m_nMapFormat["B8g8r8x8Typeless"]       = eFormatB8g8r8x8Typeless;
	m_nMapFormat["B8g8r8x8UnormSrgb"]      = eFormatB8g8r8x8UnormSrgb;
	m_nMapFormat["Bc6hTypeless"]           = eFormatBc6hTypeless;
	m_nMapFormat["Bc6hUf16"]               = eFormatBc6hUf16;
	m_nMapFormat["Bc6hSf16"]               = eFormatBc6hSf16;
	m_nMapFormat["Bc7Typeless"]            = eFormatBc7Typeless;
	m_nMapFormat["Bc7Unorm"]               = eFormatBc7Unorm;
	m_nMapFormat["Bc7UnormSrgb"]           = eFormatBc7UnormSrgb;
	m_nMapFormat["ForceUint"]              = eFormatForceUint;
}

NS_MINIR_END
