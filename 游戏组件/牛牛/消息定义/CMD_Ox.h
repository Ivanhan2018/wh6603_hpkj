#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

#pragma pack(1)

#define COMPARE_ZERO	0.001
static int s_nmultiple[5]={1,2,5,10,20};


#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
//梦成网络添加
#define	Mywtof		atof

#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#define myatoi      _wtoi
#define myatoi64	_wtoi64
//梦成网络添加
#define	Mywtof		_wtof

#endif

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID							101									//游戏 I D
#define GAME_PLAYER						4									//游戏人数
#define GAME_NAME						TEXT("牛牛")				        //游戏名字
#define MAXCOUNT						5									//扑克数目

#ifdef DEBUG
	#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
	#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本
#else
	#define VERSION_SERVER				PROCESS_VERSION(7,0,3)				//程序版本
	#define VERSION_CLIENT				PROCESS_VERSION(7,0,3)				//程序版本
#endif
//结束原因
#define GER_NO_PLAYER					0x10								//没有玩家

//游戏状态
#define GS_TK_FREE					GAME_STATUS_FREE						//等待开始
#define GS_TK_CALL					GAME_STATUS_PLAY						//叫庄状态
#define GS_TK_SCORE					GAME_STATUS_PLAY+1						//下注状态
#define GS_TK_PLAYING				GAME_STATUS_PLAY+2						//游戏进行

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START				100									//游戏开始
#define SUB_S_ADD_SCORE					101									//加注结果
#define SUB_S_PLAYER_EXIT				102									//用户强退
#define SUB_S_SEND_CARD					103									//发牌消息
#define SUB_S_GAME_END					104									//游戏结束
#define SUB_S_OPEN_CARD					105									//用户摊牌
#define SUB_S_CALL_BANKER				106									//用户叫庄
#define SUB_S_AMDIN_COMMAND				108									//系统控制

//游戏状态
struct CMD_S_StatusFree
{
	SCORE							lCellScore;							//基础积分

	//历史积分
	SCORE								lTurnScore[GAME_PLAYER];			//积分信息
	SCORE								lCollectScore[GAME_PLAYER];			//积分信息
	TCHAR								szGameRoomName[64];					//房间名称
};

//游戏状态
struct CMD_S_StatusCall
{
	SCORE								lCellScore;							//基础积分

	WORD								wCallBanker;						//叫庄用户
	BYTE                                cbDynamicJoin;                      //动态加入 
	BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态

	//历史积分
	SCORE								lTurnScore[GAME_PLAYER];			//积分信息
	SCORE								lCollectScore[GAME_PLAYER];			//积分信息
	TCHAR								szGameRoomName[64];					//房间名称
};

//游戏状态
struct CMD_S_StatusScore
{
	SCORE								lCellScore;							//基础积分
	//下注信息
	BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
	BYTE                                cbDynamicJoin;                      //动态加入
	SCORE								lTurnMaxScore;						//最大下注
	SCORE								lTableScore[GAME_PLAYER];			//下注数目
	WORD								wBankerUser;						//庄家用户

	//历史积分
	SCORE								lTurnScore[GAME_PLAYER];			//积分信息
	SCORE								lCollectScore[GAME_PLAYER];			//积分信息
	TCHAR								szGameRoomName[64];					//房间名称
};

//游戏状态
struct CMD_S_StatusPlay
{
	SCORE								lCellScore;							//基础积分
	//状态信息
	BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
	BYTE                                cbDynamicJoin;                      //动态加入
	SCORE								lTurnMaxScore;						//最大下注
	SCORE								lTableScore[GAME_PLAYER];			//下注数目
	WORD								wBankerUser;						//庄家用户

	//扑克信息
	BYTE								cbHandCardData[GAME_PLAYER][MAXCOUNT];//桌面扑克
	BYTE								bOxCard[GAME_PLAYER];				//牛牛数据

	//历史积分
	SCORE								lTurnScore[GAME_PLAYER];			//积分信息
	SCORE								lCollectScore[GAME_PLAYER];			//积分信息
	TCHAR								szGameRoomName[64];					//房间名称
};

//用户叫庄
struct CMD_S_CallBanker
{
	WORD								wCallBanker;						//叫庄用户
	bool								bFirstTimes;						//首次叫庄
};

//游戏开始
struct CMD_S_GameStart
{
	//下注信息
	SCORE								lTurnMaxScore;						//最大下注
	WORD								wBankerUser;						//庄家用户
};

//用户下注
struct CMD_S_AddScore
{
	WORD								wAddScoreUser;						//加注用户
	SCORE								lAddScoreCount;						//加注数目
};

//游戏结束
struct CMD_S_GameEnd
{
	SCORE								lGameTax[GAME_PLAYER];				//游戏税收
	SCORE								lGameScore[GAME_PLAYER];			//游戏得分
	BYTE								cbCardData[GAME_PLAYER];			//用户扑克
};

//发牌数据包
struct CMD_S_SendCard
{
	BYTE								cbCardData[GAME_PLAYER][MAXCOUNT];	//用户扑克
};

//用户退出
struct CMD_S_PlayerExit
{
	WORD								wPlayerID;							//退出用户
};

//用户摊牌
struct CMD_S_Open_Card
{
	WORD								wPlayerID;							//摊牌用户
	BYTE								bOpen;								//摊牌标志
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_CALL_BANKER				1									//用户叫庄
#define SUB_C_ADD_SCORE					2									//用户加注
#define SUB_C_OPEN_CARD					3									//用户摊牌

//用户叫庄
struct CMD_C_CallBanker
{
	BYTE								bBanker;							//做庄标志
};


//用户加注
struct CMD_C_AddScore
{
	SCORE								lScore;								//加注数目
};

//用户摊牌
struct CMD_C_OxCard
{
	BYTE								bOX;								//牛牛标志
};

//////////////////////////////////////////////////////////////////////////
//控制定义
#define SUB_C_AMDIN_COMMAND			5									//管理员命令

#define RQ_OPTION_CANCLE		1		//取消
#define RQ_OPTION_QUERYING		2		//查询
#define RQ_OPTION_SETING		3		//设置

#define CHEAT_TYPE_LOST		0		//输
#define CHEAT_TYPE_WIN		1		//赢

struct CMD_C_AdminReq
{
	BYTE cbReqType;							//请求类型
	BYTE cbCheatCount;						//控制次数
	BYTE cbCheatType;						//控制类型
	DWORD dwGameID;							//玩家标识
};

#pragma pack()

#endif