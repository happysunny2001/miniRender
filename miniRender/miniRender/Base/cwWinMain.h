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

#ifndef __CW_WINMAIN_H__
#define __CW_WINMAIN_H__

#include <Windows.h>
#include <string>
using namespace std;

#include "cwMiniRender.h"
NS_USING_MINIR;

class CW_DLL cwWinMain
{
public:
	cwWinMain(HINSTANCE hInstance, int iShowCmd);
	virtual ~cwWinMain();

	virtual void setWindowName(const string& strName);

	virtual int go();
	virtual void loop();
	virtual void update(float dt);
	virtual void onResize();

	HWND getHWnd() { return m_hWnd; }
	HINSTANCE getHInstance() { return m_hInstance; }
	void showFPS();
	LRESULT msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int getWindowWidth() const { return m_iWindowWidth; }
	int getWindowHeight() const { return m_iWindowHeight; }

	void setWindowWidth(int iWidth) { m_iWindowWidth = iWidth; }
	void setWindowHeight(int iHeight) { m_iWindowHeight = iHeight; }

	float aspectRatio() const;

	virtual void onMouseDown(CWUINT keyState, CWINT x, CWINT y);
	virtual void onMouseUp(CWUINT keyState, CWINT x, CWINT y);
	virtual void onMouseMove(CWUINT keyState, CWINT x, CWINT y);
	virtual void OnMouseWheel(CWUINT keyState, CWINT delta, CWINT x, CWINT y);

protected:
	int createWindow();
	virtual void initAll();

protected:
	HINSTANCE m_hInstance;
	int m_iShowCmd;
	HWND m_hWnd;
	CWUINT m_iWindowWidth;
	CWUINT m_iWindowHeight;

	WCHAR* m_wpcWinName;

	cwTimer* m_pTimer;
	CWBOOL m_bResizing;

	CWINT m_uLastMouseX;
	CWINT m_uLastMouseY;
	CWFLOAT m_fTheta;
	CWFLOAT m_fPhi;
	CWFLOAT m_fRadius;

};

#endif