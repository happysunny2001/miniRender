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

#ifdef _CW_D3D11_

#include "cwD3D11TerrainTile.h"
#include "Math/cwMath.h"
#include "Repertory/cwRepertory.h"
#include "Texture/cwTextureManager.h"
#include "Material/cwMaterial.h"
#include "Material/MaterialUnit/cwMaterialUnitTexture.h"
#include "Material/MaterialUnit/cwMaterialUnitFloatArray.h"
#include "Engine/cwEngine.h"
#include "Camera/cwCamera.h"

#include <vector>

NS_MINIR_BEGIN

cwD3D11TerrainTile* cwD3D11TerrainTile::create(sTerrainTileData* pTerrainTileData)
{
	cwD3D11TerrainTile* pTerrainTile = new cwD3D11TerrainTile();
	if (pTerrainTile && pTerrainTile->init(pTerrainTileData)) {
		pTerrainTile->autorelease();
		return pTerrainTile;
	}

	CW_SAFE_DELETE(pTerrainTile);
	return nullptr;
}

cwD3D11TerrainTile::cwD3D11TerrainTile() : 
m_pMatUnitFloatArray(nullptr)
{

}

cwD3D11TerrainTile::~cwD3D11TerrainTile()
{
	CW_SAFE_RELEASE_NULL(m_pMatUnitFloatArray);
}

CWBOOL cwD3D11TerrainTile::init(sTerrainTileData* pTerrainTileData)
{
	if (!cwTerrainTile::init(pTerrainTileData)) return CWFALSE;

	return CWTRUE;
}

CWVOID cwD3D11TerrainTile::loadResource()
{
	m_pMaterial->clearMaterialUnit();

	buildHeightMapTexture();
	buildLayerTexture();
	buildBlendTexture();

	m_pMaterial->setAmbient(cwVector4D(1.0f, 1.0f, 1.0f, 1.0f));
	m_pMaterial->setDiffuse(cwVector4D(1.0f, 1.0f, 1.0f, 1.0f));
	m_pMaterial->setSpecular(cwVector4D(0, 0, 0, 64.0f));
	m_pMaterial->setReflect(cwVector4D(0, 0, 0, 1.0f));
}

CWVOID cwD3D11TerrainTile::buildHeightMapTexture()
{
	std::vector<CWHALF> vecHalfHeightMap(m_pTerrainTileData->heightMapSize());

	for (CWUINT i = 0; i < m_pTerrainTileData->heightMapSize(); ++i) {
		CWHALF h = cwMathUtil::convertFloatToHalf(m_pTerrainTileData->m_pHeightMap[i]);
		vecHalfHeightMap[i] = h;
	}

	cwTextureManager* pTexManager = cwRepertory::getInstance().getTextureManager();
	m_pTexHeightMap = pTexManager->createTexture(
		m_pTerrainTileData->m_nHeightMap.m_nStrTextureFile,
		&(m_pTerrainTileData->m_pHeightMap[0]),
		m_pTerrainTileData->m_iHeightMapWidth,
		m_pTerrainTileData->m_iHeightMapHeight,
		sizeof(CWFLOAT),
		eFormatR32Float);
	CW_SAFE_RETAIN(m_pTexHeightMap);
	
	if (m_pTexHeightMap) {
		cwMaterialUnitTexture* pMatUnitTex = cwMaterialUnitTexture::create(m_pTexHeightMap, m_pTerrainTileData->m_nHeightMap.m_nStrParamName);
		if (pMatUnitTex) {
			m_pMaterial->addMaterialUnit(pMatUnitTex);
		}
	}

	CWFLOAT* pFloatArray = (CWFLOAT*)(&(m_pTerrainTileData->m_nBoundY));
	cwMaterialUnitFloatArray* pMatUnitFloatArray = cwMaterialUnitFloatArray::create(pFloatArray, 2);
	if (pMatUnitFloatArray) {
		pMatUnitFloatArray->setShaderParam("gBoundY");
		m_pMaterial->addMaterialUnit(pMatUnitFloatArray);
	}

	m_pMatUnitFloatArray = cwMaterialUnitFloatArray::create(24);
	CW_SAFE_RETAIN(m_pMatUnitFloatArray);
	if (m_pMatUnitFloatArray) {
		m_pMatUnitFloatArray->setShaderParam("gWorldFrustumPlanes");
		m_pMaterial->addMaterialUnit(m_pMatUnitFloatArray);
	}
}

CWVOID cwD3D11TerrainTile::buildLayerTexture()
{
	cwTextureManager* pTexManager = cwRepertory::getInstance().getTextureManager();

	for (auto it = m_pTerrainTileData->m_nVecLayers.begin(); it != m_pTerrainTileData->m_nVecLayers.end(); ++it) {
		cwTexture* pTexture = pTexManager->getTexture(it->m_nStrTextureFile);
		m_nVecLayerTextures.pushBack(pTexture);

		cwMaterialUnitTexture* pMatUnitTex = cwMaterialUnitTexture::create(pTexture, it->m_nStrParamName);
		if (pMatUnitTex) {
			m_pMaterial->addMaterialUnit(pMatUnitTex);
		}
	}
}

CWVOID cwD3D11TerrainTile::buildBlendTexture()
{
	cwTextureManager* pTexManager = cwRepertory::getInstance().getTextureManager();

	for (auto it = m_pTerrainTileData->m_nVecBlend.begin(); it != m_pTerrainTileData->m_nVecBlend.end(); ++it) {
		cwTexture* pTexture = pTexManager->getTexture(it->m_nStrTextureFile);
		m_nVecBlendTextures.pushBack(pTexture);

		cwMaterialUnitTexture* pMatUnitTex = cwMaterialUnitTexture::create(pTexture, it->m_nStrParamName);
		if (pMatUnitTex) {
			m_pMaterial->addMaterialUnit(pMatUnitTex);
		}
	}
}

CWVOID cwD3D11TerrainTile::render(cwRenderBatch* pRenderBatch)
{
	if (m_pMaterial && m_pMatUnitFloatArray) {
		cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getDefaultCamera();
		if (pCamera) {
			const cwFrustum& frustum = pCamera->getFrustum();
			const CWFLOAT* pFloats = (const CWFLOAT*)(frustum.getPlane());
			m_pMatUnitFloatArray->refresh(pFloats);
		}
	}

	cwTerrainTile::render(pRenderBatch);
}

NS_MINIR_END

#endif
