#include "StdAfx.h"
#include "GlobalUnits.h"
#include "MissionLogon.h"
#include "PlatformEvent.h"
#include "PlatformFrame.h"
#include "Ping.h"
//////////////////////////////////////////////////////////////////////////////////

//静态变量
CMissionLogon * CMissionLogon::m_pMissionLogon=NULL;					//对象指针

#define  IDI_LOGON_TIME	101
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMissionLogon, CDlgStatus)
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CMissionLogon::CMissionLogon()
{
	//设置变量
	m_bRegister=false;
	m_bRemPassword=false;
	m_bKillSocket = false;
	//控件指针
	m_pDlgLogon=NULL;
	//m_pDlgRegister=NULL;
	m_bKefu = false;

	//设置组件
	SetStatusViewSink(this);
	m_nAutoLogon=0;
	//设置对象
	ASSERT(m_pMissionLogon==NULL);
	if (m_pMissionLogon==NULL) m_pMissionLogon=this;

	return;
}
//析构函数
CMissionLogon::~CMissionLogon()
{
	//删除对象
	SafeDelete(m_pDlgLogon);
	//SafeDelete(m_pDlgRegister);

	//释放对象
	ASSERT(m_pMissionLogon==this);
	if (m_pMissionLogon==this) m_pMissionLogon=NULL;

	return;
}
void CMissionLogon::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == IDI_LOGON_TIME)
	{
		KillTimer(IDI_LOGON_TIME);
		OnStatusCancel();
	}
	CDlgStatus::OnTimer(nIDEvent);
}
//取消连接
VOID CMissionLogon::OnStatusCancel()
{
	//隐藏界面
	HideStatusWindow();

	//终止任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,true);

	//读取配置文件
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
	TCHAR szKey[32];
	CTime time = CTime::GetCurrentTime();

	_sntprintf(szKey,CountArray(szKey),_T("%d"),0);
	WritePrivateProfileString(time.Format(L"%Y-%m-%d"),TEXT("TodayCanUse"),szKey,szFileName);

	//重新登录
	if (m_bRegister==false)
	{
		ShowLogon();
	}
	else
	{
		ShowRegister();
	}

	return;
}

//连接事件
bool CMissionLogon::OnEventMissionLink(INT nErrorCode,INT nSocketID)
{
	//错误处理
	if (nErrorCode!=0L)
	{
		//关闭提示
		HideStatusWindow();


		//读取配置文件
		//工作目录
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//构造路径
		TCHAR szFileName[MAX_PATH]=TEXT("");
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
		TCHAR szKey[32];
		CTime time = CTime::GetCurrentTime();

		_sntprintf(szKey,CountArray(szKey),_T("%d"),0);
		WritePrivateProfileString(time.Format(L"%Y-%m-%d"),TEXT("TodayCanUse"),szKey,szFileName);

		//重新登录
		if (m_bRegister==false)
		{
			if (m_nAutoLogon>5)
			{
				//显示提示
				CInformation Information(CPlatformFrame::GetInstance());
				Information.ShowMessageBox(TEXT("尝试了所有的服务器都无法成功连接服务器，请留意网站维护公告！"),MB_ICONERROR);
				m_nAutoLogon=0;
				ShowLogon();
			}
			else
			{
				m_nAutoLogon++;
				m_pDlgLogon->LoadLogonServerInfo();
				PerformLogonMission(m_bRemPassword);

			}
			
		}
		else
		{
			ShowRegister();
		}
	}
	else
	{
		if(m_bKefu)
		{
			m_bKefu = false;
			//发送数据
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_USER_SERVICE,SUB_GP_KEFU_URL);
			
		}
		else
		{
			//变量定义
			WORD wPacketSize,wSubCmdID;
			BYTE cbBuffer[SOCKET_TCP_PACKET];

			//构造数据
			if (m_bRegister==false)
			{
				wSubCmdID=SUB_GP_LOGON_ACCOUNTS;
				wPacketSize=m_pDlgLogon->ConstructLogonPacket(cbBuffer,sizeof(cbBuffer));
			}
	// 		else
	// 		{
	// 			wSubCmdID=SUB_GP_REGISTER_ACCOUNTS;
	// 			wPacketSize=m_pDlgRegister->ConstructRegisterPacket(cbBuffer,sizeof(cbBuffer));
	// 		}

			//提示信息
			ShowStatusWindow(TEXT("正在验证用户登录信息..."));

			//发送数据
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_LOGON,wSubCmdID,cbBuffer,wPacketSize);

		}
		
	}

	return true;
}

