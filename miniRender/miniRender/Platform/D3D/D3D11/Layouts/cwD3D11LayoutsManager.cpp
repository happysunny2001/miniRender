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

#include "cwD3D11LayoutsManager.h"
#include "cwD3D11Layouts.h"
#include "Repertory/cwRepertory.h"
#include "Platform/D3D/D3D11/Shader/cwD3D11Shader.h"
#include "Shader/cwShaderManager.h"

NS_MINI_BEGIN

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

cwD3D11LayoutsManager::cwD3D11LayoutsManager():
m_pElementDescManager(nullptr)
{

}

cwD3D11LayoutsManager::~cwD3D11LayoutsManager()
{
	CW_SAFE_RELEASE_NULL(m_pElementDescManager);
}

bool cwD3D11LayoutsManager::init()
{
	m_pElementDescManager = cwInputElementDescManager::create();
	if (!m_pElementDescManager) return false;
	CW_SAFE_RETAIN(m_pElementDescManager);

	{
		cwLayouts* pLayout = cwD3D11Layouts::create(
			m_pElementDescManager->getElement(ceEleDescPosColor),
			static_cast<cwD3D11Shader*>(cwRepertory::getInstance().getShaderManager()->getShader("G:\\Source\\Git\\miniRender\\miniRender\\x64\\Debug\\effect\\D3D11\\color.fx"))
			);
		if (pLayout) {
			m_mapLayouts.insert(ceEleDescPosColor, pLayout);
		}
	}

	{
		cwLayouts* pLayout = cwD3D11Layouts::create(
			m_pElementDescManager->getElement(ceEleDescPosNormal),
			static_cast<cwD3D11Shader*>(cwRepertory::getInstance().getShaderManager()->getShader("G:\\Source\\Git\\miniRender\\miniRender\\x64\\Debug\\effect\\D3D11\\lighting.fx"))
			);
		if (pLayout) {
			m_mapLayouts.insert(ceEleDescPosNormal, pLayout);
		}
	}

	{
		cwLayouts* pLayout = cwD3D11Layouts::create(
			m_pElementDescManager->getElement(ceEleDescPosNormalTex),
			static_cast<cwD3D11Shader*>(cwRepertory::getInstance().getShaderManager()->getShader("G:\\Source\\Git\\miniRender\\miniRender\\x64\\Debug\\effect\\D3D11\\lightingTex.fx"))
			);
		if (pLayout) {
			m_mapLayouts.insert(ceEleDescPosNormalTex, pLayout);
		}
	}

	return true;
}

NS_MINI_END

#endif
