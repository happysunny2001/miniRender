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

#include "NormalMapDemoScene.h"

NormalMapDemoScene* NormalMapDemoScene::create()
{
	NormalMapDemoScene* pScene = new NormalMapDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

NormalMapDemoScene::NormalMapDemoScene():
m_pRenderCylinder(nullptr),
m_pNormalMapEffect(nullptr),
m_pDisplacementEffect(nullptr),
m_pRenderSphere(nullptr),
m_pRenderPlane(nullptr),
m_pWaveEffect(nullptr),
m_pEntityWave(nullptr)
{

}

NormalMapDemoScene::~NormalMapDemoScene()
{
	CW_SAFE_RELEASE_NULL(m_pRenderCylinder);
	CW_SAFE_RELEASE_NULL(m_pNormalMapEffect);
	CW_SAFE_RELEASE_NULL(m_pDisplacementEffect);
	CW_SAFE_RELEASE_NULL(m_pRenderSphere);
	CW_SAFE_RELEASE_NULL(m_pRenderPlane);
	CW_SAFE_RELEASE_NULL(m_pWaveEffect);
}

CWBOOL NormalMapDemoScene::init()
{
	if (!cwBaseScene::init()) return CWFALSE;

	buildScene();
	buildLight();

	return CWTRUE;
}

CWVOID NormalMapDemoScene::update(CWFLOAT dt)
{
	for (CWUINT i = 0; i < 4; ++i) {
		m_pMatUnitMatrix[i]->move(m_nMatMoveDir[i]*dt);
	}

	dt = 0.03f;
}

CWVOID NormalMapDemoScene::buildScene()
{
	buildRenderObject();
	buildEffect();

	cwEntity* pCylinder = createNormalMapCylinder();
	pCylinder->setPosition(cwVector3D(-6.0f, 0, 0));
	this->addChild(pCylinder);

	cwEntity* pSphere = createNormalMapSphere();
	pSphere->setPosition(cwVector3D(20.0f, 0, 0));
	this->addChild(pSphere);

	m_pEntityWave = createWavePlane();
	m_pEntityWave->setPosition(cwVector3D(0, -10, 0));
	this->addChild(m_pEntityWave);

	this->createSkyDome("Textures/snowcube1024.dds");
}

CWVOID NormalMapDemoScene::buildRenderObject()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateCylinder(5.0f, 5.0f, 20.0f, 10, 10, mesh);

	vector<cwVertexPosNormalTexTan> vecVertex(mesh.nVertex.size());
	for (CWUINT i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos    = mesh.nVertex[i].pos;
		vecVertex[i].normal = mesh.nVertex[i].normal;
		vecVertex[i].tex    = mesh.nVertex[i].tex;
		vecVertex[i].tan    = mesh.nVertex[i].tangentU;
	}

	m_pRenderCylinder = cwStaticRenderObject::create(
		ePrimitiveTypePatchList3,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormalTexTan), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormalTexTan");
	CW_SAFE_RETAIN(m_pRenderCylinder);

	mesh.nVertex.clear();
	mesh.nIndex.clear();

	repertory.getGeoGenerator()->generateSphere(10.0f, 10, 10, mesh);
	vecVertex.resize(mesh.nVertex.size());

	for (CWUINT i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].normal = mesh.nVertex[i].normal;
		vecVertex[i].tex = mesh.nVertex[i].tex*1.0f;
		vecVertex[i].tan = mesh.nVertex[i].tangentU;
	}

	m_pRenderSphere = cwStaticRenderObject::create(
		ePrimitiveTypePatchList3,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormalTexTan), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormalTexTan");
	CW_SAFE_RETAIN(m_pRenderSphere);

	mesh.nVertex.clear();
	mesh.nIndex.clear();

	repertory.getGeoGenerator()->generateGrid(200, 200, 50, 50, mesh);
	vecVertex.resize(mesh.nVertex.size());

	for (CWUINT i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].normal = mesh.nVertex[i].normal;
		vecVertex[i].tex = mesh.nVertex[i].tex*10.0f;
		vecVertex[i].tan = mesh.nVertex[i].tangentU;
	}

	m_pRenderPlane = cwStaticRenderObject::create(
		ePrimitiveTypePatchList3,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormalTexTan), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormalTexTan");
	CW_SAFE_RETAIN(m_pRenderPlane);
}

