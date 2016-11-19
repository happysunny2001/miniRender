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
#include "Base/cwStringConvert.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Device/cwDevice.h"
#include "Camera/cwCamera.h"
#include "Event/cwEvent.h"
#include "Event/cwTouchEvent.h"
#include "Event/cwKeyboardEvent.h"
#include "Event/cwEventManager.h"
#include "cwWinUtils.h"

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

CWBOOL cwApplication::buildWindow()
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
		return CWFALSE;
	}

	cwRepertory& repertory = cwRepertory::getInstance();

	m_uWindowWidth = 1280;
	m_uWindowHeight = 600;

	repertory.addValue(gValueWinWidth, cwValueMap(CWUINT(m_uWindowWidth)));
	repertory.addValue(gValueWinHeight, cwValueMap(CWUINT(m_uWindowHeight)));

	HWND hWnd = CreateWindow(
		m_nStrWinName.c_str(),
		m_nStrWinName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_uWindowWidth,
		m_uWindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	repertory.addValue(gValueWinHandle, cwValueMap(reinterpret_cast<CWVOID*>(hWnd)));

	return CWTRUE;
}

CWVOID cwApplication::globalInit()
{

}

CWVOID cwApplication::init()
{
	globalInit();
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

CWVOID cwApplication::mainLoop()
{
	CWINT64 currCounter;
	QueryPerformanceCounter((LARGE_INTEGER*)&currCounter);

	m_iCurrTimeCounter = currCounter;
	CWDOUBLE dDeltaTime = (m_iCurrTimeCounter - m_iPrevTimeCounter) * m_dSecondPerCount;
	if (dDeltaTime < 0) dDeltaTime = 0;
	m_iPrevTimeCounter = m_iCurrTimeCounter;

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

CWVOID cwApplication::onResize(CWUINT width, CWUINT height)
{
	//if (m_bResizing) return;
	//cwRepertory& repertory = cwRepertory::getInstance();

	//m_uWindowWidth  = width;
	//m_uWindowHeight = height;

	//repertory.addValue(gValueWinWidth, cwValueMap(width));
	//repertory.addValue(gValueWinHeight, cwValueMap(height));
	//repertory.getDevice()->resize();
	//repertory.getEngine()->resize();

	//CWFLOAT fov = repertory.getFloat(gValueFov);
	//CWFLOAT nearZ = repertory.getFloat(gValueNearZ);
	//CWFLOAT farZ = repertory.getFloat(gValueFarZ);
	//CWFLOAT aspect = (CWFLOAT)width / (CWFLOAT)height;

	//cwCamera* pCamera = repertory.getEngine()->getCamera("Default");
	//if (pCamera) {
	//	pCamera->updateProjMatrixFov(fov, aspect, nearZ, farZ);
	//}
}

CWVOID cwApplication::onResize()
{
	m_bResizing = CWFALSE;

	cwRepertory& repertory = cwRepertory::getInstance();
	onResize(repertory.getUInt(gValueWinWidth), repertory.getUInt(gValueWinHeight));
}

CWVOID cwApplication::onMouseLeftDown(CWUINT keyState, CWINT x, CWINT y)
{
	CWINT heightY = m_uWindowHeight - y;
	if (heightY < 0) heightY = 0;

	cwTouchEvent* pEvent = cwTouchEvent::create(TouchTypeDown, cwVector2D((CWFLOAT)x, (CWFLOAT)heightY));
	pEvent->setTouchButton(TouchButtonLeft);
	cwRepertory::getInstance().getEventManager()->addEvent(pEvent);
}

CWVOID cwApplication::onMouseRightDown(CWUINT keyState, CWINT x, CWINT y)
{
	CWINT heightY = m_uWindowHeight - y;
	if (heightY < 0) heightY = 0;

	cwTouchEvent* pEvent = cwTouchEvent::create(TouchTypeDown, cwVector2D((CWFLOAT)x, (CWFLOAT)heightY));
	pEvent->setTouchButton(TouchButtonRight);
	cwRepertory::getInstance().getEventManager()->addEvent(pEvent);
}

CWVOID cwApplication::onMouseMiddleDown(CWUINT keyState, CWINT x, CWINT y)
{
	CWINT heightY = m_uWindowHeight - y;
	if (heightY < 0) heightY = 0;

	cwTouchEvent* pEvent = cwTouchEvent::create(TouchTypeDown, cwVector2D((CWFLOAT)x, (CWFLOAT)heightY));
	pEvent->setTouchButton(TouchButtonMiddle);
	cwRepertory::getInstance().getEventManager()->addEvent(pEvent);
}

CWVOID cwApplication::onMouseLeftUp(CWUINT keyState, CWINT x, CWINT y)
{
	CWINT heightY = m_uWindowHeight - y;
	if (heightY < 0) heightY = 0;

	cwTouchEvent* pEvent = cwTouchEvent::create(TouchTypeUp, cwVector2D((CWFLOAT)x, (CWFLOAT)heightY));
	pEvent->setTouchButton(TouchButtonLeft);
	cwRepertory::getInstance().getEventManager()->addEvent(pEvent);
}

CWVOID cwApplication::onMouseRightUp(CWUINT keyState, CWINT x, CWINT y)
{
	CWINT heightY = m_uWindowHeight - y;
	if (heightY < 0) heightY = 0;

	cwTouchEvent* pEvent = cwTouchEvent::create(TouchTypeUp, cwVector2D((CWFLOAT)x, (CWFLOAT)heightY));
	pEvent->setTouchButton(TouchButtonRight);
	cwRepertory::getInstance().getEventManager()->addEvent(pEvent);
}

CWVOID cwApplication::onMouseMiddleUp(CWUINT keyState, CWINT x, CWINT y)
{
	CWINT heightY = m_uWindowHeight - y;
	if (heightY < 0) heightY = 0;

	cwTouchEvent* pEvent = cwTouchEvent::create(TouchTypeUp, cwVector2D((CWFLOAT)x, (CWFLOAT)heightY));
	pEvent->setTouchButton(TouchButtonMiddle);
	cwRepertory::getInstance().getEventManager()->addEvent(pEvent);
}

CWVOID cwApplication::onMouseMove(CWUINT keyState, CWINT x, CWINT y)
{
	CWINT heightY = m_uWindowHeight - y;
	if (heightY < 0) heightY = 0;

	cwRepertory::getInstance().getEventManager()->addEvent(cwTouchEvent::create(TouchTypeMoving, cwVector2D((CWFLOAT)x, (CWFLOAT)heightY)));
}

CWVOID cwApplication::OnMouseWheel(CWUINT keyState, CWINT delta, CWINT x, CWINT y)
{
	CWINT heightY = m_uWindowHeight - y;
	if (heightY < 0) heightY = 0;

	cwRepertory::getInstance().getEventManager()->addEvent(cwTouchEvent::create(TouchTypeWheel, cwVector2D((CWFLOAT)x, (CWFLOAT)heightY)));
}

CWVOID cwApplication::onKeyDown(CWUINT wParam, CWUINT lParam)
{
	KeyCode code = cwWinKeyMap::getInstance().getKeyCode(wParam);
	if (code == KeyCode::KeyNone) return;

	cwKeyboardEvent* pEvent = cwKeyboardEvent::create(code, KeyState::KeyDown);
	cwRepertory::getInstance().getEventManager()->addEvent(pEvent);
}

CWVOID cwApplication::onKeyUp(CWUINT wParam, CWUINT lParam)
{
	KeyCode code = cwWinKeyMap::getInstance().getKeyCode(wParam);
	if (code == KeyCode::KeyNone) return;

	cwKeyboardEvent* pEvent = cwKeyboardEvent::create(code, KeyState::keyUp);
	cwRepertory::getInstance().getEventManager()->addEvent(pEvent);
}

LRESULT cwApplication::msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		gameEnd();
		gameRelease();
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		onResize(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_ENTERSIZEMOVE:
		m_bResizing = true;
		return 0;
	case WM_EXITSIZEMOVE:
		onResize();
		return 0;
	case WM_LBUTTONDOWN:
		onMouseLeftDown(static_cast<CWUINT>(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONDOWN:
		onMouseRightDown(static_cast<CWUINT>(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONDOWN:
		onMouseMiddleDown(static_cast<CWUINT>(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
		onMouseLeftUp(static_cast<CWUINT>(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONUP:
		onMouseRightUp(static_cast<CWUINT>(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONUP:
		onMouseMiddleUp(static_cast<CWUINT>(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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
	case WM_KEYDOWN:
		onKeyDown(static_cast<CWUINT>(wParam), static_cast<CWUINT>(lParam));
		return 0;
	case WM_KEYUP:
		onKeyUp(static_cast<CWUINT>(wParam), static_cast<CWUINT>(lParam));
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

CWVOID cwApplication::gameRelease()
{
	cwRepertory::getInstance().releaseAll();
}

CWVOID cwApplication::refreshTitle(const CWSTRING& strTitle)
{
	CWVOID* phWnd = cwRepertory::getInstance().getPtr(gValueWinHandle);
	if (!phWnd) return;

	HWND hWnd = reinterpret_cast<HWND>(phWnd);

	CWWSTRING wStrTitle = cwStringConvert::convertToWideChar(strTitle);
	SetWindowText(hWnd, wStrTitle.c_str());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return minir::g_pApplication->msgProc(hWnd, msg, wParam, lParam);
}

NS_MINIR_END

#endif
