#ifndef WND_TOP_NEWS_HEAD_FILE
#define WND_TOP_NEWS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////
//游戏类型
class CWndTopNews : public CWnd
{
	//点击信息
protected:
	bool							m_bHovering;						//盘旋标志
	WORD							m_wItemDown;						//按下子项
	WORD							m_wItemHover;						//盘旋子项
	WORD							m_wItemActive;						//激活子项
	int								m_nNewsLength;
	int								m_nFreshLength;		
	CFont							m_font;
	int								m_nNewsX;
	CBitImage ImageItemBack;
//	CEncircleBMP					m_ItemFrameEncircle;				//框架资源
	CString							m_strNews;
	//函数定义
public:
	//构造函数
	CWndTopNews();
	//析构函数
	virtual ~CWndTopNews();

	//消息函数
protected:
	BOOL PreTranslateMessage(MSG * pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
public:
	void SetTopNews(CString strNews);


	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif