#include "Stdafx.h"
#include "Resource.h"
#include "GameServerDlg.h"
//////////////////////////////////////////////////////////////////////////////////
#include "Hardwareinfo.h"

//消息定义
#define WM_PROCESS_CMD_LINE			(WM_USER+100)						//处理命令

//////////////////////////////////////////////////////////////////////////////////

#define	WLOG	OutputDebugString



//输出16进制字符
CString HexShow(BYTE* sStr, int iLen, int iFlag)
{
	CString strHex;
	if(iLen <= 0)
		return strHex;
	register int iCount;
	for (iCount = 0; iCount < iLen; iCount++)
	{
		if (iCount % 36 == 0) 
			WLOG(L"\n");
		if (iFlag && sStr[iCount] > 0x1f) 
			TRACE(L"%2C ", sStr[iCount]);
		else 
		{
			CString strTemp;
			strTemp.Format(L"%.2X", sStr[iCount]);
			strHex += strTemp;
			TRACE(L"%.2X ", sStr[iCount]);
		}
	}
	if ((iCount - 1) % 36) 
		WLOG(L"\n");
	WLOG(L"\n");
	return strHex;
}

//简单加密
void	SimpleDecode(byte* szIn, int nLen)
{
	for(int n=0; n<nLen; n++)
	{
		szIn[n] ^= 0xFF;
		szIn[n] ^= 0xAC;
	}
}

//生成机器码
CString	GetMachineCode()
{
	BOOL	bRet = FALSE;
	BYTE	szSystemInfo[1024];
	UINT	uLen=0;
	memset(szSystemInfo, 0x00, sizeof(szSystemInfo));
	bRet = GetMacAddr(szSystemInfo, uLen);
	
	//cpuid
	GetCpuID(szSystemInfo, 12);

#ifdef	_DEBUG
	WLOG(HexShow(szSystemInfo, 10, 0));
#endif
	SimpleDecode(szSystemInfo, 10);
	CString strLocalCode = HexShow(szSystemInfo, 10, 0);
	memset(szSystemInfo, 0x00, sizeof(szSystemInfo));
	bRet = GetHdiskSerial(szSystemInfo, uLen);


#ifdef	_DEBUG
	WLOG(HexShow(szSystemInfo, 32, 0));
#endif	
//	SimpleDecode(szSystemInfo, 32);
	strLocalCode += (char*)szSystemInfo;
	strLocalCode.Replace(L" ", L"");
	strLocalCode.Replace(L"-", L"");
#ifdef _DEBUG
	WLOG(strLocalCode);
#endif
	for(int n=0; n<strLocalCode.GetLength(); n++)
	{
		if(n%6 == 0 && n!= 0)
			strLocalCode.Insert(n, L"-");
	}
#ifdef _DEBUG
	WLOG(L"\n");
	WLOG(strLocalCode);
#endif
	strLocalCode = strLocalCode.Left(48);
	return strLocalCode;
}

//校验License.
//返回值：0	成功
//1 文件不存在；2 文件已过期；3 文件格式不正确。
int		checkLicense(CString strInputCode, CString& strExpireDate)
{
	CString strLicenseFile = CINIFile::GetAppPath()+L"License.dat";
	if(!CINIFile::IsFileExist(strLicenseFile))
		return 1;
	CINIFile	iniFile(strLicenseFile);
	strExpireDate = iniFile.GetKeyVal(L"LICENSE", L"EXPRIREDATE",L"20090101");
	CString strLicenseCode= iniFile.GetKeyVal(L"LICENSE", L"LicenseCode",L"0123456789");
	if(strExpireDate.IsEmpty() || strExpireDate.GetLength() < 8)
		return 3;
	CTime tmExpire = CTime(_wtoi(strExpireDate.Left(4)), _wtoi(strExpireDate.Mid(4, 2)), _wtoi(strExpireDate.Right(2)), \
		0, 0, 0);
	CTime tmNow = CTime::GetCurrentTime();

	bool bIsValid = (tmExpire - tmNow) >= 0;
	if( !bIsValid )
		return 2;

	USES_CONVERSION;
	CString strInputLicense = strInputCode.GetBuffer(1);
	strInputLicense.Replace(L"-", L"");
	strInputLicense.Insert(5,strExpireDate.Left(2));
	strInputLicense.Insert(10, strExpireDate.Mid(2, 2));
	strInputLicense.Insert(15, strExpireDate.Mid(4, 2));
	strInputLicense.Insert(20, strExpireDate.Right(2));

	TCHAR szMD5Result[33];
	memset(szMD5Result, 0x00, sizeof(szMD5Result));
	CMD5Encrypt::EncryptData((LPCTSTR)strInputLicense , szMD5Result);
	
	CString strUpperCode = HexShow((BYTE*)W2A(szMD5Result), _tcsclen(szMD5Result), 0);
	strUpperCode.MakeUpper();
	if(strLicenseCode.MakeUpper().Compare(strUpperCode) == 0)
		return 0;
	else
		return 2;
}

