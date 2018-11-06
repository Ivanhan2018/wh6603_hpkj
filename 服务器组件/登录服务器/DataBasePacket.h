#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//登录命令
#define	DBR_GP_LOGON_ACCOUNTS		1									//帐号登录
#define DBR_GP_REGISTER_ACCOUNTS	2									//注册帐号

//账号服务
#define DBR_GP_MODIFY_MACHINE		10									//修改机器
#define DBR_GP_MODIFY_LOGON_PASS	11									//修改密码
#define DBR_GP_MODIFY_INSURE_PASS	12									//修改密码
#define DBR_GP_MODIFY_INDIVIDUAL	13									//修改资料
#define DBR_GP_MODIFY_SYSTEM_FACE	14									//修改头像

//银行命令
#define DBR_GP_USER_SAVE_SCORE		20									//存入金币
#define DBR_GP_USER_TAKE_SCORE		21									//提取金币
#define DBR_GP_USER_TRANS_SCORE     22                                  //转账金币 

//查询命令
#define DBR_GP_QUERY_INDIVIDUAL		30									//查询资料
#define DBR_GP_QUERY_INSURE_INFO	31									//查询银行

//系统命令
#define DBR_GP_LOAD_GAME_LIST		40									//加载列表
#define DBR_GP_ONLINE_COUNT_INFO	41									//在线信息
#define DBR_GP_LOAD_VIRTUAL_COUNT	42									//虚拟人数

//开奖查询
#define DBR_GP_QUERY_RESULT			50									//开奖查询
#define DBO_GP_QUERY_RESULT			51									//开奖查询

//设置网页注册返点
#define DBR_GP_SET_WEB_FANDIAN			284									//设置网页返点
#define DBO_GP_SET_WEB_FANDIAN_RESULT	285									//设置网页返点返回
//////////////////////////////////////////////////////////////////////////////////
#define DBR_GP_TOUZHU_CQSSC			52									//重庆时时彩下注
#define DBR_GP_TOUZHU_CQSSC_DAN		283									//重庆时时彩下注单式
#define DBO_GP_TOUZHU_CQSSC_RET		53									//下注返回
#define DBO_GP_TOUZHU_CONTINUE_RET		286									//下注返回
#define DBR_GP_TOUZHU_CQSSC_ZHUIHAO		287									//重庆时时彩下注单式

#define DBR_GP_TOUZHU_LOG_COUNT		54									//查询投注记录数量
#define DBO_GP_TOUZHU_LOG_COUNT_RET	55									//查询投注记录数量返回

#define DBR_GP_TOUZHU_LOG			56									//查询投注记录
#define DBO_GP_TOUZHU_LOG_RET		57									//查询投注记录返回

#define DBR_MB_TOUZHU_LOG			320									//查询投注记录
#define DBO_MB_TOUZHU_LOG_RET		321									//查询投注记录返回

#define DBR_GP_CANCEL_TOUZHU			58									//撤单
#define DBO_GP_CANCEL_TOUZHU_RET		59									//撤单返回

#define DBR_GP_CHONGZHI_TISHI 			60									//充值提示
#define DBO_GP_CHONGZHI_TISHI_RET		61									//充值提示返回

#define DBR_GP_TIXIAN_TISHI 			62									//提现提示
#define DBO_GP_TIXIAN_TISHI_RET			63									//提现提示返回

#define DBR_GP_TOUZHU_TISHI 			64									//投注提示
#define DBO_GP_TOUZHU_TISHI_RET			65									//投注提示返回

#define DBR_GP_TIXIAN_LOG_COUNT			66									//查询提现记录数量
#define DBO_GP_TIXIAN_LOG_COUNT_RET		67									//查询提现记录数量返回

#define DBR_GP_TIXIAN_LOG			68									//查询提现记录
#define DBO_GP_TIXIAN_LOG_RET		69									//查询提现记录返回

#define DBR_GP_CHONGZHI_LOG_COUNT			70									//查询充值记录数量
#define DBO_GP_CHONGZHI_LOG_COUNT_RET		71									//查询充值记录数量返回

#define DBR_GP_CHONGZHI_LOG			72									//查询提现记录
#define DBO_GP_CHONGZHI_LOG_RET		73									//查询提现记录返回

#define DBR_GP_YINGKUI_LOG_COUNT			75									//查询充值记录数量
#define DBO_GP_YINGKUI_LOG_COUNT_RET		76									//查询充值记录数量返回

#define DBR_GP_YINGKUI_LOG			77									//查询提现记录
#define DBO_GP_YINGKUI_LOG_RET		78									//查询提现记录返回

#define DBR_GP_GET_USER_INFO			79									//查询用户信息
#define DBO_GP_GET_USER_INFO_RET		80									//查询用户信息返回

#define DBR_GP_XG_QQ			81									//修改QQ
#define DBO_GP_XG_QQ_RET		82									//修改QQ

#define DBR_GP_XG_LOGIN_PASS			83									//修改登录密码
#define DBO_GP_XG_LOGIN_PASS_RET		84									//修改登录密码返回


#define DBR_GP_XG_QUKUAN_PASS			85									//修改取款密码
#define DBO_GP_XG_QUKUAN_PASS_RET		86									//修改取款密码返回


#define DBR_GP_SET_QUKUAN_PROTECT			87									//设置取款保护
#define DBO_GP_SET_QUKUAN_PROTECT_RET		88									//设置取款保护返回

#define DBR_GP_SET_QUKUAN_ZHANGHAO			89									//设置取款保护
#define DBO_GP_SET_QUKUAN_ZHANGHAO_RET		90									//设置取款保护返回

#define DBR_GP_QUERY_YINHANG			294									//获取银行
#define DBO_GP_QUERY_YINHANG_RET		295									//获取银行返回

#define DBR_GP_YINGKUI_MX_COUNT			290									//查询盈亏明细数量
#define DBO_GP_YINGKUI_MX_COUNT_RET		291									//查询盈亏明细数量返回

#define DBR_GP_QP_YINGKUI_COUNT			298									//查询棋牌盈亏数量
#define DBO_GP_QP_YINGKUI_COUNT_RET		299									//查询棋牌盈亏数量返回

#define DBR_GP_YINGKUI_MX			292									//查询盈亏明细
#define DBO_GP_YINGKUI_MX_RET		293									//查询盈亏明细返回

#define DBR_GP_QP_YINGKUI			300									//查询棋牌盈亏明细
#define DBO_GP_QP_YINGKUI_RET		301									//查询棋牌盈亏明细返回

