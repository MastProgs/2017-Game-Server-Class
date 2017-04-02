#pragma once

// SetWinowPos ( HWND, HWND, x, y, cx, cy, uFlags )
enum wParam_message
{
	server_start = 1,
	server_shutdown,
	debug_on,
	debug_off,
};

class win_main_class
{
	// window class
	HWND m_hWnd;
	MSG m_Message;
	WNDCLASS m_WndClass;
	static HINSTANCE m_hInst;

	// 그리기 위한 변수 - WM_PAINT
	static HDC m_hdc;
	static PAINTSTRUCT m_ps;
	static HBITMAP m_hBitmap;
	
	// call back function
	static LRESULT CALLBACK m_WndProc(HWND, UINT, WPARAM, LPARAM);

	// call back 함수내의 변수
	static HWND m_server_start_button, m_server_shutdown_button, m_debug_on_button, m_debug_off_button;
	static bool m_b_console_on, m_b_server_on;

public:
	win_main_class(const _In_ HINSTANCE hInstance, const _In_ int nCmdShow);
	~win_main_class();

	MSG* getMsg() { return &m_Message; }
};