#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"
#include "TraceLog.h"
#include "AddressByIp.h"
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//配置变量
	m_pGameParameter=NULL;
	m_pInitParameter=NULL;
	m_pDataBaseParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件变量
	m_pIDataBaseEngine=NULL;
	m_pIGameServiceManager=NULL;
	m_pIDataBaseEngineEvent=NULL;
	m_pIGameDataBaseEngineSink=NULL;

	//辅助变量
	ZeroMemory(&m_LogonFailure,sizeof(m_LogonFailure));
	ZeroMemory(&m_LogonSuccess,sizeof(m_LogonSuccess));

	return;
}

//析构函数
CDataBaseEngineSink::~CDataBaseEngineSink()
{
	//释放对象
	SafeRelease(m_pIGameDataBaseEngineSink);

	return;
}

//接口查询
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGameDataBaseEngine,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//获取对象
VOID * CDataBaseEngineSink::GetDataBase(REFGUID Guid, DWORD dwQueryVer)
{
	//效验状态
	ASSERT(m_GameDBModule.GetInterface()!=NULL);
	if (m_GameDBModule.GetInterface()==NULL) return NULL;

	//查询对象
	IDataBase * pIDataBase=m_GameDBModule.GetInterface();
	VOID * pIQueryObject=pIDataBase->QueryInterface(Guid,dwQueryVer);

	return pIQueryObject;
}

//获取对象
VOID * CDataBaseEngineSink::GetDataBaseEngine(REFGUID Guid, DWORD dwQueryVer)
{
	//效验状态
	ASSERT(m_pIDataBaseEngine!=NULL);
	if (m_pIDataBaseEngine==NULL) return NULL;

	//查询对象
	VOID * pIQueryObject=m_pIDataBaseEngine->QueryInterface(Guid,dwQueryVer);

	return pIQueryObject;
}

//投递结果
bool CDataBaseEngineSink::PostGameDataBaseResult(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return true;
}
void CDataBaseEngineSink::ReconnectSql()
{
	try
	{
		CTraceService::TraceString(_T("开始关闭数据库"),TraceLevel_Exception);
		m_GameDBModule->CloseConnection();
		m_TreasureDBModule->CloseConnection();

		CTraceService::TraceString(_T("关闭完成，Sleep 3s"),TraceLevel_Exception);
		Sleep(3000);

		CTraceService::TraceString(_T("开始启动服务"),TraceLevel_Exception);
		//发起连接
		m_GameDBModule->OpenConnection();
		m_GameDBAide.SetDataBase(m_GameDBModule.GetInterface());

		//发起连接
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		CTraceService::TraceString(_T("启动服务完成"),TraceLevel_Exception);

	}
	catch (CMemoryException* e)
	{
		CTraceService::TraceString(_T("还是不能用"),TraceLevel_Exception);

	}

	return ;
}

