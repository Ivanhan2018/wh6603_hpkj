#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "DrawSiceWnd.h"
#include "DlgListen.h"
#include "GameScore.h"

//////////////////////////////////////////////////////////////////////////
//消息定义

#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_OUT_CARD				(WM_USER+101)						//出牌消息
#define IDM_TRUSTEE_CONTROL			(WM_USER+102)						//托管控制
#define IDM_OPERATE_HAI_DI			(WM_USER+103)						///出牌消息
#define IDM_OUT_CARD_FINISH			(WM_USER+104)						//出牌动画结束消息
#define IDM_SEND_CARD_FINISH		(WM_USER+105)						//发牌动画结束消息
#define IDM_CARD_OPERATE			(WM_USER+106)						//玩家操作牌
#define IDM_SOUND					(WM_USER+107)						//声音提示
#define IDM_SHOW_SZ                 (WM_USER+114)                       //骰子消息
#define IDM_TING_CARD               (WM_USER+115)                       //听牌消息
#define IDM_MOPAI                   (WM_USER+116)                       //摸牌消息
#define IDM_SZSTART                 (WM_USER+117)                       //启动筛子按钮消息
#define IDM_SZ_BUTTON				(WM_USER+110)						//色子按钮消息

//////////////////////////////////////////////////////////////////////////

//视图位置
#define MYSELF_VIEW_ID				2									//视图位置

//////////////////////////////////////////////////////////////////////////

struct tagMoveCardItem
{
	bool			bReplace;											//是否补花
	bool			bOutCard;											//是否出牌,不是出牌则为发牌
	bool			bSendTail;											//是否从堆牌尾发牌
	WORD			wChairId;											//出牌或发牌位置
	WORD			wViewChairId;										//出牌或发牌视图位置
	BYTE			cbCardData;											//出牌或发牌数据
};

typedef	CWHArray<tagMoveCardItem>	CMoveCardItemArray;				//动画数据数组

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
    //zz数据变量
public:
	int								m_nDiceCount1;					//骰子数1
	int								m_nDiceCount2;					//骰子数2

	//标志变量
protected:
	bool							m_bOutCard;							//出牌标志
	bool							m_bWaitOther;						//等待标志
    BYTE							m_cbHaiDiCard;						///海底扑克
    bool                            m_bIsInitBT;                        ///按钮初始化
	bool							m_bHuangZhuang;						//荒庄标志
	bool							m_bTrustee[GAME_PLAYER];			//是否托管
	bool							m_bListenStatus[GAME_PLAYER];		//听牌标志

	//游戏属性
protected:
	LONGLONG						m_lCellScore;						///基础积分
	WORD							m_wBankerUser;						///庄家用户
	WORD							m_wCurrentUser;						//当前用户
	BYTE							m_cbFengQuan;						//风圈

	//堆立变量
protected:
	WORD							m_wHeapHand;						//堆立头部
	WORD							m_wHeapTail;						//堆立尾部

	//动作动画
protected:
	bool							m_bBombEffect;						//动作效果
	BYTE							m_cbBombFrameIndex;					//帧数索引

	//丢弃效果
	WORD							m_wDiscUser;						//丢弃用户
	BYTE							m_cbDiscFrameIndex;					//帧数索引	

	//发牌或出牌动画变量
protected:
	bool							m_bEnableAnimate;					//是否显示发牌动画
	bool							m_bCardMoving;						//是否在动画
	int								m_nXStep;							//X步长
	int								m_nYStep;							//Y步长
	int								m_nStepCount;						//移动步数
	enDirection						m_enMoveDirection;					//移动方向
	CPoint							m_ptMovePos;						//当前位置
	CRect							m_rcCardMove;						//绘画区域
	CMoveCardItemArray				m_MoveCardItemArray;				//动画数组
	tagMoveCardItem					m_CurrentMoveItem;					//当前动画数据

	//用户状态
protected:
	BYTE							m_cbCardData;						//出牌扑克
	WORD							m_wOutCardUser;						//出牌用户
	BYTE							m_cbUserAction[GAME_PLAYER];		//用户动作

	//位置变量
protected:
	CPoint							m_UserFlagPos[GAME_PLAYER];			//标志位置
	CPoint							m_UserListenPos[GAME_PLAYER];		//标志位置
	CPoint							m_PointTrustee[GAME_PLAYER];		///托管位置

	//位图变量
