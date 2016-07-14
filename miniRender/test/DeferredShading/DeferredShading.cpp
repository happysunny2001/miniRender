﻿/*
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

#include "DeferredShading.h"
#include "DeferredShadingScene.h"

DeferredShading::DeferredShading()
{

}

DeferredShading::~DeferredShading()
{

}

CWVOID DeferredShading::gameBegin()
{
	cwRepertory::getInstance().getEngine()->loadRenderer("Render/renderDeferredShading.xml");
	cwRepertory::getInstance().getEngine()->getDefaultCamera()->updateCamera(0, 15.0f, -60.0f);
	cwRepertory::getInstance().getEngine()->setFrameEnable(CWTRUE);

	DeferredShadingScene* pScene = DeferredShadingScene::create();
	cwRepertory::getInstance().getEngine()->setScene(pScene);
}

CWVOID DeferredShading::gameEnd()
{

}

CWVOID DeferredShading::gameBeginBackGround()
{

}

CWVOID DeferredShading::gameEndBackGround()
{

}
