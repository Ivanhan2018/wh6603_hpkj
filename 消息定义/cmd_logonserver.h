#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GP_LOGON				1									//广场登录

//登录模式
#define SUB_GP_LOGON_ACCOUNTS		1									//帐号登录
#define SUB_GP_REGISTER_ACCOUNTS	2									//注册帐号

#define SUB_GP_GET_CP_USERINFO		103									//查询彩票用户信息
#define SUB_GP_GET_CP_USERINFO_RET		104									//查询彩票用户信息

//登录结果
#define SUB_GP_LOGON_SUCCESS		100									//登录成功
#define SUB_GP_LOGON_FAILURE		101									//登录失败
#define SUB_GP_LOGON_FINISH			102									//登录完成

#define SUB_GP_KILL_SOCKET			644									//强制下线
//升级提示
#define SUB_GP_UPDATE_NOTIFY		200									//升级提示
// #define SUB_GP_GET_CP_USERINFO		103									//获取彩票用户信息

//////////////////////////////////////////////////////////////////////////////////

//帐号登录
struct CMD_GP_LogonAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	//梦成网络添加
	TCHAR							szQQ[LEN_QQ];		//QQ号码
	TCHAR							szEmain[LEN_EMAIL];	//E-Mail
	TCHAR							szIPhone[LEN_MOBILE_PHONE];	//手机号码

};

//帐号登录
struct CMD_GP_GetCpUserInfo
{
	int								n_userid;
};

//登录成功
struct CMD_GP_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwExperience;						//经验数值

	//用户成绩
	SCORE							lScore;								//用户积分
	SCORE							lInsure;							//用户银行
	SCORE                           lUserMedal;                         //用户奖牌 

	//用户信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR                           szAddrDescribe[LEN_ADDR_DESCRIBE];  //地址描述

	int								n_type;
	DOUBLE							f_fandian;
	DOUBLE							f_dongjie;
	DOUBLE							f_yue;

	//梦成网络
	BYTE							cbShowServerStatus;					//是否显示在线人数
};

//登录失败
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//登陆完成
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//中断时间
	WORD							wOnLineCountTime;					//更新时间
};

//升级提示
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};


struct CMD_GP_KillSocket
{
	BYTE							cbResult;
	TCHAR							szMessage[256];
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//社团信息
#define DTP_GP_MEMBER_INFO			2									//会员信息
#define	DTP_GP_UNDER_WRITE			3									//个性签名
#define DTP_GP_STATION_URL			4									//主页信息

//社团信息
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//社团索引
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
};

//会员信息
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST				1									//获取列表
#define SUB_GP_GET_ONLINE			2									//获取在线

//列表信息
#define SUB_GP_LIST_TYPE			100									//类型列表
#define SUB_GP_LIST_KIND			101									//种类列表
#define SUB_GP_LIST_SERVER			102									//房间列表
#define SUB_GP_LIST_FINISH			103									//发送完成

//在线信息
#define SUB_GR_KINE_ONLINE			300									//类型在线
#define SUB_GR_SERVER_ONLINE		301									//房间在线

//////////////////////////////////////////////////////////////////////////////////

//获取在线
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//房间数目
	WORD							wOnLineServerID[MAX_SERVER];		//房间标识
};

//类型在线
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//类型数目
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//类型在线
};

//房间在线
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//房间数目
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};

//////////////////////////////////////////////////////////////////////////////////
//服务命令

#define MDM_GP_USER_SERVICE			3									//用户服务

//账号服务
#define SUB_GP_MODIFY_MACHINE		100									//修改机器
#define SUB_GP_MODIFY_LOGON_PASS	101									//修改密码
#define SUB_GP_MODIFY_INSURE_PASS	102									//修改密码

// #define SUB_GP_GET_CP_USERINFO		103									//查询彩票用户信息
// #define SUB_GP_GET_CP_USERINFO_RET		104									//查询彩票用户信息

//修改头像
#define SUB_GP_USER_FACE_INFO		200									//头像信息
#define SUB_GP_SYSTEM_FACE_INFO		201									//系统头像

//个人资料
#define SUB_GP_USER_INDIVIDUAL		301									//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL		302									//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL	303									//修改资料

//银行服务
#define SUB_GP_USER_SAVE_SCORE		400									//存款操作
#define SUB_GP_USER_TAKE_SCORE		401									//取款操作
#define SUB_GP_USER_INSURE_INFO		402									//银行资料
#define SUB_GR_USER_TRANSFER_SCORE  403 								//取款操作
#define SUB_GP_QUERY_INSURE_INFO	404									//查询银行
#define SUB_GP_USER_INSURE_SUCCESS	405									//银行成功
#define SUB_GP_USER_INSURE_FAILURE	406									//银行失败
//游戏开奖查询
#define SUB_GP_QUERY_GAME_RESULT	500									//查询开奖结果
#define SUB_GP_QUERY_GAME_RET		501									//查询开奖结果
//查询系统时间
#define	SUB_GP_QUERY_TIME			502									//查询时间
#define	SUB_GP_QUERY_TIME_RET		503									//查询时间
//重庆时时彩下注
#define	SUB_GP_TOUZHU_CQSSC			504									//查询时间
#define	SUB_GP_TOUZHU_CQSSC_RET		505									//查询时间
#define	SUB_GP_TOUZHU_CQSSC_DAN		908									//单式下注
#define	SUB_GP_TOUZHU_CONTINUE_RET		407									//查询时间
#define	SUB_GP_TOUZHU_CQSSC_ZHUIHAO		408									//单式下注
#define	SUB_MB_TOUZHU_CQSSC_ZHUIHAO		636									//手机追号

//查询银行
#define	SUB_GP_QUERY_YINHANG			909									//查询银行
#define	SUB_GP_QUERY_YINHANG_RET		910									//查询银行
//操作结果
#define SUB_GP_OPERATE_SUCCESS		900									//操作成功
#define SUB_GP_OPERATE_FAILURE		901									//操作失败

//游戏开奖查询
#define SUB_GP_GET_CP_USER			902									//获取彩票玩家信息
#define SUB_GP_GET_CP_USER_RET		903									//获取彩票玩家信息结果

//查询返点
#define SUB_GP_GET_USER_FANDIAN			904									//获取彩票玩家返点
#define SUB_GP_GET_USER_FANDIAN_RET		905									//获取彩票玩家返点结果

//设置返点
#define SUB_GP_SET_USER_BONUS			906									//设置彩票玩家返点
#define SUB_GP_SET_USER_BONUS_RET		907									//设置彩票玩家返点结果

//获取MAPBONUS
#define SUB_GP_GET_MAP_BONUS			598									//获取MAPBONUS
#define SUB_GP_GET_MAP_BONUS_RET		599									//获取MAPBONUS结果
struct CMD_GP_GetMyMapBonus
{
	int								n_t_userid;
};
#define SUB_GP_GET_CHONGZHI_TISHI	508									//充值提示
#define SUB_GP_GET_CHONGZHI_TISHI_RET		509							//充值提示返回

