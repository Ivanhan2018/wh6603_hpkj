#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#define GAME_PLAYER					4									//游戏人数
#ifdef USE_GB
#define KIND_ID						512									//游戏 I D
#define GAME_NAME					TEXT("大众麻将")					//游戏名字
#else
#define KIND_ID						303									//游戏 I D
#define GAME_NAME					TEXT("转转麻将")					//游戏名字
#endif
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//游戏类型
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本///////////
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本//////////
//#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH)	//游戏类型

//游戏状态
#define GS_MJ_FREE					GAME_STATUS_FREE					//空闲状态
#define GS_MJ_PLAY					(GAME_STATUS_PLAY+1)				//游戏状态
//#define GS_MJ_FREE					GS_FREE								//空闲状态
//#define GS_MJ_PLAY					(GS_PLAYING+1)						//游戏状态
#define GS_MJ_HAI_DI				(GAME_STATUS_PLAY+2)						//海底状态
//#define GS_MJ_BEFOREPLAY			(GS_PLAYING+3)						//游戏前状态

//////////////////////////////////////////////////////////////////////////

//组合子项
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户

	CMD_WeaveItem()
	{
		ZeroMemory(this, sizeof(*this));
		wProvideUser = -1;
	}
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_OUT_CARD				101									//出牌命令
#define SUB_S_SEND_CARD				102									//发送扑克
#define SUB_S_OPERATE_NOTIFY		103									//操作提示
#define SUB_S_OPERATE_RESULT		104									//操作命令
#define SUB_S_OPERATE_HAI_DI		105									//海底操作
#define SUB_S_GAME_END				106									//游戏结束
#define SUB_S_TRUSTEE				107									//用户托管
#define SUB_S_TING_CARD             108                                 //听牌命令
#define SUB_S_USER_TINGCARD_RESULT	109									//听牌的结果

//游戏状态
struct CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//基础金币
	WORD							wBankerUser;						//庄家用户
	bool							bTrustee[GAME_PLAYER];				//是否托管

	CMD_S_StatusFree()
	{
		ZeroMemory(this, sizeof(*this));
		wBankerUser = -1;
	}
};

//游戏状态
struct CMD_S_StatusPlay
{
	//游戏变量
	LONGLONG						lCellScore;							//单元积分
	WORD							wSiceCount;							//骰子点数
	WORD							wBankerUser;						//庄家用户
	WORD							wCurrentUser;						//当前用户
	bool							bTrustee[GAME_PLAYER];				//是否托管

	//状态变量
	BYTE							cbHearStatus;						//听牌状态
	BYTE							cbActionCard;						//动作扑克
	BYTE							cbActionMask;						//动作掩码
	BYTE							cbLeftCardCount;					//剩余数目

	//出牌信息
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克
	BYTE							cbDiscardCount[4];					//丢弃数目
	BYTE							cbDiscardCard[4][55];				//丢弃记录

	//扑克数据
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[14];						//扑克列表

	//组合扑克
	BYTE							cbWeaveCount[4];					//组合数目
	CMD_WeaveItem					WeaveItemArray[4][4];				//组合扑克

	//听牌信息
	bool                            m_bTingCard;                         //能否听牌
	
	//是否已经操作
	bool							bIsAlreadyOper;						//是否已经操作过


	//杠补信息
	BYTE							byGangCard[4];						//杠牌
	BYTE							byGangCardNums;						//个数
	BYTE							byBuCard[4];						//补牌
	BYTE							byBuCardNums;						//个数


	//王牌
	BYTE							byKindCard;							//王牌

	//是否打骰子
	bool                            bIsPlayDice;

	CMD_S_StatusPlay()
	{
		ZeroMemory(this, sizeof(*this));
		wBankerUser = -1;
		wCurrentUser = -1;
		wOutCardUser = -1;
	}
};

//游戏开始
struct CMD_S_GameStart
{
	WORD							wSiceCount;							//骰子点数
	WORD							wBankerUser;						//庄家用户
	WORD							wCurrentUser;						//当前用户
	BYTE							cbUserAction;						//用户动作
	BYTE							cbCardData[14];						//扑克列表
	bool							bTrustee[GAME_PLAYER];				//是否托管
	//BYTE							cbKingCard;					        //王牌
	WORD                            wMPIndex;                           //王牌的上一张的索引

	BYTE							byGangCard[4];						//杠牌
	BYTE							byGangCardNums;						//个数
	BYTE							byBuCard[4];						//补牌
	BYTE							byBuCardNums;						//个数

	CMD_S_GameStart()
	{
		ZeroMemory(this, sizeof(*this));
		wBankerUser = -1;
		wCurrentUser = -1;
	}
};

//出牌命令
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克
	
	CMD_S_OutCard()
	{
		wOutCardUser = -1;
		cbOutCardData = -1;
	}
};

//发送扑克
struct CMD_S_SendCard
{
	bool							bSupplyCard;							//真补牌，假发牌 
	BYTE							cbCardData;							//扑克数据
	BYTE							cbActionMask;						//动作掩码
	WORD							wCurrentUser;						//当前用户

	BYTE							byGangCard[4];						//杠牌
	BYTE							byGangCardNums;						//个数
	BYTE							byBuCard[4];						//补牌
	BYTE							byBuCardNums;						//个数

