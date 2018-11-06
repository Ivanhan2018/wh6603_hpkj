#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"
#include "LogFile.h"
//////////////////////////////////////////////////////////////////////////////////

char szHaoma[512000] = "";
char szHaomaTemp[102400] = "";
byte pDestBuffer[102400];
int nTouZhuSign = 0;
int nZongZhu1 = 0;
static const int  timer_get_sql_time = 1;				//获取数据库时间

//彩票种类
typedef enum {
	CZChongQingSSC=1,		//重庆时时彩
	CZJiangXiSSC=2,			//江西时时彩
	CZ3D=3,					//福彩3D
	CZXinJiangSSC = 4,		//新疆时时彩
	CZPaiLie3,			//排列3
	CZGD11Xuan5,		//广东11选5
	CZCQ11Xuan5,		//重庆11选5
	CZSD11Xuan5,		//山东11选5
	CZJX11Xuan5,		//江西11选5
	CZKUAILE8,				//北京快乐8
	CZ_FENFEN_CAI,			//分分彩：5分钟开奖，24小时不停。每天288期
	CZ_WUFEN_CAI,			//5分彩
	CZ_BJKUAICHE,			//北京快车
	CZ_PK10=13,		//PK10
	CZ_LIUHECAI = 14,
	CZ_QiXingCai = 15,			//七星彩--lly
	CZHLJ11Xuan5=16,		//黑龙江11选5
	CZXingYun28,
	CZ_HGYDWFC=20,
	CZ_TianJinSSC = 23,	//天津时时彩
	CZCount
	
}CaiZhong;
//时时彩玩法种类
typedef enum 
{
	inValid,
	//QianSanZhiXuan,			//前三直选
	QianSanZhiXuan_FuShi=1,		//前三直选复式，也叫五星前三
	QianSanZhiXuan_DanShi,		//

	//HouSanZhiXuan,			//后三直选
	HouSanZhiXuan_FuShi,		//后三直选复式，也叫五星后三
	HouSanZhiXuan_DanShi,

	//QianSanZuXuan,			//前三组选
	QianSanZuXuan_ZuSanFuShi,
	QianSanZuXuan_ZuSanDanShi,
	QianSanZuXuan_ZuLiuFuShi,
	QianSanZuXuan_ZuLiuDanShi,

	//HouSanZuXuan,				//后三组选
	HouSanZuXuan_ZuSanFuShi,
	HouSanZuXuan_ZuSanDanShi,
	HouSanZuXuan_ZuLiuFuShi,
	HouSanZuXuan_ZuLiuDanShi,

	//WuXingQianEr,				//五星前二
	WuXingQianEr_FuShi,
	WuXingQianEr_DanShi,

	//WuXingHouEr,				//五星后二
	WuXingHouEr_FuShi,
	WuXingHouEr_DanShi,

	//QianErZuXuan,				//前二组选
	QianErZuXuan_FuShi,
	QianErZuXuan_DanShi,

	//HouErZuXuan,				//后二组选
	HouErZuXuan_FuShi,
	HouErZuXuan_DanShi,

	DaXiaoDanShuang,			//大小单双

	//BuDingWei,				//不定位
	BuDingWei_QianSan,
	BuDingWei_HouSan,

	DingWeiDan,					//定位胆

	RenXuan2,					//任选2,3,4
	RenXuan3,
	RenXuan4,

	QianSanHunhe,				//前三混合
	ZhongSanHunhe,				//中三混合
	HouSanHunhe,				//后三混合

	//新增直选复式玩法
	WuxingZhixuan,				//五星直选（0209新增）
	WuxingQianSi,				//五星前四
	WuxingHousi,				//五星后四
	WuxingZhongsan,				//五星中三（五星前三和五星后三见第一排）

	//新增组选玩法
	ZhongsanZusan,				//中三组三（0209新增）
	ZhongsanZuliu,				//中三组六

	//新增组选玩法
	Zuxuan120,					//组选120
	Zuxuan60,					//组选60
	Zuxuan30,					//组选30
	Zuxuan20,					//组选20
	Zuxuan10,					//组选10
	Zuxuan5,					//组选5

	//新增和值玩法
	QianSanZhiXuanHeZhi,		//前三直选和值
	HouSanZhiXuanHeZhi,			//后三直选和值
	ZhongSanZhiXuanHeZhi,		//中三直选和值

	//新增不定位
	BuDingWei_QianEr,			//不定位：前2
	BuDingWei_HouEr,			//不定位：后2

	//新增趣味玩法
	QW_YiFanFengShun,			//趣味：一帆风顺，一个一样一个数字就中奖
	QW_HaoShiChengShuang,		//趣味：好事成双，2个一样的数字才中奖
	QW_SanXingBaoXi,			//趣味：三星报喜，3个一样的数字才中奖
	QW_SiJiFaCai,				//趣味：四季发财，4个一样的数字才中奖
	NiuNiu_Num,					//牛牛：5个数字之和是10的倍数；牛1--牛10（牛牛） 1:10（无牛通吃）
	NiuNiu_None,				//牛牛：无牛 1:2
	NiuNiu_DxDs,				//牛牛：大小单双 1:2.5
	DaXiaoDanShuang_dw,			//大小单双定位
	Zonghe_DXDSLh=56,			//总和大小单双：总和值大於等於23為大，小於等於22為小。
	SSC_LongHu,					//龙虎：头大于尾为龙，尾大于头为虎；和 ？
	QianSan_Quwei,				//前三豹子顺子对子半顺杂六
	ZhongSan_Quwei,				//中三豹子顺子对子半顺杂六
	HouSan_Quwei,				//后三豹子顺子对子半顺杂六
	HouErZhixuan_Hezhi=61,		//后二直选和值
	QianErZhixuan_Hezhi,		//前二直选和值
	RenSanZhixuan_Hezhi,		//任三直选和值	0
	RenErZhixuan_Hezhi,			//任二直选和值	0
	SSC_LongHuYaJun=65,			//龙虎：亚军
	Ren3Zu3_Fushi,				//任3组3复式	0
	Ren3Zu6_Fushi,				//任3组6复式	0
	Ren3Hunhe_Zuxuan,			//任3混合 组选	0
	Ren2Zuxuan_Fushi,			//任2组选 复式	0
	Ren2Zuxuan_Danshi,			//任2组选 单式	0
	BuDingWei_ZhongSan,

	WanFaCount
}SSCWanFaKind;
//北京快8
typedef enum {
	enBJK8_RX_1=1,
	enBJK8_RX_2,
	enBJK8_RX_3,
	enBJK8_RX_4,
	enBJK8_RX_5,
	enBJK8_RX_6,
	enBJK8_RX_7,
	enBJK8_RX_8,
	enBJK8_RX_9,
	enBJK8_RX_10,

	// 	enXY28_DXDS,	//和值大小单双
	// 	enXY28_Funny,		//趣味
	// 	enXY28_JiZhi,		//极值
	// 	enXY28_TeMa=14,	//特码

	enBJK8_ShangXia=15,
	enBJK8_JiOu=16,
	enBJK8_HeZhiDXDS=17,
}BeiJingKuai8Kind;

typedef enum 
{
	IIinValid,

	IIRenXuan2=1,						//任选二

	IIRenXuan3,					//任选三
	IIRenXuan4,							//任选四
	IIRenXuan5,							//任选五

	IIRenXuan6,							//任选六

	IIRenXuan7,							//任选七

	IIRenXuan8,									//任选八
	QianYi,					//前一


	QianEr_ZhiXuan,							//前二 直选选号
	QianEr_ZuXuan,						//前二组选选号


	QianSan_ZhiXuan,							//前三直选选号
	QianSan_ZuXuan,						//前三组选选号


	IIWanFaCount
}IIXuan5GameKind;

typedef enum 
{
	Invalid,
	enWF_ZhixuanFushi,					//三星直选复式,
	enWF_ZhixuanDanshi,					//三星直选单式,
	enWF_QiansanzusanFushi,				//前三组三复式,
	enWF_QiansanzusanDanshi,			//前三组三单式,
	enWF_QiansanzuliuFushi,				//前三组六复式,
	enWF_QiansanzuliuDanshi,			//前三组六单式,
	enWF_QianyizhixuanFushi,			//前一直选复式,
	enWF_QianerzhixuanFushi,			//前二直选复式,
	enWF_QianerzhixuanDanshi,			//前二直选单式,
	enWF_Budingwei,						//三星不定位,
	enWF_HouyizhixuanFushi,				//后一直选复式,
	enWF_HouerzhixuanFushi,				//后二直选复式,
	enWF_HouerzhixuanDanshi,			//后二直选单式,
	enWF_ZhixuanHezhi,					//三星直选和值,
	enWF_DaxiaoDanshuang,				//大小单双
	enWF_DingWeiDan,					//定位胆
	enWF_Count							//玩法个数

}tagEnumTreeNumbers;

// 	//七星彩玩法种类---lly
typedef enum {
	QiXingCai_LiangDing=1,
	QiXingCai_SanDing,
	QiXingCai_SiDing,
	QiXingCai_YiDing,
	QiXingCai_LiangZiXian,
	QiXingCai_SanZiXian,
}QiXingCaiKind;

//幸运28
typedef enum {
	enBJK8_1=1,
	enBJK8_2,
	enBJK8_3,
	enBJK8_4,
	enBJK8_5,
	enBJK8_6,
	enBJK8_7,
	enBJK8_8,
	enBJK8_9,
	enBJK8_10,

	enXY28_DXDS,	//和值大小单双
	enXY28_Funny,		//趣味
	enXY28_JiZhi,		//极值
	enXY28_TeMa=14,	//特码

}XingYun28Kind;

typedef enum 
{
	PK10inValid,

	WF_GuanJun,
	WF_GuanYaJun,
	WF_YaJun,
	WF_QianSan,
	WF_DiSan,
	WF_QianSi,
	WF_DiSi,
	WF_QianWu,
	WF_DiWu,
	WF_QianLiu,
	WF_DiLiu,
	WF_QianQi,
	WF_DiQi,
	WF_QianBa,
	WF_DiBa,
	WF_QianJiu,
	WF_DiJiu,
	WF_QianShi,
	WF_DiShi,
	WF_DXDS,	//大小单双
	WF_DWD,
	WF_GYHZH,	//冠亚和值
	PK10WanFaCount
}PK10GameKind;

//构造函数
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//组件变量
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	byte szInput[8]={0};
	byte  szOutput[8]={0};
	int nRet = CWHService::LZCompressData(szInput,8,szOutput,8);
	return;
}

//析构函数
CDataBaseEngineSink::~CDataBaseEngineSink()
{
	if ((m_AccountsDBModule.GetInterface()!=NULL)||(m_AccountsDBModule.CreateInstance()!=false))
	{
		m_AccountsDBModule->CloseConnection();
	}
	if ((m_TreasureDBModule.GetInterface()!=NULL)||(m_TreasureDBModule.CreateInstance()!=false))
	{
		m_TreasureDBModule->CloseConnection();


	}
	if ((m_PlatformDBModule.GetInterface()!=NULL)||(m_PlatformDBModule.CreateInstance()!=false))
	{
		m_PlatformDBModule->CloseConnection();

	}
	if ((m_GameSSCDBModule.GetInterface()!=NULL)||(m_GameSSCDBModule.CreateInstance()!=false))
	{
		m_GameSSCDBModule->CloseConnection();

	}

}

//接口查询
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//启动事件
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//创建对象
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_TreasureDBModule.GetInterface()==NULL)&&(m_TreasureDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_PlatformDBModule.GetInterface()==NULL)&&(m_PlatformDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_GameSSCDBModule.GetInterface()==NULL)&&(m_GameSSCDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}
	try
	{
		//连接信息
		tagDataBaseParameter * pAccountsDBParameter=&m_pInitParameter->m_AccountsDBParameter;
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
		tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;
		tagDataBaseParameter * pGameSSCDBParameter=&m_pInitParameter->m_GameSSCDBParameter;
		
		//设置连接
		m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr,pAccountsDBParameter->wDataBasePort,
			pAccountsDBParameter->szDataBaseName,pAccountsDBParameter->szDataBaseUser,pAccountsDBParameter->szDataBasePass);
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
			pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass);
		m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
			pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass);

		m_GameSSCDBModule->SetConnectionInfo(pGameSSCDBParameter->szDataBaseAddr,pGameSSCDBParameter->wDataBasePort,
			pGameSSCDBParameter->szDataBaseName,pGameSSCDBParameter->szDataBaseUser,pGameSSCDBParameter->szDataBasePass);
		//发起连接
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		//发起连接
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//发起连接
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

		//发起连接
		m_GameSSCDBModule->OpenConnection();
		m_GameSSCDBAide.SetDataBase(m_GameSSCDBModule.GetInterface());

		return true;
	}
	catch (IDataBaseException * pIException)
	{

		//错误信息
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CString strLogFile;
		strLogFile.Format(L"Exception OnDataBaseEngineStart");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		CTraceService::TraceString(_T("This ok?"),TraceLevel_Exception);
		return false;
	}

	return true;
}

//停止事件
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//设置对象
	m_AccountsDBAide.SetDataBase(NULL);
	m_TreasureDBAide.SetDataBase(NULL);
	m_PlatformDBAide.SetDataBase(NULL);

	//关闭连接
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
	}

	//关闭连接
	if (m_TreasureDBModule.GetInterface()!=NULL)
	{
		m_TreasureDBModule->CloseConnection();
		m_TreasureDBModule.CloseInstance();
	}

	//关闭连接
	if (m_PlatformDBModule.GetInterface()!=NULL)
	{
		m_PlatformDBModule->CloseConnection();
		m_PlatformDBModule.CloseInstance();
	}

	//关闭连接
	if (m_GameSSCDBModule.GetInterface()!=NULL)
	{
		m_GameSSCDBModule->CloseConnection();
		m_GameSSCDBModule.CloseInstance();
	}
	//组件变量
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return true;
}

//时间事件
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return false;
}

//控制事件
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return false;
}

