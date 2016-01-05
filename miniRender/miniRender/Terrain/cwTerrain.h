﻿/*
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
#include "Entity/cwRenderNode.h"
#include "cwTerrainTile.h"

NS_MINIR_BEGIN

struct sTerrainData
{
	CWUINT m_iHorizTileCnt;
	CWUINT m_iVertTileCnt;
	CWFLOAT m_fCellSpace;
	CWUINT m_iTileVertexWidth;
	CWUINT m_iTileVertexHeight;
	CWFLOAT m_fHeightScale;

	sTerrainTileData* m_pTerrainTiles;

	sTerrainData() : m_pTerrainTiles(nullptr) {}
	~sTerrainData() {
		CW_SAFE_DELETE_ARRAY(m_pTerrainTiles);
	}

	CWFLOAT terrainTileWidth();
	CWFLOAT terrainTileHeight();
};

class cwTerrain : public cwRenderNode
{
public:
	cwTerrain();
	virtual ~cwTerrain();

	virtual CWBOOL init(const CWSTRING& strConfFile);

	virtual cwTerrainTile* getTerrainTile() { return nullptr; }
	virtual CWFLOAT getHeight(const cwVector3D& pos);
	virtual cwVector3D getMovedPosition(const cwVector3D& pos, const cwVector3D& dir, CWFLOAT fMoveLen);

	inline sTerrainData* getTerrainData() { return m_pTerrainData; }

protected:
	sTerrainData* m_pTerrainData;

};

NS_MINIR_END

#endif
