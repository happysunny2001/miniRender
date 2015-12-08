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

#include "cwShaderManager.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/cwFileSystem.h"
#include "Resource/cwResourceLoader.h"

NS_MINIR_BEGIN

cwShaderManager::cwShaderManager()
{

}

cwShaderManager::~cwShaderManager()
{

}

cwShader* cwShaderManager::createShader(const CWSTRING& strFile)
{
	cwRepertory& repertory = cwRepertory::getInstance();
	cwShader* pShader = nullptr;

	cwData* pData = repertory.getResourceLoader()->getShaderData(strFile);
	if (pData) {
		pShader = repertory.getDevice()->createShader((CWCHAR*)pData->m_pData, pData->m_uSize);
		if (pShader) {
			pShader->setName(strFile);
			appendShader(pShader);
		}

		delete pData;
	}

	return pShader;
}

cwShader* cwShaderManager::getShader(const CWSTRING& strFile)
{
	auto itFind = m_nMapShader.find(strFile);
	if (itFind != m_nMapShader.end()) {
		return itFind->second;
	}

	return createShader(strFile);
}

cwShader* cwShaderManager::getDefShader(eDefShaderID eShaderID)
{
	auto itFind = m_nMapDefShader.find(eShaderID);
	if (itFind != m_nMapDefShader.end()) return itFind->second;
	return nullptr;
}

CWBOOL cwShaderManager::isExist(const CWSTRING& strFile)
{
	std::unique_lock<std::mutex> lock(m_nMutex);

	if (m_nMapShader.find(strFile) == m_nMapShader.end()) return CWFALSE;
	return CWTRUE;
}

CWVOID cwShaderManager::appendShader(cwShader* pShader)
{
	if (pShader) {
		m_nMapShader.insert(pShader->getName(), pShader);
	}
}

CWVOID cwShaderManager::removeShader(cwShader* pShader)
{
	if (pShader) {
		m_nMapShader.erase(pShader->getName());
	}
}

CWVOID cwShaderManager::removeShader(const CWSTRING& strName)
{
	m_nMapShader.erase(strName);
}

CWBOOL cwShaderManager::init()
{
	buildShaderParam();
	return true;
}

CWVOID cwShaderManager::loadDefaultShader()
{

}

CWVOID cwShaderManager::buildShaderParam()
{
	m_strShaderParam[eShaderParamWorldViewProj]         = CW_SHADER_MAT_WORLDVIEWPROJ;
	m_strShaderParam[eShaderParamViewProj]			    = CW_SHADER_MAT_VIEWPROJ;
	m_strShaderParam[eShaderParamWorld]                 = CW_SHADER_MAT_WORLD;
	m_strShaderParam[eShaderParamWorldInvTrans]         = CW_SHADER_MAT_WORLD_INV_TRANS;
	m_strShaderParam[eShaderParamMaterial]              = CW_SHADER_MATERIAL;
	m_strShaderParam[eShaderParamDirectionalLight]      = CW_SHADER_DIRECTIONAL_LIGHT;
	m_strShaderParam[eShaderParamDirectionalLightCnt]   = CW_SHADER_DIRECTIONAL_LIGHT_CNT;
	m_strShaderParam[eShaderParamPointLight]            = CW_SHADER_POINT_LIGHT;
	m_strShaderParam[eShaderParamPointLightCnt]         = CW_SHADER_POINT_LIGHT_CNT;
	m_strShaderParam[eShaderParamSpotLight]             = CW_SHADER_SPOT_LIGHT;
	m_strShaderParam[eShaderParamSpotLightCnt]          = CW_SHADER_SPOT_LIGHT_CNT;
	m_strShaderParam[eShaderParamEyePosWorld]           = CW_SHADER_EYE_POSW;
	m_strShaderParam[eShaderParamDiffuseTexture]        = CW_SHADER_DIFFUSE_TEXTURE;
	m_strShaderParam[eShaderParamFogStart]              = CW_SHADER_FOG_START;
	m_strShaderParam[eShaderParamFogRange]              = CW_SHADER_FOG_RANGE;
	m_strShaderParam[eShaderParamFogColor]              = CW_SHADER_FOG_COLOR;
	m_strShaderParam[eShaderParamSkyCubeMap]            = CW_SHADER_SKY_CUBE_MAP;
	m_strShaderParam[eShaderParamReflectCubeMap]        = CW_SHADER_REFLECT_CUBE_MAP;
	m_strShaderParam[eShaderParamReflectFactor]         = CW_SHADER_REFLECT_FACTOR;
	m_strShaderParam[eShaderParamDynamicReflectCubeMap] = CW_SHADER_DYNAMIC_REFLECT_CUBE_MAP;
}

const CWSTRING& cwShaderManager::getShaderParamString(eShaderParamIndex eParam) const
{
	return m_strShaderParam[eParam];
}

NS_MINIR_END