//请求事件
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	CString strLogFile;
	switch (wRequestID)
	{
	case DBR_GP_LOGON_ACCOUNTS:			//帐号登录
		{
			strLogFile.Format(L"DBR_GP_LOGON_ACCOUNTS");
			LogFile::instance().LogText(strLogFile);
			return OnRequestLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS:		//注册帐号
		{
			strLogFile.Format(L"DBR_GP_REGISTER_ACCOUNTS");
			LogFile::instance().LogText(strLogFile);
			return OnRequestRegisterAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_MACHINE:			//修改机器
		{
			strLogFile.Format(L"DBR_GP_MODIFY_MACHINE");
			LogFile::instance().LogText(strLogFile);

			return OnRequestModifyMachine(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_LOGON_PASS:		//修改密码
		{
			strLogFile.Format(L"DBR_GP_MODIFY_LOGON_PASS");
			LogFile::instance().LogText(strLogFile);
			return OnRequestModifyLogonPass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INSURE_PASS:		//修改密码
		{
			strLogFile.Format(L"DBR_GP_MODIFY_INSURE_PASS");
			LogFile::instance().LogText(strLogFile);
			return OnRequestModifyInsurePass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_SYSTEM_FACE:		//修改头像
		{
			strLogFile.Format(L"DBR_GP_MODIFY_SYSTEM_FACE");
			LogFile::instance().LogText(strLogFile);
			return OnRequestModifySystemFace(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INDIVIDUAL:		//修改资料
		{
			strLogFile.Format(L"DBR_GP_MODIFY_INDIVIDUAL");
			LogFile::instance().LogText(strLogFile);
			return OnRequestModifyIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_SAVE_SCORE:		//存入金币
		{
			strLogFile.Format(L"DBR_GP_USER_SAVE_SCORE");
			LogFile::instance().LogText(strLogFile);
			return OnRequestUserSaveScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TAKE_SCORE:		//提取金币
		{
			strLogFile.Format(L"DBR_GP_USER_TAKE_SCORE");
			LogFile::instance().LogText(strLogFile);
			return OnRequestUserTakeScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TRANS_SCORE:       //转账金币
		{
			strLogFile.Format(L"DBR_GP_USER_TRANS_SCORE");
			LogFile::instance().LogText(strLogFile);
			return OnRequestUserTransScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_LHC_QIHAO:       //转账金币
		{
			strLogFile.Format(L"DBR_GP_GET_LHC_QIHAO");
			LogFile::instance().LogText(strLogFile);
			return OnRequestLhcQihao(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INDIVIDUAL:		//查询资料
		{
			strLogFile.Format(L"DBR_GP_QUERY_INDIVIDUAL");
			LogFile::instance().LogText(strLogFile);
			return OnRequestQueryIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INSURE_INFO:		//查询银行
		{
			strLogFile.Format(L"DBR_GP_QUERY_INSURE_INFO");
			LogFile::instance().LogText(strLogFile);
			return OnRequestQueryInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_GAME_LIST:			//加载列表
		{
			strLogFile.Format(L"DBR_GP_LOAD_GAME_LIST");
			LogFile::instance().LogText(strLogFile);
			return OnRequestLoadGameList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ONLINE_COUNT_INFO:		//在线信息
		{
			strLogFile.Format(L"DBR_GP_ONLINE_COUNT_INFO");
			LogFile::instance().LogText(strLogFile);
			return OnRequestOnLineCountInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_VIRTUAL_COUNT:     //虚拟人数
		{
			strLogFile.Format(L"DBR_GP_LOAD_VIRTUAL_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnRequestVirtualUserCount(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_ACCOUNTS:			//帐号登录
		{
			strLogFile.Format(L"DBR_MB_LOGON_ACCOUNTS");
			LogFile::instance().LogText(strLogFile);
			return OnMobileLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_MB_REGISTER_ACCOUNTS:		//注册帐号
		{
			strLogFile.Format(L"DBR_MB_REGISTER_ACCOUNTS");
			LogFile::instance().LogText(strLogFile);
			return OnMobileRegisterAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_RESULT:			//游戏开奖查询
		{
			strLogFile.Format(L"DBR_GP_QUERY_RESULT");
			LogFile::instance().LogText(strLogFile);
			return OnQueryGameResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_SYS_TIME:			//系统时间
		{
			strLogFile.Format(L"DBR_GP_GET_SYS_TIME");
			LogFile::instance().LogText(strLogFile);
			return OnQuerySystemTime(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_WIN_USER:
		{
			return OnQueryWinUserID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_MAP_BONUS:			//获取MAPBONUS
		{
			strLogFile.Format(L"DBR_GP_GET_MAP_BONUS");
			LogFile::instance().LogText(strLogFile);
			return OnQueryMapBonus(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_CP_USER_INFO:			//游戏开奖查询
		{
			strLogFile.Format(L"DBR_GP_GET_CP_USER_INFO");
			LogFile::instance().LogText(strLogFile);
			return OnQueryCpUserInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_MY_MAP_BONUS:
		{
			strLogFile.Format(L"DBR_GP_GET_MY_MAP_BONUS");
			LogFile::instance().LogText(strLogFile);
			return OnGetMyMapBonus(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_USER_FANDIAN:			//游戏开奖查询
		{
			strLogFile.Format(L"DBR_GP_GET_USER_FANDIAN");
			LogFile::instance().LogText(strLogFile);
			return OnGetUserFandian(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SET_USER_BONUS:			//游戏开奖查询
		{
			strLogFile.Format(L"DBR_GP_SET_USER_BONUS");
			LogFile::instance().LogText(strLogFile);
			return OnSetUserBonus(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TOUZHU_CQSSC:
		{
			strLogFile.Format(L"DBR_GP_TOUZHU_CQSSC");
			LogFile::instance().LogText(strLogFile);
			return OnTouzhuCQSSC(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_KEFU_URL:
		{
			strLogFile.Format(L"DBR_GP_TOUZHU_CQSSC");
			LogFile::instance().LogText(strLogFile);
			return OnGetKefuUrl(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TOUZHU_CQSSC_DAN:
		{
			strLogFile.Format(L"DBR_GP_TOUZHU_CQSSC_DAN");
			LogFile::instance().LogText(strLogFile);
			return OnTouzhuCQSSCDan(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TOUZHU_CQSSC_ZHUIHAO:
		{
			strLogFile.Format(L"DBR_GP_TOUZHU_CQSSC_ZHUIHAO");
			LogFile::instance().LogText(strLogFile);
			return OnTouzhuCQSSCZhuihao(dwContextID,pData,wDataSize);
		}
	case DBR_MB_TOUZHU_CQSSC_ZHUIHAO:
		{
			strLogFile.Format(L"DBR_MB_TOUZHU_CQSSC_ZHUIHAO");
			LogFile::instance().LogText(strLogFile);
			return OnMBTouzhuCQSSCZhuihao(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TOUZHU_LOG_COUNT:
		{
			strLogFile.Format(L"DBR_GP_TOUZHU_LOG_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnGetTouzhuLogCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TIXIAN_LOG_COUNT:
		{
			strLogFile.Format(L"DBR_GP_TIXIAN_LOG_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnGetTixianLogCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHONGZHI_LOG_COUNT:
		{
			strLogFile.Format(L"DBR_GP_CHONGZHI_LOG_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnGetChongzhiLogCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_HYXX_LOG_COUNT:
		{
			strLogFile.Format(L"DBR_GP_HYXX_LOG_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnGetHYXXLogCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_YINGKUI_LOG_COUNT:
		{
			strLogFile.Format(L"DBR_GP_YINGKUI_LOG_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnGetYingkuiLogCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_YINGKUI_MX_COUNT:
		{
			strLogFile.Format(L"DBR_GP_YINGKUI_MX_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnGetYingkuiMxCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QP_YINGKUI_COUNT:
		{
			strLogFile.Format(L"DBR_GP_QP_YINGKUI_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnGetQiPaiYingkuiCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJTZH_LOG_COUNT:
		{
			strLogFile.Format(L"DBR_GP_XJTZH_LOG_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJTZHLogCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHKXJTZH_LOG_COUNT:
		{
			strLogFile.Format(L"DBR_GP_CHKXJTZH_LOG_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnCHKXJTZHLogCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJYK_LOG_COUNT:
		{
			strLogFile.Format(L"DBR_GP_XJYK_LOG_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnXJYKLogCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJYK_TJ_COUNT:
		{
			strLogFile.Format(L"DBR_GP_XJYK_TJ_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnXJYKTjCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJYX_TJ_COUNT: //游戏盈亏日志数
		{
			strLogFile.Format(L"DBR_GP_XJYX_TJ_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnXJYXTjCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJCHZH_LOG_COUNT:
		{
			strLogFile.Format(L"DBR_GP_XJCHZH_LOG_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnXJCHZHLogCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJTX_LOG_COUNT:
		{
			strLogFile.Format(L"DBR_GP_XJTX_LOG_COUNT");
			LogFile::instance().LogText(strLogFile);
			return OnXJTxLogCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_CHONGZHI_TYPE:
		{
			strLogFile.Format(L"DBR_GP_GET_CHONGZHI_TYPE");
			LogFile::instance().LogText(strLogFile);
			return OnGetChongzhiXinxi(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_QUKUAN_INFO:
		{
			strLogFile.Format(L"DBR_GP_GET_QUKUAN_INFO");
			LogFile::instance().LogText(strLogFile);
			return OnGetQukuanInfo(dwContextID,pData,wDataSize);
		}
// 	case DBR_GP_QUERY_YINHANG:
// 		{
// 			return OnQueryYinHangName(dwContextID,pData,wDataSize);
// 		}
	case DBR_GP_GET_YUE_INFO:
		{
			strLogFile.Format(L"DBR_GP_GET_YUE_INFO");
			LogFile::instance().LogText(strLogFile);
			return OnGetYueInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_MORE_RECORD:
		{
			strLogFile.Format(L"DBR_GP_GET_MORE_RECORD");
			LogFile::instance().LogText(strLogFile);
			return OnGetMoreRecord(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_DAILI_HUIKUI:
		{
			strLogFile.Format(L"DBR_GP_GET_DAILI_HUIKUI");
			LogFile::instance().LogText(strLogFile);
			return OnGetDailiHuikui(dwContextID,pData,wDataSize);
		}
	case DBR_GP_DAILI_LJ:
		{
			strLogFile.Format(L"DBR_GP_DAILI_LJ");
			LogFile::instance().LogText(strLogFile);
			return OnDailiLingjiang(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUIT_GAME:
		{
			strLogFile.Format(L"DBR_GP_QUIT_GAME");
			LogFile::instance().LogText(strLogFile);
			return OnQuitGame(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_QIANDAO:
		{
			strLogFile.Format(L"DBR_GP_USER_QIANDAO");
			LogFile::instance().LogText(strLogFile);
			return OnUserQiandao(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_HUANLE_SONG:
		{
			strLogFile.Format(L"DBR_GP_USER_HUANLE_SONG");
			LogFile::instance().LogText(strLogFile);
			return OnGetUserHuanlesong(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_LING_JIANG:
		{
			strLogFile.Format(L"DBR_GP_USER_LING_JIANG");
			LogFile::instance().LogText(strLogFile);
			return OnGetUserLingJiang(dwContextID,pData,wDataSize);
		}
	case DBR_GP_DO_QUKUAN:
		{
			strLogFile.Format(L"DBR_GP_DO_QUKUAN");
			LogFile::instance().LogText(strLogFile);
			return OnDoQukuan(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TOUZHU_LOG:
		{
			strLogFile.Format(L"DBR_GP_TOUZHU_LOG");
			LogFile::instance().LogText(strLogFile);
			return OnGetTouzhuLog(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_XJCHZH_LOG: //下级充值日志
		{
			strLogFile.Format(L"DBR_GP_GET_XJCHZH_LOG");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJCHZHLog(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_XJTX_LOG: //下级提现日志
		{
			strLogFile.Format(L"DBR_GP_GET_XJTX_LOG");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJTxLog(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_XJYK_LOG:
		{
			strLogFile.Format(L"DBR_GP_GET_XJYK_LOG");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJYKLog(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_XJYK_TJ:
		{
			strLogFile.Format(L"DBR_GP_GET_XJYK_TJ");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJYKTj(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_XJYX_TJ:
		{
			strLogFile.Format(L"DBR_GP_GET_XJYX_TJ");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJYXTj(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_DA_LI_BAO:
		{
			strLogFile.Format(L"DBR_GP_GET_DA_LI_BAO");
			LogFile::instance().LogText(strLogFile);
			return OnGetDaLiBao(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_ZA_JIN_DAN:
		{
			strLogFile.Format(L"DBR_GP_GET_ZA_JIN_DAN");
			LogFile::instance().LogText(strLogFile);
			return OnGetZaJinDan(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_LUCKY_ZHUAN:
		{
			strLogFile.Format(L"DBR_GP_GET_LUCKY_ZHUAN");
			LogFile::instance().LogText(strLogFile);
			return OnGetLuckyZhuan(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CAN_DA_LI_BAO:
		{
			strLogFile.Format(L"DBR_GP_CAN_DA_LI_BAO");
			LogFile::instance().LogText(strLogFile);
			return OnCanDaLiBao(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CAN_ZA_JIN_DAN:
		{
			strLogFile.Format(L"DBR_GP_CAN_ZA_JIN_DAN");
			LogFile::instance().LogText(strLogFile);
			return OnCanZaJinDan(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CAN_LUCKY_ZHUAN:
		{
			strLogFile.Format(L"DBR_GP_CAN_LUCKY_ZHUAN");
			LogFile::instance().LogText(strLogFile);
			return OnCanLuckyZhuan(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJTZH_LOG:
		{
			strLogFile.Format(L"DBR_GP_XJTZH_LOG");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJTZHLog(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHKXJTZH_LOG:
		{
			strLogFile.Format(L"DBR_GP_CHKXJTZH_LOG");
			LogFile::instance().LogText(strLogFile);
			return OnCHKXJTZHLog(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJTZH_LOG_BY_ACT:
		{
			strLogFile.Format(L"DBR_GP_XJTZH_LOG_BY_ACT");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJTZHLogByAct(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJTX_LOG_BY_ACT:
		{
			strLogFile.Format(L"DBR_GP_XJTX_LOG_BY_ACT");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJTxLogByAct(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJTZH_LOG_BY_ID:
		{
			strLogFile.Format(L"DBR_GP_XJTZH_LOG_BY_ID");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJTZHLogByID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJTX_LOG_BY_ID:
		{
			strLogFile.Format(L"DBR_GP_XJTX_LOG_BY_ID");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJTXLogByID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHKXJTZH_LOG_BY_ACT:
		{
			strLogFile.Format(L"DBR_GP_CHKXJTZH_LOG_BY_ACT");
			LogFile::instance().LogText(strLogFile);
			return OnCHKXJTZHLogByAct(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJYK_LOG_BY_ID:
		{
			strLogFile.Format(L"DBR_GP_XJYK_LOG_BY_ID");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJYKLogByID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJYK_LOG_BY_ACT:
		{
			strLogFile.Format(L"DBR_GP_XJYK_LOG_BY_ACT");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJYKLogByAct(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJCHZH_LOG_BY_ID:   //通过ID查询下级充值日志
		{
			strLogFile.Format(L"DBR_GP_XJCHZH_LOG_BY_ID");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJCHZHLogByID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJCHZH_LOG_BY_ACT:
		{
			strLogFile.Format(L"DBR_GP_XJCHZH_LOG_BY_ACT");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJCHZHLogByAct(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJYK_TJ_BY_ID:
		{
			strLogFile.Format(L"DBR_GP_XJYK_TJ_BY_ID");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJYKTjByID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XJYK_TJ_BY_ACT:
		{
			strLogFile.Format(L"DBR_GP_XJYK_TJ_BY_ACT");
			LogFile::instance().LogText(strLogFile);
			return OnGetXJYKTjByAct(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHKXJTZH_LOG_BY_ID:
		{
			strLogFile.Format(L"DBR_GP_CHKXJTZH_LOG_BY_ID");
			LogFile::instance().LogText(strLogFile);
			return OnCHKXJTZHLogByID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_HYSHJ:
		{
			strLogFile.Format(L"DBR_GP_GET_HYSHJ");
			LogFile::instance().LogText(strLogFile);
			return OnGetHyShj(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TIXIAN_LOG:
		{
			strLogFile.Format(L"DBR_GP_TIXIAN_LOG");
			LogFile::instance().LogText(strLogFile);
			return OnGetTixianLog(dwContextID,pData,wDataSize);
		}
	case DBR_GP_HYXX_LOG:
		{
			strLogFile.Format(L"DBR_GP_HYXX_LOG");
			LogFile::instance().LogText(strLogFile);
			return OnGetHYXXLog(dwContextID,pData,wDataSize);
		}
	case DBR_GP_HYXX_LIST: //查询会员信息
		{
			strLogFile.Format(L"DBR_GP_HYXX_LIST");
			LogFile::instance().LogText(strLogFile);
			return OnGetHYXXList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_HYXX_LOG_BY_ID:
		{
			strLogFile.Format(L"DBR_GP_HYXX_LOG_BY_ID");
			LogFile::instance().LogText(strLogFile);
			return OnGetHYXXLogByID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_HYXX_LOG_BY_ACT:
		{
			strLogFile.Format(L"DBR_GP_HYXX_LOG_BY_ACT");
			LogFile::instance().LogText(strLogFile);
			return OnGetHYXXLogByAct(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XGHY_FANDIAN:
		{
			strLogFile.Format(L"DBR_GP_XGHY_FANDIAN");
			LogFile::instance().LogText(strLogFile);
			return OnXGHYFandian(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SW_DAILI:
		{
			strLogFile.Format(L"DBR_GP_SW_DAILI");
			LogFile::instance().LogText(strLogFile);
			return OnSWDaili(dwContextID,pData,wDataSize);
		}
	case DBR_GP_HYXX_ZHUANZHANG:		//会员信息转账
		{
			strLogFile.Format(L"DBR_GP_HYXX_ZHUANZHANG");
			LogFile::instance().LogText(strLogFile);
			return OnHYXXZhuanZhang(dwContextID,pData,wDataSize);
		}
	case DBR_GP_HYXX_GET_XJPEIE:		//获取下级配额
		{
			strLogFile.Format(L"DBR_GP_HYXX_GET_XJPEIE");
			LogFile::instance().LogText(strLogFile);
			return OnHYXXXiaJiPeie(dwContextID,pData,wDataSize);
		}
	case DBR_GP_HYXX_SET_XJPEIE:		//获取下级配额
		{
			strLogFile.Format(L"DBR_GP_HYXX_SET_XJPEIE");
			LogFile::instance().LogText(strLogFile);
			return OnHYXXSetXiaJiPeie(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHONGZHI_LOG:
		{
			strLogFile.Format(L"DBR_GP_CHONGZHI_LOG");
			LogFile::instance().LogText(strLogFile);
			return OnGetChongzhiLog(dwContextID,pData,wDataSize);
		}
	case DBR_GP_YINGKUI_LOG:
		{
			strLogFile.Format(L"DBR_GP_YINGKUI_LOG");
			LogFile::instance().LogText(strLogFile);
			return OnGetYingkuiLog(dwContextID,pData,wDataSize);
		}
	case DBR_GP_YINGKUI_MX:
		{
			strLogFile.Format(L"DBR_GP_YINGKUI_MX");
			LogFile::instance().LogText(strLogFile);
			return OnGetYingkuiMx(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QP_YINGKUI:
		{
			strLogFile.Format(L"DBR_GP_QP_YINGKUI");
			LogFile::instance().LogText(strLogFile);
			return OnGetQiPaiYingkui(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_REG_URL:
		{
			strLogFile.Format(L"DBR_GP_GET_REG_URL");
			LogFile::instance().LogText(strLogFile);
			return OnGetRegUrl(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_Peie:
		{
			strLogFile.Format(L"DBR_GP_GET_Peie");
			LogFile::instance().LogText(strLogFile);
			return OnGetPeie(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CANCEL_TOUZHU:
		{
			strLogFile.Format(L"DBR_GP_CANCEL_TOUZHU");
			LogFile::instance().LogText(strLogFile);
			return OnCancelTouzhu(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ADD_HY:
		{
			strLogFile.Format(L"DBR_GP_ADD_HY");
			LogFile::instance().LogText(strLogFile);
			return OnAddHuiyuan(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SET_WEB_FANDIAN:
		{
			strLogFile.Format(L"DBR_GP_SET_WEB_FANDIAN");
			LogFile::instance().LogText(strLogFile);
			return OnSetWebFandian(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHONGZHI_TISHI:
		{
			strLogFile.Format(L"DBR_GP_CHONGZHI_TISHI");
			LogFile::instance().LogText(strLogFile);
			return OnChongzhiTishi(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TIXIAN_TISHI:
		{
			strLogFile.Format(L"DBR_GP_TIXIAN_TISHI");
			LogFile::instance().LogText(strLogFile);
			return OnTixianTishi(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TOUZHU_TISHI:
		{
			strLogFile.Format(L"DBR_GP_TOUZHU_TISHI");
			LogFile::instance().LogText(strLogFile);
			return OnTouzhuTishi(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_USER_INFO:
		{
			strLogFile.Format(L"DBR_GP_GET_USER_INFO");
			LogFile::instance().LogText(strLogFile);
			return OnGetUserInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_NEWS:
		{
			strLogFile.Format(L"DBR_GP_GET_NEWS");
			LogFile::instance().LogText(strLogFile);
			return OnGetNewsInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ZHUAN_HUAN:
		{
			strLogFile.Format(L"DBR_GP_ZHUAN_HUAN");
			LogFile::instance().LogText(strLogFile);
			return OnZhuanhuan(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XG_QQ:
		{
			strLogFile.Format(L"DBR_GP_XG_QQ");
			LogFile::instance().LogText(strLogFile);
			return OnXGqq(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ALTER_GENDER:
		{
			strLogFile.Format(L"DBR_GP_ALTER_GENDER");
			LogFile::instance().LogText(strLogFile);
			return OnXGGender(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XG_LOGIN_PASS:
		{
			strLogFile.Format(L"DBR_GP_XG_LOGIN_PASS");
			LogFile::instance().LogText(strLogFile);
			return OnXGLoginPass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_XG_QUKUAN_PASS:
		{
			strLogFile.Format(L"DBR_GP_XG_QUKUAN_PASS");
			LogFile::instance().LogText(strLogFile);
			return OnXGQukuanPass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOCK_MACHINE:
		{
			strLogFile.Format(L"DBR_GP_LOCK_MACHINE");
			LogFile::instance().LogText(strLogFile);
			return OnLockMachine(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SET_QUKUAN_PROTECT:
		{
			strLogFile.Format(L"DBR_GP_SET_QUKUAN_PROTECT");
			LogFile::instance().LogText(strLogFile);
			return OnSetQukuanProtectPass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SET_QUKUAN_ZHANGHAO:
		{
			strLogFile.Format(L"DBR_GP_SET_QUKUAN_ZHANGHAO");
			LogFile::instance().LogText(strLogFile);
			return OnSetQukuanZhanghuPass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_MY_YINHANG:
		{
			strLogFile.Format(L"DBR_GP_QUERY_MY_YINHANG");
			LogFile::instance().LogText(strLogFile);
			return OnQueryMyYinHang(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_MY_PROTECT:
		{
			strLogFile.Format(L"DBR_GP_QUERY_MY_PROTECT");
			LogFile::instance().LogText(strLogFile);
			return OnQueryMyProtect(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_YINHANG:
		{
			strLogFile.Format(L"DBR_GP_QUERY_YINHANG");
			LogFile::instance().LogText(strLogFile);
			return OnQueryYinhang(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_TOUZHU_XX:
		{
			strLogFile.Format(L"DBR_GP_GET_TOUZHU_XX");
			LogFile::instance().LogText(strLogFile);
			return OnGetTouzhuXX(dwContextID,pData,wDataSize);
		}
	case DBR_MB_GET_TOUZHU_XX:
		{
			strLogFile.Format(L"DBR_GP_GET_TOUZHU_XX");
			LogFile::instance().LogText(strLogFile);
			return OnMBGetTouzhuXX(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//帐号登录
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_LogonAccounts));
		if (wDataSize!=sizeof(DBR_GP_LogonAccounts)) return false;

		//请求处理
		DBR_GP_LogonAccounts * pLogonAccounts=(DBR_GP_LogonAccounts *)pData;

		CString strLog;
		
		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		strLog.Format(L"执行判断 [%s] pBindParameter->dwSocketID:%d,dwContextID:%d",pLogonAccounts->szAccounts,pBindParameter->dwSocketID,dwContextID);
		LogFile::instance().LogText(strLog);
		if (pBindParameter->dwSocketID!=dwContextID)
		{
			strLog.Format(L"[%s] 判断错误 SocketID不同",pLogonAccounts->szAccounts);
			LogFile::instance().LogText(strLog);

			return true;
		}

		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		strLog.Format(L"LoginIP:%s",szClientAddr);
		CTraceService::TraceString(strLog,TraceLevel_Exception);
		//转换地址
		DWORD dwClientAddr= (DWORD)(((0xffff&pClientAddr[0])<<24)|((0xffff&pClientAddr[1])<<16)|((0xffff&pClientAddr[2])<<8)|(0xffff&pClientAddr[3]));

		strLog.Format(L"[%s] 执行存过之前 ",pLogonAccounts->szAccounts);
		LogFile::instance().LogText(strLog);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@dwClientIP"),(LONGLONG)dwClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@dwClientID"),2); // 1: 网页   2 pc android 

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);

		strLog.Format(L"[%s]执行存过[GSP_GP_EfficacyAccounts]之后 lResultCode：%d",pLogonAccounts->szAccounts,lResultCode);
		LogFile::instance().LogText(strLog);

		if(lResultCode != DB_SUCCESS)
		{
			CString strErrLog;
			strErrLog.Format(L"[%s]使用密码[%s]登录失败，IP为[%s]",pLogonAccounts->szAccounts,pLogonAccounts->szPassword,szClientAddr);
			LogFile::instance().LogText(strErrLog);
		}
		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{

		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GP_EfficacyAccounts");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CTraceService::TraceString(_T("This ok11?"),TraceLevel_Exception);
		ReconnectSql();
		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于网络问题，请稍后重新登录！"),false);

		//请求处理
		DBR_GP_LogonAccounts * pLogonAccounts=(DBR_GP_LogonAccounts *)pData;
		if(pLogonAccounts==NULL)
			return false;
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		CString strErrLog;
		strErrLog.Format(L"Exception [%s]使用密码[%s]登录失败，IP为[%s]",pLogonAccounts->szAccounts,pLogonAccounts->szPassword,szClientAddr);
		LogFile::instance().LogText(strErrLog);


		return false;
	}

	return true;
}

//注册处理
bool CDataBaseEngineSink::OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

		//请求处理
		DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//转换地址
		DWORD dwClientAddr= (DWORD)(((0xffff&pClientAddr[0])<<24)|((0xffff&pClientAddr[1])<<16)|((0xffff&pClientAddr[2])<<8)|(0xffff&pClientAddr[3]));

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);

		//EWIN网络添加
		m_AccountsDBAide.AddParameter(TEXT("@strQQ"),pRegisterAccounts->szQQ);
		m_AccountsDBAide.AddParameter(TEXT("@strIPhone"),pRegisterAccounts->szIPhone);
		m_AccountsDBAide.AddParameter(TEXT("@strEmain"),pRegisterAccounts->szEmain);
		


		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@dwClientIP"),(LONGLONG)dwClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GP_RegisterAccounts");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();
		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于链接服务器出现错误，请退出后重新登录！"),false);

		return false;
	}

	return true;
}

//帐号登录
bool CDataBaseEngineSink::OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_MB_LogonAccounts));
		if (wDataSize!=sizeof(DBR_MB_LogonAccounts)) return false;

		//请求处理
		DBR_MB_LogonAccounts * pLogonAccounts=(DBR_MB_LogonAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//转换地址
		DWORD dwClientAddr= (DWORD)(((0xffff&pClientAddr[0])<<24)|((0xffff&pClientAddr[1])<<16)|((0xffff&pClientAddr[2])<<8)|(0xffff&pClientAddr[3]));

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@dwClientIP"),(LONGLONG)dwClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyAccounts"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_MB_EfficacyAccounts");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();
		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于链接服务器出现错误，请退出后重新登录！"),true);

		return false;
	}

	return true;
}

//帐号注册
bool CDataBaseEngineSink::OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_MB_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_MB_RegisterAccounts)) return false;

		//请求处理
		DBR_MB_RegisterAccounts * pRegisterAccounts=(DBR_MB_RegisterAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_RegisterAccounts"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_MB_RegisterAccounts");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();
		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于链接服务器出现错误，请退出后重新登录！"),true);

		return false;
	}

	return true;
}
//撤单
bool CDataBaseEngineSink::OnCancelTouzhu(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CancelTouzhu));
		if (wDataSize!=sizeof(DBR_GP_CancelTouzhu)) return false;

		DBR_GP_CancelTouzhu* pCancelTouzhu = (DBR_GP_CancelTouzhu*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@t_id"),pCancelTouzhu->n_t_id);
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pCancelTouzhu->n_t_userid);


		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_cancel_touzhu"),true);


		DBO_GR_CancelTouzhuRet CancelTouzhuRet;
		ZeroMemory(&CancelTouzhuRet,sizeof(CancelTouzhuRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			CancelTouzhuRet.nResult = m_GameSSCDBAide.GetValue_INT(TEXT("result"));
		else
			CancelTouzhuRet.nResult = lResultCode;
		CancelTouzhuRet.n_t_userid = pCancelTouzhu->n_t_userid;
		CString strLog;
		strLog.Format(L"【%d】撤单",pCancelTouzhu->n_t_userid);
		LogFile::instance().LogText(strLog);

		m_GameSSCDBModule->CloseRecordset();
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CANCEL_TOUZHU_RET,dwContextID,&CancelTouzhuRet,sizeof(CancelTouzhuRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_cancel_touzhu");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//获取六合彩期号
bool CDataBaseEngineSink::OnRequestLhcQihao(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		m_GameSSCDBAide.ResetParameter();


		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_lhc_qihao"),true);


		DBO_GetLhcQihaoRet GetLhcQihaoRet;
		ZeroMemory(&GetLhcQihaoRet,sizeof(GetLhcQihaoRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			GetLhcQihaoRet.n_t_qihao = m_GameSSCDBAide.GetValue_INT(TEXT("qihao"));
			m_GameSSCDBAide.GetValue_SystemTime(TEXT("fdshijian"),GetLhcQihaoRet.n_t_fdshijian);

		}
		m_GameSSCDBModule->CloseRecordset();
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GET_LHC_QIHAO_RET,dwContextID,&GetLhcQihaoRet,sizeof(GetLhcQihaoRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_cancel_touzhu");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//添加会员
bool CDataBaseEngineSink::OnAddHuiyuan(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_AddHuiYuan));
		if (wDataSize!=sizeof(DBR_GP_AddHuiYuan)) return false;

		DBR_GP_AddHuiYuan* pAddHuiyuan = (DBR_GP_AddHuiYuan*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pAddHuiyuan->n_t_daili_id);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pAddHuiyuan->n_type);
		m_GameSSCDBAide.AddParameter(TEXT("@fandian"),pAddHuiyuan->f_t_fandian);
		m_GameSSCDBAide.AddParameter(TEXT("@account"),pAddHuiyuan->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@login_pwd"),pAddHuiyuan->s_t_login_pass);
		m_GameSSCDBAide.AddParameter(TEXT("@qk_pwd"),pAddHuiyuan->s_t_qukuan_pass);
		m_GameSSCDBAide.AddParameter(TEXT("@qq"),pAddHuiyuan->s_t_qq);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_add_huiyuan"),true);


		DBO_GR_AddHuiYuanRet AddHYRet;
		ZeroMemory(&AddHYRet,sizeof(AddHYRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			AddHYRet.n_t_res = m_GameSSCDBAide.GetValue_INT(TEXT("res"));
			m_GameSSCDBAide.GetValue_String(TEXT("DESCRIBE"),AddHYRet.s_t_Desc,sizeof(AddHYRet.s_t_Desc));

		}
		else
		{
			AddHYRet.n_t_res = lResultCode;

		}
		m_GameSSCDBModule->CloseRecordset();
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ADD_HY_RET,dwContextID,&AddHYRet,sizeof(AddHYRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_add_huiyuan");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//设置网页注册返点
bool CDataBaseEngineSink::OnSetWebFandian(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_SetWebFandian));
		if (wDataSize!=sizeof(DBR_GP_SetWebFandian)) return false;

		DBR_GP_SetWebFandian* pSetWebFandian = (DBR_GP_SetWebFandian*)pData;

		if(pSetWebFandian->f_t_fandian>0.0250001)
		{
			DBO_GR_SetWebFandianRet SetWebFandian;
			ZeroMemory(&SetWebFandian,sizeof(SetWebFandian));

			SetWebFandian.n_t_res = 2;
			SetWebFandian.f_t_fandian = 2.5;
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SET_WEB_FANDIAN_RESULT,dwContextID,&SetWebFandian,sizeof(SetWebFandian));

		}
		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pSetWebFandian->n_t_userid);
		m_GameSSCDBAide.AddParameter(TEXT("@fandian"),pSetWebFandian->f_t_fandian);


		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_set_web_fandian"),false);


		DBO_GR_SetWebFandianRet SetWebFandian;
		ZeroMemory(&SetWebFandian,sizeof(SetWebFandian));

		SetWebFandian.n_t_res = lResultCode;
		SetWebFandian.f_t_fandian = 2.5;
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SET_WEB_FANDIAN_RESULT,dwContextID,&SetWebFandian,sizeof(SetWebFandian));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_set_web_fandian");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//获取玩家信息
bool CDataBaseEngineSink::OnGetUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetUserInfo));
		if (wDataSize!=sizeof(DBR_GP_GetUserInfo)) return false;

		DBR_GP_GetUserInfo* pUserInfo = (DBR_GP_GetUserInfo*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pUserInfo->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_wdzh_userinfo"),true);


		DBO_GR_GetUserInfoRet GetUserInfoRet;
		ZeroMemory(&GetUserInfoRet,sizeof(GetUserInfoRet));
	
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
		
			m_GameSSCDBAide.GetValue_String(TEXT("t_qq"),GetUserInfoRet.s_t_qq,sizeof(GetUserInfoRet.s_t_qq));
			m_GameSSCDBAide.GetValue_SystemTime(TEXT("REGTIME"),GetUserInfoRet.t_reg_time);
			m_GameSSCDBAide.GetValue_String(TEXT("shangjiqq"),GetUserInfoRet.s_t_sjqq,sizeof(GetUserInfoRet.s_t_sjqq));
 			GetUserInfoRet.f_t_yongjin = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("yongjin"));
 			GetUserInfoRet.f_t_yongjinze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("yongjinze"));
 			GetUserInfoRet.f_t_fenhong = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("fenhong"));
 			GetUserInfoRet.f_t_fenhongze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("fenhongze"));
 			GetUserInfoRet.f_t_qipaiye = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("qipaiyue"));

			GetUserInfoRet.f_t_kuisunyongjin = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("kuisunyongjin"));
			GetUserInfoRet.f_t_kuisunyongjinze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("kuisunyongjinze"));
			
			m_GameSSCDBAide.GetValue_SystemTime(TEXT("fenhongshijian"),GetUserInfoRet.t_fh_time);
			m_GameSSCDBAide.GetValue_SystemTime(TEXT("yongjinjian"),GetUserInfoRet.t_yj_time);
			m_GameSSCDBAide.GetValue_SystemTime(TEXT("ksyongjinjian"),GetUserInfoRet.t_ksyj_time);


		}
		m_GameSSCDBModule->CloseRecordset();
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_USER_INFO_RET,dwContextID,&GetUserInfoRet,sizeof(GetUserInfoRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_wdzh_userinfo");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//修改QQ
bool CDataBaseEngineSink::OnXGqq(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_XGqq));
		if (wDataSize!=sizeof(DBR_GP_XGqq)) return false;

		DBR_GP_XGqq* pUserInfo = (DBR_GP_XGqq*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pUserInfo->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@user_qq"),pUserInfo->szQQ);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_xg_qq"),false);


		DBO_GR_XG_QQ_RET XGqqRet;
		ZeroMemory(&XGqqRet,sizeof(XGqqRet));

		XGqqRet.lResult = lResultCode;		

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XG_QQ_RET,dwContextID,&XGqqRet,sizeof(XGqqRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_xg_qq");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//修改性别
bool CDataBaseEngineSink::OnXGGender(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_AlterGender));
		if (wDataSize!=sizeof(DBR_GP_AlterGender)) return false;

		DBR_GP_AlterGender* pUserInfo = (DBR_GP_AlterGender*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pUserInfo->n_t_userid);
		m_GameSSCDBAide.AddParameter(TEXT("@user_gender"),pUserInfo->cbGender);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_xg_gender"),false);


		DBO_GP_AlterGenderRet XGGenderRet;
		ZeroMemory(&XGGenderRet,sizeof(XGGenderRet));

		XGGenderRet.n_t_return = lResultCode;		

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ALTER_GENDER_RET,dwContextID,&XGGenderRet,sizeof(XGGenderRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_xg_gender");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//修改会员返点
bool CDataBaseEngineSink::OnXGHYFandian(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_XGHYFandian));
		if (wDataSize!=sizeof(DBR_GP_XGHYFandian)) return false;

		DBR_GP_XGHYFandian* pUserInfo = (DBR_GP_XGHYFandian*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pUserInfo->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pUserInfo->n_xiaji_id);
		m_GameSSCDBAide.AddParameter(TEXT("@t_fandian"),pUserInfo->f_Fandian);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_xg_hy_fandian"),true);


		DBO_GP_XGHYFandian_RET XGHYFandianRet;
		ZeroMemory(&XGHYFandianRet,sizeof(XGHYFandianRet));
		XGHYFandianRet.lResult = lResultCode;
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			XGHYFandianRet.f_Fandian = pUserInfo->f_Fandian;
			m_GameSSCDBAide.GetValue_String(TEXT("t_account"),XGHYFandianRet.s_Account,sizeof(XGHYFandianRet.s_Account));

		}

		m_GameSSCDBModule->CloseRecordset();
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XGHY_FANDIAN_RET,dwContextID,&XGHYFandianRet,sizeof(XGHYFandianRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_xg_hy_fandian");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//设置为代理
bool CDataBaseEngineSink::OnSWDaili(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_SWDaili));
		if (wDataSize!=sizeof(DBR_GP_SWDaili)) return false;

		DBR_GP_SWDaili* pUserInfo = (DBR_GP_SWDaili*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pUserInfo->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@nAgentID"),pUserInfo->nAgentID);
		m_GameSSCDBAide.AddParameter(TEXT("@isagent"),pUserInfo->nDaili);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_set_user_agent"),false);


		DBO_GP_SWDaili_RET XGHYFandianRet;
		ZeroMemory(&XGHYFandianRet,sizeof(XGHYFandianRet));

		XGHYFandianRet.lResult = lResultCode;

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SW_DAILI_RET,dwContextID,&XGHYFandianRet,sizeof(XGHYFandianRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_set_user_agent");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//会员信息转账
bool CDataBaseEngineSink::OnHYXXZhuanZhang(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_HYXX_ZhuanZhang));
		if (wDataSize!=sizeof(DBR_GP_HYXX_ZhuanZhang)) return false;

		DBR_GP_HYXX_ZhuanZhang* pUserInfo = (DBR_GP_HYXX_ZhuanZhang*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pUserInfo->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@targetID"),pUserInfo->n_t_target_id);
		m_GameSSCDBAide.AddParameter(TEXT("@jine"),pUserInfo->f_t_jine);
		m_GameSSCDBAide.AddParameter(TEXT("@password"),pUserInfo->s_t_password);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_hyxx_zhuanzhang"),false);


		DBO_GP_HYXX_ZhuanZhang_RET ZhuanZhang;
		ZeroMemory(&ZhuanZhang,sizeof(ZhuanZhang));

		ZhuanZhang.lResult = lResultCode;

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_ZHUANZHANG_RET,dwContextID,&ZhuanZhang,sizeof(ZhuanZhang));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_hyxx_zhuanzhang");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}

//修改登录密码
bool CDataBaseEngineSink::OnXGLoginPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_XG_Loginpass));
		if (wDataSize!=sizeof(DBR_GP_XG_Loginpass)) return false;

		DBR_GP_XG_Loginpass* pUserInfo = (DBR_GP_XG_Loginpass*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pUserInfo->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@old_pwd"),pUserInfo->szOldPass);
		m_GameSSCDBAide.AddParameter(TEXT("@new_pwd"),pUserInfo->szNewPass);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_modify_pwd"),false);


		DBO_GR_XG_Loginpass_RET XGLoginpassRet;
		ZeroMemory(&XGLoginpassRet,sizeof(XGLoginpassRet));

		XGLoginpassRet.lResult = lResultCode;		

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XG_LOGIN_PASS_RET,dwContextID,&XGLoginpassRet,sizeof(XGLoginpassRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_modify_pwd");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//锁定机器
bool CDataBaseEngineSink::OnLockMachine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_LockMachine));
		if (wDataSize!=sizeof(DBR_GP_LockMachine)) return false;
		DBR_GP_LockMachine* pLockMachine = (DBR_GP_LockMachine*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLockMachine->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@lock"),pLockMachine->cbLock);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_lock_machine"),false);

		DBO_GR_LockMachine_RET LockMachine;
		ZeroMemory(&LockMachine,sizeof(LockMachine));

		LockMachine.lResult = lResultCode;
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOCK_MACHINE_RET,dwContextID,&LockMachine,sizeof(LockMachine));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_lock_machine");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

}
//修改取款密码
bool CDataBaseEngineSink::OnXGQukuanPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_XG_Qukuanpass));
		if (wDataSize!=sizeof(DBR_GP_XG_Qukuanpass)) return false;

		DBR_GP_XG_Qukuanpass* pUserInfo = (DBR_GP_XG_Qukuanpass*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pUserInfo->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@old_pwd"),pUserInfo->szOldPass);
		m_GameSSCDBAide.AddParameter(TEXT("@new_pwd"),pUserInfo->szNewPass);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_xg_qk_pwd"),false);


		DBO_GR_XG_Qukuanpass_RET XGLoginpassRet;
		ZeroMemory(&XGLoginpassRet,sizeof(XGLoginpassRet));

		XGLoginpassRet.lResult = lResultCode;		

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XG_QUKUAN_PASS_RET,dwContextID,&XGLoginpassRet,sizeof(XGLoginpassRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_xg_qk_pwd");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//设置取款保护
bool CDataBaseEngineSink::OnSetQukuanProtectPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_SetQukuanProtect));
		if (wDataSize!=sizeof(DBR_GP_SetQukuanProtect)) return false;

		DBR_GP_SetQukuanProtect* pUserInfo = (DBR_GP_SetQukuanProtect*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pUserInfo->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@qk_pwd"),pUserInfo->szQukuanPass);
		m_GameSSCDBAide.AddParameter(TEXT("@mb_ask"),pUserInfo->szQukuanAsk);
		m_GameSSCDBAide.AddParameter(TEXT("@mb_answer"),pUserInfo->szQukuanAnswer);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_set_mb"),false);


		DBO_GR_SetQukuanProtect_RET SetQukuanProtect;
		ZeroMemory(&SetQukuanProtect,sizeof(SetQukuanProtect));

		SetQukuanProtect.lResult = lResultCode;		

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SET_QUKUAN_PROTECT_RET,dwContextID,&SetQukuanProtect,sizeof(SetQukuanProtect));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_set_mb");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//设置取款账户
bool CDataBaseEngineSink::OnSetQukuanZhanghuPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_SetQukuanZhanghao));
		if (wDataSize!=sizeof(DBR_GP_SetQukuanZhanghao)) return false;

		DBR_GP_SetQukuanZhanghao* pUserInfo = (DBR_GP_SetQukuanZhanghao*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pUserInfo->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@khyh"),pUserInfo->szKaihuYinghang);
		m_GameSSCDBAide.AddParameter(TEXT("@khr"),pUserInfo->szKaihuRen);
		m_GameSSCDBAide.AddParameter(TEXT("@yhzhh"),pUserInfo->szYinhangZhanghu);
		m_GameSSCDBAide.AddParameter(TEXT("@qkmm"),pUserInfo->szQukuanPass);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_set_tikuan"),false);


		DBO_GR_SetQukuanZhanghao_RET SetQukuanZhanghao;
		ZeroMemory(&SetQukuanZhanghao,sizeof(SetQukuanZhanghao));

		SetQukuanZhanghao.lResult = lResultCode;		

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SET_QUKUAN_ZHANGHAO_RET,dwContextID,&SetQukuanZhanghao,sizeof(SetQukuanZhanghao));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_set_tikuan");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
 //查询取款账户
bool CDataBaseEngineSink::OnQueryMyYinHang(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBO_GP_QueryMyYinHang));
		if (wDataSize!=sizeof(DBO_GP_QueryMyYinHang)) return false;

		DBO_GP_QueryMyYinHang* pQueryMyYinHang = (DBO_GP_QueryMyYinHang*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pQueryMyYinHang->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_my_tikuanzhanghu"),true);


		DBR_GR_QueryMyYinHang_RET QueryMyYinHang_RET;
		ZeroMemory(&QueryMyYinHang_RET,sizeof(QueryMyYinHang_RET));

		QueryMyYinHang_RET.lResult = lResultCode;	

		if(lResultCode == 0 && !m_GameSSCDBModule->IsRecordsetEnd())
		{
			m_GameSSCDBAide.GetValue_String(_T("yinhang"),QueryMyYinHang_RET.szKaihuYinghang,sizeof(QueryMyYinHang_RET.szKaihuYinghang));
			m_GameSSCDBAide.GetValue_String(_T("kaihuren"),QueryMyYinHang_RET.szKaihuRen,sizeof(QueryMyYinHang_RET.szKaihuRen));
			m_GameSSCDBAide.GetValue_String(_T("yinhangzhanghu"),QueryMyYinHang_RET.szYinhangZhanghu,sizeof(QueryMyYinHang_RET.szYinhangZhanghu));
		}

		m_GameSSCDBModule->CloseRecordset();
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_MY_YINHANG_RET,dwContextID,&QueryMyYinHang_RET,sizeof(QueryMyYinHang_RET));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_my_tikuanzhanghu");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
 //查询账户保护
bool CDataBaseEngineSink::OnQueryMyProtect(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_QueryMyProTect));
		if (wDataSize!=sizeof(DBR_GP_QueryMyProTect)) return false;

		DBR_GP_QueryMyProTect* pQueryMyProtect = (DBR_GP_QueryMyProTect*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pQueryMyProtect->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_my_tikuanbaohu"),true);


		DBO_GR_QueryMyProTect_RET QueryMyProtect_RET;
		ZeroMemory(&QueryMyProtect_RET,sizeof(QueryMyProtect_RET));

		QueryMyProtect_RET.lResult = lResultCode;	

		if(lResultCode == 0 && !m_GameSSCDBModule->IsRecordsetEnd())
		{
			m_GameSSCDBAide.GetValue_String(_T("question"),QueryMyProtect_RET.szQuestion,sizeof(QueryMyProtect_RET.szQuestion));
			m_GameSSCDBAide.GetValue_String(_T("answer"),QueryMyProtect_RET.szAnswer,sizeof(QueryMyProtect_RET.szAnswer));
		}

		m_GameSSCDBModule->CloseRecordset();
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_MY_PROTECT_RET,dwContextID,&QueryMyProtect_RET,sizeof(QueryMyProtect_RET));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_my_tikuanbaohu");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//投注详细信息
bool CDataBaseEngineSink::OnMBGetTouzhuXX(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_MB_GetTouzhu));
		if (wDataSize!=sizeof(DBR_MB_GetTouzhu)) return false;

		DBR_MB_GetTouzhu* pUserInfo = (DBR_MB_GetTouzhu*)pData;

		CString strLogFile;
		strLogFile.Format(L"ExecuteProcess before %d",pUserInfo->n_t_id);
		LogFile::instance().LogText(strLogFile);

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@t_id"),pUserInfo->n_t_id);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_touzhuxx"),true);


	//	char szTempHaoma[991600]=("");
		DBO_MB_GetTouzhuRet GetTouzhuRet;
		ZeroMemory(&GetTouzhuRet,sizeof(GetTouzhuRet));
	//	USES_CONVERSION;

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetTouzhuRet.s_t_wanjiazhanghao,sizeof(GetTouzhuRet.s_t_wanjiazhanghao));
			GetTouzhuRet.n_t_dingdanhao = m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));	
			m_GameSSCDBAide.GetValue_String(TEXT("t_qishu"),GetTouzhuRet.s_t_qishu,sizeof(GetTouzhuRet.s_t_qishu));
			GetTouzhuRet.n_t_gametype = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));	
			GetTouzhuRet.n_t_gamekind = m_GameSSCDBAide.GetValue_INT(TEXT("t_kind"));	
			m_GameSSCDBAide.GetValue_String(TEXT("t_haoma"),	GetTouzhuRet.s_t_touzhuhaoma,	sizeof(GetTouzhuRet.s_t_touzhuhaoma));
			m_GameSSCDBAide.GetValue_String(TEXT("kjcode"),GetTouzhuRet.s_t_kaijianghaoma,sizeof(GetTouzhuRet.s_t_kaijianghaoma));
			m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetTouzhuRet.n_t_time);
			GetTouzhuRet.n_t_state = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));	
			GetTouzhuRet.n_t_zhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_zhushu"));	
			GetTouzhuRet.n_t_moshi = m_GameSSCDBAide.GetValue_INT(TEXT("t_moshi"));	
			GetTouzhuRet.n_t_beishu = m_GameSSCDBAide.GetValue_INT(TEXT("t_beishu"));	
			GetTouzhuRet.n_t_winzhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_winzhushu"));	
			GetTouzhuRet.f_t_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yingkui"));
			GetTouzhuRet.f_t_danzhujine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_bonus"));
			GetTouzhuRet.f_t_fandian = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_fandian"));
		}

		strLogFile.Format(L"GetValue DBO_MB_GET_TOUZHU_XX_RET");
		LogFile::instance().LogText(strLogFile);


		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_TOUZHU_XX_RET,dwContextID,&GetTouzhuRet,sizeof(GetTouzhuRet));
  

		m_GameSSCDBModule->CloseRecordset();

		return true;//m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_GET_TOUZHU_XX_RET,dwContextID,&GetTouzhuRet,sizeof(GetTouzhuRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception MB p_get_touzhuxx");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}


	return true;


}
bool	CDataBaseEngineSink::ChangeStringToA(CString strInput, string&  strOutput)
 {
 	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
 	char *str=new char[len];
 	memset(str,0,len);
 	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
 	strOutput=str;
 	delete[] str;
 	return true;
 }

//投注详细信息
bool CDataBaseEngineSink::OnGetTouzhuXX(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetTouzhu));
		if (wDataSize!=sizeof(DBR_GP_GetTouzhu)) return false;

		DBR_GP_GetTouzhu* pUserInfo = (DBR_GP_GetTouzhu*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@t_id"),pUserInfo->n_t_id);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_touzhuxx"),true);


		char szTempHaoma[991600]=("");
		DBO_GP_GetTouzhuRet GetTouzhuRet;
		ZeroMemory(&GetTouzhuRet,sizeof(GetTouzhuRet));
	//	USES_CONVERSION;

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetTouzhuRet.s_t_wanjiazhanghao,sizeof(GetTouzhuRet.s_t_wanjiazhanghao));
			GetTouzhuRet.n_t_dingdanhao = m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));	
			m_GameSSCDBAide.GetValue_String(TEXT("t_qishu"),GetTouzhuRet.s_t_qishu,sizeof(GetTouzhuRet.s_t_qishu));
			GetTouzhuRet.n_t_gametype = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));	
			GetTouzhuRet.n_t_gamekind = m_GameSSCDBAide.GetValue_INT(TEXT("t_kind"));	
			m_GameSSCDBAide.GetValue_String(TEXT("t_haoma"),	szTempHaoma,	sizeof(szTempHaoma));
			m_GameSSCDBAide.GetValue_String(TEXT("kjcode"),GetTouzhuRet.s_t_kaijianghaoma,sizeof(GetTouzhuRet.s_t_kaijianghaoma));
			m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetTouzhuRet.n_t_time);
			GetTouzhuRet.n_t_state = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));	
			GetTouzhuRet.n_t_zhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_zhushu"));	
			GetTouzhuRet.n_t_moshi = m_GameSSCDBAide.GetValue_INT(TEXT("t_moshi"));	
			GetTouzhuRet.n_t_beishu = m_GameSSCDBAide.GetValue_INT(TEXT("t_beishu"));	
			GetTouzhuRet.n_t_winzhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_winzhushu"));	
			GetTouzhuRet.f_t_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yingkui"));
			GetTouzhuRet.f_t_danzhujine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_bonus"));
			GetTouzhuRet.f_t_fandian = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_fandian"));
		}

		int wSendSize = 0;
		int nEnd = 0;

		int nLength = strlen(szTempHaoma);


// 		string strAllHaoma;
// 
// 		strAllHaoma = szTempHaoma
		//ChangeStringToA(strAHaoma, strAllHaoma);
		//byte pDestBuffer[512000];

 		int nSize = sizeof(GetTouzhuRet.s_t_touzhuhaoma);


		int nLen = CWHService::LZCompressData((byte*)szTempHaoma, nLength, pDestBuffer, sizeof(pDestBuffer));


  		int nIndex = 0;
  		GetTouzhuRet.n_t_End = 1;
  		while (wSendSize<nLen)
  		{
  
  			if(wSendSize + nSize >= nLen)
  				GetTouzhuRet.n_t_End = 0;
  
  			int nHaoMaLen = 0;
  			nHaoMaLen = min(nSize,nLen-wSendSize);
  			GetTouzhuRet.nHaoMaLen = nHaoMaLen;	//nLen;
 			GetTouzhuRet.n_Index = nIndex;
  			memset(GetTouzhuRet.s_t_touzhuhaoma, 0, sizeof(GetTouzhuRet.s_t_touzhuhaoma));
  			memcpy((byte*)GetTouzhuRet.s_t_touzhuhaoma, pDestBuffer+wSendSize, nSize);

// 			CString strLog;
// 			strLog.Format(L"\nTZHXX zhanghao:%s,dingdanhao:%d,qishu:%s,gametype:%d,gamekind:%d,haoma:%s",GetTouzhuRet.s_t_wanjiazhanghao,GetTouzhuRet.n_t_dingdanhao,GetTouzhuRet.s_t_qishu,GetTouzhuRet.n_t_gametype,GetTouzhuRet.n_t_gamekind,ChangeStringToT(szTempHaoma));
// 			OutputDebugString(strLog);

  			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_TOUZHU_XX_RET,dwContextID,&GetTouzhuRet,sizeof(GetTouzhuRet));
  
  			nIndex++;
  			wSendSize += nHaoMaLen;
  
  			if(wSendSize >= nLen)
  				break;
  
  		}

		m_GameSSCDBModule->CloseRecordset();

		return true;//m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_TOUZHU_XX_RET,dwContextID,&GetTouzhuRet,sizeof(GetTouzhuRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_touzhuxx");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}


	return true;


}
//获取会员数据
bool CDataBaseEngineSink::OnGetHyShj(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetHyShj));
		if (wDataSize!=sizeof(DBR_GP_GetHyShj)) return false;

		DBR_GP_GetHyShj* pUserInfo = (DBR_GP_GetHyShj*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pUserInfo->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@only_today"),pUserInfo->cbOnlyToday);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pUserInfo->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pUserInfo->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_huiyuantongji"),true);


		DBO_GR_GetHyShjRet GetHyShjRet;
		ZeroMemory(&GetHyShjRet,sizeof(GetHyShjRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			GetHyShjRet.n_t_xjrs = m_GameSSCDBAide.GetValue_INT(TEXT("xiajirenshu"));
			GetHyShjRet.n_t_xjzx = m_GameSSCDBAide.GetValue_INT(TEXT("xiajionline"));
			GetHyShjRet.f_t_hyye = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("huiyuanyue"));
			GetHyShjRet.n_t_jinrizhuce = m_GameSSCDBAide.GetValue_INT(TEXT("jinrizhuce"));
			GetHyShjRet.f_caipiao_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("caipiaoyingkui"));
			GetHyShjRet.f_t_xianma = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("xianmaliang"));
			GetHyShjRet.f_qp_fandian = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("qipaifandian"));
			GetHyShjRet.f_qp_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("qipaiyingkui"));
			GetHyShjRet.f_my_qp_fandian = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("myqipaifandian"));

			GetHyShjRet.f_wodefandian = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("wodefandian"));
			GetHyShjRet.f_t_jrfd = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("jinrifandian"));
			GetHyShjRet.f_zhongjiang_zonge = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("zhongjiangzonge"));
			GetHyShjRet.f_t_jrchz = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("jinrichongzhi"));
			GetHyShjRet.f_t_jrqk = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("jinriqukuan"));
			GetHyShjRet.f_t_jrtzh = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("jinritouzhu"));
			GetHyShjRet.f_t_jrhd = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("jinrihuodong"));
			GetHyShjRet.f_chedan_zonge = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("chedanzonge"));
			GetHyShjRet.f_touzhuyongjin = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("touzhuyongjin"));
			GetHyShjRet.f_kuisunyongjin = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("kuisunyongjin"));
		}
		m_GameSSCDBModule->CloseRecordset();
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_HYSHJ_RET,dwContextID,&GetHyShjRet,sizeof(GetHyShjRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_huiyuantongji");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//获取大礼包数据
bool CDataBaseEngineSink::OnGetDaLiBao(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetDaLiBao));
		if (wDataSize!=sizeof(DBR_GP_GetDaLiBao)) return false;

		DBR_GP_GetDaLiBao* pUserInfo = (DBR_GP_GetDaLiBao*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pUserInfo->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_dalibao"),true);


		int res = 0;
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			m_GameSSCDBAide.GetValue_INT(TEXT("res"));
		DBO_GP_GetDaLiBaoRet GetDaLiBaoRet;
		ZeroMemory(&GetDaLiBaoRet,sizeof(GetDaLiBaoRet));

		if(res==0)
		{
			if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
				GetDaLiBaoRet.f_t_jine = m_GameSSCDBAide.GetValue_INT(TEXT("jine"));
		}

		m_GameSSCDBModule->CloseRecordset();

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_DA_LI_BAO_RET,dwContextID,&GetDaLiBaoRet,sizeof(GetDaLiBaoRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_dalibao");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//获取大礼包数据
bool CDataBaseEngineSink::OnGetZaJinDan(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetZaJinDan));
		if (wDataSize!=sizeof(DBR_GP_GetZaJinDan)) return false;

		DBR_GP_GetZaJinDan* pUserInfo = (DBR_GP_GetZaJinDan*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pUserInfo->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@dan_type"),pUserInfo->nType);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_zajindan"),true);


		int res = 0;
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			res = m_GameSSCDBAide.GetValue_INT(TEXT("res"));
		DBO_GP_GetZaJinDanRet GetZaJinDan;
		ZeroMemory(&GetZaJinDan,sizeof(GetZaJinDan));

		if(res==0)
		{
			if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
				GetZaJinDan.f_t_jine = m_GameSSCDBAide.GetValue_INT(TEXT("jine"));
		}

		m_GameSSCDBModule->CloseRecordset();

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_ZA_JIN_DAN_RET,dwContextID,&GetZaJinDan,sizeof(GetZaJinDan));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_zajindan");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//获取大礼包数据
bool CDataBaseEngineSink::OnGetLuckyZhuan(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetLuckyZhuan));
		if (wDataSize!=sizeof(DBR_GP_GetLuckyZhuan)) return false;

		DBR_GP_GetLuckyZhuan* pUserInfo = (DBR_GP_GetLuckyZhuan*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pUserInfo->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_UserLuckyZhuanzhuan"),true);


		int res = 0;
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			res = m_GameSSCDBAide.GetValue_INT(TEXT("res"));
		DBO_GP_GetLuckyZhuanRet GetLuckyZhuan;
		ZeroMemory(&GetLuckyZhuan,sizeof(GetLuckyZhuan));

		GetLuckyZhuan.n_t_got = res;
		if(res==0)
		{
			if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
				GetLuckyZhuan.f_t_jine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("prize"));
		}

		m_GameSSCDBModule->CloseRecordset();

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_LUCKY_ZHUAN_RET,dwContextID,&GetLuckyZhuan,sizeof(GetLuckyZhuan));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_UserLuckyZhuanzhuan");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//获取大礼包资格
bool CDataBaseEngineSink::OnCanDaLiBao(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CanDaLiBao));
		if (wDataSize!=sizeof(DBR_GP_CanDaLiBao)) return false;

		DBR_GP_CanDaLiBao* pUserInfo = (DBR_GP_CanDaLiBao*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pUserInfo->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_can_dalibao"),true);


		DBO_GP_CanDaLiBaoRet GetDaLiBaoRet;
		ZeroMemory(&GetDaLiBaoRet,sizeof(GetDaLiBaoRet));
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			GetDaLiBaoRet.f_t_xiaofei = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("xiaofei"));
			GetDaLiBaoRet.f_t_chongzhi = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("chongzhi"));
			GetDaLiBaoRet.f_t_yue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yue"));
			GetDaLiBaoRet.n_t_got = m_GameSSCDBAide.GetValue_INT(TEXT("got"));

		}
		m_GameSSCDBModule->CloseRecordset();
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CAN_DA_LI_BAO_RET,dwContextID,&GetDaLiBaoRet,sizeof(GetDaLiBaoRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_can_dalibao");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//获取砸金蛋资格
bool CDataBaseEngineSink::OnCanZaJinDan(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CanZaJinDan));
		if (wDataSize!=sizeof(DBR_GP_CanZaJinDan)) return false;

		DBR_GP_CanZaJinDan* pUserInfo = (DBR_GP_CanZaJinDan*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pUserInfo->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_can_zajindan"),true);


		DBO_GP_CanZaJinDanRet GetZaJinDanRet;
		ZeroMemory(&GetZaJinDanRet,sizeof(GetZaJinDanRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			GetZaJinDanRet.f_t_xiaofei = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("xiaofei"));
			GetZaJinDanRet.n_t_got = m_GameSSCDBAide.GetValue_INT(TEXT("got"));

		}
		m_GameSSCDBModule->CloseRecordset();

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CAN_ZA_JIN_DAN_RET,dwContextID,&GetZaJinDanRet,sizeof(GetZaJinDanRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_can_zajindan");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//获取幸运转盘资格
bool CDataBaseEngineSink::OnCanLuckyZhuan(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CanLuckyZhuan));
		if (wDataSize!=sizeof(DBR_GP_CanLuckyZhuan)) return false;

		DBR_GP_CanLuckyZhuan* pUserInfo = (DBR_GP_CanLuckyZhuan*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pUserInfo->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_CanUserLuckyZhuanzhuan"),true);


		int res = 0;
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			res =m_GameSSCDBAide.GetValue_INT(TEXT("res"));
		DBO_GP_CanLuckyZhuanRet GetLuckyZhuanRet;
		ZeroMemory(&GetLuckyZhuanRet,sizeof(GetLuckyZhuanRet));

		if(res == 0)
		{
			if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
				GetLuckyZhuanRet.f_t_xiaofei = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("xiaofei"));
		}
		m_GameSSCDBModule->CloseRecordset();

		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CAN_LUCKY_ZHUAN_RET,dwContextID,&GetLuckyZhuanRet,sizeof(GetLuckyZhuanRet));

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_CanUserLuckyZhuanzhuan");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//充值提示
bool CDataBaseEngineSink::OnChongzhiTishi(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetChonzhiTishi));
		if (wDataSize!=sizeof(DBR_GP_GetChonzhiTishi)) return false;

		DBR_GP_GetChonzhiTishi* pGetChonzhiTishi = (DBR_GP_GetChonzhiTishi*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pGetChonzhiTishi->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_chongzhi_tishi"),true);

		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_ChongzhiTishiRet ChongzhiTishiRet[20];
			ZeroMemory(&ChongzhiTishiRet,sizeof(ChongzhiTishiRet));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHONGZHI_TISHI_RET,dwContextID,&ChongzhiTishiRet,sizeof(DBO_GR_ChongzhiTishiRet)*lResult);
					ZeroMemory(&ChongzhiTishiRet,sizeof(ChongzhiTishiRet));
					lResult = 0;
				}

				ChongzhiTishiRet[lResult].fJine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_jine"));
				ChongzhiTishiRet[lResult].nState = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();

			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHONGZHI_TISHI_RET,dwContextID,&ChongzhiTishiRet,sizeof(DBO_GR_ChongzhiTishiRet)*lResult);

		}
		return true;

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_chongzhi_tishi");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//提现提示
bool CDataBaseEngineSink::OnTixianTishi(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetTixianTishi));
		if (wDataSize!=sizeof(DBR_GP_GetTixianTishi)) return false;

		DBR_GP_GetTixianTishi* pGetTixianTishi = (DBR_GP_GetTixianTishi*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pGetTixianTishi->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_tixian_tishi"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_TixianTishiRet TixianTishiRet[20];
			ZeroMemory(&TixianTishiRet,sizeof(TixianTishiRet));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TIXIAN_TISHI_RET,dwContextID,&TixianTishiRet,sizeof(DBO_GR_TixianTishiRet)*lResult);
					ZeroMemory(&TixianTishiRet,sizeof(TixianTishiRet));
					lResult = 0;
				}

				TixianTishiRet[lResult].fJine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_jine"));
				TixianTishiRet[lResult].nState = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TIXIAN_TISHI_RET,dwContextID,&TixianTishiRet,sizeof(DBO_GR_TixianTishiRet)*lResult);

		}
		return true;

	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_tixian_tishi");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();


		//错误处理
		return false;
	}

	return true;
}
//投注提示
bool CDataBaseEngineSink::OnTouzhuTishi(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetTouzhuTishi));
		if (wDataSize!=sizeof(DBR_GP_GetTouzhuTishi)) return false;

		DBR_GP_GetTouzhuTishi* pGetTouzhuTishi = (DBR_GP_GetTouzhuTishi*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pGetTouzhuTishi->dwUserID);


		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_touzhu_tishi"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_TouzhuTishiRet TouzhuTishiRet[20];
			ZeroMemory(&TouzhuTishiRet,sizeof(TouzhuTishiRet));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_TISHI_RET,dwContextID,&TouzhuTishiRet,sizeof(DBO_GR_TouzhuTishiRet)*lResult);
					ZeroMemory(&TouzhuTishiRet,sizeof(TouzhuTishiRet));
					lResult = 0;
				}

				TouzhuTishiRet[lResult].nWinZhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_winzhushu"));
				TouzhuTishiRet[lResult].nState = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_qishu"),TouzhuTishiRet[lResult].szQishu,sizeof(TouzhuTishiRet[lResult].szQishu));
				TouzhuTishiRet[lResult].nType = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_TISHI_RET,dwContextID,&TouzhuTishiRet,sizeof(DBO_GR_TouzhuTishiRet)*lResult);

		}
		return true;



	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_touzhu_tishi");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}
//查询投注记录
bool CDataBaseEngineSink::OnGetTouzhuLog(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetTouzhuLog));
		if (wDataSize!=sizeof(DBR_GP_GetTouzhuLog)) return false;
		
		DBR_GP_GetTouzhuLog* pLog = (DBR_GP_GetTouzhuLog*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_touzhu_log"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetTouzhuLogRet GetTouzhuResult[20];
			ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_GetTouzhuLogRet)*lResult);
					ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
					lResult = 0;
				}
				GetTouzhuResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetTouzhuResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetTouzhuResult[lResult].n_t_kind =  m_GameSSCDBAide.GetValue_INT(TEXT("t_kind"));

				m_GameSSCDBAide.GetValue_String(TEXT("t_qishu"),GetTouzhuResult[lResult].szQishu,CountArray(GetTouzhuResult[lResult].szQishu));
				GetTouzhuResult[lResult].n_t_zhushu =  m_GameSSCDBAide.GetValue_INT(TEXT("t_zhushu"));
				GetTouzhuResult[lResult].n_t_moshi =  m_GameSSCDBAide.GetValue_INT(TEXT("t_moshi"));
				GetTouzhuResult[lResult].n_t_beishu =  m_GameSSCDBAide.GetValue_INT(TEXT("t_beishu"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetTouzhuResult[lResult].n_t_time);
				GetTouzhuResult[lResult].n_t_state = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				GetTouzhuResult[lResult].n_t_winzhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_winzhushu"));
				GetTouzhuResult[lResult].f_t_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yingkui"));
				GetTouzhuResult[lResult].n_t_zhuihao = m_GameSSCDBAide.GetValue_INT(TEXT("t_zhuihao"));
				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_GetTouzhuLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_touzhu_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}
	return true;
}
//查询新闻
bool CDataBaseEngineSink::OnGetNewsInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetNews));
		if (wDataSize!=sizeof(DBR_GP_GetNews)) return false;
		
		DBR_GP_GetNews* pLog = (DBR_GP_GetNews*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pLog->n_t_id);
		m_GameSSCDBAide.AddParameter(TEXT("@count"),pLog->n_t_ccount);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_top_news"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GP_GetNewsRet GetNews[50];
			ZeroMemory(&GetNews,sizeof(GetNews));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 50)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_NEWS_RET,dwContextID,&GetNews,sizeof(DBO_GP_GetNewsRet)*lResult);
					ZeroMemory(&GetNews,sizeof(GetNews));
					lResult = 0;
				}
				m_GameSSCDBAide.GetValue_String(TEXT("title"),GetNews[lResult].s_t_news,CountArray(GetNews[lResult].s_t_news));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetNews[lResult].s_t_time);

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_NEWS_RET,dwContextID,&GetNews,sizeof(DBO_GP_GetNewsRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_top_news");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();
		return false;
	}
	return true;

}
//转换
bool CDataBaseEngineSink::OnZhuanhuan(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_ZhuanHuan));
		if (wDataSize!=sizeof(DBR_GP_ZhuanHuan)) return false;
		
		DBR_GP_ZhuanHuan* pLog = (DBR_GP_ZhuanHuan*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pLog->n_t_userid);
		m_GameSSCDBAide.AddParameter(TEXT("@@type_id"),pLog->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@jine"),pLog->f_t_jine);

		//改到这里了,需要加存过

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_zhuanhuan"),true);

		DBO_GP_ZhuanHuanRet ZhuanhuanRet;
		ZeroMemory(&ZhuanhuanRet,sizeof(ZhuanhuanRet));
		ZhuanhuanRet.nResult = lResultCode;
		if(lResultCode == DB_SUCCESS)
		{
			if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			{
				ZhuanhuanRet.f_t_yue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yue"));
				ZhuanhuanRet.f_t_score = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_score"));
			}
		}
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ZHUAN_HUAN_RET,dwContextID,&ZhuanhuanRet,sizeof(ZhuanhuanRet));
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_zhuanhuan");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();
		return false;
	}
	return true;
}
//查询下级充值日志
bool CDataBaseEngineSink::OnGetXJCHZHLog(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJCHZHLog));
		if (wDataSize!=sizeof(DBR_GP_GetXJCHZHLog)) return false;
		
		DBR_GP_GetXJCHZHLog* pLog = (DBR_GP_GetXJCHZHLog*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLog->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLog->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLog->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjchzh_log"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJCHZHLogRet GetXICHZHLogResult[20];
			ZeroMemory(&GetXICHZHLogResult,sizeof(GetXICHZHLogResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJCHZH_LOG_RET,dwContextID,&GetXICHZHLogResult,sizeof(DBO_GR_GetXJCHZHLogRet)*lResult);
					ZeroMemory(&GetXICHZHLogResult,sizeof(GetXICHZHLogResult));
					lResult = 0;
				}
				GetXICHZHLogResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetXICHZHLogResult[lResult].n_t_user_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_user_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXICHZHLogResult[lResult].s_t_account,CountArray(GetXICHZHLogResult[lResult].s_t_account));
				GetXICHZHLogResult[lResult].f_t_jine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_jine"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetXICHZHLogResult[lResult].n_t_time);
				GetXICHZHLogResult[lResult].n_t_state =  m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_pingtai"),GetXICHZHLogResult[lResult].s_t_pingtai,CountArray(GetXICHZHLogResult[lResult].s_t_pingtai));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJCHZH_LOG_RET,dwContextID,&GetXICHZHLogResult,sizeof(DBO_GR_GetXJCHZHLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjchzh_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();

		return false;
	}
	return true;
}
//查询下级提现日志
bool CDataBaseEngineSink::OnGetXJTxLog(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJTxLog));
		if (wDataSize!=sizeof(DBR_GP_GetXJTxLog)) return false;
		
		DBR_GP_GetXJTxLog* pLog = (DBR_GP_GetXJTxLog*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLog->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLog->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLog->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtx_log"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJTxLogRet GetXJTxLogResult[20];
			ZeroMemory(&GetXJTxLogResult,sizeof(GetXJTxLogResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJTX_LOG_RET,dwContextID,&GetXJTxLogResult,sizeof(DBO_GR_GetXJTxLogRet)*lResult);
					ZeroMemory(&GetXJTxLogResult,sizeof(GetXJTxLogResult));
					lResult = 0;
				}
				GetXJTxLogResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetXJTxLogResult[lResult].n_t_user_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_user_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJTxLogResult[lResult].s_t_account,CountArray(GetXJTxLogResult[lResult].s_t_account));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetXJTxLogResult[lResult].n_t_time);
				m_GameSSCDBAide.GetValue_String(TEXT("yinhang"),GetXJTxLogResult[lResult].s_t_yinhang,CountArray(GetXJTxLogResult[lResult].s_t_yinhang));
				GetXJTxLogResult[lResult].f_t_jine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_jine"));
				m_GameSSCDBAide.GetValue_String(TEXT("kaihuren"),GetXJTxLogResult[lResult].s_t_kaihuiren,CountArray(GetXJTxLogResult[lResult].s_t_kaihuiren));
				GetXJTxLogResult[lResult].n_t_state =  m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_note"),GetXJTxLogResult[lResult].s_t_note,CountArray(GetXJTxLogResult[lResult].s_t_note));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJTX_LOG_RET,dwContextID,&GetXJTxLogResult,sizeof(DBO_GR_GetXJTxLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtx_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();

		return false;
	}
	return true;
}
//下级盈亏记录查询
bool CDataBaseEngineSink::OnGetXJYKLog(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJYKLog));
		if (wDataSize!=sizeof(DBR_GP_GetXJYKLog)) return false;

		DBR_GP_GetXJYKLog* pLog = (DBR_GP_GetXJYKLog*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLog->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLog->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLog->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);
		m_GameSSCDBAide.AddParameter(TEXT("@sorttype"),pLog->n_sort_type);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjyk_log"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJYKLogRet GetXJYKResult[20];
			ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_LOG_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKLogRet)*lResult);
					ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
					lResult = 0;
				}
				GetXJYKResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_ref_id"));
				GetXJYKResult[lResult].n_t_userid =  m_GameSSCDBAide.GetValue_INT(TEXT("t_user_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJYKResult[lResult].s_t_account,sizeof(GetXJYKResult[lResult].s_t_account));
				GetXJYKResult[lResult].f_t_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yingkui"));
				GetXJYKResult[lResult].f_t_at_yue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_at_yue"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetXJYKResult[lResult].n_t_time);
				GetXJYKResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				m_GameSSCDBAide.GetValue_String(TEXT("comments"),GetXJYKResult[lResult].s_t_comment,sizeof(GetXJYKResult[lResult].s_t_comment));


				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_LOG_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjyk_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();
		return false;
	}
	return true;
}
//下级盈亏统计查询
bool CDataBaseEngineSink::OnGetXJYKTj(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJYKTj));
		if (wDataSize!=sizeof(DBR_GP_GetXJYKTj)) return false;

		DBR_GP_GetXJYKTj* pLog = (DBR_GP_GetXJYKTj*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLog->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLog->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLog->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);
		m_GameSSCDBAide.AddParameter(TEXT("@sort_type"),pLog->n_sort_type);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjyktj"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJYKTjRet GetXJYKResult[20];
			ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_TJ_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKTjRet)*lResult);
					ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
					lResult = 0;
				}
				GetXJYKResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJYKResult[lResult].s_t_account,sizeof(GetXJYKResult[lResult].s_t_account));
				GetXJYKResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetXJYKResult[lResult].f_t_chongzhi_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("chongzhi_ze"));
				GetXJYKResult[lResult].f_t_qukuan_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("qukuan_ze"));
				GetXJYKResult[lResult].f_t_touzhu_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("touzhu_ze"));
				GetXJYKResult[lResult].f_t_zhongjiang_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("zhongjiang_ze"));
				GetXJYKResult[lResult].f_t_fandian_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("fandian_ze"));
				GetXJYKResult[lResult].f_t_xjfandian_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("xiajifandian_ze"));
				GetXJYKResult[lResult].f_t_huodong_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("huodong_ze"));
				GetXJYKResult[lResult].f_t_tuandui_ye = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yue"));
			//	GetXJYKResult[lResult].f_t_qipai_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("qipai_ze"));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_TJ_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKTjRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjyktj");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();
		return false;
	}
	return true;
}
//下级盈亏统计查询
bool CDataBaseEngineSink::OnGetXJYXTj(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJYXTj));
		if (wDataSize!=sizeof(DBR_GP_GetXJYXTj)) return false;

		DBR_GP_GetXJYXTj* pLog = (DBR_GP_GetXJYXTj*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLog->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLog->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLog->s_t_account);

		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjyktj_qipai"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJYXTjRet GetXJYXResult[20];
			ZeroMemory(&GetXJYXResult,sizeof(GetXJYXResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYX_TJ_RET,dwContextID,&GetXJYXResult,sizeof(DBO_GR_GetXJYXTjRet)*lResult);
					ZeroMemory(&GetXJYXResult,sizeof(GetXJYXResult));
					lResult = 0;
				}
				GetXJYXResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJYXResult[lResult].s_t_account,sizeof(GetXJYXResult[lResult].s_t_account));
				GetXJYXResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetXJYXResult[lResult].n_t_daili_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_daili_id"));

				GetXJYXResult[lResult].f_t_fandian_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("fandian_ze"));
				GetXJYXResult[lResult].f_t_qipai_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("qipai_ze"));
				GetXJYXResult[lResult].f_t_tuandui_ye = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yue"));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYX_TJ_RET,dwContextID,&GetXJYXResult,sizeof(DBO_GR_GetXJYXTjRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjyktj_qipai");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();
		return false;
	}
	return true;
}
//查询下级投注记录
bool CDataBaseEngineSink::OnGetXJTZHLog(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJTZHLog));
		if (wDataSize!=sizeof(DBR_GP_GetXJTZHLog)) return false;
		
		DBR_GP_GetXJTZHLog* pLog = (DBR_GP_GetXJTZHLog*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLog->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLog->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLog->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtzhtj"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJTZHLogRet GetTouzhuResult[20];
			ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_GetXJTZHLogRet)*lResult);
					ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
					lResult = 0;
				}
				DOUBLE fandian_ze = 0.0f;
				DOUBLE xiajifandian_ze = 0.0f;
				DOUBLE zhongjiang_ze = 0.0f;
				DOUBLE huodong_ze = 0.0f;

				GetTouzhuResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetTouzhuResult[lResult].s_t_account,CountArray(GetTouzhuResult[lResult].s_t_account));
				GetTouzhuResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetTouzhuResult[lResult].f_t_touzhu_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("touzhu_ze"));
				fandian_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("fandian_ze"));
				xiajifandian_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("xiajifandian_ze"));
				zhongjiang_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("zhongjiang_ze"));
				huodong_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("huodong_ze"));
				
				GetTouzhuResult[lResult].f_t_yingkui_ze = (GetTouzhuResult[lResult].f_t_touzhu_ze+fandian_ze+xiajifandian_ze+zhongjiang_ze+huodong_ze);
				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_GetXJTZHLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtzhtj");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();

		return false;
	}
	return true;
 
}
//查询下级投注记录
bool CDataBaseEngineSink::OnCHKXJTZHLog(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CHKXJTZHLog));
		if (wDataSize!=sizeof(DBR_GP_CHKXJTZHLog)) return false;
		
		DBR_GP_CHKXJTZHLog* pLog = (DBR_GP_CHKXJTZHLog*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLog->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLog->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLog->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtzh_log"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_CHKXJTZHLogRet GetTouzhuResult[20];
			ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHKXJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_CHKXJTZHLogRet)*lResult);
					ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
					lResult = 0;
				}
				GetTouzhuResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetTouzhuResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetTouzhuResult[lResult].n_t_kind = m_GameSSCDBAide.GetValue_INT(TEXT("t_kind"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_qishu"),GetTouzhuResult[lResult].s_t_qishu,sizeof(GetTouzhuResult[lResult].s_t_qishu));
				GetTouzhuResult[lResult].n_t_zhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_zhushu"));
				GetTouzhuResult[lResult].n_t_moshi = m_GameSSCDBAide.GetValue_INT(TEXT("t_moshi"));
				GetTouzhuResult[lResult].n_t_beishu = m_GameSSCDBAide.GetValue_INT(TEXT("t_beishu"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetTouzhuResult[lResult].n_t_time);
				GetTouzhuResult[lResult].n_t_state = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				GetTouzhuResult[lResult].n_t_winzhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_winzhushu"));
				GetTouzhuResult[lResult].f_t_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yingkui"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetTouzhuResult[lResult].s_t_account,sizeof(GetTouzhuResult[lResult].s_t_account));
				GetTouzhuResult[lResult].n_t_zhuihao = m_GameSSCDBAide.GetValue_INT(TEXT("t_zhuihao"));

				CString strLog;
				strLog.Format(L"t_id:%d,t_zhushu:%d,t_beishu:%d",GetTouzhuResult[lResult].n_t_id,GetTouzhuResult[lResult].n_t_zhushu,GetTouzhuResult[lResult].n_t_beishu );
				LogFile::instance().LogText(strLog);
				lResult++;	
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHKXJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_CHKXJTZHLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtzh_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();
		return false;
	}
	return true;
 
}
//查询下级提现记录
bool CDataBaseEngineSink::OnGetXJTXLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJTxLogByID));
		if (wDataSize!=sizeof(DBR_GP_GetXJTxLogByID)) return false;
		
		DBR_GP_GetXJTxLogByID* pLog = (DBR_GP_GetXJTxLogByID*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@xj_id"),pLog->nXiaJiID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtx_by_id"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJTxLogRet GetXJTxLogResult[20];
			ZeroMemory(&GetXJTxLogResult,sizeof(GetXJTxLogResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJTX_LOG_RET,dwContextID,&GetXJTxLogResult,sizeof(DBO_GR_GetXJTxLogRet)*lResult);
					ZeroMemory(&GetXJTxLogResult,sizeof(GetXJTxLogResult));
					lResult = 0;
				}
				GetXJTxLogResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetXJTxLogResult[lResult].n_t_user_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_user_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJTxLogResult[lResult].s_t_account,CountArray(GetXJTxLogResult[lResult].s_t_account));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetXJTxLogResult[lResult].n_t_time);
				m_GameSSCDBAide.GetValue_String(TEXT("yinhang"),GetXJTxLogResult[lResult].s_t_yinhang,CountArray(GetXJTxLogResult[lResult].s_t_yinhang));
				GetXJTxLogResult[lResult].f_t_jine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_jine"));
				m_GameSSCDBAide.GetValue_String(TEXT("kaihuren"),GetXJTxLogResult[lResult].s_t_kaihuiren,CountArray(GetXJTxLogResult[lResult].s_t_kaihuiren));
				GetXJTxLogResult[lResult].n_t_state =  m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_note"),GetXJTxLogResult[lResult].s_t_note,CountArray(GetXJTxLogResult[lResult].s_t_note));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();

			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJTX_LOG_RET,dwContextID,&GetXJTxLogResult,sizeof(DBO_GR_GetXJTxLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtx_by_id");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级提现日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;
}
//查询下级提现记录
bool CDataBaseEngineSink::OnGetXJTxLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJTZHLogByAct));
		if (wDataSize!=sizeof(DBR_GP_GetXJTZHLogByAct)) return false;

		DBR_GP_GetXJTZHLogByAct* pLog = (DBR_GP_GetXJTZHLogByAct*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@act"),pLog->szAccount);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtx_by_act"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJTxLogRet GetXJTxLogResult[20];
			ZeroMemory(&GetXJTxLogResult,sizeof(GetXJTxLogResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJTX_LOG_RET,dwContextID,&GetXJTxLogResult,sizeof(DBO_GR_GetXJTxLogRet)*lResult);
					ZeroMemory(&GetXJTxLogResult,sizeof(GetXJTxLogResult));
					lResult = 0;
				}
				GetXJTxLogResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetXJTxLogResult[lResult].n_t_user_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_user_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJTxLogResult[lResult].s_t_account,CountArray(GetXJTxLogResult[lResult].s_t_account));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetXJTxLogResult[lResult].n_t_time);
				m_GameSSCDBAide.GetValue_String(TEXT("yinhang"),GetXJTxLogResult[lResult].s_t_yinhang,CountArray(GetXJTxLogResult[lResult].s_t_yinhang));
				GetXJTxLogResult[lResult].f_t_jine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_jine"));
				m_GameSSCDBAide.GetValue_String(TEXT("kaihuren"),GetXJTxLogResult[lResult].s_t_kaihuiren,CountArray(GetXJTxLogResult[lResult].s_t_kaihuiren));
				GetXJTxLogResult[lResult].n_t_state =  m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_note"),GetXJTxLogResult[lResult].s_t_note,CountArray(GetXJTxLogResult[lResult].s_t_note));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();

			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJTX_LOG_RET,dwContextID,&GetXJTxLogResult,sizeof(DBO_GR_GetXJTxLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtx_by_act");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级提现记录,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);
	
		return false;
	}
	return true;
}

//查询下级投注记录
bool CDataBaseEngineSink::OnGetXJTZHLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJTZHLogByID));
		if (wDataSize!=sizeof(DBR_GP_GetXJTZHLogByID)) return false;
		
		DBR_GP_GetXJTZHLogByID* pLog = (DBR_GP_GetXJTZHLogByID*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@xj_id"),pLog->nXiaJiID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtzhtj_by_id"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJTZHLogRet GetTouzhuResult[20];
			ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_GetXJTZHLogRet)*lResult);
					ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
					lResult = 0;
				}
				GetTouzhuResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetTouzhuResult[lResult].s_t_account,CountArray(GetTouzhuResult[lResult].s_t_account));
				GetTouzhuResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetTouzhuResult[lResult].f_t_touzhu_ze = fabs(m_GameSSCDBAide.GetValue_DOUBLE(TEXT("touzhu_ze")));
				GetTouzhuResult[lResult].f_t_yingkui_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("yingkui_ze"));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();

			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_GetXJTZHLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtzhtj_by_id");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级投注记录,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;
}
//查询下级投注记录
bool CDataBaseEngineSink::OnGetXJTZHLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJTZHLogByAct));
		if (wDataSize!=sizeof(DBR_GP_GetXJTZHLogByAct)) return false;

		DBR_GP_GetXJTZHLogByAct* pLog = (DBR_GP_GetXJTZHLogByAct*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@act"),pLog->szAccount);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtzhtj_by_act"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJTZHLogRet GetTouzhuResult[20];
			ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_GetXJTZHLogRet)*lResult);
					ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
					lResult = 0;
				}
				GetTouzhuResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetTouzhuResult[lResult].s_t_account,CountArray(GetTouzhuResult[lResult].s_t_account));
				GetTouzhuResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetTouzhuResult[lResult].f_t_touzhu_ze = fabs(m_GameSSCDBAide.GetValue_DOUBLE(TEXT("touzhu_ze")));
				GetTouzhuResult[lResult].f_t_yingkui_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("yingkui_ze"));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();

			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_GetXJTZHLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtzhtj_by_act");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级投注记录,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;
}
//查询下级投注记录
bool CDataBaseEngineSink::OnCHKXJTZHLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CHKXJTZHLogByID));
		if (wDataSize!=sizeof(DBR_GP_CHKXJTZHLogByID)) return false;
		
		DBR_GP_CHKXJTZHLogByID* pLog = (DBR_GP_CHKXJTZHLogByID*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@xj_id"),pLog->nXiaJiID);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtzh_by_id"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_CHKXJTZHLogRet GetTouzhuResult[20];
			ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHKXJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_CHKXJTZHLogRet)*lResult);
					ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
					lResult = 0;
				}
				GetTouzhuResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetTouzhuResult[lResult].s_t_account,sizeof(GetTouzhuResult[lResult].s_t_account));

				GetTouzhuResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetTouzhuResult[lResult].n_t_kind = m_GameSSCDBAide.GetValue_INT(TEXT("t_kind"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_qishu"),GetTouzhuResult[lResult].s_t_qishu,sizeof(GetTouzhuResult[lResult].s_t_qishu));
				GetTouzhuResult[lResult].n_t_zhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_zhushu"));
				GetTouzhuResult[lResult].n_t_moshi = m_GameSSCDBAide.GetValue_INT(TEXT("t_moshi"));
				GetTouzhuResult[lResult].n_t_beishu = m_GameSSCDBAide.GetValue_INT(TEXT("t_beishu"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetTouzhuResult[lResult].n_t_time);
				GetTouzhuResult[lResult].n_t_state = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				GetTouzhuResult[lResult].n_t_winzhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_winzhushu"));
				GetTouzhuResult[lResult].f_t_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yingkui"));
				GetTouzhuResult[lResult].n_t_zhuihao = m_GameSSCDBAide.GetValue_INT(TEXT("t_zhuihao"));

				lResult++;	
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();

			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHKXJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_CHKXJTZHLogRet)*lResult);		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtzh_by_id");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		return false;
	}
	return true;
}
//查询下级投注记录
bool CDataBaseEngineSink::OnCHKXJTZHLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CHKXJTZHLogByAct));
		if (wDataSize!=sizeof(DBR_GP_CHKXJTZHLogByAct)) return false;

		DBR_GP_CHKXJTZHLogByAct* pLog = (DBR_GP_CHKXJTZHLogByAct*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@act"),pLog->szAccount);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtzh_by_act"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_CHKXJTZHLogRet GetTouzhuResult[20];
			ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHKXJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_CHKXJTZHLogRet)*lResult);
					ZeroMemory(&GetTouzhuResult,sizeof(GetTouzhuResult));
					lResult = 0;
				}
				GetTouzhuResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetTouzhuResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetTouzhuResult[lResult].n_t_kind = m_GameSSCDBAide.GetValue_INT(TEXT("t_kind"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_qishu"),GetTouzhuResult[lResult].s_t_qishu,sizeof(GetTouzhuResult[lResult].s_t_qishu));
				GetTouzhuResult[lResult].n_t_zhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_zhushu"));
				GetTouzhuResult[lResult].n_t_moshi = m_GameSSCDBAide.GetValue_INT(TEXT("t_moshi"));
				GetTouzhuResult[lResult].n_t_beishu = m_GameSSCDBAide.GetValue_INT(TEXT("t_beishu"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetTouzhuResult[lResult].n_t_time);
				GetTouzhuResult[lResult].n_t_state = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				GetTouzhuResult[lResult].n_t_winzhushu = m_GameSSCDBAide.GetValue_INT(TEXT("t_winzhushu"));
				GetTouzhuResult[lResult].f_t_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yingkui"));
				lResult++;	
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHKXJTZH_LOG_RET,dwContextID,&GetTouzhuResult,sizeof(DBO_GR_CHKXJTZHLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtzh_by_act");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级投注日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;

}
//查询下级盈亏记录
bool CDataBaseEngineSink::OnGetXJYKLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJYKLogByID));
		if (wDataSize!=sizeof(DBR_GP_GetXJYKLogByID)) return false;
		
		DBR_GP_GetXJYKLogByID* pLog = (DBR_GP_GetXJYKLogByID*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@xj_id"),pLog->nXiaJiID);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjyk_by_id"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJYKLogRet GetXJYKResult[20];
			ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_LOG_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKLogRet)*lResult);
					ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
					lResult = 0;
				}
				GetXJYKResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetXJYKResult[lResult].n_t_userid =  m_GameSSCDBAide.GetValue_INT(TEXT("t_user_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJYKResult[lResult].s_t_account,sizeof(GetXJYKResult[lResult].s_t_account));
				GetXJYKResult[lResult].f_t_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yingkui"));
				GetXJYKResult[lResult].f_t_at_yue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_at_yue"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetXJYKResult[lResult].n_t_time);
				GetXJYKResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				m_GameSSCDBAide.GetValue_String(TEXT("comments"),GetXJYKResult[lResult].s_t_comment,sizeof(GetXJYKResult[lResult].s_t_comment));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_LOG_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjyk_by_id");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级盈亏日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;
 
}
//查询下级盈亏记录
bool CDataBaseEngineSink::OnGetXJYKLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CHKXJTZHLogByAct));
		if (wDataSize!=sizeof(DBR_GP_CHKXJTZHLogByAct)) return false;

		DBR_GP_CHKXJTZHLogByAct* pLog = (DBR_GP_CHKXJTZHLogByAct*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@act"),pLog->szAccount);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjyk_by_act"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJYKLogRet GetXJYKResult[20];
			ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_LOG_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKLogRet)*lResult);
					ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
					lResult = 0;
				}
				GetXJYKResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetXJYKResult[lResult].n_t_userid =  m_GameSSCDBAide.GetValue_INT(TEXT("t_user_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJYKResult[lResult].s_t_account,sizeof(GetXJYKResult[lResult].s_t_account));
				GetXJYKResult[lResult].f_t_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yingkui"));
				GetXJYKResult[lResult].f_t_at_yue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_at_yue"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetXJYKResult[lResult].n_t_time);
				GetXJYKResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				m_GameSSCDBAide.GetValue_String(TEXT("comments"),GetXJYKResult[lResult].s_t_comment,sizeof(GetXJYKResult[lResult].s_t_comment));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_LOG_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjyk_by_act");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级盈亏日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;
}
//通过ID查询下级充值日志
bool CDataBaseEngineSink::OnGetXJCHZHLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJCHZHLogByID));
		if (wDataSize!=sizeof(DBR_GP_GetXJCHZHLogByID)) return false;

		DBR_GP_GetXJCHZHLogByID* pLog = (DBR_GP_GetXJCHZHLogByID*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@xj_id"),pLog->nXiaJiID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjchzh_by_id"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJCHZHLogRet GetXJYKResult[20];
			ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJCHZH_LOG_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKLogRet)*lResult);
					ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
					lResult = 0;
				}
				GetXJYKResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetXJYKResult[lResult].n_t_user_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_user_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJYKResult[lResult].s_t_account,sizeof(GetXJYKResult[lResult].s_t_account));

				GetXJYKResult[lResult].f_t_jine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_jine"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetXJYKResult[lResult].n_t_time);
				GetXJYKResult[lResult].n_t_state =  m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));

				m_GameSSCDBAide.GetValue_String(TEXT("t_pingtai"),GetXJYKResult[lResult].s_t_pingtai,sizeof(GetXJYKResult[lResult].s_t_pingtai));


				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJCHZH_LOG_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjchzh_by_id");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级充值日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;

}
//通过账户查询下级充值日志
bool CDataBaseEngineSink::OnGetXJCHZHLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJCHZHLogByAct));
		if (wDataSize!=sizeof(DBR_GP_GetXJCHZHLogByAct)) return false;

		DBR_GP_GetXJCHZHLogByAct* pLog = (DBR_GP_GetXJCHZHLogByAct*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@act"),pLog->szAccount);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjchzh_by_act"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJCHZHLogRet GetXICHZHLogResult[20];
			ZeroMemory(&GetXICHZHLogResult,sizeof(GetXICHZHLogResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJCHZH_LOG_RET,dwContextID,&GetXICHZHLogResult,sizeof(DBO_GR_GetXJCHZHLogRet)*lResult);
					ZeroMemory(&GetXICHZHLogResult,sizeof(GetXICHZHLogResult));
					lResult = 0;
				}
				GetXICHZHLogResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetXICHZHLogResult[lResult].n_t_user_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_user_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXICHZHLogResult[lResult].s_t_account,CountArray(GetXICHZHLogResult[lResult].s_t_account));
				GetXICHZHLogResult[lResult].f_t_jine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_jine"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetXICHZHLogResult[lResult].n_t_time);
				GetXICHZHLogResult[lResult].n_t_state =  m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_pingtai"),GetXICHZHLogResult[lResult].s_t_pingtai,CountArray(GetXICHZHLogResult[lResult].s_t_pingtai));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJCHZH_LOG_RET,dwContextID,&GetXICHZHLogResult,sizeof(DBO_GR_GetXJCHZHLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjchzh_by_act");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级充值日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;
}
//查询下级盈亏统计
bool CDataBaseEngineSink::OnGetXJYKTjByID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CHKXJTZHLogByID));
		if (wDataSize!=sizeof(DBR_GP_CHKXJTZHLogByID)) return false;

		DBR_GP_CHKXJTZHLogByID* pLog = (DBR_GP_CHKXJTZHLogByID*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@xj_id"),pLog->nXiaJiID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjyktj_by_id"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJYKTjRet GetXJYKResult[20];
			ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_TJ_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKTjRet)*lResult);
					ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
					lResult = 0;
				}
				GetXJYKResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJYKResult[lResult].s_t_account,sizeof(GetXJYKResult[lResult].s_t_account));
				GetXJYKResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetXJYKResult[lResult].f_t_chongzhi_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("chongzhi_ze"));
				GetXJYKResult[lResult].f_t_qukuan_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("qukuan_ze"));
				GetXJYKResult[lResult].f_t_touzhu_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("touzhu_ze"));
				GetXJYKResult[lResult].f_t_fandian_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("fandian_ze"));
				GetXJYKResult[lResult].f_t_xjfandian_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("xiajifandian_ze"));
				//GetXJYKResult[lResult].f_t_yingkui_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("yingkui_ze"));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_TJ_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKTjRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjyktj_by_id");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级统计日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;

}
//查询下级盈亏统计
bool CDataBaseEngineSink::OnGetXJYKTjByAct(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CHKXJTZHLogByAct));
		if (wDataSize!=sizeof(DBR_GP_CHKXJTZHLogByAct)) return false;

		DBR_GP_CHKXJTZHLogByAct* pLog = (DBR_GP_CHKXJTZHLogByAct*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@act"),pLog->szAccount);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjyktj_by_act"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetXJYKTjRet GetXJYKResult[20];
			ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_TJ_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKTjRet)*lResult);
					ZeroMemory(&GetXJYKResult,sizeof(GetXJYKResult));
					lResult = 0;
				}
				GetXJYKResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetXJYKResult[lResult].s_t_account,sizeof(GetXJYKResult[lResult].s_t_account));
				GetXJYKResult[lResult].n_t_type =  m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetXJYKResult[lResult].f_t_chongzhi_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("chongzhi_ze"));
				GetXJYKResult[lResult].f_t_qukuan_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("qukuan_ze"));
				GetXJYKResult[lResult].f_t_touzhu_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("touzhu_ze"));
				GetXJYKResult[lResult].f_t_fandian_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("fandian_ze"));
				GetXJYKResult[lResult].f_t_xjfandian_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("xiajifandian_ze"));
