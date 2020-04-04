
#include "VibraClick.h"
#include <tchar.h>

VibraClick *VibraClick::_inst = nullptr;
VibraClick::VibraClick()
{
	// 初始化一些变量
	m_IsStartMouseRec = false;
	m_IsStartRunMouseRec = false;
	m_MouseRecIndex = 0;
	m_RunTimerId = -1;

	m_hhMouseHook = NULL;
	m_hWnd = NULL;
	_inst = this;
}

VibraClick::~VibraClick()
{
	// 清除鼠标记录内容
	CleanMouseRecInput();

	StopMouseRec();
	StopRunMouseRec();

	// 反注册热键
	UnregisterHotKey(this->m_hWnd, m_VibraClick_StartRec);	// Ctrl + S
	UnregisterHotKey(this->m_hWnd, m_VibraClick_RunRec);	// Ctrl + E
}

void VibraClick::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	m_VibraClick_StartRec = GlobalAddAtom(_T("VibraClick_StartRec")) - 0xC000;
	m_VibraClick_RunRec = GlobalAddAtom(_T("VibraClick_RunRec")) - 0xC000;

	RegisterHotKey(this->m_hWnd, m_VibraClick_StartRec, MOD_CONTROL, 'S');	// Ctrl + S
	RegisterHotKey(this->m_hWnd, m_VibraClick_RunRec, MOD_CONTROL, 'R');	// Ctrl + R
}

void VibraClick::StartMouseRec()
{
	// 清除鼠标记录内容
	CleanMouseRecInput();
	// 设置当前为录制鼠标操作状态
	m_IsStartMouseRec = true;
	// 开始鼠标Hook(全局鼠标钩子)
	m_hhMouseHook = SetWindowsHookEx(WH_MOUSE_LL, &MouseMessageProc, GetModuleHandle(NULL), NULL);
}

void VibraClick::StopMouseRec()
{
	// 取消设置当前为录制鼠标操作状态
	m_IsStartMouseRec = false;
	// 释放鼠标Hook
	if (m_hhMouseHook != NULL)
		UnhookWindowsHookEx(m_hhMouseHook);
}

void VibraClick::StartRunMouseRec()
{
	// 开始模拟鼠标操作
	m_IsStartRunMouseRec = true;
	// 设置运行索引
	m_MouseRecIndex = 0;
	// 开启定时器
	m_RunTimerId = SetTimer(m_hWnd, 999, 10, &TimerProc);
}

void VibraClick::StopRunMouseRec()
{
	// 停止运行鼠标记录的内容
	m_IsStartRunMouseRec = false;
	// 关闭定时器
	if (m_RunTimerId != -1)
	{
		KillTimer(m_hWnd, m_RunTimerId);
		m_RunTimerId = -1;
	}
}

void VibraClick::CleanMouseRecInput()
{
	// 清除记录的内容
	for (auto input : m_vecMouseRecInput)
	{
		delete input;
		input = nullptr;
	}
	// 释放vector占用内存
	MouseRecInputVector tmp;
	m_vecMouseRecInput.swap(tmp);
	// 设置运行索引
	m_MouseRecIndex = 0;
}

void VibraClick::OnHotKey(WPARAM nHotKeyId)
{
	if (nHotKeyId == m_VibraClick_StartRec)
	{
		if (m_IsStartMouseRec)
			StopMouseRec();
		else
			StartMouseRec();
	}
	else if (nHotKeyId == m_VibraClick_RunRec)
	{
		if (m_IsStartRunMouseRec)
			StopRunMouseRec();
		else
			StartRunMouseRec();
	}
}

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT nIDEvent, DWORD dwTime)
{
	switch (nIDEvent)
	{
		case 999:
		{
			// 取余方式进行循环运行
			int MouseRecIndex = VibraClick::GetInstance()->GetMouseRecIndex();
			MouseRecIndex %= VibraClick::GetInstance()->GetMouseRecInputVector().size();
			// 读取当前索引的鼠标模拟消息
			auto pMHD = VibraClick::GetInstance()->GetMouseRecInputVector()[MouseRecIndex++];
			// 通过INPUT进行模拟操作
			INPUT Input;
			Input.type = pMHD->type;
			memcpy((void *)&Input.mi, (void *)&pMHD->mi, sizeof(MOUSEINPUT));
			// 发送模拟消息
			SendInput(1, &Input, sizeof(INPUT));
			VibraClick::GetInstance()->SetMouseRecIndex(MouseRecIndex);
		}
		break;
	}
}

