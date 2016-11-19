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
#include "Resource/cwLoadBatch.h"
#include "Resource/cwResourceLoader.h"

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

CWBOOL cwD3D11ShaderManager::init()
{
	if (!cwShaderManager::init()) return CWFALSE;

	return CWTRUE;
}

CWVOID cwD3D11ShaderManager::loadDefaultShader()
{
	//init d3d11 shader first
	//auto fileSystem = cwRepertory::getInstance().getFileSystem();

	cwLoadBatch* pBatch = cwLoadBatch::create();
	pBatch->addShader("lighting.fx");
	pBatch->addShader("colorTex.fx");
	pBatch->addShader("skyDome.fx");

	cwRepertory::getInstance().getResourceLoader()->loadSync(pBatch);

	m_nMapDefShader.insert(eDefShaderLighting, getShader("lighting.fx"));
	m_nMapDefShader.insert(eDefShaderTex, getShader("colorTex.fx"));
	m_nMapDefShader.insert(eDefShaderSkyDome, getShader("skyDome.fx"));
}

NS_MINIR_END

#endif
