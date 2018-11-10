#include "Stdafx.h"
#include "DlgLogon.h"
#include "GamePlaza.h"
#include "MissionLogon.h"
#include ".\dlglogon.h"
#include "Ping.h"
#include "MD5Checksum\MD5Checksum.h"
#import <msxml3.dll>
using namespace MSXML2;

#define IDC_LOGON_LOGO 1112615
//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					7									//圆角宽度
#define ROUND_CY					7									//圆角高度

//屏幕位置
#define LAYERED_SIZE				5									//分层大小
#define CAPTION_SIZE				135									//标题大小

//按钮数目
#define BUTTON_COUNT_CX				5									//横向数目
#define BUTTON_COUNT_CY				4									//竖向数目
#define BUTTON_ITEM_UNDER			0									//按钮底部
#define BUTTON_ITEM_START			8									//按钮开始
#define BUTTON_ITEM_SPACE			30									//按钮间距

//控件标识
#define IDC_WEB_PUBLICIZE			300									//浏览控件
#define IDC_WND_LOGON_BUTTON		301									//按钮控件

//按钮标识
#define INDEX_REGISTER				0									//注册按钮
#define INDEX_USER_HELP				1									//新手帮助
#define INDEX_BUY_SCORE				2									//立即充值
#define INDEX_HOME_PAGE				3									//官方网页

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgLogon, CDialog)

	//系统消息
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_PAINT()

	//按钮消息
	ON_BN_CLICKED(IDC_REGISTER, OnBnClickedRegister)
	//ON_CBN_EDITCHANGE(IDC_ACCOUNTS, OnCbnEditchangeAccounts)
	ON_BN_CLICKED(IDC_FORGET_PASS, OnBnClickedForget)
	ON_BN_CLICKED(IDC_KEFU, OnBnClickedKeFu)
	ON_BN_CLICKED(IDC_RUBBISH, OnBnClickedDelete)
	ON_CBN_SELCHANGE(IDC_ACCOUNTS, OnSelchangeAccounts)

	//ON_EN_CHANGE(IDC_ACCOUNTS, OnEnChangeAccounts)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// BEGIN_MESSAGE_MAP(CWndLogonButton, CWnd)
// 	ON_WM_PAINT()
// 	ON_WM_CREATE()
// 	ON_WM_SETCURSOR()
// 	ON_WM_MOUSEMOVE()
// 	ON_WM_LBUTTONUP()
// 	ON_WM_LBUTTONDOWN()
// 	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
// END_MESSAGE_MAP()
// 
//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////

//构造函数
CControlCheckButton::CControlCheckButton()
{
	//设置变量
	m_cbChecked=FALSE;
	m_pParentSink=NULL;

	//加载资源
	//m_ImageBtnBack.LoadFromResource(AfxGetResourceHandle(),IDB_CHECK_BUTTON_BACK);
	//m_ImageBtnBack.LoadImage(AfxGetResourceHandle(),TEXT("BT_CHECK_BUTTON_BACK"));
}

//析构函数
CControlCheckButton::~CControlCheckButton()
{
}

//控件区域
CRect CControlCheckButton::GetControlRect()
{
	CRect rcClient(m_ptControlBenchmark.x,m_ptControlBenchmark.y,m_ptControlBenchmark.x+m_ImageBtnBack.GetWidth()/2,m_ptControlBenchmark.y+m_ImageBtnBack.GetHeight());

	return rcClient;
}

//设置状态
VOID  CControlCheckButton::SetButtonChecked(BYTE cbChecked) 
{ 
	//设置变量
	m_cbChecked=cbChecked;

	//定义变量
	CRect rcClient = GetControlRect();

	//重绘界面
	if(m_pParentSink!=NULL) m_pParentSink->InvalidateRect(&rcClient);
};
VOID  CControlCheckButton::LoadCheckImage(UINT nIDResource) 
{
	//m_ImageBtnBack.LoadImage(AfxGetResourceHandle(),nIDResource);
}
VOID  CControlCheckButton::LoadCheckImage(LPCTSTR pszResourceName) 
{
	m_ImageBtnBack.LoadImage(AfxGetResourceHandle(),pszResourceName);
}
//绘制控件
VOID  CControlCheckButton::OnDrawControl(CDC * pDC)
{
	//参数校验
	if(pDC==NULL) return;

	//定义变量
	INT nImageWidth = m_ImageBtnBack.GetWidth()/2;
	INT nImageHeight = m_ImageBtnBack.GetHeight();
    
    //绘制背景
	//m_ImageBtnBack.BitBlt(*pDC,m_ptControlBenchmark.x,m_ptControlBenchmark.y,nImageWidth,nImageHeight,m_cbChecked==TRUE?nImageWidth:0,0);
	m_ImageBtnBack.DrawImage(pDC,m_ptControlBenchmark.x,m_ptControlBenchmark.y,nImageWidth,nImageHeight,m_cbChecked==TRUE?nImageWidth:0,0);
}