	CMD_S_SendCard()
	{
		ZeroMemory(this, sizeof(*this));
		wCurrentUser = -1;
	}
};

//操作提示
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//还原用户
	BYTE							cbActionMask;						//动作掩码
	BYTE							cbActionCard;						//动作扑克

	BYTE							byGangCard[4];						//杠牌
	BYTE							byGangCardNums;						//个数
	BYTE							byBuCard[4];						//补牌
	BYTE							byBuCardNums;						//个数

	CMD_S_OperateNotify()
	{
		ZeroMemory(this, sizeof(*this));
		wResumeUser = -1;
	}
};

//操作命令
struct CMD_S_OperateResult
{
	bool							bBringMuscleCanHu;				//明筋能胡牌
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克
	BYTE								bMuscle;							// 筋方式, 0 无效值, 1 暗筋，2 明筋，3 放筋
	WORD							wOperateUser;						//操作用户
	WORD							wProvideUser;						//供应用户
	LONGLONG					llMuscleGold[GAME_PLAYER]; //金币更动值

	CMD_S_OperateResult()
	{
		bBringMuscleCanHu=false;
		wOperateUser = 0;
		wProvideUser = 0;
		cbOperateCode = 0x00;
		cbOperateCard = 0;
		bMuscle = 1;
		
		for (BYTE i=0; i<GAME_PLAYER; i++)
		{
			llMuscleGold[i] = 0l;
		}
	}
};

//海底操作
struct CMD_S_OperateHaiDi
{
	WORD							wCurrentUser;						//当前用户

	CMD_S_OperateHaiDi()
	{
		ZeroMemory(this, sizeof(*this));
		wCurrentUser = -1;
	}
};

//游戏结束
struct CMD_S_GameEnd
{
	BYTE							cbChiHuCard;						//吃胡扑克
	BYTE							cbHaiDiCard;						//海底扑克
	BYTE							cbCardCount[4];						//扑克数目
	BYTE							cbCardData[4][14];					//扑克数据	
    BYTE                            bKingCard;                          //王牌数据
	
	bool							bIsHuangzhuang;						//是否荒庄
	
	WORD							wProvideUser;						//点炮用户	
	WORD							wChiHuKind[4];						//胡牌类型
	WORD                            wChiHuRight[4];                     //胡牌权位
	
	int                             iChiHuType;                         //胡牌类型

	bool                            IsUserLeft[GAME_PLAYER];            //用户是否强退
	BYTE                            cbKingCount[GAME_PLAYER];           //王牌数目
	
	int                             iChiHuKind[GAME_PLAYER];           //胡牌类型（0：没胡；1：自摸；2：胡牌；3：放炮） 
	
	LONGLONG						cbTimes[GAME_PLAYER];               //玩家番数
	LONGLONG						iMuscle[GAME_PLAYER];					// 筋	
	LONGLONG						iCellScore[GAME_PLAYER];            //房间倍率（单元积分）
	LONGLONG						lGameScore[4];					    //游戏积分

	CMD_S_GameEnd()
	{
		ZeroMemory(this, sizeof(*this));
		wProvideUser = -1;
	}
};
//用户托管
struct CMD_S_Trustee
{
	bool							bTrustee;							//是否托管
	WORD							wChairID;							//托管用户

	CMD_S_Trustee()
	{
		ZeroMemory(this, sizeof(*this));
		wChairID = -1;
	}
};
//用户听牌
struct CMD_S_GameTingCard
{
	WORD							wTingCardUser;						//听牌用户

	CMD_S_GameTingCard()
	{
		ZeroMemory(this, sizeof(*this));
		wTingCardUser = -1;
	}
};

//用户听牌
struct CMD_S_Listen
{
	WORD							wChairId;							//听牌用户
};

///补牌命令
struct CMD_S_ReplaceCard
{
	WORD							wReplaceUser;						//补牌用户
	BYTE							cbReplaceCard;						//补牌扑克
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CARD				1									//出牌命令
#define SUB_C_OPERATE_CARD			2									//操作扑克
#define SUB_C_OPERATE_HAI_DI		3									//海底操作
#define SUB_C_TRUSTEE				4									//用户托管
#define SUB_C_USERTINGCARD			5									//听牌命令
#define SUB_C_SZSTART               6                                   //启动筛子
#define SUB_C_LISTEN				7									///用户听牌

//出牌命令
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//扑克数据

	CMD_C_OutCard()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//操作命令
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克

	CMD_C_OperateCard()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//海底操作
struct CMD_C_OperateHaiDi
{
	BYTE							cbTakeHaiDi;						//获取海底

	CMD_C_OperateHaiDi()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

///用户听牌
struct CMD_C_Listen
{
	BYTE							cbListen;							//听牌用户
};

//用户托管
struct CMD_C_Trustee
{
	bool							bTrustee;							//是否托管	

	CMD_C_Trustee()
	{
		ZeroMemory(this, sizeof(*this));
	}
};
//用户听牌操作
struct CMD_UserTingOperate
{
	bool							bTingCard;							//是否听牌

	CMD_UserTingOperate()
	{
		ZeroMemory(this, sizeof(*this));
	}
};


//////////////////////////////////////////////////////////////////////////

#endif