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

#ifndef __CW_D3D11_TEXTURE_ARRAY_H__
#define __CW_D3D11_TEXTURE_ARRAY_H__

#ifdef _CW_D3D11_

#include "Base/cwMacros.h"
#include "Texture/cwTextureArray.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

class cwD3D11TextureArray : public cwTextureArray
{
public:
	static cwD3D11TextureArray* create(const std::vector<CWSTRING>& vecFiles);

	cwD3D11TextureArray();
	virtual ~cwD3D11TextureArray();

	virtual CWBOOL init(const std::vector<CWSTRING>& vecFiles) override;
	virtual CWHANDLE getHandle() const override;

protected:
	ID3D11ShaderResourceView* m_pShaderResource;

};

NS_MINIR_END

#endif //end _CW_D3D11_

#endif //end __CW_D3D11_TEXTURE_ARRAY_H__