LRESULT CALLBACK MouseMessageProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
	PMSLLHOOKSTRUCT pStruct = (PMSLLHOOKSTRUCT)lParam;
	// LLMHF_INJECTED标志着: 事件是否被注入，通过SendInput后会触发这个标志，也就是模拟的处理消息则不记录了
	// nCode 表示有关Hook的消息
	/*
	 * Hook Codes
	 *
	 * #define HC_ACTION           0
	 * #define HC_GETNEXT          1
	 * #define HC_SKIP             2
	 * #define HC_NOREMOVE         3
	 * #define HC_NOREM            HC_NOREMOVE
	 * #define HC_SYSMODALON       4
	 * #define HC_SYSMODALOFF      5
	*/
	if (nCode < 0 || pStruct->flags & LLMHF_INJECTED)
	{
		return CallNextHookEx(VibraClick::GetInstance()->GetHHMouseHook(), nCode, wParam, lParam);
	}

	// 是否开启录制鼠标操作
	if (!VibraClick::GetInstance()->IsStartMouseRec())
		return CallNextHookEx(VibraClick::GetInstance()->GetHHMouseHook(), nCode, wParam, lParam);

	// 判断是否为鼠标数据, 当前只是列举一部分的鼠标消息，有需要可以自己加哈~
	if (
		wParam == WM_LBUTTONDOWN ||
		wParam == WM_LBUTTONUP ||
		wParam == WM_RBUTTONDOWN ||
		wParam == WM_RBUTTONUP ||
		wParam == WM_MBUTTONDOWN ||
		wParam == WM_MBUTTONUP ||
		wParam == WM_MOUSEMOVE)
	{
		MouseRecInput *Input = new MouseRecInput;
		// 现在固定为鼠标的模拟输入
		Input->type = INPUT_MOUSE;				
		// 设置输入的数据
		Input->mi.dx = pStruct->pt.x;
		Input->mi.dy = pStruct->pt.y;
		Input->mi.mouseData = pStruct->mouseData;
		Input->mi.time = pStruct->time;
		Input->mi.dwExtraInfo = pStruct->dwExtraInfo;
		switch (wParam)
		{
			case WM_LBUTTONDOWN:
				Input->mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				break;
			case WM_LBUTTONUP:
				Input->mi.dwFlags = MOUSEEVENTF_LEFTUP;
				break;
			case WM_RBUTTONDOWN:
				Input->mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
				break;
			case WM_RBUTTONUP:
				Input->mi.dwFlags = MOUSEEVENTF_RIGHTUP;
				break;
			case WM_MBUTTONDOWN:
				Input->mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
				break;
			case WM_MBUTTONUP:
				Input->mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
				break;
			case WM_MOUSEMOVE:
			{
				int cx_screen = ::GetSystemMetrics(SM_CXSCREEN);
				int cy_screen = ::GetSystemMetrics(SM_CYSCREEN);
				Input->mi.dx = pStruct->pt.x * 65536 / cx_screen;
				Input->mi.dy = pStruct->pt.y * 65536 / cy_screen;
				Input->mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
			}
			break;
		}
		VibraClick::GetInstance()->GetMouseRecInputVector().push_back(Input);
	}
	return CallNextHookEx(VibraClick::GetInstance()->GetHHMouseHook(), nCode, wParam, lParam);
}
