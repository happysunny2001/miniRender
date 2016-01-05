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

CWFLOAT sTerrainData::terrainTileWidth()
{
	return (m_iTileVertexWidth - 1)*m_fCellSpace;
}

CWFLOAT sTerrainData::terrainTileHeight()
{
	return (m_iTileVertexHeight - 1)*m_fCellSpace;
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
	return 0;
}

cwVector3D cwTerrain::getMovedPosition(const cwVector3D& pos, const cwVector3D& dir, CWFLOAT fMoveLen)
{
	return cwVector3D::ZERO;
}

NS_MINIR_END
