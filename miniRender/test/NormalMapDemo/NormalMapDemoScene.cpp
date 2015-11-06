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
m_pNormalMapEffect(nullptr)
{

}

NormalMapDemoScene::~NormalMapDemoScene()
{
	CW_SAFE_RELEASE_NULL(m_pRenderCylinder);
	CW_SAFE_RELEASE_NULL(m_pNormalMapEffect);
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
	dt = 0.03f;
}

CWVOID NormalMapDemoScene::buildScene()
{
	buildRenderObject();
	buildEffect();

	cwEntity* pCylinder = createNormalMapCylinder();
	pCylinder->setPosition(cwVector3D(-6.0f, 0, 0));
	this->addChild(pCylinder);
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
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormalTexTan), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosNormalTexTan");
	CW_SAFE_RETAIN(m_pRenderCylinder);
}

CWVOID NormalMapDemoScene::buildEffect()
{
	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderNormalMap);
	m_pNormalMapEffect = cwEffect::create();
	m_pNormalMapEffect->setShader(pShader);
	m_pNormalMapEffect->setTech("NormalTech");
	CW_SAFE_RETAIN(m_pNormalMapEffect);
}

cwEntity* NormalMapDemoScene::createNormalMapCylinder()
{
	cwEntity* pEntity = cwEntity::create();
	pEntity->setRenderObject(m_pRenderCylinder);
	pEntity->setEffect(m_pNormalMapEffect);

	cwMaterialUnitTexture* pMUTexture = cwMaterialUnitTexture::create("Textures/bricks_nmap.dds", "gNormalTexture");
	pEntity->getMaterial()->addMaterialUnit(pMUTexture);
	pEntity->getMaterial()->setDiffuseTexture("Textures/bricks.dds");
	pEntity->getMaterial()->setSpecular(cwVector4D(1.0f, 1.0f, 1.0f, 32.0f));
	pEntity->getMaterial()->setReflect(cwVector4D(0.0f, 0.0f, 0.0f, 1.0f));

	return pEntity;
}

CWVOID NormalMapDemoScene::buildLight()
{
	cwDirectionalLight* pLightDirectional01 = cwDirectionalLight::create(
		cwVector4D(0.707f, 0.0f, 0.707f, 0),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector4D(0.7f, 0.7f, 0.6f, 1.0f),
		cwVector4D(0.8f, 0.8f, 0.7f, 1.0f));
	this->addDirectionalLight(pLightDirectional01);

	cwDirectionalLight* pLightDirectional02 = cwDirectionalLight::create(
		cwVector4D(0.0f, -0.707f, 0.707f, 0),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.4f, 0.4f, 0.4f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f));
	this->addDirectionalLight(pLightDirectional02);

	cwDirectionalLight* pLightDirectional03 = cwDirectionalLight::create(
		cwVector4D(-0.57735f, -0.57735f, -0.57735f, 0),
		cwVector4D(0.0f, 0.0f, 0.0f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f),
		cwVector4D(0.2f, 0.2f, 0.2f, 1.0f));
	this->addDirectionalLight(pLightDirectional03);
}