protected:
	CBitImage						m_ImageBack;						//背景图案
	CBitImage						m_ImageCenter;						//LOGO图
	CPngImage						m_ImageWait;						//等待提示
	CPngImage						m_ImageOutCard;						//出牌提示
	CPngImage						m_ImageUserFlag;					//用户标志
	CPngImage						m_ImageActionBack;					//动作背景
	CBitImage						m_ImageHuangZhuang;					//荒庄标志
	CPngImage						m_ImageTrustee;						//托管标志
	CPngImage						m_PngFengQuan;						//圈风标志	(无用,已被注释)
	CPngImage						m_PngListenFlag;					//听牌标志
	CPngImage						m_ImageActionAni;					//吃牌动画资源
	CPngImage						m_ImageDisc;						//丢弃效果
	CPngImage						m_ImageArrow;						//定时器箭头	
	CPngImage						m_ImageDiscKuang;					//丢弃效果
	CPngImage						m_ImageZhuang;						//庄家骰子	

	//扑克控件
public:
	CHeapCard						m_HeapCard[GAME_PLAYER];			//堆立扑克
	CUserCard						m_UserCard[3];						//用户扑克
	CTableCard						m_TableCard[GAME_PLAYER];					//桌面扑克
	CWeaveCard						m_WeaveCard[GAME_PLAYER][MAX_WEAVE];		//组合扑克
	CDiscardCard					m_DiscardCard[GAME_PLAYER];					//丢弃扑克
	CCardControl					m_HandCardControl;					//手上扑克
	CDiscardCard					m_WindCard[GAME_PLAYER];			//花牌扑克	

	//控件变量
public:
	CSkinButton					    m_btStart;							//开始按钮
	CSkinButton						m_btStusteeControl;					//拖管控制
	CSkinButton						m_btAutoPlayOn;
	CSkinButton						m_btAutoPlayOff;                    //取消托管
	CControlWnd						m_ControlWnd;						//控制窗口
	CDrawSiceWnd					m_DrawSiceWnd;						//色子控件
	CDlgListen						m_DlgListen;						//听牌对话框
	CGameScore						m_GameScore;						//成绩框

#ifdef VIDEO_GAME
	//视频组件
private:
	CVideoServiceControl 			m_DlgVedioService[4];				//视频窗口
#endif

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//继承函数
private:
	//重置界面
	virtual void ResetGameView();
	//调整控件
	virtual void RectifyControl(int nWidth, int nHeight);
	//绘画界面
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//功能函数
public:
	//基础积分
	void SetCellScore(LONGLONG lCellScore);
	//庄家用户
	void SetBankerUser(WORD wBankerUser);
	//荒庄设置
	void SetHuangZhuang(bool bHuangZhuang);
	//状态标志
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//出牌信息
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//动作信息
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//设置动作
	bool SetBombEffect(bool bBombEffect);
	//丢弃用户
	void SetDiscUser(WORD wDiscUser);
	//定时玩家
	void SetCurrentUser(WORD wCurrentUser);
	//设置托管
	void SetTrustee(WORD wTrusteeUser,bool bTrustee);
	//听牌标志
	void SetUserListenStatus(WORD wViewChairID, bool bListenStatus);
	//设置风圈
	void SetFengQuan( BYTE cbFengQuan );
	//设置堆牌头尾
	void SetHeapCardInfo( WORD wHeapHead, WORD wHeapTail );
	//玩家出牌或发牌动作
	void OnUserMoveCard( tagMoveCardItem &MoveCardItem );
	//开始出牌或发牌动画
	bool BeginMoveCard();
	//停止出牌或发牌动画
	bool StopMoveCard();
	//是否动画进行中
	bool IsMovingCard() { return m_bCardMoving; }
	//允许动画
	void EnableAnimate( bool bAnimate ) { m_bEnableAnimate = bAnimate; }
	//是否允许动画
	bool IsEnableAnimate() { return m_bEnableAnimate; }

	//辅助函数
protected:
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//构造出牌或发牌动画
	bool ConstructMoveCard();
	//绘画时间
	void DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
	//绘画准备
	void DrawUserReadyEx(CDC * pDC, int nXPos, int nYPos);

	//消息映射
protected:
	//开始按钮
	afx_msg void OnStart();
	//拖管控制
	afx_msg void OnStusteeControl();
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//定时器
	afx_msg void OnTimer(UINT nIDEvent);
//	//消息映射
//protected:
	//时间消息
	//VOID OnTimer(UINT nIDEvent);
	//开始按钮
	//afx_msg void OnStart();
	////海底按钮
	//afx_msg void OnHaiDi();
	////放弃按钮
	//afx_msg void OnNoHaiDi();
	//拖管控制
	//afx_msg void OnStusteeControl();
	////听牌按钮
	//afx_msg void OnTingCard();
	////不听按钮
	//afx_msg void OnNoTingCard();
	// 自动托管
	afx_msg void OnAutoPlayerOn();
	// 取消托管
	afx_msg void OnAutoPlayerOff();
	//建立函数
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	//afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
