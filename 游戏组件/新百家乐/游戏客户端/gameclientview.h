#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "GameLogic.h"
#include "DlgViewChart.h"
#include "ClientControl.h"

//////////////////////////////////////////////////////////////////////////

//筹码定义
#define BET_COUNT					7									//筹码数目
#define JETTON_RADII				68									//筹码半径

//消息定义confirm

#define IDM_PALY_BET				WM_USER+200							//加住信息
#define IDM_APPLY_BANKER			WM_USER+201							//申请信息
#define IDM_PLAY_SOUND				WM_USER+202							//申请信息
#define IDM_CONFIRM_BET				WM_USER+203							//申请信息
#define IDM_CANCEL_BET				WM_USER+204							//申请信息
#define IDM_CHANGE_MUSIC			WM_USER+205							//切换音乐

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//////////////////////////////////////////////////////////////////////////
//结构定义

//筹码信息
struct tagBetInfo
{
	int								nXPos;								//筹码位置
	int								nYPos;								//筹码位置
	BYTE							cbBetIndex;							//筹码索引
};

//限制提示
struct tagLimitTip
{
	int								nXPos;								//位置
	int								nYPos;								//位置
	int								nAlpha;								//透明
	BYTE							cbAreaIndex;						//索引
};

//选取闪动信息
struct tagFlashInfo
{
	BYTE							cbFlashArea;					//显示区域
	int								nFlashAreaAlpha;				//显示透明度
	bool							bFlashAreaAlpha;				//是否显示
};

enum enFlexMode
{
	enFlexNULL,
	enFlexBetTip,
	enFlexDealCrad,
	enFlexGameEnd,
};

//筹码数组
typedef CWHArray<tagBetInfo,tagBetInfo&> CBetInfoArray;

//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//限制信息
protected:
	LONGLONG						m_lMeMaxScore;						//最大下注
    LONGLONG						m_lAreaLimitScore;					//区域限制


	//下注数
protected:
	LONGLONG						m_lAllBet[AREA_MAX];				//总下注
	LONGLONG						m_lPlayBet[AREA_MAX];				//玩家下注
	//LONGLONG						m_lCurrentScore[AREA_MAX];			//当前下注

	//位置信息
protected:
	CWHArray<CRect>					m_ArrayBetArea[AREA_MAX];			//下注区域
	CPoint							m_ptBetFrame[AREA_MAX];				//下注显示位置
	CSize							m_sizeWin;							//窗口大小

	//扑克信息
protected:	
	BYTE							m_cbCardCount[2];					//扑克数目
    BYTE							m_cbTableCardArray[2][3];			//桌面扑克

	//动画变量
protected:
	int								m_nDealMove;						//发牌步数
	int								m_nDealIndex;						//发牌索引
	CPoint							m_ptDispatchCard;					//发牌位置
	CCardControl					m_DispatchCard;						//发牌扑克

	//伸缩动画
protected:
	enFlexMode						m_enFlexMode;						//伸缩索引
	int								m_nFlexMove;						//伸缩步数
	bool							m_bFlexShow;						//显示
	CPoint							m_ptFlexBeing;						//移动位置
	CPoint							m_ptFlexMove;						//移动位置
	CPngImage*						m_pImageFlex;						//移动图片

	//历史信息
protected:
	LONGLONG						m_lMeStatisticScore;				//游戏成绩
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//游戏记录
	int								m_nRecordFirst;						//开始记录
	int								m_nRecordLast;						//最后记录
	int								m_nScoreHead;						//成绩位置

	//状态变量
protected:
	LONG							m_nWinCount;						//赢数量
	LONG							m_nLoseCount;						//输数量
	BYTE							m_cbGameStatus;						//游戏状态
	WORD							m_wMeChairID;						//我的位置
	LONGLONG						m_lCurrentBet;						//当前筹码
	CString							m_strDispatchCardTips;				//发牌提示
	bool							m_bShowChangeBanker;				//轮换庄家
	bool							m_bNeedSetGameRecord;				//完成设置

	//闪动框