#define DBR_GP_QUERY_MY_YINHANG		302									//查询我的银行账户
#define DBO_GP_QUERY_MY_YINHANG_RET	303									//查询我的银行账户返回

#define DBR_GP_QUERY_YINHANG		304									//查询我的银行账户
#define DBO_GP_QUERY_YINHANG_RET	305									//查询我的银行账户返回

#define DBR_GP_QUERY_MY_PROTECT		310									//查询我的银行账户
#define DBO_GP_QUERY_MY_PROTECT_RET	311									//查询我的银行账户返回

#define DBR_GP_HYXX_LIST			312									//查询会员信息
#define DBO_GP_HYXX_LIST_RET		313									//查询会员信息返回
#define DBR_GP_ALTER_GENDER			330									//修改性别
#define DBO_GP_ALTER_GENDER_RET		331									//修改性别返回

////////////////////////会员//////////////////////////////////////////////////////////////////////////
#define DBR_GP_HYXX_LOG_COUNT			91									//查询会员信息数量
#define DBO_GP_HYXX_LOG_COUNT_RET		92									//查询会员记录数量返回

#define DBR_GP_HYXX_LOG			93									//查询会员信息数量
#define DBO_GP_HYXX_LOG_RET		94									//查询会员记录数量返回
#define DBR_GP_HYXX_LOG_BY_ID	95									//查询会员信息
#define DBR_GP_HYXX_LOG_BY_ACT	96									//查询会员信息

#define DBR_GP_XGHY_FANDIAN			97									//修改会员返点
#define DBO_GP_XGHY_FANDIAN_RET		98									//修改会员返点返回

#define DBR_GP_SW_DAILI			296									//设为代理
#define DBO_GP_SW_DAILI_RET		297									//设为代理返回

#define DBR_GP_HYXX_ZHUANZHANG			326									//会员信息转账
#define DBO_GP_HYXX_ZHUANZHANG_RET		327									//会员信息转账返回

#define DBR_GP_HYXX_GET_XJPEIE			340									//获取下级配额
#define DBO_GP_HYXX_GET_XJPEIE_RET		341									//获取下级配额返回

#define DBR_GP_HYXX_SET_XJPEIE			342									//设置下级配额
#define DBO_GP_HYXX_SET_XJPEIE_RET		343									//设置下级配额返回

#define DBR_GP_GET_KEFU_URL				344
#define DBO_GP_GET_KEFU_URL_RET			345							//获取客服链接
#define DBR_GP_GET_MY_MAP_BONUS			328									//获取个人bonus
#define DBO_GP_GET_MY_MAP_BONUS_RET		329									//获取个人bonus返回

#define DBR_GP_GET_Peie			334								//获取配额
#define DBO_GR_GET_Peie_RET		335								//返回

#define DBR_GP_GET_LHC_QIHAO			336								//获取六合彩期号
#define DBO_GR_GET_LHC_QIHAO_RET		337								//返回

#define DBR_GP_LOCK_MACHINE				338								//锁定机器
#define DBO_GR_LOCK_MACHINE_RET			339								//返回

#define DBR_GP_GET_REG_URL			99									//获取注册链接
#define DBO_GP_GET_REG_URL_RET		200									//获取注册链接返回
#define DBR_GP_ADD_HY			201									//添加下级
#define DBO_GP_ADD_HY_RET		202									//添加下级返回

#define DBR_GP_XJTZH_LOG_COUNT			203									//查询下级投注记录数量
#define DBO_GP_XJTZH_LOG_COUNT_RET		204									//查询下级投注数量返回

#define DBR_GP_XJTZH_LOG			205									//查询下级投注记录
#define DBO_GP_XJTZH_LOG_RET		206									//查询下级投注返回
#define DBR_GP_XJTZH_LOG_BY_ID			207									//查询下级投注记录通过ID
#define DBR_GP_XJTZH_LOG_BY_ACT			208									//查询下级投注记录通过账户

#define DBR_GP_CHKXJTZH_LOG_COUNT			209									//查询下级投注记录数量
#define DBO_GP_CHKXJTZH_LOG_COUNT_RET		210									//查询下级投注数量返回
#define DBR_GP_CHKXJTZH_LOG					211									//查询下级投注记录
#define DBO_GP_CHKXJTZH_LOG_RET				212									//查询下级投注返回
#define DBR_GP_CHKXJTZH_LOG_BY_ID			213									//查询下级投注记录通过ID
#define DBR_GP_CHKXJTZH_LOG_BY_ACT			214									//查询下级投注记录通过账户

#define DBR_GP_GET_HYSHJ			215									//获取会员数据
#define DBR_GP_GET_HYSHJ_RET		216									//获取会员数据返回

#define DBR_GP_XJYK_LOG_COUNT			217									//查询下级投注记录数量
#define DBO_GP_XJYK_LOG_COUNT_RET		218									//查询下级投注数量返回

#define DBR_GP_GET_XJYK_LOG			219									//获取下家盈亏
#define DBR_GP_GET_XJYK_LOG_RET		220									//获取下家盈亏返回
#define DBR_GP_XJYK_LOG_BY_ID			221									//查询下级投注记录通过ID
#define DBR_GP_XJYK_LOG_BY_ACT			222								//查询下级投注记录通过账户

#define DBR_GP_XJYK_TJ_COUNT			223									//查询下级盈亏统计数量
#define DBO_GP_XJYK_TJ_COUNT_RET		224									//查询下级盈亏统计数量返回

#define DBR_GP_GET_XJYK_TJ			225								//获取下家盈亏
#define DBR_GP_GET_XJYK_TJ_RET		226									//获取下家盈亏返回
#define DBR_GP_XJYK_TJ_BY_ID			227									//查询下级投注记录通过ID
#define DBR_GP_XJYK_TJ_BY_ACT			228								//查询下级投注记录通过账户

#define DBR_GP_XJCHZH_LOG_COUNT			229									//查询下级充值数量
#define DBO_GP_XJCHZH_LOG_COUNT_RET		230									//查询下级充值数量返回

#define DBR_GP_GET_XJCHZH_LOG			231									//获取下家充值
#define DBR_GP_GET_XJCHZH_LOG_RET		232									//获取下家充值返回
#define DBR_GP_XJCHZH_LOG_BY_ID			233									//查询下级充值记录通过ID
#define DBR_GP_XJCHZH_LOG_BY_ACT			234								//查询下级充值通过账户