//关闭事件
bool CMissionLogon::OnEventMissionShut(BYTE cbShutReason)
{
	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL && !m_bKillSocket)
	{

		TCHAR szPlatformUrl[150] = L"";
		TCHAR szPlatformUrl1[150] = L"";

		//读取配置文件
		//工作目录
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//构造路径
		TCHAR szFileName[MAX_PATH]=TEXT("");
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

		//读取配置
		CWHIniData IniData;
		IniData.SetIniFilePath(szFileName);


		int nTTL = 10000;
		CPing ping;
		PingReply reply;
		for(int i = 0;i < 5;i++)
		{
			memset(&reply, 0, sizeof(PingReply));
			TCHAR szKey[32];
			ZeroMemory(&szKey,sizeof(szKey));
			_sntprintf(szKey,CountArray(szKey),_T("LogonServer%d"),i);
			//读取配置
			IniData.ReadEncryptString(TEXT("PlazaUrl"),szKey,NULL,szPlatformUrl1,CountArray(szPlatformUrl1));

			USES_CONVERSION;
			int nValidHostName = inet_addr(W2A(szPlatformUrl1));

			if(nValidHostName == -1)
			{
				DWORD dwServerAddr = 0;
				LPHOSTENT lpHost=gethostbyname(CT2CA(szPlatformUrl1));
				if (lpHost!=NULL) 
					dwServerAddr=((LPIN_ADDR)lpHost->h_addr)->s_addr;
				in_addr t1;
				t1.S_un.S_addr = dwServerAddr;
				char* pTrueAddr = inet_ntoa(t1);
				OutputDebugStringA(pTrueAddr);

				ping.Ping(dwServerAddr,&reply);

			}
			else
			{
				char szUrl[124] = "";
				int iLength ;  
				//获取字节长度   
				iLength = WideCharToMultiByte(CP_ACP, 0, szPlatformUrl1, -1, NULL, 0, NULL, NULL);  
				//将tchar值赋给_char    
				WideCharToMultiByte(CP_ACP, 0, szPlatformUrl1, -1, szUrl, iLength, NULL, NULL);   
				ping.Ping(szUrl,&reply);
			}



			//CString strTmp;
			if(reply.m_dwRoundTripTime < nTTL || lstrlen(szPlatformUrl) == 0)
			{
				lstrcpyn(szPlatformUrl,szPlatformUrl1,CountArray(szPlatformUrl));
				if(reply.m_dwRoundTripTime > 0 && reply.m_dwRoundTripTime < 10000)
				{
					//如果PING通了，注册表记录一下当前SERVER
					CWHRegKey RegServerAddr;
					RegServerAddr.OpenRegKey(TEXT("CurrentLogonServer"),true);
					RegServerAddr.WriteString(TEXT("CurrentIpAddress"),szPlatformUrl);
				}
				nTTL = reply.m_dwRoundTripTime;
			}
		}

		if (m_pDlgLogon!=NULL) lstrcpyn(m_pDlgLogon->m_szLogonServer,szPlatformUrl,CountArray(m_pDlgLogon->m_szLogonServer));

		PerformLogonMission(m_bRemPassword);

	}

	return true;
}

//读取事件
bool CMissionLogon::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_LOGON)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_LOGON_SUCCESS:	//登录成功
			{
				return OnSocketSubLogonSuccess(pData,wDataSize);
			}
		case SUB_GP_LOGON_FAILURE:	//登录失败
			{
				return OnSocketSubLogonFailure(pData,wDataSize);
			}
		case SUB_GP_LOGON_FINISH:	//登录完成
			{
				return OnSocketSubLogonFinish(pData,wDataSize);
			}
		case SUB_GP_UPDATE_NOTIFY:	//升级提示
			{
				return OnSocketSubUpdateNotify(pData,wDataSize);
			}
		}
	}

	return false;
}

