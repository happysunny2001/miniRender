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
#include "Ref/cwRef.h"
#include "Entity/cwEntity.h"
#include "Math/cwAABB.h"
#include "Texture/cwTexture.h"

#include <vector>

NS_MINIR_BEGIN

struct sTerrainTexture
{
	CWSTRING m_nStrTextureFile;
	CWSTRING m_nStrParamName;
};

struct sTerrainTileData
{
	CWUINT x;
	CWUINT y;
	CWBOOL m_bLoaded;

	CWFLOAT* m_pHeightMap;
	CWUINT m_iHeightMapWidth;
	CWUINT m_iHeightMapHeight;
	cwAABB m_nBoundingBox;
	cwVector2D m_nBoundY; //x:minY, y:maxY
	CWFLOAT m_fCellSpace;

	sTerrainTexture m_nHeightMap;
	std::vector<sTerrainTexture> m_nVecLayers;
	std::vector<sTerrainTexture> m_nVecBlend;

	sTerrainTileData() : m_pHeightMap(nullptr), m_bLoaded(CWFALSE), x(0), y(0) {}

	CWBOOL loadHeightMap(CWFLOAT fScale);
	CWVOID releaseHeightMap();
	CWUINT heightMapSize() { return m_iHeightMapWidth*m_iHeightMapHeight; }

	CWVOID loadTextures();

private:
	CWBOOL isValid(CWINT i, CWINT j);
	CWFLOAT average(CWINT i, CWINT j);
	CWVOID smooth();
	
	CWVOID createBoundY();

};

class cwTerrainTile : public cwEntity
{
public:
	cwTerrainTile();
	virtual ~cwTerrainTile();

	virtual CWBOOL init(sTerrainTileData* pTerrainTileData);
	virtual CWVOID loadResource() = 0;
	inline sTerrainTileData* getTerrainTileData() { return m_pTerrainTileData; }

	virtual CWVOID refreshBoundingBox() override;
	CWFLOAT getHeight(const cwVector3D& pos);
	cwVector3D getMovedPosition(const cwVector3D& pos, const cwVector3D& dir, CWFLOAT fMoveLen);

protected:
	virtual CWVOID buildHeightMapTexture() = 0;
	virtual CWVOID buildLayerTexture() = 0;
	virtual CWVOID buildBlendTexture() = 0;
	virtual CWVOID releaseResource();

protected:
	sTerrainTileData* m_pTerrainTileData;
	cwTexture* m_pTexHeightMap;

	cwVector<cwTexture*> m_nVecLayerTextures;
	cwVector<cwTexture*> m_nVecBlendTextures;

};

NS_MINIR_END

#endif