#define DBR_GP_XJTX_LOG_COUNT			235									//查询下级提现数量
#define DBO_GP_XJTX_LOG_COUNT_RET		236									//查询下级提现数量返回

#define DBR_GP_GET_XJTX_LOG			237									//获取下家提现
#define DBR_GP_GET_XJTX_LOG_RET		238									//获取下家提现返回
#define DBR_GP_XJTX_LOG_BY_ID			239									//查询下级提现记录通过ID
#define DBR_GP_XJTX_LOG_BY_ACT			240								//查询下级提现通过账户

#define DBR_GP_GET_CHONGZHI_TYPE			241									//获取充值信息
#define DBO_GP_GET_CHONGZHI_TYPE_RET		242									//获取充值信息返回

#define DBR_GP_GET_QUKUAN_INFO				243									//获取取款信息
#define DBO_GP_GET_QUKUAN_INFO_RET			244									//获取取款信息返回

#define DBR_GP_DO_QUKUAN				245									//取款信息
#define DBO_GP_DO_QUKUAN_RET			246									//取款信息返回

#define DBR_GP_GET_YUE_INFO				247									//获取余额信息
#define DBO_GP_GET_YUE_INFO_RET			248									//获取余额信息返回

#define DBR_GP_QUIT_GAME				288									//退出游戏
#define DBO_GP_QUIT_GAME_RET			289									//退出游戏返回

#define DBR_GP_GET_CP_USER_INFO				249									//获取玩家信息
#define DBO_GP_GET_CP_USER_INFO_RET			250									//获取玩家信息返回

#define DBR_GP_GET_USER_FANDIAN				251									//获取玩家返点信息
#define DBO_GP_GET_USER_FANDIAN_RET			252									//获取玩家返点信息返回
#define DBR_GP_SET_USER_BONUS				253									//设置玩家返点信息
#define DBO_GP_SET_USER_BONUS_RET			254									//设置玩家返点信息返回

#define DBR_GP_GET_MAP_BONUS				255									//获取MAPBONUS
#define DBR_GP_GET_MAP_BONUS_RET			256									//获取MAPBONUS返回

#define DBR_GP_GET_DA_LI_BAO				257									//获取大礼包
#define DBR_GP_GET_DA_LI_BAO_RET			258									//获取大礼包返回

#define DBR_GP_CAN_DA_LI_BAO				259									//获取大礼包资格
#define DBR_GP_CAN_DA_LI_BAO_RET			260									//获取大礼包资格返回

#define DBR_GP_CAN_ZA_JIN_DAN				261									//获取砸金蛋资格
#define DBR_GP_CAN_ZA_JIN_DAN_RET			262									//获取砸金蛋资格返回

#define DBR_GP_GET_ZA_JIN_DAN				263									//获取砸金蛋
#define DBR_GP_GET_ZA_JIN_DAN_RET			264									//获取砸金蛋返回

#define DBR_GP_CAN_LUCKY_ZHUAN				265									//获取转盘资格
#define DBR_GP_CAN_LUCKY_ZHUAN_RET			266									//获取转盘资格返回

#define DBR_GP_GET_LUCKY_ZHUAN				267									//获取转盘
#define DBR_GP_GET_LUCKY_ZHUAN_RET			268									//获取转盘返回

#define DBR_GP_USER_QIANDAO				269									//签到
#define DBR_GP_USER_QIANDAO_RET			270									//签到返回

#define DBR_GP_USER_HUANLE_SONG				271									//欢乐送
#define DBR_GP_USER_HUANLE_SONG_RET			272									//欢乐送返回

#define DBR_GP_USER_LING_JIANG				273									//领奖
#define DBR_GP_USER_LING_JIANG_RET			274									//领奖返回

#define DBR_GP_GET_TOUZHU_XX			275									//投注详细信息
#define DBO_GP_GET_TOUZHU_XX_RET		276									//投注详细信息返回

#define DBR_GP_GET_NEWS					277									//新闻信息
#define DBO_GP_GET_NEWS_RET				278									//新闻信息返回

#define DBR_GP_ZHUAN_HUAN				279									//转换
#define DBO_GP_ZHUAN_HUAN_RET			280									//转换返回

#define DBR_GP_GET_SYS_TIME				281									//获取系统时间
#define DBO_GP_GET_SYS_TIME_RET			282									//获取系统时间返回

#define DBR_GP_XJYX_TJ_COUNT			306									//查询下级盈亏统计数量
#define DBO_GP_XJYX_TJ_COUNT_RET		307									//查询下级盈亏统计数量返回

#define DBR_GP_GET_XJYX_TJ			308								//获取下家游戏盈亏
#define DBR_GP_GET_XJYX_TJ_RET		309									//获取下家游戏盈亏返回

#define DBR_GP_GET_MORE_RECORD			314								//获取更多记录
#define DBR_GP_GET_MORE_RECORD_RET		315								//获取更多记录返回

#define DBR_GP_GET_WIN_USER				316									//获取中奖用户ID
#define DBO_GP_GET_WIN_USER_RET			317									//获取中奖用户ID返回

#define DBR_MB_GET_TOUZHU_XX			318									//投注详细信息
#define DBO_MB_GET_TOUZHU_XX_RET		319									//投注详细信息返回

#define DBR_MB_TOUZHU_CQSSC_ZHUIHAO		320									//手机追号

#define DBR_GP_GET_DAILI_HUIKUI				322					//
#define DBO_GP_GET_DAILI_HUIKUI_RET			323					//返回

//代理领奖
#define DBR_GP_DAILI_LJ						324					//
#define DBO_GP_DAILI_LJ_RET					325					//返回
struct DBO_GetLhcQihaoRet
{
	int								n_t_qihao;					//3.0总配额
	SYSTEMTIME						n_t_fdshijian;							//系统时间
};

//////////////////////////////////////////////////////////////////////////////////
//帐号登录
struct DBR_GP_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号注册
struct DBR_GP_RegisterAccounts
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	//EWIN网络添加
	TCHAR							szQQ[LEN_QQ];		//证件号码
	TCHAR							szEmain[LEN_EMAIL];	//真实名字
	TCHAR							szIPhone[LEN_MOBILE_PHONE];	//真实名字

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//修改机器
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改密码
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改头像
struct DBR_GP_ModifySystemFace
{
	//用户信息
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改资料
struct DBR_GP_ModifyIndividual
{
	//验证资料
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码

