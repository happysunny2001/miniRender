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

#ifndef __CW_TEXTURE_MANAGER_H__
#define __CW_TEXTURE_MANAGER_H__

#include "Base/cwUtils.h"
#include "Base/cwVector.h"
#include "Base/cwMap.h"
#include "Ref/cwRef.h"
#include "Repertory/cwRepertory.h"
#include "cwTexture.h"
#include "cwCubeTexture.h"
//#include "cwRenderTexture.h"

#include <mutex>

NS_MINIR_BEGIN

class CW_DLL cwTextureManager : public cwRef
{
public:
	virtual ~cwTextureManager();

	cwTexture* createTexture(const CWSTRING& strName);
	cwTexture* createTexture(const CWSTRING& strName, CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format);
	cwTexture* createTextureArray(const std::vector<CWSTRING>& vecFiles);
	cwTexture* createCubeTexture(const CWSTRING& strName);
	cwTexture* createCubeTexture(CWUINT iSize);
	//cwRenderTexture* createRenderTexture(CWFLOAT fWidth, CWFLOAT fHeight, eRenderTextureType eType = eRenderTextureShader, CWBOOL bThreading=CWFALSE);

	cwTexture* createTextureThreadSafe(const CWSTRING& strName);
	cwTexture* createTextureThreadSafe(const CWSTRING& strName, CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format);
	cwTexture* createCubeTextureThreadSafe(const CWSTRING& strName);

	cwTexture* createRWTexture(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWBOOL bThreadSafe=CWFALSE);
	cwTexture* createRWTexture(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWUINT iMSAASamples, CWBOOL bThreadSafe = CWFALSE);
	cwTexture* createRTTexture(CWBOOL bThreadSafe = CWFALSE);
	cwTexture* createRTTexture(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWBOOL bShaderUsage = CWFALSE, CWBOOL bThreadSafe = CWFALSE);
	cwTexture* createRTTexture(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWUINT iMSAASamples, CWBOOL bShaderUsage = CWFALSE, CWBOOL bThreadSafe = CWFALSE);
	cwTexture* createDSTexture(CWFLOAT fWidth, CWFLOAT fHeight, CWBOOL bShaderUsage = CWFALSE, CWBOOL bThreadSafe = CWFALSE);
	cwTexture* createDSTexture(CWFLOAT fWidth, CWFLOAT fHeight, CWUINT iMSAASamples, CWBOOL bShaderUsage = CWFALSE, CWBOOL bThreadSafe = CWFALSE);
	cwTexture* createDSTexture(CWBOOL bThreadSafe = CWFALSE);

	cwTexture* getTexture(const CWSTRING& strName);
	cwTexture* getCubeTexture(const CWSTRING& strName);
	cwTexture* getTextureArray(const std::vector<CWSTRING>& vecFiles);

	CWVOID appendTexture(cwTexture* pTex);

	cwTexture* isExist(const CWSTRING& strName);

	CWVOID removeTexture(const CWSTRING& strName);
	CWVOID removeTexture(cwTexture* pTex);
	//CWVOID removeRenderTexture(cwRenderTexture* pTex);

	//CWVOID beginResize();
	//CWVOID onResize();

protected:
	static cwTextureManager* create();
	cwTextureManager() {}

	friend class cwRepertory;

private:
	cwMap<CWSTRING, cwTexture*> m_mapTexture;
	//cwVector<cwRenderTexture*> m_vecRenderTexture;

	std::mutex m_nMutex;

};

NS_MINIR_END

#endif