bool	CreateLicenseFile(TCHAR* szFileName, TCHAR* szMachineCode, TCHAR* szExpireCode)
{
	CFile licenseFile;
	if(licenseFile.Open(szFileName,CFile::modeReadWrite|CFile::modeCreate))
	{
		CString strLicenseCode = szMachineCode;
		strLicenseCode.Replace(L"-", L"");
		CString strExpireCode = szExpireCode;
		strLicenseCode.Insert(5,  strExpireCode.Left(2));
		strLicenseCode.Insert(10, strExpireCode.Mid(2, 2));
		strLicenseCode.Insert(15, strExpireCode.Mid(4, 2));
		strLicenseCode.Insert(20, strExpireCode.Right(2));

		USES_CONVERSION;
		TCHAR szMD5Result[33];
		memset(szMD5Result, 0x00, sizeof(szMD5Result));
		CMD5Encrypt::EncryptData((LPCTSTR)strLicenseCode, szMD5Result);
		licenseFile.Write("[LICENSE]\n", strlen("[LICENSE]\n"));
		licenseFile.Write("VERSION=2.0\nPROVIDER=Typingsoft\n", strlen("VERSION=2.0\nPROVIDER=Typingsoft\n"));
		licenseFile.Write("EXPRIREDATE=", strlen("EXPRIREDATE="));
		licenseFile.Write(W2A(szExpireCode), strlen(W2A(szExpireCode)));
		licenseFile.Write("\n", strlen("\n"));
		licenseFile.Write("LicenseCode=", strlen("LicenseCode="));

		CString strMd5Code = HexShow((BYTE*)W2A(szMD5Result), _tcsclen(szMD5Result), 0);
		strMd5Code.MakeUpper();
		licenseFile.Write(W2A(strMd5Code.GetBuffer(1)), strMd5Code.GetLength());
		licenseFile.Write("\n", strlen("\n"));
		licenseFile.Close();
	}
	return true;
}

const CString		GetLicenseDesc(int nCode)
{
	if(nCode == 1)
		return TEXT("文件不存在，请检查名为License.dat的文件，并确认该文件在服务器程序的路径中！\n机器码为：");
	else if(nCode == 2)
		return TEXT("License.dat文件已过期，请联系开发商提供最新的License。机器码为：");
	else if(nCode == 3)
		return TEXT("文件格式不正确，请检查License.dat文件。机器码为：");
	else
		return TEXT("License校验成功！");
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



BEGIN_MESSAGE_MAP(CGameServerDlg, CDialog)

	//系统消息
	ON_WM_QUERYENDSESSION()
	ON_BN_CLICKED(IDC_OPEN_SERVER, OnBnClickedOpenServer)
	ON_BN_CLICKED(IDC_STOP_SERVICE, OnBnClickedStopService)
	ON_BN_CLICKED(IDC_START_SERVICE, OnBnClickedStartService)
	ON_BN_CLICKED(IDC_CREATE_SERVER, OnBnClickedCreateServer)
	ON_BN_CLICKED(IDC_OPTION_SERVER, OnBnClickedOptionServer)

	//自定消息
	ON_MESSAGE(WM_PROCESS_CMD_LINE,OnMessageProcessCmdLine)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameServerDlg::CGameServerDlg() : CDialog(IDD_DLG_GAME_SERVER)
{
	//配置参数
	m_bAutoControl=false;
	m_bOptionSuccess=false;
	ZeroMemory(&m_ModuleInitParameter,sizeof(m_ModuleInitParameter));

	return;
}

//析构函数
CGameServerDlg::~CGameServerDlg()
{
}

//控件绑定
VOID CGameServerDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACE_MESSAGE, m_TraceServiceControl);
}

//初始化函数
BOOL CGameServerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("游戏服务器 -- [ 停止 ]"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//设置组件
	m_ServiceUnits.SetServiceUnitsSink(this);

	//命令处理
	LPCTSTR pszCmdLine=AfxGetApp()->m_lpCmdLine;
	if (pszCmdLine[0]!=0) PostMessage(WM_PROCESS_CMD_LINE,0,(LPARAM)pszCmdLine);

	return TRUE;
}

//确定消息
VOID CGameServerDlg::OnOK()
{
	return;
}

