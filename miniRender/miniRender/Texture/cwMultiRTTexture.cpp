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

#include "cwMultiRTTexture.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

cwMultiRTTexture* cwMultiRTTexture::create(CWFLOAT fWidth, CWFLOAT fHeight, CWBOOL bThreadSafe)
{
	cwMultiRTTexture* pTexture = new cwMultiRTTexture();
	if (pTexture && pTexture->init(fWidth, fHeight)) {
		if (!bThreadSafe)
			pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwMultiRTTexture::cwMultiRTTexture()
{

}

cwMultiRTTexture::~cwMultiRTTexture()
{

}

CWBOOL cwMultiRTTexture::init(CWFLOAT fWidth, CWFLOAT fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	return CWTRUE;
}

CWBOOL cwMultiRTTexture::addRTTexture(eFormat format, CWBOOL bThreadSafe)
{
	cwTexture* pTexture = cwRepertory::getInstance().getDevice()->createRTTexture(m_fWidth, m_fHeight, format, CWTRUE, bThreadSafe);
	if (pTexture) {
		m_nVecRTTextures.pushBack(pTexture);
		if (bThreadSafe)
			CW_SAFE_RELEASE(pTexture);

		return CWTRUE;
	}

	return CWFALSE;
}

CWBOOL cwMultiRTTexture::addRTTexture(eFormat format, CWUINT iMSAASamples, CWBOOL bThreadSafe)
{
	cwTexture* pTexture = cwRepertory::getInstance().getDevice()->createRTTexture(m_fWidth, m_fHeight, format, iMSAASamples, CWTRUE, bThreadSafe);
	if (pTexture) {
		m_nVecRTTextures.pushBack(pTexture);
		if (bThreadSafe)
			CW_SAFE_RELEASE(pTexture);

		return CWTRUE;
	}

	return CWFALSE;
}

CWHANDLE cwMultiRTTexture::getHandle() const
{
	return nullptr;
}

std::vector<CWHANDLE>* cwMultiRTTexture::getRenderHandleArray() const
{
	static std::vector<CWHANDLE> vecHandle;

	vecHandle.clear();
	for (auto pTexture : m_nVecRTTextures) {
		auto texHandle = pTexture->getRenderHandle();
		if (texHandle)
			vecHandle.push_back(texHandle);
	}

	return &vecHandle;
}

CWHANDLE cwMultiRTTexture::getHandle(CWUINT index) const
{
	if (index >= m_nVecRTTextures.size()) return nullptr;

	return m_nVecRTTextures.at(index)->getHandle();
}

cwTexture* cwMultiRTTexture::getTexture(CWUINT index) const
{
	if (index >= m_nVecRTTextures.size()) return nullptr;
	return m_nVecRTTextures.at(index);
}

NS_MINIR_END