CWVOID NormalMapDemoScene::buildEffect()
{
	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderNormalMap);
	m_pNormalMapEffect = cwEffect::create();
	m_pNormalMapEffect->setShader(pShader);
	m_pNormalMapEffect->setTech("NormalTech");
	CW_SAFE_RETAIN(m_pNormalMapEffect);

	cwShader* pShaderDisplacement = cwRepertory::getInstance().getShaderManager()->getShader("effect/D3D11/displacementMap.fx");
	m_pDisplacementEffect = cwEffect::create();
	m_pDisplacementEffect->setShader(pShaderDisplacement);
	m_pDisplacementEffect->setTech("TechDisplacement");
	CW_SAFE_RETAIN(m_pDisplacementEffect);

	cwShader* pShaderWave = cwRepertory::getInstance().getShaderManager()->getShader("effect/D3D11/wave.fx");

	m_pWaveEffect = cwEffect::create();
	m_pWaveEffect->setShader(pShaderWave);
	m_pWaveEffect->setTech("TechWave");
	CW_SAFE_RETAIN(m_pWaveEffect);

	CWSTRING strParams[] = {"gHeightScale", "gMaxTessDistance", "gMinTessDistance", "gMaxTessFactor", "gMinTessFactor"};
	CWFLOAT fParamValues[] = {0.5f, 1.0f, 50.0f, 10.0f, 1.0f};

	for (CWUINT i = 0; i < 5; ++i) {
		cwEffectFloatParameter* pParam = cwEffectFloatParameter::create();
		pParam->setParameterName(strParams[i]);
		pParam->m_fValue = fParamValues[i];

		m_pDisplacementEffect->addParameter(pParam);
	}

	CWSTRING strParamsWave[] = { "gHeightScale0", "gHeightScale1", "gMaxTessDistance", "gMinTessDistance", "gMaxTessFactor", "gMinTessFactor" };
	CWFLOAT fParamValuesWave[] = { 0.4f, 1.2f, 1.0f, 50.0f, 10.0f, 1.0f };

	for (CWUINT i = 0; i < 6; ++i) {
		cwEffectFloatParameter* pParam = cwEffectFloatParameter::create();
		pParam->setParameterName(strParamsWave[i]);
		pParam->m_fValue = fParamValuesWave[i];

		m_pWaveEffect->addParameter(pParam);
	}
}

cwEntity* NormalMapDemoScene::createNormalMapCylinder()
{
	cwEntity* pEntity = cwEntity::create();
	pEntity->setRenderObject(m_pRenderCylinder);
	pEntity->setEffect(m_pDisplacementEffect);

	cwMaterialUnitTexture* pMUTexture = cwMaterialUnitTexture::create("Textures/bricks_nmap.dds", "gNormalTexture");
	pEntity->getMaterial()->addMaterialUnit(pMUTexture);
	pEntity->getMaterial()->setDiffuseTexture("Textures/bricks.dds");
	pEntity->getMaterial()->setSpecular(cwVector4D(1.0f, 1.0f, 1.0f, 32.0f));
	pEntity->getMaterial()->setReflect(cwVector4D(0.0f, 0.0f, 0.0f, 1.0f));

	return pEntity;
}