//启动事件
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//查询对象
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDataBaseEngine)!=NULL);
	m_pIDataBaseEngine=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDataBaseEngine);

	//创建对象
	if ((m_GameDBModule.GetInterface()==NULL)&&(m_GameDBModule.CreateInstance()==false))
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

	//连接游戏
	try
	{
		//连接信息
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;

		//设置连接
		m_GameDBModule->SetConnectionInfo(m_pDataBaseParameter->szDataBaseAddr,m_pDataBaseParameter->wDataBasePort,
		m_pDataBaseParameter->szDataBaseName,m_pDataBaseParameter->szDataBaseUser,m_pDataBaseParameter->szDataBasePass);

		//设置连接
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
		pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass);

		//发起连接
		m_GameDBModule->OpenConnection();
		m_GameDBAide.SetDataBase(m_GameDBModule.GetInterface());

		//发起连接
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//数据钩子
		ASSERT(m_pIGameServiceManager!=NULL);
		m_pIGameDataBaseEngineSink=(IGameDataBaseEngineSink *)m_pIGameServiceManager->CreateGameDataBaseEngineSink(IID_IGameDataBaseEngineSink,VER_IGameDataBaseEngineSink);

		//配置对象
		if ((m_pIGameDataBaseEngineSink!=NULL)&&(m_pIGameDataBaseEngineSink->InitializeSink(QUERY_ME_INTERFACE(IUnknownEx))==false))
		{
			//错误断言
			ASSERT(FALSE);

			//输出信息
			CTraceService::TraceString(TEXT("游戏数据库扩展钩子引擎对象配置失败"),TraceLevel_Exception);

			return false;
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//停止事件
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//配置变量
	m_pInitParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件变量
	m_pIGameServiceManager=NULL;
	m_pIDataBaseEngineEvent=NULL;

	//设置对象
	m_GameDBAide.SetDataBase(NULL);

	//释放对象
	SafeRelease(m_pIGameDataBaseEngineSink);

	//关闭连接
	if (m_GameDBModule.GetInterface()!=NULL)
	{
		m_GameDBModule->CloseConnection();
		m_GameDBModule.CloseInstance();
	}

	//关闭连接
	if (m_TreasureDBModule.GetInterface()!=NULL)
	{
		m_TreasureDBModule->CloseConnection();
		m_TreasureDBModule.CloseInstance();
	}

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
	switch (wRequestID)
	{
	case DBR_GR_LOGON_USERID:			//I D 登录
		{
			return OnRequestLogonUserID(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOGON_MOBILE:			//手机登录
		{
			return OnRequestLogonMobile(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOGON_ACCOUNTS:			//帐号登录
		{
			return OnRequestLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GR_WRITE_GAME_SCORE:		//游戏写分
		{
			return OnRequestWriteGameScore(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LEAVE_GAME_SERVER:		//离开房间
		{
			return OnRequestLeaveGameServer(dwContextID,pData,wDataSize);
		}
	case DBR_GR_DELETE_USER_LOCKER:		//解锁用户<by hxh>
		{
			return OnRequestDeleteUserLocker(dwContextID,pData,wDataSize);
		}
	case DBR_GR_GAME_SCORE_RECORD:		//游戏记录
		{
			return OnRequestGameScoreRecord(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOAD_PARAMETER:			//加载参数
		{
			return OnRequestLoadParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOAD_GAME_COLUMN:		//加载列表
		{
			return OnRequestLoadGameColumn(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOAD_ANDROID_USER:		//加载机器
		{
			return OnRequestLoadAndroidUser(dwContextID,pData,wDataSize);
		}
	case DBR_GR_USER_SAVE_SCORE:		//存入金币
		{
			return OnRequestUserSaveScore(dwContextID,pData,wDataSize);
		}
	case DBR_GR_USER_TAKE_SCORE:		//提取金币
		{
			return OnRequestUserTakeScore(dwContextID,pData,wDataSize);
		}
	case DBR_GR_USER_TRANSFER_SCORE:	//转账金币
		{
			return OnRequestUserTransferScore(dwContextID,pData,wDataSize);
		}
	case DBR_GR_QUERY_INSURE_INFO:		//查询银行
		{
			return OnRequestQueryInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GR_QUERY_IP_DESCRIBE:      //查询地址
		{
			return OnRequestQueryIpDescribe(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//I D 登录
bool CDataBaseEngineSink::OnRequestLogonUserID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_LogonUserID));
		if (wDataSize!=sizeof(DBR_GR_LogonUserID)) return false;

		//执行查询
		DBR_GR_LogonUserID * pLogonUserID=(DBR_GR_LogonUserID *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonUserID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		lstrcpyn(szClientAddr,pLogonUserID->szIPAdrr,CountArray(szClientAddr));
		//转换地址
		DWORD dwClientAddr= (DWORD)(((0xffff&pClientAddr[0])<<24)|((0xffff&pClientAddr[1])<<16)|((0xffff&pClientAddr[2])<<8)|(0xffff&pClientAddr[3]));

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pLogonUserID->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@strPassword"),pLogonUserID->szPassword);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@dwClientIP"),(LONGLONG)dwClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLogonUserID->szMachineID);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),m_LogonFailure.szDescribeString,sizeof(m_LogonFailure.szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyUserID"),true);

		CString strAddr;
		strAddr.Format(L"%s",pLogonUserID->szIPAdrr);

		//OutputDebugString(pLogonUserID->szIPAdrr);
		strAddr = GetAddressDetail(strAddr);
		//用户信息
		m_LogonSuccess.cbPlazaType = pLogonUserID->cbPlazaType; 
		lstrcpyn(m_LogonSuccess.szPassword,pLogonUserID->szPassword,CountArray(m_LogonSuccess.szPassword));
		lstrcpyn(m_LogonSuccess.szMachineID,pLogonUserID->szMachineID,CountArray(m_LogonSuccess.szMachineID));
		lstrcpyn(m_LogonSuccess.szAddrDescribe,strAddr,CountArray(m_LogonSuccess.szPassword));

	
		//结果处理
		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于链接服务器出现错误，请退出后重新登录！"),false);

		//变量定义
		DBR_GR_LogonUserID * pLogonUserID=(DBR_GR_LogonUserID *)pData;
		if(pLogonUserID==NULL) return false;

		//外部输出
		CTraceLog TraceLog;
		CString strLogFile;
		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
		TraceLog.OpenLog(strLogFile);

		//变量定义
		CString strTraceLog;
		strTraceLog.Format(TEXT("ID登录异常: dwUserID=%d,strPassword=%s,strMachineID=%s"),
			pLogonUserID->dwUserID,pLogonUserID->szPassword,pLogonUserID->szMachineID);
		
		//输出日志
		TraceLog.TraceString(strTraceLog,TraceLevel_Exception);
		CTraceService::TraceString(strTraceLog,TraceLevel_Debug);
		TraceLog.CloseLog();		

		return false;
	}

	return true;
}
CString	CDataBaseEngineSink::GetAddressDetail(CString strIpaddress)
{
	CString strAddress;
	CString strFileName = CINIFile::GetAppPath();
	strFileName += L"IP.Dat";
	if( !CINIFile::IsFileExist(strFileName))
	{
		strFileName = CINIFile::GetAppPath();
		strFileName += L"../IP.Dat";
	}
	if( !CINIFile::IsFileExist(strFileName))
		return TEXT("");

	USES_CONVERSION;
	CIpFinder	Ip(T2A(strFileName));
	std::string strContry;
	std::string strLocation;
	Ip.GetAddressByIp(T2A(strIpaddress), strContry, strLocation);
	strAddress = (strContry /*+ strLocation*/).c_str();
	return strAddress;
}
//I D 登录
bool CDataBaseEngineSink::OnRequestLogonMobile(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		CString strLog;
		strLog.Format(L"CXCP CDataBaseEngineSink wDataSize = %d,sizeof = %d",wDataSize,sizeof(DBR_GR_LogonMobile));
		//OutputDebugString(strLog);

		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_LogonMobile));
		if (wDataSize!=sizeof(DBR_GR_LogonMobile)) return false;

		//执行查询
		DBR_GR_LogonMobile * pLogonMobile=(DBR_GR_LogonMobile *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonMobile->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//转换地址
		DWORD dwClientAddr= (DWORD)(((0xffff&pClientAddr[0])<<24)|((0xffff&pClientAddr[1])<<16)|((0xffff&pClientAddr[2])<<8)|(0xffff&pClientAddr[3]));

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pLogonMobile->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@strPassword"),pLogonMobile->szPassword);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@dwClientIP"),(LONGLONG)dwClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLogonMobile->szMachineID);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),m_LogonFailure.szDescribeString,sizeof(m_LogonFailure.szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyMobile"),true);

		//用户信息
		lstrcpyn(m_LogonSuccess.szPassword,pLogonMobile->szPassword,CountArray(m_LogonSuccess.szPassword));
		lstrcpyn(m_LogonSuccess.szMachineID,pLogonMobile->szMachineID,CountArray(m_LogonSuccess.szMachineID));
		// add by hxh 20160627
#if 1
		//m_LogonSuccess.cbDeviceType=pLogonMobile->cbDeviceType;
		m_LogonSuccess.wBehaviorFlags=pLogonMobile->wBehaviorFlags;
		m_LogonSuccess.wPageTableCount=pLogonMobile->wPageTableCount;
#endif
	
		//结果处理
		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于链接服务器出现错误，请退出后重新登录！"),true);

		return false;
	}

	return true;
}

//帐号登录
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_LogonAccounts));
		if (wDataSize!=sizeof(DBR_GR_LogonAccounts)) return false;

		//请求处理
		DBR_GR_LogonAccounts * pLogonAccounts=(DBR_GR_LogonAccounts *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//转换地址
		DWORD dwClientAddr= (DWORD)(((0xffff&pClientAddr[0])<<24)|((0xffff&pClientAddr[1])<<16)|((0xffff&pClientAddr[2])<<8)|(0xffff&pClientAddr[3]));

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_GameDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@dwClientIP"),(LONGLONG)dwClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),m_LogonFailure.szDescribeString,sizeof(m_LogonFailure.szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyAccounts"),true);

		//用户信息
		lstrcpyn(m_LogonSuccess.szPassword,pLogonAccounts->szPassword,CountArray(m_LogonSuccess.szPassword));
		lstrcpyn(m_LogonSuccess.szMachineID,pLogonAccounts->szMachineID,CountArray(m_LogonSuccess.szMachineID));

		//结果处理
		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于链接服务器出现错误，请退出后重新登录！"),false);

		//变量定义
		DBR_GR_LogonAccounts * pLogonAccounts=(DBR_GR_LogonAccounts *)pData;
		if(pLogonAccounts==NULL) return false;

		//外部输出
// 		CTraceLog TraceLog;
// 		CString strLogFile;
// 		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
// 		TraceLog.OpenLog(strLogFile);

		//变量定义
		CString strTraceLog;
		strTraceLog.Format(TEXT("账号登录异常: strAccounts=%s,strPassword=%s,strMachineID=%s"),pLogonAccounts->szAccounts,pLogonAccounts->szPassword,pLogonAccounts->szMachineID);
		
		//输出日志
	//	TraceLog.TraceString(strTraceLog,TraceLevel_Exception);
		CTraceService::TraceString(strTraceLog,TraceLevel_Debug);
	//	TraceLog.CloseLog();
		ReconnectSql();

		return false;
	}

	return true;
}

//游戏写分
bool CDataBaseEngineSink::OnRequestWriteGameScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_WriteGameScore));
		if (wDataSize!=sizeof(DBR_GR_WriteGameScore)) return false;

		//请求处理
		DBR_GR_WriteGameScore * pWriteGameScore=(DBR_GR_WriteGameScore *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pWriteGameScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_GameDBAide.ResetParameter();

		//系统信息
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pWriteGameScore->dwUserID);

		//变更成绩
		m_GameDBAide.AddParameter(TEXT("@lVariationScore"),pWriteGameScore->VariationInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lVariationInsure"),pWriteGameScore->VariationInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lVariationRevenue"),pWriteGameScore->VariationInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lVariationUserMedal"),pWriteGameScore->VariationInfo.lUserMedal);		
		m_GameDBAide.AddParameter(TEXT("@lVariationWinCount"),pWriteGameScore->VariationInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationLostCount"),pWriteGameScore->VariationInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationDrawCount"),pWriteGameScore->VariationInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationFleeCount"),pWriteGameScore->VariationInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationExperience"),pWriteGameScore->VariationInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@dwVariationPlayTimeCount"),pWriteGameScore->VariationInfo.dwPlayTimeCount);

		//属性信息
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		
		//EWIN网络科技 2013.3.18
		m_GameDBAide.AddParameter(TEXT("@dwDBQuestID"),pWriteGameScore->dwDBQuestID);
		m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"),pWriteGameScore->dwInoutIndex);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_WriteGameScore"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//变量定义
		DBR_GR_WriteGameScore * pWriteGameScore=(DBR_GR_WriteGameScore *)pData;
		if(pWriteGameScore==NULL) return false;

