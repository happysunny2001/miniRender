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

#include "cwTerrainParser.h"
#include "Resource/cwResourceLoader.h"
#include "Repertory/cwRepertory.h"
#include "Base/cwStringConvert.h"
#include "Terrain/cwTerrain.h"

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

CWBOOL cwTerrainParser::parse(cwTerrain* pTerrain, const CWSTRING& strFileName)
{
	cwData* pFileData = cwRepertory::getInstance().getResourceLoader()->getFileData(strFileName);
	if (!pFileData) return CWFALSE;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.Parse((const char*)(pFileData->m_pData), pFileData->m_uSize);
	if (error != tinyxml2::XMLError::XML_SUCCESS) {
		return CWFALSE;
	}

	CW_SAFE_DELETE(pFileData);

	tinyxml2::XMLElement* pTerrainElement = doc.FirstChildElement("Terrain");
	if (!pTerrainElement) return CWFALSE;

	tinyxml2::XMLElement* pInfoElement = pTerrainElement->FirstChildElement("Info");
	tinyxml2::XMLElement* pTileListElement = pTerrainElement->FirstChildElement("TileList");

	if (!pInfoElement || !pTileListElement) return CWFALSE;

	sTerrainData* pTerrainData = new sTerrainData();
	if (!pTerrainData) return CWFALSE;

	pTerrainData->m_iHorizTileCnt     = pInfoElement->IntAttribute("Horiz");
	pTerrainData->m_iVertTileCnt      = pInfoElement->IntAttribute("Vert");
	pTerrainData->m_fCellSpace        = pInfoElement->FloatAttribute("CellSpace");
	pTerrainData->m_iTileVertexWidth  = pInfoElement->IntAttribute("TileVertexWidth");
	pTerrainData->m_iTileVertexHeight = pInfoElement->IntAttribute("TileVertexHeight");
	pTerrainData->m_fHeightScale      = pInfoElement->FloatAttribute("HeightScale");
	pTerrainData->m_fBoundingBoxOffsetScale = 0.1f;
	pTerrain->setTerrainData(pTerrainData);

	tinyxml2::XMLElement* pTileElement = pTileListElement->FirstChildElement("Tile");

	while (pTileElement) {
		sTerrainTileData* pTileData = sTerrainTileData::create();
		if (!pTileData) {
			break;
		}

		tinyxml2::XMLElement* pHeightMapElement = pTileElement->FirstChildElement("HeightMap");
		std::vector<sTerrainTexture> vecHeightMap = parseTexture(pHeightMapElement);
		if (!vecHeightMap.empty()) {
			pTileData->m_nHeightMap = vecHeightMap[0];
		}

		tinyxml2::XMLElement* pLayerElement = pTileElement->FirstChildElement("Layers");
		pTileData->m_nVecLayers = parseTexture(pLayerElement);

		tinyxml2::XMLElement* pBlendElement = pTileElement->FirstChildElement("Blend");
		pTileData->m_nVecBlend = parseTexture(pBlendElement);

		CWUINT iIndexX = pTileElement->IntAttribute("x");
		CWUINT iIndexY = pTileElement->IntAttribute("y");

		pTileData->x = iIndexX;
		pTileData->y = iIndexY;
		pTileData->m_pHeightMap = nullptr;
		pTileData->m_iHeightMapWidth = pTerrainData->m_iTileVertexWidth;
		pTileData->m_iHeightMapHeight = pTerrainData->m_iTileVertexHeight;
		pTileData->m_pTerrainData = pTerrainData;
		pTileData->m_nBoundingBox = pTerrainData->terrainTileBoundingBox(iIndexX, iIndexY);

		pTileData->updateBoundingBox();
		//pTerrainData->m_nTerrainTiles.insert(std::make_pair(sTerrainTileData::getKey(iIndexX, iIndexY), pTileData));
		pTerrain->addTerrainTileData(pTileData);

		pTileElement = pTileElement->NextSiblingElement("Tile");
	}

	pTerrainData->m_nArea.m_nMin.set(-pTerrainData->terrainWidth()*0.5f, -cwMathUtil::cwInfinity, -pTerrainData->terrainHeight()*0.5f);
	pTerrainData->m_nArea.m_nMax.set(pTerrainData->terrainWidth()*0.5f, cwMathUtil::cwInfinity, pTerrainData->terrainHeight()*0.5f);

	return CWTRUE;
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
