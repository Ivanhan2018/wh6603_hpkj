#include "StdAfx.h"
#include "MessageDlg.h"

#include "ExDispID.h"
#include "GamePlaza.h"
#include "PlazaViewItem.h"
#include "PlatformFrame.h"
//EWIN网络密码房间
#include "DlgServerPassWork.h"

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_PLAZA_BROWSER			100									//浏览控件

//图片位置
#define GAME_TITLE_CY				81									//标题高度
#define GAME_TITLE_CX				786									//标题宽度

//类型位置
#define GAME_KIND_ITEM_PX			20									//类型间隔
#define GAME_KIND_ITEM_PY			10									//类型间隔
#define GAME_KIND_ITEM_CX			99									//类型宽度
#define GAME_KIND_ITEM_CY			153									//类型高度

//类型位置
#define GAME_SERVER_ITEM_PX			2									//房间间隔
#define GAME_SERVER_ITEM_PY			3									//房间间隔
#define GAME_SERVER_ITEM_CX			190									//房间宽度
#define GAME_SERVER_ITEM_CY			225									//房间高度


//////////////////////////////////////////////////////////////////////////////////

//静态变量
CPlazaViewItem * CPlazaViewItem::m_pPlazaViewItem=NULL;					//广场指针

static int game_cx = 791;
static int game_cy = 558;
//////////////////////////////////////////////////////////////////////////////////
#define  IDC_RETURN 100250
#define  IDC_FRAME_ADD	100251

BEGIN_MESSAGE_MAP(CPlazaViewItem, CDialog)

	//系统消息
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_MESSAGE(IDM_LUCKY_NUM_SSC,SendQueryGameResult)
	ON_MESSAGE(IDM_SHOW_MENU_SSC,OnShowMenu)
	ON_MESSAGE(IDM_CLICKED_TYPE_SSC, OnBnClickedGameType)	
	ON_MESSAGE(IDM_UPDATE_ACCOUNT, OnUpdateAccount)	
	ON_MESSAGE(IDM_TANCHUANG, OnTanChuang)	
	ON_MESSAGE(IDM_SHOW_XGMM, OnShowXgmm)	
	ON_BN_CLICKED(IDC_RETURN, OnBnClickedReturnUppage)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaViewItem::CPlazaViewItem() : CDialog(IDD_GAME_PLAZA)
{
	//设置变量
	m_bHovering=false;
	m_bCreateFlag=false;
	m_cbShowItemMode=VIEW_MODE_GAME;
	m_bShowMenu=false;
	m_nMenuXPos=0;
	m_nMenuYPos=0;
	m_cbMenuType=0xff;
	//移动位置
	m_cbButtonDown=0;
	m_cbButtonHover=0;
	m_wViewItemDown=INVALID_WORD;
	m_wViewItemHover=INVALID_WORD;
	m_dwTanChuangTick = 0;
	m_bLogonSuccess = false;
	//位置变量
	m_wKindXCount=0;
	m_wKindYCount=0;
	m_wKindExcursion=0;
	m_wKindPageCount=0;
	m_wKindPageCurrent=0;
	m_wKindTypeCurrentID=0;

	m_wGameHoverItem=INVALID_WORD;
	//位置变量
	m_wServerXCount=0;
	m_wServerYCount=0;
	m_wServerExcursion=0;
	m_wServerPageCount=0;
	m_wServerPageCurrent=0;
	m_nOldTypeID = 0;
	ZeroMemory(m_strWebUrl,sizeof(m_strWebUrl));
	//读取配置文件
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	GetPrivateProfileString(TEXT("PlazaUrl"),TEXT("PlazaUrlAdd"),TEXT(""),m_strWebUrl,CountArray(m_strWebUrl),szFileName);

	//设置对象
	ASSERT(m_pPlazaViewItem==NULL);
	if (m_pPlazaViewItem==NULL) m_pPlazaViewItem=this;

	m_bCreate = 0;
	return;
}

//析构函数
CPlazaViewItem::~CPlazaViewItem()
{
	//删除类型
	for (INT i=0;i<m_GameKindInfoActive.GetCount();i++)
	{
		SafeDelete(m_GameKindInfoActive[i]);
	}
	//删除类型
	for (INT i=0;i<m_GameKindInfoInterface.GetCount();i++)
	{
		SafeDelete(m_GameKindInfoInterface[i]);
	}

	//删除类型
	for (INT i=0;i<m_GameKindInfoActiveMenu.GetCount();i++)
	{
		SafeDelete(m_GameKindInfoActiveMenu[i]);
	}

	//删除类型
	for (INT i=0;i<m_GameKindInfoBuffer.GetCount();i++)
	{
		SafeDelete(m_GameKindInfoBuffer[i]);
	}
	for (INT i=0;i<m_GameServerInfoActive.GetCount();i++)
	{
		SafeDelete(m_GameServerInfoActive[i]);
	}

// 	//删除房间
// 	for (INT i=0;i<m_GameKindInfoActive.GetCount();i++)
// 	{
// 		SafeDelete(m_GameKindInfoActive[i]);
// 	}

	//删除房间
	for (INT i=0;i<m_GameServerInfoBuffer.GetCount();i++)
	{
		SafeDelete(m_GameServerInfoBuffer[i]);
	}

	return;
}

//接口查询
VOID * CPlazaViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE_IUNKNOWNEX(IUnknownEx,Guid,dwQueryVer);
	return NULL;
}

//创建函数
BOOL CPlazaViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//设置变量
	m_bCreateFlag=true;

	//创建按钮
	CRect rcCreate(0,0,0,0);

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_Font.CreateFont(14,0,0,0,700,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));


	m_logo.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_FRAME_ADD);


	m_btnReturn.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_RETURN);
	m_btnReturn.SetImage(CBmpUtil::GetExePath() + _T("skin\\return_bt.png"));
	m_bCreateType1 = true;
	CPngImage ImageType;
	ImageType.LoadImage(AfxGetInstanceHandle(),TEXT("TYPE_CP"));
	int nTypeWidth = ImageType.GetWidth()/5;
	int nTypeHeight = ImageType.GetHeight();
	for(BYTE i = 0;i < 3;i++)
	{
		m_rcTypeRect[i].SetRect(11+i*(nTypeWidth+7),290,11+i*(nTypeWidth+7)+nTypeWidth+7,290+nTypeHeight);
	}
	m_cbShowItemMode = VIEW_MODE_GAME;

	m_nRecordTypeID = 1000;
	m_nRecordKindID = 1000;
	m_btnReturn.ShowWindow(SW_HIDE);
	return TRUE;
}
void CPlazaViewItem::CreateDlgCaipiao(int nType)
{
	if (m_bCreate>=4)
	{
		return;
	}
	if(nType == 0)
	{
		m_dlgChongQingSSC.Create(CChongQingSSC::IDD, this);
		m_dlgChongQingSSC.ConnectMainDlg(this);
		m_bCreate = 0;

	}
	else if(nType == 1)
	{
		m_dlgGuangdong11x5.Create(CGuangDong11X5::IDD, this);
		m_dlgGuangdong11x5.ConnectMainDlg(this);
		m_dlgBjKuai8.Create(CBeiJingKuai8::IDD, this);
		m_dlgBjKuai8.ConnectMainDlg(this);
		m_bCreate = 1;

	}
	else if(nType == 2)
	{

		m_dlgPaiLie3.Create(CPaiLie3::IDD, this);//
		m_dlgPaiLie3.ConnectMainDlg(this);
		m_bCreate = 2;


	}
	else if(nType == 3)
	{
		m_dlgQiXingCai.Create(CQiXingCai::IDD, this);//--lly
		m_dlgQiXingCai.ConnectMainDlg(this);
		m_dlg3D.Create(C3D::IDD, this);//
		m_dlg3D.ConnectMainDlg(this);

		m_bCreate = 3;
	}
	else 
	{
		m_dlgXingYun28.Create(CXingYun28::IDD, this);
		m_dlgXingYun28.ConnectMainDlg(this);
		m_dlgLiuHeCai.Create(CLiuHeCai::IDD, this);
		m_dlgBjPK10.Create(CBeiJingPK10::IDD, this);
		m_dlgBjPK10.ConnectMainDlg(this);
		m_bCreate = 4;

	}


	RectifyDlg();
	RedrawWindow();
	return;
}
void CPlazaViewItem::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);


	if(!bShow)
	{
		m_dlgChongQingSSC.ShowWindow(SW_HIDE);
		m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
		m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
		m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
		m_dlgChongQing11x5.ShowWindow(SW_HIDE);
		m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
		m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
		m_dlgShanDong11x5.ShowWindow(SW_HIDE);
		m_dlgFenFenCai.ShowWindow(SW_HIDE);
		m_dlgWuFenCai.ShowWindow(SW_HIDE);
		m_dlgLiuHeCai.ShowWindow(SW_HIDE);
		m_dlgQiXingCai.ShowWindow(SW_HIDE);//--lly
		m_dlgPaiLie3.ShowWindow(SW_HIDE);
		m_dlg3D.ShowWindow(SW_HIDE);
		m_dlgBjPK10.ShowWindow(SW_HIDE);
		m_dlgBjKuai8.ShowWindow(SW_HIDE);
		m_dlgXingYun28.ShowWindow(SW_HIDE);
	}
	else if(bShow)
	{
		if(theAccount.user_id <= 100)
			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
		else
		{
			m_nNowTypeID = CZChongQingSSC;
			m_dlgChongQingSSC.SetTypeID(CZChongQingSSC);
			m_dlgChongQingSSC.ShowWindow(SW_SHOW);
		}


		m_logo.Navigate(m_strWebUrl,NULL,NULL,NULL,NULL);
		m_logo.EnableWindow(false);

	}
	return;
}
VOID CPlazaViewItem::ShowTypeItemView()
{
	m_cbShowItemMode = VIEW_MODE_TYPE;
	m_dlgChongQingSSC.ShowWindow(SW_HIDE);
// 	m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
// 	m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
	m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
// 	m_dlgChongQing11x5.ShowWindow(SW_HIDE);
// 	m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 	m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
// 	m_dlgShanDong11x5.ShowWindow(SW_HIDE);
// 	m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 	m_dlgWuFenCai.ShowWindow(SW_HIDE);
	m_dlgLiuHeCai.ShowWindow(SW_HIDE);
	m_dlgQiXingCai.ShowWindow(SW_HIDE);//--lly
	m_dlgPaiLie3.ShowWindow(SW_HIDE);
	m_dlg3D.ShowWindow(SW_HIDE);
	m_dlgBjPK10.ShowWindow(SW_HIDE);
	m_dlgBjKuai8.ShowWindow(SW_HIDE);
	m_dlgXingYun28.ShowWindow(SW_HIDE);

	m_btnReturn.ShowWindow(SW_HIDE);
	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

	//CreateDlgCaipiao(1);
}
//显示类型
VOID CPlazaViewItem::ShowKindItemView(WORD wTypeID,BOOL bResetPageIndex)
{
	if(m_bCreateType1)
	{
		m_bCreateType1= false;
		CreateDlgCaipiao(4);
	}

	m_btnReturn.ShowWindow(SW_SHOW);
	if (m_dlgChongQingSSC.IsWindowVisible())
	{
		m_dlgChongQingSSC.ShowWindow(SW_HIDE);
	}
// 	else if (m_dlgJiangXiSSC.IsWindowVisible())
// 	{
// 		m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
// 	}
// 	else if (m_dlgXinjiangSSC.IsWindowVisible())
// 	{
// 		m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
// 	}
// 	else if (m_dlgWuFenCai.IsWindowVisible())
// 	{
// 		m_dlgWuFenCai.ShowWindow(SW_HIDE);
// 	}
// 	else if (m_dlgFenFenCai.IsWindowVisible())
// 	{
// 		m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 	}
	else if (m_dlgGuangdong11x5.IsWindowVisible())
	{
		m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
	}
// 	else if (m_dlgChongQing11x5.IsWindowVisible())
// 	{
// 		m_dlgChongQing11x5.ShowWindow(SW_HIDE);
// 	}
// 	else if (m_dlgJiangXi11x5.IsWindowVisible())
// 	{
// 		m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
// 	}
// 	else if (m_dlgShanDong11x5.IsWindowVisible())
// 	{
// 		m_dlgShanDong11x5.ShowWindow(SW_HIDE);
// 	}
// 	else if (m_dlgHeiLongJiang11X5.IsWindowVisible())
// 	{
// 		m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 	}
	else if (m_dlgLiuHeCai.IsWindowVisible())
	{
		m_dlgLiuHeCai.ShowWindow(SW_HIDE);
	}
	else if (m_dlg3D.IsWindowVisible())
	{
		m_dlg3D.ShowWindow(SW_HIDE);
	}
	else if (m_dlgPaiLie3.IsWindowVisible())
	{
		m_dlgPaiLie3.ShowWindow(SW_HIDE);
	}
	else if (m_dlgQiXingCai.IsWindowVisible())
	{
		m_dlgQiXingCai.ShowWindow(SW_HIDE);
	}
	else if (m_dlgBjPK10.IsWindowVisible())
	{
		m_dlgBjPK10.ShowWindow(SW_HIDE);
	}
	else if (m_dlgBjKuai8.IsWindowVisible())
	{
		m_dlgBjKuai8.ShowWindow(SW_HIDE);
	}
	else if (m_dlgXingYun28.IsWindowVisible())
	{
		m_dlgXingYun28.ShowWindow(SW_HIDE);
	}

	//获取对象
	ASSERT(CServerListData::GetInstance()!=NULL);
	CServerListData * pServerListData=CServerListData::GetInstance();

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//变量定义
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;
	tagGameKindInfo * pGameKindInfo=NULL;

	//删除对象
	for (INT i=0;i<m_GameKindInfoActive.GetCount();i++)
	{
		//获取对象
		ASSERT(m_GameKindInfoActive[i]!=NULL);
		pGameKindInfo=m_GameKindInfoActive[i];

		//设置变量
		pGameKindInfo->wSortID=0;
		pGameKindInfo->pGameKindItem=NULL;

		//清理对象
		if (pGameKindInfo->ImageKindItem.IsNull()==false)
		{
			pGameKindInfo->ImageKindItem.DestroyImage();
		}
	}

	//删除对象
	m_GameKindInfoBuffer.Append(m_GameKindInfoActive);
	m_GameKindInfoActive.RemoveAll();

	//插入对象
	while (true)
	{
		//获取对象
		pGameKindItem=pServerListData->EmunGameKindItem(Position);

		//对象判断
		if (pGameKindItem==NULL) break;

		//类型判断
		if ((wTypeID!=0)&&(pGameKindItem->m_GameKind.wTypeID!=wTypeID))
		{
			if (Position==NULL) break;
			if (Position!=NULL) continue;
		}

		//获取对象
		if (m_GameKindInfoBuffer.GetCount()>0L)
		{
			//获取对象
			INT_PTR nCount=m_GameKindInfoBuffer.GetCount();
			pGameKindInfo=m_GameKindInfoBuffer[nCount-1L];

			//删除对象
			ASSERT(pGameKindInfo!=NULL);
			m_GameKindInfoBuffer.RemoveAt(nCount-1L);
		}
		else
		{
			try
			{
				pGameKindInfo=new tagGameKindInfo;
				if (pGameKindInfo==NULL) throw TEXT("创建对象失败");
			}
			catch (...)
			{
				ASSERT(FALSE);
				break;
			}
		}

		//设置对象
		pGameKindInfo->pGameKindItem=pGameKindItem;
		pGameKindInfo->wSortID=pGameKindItem->m_GameKind.wSortID;

		//获取目录
		TCHAR szGameDirectory[LEN_PROCESS]=TEXT("");
		GetGameDirectory(szGameDirectory,CountArray(szGameDirectory),pGameKindItem->m_GameKind);

		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\ADImage\\GameKind\\GameKind_%s.PNG"),szDirectory,szGameDirectory);

		//加载图片
		if (pGameKindInfo->ImageKindItem.LoadImage(szImagePath)==false)
		{
			HINSTANCE hInstance=AfxGetInstanceHandle();
			pGameKindInfo->ImageKindItem.LoadImage(hInstance,TEXT("UNKNOWN_KIND"));
		}

		//插入对象
		INT nItem=0;
		for (nItem=0;nItem<m_GameKindInfoActive.GetCount();nItem++)
		{
			//获取对象
			ASSERT(m_GameKindInfoActive[nItem]!=NULL);
			tagGameKindInfo * pGameKindTemp=m_GameKindInfoActive[nItem];

			//排序判断
			if (pGameKindInfo->wSortID<pGameKindTemp->wSortID)
			{
				m_GameKindInfoActive.InsertAt(nItem,pGameKindInfo);
				break;
			}
		}

		//默认插入
		if (nItem==m_GameKindInfoActive.GetCount())
		{
			m_GameKindInfoActive.Add(pGameKindInfo);
		}


		//结束判断
		if (Position==NULL)
		{
			break;
		}
	}



	//设置变量
	m_cbButtonDown=0;
	m_cbButtonHover=0;
	m_wKindTypeCurrentID=wTypeID;
	m_wViewItemDown=INVALID_WORD;
	m_wViewItemHover=INVALID_WORD;
	m_cbShowItemMode=VIEW_MODE_KIND;

	//设置索引
	if(bResetPageIndex==TRUE) m_wKindPageCurrent=0;

	//调整界面
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

	return;
}

