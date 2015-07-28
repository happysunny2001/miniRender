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

class CW_DLL cwDevice
{
public:
	virtual ~cwDevice();

	virtual bool initDevice() = 0;
	virtual void resize(CWUINT width, CWUINT height) = 0;
	virtual void resize() = 0;

	virtual void createRenderTarget() = 0;
	virtual void createDepthStencil() = 0;
	virtual void createViewPort() = 0;
	virtual void createRenderState() = 0;

	virtual void beginDraw() = 0;
	virtual void endDraw() = 0;

	virtual void setInputLayout(cwLayouts* pInputLayout) = 0;
	virtual void setPrimitiveTopology(ePrimitiveType topology) = 0;
	virtual void setClearColor(const cwVector4D& fvColor) = 0;
	virtual void setRenderState(eRenderState e) = 0;
	virtual void DrawIndexed(CWUINT indexCnt, CWUINT startIndex, CWINT baseVertex) = 0;

	virtual CWVOID* getDevice() = 0;
	virtual CWVOID* getDeviceContext() = 0;

	virtual cwShader* createShader(const string& strName) = 0;
	virtual cwBuffer* createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) = 0;
	virtual cwBuffer* createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, eBufferUsage usage, CWUINT cpuFlag) = 0;
	virtual cwBuffer* createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) = 0;

	virtual cwBlend* createBlend(const BlendData& blendData) = 0;
	virtual cwStencil* createStencil(const StencilData& stencliData) = 0;

	virtual void setVertexBuffer(cwBuffer* pVertexBuffer) = 0;
	virtual void setIndexBuffer(cwBuffer* pIndexBuffer) = 0;
	//set blend state, nullptr for restore blend state
	virtual void setBlend(const cwBlend* pBlendOper) = 0;
	//set stencil state, nullptr for restore stencil state
	virtual void setStencil(const cwStencil* pStencil) = 0;

	virtual CW_RES_LOCK_DATA lockBuffer(cwBuffer* pBuffer) = 0;
	virtual void unlockBuffer(cwBuffer* pBuffer) = 0;

	virtual cwTexture* createTexture(const string& strFileName) = 0;
	virtual cwRenderTexture* createRenderTexture(float fWidth, float fHeight, eRenderTextureType eType=eRenderTextureShader) = 0;

	virtual void render(cwRenderObject* pRenderObj, const cwVector3D& worldPos, cwShader* pShader, cwCamera* pCamera) = 0;
	virtual void render(cwEntity* pEntity, cwCamera* pCamera) = 0;

	virtual void setShaderWorldTrans(cwShader* pShader, const cwMatrix4X4& trans, cwCamera* pCamera) = 0;
	virtual void setDiffuseTrans(cwShader* pShader, const cwMatrix4X4& trans) = 0;
	virtual void draw(cwShader* pShader, const string& strTech, cwRenderObject* pRenderObj) = 0;

	virtual void setRenderTarget(cwRenderTexture* pRenderTexture);
	virtual void setDepthStentil(cwRenderTexture* pDepthStencil);

	inline bool getEnableMsaa4X() const { return m_bEnableMsaa4x; }

protected:
	cwDevice();

protected:
	cwVector4D m_fvClearColor;
	eRenderState m_eRenderState;
	cwBlend* m_pBlendState; //current blend state, just for record
	bool m_bEnableMsaa4x;

	cwRenderTexture* m_pRenderTargetBkBuffer;
	cwRenderTexture* m_pCurrRenderTarget;
	cwRenderTexture* m_pDepthStencil;

	bool m_bRefreshRenderTarget;

};

NS_MINIR_END

#endif