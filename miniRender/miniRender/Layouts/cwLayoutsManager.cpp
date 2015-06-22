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

#include "cwLayoutsManager.h"
#include "cwEffectManager.h"
#include "cwRepertory.h"
#include "cwD3D11Layouts.h"

NS_MINI_BEGIN

cwLayoutsManager::~cwLayoutsManager()
{
}

void cwLayoutsManager::init()
{
	{
		cwLayouts* pLayout = cwD3D11Layouts::create(
				cwInputElementDescManager::getInstance().getElement(ceEleDescPosColor),
				cwRepertory::getInstance().getEffectManager()->getEffect("effect/color.fx")
			);
		if (pLayout) {
			m_mapLayouts.insert(ceEleDescPosColor, pLayout);
		}
	}

	{
		cwLayouts* pLayout = cwD3D11Layouts::create(
			cwInputElementDescManager::getInstance().getElement(ceEleDescPosNormal),
			cwRepertory::getInstance().getEffectManager()->getEffect("effect/lighting.fx")
			);
		if (pLayout) {
			m_mapLayouts.insert(ceEleDescPosNormal, pLayout);
		}
	}

	{
		cwLayouts* pLayout = cwD3D11Layouts::create(
			cwInputElementDescManager::getInstance().getElement(ceEleDescPosNormalTex),
			cwRepertory::getInstance().getEffectManager()->getEffect("effect/lightingTex.fx")
			);
		if (pLayout) {
			m_mapLayouts.insert(ceEleDescPosNormalTex, pLayout);
		}
	}
}

cwLayouts* cwLayoutsManager::getLayouts(ceElementDesc eType)
{
	auto it = m_mapLayouts.find(eType);
	if (it == m_mapLayouts.end()) return NULL;
	return it->second;
}

NS_MINI_END