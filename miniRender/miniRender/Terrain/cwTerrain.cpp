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

NS_MINIR_BEGIN

cwVector3D sTerrainData::terrainTilePosition(CWUSHORT i, CWUSHORT j)
{
	CWFLOAT fHalfTerrainWidth = terrainWidth()*0.5f;
	CWFLOAT fHalfTerrainHeight = terrainHeight()*0.5f;
	CWFLOAT fStartX = -fHalfTerrainWidth + terrainTileWidth()*0.5f;
	CWFLOAT fStartZ = fHalfTerrainHeight - terrainTileHeight()*0.5f;

	return cwVector3D(fStartX + i*terrainTileWidth(), 0, fStartZ - j*terrainTileHeight());
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

cwTerrain::cwTerrain() : 
m_pTerrainData(nullptr)
{

}

cwTerrain::~cwTerrain()
{
	CW_SAFE_DELETE(m_pTerrainData);
}

CWBOOL cwTerrain::init(const CWSTRING& strConfFile)
{
	if (!cwRenderNode::init()) return CWFALSE;

	cwParserManager* pParserManager = cwRepertory::getInstance().getParserManager();
	cwTerrainParser* pTerrainParser = static_cast<cwTerrainParser*>(pParserManager->getParser(eParserTerrain));
	if (!pTerrainParser) return CWFALSE;

	m_pTerrainData = pTerrainParser->parse(strConfFile);
	if (!m_pTerrainData) return CWFALSE;

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

NS_MINIR_END
