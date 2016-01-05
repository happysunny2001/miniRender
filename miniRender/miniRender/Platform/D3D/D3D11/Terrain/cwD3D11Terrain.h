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

#ifndef __CW_D3D11_TERRAIN_H__
#define __CW_D3D11_TERRAIN_H__

#ifdef _CW_D3D11_

#include "Terrain/cwTerrain.h"

NS_MINIR_BEGIN

struct sTerrainVertex
{
	cwVector3D pos;
	cwVector2D tex;
};

class cwD3D11Terrain : public cwTerrain
{
public:
	static cwD3D11Terrain* create(const CWSTRING& strConfFile);

	cwD3D11Terrain();
	virtual ~cwD3D11Terrain();

	virtual CWBOOL init(const CWSTRING& strConfFile) override;
	virtual cwTerrainTile* getTerrainTile() override;
	virtual CWFLOAT getHeight(const cwVector3D& pos) override;
	virtual cwVector3D getMovedPosition(const cwVector3D& pos, const cwVector3D& dir, CWFLOAT fMoveLen) override;

protected:
	CWVOID buildTerrainVertexBuffer();
	CWVOID buildEffect();

	CWVOID buildTerrainTile();

protected:
	CWUINT m_iPatchVertexWidth;
	CWUINT m_iPatchVertexHeight;
	CWUINT m_iPatchVertices;
	CWUINT m_iPatchFaces;
	CWFLOAT m_fPatchWidth;
	CWFLOAT m_fPatchHeight;

	CWUINT m_iPatchCells;
	CWFLOAT m_fMinTessDistance;
	CWFLOAT m_fMaxTessDistance;
	CWFLOAT m_fMinTessFactor;
	CWFLOAT m_fMaxTessFactor;
	CWFLOAT m_fTexelCellSpaceU;
	CWFLOAT m_fTexelCellSpaceV;

	cwRenderObject* m_pRenderObject;

	cwTerrainTile* m_pTerrainTile;

};

NS_MINIR_END

#endif

#endif
