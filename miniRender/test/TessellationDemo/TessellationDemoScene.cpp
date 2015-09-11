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

	buildEntityTriangle();
	buildEntityQuad();
	buildEntityBezier();

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

CWVOID TessellationDemoScene::buildEntityTriangle()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateGrid(50.0f, 50.0f, 2, 2, mesh);

	vector<cwVertexPosColor> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].color = cwVector4D(1.0, 1.0, 1.0, 1.0);
	}

	cwRenderObject* pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypePatchList3,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosColor), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), ceEleDescPosColor);

	cwShader* pShader = repertory.getShaderManager()->loadShader("effect/D3D11/tessellation.fx");
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);
	pEffect->setTech("TessTriangle");

	cwMaterial* pMaterial = cwMaterial::create();

	cwEntity* pTriangle = cwEntity::create();
	pTriangle->setEffect(pEffect);
	pTriangle->setMaterial(pMaterial);
	pTriangle->setRenderObject(pRenderObj);
	pTriangle->setPosition(cwVector3D(-25, 0, -25));
	this->addChild(pTriangle);
}

CWVOID TessellationDemoScene::buildEntityQuad()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateGrid(50.0f, 50.0f, 2, 2, mesh);

	vector<cwVertexPosColor> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].color = cwVector4D(1.0, 1.0, 1.0, 1.0);
	}

	cwRenderObject* pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypePatchList4,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosColor), static_cast<CWUINT>(mesh.nVertex.size()),
		NULL, 0, ceEleDescPosColor);

	cwShader* pShader = repertory.getShaderManager()->loadShader("effect/D3D11/tessellation.fx");
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);
	pEffect->setTech("TessQuad");

	cwMaterial* pMaterial = cwMaterial::create();

	cwEntity* pQuad = cwEntity::create();
	pQuad->setEffect(pEffect);
	pQuad->setMaterial(pMaterial);
	pQuad->setRenderObject(pRenderObj);
	pQuad->setPosition(cwVector3D(25, 0, -25));
	this->addChild(pQuad);
}

CWVOID TessellationDemoScene::buildEntityBezier()
{
	vector<cwVertexPosColor> vecVertex(16);

	vecVertex[0].pos.set(-10.0f, -10.0f, +15.0f);
	vecVertex[1].pos.set(-5.0f,    0.0f, +15.0f);
	vecVertex[2].pos.set(+5.0f,    0.0f, +15.0f);
	vecVertex[3].pos.set(+10.0f,   0.0f, +15.0f);

	vecVertex[4].pos.set(-15.0f, 0.0f, +5.0f);
	vecVertex[5].pos.set(-5.0f,  0.0f, +5.0f);
	vecVertex[6].pos.set(+5.0f, 20.0f, +5.0f);
	vecVertex[7].pos.set(+15.0f, 0.0f, +5.0f);

	vecVertex[8].pos.set(-15.0f,  0.0f, -5.0f);
	vecVertex[9].pos.set(-5.0f,   0.0f, -5.0f);
	vecVertex[10].pos.set(+5.0f,  0.0f, -5.0f);
	vecVertex[11].pos.set(+15.0f, 0.0f, -5.0f);

	vecVertex[12].pos.set(-10.0f, 10.0f, -15.0f);
	vecVertex[13].pos.set(-5.0f,  0.0f,  -15.0f);
	vecVertex[14].pos.set(+5.0f,  0.0f,  -15.0f);
	vecVertex[15].pos.set(+25.0f, 10.0f, -15.0f);

	for (CWUINT i = 0; i < 16; ++i)
		vecVertex[i].color = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);

	cwRenderObject* pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypePatchList16,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosColor), 16,
		NULL, 0, ceEleDescPosColor);

	cwRepertory& repertory = cwRepertory::getInstance();
	cwShader* pShader = repertory.getShaderManager()->loadShader("effect/D3D11/tessellation.fx");
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);
	pEffect->setTech("TessBezier");

	cwMaterial* pMaterial = cwMaterial::create();

	cwEntity* pBezier = cwEntity::create();
	pBezier->setEffect(pEffect);
	pBezier->setMaterial(pMaterial);
	pBezier->setRenderObject(pRenderObj);
	pBezier->setPosition(cwVector3D(0, 0, 25));
	this->addChild(pBezier);
}
