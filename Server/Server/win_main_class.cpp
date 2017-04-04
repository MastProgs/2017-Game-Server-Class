#pragma once
#include "stdafx.h"

// call back function 접근용 포인터
win_main_class* win_main_class::me = nullptr;

win_main_class::win_main_class(const _In_ HINSTANCE hInstance, const _In_ int nCmdShow)
{
	me = this;
	m_hInst = hInstance;

	m_WndClass.cbClsExtra = 0;
	m_WndClass.cbWndExtra = 0;
	m_WndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);	// 흰색 배경
	m_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					// 기본 커서
	m_WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// 좌상단 아이콘
	m_WndClass.hInstance = hInstance;
	m_WndClass.lpfnWndProc = m_WndProc;
	m_WndClass.lpszClassName = L"2017 Game Server";
	m_WndClass.lpszMenuName = NULL;
	m_WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&m_WndClass);

	// 창 크기 고정용
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	static RECT window_size = /*{ 0 , 0, 1024, 768 }*/{ 0,0,400,300 };
	AdjustWindowRect(&window_size, dwStyle, FALSE);

	m_hWnd = CreateWindow(m_WndClass.lpszClassName, m_WndClass.lpszClassName, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(m_hWnd, nCmdShow);
	
	// server ip check
	m_ip = m_iocp.get_server_IP();

	while (GetMessage(&m_Message, NULL, 0, 0))
	{
		TranslateMessage(&m_Message);
		DispatchMessage(&m_Message);
	}
}

win_main_class::~win_main_class()
{

}

LRESULT CALLBACK win_main_class::m_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message)
	{
	case WM_PAINT: {

		// bmp
		if (true == me->m_b_server_on) { me->m_hBitmap = (HBITMAP)LoadImage(me->m_hInst, L"my_profile_on.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); }
		else { me->m_hBitmap = (HBITMAP)LoadImage(me->m_hInst, L"my_profile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); }

		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;

		me->m_hdc = BeginPaint(hWnd, &me->m_ps);

		// bmp
		hdcMem = CreateCompatibleDC(me->m_hdc);
		oldBitmap = SelectObject(hdcMem, me->m_hBitmap);
		GetObject(me->m_hBitmap, sizeof(bitmap), &bitmap);
		BitBlt(me->m_hdc, 210, 92, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		// txt
		SetBkMode(me->m_hdc, TRANSPARENT);
		TextOut(me->m_hdc, 20, 60, L"Source = github . com / Uzchowall / gameserverclass", 51);
		TextOut(me->m_hdc, 20, 80, L"IP Address = ", 13);
		//CreateWindowEx(NULL, L"EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 100, 150, 20, hWnd, NULL, me->m_hInst, NULL);
		TextOutA(me->m_hdc, 105, 80, me->m_ip, strlen(me->m_ip));
		TextOut(me->m_hdc, 20, 225, L"Last Modified = 2017 - 03 - 23", 30);
		TextOut(me->m_hdc, 20, 245, L"blog = khjkhj2804 . blog . me", 29);
		TextOut(me->m_hdc, 20, 265, L"E-mail = khjkhj2804 @ naver . com", 33);

		EndPaint(hWnd, &me->m_ps);
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
		case server_start: {
			ShowWindow(me->m_server_start_button, SW_HIDE);

			//me->server_init_func_call = me->m_iocp.init_server;
			//me->future_func = async(me->server_init_func_call);
			me->m_iocp.init_server();

			ShowWindow(me->m_server_shutdown_button, SW_SHOW);

			me->m_b_server_on = true;
			RECT rt = { 210, 90, 400, 300 };
			InvalidateRect(hWnd, &rt, FALSE);
			break;
		}
		case server_shutdown: {
			ShowWindow(me->m_server_shutdown_button, SW_HIDE);

			//me->future_func.get();
			me->m_iocp.release_IOCP();

			ShowWindow(me->m_server_start_button, SW_SHOW);

			me->m_b_server_on = false;
			RECT rt = { 210, 90, 400, 300 };
			InvalidateRect(hWnd, &rt, FALSE);
			break;
		}
		case debug_on: {
			ShowWindow(me->m_debug_on_button, SW_HIDE);
			ShowWindow(me->m_debug_off_button, SW_SHOW);

			AllocConsole();
			freopen("conout$", "wt", stdout);

			me->m_iocp.m_b_debug_mode = true;
			
			break;
		}
		case debug_off: {
			ShowWindow(me->m_debug_on_button, SW_SHOW);
			ShowWindow(me->m_debug_off_button, SW_HIDE);

			FreeConsole();
			fclose(stdout);

			me->m_iocp.m_b_debug_mode = false;
			break;
		}
		default:
			break;
		}
	}
		break;
	case WM_CREATE: {
		// x, y, 너비, 높이
		me->m_server_start_button = CreateWindow(L"Button", L"Server Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 150, 25, hWnd, (HMENU)server_start, me->m_hInst, nullptr);
		me->m_server_shutdown_button = CreateWindow(L"Button", L"Server Shut Down", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 150, 25, hWnd, (HMENU)server_shutdown, me->m_hInst, nullptr);
		ShowWindow(me->m_server_shutdown_button, SW_HIDE);

		me->m_debug_on_button = CreateWindow(L"Button", L"Debug On", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 20, 100, 25, hWnd, (HMENU)debug_on, me->m_hInst, nullptr);
		me->m_debug_off_button = CreateWindow(L"Button", L"Debug Off", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 20, 100, 25, hWnd, (HMENU)debug_off, me->m_hInst, nullptr);
		ShowWindow(me->m_debug_on_button, SW_SHOW);
		ShowWindow(me->m_debug_off_button, SW_HIDE);

		break;
	}
	case WM_DESTROY: {
		if (true == me->m_b_server_on || true == me->m_iocp.m_b_debug_mode) { FreeConsole(); fclose(stdout); }
		// KillTimer(hWnd, 1);
		DeleteObject(me->m_hBitmap);
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}