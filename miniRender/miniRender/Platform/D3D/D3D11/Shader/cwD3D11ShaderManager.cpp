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

#include "cwD3D11ShaderManager.h"
#include "Repertory/cwRepertory.h"
#include "Platform/cwFileSystem.h"

NS_MINIR_BEGIN

cwD3D11ShaderManager* cwD3D11ShaderManager::create()
{
	cwD3D11ShaderManager* pManager = new cwD3D11ShaderManager();
	if (pManager && pManager->init()) {
		pManager->autorelease();
		return pManager;
	}

	CW_SAFE_DELETE(pManager);
	return nullptr;
}

cwD3D11ShaderManager::cwD3D11ShaderManager()
{

}

cwD3D11ShaderManager::~cwD3D11ShaderManager()
{

}

bool cwD3D11ShaderManager::init()
{
	//init d3d shader first
	auto fileSystem = cwRepertory::getInstance().getFileSystem();

	this->loadShader(fileSystem->getFullFilePath("effect/D3D11/color.fx"));
	this->loadShader(fileSystem->getFullFilePath("effect/D3D11/lighting.fx"));
	this->loadShader(fileSystem->getFullFilePath("effect/D3D11/lightingTex.fx"));

	m_nMapDefShader.insert(CW_SHADER_DEF_COLOR, getShader(fileSystem->getFullFilePath("effect/D3D11/color.fx")));
	m_nMapDefShader.insert(CW_SHADER_DEF_LIGHTING, getShader(fileSystem->getFullFilePath("effect/D3D11/lighting.fx")));
	m_nMapDefShader.insert(CW_SHADER_DEF_LIGHTINGTEX, getShader(fileSystem->getFullFilePath("effect/D3D11/lightingTex.fx")));

	return true;
}

NS_MINIR_END

#endif
