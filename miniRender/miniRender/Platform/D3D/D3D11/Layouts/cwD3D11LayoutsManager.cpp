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

#ifdef _CW_D3D11_

#include "cwD3D11LayoutsManager.h"
#include "cwD3D11Layouts.h"
#include "Repertory/cwRepertory.h"
#include "Platform/D3D/D3D11/Shader/cwD3D11Shader.h"
#include "Shader/cwShaderManager.h"
#include "Platform/cwFileSystem.h"
#include "Parser/cwParserManager.h"
#include "Device/cwDevice.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"

NS_MINIR_BEGIN

cwD3D11LayoutsManager* cwD3D11LayoutsManager::create()
{
	cwD3D11LayoutsManager* pManager = new cwD3D11LayoutsManager();
	if (pManager && pManager->init()) {
		pManager->autorelease();
		return pManager;
	}

	CW_SAFE_DELETE(pManager);
	return nullptr;
}

cwD3D11LayoutsManager::cwD3D11LayoutsManager()
{

}

cwD3D11LayoutsManager::~cwD3D11LayoutsManager()
{

}

CWBOOL cwD3D11LayoutsManager::init()
{
	loadLayout();

	return CWTRUE;
}

cwInputElementDesc* cwD3D11LayoutsManager::createElementDesc(tinyxml2::XMLElement* pLayoutElement)
{
	if (!pLayoutElement) return nullptr;

	cwParserManager* pParserManager = cwRepertory::getInstance().getParserManager();
	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	std::vector<D3D11_INPUT_ELEMENT_DESC> vecElement;

	tinyxml2::XMLElement* pElement = pLayoutElement->FirstChildElement("Element");
	while (pElement) {
		const char* pcName = pElement->Attribute("Name");
		int iIndex = pElement->IntAttribute("Index");
		const char* pcFormat = pElement->Attribute("Format");
		int iSlot = pElement->IntAttribute("Slot");
		int iOffset = pElement->IntAttribute("Offset");
		const char* pcClass = pElement->Attribute("Classification");
		int iInsRate = pElement->IntAttribute("InstanceRate");

		D3D11_INPUT_ELEMENT_DESC desc;
		desc.SemanticName = pcName;
		desc.SemanticIndex = iIndex;
		desc.Format = pD3D11Device->getFormatType(pParserManager->getFormatType(pcFormat));
		desc.InputSlot = iSlot;
		desc.AlignedByteOffset = iOffset;
		desc.InputSlotClass = pD3D11Device->getClassificationType(pParserManager->getClassificationType(pcClass));
		desc.InstanceDataStepRate = iInsRate;

		vecElement.push_back(desc);

		pElement = pElement->NextSiblingElement("Element");
	}

	if (vecElement.empty()) return nullptr;

	cwInputElementDesc *pDesc = cwInputElementDesc::create(int(vecElement.size()));
	int i = 0;
	for (auto desc : vecElement) {
		pDesc->addElementDesc(desc, i++);
	}

	return pDesc;
}

CWVOID cwD3D11LayoutsManager::loadLayout()
{
	auto shaderManager = cwRepertory::getInstance().getShaderManager();
	CWSTRING strFilePath = cwRepertory::getInstance().getFileSystem()->getFullFilePath("Configure/D3D11/InputLayout.xml");

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(strFilePath.c_str());
	if (err != tinyxml2::XML_NO_ERROR) {
		return;
	}

	tinyxml2::XMLElement* pInputLayoutElement = doc.FirstChildElement("InputLayout");
	if (!pInputLayoutElement) return ;

	tinyxml2::XMLElement* pLayoutElement = pInputLayoutElement->FirstChildElement("Layout");
	while (pLayoutElement) {
		const char* pcName = pLayoutElement->Attribute("Name");
		const char* pcShader = pLayoutElement->Attribute("Shader");

		cwInputElementDesc* pDesc = createElementDesc(pLayoutElement);
		if (pDesc) {
			cwLayouts* pLayout = cwD3D11Layouts::create(pDesc, static_cast<cwD3D11Shader*>(shaderManager->getShader(pcShader)));
			m_nMapLayouts.insert(pcName, pLayout);
		}

		pLayoutElement = pLayoutElement->NextSiblingElement("Layout");
	}
}

NS_MINIR_END

#endif