//显示登录
VOID CMissionLogon::ShowLogon()
{
	//效验状态
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return;

	//设置变量
	m_bRegister=false;

	//创建窗口
	if (m_pDlgLogon==NULL)
	{
		m_pDlgLogon=new CDlgLogon;
	}

	//创建窗口
	if (m_pDlgLogon->m_hWnd==NULL)
	{
		m_pDlgLogon->Create(IDD_DLG_LOGON,GetDesktopWindow());
	}

	//显示窗口
	if (GetActiveStatus()==false)
	{
		m_pDlgLogon->ShowWindow(SW_SHOW);
		m_pDlgLogon->SetForegroundWindow();
	}

// 	//隐藏窗口
// 	if ((m_pDlgRegister!=NULL)&&(m_pDlgRegister->m_hWnd!=NULL))
// 	{
// 		m_pDlgRegister->ShowWindow(SW_HIDE);
// 	}

	return;
}

//显示注册
VOID CMissionLogon::ShowRegister()
{
	//效验状态
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return;

	//设置变量
	m_bRegister=true;

// 	//创建窗口
// 	if (m_pDlgRegister==NULL)
// 	{
// 		m_pDlgRegister=new CDlgRegister;
// 	}
// 	
// 	//创建窗口
// 	if (m_pDlgRegister->m_hWnd==NULL)
// 	{
// 		m_pDlgRegister->Create(IDD_DLG_REGISTER,GetDesktopWindow());
// 	}
// 
// 	//显示窗口
// 	m_pDlgRegister->ShowWindow(SW_SHOW);
// 	m_pDlgRegister->SetForegroundWindow();

	//隐藏窗口
	if ((m_pDlgLogon!=NULL)&&(m_pDlgLogon->m_hWnd!=NULL))
	{
		m_pDlgLogon->ShowWindow(SW_HIDE);
	}

	return;
}

//记录用户
VOID CMissionLogon::UpdateUserInfo()
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置标志
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	if (pParameterGlobal!=NULL) pParameterGlobal->m_bRemberPassword=m_bRemPassword;

	//构造信息
	TCHAR szUserID[16];
	_sntprintf(szUserID,CountArray(szUserID),TEXT("%ld"),pGlobalUserData->dwUserID);

	//上次用户
	CWHRegKey RegUserInfo;
	RegUserInfo.OpenRegKey(REG_USER_INFO,true);
	RegUserInfo.WriteValue(TEXT("LastUserID"),pGlobalUserData->dwUserID);

	//变量定义
	TCHAR szGameID[16]=TEXT("");
	TCHAR szPassBuffer[MAX_ENCRYPT_LEN]=TEXT("");

	//用户表项
	CWHRegKey RegUserItem;
	RegUserItem.Attach(RegUserInfo.CreateItemKey(szUserID));

	//保存密码
	if (m_bRemPassword==true)
	{
		if (m_bRegister==false)
		{
			LPCTSTR pszPassword=m_pDlgLogon->m_szPassword;
			CWHEncrypt::XorEncrypt(pszPassword,szPassBuffer,CountArray(szPassBuffer));
		}
// 		else
// 		{
// 			LPCTSTR pszPassword=m_pDlgRegister->m_szLogonPass;
// 			CWHEncrypt::XorEncrypt(pszPassword,szPassBuffer,CountArray(szPassBuffer));
// 		}
	}

	//辅助信息
	RegUserItem.WriteValue(TEXT("FaceID"),pGlobalUserData->wFaceID);
	RegUserItem.WriteValue(TEXT("AcitveTimeToken"),(DWORD)time(NULL));

	//帐号信息
	RegUserItem.WriteString(TEXT("GameID"),szGameID);
	RegUserItem.WriteString(TEXT("UserPassword"),szPassBuffer);
	RegUserItem.WriteString(TEXT("UserAccount"),pGlobalUserData->szAccounts);
	RegUserItem.WriteString(TEXT("UserPassToken"),pGlobalUserData->szPassword);

	return;
}
bool CMissionLogon::CreatePlazaview()
{
	CPlatformFrame* pPlatformFrame = CPlatformFrame::GetInstance();
	return pPlatformFrame->CreatePlazaview();

}
//执行登录
bool CMissionLogon::PerformKefu()
{
	//效验状态
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true)
	{
		ASSERT(GetMissionManager()!=NULL);
		GetMissionManager()->SendData(MDM_GP_USER_SERVICE,SUB_GP_KEFU_URL);

		return false;
	}

	//获取地址
	TCHAR szCustomServer[LEN_SERVER]=TEXT("");
	if (m_pDlgLogon!=NULL) lstrcpyn(szCustomServer,m_pDlgLogon->m_szLogonServer,CountArray(szCustomServer));

	m_bKefu = true;
	//设置地址
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->SetCustomServer(szCustomServer);

	//发起连接
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{

		return false;
	}

	return true;
}

