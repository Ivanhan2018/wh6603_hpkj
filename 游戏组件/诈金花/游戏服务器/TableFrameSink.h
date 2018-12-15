#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "DlgCustomRule.h"
//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink :public ITableFrameSink, public ITableUserAction
{
			
	//游戏记录
protected:
	CString							m_GameRecord;
	TCHAR							m_szNickName[GAME_PLAYER][LEN_NICKNAME];
	LONGLONG						m_lAddScoreTimes[GAME_PLAYER];
	WORD							m_wTableID;

	//游戏变量
protected:
	BYTE							m_bOperaCount;							//操作次数
	WORD							m_wCurrentUser;							//当前用户
	WORD							m_wBankerUser;							//庄家用户
	WORD							m_wFlashUser[GAME_PLAYER];				//动画用户
	SCORE							m_lCellScore;							//单元下注

	//用户状态
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//游戏状态
	LONGLONG						m_lCompareCount;						//比牌状态
	bool							m_bGameEnd;								//结束状态
	CMD_S_GameEnd					m_StGameEnd;							//结束数据

	//扑克变量
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克
	CWHArray<WORD>					m_wCompardUser[GAME_PLAYER];			 //扑克数组

	//下注信息
protected:
	LONGLONG						m_lCellScore_Long;						//单元下注
	LONGLONG						m_lTableScore[GAME_PLAYER];				//下注数目
	LONGLONG						m_lUserMaxScore[GAME_PLAYER];			//最大下注
	LONGLONG						m_lMaxCellScore;						//单元上限
	LONGLONG						m_lCurrentTimes;						//当前倍数
	bool							m_bMingZhu[GAME_PLAYER];				//看明下注

	//AI变量
protected:
	//梦成网络添加
	///////
	bool							m_bRefreshCfg;							//每盘刷新
	bool							m_bRefreshStorageCfg;					//是否刷新库存
	//////

	static SCORE					m_lStockScore;							//总输赢分
	static LONGLONG					m_lStorageOff;							//库存减少值
	static BYTE						StorageStyle;							//当前系统吃分模式,1,正常,2杀分,3送分	
	LONGLONG						m_lStorageDeduct;						//回扣变量 千分比
	SCORE							m_lStockLimit;							//总输赢分


	LONGLONG						m_lStorageMin;							//库存最小值
	LONGLONG						m_lStorageMax;							//库存最大值
	BYTE							StorageRatio;							//库存初值输赢比例百分之N		
	BYTE							StorageMinRatio;						//库存最小值输赢比例百分之N		杀分模式
	BYTE							StorageMaxRatio;						//库存最大值输赢比例百分之N		送分模式
	LONGLONG						StorageEating;							//吃分百分比,系统到达最大值后一次性吃掉这个值
	
	
	
	TCHAR							m_szFileName[MAX_PATH];					//配置文件路经
protected:
	//控制玩家输赢
	bool							m_bWinnerCtrl;							//是否控制玩家赢
	BYTE							m_cbCheatType;							//作弊类型
	DWORD							m_dwCheatCount;							//作弊次数
	DWORD							m_dwCheatGameID;						//作弊帐号
	DWORD							m_dwCheatGameID1;						//管理帐号

	//////玩家输赢比率
	bool							m_bAndroidWinningCtrl;					//是否控制机器人输赢比率
	LONGLONG				        m_AndroidWinningRatio;					//机器人赢的比率
//梦成网络添加
	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	tagGameServiceOption			* m_pGameServiceOption;					//配置参数
	tagCustomRule					* m_pGameCustomRule;					//自定规则
	//属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数
	static const BYTE				m_GameStartMode;						//开始模式

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() {delete this; }
	//接口查询
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void  RepositionSink();
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
	//信息接口
public:
	//开始模式
	virtual BYTE  GetGameStartMode();
	//游戏状态
	virtual bool  IsUserPlaying(WORD wChairID);

	//游戏事件
public:
	//游戏开始
	virtual bool  OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool  OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool  OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool  OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);


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

	//游戏事件
protected:
	//放弃事件
	bool OnUserGiveUp(WORD wChairID,bool bExit=false);
	//看牌事件
	bool OnUserLookCard(WORD wChairID);
	//比牌事件
	bool OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID);
	//开牌事件
	bool OnUserOpenCard(WORD wUserID,bool bSystemOpen=false);
	//加注事件
	bool OnUserAddScore(WORD wChairID, LONGLONG lScore, bool bGiveUp, bool bCompareUser);
protected:
	//牌转文字
	CString TransformCardInfo( BYTE cbCardData );
protected:
	//发送管理员,机器人扑克数据
	void AndroidSendUserCard(); 
private:
	//读取配置
	void ReadConfigInformation(bool bReadFresh);
	//////输赢比率实现函数
	bool AndroidWinningControl();
	//根据用户Id获取其所坐下的椅子
	WORD GetChairIdByGameId(DWORD dwGameId);
	//控制指定玩家
	void ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType);
		//机器人作弊
	bool AndroidCheck();
	
		//管理员命令
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer);

};

//////////////////////////////////////////////////////////////////////////

#endif
