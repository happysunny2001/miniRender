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

#ifndef __CW_D3D11_STENCIL_H__
#define __CW_D3D11_STENCIL_H__

#ifdef _CW_D3D11_

#include "Base/cwUtils.h"
#include "Stencil/cwStencil.h"

NS_MINIR_BEGIN

class CW_DLL cwD3D11Stencil : public cwStencil
{
public:
	static cwD3D11Stencil* create(const StencilData& stencilData);

	cwD3D11Stencil();
	virtual ~cwD3D11Stencil();

	virtual bool init(const StencilData& stencilData) override;

	virtual CWHANDLE getStencilHandlePtr() const { return static_cast<CWHANDLE>(m_pDepthStencilState); }

protected:
	D3D11_DEPTH_STENCIL_DESC m_nDepthStencilDesc;
	ID3D11DepthStencilState* m_pDepthStencilState;

};

NS_MINIR_END

#endif

#endif
