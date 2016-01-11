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

#ifndef __CW_TERRAIN_H__
#define __CW_TERRAIN_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Base/cwUtils.h"
#include "Base/cwMap.h"
#include "Entity/cwRenderNode.h"
#include "cwTerrainTile.h"

#include <unordered_map>

NS_MINIR_BEGIN

struct sTerrainData
{
	CWUSHORT m_iHorizTileCnt;
	CWUSHORT m_iVertTileCnt;
	CWFLOAT m_fCellSpace;
	CWUINT m_iTileVertexWidth;
	CWUINT m_iTileVertexHeight;
	CWFLOAT m_fHeightScale;
	eTerrainLoadType m_eLoadType;
	cwAABB m_nArea;

	std::unordered_map<CWUINT, sTerrainTileData*> m_nTerrainTiles;

	sTerrainData() {
		m_eLoadType = eTerrainLoadOnce;
	}

	~sTerrainData() {
		for (auto it = m_nTerrainTiles.begin(); it != m_nTerrainTiles.end(); ++it) {
			if (it->second) {
				CW_SAFE_DELETE(it->second);
			}
		}
	}

	inline CWFLOAT terrainTileWidth() {
		return (m_iTileVertexWidth - 1)*m_fCellSpace;
	}

	inline CWFLOAT terrainTileHeight() {
		return (m_iTileVertexHeight - 1)*m_fCellSpace;
	}

	inline CWFLOAT terrainWidth() {
		return terrainTileWidth()*m_iHorizTileCnt;
	}

	inline CWFLOAT terrainHeight() {
		return terrainTileHeight()*m_iVertTileCnt;
	}

	cwVector3D terrainTilePosition(CWUSHORT i, CWUSHORT j);

	CWUSHORT getTerrainTileIndexX(CWFLOAT x);
	CWUSHORT getTerrainTileIndexY(CWFLOAT y);
};

class cwTerrain : public cwRenderNode
{
public:
	cwTerrain();
	virtual ~cwTerrain();

	virtual CWBOOL init(const CWSTRING& strConfFile);

	virtual CWFLOAT getHeight(const cwVector3D& pos);
	virtual cwVector3D getMovedPosition(const cwVector3D& pos, const cwVector3D& dir, CWFLOAT fMoveLen);
	cwTerrainTile* getTerrainTile(const cwVector3D& pos);

	inline sTerrainData* getTerrainData() { return m_pTerrainData; }

protected:
	sTerrainData* m_pTerrainData;
	cwMap<CWUINT, cwTerrainTile*> m_nMapTiles;

};

NS_MINIR_END

#endif
