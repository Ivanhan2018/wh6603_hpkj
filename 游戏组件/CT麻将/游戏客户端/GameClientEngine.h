#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "DirectSound.h"
#include "UserActionItem.h"

//////////////////////////////////////////////////////////////////////////

typedef CWHArray<tagUserAction *> CUserActionArray;				//操作项目

//游戏对话框
class CGameClientEngine : public CGameFrameEngine
{
	//用户变量
protected:
	WORD							m_wBankerUser;						//庄家用户
	WORD							m_wCurrentUser;						//当前用户
	WORD							m_wReplaceUser;						//补花用户
	BYTE							m_cbActionMask;						//玩家动作
	BYTE							m_cbActionCard;						//动作牌
	BYTE							m_cbListenStatus;					//听牌状态,0指未听牌,1指只胡牌,2指只自摸,3指胡牌和自摸
	bool							m_bWillHearStatus;					//即将听牌
	LONG							m_lSiceCount;						//色子数值
	
	//辅助变量
	TCHAR							m_szAccounts[GAME_PLAYER][LEN_ACCOUNTS];	//玩家名称

	//托管变量
protected:
	bool							m_bStustee;							//托管标志
	WORD							m_wTimeOutCount;					//超时次数

	//动作变量
protected:
	CUserActionArray				m_UserActionArray;					//操作项目
	CUserActionArray				m_UserActionStorage;				//操作项目

	//扑克变量
protected:
	BYTE							m_cbLeftCardCount;					//剩余数目
	BYTE							m_cbCardIndex[MAX_INDEX];			//手中扑克
	WORD							m_wHeapHead;						//牌堆头部
	WORD							m_wHeapTail;						//牌堆尾部
	BYTE							m_cbHeapCardInfo[GAME_PLAYER][2];		//堆立信息

	//组合扑克
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//组合数目
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//组合扑克

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图
	CDirectSound					m_DirectSound[3];					//出牌、发牌和开始声音
	
	//函数定义
public:
	//构造函数
	CGameClientEngine();
	//析构函数
	virtual ~CGameClientEngine();

	//常规继承
private:
	//初始函数
	virtual bool OnInitGameEngine();
	//重置框架
	virtual bool OnResetGameEngine();
	//游戏设置
	////virtual void OnGameOptionSet();
	//时间消息
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID);
	virtual bool OnEventGameClockKill(WORD t);
	//旁观状态
	virtual bool OnEventLookonMode(void * pBuffer, WORD wDataSize);
	//网络消息
	virtual bool OnEventGameMessage(WORD wSubCmdID,void * pBuffer, WORD wDataSize);
	//游戏场景
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pBuffer, WORD wDataSize);

	//消息处理
protected:
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户出牌
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//发牌消息
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//操作提示
	bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//操作结果
	bool OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//用户托管
	bool OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//用户听牌
	bool OnSubListen(const void * pBuffer,WORD wDataSize);
	//补牌消息
	bool OnSubReplaceCard(const void * pBuffer, WORD wDataSize);

	//辅助函数
protected:
	//播放声音
	void PlayCardSound(WORD wChairID,BYTE cbCardData);
	//播放声音
	void PlayActionSound(WORD wChairID,BYTE cbAction);
	//出牌判断
	bool VerdictOutCard(BYTE cbCardData);
	//设置扑克
	void SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard);
	//获取操作信息
	BYTE GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] );

	//动作操作函数
protected:
	//开始玩家动作
	bool BeginUserAction();
	//停止玩家动作
	bool StopUserAction();
	//移除当前动作
	bool RemoveCurrentAction();
	//激活动作
	tagUserAction *ActiveUserAction(enmActionKind enActionKind);
	//发牌动作
	bool BeginActionStartCard();
	//出牌动作
	bool BeginActionOutCard();
	//发牌动作
	bool BeginActionSendCard();
	//补花动作
	bool BeginActionReplaceCard();
	//提示操作
	bool BeginActionActionNotify();
	//操作结果
	bool BeginActionActionResult();
	//听牌操作
	bool BeginActionListenCard();
	//掷色子
	bool BeginActionSice();
	//游戏结束
	bool BeginActionGameEnd();
	//发牌完成
	bool OnActionStartCard();
	//出牌完成
	bool OnActionOutCard();
	//抓牌完成
	bool OnActionSendCard();
	//补花完成
	bool OnActionReplaceCard();
	//提示完成
	bool OnActionActionNotify();
	//操作完成
	bool OnActionActionResult();
	//听牌操作
	bool OnActionListenCard();
	//掷色子完成
	bool OnActionSice();
	//游戏结束
	bool OnActionGameEnd();
	//补花事件
	bool OnTimerReplaceCard();

	//消息映射
protected:
	//时间消息
	VOID OnTimer(UINT nIDEvent);
	//开始消息
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//出牌操作
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//扑克操作
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);
	//拖管控制
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	//第二次摇色子消息
	LRESULT OnSiceTwo(WPARAM wParam, LPARAM lParam);
	//玩家操作
	LRESULT OnUserAction( WPARAM wParam, LPARAM lParam );
	//操作完成
	LRESULT OnUserActionFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
