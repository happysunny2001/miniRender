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

#include "cwEntityParser.h"
#include "Generator/cwGeometryGenerator.h"
#include "Repertory/cwRepertory.h"
#include "Base/cwStruct.h"
#include "Base/cwColor.h"
#include "RenderObject/cwStaticRenderObject.h"
#include "Material/cwMaterial.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Texture/cwRenderTexture.h"
#include "Shader/cwShaderConstant.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderManager.h"
#include "Effect/cwEffect.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"
#include "Render/Stage/cwStage.h"
#include "cwEffectParser.h"
#include "cwParserManager.h"

NS_MINIR_BEGIN

cwEntityParser* cwEntityParser::create()
{
	cwEntityParser* pParser = new cwEntityParser();
	if (pParser) {
		pParser->autorelease();
		return pParser;
	}

	return nullptr;
}

cwEntityParser::cwEntityParser()
{
	m_nMapEntityParser["Position"] = CW_CALLBACK_2(cwEntityParser::parsePosition, this);
	m_nMapEntityParser["Scale"]    = CW_CALLBACK_2(cwEntityParser::parseScale, this);
	m_nMapEntityParser["Rotation"] = CW_CALLBACK_2(cwEntityParser::parseRotation, this);
	m_nMapEntityParser["Material"] = CW_CALLBACK_2(cwEntityParser::parseMaterial, this);
	m_nMapEntityParser["Effect"]   = CW_CALLBACK_2(cwEntityParser::parseEffect, this);
}

cwEntity* cwEntityParser::parse(tinyxml2::XMLElement* pEntityData)
{
	if (!pEntityData) return nullptr;

	const CWCHAR* pcType = pEntityData->Attribute("Type");
	const CWCHAR* pcVertexType = pEntityData->Attribute("VertexType");

	if (!pcType || !pcVertexType) return nullptr;
	cwEntity* pEntity = getEntity(pcType, pcVertexType);

	tinyxml2::XMLElement* pChildElement = pEntityData->FirstChildElement();
	while (pChildElement) {
		const CWCHAR* pcElementName = pChildElement->Name();
		if (m_nMapEntityParser.find(pcElementName) != m_nMapEntityParser.end()) {
			m_nMapEntityParser[pcElementName](pChildElement, pEntity);
		}

		pChildElement = pChildElement->NextSiblingElement();
	}

	return pEntity;
}

cwEntity* cwEntityParser::getEntity(const CWSTRING& strType, const CWSTRING& strVertexType)
{
	cwGeometryGenerator::cwMeshData mesh;
	
	if (strType == "Quad") {
		cwRepertory::getInstance().getGeoGenerator()->generateQuad(mesh);
	}
	else if (strType == "Box") {
		cwRepertory::getInstance().getGeoGenerator()->generateBox(mesh);
	}
	else if (strType == "None"){
		return cwEntity::create();
	}
	else {
		return nullptr;
	}

	cwRenderObject* pRenderObj = nullptr;
	if (strVertexType == "PosColor") {
		vector<cwVertexPosColor> vecVertex(mesh.nVertex.size());
		for (int i = 0; i < mesh.nVertex.size(); ++i) {
			vecVertex[i].pos = mesh.nVertex[i].pos;
			vecVertex[i].color = cwColor::white;
		}

		pRenderObj = cwStaticRenderObject::create(
			ePrimitiveTypeTriangleList,
			(CWVOID*)&vecVertex[0], sizeof(cwVertexPosColor), static_cast<CWUINT>(mesh.nVertex.size()),
			(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), strVertexType);
	}
	else if (strVertexType == "PosNormal") {
		vector<cwVertexPosNormal> vecVertex(mesh.nVertex.size());
		for (int i = 0; i < mesh.nVertex.size(); ++i) {
			vecVertex[i].pos = mesh.nVertex[i].pos;
			vecVertex[i].normal = mesh.nVertex[i].normal;
		}

		pRenderObj = cwStaticRenderObject::create(
			ePrimitiveTypeTriangleList,
			(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormal), static_cast<CWUINT>(mesh.nVertex.size()),
			(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), strVertexType);
	}
	else if (strVertexType == "PosNormalTex") {
		vector<cwVertexPosNormalTex> vecVertex(mesh.nVertex.size());
		for (int i = 0; i < mesh.nVertex.size(); ++i) {
			vecVertex[i].pos = mesh.nVertex[i].pos;
			vecVertex[i].normal = mesh.nVertex[i].normal;
			vecVertex[i].tex = mesh.nVertex[i].tex;
		}

		pRenderObj = cwStaticRenderObject::create(
			ePrimitiveTypeTriangleList,
			(CWVOID*)&vecVertex[0], sizeof(cwVertexPosNormalTex), static_cast<CWUINT>(mesh.nVertex.size()),
			(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), strVertexType);
	}
	else if (strVertexType == "PosTex") {
		vector<cwVertexPosTex> vecVertex(mesh.nVertex.size());
		for (int i = 0; i < mesh.nVertex.size(); ++i) {
			vecVertex[i].pos = mesh.nVertex[i].pos;
			vecVertex[i].tex = mesh.nVertex[i].tex;
		}

		pRenderObj = cwStaticRenderObject::create(
			ePrimitiveTypeTriangleList,
			(CWVOID*)&vecVertex[0], sizeof(cwVertexPosTex), static_cast<CWUINT>(mesh.nVertex.size()),
			(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), strVertexType);
	}
	else {
		return nullptr;
	}

	cwEntity* pEntity = cwEntity::create();
	if (!pEntity) {
		return nullptr;
	}

	pEntity->setRenderObject(pRenderObj);
	return pEntity;
}

