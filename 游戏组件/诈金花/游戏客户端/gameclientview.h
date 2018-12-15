#pragma once

#include "Stdafx.h"

#include "CardControl.h"
#include "JettonControl.h"

#include "CJettonView.h"
#include "AdminControlDlg.h"
//////////////////////////////////////////////////////////////////////////
//消息定义

//消息定义
#define IDM_START					WM_USER+100							//开始消息
#define IDM_ADD_SCORE				WM_USER+101							//加注消息
#define	IDM_CONFIRM					WM_USER+109							//确定消息	
#define	IDM_CANCEL_ADD				WM_USER+119							//取消消息	

#define IDM_LOOK_CARD				WM_USER+104							//看牌消息
#define IDM_COMPARE_CARD			WM_USER+105							//比牌消息
#define IDM_OPEN_CARD				WM_USER+106							//开牌消息
#define IDM_GIVE_UP					WM_USER+107							//放弃消息
#define IDM_SEND_CARD_FINISH		WM_USER+108							//发牌完成
#define IDM_COMPARE_USER			WM_USER+110							//被选用户
#define IDM_FALSH_CARD_FINISH		WM_USER+114							//闪牌完成

//////////////////////////////////////////////////////////////////////////
//结构定义

//发牌子项
struct tagSendCardItem
{
	WORD							wMoveIndex;
	WORD							wChairID;							//发牌用户
	BYTE							cbCardData;							//发牌数据
};



//数组说明
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//数据变量
protected:
	WORD							m_wBankerUser;						//庄家用户
	LONGLONG						m_lMaxCellScore;					//最大下注
	LONGLONG						m_lCellScore;						//最少下注
	LONGLONG						m_lTableScore[GAME_PLAYER];			//下注数目
	LONGLONG						m_lMaxScore;						//封顶开牌
	bool							m_bInvest;							//等待下注
	bool							m_bCompareCard;						//选比标志
	WORD							m_wWaitUserChoice;					//等待标志
	bool							m_bSystemOpen;						//系统开牌
	LONGLONG						m_wCurrentTimes;					//当前倍数
	//牌型数据
public:
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//游戏状态
	bool							m_bShow;							//显示牌型
	TCHAR							m_tcBuffer[GAME_PLAYER][64];		//牌的类型

	//比牌变量
protected:
	WORD							m_wConmareIndex;					//比牌索引
	WORD							m_wConmareCount;					//比牌数目
	WORD							m_wCompareChairID[2];				//比牌用户

	WORD							m_wButtonDownUser;

	//闪牌变量
protected:
	bool							m_bFalsh;							//闪牌参数
	WORD							m_wFlashUser[GAME_PLAYER];			//闪牌用户
	WORD							m_wFalshCount;						//闪牌次数

	//动画标志
protected:
	bool							m_bCartoon;							//动画标志
	WORD							m_wLoserUser;						//比牌败户
	bool							m_bStopDraw;						//闪牌标志
	CSendCardItemArray				m_SendCardItemArray;				//发牌数组

	//位置变量
protected:
	CPoint							m_ptUserCompare;					//比牌位置
	CPoint							m_SendCardPos;						//发牌位置
	CPoint							m_ptUserCard[GAME_PLAYER];			//桌牌位置
	CPoint							m_KeepPos;							//保存位置

	CPoint							m_ptUserInfo[GAME_PLAYER];
	CPoint							m_ptLevel[GAME_PLAYER];
	CPoint							m_ptScore[GAME_PLAYER];

	INT								m_nXFace;
	INT								m_nYFace;
	INT								m_nXTimer;
	INT								m_nYTimer;
	//用户状态
protected:
	BOOL							m_bCompareUser[GAME_PLAYER];		//被选比牌用户

	//按钮控件
public:
	CSkinButton						m_btStart;							//开始按钮
	CSkinButton						m_btCancel;							//清理按钮
	CSkinButton						m_btConfirm;						//确认按钮
	CSkinButton						m_btAddScore;						//加注按钮
	CSkinButton						m_btFollow;							//跟注按钮
	CSkinButton						m_btGiveUp;							//放弃按钮
	CSkinButton						m_btLookCard;						//看牌按钮
	CSkinButton						m_btCompareCard;					//比牌按钮
	CSkinButton						m_btOpenCard;						//开牌按钮

	//控件变量
public:
	CCardControl					m_CardControl[GAME_PLAYER];			//用户扑克
	CJettonControl					m_JettonControl;					//筹码控件