#define SUB_GP_GET_TIXIAN_TISHI		512									//提现提示
#define SUB_GP_GET_TIXIAN_TISHI_RET		513							//提现提示返回

#define SUB_GP_GET_TOUZHU_TISHI		516									//投注提示
#define SUB_GP_GET_TOUZHU_TISHI_RET		517							//投注提示返回

//个人账户
#define SUB_GP_GET_TOUZHU_LOG_COUNT			506								//查询投注日志数
#define SUB_GR_GET_TOUZHU_LOG_COUNT_RET		507							//查询投注日志数返回

//查询投注日志数
struct CMD_GP_GetTouzhuLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetTouzhuLogCountRet
{
	DWORD							dwCount;							//返回
};
#define SUB_GP_GET_TOUZHU_LOG			514								//查询投注日志
#define SUB_GP_GET_TOUZHU_LOG_RET		515								//返回

struct	CMD_GP_GetTouzhuLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetTouzhuLogRet
{
	int								n_t_id;
	int								n_t_type;
	int								n_t_kind;
	TCHAR							szQishu[30];
	int								n_t_zhushu;
	int								n_t_moshi;
	int								n_t_beishu;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	int								n_t_winzhushu;
	DOUBLE							f_t_yingkui;
	int								n_t_zhuihao;
};

#define SUB_GP_CANCEL_TOUZHU			510								//撤单
#define SUB_GR_CANCEL_TOUZHU_RET		511								//撤单返回
struct CMD_GP_CancelTouzhu
{
	int								n_t_userid;							//用户ID
	int								n_t_id;								//投注ID
	int								n_t_kindid;
	TCHAR							s_t_qihao[30];
};
struct CMD_GR_CancelTouzhuRet
{
	int								nResult;
};
//////////////////////////////////////////////////////////////提现日志////////////////
//个人账户
#define SUB_GP_GET_TIXIAN_LOG_COUNT			520								//查询提现日志数
#define SUB_GR_GET_TIXIAN_LOG_COUNT_RET		521							//查询提现日志数返回

//查询投注日志数
struct CMD_GP_GetTixianLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetTixianLogCountRet
{
	DWORD							dwCount;							//返回
};

//个人账户
#define SUB_GP_GET_CHONGZHI_LOG_COUNT			522								//查询提现日志数
#define SUB_GR_GET_CHONGZHI_LOG_COUNT_RET		523							//查询提现日志数返回

//查询投注日志数
struct CMD_GP_GetChongzhiLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetChongzhiLogCountRet
{
	DWORD							dwCount;							//返回
};

//个人账户
#define SUB_GP_GET_YINGKUI_LOG_COUNT			528								//查询盈亏日志数
#define SUB_GR_GET_YINGKUI_LOG_COUNT_RET		529							//查询盈亏日志数返回

//查询投注日志数
struct CMD_GP_GetYingkuiLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetYingkuiLogCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_TIXIAN_LOG			518								//查询投注日志
#define SUB_GP_GET_TIXIAN_LOG_RET		519								//返回

struct	CMD_GP_GetTixianLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetTixianLogRet
{
	int								n_t_id;
	SYSTEMTIME						n_t_time;
	TCHAR							n_t_yinhang[30];
	DOUBLE							f_t_jine;
	TCHAR							s_t_kaihuren[30];
	int								n_t_state;
	TCHAR							s_t_note[30];
};
///////////////////////盈亏明细///////////////////////////////////////////////////////
//个人账户
#define SUB_GP_GET_YINGKUI_MX_COUNT			626								//查询盈亏日志数
#define SUB_GR_GET_YINGKUI_MX_COUNT_RET		627							//查询盈亏日志数返回

//查询盈亏明细
struct CMD_GP_GetYingkuiMxCount
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetYingkuiMxCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_YINGKUI_MX			628								//查询盈亏明细
#define SUB_GP_GET_YINGKUI_MX_RET		629								//返回

struct	CMD_GP_GetYingkuiMx
{
	int							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	int							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetYingkuiMxRet
{
	int								n_t_dingdan;
	DOUBLE							f_t_yingkui;
	DOUBLE							f_t_yue;
	SYSTEMTIME						t_t_yingkuitime;
	TCHAR							s_t_type[30];
	TCHAR							s_t_zhaiyao[126];
};

///////////////////////棋牌盈亏///////////////////////////////////////////////////////
//个人账户
#define SUB_GP_GET_QP_YINGKUI_COUNT			411								//查询盈亏日志数
#define SUB_GR_GET_QP_YINGKUI_COUNT_RET		412							//查询盈亏日志数返回

//查询盈亏明细
struct CMD_GP_GetQiPaiYingkuiCount
{
	DWORD							dwUserID;							//用户ID
	int								nTypeID;					
	int								nByTime;					
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetQiPaiYingkuiCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_QP_YINGKUI			413								//查询盈亏明细
#define SUB_GP_GET_QP_YINGKUI_RET		414								//返回

struct	CMD_GP_GetQiPaiYingkui
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetQiPaiYingkuiRet
{
	int								n_t_userid;
	int								n_t_drawid;
	DOUBLE							f_t_score;
	int								n_t_playcount;
	DOUBLE							f_t_revenue;
	SYSTEMTIME							n_t_playtime;
	TCHAR							s_t_servername[33];
	TCHAR							s_t_accounts[33];
};

//////////////////////////////////////////////////////////////////////////////////////
#define SUB_GP_GET_CHONGZHI_LOG			524								//查询投注日志
#define SUB_GP_GET_CHONGZHI_LOG_RET		525								//返回

struct	CMD_GP_GetChongzhiLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetChongzhiLogRet
{
	int								n_t_id;
	DOUBLE							f_t_jine;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	TCHAR							s_t_pingtai[30];
};

#define SUB_GP_GET_YINGKUI_LOG			526								//查询盈亏日志
#define SUB_GP_GET_YINGKUI_LOG_RET		527								//返回

struct	CMD_GP_GetYingkuiLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetYingkuiLogRet
{
	DOUBLE							f_t_chongzhi;
	DOUBLE							f_t_qukuan;
	DOUBLE							f_t_touzhu;
	DOUBLE							f_t_fandian;
	DOUBLE							f_t_zhongjiang;
	DOUBLE							f_t_huodong;
	DOUBLE							f_t_chedan;
};

#define SUB_GP_GET_USER_INFO			530								//查询盈亏日志
#define SUB_GR_GET_USER_INFO_RET		531								//返回

struct	CMD_GP_GetUserInfo
{
	DWORD							dwUserID;							//用户ID
};

struct CMD_GR_GetUserInfoRet
{
	TCHAR							s_t_qq[12];
	SYSTEMTIME						t_reg_time;						// 注册时间
	TCHAR							s_t_sjqq[12];
	DOUBLE							f_t_yongjin;					//佣金
	DOUBLE							f_t_yongjinze;					//佣金总额
	DOUBLE							f_t_fenhong;					//分红
	DOUBLE							f_t_fenhongze;					//分红总额
	DOUBLE							f_t_qipaiye;					//棋牌余额
	DOUBLE							f_t_kuisunyongjin;					//亏损佣金
	DOUBLE							f_t_kuisunyongjinze;					//亏损佣金总额