// 		//外部输出
// 		CTraceLog TraceLog;
// 		CString strLogFile;
// 		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
// 		TraceLog.OpenLog(strLogFile);

		//变量定义
		CString strTraceLog;
		strTraceLog.Format(TEXT("游戏写分异常: lVariationScore=%0.2f,lVariationInsure=%0.2f,lVariationRevenue=%0.2f,lVariationUserMedal=%0.2f,lVariationWinCount=%d,lVariationLostCount=%d,lVariationDrawCount=%d,lVariationFleeCount=%d,lVariationExperience=%d,dwVariationPlayTimeCount=%d"),
		pWriteGameScore->VariationInfo.lScore,pWriteGameScore->VariationInfo.lInsure,pWriteGameScore->VariationInfo.lRevenue,pWriteGameScore->VariationInfo.lUserMedal,pWriteGameScore->VariationInfo.dwWinCount,pWriteGameScore->VariationInfo.dwLostCount,pWriteGameScore->VariationInfo.dwDrawCount,pWriteGameScore->VariationInfo.dwFleeCount,pWriteGameScore->VariationInfo.dwExperience,pWriteGameScore->VariationInfo.dwPlayTimeCount);
		
		//输出日志
	//	TraceLog.TraceString(strTraceLog,TraceLevel_Exception);
		CTraceService::TraceString(strTraceLog,TraceLevel_Debug);
	//	TraceLog.CloseLog();
		ReconnectSql();
		return false;
	}

	return true;
}

