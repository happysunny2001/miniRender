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

#include "cwCommandBatch.h"
#include "Entity/cwRenderNode.h"
#include "effect/cwEffect.h"

NS_MINIR_BEGIN

cwCommandBatch::cwCommandBatch() :
m_pUsingShader(nullptr),
m_iIndex(0)
{

}

cwCommandBatch::~cwCommandBatch()
{
	m_pUsingShader = nullptr;
}

CWVOID cwCommandBatch::reset()
{
	m_iIndex = 0;
	m_pUsingShader = nullptr;
}

CWBOOL cwCommandBatch::addRenderNode(cwRenderNode* pRenderNode)
{
	if (m_iIndex >= CW_COMMAND_NODE_MAX) return CWFALSE;

	if (pRenderNode) {
		if (pRenderNode->getEffect() && pRenderNode->getEffect()->getShader() == m_pUsingShader) {
			m_nCommandNode[m_iIndex].reset();
			m_nCommandNode[m_iIndex].binding(pRenderNode);
			m_iIndex++;
			return CWTRUE;
		}
	}

	return CWFALSE;
}

CWBOOL cwCommandBatch::addRenderNode(cwRenderNode* pRenderNode, cwEffect* pEffect)
{
	if (m_iIndex >= CW_COMMAND_NODE_MAX) return CWFALSE;

	if (pRenderNode && pEffect) {
		m_nCommandNode[m_iIndex].reset();
		m_nCommandNode[m_iIndex].binding(pRenderNode, pEffect);
		m_iIndex++;
		return CWTRUE;
	}

	return CWFALSE;
}

CWBOOL cwCommandBatch::full()
{
	return m_iIndex >= CW_COMMAND_NODE_MAX;
}

CWVOID cwCommandBatch::render()
{
	for (CWUINT i = 0; i < m_iIndex; ++i) {
		m_nCommandNode[i].render();
	}
}

NS_MINIR_END