//点击事件
VOID  CControlCheckButton::OnClickControl(CPoint Point)
{
	//定义变量
	CRect rcClient = GetControlRect();

// 	CString strLog;
// 	strLog.Format(L"x = %d,y = %d,cx = %d,cy = %d,px = %d,py = %d",rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height(),Point.x,Point.y);
// 	AfxMessageBox(strLog);
	//选中判断
	if(rcClient.PtInRect(Point))
	{
		//设置变量
		m_cbChecked=m_cbChecked==FALSE?TRUE:FALSE;

		//重绘界面
		if(m_pParentSink!=NULL)	m_pParentSink->InvalidateRect(&rcClient);
	}
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgLogon::CDlgLogon() : CDialog(IDD_DLG_LOGON)
{
	//登录信息
	m_szAccounts[0]=0;
	m_szPassword[0]=0;
	m_szLogonServer[0]=0;
	m_bCreatePlazaview = true;
	//创建画刷
	m_brBrush.CreateSolidBrush(RGB(215,223,228));

	return;
}

//析构函数
CDlgLogon::~CDlgLogon()
{
	//销毁资源
	if (m_brBrush.GetSafeHandle()!=NULL)
	{
		m_brBrush.DeleteObject();
	}
	//删除对象
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		SafeDelete(m_AccountsInfoArray[i]);
	}

	return;
}

//控件绑定
VOID CDlgLogon::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//绑定控件
	DDX_Control(pDX, IDOK, m_btEnter);
	DDX_Control(pDX, IDCANCEL, m_btQuit);

	//绑定控件
//	DDX_Control(pDX, IDC_ACCOUNTS, m_edAccounts);
	DDX_Control(pDX, IDC_ACCOUNTS, m_AccountsControl);

	DDX_Control(pDX, IDC_PASSWORD, m_PasswordControl);
	DDX_Control(pDX, IDC_LOGON_SERVER, m_ServerControl);
}
//选择改变
VOID CDlgLogon::OnSelchangeAccounts()
{
	//获取选择
	INT nCurrentSel=m_AccountsControl.GetCurSel();
	tagAccountsInfo * pAccountsInfo=(tagAccountsInfo *)m_AccountsControl.GetItemDataPtr(nCurrentSel);

	//设置控件
	if ((nCurrentSel!=LB_ERR)&&(pAccountsInfo!=NULL))
	{
		//用户帐号
		m_AccountsControl.SetCurSel(nCurrentSel);


		//设置密码
		m_PasswordControl.SetUserPassword(pAccountsInfo->szPassword);

		//记住密码
		bool bRemPassword=(pAccountsInfo->szPassword[0]!=0);
		//((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);
		m_RemPwdControl.SetButtonChecked((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);
	}
	else
	{
		//设置密码
		m_PasswordControl.SetUserPassword(TEXT(""));

		//记住密码
		//((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck(BST_UNCHECKED);
		m_RemPwdControl.SetButtonChecked(BST_UNCHECKED);
	}

	return;
}

//创建函数
BOOL CDlgLogon::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("用户登录"));	

	//注册表记录一下当前SERVER
