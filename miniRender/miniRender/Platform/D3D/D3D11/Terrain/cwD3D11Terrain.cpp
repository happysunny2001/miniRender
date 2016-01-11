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

#ifdef _CW_D3D11_

#include "cwD3D11Terrain.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderManager.h"
#include "effect/cwEffect.h"
#include "effect/cwEffectFloatParameter.h"
#include "RenderObject/cwRenderObject.h"
#include "RenderObject/cwStaticRenderObject.h"
#include "Terrain/cwTerrainTile.h"
#include "cwD3D11TerrainTile.h"

NS_MINIR_BEGIN

cwD3D11Terrain* cwD3D11Terrain::create(const CWSTRING& strConfFile)
{
	cwD3D11Terrain* pTerrain = new cwD3D11Terrain();
	if (pTerrain && pTerrain->init(strConfFile)) {
		pTerrain->autorelease();
		return pTerrain;
	}

	CW_SAFE_DELETE(pTerrain);
	return nullptr;
}

cwD3D11Terrain::cwD3D11Terrain():
m_iPatchCells(64),
m_fMinTessDistance(10.0f),
m_fMaxTessDistance(1000.0f),
m_fMinTessFactor(0),
m_fMaxTessFactor(6),
m_pRenderObject(nullptr)
{

}

cwD3D11Terrain::~cwD3D11Terrain()
{
	CW_SAFE_RELEASE_NULL(m_pRenderObject);
}

CWBOOL cwD3D11Terrain::init(const CWSTRING& strConfFile)
{
	if (!cwTerrain::init(strConfFile)) return CWFALSE;

	m_iPatchVertexWidth  = (m_pTerrainData->m_iTileVertexWidth - 1) / m_iPatchCells + 1;
	m_iPatchVertexHeight = (m_pTerrainData->m_iTileVertexHeight - 1) / m_iPatchCells + 1;
	m_iPatchVertices = m_iPatchVertexWidth*m_iPatchVertexHeight;
	m_iPatchFaces = (m_iPatchVertexWidth - 1)*(m_iPatchVertexHeight - 1);

	m_fPatchWidth = m_pTerrainData->terrainTileWidth() / (m_iPatchVertexWidth - 1);
	m_fPatchHeight = m_pTerrainData->terrainTileHeight() / (m_iPatchVertexHeight - 1);

	m_fTexelCellSpaceU = 1.0f / m_pTerrainData->m_iTileVertexWidth;
	m_fTexelCellSpaceV = 1.0f / m_pTerrainData->m_iTileVertexHeight;

	buildTerrainVertexBuffer();
	buildEffect();

	for (CWUSHORT j = 0; j < m_pTerrainData->m_iVertTileCnt; ++j) {
		for (CWUSHORT i = 0; i < m_pTerrainData->m_iHorizTileCnt; ++i) {
			buildTerrainTile(i, j);
		}
	}

	return CWTRUE;
}

CWVOID cwD3D11Terrain::buildTerrainVertexBuffer()
{
	std::vector<sTerrainVertex> vecTerrainVertex(m_iPatchVertices);

	CWFLOAT fHalfWidth = m_pTerrainData->terrainTileWidth()*0.5f;
	CWFLOAT fHalfHeight = m_pTerrainData->terrainTileHeight()*0.5f;
	CWFLOAT du = 1.0f / (m_iPatchVertexWidth - 1);
	CWFLOAT dv = 1.0f / (m_iPatchVertexHeight - 1);

	for (CWUINT j = 0; j < m_iPatchVertexHeight; ++j) {
		CWFLOAT z = fHalfHeight - j*m_fPatchHeight;
		for (CWUINT i = 0; i < m_iPatchVertexWidth; ++i) {
			CWFLOAT x = -fHalfWidth + i*m_fPatchWidth;
			vecTerrainVertex[j*m_iPatchVertexWidth + i].pos = cwVector3D(x, 0.0f, z);
			vecTerrainVertex[j*m_iPatchVertexWidth + i].tex = cwVector2D(i*du, j*dv);
		}
	}

	std::vector<CWUINT> vecTerrainIndex(m_iPatchFaces * 4);
	CWUINT k = 0;
	for (CWUINT j = 0; j < m_iPatchVertexHeight - 1; ++j) {
		for (CWUINT i = 0; i < m_iPatchVertexWidth - 1; ++i) {
			vecTerrainIndex[k + 0] = j*m_iPatchVertexWidth + i;
			vecTerrainIndex[k + 1] = j*m_iPatchVertexWidth + i + 1;
			vecTerrainIndex[k + 2] = (j + 1)*m_iPatchVertexWidth + i;
			vecTerrainIndex[k + 3] = (j + 1)*m_iPatchVertexWidth + i + 1;

			k += 4;
		}
	}

	m_pRenderObject = cwStaticRenderObject::create(
		ePrimitiveTypePatchList4,
		&(vecTerrainVertex[0]),
		sizeof(sTerrainVertex),
		m_iPatchVertices,
		&(vecTerrainIndex[0]),
		(CWUINT)vecTerrainIndex.size(),
		"TerrainVertex");
	CW_SAFE_RETAIN(m_pRenderObject);
}

