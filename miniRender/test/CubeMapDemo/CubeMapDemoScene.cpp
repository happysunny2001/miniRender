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

#include "CubeMapDemoScene.h"

CubeMapDemoScene* CubeMapDemoScene::create()
{
	CubeMapDemoScene* pScene = new CubeMapDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

CubeMapDemoScene::CubeMapDemoScene():
m_pEntitySphere(nullptr),
m_pRenderObjSphere(nullptr),
m_pMatSphere(nullptr),
m_pEffectShpere(nullptr)
{

}

CubeMapDemoScene::~CubeMapDemoScene()
{
	CW_SAFE_RELEASE_NULL(m_pEntitySphere);
	CW_SAFE_RELEASE_NULL(m_pRenderObjSphere);
	CW_SAFE_RELEASE_NULL(m_pMatSphere);
	CW_SAFE_RELEASE_NULL(m_pEffectShpere);
}

CWBOOL CubeMapDemoScene::init()
{
	if (!cwBaseScene::init()) return CWFALSE;

	buildSphereMaterial();
	buildLight();
	buildScene();

	return CWTRUE;
}

CWVOID CubeMapDemoScene::update(CWFLOAT dt)
{
	m_pNodeZero->rotate(0, cwMathUtil::cwPI*0.2f*dt, 0);
}

CWVOID CubeMapDemoScene::buildSphereMaterial()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwGeometryGenerator::cwMeshData mesh;
	repertory.getGeoGenerator()->generateSphere(3.0f, 30, 30, mesh);

	vector<cwVertexPosNormal> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].normal = mesh.nVertex[i].normal;
	}

	m_pRenderObjSphere = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormal), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormal");
	CW_SAFE_RETAIN(m_pRenderObjSphere);

	m_pMatSphere = cwMaterial::create();
	CW_SAFE_RETAIN(m_pMatSphere);

	m_pMatSphere->setAmbient(cwVector4D(0.5f, 0.5f, 0.5f, 1.0f));
	m_pMatSphere->setDiffuse(cwVector4D(1.0f, 1.0f, 1.0f, 1.0f));
	m_pMatSphere->setSpecular(cwVector4D(0.9f, 0.9f, 0.9f, 16.0f));
	m_pMatSphere->setReflect(cwVector4D(0.4f, 0.4f, 0.4f, 1.0f));

	cwMaterialUnitDynamicReflect* pMatUnit = cwMaterialUnitDynamicReflect::create("snowcube1024.dds");
	pMatUnit->setReflectionFactor(0.5f);
	m_pMatSphere->addMaterialUnit(pMatUnit);
	
	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderLighting);
	m_pEffectShpere = cwEffect::create();
	m_pEffectShpere->setShader(pShader);
	m_pEffectShpere->setTech("LightTechReflect");
	CW_SAFE_RETAIN(m_pEffectShpere);
}

cwEntity* CubeMapDemoScene::buildSphere()
{
	cwEntity* pEntity = cwEntity::create();
	pEntity->setMaterial(m_pMatSphere);
	pEntity->setRenderObject(m_pRenderObjSphere);
	pEntity->setEffect(m_pEffectShpere);

	return pEntity;
}

CWVOID CubeMapDemoScene::buildScene()
{
	m_pEntitySphere = buildSphere();
	CW_SAFE_RETAIN(m_pEntitySphere);

	m_pNodeZero = cwRenderNode::create();
	m_pEntitySphere->setPosition(cwVector3D(8.0f, 0.0f, 0.0f));
	this->addChild(m_pNodeZero);
	m_pNodeZero->addChild(m_pEntitySphere);

	cwDynamicReflectionEntity* pReflectionEntity = cwDynamicReflectionEntity::create();
	pReflectionEntity->setRenderObject(m_pRenderObjSphere);
	pReflectionEntity->setPosition(cwVector3D::ZERO);
	pReflectionEntity->setDynamicRelfectionFactor(1.0f);

	cwMaterial* pMatReflection = pReflectionEntity->getMaterial();
	pMatReflection->setAmbient(cwVector4D(0.2f, 0.2f, 0.2f, 1.0f));
	pMatReflection->setDiffuse(cwVector4D(0.6f, 0.6f, 0.6f, 1.0f));
	pMatReflection->setSpecular(cwVector4D(0.8f, 0.8f, 0.8f, 16.0f));
	pMatReflection->setReflect(cwVector4D(1.0f, 1.0f, 1.0f, 1.0f));
	this->addChild(pReflectionEntity);

	this->createSkyDome("snowcube1024.dds");
}

CWVOID CubeMapDemoScene::buildLight()
{
	cwDirectionalLight* pLightDirectional01 = cwDirectionalLight::create(
		cwVector4D(0.57735f, -0.57735f, 0.57735f, 0),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f),
		cwVector4D(1.0f, 1.0f, 1.0f, 1.0f));
	this->addDirectionalLight(pLightDirectional01);

	cwDirectionalLight* pLightDirectional02 = cwDirectionalLight::create(
		cwVector4D(-0.57735f, -0.57735f, 0.57735f, 0),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector4D(0.25f, 0.25f, 0.25f, 1.0f));
	this->addDirectionalLight(pLightDirectional02);

	cwDirectionalLight* pLightDirectional03 = cwDirectionalLight::create(
		cwVector4D(0.0f, -0.707f, -0.707f, 0),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f));
	this->addDirectionalLight(pLightDirectional03);
}