//显示房间
VOID CPlazaViewItem::ShowServerItemView(WORD wKindID,BOOL bResetPageIndex)
{
	m_dlgChongQingSSC.ShowWindow(SW_HIDE);
	m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
	m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
	m_dlgFenFenCai.ShowWindow(SW_HIDE);
	m_dlgLiuHeCai.ShowWindow(SW_HIDE);
	m_dlgWuFenCai.ShowWindow(SW_HIDE);
	m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
	m_dlgChongQing11x5.ShowWindow(SW_HIDE);
	m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
	m_dlgShanDong11x5.ShowWindow(SW_HIDE);
	m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
	m_dlgQiXingCai.ShowWindow(SW_HIDE);//--lly
	m_dlgPaiLie3.ShowWindow(SW_HIDE);
	m_dlg3D.ShowWindow(SW_HIDE);
	m_dlgBjPK10.ShowWindow(SW_HIDE);
	m_dlgBjKuai8.ShowWindow(SW_HIDE);
	m_dlgXingYun28.ShowWindow(SW_HIDE);

	m_nNowTypeID = wKindID;

	if(wKindID == CZChongQingSSC)
	{
		m_dlgChongQingSSC.SetTypeID(CZChongQingSSC);
		m_nNowTypeID = CZChongQingSSC;

		m_dlgChongQingSSC.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;

		return;
	}
	else if(wKindID == CZ_TianJinSSC)
	{
		m_nNowTypeID = CZ_TianJinSSC;

		m_dlgChongQingSSC.SetTypeID(CZ_TianJinSSC);
		m_dlgChongQingSSC.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZXinJiangSSC)
	{
		m_nNowTypeID = CZXinJiangSSC;

		m_dlgChongQingSSC.SetTypeID(CZXinJiangSSC);
		m_dlgChongQingSSC.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZ_FENFEN_CAI)
	{
		m_nNowTypeID = CZ_FENFEN_CAI;

		m_dlgChongQingSSC.SetTypeID(CZ_FENFEN_CAI);
		m_dlgChongQingSSC.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZ_WUFEN_CAI)
	{
		m_nNowTypeID = CZ_WUFEN_CAI;

		m_dlgChongQingSSC.SetTypeID(CZ_WUFEN_CAI);
		m_dlgChongQingSSC.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZ_HGYDWFC)
	{
		m_nNowTypeID = CZ_HGYDWFC;

		m_dlgChongQingSSC.SetTypeID(CZ_HGYDWFC);
		m_dlgChongQingSSC.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZGD11Xuan5)
	{
		m_nNowTypeID = CZGD11Xuan5;

		m_dlgGuangdong11x5.SetTypeID(CZGD11Xuan5);
		m_dlgGuangdong11x5.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZCQ11Xuan5)
	{
		m_nNowTypeID = CZCQ11Xuan5;

		m_dlgChongQing11x5.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZJX11Xuan5)
	{
		m_nNowTypeID = CZJX11Xuan5;

		m_dlgJiangXi11x5.SetTypeID(CZJX11Xuan5);
		m_dlgJiangXi11x5.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZSD11Xuan5)
	{
		m_nNowTypeID = CZSD11Xuan5;

		m_dlgShanDong11x5.SetTypeID(CZSD11Xuan5);
		m_dlgShanDong11x5.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZ_PK10)
	{
		m_nNowTypeID = CZ_PK10;

		m_dlgBjPK10.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZKUAILE8)
	{
		m_nNowTypeID = CZKUAILE8;

		m_dlgBjKuai8.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZXingYun28)
	{
		m_nNowTypeID = CZXingYun28;

		m_dlgXingYun28.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZHLJ11Xuan5)
	{
		m_nNowTypeID = CZHLJ11Xuan5;

		m_dlgHeiLongJiang11X5.SetTypeID(CZHLJ11Xuan5);
		m_dlgHeiLongJiang11X5.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZ_LIUHECAI)
	{
		m_nNowTypeID = CZ_LIUHECAI;

		m_dlgLiuHeCai.ShowWindow(SW_SHOW);
		m_btnReturn.ShowWindow(SW_HIDE);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if (wKindID == CZ_QiXingCai)//--lly
	{
		m_nNowTypeID = CZ_QiXingCai;

		m_dlgQiXingCai.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZPaiLie3)
	{
		m_nNowTypeID = CZPaiLie3;

		m_dlgPaiLie3.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}
	else if(wKindID == CZ3D)
	{
		m_nNowTypeID = CZ3D;

		m_dlg3D.ShowWindow(SW_SHOW);
		m_cbShowItemMode = VIEW_MODE_GAME;
		return;
	}

	m_cbShowItemMode = VIEW_MODE_SERVER;
	//获取对象
	ASSERT(CServerListData::GetInstance()!=NULL);
	CServerListData * pServerListData=CServerListData::GetInstance();

	//查找类型
	ASSERT(pServerListData->SearchGameKind(wKindID)!=NULL);
	CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(wKindID);

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//获取目录
	TCHAR szGameDirectory[LEN_PROCESS]=TEXT("");
	GetGameDirectory(szGameDirectory,CountArray(szGameDirectory),pGameKindItem->m_GameKind);

	//构造路径
	TCHAR szImageTitle[MAX_PATH]=TEXT("");
	TCHAR szImageServer[MAX_PATH]=TEXT("");
	_sntprintf(szImageTitle,CountArray(szImageTitle),TEXT("%s\\ADImage\\GameTitle\\GameTitle_%s.PNG"),szDirectory,szGameDirectory);
	_sntprintf(szImageServer,CountArray(szImageServer),TEXT("%s\\ADImage\\GameTitle\\GameServer_%s.PNG"),szDirectory,szGameDirectory);

	//销毁图片
	//if (m_ImageTitle.IsNull()==false) m_ImageTitle.DestroyImage();
	if (m_ImageServer.IsNull()==false) m_ImageServer.DestroyImage();

// 	//加载图片
// 	if (m_ImageTitle.LoadImage(szImageTitle)==false)
// 	{
// 		HINSTANCE hInstance=AfxGetInstanceHandle();
// 		m_ImageTitle.LoadImage(hInstance,TEXT("UNKNOWN_TITLE"));
// 	}

	//加载图片
	if (m_ImageServer.LoadImage(szImageServer)==false)
	{
		HINSTANCE hInstance=AfxGetInstanceHandle();
		m_ImageServer.LoadImage(hInstance,TEXT("UNKNOWN_SERVER"));
	}

	//变量定义
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;
	tagGameServerInfo * pGameServerInfo=NULL;

	//删除对象
	for (INT i=0;i<m_GameServerInfoActive.GetCount();i++)
	{
		//获取对象
		ASSERT(m_GameServerInfoActive[i]!=NULL);
		pGameServerInfo=m_GameServerInfoActive[i];

		//设置变量
		pGameServerInfo->wSortID=0;
		pGameServerInfo->wServerID=0;
		pGameServerInfo->pGameServerItem=NULL;
	}

	//删除对象
	m_GameServerInfoBuffer.Append(m_GameServerInfoActive);
	m_GameServerInfoActive.RemoveAll();

	//插入对象
	while (true)
	{
		//获取对象
		pGameServerItem=pServerListData->EmunGameServerItem(Position);

		//对象判断
		if (pGameServerItem==NULL) break;

		//类型判断
		if (pGameServerItem->m_GameServer.wKindID!=wKindID)
		{
			if (Position==NULL) break;
			if (Position!=NULL) continue;
		}

		//获取对象
		if (m_GameServerInfoBuffer.GetCount()>0L)
		{
			//获取对象
			INT_PTR nCount=m_GameServerInfoBuffer.GetCount();
			pGameServerInfo=m_GameServerInfoBuffer[nCount-1L];

			//删除对象
			ASSERT(pGameServerInfo!=NULL);
			m_GameServerInfoBuffer.RemoveAt(nCount-1L);
		}
		else
		{
			try
			{
				pGameServerInfo=new tagGameServerInfo;
				if (pGameServerInfo==NULL) throw TEXT("创建对象失败");
			}
			catch (...)
			{
				ASSERT(FALSE);
				break;
			}
		}

		//设置对象
		pGameServerInfo->pGameServerItem=pGameServerItem;
		pGameServerInfo->wSortID=pGameServerItem->m_GameServer.wSortID;
		pGameServerInfo->wServerID=pGameServerItem->m_GameServer.wServerID;

		//插入对象
		INT nItem=0;
		for (nItem=0;nItem<m_GameServerInfoActive.GetCount();nItem++)
		{
			//获取对象
			ASSERT(m_GameServerInfoActive[nItem]!=NULL);
			tagGameServerInfo * pGameServerTemp=m_GameServerInfoActive[nItem];

			//排序判断
			if (pGameServerInfo->wSortID<pGameServerTemp->wSortID)
			{
				m_GameServerInfoActive.InsertAt(nItem,pGameServerInfo);
				break;
			}
		}

		//默认插入
		if (nItem==m_GameServerInfoActive.GetCount())
		{
			m_GameServerInfoActive.Add(pGameServerInfo);
		}

		//结束判断
		if (Position==NULL)
		{
			break;
		}
	}

	//无效判断
	if (m_GameServerInfoActive.GetCount()==0)
	{
		//提示信息
		CInformation Information;
		Information.ShowMessageBox(TEXT("此游戏暂时没有可以进入的游戏房间，请选择其他游戏！"),MB_ICONINFORMATION,30L);

		return;
	}

	//设置变量
	m_cbButtonDown=0;
	m_cbButtonHover=0;
	m_wViewItemDown=INVALID_WORD;
	m_wViewItemHover=INVALID_WORD;
	m_cbShowItemMode=VIEW_MODE_SERVER;

	//设置索引
	if(bResetPageIndex==TRUE) m_wServerPageCurrent=0;

	//调整界面
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

	return;
}

//位置消息
VOID CPlazaViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);
	RectifyDlg();

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

	return;
}
//对象索引
WORD CPlazaViewItem::GetGameHoverIndex(CPoint MousePoint)
{
	if(!m_GameListRect.PtInRect(MousePoint))
	{
		
		return INVALID_WORD;
	}
	WORD wHoverItem = INVALID_WORD;

// 	wHoverItem = MousePoint.y/m_ImageItemBackMid.GetHeight();
// 	if(MousePoint.y%m_ImageItemBackMid.GetHeight()>0)
// 	{
// 		wHoverItem+=1;
// 	}
// 	wHoverItem-=1;
	return wHoverItem;
}
//对象索引
WORD CPlazaViewItem::GetHoverIndex(CPoint MousePoint)
{
	switch (m_cbShowItemMode)
	{
	case VIEW_MODE_TYPE:
		{
			for (BYTE i = 0;i<3;i++)
			{
				if(m_rcTypeRect[i].PtInRect(MousePoint))
				{
					return i;
				}
			}
			return INVALID_WORD;
		}
		//游戏模式，只要不返回INVALID_WORD就好
	case VIEW_MODE_GAME:
		{

			return VIEW_MODE_GAME;
		}
	case VIEW_MODE_KIND:	//类型视图
		{
			//无效判断
			MousePoint.y-=40;
			if (MousePoint.x<m_wKindExcursion) return INVALID_WORD;
			if (MousePoint.y<GAME_KIND_ITEM_PY) return INVALID_WORD;

			//位置计算
			INT nXItem=(MousePoint.x-m_wKindExcursion)/(GAME_KIND_ITEM_CX+GAME_KIND_ITEM_PX);
			INT nYItem=(MousePoint.y-GAME_KIND_ITEM_PY)/(GAME_KIND_ITEM_CY+GAME_KIND_ITEM_PY);

			//溢出判断
			if ((MousePoint.x-m_wKindExcursion-(nXItem*(GAME_KIND_ITEM_CX+GAME_KIND_ITEM_PX)))>GAME_KIND_ITEM_CX) return INVALID_WORD;
			if ((MousePoint.y-GAME_KIND_ITEM_PY-(nYItem*(GAME_KIND_ITEM_CY+GAME_KIND_ITEM_PY)))>GAME_KIND_ITEM_CY) return INVALID_WORD;

			//结果判断
			if ((nYItem>=0)&&(nXItem>=0)&&(nYItem<m_wKindYCount)&&(nXItem<m_wKindXCount))
			{
				WORD wViewCount=m_wKindXCount*m_wKindYCount;
				WORD wResultItem=nYItem*m_wKindXCount+nXItem;
				if ((m_wKindPageCurrent*wViewCount+wResultItem)<m_GameKindInfoActive.GetCount()) return wResultItem;
			}

			return INVALID_WORD;
		}
	case VIEW_MODE_SERVER:	//房间视图
		{
			//无效判断
			if (MousePoint.x<m_wServerExcursion) return INVALID_WORD;
			if (MousePoint.y<GAME_SERVER_ITEM_PY) return INVALID_WORD;

			//位置计算
			INT nXItem=(MousePoint.x-m_wServerExcursion)/(GAME_SERVER_ITEM_CX+GAME_SERVER_ITEM_PX);
			INT nYItem=(MousePoint.y-GAME_SERVER_ITEM_PY-GAME_TITLE_CY)/(GAME_SERVER_ITEM_CY+GAME_SERVER_ITEM_PY);

			//溢出判断
			if ((MousePoint.x-m_wServerExcursion-(nXItem*(GAME_SERVER_ITEM_CX+GAME_SERVER_ITEM_PX)))>GAME_SERVER_ITEM_CX) return INVALID_WORD;
			if ((MousePoint.y-GAME_SERVER_ITEM_PY-GAME_TITLE_CY-(nYItem*(GAME_SERVER_ITEM_CY+GAME_SERVER_ITEM_PY)))>GAME_SERVER_ITEM_CY) return INVALID_WORD;

			//结果判断
			if ((nYItem>=0)&&(nXItem>=0)&&(nYItem<m_wServerYCount)&&(nXItem<m_wServerXCount))
			{
				WORD wViewCount=m_wServerXCount*m_wServerYCount;
				WORD wResultItem=nYItem*m_wServerXCount+nXItem;
				if ((m_wServerPageCurrent*wViewCount+wResultItem)<m_GameServerInfoActive.GetCount()) return wResultItem;
			}

			return INVALID_WORD;
		}
	}

	return INVALID_WORD;
}