//离开房间
bool CDataBaseEngineSink::OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_LeaveGameServer));
		if (wDataSize!=sizeof(DBR_GR_LeaveGameServer)) return false;

		//请求处理
		DBR_GR_LeaveGameServer * pLeaveGameServer=(DBR_GR_LeaveGameServer *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLeaveGameServer->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_GameDBAide.ResetParameter();

		//用户信息
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pLeaveGameServer->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwOnLineTimeCount"),pLeaveGameServer->dwOnLineTimeCount);

		//系统信息
		m_GameDBAide.AddParameter(TEXT("@dwDBQuestID"),pLeaveGameServer->dwDBQuestID);
		m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"),pLeaveGameServer->dwInoutIndex);
		m_GameDBAide.AddParameter(TEXT("@dwLeaveReason"),pLeaveGameServer->dwLeaveReason);

		//记录成绩
		m_GameDBAide.AddParameter(TEXT("@lRecordScore"),pLeaveGameServer->RecordInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lRecordInsure"),pLeaveGameServer->RecordInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lRecordRevenue"),pLeaveGameServer->RecordInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lRecordUserMedal"),pLeaveGameServer->RecordInfo.lUserMedal);
		m_GameDBAide.AddParameter(TEXT("@lRecordWinCount"),pLeaveGameServer->RecordInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordLostCount"),pLeaveGameServer->RecordInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordDrawCount"),pLeaveGameServer->RecordInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordFleeCount"),pLeaveGameServer->RecordInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordExperience"),pLeaveGameServer->RecordInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@dwRecordPlayTimeCount"),pLeaveGameServer->RecordInfo.dwPlayTimeCount);

		//变更成绩
		m_GameDBAide.AddParameter(TEXT("@lVariationScore"),pLeaveGameServer->VariationInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lVariationInsure"),pLeaveGameServer->VariationInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lVariationRevenue"),pLeaveGameServer->VariationInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lVariationUserMedal"),pLeaveGameServer->VariationInfo.lUserMedal);
		m_GameDBAide.AddParameter(TEXT("@lVariationWinCount"),pLeaveGameServer->VariationInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationLostCount"),pLeaveGameServer->VariationInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationDrawCount"),pLeaveGameServer->VariationInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationFleeCount"),pLeaveGameServer->VariationInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationExperience"),pLeaveGameServer->VariationInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@dwVariationPlayTimeCount"),pLeaveGameServer->VariationInfo.dwPlayTimeCount);

		//其他参数
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLeaveGameServer->szMachineID);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LeaveGameServer"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		DBR_GR_LeaveGameServer * pLeaveGameServer=(DBR_GR_LeaveGameServer *)pData;
		if(pLeaveGameServer==NULL) return false;

// 		//外部输出
// 		CTraceLog TraceLog;
// 		CString strLogFile;
// 		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
// 		TraceLog.OpenLog(strLogFile);

		//变量定义
		CString strTraceLog;
		strTraceLog.Format(TEXT("离开房间异常: dwUserID=%d,dwOnLineTimeCount=%d,dwInoutIndex=%d,dwLeaveReason=%d,lRecordScore=%0.2f,lRecordInsure%0.2f,lVariationRevenue=%0.2f,lVariationUserMedal=%0.2f,lVariationWinCount=%d,lVariationLostCount=%d,lVariationDrawCount=%d,lVariationFleeCount=%d,lVariationExperience=%d,dwVariationPlayTimeCount=%d,lVariationScore=%0.2f,lVariationInsure=%0.2f,lVariationRevenue=%0.2f,lVariationUserMedal=%0.2f,lVariationWinCount=%d,lVariationLostCount=%d,lVariationDrawCount=%d,lVariationFleeCount=%d,lVariationExperience=%d,dwVariationPlayTimeCount=%d"),pLeaveGameServer->dwUserID,pLeaveGameServer->dwOnLineTimeCount,pLeaveGameServer->dwInoutIndex,pLeaveGameServer->dwLeaveReason,pLeaveGameServer->RecordInfo.lScore,pLeaveGameServer->VariationInfo.lInsure,pLeaveGameServer->RecordInfo.lRevenue,pLeaveGameServer->RecordInfo.lUserMedal,pLeaveGameServer->RecordInfo.dwWinCount,pLeaveGameServer->RecordInfo.dwLostCount,pLeaveGameServer->RecordInfo.dwDrawCount,pLeaveGameServer->RecordInfo.dwFleeCount,pLeaveGameServer->RecordInfo.dwExperience,pLeaveGameServer->RecordInfo.dwPlayTimeCount,pLeaveGameServer->VariationInfo.lScore,pLeaveGameServer->VariationInfo.lInsure,pLeaveGameServer->VariationInfo.lRevenue,pLeaveGameServer->VariationInfo.lUserMedal,pLeaveGameServer->VariationInfo.dwWinCount,pLeaveGameServer->VariationInfo.dwLostCount,pLeaveGameServer->VariationInfo.dwDrawCount,pLeaveGameServer->VariationInfo.dwFleeCount,pLeaveGameServer->VariationInfo.dwExperience,pLeaveGameServer->VariationInfo.dwPlayTimeCount);
		
		//输出日志
//		TraceLog.TraceString(strTraceLog,TraceLevel_Exception);
		CTraceService::TraceString(strTraceLog,TraceLevel_Debug);
//		TraceLog.CloseLog();		
		ReconnectSql();

		return false;
	}

	return true;
}

//解锁用户
bool CDataBaseEngineSink::OnRequestDeleteUserLocker(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_DeleteUserLocker));
		if (wDataSize!=sizeof(DBR_GR_DeleteUserLocker)) return false;

		//请求处理
		DBR_GR_DeleteUserLocker * pDeleteUserLocker=(DBR_GR_DeleteUserLocker *)pData;

		//构造参数
		m_GameDBAide.ResetParameter();

		//用户信息
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pDeleteUserLocker->dwUserID);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_DeleteUserLocker"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		DBR_GR_DeleteUserLocker * pDeleteUserLocker=(DBR_GR_DeleteUserLocker *)pData;
		if(pDeleteUserLocker==NULL) return false;

