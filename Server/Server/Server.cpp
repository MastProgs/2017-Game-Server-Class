#include "stdafx.h"

// 전역 변수:
HINSTANCE g_hInst;
HWND server_start_button, server_shutdown_button, debug_on_button, debug_off_button;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;

	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 흰색 배경
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					// 기본 커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// 좌상단 아이콘
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = L"2017 Game Server";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);

	// 창 크기 고정용
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	RECT window_size = { 0 , 0, 1024, 768 };
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
	enum wParam_message
	{
		server_start = 1,
		server_shutdown,
		debug_on,
		debug_off,

	};

    switch (message)
    {
	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
		case server_start: {
			ShowWindow(server_start_button, SW_HIDE);
			ShowWindow(server_shutdown_button, SW_SHOW);
			ShowWindow(debug_on_button, SW_SHOW);
			break;
		}
		case server_shutdown: {
			ShowWindow(server_start_button, SW_SHOW);
			ShowWindow(server_shutdown_button, SW_HIDE);
			ShowWindow(debug_on_button, SW_HIDE);
			ShowWindow(debug_off_button, SW_HIDE);
			break;
		}
		case debug_on: {
			ShowWindow(debug_on_button, SW_HIDE);
			ShowWindow(debug_off_button, SW_SHOW);

			break;
		}
		case debug_off: {
			ShowWindow(debug_on_button, SW_SHOW);
			ShowWindow(debug_off_button, SW_HIDE);

			break;
		}
		default:
			break;
		}
	}
		break;
	case WM_CREATE: {
		// x, y, 너비, 높이
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
		PostQuitMessage(0);
		break;
	}
    default:
		break;
    }

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// 정보 대화 상자의 메시지 처리기입니다.
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
