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

#include "cwRef.h"
#include "cwAutoReleasePool.h"
#include "Repertory/cwRepertory.h"
#include "Base/cwMacros.h"
#ifdef CW_DEBUG
#include "Ref/cwObjectMonitor.h"
#endif

#include <assert.h>

NS_MINIR_BEGIN

cwRef::cwRef() : m_iRef(1)
{
#ifdef CW_DEBUG
	cwRepertory::getInstance().getObjectMonitor()->addRef(this);
#endif
}

cwRef::~cwRef()
{
	m_iRef = 0;
}

CWVOID cwRef::release()
{
	assert(m_iRef > 0);
	m_iRef--;
	if (m_iRef == 0) {
#ifdef CW_DEBUG
		cwRepertory::getInstance().getObjectMonitor()->removeRef(this);
#endif
		delete this;
	}
}

CWVOID cwRef::autorelease()
{
	cwRepertory::getInstance().getAutoReleasePool()->addAutoReleaseRef(this);
}

NS_MINIR_END
