#pragma once
#include "Stdafx.h"

#define IDI_LONG_DOWN   300
#define IDM_LONG_DOWN	WM_USER+300
class CSkinButtonEx:public CSkinButton
{
protected:
	 WORD		m_wID;
public:
	CSkinButtonEx();
	~CSkinButtonEx();
	VOID SetButtonID(WORD wID){m_wID = wID;}
	 //消息函数
protected:
	//按下
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//弹起
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};