// 	CWHRegKey RegServerAddr;
// 	if(RegServerAddr.OpenRegKey(TEXT("CurrentLogonServer"), false))
// 	{
// 		TCHAR szLogonServer[255];
// 		memset(szLogonServer, 0, sizeof(szLogonServer));
// 		RegServerAddr.GetString(TEXT("CurrentIpAddress"), szLogonServer, CountArray(szLogonServer));
// 		if(_tcslen(szLogonServer) != 0)
// 		{
// 			m_ServerControl.AddString(szLogonServer);
// 		}
// 	}

	m_Font.CreateFont(20,0,0,0,0,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("Arial"));
	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_LOGON_BACK"));

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	CRect rcBtn(0,0,0,0);
	m_btForgetPass.Create(NULL,WS_CHILD|WS_VISIBLE,rcBtn,this,IDC_FORGET_PASS);
	m_btKeFu.Create(NULL,WS_CHILD|WS_VISIBLE,rcBtn,this,IDC_KEFU);
	m_btDeleteRecord.Create(NULL,WS_CHILD|WS_VISIBLE,rcBtn,this,IDC_RUBBISH);
	//边框设置
	m_ServerControl.SetDrawBorad(false);
	m_ServerControl.SetRenderImage(false);
	m_PasswordControl.SetDrawBorad(true);
	m_AccountsControl.SetDrawBorad(false);
	m_AccountsControl.SetRenderImage(false);

	m_PasswordControl.SetEnableColor(RGB(254,254,254),RGB(40,29,27),RGB(40,29,27));
	m_PasswordControl.SetWindowPos(NULL,263,148,188,20,SWP_NOZORDER|SWP_NOREDRAW);
	m_edAccounts.SetWindowPos(NULL,320,100,124,20,SWP_NOZORDER|SWP_NOREDRAW);
	m_PasswordControl.SetFont(&m_Font);
	m_edAccounts.SetEnableColor(RGB(0,0,0),RGB(255,255,255),RGB(255,255,255));
	m_edAccounts.ShowWindow(SW_HIDE);
	//m_edAccounts.SetFont(&m_Font);
	m_AccountsControl.SetBackColor(RGB(73,62,61));
	m_AccountsControl.SetFont(&m_Font);
	if(m_AccountsControl.m_hWnd)m_AccountsControl.MoveWindow(128+22+137,100,180+28,20,SWP_NOZORDER|SWP_NOMOVE);
	m_ServerControl.SetBackColor(RGB(73,62,61));
	m_ServerControl.SetFont(&m_Font);
	if(m_ServerControl.m_hWnd)m_ServerControl.MoveWindow(128+22+137,54,180+28,20,SWP_NOZORDER|SWP_NOMOVE);

	TCHAR szPlatformUrl1[124] = L"";

	//读取配置文件
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	for(int i = 0;i < 19;i++)
	{
		TCHAR szKey[33] = TEXT("");

		_sntprintf(szKey,CountArray(szKey),TEXT("ServerDesc%d"),i);
		TCHAR szDesc[64] ;
		GetPrivateProfileString(TEXT("PlazaUrlDesc"),szKey,TEXT(""),szDesc,CountArray(szDesc),szFileName);
		if(szDesc[0] == NULL)
			break;
		m_ServerControl.AddString(szDesc);

	}
	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btQuit.SetButtonImage(IDB_BT_LOGON_QUIT,TEXT("BT_LOGON_QUIT"),hInstance,false,false);
	m_btQuit.GetClientRect(rcBtn);
	m_btQuit.SetWindowPos(NULL,505,5,rcBtn.Width(),rcBtn.Height(),SWP_NOZORDER|SWP_NOSIZE);

	m_btEnter.SetButtonImage(IDB_BT_LOGON,TEXT("BT_LOGON_ENTER"),hInstance,false,false);

	m_btEnter.SetWindowPos(NULL,254,216,rcBtn.Width(),rcBtn.Height(),SWP_NOZORDER|SWP_NOSIZE);
	m_btForgetPass.SetButtonImage(IDB_BTN_FORGET_PASS,TEXT("BTN_FORGET_PASS"),hInstance,false,false);
	m_btForgetPass.GetClientRect(rcBtn);

	m_btForgetPass.SetWindowPos(NULL,442,185,rcBtn.Width(),rcBtn.Height(),SWP_NOZORDER|SWP_NOSIZE);
	m_btKeFu.SetButtonImage(IDB_KEFU,hInstance,false,false);
	m_btKeFu.GetClientRect(rcBtn);
	m_btKeFu.SetWindowPos(NULL,28,221,rcBtn.Width(),rcBtn.Height(),SWP_NOZORDER|SWP_NOSIZE);
	m_btDeleteRecord.SetButtonImage(IDB_BT_RUBBISH,hInstance,false,false);
	m_btDeleteRecord.GetClientRect(rcBtn);
	m_btDeleteRecord.SetWindowPos(NULL,509,99,rcBtn.Width(),rcBtn.Height(),SWP_NOZORDER|SWP_NOSIZE);
	//CRect rcCreate(0,0,0,0);

	//Loadweb();
	//创建控件
	//m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//EWIN网络科技
	//创建控件
// 	CRect rcWndButton(15,304,440,300);
// 	m_WndLogonButton.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcWndButton,this,IDC_WND_LOGON_BUTTON);


	//变量定义
	//CGlobalUnits * pGlobalUnits = (CGlobalUnits *)CGlobalUnits::GetInstance();
	//ASSERT(pGlobalUnits!=NULL);

	//记住密码
	m_RemPwdControl.LoadCheckImage(TEXT("BT_CHECK_BUTTON_BACK"));
	m_RemPwdControl.SetControlBenchmark(260,187);
	m_RemPwdControl.SetParentWndSink(this);

	//工作目录
	//TCHAR szDirectory[MAX_PATH]=TEXT("");
	//CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造地址
	//TCHAR szBillUrl[256]=TEXT("");
	//_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s\\Web\\LobbyTopNotice.htm"),szDirectory);

	//设置广告
	//if(pGlobalUnits!=NULL && pGlobalUnits->LocalFileExist(szBillUrl))
	//{
// 		m_PlatformPublicize.SetBackColor(RGB(43,93,128));
// 		m_PlatformPublicize.Navigate(szPlatformTopPublicize);
// 		m_PlatformPublicize.SetWindowPos(NULL,43,9,SizeWindow.cx-87,18,SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOACTIVATE);
	//}
	//else
	//	m_PlatformPublicize.SetWindowPos(NULL,43,9,SizeWindow.cx-87,18,SWP_HIDEWINDOW|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOACTIVATE);


	//配置信息
	LoadAccountsInfo();
	//LoadLogonServerInfo();
	//设置焦点
	if (m_AccountsControl.GetCurSel()!=LB_ERR)
	{
		//消息处理
		OnSelchangeAccounts();

		//设置焦点
		m_PasswordControl.SetFocus();
	}
	else
	{
		//设置焦点
		m_AccountsControl.SetFocus();
	}

	//居中窗口
	CenterWindow(this);

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//设置区域
	CRgn RgnWindow;
	//RgnWindow.CreateRoundRectRgn(20,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+30,SizeWindow.cy-LAYERED_SIZE+50,ROUND_CX,ROUND_CY);
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);
	CRect rcCreate(0,0,0,0);
	m_logo.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_LOGON_LOGO);

	m_logo.SetWindowPos(NULL,38,34,132,184,SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER);

	//分层窗口
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);
	m_DlgStatus.SetStatusViewSink(this);
	return FALSE;
}
void CDlgLogon::Loadweb()
{

	CString strPath;  //文件路径
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	int pos = strPath.ReverseFind('\\'); 
	strPath = strPath.Left(pos); 

	m_logo.Navigate(strPath +_T("\\logon.html"));
	m_logo.EnableWindow(FALSE);
}

