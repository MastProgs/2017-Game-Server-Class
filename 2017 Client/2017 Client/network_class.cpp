#include "stdafx.h"

network_class* network_class::me = nullptr;

BOOL CALLBACK network_class::AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_COMMAND: {

		switch (LOWORD(wParam))
		{
		case IDOK: {
			GetDlgItemText(hDlg, IDC_IP, me->m_server_ip, sizeof(me->m_server_ip));

			EndDialog(hDlg, IDOK);
			return TRUE;
		}
		case IDCANCEL: {
			EndDialog(hDlg, IDCANCEL);
			PostQuitMessage(0);
			return TRUE;
		}
		default:
			break;
		}

		break;
	}
	default:
		case WM_INITDIALOG: {

			SetDlgItemText(hDlg, IDC_IP, me->m_server_ip);

			return TRUE;
		}
		break;
	}

	return FALSE;
}

void network_class::init()
{
}

network_class::network_class(HWND hw, HINSTANCE hi) : m_hwnd{ hw }, m_hist{ hi }
{
	me = this;
	dlg_func_ptr func_ptr = nullptr;
	func_ptr = network_class::AboutDlgProc;

	DialogBox(m_hist, MAKEINTRESOURCE(IDD_DIALOG1), m_hwnd, reinterpret_cast<DLGPROC>(network_class::AboutDlgProc));
}


network_class::~network_class()
{
}
