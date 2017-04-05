#pragma once
//BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

class network_class
{
	static network_class* me;

	HWND m_hwnd = { 0 };
	HINSTANCE m_hist = { 0 };

	wchar_t m_server_ip[32] = { L"127.0.0.1" };
	static BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
	typedef BOOL (*dlg_func_ptr)(HWND, UINT, WPARAM, LPARAM);

	void init();

public:
	network_class(HWND, HINSTANCE);
	~network_class();
};

