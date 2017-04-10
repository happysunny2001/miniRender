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

#ifndef __CW_PIPELINE_NODE_H__
#define __CW_PIPELINE_NODE_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Ref/cwRef.h"

NS_MINIR_BEGIN

class cwShader;

class cwPipelineNode : public cwRef
{
public:
	cwPipelineNode();

	virtual CWVOID doRender() = 0;

	inline CWBOOL isEnable() const { return m_bEnable; }
	inline CWVOID setEnable(CWBOOL b) { m_bEnable = b; }

	inline const CWSTRING& name() const { return m_nName; }
	inline CWVOID setName(const CWSTRING& name) { m_nName = name; }

	virtual CWVOID bindingResultParameter(cwShader* pShader) {}
	virtual CWVOID addOuterPipeline(cwPipelineNode* pPipelineNode) {}

protected:
	CWSTRING m_nName;
	CWBOOL m_bEnable;

};

NS_MINIR_END

#endif
