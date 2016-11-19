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

#include "cwCommandNode.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Entity/cwEntity.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderConstant.h"
#include "effect/cwEffect.h"
#include "Render/cwRenderer.h"
#include "Material/cwMaterial.h"

NS_MINIR_BEGIN

cwCommandNode::cwCommandNode() :
m_pRenderNode(nullptr),
m_pEffect(nullptr)
{

}

cwCommandNode::~cwCommandNode()
{
	m_pRenderNode = nullptr;
	m_pEffect = nullptr;
}

CWVOID cwCommandNode::reset()
{
	m_pRenderNode = nullptr;
	m_pEffect = nullptr;
}

CWVOID cwCommandNode::render()
{
	if (!m_pRenderNode) return;

	cwEffect* pEffect = m_pEffect ? m_pEffect : m_pRenderNode->getEffect();
	if (!pEffect || !pEffect->getShader()) return;

	this->setShaderConstant(pEffect->getShader());
	pEffect->render(m_pRenderNode);
}

CWVOID cwCommandNode::setShaderConstant(cwShader* pShader)
{
	pShader->setVariableMatrix(CW_SHADER_MAT_WORLD, m_pRenderNode->getTransformMatrix());
	pShader->setVariableMatrix(CW_SHADER_MAT_WORLD_INV_TRANS, m_pRenderNode->getTransInvTranspose());
}

NS_MINIR_END