	//帐号资料
	BYTE							cbGender;							//用户性别
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//用户信息
	TCHAR							szUserNote[LEN_USER_NOTE];			//用户说明
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	//电话号码
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

	//联系资料
	TCHAR							szQQ[LEN_QQ];						//Q Q 号码
	TCHAR							szEMail[LEN_EMAIL];					//电子邮件
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
};

//查询资料
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//存入金币
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入金币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出金币
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取金币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账金币
struct DBR_GP_UserTransferScore
{
	DWORD                           dwUserID;                           //用户 I D
	SCORE							lTransferScore;						//转账金币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
	TCHAR                           szLogonPass[LEN_PASSWORD];          //登录密码
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};
//查询游戏开奖结果
struct DBR_GP_QueryGameResult
{
	int							wKindID;							//需要查询的游戏ID
};

struct DBR_GP_GetUserFandian
{
	int								n_t_userid;
	int								n_t_type_id;
	int								n_t_kind_id;
};
//返回
struct DBO_GR_GetUserFandianRet //获取彩票玩家返点
{
// 	int								n_t_userid;
// 	int								n_t_typeid;
	int								n_t_kindid;
	DOUBLE							f_t_bonus;
	DOUBLE							f_t_bonusPercent;
	DOUBLE							f_t_fandian;
};

struct DBR_GP_SetUserBonus
{
	int								n_t_userid;
	int								n_t_type_id;
	int								n_t_kind_id;
	DOUBLE							f_t_bonus;
	DOUBLE							f_bonuspercent;
};
//返回
struct DBO_GR_SetUserBonusRet //获取彩票玩家返点
{
	int								n_t_result;
};

struct DBR_GP_GetCpUser
{
	int								n_t_userid;
};
//返回
struct DBO_GR_GetCpUserInfoRet //获取彩票玩家信息
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



//查询充值提示
struct DBR_GP_GetChonzhiTishi
{
	DWORD							dwUserID;							//用户ID 
};

//查询提现提示
struct DBR_GP_GetTixianTishi
{
	DWORD							dwUserID;							//用户ID 
};

//查询投注提示
struct DBR_GP_GetTouzhuTishi
{
	DWORD							dwUserID;							//用户ID 
};

/////////////////////////////////////////////////////////////////////////////////////
//重庆时时彩
//下注
struct DBR_GP_TouZhuCQSSC
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
	int								nSign;								//下注标志
	int								bZhuihao;							//是否追号
	WORD							nIndex;								//index
	WORD							cbPlatform;							//平台
	DWORD							dwClientAddr;						//连接地址
};
struct DBR_GP_TouZhuCQSSC_Dan
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
	DWORD							dwClientAddr;						//连接地址
};
struct DBR_GP_TouZhuCQSSC_Zhuihao
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
	DWORD							dwClientAddr;						//连接地址
};

//////////////////////////////////////////////////////////////////////////////////
struct DBO_GR_GetMapBonusRet
{
	int								n_t_type_id;
	int								n_t_kind_id;
	DOUBLE							f_t_bonus;
	int								n_t_bonustype;
};
//返回查询系统时间
struct DBO_GR_GetSystemTime
{
	SYSTEMTIME						ServerTime;							//系统时间
};

//返回查询系统时间
struct DBO_GR_GetWinUserID
{
	int								nUserID;							//玩家ID
	DOUBLE							nWinZhushu;							//中奖金额
	DOUBLE							nZhushu;							//投注金额
	int								nState;
	TCHAR							szQishu[30];
	int								nType;
	DOUBLE							f_yingkui;
};