//				GetXJYKResult[lResult].f_t_yingkui_ze = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("yingkui_ze"));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();

			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_XJYK_TJ_RET,dwContextID,&GetXJYKResult,sizeof(DBO_GR_GetXJYKTjRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjyktj_by_act");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级统计日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;
}

//查询提现记录
bool CDataBaseEngineSink::OnGetTixianLog(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetTixianLog));
		if (wDataSize!=sizeof(DBR_GP_GetTixianLog)) return false;
		
		DBR_GP_GetTixianLog* pLog = (DBR_GP_GetTixianLog*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_tixian_log"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetTixianLogRet GetTixianResult[20];
			ZeroMemory(&GetTixianResult,sizeof(GetTixianResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TIXIAN_LOG_RET,dwContextID,&GetTixianResult,sizeof(DBO_GR_GetTixianLogRet)*lResult);
					ZeroMemory(&GetTixianResult,sizeof(GetTixianResult));
					lResult = 0;
				}
				GetTixianResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetTixianResult[lResult].n_t_time);
				m_GameSSCDBAide.GetValue_String(TEXT("yinhang"),GetTixianResult[lResult].n_t_yinhang,CountArray(GetTixianResult[lResult].n_t_yinhang));
				GetTixianResult[lResult].f_t_jine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_jine"));
				m_GameSSCDBAide.GetValue_String(TEXT("kaihuren"),GetTixianResult[lResult].s_t_kaihuren,CountArray(GetTixianResult[lResult].s_t_kaihuren));
				GetTixianResult[lResult].n_t_state = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_note"),GetTixianResult[lResult].s_t_note,CountArray(GetTixianResult[lResult].s_t_note));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TIXIAN_LOG_RET,dwContextID,&GetTixianResult,sizeof(DBO_GR_GetTixianLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_tixian_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询下级提现日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	 return true;
 
}
bool CDataBaseEngineSink::OnGetHYXXList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT(wDataSize == sizeof(DBR_GP_GetHYXXLog));
		if(wDataSize!=sizeof(DBR_GP_GetHYXXLog)) return false;

		DBR_GP_GetHYXXLog* pLog = (DBR_GP_GetHYXXLog*)pData;

		m_GameSSCDBAide.ResetParameter();

		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type_id"),pLog->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLog->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLog->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@npage"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@nsize"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);
		m_GameSSCDBAide.AddParameter(TEXT("@sorttype"),pLog->n_t_sorttype);
		m_GameSSCDBAide.AddParameter(TEXT("@online"),pLog->nOnline);
		m_GameSSCDBAide.AddParameter(TEXT("@yue"),pLog->nYue);

		LONG lResult = m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjhy_log"),true);
		if(lResult == DB_SUCCESS)
		{
			DBO_GR_GetHYXXLogRet GetHYXXResult[20];
			ZeroMemory(&GetHYXXResult,sizeof(GetHYXXResult));
			int nResult =0;
			while(!m_GameSSCDBModule->IsRecordsetEnd())
			{
				if(nResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_LOG_RET,dwContextID,&GetHYXXResult,sizeof(DBO_GR_GetHYXXLogRet)*nResult);
					ZeroMemory(&GetHYXXResult,sizeof(GetHYXXResult));
					nResult = 0;
				}
				GetHYXXResult[nResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetHYXXResult[nResult].s_t_account,CountArray(GetHYXXResult[nResult].s_t_account));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_reg_time"),GetHYXXResult[nResult].n_t_time);
				GetHYXXResult[nResult].n_t_type = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetHYXXResult[nResult].f_t_fandian = m_GameSSCDBAide.GetValue_FLOAT(TEXT("t_fandian"));
				GetHYXXResult[nResult].f_t_yue = m_GameSSCDBAide.GetValue_FLOAT(TEXT("t_yue"));
				GetHYXXResult[nResult].n_t_online = m_GameSSCDBAide.GetValue_INT(TEXT("t_online"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_qq"),GetHYXXResult[nResult].s_t_qq,CountArray(GetHYXXResult[nResult].s_t_qq));

				nResult++;
				m_GameSSCDBModule->MoveToNext();

			}
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_LOG_RET,dwContextID,&GetHYXXResult,sizeof(DBO_GR_GetHYXXLogRet)*nResult);

		}
		return true;
	}
	catch (CException* e)
	{
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjhy_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询提现记录
bool CDataBaseEngineSink::OnGetHYXXLog(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetHYXXLog));
		if (wDataSize!=sizeof(DBR_GP_GetHYXXLog)) return false;
		
		DBR_GP_GetHYXXLog* pLog = (DBR_GP_GetHYXXLog*)pData;


 		m_GameSSCDBAide.ResetParameter();
 		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
 		m_GameSSCDBAide.AddParameter(TEXT("@type_id"),pLog->n_t_type);
 		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLog->n_t_user_id);
 		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLog->s_t_account);
 		m_GameSSCDBAide.AddParameter(TEXT("@npage"),pLog->nPage);
 		m_GameSSCDBAide.AddParameter(TEXT("@nsize"),pLog->nSize);
 		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);
		m_GameSSCDBAide.AddParameter(TEXT("@sorttype"),pLog->n_t_sorttype);
		m_GameSSCDBAide.AddParameter(TEXT("@online"),pLog->nOnline);
		m_GameSSCDBAide.AddParameter(TEXT("@yue"),pLog->nYue);
 
 		LONG lResultCode = m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjhy_log"),true);
 		
 		if(lResultCode == DB_SUCCESS)
 		{
 			DBO_GR_GetHYXXLogRet GetHYXXResult[20];
 			ZeroMemory(&GetHYXXResult,sizeof(GetHYXXResult));
 			int lResult =0;
 			while(!m_GameSSCDBModule->IsRecordsetEnd())
 			{
 				if(lResult >= 20)
 				{
 					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_LOG_RET,dwContextID,&GetHYXXResult,sizeof(DBO_GR_GetHYXXLogRet)*lResult);
 					ZeroMemory(&GetHYXXResult,sizeof(GetHYXXResult));
 					lResult = 0;
 				}
 				GetHYXXResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
 				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetHYXXResult[lResult].s_t_account,CountArray(GetHYXXResult[lResult].s_t_account));
 				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_reg_time"),GetHYXXResult[lResult].n_t_time);
 				GetHYXXResult[lResult].n_t_type = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
 				GetHYXXResult[lResult].f_t_fandian = m_GameSSCDBAide.GetValue_FLOAT(TEXT("t_fandian"));
 				GetHYXXResult[lResult].f_t_yue = m_GameSSCDBAide.GetValue_FLOAT(TEXT("t_yue"));
 				GetHYXXResult[lResult].n_t_online = m_GameSSCDBAide.GetValue_INT(TEXT("t_online"));
 				m_GameSSCDBAide.GetValue_String(TEXT("t_qq"),GetHYXXResult[lResult].s_t_qq,CountArray(GetHYXXResult[lResult].s_t_qq));
 
 				lResult++;
 				m_GameSSCDBModule->MoveToNext();
 
 			}
 			m_GameSSCDBModule->CloseRecordset();
 			//发送结果
 			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_LOG_RET,dwContextID,&GetHYXXResult,sizeof(DBO_GR_GetHYXXLogRet)*lResult);

		}

		return true;