// 		//外部输出
// 		CTraceLog TraceLog;
// 		CString strLogFile;
// 		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
// 		TraceLog.OpenLog(strLogFile);

		//变量定义
		CString strTraceLog;
		strTraceLog.Format(TEXT("解锁用户异常: dwUserID=%d"),pDeleteUserLocker->dwUserID);
		
		//输出日志
//		TraceLog.TraceString(strTraceLog,TraceLevel_Exception);
		CTraceService::TraceString(strTraceLog,TraceLevel_Debug);
//		TraceLog.CloseLog();		
		ReconnectSql();

		return false;
	}

	return true;
}

//游戏记录
bool CDataBaseEngineSink::OnRequestGameScoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		DBR_GR_GameScoreRecord * pGameScoreRecord=(DBR_GR_GameScoreRecord *)pData;

		//效验参数
		ASSERT(wDataSize<=sizeof(DBR_GR_GameScoreRecord));
		ASSERT(wDataSize>=(sizeof(DBR_GR_GameScoreRecord)-sizeof(pGameScoreRecord->GameScoreRecord)));
		ASSERT(wDataSize==(sizeof(DBR_GR_GameScoreRecord)-sizeof(pGameScoreRecord->GameScoreRecord)+pGameScoreRecord->wRecordCount*sizeof(pGameScoreRecord->GameScoreRecord[0])));

		//效验参数
		if (wDataSize>sizeof(DBR_GR_GameScoreRecord)) return false;
		if (wDataSize<(sizeof(DBR_GR_GameScoreRecord)-sizeof(pGameScoreRecord->GameScoreRecord))) return false;
		if (wDataSize!=(sizeof(DBR_GR_GameScoreRecord)-sizeof(pGameScoreRecord->GameScoreRecord)+pGameScoreRecord->wRecordCount*sizeof(pGameScoreRecord->GameScoreRecord[0]))) return false;

		//房间信息
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//桌子信息
		m_GameDBAide.AddParameter(TEXT("@wTableID"),pGameScoreRecord->wTableID);
		m_GameDBAide.AddParameter(TEXT("@wUserCount"),pGameScoreRecord->wUserCount);
		m_GameDBAide.AddParameter(TEXT("@wAndroidCount"),pGameScoreRecord->wAndroidCount);

		//税收损耗
		m_GameDBAide.AddParameter(TEXT("@lWasteCount"),pGameScoreRecord->lWasteCount);
		m_GameDBAide.AddParameter(TEXT("@lRevenueCount"),pGameScoreRecord->lRevenueCount);

		//统计信息
		m_GameDBAide.AddParameter(TEXT("@lUserMemal"),pGameScoreRecord->lUserMedal);
		m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"),pGameScoreRecord->dwPlayTimeCount);

		//时间信息
		m_GameDBAide.AddParameter(TEXT("@SystemTimeStart"),pGameScoreRecord->SystemTimeStart);
		m_GameDBAide.AddParameter(TEXT("@SystemTimeConclude"),pGameScoreRecord->SystemTimeConclude);

		//EWIN网络科技 2013.3.16
		//游戏记录
		m_GameDBAide.AddParameter(TEXT("@szDrawInfo"),pGameScoreRecord->szDrawInfo);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_RecordDrawInfo"),true);

		//写入记录
		if (lResultCode==DB_SUCCESS)
		{
			//获取标识
			DWORD dwDrawID=m_GameDBAide.GetValue_DWORD(TEXT("DrawID"));

			//写入记录
			for (WORD i=0;i<pGameScoreRecord->wRecordCount;i++)
			{
				//重置参数
				m_GameDBAide.ResetParameter();
				
				//房间信息
				m_GameDBAide.AddParameter(TEXT("@dwDrawID"),dwDrawID);
				m_GameDBAide.AddParameter(TEXT("@dwUserID"),pGameScoreRecord->GameScoreRecord[i].dwUserID);
				m_GameDBAide.AddParameter(TEXT("@wChairID"),pGameScoreRecord->GameScoreRecord[i].wChairID);

				//成绩信息
				m_GameDBAide.AddParameter(TEXT("@lScore"),pGameScoreRecord->GameScoreRecord[i].lScore);
				m_GameDBAide.AddParameter(TEXT("@lGrade"),pGameScoreRecord->GameScoreRecord[i].lGrade);
				m_GameDBAide.AddParameter(TEXT("@lRevenue"),pGameScoreRecord->GameScoreRecord[i].lRevenue);
				m_GameDBAide.AddParameter(TEXT("@lUserMedal"),pGameScoreRecord->GameScoreRecord[i].lUserMedal);
				m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"),pGameScoreRecord->GameScoreRecord[i].dwPlayTimeCount);

				//执行查询
				m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_RecordDrawScore"),false);
			}
		}
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		DBR_GR_GameScoreRecord * pGameScoreRecord=(DBR_GR_GameScoreRecord *)pData;
		if(pGameScoreRecord==NULL) return false;

// 		//外部输出
// 		CTraceLog TraceLog;
// 		CString strLogFile;
// 		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
// 		TraceLog.OpenLog(strLogFile);

		//变量定义
		CString strTraceLog;
		strTraceLog.Format(TEXT("游戏记录异常: wTableID=%d,wUserCount=%d,wAndroidCount=%d,lWasteCount=%0.2f,lRevenueCount=%0.2f,lUserMemal=%0.2f,dwPlayTimeCoun=%d"),pGameScoreRecord->wTableID,pGameScoreRecord->wUserCount,pGameScoreRecord->wAndroidCount,pGameScoreRecord->lWasteCount,pGameScoreRecord->lRevenueCount,pGameScoreRecord->lUserMedal,pGameScoreRecord->dwPlayTimeCount);
		
		//输出日志
//		TraceLog.TraceString(strTraceLog,TraceLevel_Exception);
		CTraceService::TraceString(strTraceLog,TraceLevel_Debug);
	//	TraceLog.CloseLog();
		ReconnectSql();

		return false;
	}

	return true;
}

