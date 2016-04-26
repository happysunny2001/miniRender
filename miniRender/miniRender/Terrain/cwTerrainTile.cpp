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
#include "Base/cwLog.h"
#include "cwTerrain.h"

NS_MINIR_BEGIN

sTerrainTileData* sTerrainTileData::create()
{
	sTerrainTileData* pTerrainData = new sTerrainTileData();
	if (pTerrainData) {
		pTerrainData->autorelease();
		return pTerrainData;
	}

	return nullptr;
}

CWBOOL sTerrainTileData::loadHeightMap(CWFLOAT fScale)
{
	if (m_eState != eTerrainTileLoading) return CWTRUE;

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

	return CWTRUE;
}

CWVOID sTerrainTileData::releaseHeightMap()
{
	CW_SAFE_DELETE_ARRAY(m_pHeightMap);
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

	m_nBoundingBox.m_nMin.y = m_nBoundY.x;
	m_nBoundingBox.m_nMax.y = m_nBoundY.y;
}

CWVOID sTerrainTileData::updateBoundingBox()
{
	CWFLOAT fBoxOffsetWidth = m_pTerrainData->terrainTileWidth()*m_pTerrainData->m_fBoundingBoxOffsetScale;
	CWFLOAT fBoxOffsetHeight = m_pTerrainData->terrainTileHeight()*m_pTerrainData->m_fBoundingBoxOffsetScale;

	m_nBoxLoad = m_nBoundingBox;
	m_nBoxLoad.m_nMin.x -= fBoxOffsetWidth;
	m_nBoxLoad.m_nMax.x += fBoxOffsetWidth;
	m_nBoxLoad.m_nMin.z -= fBoxOffsetHeight;
	m_nBoxLoad.m_nMax.z += fBoxOffsetHeight;

	m_nBoxRelease = m_nBoundingBox;
	m_nBoxRelease.m_nMin.x -= fBoxOffsetWidth*2.0f;
	m_nBoxRelease.m_nMax.x += fBoxOffsetWidth*2.0f;
	m_nBoxRelease.m_nMin.z -= fBoxOffsetHeight*2.0f;
	m_nBoxRelease.m_nMax.z += fBoxOffsetHeight*2.0f;
}

CWVOID sTerrainTileData::loadResources()
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
	CW_SAFE_RELEASE_NULL(m_pTerrainTileData);
	releaseResource();

	cwLog::print("cwTerrainTile::~cwTerrainTile.\n");
}

CWBOOL cwTerrainTile::init(sTerrainTileData* pTerrainTileData)
{
	if (!cwEntity::init()) return CWFALSE;
	if (!pTerrainTileData) return CWFALSE;

	this->setTerrainTileData(pTerrainTileData);

	return CWTRUE;
}

CWFLOAT cwTerrainTile::getHeight(const cwVector3D& pos)
{
	CWFLOAT fXOffset = pos.x - m_nBoundingBox.m_nMin.x;
	CWFLOAT fZOffset = m_nBoundingBox.m_nMax.z - pos.z;

	CWFLOAT fIndexX = fXOffset / m_pTerrainTileData->m_pTerrainData->m_fCellSpace;
	CWFLOAT fIndexZ = fZOffset / m_pTerrainTileData->m_pTerrainData->m_fCellSpace;

	CWINT iIndexX = (CWINT)floor(fIndexX);
	CWINT iIndexZ = (CWINT)floor(fIndexZ);

	CWUINT iHeightMapWidth = m_pTerrainTileData->m_iHeightMapWidth;
	CWUINT iHeightMapHeight = m_pTerrainTileData->m_iHeightMapHeight;

	if (iIndexX < 0 || iIndexX >= (CWINT)(iHeightMapWidth - 1) ||
		iIndexZ < 0 || iIndexZ >= (CWINT)(iHeightMapHeight - 1)) return 0;

	CWFLOAT A = m_pTerrainTileData->m_pHeightMap[iIndexZ*iHeightMapWidth + iIndexX];
	CWFLOAT B = m_pTerrainTileData->m_pHeightMap[iIndexZ*iHeightMapWidth + iIndexX + 1];
	CWFLOAT C = m_pTerrainTileData->m_pHeightMap[(iIndexZ + 1)*iHeightMapWidth + iIndexX];
	CWFLOAT D = m_pTerrainTileData->m_pHeightMap[(iIndexZ + 1)*iHeightMapWidth + iIndexX + 1];

	CWFLOAT s = fIndexX - (CWFLOAT)iIndexX;
	CWFLOAT t = fIndexZ - (CWFLOAT)iIndexZ;

	if (s + t <= 1.0f) {
		CWFLOAT u = B - A;
		CWFLOAT v = C - A;
		return A + s*u + t*v;
	}

	CWFLOAT u = C - D;
	CWFLOAT v = B - D;

	return D + (1.0f - s)*u + (1.0f - t)*v;
}

cwVector3D cwTerrainTile::getMovedPosition(const cwVector3D& pos, const cwVector3D& dir, CWFLOAT fMoveLen)
{
	CWFLOAT fStartHeight = getHeight(pos);
	cwVector3D terrainStartPos = cwVector3D(pos.x, fStartHeight, pos.z);
	cwVector3D terrainEndPos = terrainStartPos + dir*fMoveLen;

	CWFLOAT fEndHeight = getHeight(terrainEndPos);
	terrainEndPos.y = fEndHeight;

	cwVector3D moveDir = terrainEndPos - terrainStartPos;
	moveDir.normalize();

	return terrainStartPos + moveDir*fMoveLen;
}

CWVOID cwTerrainTile::releaseResource()
{
	CW_SAFE_RELEASE_NULL(m_pTexHeightMap);

	if (m_pMaterial)
		m_pMaterial->clearMaterialUnit();
}

CWVOID cwTerrainTile::setTerrainTileData(sTerrainTileData* pTerrainTileData)
{
	if (m_pTerrainTileData != pTerrainTileData) {
		CW_SAFE_RETAIN(pTerrainTileData);
		CW_SAFE_RELEASE_NULL(m_pTerrainTileData);
		m_pTerrainTileData = pTerrainTileData;
	}
}

CWVOID cwTerrainTile::refreshBoundingBox()
{

}

CWVOID cwTerrainTile::streaming()
{

}

NS_MINIR_END