/*
		CString strLog;
		strLog.Format(L"HYXX dwUserID = %ld,n_t_type = %d,n_t_user_id = %d,s_t_account = %s,nPage = %d,nSize = %d,bByTime = %d,szTimeStart = %s,szTimeEnd = %s",pLog->dwUserID,pLog->n_t_type,pLog->n_t_user_id,pLog->s_t_account,pLog->nPage,pLog->nSize,pLog->bByTime,pLog->szTimeStart,pLog->szTimeEnd);
		OutputDebugString(strLog);
*/

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjhy_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询会员信息日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;
 
}
//查询提现记录
bool CDataBaseEngineSink::OnGetHYXXLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetHYXXLogByID));
		if (wDataSize!=sizeof(DBR_GP_GetHYXXLogByID)) return false;
		
		DBR_GP_GetHYXXLogByID* pLog = (DBR_GP_GetHYXXLogByID*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@xj_id"),pLog->nXiaJiID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_hyxx_by_id"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetHYXXLogRet GetHYXXResult[20];
			ZeroMemory(&GetHYXXResult,sizeof(GetHYXXResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_LOG_RET,dwContextID,&GetHYXXResult,sizeof(DBO_GR_GetHYXXLogRet)*lResult);
					ZeroMemory(&GetHYXXResult,sizeof(GetHYXXResult));
					lResult = 0;
				}
				GetHYXXResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetHYXXResult[lResult].s_t_account,CountArray(GetHYXXResult[lResult].s_t_account));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_reg_time"),GetHYXXResult[lResult].n_t_time);
				GetHYXXResult[lResult].n_t_type = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetHYXXResult[lResult].f_t_fandian = m_GameSSCDBAide.GetValue_FLOAT(TEXT("t_fandian"));
				GetHYXXResult[lResult].f_t_yue = m_GameSSCDBAide.GetValue_FLOAT(TEXT("t_yue"));
				GetHYXXResult[lResult].n_t_online = m_GameSSCDBAide.GetValue_INT(TEXT("t_online"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_qq"),GetHYXXResult[lResult].s_t_qq,CountArray(GetHYXXResult[lResult].s_t_qq));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();

			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_LOG_RET,dwContextID,&GetHYXXResult,sizeof(DBO_GR_GetHYXXLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_hyxx_by_id");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询会员信息日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);


		return false;
	}
	return true; 
}
//查询会员信息
bool CDataBaseEngineSink::OnGetHYXXLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetHYXXLogByAct));
		if (wDataSize!=sizeof(DBR_GP_GetHYXXLogByAct)) return false;
		
		DBR_GP_GetHYXXLogByAct* pLog = (DBR_GP_GetHYXXLogByAct*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@act"),pLog->szAccount);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_hyxx_by_act"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetHYXXLogRet GetHYXXResult[20];
			ZeroMemory(&GetHYXXResult,sizeof(GetHYXXResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_LOG_RET,dwContextID,&GetHYXXResult,sizeof(DBO_GR_GetHYXXLogRet)*lResult);
					ZeroMemory(&GetHYXXResult,sizeof(GetHYXXResult));
					lResult = 0;
				}
				GetHYXXResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_account"),GetHYXXResult[lResult].s_t_account,CountArray(GetHYXXResult[lResult].s_t_account));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_reg_time"),GetHYXXResult[lResult].n_t_time);
				GetHYXXResult[lResult].n_t_type = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetHYXXResult[lResult].f_t_fandian = m_GameSSCDBAide.GetValue_FLOAT(TEXT("t_fandian"));
				GetHYXXResult[lResult].f_t_yue = m_GameSSCDBAide.GetValue_FLOAT(TEXT("t_yue"));
				GetHYXXResult[lResult].n_t_online = m_GameSSCDBAide.GetValue_INT(TEXT("t_online"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_qq"),GetHYXXResult[lResult].s_t_qq,CountArray(GetHYXXResult[lResult].s_t_qq));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();

			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_LOG_RET,dwContextID,&GetHYXXResult,sizeof(DBO_GR_GetHYXXLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_hyxx_by_act");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询会员信息日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true; 
}
//查询提现记录
bool CDataBaseEngineSink::OnGetChongzhiLog(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetChongzhiLog));
		if (wDataSize!=sizeof(DBR_GP_GetChongzhiLog)) return false;
		
		DBR_GP_GetChongzhiLog* pLog = (DBR_GP_GetChongzhiLog*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_chongzhi_log"),true);
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetChongzhiRet GetChongzhiResult[20];
			ZeroMemory(&GetChongzhiResult,sizeof(GetChongzhiResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHONGZHI_LOG_RET,dwContextID,&GetChongzhiResult,sizeof(DBO_GR_GetChongzhiRet)*lResult);
					ZeroMemory(&GetChongzhiResult,sizeof(GetChongzhiResult));
					lResult = 0;
				}
				GetChongzhiResult[lResult].n_t_id =  m_GameSSCDBAide.GetValue_INT(TEXT("t_id"));
				GetChongzhiResult[lResult].f_t_jine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_jine"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetChongzhiResult[lResult].n_t_time);
				GetChongzhiResult[lResult].n_t_state = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_pingtai"),GetChongzhiResult[lResult].s_t_pingtai,CountArray(GetChongzhiResult[lResult].s_t_pingtai));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHONGZHI_LOG_RET,dwContextID,&GetChongzhiResult,sizeof(DBO_GR_GetChongzhiRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_chongzhi_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询充值日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);
		return false;
	}
	return true;
 
}
//查询盈亏记录
bool CDataBaseEngineSink::OnGetYingkuiLog(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetYingkuiLog));
		if (wDataSize!=sizeof(DBR_GP_GetYingkuiLog)) return false;
		
		DBR_GP_GetYingkuiLog* pLog = (DBR_GP_GetYingkuiLog*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_yingkui_log"),true);

		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetYingkuiLogRet GetYingkuiResult[20];
			ZeroMemory(&GetYingkuiResult,sizeof(GetYingkuiResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_YINGKUI_LOG_RET,dwContextID,&GetYingkuiResult,sizeof(DBO_GR_GetYingkuiLogRet)*lResult);
					ZeroMemory(&GetYingkuiResult,sizeof(GetYingkuiResult));
					lResult = 0;
				}
				GetYingkuiResult[lResult].f_t_chongzhi = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("CHONGZHI"));
				GetYingkuiResult[lResult].f_t_qukuan = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("QUKUAN"));
				GetYingkuiResult[lResult].f_t_touzhu = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("TOUZHU"));
				GetYingkuiResult[lResult].f_t_fandian = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("FANDIAN"));
				GetYingkuiResult[lResult].f_t_zhongjiang = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("ZHONGJIANG"));
				GetYingkuiResult[lResult].f_t_huodong = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("HUODONG"));
				GetYingkuiResult[lResult].f_t_chedan = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("CHEDAN"));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_YINGKUI_LOG_RET,dwContextID,&GetYingkuiResult,sizeof(DBO_GR_GetYingkuiLogRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_yingkui_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询盈亏日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;
 
}
//查询盈亏记录
bool CDataBaseEngineSink::OnGetYingkuiMx(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetYingkuiMx));
		if (wDataSize!=sizeof(DBR_GP_GetYingkuiMx)) return false;
		
		DBR_GP_GetYingkuiMx* pLog = (DBR_GP_GetYingkuiMx*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_my_yingkui_log"),true);

		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetYingkuiMxRet GetYingkuiResult[20];
			ZeroMemory(&GetYingkuiResult,sizeof(GetYingkuiResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_YINGKUI_MX_RET,dwContextID,&GetYingkuiResult,sizeof(DBO_GR_GetYingkuiMxRet)*lResult);
					ZeroMemory(&GetYingkuiResult,sizeof(GetYingkuiResult));
					lResult = 0;
				}

				GetYingkuiResult[lResult].n_t_dingdan = m_GameSSCDBAide.GetValue_INT(TEXT("t_ref_id"));
				GetYingkuiResult[lResult].f_t_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yingkui"));
				GetYingkuiResult[lResult].f_t_yue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_at_yue"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("t_time"),GetYingkuiResult[lResult].t_t_yingkuitime);

				m_GameSSCDBAide.GetValue_String(TEXT("kindname"),GetYingkuiResult[lResult].s_t_type ,CountArray(GetYingkuiResult[lResult].s_t_type));
				m_GameSSCDBAide.GetValue_String(TEXT("comments"),GetYingkuiResult[lResult].s_t_zhaiyao  ,CountArray(GetYingkuiResult[lResult].s_t_zhaiyao));


				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_YINGKUI_MX_RET,dwContextID,&GetYingkuiResult,sizeof(DBO_GR_GetYingkuiMxRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_my_yingkui_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);


		CString strLog;
		strLog.Format(L"查询盈亏明细日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);

		return false;
	}
	return true;
}
//查询棋牌盈亏记录
bool CDataBaseEngineSink::OnGetQiPaiYingkui(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetQiPaiYingkui));
		if (wDataSize!=sizeof(DBR_GP_GetQiPaiYingkui)) return false;
		
		DBR_GP_GetQiPaiYingkui* pLog = (DBR_GP_GetQiPaiYingkui*)pData;

		int nType = 2;
		nType = (pLog->bByTime?2:1);
		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLog->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),nType);
		m_GameSSCDBAide.AddParameter(TEXT("@page"),pLog->nPage);
		m_GameSSCDBAide.AddParameter(TEXT("@size"),pLog->nSize);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLog->bByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLog->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLog->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_yxcx_log"),true);

		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetQiPaiYingkuiRet GetYingkuiResult[20];
			ZeroMemory(&GetYingkuiResult,sizeof(GetYingkuiResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 20)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QP_YINGKUI_RET,dwContextID,&GetYingkuiResult,sizeof(DBO_GR_GetQiPaiYingkuiRet)*lResult);
					ZeroMemory(&GetYingkuiResult,sizeof(GetYingkuiResult));
					lResult = 0;
				}

				GetYingkuiResult[lResult].n_t_userid = m_GameSSCDBAide.GetValue_INT(TEXT("UserID"));
				GetYingkuiResult[lResult].n_t_drawid = m_GameSSCDBAide.GetValue_INT(TEXT("drawid"));
				GetYingkuiResult[lResult].f_t_score = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("score"));
				GetYingkuiResult[lResult].n_t_playcount = m_GameSSCDBAide.GetValue_INT(TEXT("PlayTimeCount"));
				GetYingkuiResult[lResult].f_t_revenue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("Revenue"));
				m_GameSSCDBAide.GetValue_SystemTime(TEXT("InsertTime"),GetYingkuiResult[lResult].n_t_playtime);

				m_GameSSCDBAide.GetValue_String(TEXT("ServerName"),GetYingkuiResult[lResult].s_t_servername ,CountArray(GetYingkuiResult[lResult].s_t_servername));
				m_GameSSCDBAide.GetValue_String(TEXT("accounts"),GetYingkuiResult[lResult].s_t_accounts  ,CountArray(GetYingkuiResult[lResult].s_t_accounts));


				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	

			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QP_YINGKUI_RET,dwContextID,&GetYingkuiResult,sizeof(DBO_GR_GetQiPaiYingkuiRet)*lResult);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_yxcx_log");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询棋牌盈亏日志,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);
		return false;
	}
	return true;
}
//查询投注记录数量
bool CDataBaseEngineSink::OnGetTouzhuLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetTouzhuLogCount));
		if (wDataSize!=sizeof(DBR_GP_GetTouzhuLogCount)) return false;
		
		DBR_GP_GetTouzhuLogCount* pLogCount = (DBR_GP_GetTouzhuLogCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_touzhu_log_count"),true);

		DBO_GR_GetTouzhuLogCountRet TouzhuLogRet;
		ZeroMemory(&TouzhuLogRet,sizeof(TouzhuLogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			TouzhuLogRet.dwTouzhuLogCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_LOG_COUNT_RET,dwContextID,&TouzhuLogRet,sizeof(TouzhuLogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_touzhu_log_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CString strLog;
		strLog.Format(L"查询投注记录数量,数据库异常");
		CTraceService::TraceString(strLog,TraceLevel_Exception);


	}
	return true;
}
//查询注册链接
bool CDataBaseEngineSink::OnGetRegUrl(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GetUrlFandian));
		if (wDataSize!=sizeof(DBR_GetUrlFandian)) return false;

		DBR_GetUrlFandian *pGetUrlFandian = (DBR_GetUrlFandian*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pGetUrlFandian->n_t_userid);
		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_reg_url"),true);

		DBO_GetRegUrl RegUrlRet[10];
		ZeroMemory(&RegUrlRet,sizeof(RegUrlRet));
		LONG lResult = 0;

		while(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			if(lResult>=200)
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_REG_URL_RET,dwContextID,&RegUrlRet,sizeof(DBO_GetRegUrl)*lResult);
				ZeroMemory(&RegUrlRet,sizeof(RegUrlRet));
				lResult = 0;
			}

			m_GameSSCDBAide.GetValue_String(TEXT("regURL"),RegUrlRet[lResult].s_reg_url,sizeof(RegUrlRet[lResult].s_reg_url));
			RegUrlRet[lResult].f_t_fandian = m_GameSSCDBAide.GetValue_DOUBLE(_T("fandian"));

			lResult++;
			m_GameSSCDBModule->MoveToNext();

		}
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_REG_URL_RET,dwContextID,&RegUrlRet,sizeof(DBO_GetRegUrl)*lResult);


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_reg_url");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询配额
bool CDataBaseEngineSink::OnGetPeie(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GetPeie));
		if (wDataSize!=sizeof(DBR_GetPeie)) return false;

		DBR_GetPeie *pGetUrlFandian = (DBR_GetPeie*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pGetUrlFandian->n_t_userid);
		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("b_get_PeiE"),true);

		DBR_GetPeieRet RegUrlRet;
		ZeroMemory(&RegUrlRet,sizeof(RegUrlRet));
		LONG lResult = 0;

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			RegUrlRet.n_t_peie_1 = m_GameSSCDBAide.GetValue_INT(TEXT("t_xiaji_count56"));
			RegUrlRet.n_t_peie_2 = m_GameSSCDBAide.GetValue_INT(TEXT("t_xiaji_count54"));
			RegUrlRet.n_t_peie_3 = m_GameSSCDBAide.GetValue_INT(TEXT("t_xiaji_count52"));
			RegUrlRet.n_t_peie_s_1 = m_GameSSCDBAide.GetValue_INT(TEXT("cnt3_0"));
			RegUrlRet.n_t_peie_s_2 = m_GameSSCDBAide.GetValue_INT(TEXT("cnt2_8"));
			RegUrlRet.n_t_peie_s_3 = m_GameSSCDBAide.GetValue_INT(TEXT("cnt2_7"));

		}
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GET_Peie_RET,dwContextID,&RegUrlRet,sizeof(RegUrlRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception b_get_PeiE");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询下级配额
bool CDataBaseEngineSink::OnHYXXXiaJiPeie(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GetXjPeie));
		if (wDataSize!=sizeof(DBR_GetXjPeie)) return false;

		DBR_GetXjPeie *pGetXjPeie = (DBR_GetXjPeie*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pGetXjPeie->n_t_userid);
		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("b_get_PeiE"),true);

		DBO_GetXjPeieRet RegUrlRet;
		ZeroMemory(&RegUrlRet,sizeof(RegUrlRet));
		LONG lResult = 0;

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			RegUrlRet.n_t_peie_1 = m_GameSSCDBAide.GetValue_INT(TEXT("t_xiaji_count56"));
			RegUrlRet.n_t_peie_2 = m_GameSSCDBAide.GetValue_INT(TEXT("t_xiaji_count54"));
			RegUrlRet.n_t_peie_3 = m_GameSSCDBAide.GetValue_INT(TEXT("t_xiaji_count52"));
			RegUrlRet.n_t_peie_s_1 = m_GameSSCDBAide.GetValue_INT(TEXT("cnt3_0"));
			RegUrlRet.n_t_peie_s_2 = m_GameSSCDBAide.GetValue_INT(TEXT("cnt2_8"));
			RegUrlRet.n_t_peie_s_3 = m_GameSSCDBAide.GetValue_INT(TEXT("cnt2_7"));

		}
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_GET_XJPEIE_RET,dwContextID,&RegUrlRet,sizeof(RegUrlRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception b_get_PeiE");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//设置下级配额
bool CDataBaseEngineSink::OnHYXXSetXiaJiPeie(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_SetXjPeie));
		if (wDataSize!=sizeof(DBR_SetXjPeie)) return false;

		DBR_SetXjPeie *pGetXjPeie = (DBR_SetXjPeie*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pGetXjPeie->n_t_userid);
		m_GameSSCDBAide.AddParameter(TEXT("@xjUserid"),pGetXjPeie->n_t_xj_id);
		m_GameSSCDBAide.AddParameter(TEXT("@allotNum1960"),pGetXjPeie->n_t_peie1);
		m_GameSSCDBAide.AddParameter(TEXT("@allotNum1956"),pGetXjPeie->n_t_peie2);
		m_GameSSCDBAide.AddParameter(TEXT("@allotNum1954"),pGetXjPeie->n_t_peie3);

		TCHAR szDescvalue[126] = TEXT("");
		m_GameSSCDBAide.AddParameterOutput(TEXT("@Desvalue"),szDescvalue,sizeof(szDescvalue),adParamOutput);


		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_set_UserAllotNum"),true);

		DBO_SetXjPeieRet SetXjPeieRet;
		ZeroMemory(&SetXjPeieRet,sizeof(SetXjPeieRet));
		SetXjPeieRet.n_t_result = lResultCode;

		//m_GameSSCDBAide.GetValue_String(TEXT("Desvalue"),SetXjPeieRet.s_t_desc,CountArray(SetXjPeieRet.s_t_desc));
		CDBVarValue DBVarValue;
		m_GameSSCDBModule->GetParameter(TEXT("@Desvalue"),DBVarValue);
		//OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
		lstrcpyn(SetXjPeieRet.s_t_desc,CW2CT(DBVarValue.bstrVal),CountArray(SetXjPeieRet.s_t_desc));

		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_SET_XJPEIE_RET,dwContextID,&SetXjPeieRet,sizeof(SetXjPeieRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception b_get_PeiE");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询提现记录数量
bool CDataBaseEngineSink::OnGetTixianLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetTixianLogCount));
		if (wDataSize!=sizeof(DBR_GP_GetTixianLogCount)) return false;
		
		DBR_GP_GetTixianLogCount* pLogCount = (DBR_GP_GetTixianLogCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_tixian_log_count"),true);

		DBO_GR_GetTixianLogCountRet TixianLogRet;
		ZeroMemory(&TixianLogRet,sizeof(TixianLogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			TixianLogRet.dwTixianLogCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TIXIAN_LOG_COUNT_RET,dwContextID,&TixianLogRet,sizeof(TixianLogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_tixian_log_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询充值记录数量
bool CDataBaseEngineSink::OnGetChongzhiLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetChongzhiLogCount));
		if (wDataSize!=sizeof(DBR_GP_GetChongzhiLogCount)) return false;
		
		DBR_GP_GetChongzhiLogCount* pLogCount = (DBR_GP_GetChongzhiLogCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_chongzhi_log_count"),true);

		DBO_GR_GetChongzhiLogCountRet ChonghziLogRet;
		ZeroMemory(&ChonghziLogRet,sizeof(ChonghziLogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			ChonghziLogRet.dwChongzhiLogCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHONGZHI_LOG_COUNT_RET,dwContextID,&ChonghziLogRet,sizeof(ChonghziLogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_chongzhi_log_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询会员信息记录数量
bool CDataBaseEngineSink::OnGetHYXXLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetHYXXLogCount));
		if (wDataSize!=sizeof(DBR_GP_GetHYXXLogCount)) return false;
		
		DBR_GP_GetHYXXLogCount* pLogCount = (DBR_GP_GetHYXXLogCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLogCount->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLogCount->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLogCount->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);
		m_GameSSCDBAide.AddParameter(TEXT("@online"),pLogCount->nOnline);
		m_GameSSCDBAide.AddParameter(TEXT("@yue"),pLogCount->nYue);

		CString strLog;
		strLog.Format(L"HYXXCOUNT user_id:%d,type:%d,xiaji_id:%d,xiaji_act:%s,by_time:%d,time_start:%s,time_end:%s,online:%d,yue:%d",pLogCount->dwUserID,pLogCount->n_t_type,pLogCount->n_t_user_id,pLogCount->s_t_account,pLogCount->bTime,pLogCount->szTimeStart,pLogCount->szTimeEnd,pLogCount->nOnline,pLogCount->nYue);
		OutputDebugString(strLog);
		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_hyxx_log_count"),true);

		DBO_GR_GetHYXXLogCountRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			LogRet.dwHYXXLogCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		strLog.Format(L"HYXXCOUNT lResultCode:%d,cnt:%d",lResultCode,LogRet.dwHYXXLogCount);
		OutputDebugString(strLog);
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_HYXX_LOG_COUNT_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_hyxx_log_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();
	}
	return true;
}
//查询充值记录数量
bool CDataBaseEngineSink::OnGetYingkuiLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetYingkuiLogCount));
		if (wDataSize!=sizeof(DBR_GP_GetYingkuiLogCount)) return false;
		
		DBR_GP_GetYingkuiLogCount* pLogCount = (DBR_GP_GetYingkuiLogCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_yingkui_log_count"),true);

		DBO_GR_GetYingkuiLogCountRet YingkuiLogRet;
		ZeroMemory(&YingkuiLogRet,sizeof(YingkuiLogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			YingkuiLogRet.dwYingkuiLogCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_YINGKUI_LOG_COUNT_RET,dwContextID,&YingkuiLogRet,sizeof(YingkuiLogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_yingkui_log_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询盈亏明细数量
bool CDataBaseEngineSink::OnGetYingkuiMxCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetYingkuiMxCount));
		if (wDataSize!=sizeof(DBR_GP_GetYingkuiMxCount)) return false;
		
		DBR_GP_GetYingkuiMxCount* pLogCount = (DBR_GP_GetYingkuiMxCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_yingkui_mx_count"),true);

		DBO_GR_GetYingkuiMxCountRet YingkuiMxRet;
		ZeroMemory(&YingkuiMxRet,sizeof(YingkuiMxRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			YingkuiMxRet.dwYingkuiMxCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_YINGKUI_MX_COUNT_RET,dwContextID,&YingkuiMxRet,sizeof(YingkuiMxRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_yingkui_mx_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询棋牌盈亏数量
bool CDataBaseEngineSink::OnGetQiPaiYingkuiCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetQiPaiYingkuiMxCount));
		if (wDataSize!=sizeof(DBR_GP_GetQiPaiYingkuiMxCount)) return false;
		
		DBR_GP_GetQiPaiYingkuiMxCount* pLogCount = (DBR_GP_GetQiPaiYingkuiMxCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLogCount->nTypeID);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->nByTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_yxcx_count"),true);

		DBO_GR_GetQiPaiYingkuiCountRet YingkuiRet;
		ZeroMemory(&YingkuiRet,sizeof(YingkuiRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			YingkuiRet.dwYingkuiMxCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QP_YINGKUI_COUNT_RET,dwContextID,&YingkuiRet,sizeof(YingkuiRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_yxcx_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询下级投注记录数量
bool CDataBaseEngineSink::OnGetXJTZHLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{

		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJTZHLogCount));
		if (wDataSize!=sizeof(DBR_GP_GetXJTZHLogCount)) return false;
		
		DBR_GP_GetXJTZHLogCount* pLogCount = (DBR_GP_GetXJTZHLogCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLogCount->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLogCount->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLogCount->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);


		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtzhtj_count"),true);

		DBO_GR_GetXJTZHLogCountRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			LogRet.dwCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();

		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJTZH_LOG_COUNT_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtzhtj_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询下级投注记录数量
bool CDataBaseEngineSink::OnCHKXJTZHLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_CHKXJTZHLogCount));
		if (wDataSize!=sizeof(DBR_GP_CHKXJTZHLogCount)) return false;
		
		DBR_GP_CHKXJTZHLogCount* pLogCount = (DBR_GP_CHKXJTZHLogCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLogCount->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLogCount->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLogCount->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtzh_log_count"),true);

		DBO_GR_CHKXJTZHLogCountRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			LogRet.dwCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHKXJTZH_LOG_COUNT_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtzh_log_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

	}
	return true;
}
//查询下级盈亏记录数量
bool CDataBaseEngineSink::OnXJYKLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJYKLogCount));
		if (wDataSize!=sizeof(DBR_GP_GetXJYKLogCount)) return false;
		
		DBR_GP_GetXJYKLogCount* pLogCount = (DBR_GP_GetXJYKLogCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLogCount->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLogCount->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLogCount->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
	//	m_GameSSCDBAide.AddParameter(TEXT("@t_xj_id"),pLogCount->nXiaJiaID);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);
		m_GameSSCDBAide.AddParameter(TEXT("@searchtype"),pLogCount->nType); //查询类型

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjyk_log_count"),true);

		DBO_GR_GetXJYKLogCountRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			LogRet.dwCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJYK_LOG_COUNT_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjyk_log_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询下级盈亏统计记录数量
bool CDataBaseEngineSink::OnXJYKTjCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJYKTjCount));
		if (wDataSize!=sizeof(DBR_GP_GetXJYKTjCount)) return false;
		
		DBR_GP_GetXJYKTjCount* pLogCount = (DBR_GP_GetXJYKTjCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLogCount->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLogCount->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLogCount->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjyktj_count"),true);

		DBO_GR_GetXJYKTjCountRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			LogRet.dwCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJYK_TJ_COUNT_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjyktj_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);


	}
	return true;
}
//查询下级游戏盈亏统计记录数量
bool CDataBaseEngineSink::OnXJYXTjCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXJYXTjCount));
		if (wDataSize!=sizeof(DBR_GP_GetXJYXTjCount)) return false;
		
		DBR_GP_GetXJYXTjCount* pLogCount = (DBR_GP_GetXJYXTjCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLogCount->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLogCount->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLogCount->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjyktj_qipai_count"),true);

		DBO_GR_GetXJYKTjCountRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			LogRet.dwCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJYX_TJ_COUNT_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjyktj_qipai_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询下级充值日志数量
bool CDataBaseEngineSink::OnXJCHZHLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXjChzhLogCount));
		if (wDataSize!=sizeof(DBR_GP_GetXjChzhLogCount)) return false;
		
		DBR_GP_GetXjChzhLogCount* pLogCount = (DBR_GP_GetXjChzhLogCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLogCount->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLogCount->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLogCount->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjchzh_log_count"),true);

		DBO_GR_GetXjChzhLogCountRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			LogRet.dwCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJCHZH_LOG_COUNT_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjchzh_log_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询下级提现日志数量
bool CDataBaseEngineSink::OnXJTxLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetXjTxLogCount));
		if (wDataSize!=sizeof(DBR_GP_GetXjTxLogCount)) return false;
		
		DBR_GP_GetXjTxLogCount* pLogCount = (DBR_GP_GetXjTxLogCount*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@type"),pLogCount->n_t_type);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_id"),pLogCount->n_t_user_id);
		m_GameSSCDBAide.AddParameter(TEXT("@xiaji_act"),pLogCount->s_t_account);
		m_GameSSCDBAide.AddParameter(TEXT("@by_time"),pLogCount->bTime);
		m_GameSSCDBAide.AddParameter(TEXT("@time_start"),pLogCount->szTimeStart);
		m_GameSSCDBAide.AddParameter(TEXT("@time_end"),pLogCount->szTimeEnd);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_xjtx_log_count"),true);

		DBO_GR_GetXjTxLogCountRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			LogRet.dwCount = m_GameSSCDBAide.GetValue_DWORD(TEXT("cnt"));
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_XJTX_LOG_COUNT_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_xjtx_log_count");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);


	}
	return true;
}
//查询彩票用户信息
bool CDataBaseEngineSink::OnGetCPUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetCpUserInfo));
		if (wDataSize!=sizeof(DBR_GP_GetCpUserInfo)) return false;
		
		DBR_GP_GetCpUserInfo* pLogCount = (DBR_GP_GetCpUserInfo*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->nUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_user_info"),true);

		DBO_GR_GetCpUserInfoRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			LogRet.n_t_userid = pLogCount->nUserID;
			m_GameSSCDBAide.GetValue_String(TEXT("t_account"),LogRet.s_t_account,sizeof(LogRet.s_t_account));
			LogRet.n_t_type = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
			LogRet.f_t_fandian = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_fandian"));
			m_GameSSCDBAide.GetValue_String(TEXT("t_qq"),LogRet.s_t_qq,sizeof(LogRet.s_t_qq));
			LogRet.n_t_daili_id = m_GameSSCDBAide.GetValue_INT(TEXT("t_daili_id"));
			LogRet.n_t_online = m_GameSSCDBAide.GetValue_INT(TEXT("t_online"));
			LogRet.f_t_yue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yue"));
			LogRet.f_t_dongjie = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_dongjie"));
			LogRet.n_t_ban = m_GameSSCDBAide.GetValue_INT(TEXT("t_ban"));

		}

		m_GameSSCDBModule->CloseRecordset();

		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_CP_USER_INFO_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_user_info");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询充值信息
