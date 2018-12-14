#pragma  once
#include "Stdafx.h"


#define IDC_ONE_SCORE					121								//加注按钮
#define IDC_TWO_SCORE					122								//加注按钮
#define IDC_THREE_SCORE					123								//加注按钮
#define IDC_FOUR_SCORE					124								//加注按钮
#define IDC_FIVE_SCORE					125								//加注按钮
#define IDC_ACCEPT_SCORE				126								//加注按钮
#define IDC_CLEAN_SCORE					127								//清除按钮
#define IDM_ADD_SCORE				(WM_USER+110)						//加注消息
class CJettonView
{
protected:
	BOOL			m_bShow;
	CPoint			m_ptBasePoint;
protected:
	CSkinButton		m_btJetton[5];
	CSkinButton		m_btAccept;
	CSkinButton		m_btClean;
	CBitImage		m_ImageScoreBack;
	
protected:
	SCORE	m_sBetScore;
	SCORE	m_sCellScore;
	SCORE	m_sMaxCellScore;
public:
	CJettonView();
	~CJettonView();

public :
	//创建界面
	VOID	InitView(CWnd	*pWnd);
	//调整控件
	VOID	RectifyControl(int nWidth, int nHeight);
	//按钮消息
	BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	//设置显示
	VOID	ShowWindows(BOOL bShow);
	//绘制
	VOID	DrawJettonView(CDC *pDC,CDFontEx * pViewFont);
	//底注设置
	VOID	SetCellScore(SCORE score);
	//最大设置
	VOID	SetMaxScore(SCORE score);
	//获取下注
	SCORE	GetBetScore();
	//重置数据
	VOID	RestData();
	//加注
	VOID	AddScore(SCORE score);
	//刷新区域
	CRect	GetRect();
	//显示状态
	BOOL	IsWindowVisible(){return m_bShow;}

	VOID UpDateButton();

	VOID OnCleanScore();

};