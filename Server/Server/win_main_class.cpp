#include "stdafx.h"
#include "win_main_class.h"

HINSTANCE win_main_class::m_hInst = nullptr;

HDC win_main_class::m_hdc = nullptr;
PAINTSTRUCT win_main_class::m_ps = {};

HWND win_main_class::m_server_start_button = nullptr, win_main_class::m_server_shutdown_button = nullptr, win_main_class::m_debug_on_button = nullptr, win_main_class::m_debug_off_button = nullptr;
bool win_main_class::m_b_console_on = false, win_main_class::m_b_server_on = false;
HBITMAP win_main_class::m_hBitmap = nullptr;


win_main_class::win_main_class(const _In_ HINSTANCE hInstance, const _In_ int nCmdShow)
{
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
		if (true == m_b_server_on) { m_hBitmap = (HBITMAP)LoadImage(m_hInst, L"my_profile_on.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); }
		else { m_hBitmap = (HBITMAP)LoadImage(m_hInst, L"my_profile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); }

		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;

		m_hdc = BeginPaint(hWnd, &m_ps);

		// bmp
		hdcMem = CreateCompatibleDC(m_hdc);
		oldBitmap = SelectObject(hdcMem, m_hBitmap);
		GetObject(m_hBitmap, sizeof(bitmap), &bitmap);
		BitBlt(m_hdc, 210, 92, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		// txt
		SetBkMode(m_hdc, TRANSPARENT);
		TextOut(m_hdc, 20, 60, L"Source = github . com / Uzchowall / gameserverclass", 51);
		TextOut(m_hdc, 20, 225, L"Last Modified = 2017 - 03 - 23", 30);
		TextOut(m_hdc, 20, 245, L"blog = khjkhj2804 . blog . me", 29);
		TextOut(m_hdc, 20, 265, L"E-mail = khjkhj2804 @ naver . com", 33);

		EndPaint(hWnd, &m_ps);
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
		case server_start: {
			ShowWindow(m_server_start_button, SW_HIDE);
			ShowWindow(m_server_shutdown_button, SW_SHOW);
			ShowWindow(m_debug_on_button, SW_SHOW);

			m_b_server_on = true;
			RECT rt = { 210, 90, 400, 300 };
			InvalidateRect(hWnd, &rt, FALSE);
			break;
		}
		case server_shutdown: {
			ShowWindow(m_server_start_button, SW_SHOW);
			ShowWindow(m_server_shutdown_button, SW_HIDE);
			ShowWindow(m_debug_on_button, SW_HIDE);
			ShowWindow(m_debug_off_button, SW_HIDE);

			m_b_server_on = false;
			RECT rt = { 210, 90, 400, 300 };
			InvalidateRect(hWnd, &rt, FALSE);
			if (true == m_b_console_on) { FreeConsole(); }
			break;
		}
		case debug_on: {
			ShowWindow(m_debug_on_button, SW_HIDE);
			ShowWindow(m_debug_off_button, SW_SHOW);

			AllocConsole();
			m_b_console_on = true;
			break;
		}
		case debug_off: {
			ShowWindow(m_debug_on_button, SW_SHOW);
			ShowWindow(m_debug_off_button, SW_HIDE);

			FreeConsole();
			m_b_console_on = false;
			break;
		}
		default:
			break;
		}
	}
		break;
	case WM_CREATE: {
		// x, y, 너비, 높이
		m_server_start_button = CreateWindow(L"Button", L"Server Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 150, 25, hWnd, (HMENU)server_start, m_hInst, nullptr);
		m_server_shutdown_button = CreateWindow(L"Button", L"Server Shut Down", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 150, 25, hWnd, (HMENU)server_shutdown, m_hInst, nullptr);
		ShowWindow(m_server_shutdown_button, SW_HIDE);

		m_debug_on_button = CreateWindow(L"Button", L"Debug On", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 20, 100, 25, hWnd, (HMENU)debug_on, m_hInst, nullptr);
		m_debug_off_button = CreateWindow(L"Button", L"Debug Off", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 20, 100, 25, hWnd, (HMENU)debug_off, m_hInst, nullptr);
		ShowWindow(m_debug_on_button, SW_HIDE);
		ShowWindow(m_debug_off_button, SW_HIDE);

		break;
	}
	case WM_DESTROY: {
		if (true == m_b_server_on) { FreeConsole(); }
		KillTimer(hWnd, 1);
		DeleteObject(m_hBitmap);
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}