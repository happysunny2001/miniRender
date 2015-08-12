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

#include "StencilDemoScene.h"

StencilDemoScene* StencilDemoScene::create()
{
	StencilDemoScene* pScene = new StencilDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

StencilDemoScene::StencilDemoScene()
{

}

StencilDemoScene::~StencilDemoScene()
{

}

CWBOOL StencilDemoScene::init()
{
	if (!cwScene::init()) return CWFALSE;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown = CW_CALLBACK_1(StencilDemoScene::onTouchDown, this);
	pTouchListener->onTouchUp = CW_CALLBACK_1(StencilDemoScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(StencilDemoScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	m_fTheta = 0.1f;
	m_fPhi = -cwMathUtil::cwPIx2 / 8.0f;
	m_fRadius = 100.0f;
	m_bTouchDown = false;

	buildEntity();
	buildLights();

	return CWTRUE;
}

CWVOID StencilDemoScene::onTouchDown(cwTouch* pTouch)
{
	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;

	m_bTouchDown = true;
}

CWVOID StencilDemoScene::onTouchUp(cwTouch* pTouch)
{
	m_bTouchDown = false;
}

CWVOID StencilDemoScene::onTouchMoving(cwTouch* pTouch)
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

CWVOID StencilDemoScene::buildEntity()
{
	buildGround();
	buildCar();
}

CWVOID StencilDemoScene::buildGround()
{
	cwGeometryGenerator::cwMeshData mesh;
	cwRepertory::getInstance().getGeoGenerator()->generateGrid(100.0f, 100.0f, 10, 10, mesh);
	cwStaticRenderObject* pGroundObj = createRenderObj(mesh);

	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderLightingTex);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	cwMaterial* pMaterial = cwMaterial::create();
	pMaterial->setDiffuseTexture("Textures/checkboard.dds");
	pMaterial->scaleDiffuseTexture(cwVector2D(5.0f, 5.0f));

	cwEntity* pEntityGround = cwEntity::create();
	pEntityGround->setRenderObject(pGroundObj);
	pEntityGround->setMaterial(pMaterial);
	pEntityGround->setEffect(pEffect);
	this->addChild(pEntityGround);
}

CWVOID StencilDemoScene::buildCar()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->loadTxt(repertory.getFileSystem()->getFullFilePath("Models/car.txt"), mesh);

	vector<cwVertexPosNormal> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].normal = mesh.nVertex[i].normal;
	}

	cwRenderObject *pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormal), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), ceEleDescPosNormal);

	cwShader* pShader = repertory.getShaderManager()->getDefShader(eDefShaderLighting);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	cwMaterial* pMaterial = cwMaterial::create();

	cwEntity* pCar = cwEntity::create();
	pCar->setMaterial(pMaterial);
	pCar->setRenderObject(pRenderObj);
	pCar->setPosition(cwVector3D(0, 20, 0));
	pCar->setScale(cwVector3D(3.0, 3.0, 3.0));
	pCar->setEffect(pEffect);

	this->addChild(pCar);
}

CWVOID StencilDemoScene::buildLights()
{
	cwLight* pLightDirectional = cwLight::createDirectionalLight(
		cwVector3D(1.0f, -1.0f, 0.0f),
		cwVector4D(0.5f, 0.5f, 0.5f, 1.0f),
		cwVector4D(0.8f, 0.8f, 0.8f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f));
	this->addLight(pLightDirectional);
}

cwStaticRenderObject* StencilDemoScene::createRenderObj(cwGeometryGenerator::cwMeshData& mesh)
{
	vector<cwVertexPosNormalTex> vecVertex;
	vecVertex.resize(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].normal = mesh.nVertex[i].normal;
		vecVertex[i].tex = mesh.nVertex[i].tex;
	}

	return cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormalTex), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), ceEleDescPosNormalTex);
}
