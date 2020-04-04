//++++++++++++++++++++++++++++++++++
// �궨��
//----------------------------------
#ifndef UNICODE
#define UNICODE											// ʹ��UNICODE����,����ڱ�����������ʹ��UNICODE�ַ����˴�����
#endif
#ifndef _UNICODE
#define _UNICODE										// ʹ��UNICODE����,����ڱ�����������ʹ��UNICODE�ַ����˴�����
#endif

//++++++++++++++++++++++++++++++++++
// ͷ�ļ�
//----------------------------------
#include <windows.h>									// Win32��������Ҫ��ͷ�ļ�
#include <tchar.h>										// �����ַ���ͷ�ļ�

#include "VibraClick.h"									// ���ģ����ͷ�ļ�

//++++++++++++++++++++++++++++++++++
// ȫ�ֱ���
//----------------------------------
TCHAR g_lpszClassName[] = _T("VibraClick");				// �����������
TCHAR g_lpszWindowName[] = _T("VibraClick");			// ���ڵ�����,(Ҳ���Ǵ��ڵı���)

VibraClick g_vibraClick;								// ���ģ����

//++++++++++++++++++++++++++++++++++
// ��������
//----------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// ������Ϣ�������
VOID VibraClick_Init(HWND);								// ��ʼ�����

//++++++++++++++++++++++++++++++++++
// ��Ϸ������
//----------------------------------
INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, INT nCmdShow)
{
	/* 1.���һ�������� */
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WndProc;
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
	wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_lpszClassName;

	/* 2.ע�ᴰ���� */
	if (RegisterClassEx(&wcex) == ((ATOM)0))
	{
		::MessageBox(NULL, _T("ע�ᴰ����ʧ��!"), _T("����"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 3.��������, ��������ʾ */
	int cx = ::GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
	HWND hWnd = CreateWindowEx(
		NULL,
		g_lpszClassName,
		g_lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		(cx - 650) / 2,
		(cy - 650) / 2,
		650, 650,
		NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		::MessageBox(NULL, _T("��������ʧ��!"), _T("����"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 4.������ʾ���� */
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* ��ʼ�� */
	VibraClick_Init(hWnd);

	/* 5.��Ϣѭ�� */
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		// Sleep ��Ϣ10���룬������ѭ�����Ĺ����CPU
		Sleep(10);
	}

	return msg.wParam;
}

//++++++++++++++++++++++++++++++++++
// ������Ϣ�������
//----------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_HOTKEY:
		g_vibraClick.OnHotKey(wParam);
		break;
	case WM_DESTROY:
		// �������٣��ͷ���Դ
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	return ((LRESULT)0);
}

VOID VibraClick_Init(HWND hWnd)
{
	g_vibraClick.Init(hWnd);
}