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

#include "cwRenderer.h"
#include "cwStage.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

cwRenderer* cwRenderer::create()
{
	cwRenderer* pRenderer = new cwRenderer();
	if (pRenderer && pRenderer->init()) {
		pRenderer->autorelease();
		return pRenderer;
	}

	CW_SAFE_DELETE(pRenderer);
	return nullptr;
}

cwRenderer::cwRenderer():
m_pCurrCamera(nullptr)
{

}

cwRenderer::~cwRenderer()
{
	for (auto pStage : m_nVecStage) {
		CW_SAFE_DELETE(pStage);
	}

	m_pCurrCamera = nullptr;
}

CWBOOL cwRenderer::init()
{
	return true;
}

CWVOID cwRenderer::setCurrCamera(cwCamera* pCamera)
{
	m_pCurrCamera = pCamera;
}

CWVOID cwRenderer::addStage(cwStage* pStage)
{
	m_nVecStage.push_back(pStage);
}

CWVOID cwRenderer::render()
{
	for (auto pStage : m_nVecStage) {
		this->render(pStage);
	}

	cwRepertory::getInstance().getDevice()->swap();
}

CWVOID cwRenderer::render(cwStage* pStage)
{
	if (!pStage) return;

	pStage->begin();
	pStage->render();
	pStage->end();
}

cwStage* cwRenderer::getStage(const CWSTRING& strName)
{
	for (auto pStage : m_nVecStage) {
		if (pStage->getName == strName) return pStage;
	}

	return nullptr;
}

NS_MINIR_END