	SYSTEMTIME						t_ksyj_time;						// 亏损佣金时间

	SYSTEMTIME						t_fh_time;						//	分红时间 
	SYSTEMTIME						t_yj_time;						// 佣金时间

};
#define SUB_GP_XG_QQ			532								//修改QQ
#define SUB_GR_XG_QQ_RET		533								//返回

struct	CMD_GP_XGqq
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szQQ[11];							//新QQ 
};
struct CMD_GR_XG_QQ_RET
{
	LONG							lResult;
};

#define SUB_GP_XG_LOGIN_PASS			534								//修改登录密码
#define SUB_GR_XG_LOGIN_PASS_RET		535								//返回

struct	CMD_GP_XG_Loginpass
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szOldPass[33];						//旧密码
	TCHAR							szNewPass[33];						//新密码
};
struct CMD_GR_XG_Loginpass_RET
{
	int							lResult;
};
#define SUB_GP_LOCK_MACHINE			919								//锁定本机
#define SUB_GR_LOCK_MACHINE_RET		920								//返回

struct	CMD_GP_LockMachine
{
	DWORD							dwUserID;							//用户ID
	BYTE						cbLock;									//锁定
};
struct CMD_GR_LockMachine_RET
{
	int							lResult;
};

#define SUB_GP_XG_QUKUAN_PASS			536								//修改登录密码
#define SUB_GR_XG_QUKUAN_PASS_RET		537								//返回

struct	CMD_GP_XG_Qukuanpass
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szOldPass[33];						//旧密码
	TCHAR							szNewPass[33];						//新密码
};
struct CMD_GR_XG_Qukuanpass_RET
{
	int							lResult;
};
#define SUB_GP_SET_QUKUAN_PROTECT			538								//设置取款保护
#define SUB_GR_SET_QUKUAN_PROTECT_RET		539								//返回

struct	CMD_GP_SetQukuanProtect
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szQukuanPass[33];						//密码
	TCHAR							szQukuanAsk[33];						//问题
	TCHAR							szQukuanAnswer[33];						//答案
};
struct CMD_GR_SetQukuanProtect_RET
{
	int							lResult;
};

#define SUB_GP_SET_QUKUAN_ZHANGHAO			540								//设置取款保护
#define SUB_GR_SET_QUKUAN_ZHANGHAO_RET		541								//返回

struct	CMD_GP_SetQukuanZhanghao
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szKaihuYinghang[33];						//密码
	TCHAR							szKaihuRen[33];						//问题
	TCHAR							szYinhangZhanghu[33];						//答案
	TCHAR							szQukuanPass[33];						//答案
};
struct CMD_GR_SetQukuanZhanghao_RET
{
	int							lResult;
};

struct CMD_GR_QueryYinhang_RET
{
	LONG							lResultCode;						//操作代码
	TCHAR							szYinHangName[128];					//成功消息
};

#define SUB_GP_QUERY_MY_YINHANG			415								//查询自己的提款账号设置
#define SUB_GR_QUERY_MY_YINHANG_RET		416								//返回

struct	CMD_GP_QueryMyYinHang
{
	DWORD							dwUserID;							//用户ID
};
struct CMD_GR_QueryMyYinHang_RET
{
	int							lResult;
	TCHAR						szKaihuYinghang[33];
	TCHAR						szKaihuRen[33];
	TCHAR						szYinhangZhanghu[33];				
};

#define SUB_GP_QUERY_MY_PROTECT			423								//查询自己的设置
#define SUB_GR_QUERY_MY_PROTECT_RET		424								//返回

struct	CMD_GP_QueryMyProTect
{
	DWORD							dwUserID;							//用户ID
};
struct CMD_GR_QueryMyProTect_RET
{
	int							lResult;
	TCHAR						szQuestion[33];
	TCHAR						szAnswer[33];
};

/////////////////会员//////////////////////////////////////////////////////////////////
//个人账户
#define SUB_GP_GET_HYXX_LOG_COUNT			542								//查询会员信息日志数
#define SUB_GR_GET_HYXX_LOG_COUNT_RET		543							//查询会员信息日志数返回

//查询投注日志数
struct CMD_GP_GetHYXXLogCount
{
	int								dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	bool							bTime;								//通过时间查询
 	TCHAR							szTimeStart[30];					//起始时间
 	TCHAR							szTimeEnd[30];						//结束时间
	int								nOnline;							//在线玩家
	int								nYue;								//有额玩家
};
//返回
struct CMD_GR_GetHYXXLogCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_HYXX_LOG			544								//查询盈亏日志
#define SUB_GP_GET_HYXX_LOG_RET		545								//返回


#define SUB_GP_GET_HYXX_LIST			431								//查询会员信息
#define SUB_GP_GET_HYXX_LIST_RET		432								//返回

struct	CMD_GP_GetHYXXLog
{
	int								dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	int								n_t_sorttype;						//排序
	int								nPage;								//页
	int								nSize;								//页的大小
	int								bByTime;							//通过时间
	int								nOnline;							//在线玩家
	int								nYue;								//有额玩家
	TCHAR						szTimeStart[30];					//起始时间
	TCHAR						szTimeEnd[30];						//结束时间

};

struct CMD_GR_GetHYXXLogRet
{
	int								n_t_id;
	TCHAR							s_t_account[32];
	SYSTEMTIME						n_t_time;
	int								n_t_type;
	DOUBLE							f_t_fandian;
	DOUBLE							f_t_yue;
	int								n_t_online;
	TCHAR							s_t_qq[11];

};
#define SUB_GP_GET_HYXX_LOG_BY_ID			546								//查询会员信息日志
// #define SUB_GP_GET_HYXX_LOG_BY_ID_RET		547								//返回

struct	CMD_GP_GetHYXXLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
};
#define SUB_GP_GET_HYXX_LOG_BY_ACT			547								//查询会员信息日志

struct	CMD_GP_GetHYXXLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};


#define SUB_GP_XG_HY_FANDIAN			548								//修改QQ
#define SUB_GR_XG_HY_FANDIAN_RET		549								//返回

struct	CMD_GP_XGHYFandian
{
	int								dwUserID;							//用户ID
	int								n_xiaji_id;							//下级ID
	DOUBLE							f_Fandian;							//返点
};
struct CMD_GP_XGHYFandian_RET
{
	LONG							lResult;
	DOUBLE							f_Fandian;
	TCHAR							s_Account[33];
};


#define SUB_GP_SW_DAILI				911								//设为代理
#define SUB_GR_SW_DAILI_RET			912								//返回

