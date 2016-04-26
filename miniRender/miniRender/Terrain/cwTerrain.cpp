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

#include "cwTerrain.h"
#include "Parser/cwParserManager.h"
#include "Parser/cwTerrainParser.h"
#include "Repertory/cwRepertory.h"
#include "Resource/cwLoadBatch.h"
#include "Resource/cwStreaming.h"
#include "Resource/cwResourceLoader.h"
#include "Engine/cwEngine.h"
#include "Camera/cwCamera.h"
#include "Base/cwLog.h"

NS_MINIR_BEGIN

cwVector3D sTerrainData::terrainTilePosition(CWUSHORT i, CWUSHORT j)
{
	CWFLOAT fHalfTerrainWidth = terrainWidth()*0.5f;
	CWFLOAT fHalfTerrainHeight = terrainHeight()*0.5f;
	CWFLOAT fStartX = -fHalfTerrainWidth + terrainTileWidth()*0.5f;
	CWFLOAT fStartZ = fHalfTerrainHeight - terrainTileHeight()*0.5f;

	return cwVector3D(fStartX + i*terrainTileWidth(), 0, fStartZ - j*terrainTileHeight());
}

cwAABB sTerrainData::terrainTileBoundingBox(CWUSHORT i, CWUSHORT j)
{
	cwVector3D tilePos = this->terrainTilePosition(i, j);
	CWFLOAT fHalfWidth = this->terrainTileWidth()*0.5f;
	CWFLOAT fHalfHeight = this->terrainTileHeight()*0.5f;
	
	cwAABB aabb;
	aabb.m_nMin.set(tilePos.x - fHalfWidth, -cwMathUtil::cwInfinity, tilePos.z - fHalfHeight);
	aabb.m_nMax.set(tilePos.x + fHalfWidth, cwMathUtil::cwInfinity, tilePos.z + fHalfHeight);

	return aabb;
}

CWUSHORT sTerrainData::getTerrainTileIndexX(CWFLOAT x)
{
	CWFLOAT fHalfTerrainWidth = terrainWidth()*0.5f;
	CWFLOAT fLeft = fHalfTerrainWidth + x;
	if (fLeft < 0) return gInvalidTerrainTileIndex;
	CWUSHORT index = (CWUSHORT)floorf(fLeft / terrainTileWidth());
	if (index >= m_iHorizTileCnt) return gInvalidTerrainTileIndex;

	return index;
}

CWUSHORT sTerrainData::getTerrainTileIndexY(CWFLOAT y)
{
	CWFLOAT fHalfTerrainHeight = terrainHeight()*0.5f;
	CWFLOAT fTop = fHalfTerrainHeight - y;
	if (fTop < 0) return gInvalidTerrainTileIndex;
	CWUSHORT index = (CWUSHORT)floorf(fTop / terrainTileHeight());
	if (index >= m_iVertTileCnt) return gInvalidTerrainTileIndex;

	return index;
}

CWUSHORT sTerrainData::getTerrainTileIndexXIn(CWFLOAT x)
{
	CWFLOAT fHalfTerrainWidth = terrainWidth()*0.5f;
	CWFLOAT fLeft = fHalfTerrainWidth + x;
	if (fLeft < 0) return 0;
	CWUSHORT index = (CWUSHORT)floorf(fLeft / terrainTileWidth());
	if (index >= m_iHorizTileCnt) return m_iHorizTileCnt-1;

	return index;
}

CWUSHORT sTerrainData::getTerrainTileIndexYIn(CWFLOAT y)
{
	CWFLOAT fHalfTerrainHeight = terrainHeight()*0.5f;
	CWFLOAT fTop = fHalfTerrainHeight - y;
	if (fTop < 0) return 0;
	CWUSHORT index = (CWUSHORT)floorf(fTop / terrainTileHeight());
	if (index >= m_iVertTileCnt) return m_iVertTileCnt-1;

	return index;
}

cwTerrain::cwTerrain() : 
m_pTerrainData(nullptr)
{

}

cwTerrain::~cwTerrain()
{
	for (auto it = m_nMapTiles.begin(); it != m_nMapTiles.end(); ++it) {
		it->second->streamRelease();
	}

	for (auto it = m_nLoadingTiles.begin(); it != m_nLoadingTiles.end(); ++it) {
		it->second->streamRelease();
	}

	this->clearScheduler();
	CW_SAFE_DELETE(m_pTerrainData);
	cwLog::print("cwTerrain::~cwTerrain.\n");
}

