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

#include "cwTerrainParser.h"
#include "Resource/cwResourceLoader.h"
#include "Repertory/cwRepertory.h"
#include "Base/cwStringConvert.h"

NS_MINIR_BEGIN

cwTerrainParser* cwTerrainParser::create()
{
	cwTerrainParser* pParser = new cwTerrainParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwTerrainParser::cwTerrainParser()
{

}

sTerrainData* cwTerrainParser::parse(const CWSTRING& strFileName)
{
	cwData* pFileData = cwRepertory::getInstance().getResourceLoader()->getFileData(strFileName);
	if (!pFileData) return nullptr;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.Parse((const char*)(pFileData->m_pData), pFileData->m_uSize);
	if (error != tinyxml2::XMLError::XML_SUCCESS) {
		return nullptr;
	}

	CW_SAFE_DELETE(pFileData);

	tinyxml2::XMLElement* pTerrainElement = doc.FirstChildElement("Terrain");
	if (!pTerrainElement) return nullptr;

	tinyxml2::XMLElement* pInfoElement = pTerrainElement->FirstChildElement("Info");
	tinyxml2::XMLElement* pTileListElement = pTerrainElement->FirstChildElement("TileList");

	if (!pInfoElement || !pTileListElement) return nullptr;

	sTerrainData* pTerrainData = new sTerrainData();
	if (!pTerrainData) return nullptr;

	pTerrainData->m_iHorizTileCnt     = pInfoElement->IntAttribute("Horiz");
	pTerrainData->m_iVertTileCnt      = pInfoElement->IntAttribute("Vert");
	pTerrainData->m_fCellSpace        = pInfoElement->FloatAttribute("CellSpace");
	pTerrainData->m_iTileVertexWidth  = pInfoElement->IntAttribute("TileVertexWidth");
	pTerrainData->m_iTileVertexHeight = pInfoElement->IntAttribute("TileVertexHeight");
	pTerrainData->m_fHeightScale      = pInfoElement->FloatAttribute("HeightScale");

	pTerrainData->m_pTerrainTiles = new sTerrainTileData[pTerrainData->m_iHorizTileCnt*pTerrainData->m_iVertTileCnt];
	if (!pTerrainData->m_pTerrainTiles) {
		CW_SAFE_DELETE(pTerrainData);
		return nullptr;
	}

	tinyxml2::XMLElement* pTileElement = pTileListElement->FirstChildElement("Tile");
	for (CWUINT j = 0; j < pTerrainData->m_iVertTileCnt; ++j) {
		for (CWUINT i = 0; i < pTerrainData->m_iHorizTileCnt; ++i) {
			if (!pTileElement) break;

			CWUINT iTileIndex = j*pTerrainData->m_iHorizTileCnt + i;
			sTerrainTileData& terrainTile = pTerrainData->m_pTerrainTiles[iTileIndex];

			tinyxml2::XMLElement* pHeightMapElement = pTileElement->FirstChildElement("HeightMap");
			std::vector<sTerrainTexture> vecHeightMap = parseTexture(pHeightMapElement);
			if (!vecHeightMap.empty()) {
				terrainTile.m_nHeightMap = vecHeightMap[0];
			}

			tinyxml2::XMLElement* pLayerElement = pTileElement->FirstChildElement("Layers");
			terrainTile.m_nVecLayers = parseTexture(pLayerElement);

			tinyxml2::XMLElement* pBlendElement = pTileElement->FirstChildElement("Blend");
			terrainTile.m_nVecBlend = parseTexture(pBlendElement);

			terrainTile.x = i;
			terrainTile.y = j;
			terrainTile.m_bLoaded = CWFALSE;
			terrainTile.m_pHeightMap = nullptr;
			terrainTile.m_iHeightMapWidth = pTerrainData->m_iTileVertexWidth;
			terrainTile.m_iHeightMapHeight = pTerrainData->m_iTileVertexHeight;
			terrainTile.m_fCellSpace = pTerrainData->m_fCellSpace;

			pTileElement = pTileElement->NextSiblingElement("Tile");
		}

		if (!pTileElement) break;
	}

	return pTerrainData;
}

std::vector<sTerrainTexture> cwTerrainParser::parseTexture(tinyxml2::XMLElement* pElement)
{
	std::vector<sTerrainTexture> vecTextures;
	tinyxml2::XMLElement* pTextureElement = pElement->FirstChildElement("Texture");
	
	while (pTextureElement) {
		const char* pcTexture = pTextureElement->Attribute("File");
		const char* pcParam = pTextureElement->Attribute("ShaderParam");

		sTerrainTexture sTex;
		sTex.m_nStrTextureFile = pcTexture;
		sTex.m_nStrParamName = pcParam;

		vecTextures.push_back(sTex);

		pTextureElement = pTextureElement->NextSiblingElement("Texture");
	}

	return vecTextures;
}

NS_MINIR_END
