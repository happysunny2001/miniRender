/*
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

#include "DeferredShadingScene.h"

DeferredShadingScene* DeferredShadingScene::create()
{
	DeferredShadingScene* pScene = new DeferredShadingScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

DeferredShadingScene::DeferredShadingScene() :
m_pPlaneObject(nullptr),
m_pSphereObject(nullptr),
m_pEntityPlane(nullptr),
m_pEntitySphere(nullptr),
m_pBoxObject(nullptr),
m_pEntityBox01(nullptr),
m_pEntityBox02(nullptr),
m_pEntityBox03(nullptr)
{

}

DeferredShadingScene::~DeferredShadingScene()
{
	CW_SAFE_RELEASE_NULL(m_pPlaneObject);
	CW_SAFE_RELEASE_NULL(m_pSphereObject);
	CW_SAFE_RELEASE_NULL(m_pEntityPlane);
	CW_SAFE_RELEASE_NULL(m_pEntitySphere);
	CW_SAFE_RELEASE_NULL(m_pBoxObject);
	CW_SAFE_RELEASE_NULL(m_pEntityBox01);
	CW_SAFE_RELEASE_NULL(m_pEntityBox02);
	CW_SAFE_RELEASE_NULL(m_pEntityBox03);
}

CWBOOL DeferredShadingScene::init()
{
	if (!cwBaseScene::init()) return CWFALSE;

	buildRenderObject();
	buildEntity();
	buildLight();

	this->initScene();

	return CWTRUE;
}

CWVOID DeferredShadingScene::update(CWFLOAT dt)
{

}

CWVOID DeferredShadingScene::buildRenderObject()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateGrid(100, 100, 20, 20, mesh);

	vector<cwVertexPosNormal> vecPlane(mesh.nVertex.size());
	for (CWUINT i = 0; i < mesh.nVertex.size(); ++i) {
		vecPlane[i].pos = mesh.nVertex[i].pos;
		vecPlane[i].normal = mesh.nVertex[i].normal;
	}

	m_pPlaneObject = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecPlane[0], sizeof(cwVertexPosNormal), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormal");
	CW_SAFE_RETAIN(m_pPlaneObject);

	repertory.getGeoGenerator()->generateBox(mesh);
	vector<cwVertexPosNormal> vecBox(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecBox[i].pos = mesh.nVertex[i].pos;
		vecBox[i].normal = mesh.nVertex[i].normal;
	}

	m_pBoxObject = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecBox[0], sizeof(cwVertexPosNormal), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormal");
	CW_SAFE_RETAIN(m_pBoxObject);

	repertory.getGeoGenerator()->generateSphere(1.0, 20, 20, mesh);
	vector<cwVertexPosNormal> vecSphere(mesh.nVertex.size());
	for (CWUINT i = 0; i < mesh.nVertex.size(); ++i) {
		vecSphere[i].pos = mesh.nVertex[i].pos;
		vecSphere[i].normal = mesh.nVertex[i].normal;
	}

	m_pSphereObject = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecSphere[0], sizeof(cwVertexPosNormal), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormal");
	CW_SAFE_RETAIN(m_pSphereObject);
}

CWVOID DeferredShadingScene::buildEntity()
{
	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderLighting);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);
	pEffect->setTech("LightTech");

	cwMaterial* pMaterial = cwMaterial::create();
	pMaterial->setDiffuse(cwVector4D(0.8f, 0.8f, 0.8f, 1.0f));

	m_pEntityPlane = cwEntity::create();
	m_pEntityPlane->setRenderObject(m_pPlaneObject);
	m_pEntityPlane->setEffect(pEffect);
	m_pEntityPlane->setCastShadow(CWFALSE);
	CW_SAFE_RETAIN(m_pEntityPlane);

	m_pEntitySphere = cwEntity::create();
	m_pEntitySphere->setRenderObject(m_pSphereObject);
	m_pEntitySphere->setEffect(pEffect);
	m_pEntitySphere->setMaterial(pMaterial);
	CW_SAFE_RETAIN(m_pEntitySphere);

	m_pEntityBox01 = cwEntity::create();
	m_pEntityBox01->setRenderObject(m_pBoxObject);
	m_pEntityBox01->setEffect(pEffect);
	m_pEntityBox01->setMaterial(pMaterial);
	CW_SAFE_RETAIN(m_pEntityBox01);

	m_pEntityBox02 = cwEntity::create();
	m_pEntityBox02->setRenderObject(m_pBoxObject);
	m_pEntityBox02->setEffect(pEffect);
	m_pEntityBox02->setMaterial(pMaterial);
	CW_SAFE_RETAIN(m_pEntityBox02);

	m_pEntityBox03 = cwEntity::create();
	m_pEntityBox03->setRenderObject(m_pBoxObject);
	m_pEntityBox03->setEffect(pEffect);
	m_pEntityBox03->setMaterial(pMaterial);
	CW_SAFE_RETAIN(m_pEntityBox03);
}

CWVOID DeferredShadingScene::buildLight()
{
	cwVector4D dir = cwVector4D(-1.0, -1.0, 0, 0);
	dir.normalize();

	m_pLightDirectional = cwDirectionalLight::create(
		dir,
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.6f, 0.6f, 0.6f, 1.0f),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLightDirectional->setCastShadow(CWFALSE);
	this->addDirectionalLight(m_pLightDirectional);

	//cwPointLight* pLightPoint = cwPointLight::create(
	//	cwVector4D(20.0f, 20.0f, 0.0f, 1.0f),
	//	cwVector4D::ZERO,
	//	cwVector4D(0.8f, 0.8f, 0.8f, 1.0f),
	//	cwVector4D(1.0f, 1.0f, 1.0f, 8.0f),
	//	cwVector4D(0.0f, 0.05f, 0.0f, 50.0f));
	//this->addPointLight(pLightPoint);
}

CWVOID DeferredShadingScene::initScene()
{
	m_pEntityPlane->setPosition(0, 0, 0);
	this->addChild(m_pEntityPlane);

	m_pEntityBox01->setScale(4.0f, 4.0f, 4.0f);
	m_pEntityBox01->setPosition(0, 2.0f, 0);
	this->addChild(m_pEntityBox01);

	m_pEntityBox02->setScale(4.0f, 16.0f, 4.0f);
	m_pEntityBox02->setRotation(0, cwMathUtil::angleRadian(30), 0);
	m_pEntityBox02->setPosition(6.0, 8.0f, 0);
	this->addChild(m_pEntityBox02);

	m_pEntityBox03->setScale(4.0f, 4.0f, 4.0f);
	m_pEntityBox03->setRotation(0, -cwMathUtil::angleRadian(30), 0);
	m_pEntityBox03->setPosition(-6.0, 2.0f, -5.0);
	this->addChild(m_pEntityBox03);

	m_pEntitySphere->setScale(4.0f, 4.0f, 4.0f);
	m_pEntitySphere->setPosition(0, 10.0f, -10.0f);
	this->addChild(m_pEntitySphere);
}
