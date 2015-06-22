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

#include <windows.h>
#include <windowsx.h>
#include "cwWinMain.h"
#include "cwTimer.h"
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "cwEffectManager.h"
#include "cwLayoutsManager.h"
#include "cwD3D11Device.h"
#include "cwStringConvert.h"
#include "cwAutoReleasePool.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static cwWinMain* g_WinMain = NULL;

cwWinMain::cwWinMain(HINSTANCE hInstance, int iShowCmd) :
m_hInstance(hInstance),
m_iShowCmd(iShowCmd),
m_wpcWinName(NULL),
m_iWindowWidth(800),
m_iWindowHeight(600),
m_bResizing(false),
m_pTimer(NULL),
m_uLastMouseX(0),
m_uLastMouseY(0),
m_fTheta(0.1f),
m_fPhi(cwMathUtil::cwPIx2/8.0f),
m_fRadius(10.0f),
m_pCamera(nullptr)
{
	g_WinMain = this;
	createWindow();
}

cwWinMain::~cwWinMain()
{
	g_WinMain = NULL;
	CW_SAFE_DELETE_ARRAY(m_wpcWinName);
	CW_SAFE_DELETE(m_pTimer);
	CW_SAFE_RELEASE_NULL(m_pCamera);
}

void cwWinMain::setWindowName(const string& strName)
{
	CW_SAFE_DELETE_ARRAY(m_wpcWinName);

	int iSize = MultiByteToWideChar(CP_ACP, 0, strName.c_str(), -1, NULL, 0);
	m_wpcWinName = new WCHAR[iSize];
	MultiByteToWideChar(CP_ACP, 0, strName.c_str(), -1, m_wpcWinName, iSize);
}

int cwWinMain::createWindow()
{
	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = m_wpcWinName==NULL?L"test":m_wpcWinName;

	if (!RegisterClass(&wndClass)) {
		return -1;
	}

	m_hWnd = CreateWindow(
		m_wpcWinName == NULL ? L"test" : m_wpcWinName,
		m_wpcWinName == NULL ? L"test" : m_wpcWinName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		m_iWindowWidth,
		m_iWindowHeight,
		NULL,
		NULL,
		m_hInstance,
		NULL);

	return 0;
}

void cwWinMain::initAll()
{
	cwRepertory::getInstance().addValue(gValueWinHandle, cwValueMap(reinterpret_cast<CWVOID*>(m_hWnd)));
	cwRepertory::getInstance().addValue(gValueWinWidth, cwValueMap(m_iWindowWidth));
	cwRepertory::getInstance().addValue(gValueWinHeight, cwValueMap(m_iWindowHeight));
	cwRepertory::getInstance().initAll();

	m_pTimer = new cwTimer();
	assert(m_pTimer != NULL);
	m_pTimer->reset();

	m_pCamera = cwCamera::create();
	CW_SAFE_RETAIN(m_pCamera);
	m_pCamera->updateViewMatrix(0, 1.0f, -10.0f);
}

float cwWinMain::aspectRatio() const
{
	return static_cast<float>(m_iWindowHeight) / m_iWindowHeight;
}

int cwWinMain::go()
{
	createWindow();
	initAll();

	MSG msg = { 0 };

	ShowWindow(m_hWnd, m_iShowCmd);
	UpdateWindow(m_hWnd);

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			loop();
		}
	}

	return (int)msg.wParam;
}

void cwWinMain::loop()
{
	m_pTimer->tick();
	showFPS();
	update(m_pTimer->deltaTime());

	cwRepertory::getInstance().getDevice()->beginDraw();
	draw();
	cwRepertory::getInstance().getDevice()->endDraw();
	cwRepertory::getInstance().getAutoReleasePool()->clear();
}

void cwWinMain::update(float dt)
{
	float x = m_fRadius*sinf(m_fPhi)*cosf(m_fTheta);
	float z = m_fRadius*sinf(m_fPhi)*sinf(m_fTheta);
	float y = m_fRadius*cosf(m_fPhi);

	m_pCamera->updateViewMatrix(x, y, z);
}

void cwWinMain::draw()
{

}

void cwWinMain::onResize()
{
	cwRepertory::getInstance().addValue(gValueWinWidth, m_iWindowWidth);
	cwRepertory::getInstance().addValue(gValueWinHeight, m_iWindowHeight);
	cwRepertory::getInstance().getDevice()->resize(m_iWindowWidth, m_iWindowHeight);

	m_pCamera->updateProjMatrix(0.25f*3.14159f, aspectRatio(), 1.0f, 1000.0f);
}

void cwWinMain::showFPS()
{
	cwRepertory::getInstance().refreshWindowTitle(cwStringConvert::createFPSString(wstring(m_wpcWinName), 
																				   m_pTimer->fps()).c_str());
}

LRESULT cwWinMain::msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		m_iWindowWidth  = LOWORD(lParam);
		m_iWindowHeight = HIWORD(lParam);

		if (!m_bResizing) {
			onResize();
		}

		return 0;
	case WM_ENTERSIZEMOVE:
		m_bResizing = true;
		return 0;
	case WM_EXITSIZEMOVE:
		m_bResizing = false;
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

void cwWinMain::onMouseDown(CWUINT keyState, CWINT x, CWINT y)
{
	m_uLastMouseX = x;
	m_uLastMouseY = y;

	SetCapture(m_hWnd);
}

void cwWinMain::onMouseUp(CWUINT keyState, CWINT x, CWINT y)
{
	ReleaseCapture();
}

void cwWinMain::onMouseMove(CWUINT keyState, CWINT x, CWINT y)
{
	if (keyState & CW_LBUTTON) {
		CWFLOAT dx = XMConvertToRadians(static_cast<CWFLOAT>(x - m_uLastMouseX));
		CWFLOAT dy = XMConvertToRadians(static_cast<CWFLOAT>(y - m_uLastMouseY));

		m_fTheta -= dx;
		m_fPhi   -= dy;

		m_fPhi = min(max(0.1f, m_fPhi), cwMathUtil::cwPI-0.1f);
	}

	m_uLastMouseX = x;
	m_uLastMouseY = y;
}

void cwWinMain::OnMouseWheel(CWUINT keyState, CWINT delta, CWINT x, CWINT y)
{
	if (delta > 0)
		m_fRadius++;
	else {
		m_fRadius--;
		if (m_fRadius < 1.0f)
			m_fRadius = 1.0f;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_WinMain->msgProc(hWnd, msg, wParam, lParam);
}