//取消函数
VOID CGameServerDlg::OnCancel()
{
	//关闭询问
	if (m_ServiceUnits.GetServiceStatus()!=ServiceStatus_Stop)
	{
		LPCTSTR pszQuestion=TEXT("游戏服务器正在运行中，您确实要关闭服务器吗？");
		if (AfxMessageBox(pszQuestion,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION)!=IDYES) return;
	}

	//停止服务
	m_ServiceUnits.ConcludeService();

	__super::OnCancel();
}

//消息解释
BOOL CGameServerDlg::PreTranslateMessage(MSG * pMsg)
{
	//按键过滤
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//服务状态
VOID CGameServerDlg::OnServiceUnitsStatus(enServiceStatus ServiceStatus)
{
	//状态设置
	switch (ServiceStatus)
	{
	case ServiceStatus_Stop:	//停止状态
		{
			//更新标题
			UpdateServerTitle(ServiceStatus);

			//服务按钮
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(FALSE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(TRUE);

			//配置按钮
			GetDlgItem(IDC_OPEN_MATCH)->EnableWindow(TRUE);
			GetDlgItem(IDC_OPEN_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_CREATE_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_OPTION_SERVER)->EnableWindow(TRUE);

			//运行按钮
			GetDlgItem(IDC_RUN_PARAMETER)->EnableWindow(FALSE);
			GetDlgItem(IDC_SERVICE_CONTROL)->EnableWindow(FALSE);

			//提示信息
			LPCTSTR pszDescribe=TEXT("服务停止成功");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	case ServiceStatus_Config:	//配置状态
		{
			//更新标题
			UpdateServerTitle(ServiceStatus);

			//设置按钮
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);

			//配置按钮
			GetDlgItem(IDC_OPEN_MATCH)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPEN_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_CREATE_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPTION_SERVER)->EnableWindow(FALSE);

			//运行按钮
			GetDlgItem(IDC_RUN_PARAMETER)->EnableWindow(FALSE);
			GetDlgItem(IDC_SERVICE_CONTROL)->EnableWindow(FALSE);

			//提示信息
			LPCTSTR pszDescribe=TEXT("正在初始化组件...");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	case ServiceStatus_Service:	//服务状态
		{
			//更新标题
			UpdateServerTitle(ServiceStatus);

			//服务按钮
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);

			//配置按钮
			GetDlgItem(IDC_OPEN_MATCH)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPEN_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_CREATE_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPTION_SERVER)->EnableWindow(FALSE);

			//运行按钮
			GetDlgItem(IDC_RUN_PARAMETER)->EnableWindow(TRUE);
			GetDlgItem(IDC_SERVICE_CONTROL)->EnableWindow(TRUE);

			//提示信息
			LPCTSTR pszDescribe=TEXT("服务启动成功");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	}

	return;
}

//更新图标
VOID CGameServerDlg::UpdateServerLogo(LPCTSTR pszServerDLL)
{
	//加载资源
	HINSTANCE hInstance=AfxLoadLibrary(pszServerDLL);

	//加载图形
	if (hInstance!=NULL)
	{
		//设置资源
		AfxSetResourceHandle(hInstance);

		//设置资源
		CStatic * pServerLogo=(CStatic *)GetDlgItem(IDC_SERVER_LOGO);
		pServerLogo->SetIcon(::LoadIcon(hInstance,TEXT("SERVER_ICON")));

		//释放资源
		AfxFreeLibrary(hInstance);
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}

	return;
}

//更新标题
VOID CGameServerDlg::UpdateServerTitle(enServiceStatus ServiceStatus)
{
	//变量定义
	LPCTSTR pszStatusName=NULL;
	LPCTSTR pszServerName=NULL;

	//状态字符
	switch (ServiceStatus)
	{
	case ServiceStatus_Stop:	//停止状态
		{
			pszStatusName=TEXT("停止");
			break;
		}
	case ServiceStatus_Config:	//配置状态
		{
			pszStatusName=TEXT("初始化");
			break;
		}
	case ServiceStatus_Service:	//运行状态
		{
			pszStatusName=TEXT("运行");
			break;
		}
	}

	//房间名字
	if (m_bOptionSuccess==false) pszServerName=TEXT("游戏服务器");
	else pszServerName=m_ModuleInitParameter.GameServiceOption.szServerName;

	//构造标题
	TCHAR szTitle[128]=TEXT("");
	_sntprintf(szTitle,CountArray(szTitle),TEXT("[ %s ] -- [ %s ]"),pszServerName,pszStatusName);

	//设置标题
	SetWindowText(szTitle);

	return;
}

//更新状态
VOID CGameServerDlg::UpdateParameterStatus(tagModuleInitParameter & ModuleInitParameter)
{
	//检查License
	CString strMcode = GetMachineCode();
	CString strExpireDate;
	int nCode = checkLicense(strMcode, strExpireDate);
	if ( nCode != 0)
	{
		CTraceService::TraceString(GetLicenseDesc(nCode), TraceLevel_Warning);
		CString strMCode = GetMachineCode();
		CTraceService::TraceString(strMCode, TraceLevel_Warning);
		return;
	}

	//设置变量
	m_bOptionSuccess=true;
	m_ModuleInitParameter=ModuleInitParameter;

	//更新标题
	UpdateServerTitle(ServiceStatus_Stop);
	UpdateServerLogo(ModuleInitParameter.GameServiceAttrib.szServerDLLName);

	//设置按钮
	GetDlgItem(IDC_START_SERVICE)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPTION_SERVER)->EnableWindow(TRUE);

	//设置控件
	SetDlgItemText(IDC_GAME_NAME,m_ModuleInitParameter.GameServiceAttrib.szGameName);
	SetDlgItemText(IDC_SERVER_NAME,m_ModuleInitParameter.GameServiceOption.szServerName);

	//监听端口
	if (m_ModuleInitParameter.GameServiceOption.wServerPort==0)
	{
		SetDlgItemText(IDC_SERVER_PORT,TEXT("自动适配"));
	}
	else
	{
		SetDlgItemInt(IDC_SERVER_PORT,m_ModuleInitParameter.GameServiceOption.wServerPort);
	}

	//设置模块
	LPCTSTR pszServerDLLName=m_ModuleInitParameter.GameServiceAttrib.szServerDLLName;
	m_ServiceUnits.CollocateService(pszServerDLLName,m_ModuleInitParameter.GameServiceOption);

	//构造提示
	TCHAR szString[256]=TEXT("");
	LPCTSTR pszServerName=m_ModuleInitParameter.GameServiceOption.szServerName;
	_sntprintf(szString,CountArray(szString),TEXT("[ %s ] 房间参数加载成功"),pszServerName);

	//输出信息
	CTraceService::TraceString(szString,TraceLevel_Normal);

	return;
}

