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

#include "cwWinUtils.h"

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_

#include <windows.h>

NS_MINIR_BEGIN

cwWinKeyMap& cwWinKeyMap::getInstance()
{
	static cwWinKeyMap keyMap;
	return keyMap;
}

cwWinKeyMap::cwWinKeyMap()
{
	m_nMapKeyMap[VK_BACK] = Back;
	m_nMapKeyMap[VK_TAB] = Tab;
	m_nMapKeyMap[VK_CLEAR] = KeyCode::Clear;
	m_nMapKeyMap[VK_RETURN] = Enter;
	m_nMapKeyMap[VK_SHIFT] = Shift;
	m_nMapKeyMap[VK_CONTROL] = Control;
	m_nMapKeyMap[VK_MENU] = Alt;
	m_nMapKeyMap[VK_PAUSE] = Pause;
	m_nMapKeyMap[VK_CAPITAL] = Caps;
	m_nMapKeyMap[VK_ESCAPE] = ESC;
	m_nMapKeyMap[VK_SPACE] = Space;
	m_nMapKeyMap[VK_PRIOR] = PageUp;
	m_nMapKeyMap[VK_NEXT] = PageDown;
	m_nMapKeyMap[VK_END] = End;
	m_nMapKeyMap[VK_HOME] = Home;
	m_nMapKeyMap[VK_LEFT] = Left;
	m_nMapKeyMap[VK_UP] = Up;
	m_nMapKeyMap[VK_RIGHT] = Right;
	m_nMapKeyMap[VK_DOWN] = Down;
	m_nMapKeyMap[VK_SELECT] = Select;
	m_nMapKeyMap[VK_PRINT] = KeyCode::Print;
	m_nMapKeyMap[VK_EXECUTE] = Execute;
	m_nMapKeyMap[VK_SNAPSHOT] = PrintScreen;
	m_nMapKeyMap[VK_INSERT] = Insert;
	m_nMapKeyMap[VK_DELETE] = Delete;
	m_nMapKeyMap[VK_HELP] = Help;
	m_nMapKeyMap[0x30] = Zero;
	m_nMapKeyMap[0x31] = One;
	m_nMapKeyMap[0x32] = Two;
	m_nMapKeyMap[0x33] = Three;
	m_nMapKeyMap[0x34] = Four;
	m_nMapKeyMap[0x35] = Five;
	m_nMapKeyMap[0x36] = Six;
	m_nMapKeyMap[0x37] = Seven;
	m_nMapKeyMap[0x38] = Eight;
	m_nMapKeyMap[0x39] = Nine;
	m_nMapKeyMap[0x41] = A;
	m_nMapKeyMap[0x42] = B;
	m_nMapKeyMap[0x43] = C;
	m_nMapKeyMap[0x44] = D;
	m_nMapKeyMap[0x45] = E;
	m_nMapKeyMap[0x46] = F;
	m_nMapKeyMap[0x47] = G;
	m_nMapKeyMap[0x48] = H;
	m_nMapKeyMap[0x49] = I;
	m_nMapKeyMap[0x4A] = J;
	m_nMapKeyMap[0x4B] = K;
	m_nMapKeyMap[0x4C] = L;
	m_nMapKeyMap[0x4D] = M;
	m_nMapKeyMap[0x4E] = N;
	m_nMapKeyMap[0x4F] = O;
	m_nMapKeyMap[0x50] = P;
	m_nMapKeyMap[0x51] = Q;
	m_nMapKeyMap[0x52] = R;
	m_nMapKeyMap[0x53] = S;
	m_nMapKeyMap[0x54] = T;
	m_nMapKeyMap[0x55] = U;
	m_nMapKeyMap[0x56] = V;
	m_nMapKeyMap[0x57] = W;
	m_nMapKeyMap[0x58] = X;
	m_nMapKeyMap[0x59] = Y;
	m_nMapKeyMap[0x5A] = Z;
	m_nMapKeyMap[VK_LWIN] = LWin;
	m_nMapKeyMap[VK_RWIN] = RWin;
	m_nMapKeyMap[VK_NUMPAD0] = Num0;
	m_nMapKeyMap[VK_NUMPAD1] = Num1;
	m_nMapKeyMap[VK_NUMPAD2] = Num2;
	m_nMapKeyMap[VK_NUMPAD3] = Num3;
	m_nMapKeyMap[VK_NUMPAD4] = Num4;
	m_nMapKeyMap[VK_NUMPAD5] = Num5;
	m_nMapKeyMap[VK_NUMPAD6] = Num6;
	m_nMapKeyMap[VK_NUMPAD7] = Num7;
	m_nMapKeyMap[VK_NUMPAD8] = Num8;
	m_nMapKeyMap[VK_NUMPAD9] = Num9;
	m_nMapKeyMap[VK_ADD] = Add;
	m_nMapKeyMap[VK_SEPARATOR] = Separator;
	m_nMapKeyMap[VK_SUBTRACT] = Sub;
	m_nMapKeyMap[VK_DECIMAL] = Decimal;
	m_nMapKeyMap[VK_DIVIDE] = Div;
	m_nMapKeyMap[VK_F1] = F1;
	m_nMapKeyMap[VK_F2] = F2;
	m_nMapKeyMap[VK_F3] = F3;
	m_nMapKeyMap[VK_F4] = F4;
	m_nMapKeyMap[VK_F5] = F5;
	m_nMapKeyMap[VK_F6] = F6;
	m_nMapKeyMap[VK_F7] = F7;
	m_nMapKeyMap[VK_F8] = F8;
	m_nMapKeyMap[VK_F9] = F9;
	m_nMapKeyMap[VK_F10] = F10;
	m_nMapKeyMap[VK_F11] = F11;
	m_nMapKeyMap[VK_F12] = F12;
	m_nMapKeyMap[VK_F13] = F13;
	m_nMapKeyMap[VK_F14] = F14;
	m_nMapKeyMap[VK_F15] = F15;
	m_nMapKeyMap[VK_F16] = F16;
	m_nMapKeyMap[VK_F17] = F17;
	m_nMapKeyMap[VK_F18] = F18;
	m_nMapKeyMap[VK_F19] = F19;
	m_nMapKeyMap[VK_F20] = F20;
	m_nMapKeyMap[VK_F21] = F21;
	m_nMapKeyMap[VK_F22] = F22;
	m_nMapKeyMap[VK_F23] = F23;
	m_nMapKeyMap[VK_F24] = F24;
	m_nMapKeyMap[VK_NUMLOCK] = NumLock;
	m_nMapKeyMap[VK_SCROLL] = Scroll;
	m_nMapKeyMap[VK_LSHIFT] = LShift;
	m_nMapKeyMap[VK_RSHIFT] = RShift;
	m_nMapKeyMap[VK_LCONTROL] = LControl;
	m_nMapKeyMap[VK_RCONTROL] = RControl;
	m_nMapKeyMap[VK_LMENU] = LMenu;
	m_nMapKeyMap[VK_RMENU] = RMenu;
}

cwWinKeyMap::~cwWinKeyMap()
{

}

KeyCode cwWinKeyMap::getKeyCode(CWUINT iKey) const
{
	auto it = m_nMapKeyMap.find(iKey);
	if (it == m_nMapKeyMap.end()) return KeyCode::KeyNone;
	return it->second;
}

NS_MINIR_END

#endif