struct	CMD_GP_SWDaili
{
	DWORD							dwUserID;							//用户ID
	DWORD							nAgentID;							//代理ID
	int 							nDaili;							//是否代理
};
struct CMD_GP_SWDaili_RET
{
	LONG							lResult;
};
///////////////////////会员转账///////////////////////////////////////////////////////////
#define SUB_GP_HYXX_ZHUANZHANG				913								//设为代理
#define SUB_GR_HYXX_ZHUANZHANG_RET			914								//返回

struct	CMD_GP_HYXX_ZhuanZhang
{
	int								n_t_user_id;							//自己ID
	int								n_t_target_id;							//目标ID
	DOUBLE 							f_t_jine;								//金额
	TCHAR							s_t_password[126];							//密码
};
struct CMD_GP_HYXX_ZhuanZhang_RET
{
	int							lResult;
};

#define SUB_GP_GET_Peie			915								//获取配额
#define SUB_GR_GET_Peie_RET		916								//返回

struct CMD_GetPeie
{
	int								n_t_userid;						//ID
};
struct CMD_GetPeieRet
{
	int								n_t_peie_1;					//3.0总配额
	int								n_t_peie_2;					//2.8总配额
	int								n_t_peie_3;					//2.7总配额
	int								n_t_peie_s_1;				//3.0已用配额
	int								n_t_peie_s_2;				//2.8已用配额
	int								n_t_peie_s_3;				//2.7已用配额
};
#define SUB_GP_GET_LHC_QIHAO			917								//获取六合彩期号
#define SUB_GR_GET_LHC_QIHAO_RET		918								//返回

// struct CMD_GetPeie
// {
// 	int								n_t_userid;						//ID
// };
struct CMD_GetLhcQihaoRet
{
	int								n_t_qihao;					//3.0总配额
	SYSTEMTIME						n_t_fdshijian;							//系统时间
};

#define SUB_GP_GET_XJ_Peie				921								//获取下级配额
#define SUB_GR_GET_XJ_Peie_RET			922								//返回

 struct CMD_GetXjPeie
 {
 	int								n_t_userid;						//ID
 };
struct CMD_GetXjPeieRet
{
	int								n_t_peie_1;					//3.0总配额
	int								n_t_peie_2;					//2.8总配额
	int								n_t_peie_3;					//2.7总配额
	int								n_t_peie_s_1;				//3.0已用配额
	int								n_t_peie_s_2;				//2.8已用配额
	int								n_t_peie_s_3;				//2.7已用配额
};
#define SUB_GP_SET_XJ_Peie				923								//设置下级配额
#define SUB_GR_SET_XJ_Peie_RET			924								//返回

 struct CMD_SetXjPeie
 {
 	int								n_t_userid;						//ID
	int								n_t_xj_id;						//下级ID
	int								n_t_peie1;
	int								n_t_peie2;
	int								n_t_peie3;
 };
struct CMD_SetXjPeieRet
{
	int								n_t_result;					//返回
	TCHAR							s_t_desc[126];				//描述
};


#define SUB_GP_KEFU_URL				925								//设置下级配额
#define SUB_GR_KEFU_URL_RET			926								//返回
struct CMD_GetKefuUrlRet
{
	int								n_t_result;					//返回
	TCHAR							s_t_desc[256];				//描述
};

//////////////////////////////////////////////////////////////////////////////////
#define SUB_GP_GET_REG_URL			550								//获取注册链接
#define SUB_GR_GET_REG_URL_RET		551								//返回

struct CMD_GetUrlFandian
{
	int								n_t_userid;						//ID
};
struct CMD_GetRegUrl
{
	TCHAR							s_reg_url[126];					//注册链接
	DOUBLE							f_t_fandian;
};

#define SUB_GP_ADD_HY				552								//添加下级
#define SUB_GR_ADD_HY_RET			553								//返回

struct CMD_GP_AddHuiYuan
{
	int								n_t_daili_id;
	int								n_type;
	DOUBLE							f_t_fandian;
	TCHAR							s_t_account[33];
	TCHAR							s_t_login_pass[33];
	TCHAR							s_t_qukuan_pass[33];
	TCHAR							s_t_qq[33];
};
struct CMD_GR_AddHuiYuanRet
{
	int								n_t_res;
	TCHAR							s_t_Desc[126];
};

#define SUB_GP_SET_WEB_FANDIAN				630								//设置网页注册返点
#define SUB_GR_SET_WEB_FANDIAN_RET			631								//返回

struct CMD_GP_SetWebFandian
{
	int								n_t_userid;
	DOUBLE							f_t_fandian;
};
struct CMD_GR_SetWebFandianRet
{
	int								n_t_res;
	DOUBLE							f_t_fandian;
};

//个人账户
#define SUB_GP_GET_XJTZH_LOG_COUNT			554								//查询下级投注日志数
#define SUB_GR_GET_XJTZH_LOG_COUNT_RET		555							//查询下级投注日志数返回

//查询投注日志数
struct CMD_GP_GetXJTZHLogCount
{
	int							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetXJTZHLogCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJTZH_LOG			556								//查询下级投注
#define SUB_GP_GET_XJTZH_LOG_RET		557								//返回

struct	CMD_GP_GetXJTZHLog
{
	int							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJTZHLogRet
{
	int								n_t_id;
	TCHAR							s_t_account[32];
	int								n_t_type;
	DOUBLE							f_t_touzhu_ze;
	DOUBLE							f_t_yingkui_ze;
};

#define SUB_GP_GET_XJTZH_LOG_BY_ID			558								//查询会员信息日志

struct	CMD_GP_GetXJTZHLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
};
#define SUB_GP_GET_XJTZH_LOG_BY_ACT			559								//查询会员信息日志

struct	CMD_GP_GetXJTZHLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};


//个人账户
#define SUB_GP_CHK_XJTZH_LOG_COUNT			560								//查询下级投注日志数
#define SUB_GR_CHK_XJTZH_LOG_COUNT_RET		561							//查询下级投注日志数返回

//查询投注日志数
struct CMD_GP_CHKXJTZHLogCount
{
	DWORD							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	int								n_xj_id;							//下级ID 
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_CHKXJTZHLogCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_CHK_XJTZH_LOG			562								//查询下级投注
#define SUB_GP_CHK_XJTZH_LOG_RET		563								//返回

struct	CMD_GP_CHKXJTZHLog
{
	DWORD							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_CHKXJTZHLogRet
{
	int								n_t_id;
	TCHAR							s_t_account[33];
	int								n_t_type;
	int								n_t_kind;
	TCHAR							s_t_qishu[30];
	int								n_t_zhushu;
	int								n_t_moshi;
	int								n_t_beishu;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	int								n_t_winzhushu;
	DOUBLE							f_t_yingkui;
	int								n_t_zhuihao;
};

#define SUB_GP_CHK_XJTZH_LOG_BY_ID			564								//查询会员信息日志

struct	CMD_GP_CHKXJTZHLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
	int								nPage;								//页
	int								nSize;								//页的大小

};
#define SUB_GP_CHK_XJTZH_LOG_BY_ACT			565								//查询会员信息日志

struct	CMD_GP_CHKXJTZHLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};

