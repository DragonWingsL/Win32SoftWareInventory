//++++++++++++++++++++++++++++++++++
// 宏定义
//----------------------------------
#ifndef UNICODE
#define UNICODE											// 使用UNICODE编码,如果在编译器设置了使用UNICODE字符集此处可免
#endif
#ifndef _UNICODE
#define _UNICODE										// 使用UNICODE编码,如果在编译器设置了使用UNICODE字符集此处可免
#endif

//++++++++++++++++++++++++++++++++++
// 头文件
//----------------------------------
#include <windows.h>									// Win32程序最重要的头文件
#include <tchar.h>										// 兼容字符集头文件

#include "VibraClick.h"									// 鼠标模拟器头文件

//++++++++++++++++++++++++++++++++++
// 全局变量
//----------------------------------
TCHAR g_lpszClassName[] = _T("VibraClick");				// 窗口类的名称
TCHAR g_lpszWindowName[] = _T("VibraClick");			// 窗口的名称,(也就是窗口的标题)

VibraClick g_vibraClick;								// 鼠标模拟器

//++++++++++++++++++++++++++++++++++
// 函数声明
//----------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// 窗口消息处理过程
VOID VibraClick_Init(HWND);								// 初始化软件

//++++++++++++++++++++++++++++++++++
// 游戏主函数
//----------------------------------
INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, INT nCmdShow)
{
	/* 1.设计一个窗口类 */
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

	/* 2.注册窗口类 */
	if (RegisterClassEx(&wcex) == ((ATOM)0))
	{
		::MessageBox(NULL, _T("注册窗口类失败!"), _T("错误"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 3.创建窗口, 并居中显示 */
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
		::MessageBox(NULL, _T("创建窗口失败!"), _T("错误"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 4.更新显示窗口 */
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* 初始化 */
	VibraClick_Init(hWnd);

	/* 5.消息循环 */
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		// Sleep 休息10毫秒，避免主循环消耗过多的CPU
		Sleep(10);
	}

	return msg.wParam;
}

//++++++++++++++++++++++++++++++++++
// 窗口消息处理过程
//----------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_HOTKEY:
		g_vibraClick.OnHotKey(wParam);
		break;
	case WM_DESTROY:
		// 窗口销毁，释放资源
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