//查询银行
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//在线信息
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//类型数目
	DWORD							dwOnLineCountSum;					//总在线数
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//类型在线
};
///////////////////////////////////////////////////////////////////////////////////////
//投注日志数
struct DBR_GP_GetTouzhuLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//提现日志数
struct DBR_GP_GetTixianLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//充值日志数
struct DBR_GP_GetChongzhiLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//充值日志数
struct DBR_GP_GetHYXXLogCount
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
//盈亏日志数
struct DBR_GP_GetYingkuiLogCount
{
	int								dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//盈亏明细数
struct DBR_GP_GetYingkuiMxCount
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//棋牌盈亏数
struct DBR_GP_GetQiPaiYingkuiMxCount
{
	DWORD							dwUserID;							//用户ID
	int								nTypeID;					
	int								nByTime;					
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

//查询投注日志数
struct DBR_GP_GetXJTZHLogCount
{
	int							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	bool							bTime;								//通过时间查询
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//查询投注日志数
struct DBR_GP_CHKXJTZHLogCount
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
struct DBO_GR_GetXJTZHLogCountRet
{
	DWORD							dwCount;							//返回
};
//查询投注日志数
struct DBR_GP_GetXJYKLogCount
{
	DWORD							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHAR							s_t_account[30];					//查询账户
	bool							bTime;								//通过时间查询
	int								nXiaJiaID;							//下家ID 
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
	int								nType;								//0:投注  1 返点  2下级返点  3 中奖 4 充值 5取款 6棋牌盈亏
};
//返回
struct DBO_GR_GetXJYKLogCountRet
{
	DWORD							dwCount;							//返回
};
//查询投注日志数
struct DBR_GP_GetXJYKTjCount
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
struct DBO_GR_GetXJYKTjCountRet
{
	DWORD							dwCount;							//返回
};
//查询投注日志数
struct DBR_GP_GetXJYXTjCount
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
struct DBO_GR_GetXJYXTjCountRet
{
	DWORD							dwCount;							//返回
};
//查询下级充值日志数
struct DBR_GP_GetXjChzhLogCount
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
struct DBO_GR_GetXjChzhLogCountRet
{
	DWORD							dwCount;							//返回
};

//查询下级提现日志数
struct DBR_GP_GetXjTxLogCount
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
struct DBO_GR_GetXjTxLogCountRet
{
	DWORD							dwCount;							//返回
};
// 返回
// struct DBO_GR_GetCpUserInfoRet //获取彩票玩家信息
// {
// 	int								n_t_userid;
// 	TCHAR							s_t_account[30];
// 	int								n_t_type;
// 	DOUBLE							f_t_fandian;
// 	TCHAR							s_t_qq[11];
// 	int								n_t_daili_id;
// 	int								n_t_online;
// 	DOUBLE							f_t_yue;
// 	DOUBLE							f_t_dongjie;
// 	int								n_t_ban;
// 
// };

struct DBR_GP_GetChongzhiType
{
	int								nUserID;					//用户ID
	BYTE							cbType;						//类型
};

struct DBO_GP_GetChongzhiTypeRet
{
	TCHAR							s_t_yinhang[60];				//银行
	TCHAR							s_t_kaihuren[60];				//开户人
	TCHAR							s_t_zhanghao[60];				//账号
	TCHAR							s_t_web[60];					//网页
};
struct DBR_GP_GetQukuanInfo				//获取取款信息
{
	DWORD							dwUserID;						//用户ID
};

struct DBO_GP_GetQukuanInfoRet
{
	DOUBLE							f_t_yue;						//余额
	TCHAR							s_t_yinhang[60];				//银行
	TCHAR							s_t_kaihuren[60];				//开户人
	TCHAR							s_t_zhanghao[60];				//账号
	BYTE							n_t_isvip;						//是否超级VIP
};
//获取银行名字返回
struct DBO_GP_GetAllYinHangNameRet
{
	TCHAR							s_t_yinhang[60];				//银行
	int								n_QiYong;						//是否启用
};

struct DBR_GP_GetLastYue			//余额信息
{
	DWORD							dwUserID;						//用户ID
};
struct DBR_GP_GetMoreRecord
{
	int							n_t_type;					//类型   1 转金币   2 转彩金
};

struct DBO_GP_GetMoreRecordRet
{
	WORD							wKindID;							//操作代码
	TCHAR							wPeriod[30];							//期
	TCHAR							szLotNum[45];						//开奖数据
	TCHAR							szShijian[30];							//时间
};
struct DBR_GP_GetDailiHuikui
{
	int							n_t_userid;					//用户ID
};

struct DBO_GP_GetDailiHuikuiRet
{
	BYTE						cbNewRegUser;					//当天
	DOUBLE						f_t_touzhu;						//投注
	DOUBLE						f_t_chongzhi;					//充值
};
struct DBR_GP_DailiLingjiang
{
	int							n_t_userid;					//用户ID
};

struct DBO_GP_DailiLingjiangRet
{
	int							n_t_userid;					//用户ID 
	BYTE						n_t_state;					//所有状态 0 未  1已领
	DOUBLE						f_t_jine;						//金额
};

struct DBO_GP_GetLastYueRet
{
	DOUBLE							f_t_yue;						//余额
	DOUBLE							f_t_dongjie;					//冻结
	DOUBLE							f_t_score;						//游戏币
};
struct DBR_GP_QuitGame
{
	DWORD							dwUserID;						//用户ID
};

struct DBO_GP_QuitGameRet
{
	int								nResult;
	int								nUserID;
};

struct DBR_GP_UserQiandao
{
	DWORD							dwUserID;						//用户ID
};

struct DBO_GP_UserQiandaoRet
{
	int							n_t_res;
};

struct DBR_GP_GetUserHuanlesong
{
	DWORD							dwUserID;						//用户ID
};

struct DBO_GP_GetUserHuanlesongRet
{
	int							nRes;
	DOUBLE						f_t_xiaofei;
	DOUBLE						f_t_prize;
	int							n_t_count;
	int							n_t_XXcount;
};

struct DBR_GP_GetLingJiang
{
	DWORD							dwUserID;						//用户ID
};

struct DBO_GP_GetLingJiangRet
{
	int							n_t_got;
	DOUBLE							f_t_jine;						//余额
};


struct DBR_GP_DoQukuan				//取款
{
	DWORD							dwUserID;						//用户ID
	DOUBLE							fJine;							//金额
	TCHAR							szQukuanPass[33];				//取款密码
};

struct DBO_GP_DoQukuanRet
{
	int								nResult;						//返回值
	TCHAR							szDesc[126];					//取款提示
};

struct	DBR_GP_GetXJCHZHLog									//下级充值日志
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

struct DBO_GR_GetXJCHZHLogRet
{
	int								n_t_id;
	int								n_t_user_id;
	TCHAR							s_t_account[30];
	DOUBLE							f_t_jine;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	TCHAR							s_t_pingtai[30];
};
struct	DBR_GP_GetXJTxLog    //下家提现
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

struct DBO_GR_GetXJTxLogRet   // 下家提现返回
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

//返回
struct DBO_GR_CHKXJTZHLogCountRet
{
	DWORD							dwCount;							//返回
};

struct	DBR_GP_GetXJYKLog
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

struct DBO_GR_GetXJYKLogRet
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
struct	DBR_GP_GetXJYKTj
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
struct	DBR_GP_GetXJYXTj
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

struct DBR_GP_GetDaLiBao				//获取大礼包
{
	DWORD							dwUserID;						//用户ID
};

struct DBO_GP_GetDaLiBaoRet
{
	int							f_t_jine;						//余额
};

struct DBR_GP_GetZaJinDan
{
	DWORD							dwUserID;						//用户ID
	int								nType;						//金蛋type;
};

struct DBO_GP_GetZaJinDanRet
{
	int							f_t_jine;						//余额
};
struct DBR_GP_CanDaLiBao//判断大礼包资格
{
	DWORD							dwUserID;						//用户ID
};

struct DBO_GP_CanDaLiBaoRet
{
	DOUBLE						f_t_xiaofei;
	DOUBLE						f_t_chongzhi;
	DOUBLE						f_t_yue;
	int							n_t_got;						//余额
};

struct DBR_GP_CanZaJinDan
{
	DWORD							dwUserID;						//用户ID
};

struct DBO_GP_CanZaJinDanRet
{
	DOUBLE						f_t_xiaofei;
	int							n_t_got;						//余额
};

struct DBR_GP_CanLuckyZhuan							//幸运转盘资格
{
	DWORD							dwUserID;						//用户ID
};

struct DBO_GP_CanLuckyZhuanRet
{
	DOUBLE						f_t_xiaofei;
};

struct DBR_GP_GetLuckyZhuan						//幸运转盘
{
	DWORD							dwUserID;						//用户ID
};

struct DBO_GP_GetLuckyZhuanRet
{
	int							n_t_got;
	DOUBLE							f_t_jine;						//余额
};

struct DBO_GR_GetXJYKTjRet
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
struct DBO_GR_GetXJYXTjRet
{
	int								n_t_id;
	TCHAR							s_t_account[30];
	int								n_t_type;
	int								n_t_daili_id;