//消息解释
BOOL CDlgLogon::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//确定函数
VOID CDlgLogon::OnOK()
{
	//获取信息
	if (GetInformation()==false) return;

	//隐藏窗口
	ShowWindow(SW_HIDE);

	CString strSiteURL;
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));
	m_strCurrentDir = szDirectory;
	m_strCurrentDir += L"\\";

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Update\\update.ini"),szDirectory);
	//构造地址
	TCHAR szNavigation[256]=TEXT("");

	GetPrivateProfileString(TEXT("SERVER"),TEXT("SERVER1"),TEXT(""),szNavigation,CountArray(szNavigation),szFileName);
	strSiteURL = szNavigation;
	m_strSiteUrl = szNavigation;
	strSiteURL+=_T("/update/update.xml");
	m_strSiteUrl+=_T("\\");
	//EWIN网络 科技
	if(lstrcmp(AfxGetApp()->m_lpCmdLine,TEXT("UPDATED"))!=0 && CheckVersionUpdate(strSiteURL))
	{
		//启动程序
		CHAR szModuleName[MAX_PATH]={0};
		GetModuleFileNameA(AfxGetInstanceHandle(), szModuleName, MAX_PATH);
		PathRemoveFileSpecA(szModuleName);
		strcat(szModuleName, "\\Update.exe");
		WinExec(szModuleName, SW_SHOWDEFAULT);


		return ;

	}

	//执行登录
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(m_RemPwdControl.GetButtonChecked()==TRUE);

	return;
}
BOOL CDlgLogon::CheckVersionUpdate(CString& strUpdateURL)
{
	BOOL bHasUpdate = FALSE;

	HRESULT hr = S_OK;
	IXMLHTTPRequestPtr pHttpRequest;
	IDispatchPtr pDispatch;
	MSXML2::IXMLDOMDocumentPtr pXmlDoc;
	MSXML2::IXMLDOMNodeListPtr pList;
	MSXML2::IXMLDOMElementPtr pChild;

	UINT nFileSize;
	CString strFileName, strFileVer, strMD5String,strFilePath;
	LONG lElementCount = 0L;

	try
	{
		hr = pHttpRequest.CreateInstance(TEXT("Msxml2.XMLHTTP.3.0"));
		if( FAILED(hr) )
			_com_issue_error(hr);

		hr = pHttpRequest->open(TEXT("GET"), (_bstr_t)strUpdateURL, false);
		if( FAILED(hr) )
			_com_issue_error(hr);

		hr = pHttpRequest->send();
		if( FAILED(hr) )
			_com_issue_error(hr);

		if (pHttpRequest->Getstatus() != 200)
			throw (0);

		pDispatch = pHttpRequest->GetresponseXML();
		hr = pDispatch->QueryInterface(pXmlDoc.GetIID(), (void**)&pXmlDoc);
		if( FAILED(hr) )
			_com_issue_error(hr);

		pList = pXmlDoc->selectNodes("/manifest/filelist/file");
		lElementCount = pList->Getlength();
		for( LONG i = 0; i < lElementCount; i++ )
		{
			pChild = pList->Getitem(i);
			 strFilePath = pChild->getAttribute("filepath");
			strFileName = pChild->getAttribute("filename");
			nFileSize = pChild->getAttribute("filesize");
			strFileVer = pChild->getAttribute("fileversion");
			strMD5String = pChild->getAttribute("md5");
			//如果本地文件存在则效验文件，不存在就下载这个文件
			if(strFilePath!=""){
				strFilePath=strFilePath.TrimLeft(_T("\\"));
				strFilePath+=_T("\\");
			}
			CString strRemoteFile=m_strSiteUrl+strFilePath+strFileName;
			CString strPathFileName=m_strCurrentDir+strFilePath+strFileName;
			BOOL iRet=PathFileExists(strPathFileName);
			CString strLog;
			strLog.Format(_T("\nUPDATE %s %s,%s"),(iRet?_T("存在文件"):_T("不存在文件")),strPathFileName,strRemoteFile);
			OutputDebugString(strLog);
			if(iRet)
			{
				//效验文件,如果MD5码不相同则重新下载
				CString strLocalMD5String =CMD5Checksum::GetMD5(strPathFileName);
				CString strLog;
				strLog.Format(_T("\nUPDATE  本地MD5:%s 远程MD5:%s %s"),strLocalMD5String,strMD5String,(strLocalMD5String==strMD5String?_T("相同"):_T("不相同")));
				OutputDebugString(strLog);
				if ( strLocalMD5String!= strMD5String)
				{
					bHasUpdate = TRUE;
				}	
			}
			else
			{
				bHasUpdate = TRUE;
			}
		}

		return bHasUpdate;
	}
#ifdef _DEBUG
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Error code:%d\nMessage:%s\nDescrption:%s"), (int)e.WCode(), e.ErrorMessage(), (TCHAR*)e.Description());
		MessageBox(strError, _T("提示"), MB_OK|MB_ICONWARNING);
		return FALSE;
	}
