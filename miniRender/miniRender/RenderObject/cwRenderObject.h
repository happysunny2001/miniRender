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

#ifndef _CW_RENDEROBJECT_H_
#define _CW_RENDEROBJECT_H_

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Ref/cwRef.h"
#include "Math/cwMath.h"

NS_MINIR_BEGIN

class cwBuffer;
class cwLayouts;
class cwCamera;

class CW_DLL cwRenderObject : public cwRef
{
public:
	cwRenderObject();
	virtual ~cwRenderObject();

	virtual CWBOOL init(
		ePrimitiveType topology,
		CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
		CWVOID* pIndexData, CWUINT uIndexCnt, 
		const CWSTRING& strLayout, CWUINT uPositionOffset=0);

	virtual CWBOOL rebuild(
		CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
		CWVOID* pIndexData, CWUINT uIndexCnt,
		CWUINT uPositionOffset = 0);

	virtual CWVOID preRender() {}
	CWVOID updateVertexData(CWVOID* pData);
	virtual CWVOID updateVertexData(CWVOID* pData, CWUINT uSize);

	virtual CWVOID updateIndexData(CWVOID* pData, CWUINT uSize);

	inline cwLayouts* getInputLayout() { return m_pLayout; }
	inline ePrimitiveType getPrimitiveTopology() { return m_nTopology; }

	inline cwBuffer* getVertexBuffer() { return m_pVertexBuffer; }
	inline cwBuffer* getIndexBuffer() { return m_pIndexBuffer; }

	inline CWVOID* getVertexData() { return m_pVertexData; }
	CWVOID* getVertexData(CWUINT i);
	inline CWUINT getVertexStride() const { return m_uStride; }
	inline CWUINT getVertexCnt() const { return m_uVertexCnt; }

	inline const cwAABB& getBoundingBox() const { return m_nAabb; }

	inline CWUINT* getIndexData() { return m_pIndexData; }
	inline CWUINT getIndexCnt() const { return m_uIndexCnt; }

protected:
	CWVOID saveBufferData(CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt, CWUINT uPositionOffset, CWVOID* pIndexData, CWUINT uIndexCnt);
	CWVOID calBoundingBox();

	virtual CWBOOL buildIndexBuffer(CWVOID* pIndexData, CWUINT uIndexCnt);

protected:
	cwBuffer* m_pVertexBuffer;
	cwBuffer* m_pIndexBuffer;
	cwLayouts* m_pLayout;
	ePrimitiveType m_nTopology;

	//native vertex and index buffer data
	CWVOID* m_pVertexData;
	CWUINT m_uStride;
	CWUINT m_uVertexCnt;
	CWUINT m_uPositionOffset;

	CWUINT* m_pIndexData;
	CWUINT m_uIndexCnt;

	cwAABB m_nAabb;

};

NS_MINIR_END

#endif