protected:
	CWHArray<BYTE>					m_ArrayFlashArea;					//胜利玩家
	int								m_nFlashAreaAlpha;					//胜利显示
	bool							m_bFlashAreaAlpha;					//胜利显示
	CWHArray<tagFlashInfo>			m_ArrayCurrentFlash;				//选中区域

	//限制提示
protected:
	CWHArray<tagLimitTip>			m_ArrayLimitTip;					//限制提示

	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	WORD							m_wBankerTime;						//做庄次数
	LONGLONG						m_lBankerScore;						//庄家积分
	LONGLONG						m_lBankerWinScore;					//庄家成绩	
	LONGLONG						m_lBankerWinTempScore;				//庄家成绩[零时保存]
	bool							m_bEnableSysBanker;					//系统做庄

	//当局成绩
public:
	LONGLONG						m_lPlayScore[AREA_MAX];				//玩家输赢
	SCORE							m_lPlayAllScore;					//玩家总输赢

	//结束动画
public:
	int								m_nWinShowArea;						//赢动画区域
	int								m_nWinShowIndex[3];					//赢动画索引

	//数据变量
protected:
	CPoint							m_ptBetNumber[AREA_MAX];			//数字位置
	CPoint							m_ptAllBetNumber[AREA_MAX];			//数字位置
	CBetInfoArray					m_BetInfoArray[AREA_MAX];			//筹码数组

protected:
	WORD							m_wSystemFaceID;

	//控件变量
public:
	CSkinButton						m_btBet1;							//筹码按钮
	CSkinButton						m_btBet3;							//筹码按钮
	CSkinButton						m_btBet5;							//筹码按钮
	CSkinButton						m_btBet10;							//筹码按钮
	CSkinButton						m_btBet50;							//筹码按钮
	CSkinButton						m_btBet100;							//筹码按钮
	CSkinButton						m_btBet1000;						//筹码按钮

	//CSkinButton						m_btBetComfirm;						//筹码确认
	CSkinButton						m_btBetCancel;						//筹码取消

	CSkinButton						m_btViewChart;						//查看路子
	
	CSkinButton						m_btApplyBanker;					//申请庄家
	CSkinButton						m_btCancelBanker;					//取消庄家

	CSkinButton						m_btScoreMoveL;						//移动成绩
	CSkinButton						m_btScoreMoveR;						//移动成绩

	CSkinButton						m_btValleysUp;						//按钮
	CSkinButton						m_btValleysDown;					//按钮
	CSkinButton						m_btLast;								//存款按钮
	CSkinButton						m_btNext;							//取款按钮

	CButton							m_btOpenAdmin;


	//字体资源
public:
	CDFontEx*						m_FontCurrent;						//当前字体
	CDFontEx						m_FontNormal;						//正常字体
	CDFontEx						m_FontValleysListOne;				//上庄列表字体一
	CDFontEx						m_FontValleysListTwo;				//上庄列表字体二

	//控件变量
public:
	INT								m_nShowValleyIndex;					//显示上庄列表
	CWHArray<WORD>					m_ValleysList;						//上庄列表			
	CCardControl					m_CardControl[2];					//扑克控件	
	CGameLogic						m_GameLogic;						//游戏逻辑
	CDlgViewChart					m_DlgViewChart;						//查看路子

	//界面变量
protected:
	CBitImage						m_ImageViewFill;					//背景位图
	CPngImage						m_ImageViewBack;					//背景位图
	CPngImage						m_ImageViewBackJB;					//背景位图
	CPngImage						m_ImageWinFlags;					//标志位图
	CPngImage						m_ImageBetView;						//筹码视图
	CPngImage						m_ImageScoreBack;					//数字视图
	CPngImage						m_ImageScoreNumber;					//数字视图
	CPngImage						m_ImageMeScoreBack;					//数字视图
	CPngImage						m_ImageMeScoreNumber;				//数字视图


	//边框资源
