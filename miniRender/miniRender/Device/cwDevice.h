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

#ifndef __CW_RENDER_DEVICE_H__
#define __CW_RENDER_DEVICE_H__

#include "Base/cwUtils.h"
#include "Base/cwStruct.h"
#include "Math/cwMath.h"

NS_MINIR_BEGIN

class cwLayouts;
class cwShader;
class cwVertexBuffer;
class cwIndexBuffer;
class cwBuffer;
class cwRenderObject;
class cwCamera;
class cwEntity;
class cwTexture;
class cwBlend;
class cwStencil;
class cwRenderTexture;
class cwViewPort;

class CW_DLL cwDevice
{
public:
	virtual ~cwDevice();

	virtual CWBOOL initDevice() = 0;
	virtual CWVOID resize(CWUINT width, CWUINT height) = 0;
	virtual CWVOID resize() = 0;

	virtual cwViewPort* createViewPort(CWFLOAT fTopLeftX, CWFLOAT fTopLeftY, CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fMinDepth=0.0f, CWFLOAT fMaxDepth=1.0f) = 0;
	virtual CWVOID setViewPort(cwViewPort* pViewPort);
	virtual CWVOID createRenderState() = 0;

	virtual CWVOID beginDraw() = 0;
	virtual CWVOID endDraw() = 0;
	virtual CWVOID swap() = 0;

	virtual CWVOID setInputLayout(cwLayouts* pInputLayout) = 0;
	virtual CWVOID setPrimitiveTopology(ePrimitiveType topology) = 0;
	virtual CWVOID setClearColor(const cwVector4D& fvColor) = 0;
	virtual CWVOID setRenderState(eRenderState e) = 0;
	virtual CWVOID DrawIndexed(CWUINT indexCnt, CWUINT startIndex, CWINT baseVertex) = 0;

	virtual cwShader* createShader(const string& strName) = 0;
	virtual cwBuffer* createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) = 0;
	virtual cwBuffer* createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, eBufferUsage usage, CWUINT cpuFlag) = 0;
	virtual cwBuffer* createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) = 0;

	virtual cwBlend* createBlend(const BlendData& blendData) = 0;
	virtual cwStencil* createStencil(const StencilData& stencliData) = 0;

	virtual CWVOID setVertexBuffer(cwBuffer* pVertexBuffer) = 0;
	virtual CWVOID setIndexBuffer(cwBuffer* pIndexBuffer) = 0;
	//set blend state, nullptr for restore blend state
	virtual CWVOID setBlend(const cwBlend* pBlendOper) = 0;
	//set stencil state, nullptr for restore stencil state
	virtual CWVOID setStencil(const cwStencil* pStencil) = 0;

	virtual cwTexture* createTexture(const CWSTRING& strFileName) = 0;
	virtual cwRenderTexture* createRenderTexture(CWFLOAT fWidth, CWFLOAT fHeight, eRenderTextureType eType = eRenderTextureShader) = 0;

	virtual CWVOID render(cwRenderObject* pRenderObj, const cwVector3D& worldPos, cwShader* pShader, cwCamera* pCamera) = 0;
	virtual CWVOID render(cwEntity* pEntity, cwCamera* pCamera) = 0;

	virtual CWVOID setShaderWorldTrans(cwShader* pShader, const cwMatrix4X4& trans, cwCamera* pCamera) = 0;
	virtual CWVOID setDiffuseTrans(cwShader* pShader, const cwMatrix4X4& trans) = 0;
	virtual CWVOID draw(cwShader* pShader, const string& strTech, cwRenderObject* pRenderObj) = 0;

	virtual CWVOID setRenderTarget(cwRenderTexture* pRenderTexture);

	inline CWBOOL getEnableMsaa4X() const { return m_bEnableMsaa4x; }

protected:
	cwDevice();

	virtual CWVOID createDefaultViewPort();
	virtual CWVOID createDefaultRenderTarget();

protected:
	cwVector4D m_fvClearColor;
	eRenderState m_eRenderState;
	cwBlend* m_pBlendState; //current blend state, just for record
	CWBOOL m_bEnableMsaa4x;

	cwRenderTexture* m_pRenderTargetBkBuffer;
	cwRenderTexture* m_pCurrRenderTarget;
	CWBOOL m_bRefreshRenderTarget;

	cwViewPort* m_pDefaultViewPort;
	cwViewPort* m_pCurrViewPort;
	CWBOOL m_bRefreshViewPort;

};

NS_MINIR_END

#endif