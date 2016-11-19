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

#ifndef __CW_COMMAND_BATCH_H__
#define __CW_COMMAND_BATCH_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "cwCommandNode.h"

NS_MINIR_BEGIN

#define CW_COMMAND_NODE_MAX 1024

class cwShader;
class cwRenderNode;
class cwEffect;

class cwCommandBatch
{
public:
	cwCommandBatch();
	~cwCommandBatch();

	CWVOID reset();
	CWBOOL addRenderNode(cwRenderNode* pRenderNode);
	CWBOOL addRenderNode(cwRenderNode* pRenderNode, cwEffect* pEffect);
	CWBOOL full();
	CWVOID render();

	inline CWVOID setUsingShader(cwShader* pShader) { m_pUsingShader = pShader; }
	inline cwShader* getUsingShader() const { return m_pUsingShader; }

protected:
	cwCommandNode m_nCommandNode[CW_COMMAND_NODE_MAX];
	CWUINT m_iIndex;
	cwShader* m_pUsingShader;

};

NS_MINIR_END

#endif