#endif
	catch(...)
	{
		return FALSE;
	}
}


//取消消息
VOID CDlgLogon::OnCancel()
{
	//关闭窗口
	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	return;
}

//构造数据
WORD CDlgLogon::ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize)
{
	//变量定义
	CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)cbBuffer;

	//其他信息
	CWHService::GetMachineID(pLogonAccounts->szMachineID);
	CWHEncrypt::MD5Encrypt(m_szPassword,pLogonAccounts->szPassword);

	//登录信息
	pLogonAccounts->dwPlazaVersion=VERSION_PLAZA;
	lstrcpyn(pLogonAccounts->szAccounts,m_szAccounts,CountArray(pLogonAccounts->szAccounts));

	//保存密码
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	lstrcpyn(pGlobalUserData->szPassword,pLogonAccounts->szPassword,CountArray(pGlobalUserData->szPassword));

	return sizeof(CMD_GP_LogonAccounts);
}

//加载账号
VOID CDlgLogon::LoadAccountsInfo()
{
	//读取信息
	CWHRegKey RegUserInfo;
	if (RegUserInfo.OpenRegKey(REG_USER_INFO,false)==false) return;

	//变量定义
	DWORD dwRegIndex=0L;
	DWORD dwLastUserID=RegUserInfo.GetValue(TEXT("LastUserID"),0L);

	//读取信息
	do
	{
		//变量定义
		tagAccountsInfo AccountsInfo;
		ZeroMemory(&AccountsInfo,sizeof(AccountsInfo));

		//读取键项
		TCHAR szKeyItemName[16]=TEXT("");
		if (RegUserInfo.EnumItemString(dwRegIndex++,szKeyItemName,CountArray(szKeyItemName))==false) break;

		//打开子键
		CWHRegKey RegUserItem;
		if (RegUserItem.Attach(RegUserInfo.OpenItemKey(szKeyItemName))==NULL) continue;

		//帐户信息
		AccountsInfo.dwUserID=_tstol(szKeyItemName);
		RegUserItem.GetString(TEXT("GameID"),AccountsInfo.szGameID,CountArray(AccountsInfo.szGameID));
		RegUserItem.GetString(TEXT("UserAccount"),AccountsInfo.szAccounts,CountArray(AccountsInfo.szAccounts));

		//读取密码
		TCHAR szPassword[MAX_ENCRYPT_LEN]=TEXT("");
		RegUserItem.GetString(TEXT("UserPassword"),szPassword,CountArray(szPassword));

		//解开密码
		if (szPassword[0]!=0)
		{
			CWHEncrypt::XorCrevasse(szPassword,AccountsInfo.szPassword,CountArray(AccountsInfo.szPassword));
		}

		//构造数据
		tagAccountsInfo * pAccountsInfo=new tagAccountsInfo;
		CopyMemory(pAccountsInfo,&AccountsInfo,sizeof(AccountsInfo));

		//插入数据
		m_AccountsInfoArray.InsertAt(m_AccountsInfoArray.GetCount(),pAccountsInfo);

	} while (true);

	//插入信息
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		//插入信息
		INT nGameIDItem=CB_ERR;
		INT nAccountsItem=CB_ERR;
		tagAccountsInfo * pAccountsInfo=m_AccountsInfoArray[i];

		//插入信息
		if (pAccountsInfo->szAccounts[0]!=0) nAccountsItem=m_AccountsControl.AddString(pAccountsInfo->szAccounts);

		//设置数据
		if (nAccountsItem!=CB_ERR) m_AccountsControl.SetItemDataPtr(nAccountsItem,pAccountsInfo);

		//设置选择
		if ((dwLastUserID!=0L)&&(pAccountsInfo->dwUserID==dwLastUserID))
		{
			if (nAccountsItem!=CB_ERR) m_AccountsControl.SetCurSel(nAccountsItem);
		}
	}

	//设置选择
	if ((m_AccountsControl.GetCount()>0)&&(m_AccountsControl.GetCurSel()==LB_ERR)) m_AccountsControl.SetCurSel(0);

	return;
}