//启动房间
bool CGameServerDlg::StartServerService(WORD wServerID)
{
	//变量定义
	tagDataBaseParameter DataBaseParameter;
	ZeroMemory(&DataBaseParameter,sizeof(DataBaseParameter));

	//设置参数
	InitDataBaseParameter(DataBaseParameter);
	m_ModuleDBParameter.SetPlatformDBParameter(DataBaseParameter);

		//检查License
	CString strMcode = GetMachineCode();
	CString strExpireDate;
	int nCode = checkLicense(strMcode, strExpireDate);
	if ( nCode != 0)
	{
		CTraceService::TraceString(GetLicenseDesc(nCode), TraceLevel_Warning);
		CString strMCode = GetMachineCode();
		CTraceService::TraceString(strMCode, TraceLevel_Warning);
		return false;
	}


	//读取配置
	CDlgServerItem DlgServerItem;
	if (DlgServerItem.OpenGameServer(wServerID)==false)
	{
		CTraceService::TraceString(TEXT("房间配置参数不存在或者加载失败"),TraceLevel_Exception);
		return false;
	}

	//更新状态
	UpdateParameterStatus(DlgServerItem.m_ModuleInitParameter);

	//启动服务
	try
	{
		m_ServiceUnits.StartService();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return true;
}

//获取连接
bool CGameServerDlg::InitDataBaseParameter(tagDataBaseParameter & DataBaseParameter)
{
	//获取路径
	TCHAR szWorkDir[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));

	//构造路径
	TCHAR szIniFile[MAX_PATH]=TEXT("");
	_sntprintf(szIniFile,CountArray(szIniFile),TEXT("%s\\ServerParameter.ini"),szWorkDir);

	//读取配置
	CWHIniData IniData;
	IniData.SetIniFilePath(szIniFile);
	//检查License
	CString strMcode = GetMachineCode();
	CString strExpireDate;
	int nCode = checkLicense(strMcode, strExpireDate);
	if ( nCode != 0)
	{
		CTraceService::TraceString(GetLicenseDesc(nCode), TraceLevel_Warning);
		CString strMCode = GetMachineCode();
		CTraceService::TraceString(strMCode, TraceLevel_Warning);
		return false;
	}

	//连接信息
	DataBaseParameter.wDataBasePort=(WORD)IniData.ReadInt(TEXT("PlatformDB"),TEXT("DBPort"),1433);
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBAddr"),NULL,DataBaseParameter.szDataBaseAddr,CountArray(DataBaseParameter.szDataBaseAddr));
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBUser"),NULL,DataBaseParameter.szDataBaseUser,CountArray(DataBaseParameter.szDataBaseUser));
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBPass"),NULL,DataBaseParameter.szDataBasePass,CountArray(DataBaseParameter.szDataBasePass));
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBName"),szPlatformDB,DataBaseParameter.szDataBaseName,CountArray(DataBaseParameter.szDataBaseName));

	return true;
}

