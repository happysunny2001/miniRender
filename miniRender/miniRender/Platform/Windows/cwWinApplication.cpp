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

#include "cwWinApplication.h"

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
#include <windows.h>
#include <windowsx.h>

#include "Base/cwValueMap.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Device/cwDevice.h"
#include "Camera/cwCamera.h"
#include "Event/cwEvent.h"
#include "Event/cwTouchEvent.h"
#include "Event/cwEventManager.h"

NS_MINIR_BEGIN

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static cwApplication* g_pApplication = nullptr;

cwApplication::cwApplication():
m_bResizing(false)
{
	g_pApplication = this;
	buildWindow();
}

cwApplication::~cwApplication()
{
	g_pApplication = nullptr;
}

bool cwApplication::buildWindow()
{
	m_nStrWinName = L"Mini Render - Win";

	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = m_nStrWinName.c_str();

	if (!RegisterClass(&wndClass)) {
		return false;
	}

	cwRepertory& repertory = cwRepertory::getInstance();
	repertory.addValue(gValueWinWidth,  cwValueMap(CWUINT(800)));
	repertory.addValue(gValueWinHeight, cwValueMap(CWUINT(600)));

	HWND hWnd = CreateWindow(
		m_nStrWinName.c_str(),
		m_nStrWinName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		repertory.getUInt(gValueWinWidth),
		repertory.getUInt(gValueWinHeight),
		NULL,
		NULL,
		hInstance,
		NULL);

	repertory.addValue(gValueWinHandle, cwValueMap(reinterpret_cast<CWVOID*>(hWnd)));

	return true;
}

void cwApplication::init()
{
	cwRepertory::getInstance().initAll();

	gameBegin();

	CWINT64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	m_dSecondPerCount = 1.0 / (CWDOUBLE)countsPerSecond;

	CWINT64 currCounter;
	QueryPerformanceCounter((LARGE_INTEGER*)&currCounter);
	m_iPrevTimeCounter = currCounter;
	m_iCurrTimeCounter = currCounter;
}

void cwApplication::mainLoop()
{
	CWINT64 currCounter;
	QueryPerformanceCounter((LARGE_INTEGER*)&currCounter);

	m_iCurrTimeCounter = currCounter;
	CWDOUBLE dDeltaTime = (m_iCurrTimeCounter - m_iPrevTimeCounter) * m_dSecondPerCount;
	if (dDeltaTime < 0) dDeltaTime = 0;

	cwRepertory::getInstance().getEngine()->mainLoop((CWFLOAT)dDeltaTime);
}

CWINT cwApplication::go()
{
	init();

	MSG msg = { 0 };

	cwRepertory& repertory = cwRepertory::getInstance();
	HWND hWnd = static_cast<HWND>(repertory.getPtr(gValueWinHandle));

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			mainLoop();
		}
	}

	return (int)msg.wParam;
}

void cwApplication::onResize(CWUINT width, CWUINT height)
{
	if (m_bResizing) return;
	cwRepertory& repertory = cwRepertory::getInstance();

	repertory.addValue(gValueWinWidth, cwValueMap(width));
	repertory.addValue(gValueWinHeight, cwValueMap(height));
	repertory.getDevice()->resize(width, height);

	CWFLOAT fov = repertory.getFloat(gValueFov);
	CWFLOAT nearZ = repertory.getFloat(gValueNearZ);
	CWFLOAT farZ = repertory.getFloat(gValueFarZ);
	CWFLOAT aspect = (CWFLOAT)width / (CWFLOAT)height;

	repertory.getEngine()->getDefaultCamera()->updateProjMatrix(fov, aspect, nearZ, farZ);
}

void cwApplication::onResize()
{
	m_bResizing = false;

	cwRepertory& repertory = cwRepertory::getInstance();
	onResize(repertory.getUInt(gValueWinWidth), repertory.getUInt(gValueWinHeight));
}

void cwApplication::onMouseDown(CWUINT keyState, CWINT x, CWINT y)
{
	cwRepertory::getInstance().getEventManager()->addEvent(cwTouchEvent::create(TouchTypeDown, cwVector2D(x, y)));
}

void cwApplication::onMouseUp(CWUINT keyState, CWINT x, CWINT y)
{
	cwRepertory::getInstance().getEventManager()->addEvent(cwTouchEvent::create(TouchTypeUp, cwVector2D(x, y)));
}

void cwApplication::onMouseMove(CWUINT keyState, CWINT x, CWINT y)
{
	cwRepertory::getInstance().getEventManager()->addEvent(cwTouchEvent::create(TouchTypeMoving, cwVector2D(x, y)));
}

void cwApplication::OnMouseWheel(CWUINT keyState, CWINT delta, CWINT x, CWINT y)
{
	cwRepertory::getInstance().getEventManager()->addEvent(cwTouchEvent::create(TouchTypeWheel, cwVector2D(x, y)));
}

LRESULT cwApplication::msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		gameEnd();
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		onResize(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_ENTERSIZEMOVE:
		m_bResizing = true;
		return 0;
	case WM_EXITSIZEMOVE:
		onResize();
		return 0;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		onMouseDown(static_cast<CWUINT>(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		onMouseUp(static_cast<CWUINT>(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		onMouseMove(static_cast<CWUINT>(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEWHEEL:
		OnMouseWheel(
			GET_KEYSTATE_WPARAM(wParam),
			GET_WHEEL_DELTA_WPARAM(wParam),
			GET_X_LPARAM(lParam),
			GET_Y_LPARAM(lParam));
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return minir::g_pApplication->msgProc(hWnd, msg, wParam, lParam);
}

NS_MINIR_END

#endif