//地址信息
VOID CDlgLogon::LoadLogonServerInfo()
{

//	if (m_ServerControl.GetCurSel()==LB_ERR)
	{
	//	if (m_ServerControl.GetCount()==0L)
		{
			m_DlgStatus.ShowStatusWindow(L"正在搜索最优主站...");
			
			TCHAR szPlatformUrl[124] = L"";
			TCHAR szPlatformUrl1[124] = L"";

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

			CTime time = CTime::GetCurrentTime();
			TCHAR szTodayURL[124] = L"";
			int nUrlID = GetPrivateProfileInt(time.Format(L"%Y-%m-%d"),TEXT("TodayURLID"),0,szFileName);
			int nUrlCanUse = GetPrivateProfileInt(time.Format(L"%Y-%m-%d"),TEXT("TodayCanUse"),0,szFileName);
			TCHAR szKeyBoard[124]=L"";
			_sntprintf(szKeyBoard,CountArray(szKeyBoard),_T("LogonServer%d"),nUrlID);
			IniData.ReadEncryptString(TEXT("PlazaUrl"),szKeyBoard,TEXT(""),szTodayURL,CountArray(szTodayURL));
			CString strUrl;
			strUrl.Format(L"%s",szTodayURL);


			int nUrl=0;
			TCHAR szKey[32];

			if(strUrl.IsEmpty())
			{
				int nTTL = 10000;
				CPing ping;
				PingReply reply;
				for(int i = 0;i < 4;i++)
				{
// 					if(!strUrl.IsEmpty()&&(i == nUrlID))
// 					{
// 						continue;
// 					}
					memset(&reply, 0, sizeof(PingReply));
			//		TCHAR szKey[32];
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
					if((reply.m_dwRoundTripTime < nTTL) || lstrlen(szPlatformUrl) == 0)
					{
						lstrcpyn(szPlatformUrl,szPlatformUrl1,CountArray(szPlatformUrl));
						nUrl = i;
//						if(reply.m_dwRoundTripTime > 0 && reply.m_dwRoundTripTime < 10000)
//						{
// 							//如果PING通了，注册表记录一下当前SERVER
// 							CWHRegKey RegServerAddr;
// 							RegServerAddr.OpenRegKey(TEXT("CurrentLogonServer"),true);
// 							RegServerAddr.WriteString(TEXT("CurrentIpAddress"),szPlatformUrl);
//						}
						nTTL = reply.m_dwRoundTripTime;
					}
				}

			}
			else if(nUrlCanUse==0)
			{
				nUrl = (nUrlID+1)%5;
				ZeroMemory(&szKey,sizeof(szKey));
				_sntprintf(szKey,CountArray(szKey),_T("LogonServer%d"),nUrl);
				//读取配置
				IniData.ReadEncryptString(TEXT("PlazaUrl"),szKey,NULL,szPlatformUrl,CountArray(szPlatformUrl));

			}
			else
			{
				lstrcpyn(szPlatformUrl,szTodayURL,CountArray(szPlatformUrl));
			}

			ZeroMemory(&szKey,sizeof(szKey));
			_sntprintf(szKey,CountArray(szKey),_T("LogonServer%d"),nUrl);

			GetPrivateProfileString(TEXT("PlazaUrl"),szKey,NULL,szTodayURL,CountArray(szTodayURL),szFileName);

			WritePrivateProfileString(time.Format(L"%Y-%m-%d"),TEXT("TodayURL"),szTodayURL,szFileName);
			_sntprintf(szKey,CountArray(szKey),_T("%d"),nUrl);
			WritePrivateProfileString(time.Format(L"%Y-%m-%d"),TEXT("TodayURLID"),szKey,szFileName);
			_sntprintf(szKey,CountArray(szKey),_T("%d"),1);
			WritePrivateProfileString(time.Format(L"%Y-%m-%d"),TEXT("TodayCanUse"),szKey,szFileName);
		

			_sntprintf(m_szLogonServer,CountArray(m_szLogonServer),_T("%s"),szPlatformUrl);
			m_ServerControl.SetCurSel(nUrl);
			m_DlgStatus.HideStatusWindow();
		}
// 		else
// 		{
// 			m_ServerControl.SetCurSel(0);
// 		}
	}

	return;
}
//取消连接
VOID CDlgLogon::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

//获取信息
bool CDlgLogon::GetInformation()
{
	//获取密码
	m_PasswordControl.GetUserPassword(m_szPassword);

	//获取信息
	GetDlgItemText(IDC_ACCOUNTS,m_szAccounts,CountArray(m_szAccounts));
	//GetDlgItemText(IDC_LOGON_SERVER,m_szLogonServer,CountArray(m_szLogonServer));

	//地址判断
	if (m_szLogonServer[0]==0L)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请选择“登录服务器”后再登录，若有困难请点击“新手指南”获取帮助！"),MB_ICONERROR,0);

		//显示窗口
		ShowWindow(SW_SHOW);

		//设置焦点
		m_ServerControl.SetFocus();

		return false;
	}

	//帐号判断
	if (m_szAccounts[0]==0L)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请您输入帐号后再登录，若有困难请点击“新手指南”获取帮助！"),MB_ICONERROR,0);

		//显示窗口
		ShowWindow(SW_SHOW);

		//设置焦点
		m_edAccounts.SetFocus();

		return false;
	}

	//密码判断
	if (m_szPassword[0]==0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请您输入密码后再登录，若有困难请点击“新手指南”获取帮助！"),MB_ICONERROR,0);

		//显示窗口
		ShowWindow(SW_SHOW);

		//设置焦点
		m_PasswordControl.SetFocus();

		return false;
	}

	return true;
}