protected:
	CPngImage						m_ImageFrame[AREA_MAX];				//边框图片

	CPngImage						m_ImageMeBanker;					//切换庄家
	CPngImage						m_ImageChangeBanker;				//切换庄家
	CPngImage						m_ImageNoBanker;					//切换庄家
	CPngImage						m_ImageNodeBanker;					//切换庄家
	

	//伸缩动画
protected:
	CPngImage						m_ImageBetTip;						//下注提示
	CPngImage						m_ImageBankerTip;					//庄家提示
	CPngImage						m_ImageGameEnd;						//成绩背景
	CPngImage						m_ImageDealBack;					//发牌背景

	//结束资源
protected:
	CPngImage						m_ImageGamePoint;					//点数图片
	CPngImage						m_ImageGameEndMyScore;				//玩家结束积分
	CPngImage						m_ImageGameEndAllScore;				//玩家结束积分
	CPngImage						m_ImageGameEndPoint;				//结束界面点数

	CPngImage						m_ImageWinType;						//输赢类型
	CPngImage						m_ImageWinXian;						//闲输
	CPngImage						m_ImageWinZhuang;					//庄输


	//界面变量
protected:
	CPngImage						m_ImageBrandBoxRight;				//牌盒右

	CPngImage						m_ImageTimeNumber;					//时间数字
	CPngImage						m_ImageTimeBack;					//时间背景
	CPngImage						m_ImageTimeType;					//时间标识
	CPngImage						m_ImageTimeFlag;					//时间标识

	CPngImage						m_ImagePlayLeft;					//玩家信息左
	CPngImage						m_ImagePlayMiddle;					//玩家信息中
	CPngImage						m_ImagePlayRight;					//玩家信息右

	CPngImage						m_ImageScoreInfo;					//分数信息
	CPngImage						m_ImageBankerInfo;					//庄家信息
	CPngImage						m_ImageWaitValleys;					//等待上庄
	CPngImage						m_ImageWaitFirst;					//第一等待

	CPngImage						m_ImageLimitTip;					//限制提示

	//控制
public:
	HINSTANCE						m_hControlInst;
	IClientControlDlg*				m_pClientControlDlg;


	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//控制接口
public:
	//重置界面
	virtual VOID ResetGameView();

	//继承函数
private:
	//调整控件
	virtual VOID RectifyControl(int nWidth, int nHeight);
	//绘画界面
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);
	//WIN7支持
	virtual bool RealizeWIN7() { return true; }


	//设置函数
public:
	//设置状态
	VOID SetGameStatus(BYTE cbGameStatus);
	//我的位置
	void SetMeChairID(WORD wMeChairID);
	//设置最大下注
	void SetPlayBetScore(LONGLONG lPlayBetScore);
	//区域限制
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);
	//庄家信息
	void SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore);
	//庄家信息
	void SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore, LONGLONG lBankerWinScore, WORD wBankerTime);
	//庄家信息
	void SetBankerOverInfo(LONGLONG lBankerWinScore, WORD wBankerTime);
	//设置系统是否坐庄
	void SetEnableSysBanker(bool bEnableSysBanker);
	//个人下注
	void SetPlayBet(BYTE cbViewIndex, LONGLONG lBetCount);
	//全部下注
	void SetAllBet(BYTE cbViewIndex, LONGLONG lBetCount);
	//设置筹码
	void AddChip(BYTE cbViewIndex, LONGLONG lScoreCount);
	//当局成绩
	void SetCurGameScore(LONGLONG lPlayScore[AREA_MAX], SCORE lPlayAllScore);
	//设置扑克
	void SetCardInfo(BYTE cbCardCount[2], BYTE cbTableCardArray[2][3]);
	//设置筹码
	void SetCurrentBet(LONGLONG lCurrentBet);
	//历史记录
	void SetGameHistory(enOperateResult OperateResult, BYTE cbPlayerCount, BYTE cbBankerCount, BYTE cbKingWinner, bool bPlayerTwoPair, bool bBankerTwoPair);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//是否下注
	BOOL IsHasBet();
	//动画函数
