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

NS_MINIR_BEGIN

class CW_DLL cwApplication
{
public:
	cwApplication();
	virtual ~cwApplication();

	virtual void gameBegin() = 0;
	virtual void gameEnd() = 0;

	virtual void gameBeginBackGround() = 0;
	virtual void gameEndBackGround() = 0;

	virtual void refreshTitle(const CWSTRING& strTitle);

public:
	CWINT go();

private:
	void init();
	void onResize();
	void onResize(CWUINT width, CWUINT height);
	bool buildWindow();
	void mainLoop();

	LRESULT msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void onMouseDown(CWUINT keyState, CWINT x, CWINT y);
	void onMouseUp(CWUINT keyState, CWINT x, CWINT y);
	void onMouseMove(CWUINT keyState, CWINT x, CWINT y);
	void OnMouseWheel(CWUINT keyState, CWINT delta, CWINT x, CWINT y);

private:
	CWWSTRING m_nStrWinName;

	CWDOUBLE m_dSecondPerCount;
	CWINT64 m_iPrevTimeCounter;
	CWINT64 m_iCurrTimeCounter;

	CWUINT m_uWindowWidth;
	CWUINT m_uWindowHeight;

	bool m_bResizing;

};

NS_MINIR_END

#endif

#endif

