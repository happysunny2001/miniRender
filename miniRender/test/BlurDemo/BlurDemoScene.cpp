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

#include "BlurDemoScene.h"

BlurDemoScene* BlurDemoScene::create()
{
	BlurDemoScene* pScene = new BlurDemoScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}

	CW_SAFE_DELETE(pScene);
	return nullptr;
}

BlurDemoScene::BlurDemoScene()
{

}

BlurDemoScene::~BlurDemoScene()
{

}

CWBOOL BlurDemoScene::init()
{
	if (!cwBaseScene::init()) return CWFALSE;

	buildEntity();
	buildLight();

	return CWTRUE;
}

CWVOID BlurDemoScene::buildEntity()
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

	cwTexture* pTexTerrain = cwRepertory::getInstance().getTextureManager()->getTexture("Textures/grass.dds");
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

CWVOID BlurDemoScene::buildLight()
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