CWBOOL cwTerrain::init(const CWSTRING& strConfFile)
{
	if (!cwRenderNode::init()) return CWFALSE;

	cwParserManager* pParserManager = cwRepertory::getInstance().getParserManager();
	cwTerrainParser* pTerrainParser = static_cast<cwTerrainParser*>(pParserManager->getParser(eParserTerrain));
	if (!pTerrainParser) return CWFALSE;

	if (!pTerrainParser->parse(this, strConfFile)) return CWFALSE;

	if (m_pTerrainData->m_eLoadType == eTerrainThreading) {
		this->schedulerUpdate();
	}

	return CWTRUE;
}

CWFLOAT cwTerrain::getHeight(const cwVector3D& pos)
{
	cwTerrainTile* pTerrainTile = getTerrainTile(pos);
	if (pTerrainTile) {
		return pTerrainTile->getHeight(pos);
	}

	return 0;
}

cwVector3D cwTerrain::getMovedPosition(const cwVector3D& pos, const cwVector3D& dir, CWFLOAT fMoveLen)
{
	cwTerrainTile* pTerrainTile = getTerrainTile(pos);
	if (pTerrainTile) {
		return pTerrainTile->getMovedPosition(pos, dir, fMoveLen);
	}

	cwVector3D endPos = pos + dir*fMoveLen;
	endPos.y = 0;
	return endPos;
}

cwTerrainTile* cwTerrain::getTerrainTile(const cwVector3D& pos)
{
	CWUSHORT uIdxX = m_pTerrainData->getTerrainTileIndexX(pos.x);
	if (uIdxX == gInvalidTerrainTileIndex) return nullptr;

	CWUSHORT uIdxY = m_pTerrainData->getTerrainTileIndexY(pos.z);
	if (uIdxY == gInvalidTerrainTileIndex) return nullptr;

	CWUINT iKey = sTerrainTileData::getKey(uIdxX, uIdxY);
	auto it = m_nMapTiles.find(iKey);
	if (it == m_nMapTiles.end()) return nullptr;
	return it->second;
}

CWVOID cwTerrain::getTerrainTiles(const cwVector3D& pos, CWFLOAT fRadius, std::vector<sTerrainTileData*>& vecRet)
{
	cwVector3D bl = pos - cwVector3D(fRadius, 0, -fRadius);
	cwVector3D tr = pos + cwVector3D(fRadius, 0, -fRadius);

	CWUSHORT blX = m_pTerrainData->getTerrainTileIndexXIn(bl.x);
	CWUSHORT blY = m_pTerrainData->getTerrainTileIndexYIn(bl.z);
	CWUSHORT trX = m_pTerrainData->getTerrainTileIndexXIn(tr.x);
	CWUSHORT trY = m_pTerrainData->getTerrainTileIndexYIn(tr.z);

	if (blX > 0) blX--;
	if (blY > 0) blY--;
	if (trX < m_pTerrainData->m_iHorizTileCnt - 1) trX++;
	if (trY < m_pTerrainData->m_iVertTileCnt - 1) trY++;

	for (CWUSHORT j = blY; j <= trY; ++j) {
		for (CWUSHORT i = blX; i <= trX; ++i) {
			auto it = m_mapTerrainTiles.find(sTerrainTileData::getKey(i, j));
			if (it != m_mapTerrainTiles.end() && it->second->m_eState == eTerrainTileOffline)
				vecRet.push_back(it->second);
		}
	}
}

CWVOID cwTerrain::addTerrainTileData(sTerrainTileData* pTerrainTileData)
{
	if (pTerrainTileData) {
		m_mapTerrainTiles.insert(pTerrainTileData->getKey(), pTerrainTileData);
	}
}

CWVOID cwTerrain::loadTerrainTileOver(cwLoadBatch* pBatch)
{
	if (pBatch && pBatch->m_pObjStreaming) {
		cwTerrainTile* pTile = dynamic_cast<cwTerrainTile*>(pBatch->m_pObjStreaming);
		if (pTile) {
			this->addTerrainTile(pTile);
			m_nLoadingTiles.erase(pTile->getTerrainTileData()->getKey());
		}
	}
}