CWVOID cwEntityParser::parsePosition(tinyxml2::XMLElement* pPosData, cwEntity* pEntity)
{
	if (!pPosData || !pEntity) return;
	
	CWFLOAT fX = pPosData->FloatAttribute("x");
	CWFLOAT fY = pPosData->FloatAttribute("y");
	CWFLOAT fZ = pPosData->FloatAttribute("z");

	pEntity->setPosition(fX, fY, fZ);
}

CWVOID cwEntityParser::parseScale(tinyxml2::XMLElement* pScaleData, cwEntity* pEntity)
{
	if (!pScaleData || !pEntity) return;

	CWFLOAT fX = pScaleData->FloatAttribute("x");
	CWFLOAT fY = pScaleData->FloatAttribute("y");
	CWFLOAT fZ = pScaleData->FloatAttribute("z");

	pEntity->setScale(fX, fY, fZ);
}

CWVOID cwEntityParser::parseRotation(tinyxml2::XMLElement* pRotData, cwEntity* pEntity)
{
	if (!pRotData || !pEntity) return;

	CWFLOAT fX = pRotData->FloatAttribute("x");
	CWFLOAT fY = pRotData->FloatAttribute("y");
	CWFLOAT fZ = pRotData->FloatAttribute("z");

	pEntity->setRotation(fX, fY, fZ);
}

CWVOID cwEntityParser::parseMaterial(tinyxml2::XMLElement* pMaterialData, cwEntity* pEntity)
{
	if (!pMaterialData || !pEntity) return;

	cwMaterial* pMaterial = cwMaterial::create();
	if (!pMaterial) return;

	tinyxml2::XMLElement* pAmbientElement = pMaterialData->FirstChildElement("Ambient");
	if (pAmbientElement) {
		pMaterial->setAmbient(cwVector4D(
			pAmbientElement->FloatAttribute("r"),
			pAmbientElement->FloatAttribute("g"),
			pAmbientElement->FloatAttribute("b"),
			pAmbientElement->FloatAttribute("a")));
	}

	tinyxml2::XMLElement* pDiffuseElement = pMaterialData->FirstChildElement("Diffuse");
	if (pDiffuseElement) {
		pMaterial->setDiffuse(cwVector4D(
			pDiffuseElement->FloatAttribute("r"),
			pDiffuseElement->FloatAttribute("g"),
			pDiffuseElement->FloatAttribute("b"),
			pDiffuseElement->FloatAttribute("a")));
	}

	tinyxml2::XMLElement* pSpecularElement = pMaterialData->FirstChildElement("Specular");
	if (pSpecularElement) {
		pMaterial->setSpecular(cwVector4D(
			pSpecularElement->FloatAttribute("r"),
			pSpecularElement->FloatAttribute("g"),
			pSpecularElement->FloatAttribute("b"),
			pSpecularElement->FloatAttribute("a")));
	}

	tinyxml2::XMLElement* pReflectElement = pMaterialData->FirstChildElement("Reflect");
	if (pReflectElement) {
		pMaterial->setReflect(cwVector4D(
			pReflectElement->FloatAttribute("r"),
			pReflectElement->FloatAttribute("g"),
			pReflectElement->FloatAttribute("b"),
			pReflectElement->FloatAttribute("a")));
	}

	tinyxml2::XMLElement* pTextureElement = pMaterialData->FirstChildElement("DiffuseTexture");
	if (pTextureElement) {
		const char* pcStage = pTextureElement->Attribute("Stage");
		const char* pcName = pTextureElement->Attribute("Name");
		if (pcStage && pcName) {
			cwRenderer* pRenderer = cwRepertory::getInstance().getEngine()->getRenderer();
			cwStage* pStage = pRenderer->getStage(pcStage);
			if (pStage) {
				cwTexture* pTexture = pStage->getStageTexture(pcName);
				if (pTexture) {
					pMaterial->setDiffuseTexture(pTexture);
				}
			}
		}
	}

	pEntity->setMaterial(pMaterial);
}

CWVOID cwEntityParser::parseEffect(tinyxml2::XMLElement* pEffectData, cwEntity* pEntity)
{
	if (!pEffectData || !pEntity) return;

	cwEffectParser* pEffectParser = static_cast<cwEffectParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserEffect));
	if (!pEffectParser) return;

	cwEffect* pEffect = pEffectParser->parse(pEffectData);
	if (pEffect) {
		pEntity->setEffect(pEffect);
	}
}

NS_MINIR_END
