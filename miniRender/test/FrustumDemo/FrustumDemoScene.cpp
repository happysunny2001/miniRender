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

#include "FrustumDemoScene.h"

FrustumDemoScene* FrustumDemoScene::create()
{
	FrustumDemoScene* pScene = new FrustumDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

FrustumDemoScene::FrustumDemoScene() :
m_pCamera(nullptr)
{
	memset(m_pCar, 0, sizeof(cwEntity*)* 5 * 5 * 5);
}

FrustumDemoScene::~FrustumDemoScene()
{
	for (int i = 0; i < 5 * 5 * 5; ++i) {
		CW_SAFE_RELEASE_NULL(m_pCar[i]);
	}
	CW_SAFE_RELEASE_NULL(m_pCamera);
}
CWBOOL FrustumDemoScene::init()
{
	if (!cwScene::init()) return CWFALSE;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown = CW_CALLBACK_1(FrustumDemoScene::onTouchDown, this);
	pTouchListener->onTouchUp = CW_CALLBACK_1(FrustumDemoScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(FrustumDemoScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	cwKeyboardEventListener* pKeyListener = cwKeyboardEventListener::create();
	pKeyListener->onKeyDown = CW_CALLBACK_1(FrustumDemoScene::onKeyDown, this);
	pKeyListener->onKeyUp = CW_CALLBACK_1(FrustumDemoScene::onKeyUp, this);
	this->addEventListener(pKeyListener);

	this->schedulerUpdate();
	m_bTouchDown = false;

	buildEntity();
	buildLight();
	buildCamera();

	return CWTRUE;
}

CWVOID FrustumDemoScene::onTouchDown(cwTouch* pTouch)
{
	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;

	m_bTouchDown = true;
}

CWVOID FrustumDemoScene::onTouchUp(cwTouch* pTouch)
{
	m_bTouchDown = false;
}

CWVOID FrustumDemoScene::onTouchMoving(cwTouch* pTouch)
{
	if (m_bTouchDown) {
		CWFLOAT dx = cwMathUtil::angleRadian(pTouch->getScreenPos().x - m_fLastX);
		CWFLOAT dy = cwMathUtil::angleRadian(pTouch->getScreenPos().y - m_fLastY);

		cwRepertory::getInstance().getEngine()->getDefaultCamera()->yaw(dx);
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->pitch(-dy);
	}

	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;
}

CWVOID FrustumDemoScene::update(CWFLOAT dt)
{
	if (isKeyDown(KeyCode::A)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->strafe(-10 * dt);
	}
	else if (isKeyDown(KeyCode::D)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->strafe(10 * dt);
	}

	if (isKeyDown(KeyCode::W)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->walk(10 * dt);
	}
	else if (isKeyDown(KeyCode::S)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->walk(-10 * dt);
	}

	for (int i = 0; i < 5 * 5 * 5; ++i)
		cwRepertory::getInstance().getEngine()->getRenderer()->renderPrimitive(m_pCar[i]->getBoundingBox());

	cwRepertory::getInstance().getEngine()->getRenderer()->renderPrimitive(m_pCamera);
}

CWVOID FrustumDemoScene::buildEntity()
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
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormal");

	cwShader* pShader = repertory.getShaderManager()->getDefShader(eDefShaderLighting);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	cwMaterial* pMaterial = cwMaterial::create();

	for (CWUINT i = 0; i < 5; ++i) {
		for (CWUINT j = 0; j < 5; ++j) {
			for (CWUINT k = 0; k < 5; ++k) {

				CWUINT index = i * 5 * 5 + j * 5 + k;
				CWFLOAT fX = -50.0f + i*25.0f;
				CWFLOAT fY = -50.0f + j*25.0f;
				CWFLOAT fZ = -50.0f + k*25.0f;

				m_pCar[index] = cwEntity::create();
				m_pCar[index]->setMaterial(pMaterial);
				m_pCar[index]->setRenderObject(pRenderObj);
				m_pCar[index]->setPosition(cwVector3D(fX, fY, fZ));
				m_pCar[index]->setEffect(pEffect);
				CW_SAFE_RETAIN(m_pCar[index]);
				this->addChild(m_pCar[index]);

				cwLog::print("index:%d, pos:[%f,%f,%f]\n", index, fX, fY, fZ);
			}
		}
	}
}

CWVOID FrustumDemoScene::buildLight()
{
	cwDirectionalLight* pLightDirectional = cwDirectionalLight::create(
		cwVector4D(0, -1.0, 0, 0),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f));
	this->addDirectionalLight(pLightDirectional);

	cwPointLight* pLightPoint = cwPointLight::create(
		cwVector4D(30.0f, 80.0f, 0, 0),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector4D(0.0f, 0.03f, 0.0f, 1000.0f));
	this->addPointLight(pLightPoint);

	cwSpotLight* pLightSpot = cwSpotLight::create(
		cwVector4D(-20.0f, 50.0f, 0, 0),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.0f, 0.1f, 0.0f, 1000.0f),
		cwVector4D(0, -1.0, 0, 10.0f));
	this->addSpotLight(pLightSpot);
}

CWVOID FrustumDemoScene::buildCamera()
{
	m_pCamera = cwCamera::create();
	CW_SAFE_RETAIN(m_pCamera);

	m_pCamera->yaw(-0.5f*cwMathUtil::cwPI);
	m_pCamera->walk(-20);

	m_pCamera->updateProjMatrix(0.25*cwMathUtil::cwPI, 800.0f / 600.0f, 10.0f, 50.0f);
}
