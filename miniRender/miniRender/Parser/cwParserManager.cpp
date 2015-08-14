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

	m_nMapBool["True"] = CWTRUE;
	m_nMapBool["False"] = CWFALSE;
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

	return CWTRUE;
}

CWBOOL cwParserManager::getBool(const CWSTRING& strBool)
{
	if (m_nMapBool.find(strBool) != m_nMapBool.end())
		return m_nMapBool[strBool];

	return CWFALSE;
}

NS_MINIR_END