//执行登录
bool CMissionLogon::PerformLogonMission(bool bRemPassword)
{
	//效验状态
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true)
	{
		//变量定义
		WORD wPacketSize,wSubCmdID;
		BYTE cbBuffer[SOCKET_TCP_PACKET];

		//构造数据
		if (m_bRegister==false)
		{
			wSubCmdID=SUB_GP_LOGON_ACCOUNTS;
			wPacketSize=m_pDlgLogon->ConstructLogonPacket(cbBuffer,sizeof(cbBuffer));
		}
		// 		else
		// 		{
		// 			wSubCmdID=SUB_GP_REGISTER_ACCOUNTS;
		// 			wPacketSize=m_pDlgRegister->ConstructRegisterPacket(cbBuffer,sizeof(cbBuffer));
		// 		}

		//提示信息
		ShowStatusWindow(TEXT("正在验证用户登录信息..."));

		//发送数据
		ASSERT(GetMissionManager()!=NULL);
		GetMissionManager()->SendData(MDM_GP_LOGON,wSubCmdID,cbBuffer,wPacketSize);

		return false;
	}

	m_bKefu = false;
	//获取地址
	TCHAR szCustomServer[LEN_SERVER]=TEXT("");
	if (m_pDlgLogon!=NULL) lstrcpyn(szCustomServer,m_pDlgLogon->m_szLogonServer,CountArray(szCustomServer));

	//设置地址
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->SetCustomServer(szCustomServer);


	CPlatformFrame* pPlatformFrame = CPlatformFrame::GetInstance();
	pPlatformFrame->AvtiveMainMissionItem();

	SetTimer(IDI_LOGON_TIME,3*1000,NULL);
	//发起连接
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{
		//重新登录
		if (m_bRegister==false)
		{
			ShowLogon();
		}
		else
		{
			ShowRegister();
		}

		return false;
	}

	//设置变量
	m_bRemPassword=bRemPassword;

	///EWIN网络删除
	//写入地址
	//CGlobalServer GlobalServer;
	//GlobalServer.SetLastServerName(szCustomServer);

	//显示进度
	ShowStatusWindow(TEXT("正在连接服务器，请耐心稍候片刻..."));

	return true;
}