//注册帐号
VOID CDlgLogon::OnBnClickedRegister()
{
	//隐藏窗口
	ShowWindow(SW_HIDE);

	//获取地址
	CString strLogonServer;
	GetDlgItemText(IDC_LOGON_SERVER,strLogonServer);

	//构造地址
	strLogonServer.TrimLeft();
	strLogonServer.TrimRight();
	TCHAR szPlatformUrl[124] = L"";

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

	TCHAR szKey[20];
	ZeroMemory(&szKey,sizeof(szKey));
	_sntprintf(szKey,CountArray(szKey),_T("LogonServer%d"),rand()%20);
	//读取配置
	IniData.ReadEncryptString(TEXT("PlazaUrl"),szKey,NULL,szPlatformUrl,CountArray(szPlatformUrl));

	lstrcpyn(m_szLogonServer,(strLogonServer.IsEmpty()==true)?szPlatformUrl:strLogonServer,CountArray(m_szLogonServer));

	//显示注册
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowRegister();

	return;
}
#include "MessageDlg.h"
//找回密码
VOID CDlgLogon::OnBnClickedForget()
{

	MyMessageBox(L"请联系客服找回密码！");
	return;
	//读取配置文件
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	//构造地址
	TCHAR szLogonLink[256]=TEXT("");
	//	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%sLogonLink4.aspx"),szPlatformLink);
	GetPrivateProfileString(TEXT("register"),TEXT("Link"),TEXT(""),szLogonLink,CountArray(szLogonLink),szFileName);

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}
VOID CDlgLogon::OnBnClickedDelete()
{
	//变量定义
	CAccountsCtrl * pAccountsCtrl=NULL;
	tagAccountsInfo * pAccountsInfo=NULL;

	pAccountsCtrl=&m_AccountsControl;

	//获取信息
	INT nCurrentSel=pAccountsCtrl->GetCurSel();

	//删除处理
	if (nCurrentSel!=LB_ERR)
	{
		//获取变量
		ASSERT(pAccountsCtrl->GetItemDataPtr(nCurrentSel)!=NULL);
		pAccountsInfo=(tagAccountsInfo *)pAccountsCtrl->GetItemDataPtr(nCurrentSel);

		//构造标识
		CString strUserID;
		strUserID.Format(TEXT("%ld"),pAccountsInfo->dwUserID);

		//用户表项
		CWHRegKey RegUserInfo;
		RegUserInfo.OpenRegKey(REG_USER_INFO,false);

		//删除表项
		RegUserInfo.RecurseDeleteKey(strUserID);


		//删除列表
		for (INT i=0;i<m_AccountsControl.GetCount();i++)
		{
			if (m_AccountsControl.GetItemDataPtr(i)==pAccountsInfo)
			{
				//删除字符
				m_AccountsControl.DeleteString(i);
				m_AccountsControl.SetWindowText(TEXT(""));

				//更新选择
				if ((m_AccountsControl.GetCurSel()==LB_ERR)&&(m_AccountsControl.GetCount()>0))
				{
					m_AccountsControl.SetCurSel(0);
				}

				break;
			}
		}
	}

	//设置焦点
	pAccountsCtrl->SetFocus();

	//获取变量
	nCurrentSel=pAccountsCtrl->GetCurSel();
	pAccountsInfo=(nCurrentSel!=LB_ERR)?(tagAccountsInfo *)pAccountsCtrl->GetItemDataPtr(nCurrentSel):NULL;

	//设置密码
	m_PasswordControl.SetUserPassword((pAccountsInfo!=NULL)?pAccountsInfo->szPassword:TEXT(""));

	//记住密码
	bool bRemPassword=(pAccountsInfo!=NULL)?(pAccountsInfo->szPassword[0]!=0):false;
	//((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);
	m_RemPwdControl.SetButtonChecked((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);

	return;
}
//客服
VOID CDlgLogon::OnBnClickedKeFu()
{
// 	//构造地址
// 	TCHAR szNavigation[256]=TEXT("");
// //	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("http://ala.zoossoft.com/LR/Chatpre.aspx?id=ALA22728236&lng=cn"));
// 
// 	//读取配置文件
// 	//工作目录
// 	TCHAR szDirectory[MAX_PATH]=TEXT("");
// 	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));
// 
// 	//构造路径
// 	TCHAR szFileName[MAX_PATH]=TEXT("");
// 	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
// 
// 	//构造地址
// 	TCHAR szLogonLink[256]=TEXT("");
// 	//	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%sLogonLink4.aspx"),szPlatformLink);
// 	GetPrivateProfileString(TEXT("kefu"),TEXT("Link"),TEXT(""),szNavigation,CountArray(szNavigation),szFileName);
// 
// 	//连接地址
// 	ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
	//执行登录
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformKefu();

	return;
}

//编辑改变
VOID CDlgLogon::OnCbnEditchangeAccounts()
{
	//安全设置
	((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck(BST_UNCHECKED);

	//密码重置
	if (m_PasswordControl.IsFalsityPassword()==true) m_PasswordControl.SetUserPassword(NULL);
	m_edAccounts.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );

	return;
}

//绘画背景
BOOL CDlgLogon::OnEraseBkgnd(CDC * pDC)
{
// 	//获取位置
// 	CRect rcClient;
// 	GetClientRect(&rcClient);
// 
// 	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
// 
// 	Graphics graphics( pDC->m_hDC );
// 
// 	Image image(CBmpUtil::GetExePath() + _T("skin\\DLG_LOGON_BACK.png"));
// 
// 	UINT width = image.GetWidth();
// 
// 	UINT height = image.GetHeight();
// 
// 	// 不进行缩放
// 	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
// 	graphics.DrawImage( &image,0,0);
//	m_RemPwdControl.OnDrawControl(pDC);

	return false;
}
//重画消息
VOID CDlgLogon::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int Height = rect.bottom - rect.top;

	CDC *pDC = this->GetDC();
 	Image image(CBmpUtil::GetExePath() + _T("skin\\DLG_LOGON_BACK.png"));
 
 	UINT width = image.GetWidth();
 
 	UINT height = image.GetHeight();
// 	CDC MenmDC;
// 	CBitmap MemBitmap;
// 	MenmDC.CreateCompatibleDC(NULL);
// 	MemBitmap.CreateCompatibleBitmap(pDC, width, height);
// 	CBitmap *pOldBit = MenmDC.SelectObject(&MemBitmap);
// 	MenmDC.SetBkMode(TRANSPARENT);
// 
// 
// 	using namespace Gdiplus;
// 	Graphics graphics(MenmDC.m_hDC);
// 	graphics.DrawImage(&image, Rect(0, 0,width,height), 0, 0, width,height, UnitPixel);
	/*********************************************************/
	CPngImage BackImage;
	BackImage.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_LOGON_BACK"));
	BackImage.DrawImage(pDC,0,0);

	//pDC->BitBlt(0, 0, width, height, &MenmDC, 0, 0, SRCCOPY);
	m_RemPwdControl.OnDrawControl(pDC);

// 	MenmDC.SelectObject(pOldBit);
// 	MemBitmap.DeleteObject();
// 	MenmDC.DeleteDC();
// 	ReleaseDC(pDC);
	return;
}

//显示消息
VOID CDlgLogon::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//显示分层
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
		if(bShow)
		{
			LoadLogonServerInfo();
			Loadweb();

		}
	}

	return;
}

