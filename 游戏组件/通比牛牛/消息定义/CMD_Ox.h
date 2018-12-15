#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

#pragma pack(push)  
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID							136									//游戏 I D
#define GAME_PLAYER						6									//游戏人数
#define GAME_NAME						TEXT("通比牛牛")					//游戏名字
#define MAX_COUNT						5									//最大数目
#define MAX_JETTON_AREA					6									//下注区域
#define MAX_TIMES						5									//最大赔率

#define VERSION_SERVER					PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//程序版本

//游戏状态
#define GS_TK_FREE						GAME_STATUS_FREE					//等待开始
#define GS_TK_PLAYING					GAME_STATUS_PLAY					//游戏进行

//命令消息
#define IDM_ADMIN_UPDATE_STORAGE		WM_USER+1001
#define IDM_ADMIN_MODIFY_STORAGE		WM_USER+1011
#define IDM_REQUEST_QUERY_USER			WM_USER+1012
#define IDM_USER_CONTROL				WM_USER+1013

//操作记录
#define MAX_OPERATION_RECORD			20									//操作记录条数
#define RECORD_LENGTH					128									//每条记录字长

//////////////////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START				100									//游戏开始
#define SUB_S_PLAYER_EXIT				101									//用户强退
#define SUB_S_SEND_CARD					102									//发牌消息
#define SUB_S_GAME_END					103									//游戏结束
#define SUB_S_OPEN_CARD					104									//用户摊牌
#define SUB_S_ANDROID_BANKOPERATOR		105									//机器人银行操作
#define SUB_S_ADMIN_STORAGE_INFO		112									//刷新控制服务端
#define SUB_S_REQUEST_QUERY_RESULT		113									//查询用户结果
#define SUB_S_USER_CONTROL				114									//用户控制
#define SUB_S_USER_CONTROL_COMPLETE		115									//用户控制完成
#define SUB_S_OPERATION_RECORD		    116									//操作记录

//机器人存款取款
struct tagCustomAndroid
{
	SCORE									lRobotScoreMin;	
	SCORE									lRobotScoreMax;
	SCORE	                                lRobotBankGet; 
	SCORE									lRobotBankGetBanker; 
	SCORE									lRobotBankStoMul; 
};

//控制类型
typedef enum{CONTINUE_WIN, CONTINUE_LOST, CONTINUE_CANCEL}CONTROL_TYPE;

//控制结果      控制成功 、控制失败 、控制取消成功 、控制取消无效
typedef enum{CONTROL_SUCCEED, CONTROL_FAIL, CONTROL_CANCEL_SUCCEED, CONTROL_CANCEL_INVALID}CONTROL_RESULT;

//用户行为
typedef enum{USER_SITDOWN, USER_STANDUP, USER_OFFLINE, USER_RECONNECT}USERACTION;

//控制信息
typedef struct
{
	CONTROL_TYPE						control_type;					  //控制类型
	BYTE								cbControlCount;					  //控制局数
	bool							    bCancelControl;					  //取消标识
}USERCONTROL;

//房间用户信息
typedef struct
{
	WORD								wChairID;							//椅子ID
	WORD								wTableID;							//桌子ID
	DWORD								dwGameID;							//GAMEID
	bool								bAndroid;							//机器人标识
	TCHAR								szNickName[LEN_NICKNAME];			//用户昵称
	BYTE								cbUserStatus;						//用户状态
	BYTE								cbGameStatus;						//游戏状态
}ROOMUSERINFO;

//房间用户控制
typedef struct
{
	ROOMUSERINFO						roomUserInfo;						//房间用户信息
	USERCONTROL							userControl;						//用户控制
}ROOMUSERCONTROL;

//游戏状态
struct CMD_S_StatusFree
{
	//游戏变量
	LONGLONG							lCellScore;							//基础积分
	LONGLONG							lRoomStorageStart;					//房间起始库存
	LONGLONG							lRoomStorageCurrent;				//房间当前库存

	//历史积分
	LONGLONG							lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG							lCollectScore[GAME_PLAYER];			//积分信息
	tagCustomAndroid					CustomAndroid;						//机器人配置
};