//按钮测试
BYTE CPlazaViewItem::GetHoverButton(WORD wViewIndex, CPoint MousePoint)
{
	//无效判断
	if (wViewIndex==INVALID_WORD) return 0;

	//按钮检测
	switch (m_cbShowItemMode)
	{
	case VIEW_MODE_TYPE:
		{
			for (BYTE i = 0;i<3;i++)
			{
				if(m_rcTypeRect[i].PtInRect(MousePoint))
				{
					return i;
				}
			}
			return INVALID_WORD;
		}
	case VIEW_MODE_GAME:
		{
			CRect rcOpenLottery(560,3,750,42);
			if(rcOpenLottery.PtInRect(MousePoint))
			{
				//MessageBox(L"开奖记录");			
				return BT_OPEN_LOTTERY;
			}
			break;
		}
	case VIEW_MODE_KIND:	//类型视图
		{
			//设置位置
// 			MousePoint.x-=(m_wKindExcursion+(wViewIndex%m_wKindXCount)*(GAME_KIND_ITEM_CX+GAME_KIND_ITEM_PX));
// 			MousePoint.y-=(GAME_SERVER_ITEM_PY+(wViewIndex/m_wKindXCount)*(GAME_KIND_ITEM_CY+GAME_KIND_ITEM_PY));
			MousePoint.x-=(m_wKindExcursion+(wViewIndex%m_wKindXCount)*(GAME_KIND_ITEM_CX+GAME_KIND_ITEM_PX));
			MousePoint.y-=(GAME_SERVER_ITEM_PY+(wViewIndex/m_wKindXCount)*(GAME_KIND_ITEM_CY+GAME_KIND_ITEM_PY));

			MousePoint.y-=40;
			//进入房间
			CRect rcEnterKind(0,0,GAME_KIND_ITEM_CX,GAME_KIND_ITEM_CY);
			if (rcEnterKind.PtInRect(MousePoint)==TRUE) return BT_ENTER_KIND;

			break;
		}
	case VIEW_MODE_SERVER:	//房间视图
		{
			//设置位置
			MousePoint.x-=(m_wServerExcursion+(wViewIndex%m_wServerXCount)*(GAME_SERVER_ITEM_CX+GAME_SERVER_ITEM_PX));
			MousePoint.y-=(GAME_SERVER_ITEM_PY+GAME_TITLE_CY+(wViewIndex/m_wServerXCount)*(GAME_SERVER_ITEM_CY+GAME_SERVER_ITEM_PY));

			//进入房间
			CRect rcEnterServer(7,179,178,219);
			if (rcEnterServer.PtInRect(MousePoint)==TRUE) return BT_ENTER_SERVER;

			break;
		}
	}

	return 0;
}

//查看规则
VOID CPlazaViewItem::OnButtonViewRule(WORD wKindID)
{
	//构造地址
	TCHAR szRuleLink[MAX_PATH]=TEXT("");
	_sntprintf(szRuleLink,CountArray(szRuleLink),TEXT("%s/GameDetails.aspx?KID=%ld"),szPlatformLink,wKindID);

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szRuleLink,NULL,NULL,SW_NORMAL);

	return;
}

