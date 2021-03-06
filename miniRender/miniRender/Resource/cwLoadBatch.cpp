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

#include "cwLoadBatch.h"
#include "Base/cwLog.h"
#include "Repertory/cwRepertory.h"
#include "Texture/cwTextureManager.h"
#include "Shader/cwShaderManager.h"
#include "cwStreaming.h"

NS_MINIR_BEGIN

cwLoadBatch* cwLoadBatch::create()
{
	cwLoadBatch* pBatch = new cwLoadBatch();
	if (pBatch) {
		pBatch->autorelease();
		return pBatch;
	}

	return nullptr;
}

cwLoadBatch::cwLoadBatch():
onLoadOver(nullptr),
m_pObjStreaming(nullptr)
{

}

cwLoadBatch::~cwLoadBatch()
{
	onLoadOver = nullptr;
	m_pObjStreaming = nullptr;
	cwLog::print("cwLoadBatch::~cwLoadBatch.\n");
}

CWVOID cwLoadBatch::reset()
{

}

CWVOID cwLoadBatch::addResource(cwResourceInfo& resInfo)
{
	m_nVecResource.push_back(resInfo);
}

CWVOID cwLoadBatch::addTexture2D(const CWSTRING& strName)
{
	cwResourceInfo resInfo;
	resInfo.m_eType = eResourceTypeTexture2D;
	resInfo.m_nStrName = strName;
	addResource(resInfo);
}

CWVOID cwLoadBatch::addTextureCube(const CWSTRING& strName)
{
	cwResourceInfo resInfo;
	resInfo.m_eType = eResourceTypeTextureCube;
	resInfo.m_nStrName = strName;
	addResource(resInfo);
}

CWVOID cwLoadBatch::addShader(const CWSTRING& strName)
{
	cwResourceInfo resInfo;
	resInfo.m_eType = eResourceTypeShader;
	resInfo.m_nStrName = strName;
	addResource(resInfo);
}

CWVOID cwLoadBatch::onOverCallback()
{
	if (onLoadOver) {
		onLoadOver(this);
	}
}

CWVOID cwLoadBatch::onStreaming()
{
	if (m_pObjStreaming)
		m_pObjStreaming->streaming();
}

CWVOID cwLoadBatch::streamingEnd()
{
	if (m_pObjStreaming)
		m_pObjStreaming->streamEnd();
}

CWVOID cwLoadBatch::streamRelease()
{
	if (m_pObjStreaming) {
		m_pObjStreaming->streamRelease();
	}
}

CWVOID cwLoadBatch::loadFailed()
{
	if (m_pObjStreaming)
		m_pObjStreaming->streamFailed();
}

CWBOOL cwLoadBatch::isStreamCancelled()
{
	if (m_pObjStreaming) {
		return m_pObjStreaming->getStreamState() == eStreamStateCancel;
	}

	return CWFALSE;
}

NS_MINIR_END