bool CDataBaseEngineSink::OnGetChongzhiXinxi(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetChongzhiType));
		if (wDataSize!=sizeof(DBR_GP_GetChongzhiType)) return false;
		
		DBR_GP_GetChongzhiType* pLogCount = (DBR_GP_GetChongzhiType*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@t_user_id"),pLogCount->nUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@t_type"),pLogCount->cbType);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_chongzhi_xinxi"),true);

		DBO_GP_GetChongzhiTypeRet LogRet[10];
		ZeroMemory(&LogRet,sizeof(LogRet));
		LONG lResult = 0;
		while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
		{
			if(lResult >= 10)
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_CHONGZHI_TYPE_RET,dwContextID,&LogRet,sizeof(DBO_GP_GetChongzhiTypeRet)*lResult);
				ZeroMemory(&LogRet,sizeof(LogRet));
				lResult = 0;
			}
			m_GameSSCDBAide.GetValue_String(TEXT("t_yinhang"),LogRet[lResult].s_t_yinhang,sizeof(LogRet[lResult].s_t_yinhang));
			m_GameSSCDBAide.GetValue_String(TEXT("t_kaihuren"),LogRet[lResult].s_t_kaihuren,sizeof(LogRet[lResult].s_t_kaihuren));
			m_GameSSCDBAide.GetValue_String(TEXT("t_zhanghao"),LogRet[lResult].s_t_zhanghao,sizeof(LogRet[lResult].s_t_zhanghao));
			m_GameSSCDBAide.GetValue_String(TEXT("t_web"),LogRet[lResult].s_t_web,sizeof(LogRet[lResult].s_t_web));
			lResult++;
			m_GameSSCDBModule->MoveToNext();
		}

		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_CHONGZHI_TYPE_RET,dwContextID,&LogRet,sizeof(DBO_GP_GetChongzhiTypeRet)*lResult);


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_chongzhi_xinxi");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);


	}
	return true;
}
//查询取款信息
bool CDataBaseEngineSink::OnGetQukuanInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetQukuanInfo));
		if (wDataSize!=sizeof(DBR_GP_GetQukuanInfo)) return false;
		
		DBR_GP_GetQukuanInfo* pLogCount = (DBR_GP_GetQukuanInfo*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_tikuan"),true);

		DBO_GP_GetQukuanInfoRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			LogRet.f_t_yue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("yue"));
			m_GameSSCDBAide.GetValue_String(TEXT("t_yinhang"),LogRet.s_t_yinhang,sizeof(LogRet.s_t_yinhang));
			m_GameSSCDBAide.GetValue_String(TEXT("t_kaihuren"),LogRet.s_t_kaihuren,sizeof(LogRet.s_t_kaihuren));
			m_GameSSCDBAide.GetValue_String(TEXT("t_zhanghu"),LogRet.s_t_zhanghao,sizeof(LogRet.s_t_zhanghao));
			
			LogRet.n_t_isvip = m_GameSSCDBAide.GetValue_INT(TEXT("IsVIP"));
		}

		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_QUKUAN_INFO_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_tikuan");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询取款信息
bool CDataBaseEngineSink::OnQueryYinHangName(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{

 		m_GameSSCDBAide.ResetParameter();
 
 		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_yinhang_name"),true);

		if (lResultCode == DB_SUCCESS)
		{
			DBO_GP_GetAllYinHangNameRet LogRet[20];
			ZeroMemory(&LogRet,sizeof(LogRet));

			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 19)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_YINHANG_RET,dwContextID,&LogRet,sizeof(DBO_GP_GetAllYinHangNameRet)*lResult);
					ZeroMemory(&LogRet,sizeof(LogRet));
					lResult = 0;
				}

				m_GameSSCDBAide.GetValue_String(_T("yinhang"),LogRet->s_t_yinhang,CountArray(LogRet->s_t_yinhang));
				LogRet->n_QiYong = m_GameSSCDBAide.GetValue_INT(_T("qiyong"));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();

			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_YINHANG_RET,dwContextID,&LogRet,sizeof(DBO_GP_GetAllYinHangNameRet)*lResult);
		}

		return true;

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_yinhang_name");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询余额信息
bool CDataBaseEngineSink::OnGetYueInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetLastYue));
		if (wDataSize!=sizeof(DBR_GP_GetLastYue)) return false;
		
		DBR_GP_GetLastYue* pLogCount = (DBR_GP_GetLastYue*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_last_yue"),true);

		DBO_GP_GetLastYueRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			LogRet.f_t_yue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yue"));
			LogRet.f_t_dongjie = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_dongjie"));
			LogRet.f_t_score = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_score"));

		}
		CString strLog;
		strLog.Format(L"【%d】余额[%.4lf]棋牌[%.4lf]",pLogCount->dwUserID,LogRet.f_t_yue,LogRet.f_t_score);
		LogFile::instance().LogText(strLog);
		
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_YUE_INFO_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_last_yue");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询更多开奖记录
bool CDataBaseEngineSink::OnGetMoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetMoreRecord));
		if (wDataSize!=sizeof(DBR_GP_GetMoreRecord)) return false;
		
		DBR_GP_GetMoreRecord* pLogCount = (DBR_GP_GetMoreRecord*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@type_id"),pLogCount->n_t_type);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_more_record"),true);

		//结果处理
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GP_GetMoreRecordRet QueryLotResult[20];
			ZeroMemory(&QueryLotResult,sizeof(QueryLotResult));
			LONG lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult>=20)
				{
					break;
				}
				QueryLotResult[lResult].wKindID = m_GameSSCDBAide.GetValue_INT(TEXT("typeid"));
				m_GameSSCDBAide.GetValue_String(TEXT("expect"),QueryLotResult[lResult].wPeriod,CountArray(QueryLotResult[lResult].wPeriod));
				m_GameSSCDBAide.GetValue_String(TEXT("opencode"),QueryLotResult[lResult].szLotNum,CountArray(QueryLotResult[lResult].szLotNum));
				m_GameSSCDBAide.GetValue_String(TEXT("opentime"),QueryLotResult[lResult].szShijian,CountArray(QueryLotResult[lResult].szShijian));


				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_MORE_RECORD_RET,dwContextID,&QueryLotResult,sizeof(DBO_GP_GetMoreRecordRet)*lResult);
		}
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_more_record");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//查询代理回馈
bool CDataBaseEngineSink::OnGetDailiHuikui(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetDailiHuikui));
		if (wDataSize!=sizeof(DBR_GP_GetDailiHuikui)) return false;
		
		DBR_GP_GetDailiHuikui* pLogCount = (DBR_GP_GetDailiHuikui*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->n_t_userid);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_check_daililj"),true);

		DBO_GP_GetDailiHuikuiRet GetDailiHuikuiRet;
		ZeroMemory(&GetDailiHuikuiRet,sizeof(GetDailiHuikuiRet));

		GetDailiHuikuiRet.cbNewRegUser = m_GameSSCDBAide.GetValue_INT(TEXT("NewReg")); //是否新注册用户
		GetDailiHuikuiRet.f_t_touzhu = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("touzhu")); //投注量
		GetDailiHuikuiRet.f_t_chongzhi = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("chongzhi")); //充值量

		m_GameSSCDBModule->CloseRecordset();

			//发送结果
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_DAILI_HUIKUI_RET,dwContextID,&GetDailiHuikuiRet,sizeof(GetDailiHuikuiRet));
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_check_daililj");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//代理领奖
bool CDataBaseEngineSink::OnDailiLingjiang(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_DailiLingjiang));
		if (wDataSize!=sizeof(DBR_GP_DailiLingjiang)) return false;
		
		DBR_GP_DailiLingjiang* pDailiLingjiang = (DBR_GP_DailiLingjiang*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pDailiLingjiang->n_t_userid);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_daili_lingjiang"),true);

		DBO_GP_DailiLingjiangRet DailiLingjiang;
		ZeroMemory(&DailiLingjiang,sizeof(DailiLingjiang));

		DailiLingjiang.n_t_userid = pDailiLingjiang->n_t_userid;
		DailiLingjiang.n_t_state = m_GameSSCDBAide.GetValue_INT(TEXT("t_state")); //签到返回
		DailiLingjiang.f_t_jine = m_GameSSCDBAide.GetValue_INT(TEXT("t_jine")); //签到返回
		m_GameSSCDBModule->CloseRecordset();
			//发送结果
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_DAILI_LJ_RET,dwContextID,&DailiLingjiang,sizeof(DailiLingjiang));
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_daili_lingjiang");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//退出游戏
bool CDataBaseEngineSink::OnQuitGame(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_QuitGame));
		if (wDataSize!=sizeof(DBR_GP_QuitGame)) return false;
		
		DBR_GP_QuitGame* pLogCount = (DBR_GP_QuitGame*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_logout"),false);

		CString strLog;
		strLog.Format(L"%d退出游戏",pLogCount->dwUserID);
		LogFile::instance().LogText(strLog);

 		DBO_GP_QuitGameRet LogRet;
 		ZeroMemory(&LogRet,sizeof(LogRet));
 		
 		LogRet.nResult = lResultCode;
		LogRet.nUserID = pLogCount->dwUserID;
 		//发送结果
 		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUIT_GAME_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_logout");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//签到
bool CDataBaseEngineSink::OnUserQiandao(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_UserQiandao));
		if (wDataSize!=sizeof(DBR_GP_UserQiandao)) return false;
		
		DBR_GP_UserQiandao* pLogCount = (DBR_GP_UserQiandao*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_user_signedin_new"),true);

		DBO_GP_UserQiandaoRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		LogRet.n_t_res = lResultCode;
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_QIANDAO_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_user_signedin");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//欢乐送
bool CDataBaseEngineSink::OnGetUserHuanlesong(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetUserHuanlesong));
		if (wDataSize!=sizeof(DBR_GP_GetUserHuanlesong)) return false;
		
		DBR_GP_GetUserHuanlesong* pLogCount = (DBR_GP_GetUserHuanlesong*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pLogCount->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_userhuanlesong_new"),true);

		DBO_GP_GetUserHuanlesongRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		int nres = 0;
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			nres = m_GameSSCDBAide.GetValue_INT(TEXT("res"));
		LogRet.nRes = nres;
	//	if(nres<2)
		{
			if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			{
				LogRet.f_t_xiaofei = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("xiaofei"));
				LogRet.f_t_prize = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("prize"));
				LogRet.n_t_count = m_GameSSCDBAide.GetValue_INT(TEXT("NCOUNT"));
				LogRet.n_t_XXcount =  m_GameSSCDBAide.GetValue_INT(TEXT("XXcount"));

			}

		}
		m_GameSSCDBModule->CloseRecordset();
		
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_HUANLE_SONG_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_userhuanlesong");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//领奖
bool CDataBaseEngineSink::OnGetUserLingJiang(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetLingJiang));
		if (wDataSize!=sizeof(DBR_GP_GetLingJiang)) return false;
		
		DBR_GP_GetLingJiang* pLogCount = (DBR_GP_GetLingJiang*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_UserHuanlesongLingjiang"),true);

		DBO_GP_GetLingJiangRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		int nres = 0;
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			nres = m_GameSSCDBAide.GetValue_INT(TEXT("res"));
		LogRet.n_t_got = nres;
		if(nres==0)
		{
			if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
				LogRet.f_t_jine = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("prize"));
		}
		m_GameSSCDBModule->CloseRecordset();
		
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_LING_JIANG_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_UserHuanlesongLingjiang");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}
	return true;
}
//取款
bool CDataBaseEngineSink::OnDoQukuan(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_DoQukuan));
		if (wDataSize!=sizeof(DBR_GP_DoQukuan)) return false;
		
		DBR_GP_DoQukuan* pLogCount = (DBR_GP_DoQukuan*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->dwUserID);
		m_GameSSCDBAide.AddParameter(TEXT("@jine"),pLogCount->fJine);
		m_GameSSCDBAide.AddParameter(TEXT("@tikuan_pwd"),pLogCount->szQukuanPass);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_GameSSCDBAide.AddParameterOutput(TEXT("@msg"),szDescribe,sizeof(szDescribe),adParamOutput);


		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_qukuan"),true);

		DBO_GP_DoQukuanRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));
		
		LogRet.nResult = lResultCode;
		if(lResultCode == 8)
		{
			//获取信息
			CDBVarValue DBVarValue;
			m_GameSSCDBAide.GetParameter(TEXT("@msg"),DBVarValue);

			lstrcpyn(LogRet.szDesc,CW2CT(DBVarValue.bstrVal),CountArray(LogRet.szDesc));
		}
		m_GameSSCDBModule->CloseRecordset();
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_DO_QUKUAN_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_qukuan");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

	}
	return true;
}
//查询玩家信息
bool CDataBaseEngineSink::OnQueryCpUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetCpUser));
		if (wDataSize!=sizeof(DBR_GP_GetCpUser)) return false;
		
		DBR_GP_GetCpUser* pLogCount = (DBR_GP_GetCpUser*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->n_t_userid);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_user_info"),true);

		DBO_GR_GetCpUserInfoRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			LogRet.n_t_userid = pLogCount->n_t_userid;
			m_GameSSCDBAide.GetValue_String(TEXT("t_account"),LogRet.s_t_account,sizeof(LogRet.s_t_account));
			LogRet.n_t_type = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
			LogRet.f_t_fandian = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_fandian"));
			m_GameSSCDBAide.GetValue_String(TEXT("t_qq"),LogRet.s_t_qq,sizeof(LogRet.s_t_qq));
			LogRet.n_t_daili_id = m_GameSSCDBAide.GetValue_INT(TEXT("t_daili_id"));
			LogRet.n_t_online = m_GameSSCDBAide.GetValue_INT(TEXT("t_online"));
			LogRet.f_t_yue = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_yue"));
			LogRet.f_t_dongjie = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("t_dongjie"));
			LogRet.n_t_ban = m_GameSSCDBAide.GetValue_INT(TEXT("t_ban"));

		}

		m_GameSSCDBModule->CloseRecordset();

		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_CP_USER_INFO_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_user_info");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

	}
	return true;
}
//查询玩家信息
bool CDataBaseEngineSink::OnGetMyMapBonus(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetMyMapBonus));
		if (wDataSize!=sizeof(DBR_GP_GetMyMapBonus)) return false;
		
		DBR_GP_GetMyMapBonus* pLogCount = (DBR_GP_GetMyMapBonus*)pData;

		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pLogCount->n_t_userid);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_my_bonus"),true);

		DBO_GP_GetMyMapBonusRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
		{
			LogRet.n_t_type = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
		}

		m_GameSSCDBModule->CloseRecordset();

		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_MY_MAP_BONUS_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_my_bonus");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

	}
	return true;
}
//查询玩家返点
bool CDataBaseEngineSink::OnGetUserFandian(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetUserFandian));
		if (wDataSize!=sizeof(DBR_GP_GetUserFandian)) return false;
		
		DBR_GP_GetUserFandian* pLogCount = (DBR_GP_GetUserFandian*)pData;


		m_GameSSCDBAide.ResetParameter();
		m_GameSSCDBAide.AddParameter(TEXT("@userid"),pLogCount->n_t_userid);
		m_GameSSCDBAide.AddParameter(TEXT("@typeid"),pLogCount->n_t_type_id);
		m_GameSSCDBAide.AddParameter(TEXT("@kindid"),pLogCount->n_t_kind_id);


		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_user_bonus"),true);

		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetUserFandianRet LogRet[200];
			ZeroMemory(&LogRet,sizeof(LogRet));
			LONG lResult = 0;
			while(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			{
				if(lResult>=200)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_USER_FANDIAN_RET,dwContextID,&LogRet,sizeof(DBO_GR_GetUserFandianRet)*lResult);
					lResult = 0;
				}