//鼠标消息
VOID CDlgLogon::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟标题
	if (Point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//位置改变
VOID CDlgLogon::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//移动分层
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//控件颜色
HBRUSH CDlgLogon::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	if(pWnd->GetDlgCtrlID() == IDC_ACCOUNTS)
	{
	//	pDC->SetBkMode(TRANSPARENT);
		m_edAccounts.SetFont(&m_Font,false);
		pDC->SetBkColor(RGB(255,255,255));
		HBRUSH B = CreateSolidBrush(RGB(255,255,255)); 
		return (HBRUSH) B;

	}
	if(pWnd->GetDlgCtrlID() == IDC_PASSWORD)
	{
		pDC->SetBkMode(TRANSPARENT);
		m_PasswordControl.SetFont(&m_Font,false);
		pDC->SetBkColor(RGB(255,0,0));
		HBRUSH B = CreateSolidBrush(RGB(234,179,106)); 
		//return (HBRUSH) B;

	}

	switch (nCtlColor)
	{
	//case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(10,10,10));
			return m_brBrush;
		}
	case IDC_ACCOUNTS:
		{
			pDC->SetBkMode(TRANSPARENT);
			m_edAccounts.SetFont(&m_Font,false);
			pDC->SetBkColor(RGB(255,0,0));
			HBRUSH B = CreateSolidBrush(RGB(255,0,0)); 
			return (HBRUSH) B;

			//pDC->SetFont(&m_Font);
			//return m_brBrush;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor); ;
}

//控件改变
void CDlgLogon::OnEnChangeAccounts()
{
//	m_edAccounts.SetFont(&m_Font,false);

	//虚假密码
	if(m_PasswordControl.m_hWnd!=NULL && m_PasswordControl.IsFalsityPassword()==true)
	{
		//密码设置
		m_PasswordControl.SetUserPassword(TEXT(""));

        //安全设置
		m_RemPwdControl.SetButtonChecked(FALSE);
	}
	m_edAccounts.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );

}

//鼠标弹起
void CDlgLogon::OnLButtonUp(UINT nFlags, CPoint point)
{
	//选中事件
	m_RemPwdControl.OnClickControl(point);

	CDialog::OnLButtonUp(nFlags, point);
}



//////////////////////////////////////////////////////////////////////////////////



