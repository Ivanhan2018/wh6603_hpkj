#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "SendCard.h"
//#include "..\..\开发库\Include\cnd_ILogMgr.h"
//////////////////////////////////////////////////////////////////////////
//枚举定义

//效验类型
enum enEstimatKind
{
	EstimatKind_OutCard,			//出牌效验
	EstimatKind_GangCard,			//杠牌效验
};

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//游戏变量
protected:
	WORD							m_wSiceCount;							//骰子点数
	WORD							m_wBankerUser;							//庄家用户
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//用户扑克
	bool							m_bTrustee[GAME_PLAYER];				//是否托管
	//WORD                            m_wMPIndex;                             //摸牌索引
	

	//出牌信息
protected:
	WORD							m_wOutCardUser;							//出牌用户
	BYTE							m_cbOutCardData;						//出牌扑克
	BYTE							m_cbOutCardCount;						//出牌数目
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//丢弃数目
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];		//丢弃记录

	//发牌信息
protected:
	BYTE							m_cbSendCardData;						//发牌扑克
	BYTE							m_cbSendCardCount;						//发牌数目
	BYTE							m_cbLeftCardCount;						//剩余数目
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//库存扑克
	BYTE							m_cbKingCard[4];						//王牌
	BYTE							m_cbKingCount[GAME_PLAYER];				//王牌个数
	//BYTE                            m_cbMoCard;                             //摸的牌


	//
protected:
	//tagGSKUResult					m_tagUserGSKUResult[GAME_PLAYER];

	//运行变量
protected:
	WORD							m_wResumeUser;							//还原用户
	WORD							m_wCurrentUser;							//当前用户
	WORD							m_wProvideUser;							//供应用户
	BYTE							m_cbProvideCard;						//供应扑克
	LONGLONG					m_llMuscle[GAME_PLAYER];					// 筋数目					

	//状态变量
protected:
	bool							m_bSendStatus;							//发牌状态
	bool							m_bGangStatus;							//抢杆状态
	//bool							m_bEnjoinChiHu[GAME_PLAYER];			//禁止吃胡
	//bool							m_bEnjoinChiPeng[GAME_PLAYER];			//禁止吃碰

	//用户状态
public:
	bool							m_bResponse[GAME_PLAYER];				//响应标志
	WORD							m_cbUserAction[GAME_PLAYER];			//用户动作
	BYTE							m_cbOperateCard[GAME_PLAYER];			//操作扑克
	WORD							m_cbPerformAction[GAME_PLAYER];			//执行动作

	//组合扑克
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];		//组合数目
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][4];		//组合扑克
	BYTE							m_byGangCard[GAME_PLAYER][4];			//杠牌
	BYTE							m_byGangCardNums[GAME_PLAYER];			//个数
	BYTE							m_byBuCard[GAME_PLAYER][4];				//补牌
	BYTE							m_byBuCardNums[GAME_PLAYER];			//个数



//2011-8-8	osc update
private:
	//bool							m_bUserFirstIsTingPai[GAME_PLAYER];		//是否第一次听牌
	//bool							m_bHearStatus[GAME_PLAYER];				//听牌状态
	//bool							m_bIsUserFirstOutCard[GAME_PLAYER];		//是否第一次出牌
	WORD							m_byUserHuCardType[GAME_PLAYER];		//玩家胡牌类型
	//int								m_iSaveGameOutCardCount;				//保存游戏出牌数(用于杠上炮)
	//int								m_iSaveGameSendCardCount;				//保存游戏发牌数(用于杠上炮)	
	BYTE							m_byGameEndType;						//游戏结束类型, 0:荒庄, 1:自摸; 2:放炮; 3:海底自摸; 4:海底放炮; 5（海底）荒庄

	bool                            m_bIsPlayDice;                         //是否打骰子


	//结束信息
protected:
	WORD							m_wHaiDiUser;							//海底用户
	BYTE							m_cbChiHuCard;							//吃胡扑克
	tagChiHuResult					m_ChiHuResult[GAME_PLAYER];				//吃胡结果
	CMD_S_OperateResult		m_OperateResult;						///< 明筋可胡，数据暂存

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数

private:
	CSendCard						m_SendCardTool;							//v

	//属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数
	//static const enStartMode		m_GameStartMode;						//开始模式
private:
	//cnd_ILogMgr*	m_pRoomLog;	// 日志组件的指针
	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual void  Release() { if (IsValid()) delete this; return ; }
	//是否有效
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	// 通向游戏服务端的通道，可以传递任何简易数据或成员指针
	virtual bool channel_to_game_service(int iMainCode, int iAssistantCode, void* pData, int iDataLen);

	//初始化
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void  RepositionSink();

	//信息接口
public:
	////开始模式
	//virtual enStartMode  GetGameStartMode();
	//游戏状态
	virtual bool  IsUserPlaying(WORD wChairID);

	//游戏事件
public:
	//游戏开始
	virtual bool  OnEventGameStart();
	//游戏结束
	virtual bool  OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool  OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool  OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool  OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//动作事件
public:
	//用户同意
	virtual bool  OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	//用户断线
	virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool  OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);


public:
	//初始化数据
	void InitData();
	// 开门消息
	bool OnUserOpenDoor();

	//游戏事件
protected:
	//用户出牌
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//用户操作
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);
	/// 当前玩家操作
	bool OnCurUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);
	/// 其他玩家操作
	bool OnOtherUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);
	////海底操作
	//bool OnUserOperateHaiDi(WORD wChairID, BYTE cbTakeHaiDi);
	//用户听牌操作
	bool OnUserTingCard(WORD wChairID, bool bIsTing=false);

	//辅助函数
protected:
	//发送操作
	bool SendOperateNotify();
	//派发扑克
	/// [@param bSupplyCard in] 真补牌，假正常发牌
	bool DispatchCardData(WORD wCurrentUser, bool bSupplyCard);
	//响应判断
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
	//bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
	//用户操作
	void UserStartOperation();
	/// 写筋分
	/// [@param wWinChairID in] 赢 玩家ID
	/// [@param wLostChairID in] 输 钱玩家ID
	/// [@param iMuscle in] 番数
	/// [@param llResultGold out] 每个玩家的变动金币
	bool OnWriteMuscleScore(WORD wWinChairID, WORD wLostChairID, int iMuscle, LONGLONG llResultGold[]);
	bool OnWriteMuscleScore(WORD wWinChairID, int iMuscle, LONGLONG llResultGold[]);


//	//任务判断  CP add 4.12
//protected:
//	void OnUserTaskJudgement(WORD wChairID, BYTE btTag);
	/// 测试结算
	void TestGameEndFun(BYTE bSelect);
	///////////////////////////
public:
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	virtual void SetGameBaseScore(LONG lBaseScore);
	virtual bool OnActionUserOnReady(WORD,IServerUserItem *,void *,WORD);
};



//////////////////////////////////////////////////////////////////////////

#endif