//个人账户
#define SUB_GP_GET_HYSHJ			566								//查询会员数据
#define SUB_GR_GET_HYSHJ_RET		567							//查询会员数据返回

//查询投注日志数
struct CMD_GP_GetHyShj
{
	int							dwUserID;							//用户ID
	BYTE						cbOnlyToday;						//只看今天   0 不是    1 是
	TCHAR						szTimeStart[30];					//起始时间
	TCHAR						szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetHyShjRet
{
	int								n_t_xjrs;						//下级人数
	int								n_t_xjzx;						//下级在线
	DOUBLE							f_t_hyye;						//会员余额
	DOUBLE							f_t_jrfd;						//今日返点
	DOUBLE							f_t_jrchz;						//今日充值
	DOUBLE							f_t_jrqk;						//今日取款
	DOUBLE							f_t_jrtzh;						//今日投注
	DOUBLE							f_t_jrhd;						//今日活动
	int								n_t_jinrizhuce;					//今日注册
	DOUBLE							f_chedan_zonge;					//撤单总额
	DOUBLE							f_zhongjiang_zonge;				//中奖总额
	DOUBLE							f_wodefandian;					//我的返点-彩票
	DOUBLE							f_caipiao_yingkui;				//彩票盈亏
	DOUBLE							f_t_xianma;						//冼码
	DOUBLE							f_qp_fandian;					//棋牌返点
	DOUBLE							f_qp_yingkui;					//棋牌盈亏
	DOUBLE							f_my_qp_fandian;				//我的棋牌返点
	DOUBLE							f_touzhuyongjin;				//投注佣金
	DOUBLE							f_kuisunyongjin;				//亏损佣金
};

//个人账户
#define SUB_GP_GET_XJYK_LOG_COUNT			568								//查询下级盈亏信息日志数
#define SUB_GR_GET_XJYK_LOG_COUNT_RET		569							//查询下级盈亏信息日志数返回

//查询下级盈亏日志数
struct CMD_GP_GetXJYKLogCount
{
	DWORD							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	bool							bTime;								//通过时间查询
	int								nXiaJiID;							//下级ID
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
	int								nType;								//0:投注  1 返点  2下级返点  3 中奖 4 充值 5取款 6棋牌盈亏
};
//返回
struct CMD_GR_GetXJYKLogCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJYK_LOG			570								//查询下级盈亏
#define SUB_GP_GET_XJYK_LOG_RET		571								//返回

struct	CMD_GP_GetXJYKLog
{
	DWORD							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	int								n_sort_type;						//排序    0:团队充值(降)  1 团队充值(升)  2团队取款(降)  3 团队取款(升) 4 团队投注(降) 5团队投注(升) 6团队中奖(降) 7团队中奖(升)  8团队返点(降)  9团队返点(升)
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJYKLogRet
{
	int								n_t_id;
	int								n_t_userid;
	TCHAR							s_t_account[30];
	DOUBLE							f_t_yingkui;
	DOUBLE							f_t_at_yue;

	SYSTEMTIME						n_t_time;
	int								n_t_type;
	TCHAR							s_t_comment[126];

};

#define SUB_GP_GET_XJYK_LOG_BY_ID			572								//查询下级盈亏信息日志

struct	CMD_GP_GetXJYKLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
		int								nPage;								//页
	int								nSize;								//页的大小

};
#define SUB_GP_GET_XJYK_LOG_BY_ACT			573								//查询下级盈亏信息日志

struct	CMD_GP_GetXJYKLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};

//////////////////////////下级盈亏统计////////////////
//个人账户
#define SUB_GP_GET_XJYK_TJ_COUNT			574								//查询下级盈亏信息日志数
#define SUB_GR_GET_XJYK_TJ_COUNT_RET		575							//查询下级盈亏信息日志数返回