//进入类型
VOID CPlazaViewItem::OnButtonEnterKind(WORD wKindID)
{
	//查找对象
	CServerListData * pServerListData=CServerListData::GetInstance();
	CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(wKindID);

	m_btnReturn.ShowWindow(SW_SHOW);
	BringWindowToTop();

	m_nNowTypeID = wKindID;
	//事件处理
	if (pGameKindItem!=NULL)
	{
		CaiZhong wKindID = (CaiZhong)pGameKindItem->m_GameKind.wKindID;
		if(wKindID == CZChongQingSSC||wKindID == CZ_TianJinSSC||wKindID == CZXinJiangSSC||wKindID == CZ_FENFEN_CAI||wKindID == CZ_WUFEN_CAI||wKindID == CZ_HGYDWFC)
		{
			m_dlgChongQingSSC.SetTypeID(wKindID);
			m_dlgChongQingSSC.ShowWindow(SW_SHOW);
// 			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
// 			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 			m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 			m_dlgWuFenCai.ShowWindow(SW_HIDE);
			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
			m_dlgPaiLie3.ShowWindow(SW_HIDE);
			m_dlg3D.ShowWindow(SW_HIDE);
			m_dlgBjPK10.ShowWindow(SW_HIDE);
			m_dlgBjKuai8.ShowWindow(SW_HIDE);
			m_dlgXingYun28.ShowWindow(SW_HIDE);

			m_cbShowItemMode = VIEW_MODE_GAME;
		}
// 		else if(pGameKindItem->m_GameKind.wKindID == CZ_TianJinSSC)
// 		{
// 			m_dlgChongQingSSC.SetTypeID(CZ_TianJinSSC);
// 			m_dlgChongQingSSC.ShowWindow(SW_SHOW);
// 			//m_dlgChongQingSSC.ShowWindow(SW_HIDE);
// 			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
// 			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
// 			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
// 			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
// 			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
// 			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 			m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 			m_dlgWuFenCai.ShowWindow(SW_HIDE);
// 			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
// 			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
// 			m_dlgPaiLie3.ShowWindow(SW_HIDE);
// 			m_dlg3D.ShowWindow(SW_HIDE);
// 			m_dlgBjPK10.ShowWindow(SW_HIDE);
// 			m_dlgBjKuai8.ShowWindow(SW_HIDE);
// 			m_dlgXingYun28.ShowWindow(SW_HIDE);
// 
// 			m_cbShowItemMode = VIEW_MODE_GAME;
// 		}
// 		else if(pGameKindItem->m_GameKind.wKindID == CZXinJiangSSC)
// 		{
// 			m_dlgXinjiangSSC.SetTypeID(CZXinJiangSSC);
// 			m_dlgXinjiangSSC.ShowWindow(SW_SHOW);
// 			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
// 			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
// 			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
// 			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
// 			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
// 			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
// 			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 			m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 			m_dlgWuFenCai.ShowWindow(SW_HIDE);
// 			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
// 			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
// 			m_dlgPaiLie3.ShowWindow(SW_HIDE);
// 			m_dlg3D.ShowWindow(SW_HIDE);
// 			m_dlgBjPK10.ShowWindow(SW_HIDE);
// 			m_dlgBjKuai8.ShowWindow(SW_HIDE);
// 			m_dlgXingYun28.ShowWindow(SW_HIDE);
// 
// 			m_cbShowItemMode = VIEW_MODE_GAME;
// 		}
// 		else if(pGameKindItem->m_GameKind.wKindID == CZ_FENFEN_CAI)
// 		{
// 			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
// 			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
// 			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
// 			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
// 			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
// 			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
// 			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
// 			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 			m_dlgFenFenCai.SetTypeID(CZ_FENFEN_CAI);
// 			m_dlgFenFenCai.ShowWindow(SW_SHOW);
// 			m_dlgWuFenCai.ShowWindow(SW_HIDE);
// 			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
// 			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
// 			m_dlgPaiLie3.ShowWindow(SW_HIDE);
// 			m_dlg3D.ShowWindow(SW_HIDE);
// 			m_dlgBjPK10.ShowWindow(SW_HIDE);
// 			m_dlgBjKuai8.ShowWindow(SW_HIDE);
// 			m_dlgXingYun28.ShowWindow(SW_HIDE);
// 
// 			m_cbShowItemMode = VIEW_MODE_GAME;
// 		}
// 		else if(pGameKindItem->m_GameKind.wKindID == CZ_WUFEN_CAI)
// 		{
// 			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
// 			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
// 			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
// 			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
// 			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
// 			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
// 			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
// 			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 			m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 			m_dlgWuFenCai.SetTypeID(CZ_WUFEN_CAI);
// 			m_dlgWuFenCai.ShowWindow(SW_SHOW);
// 			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
// 			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
// 			m_dlgPaiLie3.ShowWindow(SW_HIDE);
// 			m_dlg3D.ShowWindow(SW_HIDE);
// 			m_dlgBjPK10.ShowWindow(SW_HIDE);
// 			m_dlgBjKuai8.ShowWindow(SW_HIDE);
// 			m_dlgXingYun28.ShowWindow(SW_HIDE);
// 
// 			m_cbShowItemMode = VIEW_MODE_GAME;
// 		}
		else if(wKindID == CZGD11Xuan5||wKindID == CZJX11Xuan5||wKindID == CZSD11Xuan5||wKindID == CZHLJ11Xuan5)
		{
//			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
// 			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
			m_dlgGuangdong11x5.SetTypeID(wKindID);
			m_dlgGuangdong11x5.ShowWindow(SW_SHOW);
// 			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
// 			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
// 			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
// 			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 			m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 			m_dlgWuFenCai.ShowWindow(SW_HIDE);
			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
			m_dlgPaiLie3.ShowWindow(SW_HIDE);
			m_dlg3D.ShowWindow(SW_HIDE);
			m_dlgBjPK10.ShowWindow(SW_HIDE);
			m_dlgBjKuai8.ShowWindow(SW_HIDE);
			m_dlgXingYun28.ShowWindow(SW_HIDE);

			m_cbShowItemMode = VIEW_MODE_GAME;
		}
// 		else if(pGameKindItem->m_GameKind.wKindID == CZCQ11Xuan5)
// 		{
// 			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
// 			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
// 			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
// 			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
// 			m_dlgChongQing11x5.ShowWindow(SW_SHOW);
// 			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
// 			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
// 			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 			m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 			m_dlgWuFenCai.ShowWindow(SW_HIDE);
// 			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
// 			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
// 			m_dlgPaiLie3.ShowWindow(SW_HIDE);
// 			m_dlg3D.ShowWindow(SW_HIDE);
// 			m_dlgBjPK10.ShowWindow(SW_HIDE);
// 			m_dlgBjKuai8.ShowWindow(SW_HIDE);
// 			m_dlgXingYun28.ShowWindow(SW_HIDE);
// 
// 			m_cbShowItemMode = VIEW_MODE_GAME;
// 		}
// 		else if(pGameKindItem->m_GameKind.wKindID == CZJX11Xuan5)
// 		{
// 			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
// 			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
// 			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
// 			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
// 			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
// 			m_dlgJiangXi11x5.SetTypeID(CZJX11Xuan5);
// 			m_dlgJiangXi11x5.ShowWindow(SW_SHOW);
// 			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
// 			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 			m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 			m_dlgWuFenCai.ShowWindow(SW_HIDE);
// 			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
// 			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
// 			m_dlgPaiLie3.ShowWindow(SW_HIDE);
// 			m_dlg3D.ShowWindow(SW_HIDE);
// 			m_dlgBjPK10.ShowWindow(SW_HIDE);
// 			m_dlgBjKuai8.ShowWindow(SW_HIDE);
// 			m_dlgXingYun28.ShowWindow(SW_HIDE);
// 
// 			m_cbShowItemMode = VIEW_MODE_GAME;
// 		}
// 		else if(pGameKindItem->m_GameKind.wKindID == CZSD11Xuan5)
// 		{
// 			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
// 			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
// 			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
// 			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
// 			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
// 			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
// 			m_dlgShanDong11x5.SetTypeID(CZSD11Xuan5);
// 			m_dlgShanDong11x5.ShowWindow(SW_SHOW);
// 			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
// 			m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
// 			m_dlgWuFenCai.ShowWindow(SW_HIDE);
// 			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
// 			m_dlgPaiLie3.ShowWindow(SW_HIDE);
// 			m_dlg3D.ShowWindow(SW_HIDE);
// 			m_dlgBjPK10.ShowWindow(SW_HIDE);
// 			m_dlgBjKuai8.ShowWindow(SW_HIDE);
// 			m_dlgXingYun28.ShowWindow(SW_HIDE);
// 
// 			m_cbShowItemMode = VIEW_MODE_GAME;
// 		}
		else if(pGameKindItem->m_GameKind.wKindID == CZ_PK10)
		{
			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
			m_dlgFenFenCai.ShowWindow(SW_HIDE);
			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
			m_dlgWuFenCai.ShowWindow(SW_HIDE);
			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
			m_dlgPaiLie3.ShowWindow(SW_HIDE);
			m_dlg3D.ShowWindow(SW_HIDE);
			m_dlg3D.ShowWindow(SW_HIDE);
			m_dlgBjPK10.ShowWindow(SW_SHOW);
			m_dlgBjKuai8.ShowWindow(SW_HIDE);
			m_dlgXingYun28.ShowWindow(SW_HIDE);

			m_cbShowItemMode = VIEW_MODE_GAME;
		}
		else if(pGameKindItem->m_GameKind.wKindID == CZ_LIUHECAI)
		{
			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
			m_dlgFenFenCai.ShowWindow(SW_HIDE);
			m_dlgWuFenCai.ShowWindow(SW_HIDE);
			m_dlgLiuHeCai.ShowWindow(SW_SHOW);
			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
			m_dlgPaiLie3.ShowWindow(SW_HIDE);
			m_dlg3D.ShowWindow(SW_HIDE);
			m_dlgBjPK10.ShowWindow(SW_HIDE);
			m_dlgBjKuai8.ShowWindow(SW_HIDE);
			m_dlgXingYun28.ShowWindow(SW_HIDE);
			//m_btnReturn.ShowWindow(SW_HIDE);
			m_cbShowItemMode = VIEW_MODE_GAME;

		}
// 		else if(pGameKindItem->m_GameKind.wKindID == CZHLJ11Xuan5)
// 		{
// 			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
// 			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
// 			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
// 			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
// 			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
// 			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
// 			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
// 			m_dlgHeiLongJiang11X5.SetTypeID(CZHLJ11Xuan5);
// 			m_dlgHeiLongJiang11X5.ShowWindow(SW_SHOW);
// 			m_dlgFenFenCai.ShowWindow(SW_HIDE);
// 			m_dlgWuFenCai.ShowWindow(SW_HIDE);
// 			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
// 			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
// 			m_dlgPaiLie3.ShowWindow(SW_HIDE);
// 			m_dlg3D.ShowWindow(SW_HIDE);
// 			m_dlgBjPK10.ShowWindow(SW_HIDE);
// 			m_dlgBjKuai8.ShowWindow(SW_HIDE);
// 			m_dlgXingYun28.ShowWindow(SW_HIDE);
// 
// 			m_cbShowItemMode = VIEW_MODE_GAME;
// 
// 		}
		else if(pGameKindItem->m_GameKind.wKindID == CZ_QiXingCai)
		{
			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
			m_dlgFenFenCai.ShowWindow(SW_HIDE);
			m_dlgWuFenCai.ShowWindow(SW_HIDE);
			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
			m_dlgQiXingCai.ShowWindow(SW_SHOW);//lly
			m_dlgPaiLie3.ShowWindow(SW_HIDE);
			m_dlg3D.ShowWindow(SW_HIDE);
			m_dlgBjPK10.ShowWindow(SW_HIDE);
			m_dlgBjKuai8.ShowWindow(SW_HIDE);
			m_dlgXingYun28.ShowWindow(SW_HIDE);

			m_cbShowItemMode = VIEW_MODE_GAME;

		}
		else if(pGameKindItem->m_GameKind.wKindID == CZPaiLie3)
		{
			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
			m_dlgFenFenCai.ShowWindow(SW_HIDE);
			m_dlgWuFenCai.ShowWindow(SW_HIDE);
			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
			m_dlgPaiLie3.ShowWindow(SW_SHOW);
			m_dlg3D.ShowWindow(SW_HIDE);
			m_dlgBjPK10.ShowWindow(SW_HIDE);
			m_dlgBjKuai8.ShowWindow(SW_HIDE);
			m_dlgXingYun28.ShowWindow(SW_HIDE);

			m_cbShowItemMode = VIEW_MODE_GAME;

		}
		else if(pGameKindItem->m_GameKind.wKindID == CZ3D)
		{
			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
			m_dlgFenFenCai.ShowWindow(SW_HIDE);
			m_dlgWuFenCai.ShowWindow(SW_HIDE);
			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
			m_dlgPaiLie3.ShowWindow(SW_HIDE);
			m_dlg3D.ShowWindow(SW_SHOW);
			m_dlgBjPK10.ShowWindow(SW_HIDE);
			m_dlgBjKuai8.ShowWindow(SW_HIDE);
			m_dlgXingYun28.ShowWindow(SW_HIDE);

			m_cbShowItemMode = VIEW_MODE_GAME;

		}
		else if(pGameKindItem->m_GameKind.wKindID == CZKUAILE8)
		{
			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
			m_dlgFenFenCai.ShowWindow(SW_HIDE);
			m_dlgWuFenCai.ShowWindow(SW_HIDE);
			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
			m_dlgPaiLie3.ShowWindow(SW_HIDE);
			m_dlg3D.ShowWindow(SW_HIDE);
			m_dlgBjPK10.ShowWindow(SW_HIDE);
			m_dlgBjKuai8.ShowWindow(SW_SHOW);
			m_dlgXingYun28.ShowWindow(SW_HIDE);

			m_cbShowItemMode = VIEW_MODE_GAME;

		}
		else if(pGameKindItem->m_GameKind.wKindID == CZXingYun28)
		{
			m_dlgChongQingSSC.ShowWindow(SW_HIDE);
			m_dlgJiangXiSSC.ShowWindow(SW_HIDE);
			m_dlgXinjiangSSC.ShowWindow(SW_HIDE);
			m_dlgGuangdong11x5.ShowWindow(SW_HIDE);
			m_dlgChongQing11x5.ShowWindow(SW_HIDE);
			m_dlgJiangXi11x5.ShowWindow(SW_HIDE);
			m_dlgShanDong11x5.ShowWindow(SW_HIDE);
			m_dlgHeiLongJiang11X5.ShowWindow(SW_HIDE);
			m_dlgFenFenCai.ShowWindow(SW_HIDE);
			m_dlgWuFenCai.ShowWindow(SW_HIDE);
			m_dlgLiuHeCai.ShowWindow(SW_HIDE);
			m_dlgQiXingCai.ShowWindow(SW_HIDE);//lly
			m_dlgPaiLie3.ShowWindow(SW_HIDE);
			m_dlg3D.ShowWindow(SW_HIDE);
			m_dlgBjPK10.ShowWindow(SW_HIDE);
			m_dlgBjKuai8.ShowWindow(SW_HIDE);
			m_dlgXingYun28.ShowWindow(SW_SHOW);

			m_cbShowItemMode = VIEW_MODE_GAME;

		}
		else if (pGameKindItem->m_dwProcessVersion==0L && pGameKindItem->m_GameKind.wKindID>100)
		{
			//安装游戏
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
			pGlobalUnits->DownLoadClient(pGameKindItem->m_GameKind.szKindName,wKindID,0);
		}
		else
		{
			//显示房间
			ShowServerItemView(wKindID);
		}
	}

	return;
}

//进入房间
VOID CPlazaViewItem::OnButtonEnterServer(WORD wServerID)
{
	//获取对象
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	CServerListData * pServerListData=CServerListData::GetInstance();

	m_cbShowItemMode = VIEW_MODE_SERVER;
	//进入房间
	CGameServerItem * pGameServerItem=pServerListData->SearchGameServer(wServerID);
	if (pGameServerItem!=NULL) pPlatformFrame->EntranceServerItem(pGameServerItem);

	return;
}

//上页按钮
VOID CPlazaViewItem::OnBnClickedLastKind()
{
	//效验状态
	ASSERT(m_wKindPageCurrent>0);
	if (m_wKindPageCurrent==0) return;

	//设置变量
	m_wKindPageCurrent--;

	//控制按钮
// 	m_btLastKind.EnableWindow((m_wKindPageCurrent>0)?TRUE:FALSE);
// 	m_btNextKind.EnableWindow(((m_wKindPageCurrent+1)<m_wKindPageCount)?TRUE:FALSE);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

	return;
}

