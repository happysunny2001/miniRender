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

#include "BoxDemoScene.h"

#include <sstream>

BoxDemoScene* BoxDemoScene::create()
{
	BoxDemoScene* pScene = new BoxDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

BoxDemoScene::BoxDemoScene():
m_bTouchDown(false),
m_fTime(0),
m_iCount(0),
m_pRenderTex(nullptr),
m_pTerrain(nullptr)
{

}

BoxDemoScene::~BoxDemoScene()
{
	CW_SAFE_RELEASE_NULL(m_pTerrain);
}

bool BoxDemoScene::init()
{
	if (!cwScene::init()) return false;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown   = CW_CALLBACK_1(BoxDemoScene::onTouchDown, this);
	pTouchListener->onTouchUp     = CW_CALLBACK_1(BoxDemoScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(BoxDemoScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	this->schedulerUpdate();

	m_fTheta = 0.1f;
	m_fPhi = -cwMathUtil::cwPIx2 / 8.0f;
	m_fRadius = 20.0f;

	m_pRenderTex = cwRepertory::getInstance().getTextureManager()->createRenderTexture(1.0f, 1.0f);
	buildPlane();

	return true;
}

void BoxDemoScene::onTouchDown(cwTouch* pTouch)
{
	wstringstream wss;
	wss << pTouch->getScreenPos().x << "," << pTouch->getScreenPos().y << "\n";
	OutputDebugString(wss.str().c_str());

	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;

	m_bTouchDown = true;
}

void BoxDemoScene::onTouchUp(cwTouch* pTouch)
{
	m_bTouchDown = false;
}

void BoxDemoScene::onTouchMoving(cwTouch* pTouch)
{
	if (m_bTouchDown) {
		CWFLOAT dx = XMConvertToRadians(pTouch->getScreenPos().x - m_fLastX);
		CWFLOAT dy = XMConvertToRadians(pTouch->getScreenPos().y - m_fLastY);

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

void BoxDemoScene::update(CWFLOAT dt)
{
	m_fTime += dt;
	if (m_fTime >= 1.0f) {
		OutputDebugString(L"BoxDemoScene::update\n");
		m_fTime = 0;
		m_iCount++;

		if (m_iCount >= 5) {
			this->clearScheduler();
		}
	}
}

CWVOID BoxDemoScene::render()
{
	//if (m_bVisible) {
	//	this->transform();

	//	cwRepertory::getInstance().getDevice()->setClearColor(cwColor::blue);
	//	cwRepertory::getInstance().getDevice()->setRenderTarget(m_pRenderTex);
	//	cwRepertory::getInstance().getDevice()->beginDraw();
	//	this->renderSelf();
	//	this->renderChildren();
	//	cwRepertory::getInstance().getDevice()->endDraw();


	//	cwRepertory::getInstance().getDevice()->setClearColor(cwColor::sliver);
	//	cwRepertory::getInstance().getDevice()->setRenderTarget(nullptr);
	//	cwRepertory::getInstance().getDevice()->beginDraw();
	//	m_pTerrain->transform();
	//	cwRepertory::getInstance().getEngine()->render(m_pTerrain);
	//	cwRepertory::getInstance().getDevice()->endDraw();
	//	cwRepertory::getInstance().getDevice()->swap();
	//}
}

void BoxDemoScene::buildPlane()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateGrid(200.0f, 200.0f, 10, 10, mesh);

	vector<cwVertexPosNormalTex> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].pos.y = 0;
		vecVertex[i].normal = cwVector3D(0, 1.0, 0);
		vecVertex[i].tex = mesh.nVertex[i].tex;
	}

	cwRenderObject* pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormalTex), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), ceEleDescPosNormalTex);

	cwShader* pShader = repertory.getShaderManager()->getDefShader(eDefShaderLightingTex);
	cwMaterial* pMaterial = cwMaterial::create();
	//pMaterial->setShader(pShader);
	pMaterial->setDiffuseTexture(m_pRenderTex);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	m_pTerrain = cwEntity::create();
	m_pTerrain->setMaterial(pMaterial);
	m_pTerrain->setRenderObject(pRenderObj);
	m_pTerrain->setPosition(cwVector3D::ZERO);
	m_pTerrain->setEffect(pEffect);
	//this->addChild(m_pTerrain);
	CW_SAFE_RETAIN(m_pTerrain);

	cwLight* pLightDirectional = cwLight::createDirectionalLight(
		cwVector3D(0, -1.0, 0),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f));
	this->addLight(pLightDirectional);
}