// 				LogRet[lResult].n_t_userid = pLogCount->n_t_userid;
// 				LogRet[lResult].n_t_typeid = pLogCount->n_t_type_id;
				LogRet[lResult].n_t_kindid = m_GameSSCDBAide.GetValue_INT(TEXT("kindid"));
				LogRet[lResult].f_t_bonus = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("bonus"));
				LogRet[lResult].f_t_bonusPercent = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("bonusPercent"));
				LogRet[lResult].f_t_fandian = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("Fandian"));


				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_USER_FANDIAN_RET,dwContextID,&LogRet,sizeof(DBO_GR_GetUserFandianRet)*lResult);


		}


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_user_bonus");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

	}
	return true;
}
//设置玩家返点
bool CDataBaseEngineSink::OnSetUserBonus(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_SetUserBonus));
		if (wDataSize!=sizeof(DBR_GP_SetUserBonus)) return false;
		
		
		DBO_GR_SetUserBonusRet LogRet;
		ZeroMemory(&LogRet,sizeof(LogRet));

		DBR_GP_SetUserBonus* pLogCount = (DBR_GP_SetUserBonus*)pData;
		if(pLogCount->f_t_bonus == 0)
		{
			LogRet.n_t_result = 1;
		}
		else
		{
			m_GameSSCDBAide.ResetParameter();
			m_GameSSCDBAide.AddParameter(TEXT("@userid"),pLogCount->n_t_userid);
			m_GameSSCDBAide.AddParameter(TEXT("@typeid"),pLogCount->n_t_type_id);
			m_GameSSCDBAide.AddParameter(TEXT("@kindid"),pLogCount->n_t_kind_id);
			m_GameSSCDBAide.AddParameter(TEXT("@bonus"),pLogCount->f_t_bonus);
			m_GameSSCDBAide.AddParameter(TEXT("@bonusPercent"),pLogCount->f_bonuspercent);
			LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_set_user_bonus"),false);

			LogRet.n_t_result = lResultCode;
		}

		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SET_USER_BONUS_RET,dwContextID,&LogRet,sizeof(LogRet));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_set_user_bonus");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

	}
	return true;
}

//获取系统时间
bool CDataBaseEngineSink::OnQuerySystemTime(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{

		m_GameSSCDBAide.ResetParameter();

		m_GameSSCDBAide.AddParameter(TEXT("@from"),1);
		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_systime"),true);

		DBO_GR_GetSystemTime GetSysTime;
		ZeroMemory(&GetSysTime,sizeof(GetSysTime));
		if(m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			m_GameSSCDBAide.GetValue_SystemTime(TEXT("dte"),GetSysTime.ServerTime);

		m_GameSSCDBModule->CloseRecordset();
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_SYS_TIME_RET,dwContextID,&GetSysTime,sizeof(GetSysTime));


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_systime");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();
		return false;
	}
	return true;
}
//获取中奖用户ID
bool CDataBaseEngineSink::OnQueryWinUserID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{


		m_GameSSCDBAide.ResetParameter();

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_touzhu_tishi_web"),true);

		LONG lResult = 0;
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetWinUserID GetWinUserID[200];
			ZeroMemory(&GetWinUserID,sizeof(GetWinUserID));
			while (m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			{
				if(lResult>=200)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_WIN_USER_RET,dwContextID,&GetWinUserID,sizeof(DBO_GR_GetWinUserID)*lResult);
					ZeroMemory(&GetWinUserID,sizeof(GetWinUserID));
					lResult = 0;
				}
				GetWinUserID[lResult].nUserID = m_GameSSCDBAide.GetValue_INT(TEXT("t_user_id"));
				GetWinUserID[lResult].nWinZhushu = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("sumwinzhushu"));
				GetWinUserID[lResult].nState = m_GameSSCDBAide.GetValue_INT(TEXT("t_state"));
				m_GameSSCDBAide.GetValue_String(TEXT("t_qishu"),GetWinUserID[lResult].szQishu,sizeof(GetWinUserID[lResult].szQishu));
				GetWinUserID[lResult].nType = m_GameSSCDBAide.GetValue_INT(TEXT("t_type"));
				GetWinUserID[lResult].f_yingkui = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("sumyingkui"));
				GetWinUserID[lResult].nZhushu = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("sumzhushu"));

				CString strLog;
				strLog.Format(L"ZHONGJIANGTISHI [%d] touzhu:%.2lf,zhongjiang:%.2lf,yingkui:%.2lf",GetWinUserID[lResult].nUserID,GetWinUserID[lResult].nZhushu,GetWinUserID[lResult].nWinZhushu,GetWinUserID[lResult].f_yingkui);
				OutputDebugString(strLog);
				lResult++;
				m_GameSSCDBModule->MoveToNext();

			}

			CString strLog;
			strLog.Format(L"TOUZHUWINNER lResult:%d",lResult);
			OutputDebugString(strLog);
			m_GameSSCDBModule->CloseRecordset();

			if(lResult >0)
			{
				m_GameSSCDBAide.ResetParameter();

				lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_update_tishi"),false);

			}
			else
			{

				CString strLog;
				strLog.Format(L"ZHONGJIANGTISHI 没有中奖");
				OutputDebugString(strLog);

			}
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_WIN_USER_RET,dwContextID,&GetWinUserID,sizeof(DBO_GR_GetWinUserID)*lResult);

		}

	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_touzhu_tishi_web");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();
		return false;
	}
	return true;
}
//获取MAPBONUS
bool CDataBaseEngineSink::OnQueryMapBonus(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{

		m_GameSSCDBAide.ResetParameter();
		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_map_bonus_new"),true);

		if(lResultCode == DB_SUCCESS)
		{
			DBO_GR_GetMapBonusRet LogRet[500];
			ZeroMemory(&LogRet,sizeof(LogRet));
			LONG lResult = 0;
			while (m_GameSSCDBModule->IsRecordsetEnd() == FALSE)
			{
				if(lResult>=500)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_MAP_BONUS_RET,dwContextID,&LogRet,sizeof(DBO_GR_GetMapBonusRet)*lResult);
					ZeroMemory(&LogRet,sizeof(LogRet));
					lResult = 0;
				}
				LogRet[lResult].n_t_type_id = m_GameSSCDBAide.GetValue_INT(TEXT("TypeID"));
				LogRet[lResult].n_t_kind_id = m_GameSSCDBAide.GetValue_INT(TEXT("KindID"));
				LogRet[lResult].f_t_bonus = m_GameSSCDBAide.GetValue_DOUBLE(TEXT("bonus"));
				LogRet[lResult].n_t_bonustype = m_GameSSCDBAide.GetValue_INT(TEXT("bonustype"));

				if(LogRet[lResult].n_t_kind_id == 66 && LogRet[lResult].n_t_type_id == 1)
				{
					CString strLog;
					strLog.Format(L"\nData GETMAPBONUS f_t_bonus:%0.2lf,n_t_kind_id:%d,n_t_type_id:%d",LogRet[lResult].f_t_bonus,LogRet[lResult].n_t_kind_id,LogRet[lResult].n_t_type_id);
					OutputDebugString(strLog);

				}

// 				CString strLog;
// 				strLog.Format(L"PMAPBONUS typeid:%d,kindid:%d,bonus:%.2lf,type:%d",LogRet[lResult].n_t_type_id,LogRet[lResult].n_t_kind_id,LogRet[lResult].f_t_bonus,LogRet[lResult].n_t_bonustype);
// 				OutputDebugString(strLog);
				lResult++;
				m_GameSSCDBModule->MoveToNext();

			}
			m_GameSSCDBModule->CloseRecordset();

			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GET_MAP_BONUS_RET,dwContextID,&LogRet,sizeof(DBO_GR_GetMapBonusRet)*lResult);

		}


	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_map_bonus");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

	}
	return true;
}
//投注
bool CDataBaseEngineSink::OnGetKefuUrl(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		m_GameSSCDBAide.ResetParameter();

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_getkefu"),true);

		DBO_GP_GetKefuUrlRet GetKefuUrlRet;
		ZeroMemory(&GetKefuUrlRet,sizeof(GetKefuUrlRet));

		GetKefuUrlRet.n_t_result = lResultCode;
		m_GameSSCDBAide.GetValue_String(TEXT("serviceURL"),GetKefuUrlRet.s_t_desc,sizeof(GetKefuUrlRet.s_t_desc));
		m_GameSSCDBModule->CloseRecordset();

		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_KEFU_URL_RET,dwContextID,&GetKefuUrlRet,sizeof(GetKefuUrlRet));
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_map_bonus");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

	}
	return true;

}
//投注
bool CDataBaseEngineSink::OnTouzhuCQSSC(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_TouZhuCQSSC));
		if (wDataSize!=sizeof(DBR_GP_TouZhuCQSSC)) return false;

		DBR_GP_TouZhuCQSSC* pTouzhuCQSSC = (DBR_GP_TouZhuCQSSC*)pData;

		if(pTouzhuCQSSC->nZhushu <= 0)
		{
			return false;
		}

		CString strAllHaoma = ChangeStringToT(pTouzhuCQSSC->strHaoma);
		if(pTouzhuCQSSC->nGameKind == ZhongSanZhiXuanHeZhi || pTouzhuCQSSC->nGameKind == QianSanZhiXuanHeZhi || pTouzhuCQSSC->nGameKind == HouSanZhiXuanHeZhi)
		{
			CString strDanhao;
			AfxExtractSubString(strDanhao,strAllHaoma,0,',');
			if(strDanhao == strAllHaoma)
			{
				strAllHaoma = AddDouHao(strAllHaoma);
			}
		}

// 		//正式语句
// 		if(!CheckTouzhuNum(strAllHaoma,0,pTouzhuCQSSC->dwUserID,ChangeStringToT(pTouzhuCQSSC->strQishu)))
// 		{
// 			return false;
// 		}
		int nCheckZongzhu = GetTouZhuZongShu(pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma);
		int nZhushu  = 0;
		nZhushu= pTouzhuCQSSC->nZhushu;
		CString strLog;
		strLog.Format(L"TOUZHU [%d] TypeID:%d,KindID:%d,haoma:[%s],zhushu:%d,checkzhushu:%d",pTouzhuCQSSC->dwUserID,pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma,nZhushu,nCheckZongzhu);
		CTraceService::TraceString(strLog,TraceLevel_Normal);
		if ((pTouzhuCQSSC->nGameType == CZ_PK10&&pTouzhuCQSSC->nGameKind == WF_GYHZH )||((pTouzhuCQSSC->nGameType == CZ3D||pTouzhuCQSSC->nGameType == CZPaiLie3)&&pTouzhuCQSSC->nGameKind == enWF_ZhixuanHezhi))
		{
			nCheckZongzhu = nZhushu;
		}

		if(nCheckZongzhu != nZhushu)
		{
			TCHAR szClientAddr[16]=TEXT("");
			BYTE * pClientAddr=(BYTE *)&pTouzhuCQSSC->dwClientAddr;
			_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

			CString strLogFile;
			strLogFile.Format(L"WRONGTOUZHU [%d] TypeID:%d,KindID:%d,haoma:[%s],zhushu:%d,checkzhushu:%d,ip:%s",pTouzhuCQSSC->dwUserID,pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma,nZhushu,nCheckZongzhu,szClientAddr);
			LogFile::instance().LogText(strLogFile);

			DBO_GR_TouzhuResult  TouzhuRes;
			ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
			TouzhuRes.lResultCode = 11;
			TouzhuRes.nSign = pTouzhuCQSSC->nSign;
			TouzhuRes.nUserID = pTouzhuCQSSC->dwUserID;


			ZeroMemory(&szHaoma,sizeof(szHaoma));
			nZhushu = 0;
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CQSSC_RET,dwContextID,&TouzhuRes,sizeof(TouzhuRes));

			return false;
		}
// 		strLog.Format(L"CQSSCTZH touzhu4");
// 		OutputDebugString(strLog);

		if(pTouzhuCQSSC->nZhushu <= 0)
		{
			return false;
		}

 		m_GameSSCDBAide.ResetParameter();
 		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pTouzhuCQSSC->dwUserID);
 		m_GameSSCDBAide.AddParameter(TEXT("@qishu"),ChangeStringToT(pTouzhuCQSSC->strQishu));
 		m_GameSSCDBAide.AddParameter(TEXT("@type"),pTouzhuCQSSC->nGameType);
 		m_GameSSCDBAide.AddParameter(TEXT("@kind"),pTouzhuCQSSC->nGameKind);
 		m_GameSSCDBAide.AddParameter(TEXT("@haoma"),strAllHaoma);
 		m_GameSSCDBAide.AddParameter(TEXT("@zhushu"),pTouzhuCQSSC->nZhushu);
 		m_GameSSCDBAide.AddParameter(TEXT("@beishu"),pTouzhuCQSSC->nBeitou);
		m_GameSSCDBAide.AddParameter(TEXT("@moshi"),pTouzhuCQSSC->nMoshi);
		m_GameSSCDBAide.AddParameter(TEXT("@isZhuihao"),pTouzhuCQSSC->bZhuihao);
		m_GameSSCDBAide.AddParameter(TEXT("@platform"),pTouzhuCQSSC->cbPlatform);


		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_touzhu"),false);
		strLog.Format(L"CQSSCTZH touzhu5");
		OutputDebugString(strLog);


		if(lResultCode != 0)
		{
			USES_CONVERSION;
			CString strLog;
			strLog.Format(L"期号：%s, 投注返回错误：%d", A2T(pTouzhuCQSSC->strQishu), lResultCode);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
		}

		DBO_GR_TouzhuResult  TouzhuRes;
		ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
		TouzhuRes.lResultCode = lResultCode;
		TouzhuRes.nSign = pTouzhuCQSSC->nSign;
		TouzhuRes.nUserID = pTouzhuCQSSC->dwUserID;

		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CQSSC_RET,dwContextID,&TouzhuRes,sizeof(TouzhuRes));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception OnTouzhuCQSSC p_touzhu");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();

		return false;
	}
	return true;
}
//获取投注数
int CDataBaseEngineSink::GetTouZhuZongShu(int nTypeID,int nKindID,CString strHaoma)
{
	int nZongZhu = 1;

	switch(nTypeID)
	{
	case CZChongQingSSC:
	case CZJiangXiSSC:
	case  CZXinJiangSSC:
	case CZ_FENFEN_CAI:
	case CZ_WUFEN_CAI:
	case CZ_TianJinSSC:
	case CZ_HGYDWFC:
		{
			nZongZhu = GetSSCZhushu(nTypeID,nKindID,strHaoma);
			CString strLog;
			strLog.Format(L"本次共:%d注",nZongZhu);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
			return nZongZhu;
		}
	case CZGD11Xuan5:
	case CZSD11Xuan5:
	case CZJX11Xuan5:
	case CZHLJ11Xuan5:
		{
			nZongZhu = Get11X5Zhushu(nTypeID,nKindID,strHaoma);
			CString strLog;
			strLog.Format(L"本次共:%d注",nZongZhu);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
			return nZongZhu;
		}
	case CZ3D:
	case CZPaiLie3:
		{
			nZongZhu = Get3DZhushu(nTypeID,nKindID,strHaoma);
			CString strLog;
			strLog.Format(L"本次共:%d注",nZongZhu);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
			return nZongZhu;
		}
	case CZ_QiXingCai:
		{
			nZongZhu = GetQXCZhushu(nTypeID,nKindID,strHaoma);
			CString strLog;
			strLog.Format(L"本次共:%d注",nZongZhu);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
			return nZongZhu;

		}
	case CZ_PK10:
		{
			nZongZhu = GetPK10Zhushu(nTypeID,nKindID,strHaoma);
			CString strLog;
			strLog.Format(L"本次共:%d注",nZongZhu);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
			return nZongZhu;

		}
	case CZKUAILE8:
		{
			nZongZhu = GetKL8Zhushu(nTypeID,nKindID,strHaoma);
			CString strLog;
			strLog.Format(L"本次共:%d注",nZongZhu);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
			return nZongZhu;
		}
	case CZXingYun28:
		{
			nZongZhu = GetXY28Zhushu(nTypeID,nKindID,strHaoma);
			CString strLog;
			strLog.Format(L"本次共:%d注",nZongZhu);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
			return nZongZhu;
		}
	}
	return nZongZhu;
}
#include <vector>
int SplitString(const string &srcStr, vector<string> &destVec, const string splitStr="|")
{
	if(srcStr.size()==0 || srcStr.empty())
	{   
		return false;
	}   
	size_t oldPos,newPos;
	oldPos=0;
	newPos=0;
	string tempData;
	while(1)
	{   
		newPos=srcStr.find(splitStr,oldPos);
		if(newPos!=string::npos)
		{   
			tempData = srcStr.substr(oldPos,newPos-oldPos);
			destVec.push_back(tempData);
			oldPos=newPos+splitStr.size();
		}   
		else if(oldPos<=srcStr.size())
		{   
			tempData= srcStr.substr(oldPos);
			destVec.push_back(tempData);
			break;
		}   
		else
		{   
			break;
		}   
	}   
	return true;
}
#include <algorithm>
#include <functional> 
//获取投注数
int CDataBaseEngineSink::GetSSCZhushu(int nTypeID,int nKindID,CString strHaoma)
{
	int nZongZhu = 0;
	int nIndex = 0;
	CString strTempHaoma;
	CString strLog;

 	string strGetHaoma;
 
 	ChangeStringToA(strHaoma,strGetHaoma);
 
	vector<string> vectHaoma;

	if(SplitString(strGetHaoma,vectHaoma) == false)
	{
		return 0;
	}


	size_t vectSize = vectHaoma.size();
	for(int i = 0;i < vectSize;i++)
	{
		string haoma = vectHaoma[i];
		if(haoma.empty())
			continue;
		vector<string> vectDanHaoma;
		SplitString(haoma,vectDanHaoma,",");
		size_t vectDanSize = vectDanHaoma.size();

		if(nKindID == WuxingZhixuan)  //五星
		{
			int nSingleZhushu = 1;
			
			if(vectDanSize == 1&& haoma == vectDanHaoma[0].c_str())
			{
				nSingleZhushu = 1;
			}
			else if(vectDanSize == 5)
			{
				for(int j = 0;j < vectDanSize;j++)
				{
					string strDanHaoma = vectDanHaoma[j].c_str();
					if (strDanHaoma.empty())
					{
						return 0;
					}
					int nLength = strDanHaoma.size();
					nSingleZhushu *=nLength;
				}
			}
			else 
			{
				return 0;
			}

			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == SSC_LongHu)
		{
			string strDanHaoma = vectDanHaoma[0].c_str();
			if(strDanHaoma.empty())
				return 0;
			int nLeng = strDanHaoma.length();
			nZongZhu += nLeng;
		}
		else if(nKindID == WuxingQianSi || nKindID == WuxingHousi)  // 四星
		{
			int nSingleZhushu = 1;

			if(vectDanSize == 1&& haoma == vectDanHaoma[0].c_str())
			{
				nSingleZhushu = 1;
			}
			else if(vectDanSize == 4)
			{
				for(int j = 0;j < vectDanSize;j++)
				{
					string strDanHaoma = vectDanHaoma[j].c_str();
					if (strDanHaoma.empty())
					{
						return 0;
					}
					int nLength = strDanHaoma.size();
					nSingleZhushu *=nLength;
				}
			}
			else 
			{
				return 0;
			}

			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == WuxingZhongsan || nKindID == QianSanZhiXuan_FuShi || nKindID == HouSanZhiXuan_FuShi) // 三星
		{
			int nSingleZhushu = 1;

			if(vectDanSize == 1&& haoma == vectDanHaoma[0].c_str())
			{
				nSingleZhushu = 1;
			}
			else if(vectDanSize == 3)
			{
				for(int j = 0;j < vectDanSize;j++)
				{
					string strDanHaoma = vectDanHaoma[j].c_str();
					if (strDanHaoma.empty())
					{
						return 0;
					}
					int nLength = strDanHaoma.size();
					nSingleZhushu *=nLength;
				}
			}
			else 
			{
				return 0;
			}

			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == WuXingQianEr_FuShi || nKindID == WuXingHouEr_FuShi || nKindID == WuXingHouEr_DanShi|| nKindID == WuXingQianEr_DanShi)   //二星
		{
			int nSingleZhushu = 1;

			if(vectDanSize == 1&& haoma == vectDanHaoma[0].c_str())
			{
				nSingleZhushu = 1;
			}
			else if(vectDanSize == 2)
			{
				for(int j = 0;j < vectDanSize;j++)
				{
					string strDanHaoma = vectDanHaoma[j].c_str();
					if (strDanHaoma.empty())
					{
						return 0;
					}
					int nLength = strDanHaoma.size();
					nSingleZhushu *=nLength;
				}
			}
			else 
			{
				return 0;
			}

			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == QianSanZuXuan_ZuLiuFuShi || nKindID == HouSanZuXuan_ZuLiuFuShi || nKindID == ZhongsanZuliu) //前三后三中三   组选六
		{
			int nLength = haoma.size()/2+1;
			int nSingleZhu = (nLength*(nLength-1)*(nLength-2))/6;


			nZongZhu +=nSingleZhu;

		}
		else if(nKindID == QianSanZuXuan_ZuSanFuShi || nKindID == HouSanZuXuan_ZuSanFuShi || nKindID == ZhongsanZusan)  //前三后三中三   组选三
		{
			int nLength = haoma.size()/2+1;

			bool bKeyboard = false;
			if(nLength == 3)
			{

				if((vectDanHaoma[0] == vectDanHaoma[1])||(vectDanHaoma[1] == vectDanHaoma[2])||(vectDanHaoma[0] == vectDanHaoma[2]))
				{
					bKeyboard = true;
				}
			}
			int nSingleZhu = 0;
			if(bKeyboard)
				nSingleZhu = 1;
			else
				nSingleZhu = (nLength*(nLength-1));


			nZongZhu +=nSingleZhu;
		}
		else if(nKindID == QianErZuXuan_FuShi || nKindID == HouErZuXuan_FuShi || nKindID == QianErZuXuan_DanShi || nKindID == HouErZuXuan_DanShi)  //前二 后二组选
		{

			int nLength = haoma.size()/2+1;
			int nSingleZhu = 0;

			nSingleZhu = (nLength*(nLength-1))/2;

			nZongZhu += nSingleZhu;

		}
		else if(nKindID == QianSanZhiXuanHeZhi || nKindID == HouSanZhiXuanHeZhi || nKindID == ZhongSanZhiXuanHeZhi)
		{
			int nSingleZhushu = 0;
			for(int n = 0;n < vectDanSize;n++)
			{
				string strDanHaoma = vectDanHaoma[n].c_str();
				if (strDanHaoma.empty())
				{
					continue;
				}
				int nHezhi = 0;
				nHezhi = atoi(strDanHaoma.c_str());

				for(int m = 0;m < 10;m++)
				{
					for(int n = 0;n < 10;n++)
					{
						for (int o = 0;o < 10;o++)
						{
							if((m+n+o) == nHezhi)
							{
								nSingleZhushu++;
							}
						}
					}
				}

			}
			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == QianErZhixuan_Hezhi || nKindID == HouErZhixuan_Hezhi)
		{
			int nSingleZhushu = 0;
			for(int j = 0;j < vectDanSize;j++)
			{
				string strDanHaoma = vectDanHaoma[j].c_str();
				if (strDanHaoma.empty())
				{
					continue;
				}
				int nHezhi = 0;
				nHezhi = atoi(strDanHaoma.c_str());

				for(int m = 0;m < 10;m++)
				{
					for(int n = 0;n < 10;n++)
					{
						if((m+n) == nHezhi)
						{
							nSingleZhushu++;
						}
					}
				}

			}
			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == DaXiaoDanShuang)
		{
			int nSingleZhushu = 1;

			for(int j = 0;j < vectDanSize;j++)
			{
				string strDanHaoma = vectDanHaoma[j].c_str();
				if (strDanHaoma.empty())
				{
					continue;
				}
				int nLength = strDanHaoma.size();
				nSingleZhushu *= nLength;
			}

			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == Zonghe_DXDSLh)
		{
			int nSingleZhushu = 0;

			for(int j = 0;j < vectDanSize;j++)
			{
				string strDanHaoma = vectDanHaoma[j].c_str();
				if (strDanHaoma.empty())
				{
					continue;
				}
				int nLength = strDanHaoma.size();
				nSingleZhushu += nLength;
			}

			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == DaXiaoDanShuang_dw)
		{

			int nSingleZhushu = 0;

			for(int j = 0;j < vectDanSize;j++)
			{
				string strDanHaoma = vectDanHaoma[j].c_str();
				if (strDanHaoma.empty())
				{
					continue;
				}
				int nLength = strDanHaoma.size();
				nSingleZhushu += nLength;
			}

			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == BuDingWei_HouSan || nKindID == BuDingWei_QianSan || nKindID == BuDingWei_QianEr || nKindID == BuDingWei_HouEr||nKindID == BuDingWei_ZhongSan)
		{
			int nLength = haoma.size()/2+1;

			nZongZhu +=nLength;
		}
		else if(nKindID == DingWeiDan)
		{
			int nSingleZhushu = 0;
			for(int j = 0;j < vectDanSize;j++)
			{
				string strDanHaoma = vectDanHaoma[j].c_str();
				if (strDanHaoma.empty())
				{
					continue;
				}
				int nLength = strDanHaoma.size();
				nSingleZhushu += nLength;
			}

			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == QianSanHunhe || nKindID == HouSanHunhe || nKindID == ZhongSanHunhe)
		{
			nZongZhu+=1;
		}
		else if(nKindID == QW_YiFanFengShun || nKindID == QW_HaoShiChengShuang || nKindID == QW_SanXingBaoXi || nKindID == QW_SiJiFaCai)
		{
			int nLength = haoma.size()/2+1;
			nZongZhu+=nLength;
		}
		else if(nKindID == RenXuan2 || nKindID == RenXuan3 || nKindID == RenXuan4)
		{
			int nSingleZhushu = 1;

			for(int j = 0;j < vectDanSize;j++)
			{
				string strDanHaoma = vectDanHaoma[j].c_str();
				if (strDanHaoma.empty())
				{
					continue;
				}
				int nLength = strDanHaoma.size();
				nSingleZhushu *= nLength;
			}

			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == NiuNiu_Num)
		{
			int nSingleZhushu = 0;

			for(int j = 0;j < vectDanSize;j++)
			{
				string strDanHaoma = vectDanHaoma[j].c_str();
				if (strDanHaoma.empty())
				{
					continue;
				}
				int nLength = strDanHaoma.size();
				nSingleZhushu ++;
			}

			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == Ren3Zu6_Fushi||nKindID == Ren3Hunhe_Zuxuan)
		{
			int nSingleZhushu = 0;
			int nWeiCount = 0;
			string strWei = vectDanHaoma[1];
			for(int j = 0;j < strWei.size();j++)
			{
				if(strWei[j] == '1')
				{
					nWeiCount++;
				}

			}
			if(nWeiCount < 3)
			{
				return 0;
			}

			int nLength = vectDanHaoma[0].size();
			if(nWeiCount == 3)
			{
				nSingleZhushu = nLength* (nLength - 1) * (nLength - 2) / 6;
			}
			else if(nWeiCount == 4)
			{
				nSingleZhushu = nWeiCount*nLength * (nLength - 1) * (nLength - 2) / 6;
			}
			else if(nWeiCount == 5)
			{
				nSingleZhushu = nWeiCount*nLength * (nLength - 1)*2 * (nLength - 2) / 6;
			}
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == Ren3Zu3_Fushi)
		{
			int nSingleZhushu = 0;
			int nWeiCount = 0;
			string strWei = vectDanHaoma[1];
			for(int j = 0;j < strWei.size();j++)
			{
				if(strWei[j] == '1')
				{
					nWeiCount++;
				}

			}
			if(nWeiCount < 3)
			{
				return 0;
			}

			int nLength = vectDanHaoma[0].size();

			string strDanHaoma = vectDanHaoma[0];
			if(nLength == 3&&(strDanHaoma[0] == strDanHaoma[1])||(strDanHaoma[1] == strDanHaoma[2])||(strDanHaoma[0] == strDanHaoma[2]))
			{
				if(nWeiCount == 3)
				{
					nSingleZhushu = 1;
				}
				else if(nWeiCount == 4)
				{
					nSingleZhushu = 4;
				}
				else if(nWeiCount == 5)
				{
					nSingleZhushu = 10;
				}

			}
			else
			{
				if(nWeiCount == 3)
				{
					nSingleZhushu = nLength* (nLength - 1);
				}
				else if(nWeiCount == 4)
				{
					nSingleZhushu = nWeiCount*nLength * (nLength - 1);
				}
				else if(nWeiCount == 5)
				{
					nSingleZhushu = nWeiCount*nLength * (nLength - 1)*2;
				}
			}
			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == Ren2Zuxuan_Fushi)
		{
			int nSingleZhushu = 0;
			int nWeiCount = 0;
			string strWei = vectDanHaoma[1];
			for(int j = 0;j < strWei.size();j++)
			{
				if(strWei[j] == '1')
				{
					nWeiCount++;
				}

			}
			if(nWeiCount < 2)
			{
				return 0;
			}

			int nLength = vectDanHaoma[0].size();
			if(nWeiCount == 2)
			{
				nSingleZhushu = nLength * (nLength - 1) / 2;
			}
			else if(nWeiCount == 3)
			{
				nSingleZhushu = nWeiCount*nLength * (nLength - 1) / 2;
			}
			else if(nWeiCount == 4)
			{
				nSingleZhushu = (nWeiCount-1)*(nWeiCount-2)*nLength * (nLength - 1) / 2;
			}
			else if(nWeiCount == 5)
			{
				nSingleZhushu = nWeiCount*nLength * (nLength - 1) ;
			}
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == Zuxuan120)
		{
			int nSingleZhushu = 0;
			int nCount = vectDanSize;
			nSingleZhushu = (nCount-4)*(nCount-3)*(nCount-2)*(nCount-1)*nCount/120;
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == Zuxuan60)
		{
			string::iterator new_end = std::remove_if(haoma.begin(), haoma.end(), bind2nd(equal_to <char>(), ',')); 

			haoma.erase(new_end, haoma.end()); 

			vector<string> vectZuDanHaoma;
			SplitString(haoma,vectZuDanHaoma,";");
			size_t vectZuDanSize = vectZuDanHaoma.size();

			int nSingleZhushu = 0;
			string strFrist = vectZuDanHaoma[0];
			string strSecond = vectZuDanHaoma[1];
			int nChongfu = 0;
			for(int j = 0;j < strFrist.size();j++)
			{
				for (int k = 0;k < strSecond.size();k++)
				{
					if(strFrist[j] == strSecond[k])
					{
						nChongfu++;
						if(strSecond.size()-1<3)
							return 0;
					}
				}

				int nFenzi = 1;
				int nFenmu = 1;
				for(int m = (strSecond.size() - nChongfu);m >0;m-- )
				{
					nFenzi *= m;
				}
				for(int m = (strSecond.size() - nChongfu-3);m > 0;m--)
				{
					nFenmu *= m;
				}

				nFenmu*=6;
				nSingleZhushu += nFenzi/nFenmu;

				nChongfu=0;

			}
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == Zuxuan30)
		{
			string::iterator new_end = std::remove_if(haoma.begin(), haoma.end(), bind2nd(equal_to <char>(), ',')); 

			haoma.erase(new_end, haoma.end()); 

			vector<string> vectZuDanHaoma;
			SplitString(haoma,vectZuDanHaoma,";");
			size_t vectZuDanSize = vectZuDanHaoma.size();

			int nSingleZhushu = 0;
			string strFrist = vectZuDanHaoma[0];
			string strSecond = vectZuDanHaoma[1];
			int nChongfu = 0;

			for (int j = 0;j < strSecond.size();j++)
			{
				for (int k = 0;k < strFrist.size();k++)
				{
					if(strFrist[k] == strSecond[j])
					{
						nChongfu++;
						if((strFrist.size() == nChongfu)||(strSecond.size() == nChongfu))
						{
							return 0;
						}
					}
				}

				int nFenzi = 1;
				int nFenmu = 1;
				for(int m = (strFrist.size() - nChongfu);m >0;m-- )
				{
					nFenzi *= m;
				}
				for(int m = (strFrist.size() - nChongfu-2);m > 0;m--)
				{
					nFenmu *= m;
				}

				nFenmu*=2;
				nSingleZhushu += nFenzi/nFenmu;
				nChongfu=0;
			}

			nZongZhu+=nSingleZhushu;


		}
		else if(nKindID == Zuxuan20)
		{
			string::iterator new_end = std::remove_if(haoma.begin(), haoma.end(), bind2nd(equal_to <char>(), ',')); 

			haoma.erase(new_end, haoma.end()); 

			vector<string> vectZuDanHaoma;
			SplitString(haoma,vectZuDanHaoma,";");
			size_t vectZuDanSize = vectZuDanHaoma.size();

			int nSingleZhushu = 0;
			string strFrist = vectZuDanHaoma[0];
			string strSecond = vectZuDanHaoma[1];
			int nChongfu = 0;
			for(int j = 0;j < strFrist.size();j++)
			{
				for (int k = 0;k < strSecond.size();k++)
				{
					if(strFrist[j] == strSecond[k])
					{
						nChongfu++;
						if((strFrist == strSecond)&&( strFrist.size() == 2)&&( strSecond.size() == 2))
							return 0;
					}
				}

				int nFenzi = 1;
				int nFenmu = 1;
				nFenzi = (strSecond.size()-nChongfu)*(strSecond.size()-nChongfu-1);

				nFenmu*=2;
				nSingleZhushu += nFenzi/nFenmu;

				nChongfu=0;

			}
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == Zuxuan10||nKindID == Zuxuan5)
		{
			string::iterator new_end = std::remove_if(haoma.begin(), haoma.end(), bind2nd(equal_to <char>(), ',')); 

			haoma.erase(new_end, haoma.end()); 

			vector<string> vectZuDanHaoma;
			SplitString(haoma,vectZuDanHaoma,";");
			size_t vectZuDanSize = vectZuDanHaoma.size();

			int nSingleZhushu = 0;
			string strFrist = vectZuDanHaoma[0];
			string strSecond = vectZuDanHaoma[1];
			int nChongfu = 0;
			for(int j = 0;j < strFrist.size();j++)
			{
				for (int k = 0;k < strSecond.size();k++)
				{
					if(strFrist[j] == strSecond[k])
					{
						nChongfu++;
					}
				}

				nSingleZhushu+= (strSecond.size()-nChongfu);
				nChongfu = 0;

			}
			nZongZhu+=nSingleZhushu;
		}


	}



	return nZongZhu;
}
//获取投注数
int CDataBaseEngineSink::Get11X5Zhushu(int nTypeID,int nKindID,CString strHaoma)
{
	int nZongZhu = 0;
	int nIndex = 0;
	CString strTempHaoma;
	CString strLog;
	do 
	{
		if(!AfxExtractSubString(strTempHaoma, strHaoma, nIndex++, '|'))
			break;
		if(strTempHaoma.IsEmpty())
			break;

		if(nKindID == IIRenXuan2)
		{
			int nCount = strTempHaoma.GetLength()/3+1;
			nZongZhu += (nCount-1)*nCount/2;

		}
		else if(nKindID == IIRenXuan3)
		{
			int nCount = strTempHaoma.GetLength()/3+1;
			nZongZhu += (nCount-1)*(nCount-2)*nCount/6;
		}
		else if(nKindID == IIRenXuan4)
		{
			int nCount = strTempHaoma.GetLength()/3+1;
			nZongZhu += (nCount-1)*(nCount-2)*(nCount-3)*nCount/24;
		}
		else if(nKindID == IIRenXuan5)
		{
			int nCount = strTempHaoma.GetLength()/3+1;
			nZongZhu += (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*nCount/120;
		}
		else if(nKindID == IIRenXuan6)
		{
			int nCount = strTempHaoma.GetLength()/3+1;
			nZongZhu += (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*nCount/720;
		}
		else if(nKindID == IIRenXuan7)
		{
			int nCount = strTempHaoma.GetLength()/3+1;
			nZongZhu += (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*(nCount-6)*nCount/5040;
		}
		else if(nKindID == IIRenXuan8)
		{
			int nCount = strTempHaoma.GetLength()/3+1;
			nZongZhu += (nCount-1)*(nCount-2)*(nCount-3)*(nCount-4)*(nCount-5)*(nCount-6)*(nCount-7)*nCount/40320;
		}
		else if(nKindID == QianYi)
		{
			int nCount = strTempHaoma.GetLength()/3+1;
			nZongZhu += nCount;
		}
		else if(nKindID == QianEr_ZhiXuan)
		{
			CString strDanHao;
			int nWei = 0;
			int nSingleZhushu = 0;
			CString strCalHaoma[2];

			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				strCalHaoma[nWei-1] = strDanHao;
			}
			while(nWei<2);

			CString strWan,strQian;
			strWan = strCalHaoma[0];
			strQian = strCalHaoma[1];
			CString strTempWan,strTempQian;
			for(int i = 0;i < strWan.GetLength();i+=2)
			{
				strTempWan = strWan.GetAt(i);
				strTempWan += strWan.GetAt(i+1);
				for(int j = 0;j < strQian.GetLength();j+=2)
				{
					strTempQian = strQian.GetAt(j);
					strTempQian += strQian.GetAt(j+1);
					if (strTempQian != strTempWan)
					{
						nSingleZhushu++;
					}
				}
			}

			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == QianEr_ZuXuan)
		{
			int nCount = strTempHaoma.GetLength()/3+1;
			int nSingleZhu = 0;
			nSingleZhu = (nCount-1)*nCount/2;
			nZongZhu+=nSingleZhu;
		}
		else if(nKindID == QianSan_ZhiXuan)
		{
			CString strDanHao;
			int nWei = 0;
			int nSingleZhushu = 0;
			CString strCalHaoma[3];
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;

				strCalHaoma[nWei-1] = strDanHao;
// 				int nLength = strDanHao.GetLength()/2;
// 				nSingleZhushu *= nLength;
			}
			while(nWei<3);
			CString strWan,strQian,strBai;
			strWan = strCalHaoma[0];
			strQian = strCalHaoma[1];
			strBai = strCalHaoma[2];
			CString strTempWan,strTempQian,strTempBai;
			for(int i = 0;i < strWan.GetLength();i+=2)
			{
				strTempWan = strWan.GetAt(i);
				strTempWan += strWan.GetAt(i+1);
				for(int j = 0;j < strQian.GetLength();j+=2)
				{
					strTempQian = strQian.GetAt(j);
					strTempQian += strQian.GetAt(j+1);
					for(int k = 0;k < strBai.GetLength();k+=2)
					{
						strTempBai = strBai.GetAt(k);
						strTempBai += strBai.GetAt(k+1);
						if (strTempQian != strTempWan && strTempBai != strTempQian && strTempBai != strTempWan)
						{
							nSingleZhushu++;
						}
					}
				}
			}
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == QianSan_ZuXuan)
		{
			int nCount = strTempHaoma.GetLength()/3+1;
			int nSingleZhu = 0;
			nSingleZhu = (nCount-2)*(nCount-1)*nCount/6;
			nZongZhu+=nSingleZhu;
		}

	}
	while(nIndex<80000);
	return nZongZhu;
}
//3D、排列三
//获取投注数
int CDataBaseEngineSink::Get3DZhushu(int nTypeID,int nKindID,CString strHaoma)
{
	int nZongZhu = 0;
	int nIndex = 0;
	CString strTempHaoma;
	CString strLog;
	do 
	{
		if(!AfxExtractSubString(strTempHaoma, strHaoma, nIndex++, '|'))
			break;
		if(strTempHaoma.IsEmpty())
			break;

		if(nKindID == enWF_ZhixuanFushi)
		{
			CString strDanHao;
			int nWei = 0;
			int nSingleZhushu = 1;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				int nLength = strDanHao.GetLength();
				nSingleZhushu *= nLength;
			}
			while(nWei<3);
			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == enWF_DingWeiDan||nKindID == enWF_DaxiaoDanshuang)
		{
			CString strDanHao;
			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				int nLength = strDanHao.GetLength();
				nSingleZhushu += nLength;
			}
			while(nWei<3);
			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == enWF_ZhixuanDanshi)
		{
			nZongZhu += 1;
		}
		else if(nKindID == enWF_QiansanzusanFushi)
		{
			int nCount = strTempHaoma.GetLength();
			nZongZhu += (nCount-1)*nCount;
		}
		else if(nKindID == enWF_QiansanzusanDanshi)
		{
			nZongZhu += 1;
		}
		else if(nKindID == enWF_QiansanzuliuFushi)
		{
			int nCount = strTempHaoma.GetLength();
			nZongZhu += (nCount-1)*(nCount-2)*nCount/6;
		}
		else if(nKindID == enWF_QiansanzuliuDanshi)
		{
			nZongZhu += 1;
		}
		else if(nKindID == enWF_QianyizhixuanFushi)
		{
			int nCount = strTempHaoma.GetLength();
			nZongZhu += nCount;
		}
		else if(nKindID == enWF_QianerzhixuanFushi)
		{
			CString strDanHao;
			int nWei = 0;
			int nSingleZhushu = 1;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				int nLength = strDanHao.GetLength();
				nSingleZhushu *= nLength;
			}
			while(nWei<2);
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == enWF_QianerzhixuanDanshi)
		{
			nZongZhu += 1;
		}
		else if(nKindID == enWF_Budingwei)
		{
			int nCount = (strTempHaoma.GetLength()+1)/2;
			nZongZhu+=nCount;
		}
		else if(nKindID == enWF_HouyizhixuanFushi)
		{
			int nCount = strTempHaoma.GetLength();
			nZongZhu+=nCount;
		}
		else if(nKindID == enWF_HouerzhixuanFushi)
		{
			CString strDanHao;
			int nWei = 0;
			int nSingleZhushu = 1;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				int nLength = strDanHao.GetLength();
				nSingleZhushu *= nLength;
			}
			while(nWei<2);
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == enWF_HouerzhixuanDanshi)
		{
			nZongZhu += 1;
		}
		

	}
	while(nIndex<80000);
	return nZongZhu;
}
//获取幸运28注数
int CDataBaseEngineSink::GetXY28Zhushu(int nTypeID,int nKindID,CString strHaoma)
{
	int nZongZhu = 0;
	int nIndex = 0;
	CString strTempHaoma;

	do 
	{
		if(!AfxExtractSubString(strTempHaoma, strHaoma, nIndex++, '|'))
			break;
		if(strTempHaoma.IsEmpty())
			break;

		if(nKindID == enXY28_TeMa)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() == 2)
					nSingleZhushu += 1;	
			}
			while(nWei<90);

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enXY28_DXDS)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() <= 2)
					nSingleZhushu += 1;	
			}
			while(nWei<90);
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == enXY28_JiZhi||nKindID == enXY28_Funny)
		{
			if(strTempHaoma.GetLength()==2)
				nZongZhu += 1;	
		}
	}while(nIndex < 80000);


	return nZongZhu;
}
//获取快乐8注数
int CDataBaseEngineSink::GetKL8Zhushu(int nTypeID,int nKindID,CString strHaoma)
{
	int nZongZhu = 0;
	int nIndex = 0;
	CString strTempHaoma;

	do 
	{
		if(!AfxExtractSubString(strTempHaoma, strHaoma, nIndex++, '|'))
			break;
		if(strTempHaoma.IsEmpty())
			break;
		if(nKindID == enBJK8_RX_1)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() == 2)
					nSingleZhushu += 1;	
			}
			while(nWei<90);

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enBJK8_RX_2)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() != 2)
					return 0;
			}
			while(nWei<90);

			int nCount = strTempHaoma.GetLength();
			nCount+=1;
			if(nCount < 4)
				return 0;
			int count = nCount/3;
			nSingleZhushu = (count*(count-1))/2;

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enBJK8_RX_3)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() != 2)
					return 0;
			}
			while(nWei<90);

			int nCount = strTempHaoma.GetLength();
			nCount+=1;

			if(nCount < 9)
				return 0;
			int count = nCount/3;
			nSingleZhushu = (count*(count-1)*(count-2))/6;

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enBJK8_RX_4)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() != 2)
					return 0;
			}
			while(nWei<90);

			int nCount = strTempHaoma.GetLength();
			nCount+=1;

			if(nCount < 12)
				return 0;
			int count = nCount/3;
			nSingleZhushu = (count*(count-1)*(count-2)*(count-3))/24;

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enBJK8_RX_5)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() != 2)
					return 0;
			}
			while(nWei<90);

			int nCount = strTempHaoma.GetLength();
			nCount+=1;

			if(nCount < 15)
				return 0;
			int count = nCount/3;
			nSingleZhushu = (count*(count-1)*(count-2)*(count-3)*(count-4))/120;

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enBJK8_RX_6)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() != 2)
					return 0;
			}
			while(nWei<90);

			int nCount = strTempHaoma.GetLength();
			nCount+=1;

			if(nCount < 18)
				return 0;
			int count = nCount/3;
			nSingleZhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5))/720;

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enBJK8_RX_7)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() != 2)
					return 0;
			}
			while(nWei<90);

			int nCount = strTempHaoma.GetLength();
			nCount+=1;

			if(nCount < 21)
				return 0;
			int count = nCount/3;
			nSingleZhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6))/5040;

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enBJK8_RX_8)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() != 2)
					return 0;
			}
			while(nWei<90);

			int nCount = strTempHaoma.GetLength();
			nCount+=1;

			if(nCount < 24)
				return 0;
			int count = nCount/3;
			nSingleZhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6)*(count-7))/40320;

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enBJK8_RX_9)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() != 2)
					return 0;
			}
			while(nWei<90);

			int nCount = strTempHaoma.GetLength();
			nCount+=1;

			if(nCount < 27)
				return 0;
			int count = nCount/3;
			nSingleZhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6)*(count-7)*(count-8))/362880;

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enBJK8_RX_10)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() != 2)
					return 0;
			}
			while(nWei<90);

			int nCount = strTempHaoma.GetLength();
			nCount+=1;

			if(nCount < 30)
				return 0;
			int count = nCount/3;
			nSingleZhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6)*(count-7)*(count-8)*(count-9))/3628800;

			nZongZhu += nSingleZhushu;

		}
		else if(nKindID == enBJK8_ShangXia || nKindID == enBJK8_JiOu||nKindID == enBJK8_HeZhiDXDS)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() == 1)
					nSingleZhushu++;
			}
			while(nWei<4);

			nZongZhu += nSingleZhushu;
	
		}


	}while(nIndex < 80000);

	return nZongZhu;

}
int CDataBaseEngineSink::GetPK10Zhushu(int nTypeID,int nKindID,CString strHaoma)
{
	int nZongZhu = 0;
	int nIndex = 0;
	CString strTempHaoma;

	do 
	{
		if(!AfxExtractSubString(strTempHaoma, strHaoma, nIndex++, '|'))
			break;
		if(strTempHaoma.IsEmpty())
			break;

		if(nKindID == WF_GuanJun || nKindID == WF_YaJun || nKindID == WF_DiSan || nKindID == WF_DiSi || nKindID == WF_DiWu || nKindID == WF_DiLiu || nKindID == WF_DiQi || nKindID == WF_DiBa || nKindID == WF_DiJiu || nKindID == WF_DiShi   )
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() == 2)
					nSingleZhushu += 1;	
			}
			while(nWei<11);

			nZongZhu += nSingleZhushu;
		}
		else if(nKindID == WF_GuanYaJun)
		{
			CString str[5];
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;

				str[nWei-1] = strDanHao;
			}
			while(nWei<2);

			int nWanCount = str[0].GetLength();
			int nQianCount = str[1].GetLength();

			CString strTemp;
			for(BYTE i = 0;i < nWanCount;i+=2)
			{
				strTemp = str[0].Mid(i,2);
				for(BYTE j = 0;j < nQianCount; j+=2)
				{
					CString strQian = str[1].Mid(j,2);
					if(strTemp == strQian)
						continue;

					nSingleZhushu += 1;
				}
			}

			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == WF_QianSan)
		{
			CString str[5];
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;

				str[nWei-1] = strDanHao;
			}
			while(nWei<3);

			int nWanCount = str[0].GetLength();
			int nQianCount = str[1].GetLength();
			int nBaiCount = str[2].GetLength();

			CString strTemp;
			for(BYTE i = 0;i < nWanCount;i+=2)
			{
				strTemp = str[0].Mid(i,2);
				for(BYTE j = 0;j < nQianCount; j+=2)
				{
					CString strQian = str[1].Mid(j,2);
					if(strTemp == strQian)
						continue;

					for(BYTE k = 0;k < nBaiCount;k+=2)
					{
						CString strBai = str[2].Mid(k,2);
						if(strBai == strQian || strBai == strTemp)
							continue;
						nSingleZhushu += 1;
					}
				}
			}

			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == WF_QianSi)
		{
			CString str[5];
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;

				str[nWei-1] = strDanHao;
			}
			while(nWei<4);

			int nWanCount = str[0].GetLength();
			int nQianCount = str[1].GetLength();
			int nBaiCount = str[2].GetLength();
			int nShiCount = str[3].GetLength();

			CString strTemp;
			for(BYTE i = 0;i < nWanCount;i+=2)
			{
				strTemp = str[0].Mid(i,2);
				for(BYTE j = 0;j < nQianCount; j+=2)
				{
					CString strQian = str[1].Mid(j,2);
					if(strTemp == strQian)
						continue;

					for(BYTE k = 0;k < nBaiCount;k+=2)
					{
						CString strBai = str[2].Mid(k,2);
						if(strBai == strQian || strBai == strTemp)
							continue;
						for(BYTE l = 0;l < nShiCount;l+=2)
						{
							CString strShi =str[3].Mid(l,2);
							if(strBai == strQian || strBai == strTemp|| strShi == strBai || strShi == strTemp || strShi == strQian )
								continue;
							nSingleZhushu += 1;
						}
					}
				}
			}

			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == WF_QianWu)
		{
			CString str[5];
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;

				str[nWei-1] = strDanHao;
			}
			while(nWei<5);

			int nWanCount = str[0].GetLength();
			int nQianCount = str[1].GetLength();
			int nBaiCount = str[2].GetLength();
			int nShiCount = str[3].GetLength();
			int nGeCount = str[4].GetLength();

			CString strTemp;
			for(BYTE i = 0;i < nWanCount;i+=2)
			{
				strTemp = str[0].Mid(i,2);
				for(BYTE j = 0;j < nQianCount; j+=2)
				{
					CString strQian = str[1].Mid(j,2);
					if(strTemp == strQian)
						continue;

					for(BYTE k = 0;k < nBaiCount;k+=2)
					{
						CString strBai = str[2].Mid(k,2);
						if(strBai == strQian || strBai == strTemp)
							continue;
						for(BYTE l = 0;l < nShiCount;l+=2)
						{
							CString strShi =str[3].Mid(l,2);
							if(strShi == strQian || strShi == strTemp|| strShi == strBai  )
								continue;
							for(BYTE m = 0;m < nGeCount;m+=2)
							{
								CString strGe =str[4].Mid(m,2);
								if(strBai == strGe || strGe == strTemp|| strShi == strGe || strGe == strQian )
									continue;
								nSingleZhushu += 1;
							}
						//	nSingleZhushu += 1;
						}
					}
				}
			}

			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == WF_QianLiu||nKindID == WF_QianQi||nKindID == WF_QianBa||nKindID == WF_QianJiu||nKindID == WF_QianShi)
		{
			int nMost = 0;
			if(nKindID == WF_QianWu)
				nMost = 5;
			else if(nKindID == WF_QianLiu)
				nMost = 6;
			else if(nKindID == WF_QianQi)
				nMost = 7;
			else if(nKindID == WF_QianBa)
				nMost = 8;
			else if(nKindID == WF_QianJiu)
				nMost = 9;
			else if(nKindID == WF_QianShi)
				nMost = 10;
			CString strDanHao;

			int nSuit = 0;
			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					return 0;

				if(strDanHao.GetLength()>2)
					return 0;
				nSuit++;
			}
			while(nWei<nMost);
			if(nSuit == nMost)
				nZongZhu += 1;
		}
		else if(WF_DXDS == nKindID )
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				if(strDanHao.GetLength() == 1)
					nSingleZhushu += 1;	
			}
			while(nWei<11);

			nZongZhu += nSingleZhushu;
		}
		else if(nKindID == WF_DWD)
		{
			CString strDanHao;

			int nWei = 0;
			int nSingleZhushu = 0;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				int nCount = strDanHao.GetLength();
					nSingleZhushu += nCount/2;	
			}
			while(nWei<11);

			nZongZhu += nSingleZhushu;

		}
	} while (nIndex<80000);

	return nZongZhu;
}
//七星彩
//获取投注数
int CDataBaseEngineSink::GetQXCZhushu(int nTypeID,int nKindID,CString strHaoma)
{
	int nZongZhu = 0;
	int nIndex = 0;
	CString strTempHaoma;
	CString strLog;
	do 
	{
		if(!AfxExtractSubString(strTempHaoma, strHaoma, nIndex++, '|'))
			break;
		if(strTempHaoma.IsEmpty())
			break;

		if(nKindID == QiXingCai_LiangDing)
		{
			CString strDanHao;
			int nWei = 0;
			int nSingleZhushu = 1;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				int nLength = strDanHao.GetLength();
				nSingleZhushu *= nLength;
			}
			while(nWei<4);
			nZongZhu+=nSingleZhushu;

		}
		else if(nKindID == QiXingCai_SanDing)
		{
			CString strDanHao;
			int nWei = 0;
			int nSingleZhushu = 1;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				int nLength = strDanHao.GetLength();
				nSingleZhushu *= nLength;
			}
			while(nWei<4);
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == QiXingCai_SiDing)
		{
			CString strDanHao;
			int nWei = 0;
			int nSingleZhushu = 1;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				int nLength = strDanHao.GetLength();
				nSingleZhushu *= nLength;
			}
			while(nWei<4);
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == QiXingCai_YiDing)
		{
			CString strDanHao;
			int nWei = 0;
			int nSingleZhushu = 1;
			do 
			{
				if(!AfxExtractSubString(strDanHao,strTempHaoma,  nWei++, ','))
					break;

				if(strDanHao.IsEmpty())
					continue;
				int nLength = strDanHao.GetLength();
				nSingleZhushu *= nLength;
			}
			while(nWei<4);
			nZongZhu+=nSingleZhushu;
		}
		else if(nKindID == QiXingCai_LiangZiXian)
		{
			nZongZhu += 1;
		}
		else if(nKindID == QiXingCai_SanZiXian)
		{
			nZongZhu += 1;
		}

	}
	while(nIndex<80000);
	return nZongZhu;
}

