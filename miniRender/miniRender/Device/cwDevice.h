/*
Copyright © 2015-2016 Ziwei Wang

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
#include "cwDeviceStatus.h"
#include <stack>

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
class cwTextureArray;
class cwBatchEntity;
class cwCubeTexture;
class cwTerrain;

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
	virtual CWVOID setRenderState(eRenderState e) = 0;
	virtual eRenderState getRenderState();

	virtual CWVOID beginDraw(CWBOOL bClearColor = CWTRUE, CWBOOL bClearDepth = CWTRUE, CWBOOL bClearStencil = CWTRUE) = 0;
	virtual CWVOID endDraw() = 0;
	virtual CWVOID swap() = 0;

	virtual CWBOOL disableZBuffer();
	virtual CWBOOL enableZBuffer();

	virtual CWVOID setInputLayout(cwLayouts* pInputLayout) = 0;
	virtual CWVOID setPrimitiveTopology(ePrimitiveType topology) = 0;
	virtual CWVOID setClearColor(const cwVector4D& fvColor) = 0;

	virtual cwShader* createShader(const string& strFileName) = 0;
	virtual cwShader* createShaderThreadSafe(const string& strFileName) = 0;
	virtual cwShader* createShader(const CWCHAR* pcSourceData, CWUINT64 uSize) = 0;
	virtual cwShader* createShaderThreadSafe(const CWCHAR* pcSourceData, CWUINT64 uSize) = 0;

	virtual cwBuffer* createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) = 0;
	virtual cwBuffer* createVertexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, eBufferUsage usage, eAccessFlag cpuFlag) = 0;
	virtual cwBuffer* createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) = 0;
	virtual cwBuffer* createIndexBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, eBufferUsage usage, eAccessFlag cpuFlag) = 0;
	virtual cwBuffer* createShaderBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt, CWBOOL bWritable = CWFALSE, CWBOOL bAppend = CWFALSE) = 0;
	virtual cwBuffer* createBufferOutput(CWUINT uStride, CWUINT uCnt) = 0;
	virtual cwBuffer* createBuffer(CWUINT uCnt, eBufferUsage usage, eBufferBindFlag bindFlag, eAccessFlag uCpuFlag, CWUINT miscFlag, CWUINT uStride) = 0;
	virtual cwBuffer* createShaderStructedBuffer(CWVOID* pData, CWUINT uStride, CWUINT uCnt) = 0;

	virtual cwBlend* createBlend(const cwBlendData& blendData) = 0;
	virtual cwStencil* createStencil(const cwStencilData& stencliData) = 0;

	virtual cwTexture* createTexture(const CWSTRING& strFileName) = 0;
	virtual cwTexture* createTextureThreadSafe(const CWSTRING& strFileName) = 0;
	virtual cwTexture* createTexture(CWVOID* pData, CWUINT64 uSize) = 0;
	virtual cwTexture* createTextureThreadSafe(CWVOID* pData, CWUINT64 uSize) = 0;
	virtual cwTexture* createTexture(CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format) = 0;
	virtual cwTexture* createTextureThreadSafe(CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format) = 0;

	virtual cwTexture* createCubeTexture(const CWSTRING& strFileName) = 0;
	virtual cwTexture* createCubeTextureThreadSafe(const CWSTRING& strFileName) = 0;
	virtual cwCubeTexture* createCubeTexture(CWUINT iSize) = 0;
	virtual cwTexture* createCubeTexture(CWVOID* pData, CWUINT64 uSize) = 0;
	virtual cwTexture* createCubeTextureThreadSafe(CWVOID* pData, CWUINT64 uSize) = 0;

	virtual cwRenderTexture* createRenderTexture(CWFLOAT fWidth, CWFLOAT fHeight, eRenderTextureType eType = eRenderTextureShader, CWBOOL bThreading=CWFALSE) = 0;
	virtual cwTexture* createTextureArray(const std::vector<CWSTRING>& vecFiles) = 0;
	virtual cwTexture* createTextureArrayThreadSafe(const std::vector<CWSTRING>& vecFiles) = 0;

	virtual cwTexture* createRTTexture(CWBOOL bThreadSafe = CWFALSE) = 0;
	virtual cwTexture* createRTTexture(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWBOOL bShaderUsage = CWFALSE, CWBOOL bThreadSafe = CWFALSE) = 0;
	virtual cwTexture* createRWTexture(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWBOOL bThreadSafe = CWFALSE) = 0;
	virtual cwTexture* createDSTexture(CWFLOAT fWidth, CWFLOAT fHeight, CWBOOL bShaderUsage = CWFALSE, CWBOOL bThreadSafe = CWFALSE) = 0;
	virtual cwTexture* createDSTexture(CWBOOL bThreadSafe = CWFALSE) = 0;

	virtual cwBatchEntity* createBatchEntity() = 0;

	virtual cwTerrain* createTerrain(const CWSTRING& strConfFile) = 0;

	virtual CWVOID setVertexBuffer(cwBuffer* pVertexBuffer) = 0;
	virtual CWVOID setVertexBuffer(cwBuffer** pBuffers, CWUINT uCnt) = 0;
	virtual CWVOID setIndexBuffer(cwBuffer* pIndexBuffer) = 0;
	//set blend state, nullptr for restore blend state
	virtual CWVOID setBlend(const cwBlend* pBlendOper) = 0;
	//set stencil state, nullptr for restore stencil state
	virtual CWVOID setStencil(const cwStencil* pStencil) = 0;
	virtual CWVOID setShaderWorldTrans(cwShader* pShader, const cwMatrix4X4& trans, cwCamera* pCamera) = 0;
	virtual CWVOID setRenderTarget(cwTexture* pRenderTexture);
	virtual CWVOID setDepthStencil(cwTexture* pDepthStencil);

	virtual CWVOID clearShaderResource() = 0;

	virtual CWVOID render(cwRenderObject* pRenderObj, const cwVector3D& worldPos, cwShader* pShader, cwCamera* pCamera) = 0;
	virtual CWVOID draw(cwShader* pShader, const CWSTRING& strTech, cwRenderObject* pRenderObj) = 0;
	virtual CWVOID draw(cwShader* pShader, const CWSTRING& strTech, std::vector<cwRenderObject*>& vecRenderObject, CWUINT uCnt) = 0;
	virtual CWVOID drawGP(cwShader* pShader, const CWSTRING& strTech, cwGPInfo* pGPInfo) = 0;

	virtual CWVOID pushDeviceStatus();
	virtual CWVOID popDeviceStatus();

	inline CWBOOL getEnableMsaa4X() const { return m_bEnableMsaa4x; }
	inline CWVOID setEnableMsaa4X(bool b) { m_bEnableMsaa4x = b; }
	inline cwStencil* getStencil() const { return m_pStencil; }
	inline cwViewPort* getViewPort() const { return m_pCurrViewPort; }

protected:
	cwDevice();

	virtual CWVOID createDefaultViewPort();
	virtual CWVOID createDefaultRenderTarget();
	virtual CWVOID createDefaultStencil();

protected:
	cwVector4D m_fvClearColor;
	eRenderState m_eRenderState;
	cwBlend* m_pBlendState; //current blend state, just for record
	cwStencil* m_pStencil;  //current stencil state, just for record
	cwStencil* m_pDisableZBuffer; //for disable z-buffer
	CWBOOL m_bEnableMsaa4x;

	cwTexture* m_pRenderTargetBkBuffer;
	//cwTexture* m_pCurrRenderTarget;
	CWBOOL m_bRefreshRenderTarget;

	cwTexture* m_pRenderTarget;
	cwTexture* m_pDepthStencil;

	cwViewPort* m_pDefaultViewPort;
	cwViewPort* m_pCurrViewPort;
	CWBOOL m_bRefreshViewPort;

	stack<cwDeviceStatus> m_nDeviceStatusStack;

};

NS_MINIR_END

#endif