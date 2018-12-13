#ifndef USER_ACTION_ITEM_HEAD_FILE
#define USER_ACTION_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//移动类型
enum enmActionKind
{
	AK_NULL					= 0,							//
	AK_Sice,												//掷色子
	AK_StartCard,											//开局
	AK_OutCard,												//出牌
	AK_SendCard,											//发牌
	AK_ActionNotify,										//提示操作
	AK_ActionResult,										//操作结果
	AK_ReplaceCard,											//补牌
	AK_ListenCard,											//听牌操作
	AK_GameEnd												//结束
};

//////////////////////////////////////////////////////////////////////////

//玩家动作
struct tagUserAction
{
	enmActionKind					enActionKind;						//动作类型
	BOOL							bHandleStatus;						//处理状态

	//构造函数
	tagUserAction()
	{
		enActionKind = AK_NULL;
		ResetAction();
	}
	//析构函数
	virtual ~tagUserAction() {}
	//重置函数
	virtual VOID ResetAction()
	{
		bHandleStatus = FALSE;
	}
};

//掷色子
struct tagActionSice: public tagUserAction
{
	LONG							lSiceCount;							//色子数

	//构造函数
	tagActionSice()
	{
		enActionKind = AK_Sice;
		ResetAction();
	}
	//重置函数
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		lSiceCount = 0;
	}
};

//开局发牌
struct tagActionStartCard: public tagUserAction
{
	WORD							wChairId;							//发牌玩家
	BYTE							cbCardCount;						//发牌数
	BYTE							cbCardData[4];						//发牌值
	WORD							wHeapId;							//发牌位置
	WORD							wHeapCardIndex;						//堆牌位置
	bool							bLastItem;							//是否最后一张牌

	//构造函数
	tagActionStartCard()
	{
		enActionKind = AK_StartCard;
		ResetAction();
	}
	//重置函数
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		wChairId = INVALID_CHAIR;
		cbCardCount = 0;
		ZeroMemory( cbCardData,sizeof(cbCardData) );
		wHeapId = INVALID_CHAIR;
		wHeapCardIndex = 0;
		bLastItem = false;
	}
};

//出牌
struct tagActionOutCard: public tagUserAction
{
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克

	//构造函数
	tagActionOutCard()
	{
		enActionKind = AK_OutCard;
		ResetAction();
	}
	//重置函数
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		wOutCardUser = INVALID_CHAIR;
		cbOutCardData = 0;
	}
};

//发牌
struct tagActionSendCard: public tagUserAction
{
	BYTE							cbCardData;							//扑克数据
	BYTE							cbActionMask;						//动作掩码
	WORD							wCurrentUser;						//当前用户
	WORD							wSendCardUser;						//发牌用户
	bool							bTail;								//是否从尾部发牌

	//构造函数
	tagActionSendCard()
	{
		enActionKind = AK_SendCard;
		ResetAction();
	}
	//重置函数
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		cbCardData = 0;
		cbActionMask = WIK_NULL;
		wCurrentUser = INVALID_CHAIR;
		wSendCardUser = INVALID_CHAIR;
		bTail = false;
	}
};

//提示操作
struct tagActionActionNotify: public tagUserAction
{
	BYTE							cbActionCard;						//操作牌
	BYTE							cbActionMask;						//操作码

	//构造函数
	tagActionActionNotify()
	{
		enActionKind = AK_ActionNotify;
		ResetAction();
	}
	//重置函数
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		cbActionCard = 0;
		cbActionMask = WIK_NULL;
	}
};

//操作结果
struct tagActionActionResult: public tagUserAction
{
	WORD							wOperateUser;						//操作用户
	WORD							wProvideUser;						//供应用户
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard[3];					//操作扑克

	//构造函数
	tagActionActionResult()
	{
		enActionKind = AK_ActionResult;
		ResetAction();
	}
	//重置函数
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		wOperateUser = INVALID_CHAIR;
		wProvideUser = INVALID_CHAIR;
		cbOperateCode = WIK_NULL;
		ZeroMemory( cbOperateCard,sizeof(cbOperateCard) );
	}
};

//补花
struct tagActionReplaceCard: public tagUserAction
{
	BYTE							cbReplaceCard;						//扑克数据
	WORD							wReplaceUser;						//补花玩家

	//构造函数
	tagActionReplaceCard()
	{
		enActionKind = AK_ReplaceCard;
		ResetUserAction();
	}

	//重置动作
	VOID ResetUserAction()
	{
		bHandleStatus = FALSE;
		cbReplaceCard = 0;
		wReplaceUser = INVALID_CHAIR;
	}
};

//听牌操作
struct tagActionListenCard: public tagUserAction
{
	WORD							wChairId;							//听牌用户

	//构造函数
	tagActionListenCard()
	{
		enActionKind = AK_ListenCard;
		ResetUserAction();
	}

	//重置动作
	VOID ResetUserAction()
	{
		bHandleStatus = FALSE;
		wChairId = INVALID_CHAIR;
	}
};

//游戏结束
struct tagActionGameEnd: public tagUserAction
{
	LONG							lGameTax;							//游戏税收
	WORD							wProvideUser;						//供应用户
	BYTE							cbProvideCard;						//供应扑克
	DWORD							dwChiHuKind[GAME_PLAYER];			//胡牌类型
	DWORD							dwChiHuRight[MAX_RIGHT_COUNT];		//胡牌类型
	BYTE							cbHuaCardCount;						//花牌个数
	BYTE							cbFanCount;							//总番数

	//积分信息
	LONGLONG						lGameScore[GAME_PLAYER];			//游戏积分

	//扑克信息
	BYTE							cbCardCount[GAME_PLAYER];			//扑克数目
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//扑克数据


	//构造函数
	tagActionGameEnd()
	{
		enActionKind = AK_GameEnd;
		ResetAction();
	}
	//重置函数
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		lGameTax = 0;
		wProvideUser = INVALID_CHAIR;
		cbProvideCard = 0;
		ZeroMemory( dwChiHuKind,sizeof(dwChiHuKind) );
		ZeroMemory( dwChiHuRight,sizeof(dwChiHuRight) );
		cbHuaCardCount = 0;
		cbFanCount = 0;
		ZeroMemory( lGameScore,sizeof(lGameScore) );
		ZeroMemory( cbCardCount,sizeof(cbCardCount) );
		ZeroMemory( cbCardData,sizeof(cbCardData) );
	}
};

//////////////////////////////////////////////////////////////////////////

#endif