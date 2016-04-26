/*
Copyright © 2016 Ziwei Wang

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

#include "TempScene.h"
#include <sstream>

TempScene* TempScene::create()
{
	TempScene* pScene = new TempScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

TempScene::TempScene() 
{

}

TempScene::~TempScene()
{
	if (m_pLblCnt) {
		m_pLblCnt->removeFromParent();
		m_pLblCnt = NULL;
	}
}

CWBOOL TempScene::init()
{
	if (!cwBaseScene::init()) return CWFALSE;

	buildLabel();

	return CWTRUE;
}

CWVOID TempScene::update(CWFLOAT dt)
{
	std::stringstream ss;
	ss << cwRepertory::getInstance().getEngine()->getSpatial()->getObjCnt();
	m_pLblCnt->setString(ss.str());
}

CWVOID TempScene::buildLabel()
{
	m_pLblCnt = cwLabel::create("0", "frame_text.png", '0', 10);
	m_pLblCnt->setPosition(-100, -100);
	cwRepertory::getInstance().getEngine()->addNode2D(m_pLblCnt);
	m_pLblCnt->setTag(200);
}