//登录成功
bool CMissionLogon::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonSuccess));
	if (wDataSize<sizeof(CMD_GP_LogonSuccess)) return false;

	//消息处理
	CMD_GP_LogonSuccess * pLogonSuccess=(CMD_GP_LogonSuccess *)pData;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//保存信息
	pGlobalUserData->wFaceID=pLogonSuccess->wFaceID;
	pGlobalUserData->cbGender=pLogonSuccess->cbGender;
	pGlobalUserData->dwUserID=pLogonSuccess->dwUserID;
	pGlobalUserData->dwExperience=pLogonSuccess->dwExperience;
	pGlobalUserData->cbMoorMachine=pLogonSuccess->cbMoorMachine;

	//用户积分
	pGlobalUserData->lScore=pLogonSuccess->lScore;
	pGlobalUserData->lInsure=pLogonSuccess->lInsure;
	pGlobalUserData->lUserMedal=pLogonSuccess->lUserMedal;


	//显示配置
	pParameterGlobal->m_bShowServerStatus=(pLogonSuccess->cbShowServerStatus!=0);

	//帐号信息
	lstrcpyn(pGlobalUserData->szAccounts,pLogonSuccess->szAccounts,CountArray(pGlobalUserData->szAccounts));
	lstrcpyn(pGlobalUserData->szNickName,pLogonSuccess->szNickName,CountArray(pGlobalUserData->szNickName));

	//扩展信息
	lstrcpyn(pGlobalUserData->szAddrDescribe,pLogonSuccess->szAddrDescribe,CountArray(pGlobalUserData->szAddrDescribe));


	theAccount.Scoretype = MoShi_Yuan;
	theAccount.fandian = pLogonSuccess->f_fandian;
 	theAccount.user_id = pLogonSuccess->dwUserID;
	theAccount.cbgender = pLogonSuccess->cbGender;
 	lstrcpyn(theAccount.account,pGlobalUserData->szAccounts,CountArray(theAccount.account));

 	theAccount.yue = pLogonSuccess->f_yue;
	theAccount.dongjie = pLogonSuccess->f_dongjie;
	theAccount.type = pLogonSuccess->n_type;
	theAccount.moormachine = pLogonSuccess->cbMoorMachine;

	//扩展信息
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pLogonSuccess+1,wDataSize-sizeof(CMD_GP_LogonSuccess));
	while (true)
	{
		//提取数据
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		//提取数据
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_MEMBER_INFO:	//会员信息
			{
				ASSERT(DataDescribe.wDataSize==sizeof(DTP_GP_MemberInfo));
				if (DataDescribe.wDataSize==sizeof(DTP_GP_MemberInfo))
				{
					DTP_GP_MemberInfo * pMemberInfo=(DTP_GP_MemberInfo *)pDataBuffer;
					pGlobalUserData->cbMemberOrder=pMemberInfo->cbMemberOrder;
					pGlobalUserData->MemberOverDate=pMemberInfo->MemberOverDate;
				}

				break;
			}
		}
	}

	//设置提示
	ShowStatusWindow(TEXT("正在获取游戏列表..."));

	return true;
}

//登录失败
bool CMissionLogon::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GP_LogonFailure * pLogonFailure=(CMD_GP_LogonFailure *)pData;

	//效验数据
	ASSERT(wDataSize>=(sizeof(CMD_GP_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GP_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//关闭处理
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

	//进度界面
	HideStatusWindow();

	//显示消息
	LPCTSTR pszDescribeString(pLogonFailure->szDescribeString);
	if (lstrlen(pszDescribeString)>0) ShowInformation(pszDescribeString,MB_ICONERROR,60);

	//重新登录
	if (m_bRegister==false)
	{
		ShowLogon();
	}
	else
	{
		ShowRegister();
	}

	return true;
}
//登录完成
bool CMissionLogon::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GP_LogonFinish));
	if (wDataSize!=sizeof(CMD_GP_LogonFinish)) return false;

	//变量定义
	CMD_GP_LogonFinish * pLogonFinish=(CMD_GP_LogonFinish *)pData;

	//变量定义
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//时间配置
	pParameterGlobal->m_wIntermitTime=pLogonFinish->wIntermitTime;
	pParameterGlobal->m_wOnLineCountTime=pLogonFinish->wOnLineCountTime;

	//更新信息
	UpdateUserInfo();

	//关闭窗口
	HideStatusWindow();

	//删除控件
	SafeDelete(m_pDlgLogon);
	//SafeDelete(m_pDlgRegister);

	//终止任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL)
	{
		pMissionManager->ConcludeMissionItem(this,false);
		//pMissionManager->DeleteMissionItem(this);
	}
	//发送事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGON,0L);



	return true;
}

//升级提示
bool CMissionLogon::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GP_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GP_UpdateNotify)) return false;

	//设置变量
	CMD_GP_UpdateNotify * pUpdateNotify=(CMD_GP_UpdateNotify *)pData;

	//终止任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

	//隐藏窗口
	HideStatusWindow();

	//提示消息
	if ((pUpdateNotify->cbMustUpdate==TRUE)||(pUpdateNotify->cbAdviceUpdate==TRUE))
	{
		//构造提示
		if (pUpdateNotify->cbMustUpdate==FALSE)
		{
			//构造提示
			LPCTSTR pszString=TEXT("游戏大厅已升级，您现在的版本还能继续使用，建议您立即下载更新！");

			//退出判断
			CInformation Information(CPlatformFrame::GetInstance());
			if (Information.ShowMessageBox(pszString,MB_ICONINFORMATION|MB_YESNO,0)!=IDYES) return true;
		}
	}

	//下载大厅
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("游戏大厅"),0,0);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