CString CDataBaseEngineSink::AddDouHao(CString strHaoma)
{
	CString strTempHaoma;

	CString strDanHao;
	int nIndex = 0;
	do 
	{
		if(!AfxExtractSubString(strDanHao,strHaoma,  nIndex++, '|'))
			break;
		if(strDanHao.IsEmpty())
			break;
		int nLength = strDanHao.GetLength();
		for(int i = 0;i < nLength;i++)
		{
			strTempHaoma += strDanHao[i];
			if(i%2 == 1 && i !=nLength-1)
			{
				strTempHaoma+=_T(",");
			}
		}
		strTempHaoma += _T("|");

	} while (nIndex<80000);
	return strTempHaoma;
}
//投注追号
bool CDataBaseEngineSink::OnTouzhuCQSSCZhuihao(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_TouZhuCQSSC_Zhuihao));
		if (wDataSize!=sizeof(DBR_GP_TouZhuCQSSC_Zhuihao))
		{
			return false;
		}
		DBR_GP_TouZhuCQSSC_Zhuihao* pTouzhuCQSSC = (DBR_GP_TouZhuCQSSC_Zhuihao*)pData;
		static int nIndex = 0;
		int nZhushu = 0;
		if( pTouzhuCQSSC->nIndex == 0 || nTouZhuSign != pTouzhuCQSSC->nSign)
		{
			nIndex = 0;
			nZhushu = 0;
			nTouZhuSign = pTouzhuCQSSC->nSign;

			memset(szHaomaTemp, 0, sizeof(szHaomaTemp));
			memcpy(szHaomaTemp,	pTouzhuCQSSC->strHaoma,pTouzhuCQSSC->nHaoMaLen);
		}
		else
		{
			int nPos = sizeof(pTouzhuCQSSC->strHaoma)* pTouzhuCQSSC->nIndex;
			memcpy(szHaomaTemp+nPos,	pTouzhuCQSSC->strHaoma,pTouzhuCQSSC->nHaoMaLen);
		}


		nZhushu = pTouzhuCQSSC->nZhushu; 
 		nIndex++; 			
 		if(pTouzhuCQSSC->nEnd == 1)
 		{
 			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CONTINUE_RET,dwContextID,NULL,NULL);;
 		}
		//m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CONTINUE_RET,dwContextID,NULL,NULL);
		memset(szHaoma, 0, sizeof(szHaoma));

		int nLeng =(sizeof(pTouzhuCQSSC->strHaoma)* pTouzhuCQSSC->nIndex+pTouzhuCQSSC->nHaoMaLen);

		int nRet =  CWHService::LZUnCompressData((byte*)szHaomaTemp, nLeng, (byte*)szHaoma, sizeof(szHaoma));

		if(nZhushu <= 0)
		{
			return false;
		}

		CString strAllHaoma = ChangeStringToT(szHaoma);
		CString strQishu ;
		strQishu = ChangeStringToT(pTouzhuCQSSC->strQishu[0]);

// 		if(nLeng<1024)
// 		{
			if(pTouzhuCQSSC->nGameKind == ZhongSanZhiXuanHeZhi || pTouzhuCQSSC->nGameKind == QianSanZhiXuanHeZhi || pTouzhuCQSSC->nGameKind == HouSanZhiXuanHeZhi)
			{
				CString strDanhao;
				AfxExtractSubString(strDanhao,strAllHaoma,0,',');

				if(strDanhao == strAllHaoma)
				{
					strAllHaoma = AddDouHao(strAllHaoma);
				}

			}
// 
			int nCheckZongzhu = GetTouZhuZongShu(pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma);

			CString strLog;
			strLog.Format(L"TOUZHU [%d] TypeID:%d,KindID:%d,haoma:[%s],zhushu:%d,checkzhushu:%d",pTouzhuCQSSC->dwUserID,pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma,nZhushu,nCheckZongzhu);
			CTraceService::TraceString(strLog,TraceLevel_Normal);

			if ((pTouzhuCQSSC->nGameType == CZ_PK10&&pTouzhuCQSSC->nGameKind == WF_GYHZH )||((pTouzhuCQSSC->nGameType == CZ3D||pTouzhuCQSSC->nGameType == CZPaiLie3)&&pTouzhuCQSSC->nGameKind == enWF_ZhixuanHezhi))
			{
				nCheckZongzhu = nZhushu;
			}
			if(nCheckZongzhu != nZhushu)
			{
				CString strLogFile;
				strLogFile.Format(L"WRONGTOUZHU [%d] TypeID:%d,KindID:%d,haoma:[%s],zhushu:%d,checkzhushu:%d",pTouzhuCQSSC->dwUserID,pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma,nZhushu,nCheckZongzhu);
				LogFile::instance().LogText(strLogFile);
				DBO_GR_TouzhuResult  TouzhuRes;
				ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
				TouzhuRes.lResultCode = 11;
				TouzhuRes.nSign = pTouzhuCQSSC->nSign;
				TouzhuRes.nUserID = pTouzhuCQSSC->dwUserID;


				ZeroMemory(&szHaoma,sizeof(szHaoma));
				nZhushu = 0;
				//发送结果
				return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CQSSC_RET,dwContextID,&TouzhuRes,sizeof(TouzhuRes));


				return false;
			}
// 
// // 			CString strQishu ;
// // 			//for (int i = 0;i < 50;i++)
// // 			{
// // 				strQishu = ChangeStringToT(pTouzhuCQSSC->strQishu[0]);
// 
// 
// 				//正式语句
// 				if(!CheckTouzhuNum(strAllHaoma,nLeng,pTouzhuCQSSC->dwUserID,strQishu))
// 				{
// 					return false;
// 				}
// 
// 
// 		//	}
// 
// 		}
			if(nZhushu <= 0)
			{
				DBO_GR_TouzhuResult  TouzhuRes;
				ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
				TouzhuRes.lResultCode = 11;
				TouzhuRes.nSign = pTouzhuCQSSC->nSign;
				TouzhuRes.nUserID = pTouzhuCQSSC->dwUserID;


				ZeroMemory(&szHaoma,sizeof(szHaoma));
				nZhushu = 0;
				//发送结果
				return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CQSSC_RET,dwContextID,&TouzhuRes,sizeof(TouzhuRes));
			}

		LONG lResultCode=0;
		for(int i = 0;i < 50;i++)
		{
			strQishu = ChangeStringToT(pTouzhuCQSSC->strQishu[i]);

			if(strQishu.GetLength() == 11 && pTouzhuCQSSC->nGameType == CZXinJiangSSC)
			{
				CString strRight;
				strRight = strQishu.Right(2);

				strQishu = (strQishu.Left(8)+strRight);
			}
			CTraceService::TraceString(strQishu,TraceLevel_Exception);

			if(strQishu.IsEmpty() || strQishu.GetLength() == 0)
				break;

 			m_GameSSCDBAide.ResetParameter();
 			m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pTouzhuCQSSC->dwUserID);
 			m_GameSSCDBAide.AddParameter(TEXT("@qishu"),strQishu);
 			m_GameSSCDBAide.AddParameter(TEXT("@type"),pTouzhuCQSSC->nGameType);
 			m_GameSSCDBAide.AddParameter(TEXT("@kind"),pTouzhuCQSSC->nGameKind);
 			m_GameSSCDBAide.AddParameter(TEXT("@haoma"), strAllHaoma);
 			m_GameSSCDBAide.AddParameter(TEXT("@zhushu"), nZhushu);
 			m_GameSSCDBAide.AddParameter(TEXT("@beishu"),pTouzhuCQSSC->nBeitou[i]);
			m_GameSSCDBAide.AddParameter(TEXT("@moshi"),pTouzhuCQSSC->nMoshi);
			m_GameSSCDBAide.AddParameter(TEXT("@isZhuihao"),pTouzhuCQSSC->bZhuihao);
			m_GameSSCDBAide.AddParameter(TEXT("@platform"),1);

			lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_touzhu"),false);
			if(lResultCode!=0)
			{
				break;
			}
		}
		if(lResultCode != 0)
		{
			USES_CONVERSION;
			CString strLog;
			strLog.Format(L"期号：%s, 投注返回错误：%d", strQishu, lResultCode);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
		}
		DBO_GR_TouzhuResult  TouzhuRes;
		ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
		TouzhuRes.lResultCode = lResultCode;
		TouzhuRes.nSign = pTouzhuCQSSC->nSign;
		TouzhuRes.nUserID = pTouzhuCQSSC->dwUserID;

		ZeroMemory(&szHaoma,sizeof(szHaoma));
		nZhushu = 0;
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CQSSC_RET,dwContextID,&TouzhuRes,sizeof(TouzhuRes));

	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception OnTouzhuCQSSCZhuihao p_touzhu");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();

		return false;
	}
	return true;
}
//投注追号
bool CDataBaseEngineSink::OnMBTouzhuCQSSCZhuihao(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_TouZhuCQSSC_Zhuihao));
		if (wDataSize!=sizeof(DBR_GP_TouZhuCQSSC_Zhuihao))
		{
			CString strLog;
			strLog.Format(L"MOBILE  wDataSize!=sizeof(DBR_GP_TouZhuCQSSC_Zhuihao)");
			CTraceService::TraceString(strLog,TraceLevel_Exception);
			return false;
		}
		DBR_GP_TouZhuCQSSC_Zhuihao* pTouzhuCQSSC = (DBR_GP_TouZhuCQSSC_Zhuihao*)pData;
// 		static int nIndex = 0;
// 
// 		if( pTouzhuCQSSC->nIndex == 0 || nTouZhuSign != pTouzhuCQSSC->nSign)
// 		{
// 			nIndex = 0;
// 			nZongZhu = 0;
// 			nTouZhuSign = pTouzhuCQSSC->nSign;
// 
// 			memset(szHaomaTemp, 0, sizeof(szHaomaTemp));
// 			memcpy(szHaomaTemp,	pTouzhuCQSSC->strHaoma,pTouzhuCQSSC->nHaoMaLen);
// 		}
// 		else
// 		{
// 			int nPos = sizeof(pTouzhuCQSSC->strHaoma)* pTouzhuCQSSC->nIndex;
// 			memcpy(szHaomaTemp+nPos,	pTouzhuCQSSC->strHaoma,pTouzhuCQSSC->nHaoMaLen);
// 		}
// 
// 		nZongZhu = pTouzhuCQSSC->nZhushu; 
//  		nIndex++; 			
//  		if(pTouzhuCQSSC->nEnd == 1)
//  		{
//  			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CONTINUE_RET,dwContextID,NULL,NULL);;
//  		}
// 		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CONTINUE_RET,dwContextID,NULL,NULL);
// 		memset(szHaoma, 0, sizeof(szHaoma));
// 
// 		int nLeng =(sizeof(pTouzhuCQSSC->strHaoma)* pTouzhuCQSSC->nIndex+pTouzhuCQSSC->nHaoMaLen);
// 
// 	//	int nRet =  CWHService::LZUnCompressData((byte*)szHaomaTemp, nLeng, (byte*)szHaoma, sizeof(szHaoma));
// 
// 		strcpy(szHaoma,szHaomaTemp);
// 
		if(pTouzhuCQSSC->nZhushu <= 0)
		{
			CString strLog;
			strLog.Format(L"MOBILE pTouzhuCQSSC->nZhushu <= 0");
			CTraceService::TraceString(strLog,TraceLevel_Exception);

			return false;
		}

		CString strAllHaoma = ChangeStringToT(pTouzhuCQSSC->strHaoma);
