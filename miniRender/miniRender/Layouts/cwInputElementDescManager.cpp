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

#include "cwInputElementDescManager.h"
#include "cwShaderConstant.h"

NS_MINI_BEGIN

cwInputElementDesc* cwInputElementDesc::create(int iElementCnt)
{
	cwInputElementDesc* pDesc = new cwInputElementDesc();
	if (pDesc && pDesc->init(iElementCnt)) {
		pDesc->autorelease();
		return pDesc;
	}

	CW_SAFE_DELETE(pDesc);
	return nullptr;
}

cwInputElementDesc::cwInputElementDesc()
{

}

cwInputElementDesc::~cwInputElementDesc()
{
	CW_SAFE_DELETE_ARRAY(m_pElementList);
	m_iElementCnt = 0;
}

bool cwInputElementDesc::init(int iElementCnt)
{
	m_pElementList = new D3D11_INPUT_ELEMENT_DESC[iElementCnt];
	if (!m_pElementList) return false;
	m_iElementCnt = iElementCnt;

	return true;
}

void cwInputElementDesc::addElementDesc(const CWCHAR* name, DXGI_FORMAT format, CWUINT offset, int index)
{
	m_pElementList[index] = { name, 0, format, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

cwInputElementDescManager::cwInputElementDescManager()
{
	buildDescMap();
}

cwInputElementDescManager::~cwInputElementDescManager()
{

}

cwInputElementDesc* cwInputElementDescManager::getElement(ceElementDesc e)
{
	auto it = m_mapDesc.find(e);
	if (it == m_mapDesc.end()) return NULL;
	return it->second;
}

void cwInputElementDescManager::buildDescMap()
{
	{
		cwInputElementDesc *pDescPosColor = cwInputElementDesc::create(2);
		pDescPosColor->addElementDesc(CW_SIG_POSITION, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
		pDescPosColor->addElementDesc(CW_SIG_COLOR, DXGI_FORMAT_R32G32B32A32_FLOAT, 12, 1);
		m_mapDesc.insert(ceEleDescPosColor, pDescPosColor);
	}

	{
		cwInputElementDesc *pDescPosNormal = cwInputElementDesc::create(2);
		pDescPosNormal->addElementDesc(CW_SIG_POSITION, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
		pDescPosNormal->addElementDesc(CW_SIG_NORMAL, DXGI_FORMAT_R32G32B32_FLOAT, 12, 1);
		m_mapDesc.insert(ceEleDescPosNormal, pDescPosNormal);
	}

	{
		cwInputElementDesc *pDescPosNormalTex = cwInputElementDesc::create(3);
		pDescPosNormalTex->addElementDesc(CW_SIG_POSITION, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
		pDescPosNormalTex->addElementDesc(CW_SIG_NORMAL, DXGI_FORMAT_R32G32B32_FLOAT, 12, 1);
		pDescPosNormalTex->addElementDesc(CW_SIG_TEXCOORD, DXGI_FORMAT_R32G32_FLOAT, 24, 2);
		m_mapDesc.insert(ceEleDescPosNormalTex, pDescPosNormalTex);
	}
}

NS_MINI_END