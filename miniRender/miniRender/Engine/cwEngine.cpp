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

#include "cwEngine.h"
#include "cwScene.h"

NS_MINI_BEGIN

cwEngine& cwEngine::getInstance()
{
	static cwEngine engine;
	return engine;
}

cwEngine::cwEngine():
m_pCurrScene(nullptr)
{

}

cwEngine::~cwEngine()
{
	CW_SAFE_RELEASE_NULL(m_pCurrScene);
}

void cwEngine::setScene(cwScene* pScene)
{
	if (pScene == m_pCurrScene) return;
	CW_SAFE_RETAIN(pScene);
	CW_SAFE_RELEASE_NULL(m_pCurrScene);
	m_pCurrScene = pScene;
}

void cwEngine::mainLoop(float dt)
{
	if (m_pCurrScene) {
		m_pCurrScene->update(dt);
		m_pCurrScene->render();
	}
}

NS_MINI_END