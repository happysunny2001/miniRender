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

#include "cwScene.h"
#include "cwMacros.h"

NS_CW_BEGIN

cwScene* cwScene::create()
{
	cwScene* pScene = new cwScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}
	CW_SAFE_DELETE(pScene);
	return nullptr;
}

cwScene::cwScene()
{

}

cwScene::~cwScene()
{

}

bool cwScene::init()
{
	if (!cwRenderNode::init()) return false;
	return true;
}

void cwScene::appendLight(cwLight* pLight)
{
	if (!pLight) return;
	if (m_nVecLights.contains(pLight)) return;
	m_nVecLights.pushBack(pLight);
}

void cwScene::removeLight(cwLight* pLight)
{
	if (!pLight) return;
	m_nVecLights.erase(pLight);
}

const cwVector<cwLight*>& cwScene::getLights() const
{
	return m_nVecLights;
}

NS_CW_END