//加载参数
bool CDataBaseEngineSink::OnRequestLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		DBO_GR_GameParameter GameParameter;
		ZeroMemory(&GameParameter,sizeof(GameParameter));

		//执行查询
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadParameter"),true);

		//读取信息
		if (lResultCode==DB_SUCCESS)
		{
			//版本信息
			GameParameter.dwClientVersion=m_GameDBAide.GetValue_DWORD(TEXT("ClientVersion"));
			GameParameter.dwServerVersion=m_GameDBAide.GetValue_DWORD(TEXT("ServerVersion"));

			//读取变量
			GameParameter.wRevenueRate=m_GameDBAide.GetValue_WORD(TEXT("RevenueRate"));
		
			//发送信息
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_PARAMETER,dwContextID,&GameParameter,sizeof(GameParameter));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		ReconnectSql();
		return false;
	}

	return true;
}

//加载列表
bool CDataBaseEngineSink::OnRequestLoadGameColumn(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		DBO_GR_GameColumnInfo GameColumnInfo;
		ZeroMemory(&GameColumnInfo,sizeof(GameColumnInfo));

		//执行查询
		m_GameDBAide.ResetParameter();
		GameColumnInfo.lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_GameColumnItem"),true);

		//读取信息
		for (WORD i=0;i<CountArray(GameColumnInfo.ColumnItemInfo);i++)
		{
			//结束判断
			if (m_GameDBModule->IsRecordsetEnd()==true) break;

			//溢出效验
			ASSERT(GameColumnInfo.cbColumnCount<CountArray(GameColumnInfo.ColumnItemInfo));
			if (GameColumnInfo.cbColumnCount>=CountArray(GameColumnInfo.ColumnItemInfo)) break;

			//读取数据
			GameColumnInfo.cbColumnCount++;
			GameColumnInfo.ColumnItemInfo[i].cbColumnWidth=m_GameDBAide.GetValue_BYTE(TEXT("ColumnWidth"));
			GameColumnInfo.ColumnItemInfo[i].cbDataDescribe=m_GameDBAide.GetValue_BYTE(TEXT("DataDescribe"));
			m_GameDBAide.GetValue_String(TEXT("ColumnName"),GameColumnInfo.ColumnItemInfo[i].szColumnName,CountArray(GameColumnInfo.ColumnItemInfo[i].szColumnName));

			//移动记录
			m_GameDBModule->MoveToNext();
		}

		//发送信息
		WORD wHeadSize=sizeof(GameColumnInfo)-sizeof(GameColumnInfo.ColumnItemInfo);
		WORD wPacketSize=wHeadSize+GameColumnInfo.cbColumnCount*sizeof(GameColumnInfo.ColumnItemInfo[0]);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_COLUMN_INFO,dwContextID,&GameColumnInfo,wPacketSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//变量定义
		DBO_GR_GameColumnInfo GameColumnInfo;
		ZeroMemory(&GameColumnInfo,sizeof(GameColumnInfo));

		//构造变量
		GameColumnInfo.cbColumnCount=0L;
		GameColumnInfo.lResultCode=pIException->GetExceptionResult();

		//发送信息
		WORD wPacketSize=sizeof(GameColumnInfo)-sizeof(GameColumnInfo.ColumnItemInfo);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_COLUMN_INFO,dwContextID,&GameColumnInfo,wPacketSize);

		return false;
	}

	return true;
}

//加载机器
bool CDataBaseEngineSink::OnRequestLoadAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		DBO_GR_GameAndroidInfo GameAndroidInfo;
		ZeroMemory(&GameAndroidInfo,sizeof(GameAndroidInfo));

		//用户帐户
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//执行查询
		GameAndroidInfo.lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadAndroidUser"),true);

		//读取信息
		for (WORD i=0;i<CountArray(GameAndroidInfo.AndroidParameter);i++)
		{
			//结束判断
			if (m_GameDBModule->IsRecordsetEnd()==true) break;

			//溢出效验
			ASSERT(GameAndroidInfo.wAndroidCount<CountArray(GameAndroidInfo.AndroidParameter));
			if (GameAndroidInfo.wAndroidCount>=CountArray(GameAndroidInfo.AndroidParameter)) break;

			//读取数据
			GameAndroidInfo.wAndroidCount++;
			GameAndroidInfo.AndroidParameter[i].dwUserID=m_GameDBAide.GetValue_DWORD(TEXT("UserID"));
			GameAndroidInfo.AndroidParameter[i].dwServiceTime=m_GameDBAide.GetValue_DWORD(TEXT("ServiceTime"));
			GameAndroidInfo.AndroidParameter[i].dwMinPlayDraw=m_GameDBAide.GetValue_DWORD(TEXT("MinPlayDraw"));
			GameAndroidInfo.AndroidParameter[i].dwMaxPlayDraw=m_GameDBAide.GetValue_DWORD(TEXT("MaxPlayDraw"));
			GameAndroidInfo.AndroidParameter[i].dwMinReposeTime=m_GameDBAide.GetValue_DWORD(TEXT("MinReposeTime"));
			GameAndroidInfo.AndroidParameter[i].dwMaxReposeTime=m_GameDBAide.GetValue_DWORD(TEXT("MaxReposeTime"));
			GameAndroidInfo.AndroidParameter[i].dwServiceGender=m_GameDBAide.GetValue_DWORD(TEXT("ServiceGender"));
			GameAndroidInfo.AndroidParameter[i].lMinTakeScore=(SCORE)m_GameDBAide.GetValue_LONGLONG(TEXT("MinTakeScore"));
			GameAndroidInfo.AndroidParameter[i].lMaxTakeScore=(SCORE)m_GameDBAide.GetValue_LONGLONG(TEXT("MaxTakeScore"));

			//EWIN网络
			//读取机器人地址
			m_GameDBAide.GetValue_String(TEXT("AndroidNote"),GameAndroidInfo.AndroidParameter[i].szAddrDescribe,sizeof(GameAndroidInfo.AndroidParameter[i].szAddrDescribe));

			//移动记录
			m_GameDBModule->MoveToNext();
		}

		//发送信息
		WORD wHeadSize=sizeof(GameAndroidInfo)-sizeof(GameAndroidInfo.AndroidParameter);
		WORD wDataSize=GameAndroidInfo.wAndroidCount*sizeof(GameAndroidInfo.AndroidParameter[0]);

		//CString ss;
		//ss.Format(_T("%d"),wHeadSize+wDataSize);
		//AfxMessageBox(ss);

		//EWIN网络 此处数据大小有限制，最大值为16376
		//带IP地址信息的机器人最大可以加200个

		//计算数据包长度，如果超出提示出错
		if((wHeadSize+wDataSize)>16375){

			//输出错误
			CTraceService::TraceString(TEXT("加载机器人：数据包长度超出最大值16376"),TraceLevel_Exception);

		}


		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_GAME_ANDROID_INFO,dwContextID,&GameAndroidInfo,wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//变量定义
		DBO_GR_GameAndroidInfo GameAndroidInfo;
		ZeroMemory(&GameAndroidInfo,sizeof(GameAndroidInfo));

		//构造变量
		GameAndroidInfo.wAndroidCount=0L;
		GameAndroidInfo.lResultCode=DB_ERROR;

		//发送信息
		WORD wHeadSize=sizeof(GameAndroidInfo)-sizeof(GameAndroidInfo.AndroidParameter);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_GAME_ANDROID_INFO,dwContextID,&GameAndroidInfo,wHeadSize);
	}

	return false;
}

