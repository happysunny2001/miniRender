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

#include "cwTerrainTile.h"
#include "Repertory/cwRepertory.h"
#include "Resource/cwResourceLoader.h"
#include "Material/cwMaterial.h"
#include "Texture/cwTextureManager.h"

NS_MINIR_BEGIN

CWBOOL sTerrainTileData::loadHeightMap(CWFLOAT fScale)
{
	if (m_bLoaded) return CWTRUE;

	CW_SAFE_DELETE_ARRAY(m_pHeightMap);

	cwData* pData = cwRepertory::getInstance().getResourceLoader()->getFileData(m_nHeightMap.m_nStrTextureFile);
	if (!pData) return CWFALSE;
	if (!pData->m_pData) {
		CW_SAFE_DELETE(pData);
		return CWFALSE;
	}

	m_pHeightMap = new CWFLOAT[m_iHeightMapWidth*m_iHeightMapHeight];
	if (!m_pHeightMap) {
		CW_SAFE_DELETE(pData);
		return CWFALSE;
	}

	for (CWUINT64 i = 0; i < pData->m_uSize; ++i) {
		m_pHeightMap[i] = (pData->m_pData[i] / 255.0f)*fScale;
	}

	CW_SAFE_DELETE(pData);

	smooth();
	createBoundY();

	m_bLoaded = CWTRUE;

	return CWTRUE;
}

CWVOID sTerrainTileData::releaseHeightMap()
{
	CW_SAFE_DELETE_ARRAY(m_pHeightMap);
	m_bLoaded = CWFALSE;
}

CWBOOL sTerrainTileData::isValid(CWINT i, CWINT j)
{
	if (i >= (CWINT)m_iHeightMapWidth || j >= (CWINT)m_iHeightMapHeight ||
		i < 0 || j < 0) return CWFALSE;
	return CWTRUE;
}

CWFLOAT sTerrainTileData::average(CWINT i, CWINT j)
{
	CWFLOAT fAvg = 0;
	CWUINT iCnt = 0;

	for (CWINT m = j - 1; m <= j + 1; ++m) {
		for (CWINT n = i - 1; n <= i + 1; ++n) {
			if (isValid(n, m)) {
				fAvg += m_pHeightMap[m*m_iHeightMapWidth+n];
				iCnt++;
			}
		}
	}

	if (iCnt == 0) return 0;
	return fAvg / iCnt;
}

CWVOID sTerrainTileData::smooth()
{
	if (!m_pHeightMap) return;
	CWFLOAT* pSmoothHeightMap = new CWFLOAT[m_iHeightMapWidth*m_iHeightMapHeight];
	if (!pSmoothHeightMap) return;

	for (CWUINT j = 0; j < m_iHeightMapHeight; ++j) {
		for (CWUINT i = 0; i < m_iHeightMapWidth; ++i) {
			pSmoothHeightMap[j*m_iHeightMapWidth + i] = average(i, j);
		}
	}

	CW_SAFE_DELETE_ARRAY(m_pHeightMap);
	m_pHeightMap = pSmoothHeightMap;
}

CWVOID sTerrainTileData::createBoundY()
{
	CWFLOAT fMinY = cwMathUtil::cwInfinity;
	CWFLOAT fMaxY = -cwMathUtil::cwInfinity;

	for (CWUINT j = 0; j < m_iHeightMapHeight; ++j) {
		for (CWUINT i = 0; i < m_iHeightMapWidth; ++i) {
			CWFLOAT fHeight = m_pHeightMap[j*m_iHeightMapWidth + i];
			fMinY = std::min(fMinY, fHeight);
			fMaxY = std::max(fMaxY, fHeight);
		}
	}

	m_nBoundY.set(fMinY, fMaxY);
}

CWVOID sTerrainTileData::loadTextures()
{
	cwRepertory& repertory = cwRepertory::getInstance();
	cwTextureManager* pTexManager = repertory.getTextureManager();

	for (auto it = m_nVecLayers.begin(); it != m_nVecLayers.end(); ++it) {
		pTexManager->createTexture(it->m_nStrTextureFile);
	}

	for (auto it = m_nVecBlend.begin(); it != m_nVecBlend.end(); ++it) {
		pTexManager->createTexture(it->m_nStrTextureFile);
	}
}

cwTerrainTile::cwTerrainTile() : 
m_pTerrainTileData(nullptr),
m_pTexHeightMap(nullptr)
{

}

cwTerrainTile::~cwTerrainTile()
{
	m_pTerrainTileData = nullptr;
	releaseResource();
}

CWBOOL cwTerrainTile::init(sTerrainTileData* pTerrainTileData)
{
	if (!cwEntity::init()) return CWFALSE;
	if (!pTerrainTileData) return CWFALSE;

	m_pTerrainTileData = pTerrainTileData;

	return CWTRUE;
}

CWVOID cwTerrainTile::releaseResource()
{
	CW_SAFE_RELEASE_NULL(m_pTexHeightMap);

	m_pMaterial->clearMaterialUnit();
	m_nVecLayerTextures.clear();
	m_nVecBlendTextures.clear();
}

CWVOID cwTerrainTile::refreshBoundingBox()
{

}

NS_MINIR_END