//查询下级盈亏日志数
struct CMD_GP_GetXJYKTjCount
{
	int							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetXJYKTjCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJYK_TJ			576							//查询下级盈亏
#define SUB_GP_GET_XJYK_TJ_RET		577								//返回

struct	CMD_GP_GetXJYKTj
{
	int							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	int								nPage;								//页
	int								nSize;								//页的大小
	int								n_sort_type;						//排序    0:投注  1 充值  2取款  3 中奖 4 盈亏
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJYKTjRet
{
	int								n_t_id;
	TCHAR							s_t_account[30];
	int								n_t_type;
	DOUBLE							f_t_chongzhi_ze;
	DOUBLE							f_t_qukuan_ze;
	DOUBLE							f_t_touzhu_ze;
	DOUBLE							f_t_zhongjiang_ze;
	DOUBLE							f_t_fandian_ze;
	DOUBLE							f_t_xjfandian_ze;
	DOUBLE							f_t_huodong_ze;
	DOUBLE							f_t_tuandui_ye;
//	DOUBLE							f_t_qipai_ze;
};
//////////////////////////下级游戏盈亏统计////////////////
//个人账户
#define SUB_GP_GET_XJYX_TJ_COUNT			419								//查询下级游戏盈亏信息日志数
#define SUB_GR_GET_XJYX_TJ_COUNT_RET		420							//查询下级游戏盈亏信息日志数返回

//查询下级盈亏日志数
struct CMD_GP_GetXJYXTjCount
{
	int							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetXJYXTjCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJYX_TJ			421							//查询下级盈亏
#define SUB_GP_GET_XJYX_TJ_RET		422								//返回

struct	CMD_GP_GetXJYXTj
{
	int							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJYXTjRet
{
	int								n_t_id;
	TCHAR							s_t_account[30];
	int								n_t_type;
	int								n_t_daili_id;

	DOUBLE							f_t_fandian_ze;
	DOUBLE							f_t_qipai_ze;
	DOUBLE							f_t_tuandui_ye;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SUB_GP_GET_XJYK_TJ_BY_ID			578								//查询下级盈亏信息日志

struct	CMD_GP_GetXJYKTjByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
};
#define SUB_GP_GET_XJYK_TJ_BY_ACT			579								//查询下级盈亏信息日志

struct	CMD_GP_GetXJYKTjByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};

//////////////////////////下级充值日志////////////////
//个人账户
#define SUB_GP_GET_XJCHZH_LOG_COUNT			580								//查询下级充值日志数
#define SUB_GR_GET_XJCHZH_LOG_COUNT_RET		581							//查询下级充值日志数返回

//查询下级盈亏日志数
struct CMD_GP_GetXjChzhLogCount
{
	DWORD							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetXjChzhLogCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJCHZH_LOG			582							//查询下级充值
#define SUB_GP_GET_XJCHZH_LOG_RET		583								//返回

struct	CMD_GP_GetXJCHZHLog
{
	DWORD							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJCHZHLogRet
{
	int								n_t_id;
	int								n_t_user_id;
	TCHAR							s_t_account[30];
	DOUBLE							f_t_jine;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	TCHAR							s_t_pingtai[30];
};

#define SUB_GP_GET_XJCHZH_LOG_BY_ID			584								//查询下级盈亏信息日志

struct	CMD_GP_GetXJCHZHLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
};
#define SUB_GP_GET_XJCHZH_LOG_BY_ACT			585								//查询下级盈亏信息日志

struct	CMD_GP_GetXJCHZHLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};
//////////////////////////下级提现日志////////////////
//个人账户
#define SUB_GP_GET_XJTX_LOG_COUNT			586								//查询下级充值日志数
#define SUB_GR_GET_XJTX_LOG_COUNT_RET		587							//查询下级充值日志数返回

//查询下级提现日志数
struct CMD_GP_GetXjTxLogCount
{
	DWORD							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetXjTxLogCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJTX_LOG			588							//查询下级充值
#define SUB_GP_GET_XJTX_LOG_RET		589								//返回

struct	CMD_GP_GetXJTxLog
{
	DWORD							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJTxLogRet
{
	int								n_t_id;
	int								n_t_user_id;
	TCHAR							s_t_account[30];
	SYSTEMTIME						n_t_time;
	TCHAR							s_t_yinhang[30];
	DOUBLE							f_t_jine;
	TCHAR							s_t_kaihuiren[30];
	int								n_t_state;
	TCHAR							s_t_note[30];
};

#define SUB_GP_GET_XJTX_LOG_BY_ID			590								//查询下级盈亏信息日志

struct	CMD_GP_GetXJTxLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
};
#define SUB_GP_GET_XJTX_LOG_BY_ACT			591								//查询下级盈亏信息日志

struct	CMD_GP_GetXJTxLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};

#define SUB_GP_CHONG_ZHI_TYPE					592					//获取充值信息
#define SUB_GP_CHONG_ZHI_TYPE_RET				593					//获取充值信息返回

struct CMD_GP_GetChongzhiType
{
	int								nUserID;					//玩家ID
	BYTE							cbType;						//类型
};

struct CMD_GP_GetChongzhiTypeRet
{
	TCHAR							s_t_yinhang[60];				//银行
	TCHAR							s_t_kaihuren[60];				//开户人
	TCHAR							s_t_zhanghao[60];				//账号
	TCHAR							s_t_web[60];					//网页
};
#define SUB_GP_GET_ALL_YINHANG_NAME					417					//获取银行
#define SUB_GP_GET_ALL_YINHANG_NAME_RET				418					//获取银行返回

// struct CMD_GP_GetAllYinHangName
// {
// 	int								nUserID;					//玩家ID
// 	BYTE							cbType;						//类型
// };

struct CMD_GP_GetAllYinHangNameRet
{
	TCHAR							s_t_yinhang[60];				//银行
	int								n_QiYong;						//是否启用
};

#define SUB_GP_QU_KUAN_INFO					594					//获取取款信息
#define SUB_GP_QU_KUAN_INFO_RET				595					//获取取款信息返回

struct CMD_GP_GetQukuanInfo
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_GetQukuanInfoRet
{
	DOUBLE							f_t_yue;						//余额
	TCHAR							s_t_yinhang[60];				//银行
	TCHAR							s_t_kaihuren[60];				//开户人
	TCHAR							s_t_zhanghao[60];				//账号
	BYTE							n_t_isvip;						//是否超级VIP
};

#define SUB_GP_DO_QU_KUAN					600					//取款信息
#define SUB_GP_DO_QU_KUAN_RET				601					//取款信息返回

struct CMD_GP_DoQukuan
{
	DWORD							dwUserID;						//用户ID
	DOUBLE							fJine;							//金额
	TCHAR							szQukuanPass[33];				//取款密码
};

struct CMD_GP_DoQukuanRet
{
	int								nResult;						//返回值
	TCHAR							szDesc[126];					//取款提示
};

#define SUB_GP_GET_LAST_YUE					596					//获取最新余额信息
#define SUB_GP_GET_LAST_YUE_RET				597					//获取最新余额信息返回

struct CMD_GP_GetLastYue
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_GetLastYueRet
{
	DOUBLE							f_t_yue;						//余额
	DOUBLE							f_t_dongjie;					//冻结
	DOUBLE							f_t_score;						//游戏币
};

#define SUB_GP_QUIT_GAME					409					//获取最新余额信息
#define SUB_GP_QUIT_GAME_RET				410					//获取最新余额信息返回

struct CMD_GP_QuitGame
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_QuitGameRet
{
	int								nResult;
};

///////////////////////////////大礼包/////////////////////////////////////////////
#define SUB_GP_GET_DA_LI_BAO				602					//获取大礼包 信息
#define SUB_GP_GET_DA_LI_BAO_RET			603					//获取大礼包 信息返回

struct CMD_GP_GetDaLiBao
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_GetDaLiBaoRet
{
	int							f_t_jine;						//余额
};
#define SUB_GP_CAN_DA_LI_BAO				604					//获取大礼包 信息
#define SUB_GP_CAN_DA_LI_BAO_RET			605					//获取大礼包 信息返回

struct CMD_GP_CanDaLiBao
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_CanDaLiBaoRet
{
	DOUBLE						f_t_xiaofei;
	DOUBLE						f_t_chongzhi;
	DOUBLE						f_t_yue;
	int							n_t_got;						//余额
};
//////////////////////////////////////砸金蛋////////////////////////////////////////
#define SUB_GP_CAN_ZA_JIN_DAN				606					//获取大礼包 信息
#define SUB_GP_CAN_ZA_JIN_DAN_RET			607					//获取大礼包 信息返回

struct CMD_GP_CanZaJinDan
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_CanZaJinDanRet
{
	DOUBLE						f_t_xiaofei;
	int							n_t_got;						//余额
};

#define SUB_GP_GET_ZA_JIN_DAN				608					//获取大礼包 信息
#define SUB_GP_GET_ZA_JIN_DAN_RET			609					//获取大礼包 信息返回

struct CMD_GP_GetZaJinDan
{
	DWORD							dwUserID;						//用户ID
	int								nType;						//金蛋type
};

struct CMD_GP_GetZaJinDanRet
{
	int							f_t_jine;						//余额
};
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////幸运转转////////////////////////////////////////
#define SUB_GP_CAN_LUCKY_ZHUAN				610					//获取幸运转转 信息
#define SUB_GP_CAN_LUCKY_ZHUAN_RET			611					//获取幸运转转 信息返回

struct CMD_GP_CanLuckyZhuan
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_CanLuckyZhuanRet
{
	DOUBLE						f_t_xiaofei;
};

#define SUB_GP_GET_LUCKY_ZHUAN				612					//获取幸运转转 信息
#define SUB_GP_GET_LUCKY_ZHUAN_RET			613					//获取幸运转转信息返回

struct CMD_GP_GetLuckyZhuan
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_GetLuckyZhuanRet
{
	int							n_t_got;
	DOUBLE							f_t_jine;						//余额
};
//////////////////////////////////////////////////////////////////////////////////
#define SUB_GP_GET_USER_QIANDAO				614					//签到信息
#define SUB_GP_GET_USER_QIANDAO_RET			615					//签到返回

struct CMD_GP_UserQiandao
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_UserQiandaoRet
{
	int							n_t_res;
};
#define SUB_GP_GET_USER_HUANLESONG				616					//欢乐送信息
#define SUB_GP_GET_USER_HUANLESONG_RET			617					//欢乐送返回

struct CMD_GP_GetUserHuanlesong
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_GetUserHuanlesongRet
{
	int							nRes;
	DOUBLE						f_t_xiaofei;
	DOUBLE						f_t_prize;
	int							n_t_count;
	int							n_t_XXcount;
};

#define SUB_GP_GET_LING_JIANG				618					//获取领奖信息
#define SUB_GP_GET_LING_JIANG_RET			619					//获取领奖信息返回

struct CMD_GP_GetLingJiang
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_GetLingJiangRet
{
	int							n_t_got;
	DOUBLE							f_t_jine;						//余额
};

#define SUB_GP_GET_TOU_ZHU_XX				620					//获取投注信息
#define SUB_GP_GET_TOU_ZHU_XX_RET			621					//获取投注信息返回

struct CMD_GP_GetTouzhu
{
	int							n_t_id;						//ID
};

struct CMD_GP_GetTouzhuRet
{
	TCHAR						s_t_wanjiazhanghao[30];				//账户
	int							n_t_dingdanhao;						//订单号
	TCHAR						s_t_qishu[20];						//期数
	int							n_t_gametype;						//游戏类别
	int							n_t_gamekind;						//玩法
	char						s_t_touzhuhaoma[13312];				//投注号码
	TCHAR						s_t_kaijianghaoma[45];				//开奖号码
	SYSTEMTIME					n_t_time;
	int							n_t_state;
	int							n_t_zhushu;
	int							n_t_moshi;
	int							n_t_beishu;
	int							n_t_winzhushu;
	DOUBLE						f_t_yingkui;
	DOUBLE						f_t_danzhujine;
	DOUBLE						f_t_fandian;
	int							n_t_End;
	int							nHaoMaLen;
	int							n_Index;
};

#define SUB_GP_GET_NEWS				622					//获取新闻
#define SUB_GP_GET_NEWS_RET			623					//获取新闻返回

struct CMD_GP_GetNews
{
	int							n_t_id;						//ID
	int							n_t_ccount;			//数量
};

struct CMD_GP_GetNewsRet
{
	TCHAR						s_t_news[512];				//新闻

};

#define SUB_GP_ZHUANHUAN				624					//转换
#define SUB_GP_ZHUANHUAN_RET			625					//转换返回

struct CMD_GP_ZhuanHuan
{
	int							n_t_userid;					//用户ID
	int							n_t_type;					//类型   1 转金币   2 转彩金
	DOUBLE						f_t_jine;					//金额
};

struct CMD_GP_ZhuanHuanRet
{
	int							nResult;
	DOUBLE						f_t_yue;
	DOUBLE						f_t_score;
};
///////////////////////////////////////////////////////////////////////////////////
//获取更多开奖结果
#define SUB_GP_GET_MORE_RECORD				632					//
#define SUB_GP_GET_MORE_RECORD_RET			633					//返回

struct CMD_GP_GetMoreRecord
{
	int							n_t_type;					//类型   1 转金币   2 转彩金
};

struct CMD_GP_GetMoreRecordRet
{
	WORD							wKindID;							//操作代码
	TCHAR							wPeriod[30];							//期
	TCHAR							szLotNum[45];						//开奖数据
	TCHAR							szShijian[30];							//时间
};

#define SUB_MB_GET_TOU_ZHU_XX				634					//获取投注信息
#define SUB_MB_GET_TOU_ZHU_XX_RET			635					//获取投注信息返回

struct CMD_MB_GetTouzhu
{
	int							n_t_id;						//ID
};

struct CMD_MB_GetTouzhuRet
{
	TCHAR						s_t_wanjiazhanghao[30];				//账户
	int							n_t_dingdanhao;						//订单号
	TCHAR						s_t_qishu[20];						//期数
	int							n_t_gametype;						//游戏类别
	int							n_t_gamekind;						//玩法
	char						s_t_touzhuhaoma[13312];				//投注号码
	TCHAR						s_t_kaijianghaoma[45];				//开奖号码
	SYSTEMTIME					n_t_time;
	int							n_t_state;
	int							n_t_zhushu;
	int							n_t_moshi;
	int							n_t_beishu;
	int							n_t_winzhushu;
	DOUBLE						f_t_yingkui;
	DOUBLE						f_t_danzhujine;
	DOUBLE						f_t_fandian;
	int							n_t_End;
	int							nHaoMaLen;
	int							n_Index;
};
///////////////////////////////////////////////////////////////////////////////////
//获取代理回馈
#define SUB_GP_GET_DAILI_HUIKUI				638					//
#define SUB_GP_GET_DAILI_HUIKUI_RET			639					//返回

struct CMD_GP_GetDailiHuikui
{
	int							n_t_userid;					//用户ID
};

struct CMD_GP_GetDailiHuikuiRet
{
	BYTE						cbNewRegUser;					//当天
	DOUBLE						f_t_touzhu;						//投注
	DOUBLE						f_t_chongzhi;					//充值
};

//代理领奖
#define SUB_GP_DAILI_LJ						640					//
#define SUB_GP_DAILI_LJ_RET					641					//返回

struct CMD_GP_DailiLingjiang
{
	int							n_t_userid;					//用户ID
};

struct CMD_GP_DailiLingjiangRet
{
	BYTE						n_t_state;					//所有状态 0 未  1已领
	DOUBLE						f_t_jine;						//金额
};
///////////////////////////////////////////////////////////////////////////
//修改性别
#define SUB_GP_ALTER_GENDER					642					//
#define SUB_GP_ALTER_GENDER_RET				643					//返回

struct CMD_GP_AlterGender
{
	int								n_t_userid;					//用户ID
	BYTE 							cbGender;					//性别
};

struct CMD_GP_AlterGenderRet
{
	BYTE						n_t_return;					//所有状态 0 未  1已领
};

///////////////////////////////////////////////////////////////////////////////////
//修改密码
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//////////////////////////////////////////////////////////////////////////////////
struct CMD_GP_GetCpUser
{
	int								n_t_userid;
};
//返回
struct CMD_GR_GetCpUserInfoRet //获取彩票玩家信息
{
	int								n_t_userid;
	TCHAR							s_t_account[30];
	int								n_t_type;
	DOUBLE							f_t_fandian;
	TCHAR							s_t_qq[11];
	int								n_t_daili_id;
	int								n_t_online;
	DOUBLE							f_t_yue;
	DOUBLE							f_t_dongjie;
	int								n_t_ban;

};

struct CMD_GP_GetUserFandian
{
	int								n_t_userid;
	int								n_t_type_id;
	int								n_t_kind_id;
};
//返回
struct CMD_GR_GetUserFandianRet //获取彩票玩家返点
{
	int								n_t_kindid;
	DOUBLE							f_t_bonus;
	DOUBLE							f_t_bonusPercent;
	DOUBLE							f_t_fandian;
};
struct CMD_GR_GetMapBonusRet
{
	int								n_t_type_id;
	int								n_t_kind_id;
	DOUBLE							f_t_bonus;
};

struct CMD_GP_SetUserBonus
{
	int								n_t_userid;
	int								n_t_type_id;
	int								n_t_kind_id;
	DOUBLE							f_t_bonus;
	DOUBLE							f_bonuspercent;
};
//返回
struct CMD_GR_SetUserBonusRet //获取彩票玩家返点
{
	int								n_t_result;
};

//查询游戏开奖结果
struct CMD_GP_QueryGameResult
{
	int							wKindID;							//需要查询的游戏ID
};
//返回查询系统时间
struct CMD_GR_GetSystemTime
{
	SYSTEMTIME						ServerTime;							//系统时间
};
//查询充值提示
struct CMD_GP_GetChonzhiTishi
{
	DWORD							dwUserID;							//用户ID 
};
//充值提示返回
struct CMD_GR_ChongzhiTishiRet
{
	DOUBLE							fJine;
	int								nState;
};
//查询提现提示
struct CMD_GP_GetTixianTishi
{
	DWORD							dwUserID;							//用户ID 
};
//提现提示返回
struct CMD_GR_TixianTishiRet
{
	DOUBLE							fJine;
	int								nState;
};

//投注提示
struct CMD_GP_GetTouzhuTishi
{
	DWORD							dwUserID;							//用户ID 
};
//提现提示返回
struct CMD_GR_TouzhuTishiRet
{
	DOUBLE								nWinZhushu;
	int								nState;
	TCHAR							szQishu[30];
	int								nType;
	DOUBLE							f_yingkui;
	DOUBLE								nZhushu;
};
/////////////////////////////////////////////////////////////////////////////////
//用户头像
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//头像标识
};

//修改头像
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};
/////////////////////////////////////////////////////////////////////////////////////
//重庆时时彩
//下注
struct CMD_GP_TouZhuCQSSC
{
	DWORD							dwUserID;							//用户ID
	char							strQishu[20];						//期数
	int								nGameType;							//游戏类别
	int								nGameKind;							//玩法种类
	char							strHaoma[15360];					//下注号码
	int								nHaoMaLen;							//号码长度
	int								nZhushu;							//注数
	int								nBeitou;							//倍数
	int								nMoshi;								//模式  元角分
	int								nSign;								//投注标志
	int								bZhuihao;							//是否追号
	WORD							nIndex;								//index
	WORD							cbPlatform;							//平台
};
struct CMD_GP_TouZhuCQSSC_Dan
{
	DWORD							dwUserID;							//用户ID
	char							strQishu[20];						//期数
	int								nGameType;							//游戏类别
	int								nGameKind;							//玩法种类
	char							strHaoma[15360];					//下注号码
	int								nHaoMaLen;							//号码长度
	int								nZhushu;							//注数
	int								nBeitou;							//倍数
	int								nMoshi;								//模式  元角分
	int								nSign;								//投注标志
	int								nEnd;								//是否发送结束
	int								bZhuihao;							//是否追号
	int								nIndex;								//index
};
struct CMD_GP_TouZhuCQSSC_Zhuihao
{
	DWORD							dwUserID;							//用户ID
	char							strQishu[50][20];						//期数
	int								nGameType;							//游戏类别
	int								nGameKind;							//玩法种类
	char							strHaoma[14336];					//下注号码
	int								nHaoMaLen;							//号码长度
	int								nZhushu;							//注数
	int								nBeitou[50];							//倍数
	int								nMoshi;								//模式  元角分
	int								nSign;								//投注标志
	int								nEnd;								//是否发送结束
	int								bZhuihao;							//是否追号
	int								nIndex;								//index
};
//////////////////////////////////////////////////////////////////////////////////

//绑定机器
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//////////////////////////////////////////////////////////////////////////////////

//个人资料
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//用户 I D
};