//查询IP描述
bool CDataBaseEngineSink::OnRequestQueryIpDescribe(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		DBR_GR_QueryIpDescribeSuccess QueryIpDescribeSuccess;
		ZeroMemory(&QueryIpDescribeSuccess,sizeof(QueryIpDescribeSuccess));

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");		

		//执行查询
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);
		m_GameDBAide.ExecuteProcess(TEXT("GSP_GP_QueryIpAddress"),true);

		//读取数据
		m_GameDBAide.GetValue_String(TEXT("IpAddrDescribe"),QueryIpDescribeSuccess.szAddrDescribe,sizeof(QueryIpDescribeSuccess.szAddrDescribe));

		//发送信息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_QUERY_IP_DESCRIBE,dwContextID,&QueryIpDescribeSuccess,sizeof(QueryIpDescribeSuccess));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//变量定义
		DBR_GR_QueryIpDescribeSuccess QueryIpDescribeSuccess;
		ZeroMemory(&QueryIpDescribeSuccess,sizeof(QueryIpDescribeSuccess));

		//构造变量
		QueryIpDescribeSuccess.szAddrDescribe[0]=0;

		//发送信息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_QUERY_IP_DESCRIBE,dwContextID,&QueryIpDescribeSuccess,sizeof(QueryIpDescribeSuccess));

		//外部输出
		CTraceLog TraceLog;
		CString strLogFile;
		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
		TraceLog.OpenLog(strLogFile);
		
		//输出日志
		TraceLog.TraceString(TEXT("查询IP异常"),TraceLevel_Exception);
		CTraceService::TraceString(TEXT("查询IP异常"),TraceLevel_Debug);
		TraceLog.CloseLog();
	}

	return false;
}

//存入金币
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_UserSaveScore));
	if (wDataSize!=sizeof(DBR_GR_UserSaveScore)) return false;

	//变量定义
	DBR_GR_UserSaveScore * pUserSaveScore=(DBR_GR_UserSaveScore *)pData;

	//请求处理
	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
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
		OnInsureDisposeResult(dwContextID,lResultCode,pUserSaveScore->lSaveScore,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,pUserSaveScore->lSaveScore,TEXT("由于链接服务器出现错误，请退出后重新登录！"),false);

		//外部输出
		CTraceLog TraceLog;
		CString strLogFile;
		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
		TraceLog.OpenLog(strLogFile);

		//变量定义
		CString strTraceLog;
		strTraceLog.Format(TEXT("存入金币异常: dwUserI=%d,lSaveScore=%0.2f"),pUserSaveScore->dwUserID,pUserSaveScore->lSaveScore);
		
		//输出日志
		TraceLog.TraceString(strTraceLog,TraceLevel_Exception);
		CTraceService::TraceString(strTraceLog,TraceLevel_Debug);
		TraceLog.CloseLog();

		return false;
	}

	return true;
}

//提取金币
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_UserTakeScore));
	if (wDataSize!=sizeof(DBR_GR_UserTakeScore)) return false;

	//变量定义
	DBR_GR_UserTakeScore * pUserTakeScore=(DBR_GR_UserTakeScore *)pData;

	//请求处理
	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
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
		OnInsureDisposeResult(dwContextID,lResultCode,0L,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于链接服务器出现错误，请退出后重新登录！"),false);

		//外部输出
		CTraceLog TraceLog;
		CString strLogFile;
		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
		TraceLog.OpenLog(strLogFile);

		//变量定义
		CString strTraceLog;
		strTraceLog.Format(TEXT("提取金币异常: dwUserI=%d,lTakeScore=%0.2f,strPassword=%s"),pUserTakeScore->dwUserID,pUserTakeScore->lTakeScore,pUserTakeScore->szPassword);
		
		//输出日志
		TraceLog.TraceString(strTraceLog,TraceLevel_Exception);
		CTraceService::TraceString(strTraceLog,TraceLevel_Debug);
		TraceLog.CloseLog();

		return false;
	}

	return true;
}