	DOUBLE							f_t_fandian_ze;
	DOUBLE							f_t_qipai_ze;
	DOUBLE							f_t_tuandui_ye;
};

struct	DBR_GP_GetTouzhuLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};

struct	DBR_GP_GetXJTZHLog
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
struct	DBR_GP_CHKXJTZHLog
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

struct	DBR_GP_GetXJTZHLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
};

struct	DBR_GP_GetXJTxLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
};

struct	DBR_GP_GetXJTxLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};

struct	DBR_GP_GetXJTZHLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};
struct	DBR_GP_GetXJYKLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
	int								nPage;								//页
	int								nSize;								//页的大小

};
struct	DBR_GP_GetXJCHZHLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
};

struct	DBR_GP_GetXJCHZHLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};

struct	DBR_GP_GetXJYKLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};
struct	DBR_GP_GetXJYKTjByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
};

struct	DBR_GP_GetXJYKTjByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};
struct	DBR_GP_CHKXJTZHLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;							//下级ID
	int								nPage;								//页
	int								nSize;								//页的大小
};
struct	DBR_GP_GetHyShj //获取会员数据
{
	int							dwUserID;							//用户ID
	BYTE						cbOnlyToday;						//只看今天   0 不是    1 是
	TCHAR						szTimeStart[30];					//起始时间
	TCHAR						szTimeEnd[30];						//结束时间
};
//返回
struct DBO_GR_GetHyShjRet
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

struct	DBR_GP_CHKXJTZHLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};

struct DBO_GR_GetXJTZHLogRet
{
	int								n_t_id;
	TCHAR							s_t_account[32];
	int								n_t_type;
	DOUBLE							f_t_touzhu_ze;
	DOUBLE							f_t_yingkui_ze;
};
struct DBO_GR_CHKXJTZHLogRet
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


struct	DBR_GP_GetHYXXLog
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
struct	DBR_GP_GetHYXXLogByID
{
	DWORD							dwUserID;							//用户ID
	int								nXiaJiID;								//下级ID 
};
struct	DBR_GP_GetHYXXLogByAct
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szAccount[33];							//下级账户
};
struct	DBR_GP_SWDaili
{
	DWORD							dwUserID;							//用户ID
	DWORD							nAgentID;							//代理ID
	int 							nDaili;							//是否代理
};
struct DBO_GP_SWDaili_RET
{
	LONG							lResult;
};
struct DBR_GetXjPeie
{
	int								n_t_userid;						//ID
};
struct DBO_GetXjPeieRet
{
	int								n_t_peie_1;					//3.0总配额
	int								n_t_peie_2;					//2.8总配额
	int								n_t_peie_3;					//2.7总配额
	int								n_t_peie_s_1;				//3.0已用配额
	int								n_t_peie_s_2;				//2.8已用配额
	int								n_t_peie_s_3;				//2.7已用配额
};
struct DBR_SetXjPeie
{
	int								n_t_userid;						//ID
	int								n_t_xj_id;						//下级ID
	int								n_t_peie1;
	int								n_t_peie2;
	int								n_t_peie3;
};
struct DBO_SetXjPeieRet
{
	int								n_t_result;					//返回
	TCHAR							s_t_desc[126];				//描述
};

struct	DBR_GP_HYXX_ZhuanZhang
{
	int								n_t_user_id;							//自己ID
	int								n_t_target_id;							//目标ID
	DOUBLE 							f_t_jine;								//金额
	TCHAR							s_t_password[126];							//密码
};
struct DBO_GP_HYXX_ZhuanZhang_RET
{
	int							lResult;
};


struct	DBR_GP_XGHYFandian
{
	int								dwUserID;							//用户ID
	int								n_xiaji_id;							//下级ID
	DOUBLE							f_Fandian;							//返点
};
struct DBO_GP_XGHYFandian_RET
{
	LONG							lResult;
	DOUBLE							f_Fandian;
	TCHAR							s_Account[33];
};


//提现日志
struct	DBR_GP_GetTixianLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//充值日志
struct	DBR_GP_GetChongzhiLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//充值日志
struct	DBR_GP_GetYingkuiLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//盈亏明细
struct	DBR_GP_GetYingkuiMx
{
	int							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	int							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
//盈亏明细
struct	DBR_GP_GetQiPaiYingkui
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHAR							szTimeStart[30];					//起始时间
	TCHAR							szTimeEnd[30];						//结束时间
};
struct DBR_GetUrlFandian
{
	int								n_t_userid;						//ID
};

struct DBR_GP_CancelTouzhu
{
	int								n_t_userid;							//用户ID
	int								n_t_id;								//投注ID
	int								n_t_kindid;
	TCHAR							s_t_qihao[30];
};
struct DBR_GetPeie
{
	int								n_t_userid;						//ID
};
struct DBR_GetPeieRet
{
	int								n_t_peie_1;
	int								n_t_peie_2;
	int								n_t_peie_3;
	int								n_t_peie_s_1;
	int								n_t_peie_s_2;
	int								n_t_peie_s_3;
};

struct DBR_GP_AddHuiYuan
{
	int								n_t_daili_id;
	int								n_type;
	DOUBLE							f_t_fandian;
	TCHAR							s_t_account[33];
	TCHAR							s_t_login_pass[33];
	TCHAR							s_t_qukuan_pass[33];
	TCHAR							s_t_qq[33];
};

struct DBR_GP_SetWebFandian
{
	int								n_t_userid;
	DOUBLE							f_t_fandian;
};
struct DBO_GR_SetWebFandianRet
{
	int								n_t_res;
	DOUBLE							f_t_fandian;

};

struct DBO_GR_AddHuiYuanRet
{
	int								n_t_res;
	TCHAR							s_t_Desc[126];
};

struct	DBR_GP_GetUserInfo
{
	DWORD							dwUserID;							//用户ID
};

struct DBO_GR_GetUserInfoRet
{
	TCHAR							s_t_qq[12];	
	SYSTEMTIME						t_reg_time;						// 注册时间
	TCHAR							s_t_sjqq[12];
	DOUBLE							f_t_yongjin;					//投注佣金
	DOUBLE							f_t_yongjinze;					//投注佣金总额
	DOUBLE							f_t_fenhong;					//分红
	DOUBLE							f_t_fenhongze;					//分红总额
	DOUBLE							f_t_qipaiye;					//棋牌余额
	DOUBLE							f_t_kuisunyongjin;					//亏损佣金
	DOUBLE							f_t_kuisunyongjinze;					//亏损佣金总额
	
