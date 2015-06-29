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

#ifndef __CW_WIN_APPLICATION_H__
#define __CW_WIN_APPLICATION_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Platform/cwPlatform.h"

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_

#include <windows.h>

NS_MINI_BEGIN

class cwApplication
{
public:
	cwApplication();
	virtual ~cwApplication();

	virtual void gameBegin();
	virtual void gameEnd();

	virtual void gameBeginBackGround();
	virtual void gameEndBackGround();

public:
	CWINT go();

private:
	void init();
	void onResize(CWUINT width, CWUINT height);
	bool buildWindow();
	void mainLoop();

	HRESULT msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	CWWSTRING m_nStrWinName;

	CWDOUBLE m_dSecondPerCount;
	CWINT64 m_iPrevTimeCounter;
	CWINT64 m_iCurrTimeCounter;

	bool m_bResizing;

};

NS_MINI_END

#endif

#endif