public:
	CAdminControlDlg				m_AdminDlg;							//管理员控制
	CButton							m_btOpenAdmin;

	//位置信息
protected:
	CPoint							m_PointBanker[GAME_PLAYER];			//庄家位置
	CPoint							m_ptJettons[GAME_PLAYER];			//筹码位置

	//界面变量
protected:
	//CBitImage						m_ImageBanker;						//庄家资源
	//CBitImage						m_ImageCard;						//扑克资源
	CBitImage						m_ImageTitle;						//标题资源
	CBitImage						m_ImageViewBack;					//背景资源
	CBitImage						m_ImageViewCenter;					//背景资源
	CBitImage						m_ImageJeton;						//筹码资源
	CBitImage						m_ImageArrowhead;					//箭头资源
	CBitImage						m_ImageReady;						//准备资源
	CBitImage						m_ImagePocket;						//金袋资源
	CBitImage						m_ImageBalance;						//结算资源


	CPngImage						m_ImageBanker;
	CPngImage						m_ImageCard;
	CPngImage						m_ImageUserInfo;
	CPngImage						m_ImageTime;
	
	CPngImage						m_ImgaeScoreBackR;					// 结束背景
	CPngImage						m_ImgaeScoreBackL;					// 结束背景
	CPngImage						m_ImgaeScoreBackM;					// 结束背景
	CPngImage						m_ImgaeScoreWin;					// 结束分数
	CPngImage						m_ImgaeScoreLose;					// 结束分数
	CPngImage						m_ImgaeScoreAll;

	LONGLONG						m_lEndScore[GAME_PLAYER];
	CPoint							m_ptGameEndScore[GAME_PLAYER];

	//12-7-31
	CPngImage						m_PngOffLine;
	CPoint							m_ptOffLine[GAME_PLAYER];

public:
	CJettonView						m_JettonView;
	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();
	//WIN7支持
	virtual bool RealizeWIN7() { return true; }
	//重载函数
protected:
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//继承函数
private:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

public:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//功能函数
public:
	//设置下注
	void SetUserTableScore(WORD wChairID, LONGLONG lTableScore,LONGLONG lCurrentScore);
	//开始筹码动画
	void BeginMoveJettons();
	//停止筹码动画
	void StopMoveJettons();
	//胜利玩家
	void SetGameEndInfo( WORD wWinner );
	//等待选择
	void SetWaitUserChoice(WORD wChoice);
	//庄家标志
	void SetBankerUser(WORD wBankerUse);
	//左上信息
	void SetScoreInfo(LONGLONG lMaxCellScore,LONGLONG lCellScore);
	//选比标志
	void SetCompareCard(bool bCompareCard,BOOL bCompareUser[]);
	//停止更新
	void StopUpdataScore(bool bStop);
	//动画判断
	bool IsDispatchCard(){return m_bCartoon;}
	//获取信息
	bool GetCompareInfo(){return m_bCompareCard;};
	// 结束积分
	void SetGameEndScore(LONGLONG score[GAME_PLAYER]);
	// 系统开牌
	void SetSystemOpen(bool bSystem);
	// 设置封顶
	void SetMaxScore(LONGLONG lScore){m_lMaxScore = lScore;}
	// 当前倍数
	void SetCurrentTimes(LONGLONG wTimes){m_wCurrentTimes = wTimes;}
	
	//管理员控制
	VOID OpenAdminWnd();

	//动画函数
public:
	//比牌动画
	void PerformCompareCard(WORD wCompareUser[2], WORD wLoserUser);
	//发牌动画
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//闪牌动画
	bool bFalshCard(WORD wFalshUser[]);
	//发牌处理
	bool SendCard();
	//闪牌处理
	bool FlashCard();
	//比牌处理
	bool CompareCard();
	//停止闪牌
	void StopFlashCard();
	//停止比牌
	void StopCompareCard();
	//完成发牌
	void FinishDispatchCard();
	//更新视图
	void RefreshGameView();
	// 时钟绘制
	void DrawUserClockEx(CDC *pDC,INT nXPos, INT nYPos,WORD wChairID);
	//绘画扑克
	void DrawCompareCard(CDC * pDC, INT nXPos, INT nYPos, bool bChapped);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat=DT_LEFT);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect,int nWidth, int nWeight,UINT nFormat=DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	// 绘画结束分数
	void DrawOverScore(CDC * pDC, SCORE lOutNum, INT nXPos, INT nYPos );
	//消息映射
protected:
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