	SYSTEMTIME						t_ksyj_time;						// 亏损佣金时间
	SYSTEMTIME						t_fh_time;						//	分红时间 
	SYSTEMTIME						t_yj_time;						// 佣金时间
};

struct DBR_GP_GetNews
{
	int							n_t_id;						//ID
	int							n_t_ccount;			//数量
};

struct DBO_GP_GetNewsRet
{
	TCHAR						s_t_news[512];				//新闻
	SYSTEMTIME					s_t_time;					//更新时间
};

//转换
struct DBR_GP_ZhuanHuan
{
	int							n_t_userid;					//用户ID
	int							n_t_type;					//类型   1 转金币   2 转彩金
	DOUBLE						f_t_jine;					//金额
};

struct DBO_GP_ZhuanHuanRet
{
	int							nResult;
	DOUBLE						f_t_yue;
	DOUBLE						f_t_score;
};
struct DBR_GP_AlterGender
{
	int								n_t_userid;					//用户ID
	BYTE 							cbGender;					//性别
};

struct DBO_GP_AlterGenderRet
{
	BYTE						n_t_return;					//所有状态 0 未  1已领
};

//修改QQ 
struct	DBR_GP_XGqq
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szQQ[11];							//新QQ 
};
struct DBO_GR_XG_QQ_RET
{
	LONG							lResult;
};

struct	DBR_GP_XG_Loginpass
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szOldPass[33];						//旧密码
	TCHAR							szNewPass[33];						//新密码
};
struct DBO_GR_XG_Loginpass_RET
{
	int							lResult;
};

struct	DBR_GP_XG_Qukuanpass
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szOldPass[33];						//旧密码
	TCHAR							szNewPass[33];						//新密码
};
struct DBO_GR_XG_Qukuanpass_RET
{
	int							lResult;
};

struct	DBR_GP_LockMachine
{
	DWORD							dwUserID;							//用户ID
	BYTE						cbLock;									//锁定
};
struct DBO_GR_LockMachine_RET
{
	int							lResult;
};

struct	DBR_GP_SetQukuanProtect
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szQukuanPass[33];						//密码
	TCHAR							szQukuanAsk[33];						//问题
	TCHAR							szQukuanAnswer[33];						//答案
};
struct DBO_GR_SetQukuanProtect_RET
{
	int							lResult;
};
struct	DBR_GP_QueryMyProTect
{
	DWORD							dwUserID;							//用户ID
};
struct DBO_GR_QueryMyProTect_RET
{
	int							lResult;
	TCHAR						szQuestion[33];
	TCHAR						szAnswer[33];
};

struct	DBR_GP_SetQukuanZhanghao
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szKaihuYinghang[33];						//密码
	TCHAR							szKaihuRen[33];						//问题
	TCHAR							szYinhangZhanghu[33];						//答案
	TCHAR							szQukuanPass[33];						//答案
};

struct	DBO_GP_QueryMyYinHang
{
	DWORD							dwUserID;							//用户ID
};
struct DBR_GR_QueryMyYinHang_RET
{
	int							lResult;
	TCHAR						szKaihuYinghang[33];
	TCHAR						szKaihuRen[33];
	TCHAR						szYinhangZhanghu[33];				
};

struct DBO_GR_SetQukuanZhanghao_RET
{
	int							lResult;
};

struct DBR_GP_GetTouzhu
{
	int							n_t_id;						//ID
};
struct DBR_MB_GetTouzhu
{
	int							n_t_id;						//ID
};

struct DBO_MB_GetTouzhuRet
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


struct DBO_GP_GetTouzhuRet
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

//提现记录返回
struct DBO_GR_GetTixianLogRet
{
	int								n_t_id;
	SYSTEMTIME						n_t_time;
	TCHAR							n_t_yinhang[30];
	DOUBLE							f_t_jine;
	TCHAR							s_t_kaihuren[30];
	int								n_t_state;
	TCHAR							s_t_note[30];
};
struct DBO_GR_GetHYXXLogRet
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


struct DBO_GR_GetTouzhuLogRet
{
	int								n_t_id;
	int								n_t_type;
	int								n_t_kind;
	TCHAR							szQishu[30];
	int								n_t_zhushu;
	int								n_t_moshi;
	int								n_t_beishu;
	SYSTEMTIME							n_t_time;
	int								n_t_state;
	int								n_t_winzhushu;
	DOUBLE							f_t_yingkui;
	int								n_t_zhuihao;
};

struct DBO_GR_GetChongzhiRet
{
	int								n_t_id;
	DOUBLE							f_t_jine;
	SYSTEMTIME							n_t_time;
	int								n_t_state;
	TCHAR							s_t_pingtai[30];

};
struct DBO_GR_GetYingkuiLogRet
{
	DOUBLE							f_t_chongzhi;
	DOUBLE							f_t_qukuan;
	DOUBLE							f_t_touzhu;
	DOUBLE							f_t_fandian;
	DOUBLE							f_t_zhongjiang;
	DOUBLE							f_t_huodong;
	DOUBLE							f_t_chedan;
};

//盈亏明细
struct DBO_GR_GetYingkuiMxRet
{
	int								n_t_dingdan;
	DOUBLE							f_t_yingkui;
	DOUBLE							f_t_yue;
	SYSTEMTIME						t_t_yingkuitime;
	TCHAR							s_t_type[30];
	TCHAR							s_t_zhaiyao[126];
};
//棋牌盈亏明细
struct DBO_GR_GetQiPaiYingkuiRet
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

//撤单返回
struct DBO_GR_CancelTouzhuRet
{
	int								nResult;
	int								n_t_userid;
};
//充值提示返回
struct DBO_GR_ChongzhiTishiRet
{
	DOUBLE							fJine;
	int								nState;
};
//提现提示返回
struct DBO_GR_TixianTishiRet
{
	DOUBLE							fJine;
	int								nState;
};
//投注提示返回
struct DBO_GR_TouzhuTishiRet
{
	int								nWinZhushu;
	int								nState;
	TCHAR							szQishu[30];
	int								nType;
};
struct DBR_GP_GetCpUserInfo
{
	int								nUserID;
};
//////////////////////////////////////////////////////////////////////////////////
//输出信息

//登录结果
#define DBO_GP_LOGON_SUCCESS		100									//登录成功
#define DBO_GP_LOGON_FAILURE		101									//登录失败

//操作结果
#define DBO_GP_USER_FACE_INFO		110									//用户头像
#define DBO_GP_USER_INDIVIDUAL		111									//用户资料

