#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "HistoryScore.h"

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
		//游戏记录
protected:
	CString							m_GameRecord;
	TCHAR							m_szNickName[GAME_PLAYER][LEN_NICKNAME];
	//游戏变量
protected:
	WORD							m_wBankerUser;							//庄家用户
	WORD							m_wFisrtCallUser;						//始叫用户
	WORD							m_wCurrentUser;							//当前用户
	SCORE							m_lExitScore;							//强退分数 

	//用户数据
protected:
	BYTE                            m_cbDynamicJoin[GAME_PLAYER];           //动态加入
	BYTE							m_cbPlayStatus[GAME_PLAYER];				//游戏状态
	BYTE							m_bCallStatus[GAME_PLAYER];				//叫庄状态
	BYTE							m_bOxCard[GAME_PLAYER];					//牛牛数据
	SCORE							m_lTableScore[GAME_PLAYER];				//下注数目

	//扑克变量
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克

	//下注信息
protected:
	SCORE							m_lTurnMaxScore[GAME_PLAYER];			//最大下注


//////////////////////////////梦成网络添加
	//AI变量
protected:
	
	bool							m_bRefreshCfg;							//每盘刷新
	bool							m_bRefreshStorageCfg;					//是否刷新库存
	//////全局静态变量
	static SCORE					m_lStockScore;							//总输赢分
	static LONGLONG					m_lStorageOff;							//库存减少值
	static BYTE						StorageStyle;							//当前系统吃分模式,1,正常,2杀分,3送分	
	LONGLONG						m_lStorageDeduct;						//回扣变量 千分比
	SCORE							m_lStockLimit;							//总输赢分
	static SCORE							m_lAndroidWinAndLose;					//机器人输赢总值	
	//梦成网络添加
	LONGLONG						m_lStorageMin;							//库存最小值
	LONGLONG						m_lStorageMax;							//库存最大值
	BYTE							StorageRatio;							//库存初值输赢比例百分之N		
	BYTE							StorageMinRatio;						//库存最小值输赢比例百分之N		杀分模式
	BYTE							StorageMaxRatio;						//库存最大值输赢比例百分之N		送分模式
	LONGLONG						StorageEating;							//吃分百分比,系统到达最大值后一次性吃掉这个值
	
				
	TCHAR							m_szFileName[MAX_PATH];					//配置路经
	//梦成网络添加
protected:
	//控制玩家输赢
	bool							m_bWinnerCtrl;							//是否控制玩家赢
	DWORD							m_dwCheatGameID;						//作弊帐号
	DWORD							m_dwCheatCount;							//作弊次数
	BYTE							m_cbCheatType;							//作弊类型
	DWORD							m_dwCheatGameID1;						//管理帐号
	//////玩家输赢比率
	bool							m_bAndroidWinningCtrl;					//是否控制机器人输赢比率
	LONGLONG				        m_AndroidWinningRatio;					//机器人赢的比率

//////////////////////////////梦成网络添加
	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数
	CHistoryScore					m_HistoryScore;							//历史成绩

	//属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	
	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}

	//游戏事件
public:
	//游戏开始
	virtual bool  OnEventGameStart();
	//游戏结束
	virtual bool  OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool  OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool  OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool  OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//数据事件
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}

	//游戏事件
protected:
	//叫庄事件
	bool OnUserCallBanker(WORD wChairID, BYTE bBanker);
	//加注事件
	bool OnUserAddScore(WORD wChairID, SCORE);
	//摊牌事件
	bool OnUserOpenCard(WORD wChairID, BYTE bOx);
protected:
	//牌转文字
	CString TransformCardInfo( BYTE cbCardData );
	//功能函数
private:
	//////输赢比率实现函数
	bool AndroidWinningControl();
	//机器人库存控制
	bool AndroidCheck();
	//控制指定玩家
	bool ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType);
	//管理员命令
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer);
	//获取椅子号
	WORD GetChairIdByGameId(DWORD dwGameID);
	//读取配置
	void ReadConfigInformation(bool bReadFresh);
};

//////////////////////////////////////////////////////////////////////////

#endif
