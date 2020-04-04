

#pragma once

#ifndef __VIBRA_CLICK_H__
#define __VIBRA_CLICK_H__

#include <Windows.h>
#include <vector>

struct MouseRecInput {
	DWORD type;
	MOUSEINPUT mi;
};

// ��ʱ���ص�
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT nIDEvent, DWORD dwTime);
// ���Hook��Ϣ����
LRESULT CALLBACK MouseMessageProc(INT nCode, WPARAM wParam, LPARAM lParam);
class VibraClick
{
	using MouseRecInputVector = std::vector<MouseRecInput *>;
public:
	VibraClick();
	~VibraClick();

public:
	void Init(HWND hWnd);				// ��ʼ��
	void StartMouseRec();				// ��ʼ¼��������
	void StopMouseRec();				// ֹͣ¼��������
	void StartRunMouseRec();			// ��ʼ�������¼�Ƶ�����
	void StopRunMouseRec();				// ֹͣ�������¼�Ƶ�����
	void CleanMouseRecInput();			// �������¼����
	void OnHotKey(WPARAM nHotKeyId);	// �ȼ�����

public:
	static VibraClick *GetInstance() { return _inst; }
	bool IsStartMouseRec() { return m_IsStartMouseRec; }
	bool IsStartRunMouseRec() { return m_IsStartRunMouseRec; }
	HHOOK GetHHMouseHook() { return m_hhMouseHook; }
	MouseRecInputVector &GetMouseRecInputVector() { return m_vecMouseRecInput; }
	int GetMouseRecIndex() { return m_MouseRecIndex; }
	void SetMouseRecIndex(int value) { m_MouseRecIndex = value; }

protected:
	static VibraClick *_inst;					// ʵ���Լ�
	HWND m_hWnd;								// ���ھ��

	MouseRecInputVector m_vecMouseRecInput;		// ����������
	bool m_IsStartMouseRec;						// �Ƿ�ʼ��¼������
	bool m_IsStartRunMouseRec;					// �Ƿ�ʼ������
	int m_MouseRecIndex;						// ����������
	UINT_PTR m_RunTimerId;						// ���еĶ�ʱ��Id

	ATOM m_VibraClick_StartRec;					// ��ʼ��¼�ȼ�id
	ATOM m_VibraClick_RunRec;					// ��ʼ���м�¼�ȼ�id

	// ����Hook
	HHOOK m_hhMouseHook;

};

#endif // !__VIBRA_CLICK_H__