//转账金币
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_UserTransferScore));
	if (wDataSize!=sizeof(DBR_GR_UserTransferScore)) return false;

	//变量定义
	DBR_GR_UserTransferScore * pUserTransferScore=(DBR_GR_UserTransferScore *)pData;

	//请求处理
	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransferScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strLogonPassword"),pUserTransferScore->szLogonPass);
		m_TreasureDBAide.AddParameter(TEXT("@strInsurePassword"),pUserTransferScore->szInsurePass);
		m_TreasureDBAide.AddParameter(TEXT("@lTransferScore"),pUserTransferScore->lTransferScore);
		m_TreasureDBAide.AddParameter(TEXT("@strDestAccounts"),pUserTransferScore->szAccounts);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransferScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,0L,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于链接服务器出现错误，请退出后重新登录！"),false);

		//外部输出
		CTraceLog TraceLog;
		CString strLogFile;
		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
		TraceLog.OpenLog(strLogFile);

		//变量定义
		CString strTraceLog;
		strTraceLog.Format(TEXT("转账金币异常: dwUserID=%d,strLogonPassword=%s,strInsurePassword=%s,lTransferScore=%0.2f,strDestAccounts=%s"),pUserTransferScore->dwUserID,pUserTransferScore->szLogonPass,pUserTransferScore->szInsurePass,pUserTransferScore->lTransferScore,pUserTransferScore->szAccounts);
		
		//输出日志
		TraceLog.TraceString(strTraceLog,TraceLevel_Exception);
		CTraceService::TraceString(strTraceLog,TraceLevel_Debug);
		TraceLog.CloseLog();

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
		ASSERT(wDataSize==sizeof(DBR_GR_QueryInsureInfo));
		if (wDataSize!=sizeof(DBR_GR_QueryInsureInfo)) return false;

		//请求处理
		DBR_GR_QueryInsureInfo * pQueryInsureInfo=(DBR_GR_QueryInsureInfo *)pData;

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
			DBO_GR_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//银行信息
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_DOUBLE(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_DOUBLE(TEXT("Insure"));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),0L,CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于链接服务器出现错误，请退出后重新登录！"),false);

		//请求处理
		DBR_GR_QueryInsureInfo * pQueryInsureInfo=(DBR_GR_QueryInsureInfo *)pData;
		if(pQueryInsureInfo==NULL)  return false;

		//外部输出
		CTraceLog TraceLog;
		CString strLogFile;
		strLogFile.Format(TEXT("%d_%d_DataBase.log"),m_pGameServiceOption->wKindID,m_pGameServiceOption->wServerID);
		TraceLog.OpenLog(strLogFile);

		//变量定义
		CString strTraceLog;
		strTraceLog.Format(TEXT("查询银行异常: dwUserID=%d"),pQueryInsureInfo->dwUserID);
		
		//输出日志
		TraceLog.TraceString(strTraceLog,TraceLevel_Exception);
		CTraceService::TraceString(strTraceLog,TraceLevel_Debug);
		TraceLog.CloseLog();

		return false;
	}

	return true;
}

//登录结果
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//属性资料
		m_LogonSuccess.wFaceID=m_GameDBAide.GetValue_WORD(TEXT("FaceID"));
		m_LogonSuccess.dwUserID=m_GameDBAide.GetValue_DWORD(TEXT("UserID"));
		m_GameDBAide.GetValue_String(TEXT("NickName"),m_LogonSuccess.szNickName,CountArray(m_LogonSuccess.szNickName));

		//用户资料
		m_LogonSuccess.cbGender=m_GameDBAide.GetValue_BYTE(TEXT("Gender"));
		m_LogonSuccess.cbMemberOrder=m_GameDBAide.GetValue_BYTE(TEXT("MemberOrder"));
		m_LogonSuccess.cbMasterOrder=m_GameDBAide.GetValue_BYTE(TEXT("MasterOrder"));

		//积分信息
		m_LogonSuccess.lScore=m_GameDBAide.GetValue_DOUBLE(TEXT("Score"));
		m_LogonSuccess.lInsure=m_GameDBAide.GetValue_DOUBLE(TEXT("Insure"));
		m_LogonSuccess.lUserMedal=m_GameDBAide.GetValue_DOUBLE(TEXT("UserMedal"));
		m_LogonSuccess.dwWinCount=m_GameDBAide.GetValue_LONG(TEXT("WinCount"));
		m_LogonSuccess.dwLostCount=m_GameDBAide.GetValue_LONG(TEXT("LostCount"));
		m_LogonSuccess.dwDrawCount=m_GameDBAide.GetValue_LONG(TEXT("DrawCount"));
		m_LogonSuccess.dwFleeCount=m_GameDBAide.GetValue_LONG(TEXT("FleeCount"));
		m_LogonSuccess.dwExperience=m_GameDBAide.GetValue_LONG(TEXT("Experience"));

		//附加信息
		m_LogonSuccess.dwUserRight=m_GameDBAide.GetValue_DWORD(TEXT("UserRight"));
		m_LogonSuccess.dwMasterRight=m_GameDBAide.GetValue_DWORD(TEXT("MasterRight"));

		//索引变量
		m_LogonSuccess.dwInoutIndex=m_GameDBAide.GetValue_DWORD(TEXT("InoutIndex"));

		//IP描述
	//	m_GameDBAide.GetValue_String(TEXT("IpAddrDescribe"),m_LogonSuccess.szAddrDescribe,CountArray(m_LogonSuccess.szAddrDescribe));

		CString strLog;
		strLog.Format(L"\nIPAddr = %s",m_LogonSuccess.szAddrDescribe);
		//OutputDebugString(strLog);
		//获取信息
		lstrcpyn(m_LogonSuccess.szDescribeString,pszErrorString,CountArray(m_LogonSuccess.szDescribeString));


		//发送结果
		WORD wDataSize=CountStringBuffer(m_LogonSuccess.szDescribeString);
		WORD wHeadSize=sizeof(m_LogonSuccess)-sizeof(m_LogonSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOGON_SUCCESS,dwContextID,&m_LogonSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//变量定义
		DBO_GR_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));

		//构造数据
		LogonFailure.lResultCode=dwErrorCode;
		lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
		WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
	}

	return;
}

//银行结果
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//变量定义
		DBO_GR_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//构造变量
		UserInsureSuccess.lFrozenedScore=lFrozenedScore;
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
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//变量定义
		DBO_GR_UserInsureFailure UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//构造变量
		UserInsureFailure.lResultCode=dwErrorCode;
		UserInsureFailure.lFrozenedScore=lFrozenedScore;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