//启动服务
VOID CGameServerDlg::OnBnClickedStartService()
{
	//启动服务
	try
	{
		m_ServiceUnits.StartService();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return;
}

//停止服务
VOID CGameServerDlg::OnBnClickedStopService()
{
	//停止服务
	try
	{
		m_ServiceUnits.ConcludeService();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return;
}

//打开房间
VOID CGameServerDlg::OnBnClickedOpenServer()
{
	//变量定义
	tagDataBaseParameter DataBaseParameter;
	ZeroMemory(&DataBaseParameter,sizeof(DataBaseParameter));

	//设置参数
	InitDataBaseParameter(DataBaseParameter);
	m_ModuleDBParameter.SetPlatformDBParameter(DataBaseParameter);
	
	//检查License
	CString strMcode = GetMachineCode();
	CString strExpireDate;
	int nCode = checkLicense(strMcode, strExpireDate);
	if ( nCode != 0)
	{
		CTraceService::TraceString(GetLicenseDesc(nCode), TraceLevel_Warning);
		CString strMCode = GetMachineCode();
		CTraceService::TraceString(strMCode, TraceLevel_Warning);
		return;
	}

	//配置房间
	CDlgServerItem DlgServerItem;
	if (DlgServerItem.OpenGameServer()==false) return;

	//更新状态
	UpdateParameterStatus(DlgServerItem.m_ModuleInitParameter);

	return;
}

//创建房间
VOID CGameServerDlg::OnBnClickedCreateServer()
{
	//变量定义
	tagDataBaseParameter DataBaseParameter;
	ZeroMemory(&DataBaseParameter,sizeof(DataBaseParameter));

	//设置参数
	InitDataBaseParameter(DataBaseParameter);
	m_ModuleDBParameter.SetPlatformDBParameter(DataBaseParameter);

	//创建房间
	CDlgServerWizard DlgServerWizard;
	if (DlgServerWizard.CreateGameServer()==false) return;

	//更新状态
	UpdateParameterStatus(DlgServerWizard.m_ModuleInitParameter);

	return;
}

//配置房间
VOID CGameServerDlg::OnBnClickedOptionServer()
{
	//游戏模块
	CGameServiceManagerHelper GameServiceManager;
	GameServiceManager.SetModuleCreateInfo(m_ModuleInitParameter.GameServiceAttrib.szServerDLLName,GAME_SERVICE_CREATE_NAME);

	//加载模块
	if (GameServiceManager.CreateInstance()==false)
	{
		AfxMessageBox(TEXT("服务组件不存在或者加载失败，配置房间失败"),MB_ICONERROR);
		return;
	}

	//配置房间
	CDlgServerWizard DlgServerWizard;
	DlgServerWizard.SetWizardParameter(GameServiceManager.GetInterface(),&m_ModuleInitParameter.GameServiceOption);

	//创建房间
	if (DlgServerWizard.CreateGameServer()==false) return;

	//更新状态
	UpdateParameterStatus(DlgServerWizard.m_ModuleInitParameter);

	return;
}

//关闭询问
BOOL CGameServerDlg::OnQueryEndSession()
{
	//提示消息
	if (m_ServiceUnits.GetServiceStatus()!=ServiceStatus_Stop)
	{
		CTraceService::TraceString(TEXT("服务正在运行中，系统要求注销回话请求失败"),TraceLevel_Warning);
		return FALSE;
	}

	return TRUE;
}

//命令处理
LRESULT CGameServerDlg::OnMessageProcessCmdLine(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	CWHCommandLine CommandLine;
	LPCTSTR pszCommandLine=(LPCTSTR)(lParam);

	//房间标识
	TCHAR szSrverID[32]=TEXT("");
	if (CommandLine.SearchCommandItem(pszCommandLine,TEXT("/ServerID:"),szSrverID,CountArray(szSrverID))==true)
	{
		//获取房间
		WORD wServerID=(WORD)(_tstol(szSrverID));

		//启动房间
		if (wServerID!=0)
		{
			//设置变量
			m_bAutoControl=true;

			//启动房间
			StartServerService(wServerID);
		}
	}

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