CWVOID cwTerrain::addTerrainTile(cwTerrainTile* pTerrainTile)
{
	if (pTerrainTile) {
		const cwAABB& boundingBox = pTerrainTile->getBoundingBox();
		cwPoint3D center = boundingBox.center();
		center.y = 0;
		pTerrainTile->setPosition(center);
		pTerrainTile->setEffect(m_pEffect);

		this->addChild(pTerrainTile);
		sTerrainTileData* pTerrainTileData = pTerrainTile->getTerrainTileData();
		m_nMapTiles.insert(pTerrainTileData->getKey(), pTerrainTile);
	}
}

CWVOID cwTerrain::removeTerrainTile(cwTerrainTile* pTerrainTile)
{
	if (pTerrainTile) {
		sTerrainTileData* pTerrainTileData = pTerrainTile->getTerrainTileData();

		cwLog::print("Release Terrain Tile:[%d,%d]\n", pTerrainTileData->x, pTerrainTileData->y);
		
		pTerrainTile->streamRelease();
		this->removeChild(pTerrainTile);
		m_nMapTiles.erase(sTerrainTileData::getKey(pTerrainTileData->x, pTerrainTileData->y));
	}
}

CWVOID cwTerrain::update(CWFLOAT dt)
{
	checkTileRelease();
	checkTerrainLoad();
}

CWVOID cwTerrain::checkTileRelease()
{
	static std::vector<cwTerrainTile*> vecRemoveTiles;

	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getDefaultCamera();
	if (!pCamera) return;

	vecRemoveTiles.clear();
	//const cwFrustum& frustum = pCamera->getFrustum();
	cwCircle cCamera(pCamera->getPos(), pCamera->getFarZ());

	for (auto it = m_nMapTiles.begin(); it != m_nMapTiles.end(); ++it) {
		sTerrainTileData* pTerrainTileData = it->second->getTerrainTileData();
		if (!cCamera.intersection(pTerrainTileData->m_nBoxRelease)) {
			vecRemoveTiles.push_back(it->second);
		}
		/*int iSect = frustum.intersection(pTerrainTileData->m_nBoxRelease);
		if (!frustum.isCollide(iSect)) {
			vecRemoveTiles.push_back(it->second);
		}*/
	}

	if (!vecRemoveTiles.empty()) {
		for (auto pTerainTile : vecRemoveTiles) {
			this->removeTerrainTile(pTerainTile);
		}
		vecRemoveTiles.clear();
	}
}

CWVOID cwTerrain::checkTerrainLoad()
{
	static std::vector<sTerrainTileData*> vecRet;
	static std::vector<sTerrainTileData*> vecLoad;

	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getDefaultCamera();
	if (!pCamera) return;

	vecRet.clear();
	this->getTerrainTiles(pCamera->getPos(), pCamera->getFarZ(), vecRet);
	if (vecRet.empty()) return;
	vecLoad.clear();

	cwCircle cCamera(pCamera->getPos(), pCamera->getFarZ());
	for (auto pTileData : vecRet) {
		if (pTileData->m_eState != eTerrainTileOffline) continue;

		if (cCamera.intersection(pTileData->m_nBoxLoad)) {
			vecLoad.push_back(pTileData);
		}
	}

	if (!vecLoad.empty()) {
		cwResourceLoader* pResLoader = cwRepertory::getInstance().getResourceLoader();
		for (auto pTileData : vecLoad) {
			cwLog::print("Begin Load Terrain Tile [%d,%d]\n", pTileData->x, pTileData->y);
			cwTerrainTile* pTerrainTile = createTerrainTile(pTileData);
			if (pTerrainTile) {
				pResLoader->loadAsync(pTerrainTile);

				static_cast<cwStreaming*>(pTerrainTile)->getLoadBatch()->onLoadOver = CW_CALLBACK_1(cwTerrain::loadTerrainTileOver, this);
				m_nLoadingTiles.insert(pTerrainTile->getTerrainTileData()->getKey(), pTerrainTile);
			}
		}

		vecLoad.clear();
	}

	vecRet.clear();
}

cwTerrainTile* cwTerrain::createTerrainTile(sTerrainTileData* pTileData)
{
	return nullptr;
}

NS_MINIR_END