//  		CString strLog;
//  		strLog.Format(L"strAllHaoma:%s",strAllHaoma);
//  		CTraceService::TraceString(strLog,TraceLevel_Exception);
// 
		int nZhushu = 0;
		nZhushu = pTouzhuCQSSC->nZhushu;
 		int nCheckZongzhu = GetTouZhuZongShu(pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma);
		CString strLog;
		strLog.Format(L"TOUZHU [%d] TypeID:%d,KindID:%d,haoma:[%s],zhushu:%d,checkzhushu:%d",pTouzhuCQSSC->dwUserID,pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma,nZhushu,nCheckZongzhu);
		CTraceService::TraceString(strLog,TraceLevel_Normal);
		if ((pTouzhuCQSSC->nGameType == CZ_PK10&&pTouzhuCQSSC->nGameKind == WF_GYHZH )||((pTouzhuCQSSC->nGameType == CZ3D||pTouzhuCQSSC->nGameType == CZPaiLie3)&&pTouzhuCQSSC->nGameKind == enWF_ZhixuanHezhi))
		{
			nCheckZongzhu = nZhushu;
		}

 		if(nCheckZongzhu != nZhushu)
 		{

			CString strLog;
			strLog.Format(L"MOBILE nCheckZongzhu != nZhushu");
			CTraceService::TraceString(strLog,TraceLevel_Exception);

			TCHAR szClientAddr[16]=TEXT("");
			BYTE * pClientAddr=(BYTE *)&pTouzhuCQSSC->dwClientAddr;
			_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

			CString strLogFile;
			strLogFile.Format(L"WRONGTOUZHU [%d] TypeID:%d,KindID:%d,haoma:[%s],zhushu:%d,checkzhushu:%d,ip:%s",pTouzhuCQSSC->dwUserID,pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma,nZhushu,nCheckZongzhu,szClientAddr);
			LogFile::instance().LogText(strLogFile);
			DBO_GR_TouzhuResult  TouzhuRes;
			ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
			TouzhuRes.lResultCode = 11;
			TouzhuRes.nSign = pTouzhuCQSSC->nSign;
			TouzhuRes.nUserID = pTouzhuCQSSC->dwUserID;


			ZeroMemory(&szHaoma,sizeof(szHaoma));
			nZhushu = 0;
			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CQSSC_RET,dwContextID,&TouzhuRes,sizeof(TouzhuRes));

 			return false;
 		}

		LONG lResultCode=0;
		CString strQishu ;
		//for (int i = 0;i < 50;i++)
		{
			strQishu = ChangeStringToT(pTouzhuCQSSC->strQishu[0]);


			//正式语句
			if(!CheckTouzhuNum(strAllHaoma,0,pTouzhuCQSSC->dwUserID,strQishu))
			{
				CString strLog;
				strLog.Format(L"MOBILE !CheckTouzhuNum");
				CTraceService::TraceString(strLog,TraceLevel_Exception);

				return false;
			}


		}
		for(int i = 0;i < 50;i++)
		{
			if(pTouzhuCQSSC->nBeitou[i]>999)
			{
				DBO_GR_TouzhuResult  TouzhuRes;
				ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
				TouzhuRes.lResultCode = 9;
				TouzhuRes.nSign = pTouzhuCQSSC->nSign;
				TouzhuRes.nUserID = pTouzhuCQSSC->dwUserID;

				ZeroMemory(&szHaoma,sizeof(szHaoma));
				nZhushu = 0;
				//发送结果
				return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CQSSC_RET,dwContextID,&TouzhuRes,sizeof(TouzhuRes));
			}

		}

		if(nZhushu<= 0)
		{
			return false;
		}
		for(int i = 0;i < 50;i++)
		{
			strQishu = ChangeStringToT(pTouzhuCQSSC->strQishu[i]);

			if(strQishu.IsEmpty() || strQishu.GetLength() == 0)
				break;


 			m_GameSSCDBAide.ResetParameter();
 			m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pTouzhuCQSSC->dwUserID);
 			m_GameSSCDBAide.AddParameter(TEXT("@qishu"),strQishu);
 			m_GameSSCDBAide.AddParameter(TEXT("@type"),pTouzhuCQSSC->nGameType);
 			m_GameSSCDBAide.AddParameter(TEXT("@kind"),pTouzhuCQSSC->nGameKind);
 			m_GameSSCDBAide.AddParameter(TEXT("@haoma"), strAllHaoma);
 			m_GameSSCDBAide.AddParameter(TEXT("@zhushu"), nZhushu);
 			m_GameSSCDBAide.AddParameter(TEXT("@beishu"),pTouzhuCQSSC->nBeitou[i]);
			m_GameSSCDBAide.AddParameter(TEXT("@moshi"),pTouzhuCQSSC->nMoshi);
			m_GameSSCDBAide.AddParameter(TEXT("@isZhuihao"),pTouzhuCQSSC->bZhuihao);

			lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_touzhu"),false);
			if(lResultCode != 0)
				break;
		}
		if(lResultCode != 0)
		{
			USES_CONVERSION;
			CString strLog;
			strLog.Format(L"期号：%s, 投注返回错误：%d", strQishu, lResultCode);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
		}
		DBO_GR_TouzhuResult  TouzhuRes;
		ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
		TouzhuRes.lResultCode = lResultCode;
		TouzhuRes.nSign = pTouzhuCQSSC->nSign;
		TouzhuRes.nUserID = pTouzhuCQSSC->dwUserID;

		ZeroMemory(&szHaoma,sizeof(szHaoma));
		nZhushu = 0;
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CQSSC_RET,dwContextID,&TouzhuRes,sizeof(TouzhuRes));

	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception OnTouzhuCQSSCZhuihao p_touzhu");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();

		return false;
	}
	return true;
}

CString	CDataBaseEngineSink::ChangeStringToT(CStringA strInput)
{
	CString  strOutput(strInput);
// 	USES_CONVERSION;
// 	strOutput = A2T(strInput);

	return strOutput;
}
bool CDataBaseEngineSink::CheckTouzhuNum(CString strHaoma,int nLength,int nUserID,CString strQihao)
{
	nLength = strHaoma.GetLength();
	for (int i = 0;i < nLength;i++)
	{
		CString str;
		str=strHaoma.GetAt(i);
		
		if(!iswdigit(strHaoma.GetAt(i))&&(str.Compare(_T(","))!=0)&&(str!=_T(";"))&&(str!=_T("|"))&&(str!=_T("\0")))
		{
			TCHAR szConfigFileName[124]=L"";
			//设置文件名
			TCHAR szPath[MAX_PATH]=TEXT("");
			GetCurrentDirectory(sizeof(szPath),szPath);
			_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\ErrHaoma.ini"),szPath);

			CTime t = CTime::GetCurrentTime();
			TCHAR szKey[33]=L"";
			TCHAR szErrCode[124]=L"";
			_sntprintf(szKey,sizeof(szKey),TEXT("%s"),t.Format(L"%Y-%m-%d"));
			int nCount = GetPrivateProfileInt(szKey,L"ErrCount",0,szConfigFileName);
			_sntprintf(szErrCode,sizeof(szErrCode),TEXT("%d"),nUserID);
			TCHAR szUserID[33]=L"";
			TCHAR szHaoma[33]=L"";
			TCHAR szQihao[33]=L"";
			TCHAR szCount[33]=L"";
			_sntprintf(szUserID,sizeof(szUserID),TEXT("UserID%d"),nCount);
			_sntprintf(szHaoma,sizeof(szHaoma),TEXT("ErrHaoma%d"),nCount);
			_sntprintf(szQihao,sizeof(szQihao),TEXT("Qihao%d"),nCount);

			nCount++;
			_sntprintf(szCount,sizeof(szCount),TEXT("%d"),nCount);
			WritePrivateProfileString(szKey,L"ErrCount",szCount,szConfigFileName);

			WritePrivateProfileString(szKey,szUserID,szErrCode,szConfigFileName);
			_sntprintf(szErrCode,sizeof(szErrCode),TEXT("包含%s字符，%s"),str,strHaoma);
			WritePrivateProfileString(szKey,szHaoma,szErrCode,szConfigFileName);
			WritePrivateProfileString(szKey,szQihao,strQihao,szConfigFileName);

			return false;
		}
			
	}
	return true;
}
//投注
bool CDataBaseEngineSink::OnTouzhuCQSSCDan(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_TouZhuCQSSC_Dan));
		if (wDataSize!=sizeof(DBR_GP_TouZhuCQSSC_Dan))
		{
			return false;
		}
		CString strLog;
		strLog.Format(L"CQSSCTZH touzhu3");
		OutputDebugString(strLog);

		DBR_GP_TouZhuCQSSC_Dan* pTouzhuCQSSC = (DBR_GP_TouZhuCQSSC_Dan*)pData;

		if( pTouzhuCQSSC->nIndex == 0 || nTouZhuSign != pTouzhuCQSSC->nSign)
		{
		//	nZhushu = 0;
			nTouZhuSign = pTouzhuCQSSC->nSign;

			memset(szHaomaTemp, 0, sizeof(szHaomaTemp));
			memcpy(szHaomaTemp,	pTouzhuCQSSC->strHaoma,pTouzhuCQSSC->nHaoMaLen);
		}
		else
		{
			int nPos = sizeof(pTouzhuCQSSC->strHaoma)* pTouzhuCQSSC->nIndex;
			memcpy(szHaomaTemp+nPos,	pTouzhuCQSSC->strHaoma,pTouzhuCQSSC->nHaoMaLen);
		}
 		if(pTouzhuCQSSC->nEnd == 1)
 		{
 			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CONTINUE_RET,dwContextID,NULL,NULL);
 		}
		int nZhushu = 0;
		nZhushu = pTouzhuCQSSC->nZhushu; 
		memset(szHaoma, 0, sizeof(szHaoma));

		int nLeng =(sizeof(pTouzhuCQSSC->strHaoma)* pTouzhuCQSSC->nIndex+pTouzhuCQSSC->nHaoMaLen);

	
		int nCeshiSize = sizeof(szHaoma);
		int nRet =  CWHService::LZUnCompressData((byte*)szHaomaTemp, nLeng, (byte*)szHaoma, sizeof(szHaoma));
	
		CString strAllHaoma = ChangeStringToT(szHaoma);
		nCeshiSize = strAllHaoma.GetLength();
// 		if(nLeng <= 1024)
// 		{
// 			//正式语句
// 			if(!CheckTouzhuNum(strAllHaoma,nLeng,pTouzhuCQSSC->dwUserID,ChangeStringToT(pTouzhuCQSSC->strQishu)))
// 			{
// 				return false;
// 			}
//  
//  
  			if(nZhushu <= 0)
  			{
  				return false;
  			}
// 			if(pTouzhuCQSSC->nGameKind == ZhongSanZhiXuanHeZhi || pTouzhuCQSSC->nGameKind == QianSanZhiXuanHeZhi || pTouzhuCQSSC->nGameKind == HouSanZhiXuanHeZhi)
// 			{
// 				CString strDanhao;
// 				AfxExtractSubString(strDanhao,strAllHaoma,0,',');
// 		
// 				if(strDanhao == strAllHaoma)
// 				{
// 					strAllHaoma = AddDouHao(strAllHaoma);
// 				}
// 			}
// 
// 			strLog.Format(L"CQSSCTZH touzhu5");
// 			OutputDebugString(strLog);
 			int nCheckZongzhu = GetTouZhuZongShu(pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma);
			strLog.Format(L"TOUZHU [%d] TypeID:%d,KindID:%d,zhushu:%d,checkzhushu:%d,haoma:[%s]",pTouzhuCQSSC->dwUserID,pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,nZhushu,nCheckZongzhu,strAllHaoma);
			CTraceService::TraceString(strLog,TraceLevel_Normal);
			if ((pTouzhuCQSSC->nGameType == CZ_PK10&&pTouzhuCQSSC->nGameKind == WF_GYHZH )||((pTouzhuCQSSC->nGameType == CZ3D||pTouzhuCQSSC->nGameType == CZPaiLie3)&&pTouzhuCQSSC->nGameKind == enWF_ZhixuanHezhi))
			{
				nCheckZongzhu = nZhushu;
			}

			if(nCheckZongzhu != nZhushu&&pTouzhuCQSSC->nGameType != CZ_LIUHECAI)
			{

				TCHAR szClientAddr[16]=TEXT("");
				BYTE * pClientAddr=(BYTE *)&pTouzhuCQSSC->dwClientAddr;
				_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

				CString strLogFile;
				strLogFile.Format(L"WRONGTOUZHU [%d] TypeID:%d,KindID:%d,haoma:[%s],zhushu:%d,checkzhushu:%d,ip:%s",pTouzhuCQSSC->dwUserID,pTouzhuCQSSC->nGameType,pTouzhuCQSSC->nGameKind,strAllHaoma,nZhushu,nCheckZongzhu,szClientAddr);
				LogFile::instance().LogText(strLogFile);

				DBO_GR_TouzhuResult  TouzhuRes;
				ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
				TouzhuRes.lResultCode = 11;
				TouzhuRes.nSign = pTouzhuCQSSC->nSign;
				TouzhuRes.nUserID = pTouzhuCQSSC->dwUserID;


				ZeroMemory(&szHaoma,sizeof(szHaoma));
				nZhushu = 0;
				//发送结果
				return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CQSSC_RET,dwContextID,&TouzhuRes,sizeof(TouzhuRes));

				return false;
			}
// 			strLog.Format(L"TESTTOUZHU touzhu6");
// 			OutputDebugString(strLog);
// 
// 		}
			if(nZhushu <= 0)
			{
				return false;
			}

 		m_GameSSCDBAide.ResetParameter();
 		m_GameSSCDBAide.AddParameter(TEXT("@user_id"),pTouzhuCQSSC->dwUserID);
 		m_GameSSCDBAide.AddParameter(TEXT("@qishu"),ChangeStringToT(pTouzhuCQSSC->strQishu));
 		m_GameSSCDBAide.AddParameter(TEXT("@type"),pTouzhuCQSSC->nGameType);
 		m_GameSSCDBAide.AddParameter(TEXT("@kind"),pTouzhuCQSSC->nGameKind);
 		m_GameSSCDBAide.AddParameter(TEXT("@haoma"), strAllHaoma);
 		m_GameSSCDBAide.AddParameter(TEXT("@zhushu"), nZhushu);
 		m_GameSSCDBAide.AddParameter(TEXT("@beishu"),pTouzhuCQSSC->nBeitou);
		m_GameSSCDBAide.AddParameter(TEXT("@moshi"),pTouzhuCQSSC->nMoshi);
		m_GameSSCDBAide.AddParameter(TEXT("@isZhuihao"),pTouzhuCQSSC->bZhuihao);
		m_GameSSCDBAide.AddParameter(TEXT("@platform"),1);

		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_touzhu"),false);

		strLog.Format(L"CQSSCTZH touzhu5");
		OutputDebugString(strLog);

		if(lResultCode != 0)
		{
			USES_CONVERSION;
			CString strLog;
			strLog.Format(L"userid：%d, 投注返回错误：%d", pTouzhuCQSSC->dwUserID, lResultCode);
			CTraceService::TraceString(strLog,TraceLevel_Exception);
		}
		DBO_GR_TouzhuResult  TouzhuRes;
		ZeroMemory(&TouzhuRes,sizeof(TouzhuRes));
		TouzhuRes.lResultCode = lResultCode;
		TouzhuRes.nSign = pTouzhuCQSSC->nSign;
		TouzhuRes.nUserID = pTouzhuCQSSC->dwUserID;


		ZeroMemory(&szHaoma,sizeof(szHaoma));
		nZhushu = 0;
		//发送结果
		return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TOUZHU_CQSSC_RET,dwContextID,&TouzhuRes,sizeof(TouzhuRes));

	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception OnTouzhuCQSSCDan p_touzhu");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();
		return false;
	}
	return true;
}
//查询游戏开奖结果
bool CDataBaseEngineSink::OnQueryGameResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_QueryGameResult));
		if (wDataSize!=sizeof(DBR_GP_QueryGameResult)) return false;
//		int nSSCtypeid[9] = {CZChongQingSSC,CZJiangXiSSC,CZXinJiangSSC,CZGD11Xuan5,CZCQ11Xuan5,CZSD11Xuan5,CZJX11Xuan5,CZ_FENFEN_CAI,CZ_WUFEN_CAI};

		DBR_GP_QueryGameResult* pQueryGameResult = (DBR_GP_QueryGameResult*)pData;

		int nTypeid = pQueryGameResult->wKindID;
 		m_GameSSCDBAide.ResetParameter();
// 		m_GameSSCDBAide.AddParameter(TEXT("@nKindID"),nTypeid);

		//执行查询
		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_get_lucky_num"),true);

		//结果处理
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GP_QueryLotResult QueryLotResult[70];
			ZeroMemory(&QueryLotResult,sizeof(QueryLotResult));
			LONG lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult>=70)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_RESULT,dwContextID,&QueryLotResult,sizeof(DBO_GP_QueryLotResult)*lResult);
					ZeroMemory(&QueryLotResult,sizeof(QueryLotResult));
					lResult = 0;
				}
				QueryLotResult[lResult].wKindID = m_GameSSCDBAide.GetValue_INT(TEXT("typeid"));
				m_GameSSCDBAide.GetValue_String(TEXT("expect"),QueryLotResult[lResult].wPeriod,CountArray(QueryLotResult[lResult].wPeriod));
				m_GameSSCDBAide.GetValue_String(TEXT("opencode"),QueryLotResult[lResult].szLotNum,CountArray(QueryLotResult[lResult].szLotNum));
				m_GameSSCDBAide.GetValue_String(TEXT("opentime"),QueryLotResult[lResult].szShijian,CountArray(QueryLotResult[lResult].szShijian));



				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();
			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_RESULT,dwContextID,&QueryLotResult,sizeof(DBO_GP_QueryLotResult)*lResult);
		}

		return true;
	

	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_get_lucky_num");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}
	return true;
}
//查询银行
bool CDataBaseEngineSink::OnQueryYinhang(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{

	
 		m_GameSSCDBAide.ResetParameter();

		//执行查询
		LONG lResultCode=m_GameSSCDBAide.ExecuteProcess(TEXT("p_getyinhang"),true);

		//结果处理
		if(lResultCode == DB_SUCCESS)
		{
			DBO_GP_GetBankName QueryYinHangResult[20];
			ZeroMemory(&QueryYinHangResult,sizeof(QueryYinHangResult));
			BYTE lResult = 0;
			while ((m_GameSSCDBModule->IsRecordsetEnd() == FALSE))
			{
				if(lResult >= 19)
				{
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_YINHANG_RET,dwContextID,&QueryYinHangResult,sizeof(DBO_GP_GetBankName)*lResult);
					lResult = 0;
				}
				m_GameSSCDBAide.GetValue_String(TEXT("yinhang"),QueryYinHangResult[lResult].szYinHangName,CountArray(QueryYinHangResult[lResult].szYinHangName));

				lResult++;
				m_GameSSCDBModule->MoveToNext();
			}	
			m_GameSSCDBModule->CloseRecordset();

			//发送结果
			return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_YINHANG_RET,dwContextID,&QueryYinHangResult,sizeof(DBO_GP_GetBankName)*lResult);
		}

	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception p_getyinhang");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		return false;
	}
	return true;
}
//修改机器
bool CDataBaseEngineSink::OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyMachine));
		if (wDataSize!=sizeof(DBR_GP_ModifyMachine)) return false;

		//请求处理
		DBR_GP_ModifyMachine * pModifyMachine=(DBR_GP_ModifyMachine *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//绑定操作
		if (pModifyMachine->cbBind==TRUE)
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachine"),false);
		}
		else
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UnMoorMachine"),false);
		}

		//结果处理
		if (m_AccountsDBAide.GetReturnValue()==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_OperateSuccess OperateSuccess;
			ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

			//获取信息
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//构造变量
			OperateSuccess.lResultCode=m_AccountsDBModule->GetReturnValue();
			lstrcpyn(OperateSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateSuccess.szDescribeString));

			//发送结果
			WORD wStringSize=CountStringBuffer(OperateSuccess.szDescribeString);
			WORD wSendSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wSendSize);
		}
		else
		{
			//变量定义
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//获取信息
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//构造变量
			OperateFailure.lResultCode=m_AccountsDBModule->GetReturnValue();
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//发送结果
			WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CTraceService::TraceString(_T("修改机器 ThASDDis ok?"),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GP_MoorMachine");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		return false;
	}

	return true;
}

//修改头像
bool CDataBaseEngineSink::OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_ModifySystemFace));
		if (wDataSize!=sizeof(DBR_GP_ModifySystemFace)) return false;

		//请求处理
		DBR_GP_ModifySystemFace * pModifySystemFace=(DBR_GP_ModifySystemFace *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifySystemFace->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifySystemFace->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifySystemFace->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pModifySystemFace->wFaceID);

		//机器信息
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifySystemFace->szMachineID);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//结果处理
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SystemFaceInsert"),true)==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

			//读取信息
			UserFaceInfo.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//结果处理
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CTraceService::TraceString(_T("TASDhis ok?"),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GP_SystemFaceInsert");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		return false;
	}

	return true;
}

//修改密码
bool CDataBaseEngineSink::OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyLogonPass));
		if (wDataSize!=sizeof(DBR_GP_ModifyLogonPass)) return false;

		//请求处理
		DBR_GP_ModifyLogonPass * pModifyLogonPass=(DBR_GP_ModifyLogonPass *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyLogonPass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyLogonPass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyLogonPass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyLogonPass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyLogonPassword"),false);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CTraceService::TraceString(_T("TASDDDhis ok?"),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GP_ModifyLogonPassword");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);


		return false;
	}

	return true;
}

//修改密码
bool CDataBaseEngineSink::OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyInsurePass));
		if (wDataSize!=sizeof(DBR_GP_ModifyInsurePass)) return false;

		//请求处理
		DBR_GP_ModifyInsurePass * pModifyInsurePass=(DBR_GP_ModifyInsurePass *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyInsurePass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//结果处理
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyInsurePassword"),false);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
			CTraceService::TraceString(_T("OnRequestModifyInsurePass This ok?"),TraceLevel_Exception);
			CString strLogFile;
			strLogFile.Format(L"Exception GSP_GP_ModifyInsurePassword");
			CTraceService::TraceString(strLogFile,TraceLevel_Exception);


		return false;
	}

	return true;
}

//修改资料
bool CDataBaseEngineSink::OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyIndividual));
		if (wDataSize!=sizeof(DBR_GP_ModifyIndividual)) return false;

		//请求处理
		DBR_GP_ModifyIndividual * pModifyIndividual=(DBR_GP_ModifyIndividual *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyIndividual->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pModifyIndividual->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pModifyIndividual->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pModifyIndividual->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strQQ"),pModifyIndividual->szQQ);
		m_AccountsDBAide.AddParameter(TEXT("@strEMail"),pModifyIndividual->szEMail);
		m_AccountsDBAide.AddParameter(TEXT("@strSeatPhone"),pModifyIndividual->szSeatPhone);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pModifyIndividual->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strDwellingPlace"),pModifyIndividual->szDwellingPlace);
		m_AccountsDBAide.AddParameter(TEXT("@strUserNote"),pModifyIndividual->szUserNote);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUserIndividual"),false);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CTraceService::TraceString(_T("OnRequestModifyIndividual This ok?"),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GP_ModifyUserIndividual");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);


		return false;
	}

	return true;
}

//存入金币
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_UserSaveScore));
		if (wDataSize!=sizeof(DBR_GP_UserSaveScore)) return false;

		//请求处理
		DBR_GP_UserSaveScore * pUserSaveScore=(DBR_GP_UserSaveScore *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserSaveScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CTraceService::TraceString(_T("ThiASDDBB3s ok?"),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GR_UserSaveScore");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		return false;
	}

	return true;
}

//提取金币
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_UserTakeScore));
		if (wDataSize!=sizeof(DBR_GP_UserTakeScore)) return false;

		//请求处理
		DBR_GP_UserTakeScore * pUserTakeScore=(DBR_GP_UserTakeScore *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTakeScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
CTraceService::TraceString(_T("ThisQWEWE ok?"),TraceLevel_Exception);
CString strLogFile;
strLogFile.Format(L"Exception GSP_GR_UserTakeScore");
CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		return false;
	}

	return true;
}

//转账金币
bool CDataBaseEngineSink::OnRequestUserTransScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_UserTransferScore));
		if (wDataSize!=sizeof(DBR_GP_UserTransferScore)) return false;

		//请求处理
		DBR_GP_UserTransferScore * pUserTransScore=(DBR_GP_UserTransferScore *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTransScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strLogonPassword"),pUserTransScore->szLogonPass);
		m_TreasureDBAide.AddParameter(TEXT("@strInsurePassword"),pUserTransScore->szInsurePass);
		m_TreasureDBAide.AddParameter(TEXT("@lTransferScore"),pUserTransScore->lTransferScore);
		m_TreasureDBAide.AddParameter(TEXT("@strDestAccounts"),pUserTransScore->szAccounts);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GR_UserTransferScore");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		return false;
	}

	return true;
}

//查询资料
bool CDataBaseEngineSink::OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_QueryIndividual));
		if (wDataSize!=sizeof(DBR_GP_QueryIndividual)) return false;

		//请求处理
		DBR_GP_QueryIndividual * pQueryIndividual=(DBR_GP_QueryIndividual *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQueryIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);


		////////EWIN网络修改
		//结果处理
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserIndividual"),true)==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_UserIndividual UserIndividual;
			ZeroMemory(&UserIndividual,sizeof(UserIndividual));

			//用户信息
			UserIndividual.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"),UserIndividual.szUserNote,CountArray(UserIndividual.szUserNote));
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"),UserIndividual.szCompellation,CountArray(UserIndividual.szCompellation));

			//电话号码
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),UserIndividual.szSeatPhone,CountArray(UserIndividual.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),UserIndividual.szMobilePhone,CountArray(UserIndividual.szMobilePhone));

			//联系资料
			m_AccountsDBAide.GetValue_String(TEXT("QQ"),UserIndividual.szQQ,CountArray(UserIndividual.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"),UserIndividual.szEMail,CountArray(UserIndividual.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),UserIndividual.szDwellingPlace,CountArray(UserIndividual.szDwellingPlace));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INDIVIDUAL,dwContextID,&UserIndividual,sizeof(UserIndividual));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GP_QueryUserIndividual");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}

//查询银行
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureInfo));
		if (wDataSize!=sizeof(DBR_GP_QueryInsureInfo)) return false;

		//请求处理
		DBR_GP_QueryInsureInfo * pQueryInsureInfo=(DBR_GP_QueryInsureInfo *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryInsureInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//结果处理
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserInsureInfo"),true)==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//银行信息
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_DOUBLE(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_DOUBLE(TEXT("Insure"));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnOperateDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CTraceService::TraceString(_T("ThiASDWWs ok?"),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GR_QueryUserInsureInfo");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);


		return false;
	}

	return true;
}

//加载列表
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//加载类型
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

		//发送种类
		wPacketSize=0;
		DBO_GP_GameType * pGameType=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GameType))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
			pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));

			//设置位移
			wPacketSize+=sizeof(DBO_GP_GameType);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		m_PlatformDBModule->CloseRecordset();
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//读取类型
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

		//发送类型
		wPacketSize=0;
		DBO_GP_GameKind * pGameKind=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GameKind))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
			pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
			m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
			m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));

			//设置位移
			wPacketSize+=sizeof(DBO_GP_GameKind);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		m_PlatformDBModule->CloseRecordset();
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);

		//变量定义
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//设置变量
		GameListResult.cbSuccess=TRUE;

		//发送消息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GP_LoadGameKindItem");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		CTraceService::TraceString(_T("ThisASDDDDD ok?"),TraceLevel_Exception);
		//变量定义
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//设置变量
		GameListResult.cbSuccess=FALSE;

		//发送消息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));
		ReconnectSql();
		return false;
	}

	return true;
}
void CDataBaseEngineSink::ReconnectSql()
{
	try
	{
		CTraceService::TraceString(_T("开始关闭数据库"),TraceLevel_Exception);
		m_AccountsDBModule->CloseConnection();
		m_TreasureDBModule->CloseConnection();
		m_PlatformDBModule->CloseConnection();
		m_GameSSCDBModule->CloseConnection();
		CTraceService::TraceString(_T("关闭完成，Sleep 3s"),TraceLevel_Exception);
		Sleep(3000);

		CTraceService::TraceString(_T("开始启动服务"),TraceLevel_Exception);
		//发起连接
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		//发起连接
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//发起连接
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

		//发起连接
		m_GameSSCDBModule->OpenConnection();
		m_GameSSCDBAide.SetDataBase(m_GameSSCDBModule.GetInterface());
	//	CTraceService::TraceString(_T("启动服务完成"),TraceLevel_Exception);
		CTraceService::TraceString(_T("启动服务完成"),TraceLevel_Exception);

	}
	catch (CMemoryException* e)
	{
		CTraceService::TraceString(_T("还是不能用"),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception ReconnectSql");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

	}

	return ;
}
//在线信息
bool CDataBaseEngineSink::OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		DBR_GP_OnLineCountInfo * pOnLineCountInfo=(DBR_GP_OnLineCountInfo *)pData;
		WORD wHeadSize=(sizeof(DBR_GP_OnLineCountInfo)-sizeof(pOnLineCountInfo->OnLineCountKind));

		//效验数据
		ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind))));
		if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

		//机器标识
		TCHAR szMachineID[LEN_MACHINE_ID];
		CWHService::GetMachineID(szMachineID);

		//构造信息
		TCHAR szOnLineCountKind[2048]=TEXT("");
		for (WORD i=0;i<pOnLineCountInfo->wKindCount;i++)
		{
			INT nLength=lstrlen(szOnLineCountKind);
			_sntprintf(&szOnLineCountKind[nLength],CountArray(szOnLineCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[i].wKindID,
				pOnLineCountInfo->OnLineCountKind[i].dwOnLineCount);
		}

		//构造参数
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineServer"),m_pInitParameter->m_szServerName);
		m_PlatformDBAide.AddParameter(TEXT("@dwOnLineCountSum"),pOnLineCountInfo->dwOnLineCountSum);
		m_PlatformDBAide.AddParameter(TEXT("@strOnLineCountKind"),szOnLineCountKind);

		//执行命令
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_OnLineCountInfo"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GP_OnLineCountInfo");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);

		ReconnectSql();
		return false;
	}

	return true;
}


//虚拟在线
bool CDataBaseEngineSink::OnRequestVirtualUserCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//加载类型
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadVirtualUserCount"),true);

		//发送种类
		wPacketSize=0;
		DBO_GP_VirtualCount * pVirtualCount=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GameType))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VIRTUAL_COUNT,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pVirtualCount=(DBO_GP_VirtualCount *)(cbBuffer+wPacketSize);
			pVirtualCount->wServerID=m_PlatformDBAide.GetValue_WORD(TEXT("ServerID"));
			pVirtualCount->dwVirtualCount=m_PlatformDBAide.GetValue_DWORD(TEXT("VirtualUserCount"));

			//设置位移
			wPacketSize+=sizeof(DBO_GP_VirtualCount);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		m_PlatformDBModule->CloseRecordset();
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VIRTUAL_COUNT,dwContextID,cbBuffer,wPacketSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		CString strLogFile;
		strLogFile.Format(L"Exception GSP_GP_LoadVirtualUserCount");
		CTraceService::TraceString(strLogFile,TraceLevel_Exception);
		ReconnectSql();

		return false;
	}

	return true;
}

//登录成功
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (bMobileClient==false)
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			CString strLog;
			strLog.Format(L"获取属性资料");
			LogFile::instance().LogText(strLog);

			//属性资料
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("IpAddrDescribe"),LogonSuccess.szAddrDescribe,CountArray(LogonSuccess.szAddrDescribe));

			//用户积分
			LogonSuccess.lScore=m_AccountsDBAide.GetValue_DOUBLE(TEXT("Score"));
			LogonSuccess.lInsure=m_AccountsDBAide.GetValue_DOUBLE(TEXT("Insure"));
			LogonSuccess.lUserMedal=m_AccountsDBAide.GetValue_DOUBLE(TEXT("UserMedal"));

			//用户资料
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));

			//会员资料
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			LogonSuccess.n_type = m_AccountsDBAide.GetValue_INT(TEXT("t_type"));
			LogonSuccess.f_fandian = m_AccountsDBAide.GetValue_DOUBLE(TEXT("t_fandian"));
			LogonSuccess.f_yue = m_AccountsDBAide.GetValue_DOUBLE(TEXT("t_yue"));
			LogonSuccess.f_dongjie = m_AccountsDBAide.GetValue_DOUBLE(TEXT("t_dongjie"));
			//获取信息
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//发送结果
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			strLog.Format(L"获取完成，开始发送");
			LogFile::instance().LogText(strLog);

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else
		{
			//变量定义
			DBO_GP_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//构造数据
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//发送结果
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//变量定义
			DBO_MB_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//属性资料
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("IpAddrDescribe"),LogonSuccess.szAddrDescribe,CountArray(LogonSuccess.szAddrDescribe));

			//获取信息
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//发送结果
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else
		{
			//变量定义
			DBO_MB_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//构造数据
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//发送结果
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//银行结果
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//变量定义
		DBO_GP_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//构造变量
		UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
		UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_DOUBLE(TEXT("SourceScore"));
		UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_DOUBLE(TEXT("SourceInsure"));
		UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_DOUBLE(TEXT("InsureRevenue"));
		UserInsureSuccess.lVariationScore=m_TreasureDBAide.GetValue_DOUBLE(TEXT("VariationScore"));
		UserInsureSuccess.lVariationInsure=m_TreasureDBAide.GetValue_DOUBLE(TEXT("VariationInsure"));
		lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//变量定义
		DBO_GP_UserInsureFailure UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//构造变量
		UserInsureFailure.lResultCode=dwErrorCode;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

//操作结果
VOID CDataBaseEngineSink::OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//变量定义
		DBO_GP_OperateSuccess OperateSuccess;
		ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

		//构造变量
		OperateSuccess.lResultCode=dwErrorCode;
		lstrcpyn(OperateSuccess.szDescribeString,pszErrorString,CountArray(OperateSuccess.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(OperateSuccess.szDescribeString);
		WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//变量定义
		DBO_GP_OperateFailure OperateFailure;
		ZeroMemory(&OperateFailure,sizeof(OperateFailure));

		//构造变量
		OperateFailure.lResultCode=dwErrorCode;
		lstrcpyn(OperateFailure.szDescribeString,pszErrorString,CountArray(OperateFailure.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(OperateFailure.szDescribeString);
		WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wHeadSize+wDataSize);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
