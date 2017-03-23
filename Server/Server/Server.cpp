#include "stdafx.h"

// ���� ����:
HINSTANCE g_hInst;
HBITMAP hBitmap = NULL;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;

	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);	// ��� ���
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					// �⺻ Ŀ��
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// �»�� ������
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = L"2017 Game Server";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);

	// â ũ�� ������
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	static RECT window_size = /*{ 0 , 0, 1024, 768 }*/{ 0,0,400,300 };
	AdjustWindowRect(&window_size, dwStyle, FALSE);

	hWnd = CreateWindow(WndClass.lpszClassName, WndClass.lpszClassName, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(hWnd, nCmdShow);


	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND server_start_button, server_shutdown_button, debug_on_button, debug_off_button;	
	static bool b_console_on = false, b_server_on = false;

	// SetWinowPos ( HWND, HWND, x, y, cx, cy, uFlags )
	enum wParam_message
	{
		server_start = 1,
		server_shutdown,
		debug_on,
		debug_off,

	};

	// �׸��� ���� ���� - WM_PAINT
	HDC hdc;
	PAINTSTRUCT ps;
	
    switch (message)
    {
	case WM_PAINT: {

		// bmp
		if (b_server_on) {
			hBitmap = (HBITMAP)LoadImage(g_hInst, L"my_profile_on.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		}
		else {
			hBitmap = (HBITMAP)LoadImage(g_hInst, L"my_profile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		}

		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;

		hdc = BeginPaint(hWnd, &ps);

		// bmp
		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hBitmap);
		GetObject(hBitmap, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 210, 92, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		// txt
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 20, 0, L"Source = github . com / Uzchowall / gameserverclass", 51);
		TextOut(hdc, 20, 225, L"Last Modified = 2017 - 03 - 23", 30);
		TextOut(hdc, 20, 245, L"blog = khjkhj2804 . blog . me", 29);
		TextOut(hdc, 20, 265, L"E-mail = khjkhj2804 @ naver . com", 33);
		
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
		case server_start: {
			ShowWindow(server_start_button, SW_HIDE);
			ShowWindow(server_shutdown_button, SW_SHOW);
			ShowWindow(debug_on_button, SW_SHOW);

			b_server_on = true;
			RECT rt = { 210, 90, 400, 300 };
			InvalidateRect(hWnd, &rt, FALSE);
			break;
		}
		case server_shutdown: {
			ShowWindow(server_start_button, SW_SHOW);
			ShowWindow(server_shutdown_button, SW_HIDE);
			ShowWindow(debug_on_button, SW_HIDE);
			ShowWindow(debug_off_button, SW_HIDE);

			b_server_on = false;
			RECT rt = { 210, 90, 400, 300 };
			InvalidateRect(hWnd, &rt, FALSE);
			if (b_console_on) { FreeConsole(); }
			break;
		}
		case debug_on: {
			ShowWindow(debug_on_button, SW_HIDE);
			ShowWindow(debug_off_button, SW_SHOW);

			AllocConsole();
			b_console_on = true;
			break;
		}
		case debug_off: {
			ShowWindow(debug_on_button, SW_SHOW);
			ShowWindow(debug_off_button, SW_HIDE);

			FreeConsole();
			b_console_on = false;
			break;
		}
		default:
			break;
		}
	}
		break;
	case WM_CREATE: {
		// x, y, �ʺ�, ����
		server_start_button = CreateWindow(L"Button", L"Server Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 150, 25, hWnd, (HMENU)server_start, g_hInst, nullptr);
		server_shutdown_button = CreateWindow(L"Button", L"Server Shut Down", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 150, 25, hWnd, (HMENU)server_shutdown, g_hInst, nullptr);
		ShowWindow(server_shutdown_button, SW_HIDE);

		debug_on_button = CreateWindow(L"Button", L"Debug On", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 20, 100, 25, hWnd, (HMENU)debug_on, g_hInst, nullptr);
		debug_off_button = CreateWindow(L"Button", L"Debug Off", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 20, 100, 25, hWnd, (HMENU)debug_off, g_hInst, nullptr);
		ShowWindow(debug_on_button, SW_HIDE);
		ShowWindow(debug_off_button, SW_HIDE);		

		break;
	}
	case WM_DESTROY: {
		if (b_console_on) { FreeConsole(); }
		KillTimer(hWnd, 1);
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	}
    default:
		break;
    }

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}
