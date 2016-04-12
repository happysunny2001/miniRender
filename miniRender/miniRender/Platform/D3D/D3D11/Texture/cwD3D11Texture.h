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

#ifndef __CW_D3D11TEXTURE_H__
#define __CW_D3D11TEXTURE_H__

#ifdef _CW_D3D11_

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Texture/cwTexture.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

class CW_DLL cwD3D11Texture : public cwTexture
{
public:
	static cwD3D11Texture* create(const CWSTRING& strFileName);
	static cwD3D11Texture* createThreadSafe(const CWSTRING& strFileName);

	static cwD3D11Texture* create(CWVOID* pData, CWUINT64 uSize);
	static cwD3D11Texture* createThreadSafe(CWVOID* pData, CWUINT64 uSize);

	static cwD3D11Texture* create(CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format);
	static cwD3D11Texture* createThreadSafe(CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format);

	cwD3D11Texture();
	virtual ~cwD3D11Texture();

	virtual CWBOOL init(const CWSTRING& strFileName);
	virtual CWBOOL init(CWVOID* pData, CWUINT64 uSize);
	virtual CWBOOL init(CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format);
	virtual CWHANDLE getHandle() const override;

protected:
	ID3D11ShaderResourceView* m_pShaderResource;

};

NS_MINIR_END

#endif

#endif
