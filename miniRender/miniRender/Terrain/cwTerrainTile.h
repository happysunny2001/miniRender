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

#ifndef __CW_TERRAIN_TILE_H__
#define __CW_TERRAIN_TILE_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Base/cwLog.h"
#include "Ref/cwRef.h"
#include "Entity/cwEntity.h"
#include "Math/cwAABB.h"
#include "Texture/cwTexture.h"
#include "Resource/cwStreaming.h"

#include <vector>

NS_MINIR_BEGIN

struct sTerrainData;

struct sTerrainTexture
{
	CWSTRING m_nStrTextureFile;
	CWSTRING m_nStrParamName;
};

class sTerrainTileData : public cwRef
{
public:
	//the index of tile, top left is [0, 0]
	CWUSHORT x;
	CWUSHORT y;
	eTerrainTileState m_eState;

	CWFLOAT* m_pHeightMap;
	CWUINT m_iHeightMapWidth;
	CWUINT m_iHeightMapHeight;
	cwVector2D m_nBoundY; //x:minY, y:maxY
	sTerrainData* m_pTerrainData;

	cwAABB m_nBoundingBox;
	cwAABB m_nBoxLoad;
	cwAABB m_nBoxRelease;

	sTerrainTexture m_nHeightMap;
	std::vector<sTerrainTexture> m_nVecLayers;
	std::vector<sTerrainTexture> m_nVecBlend;

	static sTerrainTileData* create();

	static CWUINT getKey(CWUSHORT x, CWUSHORT y) {
		CWUINT iKey = x;
		return iKey << 16 | y;
	}

	CWUINT getKey() {
		CWUINT iKey = x;
		return iKey << 16 | y;
	}
	
	~sTerrainTileData() {
		releaseHeightMap();
		cwLog::print("sTerrainTileData release.\n");
	}

	CWBOOL loadHeightMap(CWFLOAT fScale);
	CWVOID releaseHeightMap();
	CWUINT heightMapSize() { return m_iHeightMapWidth*m_iHeightMapHeight; }
	CWVOID updateBoundingBox();

	CWVOID loadResources();

private:
	sTerrainTileData() : m_pHeightMap(nullptr), m_eState(eTerrainTileOffline), x(0), y(0) {}

	CWBOOL isValid(CWINT i, CWINT j);
	CWFLOAT average(CWINT i, CWINT j);
	CWVOID smooth();
	
	CWVOID createBoundY();

};

class cwTerrainTile : public cwEntity, public cwStreaming
{
public:
	cwTerrainTile();
	virtual ~cwTerrainTile();

	virtual CWBOOL init(sTerrainTileData* pTerrainTileData);
	virtual CWVOID loadResource() = 0;
	inline sTerrainTileData* getTerrainTileData() { return m_pTerrainTileData; }
	CWVOID setTerrainTileData(sTerrainTileData* pTerrainTileData);

	virtual CWVOID refreshBoundingBox() override;
	CWFLOAT getHeight(const cwVector3D& pos);
	cwVector3D getMovedPosition(const cwVector3D& pos, const cwVector3D& dir, CWFLOAT fMoveLen);

	virtual CWVOID streaming() override;

protected:
	virtual CWVOID buildHeightMapTexture() = 0;
	virtual CWVOID buildLayerTexture(CWBOOL bThreading=CWFALSE) = 0;
	virtual CWVOID buildBlendTexture(CWBOOL bThreading=CWFALSE) = 0;
	virtual CWVOID releaseResource();

protected:
	sTerrainTileData* m_pTerrainTileData;
	cwTexture* m_pTexHeightMap;

	//cwVector<cwTexture*> m_nVecLayerTextures;
	//cwVector<cwTexture*> m_nVecBlendTextures;

};

NS_MINIR_END

#endif
