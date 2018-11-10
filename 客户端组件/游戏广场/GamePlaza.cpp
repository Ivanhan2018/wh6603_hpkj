#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "PlatformFrame.h"
#include "MD5Checksum\MD5Checksum.h"
//////////////////////////////////////////////////////////////////////////////////
#import <msxml3.dll>
using namespace MSXML2;

BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePlazaApp::CGamePlazaApp()
{
	//设置变量
	m_pIDispatch=NULL;
	m_pCustomOccManager=NULL;

	return;
}

//初始函数
BOOL CGamePlazaApp::InitInstance()
{
	__super::InitInstance();

#ifndef _DEBUG
#ifndef RELEASE_MANAGER

	//存在判断
	bool bPlazaExist=false;
	CMutex Mutex(FALSE,szPlazaClass,NULL);
	if (Mutex.Lock(0)==FALSE) bPlazaExist=true;

	LoadLibraryA("RICHED20.DLL");
	//搜索窗口
	CWnd * pWndGamePlaza=CWnd::FindWindow(szPlazaClass,NULL);
	if (pWndGamePlaza!=NULL) 
	{
		//设置变量
		bPlazaExist=true;

		//还原窗口
		if (pWndGamePlaza->IsIconic()) 
		{
			pWndGamePlaza->ShowWindow(SW_RESTORE);
		}

		//激活窗口
		pWndGamePlaza->SetActiveWindow();
		pWndGamePlaza->BringWindowToTop();
		pWndGamePlaza->SetForegroundWindow();
	}

	//结果处理
	if (bPlazaExist==true) return FALSE;

#endif
#endif

	//环境配置
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//随机种子
	srand((DWORD)time(NULL));

	//设置表名
	SetRegistryKey(szProductKey);


	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//安装目录
	CWHRegKey RegInstallPath;
	RegInstallPath.OpenRegKey(TEXT("HpKjGameDev"),true);
	RegInstallPath.WriteString(TEXT("InstallPath"),szDirectory);

	//加载配置
	m_ParameterGlobal.LoadParameter();
	m_UserInformation.LoadInformation();

	//设置目录
	SetCurrentDirectory(szDirectory);
	m_SkinResourceManager.SetSkinResource(m_ParameterGlobal.m_SkinRenderInfo);

	m_strCurrentDir = szDirectory;
	m_strCurrentDir += L"\\";
	//创建对象
	m_pIDispatch=new CImpIDispatch;
	AfxEnableControlContainer(new CCustomOccManager); 

	//创建组件
	if (m_FaceItemControlModule.CreateInstance()==false) return false;
	if (m_UserOrderParserModule.CreateInstance()==false) return false;

	//用户元素
	m_UserItemElement.SetUserOrderParser(m_UserOrderParserModule.GetInterface());
	m_UserItemElement.SetFaceItemControl(m_FaceItemControlModule.GetInterface());

	theApp.InitCaiZhong();

//  	//检查更新
//  #ifndef DEBUG 
//  #ifndef RELEASE_MANAGER
//  
// //  	CString strSiteURL;
// //  
// //  	//构造路径
// //  	TCHAR szFileName[MAX_PATH]=TEXT("");
// //  	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\update.ini"),szDirectory);
// //  	//构造地址
// //  	TCHAR szNavigation[256]=TEXT("");
// //  
// //  	GetPrivateProfileString(TEXT("SERVER"),TEXT("SERVER1"),TEXT(""),szNavigation,CountArray(szNavigation),szFileName);
// //  	strSiteURL = szNavigation;
// //  
// //  	strSiteURL+=_T("/update/update.xml");
// //  	//EWIN网络 科技
// //  	if(lstrcmp(AfxGetApp()->m_lpCmdLine,TEXT("UPDATED"))!=0 && CheckVersionUpdate(strSiteURL))
// //  	{
// //  		//启动程序
// //  		CHAR szModuleName[MAX_PATH]={0};
// //  		GetModuleFileNameA(AfxGetInstanceHandle(), szModuleName, MAX_PATH);
// //  		PathRemoveFileSpecA(szModuleName);
// //  		strcat(szModuleName, "\\Update.exe");
// //  		WinExec(szModuleName, SW_SHOWDEFAULT);
// //  
// //  
// //  		return true;
// //  
// //  	}
//  #endif
//  #endif
	theBeginCount = ::GetTickCount();
	//变量定义
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss,sizeof(WndClasss));

	//注册窗口
	WndClasss.style=CS_DBLCLKS;
	WndClasss.hIcon=LoadIcon(IDR_MAINFRAME);
	WndClasss.lpfnWndProc=DefWindowProc;
	WndClasss.lpszClassName=szPlazaClass;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();

	//建立窗口
	CPlatformFrame * pPlatformFrame=new CPlatformFrame();
	pPlatformFrame->Create(szPlazaClass,szProduct,WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));
	//设置变量
	m_pMainWnd=pPlatformFrame;


	return TRUE;
}
bool	CGamePlazaApp::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}
//获取系统时间
bool CGamePlazaApp::GetSystemT()
{
//	SYSTEMTIME sysT;
// 	memset(&sysT, 0, sizeof(SYSTEMTIME));
// 	GetSystemTime(sysT);
// 	if(!theDBCmd.GetSysTime(sysT))
// 	{
// 		return false;
// 	}

// 	theTimeCS.Lock();
// 	theTime = CTime::GetCurrentTime()/*(sysT)*/;
// 	theTimeCS.Unlock();

	return true;
}
BOOL CGamePlazaApp::CheckVersionUpdate(CString& strUpdateURL)
{
	BOOL bHasUpdate = FALSE;

	HRESULT hr = S_OK;
	IXMLHTTPRequestPtr pHttpRequest;
	IDispatchPtr pDispatch;
	MSXML2::IXMLDOMDocumentPtr pXmlDoc;
	MSXML2::IXMLDOMNodeListPtr pList;
	MSXML2::IXMLDOMElementPtr pChild;

	UINT nFileSize;
	CString strFileName, strFileVer, strMD5String;
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
			strFileName = pChild->getAttribute("filename");
			nFileSize = pChild->getAttribute("filesize");
			strFileVer = pChild->getAttribute("fileversion");
			strMD5String = pChild->getAttribute("md5");

			//如果本地文件存在则效验文件，不存在就下载这个文件
			if (PathFileExists(m_strCurrentDir+strFileName))
			{
				//加密密码
				TCHAR szInsurePass[LEN_MD5];
				CWHEncrypt::MD5Encrypt(m_strCurrentDir+strFileName,szInsurePass);

				CHAR szModuleName[MAX_PATH]={0};
				GetModuleFileNameA(AfxGetInstanceHandle(), szModuleName, MAX_PATH);

		
				CString strFName = (m_strCurrentDir+strFileName);

				CString strMD5File = CMD5Checksum::GetMD5(strFName) ;
				//效验文件,如果MD5码不相同则重新下载
				if (strMD5File != strMD5String)
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


//获取
void CGamePlazaApp::GetTime(CTime& time)
{
	theTimeCS.Lock();
	time = theTime;
	theTimeCS.Unlock();
}

//初始化彩种信息
void CGamePlazaApp::InitCaiZhong()
{
	theGameType[CZJiangXiSSC] = _T("江西时时彩");
	theGameType[CZ_TianJinSSC] = _T("天津时时彩");

	theGameType[CZChongQingSSC] = _T("重庆时时彩");
	theGameType[CZ_FENFEN_CAI] = _T("吉利分分彩");
	theGameType[CZ_WUFEN_CAI] = _T("吉利五分彩");
	theGameType[CZXinJiangSSC] = _T("新疆时时彩");
	theGameType[CZ_HGYDWFC] = _T("韩国1.5分彩");
	theGameType[CZ3D] = _T("3D彩");
	theGameType[CZGD11Xuan5]=_T("广东11选5");
	theGameType[CZCQ11Xuan5]=_T("重庆11选5");
	theGameType[CZJX11Xuan5]=_T("江西11选5");
	theGameType[CZSD11Xuan5]=_T("山东11选5");
	theGameType[CZ_LIUHECAI] = _T("六合彩");
	theGameType[CZHLJ11Xuan5] = _T("黑龙江11选5");
	theGameType[CZ_QiXingCai] = _T("七星彩");
	theGameType[CZKUAILE8] = _T("北京快8");
	theGameType[CZPaiLie3] = _T("排列三");
	theGameType[CZ_PK10] = _T("北京PK10");
	theGameType[CZXingYun28] = _T("幸运28");

}

//退出函数
INT CGamePlazaApp::ExitInstance()
{
	//保存配置
	m_ParameterGlobal.SaveParameter();

	//删除对象
	SafeDelete(m_pIDispatch);
	SafeDelete(m_pCustomOccManager);

	return __super::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////////////

//应用程序对象
CGamePlazaApp theApp;

CMap<unsigned int, unsigned int, double, double> theBonus;
//彩种信息-重庆时时彩
CMap<int, int, CString, CString&> theGameType;
//玩法模式-后三组三复式
CMap<int, int, CString, CString&> theGameKind;
//本地时间与系统时间的差值
//long theTimeDiff = 0L;
//系统时间
CTime theTime;
BaseUser theAccount;
//////////////////////////////////////////////////////////////////////////////////
