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

#include "GeometryShaderDemoScene.h"

GeometryShaderDemoScene* GeometryShaderDemoScene::create()
{
	GeometryShaderDemoScene* pScene = new GeometryShaderDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

GeometryShaderDemoScene::GeometryShaderDemoScene()
{

}

GeometryShaderDemoScene::~GeometryShaderDemoScene()
{

}

CWBOOL GeometryShaderDemoScene::init()
{
	if (!cwScene::init()) return CWFALSE;

	cwTouchEventListener* pTouchListener = cwTouchEventListener::create();
	pTouchListener->onTouchDown = CW_CALLBACK_1(GeometryShaderDemoScene::onTouchDown, this);
	pTouchListener->onTouchUp = CW_CALLBACK_1(GeometryShaderDemoScene::onTouchUp, this);
	pTouchListener->onTouchMoving = CW_CALLBACK_1(GeometryShaderDemoScene::onTouchMoving, this);
	this->addEventListener(pTouchListener);

	this->schedulerUpdate();

	m_bTouchDown = false;

	buildLight();
	buildTerrain();
	buildBillboard();

	return CWTRUE;
}

CWVOID GeometryShaderDemoScene::onTouchDown(cwTouch* pTouch)
{
	m_fLastX = pTouch->getScreenPos().x;
	m_fLastY = pTouch->getScreenPos().y;

	m_bTouchDown = true;
}

CWVOID GeometryShaderDemoScene::onTouchUp(cwTouch* pTouch)
{
	m_bTouchDown = false;
}

CWVOID GeometryShaderDemoScene::onTouchMoving(cwTouch* pTouch)
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

CWVOID GeometryShaderDemoScene::update(CWFLOAT dt)
{
	if (isKeyDown(KeyCode::A)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->strafe(-50 * dt);
	}
	else if (isKeyDown(KeyCode::D)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->strafe(50 * dt);
	}

	if (isKeyDown(KeyCode::W)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->walk(50 * dt);
	}
	else if (isKeyDown(KeyCode::S)) {
		cwRepertory::getInstance().getEngine()->getDefaultCamera()->walk(-50 * dt);
	}
}

CWVOID GeometryShaderDemoScene::buildTerrain()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateGrid(200.0f, 200.0f, 80, 80, mesh);

	vector<cwVertexPosNormalTex> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].pos.y = repertory.getGeoGenerator()->getTerrainHeight(vecVertex[i].pos.x, vecVertex[i].pos.z);
		vecVertex[i].normal = repertory.getGeoGenerator()->getTerrainNormal(vecVertex[i].pos.x, vecVertex[i].pos.z);
		vecVertex[i].tex = mesh.nVertex[i].tex;
	}

	cwRenderObject* pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormalTex), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormalTex");

	cwTexture* pTexTerrain = cwRepertory::getInstance().getTextureManager()->getTexture("grass.dds");
	cwShader* pShader = repertory.getShaderManager()->getDefShader(eDefShaderLightingTex);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	cwMaterial* pMaterial = cwMaterial::create();
	if (pTexTerrain) {
		pMaterial->setDiffuseTexture(pTexTerrain);
	}

	cwEntity* pTerrain = cwEntity::create();
	pTerrain->setMaterial(pMaterial);
	pTerrain->setEffect(pEffect);
	pTerrain->setRenderObject(pRenderObj);
	pTerrain->setPosition(cwVector3D::ZERO);
	this->addChild(pTerrain);
}

CWVOID GeometryShaderDemoScene::buildBillboard()
{
	cwShader* pBillboardShader = cwRepertory::getInstance().getShaderManager()->getShader("geometryBillboard.fx");
	std::vector<CWSTRING> vecTexFiles;
	vecTexFiles.push_back("Textures/tree0.dds");
	vecTexFiles.push_back("Textures/tree1.dds");
	vecTexFiles.push_back("Textures/tree2.dds");
	vecTexFiles.push_back("Textures/tree3.dds");
	cwTexture* pTreeTexture = cwRepertory::getInstance().getTextureManager()->getTextureArray(vecTexFiles);

	vector<cwVertexPosTex> vecVertex(4);

	vecVertex[0].pos = cwVector3D(10, 20, 50);
	vecVertex[0].tex = cwVector2D(30, 30);

	vecVertex[1].pos = cwVector3D(-20, 20, 50);
	vecVertex[1].tex = cwVector2D(30, 30);

	vecVertex[2].pos = cwVector3D(10, 20, -10);
	vecVertex[2].tex = cwVector2D(30, 30);

	vecVertex[3].pos = cwVector3D(60, 20, 100);
	vecVertex[3].tex = cwVector2D(30, 30);

	cwRenderObject* pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypePointList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosTex), 4,
		NULL, 0, "PosTex");

	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pBillboardShader);

	cwEffectTextureParameter* pEffectParam = cwEffectTextureParameter::create();
	pEffectParam->setParameterName("gTreeMapArray");
	pEffectParam->setTexture(pTreeTexture);
	pEffect->addParameter(pEffectParam);

	cwBlendData blendData;
	blendData.bAlphaToCoverage = CWTRUE;
	blendData.bEnable = CWFALSE;
	cwBlend* pBlend = cwRepertory::getInstance().getDevice()->createBlend(blendData);

	cwMaterial* pMaterial = cwMaterial::create();
	cwEntity* pBillboard = cwEntity::create();
	pBillboard->setMaterial(pMaterial);
	pBillboard->setBlend(pBlend);
	pBillboard->setEffect(pEffect);
	pBillboard->setRenderObject(pRenderObj);
	pBillboard->setPosition(cwVector3D::ZERO);
	this->addChild(pBillboard);
}

CWVOID GeometryShaderDemoScene::buildLight()
{
	cwDirectionalLight* pLightDirectional = cwDirectionalLight::create(
		cwVector4D(0, -1.0, 0, 0),
		cwVector4D(0.5f, 0.5f, 0.5f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f));
	this->addDirectionalLight(pLightDirectional);

	cwPointLight* pLightPoint = cwPointLight::create(
		cwVector4D(50.0f, 100.0f, 0, 0),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.0f, 0.1f, 0.0f, 1000.0f));
	this->addPointLight(pLightPoint);

	cwSpotLight* pLightSpot = cwSpotLight::create(
		cwVector4D(-50.0f, 100.0f, 0, 0),
		cwVector4D(0.1f, 0.1f, 0.1f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.7f, 1.0f),
		cwVector4D(0.0f, 0.1f, 0.0f, 1000.0f),
		cwVector4D(0, -1.0, 0, 48.0f));
	this->addSpotLight(pLightSpot);
}