cwEntity* NormalMapDemoScene::createNormalMapSphere()
{
	cwEntity* pEntity = cwEntity::create();
	pEntity->setRenderObject(m_pRenderSphere);
	pEntity->setEffect(m_pDisplacementEffect);

	cwMaterialUnitTexture* pMUTexture = cwMaterialUnitTexture::create("Textures/stone01_NRM.dds", "gNormalTexture");
	pEntity->getMaterial()->addMaterialUnit(pMUTexture);
	pEntity->getMaterial()->setDiffuseTexture("Textures/stone01.png");
	pEntity->getMaterial()->setDiffuse(cwVector4D(0.8f, 0.8f, 0.8f, 1.0f));
	pEntity->getMaterial()->setSpecular(cwVector4D(0.1f, 0.1f, 0.1f, 16.0f));
	pEntity->getMaterial()->setReflect(cwVector4D(0.0f, 0.0f, 0.0f, 1.0f));

	return pEntity;
}

cwEntity* NormalMapDemoScene::createWavePlane()
{
	cwEntity* pEntity = cwEntity::create();
	pEntity->setRenderObject(m_pRenderPlane);
	pEntity->setEffect(m_pWaveEffect);

	cwMaterialUnitTexture* pMUTexture0 = cwMaterialUnitTexture::create("Textures/waves0.dds", "gWaveTexture0");
	pEntity->getMaterial()->addMaterialUnit(pMUTexture0);

	cwMaterialUnitTexture* pMUTexture1 = cwMaterialUnitTexture::create("Textures/waves1.dds", "gWaveTexture1");
	pEntity->getMaterial()->addMaterialUnit(pMUTexture1);

	cwMaterialUnitTexture* pMUSky = cwMaterialUnitTexture::createCube("Textures/snowcube1024.dds", CW_SHADER_SKY_CUBE_MAP);
	pEntity->getMaterial()->addMaterialUnit(pMUSky);

	pEntity->getMaterial()->setAmbient(cwVector4D(0.1f, 0.1f, 0.3f, 1.0f));
	pEntity->getMaterial()->setDiffuse(cwVector4D(0.4f, 0.4f, 0.7f, 1.0f));
	pEntity->getMaterial()->setSpecular(cwVector4D(0.8f, 0.8f, 0.8f, 128.0f));
	pEntity->getMaterial()->setReflect(cwVector4D(0.4f, 0.4f, 0.4f, 1.0f));

	CWSTRING strShaderParam[4] = {"gWavevDispTexTransform0", "gWavevDispTexTransform1", "gWavevNormalTexTransform0", "gWavevNormalTexTransform1"};

	for (CWUINT i = 0; i < 4; ++i) {
		m_pMatUnitMatrix[i] = cwMaterialUnitMatrix::create();
		m_pMatUnitMatrix[i]->setShaderParam(strShaderParam[i]);
		pEntity->getMaterial()->addMaterialUnit(m_pMatUnitMatrix[i]);
	}

	m_nMatMoveDir[0].set(0.01f, 0.03f);
	m_nMatMoveDir[1].set(0.01f, 0.03f);
	m_nMatMoveDir[2].set(0.01f, 0.2f);
	m_nMatMoveDir[3].set(-0.02f, 0.05f);

	return pEntity;
}

CWVOID NormalMapDemoScene::buildLight()
{
	cwDirectionalLight* pLightDirectional01 = cwDirectionalLight::create(
		cwVector4D(0.707f, 0.0f, 0.707f, 0),
		cwVector4D(0.3f, 0.3f, 0.3f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.6f, 1.0f),
		cwVector4D(0.8f, 0.8f, 0.7f, 1.0f));
	this->addDirectionalLight(pLightDirectional01);

	cwDirectionalLight* pLightDirectional02 = cwDirectionalLight::create(
		cwVector4D(0.0f, -0.707f, 0.707f, 0),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.5f, 0.5f, 0.5f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f));
	this->addDirectionalLight(pLightDirectional02);

	cwDirectionalLight* pLightDirectional03 = cwDirectionalLight::create(
		cwVector4D(-0.57735f, -0.57735f, -0.57735f, 0),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f));
	this->addDirectionalLight(pLightDirectional03);
}