//游戏状态
struct CMD_S_StatusPlay
{
	//状态信息
	BYTE                                cbDynamicJoin;                      //动态加入
	BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
	LONGLONG							lTableScore[GAME_PLAYER];			//下注数目
	LONGLONG							lCellScore;							//基础积分
	WORD								wBankerUser;						//庄家用户

	LONGLONG							lRoomStorageStart;					//房间起始库存
	LONGLONG							lRoomStorageCurrent;				//房间当前库存
	tagCustomAndroid					CustomAndroid;						//机器人配置

	//扑克信息
	BYTE								bOxCard[GAME_PLAYER];				//牛牛数据
	BYTE								cbOxCardData[GAME_PLAYER][MAX_COUNT];//牛牛扑克
	BYTE								cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克
	

	//历史积分
	LONGLONG							lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG							lCollectScore[GAME_PLAYER];			//积分信息
};

//游戏开始
struct CMD_S_GameStart
{
	WORD								wBankerUser;						//庄家用户
	BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//用户扑克
	LONGLONG							lCellScore;							//游戏底分
};

//游戏结束
struct CMD_S_GameEnd
{
	LONGLONG							lGameTax[GAME_PLAYER];				//游戏税收
	LONGLONG							lGameScore[GAME_PLAYER];			//游戏得分
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//用户扑克
	BYTE								cbDelayOverGame;
};

//发牌数据包
struct CMD_S_SendCard
{
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//用户扑克
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

struct CMD_S_RequestQueryResult
{
	ROOMUSERINFO						userinfo;							//用户信息
	bool								bFind;								//找到标识
};

//用户控制
struct CMD_S_UserControl
{
	DWORD									dwGameID;							//GAMEID
	TCHAR									szNickName[LEN_NICKNAME];			//用户昵称
	CONTROL_RESULT							controlResult;						//控制结果
	CONTROL_TYPE							controlType;						//控制类型
	BYTE									cbControlCount;						//控制局数
};

//用户控制
struct CMD_S_UserControlComplete
{
	DWORD									dwGameID;							//GAMEID
	TCHAR									szNickName[LEN_NICKNAME];			//用户昵称
	CONTROL_TYPE							controlType;						//控制类型
	BYTE									cbRemainControlCount;				//剩余控制局数
};

//控制服务端库存信息
struct CMD_S_ADMIN_STORAGE_INFO
{
	LONGLONG	lRoomStorageStart;						//房间起始库存
	LONGLONG	lRoomStorageCurrent;
	LONGLONG	lRoomStorageDeduct;
	LONGLONG	lMaxRoomStorage[2];
	WORD		wRoomStorageMul[2];
};

//操作记录
struct CMD_S_Operation_Record
{
	TCHAR		szRecord[MAX_OPERATION_RECORD][RECORD_LENGTH];					//记录最新操作的20条记录
};

//////////////////////////////////////////////////////////////////////////

//客户端命令结构
#define SUB_C_OPEN_CARD					1									//用户摊牌
#define SUB_C_STORAGE					6									//更新库存
#define	SUB_C_STORAGEMAXMUL				7									//设置上限
#define SUB_C_REQUEST_QUERY_USER		8									//请求查询用户
#define SUB_C_USER_CONTROL				9									//用户控制

//用户摊牌
struct CMD_C_OxCard
{
	BYTE								bOX;								//牛牛标志
	BYTE								cbOxCardData[MAX_COUNT];			//牛牛扑克
};

struct CMD_C_UpdateStorage
{
	LONGLONG						lRoomStorageCurrent;					//库存数值
	LONGLONG						lRoomStorageDeduct;						//库存数值
};

struct CMD_C_ModifyStorage
{
	LONGLONG						lMaxRoomStorage[2];							//库存上限
	WORD							wRoomStorageMul[2];							//赢分概率
};

struct CMD_C_RequestQuery_User
{
	DWORD							dwGameID;								//查询用户GAMEID
	TCHAR							szNickName[LEN_NICKNAME];			    //查询用户昵称
};

//用户控制
struct CMD_C_UserControl
{
	DWORD									dwGameID;							//GAMEID
	TCHAR									szNickName[LEN_NICKNAME];			//用户昵称
	USERCONTROL								userControlInfo;					//
};

#pragma pack(pop)

#endif
