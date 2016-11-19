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

#include "cwMirror.h"
#include "Base/cwStruct.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Blend/cwBlend.h"
#include "Stencil/cwStencil.h"
#include "Engine/cwEngine.h"
//#include "Render/Stage/Layer/cwStageLayer.h"
#include "Render/Stage/cwStage.h"
#include "Render/cwRenderer.h"
#include "Render/cwRenderer.h"

NS_MINIR_BEGIN

cwMirror* cwMirror::create()
{
	cwMirror* pMirror = new cwMirror();
	if (pMirror && pMirror->init()) {
		pMirror->autorelease();
		return pMirror;
	}

	CW_SAFE_DELETE(pMirror);
	return nullptr;
}

cwMirror::cwMirror()
{

}

cwMirror::~cwMirror()
{

}

CWBOOL cwMirror::init()
{
	if (!cwEntity::init()) return CWFALSE;

	m_eRenderType = eRenderTypeMirror;
	m_nReflectPlane.m_nNormal.zero();
	m_nReflectPlane.m_nNormal.y = 1.0f;

	return CWTRUE;
}

CWVOID cwMirror::refreshTransform()
{
	cwEntity::refreshTransform();
	updatePlane();
}

CWVOID cwMirror::setReflectPlane(const cwPlane& plane)
{
	m_nReflectPlane = plane;
	m_nMatReflect.reflect(m_nReflectPlane);
}

CWVOID cwMirror::updatePlane()
{
	m_nReflectPlane.m_nNormal.zero();
	m_nReflectPlane.m_nNormal.y = 1.0f;

	m_nReflectPlane.update(m_nTrans);
	m_nMatReflect.reflect(m_nReflectPlane);
}

NS_MINIR_END