CWVOID cwD3D11Terrain::buildEffect()
{
	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getShader("terrain.fx");
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);

	cwEffectFloatParameter* pParamMinDist = cwEffectFloatParameter::create();
	pParamMinDist->setParameterName("gMinDist");
	pParamMinDist->m_fValue = m_fMinTessDistance;
	pEffect->addParameter(pParamMinDist);

	cwEffectFloatParameter* pParamMaxDist = cwEffectFloatParameter::create();
	pParamMaxDist->setParameterName("gMaxDist");
	pParamMaxDist->m_fValue = m_fMaxTessDistance;
	pEffect->addParameter(pParamMaxDist);

	cwEffectFloatParameter* pParamMinTess = cwEffectFloatParameter::create();
	pParamMinTess->setParameterName("gMinTess");
	pParamMinTess->m_fValue = m_fMinTessFactor;
	pEffect->addParameter(pParamMinTess);

	cwEffectFloatParameter* pParamMaxTess = cwEffectFloatParameter::create();
	pParamMaxTess->setParameterName("gMaxTess");
	pParamMaxTess->m_fValue = m_fMaxTessFactor;
	pEffect->addParameter(pParamMaxTess);

	cwEffectFloatParameter* pParamTexelCellU = cwEffectFloatParameter::create();
	pParamTexelCellU->setParameterName("gTexelCellSpaceU");
	pParamTexelCellU->m_fValue = m_fTexelCellSpaceU;
	pEffect->addParameter(pParamTexelCellU);

	cwEffectFloatParameter* pParamTexelCellV = cwEffectFloatParameter::create();
	pParamTexelCellV->setParameterName("gTexelCellSpaceV");
	pParamTexelCellV->m_fValue = m_fTexelCellSpaceV;
	pEffect->addParameter(pParamTexelCellV);

	cwEffectFloatParameter* pParamCellSpace = cwEffectFloatParameter::create();
	pParamCellSpace->setParameterName("gWorldCellSpace");
	pParamCellSpace->m_fValue = m_pTerrainData->m_fCellSpace;
	pEffect->addParameter(pParamCellSpace);

	this->setEffect(pEffect);
}

CWVOID cwD3D11Terrain::buildTerrainTile(CWUSHORT i, CWUSHORT j)
{
	if (!m_pTerrainData) return;

	sTerrainTileData* pTileData = m_pTerrainData->m_nTerrainTiles[sTerrainTileData::getKey(i, j)];
	pTileData->loadHeightMap(m_pTerrainData->m_fHeightScale);
	pTileData->loadResources();

	cwTerrainTile* pTerrainTile = cwD3D11TerrainTile::create(pTileData);
	if (!pTerrainTile) return;
	pTerrainTile->loadResource();

	cwVector3D tilePos = m_pTerrainData->terrainTilePosition(i, j);
	CWFLOAT fHalfWidth = m_pTerrainData->terrainTileWidth()*0.5f;
	CWFLOAT fHalfHeight = m_pTerrainData->terrainTileHeight()*0.5f;
	const cwVector2D& bound = pTileData->m_nBoundY;

	cwAABB aabb;
	aabb.m_nMin.set(tilePos.x - fHalfWidth, bound.x, tilePos.z - fHalfHeight);
	aabb.m_nMax.set(tilePos.x + fHalfWidth, bound.y, tilePos.z + fHalfHeight);

	pTerrainTile->setBoundingBox(aabb);
	pTerrainTile->setPosition(tilePos);
	pTerrainTile->setEffect(m_pEffect);
	pTerrainTile->setRenderObject(m_pRenderObject);

	this->addChild(pTerrainTile);
	m_nMapTiles.insert(sTerrainTileData::getKey(i, j), pTerrainTile);
}

NS_MINIR_END

#endif