//银行命令
#define DBO_GP_USER_INSURE_INFO		120									//银行资料
#define DBO_GP_USER_INSURE_SUCCESS	121									//银行成功
#define DBO_GP_USER_INSURE_FAILURE	122									//银行失败

//列表结果
#define DBO_GP_GAME_TYPE_ITEM		130									//种类信息
#define DBO_GP_GAME_KIND_ITEM		131									//类型信息
#define DBO_GP_VIRTUAL_COUNT        132                                 //虚拟人数 
#define DBO_GP_GAME_LIST_RESULT		133									//加载结果

//服务结果
#define DBO_GP_OPERATE_SUCCESS		500									//操作成功
#define DBO_GP_OPERATE_FAILURE		501									//操作失败


//登录成功
struct DBO_GP_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户标识
	DWORD							dwExperience;						//经验数值
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//用户成绩
	SCORE							lScore;								//用户积分
	SCORE							lInsure;							//用户银行
	SCORE                           lUserMedal;                         //用户奖牌

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器

	//会员资料
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间

	//扩展信息
	TCHAR                           szAddrDescribe[LEN_ADDR_DESCRIBE];  //地址描述

	int								n_type;
	DOUBLE							f_fandian;
	DOUBLE							f_dongjie;
	DOUBLE							f_yue;
	//描述信息
	TCHAR							szDescribeString[128];				//描述消息

};

//登录失败
struct DBO_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//头像信息
struct DBO_GP_UserFaceInfo
{
	WORD							wFaceID;							//头像标识
};

//个人资料
struct DBO_GP_UserIndividual
{
	//用户信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//用户说明
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	//电话号码
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

	//联系资料
	TCHAR							szQQ[LEN_QQ];						//Q Q 号码
	TCHAR							szEMail[LEN_EMAIL];					//电子邮件
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
};

//银行资料
struct DBO_GP_UserInsureInfo
{
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//银行成功
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSourceScore;						//原来金币
	SCORE							lSourceInsure;						//原来金币
	SCORE							lInsureRevenue;						//银行税收
	SCORE							lVariationScore;					//金币变化
	SCORE							lVariationInsure;					//银行变化
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct  DBO_GP_UserInsureFailure
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息
};

//操作失败
struct DBO_GP_OperateFailure
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct DBO_GP_OperateSuccess
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//成功消息
};
//获取银行
struct DBO_GP_GetBankName
{
	LONG							lResultCode;						//操作代码
	TCHAR							szYinHangName[128];					//成功消息
};

//开奖查询结果
struct DBO_GP_QueryLotResult
{
	WORD							wKindID;							//操作代码
// 	TCHAR							szCode[32];							//
// 	TCHAR							szPlayKey[32];						//游戏简拼
// 	TCHAR							cbSerialID[32];							//当天期数 跟SerialData 合组成Period
// 	TCHAR							wSerialDate[32];						//日期
	TCHAR							wPeriod[30];							//期
	TCHAR							szLotNum[45];						//开奖数据
	TCHAR							szShijian[30];							//时间
// 	SYSTEMTIME						LotTime;							//时间
// 	BYTE							cbStatus;							//状态
};
struct DBO_GP_GetKefuUrlRet
{
	int								n_t_result;					//返回
	TCHAR							s_t_desc[256];				//描述

};
//重庆时时彩投注
struct DBO_GR_TouzhuResult
{
	int								nUserID;							//用户ID
	LONG							lResultCode;						//返回值
	TCHAR							szDesc[126];						//返回描述
	int								nSign;								//标志
};
//查询投注日志返回
struct DBO_GR_GetTouzhuLogCountRet
{
	DWORD							dwTouzhuLogCount;					//数量
};
//查询提现日志返回
struct DBO_GR_GetTixianLogCountRet
{
	DWORD							dwTixianLogCount;					//数量
};
//查询充值日志返回
struct DBO_GR_GetChongzhiLogCountRet
{
	DWORD							dwChongzhiLogCount;					//数量
};
//盈亏充值日志返回
struct DBO_GR_GetYingkuiLogCountRet
{
	DWORD							dwYingkuiLogCount;					//数量
};
//盈亏明细数量返回
struct DBO_GR_GetYingkuiMxCountRet
{
	DWORD							dwYingkuiMxCount;					//数量
};
//盈亏明细数量返回
struct DBO_GR_GetQiPaiYingkuiCountRet
{
	DWORD							dwYingkuiMxCount;					//数量
};
//查询会员日志返回
struct DBO_GR_GetHYXXLogCountRet
{
	DWORD							dwHYXXLogCount;					//数量
};

struct DBO_GetRegUrl
{
	TCHAR							s_reg_url[126];					//注册链接
	DOUBLE							f_t_fandian;
};

//////////////////////////////////////////////////////////////////////////////////

//游戏类型
struct DBO_GP_GameType
{
	WORD							wSortID;							//排序索引
	WORD							wTypeID;							//类型索引
	TCHAR							szTypeName[LEN_TYPE];				//种类名字
};

//游戏种类
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//类型索引
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wGameID;							//模块索引
	TCHAR							szKindName[LEN_KIND];				//游戏名字
	TCHAR							szProcessName[LEN_PROCESS];			//进程名字
};

//虚拟人数
struct DBO_GP_VirtualCount
{
	WORD                           wServerID;                           //房间标识
	DWORD                          dwVirtualCount;                       //虚拟人数  
};

//加载结果
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//成功标志
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//手机数据包

//登录命令
#define	DBR_MB_LOGON_ACCOUNTS		500									//帐号登录
#define DBR_MB_REGISTER_ACCOUNTS	501									//注册帐号

//登录结果
#define DBO_MB_LOGON_SUCCESS		600									//登录成功
#define DBO_MB_LOGON_FAILURE		601									//登录失败

//////////////////////////////////////////////////////////////////////////////////

//帐号登录
struct DBR_MB_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号注册
struct DBR_MB_RegisterAccounts
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//登录成功
struct DBO_MB_LogonSuccess
{
	//用户属性
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwExperience;						//经验数值
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称

	//描述信息
	TCHAR							szDescribeString[128];				//描述消息

	//扩展信息
	TCHAR                           szAddrDescribe[LEN_ADDR_DESCRIBE];  //地址描述
};


//登录失败
struct DBO_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

struct DBR_GP_GetMyMapBonus
{
	int								n_t_userid;
};
struct DBO_GP_GetMyMapBonusRet
{
	int								n_t_type;
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()
#endif