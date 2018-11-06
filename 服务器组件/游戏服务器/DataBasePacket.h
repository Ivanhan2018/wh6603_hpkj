#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//用户事件
#define	DBR_GR_LOGON_USERID			100									//I D 登录
#define	DBR_GR_LOGON_MOBILE			101									//手机登录
#define	DBR_GR_LOGON_ACCOUNTS		102									//帐号登录
#define DBR_GR_QUERY_IP_DESCRIBE    103                                 //查询IP描述 

//系统事件
#define DBR_GR_WRITE_GAME_SCORE		200									//游戏积分
#define DBR_GR_LEAVE_GAME_SERVER	201									//离开房间
#define DBR_GR_GAME_SCORE_RECORD	202									//积分记录
#define DBR_GR_DELETE_USER_LOCKER	203									//解锁用户<by hxh>

//配置事件
#define DBR_GR_LOAD_PARAMETER		300									//加载参数
#define DBR_GR_LOAD_GAME_COLUMN		301									//加载列表
#define DBR_GR_LOAD_ANDROID_USER	302									//加载机器

//银行事件
#define DBR_GR_USER_SAVE_SCORE		400									//存入金币
#define DBR_GR_USER_TAKE_SCORE		401									//提取金币
#define DBR_GR_USER_TRANSFER_SCORE	402									//转账金币
#define DBR_GR_QUERY_INSURE_INFO	403									//查询银行

//游戏事件
#define DBR_GR_GAME_FRAME_REQUEST	400									//游戏请求

//////////////////////////////////////////////////////////////////////////////////
//输出信息

//逻辑事件
#define DBO_GR_LOGON_SUCCESS		100									//登录成功
#define DBO_GR_LOGON_FAILURE		101									//登录失败

//配置事件
#define DBO_GR_GAME_PARAMETER		200									//配置信息
#define DBO_GR_GAME_COLUMN_INFO		201									//列表信息
#define DBR_GR_GAME_ANDROID_INFO	202									//机器信息

//银行命令
#define DBO_GR_USER_INSURE_INFO		300									//银行资料
#define DBO_GR_USER_INSURE_SUCCESS	301									//银行成功
#define DBO_GR_USER_INSURE_FAILURE	302									//银行失败

//游戏事件
#define DBO_GR_GAME_FRAME_RESULT	400									//游戏结果

//用户事件
#define DBO_GR_QUERY_IP_DESCRIBE    500                                 //查询IP描述 

//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct DBR_GR_LogonUserID
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//附加信息
	BYTE                            cbPlazaType;                        //广场类型
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szIPAdrr[512];					//玩家IP

};

//ID 登录
struct DBR_GR_LogonMobile
{	
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	// add by hxh 20160627
#if 1
	//BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数
#endif
	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//帐号登录
struct DBR_GR_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	BYTE                            cbPlazaType;                        //广场类型
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//游戏积分
struct DBR_GR_WriteGameScore
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址

	//用户信息
	DWORD							dwDBQuestID;						//请求标识
	DWORD							dwInoutIndex;						//进出索引

	//成绩变量
	tagVariationInfo				VariationInfo;						//提取信息
};

//离开房间
struct DBR_GR_LeaveGameServer
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwInoutIndex;						//记录索引
	DWORD							dwDBQuestID;						//请求标识
	DWORD							dwLeaveReason;						//离开原因
	DWORD							dwOnLineTimeCount;					//在线时长

	//成绩变量
	tagVariationInfo				RecordInfo;							//记录信息
	tagVariationInfo				VariationInfo;						//提取信息

	//系统信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//解锁用户<by hxh>
struct DBR_GR_DeleteUserLocker
{
	//用户信息
	DWORD							dwUserID;							//用户标识
};

//游戏记录
struct DBR_GR_GameScoreRecord
{
	//桌子信息
	WORD							wTableID;							//桌子号码
	WORD							wUserCount;							//用户数目
	WORD							wAndroidCount;						//机器数目

	//损耗税收
	SCORE							lWasteCount;						//损耗数目
	SCORE							lRevenueCount;						//税收数目

	//统计信息
	SCORE							lUserMedal;							//奖牌数目
	DWORD							dwPlayTimeCount;					//游戏时间

	//时间信息
	SYSTEMTIME						SystemTimeStart;					//开始时间
	SYSTEMTIME						SystemTimeConclude;					//结束时间

	//积分记录
	WORD							wRecordCount;						//记录数目
	tagGameScoreRecord				GameScoreRecord[MAX_CHAIR];			//游戏记录

	//游戏记录
	TCHAR                           szDrawInfo[DRAW_INFO_MAX_LEN];      //游戏记录
};

//存入金币
struct DBR_GR_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入金币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出金币
struct DBR_GR_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取金币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账金币
struct DBR_GR_UserTransferScore
{
	DWORD                           dwUserID;                           //用户 I D
	SCORE							lTransferScore;						//转账金币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
	TCHAR                           szLogonPass[LEN_PASSWORD];          //登录密码
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询银行
struct DBR_GR_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//////////////////////////////////////////////////////////////////////////////////

//登录成功
struct DBO_GR_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lInsure;							//用户银行
	SCORE                           lUserMedal;                         //用户奖牌 
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwExperience;						//用户经验

	//用户权限
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限

	//索引变量
	DWORD							dwInoutIndex;						//记录索引
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR                           szAddrDescribe[LEN_ADDR_DESCRIBE];  //地址描述

	// add by hxh 20160627
#if 1
	//手机定义
	//BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数
#endif

	//辅助信息
	BYTE                            cbPlazaType;                        //广场类型
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szDescribeString[128];				//错误消息
};

//登录失败
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};

//配置信息
struct DBO_GR_GameParameter
{
	WORD							wRevenueRate;						//银行税收

	//版本信息
	DWORD							dwClientVersion;					//客户版本
	DWORD							dwServerVersion;					//服务版本
};

//列表信息
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//结果代码
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//列表信息
};

//机器信息
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//结果代码
	WORD							wAndroidCount;						//用户数目
	tagAndroidParameter				AndroidParameter[MAX_ANDROID];		//机器信息
};

//银行资料
struct DBO_GR_UserInsureInfo
{
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//银行成功
struct DBO_GR_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSourceScore;						//原来金币
	SCORE							lSourceInsure;						//原来金币
	SCORE							lInsureRevenue;						//银行税收
	SCORE							lFrozenedScore;						//冻结积分
	SCORE							lVariationScore;					//金币变化
	SCORE							lVariationInsure;					//银行变化
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct DBO_GR_UserInsureFailure
{
	LONG							lResultCode;						//操作代码
	SCORE							lFrozenedScore;						//冻结积分
	TCHAR							szDescribeString[128];				//描述消息
};

//查询成功
struct DBR_GR_QueryIpDescribeSuccess
{
	TCHAR                           szAddrDescribe[LEN_ADDR_DESCRIBE];  //地址描述
};

//系统消息<by hxh>
struct DBR_GR_SystemMessage
{
	BYTE                            cbMessageID;                        //消息ID
	BYTE							cbMessageType;						//消息类型
	BYTE                            cbAllRoom;                          //全体房间
	DWORD							dwTimeRate;						    //时间频率
	__time64_t                      tConcludeTime;                      //结束时间
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

//////////////////////////////////////////////////////////////////////////////////

#endif