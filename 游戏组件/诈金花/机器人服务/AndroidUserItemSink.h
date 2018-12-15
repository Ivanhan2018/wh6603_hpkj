#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//游戏变量
protected:
	WORD							m_wCurrentUser;							 //当前用户
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克
	bool							m_bFirstAdd;							 //开始第一个操作，不可以比牌
	//用户状态
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//游戏状态
	LONGLONG						m_lTableScore[GAME_PLAYER];				//下注数目

	//下注信息
protected:
	LONGLONG						m_lMaxScore;							//封顶数目
	LONGLONG						m_lMaxCellScore;						//单元上限
	LONGLONG						m_lUserAddScore;						//用户加注(总)数目
	WORD							m_bCompareState;						//比牌状态
	LONGLONG						m_lCellScore;							//单元下注
	LONGLONG						m_lCurrentTimes;						//当前倍数
	LONGLONG						m_lUserMaxScore;						//用户最大分数


	//梦成网络
	bool							m_bMingZhu[GAME_PLAYER];				//看牌动作
	bool							m_bBiggest;								//自已最大
	BYTE                            m_cbCardV;								//当前牌的类型
	bool							m_bIsYz;								//牌是否单张或者特种牌形
	int								m_bMinZhuIndex;							//存款倍数 //看牌机率
	//控件变量
public:
	CGameLogic						m_GameLogic;							//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;					//用户接口
	

	//////////////////////////////机器人输分限制
	//银行操作
	LONGLONG						m_lRobotBankGetScore;					//取款最小数额(取款数是该值以上的一个随机数)
	LONGLONG						m_lRobotBankGetScoreBanker;				//取款最大数额(此数值一定要大于RobotBankGet)
	int								m_nRobotBankStorageMul;					//存款倍数
	LONGLONG						m_lBankerLostScore;						//庄家输分 (当机器人为庄)
	LONGLONG						m_lRobotScoreRange[2];					//最大范围
	LONGLONG						m_MinLostScore;							//输分下限 低于这个值,看牌机率比较少
	LONGLONG						m_MaxLostScore;							//输分上限 超过这个值,看牌机率非常大
	int								m_MinLostIndex;							//看牌机率比较少
	int								m_MaxLostIndex;							//看牌机率非常大
	TCHAR							m_szRoomName[32];						//配置房间
	/////////////////////////////机器人输分限制
	
	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { delete this; }
	//接口查询
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool  RepositionSink();

	//游戏事件
public:
	//时间消息
	virtual bool  OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool  OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏消息
	virtual bool  OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//场景消息
	virtual bool  OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//用户事件
public:
	//用户进入
	virtual void  OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual void  OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual void  OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual void  OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);


	//消息处理
protected:
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户放弃
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//用户看牌
	bool OnSubLookCard(const void * pBuffer, WORD wDataSize);
	//用户比牌
	bool OnSubCompareCard(const void * pBuffer, WORD wDataSize);
	//用户开牌
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//用户强退
	bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//全部操作
	bool Alloperations();
	//比牌
	bool OnAllCompare();
	//概率
	bool GaiLv(BYTE bNum);
	
	bool IsCanYa();
	//删除所有时钟
	void KillAllTimer();
	//读取配置
	void ReadConfigInformation();

};

//////////////////////////////////////////////////////////////////////////

#endif
