#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "DirectSound.h"


//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameEngine
{
	//zz
public:
	BYTE							m_byCanSelectCard[4];				//能选择的牌数据
	BYTE							m_byCanSelectCount;					//能选择的牌数目
	BYTE							m_byCanSelectType;					//类型，杠，碰

	//用户变量
protected:
	WORD							m_wBankerUser;						///庄家用户
	WORD							m_wCurrentUser;						///当前用户
	WORD							m_wReplaceUser;						//补花用户
	BYTE							m_cbActionMask;						//玩家动作
	BYTE							m_cbActionCard;						//动作牌
	BYTE							m_cbListenStatus;					//听牌状态,0指未听牌,1指只胡牌,2指只自摸,3指胡牌和自摸
	bool							m_bWillHearStatus;					//即将听牌
	LONG							m_lSiceCount;						//色子数值
	bool							m_bMustWanTong;						//是否首次万同

	//zz状态变量
	//bool							m_bHearStatus;						//听牌状态
	bool                            m_bZM;                              //是否自摸 
	bool                            m_bFP;                              //是否吃炮
	bool							m_bIsMeCanOutCard;					//是否自己可以出牌
	//zz动作信息
	BYTE							m_byUserAction[4];					//用户动作
	//zz声音信息
	BYTE							m_cbGender;					         //用户性别
	CString                         m_InfoSound;                         //声音信息
	//zz堆立变量
	WORD							m_wHeapHead;						//堆立头部
	WORD							m_wHeapTail;						//堆立尾部
	BYTE							m_cbHeapCardInfo[4][2];				//堆牌信息
	//zz出牌信息
	WORD							m_wOutCardUser;						//出牌用户
	BYTE							m_cbOutCardData;					//出牌扑克
	BYTE							m_cbDiscardCount[GAME_PLAYER];		//丢弃数目
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];	//丢弃记录
	
	//辅助变量
	TCHAR							m_szAccounts[GAME_PLAYER][LEN_ACCOUNTS];	//玩家名称
	CMD_S_GameStart					m_GameStart;						//开始消息
	CMD_S_SendCard					m_SendCard;							//发牌消息

	//托管变量
protected:
	bool							m_bStustee;							///托管标志
	WORD							m_wTimeOutCount;					///超时次数

	//扑克变量
protected:
	BYTE							m_cbLeftCardCount;					///剩余数目
	BYTE							m_cbCardIndex[MAX_INDEX];			///手中扑克

	BYTE							m_cbEnjoinCardCount;				//禁止出牌
	BYTE							m_cbEnjoinCardData[GAME_PLAYER];	//禁止出牌

	//组合扑克
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//组合数目
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//组合扑克

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图
	CDirectSound					m_DirectSound[3];					//出牌和发牌声音

	//函数定义
public:
	//构造函数
	CGameClientDlg();
	//析构函数
	virtual ~CGameClientDlg();

	//常规继承
private:
	//初始函数
	virtual bool OnInitGameEngine();
	//重置框架
	virtual bool OnResetGameEngine();
	//游戏设置
	virtual void OnGameOptionSet();
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
	//是否万同
	bool OnSubWanTong(const void * pBuffer, WORD wDataSize);
//	//消息处理
//protected:
	bool OnSubStartSZFlash();
	////游戏开始
	//bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	////用户出牌
	//bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	////发牌消息
	//bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	////操作提示
	//bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	////操作结果
	//bool OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	////海底操作
	//bool OnSubOperateHaiDi(const void * pBuffer, WORD wDataSize);
	////游戏结束
	//bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	////用户托管
	//bool OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//听牌
	//bool OnSubTingCard(const void * pBuffer, WORD wDataSize);
	//听牌结果
	//bool OnSubTingCardResult(const void * pBuffer, WORD wDataSize);

	//辅助函数
protected:
	//播放声音
	void PlaySound(CString szSoundRes, bool bLoop);
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
	//清空所有定时器
	void ClearAllTimer();
	//发送命令
	void SendCardCommand(BYTE byOperatorCode, BYTE byOperatorCard);

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
	//摇色子结束消息
	LRESULT OnSiceFinish(WPARAM wParam, LPARAM lParam);
	//出牌完成消息
	LRESULT OnOutCardFinish( WPARAM wParam, LPARAM lParam );
	//发牌完成消息
	LRESULT OnSendCardFinish( WPARAM wParam, LPARAM lParam );
	//玩家操作
	LRESULT OnUserAction( WPARAM wParam, LPARAM lParam );
	//玩家操作
	LRESULT OnUserMove( WPARAM wParam, LPARAM lParam );
	//声音提示
	LRESULT OnSound(WPARAM wParam,LPARAM lParam);
	//消息映射
protected:
	//时间消息
	//afx_msg void OnTimer(UINT nIDEvent);
	//启动筛子按钮
	LRESULT OnSZStart(WPARAM wParam, LPARAM lParam);
	//开始消息
	//LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//摸牌消息
	LRESULT OnMoPai(WPARAM wParam, LPARAM lParam);
	//出牌操作
	//LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//扑克操作
	//LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);
	////海底操作
	//LRESULT OnOperateHaiDi(WPARAM wParam, LPARAM lParam);
	//拖管控制
	//LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	//骰子消息
	LRESULT OnShowSZ(WPARAM wParam, LPARAM lParam);
	//听牌消息
	LRESULT OnTingCard(WPARAM wParam, LPARAM lParam);
	//色子动画按钮
	LRESULT OnSZFlashBT(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