//下页按钮
VOID CPlazaViewItem::OnBnClickedNextKind()
{
	//效验状态
	ASSERT((m_wKindPageCurrent+1)<m_wKindPageCount);
	if ((m_wKindPageCurrent+1)>=m_wKindPageCount) return;

	//设置变量
	m_wKindPageCurrent++;

	//控制按钮
// 	m_btLastKind.EnableWindow((m_wKindPageCurrent>0)?TRUE:FALSE);
// 	m_btNextKind.EnableWindow(((m_wKindPageCurrent+1)<m_wKindPageCount)?TRUE:FALSE);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

	return;
}

//返回按钮
VOID CPlazaViewItem::OnBnClickedReturnHall()
{
	//返回大厅
//	ShowKindItemView(m_wKindTypeCurrentID,FALSE);

	return;
}
 void CPlazaViewItem::OnBnClickedReturnUppage()
 {
	 if(m_nRecordKindID == 1000 && m_nRecordTypeID != 1000)
	 {
		 CWnd* pParent = GetParent();
		 if(pParent!=NULL)
		 {
			 pParent->PostMessage(IDM_RETURN_GAME,1,0);
		 }
		 m_nRecordTypeID=1000;
		 return;

	 }

	 m_nRecordKindID = 1000;
	 m_nNowTypeID = 0;
	 //m_btnReturn.ShowWindow(SW_HIDE);
	if(m_nRecordTypeID == 0)
	{
		ShowKindItemView(1);
	}
	else if(m_nRecordTypeID == 1)
	{
		ShowKindItemView(4);
	}
	else 
	{
	}

	return;
 }
//上页按钮
VOID CPlazaViewItem::OnBnClickedLastServer()
{
	//效验状态
	ASSERT(m_wServerPageCurrent>0);
	if (m_wServerPageCurrent==0) return;

	//设置变量
	m_wServerPageCurrent--;

	//控制按钮
// 	m_btLastServer.EnableWindow((m_wServerPageCurrent>0)?TRUE:FALSE);
// 	m_btNextServer.EnableWindow(((m_wServerPageCurrent+1)<m_wServerPageCount)?TRUE:FALSE);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

	return;
}

//下页按钮
VOID CPlazaViewItem::OnBnClickedNextServer()
{
	//效验状态
	ASSERT((m_wServerPageCurrent+1)<m_wServerPageCount);
	if ((m_wServerPageCurrent+1)>=m_wServerPageCount) return;

	//设置变量
	m_wServerPageCurrent++;

// 	//控制按钮
// 	m_btLastServer.EnableWindow((m_wServerPageCurrent>0)?TRUE:FALSE);
// 	m_btNextServer.EnableWindow(((m_wServerPageCurrent+1)<m_wServerPageCount)?TRUE:FALSE);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

	return;
}

