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

#include "TessellationDemoScene.h"

TessellationDemoScene* TessellationDemoScene::create()
{
	TessellationDemoScene* pScene = new TessellationDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

TessellationDemoScene::TessellationDemoScene()
{

}

TessellationDemoScene::~TessellationDemoScene()
{

}

CWBOOL TessellationDemoScene::init()
{
	if (!cwScene::init()) return CWFALSE;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown = CW_CALLBACK_1(TessellationDemoScene::onTouchDown, this);
	pTouchListener->onTouchUp = CW_CALLBACK_1(TessellationDemoScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(TessellationDemoScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	m_fTheta = 0.1f;
	m_fPhi = -cwMathUtil::cwPIx2 / 8.0f;
	m_fRadius = 150.0f;
	m_bTouchDown = CWFALSE;

	buildEntity();

	return CWTRUE;
}

CWVOID TessellationDemoScene::onTouchDown(cwTouch* pTouch)
{
	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;

	m_bTouchDown = true;
}

CWVOID TessellationDemoScene::onTouchUp(cwTouch* pTouch)
{
	m_bTouchDown = false;
}

CWVOID TessellationDemoScene::onTouchMoving(cwTouch* pTouch)
{
	if (m_bTouchDown) {
		CWFLOAT dx = cwMathUtil::angleRadian(pTouch->getScreenPos().x - m_fLastX);
		CWFLOAT dy = cwMathUtil::angleRadian(pTouch->getScreenPos().y - m_fLastY);

		m_fTheta -= dx;
		m_fPhi += dy;
		m_fPhi = min(max(0.1f, m_fPhi), cwMathUtil::cwPI - 0.1f);

		float x = m_fRadius*sinf(m_fPhi)*cosf(m_fTheta);
		float z = m_fRadius*sinf(m_fPhi)*sinf(m_fTheta);
		float y = m_fRadius*cosf(m_fPhi);

		cwRepertory::getInstance().getEngine()->getDefaultCamera()->updateCamera(x, y, z);
	}

	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;
}

CWVOID TessellationDemoScene::buildEntity()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateGrid(20.0f, 20.0f, 2, 2, mesh);

	vector<cwVertexPosColor> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].color = cwVector4D(1.0, 1.0, 1.0, 1.0);
	}

	cwRenderObject* pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypePatchList3,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosColor), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), ceEleDescPosColor);

	cwShader* pShader = repertory.getShaderManager()->getDefShader(eDefShaderLightingTex);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	cwEntity* pQuad = cwEntity::create();
	pQuad->setEffect(pEffect);
	pQuad->setRenderObject(pRenderObj);
	pQuad->setPosition(cwVector3D::ZERO);
	this->addChild(pQuad);
}
