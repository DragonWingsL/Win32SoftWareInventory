

#pragma once

#ifndef __VIBRA_CLICK_H__
#define __VIBRA_CLICK_H__

#include <Windows.h>
#include <vector>

struct MouseRecInput {
	DWORD type;
	MOUSEINPUT mi;
};

// 定时器回调
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT nIDEvent, DWORD dwTime);
// 鼠标Hook消息处理
LRESULT CALLBACK MouseMessageProc(INT nCode, WPARAM wParam, LPARAM lParam);
class VibraClick
{
	using MouseRecInputVector = std::vector<MouseRecInput *>;
public:
	VibraClick();
	~VibraClick();

public:
	void Init(HWND hWnd);				// 初始化
	void StartMouseRec();				// 开始录制鼠标操作
	void StopMouseRec();				// 停止录制鼠标操作
	void StartRunMouseRec();			// 开始运行鼠标录制的内容
	void StopRunMouseRec();				// 停止运行鼠标录制的内容
	void CleanMouseRecInput();			// 清除鼠标记录内容
	void OnHotKey(WPARAM nHotKeyId);	// 热键处理

public:
	static VibraClick *GetInstance() { return _inst; }
	bool IsStartMouseRec() { return m_IsStartMouseRec; }
	bool IsStartRunMouseRec() { return m_IsStartRunMouseRec; }
	HHOOK GetHHMouseHook() { return m_hhMouseHook; }
	MouseRecInputVector &GetMouseRecInputVector() { return m_vecMouseRecInput; }
	int GetMouseRecIndex() { return m_MouseRecIndex; }
	void SetMouseRecIndex(int value) { m_MouseRecIndex = value; }

protected:
	static VibraClick *_inst;					// 实例自己
	HWND m_hWnd;								// 窗口句柄

	MouseRecInputVector m_vecMouseRecInput;		// 鼠标操作数据
	bool m_IsStartMouseRec;						// 是否开始记录鼠标操作
	bool m_IsStartRunMouseRec;					// 是否开始鼠标操作
	int m_MouseRecIndex;						// 鼠标操作索引
	UINT_PTR m_RunTimerId;						// 运行的定时器Id

	ATOM m_VibraClick_StartRec;					// 开始记录热键id
	ATOM m_VibraClick_RunRec;					// 开始运行记录热键id

	// 鼠标的Hook
	HHOOK m_hhMouseHook;

};

#endif // !__VIBRA_CLICK_H__