//查询信息
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//用户 I D
};

//修改资料
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//用户昵称
#define DTP_GP_UI_USER_NOTE			2									//用户说明
#define DTP_GP_UI_QQ				4									//Q Q 号码
#define DTP_GP_UI_EMAIL				5									//电子邮件
#define DTP_GP_UI_SEAT_PHONE		6									//固定电话
#define DTP_GP_UI_MOBILE_PHONE		7									//移动电话
#define DTP_GP_UI_COMPELLATION		8									//真实名字
#define DTP_GP_UI_DWELLING_PLACE	9									//联系地址

//////////////////////////////////////////////////////////////////////////////////

//银行资料
struct CMD_GP_UserInsureInfo
{
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//存入金币
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入金币
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账金币
struct CMD_GP_UserTransferScore
{
	DWORD                           dwUserID;                           //用户 I D
	SCORE							lTransferScore;						//转账金币
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
	TCHAR                           szLogonPass[LEN_PASSWORD];          //登录密码
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//提取结果
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//查询银行
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
};

//////////////////////////////////////////////////////////////////////////////////

//操作失败
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//成功消息
};

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_GP_C_SearchCorrespond
{
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//登录命令
#define MDM_MB_LOGON				100									//广场登录

//登录模式
#define SUB_MB_LOGON_ACCOUNTS		1									//帐号登录
#define SUB_MB_REGISTER_ACCOUNTS	2									//注册帐号

//登录结果
#define SUB_MB_LOGON_SUCCESS		100									//登录成功
#define SUB_MB_LOGON_FAILURE		101									//登录失败

//升级提示
#define SUB_MB_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////

//帐号登录
struct CMD_MB_LogonAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//注册帐号
struct CMD_MB_RegisterAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//登录成功
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwExperience;						//经验数值
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//登录失败
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//升级提示
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};
//开奖查询结果
struct CMD_GP_QueryLotResult
{
	WORD							wKindID;							//操作代码
	TCHAR							wPeriod[30];							//期
	TCHAR							szLotNum[45];						//开奖数据
	TCHAR							szShijian[30];							//时间
};
struct CMD_GR_TouzhuRes
{
	LONG							lResult;							//返回值
	TCHAR							szDesc[126];						//返回描述
	int								nSign;								//标志

};
//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_MB_SERVER_LIST			101									//列表信息

//列表信息
#define SUB_MB_LIST_KIND			100									//种类列表
#define SUB_MB_LIST_SERVER			101									//房间列表
#define SUB_MB_LIST_FINISH			200									//列表完成

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif