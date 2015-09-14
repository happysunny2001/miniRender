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

#include "cwKeyboardEventInterface.h"

NS_MINIR_BEGIN

CWVOID cwKeyboardEventInterface::onKeyDown(cwKeyboard* pKey)
{
	if (pKey) {
		if (!isKeyDown(pKey->getKeyCode())) {
			m_nMapKey[pKey->getKeyCode()] = 1;
		}
	}
}

CWVOID cwKeyboardEventInterface::onKeyUp(cwKeyboard* pKey)
{
	auto it = m_nMapKey.find(pKey->getKeyCode());
	if (it != m_nMapKey.end()) {
		m_nMapKey.erase(it);
	}
}

CWBOOL cwKeyboardEventInterface::isKeyDown(KeyCode code)
{
	auto it = m_nMapKey.find(code);
	if (it == m_nMapKey.end()) return CWFALSE;

	return CWTRUE;
}

NS_MINIR_END
