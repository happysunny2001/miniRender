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

#ifndef __CW_STREAMING_H__
#define __CW_STREAMING_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Base/cwUtils.h"

NS_MINIR_BEGIN

class cwLoadBatch;
class cwRemoveBatch;

class cwStreaming
{
public:
	cwStreaming();
	virtual ~cwStreaming();

	virtual CWVOID streamPrepare();
	virtual CWVOID streamBegin();
	virtual CWVOID streaming() = 0;
	virtual CWVOID streamEnd();
	virtual CWVOID streamClean();
	virtual CWVOID streamFailed();
	virtual CWVOID streamCancel();
	virtual CWVOID streamRelease();

	virtual cwRemoveBatch* buildRemoveBatch();

	inline cwLoadBatch* getLoadBatch() const { return m_pLoadBatch; }
	inline eStreamState getStreamState() const { return m_eState; }
	CWBOOL canRelease();

protected:
	cwLoadBatch* m_pLoadBatch;
	eStreamState m_eState;

};

NS_MINIR_END

#endif
