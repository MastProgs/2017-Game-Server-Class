#pragma once
#include "stdafx.h"

// call back function ���ٿ� ������
win_main_class* win_main_class::me = nullptr;

win_main_class::win_main_class(const _In_ HINSTANCE hInstance, const _In_ int nCmdShow)
{
	me = this;
	m_hInst = hInstance;

	m_WndClass.cbClsExtra = 0;
	m_WndClass.cbWndExtra = 0;
	m_WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ��� ���
	m_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);				// �⺻ Ŀ��
	m_WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// �»�� ������
	m_WndClass.hInstance = hInstance;
	m_WndClass.lpfnWndProc = m_WndProc;
	m_WndClass.lpszClassName = L"2017 Game Client";
	m_WndClass.lpszMenuName = NULL;
	m_WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&m_WndClass);

	// â ũ�� ������
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	static RECT window_size = { 0 , 0, 1024, 768 };
	AdjustWindowRect(&window_size, dwStyle, FALSE);

	m_hWnd = CreateWindow(m_WndClass.lpszClassName, m_WndClass.lpszClassName, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(m_hWnd, nCmdShow);
	
	network_class network(m_hWnd, hInstance);

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
		me->m_hdc = BeginPaint(hWnd, &me->m_ps);

		EndPaint(hWnd, &me->m_ps);
		break;
	}
	default:
		case WM_CREATE: {

			break;
		}
		case WM_DESTROY: {

			PostQuitMessage(0);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}