public:	
	//开始发牌
	void DispatchCard();
	//结束发牌
	void FinishDispatchCard(bool bScene = false);
	//伸缩动画
	void FlexAnimation(enFlexMode nFlexMode, bool bShow, bool bMove = true);
	//闪烁动画
	void FlashAnimation( bool bBegin );


	//绘画函数
protected:
	// 绘画顶部信息
	void DrawTopInfo(CDC *pDC, int nWidth, int nHeight);
	// 绘画底部信息
	void DrawBottomInfo(CDC *pDC, int nWidth, int nHeight);
	//显示输赢
	void DrawWinType(CDC *pDC, int nWidth, int nHeight, int nBeginX, int nBeginY );
	//显示结果
	void DrawGameOver(CDC *pDC, int nWidth, int nHeight, int nBeginX, int nBeginY );
	// 闪烁边框
	void DrawFlashFrame(CDC *pDC, int nWidth, int nHeight);

	//计算限制
public:
	//最大下注
	LONGLONG GetMaxPlayerScore(BYTE cbBetArea);

	//界面函数
public:
	//清理筹码
	void CleanUserBet();
	//发牌提示
	void SetDispatchCardTips();
	//系统头像
	void SetSystemFaceID(WORD wFaceId){m_wSystemFaceID =wFaceId;}
	//内联函数
public:
	//当前筹码
	inline LONGLONG GetCurrentBet() { return m_lCurrentBet; }	
	//当前筹码
	//inline LONGLONG GetCurrentBet(BYTE cbArea){return m_lCurrentScore[cbArea];}
	//设置当前筹码
	//void SetCurrentBet(WORD cbArea,LONGLONG lscore);

	void CleanAreaChip(BYTE index);
	//内部函数
private:
	//获取区域
	BYTE GetBetArea(CPoint MousePoint);
	//推断赢家
	void DeduceWinner(BYTE* pWinArea);
	//推断赢家
	void DeduceWinner(BYTE &cbWinner, BYTE &cbKingWinner, bool &bPlayerTwoPair, bool &bBankerTwoPair);
	// 添加逗号
	CString AddComma( LONGLONG lScore , bool bPlus = false);
	// 添加逗号
	CString AddCommaDOUBLE( DOUBLE lScore , bool bPlus = false);
	// 删除逗号
	LONGLONG DeleteComma( CString strScore );

	//设置当前字体
	void SetCurrentFont( CDFontEx* pFontCurrent ) { m_FontCurrent = pFontCurrent; }

	//绘画扩充函数
private:
	//绘画头像
	bool DrawUserFace(CDC * pDC, INT nXPos, INT nYPos,INT nWidth,INT nHeight, IClientUserItem * pIClientUserItem);
	//艺术字体
	void DrawTextStringE(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//绘画字符
	void DrawTextStringE(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	// 绘画时钟
	void DrawTime(CDC * pDC, WORD wUserTime,INT nXPos, INT nYPos);

	//按钮消息
protected:
	afx_msg void OnLastMusic();
	afx_msg void OnNextMusic();

	//筹码按钮
	afx_msg void OnBetButton1();
	//筹码按钮
	afx_msg void OnBetButton3();
	//筹码按钮
	afx_msg void OnBetButton5();
	//筹码按钮
	afx_msg void OnBetButton10();
	//筹码按钮
	afx_msg void OnBetButton50();
	//筹码按钮
	afx_msg void OnBetButton100();
	//筹码按钮
	afx_msg void OnBetButton1000();
	//查看路子
	afx_msg void OnViewChart();
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
	//移动按钮
	afx_msg void OnScoreMoveL();
	//移动按钮
	afx_msg void OnScoreMoveR();
	//上翻页
	afx_msg void OnValleysUp();
	//下翻页
	afx_msg void OnValleysDown();
	//管理员控制
	afx_msg void OpenAdminWnd();
	//确认下注
	//afx_msg void OnComfirmBet();
	//取消下注
	afx_msg void OnCancelBet();
	//消息映射
protected:
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//鼠标消息
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