//绘画背景
BOOL CPlazaViewItem::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建设备
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//绘画背景
	CBitImage ImageGround;
	ImageGround.LoadFromResource(AfxGetInstanceHandle(),IDB_PLAZA_GROUND1);
	ImageGround.DrawImageTile(pBufferDC,0,-3,ImageGround.GetWidth(),ImageGround.GetHeight()/*rcClient.Width(),rcClient.Height()*/);

	//绘画界面
	switch (m_cbShowItemMode)
	{
	case VIEW_MODE_TYPE:
		{
			//绘画背景
			CBitImage ImageGround;
			ImageGround.LoadFromResource(AfxGetInstanceHandle(),IDB_PLAZA_GROUND);
			ImageGround.DrawImageTile(pBufferDC,0,0,ImageGround.GetWidth(),ImageGround.GetHeight()/*rcClient.Width(),rcClient.Height()*/);

			if(!m_logo.IsWindowVisible())
				m_logo.ShowWindow(SW_SHOW);
			CPngImage ImageTypeCP;
			ImageTypeCP.LoadImage(AfxGetInstanceHandle(),TEXT("TYPE_CP"));

			//获取大小
			CSize SizeEnterType;
			SizeEnterType.SetSize(ImageTypeCP.GetWidth()/5L,ImageTypeCP.GetHeight());


			//进入游戏
			INT nXEnterIndexCp=0;
			bool bEnterButtonDownCp=((m_wViewItemDown==0)&&(m_cbButtonDown==0));
			bool bEnterButtonHoverCp=((m_wViewItemHover==0)&&(m_cbButtonHover==0));

			//进入游戏
			if ((bEnterButtonHoverCp==true)&&(bEnterButtonDownCp==true)) nXEnterIndexCp=1;
			if ((bEnterButtonHoverCp==true)&&(bEnterButtonDownCp==false)) nXEnterIndexCp=2;


		//	ImageEnterKind.DrawImage(pDC,nXPos,nYPos,SizeEnterKind.cx,SizeEnterKind.cy,nXEnterIndex*SizeEnterKind.cx,0);

			ImageTypeCP.DrawImage(pBufferDC,m_rcTypeRect[0].left,m_rcTypeRect[0].top,SizeEnterType.cx,SizeEnterType.cy,nXEnterIndexCp*SizeEnterType.cx,0);

			CPngImage ImageTypeQP;
			ImageTypeQP.LoadImage(AfxGetInstanceHandle(),TEXT("TYPE_QP"));

			//进入游戏
			INT nXEnterIndexQp=0;
			bool bEnterButtonDownQp=((m_wViewItemDown==1)&&(m_cbButtonDown==1));
			bool bEnterButtonHoverQp=((m_wViewItemHover==1)&&(m_cbButtonHover==1));

			//进入游戏
			if ((bEnterButtonHoverQp==true)&&(bEnterButtonDownQp==true)) nXEnterIndexQp=1;
			if ((bEnterButtonHoverQp==true)&&(bEnterButtonDownQp==false)) nXEnterIndexQp=2;

			ImageTypeQP.DrawImage(pBufferDC,m_rcTypeRect[1].left,m_rcTypeRect[1].top,SizeEnterType.cx,SizeEnterType.cy,nXEnterIndexQp*SizeEnterType.cx,0);
			CPngImage ImageTypeTY;
			ImageTypeTY.LoadImage(AfxGetInstanceHandle(),TEXT("TYPE_TY"));

			//进入游戏
			INT nXEnterIndexTy=0;
			bool bEnterButtonDownTy=((m_wViewItemDown==2)&&(m_cbButtonDown==2));
			bool bEnterButtonHoverTy=((m_wViewItemHover==2)&&(m_cbButtonHover==2));

			//进入游戏
			if ((bEnterButtonHoverTy==true)&&(bEnterButtonDownTy==true)) nXEnterIndexTy=1;
			if ((bEnterButtonHoverTy==true)&&(bEnterButtonDownTy==false)) nXEnterIndexTy=2;

			ImageTypeTY.DrawImage(pBufferDC,m_rcTypeRect[2].left,m_rcTypeRect[2].top,SizeEnterType.cx,SizeEnterType.cy,nXEnterIndexTy*SizeEnterType.cx,0);

			break;
		}
	case VIEW_MODE_GAME:
		{
				m_logo.ShowWindow(SW_HIDE);

			//绘画对象
// 			tagGameKindInfo * pGameKindInfo=m_GameKindInfoActiveMenu[0];
// 			if (pGameKindInfo!=NULL) DrawGameSsc(pBufferDC,m_wViewItemHover,0,0,pGameKindInfo);

			break;
		}
	case VIEW_MODE_KIND:	//类型视图
		{
			m_logo.ShowWindow(SW_HIDE);

			//变量定义
			WORD wViewCount=m_wKindXCount*m_wKindYCount;
			WORD wStartIndex=wViewCount*m_wKindPageCurrent;

			//绘画类型
			for (WORD i=0;i<wViewCount;i++)
			{
				//完成判断
				if ((wStartIndex+i)>=m_GameKindInfoActive.GetCount())
				{
					break;
				}
				//绘画对象
				tagGameKindInfo * pGameKindInfo=m_GameKindInfoActive[wStartIndex+i];

				if (pGameKindInfo==NULL)
					continue;
				int nTypeID = pGameKindInfo->pGameKindItem->m_GameKind.wTypeID;
				int nSortID = pGameKindInfo->pGameKindItem->m_GameKind.wSortID;
				//计算位置
				//INT nXDrawPos=m_wKindExcursion+(i%m_wKindXCount)*(GAME_KIND_ITEM_CX+GAME_KIND_ITEM_PX);
				//INT nYDrawPos=GAME_KIND_ITEM_PY+(i/m_wKindXCount)*(GAME_KIND_ITEM_CY+GAME_KIND_ITEM_PY);
//				INT nXDrawPos=m_wKindExcursion+(nSortID-1)*(GAME_KIND_ITEM_CX+GAME_KIND_ITEM_PX)+4;
//				INT nYDrawPos=GAME_KIND_ITEM_PY+(nTypeID-1)*(GAME_KIND_ITEM_CY+GAME_KIND_ITEM_PY)+30;

				INT nXDrawPos=m_wKindExcursion+(i%m_wKindXCount)*(GAME_KIND_ITEM_CX+GAME_KIND_ITEM_PX);
				INT nYDrawPos=GAME_KIND_ITEM_PY+(i/m_wKindXCount)*(GAME_KIND_ITEM_CY+GAME_KIND_ITEM_PY);

				nYDrawPos+=40;
				int nItem = (nTypeID-1)*m_wKindXCount+(nSortID-1);

// 				if(nTypeID == 4)
// 				{
// 					nYDrawPos += 40;
// 				}


				DrawKindItem(pBufferDC,i,nXDrawPos,nYDrawPos,pGameKindInfo);
			}



			break;
		}
	case VIEW_MODE_SERVER:	//房间视图
		{
			m_logo.ShowWindow(SW_HIDE);

			//变量定义
			WORD wViewCount=m_wServerXCount*m_wServerYCount;
			WORD wStartIndex=wViewCount*m_wServerPageCurrent;

			//绘画标题
			//m_ImageTitle.DrawImage(pBufferDC,(rcClient.Width()-GAME_TITLE_CX)/2,0,GAME_TITLE_CX,GAME_TITLE_CY,0,0);

			//绘画类型
			for (WORD i=0;i<wViewCount;i++)
			{
				//完成判断
				if ((wStartIndex+i)>=m_GameServerInfoActive.GetCount())
				{
					break;
				}

				//计算位置
				INT nXDrawPos=m_wServerExcursion+(i%m_wServerXCount)*(GAME_SERVER_ITEM_CX+GAME_SERVER_ITEM_PX);
				INT nYDrawPos=GAME_TITLE_CY+GAME_SERVER_ITEM_PY+(i/m_wServerXCount)*(GAME_SERVER_ITEM_CY+GAME_SERVER_ITEM_PY);

				//绘画对象
				tagGameServerInfo * pGameServerInfo=m_GameServerInfoActive[wStartIndex+i];
				if (pGameServerInfo!=NULL) DrawServerItem(pBufferDC,i,nXDrawPos,nYDrawPos,pGameServerInfo);
			}

			break;
		}
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//界面更新
VOID CPlazaViewItem::InValidateWndView(BYTE cbViewType)
{
	//模式判断
	if(m_cbShowItemMode==cbViewType) InvalidateRect(NULL);

	return;
}

//绘画类型
VOID CPlazaViewItem::DrawKindItem(CDC * pDC, WORD wViewIndex, INT nXPos, INT nYPos, tagGameKindInfo * pGameKindInfo)
{
	if(m_cbShowItemMode != VIEW_MODE_KIND)
		return;
	//效验参数
	ASSERT(pGameKindInfo!=NULL);
	if (pGameKindInfo==NULL) return;

	//安装判断
	bool bInstall=false;
	if (pGameKindInfo->pGameKindItem->m_dwProcessVersion!=0L) bInstall=true;

	//加载资源
	CPngImage ImageEnterKind;
	ImageEnterKind.LoadImage(AfxGetInstanceHandle(),(bInstall==true)?TEXT("BT_ENTER_KIND"):TEXT("BT_INSTALL_GAME"));

	//获取大小
	CSize SizeEnterKind;
	SizeEnterKind.SetSize(ImageEnterKind.GetWidth()/5L,ImageEnterKind.GetHeight());


	//进入游戏
	INT nXEnterIndex=0;
	bool bEnterButtonDown=((m_wViewItemDown==wViewIndex)&&(m_cbButtonDown==BT_ENTER_KIND));
	bool bEnterButtonHover=((m_wViewItemHover==wViewIndex)&&(m_cbButtonHover==BT_ENTER_KIND));

	//进入游戏
	if ((bEnterButtonHover==true)&&(bEnterButtonDown==true)) nXEnterIndex=1;
	if ((bEnterButtonHover==true)&&(bEnterButtonDown==false)) nXEnterIndex=2;



	//绘画类型
	if (pGameKindInfo->ImageKindItem.IsNull()==false)
	{
		CSize SizeImage;
		SizeImage.SetSize(pGameKindInfo->ImageKindItem.GetWidth(),pGameKindInfo->ImageKindItem.GetHeight());

		pGameKindInfo->ImageKindItem.DrawImage(pDC,nXPos,nYPos,GAME_KIND_ITEM_CX,GAME_KIND_ITEM_CY,0,0,SizeImage.cx,SizeImage.cy);
	}
	ImageEnterKind.DrawImage(pDC,nXPos,nYPos,SizeEnterKind.cx,SizeEnterKind.cy,nXEnterIndex*SizeEnterKind.cx,0);
	return;
}

//绘画房间
VOID CPlazaViewItem::DrawServerItem(CDC * pDC, WORD wViewIndex, INT nXPos, INT nYPos, tagGameServerInfo * pGameServerInfo)
{
	//效验参数
	ASSERT(pGameServerInfo!=NULL);
	if (pGameServerInfo==NULL) return;

	//加载资源
	CPngImage ImageEnterServer;
	ImageEnterServer.LoadImage(AfxGetInstanceHandle(),TEXT("BT_ENTER_SERVER"));

	//获取大小
	CSize SizeEnterServer;
	SizeEnterServer.SetSize(ImageEnterServer.GetWidth()/5L,ImageEnterServer.GetHeight());

	//绘画类型
	if (m_ImageServer.IsNull()==false)
	{
		CSize SizeImage;
		SizeImage.SetSize(m_ImageServer.GetWidth(),m_ImageServer.GetHeight());
		m_ImageServer.DrawImage(pDC,nXPos,nYPos,GAME_SERVER_ITEM_CX,GAME_SERVER_ITEM_CY,0,0,SizeImage.cx,SizeImage.cy);
	}

	//进入游戏
	INT nXEnterIndex=0;
	bool bEnterButtonDown=((m_wViewItemDown==wViewIndex)&&(m_cbButtonDown==BT_ENTER_SERVER));
	bool bEnterButtonHover=((m_wViewItemHover==wViewIndex)&&(m_cbButtonHover==BT_ENTER_SERVER));

	//进入游戏
	if ((bEnterButtonHover==true)&&(bEnterButtonDown==true)) nXEnterIndex=1;
	if ((bEnterButtonHover==true)&&(bEnterButtonDown==false)) nXEnterIndex=2;
	ImageEnterServer.DrawImage(pDC,nXPos+7,nYPos+179,SizeEnterServer.cx,SizeEnterServer.cy,nXEnterIndex*SizeEnterServer.cx,0);




	//获取全局属性
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//负载信息
	TCHAR szOnlineCount[32]=TEXT("");
	if(pParameterGlobal->m_bShowServerStatus)
	{
	if(pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount>=150) 
		_sntprintf(szOnlineCount,CountArray(szOnlineCount),TEXT("繁忙"));
	if(pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount>=80&&pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount<150) 
		_sntprintf(szOnlineCount,CountArray(szOnlineCount),TEXT("拥挤"));
	if(pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount>=20&&pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount<80) 
		_sntprintf(szOnlineCount,CountArray(szOnlineCount),TEXT("良好"));
	if(pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount<20)
		_sntprintf(szOnlineCount,CountArray(szOnlineCount),TEXT("空闲"));
	}
	else{
		//在线人数
		_sntprintf(szOnlineCount,CountArray(szOnlineCount),TEXT("%ld"),pGameServerInfo->pGameServerItem->m_GameServer.dwOnLineCount);

	}





	//单元积分
	TCHAR szServerScore[32]=TEXT("");
	_sntprintf(szServerScore,CountArray(szServerScore),SCORE_STRING,pGameServerInfo->pGameServerItem->m_GameServer.lServerScore);

	//需要金币
	TCHAR szMinServerScore[32]=TEXT("");
	_sntprintf(szMinServerScore,CountArray(szMinServerScore),SCORE_STRING,pGameServerInfo->pGameServerItem->m_GameServer.lMinServerScore);

	//房间名字
	TCHAR szServerName[LEN_SERVER]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),pGameServerInfo->pGameServerItem->m_GameServer.szServerName);

	//设置环境
	pDC->SetBkColor(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画信息
	CRect rcOnlineCount(nXPos+78,nYPos+79,nXPos+168,nYPos+100);
	CRect rcServerName(nXPos+22,nYPos+148,nXPos+168,nYPos+169);
	CRect rcServerScore(nXPos+78,nYPos+102,nXPos+168,nYPos+123);
	CRect rcMinServerScore(nXPos+78,nYPos+125,nXPos+168,nYPos+146);
	pDC->DrawText(szServerName,lstrlen(szServerName),rcServerName,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	pDC->DrawText(szOnlineCount,lstrlen(szOnlineCount),rcOnlineCount,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	pDC->DrawText(szServerScore,lstrlen(szServerScore),rcServerScore,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	pDC->DrawText(szMinServerScore,lstrlen(szMinServerScore),rcMinServerScore,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

	return;
}

VOID CPlazaViewItem::RectifyDlg()
{
	if (m_dlgChongQingSSC.GetSafeHwnd() != NULL)
	{
		m_dlgChongQingSSC.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}

	if (m_dlgJiangXiSSC.GetSafeHwnd() != NULL)
	{
		m_dlgJiangXiSSC.SetWindowPos(NULL,7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgQiXingCai.GetSafeHwnd() != NULL)//---LLY
	{
		m_dlgQiXingCai.SetWindowPos(NULL,  7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgPaiLie3.GetSafeHwnd() != NULL)//
	{
		m_dlgPaiLie3.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlg3D.GetSafeHwnd() != NULL)//
	{
		m_dlg3D.SetWindowPos(NULL,  7,32, game_cx, game_cy, SWP_NOZORDER);
	}

	if (m_dlgXinjiangSSC.GetSafeHwnd() != NULL)
	{
		m_dlgXinjiangSSC.SetWindowPos(NULL,7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgFenFenCai.GetSafeHwnd() != NULL)
	{
		m_dlgFenFenCai.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgWuFenCai.GetSafeHwnd() != NULL)
	{
		m_dlgWuFenCai.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgGuangdong11x5.GetSafeHwnd() != NULL)
	{
		m_dlgGuangdong11x5.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgChongQing11x5.GetSafeHwnd() != NULL)
	{
		m_dlgChongQing11x5.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgJiangXi11x5.GetSafeHwnd() != NULL)
	{
		m_dlgJiangXi11x5.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgShanDong11x5.GetSafeHwnd() != NULL)
	{
		m_dlgShanDong11x5.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgBjPK10.GetSafeHwnd() != NULL)
	{
		m_dlgBjPK10.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgBjKuai8.GetSafeHwnd() != NULL)
	{
		m_dlgBjKuai8.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgXingYun28.GetSafeHwnd() != NULL)
	{
		m_dlgXingYun28.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgHeiLongJiang11X5.GetSafeHwnd() != NULL)
	{
		m_dlgHeiLongJiang11X5.SetWindowPos(NULL, 7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	if (m_dlgLiuHeCai.GetSafeHwnd() != NULL)
	{
		m_dlgLiuHeCai.SetWindowPos(NULL,7,32, game_cx, game_cy, SWP_NOZORDER);
	}
	return;
}

//调整控件
VOID CPlazaViewItem::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//按钮大小
// 	CRect rcButton;
// 	m_btLastKind.GetWindowRect(&rcButton);
// 	//类型位置
	if(m_cbShowItemMode == VIEW_MODE_KIND)
	{
 		m_wKindXCount=(nWidth+GAME_KIND_ITEM_PX)/(GAME_KIND_ITEM_CX+GAME_KIND_ITEM_PX);
 		m_wKindYCount=(nHeight+GAME_KIND_ITEM_PY)/(GAME_KIND_ITEM_CY+GAME_KIND_ITEM_PY);
 		m_wKindExcursion=(nWidth+GAME_KIND_ITEM_PX-m_wKindXCount*(GAME_KIND_ITEM_CX+GAME_KIND_ITEM_PX))/2-3;
	 
 		//类型页面
 		WORD wViewCountKind=m_wKindXCount*m_wKindYCount;
 		m_wKindPageCount=(WORD)(m_GameKindInfoActive.GetCount()+wViewCountKind-1)/wViewCountKind;
 		m_wKindPageCurrent=(WORD)__min(m_GameKindInfoActive.GetCount()/wViewCountKind,m_wKindPageCurrent);
	}
// 	else
// 	{
// 		m_wKindXCount = 0;
// 		m_wKindYCount = 0;
// 		m_wKindPageCount = 0;
// 		m_wKindPageCurrent = 0;
// 	}

 	//房间位置
 	m_wServerXCount=(nWidth+GAME_SERVER_ITEM_PX)/(GAME_SERVER_ITEM_CX+GAME_SERVER_ITEM_PX);
 	m_wServerYCount=(nHeight+GAME_SERVER_ITEM_PY)/(GAME_SERVER_ITEM_CY+GAME_SERVER_ITEM_PY);
 	m_wServerExcursion=(nWidth+GAME_SERVER_ITEM_PX-m_wServerXCount*(GAME_SERVER_ITEM_CX+GAME_SERVER_ITEM_PX))/2;
 
 	//房间页面
 	WORD wViewCountServer=m_wServerXCount*m_wServerYCount;
 	m_wServerPageCount=(WORD)(m_GameServerInfoActive.GetCount()+wViewCountServer-1)/(m_wServerXCount*m_wServerYCount);
 	m_wServerPageCurrent=(WORD)__min(m_GameServerInfoActive.GetCount()/(m_wServerXCount*m_wServerYCount),m_wServerPageCurrent);

	if(m_btnReturn.GetSafeHwnd()!=NULL)
		m_btnReturn.SetWindowPos(NULL,nWidth-m_btnReturn.Width()-14,0,m_btnReturn.Width(),m_btnReturn.Height(),SWP_NOZORDER);

	//m_logo.SetWindowPos(NULL,7,32,785,250,SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER);
	//移动准备
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	DeferWindowPos(hDwp,m_logo,NULL,7,32,785,250,uFlags);
	EndDeferWindowPos(hDwp);

// 	//按钮控制
// 	m_btLastKind.EnableWindow((m_wKindPageCurrent>0)?TRUE:FALSE);
// 	m_btNextKind.EnableWindow(((m_wKindPageCurrent+1)<m_wKindPageCount)?TRUE:FALSE);
// 
// 
// 	//按钮控制
// 	m_btLastServer.EnableWindow((m_wServerPageCurrent>0)?TRUE:FALSE);
// 	m_btNextServer.EnableWindow(((m_wServerPageCurrent+1)<m_wServerPageCount)?TRUE:FALSE);

// 	//变量定义
// 	HDWP hDwp=BeginDeferWindowPos(64);
// 	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER|SWP_NOSIZE;
// 
// 	//类型按钮
// 	UINT uKindFlags=(m_cbShowItemMode==VIEW_MODE_KIND)?SWP_SHOWWINDOW:SWP_HIDEWINDOW;
// 	DeferWindowPos(hDwp,m_btLastKind,NULL,m_wKindExcursion,nHeight-rcButton.Height()-2,0,0,uFlags|uKindFlags);
// 	DeferWindowPos(hDwp,m_btNextKind,NULL,nWidth-rcButton.Width()-m_wKindExcursion,nHeight-rcButton.Height()-2,0,0,uFlags|uKindFlags);
// 
// 	//房间按钮
// 	CRect rcButtonServer;
// 	m_btLastServer.GetWindowRect(&rcButtonServer);
// 	UINT uServerFlags=(m_cbShowItemMode==VIEW_MODE_SERVER)?SWP_SHOWWINDOW:SWP_HIDEWINDOW;
// 	DeferWindowPos(hDwp,m_btReturnHall,NULL,m_wServerExcursion+2,nHeight-rcButton.Height()-2,0,0,uFlags|uServerFlags);
// 	DeferWindowPos(hDwp,m_btLastServer,NULL,nWidth/2+2,nHeight-rcButtonServer.Height()-2,0,0,uFlags|uServerFlags);
// 	DeferWindowPos(hDwp,m_btNextServer,NULL,nWidth/2+rcButtonServer.Width()+6,nHeight-rcButtonServer.Height()-2,0,0,uFlags|uServerFlags);


// 	m_btLastKind.ShowWindow(SW_HIDE/*(m_wKindTypeCurrentID == 1)?SW_HIDE:SW_SHOW*/);
// 	m_btNextKind.ShowWindow(SW_HIDE/*(m_wKindTypeCurrentID == 1)?SW_HIDE:SW_SHOW*/);
// 	m_btLastServer.ShowWindow(SW_HIDE/*(m_wKindTypeCurrentID == 1)?SW_HIDE:SW_SHOW*/);
// 	m_btNextServer.ShowWindow(SW_HIDE/*(m_wKindTypeCurrentID == 1)?SW_HIDE:SW_SHOW*/);
	//结束调整
//	EndDeferWindowPos(hDwp);

	return;
}

//资源目录
VOID CPlazaViewItem::GetGameDirectory(TCHAR szDirectory[], WORD wBufferCount, tagGameKind & GameKind)
{
	//变量定义
	WORD wStringIndex=0;

	//构造目录
	ZeroMemory(szDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szDirectory,GameKind.szProcessName,wBufferCount);

	//构造目录
	while ((szDirectory[wStringIndex]!=0)&&(szDirectory[wStringIndex]!=TEXT('.')))
	{
		wStringIndex++;
	}

	//字符终止
	szDirectory[wStringIndex]=0;

	return;
}

//鼠标消息
VOID CPlazaViewItem::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);
	//位置计算
	WORD wViewItemHover=GetHoverIndex(Point);

	BYTE cbButtonHover=GetHoverButton(wViewItemHover,Point);

	//数据调整
	if ((m_wViewItemDown!=INVALID_WORD)&&(m_cbButtonDown!=0))
	{
		if (cbButtonHover!=m_cbButtonDown) cbButtonHover=0;
		if (wViewItemHover!=m_wViewItemDown) wViewItemHover=INVALID_WORD;
	}

	//盘旋变量
	if ((wViewItemHover!=m_wViewItemHover)||(m_cbButtonHover!=cbButtonHover))
	{
		//设置变量
		m_cbButtonHover=cbButtonHover;
		m_wViewItemHover=wViewItemHover;

		CRect rcRedraw;
		rcRedraw.CopyRect(rcClient);
		rcRedraw.top+= 25;
		//更新界面
		RedrawWindow(&rcRedraw,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//进入判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}
//艺术字体
void CPlazaViewItem::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//变量定义
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	pDC->SetTextColor(crFrame);
// 	for (int i=0;i<CountArray(nXExcursion);i++)
// 	{
// 		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
// 	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}
//鼠标消息
VOID CPlazaViewItem::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//结果动作
	BYTE cbButtonDown=0;
	WORD wViewItemDown=INVALID_WORD;

	//设置变量
	if ((m_wViewItemDown!=INVALID_WORD)&&(m_wViewItemDown==m_wViewItemHover)&&(m_cbButtonDown==m_cbButtonHover))
	{
		cbButtonDown=m_cbButtonDown;
		wViewItemDown=m_wViewItemDown;
	}

	//按起处理
	if ((m_wViewItemDown!=INVALID_WORD)&&(m_cbButtonDown!=0))
	{
		//释放鼠标
		ReleaseCapture();

		//设置变量
		m_cbButtonDown=0;
		m_wViewItemDown=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	}

	if(m_cbShowItemMode == VIEW_MODE_TYPE&&m_wViewItemHover!=INVALID_WORD)
	{
 		switch (m_wViewItemHover)
 		{
 		case 0:
 			{
 				m_nRecordTypeID = 0;
 				ShowKindItemView(1);
 
 				
 				break;
 			}
 		case 1:		//查看规则
 			{
 				m_nRecordTypeID = 1;
 				ShowKindItemView(4);
 
 				break;
 			}
 		case 2:		//进入类型
 			{
 				MyMessageBox(L"努力研发中，敬请期待！");
 				break;
 				m_nRecordTypeID = 2;
 				
 
 				break;
 			}
 		}
			
	}
	else if ((wViewItemDown!=INVALID_WORD)&&(cbButtonDown!=0))
	{
		switch (cbButtonDown)
		{
		case BT_OPEN_LOTTERY:
			{
				break;
			}
		case BT_ENTER_KIND:		//进入类型
			{
				//进入类型
				WORD wViewCount=m_wKindXCount*m_wKindYCount*m_wKindPageCurrent;
				tagGameKindInfo * pGameKindInfo=m_GameKindInfoActive[wViewCount+wViewItemDown];
				
				if (pGameKindInfo!=NULL)
				{
					m_nRecordKindID = pGameKindInfo->pGameKindItem->m_GameKind.wKindID;
					m_btnReturn.ShowWindow(SW_SHOW);
					OnButtonEnterKind(pGameKindInfo->pGameKindItem->m_GameKind.wKindID);
				}

				break;
			}
		case BT_ENTER_SERVER:	//进入房间
			{
				m_cbShowItemMode = VIEW_MODE_SERVER;
				//进入房间
				WORD wViewCount=m_wServerXCount*m_wServerYCount*m_wServerPageCurrent;
				tagGameServerInfo * pGameServerInfo=m_GameServerInfoActive[wViewCount+wViewItemDown];
				
				if (pGameServerInfo!=NULL)
				{
					
					//EWIN网络房间密码 进入房间前判断密码是否正确
					if(pGameServerInfo->pGameServerItem->m_GameServer.wServerKind==0)
					{
						OnButtonEnterServer(pGameServerInfo->pGameServerItem->m_GameServer.wServerID);
					}else
					{
						//EWIN网络房间密码 显示输入密码窗口！
						//AfxMessageBox(_T("需要输入密码"));
						//显示窗口
						CDlgServerPassWork DlgServerPassWork;
						//设置密码
						DlgServerPassWork.SetPassWork(pGameServerInfo->pGameServerItem->m_GameServer.szServerPassWork);
						INT_PTR nResult=DlgServerPassWork.DoModal();
						if(nResult==99)
						{
							//密码正确进入房间
							OnButtonEnterServer(pGameServerInfo->pGameServerItem->m_GameServer.wServerID);
						
						}
					
					}
				}

				break;
			}
		}
	}

	return;
}
void CPlazaViewItem::ResetRecordTypeKind()
{
	m_nRecordKindID = 1000;
	m_nRecordTypeID = 1000;
	//m_btnReturn.ShowWindow(SW_HIDE);
	return;
}

void CPlazaViewItem::ReturnTouzhu()
{
	if(m_nRecordTypeID > 2)
	{
		ShowTypeItemView();
		return;
	}
	if(m_nRecordTypeID == 0)
	{
		m_nRecordTypeID = 0;
		
// 		CWnd* pParent = GetParent();
// 		if(pParent!=NULL)
// 		{
// 			pParent->PostMessage(IDM_SHOW_WND_BAR,1,0);
// 		}

		if(m_nRecordKindID != 1000)
		{
			m_btnReturn.ShowWindow(SW_SHOW);
			OnButtonEnterKind(m_nRecordKindID);
		}
		else
		{
			ShowKindItemView(1);
		}
	}
	else if(m_nRecordTypeID == 1)
	{
		m_nRecordTypeID = 1;
		ShowKindItemView(4);
	//	m_btnReturn.ShowWindow(SW_SHOW);
// 		CWnd* pParent = GetParent();
// 		if(pParent!=NULL)
// 		{
// 			pParent->PostMessage(IDM_SHOW_WND_BAR,1,0);
// 		}

	}
}
//鼠标消息
VOID CPlazaViewItem::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//设置焦点
	SetFocus();

	//按下处理
	if ((m_wViewItemHover!=INVALID_WORD)&&(m_cbButtonHover!=0))
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_cbButtonDown=m_cbButtonHover;
		m_wViewItemDown=m_wViewItemHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	}

	if(m_GameListRect.PtInRect(Point))
	{
		WORD wDownItem = GetGameHoverIndex(Point);
		if(wDownItem == INVALID_WORD) return;

		//m_cbButtonDown = wDownItem;
		m_wViewItemDown = wDownItem;
	}

	return;
}

//鼠标消息
LRESULT CPlazaViewItem::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置状态
	m_bHovering=false;

	//更新界面
	if (m_wViewItemHover!=INVALID_WORD)
	{
		//设置变量
		m_cbButtonDown=0;
		m_cbButtonHover=0;
		m_wViewItemDown=INVALID_WORD;
		m_wViewItemHover=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	}
	m_wGameHoverItem=INVALID_WORD;
// 	CWnd* pParent = GetParent();
// 	if(pParent!=NULL)
// 	{
// 		pParent->PostMessage(IDM_SHOW_MENU,0,INVALID_WORD);
// 	}
// 	m_bShowMenu = false;
//  	//更新界面
//  	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

	return 0;
}
//查询开奖数据
LRESULT CPlazaViewItem::SendQueryGameResult(WPARAM wParam, LPARAM lParam)
{
// 	WORD wKindID = (WORD)wParam;
// 	CWnd *pParent = GetParent();
// 	if(pParent!=NULL)
// 	{
// 		//目前只查询时时彩开奖 暂用7代替
// 		pParent->PostMessage(IDM_SEND_QUERY,wKindID,0);
// 	}
	return 1;
}
//查询开奖数据
LRESULT CPlazaViewItem::OnUpdateAccount(WPARAM wParam, LPARAM lParam)
{
	CWnd *pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->SendMessage(IDM_UPDATE_ACCOUNT,0,0);
	}
	return 1;
}
//更新余额
LRESULT CPlazaViewItem::OnUpdateYue(WPARAM wParam, LPARAM lParam)
{
	return 1;
}
//更新余额
LRESULT CPlazaViewItem::OnTanChuang(WPARAM wParam, LPARAM lParam)
{
	if(GetTickCount()-m_dwTanChuangTick < 5000)
		return 1;
	m_dwTanChuangTick = GetTickCount();
	CInformation Information;
	Information.ShowMessageBox(L"提示",L"当前期已结束，进入下期投注...",MB_OK,3);
	return 1;
}
//修改密码
LRESULT CPlazaViewItem::OnShowXgmm(WPARAM wParam, LPARAM lParam)
{
	CWnd *pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->SendMessage(IDM_SHOW_XGMM,0,0);
	}
	return 1;
}
VOID CPlazaViewItem::ShowGameMenu(bool bShowMenu,WORD wMenuType,int nXPos,int nYPos)
{
	m_bShowMenu=bShowMenu;
	m_nMenuXPos=nXPos;
	m_nMenuYPos=nYPos;
	m_cbMenuType=wMenuType;



	//获取对象
	ASSERT(CServerListData::GetInstance()!=NULL);
	CServerListData * pServerListData=CServerListData::GetInstance();

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//变量定义
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;
	tagGameKindInfo * pGameKindInfo=NULL;

	//删除对象
	for (INT i=0;i<m_GameKindInfoActiveMenu.GetCount();i++)
	{
		//获取对象
		ASSERT(m_GameKindInfoActiveMenu[i]!=NULL);
		pGameKindInfo=m_GameKindInfoActiveMenu[i];
	
		if(pGameKindInfo == NULL) continue;
		//设置变量
		pGameKindInfo->wSortID=0;
		pGameKindInfo->pGameKindItem=NULL;

		//清理对象
		if (pGameKindInfo->ImageKindItem.IsNull()==false)
		{
			pGameKindInfo->ImageKindItem.DestroyImage();
		}
	}

	//删除对象
	m_GameKindInfoBuffer.Append(m_GameKindInfoActiveMenu);
	m_GameKindInfoActiveMenu.RemoveAll();

	//插入对象
	while (true)
	{
		//获取对象
		pGameKindItem=pServerListData->EmunGameKindItem(Position);

		//对象判断
		if (pGameKindItem==NULL) break;

		//类型判断
		if ((wMenuType!=0)&&(pGameKindItem->m_GameKind.wTypeID!=wMenuType))
		{
			if (Position==NULL) break;
			if (Position!=NULL) continue;
		}

		//获取对象
		if (m_GameKindInfoBuffer.GetCount()>0L)
		{
			//获取对象
			INT_PTR nCount=m_GameKindInfoBuffer.GetCount();
			pGameKindInfo=m_GameKindInfoBuffer[nCount-1L];

			//删除对象
			ASSERT(pGameKindInfo!=NULL);
			m_GameKindInfoBuffer.RemoveAt(nCount-1L);
		}
		else
		{
			try
			{
				pGameKindInfo=new tagGameKindInfo;
//				if (pGameKindInfo==NULL) throw TEXT("创建对象失败");
				if (pGameKindInfo == NULL)
				{
					throw TEXT("创建对象失败");
				}
			}
			catch (...)
			{
				ASSERT(FALSE);
				break;
			}
		}

		//设置对象
		pGameKindInfo->pGameKindItem=pGameKindItem;
		pGameKindInfo->wSortID=pGameKindItem->m_GameKind.wSortID;

		//获取目录
		TCHAR szGameDirectory[LEN_PROCESS]=TEXT("");
		GetGameDirectory(szGameDirectory,CountArray(szGameDirectory),pGameKindItem->m_GameKind);


		//插入对象
		INT nItem=0;
		for (nItem=0;nItem<m_GameKindInfoActiveMenu.GetCount();nItem++)
		{
			//获取对象
			ASSERT(m_GameKindInfoActiveMenu[nItem]!=NULL);
			tagGameKindInfo * pGameKindTemp=m_GameKindInfoActiveMenu[nItem];

			//排序判断
			if (pGameKindInfo->wSortID<pGameKindTemp->wSortID)
			{
				m_GameKindInfoActiveMenu.InsertAt(nItem,pGameKindInfo);
				break;
			}
		}

		//默认插入
		if (nItem==m_GameKindInfoActiveMenu.GetCount())
		{
			m_GameKindInfoActiveMenu.Add(pGameKindInfo);
		}

		//结束判断
		if (Position==NULL)
		{
			break;
		}
	}

		//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	return;
}

//服务器返回开奖号码
VOID CPlazaViewItem::AddOpenResult(CMD_GP_QueryLotResult* pData, int nIndex)
{
	int nTypeID = pData->wKindID;

	if(nTypeID != m_nNowTypeID)
		return;
//	return;
	if(nTypeID == CZChongQingSSC||nTypeID == CZ_HGYDWFC)
		m_dlgChongQingSSC.GetTopLuckyNumber(pData,	nIndex,nTypeID);
	else if (nTypeID == CZ_TianJinSSC)
		m_dlgChongQingSSC.GetTopLuckyNumber(pData,	nIndex,nTypeID);
	else if (nTypeID == CZXinJiangSSC)
		m_dlgChongQingSSC.GetTopLuckyNumber(pData,nIndex,nTypeID);
	else if (nTypeID == CZ_FENFEN_CAI)
		m_dlgChongQingSSC.GetTopLuckyNumber(pData,	nIndex,nTypeID);
	else if (nTypeID == CZ_WUFEN_CAI)
		m_dlgChongQingSSC.GetTopLuckyNumber(pData,nIndex,nTypeID);
	else if (nTypeID == CZGD11Xuan5)
		m_dlgGuangdong11x5.GetTopLuckyNumber(pData,	nIndex,nTypeID);
	else if (nTypeID == CZCQ11Xuan5)
		m_dlgChongQing11x5.GetTopLuckyNumber(pData,nIndex,nTypeID);
	else if (nTypeID == CZSD11Xuan5)   //暂改 测试用
	{
		m_dlgGuangdong11x5.GetTopLuckyNumber(pData,	nIndex,nTypeID);
		//m_dlgHeiLongJiang11X5.GetTopLuckyNumber(pData,	nIndex);
	}
	else if(nTypeID == CZ_PK10)
		m_dlgBjPK10.GetTopLuckyNumber(pData,	nIndex);
	else if(nTypeID == CZKUAILE8)
	{
		m_dlgBjKuai8.GetTopLuckyNumber(pData,	nIndex);
	}
	else if(nTypeID == CZXingYun28)
	{
		m_dlgXingYun28.GetTopLuckyNumber(pData,	nIndex);
	}
	else if (nTypeID == CZHLJ11Xuan5)
		m_dlgGuangdong11x5.GetTopLuckyNumber(pData,	nIndex,nTypeID);
	else if (nTypeID == CZJX11Xuan5)
		m_dlgGuangdong11x5.GetTopLuckyNumber(pData,	nIndex,nTypeID);
	else if (nTypeID == CZ_LIUHECAI)
		m_dlgLiuHeCai.GetTopLuckyNumber(pData,	nIndex);
	else if (nTypeID == CZ_QiXingCai)//lly
		m_dlgQiXingCai.GetTopLuckyNumber(pData, nIndex);
	else if (nTypeID == CZPaiLie3)
		m_dlgPaiLie3.GetTopLuckyNumber(pData,	nIndex);
	else if (nTypeID == CZ3D)
		m_dlg3D.GetTopLuckyNumber(pData,	nIndex);

	
	return;
}
//显示菜单框
LRESULT CPlazaViewItem::OnShowMenu(WPARAM wParam, LPARAM lParam)
{
	BYTE cbShow = (BYTE)wParam;
	BYTE cbMenuType = (BYTE)lParam;
	CWnd* pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->PostMessage(IDM_SHOW_MENU,cbShow,cbMenuType);
	}

	return 1;
}
//显示菜单框
LRESULT CPlazaViewItem::OnBnClickedGameType(WPARAM wParam, LPARAM lParam)
{
	WORD wViewItemDown = (WORD)wParam;
	BYTE cbMenuType = (BYTE)lParam;

	CWnd* pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->PostMessage(IDM_CLICKED_TYPE,cbMenuType,0);
	}

	//进入类型
	//WORD wViewCount=m_wKindXCount*m_wKindYCount*m_wKindPageCurrent;
	tagGameKindInfo * pGameKindInfo=m_GameKindInfoActiveMenu[/*wViewCount+*/wViewItemDown];
	if (pGameKindInfo!=NULL) OnButtonEnterKind(pGameKindInfo->pGameKindItem->m_GameKind.wKindID);


	return 1;
}
//////////////////////////////////////////////////////////////////////////////////
VOID CPlazaViewItem::FreshGame()
{
	if(m_dlgChongQingSSC.IsWindowVisible())
	{
		m_dlgChongQingSSC.FlushZongjine();
	}
	else if(m_dlgJiangXiSSC.IsWindowVisible())
	{
		m_dlgJiangXiSSC.FlushZongjine();
	}
	else if(m_dlgXinjiangSSC.IsWindowVisible())
	{
		m_dlgJiangXiSSC.FlushZongjine();
	}
	else if(m_dlgFenFenCai.IsWindowVisible())
	{
		m_dlgFenFenCai.FlushZongjine();
	}
	else if(m_dlgWuFenCai.IsWindowVisible())
	{
		m_dlgWuFenCai.FlushZongjine();
	}
	else if(m_dlgGuangdong11x5.IsWindowVisible())
	{
		m_dlgGuangdong11x5.FlushZongjine();
	}
	else if(m_dlgChongQing11x5.IsWindowVisible())
	{
		m_dlgChongQing11x5.FlushZongjine();
	}
	else if(m_dlgJiangXi11x5.IsWindowVisible())
	{
		m_dlgJiangXi11x5.FlushZongjine();
	}
	else if(m_dlgShanDong11x5.IsWindowVisible())
	{
		m_dlgShanDong11x5.FlushZongjine();
	}
	else if(m_dlgBjPK10.IsWindowVisible())
	{
		m_dlgBjPK10.FlushZongjine();
	}
	else if(m_dlgBjKuai8.IsWindowVisible())
	{
		m_dlgBjKuai8.FlushZongjine();
	}
	else if(m_dlgXingYun28.IsWindowVisible())
	{
		m_dlgXingYun28.FlushZongjine();
	}
	else if(m_dlgHeiLongJiang11X5.IsWindowVisible())
	{
		m_dlgHeiLongJiang11X5.FlushZongjine();
	}
	else if(m_dlgLiuHeCai.IsWindowVisible())
	{
		m_dlgLiuHeCai.FlushZongjine();
	}
	else if(m_dlgQiXingCai.IsWindowVisible())//lly
	{
		m_dlgQiXingCai.FlushZongjine();
	}
	else if(m_dlgPaiLie3.IsWindowVisible())
	{
		m_dlgPaiLie3.FlushZongjine();
	}
	else if(m_dlg3D.IsWindowVisible())
	{
		m_dlg3D.FlushZongjine();
	}


}
VOID CPlazaViewItem::ShowMenu(WORD wMenuType,CGameKindInfoArray& GameKindInfo)
{
	CGameKindInfoArray GameKindInfoActiveMenu;
	//获取对象
	ASSERT(CServerListData::GetInstance()!=NULL);
	CServerListData * pServerListData=CServerListData::GetInstance();

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//变量定义
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;
	tagGameKindInfo * pGameKindInfo=NULL;

	//删除对象
	for (INT i=0;i<GameKindInfoActiveMenu.GetCount();i++)
	{
		//获取对象
		ASSERT(GameKindInfoActiveMenu[i]!=NULL);
		pGameKindInfo=GameKindInfoActiveMenu[i];
	
		if(pGameKindInfo == NULL) continue;
		//设置变量
		pGameKindInfo->wSortID=0;
		pGameKindInfo->pGameKindItem=NULL;

		//清理对象
		if (pGameKindInfo->ImageKindItem.IsNull()==false)
		{
			pGameKindInfo->ImageKindItem.DestroyImage();
		}
	}

	//删除对象
	m_GameKindInfoBuffer.Append(GameKindInfoActiveMenu);
	GameKindInfoActiveMenu.RemoveAll();

	//插入对象
	while (true)
	{
		//获取对象
		pGameKindItem=pServerListData->EmunGameKindItem(Position);

		//对象判断
		if (pGameKindItem==NULL) break;

		//类型判断
		if ((wMenuType!=0)&&(pGameKindItem->m_GameKind.wTypeID!=wMenuType))
		{
			if (Position==NULL) break;
			if (Position!=NULL) continue;
		}

		//获取对象
		if (m_GameKindInfoBuffer.GetCount()>0L)
		{
			//获取对象
			INT_PTR nCount=m_GameKindInfoBuffer.GetCount();
			pGameKindInfo=m_GameKindInfoBuffer[nCount-1L];

			//删除对象
			ASSERT(pGameKindInfo!=NULL);
			m_GameKindInfoBuffer.RemoveAt(nCount-1L);
		}
		else
		{
			try
			{
				pGameKindInfo=new tagGameKindInfo;
//				if (pGameKindInfo==NULL) throw TEXT("创建对象失败");
				if (pGameKindInfo == NULL)
				{
					throw TEXT("创建对象失败");
				}
			}
			catch (...)
			{
				ASSERT(FALSE);
				break;
			}
		}

		//设置对象
		pGameKindInfo->pGameKindItem=pGameKindItem;
		pGameKindInfo->wSortID=pGameKindItem->m_GameKind.wSortID;

		//插入对象
		INT nItem=0;
		for (nItem=0;nItem<GameKindInfoActiveMenu.GetCount();nItem++)
		{
			//获取对象
			ASSERT(GameKindInfoActiveMenu[nItem]!=NULL);
			tagGameKindInfo * pGameKindTemp=GameKindInfoActiveMenu[nItem];

			//排序判断
			if (pGameKindInfo->wSortID<pGameKindTemp->wSortID)
			{
				GameKindInfoActiveMenu.InsertAt(nItem,pGameKindInfo);
				break;
			}
		}

		//默认插入
		if (nItem==GameKindInfoActiveMenu.GetCount())
		{
			GameKindInfoActiveMenu.Add(pGameKindInfo);
		}

		//结束判断
		if (Position==NULL)
		{
			break;
		}
	}

	//GameKindInfo = m_GameKindInfoActive;
	for(int i = 0;i < GameKindInfoActiveMenu.GetCount();i++)
	{
		tagGameKindInfo *pGameKind = GameKindInfoActiveMenu.GetAt(i);
		if(pGameKind == NULL) continue;

		GameKindInfo.Add(pGameKind);
	}

	return;
}

//读取事件
bool CPlazaViewItem::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{

		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_USER_FANDIAN_RET:
		case SUB_GP_TOUZHU_CONTINUE_RET:
		case SUB_GP_SET_USER_BONUS_RET:
		case SUB_GP_TOUZHU_CQSSC_RET:
		case SUB_GP_GET_MORE_RECORD_RET:
		case SUB_GR_GET_LHC_QIHAO_RET:
			{
				if(m_dlgChongQingSSC.IsWindowVisible())
					m_dlgChongQingSSC.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgJiangXiSSC.IsWindowVisible())
					m_dlgJiangXiSSC.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgXinjiangSSC.IsWindowVisible())
					m_dlgXinjiangSSC.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgFenFenCai.IsWindowVisible())
					m_dlgFenFenCai.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgWuFenCai.IsWindowVisible())
					m_dlgWuFenCai.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgGuangdong11x5.IsWindowVisible())
					m_dlgGuangdong11x5.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgChongQing11x5.IsWindowVisible())
					m_dlgChongQing11x5.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgHeiLongJiang11X5.IsWindowVisible())
					m_dlgHeiLongJiang11X5.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgShanDong11x5.IsWindowVisible())
					m_dlgShanDong11x5.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgBjPK10.IsWindowVisible())
					m_dlgBjPK10.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgBjKuai8.IsWindowVisible())
					m_dlgBjKuai8.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgXingYun28.IsWindowVisible())
					m_dlgXingYun28.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgJiangXi11x5.IsWindowVisible())
					m_dlgJiangXi11x5.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgLiuHeCai.IsWindowVisible())
					m_dlgLiuHeCai.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgQiXingCai.IsWindowVisible())//lly
					m_dlgQiXingCai.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlgPaiLie3.IsWindowVisible())
					m_dlgPaiLie3.OnEventMissionRead(Command,pData,wDataSize);
				else if (m_dlg3D.IsWindowVisible())
					m_dlg3D.OnEventMissionRead(Command,pData,wDataSize);

				return true;
			}
		}
	}
	return true;
}

VOID CPlazaViewItem::SetLogonSuccess(bool bSuccess)
{
	m_bLogonSuccess = bSuccess;
	m_dlgChongQingSSC.SetLogonSuccess(bSuccess);
}