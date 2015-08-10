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

#ifndef __CW_RENDERER_H__
#define __CW_RENDERER_H__

#include "Base/cwMacros.h"
#include "Ref/cwRef.h"

#include <vector>

NS_MINIR_BEGIN

class cwStage;
class cwCamera;

class cwRenderer : public cwRef
{
public:
	static cwRenderer* create();

	cwRenderer();
	virtual ~cwRenderer();

	virtual CWBOOL init();
	virtual CWVOID setCurrCamera(cwCamera* pCamera);
	virtual CWVOID addStage(cwStage* pStage);
	virtual cwStage* getStage(const CWSTRING& strName);

	const CWSTRING& getFullPath() const { return m_strFullPath; }
	CWVOID setFullPath(const CWSTRING& strPath) { m_strFullPath = strPath; }

	virtual CWVOID render();

protected:
	virtual CWVOID render(cwStage* pStage);

protected:
	CWSTRING m_strFullPath;

	std::vector<cwStage*> m_nVecStage;
	cwCamera* m_pCurrCamera;

};

NS_MINIR_END

#endif
