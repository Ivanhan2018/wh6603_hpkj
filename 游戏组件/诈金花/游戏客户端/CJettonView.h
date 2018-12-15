#pragma once
#include "Stdafx.h"
#include "Resource.h"

class CJettonView
{
	//分数变量
protected:
	LONGLONG			m_sCellScore;				//单元积分
	LONGLONG			m_sLimitMax;				//下注上限
	LONGLONG			m_sLimitMin;				//下注下限
	LONGLONG			m_sCheck;					//下注金额
	BOOL				m_bMingZhu;
	//图片资源
protected:
	CPngImage			m_PngBackView;				//背景图片
	CPngImage			m_ImageDrag;				// 拖动图片
	CFont				m_font;
	COLORREF			m_Color;

	//标识变量
protected:
	BOOL				m_bShowView;				//显示控制

	//位置变量
protected:
	CPoint				m_ptBasePoint;				//起始坐标
	CRect				m_rcCheckScore;
	
	//拖动变量
protected:
	bool				m_bMouseDown;			// 鼠标点击
	int					m_lDragSite;			// 拖动位置
	CRect				m_rectDrag;				// 拖动区域
	CSize				m_sizeDrag;				// 拖动图片大小


	//按钮控件
protected:
	CSkinButton			m_JettonButton[6];		//筹码按钮

public:
	//构造函数
	CJettonView();
	//析构函数
	~CJettonView();

	//----------------------------------------------------------------------------------

	//初始化
	BOOL	InitView(CWnd *pParent);
	//命令函数
	BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	// 消息解释
	BOOL	PreTranslateMessage(MSG * pMsg);
	//绘制界面
	VOID	DrawView(CWnd * pWnd,CDC * pDC, INT nWidth, INT nHeight);
	//调整控件
	VOID	RectifyControl(INT nWidth, INT nHeight);
	//显示控制
	VOID	ShowWindows(BOOL bshow);
	//----------------------------------------------------------------------------------

	//清空数据
	VOID	EmptyData();
	//设置信息
	VOID	SetSocreInfo(LONGLONG slimitmax,LONGLONG slimitmin,BOOL	bMingZhu);
	//加注操作
	VOID	AddScore(LONGLONG saddscore);
	//减注操作
	VOID	MinusScore(LONGLONG sminusscore);
	//还原最小
	VOID	CleanScore();
	//显示区域
	CRect	GetRect();
	//更新滑块
	VOID	UpdataDrag();
	//----------------------------------------------------------------------------------

	//拖动设置
	VOID    SetDragScore();
	//设置下注
	VOID	SetCheckScore(LONGLONG scheck);
	//设置上限
	VOID	SetLimitMaxScore(LONGLONG slimitmax);
	//设置下限
	VOID	SetLimitMinScore(LONGLONG slimitmin);
	//单元积分
	VOID	SetCellScore(LONGLONG	sCellScore);
	//获取下注
	LONGLONG	GetCheckScore();
	//获取上限
	LONGLONG	GetLimitMaxScore(){return m_sLimitMax;}
	//获取下限
	LONGLONG	GetLimitMinScore(){return m_sLimitMin;}

	BOOL	IsWindowVisible(){return m_bShowView;}
	
	VOID UpdateButton(BOOL bShow[4]);

};