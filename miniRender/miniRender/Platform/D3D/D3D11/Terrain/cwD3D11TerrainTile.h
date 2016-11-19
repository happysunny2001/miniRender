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

#ifndef __CW_D3D11_TERRAIN_TILE_H__
#define __CW_D3D11_TERRAIN_TILE_H__

#ifdef _CW_D3D11_

#include "Terrain/cwTerrainTile.h"

NS_MINIR_BEGIN

class cwMaterialUnitFloatArray;

class cwD3D11TerrainTile : public cwTerrainTile
{
public:
	static cwD3D11TerrainTile* create(sTerrainTileData* pTerrainTileData);

	cwD3D11TerrainTile();
	virtual ~cwD3D11TerrainTile();

	virtual CWBOOL init(sTerrainTileData* pTerrainTileData) override;
	virtual CWVOID loadResource() override;
	//virtual CWVOID render(cwRenderBatch* pRenderBatch) override;
	virtual CWVOID render(cwEffect* pEffect) override;

	virtual CWVOID streamPrepare() override;
	virtual CWVOID streamBegin() override;
	virtual CWVOID streaming() override;
	virtual CWVOID streamEnd() override;
	virtual CWVOID streamRelease() override;
	virtual cwRemoveBatch* buildRemoveBatch() override;

protected:
	virtual CWVOID buildHeightMapTexture() override;
	virtual CWVOID buildLayerTexture(CWBOOL bThreading = CWFALSE) override;
	virtual CWVOID buildBlendTexture(CWBOOL bThreading = CWFALSE) override;

	virtual CWVOID buildMaterial() override;

protected:
	cwMaterialUnitFloatArray* m_pMatUnitFrustum;

};

NS_MINIR_END

#endif

#endif
