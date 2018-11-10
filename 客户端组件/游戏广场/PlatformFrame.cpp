#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgEnquire.h"
#include "PlatformFrame.h"
#include "MessageDlg.h"
#include "HapiDump.h"
//////////////////////////////////////////////////////////////////////////////////
#define IDC_LST_MENU	WM_USER + 1000

//圆角大小
#define ROUND_CX					1									//圆角宽度
#define ROUND_CY					1									//圆角高度

//阴影定义
#define SHADOW_CX					5									//阴影宽度
#define SHADOW_CY					5									//阴影高度

//屏幕位置
#define BORAD_SIZE					5									//边框大小
#define CAPTION_SIZE				179									//标题大小

//控制位置
#define SPLITTER_CX					0									//拆分宽度
#define LESS_LIST_CX				205									//列表宽度
#define FRAME_AFFICHE_CX			222									//框架公告

//屏幕限制
#define LESS_SCREEN_CY				740									//最小高度
#define LESS_SCREEN_CX				1030								//最小宽度

//////////////////////////////////////////////////////////////////////////////////

//控制按钮
#define IDC_MIN						100									//最小按钮
#define IDC_CLOSE					101									//关闭按钮
#define IDC_SOUND					102									//声音按钮
#define IDC_NET						103									//网络按钮

//控件标识
#define IDC_WEB_PUBLICIZE			300									//浏览控件
#define IDC_WEB_PUBLICIZE1          301                                 //浏览控件 
#define IDC_WEB_PUBLICIZE2          302                                 //浏览控件 
#define IDC_SKIN_SPLITTER			303									//拆分控件
#define IDC_SYSTEM_TRAY_ICON		304									//任务图标
#define IDC_WEB_LOGO		        306                                 //LOGO控件 
#define IDC_WEB_LB_ADD		        307                                 //左下角广告

#define	IDC_TOP_NWES				305									//新闻控件

//控件标识
#define IDC_GAME_TYPE_CTRL			310									//类型框架
#define IDC_USER_INFO_CTRL			311									//用户信息
#define IDC_LAST_TOP_NEWS			312									//新闻
//这个是新闻动态的滚动动画
static const int TimerNews		= 4;
//这个是用来本地刷新开奖数据的
static const int TimerZhunBeiData = 5;
static const int TimerJiShi = 6;
static const int TimerNews1		= 7;
static const int TimerLuckyNum		= 8;

static const int TimerReconnect		= 9;
static const int TimerTopNews		= 3;

//范围按钮
#define IDC_NAVIGATION				400									//导航按钮

#define  IDC_ZHUANHUAN_CAIBI		500									//转换彩币
#define  IDC_ZHUANHUAN_JINBI		501									//转换金币
#define	 IDC_HOME					502									//首页按钮
//////////////////////////////////////////////////////////////////////////////////
//静态变量

CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//框架指针
static bool bShowInfomation = true;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame, CFrameWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_MOUSEMOVE()
	//自定消息
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)
	ON_MESSAGE(WM_INSUREPLAZA_UPDATA, OnMessageInsureUpdate)
	ON_MESSAGE(IDM_SHOW_MENU,OnShowMenu)
	ON_MESSAGE(IDM_CLICKED_TYPE,OnUpdateButton)
	ON_MESSAGE(IDM_SEND_QUERY,OnQueryGameResult)
	ON_MESSAGE(IDM_UPDATE_ACCOUNT,OnUpDateAccoount)
	ON_MESSAGE(IDM_SHOW_XGMM,OnShowXgmm)
	ON_NOTIFY(NM_CLICK, IDC_TOP_NWES, OnGridClick)
	ON_MESSAGE(IDM_REFRESH_YUE,OnRefreshYue)
	ON_MESSAGE(IDM_ZHUANHUAN_TO_CAIPIAO,OnZhuanhuanCaibi)
	ON_MESSAGE(IDM_ZHUANHUAN_TO_QIPAI,OnZhuanhuanJinbi)
	ON_MESSAGE(IDM_FRESH_GAME,OnFreshGame)
	ON_MESSAGE(IDM_TOUZHU_TISHI,OnTouZhuTishi)
	ON_MESSAGE(IDM_RETURN_GAME,ReturnTouzhu)
	ON_MESSAGE(IDM_RELEASE_FACE,ReleaseFace)
	ON_WM_TIMER()

END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlatformFrame::CPlatformFrame()
{
	//状态变量
	m_bRectify=false;
	m_bLogonFail=false;
	m_rcViewItem.SetRect(0,0,0,0);
	m_bGame=true;
	m_bGetLuckyNums=false;
	m_nNewsYPos=362+360;
	m_nNewsYPos -= 290;
	m_nreduce=0;
	m_dwTickCount = 0;
	m_cbShowNewsType = 0;
	m_bKillSocket = false;
	m_bCreate = false;
	m_bSound=true;
	m_nTop = 0;//400;
	//Menu.CreateMenu();
	m_bCanSend = false;
	//任务组件
	m_MissionManager.InsertMissionItem(&m_MissionList);
	m_MissionManager.InsertMissionItem(&m_MissionLogon);
	m_MissionManager.InsertMissionItem(this);

	m_bGetNews=false;
	m_bQuitGame=false;
	m_bGetMapBonus=false;
	//平台变量
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) m_pPlatformFrame=this;

	m_bToCaijin=false;
	m_bChaxunYue=false;
	m_bToJinbi = false;
	//框架环绕
	tagEncircleResource	EncircleFrame;
	EncircleFrame.pszImageTL=TEXT("FRAME_TL");
	EncircleFrame.pszImageTM=TEXT("FRAME_TM");
	EncircleFrame.pszImageTR=TEXT("FRAME_TR");
	EncircleFrame.pszImageML=TEXT("FRAME_ML");
	EncircleFrame.pszImageMR=TEXT("FRAME_MR");
	EncircleFrame.pszImageBL=TEXT("FRAME_BL");
	EncircleFrame.pszImageBM=TEXT("FRAME_BM");
	EncircleFrame.pszImageBR=TEXT("FRAME_BR");
	m_FrameEncircle.InitEncircleResource(EncircleFrame,AfxGetInstanceHandle());

	//子项资源
	tagEncircleResource	EncircleItemFrame;
	EncircleItemFrame.pszImageTL=MAKEINTRESOURCE(IDB_ITEM_FRAME_TL);
	EncircleItemFrame.pszImageTM=MAKEINTRESOURCE(IDB_ITEM_FRAME_TM);
	EncircleItemFrame.pszImageTR=MAKEINTRESOURCE(IDB_ITEM_FRAME_TR);
	EncircleItemFrame.pszImageML=MAKEINTRESOURCE(IDB_ITEM_FRAME_ML);
	EncircleItemFrame.pszImageMR=MAKEINTRESOURCE(IDB_ITEM_FRAME_MR);
	EncircleItemFrame.pszImageBL=MAKEINTRESOURCE(IDB_ITEM_FRAME_BL);
	EncircleItemFrame.pszImageBM=MAKEINTRESOURCE(IDB_ITEM_FRAME_BM);
	EncircleItemFrame.pszImageBR=MAKEINTRESOURCE(IDB_ITEM_FRAME_BR);
	EncircleItemFrame.pszImageGR=MAKEINTRESOURCE(IDB_AFFICHE_GROUP);
	m_ItemFrameEncircle.InitEncircleResource(EncircleItemFrame,AfxGetInstanceHandle());
	m_bCreateType4 = true;

	DeclareDumpFile();

	return;
}

//析构函数
CPlatformFrame::~CPlatformFrame()
{
	//平台变量
	ASSERT(m_pPlatformFrame==this);
	if (m_pPlatformFrame==this)
	{
		m_pPlatformFrame=NULL;
		delete []m_pPlatformFrame;
	}

	return;
}

//接口查询
VOID * CPlatformFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
}

//获取通知
VOID CPlatformFrame::OnGameItemFinish()
{
	//默认显示游戏列表
	//显示列表
	//m_PlazaViewItem.ShowKindItemView(0);
	m_PlazaViewItem.ShowTypeItemView();

	m_PlazaViewItem.CreateDlgCaipiao(2);
	return;
}

//获取通知
VOID CPlatformFrame::OnGameKindFinish(WORD wKindID)
{
	return;
}

//插入通知
VOID CPlatformFrame::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	if(m_bCreateType4)
	{
		m_bCreateType4 = false;
		m_PlazaViewItem.CreateDlgCaipiao(1);
	}
	//插入处理
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//游戏种类
		{
			//插入种类
			m_WndGameTypeCtrl.InsertGameType((CGameTypeItem *)pGameListItem);

			break;
		}
	case ItemGenre_Kind:	//游戏类型
		{
			break;
		}
	case ItemGenre_Server:	//游戏房间
		{
			break;
		}
	}

	return;
}

//更新通知
VOID CPlatformFrame::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	return;
}

//删除通知
VOID CPlatformFrame::OnGameItemDelete(CGameListItem * pGameListItem)
{
	return;
}

//连接事件
bool CPlatformFrame::OnEventMissionLink(INT nErrorCode,INT nSocketID)
{

	return true;
}
bool CPlatformFrame::CreatePlazaview()
{
	m_PlazaViewItem.CreateDlgCaipiao(0);
	return true;
}
VOID CPlatformFrame::AvtiveMainMissionItem()
{
	if(m_bCreate == false)
	{
		m_bCreate = true;
		//创建广场
		CRect rcCreate(0,0,0,0);

		m_dlgUserAccount.Create(IDD_USER_ACCOUNT,this);
		m_dlgUserAccount.ShowWindow(SW_HIDE);
		m_dlgHuiyuan.Create(CHuiYuanDlg::IDD,this);
		m_dlgHuiyuan.ShowWindow(SW_HIDE);
		m_dlgChongzhi.Create(CChongZhiDlg::IDD,this);
		m_dlgChongzhi.ShowWindow(SW_HIDE);
		m_dlgQukuan.Create(CQuKuanDlg::IDD,this);
		m_dlgQukuan.ShowWindow(SW_HIDE);
		m_dlgHuodong.Create(CHuoDongDlg::IDD,this);
		m_dlgHuodong.ShowWindow(SW_HIDE);
		m_dlgTouZhu.Create(CZhangHuTzhLogDlg::IDD,this);
		m_dlgTouZhu.ShowWindow(SW_HIDE);
		m_PlazaViewItem.CreateDlgCaipiao(0);

	}

	//变量定义
	ASSERT(GetMissionManager()!=NULL);
	CMissionManager * pMissionManager=GetMissionManager();

	//发起连接
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{

		m_bShutdowSocket = true;
		return;
	}

	m_bShutdowSocket = false;
	return;
}
VOID CPlatformFrame::SendToServer(int nSendType)
{
	if(!m_bCanSend)
		return;

	if(nSendType == 1)   //下注记录数量
	{
		CMD_GP_GetNews GetNews;
		ZeroMemory(&GetNews,sizeof(GetNews));

		GetNews.n_t_id = theAccount.user_id;
		GetNews.n_t_ccount = 5;
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_NEWS,&GetNews,sizeof(GetNews));

	}

	if(nSendType == 2)
	{
		//m_bToJinbi = true;
		CMD_GP_ZhuanHuan Zhuanhuan;
		Zhuanhuan.n_t_userid = theAccount.user_id;
		Zhuanhuan.n_t_type = 1;
		Zhuanhuan.f_t_jine = m_fJiaoyiScore;

		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_ZHUANHUAN,&Zhuanhuan,sizeof(Zhuanhuan));

	}

	if(nSendType == 3)
	{
	//	m_bToCaijin = true;
		CMD_GP_ZhuanHuan Zhuanhuan;
		Zhuanhuan.n_t_userid = theAccount.user_id;
		Zhuanhuan.n_t_type = 2;
		Zhuanhuan.f_t_jine = m_fJiaoyiScore;

		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_ZHUANHUAN,&Zhuanhuan,sizeof(Zhuanhuan));

	}

	if(nSendType == 4)
	{
	//	m_bChaxunYue=true;
		CMD_GP_GetLastYue GetLastYue;
		ZeroMemory(&GetLastYue,sizeof(GetLastYue));

		GetLastYue.dwUserID = theAccount.user_id;

		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_LAST_YUE,&GetLastYue,sizeof(GetLastYue));

	}
	if(nSendType == 5)
	{
		//m_bGetTime=true;
		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_TIME);

	}

	if(nSendType == 6)
	{
	//	m_bQuitGame=true;
		CMD_GP_QuitGame QuitGame;
		ZeroMemory(&QuitGame,sizeof(QuitGame));

		QuitGame.dwUserID = theAccount.user_id;
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUIT_GAME,&QuitGame,sizeof(QuitGame));

		m_MissionManager.ConcludeMissionItem(this,false);

	}

	if(nSendType == 7)
	{
		//m_bGetMapBonus=true;
		CMD_GP_GetMyMapBonus GetMyMapBonus;
		ZeroMemory(&GetMyMapBonus,sizeof(GetMyMapBonus));
		GetMyMapBonus.n_t_userid = theAccount.user_id;
		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_MAP_BONUS,&GetMyMapBonus,sizeof(GetMyMapBonus));

	}

	if(nSendType == 8)
	{
		CMD_GP_QueryGameResult QueryGameResult;
		QueryGameResult.wKindID = CZChongQingSSC;

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_GAME_RESULT,&QueryGameResult,sizeof(QueryGameResult));
	}
	if(nSendType == 9)
	{
		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_KEFU_URL);

	}
	return;
}
//关闭事件
bool CPlatformFrame::OnEventMissionShut(BYTE cbShutReason)
{
	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL && !m_bKillSocket)
	{
		m_bCanSend = false;
		if(bShowInfomation)
		{
			bShowInfomation = false;

			m_btNet.EnableWindow(FALSE);

			SetTimer(TimerReconnect,1000,NULL);
			if(!m_bLogonFail)
			{
				m_bLogonFail = false;
				m_DlgStatus.ShowStatusWindow(L"正在重新连接远程服务器....");
			}
		}


	}
// 	else if(m_bKillSocket)
// 	{
// 		OnClose();
// 	}

	return true;
}
//取消连接
VOID CPlatformFrame::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}



UINT CPlatformFrame::GetLuckNumber(LPVOID lpParam)
{
	return 1;
	//重庆时时彩开奖处
	CString xmlurl = _T("http://www.cailele.com/static/ssc/newlyopenlist.xml");
	CString strPath;  //文件路径
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	int pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\cqssc.xml"); 
	DeleteUrlCacheEntry(xmlurl);										//清理缓存
	if (FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)))
	{
		//OutputDebugString(strPath);
	}

	//江西时时彩开奖处
	xmlurl = _T("http://www.cailele.com/static/jxssc/newlyopenlist.xml");
	pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\jxssc.xml"); 
	DeleteUrlCacheEntry(xmlurl);											//清理缓存
	if (FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)))
	{
		//OutputDebugString(strPath);
	}

 	xmlurl = _T("http://www.xjflcp.com/servlet/sscSaleEnd?flag=openPrize");
 	pos = strPath.ReverseFind(_T('\\')); 
 	strPath = strPath.Left(pos)+ _T("\\xjssc.xml"); 
 	DeleteUrlCacheEntry(xmlurl);											//清理缓存
 	if (FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)))
 	{
 		//OutputDebugString(strPath);
 	}

	//广东11选5
	xmlurl = _T("http://www.cailele.com/static/gd11x5/newlyopenlist.xml");
	pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\gd11x5.xml"); 
	DeleteUrlCacheEntry(xmlurl);											//清理缓存
	if(FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)) )
	{
		//OutputDebugString(strPath);
	}

	//重庆11选5
	xmlurl = _T("http://www.cailele.com/static/cq11x5/newlyopenlist.xml");
	pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\cq11x5.xml"); 
	DeleteUrlCacheEntry(xmlurl);											//清理缓存
	if(FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)) )
	{
		//OutputDebugString(strPath);
	}

	//江西11选5
	xmlurl = _T("http://www.cailele.com/static/jxdlc/newlyopenlist.xml");
	pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\jxdlc.xml"); 
	DeleteUrlCacheEntry(xmlurl);											//清理缓存
	if(FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)) )
	{
		//OutputDebugString(strPath);
	}

	//11运
	xmlurl = _T("http://www.cailele.com/static/11yun/newlyopenlist.xml");
	pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\11yun.xml"); 
	DeleteUrlCacheEntry(xmlurl);											//清理缓存
	if(FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)) )
	{
		//OutputDebugString(strPath);
	}
	return 0;
}


bool CPlatformFrame::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if(Command.wMainCmdID == MDM_GP_USER_SERVICE)
	{
		bShowInfomation=true;
		switch (Command.wSubCmdID)
		{
// 		case SUB_GP_QUIT_GAME_RET:
// 			{
// 				SetTimer(55555,2000,NULL);
// 			}
		case SUB_GP_KILL_SOCKET:
			{
				CMD_GP_KillSocket* pKillSocket = (CMD_GP_KillSocket*)pData;

				m_bKillSocket = true;
				CInformation Information(this);
				if(Information.ShowMessageBox(pKillSocket->szMessage) == IDOK)
				{
					//SetTimer(55555,2000,NULL);
					//OnClose();
					exit(0);
					//SendMessage(WM_CLOSE);
				}

				return true;
			}
		case SUB_GR_GET_HYXX_LOG_COUNT_RET: //会员信息
		case SUB_GP_GET_HYXX_LOG_RET:
		case SUB_GR_XG_HY_FANDIAN_RET:
		case SUB_GR_SW_DAILI_RET:
		case SUB_GR_HYXX_ZHUANZHANG_RET:
		case SUB_GR_GET_XJ_Peie_RET:
		case SUB_GR_SET_XJ_Peie_RET:
			{
				if(m_dlgHuiyuan.m_hyxxDlg.IsWindowVisible())
					m_dlgHuiyuan.m_hyxxDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_REG_URL_RET: //会员添加下级
		case SUB_GR_ADD_HY_RET:
		case SUB_GR_SET_WEB_FANDIAN_RET:
		case SUB_GR_GET_Peie_RET:
			{
				if(m_dlgHuiyuan.m_tjxjDlg.IsWindowVisible())
					m_dlgHuiyuan.m_tjxjDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_XJTZH_LOG_COUNT_RET: //会员下级投注统计
		case SUB_GP_GET_XJTZH_LOG_RET:
			{
				if(m_dlgHuiyuan.m_xjtzhtjDlg.IsWindowVisible())
					m_dlgHuiyuan.m_xjtzhtjDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_CHK_XJTZH_LOG_COUNT_RET: //会员下级投注记录
		case SUB_GP_CHK_XJTZH_LOG_RET:
			{
				if(m_dlgHuiyuan.m_chkxjtzhDlg.IsWindowVisible())
					m_dlgHuiyuan.m_chkxjtzhDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_HYSHJ_RET: //会员数据
			{
				if(m_dlgHuiyuan.m_hyshjDlg.IsWindowVisible())
					m_dlgHuiyuan.m_hyshjDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_XJYK_TJ_COUNT_RET: //下级盈亏统计
		case SUB_GP_GET_XJYK_TJ_RET:
			{
				if(m_dlgHuiyuan.m_yktjDlg.IsWindowVisible())
					m_dlgHuiyuan.m_yktjDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
		
			}
		case SUB_GR_GET_XJYK_LOG_COUNT_RET: //下级盈亏明细
		case SUB_GP_GET_XJYK_LOG_RET:
			{
				if(m_dlgHuiyuan.m_yklogDlg.IsWindowVisible())
					m_dlgHuiyuan.m_yklogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_XJCHZH_LOG_COUNT_RET: //下级充值日志
		case SUB_GP_GET_XJCHZH_LOG_RET:
			{
				if(m_dlgHuiyuan.m_chzhlogDlg.IsWindowVisible())
					m_dlgHuiyuan.m_chzhlogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_XJTX_LOG_COUNT_RET: // 会员下级提现
		case SUB_GP_GET_XJTX_LOG_RET:
			{
				if(m_dlgHuiyuan.m_txlogDlg.IsWindowVisible())
					m_dlgHuiyuan.m_txlogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_QP_YINGKUI_COUNT_RET: //会员游戏统计
		case SUB_GP_GET_QP_YINGKUI_RET:
			{
				if(m_dlgHuiyuan.m_xjyxjlDlg.IsWindowVisible())
					m_dlgHuiyuan.m_xjyxjlDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				else if(m_dlgUserAccount.m_qpykdlg.IsWindowVisible())
					m_dlgUserAccount.m_qpykdlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_XJYX_TJ_COUNT_RET:  //会员下级游戏记录
		case SUB_GP_GET_XJYX_TJ_RET:
			{
				if(m_dlgHuiyuan.m_yxtjlDlg.IsWindowVisible())
					m_dlgHuiyuan.m_yxtjlDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_CHONG_ZHI_TYPE_RET: //充值
			{
				if(m_dlgChongzhi.IsWindowVisible())
					m_dlgChongzhi.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_QU_KUAN_INFO_RET: //取款
		case SUB_GP_DO_QU_KUAN_RET:
			{
				if(m_dlgQukuan.IsWindowVisible())
					m_dlgQukuan.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_USER_QIANDAO_RET:     //活动签到
		case SUB_GP_GET_USER_HUANLESONG_RET:
		case SUB_GP_GET_LING_JIANG_RET:
			{
				if(m_dlgHuodong.m_huanlesong.IsWindowVisible())
					m_dlgHuodong.m_huanlesong.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_DAILI_HUIKUI_RET:
		case SUB_GP_DAILI_LJ_RET:
			{
				if(m_dlgHuodong.m_daili.IsWindowVisible())
					m_dlgHuodong.m_daili.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_LUCKY_ZHUAN_RET: //活动大转盘
		case SUB_GP_CAN_LUCKY_ZHUAN_RET:
			{
				if(m_dlgHuodong.m_zhuanzhuan.IsWindowVisible())
					m_dlgHuodong.m_zhuanzhuan.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_GET_USER_INFO_RET: //账户个人信息
		case SUB_GR_XG_QQ_RET:
		case SUB_GP_ALTER_GENDER_RET:
			{
				if(m_dlgUserAccount.m_baseInfoDlg.IsWindowVisible())
					m_dlgUserAccount.m_baseInfoDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GR_XG_LOGIN_PASS_RET://账户修改密码
			{
				ASSERT(wDataSize == sizeof(CMD_GR_XG_Loginpass_RET));
				if(wDataSize!=sizeof(CMD_GR_XG_Loginpass_RET))
					return false;

				CMD_GR_XG_Loginpass_RET* pCountRet = (CMD_GR_XG_Loginpass_RET*)pData;

				int lResult = pCountRet->lResult;
				if(lResult == 0)
				{
					CString msg;
					msg.Format(_T("您已经成功修改密码，请重新登录！"));
					CInformation Information;
					if(Information.ShowMessageBox(msg,MB_ICONINFORMATION,30)==1)
					{
						//全局设置
						CGlobalUnits::GetInstance()->DeleteUserCookie();
						CGlobalUserInfo::GetInstance()->ResetUserInfoData();

						m_PlazaViewItem.ShowWindow(SW_HIDE);
						SendToServer(6);
						//发送事件
						CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
						if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

					}


				}
				else if (lResult == 1)
				{
					MyMessageBox(_T("旧密码不正确"));
					return true;
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("修改密码失败，请重试"));
					return true;
				}

				return true;
			}
		case SUB_GR_XG_QUKUAN_PASS_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_XG_Qukuanpass_RET));
				if(wDataSize!=sizeof(CMD_GR_XG_Qukuanpass_RET))
					return false;

				CMD_GR_XG_Qukuanpass_RET* pXGqqRet = (CMD_GR_XG_Qukuanpass_RET*)pData;

				int lResult = pXGqqRet->lResult;
				if(lResult == 0)
				{
					CString msg;
					msg.Format(_T("您已经成功修改密码"));
					CInformation Information;
					if(Information.ShowMessageBox(msg,MB_ICONINFORMATION,30)==1)
					{
						//全局设置
						CGlobalUnits::GetInstance()->DeleteUserCookie();
						CGlobalUserInfo::GetInstance()->ResetUserInfoData();

						m_PlazaViewItem.ShowWindow(SW_HIDE);

						SendToServer(6);
						//发送事件
						CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
						if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

					}

				}
				else if (lResult == 1)
				{
					MyMessageBox(_T("旧密码不正确"));
					return true;
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("修改密码失败，请重试"));
					return true;
				}
				return true;

			}
		case SUB_GR_LOCK_MACHINE_RET:
			{
				if(m_dlgUserAccount.m_xgmmDlg.IsWindowVisible())
					m_dlgUserAccount.m_xgmmDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_QUERY_YINHANG_RET:			//设置账户提款
		case SUB_GR_QUERY_MY_YINHANG_RET:
		case SUB_GR_SET_QUKUAN_ZHANGHAO_RET:
			{
				if(m_dlgUserAccount.m_shzhTkzhh.IsWindowVisible())
					m_dlgUserAccount.m_shzhTkzhh.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_QUERY_MY_PROTECT_RET://设置账户保护
		case SUB_GR_SET_QUKUAN_PROTECT_RET:
			{
				if(m_dlgUserAccount.m_zhhbhDlg.IsWindowVisible())
					m_dlgUserAccount.m_zhhbhDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_TOUZHU_LOG_COUNT_RET: //账户投注记录
		case SUB_GP_GET_TOUZHU_LOG_RET:
		case SUB_GR_CANCEL_TOUZHU_RET:
			{
				if(m_dlgTouZhu.IsWindowVisible())
				{
					m_dlgTouZhu.OnEventMissionRead(Command,  pData,  wDataSize);
				}
				else if(m_dlgUserAccount.m_tzhlogDlg.IsWindowVisible())
					m_dlgUserAccount.m_tzhlogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_TIXIAN_LOG_COUNT_RET: //账户提现
		case SUB_GP_GET_TIXIAN_LOG_RET:
			{
				if(m_dlgUserAccount.m_txlogDlg.IsWindowVisible())
					m_dlgUserAccount.m_txlogDlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_CHONGZHI_LOG_COUNT_RET: //账户充值
		case SUB_GP_GET_CHONGZHI_LOG_RET:
			{
				if(m_dlgUserAccount.m_chzhlogdlg.IsWindowVisible())
					m_dlgUserAccount.m_chzhlogdlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_YINGKUI_LOG_COUNT_RET: //账户盈亏
		case SUB_GP_GET_YINGKUI_LOG_RET:
			{
				if(m_dlgUserAccount.m_yklogdlg.IsWindowVisible())
					m_dlgUserAccount.m_yklogdlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;

			}
		case SUB_GR_GET_YINGKUI_MX_COUNT_RET: //账户盈亏明细
		case SUB_GP_GET_YINGKUI_MX_RET:
			{
				if(m_dlgUserAccount.m_ykmxdlg.IsWindowVisible())
					m_dlgUserAccount.m_ykmxdlg.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_USER_FANDIAN_RET: //游戏获取返点
		case SUB_GP_TOUZHU_CONTINUE_RET:
		case SUB_GP_SET_USER_BONUS_RET:
		case SUB_GP_TOUZHU_CQSSC_RET:
		case SUB_GP_GET_MORE_RECORD_RET:
		case SUB_GR_GET_LHC_QIHAO_RET:
			{
				if(m_PlazaViewItem.IsWindowVisible())
					m_PlazaViewItem.OnEventMissionRead(Command,  pData,  wDataSize);
				return true;
			}
		case SUB_GP_GET_LAST_YUE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetLastYueRet));
				if(wDataSize != sizeof(CMD_GP_GetLastYueRet)) return false;

				CMD_GP_GetLastYueRet* pGetYueRet = (CMD_GP_GetLastYueRet*)pData;

				theAccount.yue = pGetYueRet->f_t_yue;
				theAccount.dongjie = pGetYueRet->f_t_dongjie;

				//用户信息
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				pGlobalUserData->lScore = pGetYueRet->f_t_score;

				OnUpDateAccoount(0,0);
				return true;
			}
		case SUB_GP_GET_TOUZHU_TISHI_RET:
			{
				m_tishiDlg.OnEventMissionRead(Command,  pData,  wDataSize);
// 
// 				ASSERT((wDataSize% sizeof(CMD_GR_TouzhuTishiRet))==0);
// 				if(( wDataSize% sizeof(CMD_GR_TouzhuTishiRet))!=0) return false;
// 
// 				int nCount = wDataSize/sizeof(CMD_GR_TixianTishiRet);
// 
// 				bool bFresh = false;
// 
// 				for (int i = 0;i < nCount;i++)
// 				{
// 					CMD_GR_TouzhuTishiRet* pTouzhuTishiRet = ((CMD_GR_TouzhuTishiRet*)pData+i);
// 
// 					CString strTemp;
// 					strTemp.Format(L"%s", pTouzhuTishiRet->szQishu);
// 					if(strTemp.IsEmpty())
// 						continue;
// 
// 					CString msg;
// 					if(pTouzhuTishiRet->nWinZhushu <= 0) {
// 						msg.Format(_T("没中奖，继续努力！\n%s\n%s期\n盈亏:%.2lf"), theGameType[pTouzhuTishiRet->nType], pTouzhuTishiRet->szQishu,pTouzhuTishiRet->f_yingkui);
// 					} else {
// 						bFresh = true;
// 
// 						msg.Format(_T("投注彩种:%s\n\n投注期号:%s\n\n盈亏:%.2lf"), theGameType[pTouzhuTishiRet->nType], pTouzhuTishiRet->szQishu,pTouzhuTishiRet->f_yingkui);
// 					}
// 
// 					m_tishiDlg.SetTiShiMessage(msg);
// 				}
				return true;

			}

		case SUB_GP_GET_MAP_BONUS_RET:
			{

				ASSERT(wDataSize % sizeof(CMD_GR_GetMapBonusRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetMapBonusRet)!=0) return false;

				int nCount = wDataSize/sizeof(CMD_GR_GetMapBonusRet);
				for(int i = 0;i < nCount;i++)
				{
					CMD_GR_GetMapBonusRet* pGetMapBonus = ((CMD_GR_GetMapBonusRet*)pData+i);

					int index = (pGetMapBonus->n_t_type_id << 16) | (unsigned short)pGetMapBonus->n_t_kind_id;
					
					theBonus[index] = pGetMapBonus->f_t_bonus;

				}
				return true;
			}
		case SUB_GP_GET_NEWS_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GP_GetNewsRet)==0);
				if(wDataSize % sizeof(CMD_GP_GetNewsRet)!=0) return false;

				m_Grid.DeleteAllItems();
				m_vecTitle.clear();
				int nCount = wDataSize/sizeof(CMD_GP_GetNewsRet);
				CString strTemp;

				for(int i =0;i < nCount;i++)
				{
					CMD_GP_GetNewsRet* pNewsRet = ((CMD_GP_GetNewsRet*)pData+i);
					m_vecTitle.push_back(pNewsRet->s_t_news);

				}

				//SetTimer(TimerNews, 50, NULL);	
				CString strText;
				//pDC->SetTextColor(RGB(50,50,50));
				for(unsigned int n=0; n<m_vecTitle.size(); n++)
				{
					strText += m_vecTitle[n].c_str();
					//strText += L"\r\n";
				}
				m_WndTopNews.SetTopNews(strText);

				int nLength = strText.GetLength()/15;
				m_nNewsHeight = nLength*17;
				CRect rcClient;
				GetClientRect(rcClient);
				m_nTop = rcClient.Height() - m_nNewsHeight-300;
				CRect Rct(16,362,235,362+400);

				if(m_bGame)
					InvalidateRect(&Rct);

				GridCtrlInit();
				return true;
			}
		case SUB_GP_QUERY_TIME_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetSystemTime));
				if(wDataSize != sizeof(CMD_GR_GetSystemTime)) return false;

				CMD_GR_GetSystemTime* pSystemTime = (CMD_GR_GetSystemTime*)pData;
				theTimeCS.Lock();
				CTime servertime(pSystemTime->ServerTime);
				theTime = servertime;
				theTimeCS.Unlock();

				return true;
			}
		case SUB_GR_KEFU_URL_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GetKefuUrlRet));
				if(wDataSize != sizeof(CMD_GetKefuUrlRet)) return false;

				CMD_GetKefuUrlRet* pGetKefuUrlRet = (CMD_GetKefuUrlRet*)pData;

				if(pGetKefuUrlRet->n_t_result == 0)
				{
					ShellExecute(NULL,TEXT("OPEN"),pGetKefuUrlRet->s_t_desc,NULL,NULL,SW_NORMAL);
				}
			}
		case SUB_GP_ZHUANHUAN_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_ZhuanHuanRet));
				if(wDataSize != sizeof(CMD_GP_ZhuanHuanRet)) return false;

				CMD_GP_ZhuanHuanRet* pZhuanhuanRet = (CMD_GP_ZhuanHuanRet*)pData;

				if(pZhuanhuanRet->nResult == 0)
				{
					theAccount.yue = pZhuanhuanRet->f_t_yue;
					CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					pGlobalUserData->lScore = pZhuanhuanRet->f_t_score;

					OnUpDateAccoount(0,0);
					MyMessageBox(L"转换成功,请查收");
				}
				else if(pZhuanhuanRet->nResult == 1)
				{
					MyMessageBox(L"余额不足，转换失败");
				}
				else if(pZhuanhuanRet->nResult == 2)
				{
					MyMessageBox(L"转换失败,账户信息不存在");
				}
				else if(pZhuanhuanRet->nResult == 3)
				{
					MyMessageBox(L"您正在棋牌游戏中，请退出后再进行转换！");
				}
				return true;
			}
		case SUB_GP_QUERY_GAME_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GP_QueryLotResult)==0);
				if(wDataSize % sizeof(CMD_GP_QueryLotResult)!=0)
					return false;

				int nCount = wDataSize/sizeof(CMD_GP_QueryLotResult);
				CMD_GP_QueryLotResult* pQueryNumResult = (CMD_GP_QueryLotResult*) pData;
				int nIndex=0;
				int nFirstTypeID = pQueryNumResult->wKindID;

				for(int nn=0; nn<nCount; nn++)
				{
					CMD_GP_QueryLotResult* pResult  = pQueryNumResult + nn;
					int nTypeid = pResult->wKindID;
					if(nTypeid != nFirstTypeID)
					{
						nIndex=0;
						nFirstTypeID = nTypeid;
					}

					m_PlazaViewItem.AddOpenResult(pResult, nIndex);
					nIndex++;
				}

				return true;
				//屏蔽写文件的事情；直接传递给彩种
				if(0)
				{
					CString strPath;  //文件路径
					GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
					strPath.ReleaseBuffer(); 
					int pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\cqssc.xml"); 

					DeleteFile(strPath);
 					pos = strPath.ReverseFind(_T('\\')); 
 					strPath = strPath.Left(pos)+ _T("\\jxssc.xml"); 
 					DeleteFile(strPath);

					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\xjssc.xml"); 
					DeleteFile(strPath);

					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\fenfencai.xml"); 
					DeleteFile(strPath);

					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\wufencai.xml"); 
					DeleteFile(strPath);

					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\gd11x5.xml"); 
					DeleteFile(strPath);

					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\cq11x5.xml"); 
					DeleteFile(strPath);
					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\11yun.xml"); 
					DeleteFile(strPath);
					pos = strPath.ReverseFind(_T('\\')); 
					strPath = strPath.Left(pos)+ _T("\\jxdlc.xml"); 
					DeleteFile(strPath);

					m_xml.SetDoc(L"<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n");
					m_xml.AddElem(L"xml");

					WORD wCount = wDataSize/sizeof(CMD_GP_QueryLotResult);

					CMD_GP_QueryLotResult* pQueryLotResult  =  ((CMD_GP_QueryLotResult*)pData);
					int nFirstTypeID = pQueryLotResult->wKindID;
					for(int i = 0;i < wCount;i++)
					{
						CMD_GP_QueryLotResult* pQueryLotResult  =  ((CMD_GP_QueryLotResult*)pData+i);


						int nTypeid = pQueryLotResult->wKindID;

						if(nTypeid != nFirstTypeID)
						{
							m_xml.RemoveChildElem();
							m_xml.SetDoc(L"<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n");
							m_xml.AddElem(L"xml");

							nFirstTypeID = nTypeid;
						}
						switch(nTypeid)
						{
						case CZChongQingSSC:
							{
								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"cqssc.xml");

								m_xml.OutOfElem();
								break;
							}
 						case CZJiangXiSSC:
 							{
								
 								CString strTmp;
 								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
 								m_xml.AddChildElem(strTmp);
 								m_xml.IntoElem();						
	 
 								m_xml.Save(L"jxssc.xml");
	 
 								m_xml.OutOfElem();
 								break;
 							}
						case CZXinJiangSSC:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"xjssc.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZ_FENFEN_CAI:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"fenfencai.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZ_WUFEN_CAI:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"wufencai.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZGD11Xuan5:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"gd11x5.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZCQ11Xuan5:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"cq11x5.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZSD11Xuan5:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"11yun.xml");

								m_xml.OutOfElem();
								break;
							}
						case CZJX11Xuan5:
							{

								CString strTmp;
								strTmp.Format(L"row expect=\"%s\" opencode=\"%s\" opentime=\"%s\"",pQueryLotResult->wPeriod,pQueryLotResult->szLotNum,pQueryLotResult->szShijian);
								m_xml.AddChildElem(strTmp);
								m_xml.IntoElem();						

								m_xml.Save(L"jxdlc.xml");

								m_xml.OutOfElem();
								break;
							}
						default:
							break;
						}
					}


				}//end of if(0)
				return true;
			}

		}

	}

	return true;
}
//消息过虑
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}
void CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(TimerNews == nIDEvent)
	{
		m_nNewsYPos -= 290;
		m_nreduce += 1;
		KillTimer(TimerNews);
		CRect Rct(16,362,235,362+400);
		if(m_bGame)
			InvalidateRect(&Rct);

	}
	else if(TimerLuckyNum == nIDEvent)
	{
		SendToServer(8);
	}
	else if(TimerNews1 == nIDEvent)
	{
		//SetTimer(TimerNews,50,NULL);
	}
	else if (TimerZhunBeiData == nIDEvent)
	{
		SendToServer(5);
		//获取开奖数据
		//AfxBeginThread(GetLuckNumber, this);
	}
	else if(TimerJiShi == nIDEvent)
	{
			theTimeCS.Lock();
			theTime += CTimeSpan(0, 0, 0, 1);
			theTimeCS.Unlock();

	}
	else if(55555 == nIDEvent)
	{
		KillTimer(nIDEvent);
		OnClose();
	}
	else if(TimerReconnect == nIDEvent)
	{
		KillTimer(TimerReconnect);
		//重试任务
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,false)==true)
		{
			m_btNet.EnableWindow(TRUE);
			m_DlgStatus.HideStatusWindow();
			KillTimer(TimerReconnect);
			m_bCanSend = true;
			return ;
		}
		else
		{
			CMissionManager * pMissionManager=GetMissionManager();
			pMissionManager->ConcludeMissionItem(this,false);

		}
	}

	__super::OnTimer(nIDEvent);
}
LRESULT CPlatformFrame::ReturnTouzhu(WPARAM wParam, LPARAM lParam)
{
	OnRefreshYue(0,0);
	//m_WndBar.ShowWindow(SW_HIDE);
	m_PlazaViewItem.ShowWindow(SW_SHOW);

	m_cbShowNewsType = 0;
	CRect rcClient;
	GetClientRect(rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());
	m_dlgUserAccount.ShowWindow(SW_HIDE);
	m_dlgHuiyuan.ShowWindow(SW_HIDE);
	m_WndUserInfoCtrl.ShowWindow(SW_SHOW);
	m_Publicizeurl.ShowWindow(SW_SHOW);
	m_Grid.ShowWindow(SW_HIDE);
	m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
	m_dlgChongzhi.ShowWindow(SW_HIDE);
	m_dlgQukuan.ShowWindow(SW_HIDE);
	m_dlgHuodong.ShowWindow(SW_HIDE);
	m_dlgTouZhu.ShowWindow(SW_HIDE);
	m_bGame=true;
	CRect Rct(16,362,235,362+400);
	if(m_bGame)
		InvalidateRect(&Rct);

	m_PlazaViewItem.ReturnTouzhu();
	return 1;
}
LRESULT CPlatformFrame::ReleaseFace(WPARAM wParam, LPARAM lParam)
{
	m_WndUserInfoCtrl.ReleaseFace();
	return true;
}
//命令函数
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//功能按钮
	switch (nCommandID)
	{
	case IDC_HOME:
		{
			OnRefreshYue(0,0);
			m_PlazaViewItem.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 0;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_PlazaViewItem.ShowTypeItemView();
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_SHOW);
			m_Publicizeurl.ShowWindow(SW_SHOW);
			m_Grid.ShowWindow(SW_HIDE);
			//m_WndGameTypeCtrl.ShowWindow(SW_SHOW);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_HIDE);
			m_dlgQukuan.ShowWindow(SW_HIDE);
			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_HIDE);
			m_bGame=true;
			m_PlazaViewItem.ResetRecordTypeKind();
			CRect Rct(16,362,235,362+400);
			ReturnTouzhu(1,1);
			if(m_bGame)
				InvalidateRect(&Rct);

			return true;
		}
	case IDC_MIN:				//最小按钮
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_CLOSE:				//关闭按钮
		{
			PostMessage(WM_CLOSE,0,0);
			return TRUE;
		}
	case IDC_SOUND:				//声音按钮
		{
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_bSound = !m_bSound;
			if(m_bSound)
			{
				m_btSound.SetButtonImage(IDB_BT_SOUND,hInstance,true,false);
			}
			else
			{
				m_btSound.SetButtonImage(IDB_BT_MUTE,hInstance,true,false);
			}
			return TRUE;
		}
	case IDC_NET:
		{
			return TRUE;
		}
	case IDC_ZHUANHUAN_JINBI:
		{
			OnZhuanhuanJinbi(wParam,lParam);
			return TRUE;
		}
	case IDC_ZHUANHUAN_CAIBI:
		{
			OnZhuanhuanCaibi(wParam,lParam);
			return TRUE;
		}
	case IDC_NAVIGATION+0:
		{
			if(m_dwTickCount == 0)
			{
				m_dwTickCount = GetTickCount();
			}
			else if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"您点击过于频繁，请稍等片刻！");
				return true;
			}


			m_PlazaViewItem.ShowWindow(SW_HIDE);
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 2;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_dlgQukuan.ShowWindow(SW_HIDE);
			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_HIDE);

			m_bGame=false;

			return TRUE;

		}
	case IDC_NAVIGATION+1:
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"您点击过于频繁，请稍等片刻！");
				return true;
			}


			m_PlazaViewItem.ShowWindow(SW_HIDE);
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_HIDE);
			m_dlgQukuan.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 2;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_HIDE);

			m_bGame=false;
			return TRUE;
		
		}
	case IDC_NAVIGATION+2:
		{
// 			if( GetTickCount()-m_dwTickCount <100)
// 			{
// 				MyMessageBox(L"您点击过于频繁，请稍等片刻！");
// 				return true;
// 			}
 			m_PlazaViewItem.ShowWindow(SW_HIDE);
 			m_dlgUserAccount.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 1;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

 			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
 			m_dlgHuiyuan.ShowWindow(SW_HIDE);
 			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
 			m_dlgChongzhi.ShowWindow(SW_HIDE);
 			m_dlgQukuan.ShowWindow(SW_HIDE);
 			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_HIDE);
			m_bGame=false;
			
			return TRUE;
		
		}
	case IDC_NAVIGATION+3:
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"您点击过于频繁，请稍等片刻！");
				return true;
			}

 			m_PlazaViewItem.ShowWindow(SW_HIDE);
 			m_dlgUserAccount.ShowWindow(SW_HIDE);
 			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
 			m_dlgHuiyuan.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 1;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

 			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
 			m_dlgChongzhi.ShowWindow(SW_HIDE);
 			m_dlgQukuan.ShowWindow(SW_HIDE);
 			m_dlgHuodong.ShowWindow(SW_HIDE);
 
			m_dlgTouZhu.ShowWindow(SW_HIDE);

			m_bGame=false;
			return TRUE;
		}
	case IDC_NAVIGATION+4:
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"您点击过于频繁，请稍等片刻！");
				return true;
			}

			m_PlazaViewItem.ShowWindow(SW_HIDE);
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_HIDE);
			m_dlgQukuan.ShowWindow(SW_HIDE);
			m_dlgHuodong.ShowWindow(SW_HIDE);
			m_dlgTouZhu.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 2;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_bGame=false;
			return TRUE;
		}
	case IDC_NAVIGATION+5:		//导航按钮
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"您点击过于频繁，请稍等片刻！");
				return true;
			}

			m_PlazaViewItem.ShowWindow(SW_HIDE);
			m_dlgUserAccount.ShowWindow(SW_HIDE);
			m_WndUserInfoCtrl.ShowWindow(SW_HIDE);
			m_Publicizeurl.ShowWindow(SW_HIDE);
			m_Grid.ShowWindow(SW_HIDE);
			m_dlgHuiyuan.ShowWindow(SW_HIDE);
			m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
			m_dlgChongzhi.ShowWindow(SW_HIDE);
			m_dlgQukuan.ShowWindow(SW_HIDE);
			m_dlgHuodong.ShowWindow(SW_SHOW);
			m_cbShowNewsType = 1;
			CRect rcClient;
			GetClientRect(rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			m_dlgTouZhu.ShowWindow(SW_HIDE);

			m_bGame=false;
			return TRUE;
			//获取指针
			CGlobalUnits * pGlobalUnits = (CGlobalUnits *)CGlobalUnits::GetInstance();
			ASSERT(pGlobalUnits!=NULL);

			//构造地址
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%sSpreader.aspx"),szPlatformLink);

			//连接地址
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);

			return TRUE;

		}
	case IDC_NAVIGATION+6:
		{
			if( GetTickCount()-m_dwTickCount <100)
			{
				MyMessageBox(L"您点击过于频繁，请稍等片刻！");
				return true;
			}

// 			//获取指针
// 			CGlobalUnits * pGlobalUnits = (CGlobalUnits *)CGlobalUnits::GetInstance();
// 			ASSERT(pGlobalUnits!=NULL);
// 
// 			//构造地址
// 			TCHAR szNavigation[256]=TEXT("");
// 		//	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("http://ala.zoossoft.com/LR/Chatpre.aspx?id=ALA22728236&lng=cn"),szPlatformLink);
// 
// 			//读取配置文件
// 			//工作目录
// 			TCHAR szDirectory[MAX_PATH]=TEXT("");
// 			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));
// 
// 			//构造路径
// 			TCHAR szFileName[MAX_PATH]=TEXT("");
// 			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);
// 
// 			//构造地址
// 			TCHAR szLogonLink[256]=TEXT("");
// 			//	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%sLogonLink4.aspx"),szPlatformLink);
// 			GetPrivateProfileString(TEXT("kefu"),TEXT("Link"),TEXT(""),szNavigation,CountArray(szNavigation),szFileName);
// 
// 
// 			//连接地址
// 			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);

			SendToServer(9);
			return TRUE;

		}
	}

	//菜单命令
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//用户登录
		{
			m_MissionLogon.ShowLogon();
		//	AvtiveMainMissionItem();

			return TRUE;
		}
	case IDM_SWITCH_ACCOUNTS:	//切换帐号
		{
			//切换询问
			if (m_ServerViewItem.m_hWnd!=NULL)
			{
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("游戏房间即将关闭，确实要“切换帐号”吗？ "),MB_YESNO)!=IDYES) return TRUE;
			}

			//全局设置
			CGlobalUnits::GetInstance()->DeleteUserCookie();
			CGlobalUserInfo::GetInstance()->ResetUserInfoData();

			m_PlazaViewItem.CreateDlgCaipiao(4);
			m_PlazaViewItem.ShowWindow(SW_HIDE);

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

			
			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//调整控件
VOID CPlatformFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if ((nWidth==0)||(nHeight==0)) return;

	//拆分位置
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//环绕信息
	tagEncircleInfo EncircleInfoFrame;
	tagEncircleInfo EncircleInfoAffiche;
	tagEncircleInfo EncircleInfoItemFrame;
	m_FrameEncircle.GetEncircleInfo(EncircleInfoFrame);
	//m_AfficheEncircle.GetEncircleInfo(EncircleInfoAffiche);
	m_ItemFrameEncircle.GetEncircleInfo(EncircleInfoItemFrame);

	//获取位置
	ScreenToClient(&rcSplitter);

	//拆分范围
	INT nLessPos=nWidth*25/100,nMaxPos=nWidth/2;
	if (nLessPos>=LESS_LIST_CX) nLessPos=LESS_LIST_CX;

	//调整拆分
	if (m_bRectify==false)
	{
		m_bRectify=true;
		rcSplitter.left=nLessPos;
		rcSplitter.right=rcSplitter.left+SPLITTER_CX;
	}

	//拆分范围
	m_SkinSplitter.SetSplitterRange(nLessPos,nMaxPos);

	//调整位置
	if ((nWidth!=0)&&(nHeight!=0))
	{
		if (rcSplitter.left<nLessPos)
		{
			rcSplitter.left=nLessPos;
			rcSplitter.right=rcSplitter.left+SPLITTER_CX;
		}
		if (rcSplitter.right>nMaxPos)
		{
			rcSplitter.right=nMaxPos;
			rcSplitter.left=rcSplitter.right-SPLITTER_CX;
		}
	}

	//位置调整
	rcSplitter.top=EncircleInfoFrame.nTBorder;
	rcSplitter.bottom=nHeight-EncircleInfoFrame.nBBorder;

	//控件位置
	CRect rcUserInfoCtrl;
	m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//控制按钮
	DeferWindowPos(hDwp,m_btMin,NULL,nWidth-79,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,nWidth-48,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSound,NULL,nWidth-110,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btNet,NULL,nWidth-155,3,0,0,uFlags|SWP_NOSIZE);
	//导航按钮
	CRect rcNavigation;
	m_btNavigation[1].GetWindowRect(&rcNavigation);

	//导航按钮
	INT nNavigationArea=rcNavigation.Width()*CountArray(m_btNavigation);
	INT nSpace=0;//__min(__max((nWidth-nNavigationArea-328)/(CountArray(m_btNavigation)-1),0),49);

	//导航按钮
	for (INT i=0;i<CountArray(m_btNavigation);i++)
	{
		INT nIndex=CountArray(m_btNavigation)-i;
	//	m_btNavigation[i].GetWindowRect(&rcNavigation);
		INT nXExcursion=nWidth-(rcNavigation.Width()+nSpace)*nIndex-13;
//		if(i!=0)
		//	nXExcursion+=3;
		DeferWindowPos(hDwp,m_btNavigation[i],NULL,nXExcursion,31,0,0,uFlags|SWP_NOSIZE);
	}
	DeferWindowPos(hDwp,m_btHome,NULL,nWidth-(rcNavigation.Width()+nSpace)*8-16,31,0,0,uFlags|SWP_NOSIZE);

	//视图位置
	m_rcViewItem.left=rcSplitter.right+EncircleInfoItemFrame.nLBorder-10;
	m_rcViewItem.top=EncircleInfoFrame.nTBorder+EncircleInfoItemFrame.nTBorder-11;
	m_rcViewItem.right=nWidth-EncircleInfoFrame.nRBorder-EncircleInfoItemFrame.nRBorder+8;
	m_rcViewItem.bottom=nHeight-EncircleInfoFrame.nBBorder-EncircleInfoItemFrame.nBBorder+6;
	

	//类型框架
	CSize SizeGameTypeCtrl;
	m_WndGameTypeCtrl.GetControlSize(SizeGameTypeCtrl);
	if(m_cbShowNewsType == 1)
	{
		DeferWindowPos(hDwp,m_WndTopNews,NULL,rcSplitter.right-100,EncircleInfoFrame.nTBorder+EncircleInfoItemFrame.nTBorder-SizeGameTypeCtrl.cy+138,
			SizeGameTypeCtrl.cx-148+220,24,uFlags);
	}
	else if(m_cbShowNewsType == 2)
	{
		DeferWindowPos(hDwp,m_WndTopNews,NULL,rcSplitter.right+64-226,EncircleInfoFrame.nTBorder+EncircleInfoItemFrame.nTBorder-SizeGameTypeCtrl.cy+138,
			SizeGameTypeCtrl.cx+134,24,uFlags);

	}
	else 
	{
		DeferWindowPos(hDwp,m_WndTopNews,NULL,rcSplitter.right+62,EncircleInfoFrame.nTBorder+EncircleInfoItemFrame.nTBorder-SizeGameTypeCtrl.cy+138,
			SizeGameTypeCtrl.cx-92,24,uFlags);

	}


	//游戏广场
	DeferWindowPos(hDwp,m_PlazaViewItem,NULL,m_rcViewItem.left+18,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags);
	DeferWindowPos(hDwp,m_WndGameTypeCtrl,NULL,rcSplitter.right+16,EncircleInfoFrame.nTBorder+EncircleInfoItemFrame.nTBorder-SizeGameTypeCtrl.cy+500,
		SizeGameTypeCtrl.cx,SizeGameTypeCtrl.cy,uFlags);
	DeferWindowPos(hDwp,m_ServerViewItem,NULL,m_rcViewItem.left,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags);
	DeferWindowPos(hDwp,m_dlgUserAccount,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgHuiyuan,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgChongzhi,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgQukuan,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgHuodong,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);
	DeferWindowPos(hDwp,m_dlgTouZhu,NULL,0,m_rcViewItem.top,nWidth,m_rcViewItem.Height()+10,uFlags);

	//其他控件
	DeferWindowPos(hDwp,m_SkinSplitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);
	DeferWindowPos(hDwp,m_WndUserInfoCtrl,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder+14,rcSplitter.left-EncircleInfoFrame.nLBorder+20,rcUserInfoCtrl.Height(),uFlags);
	
	m_nTopNewX = EncircleInfoFrame.nLBorder+2;
	m_nTopNewY = EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()-25;
	//DeferWindowPos(hDwp,m_PlatformPublicize,NULL,EncircleInfoFrame.nLBorder+EncircleInfoAffiche.nLBorder-5,EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()+EncircleInfoAffiche.nTBorder+10,rcSplitter.left-EncircleInfoAffiche.nLBorder-EncircleInfoAffiche.nRBorder-EncircleInfoFrame.nLBorder+5,nHeight-EncircleInfoAffiche.nTBorder-EncircleInfoAffiche.nBBorder-EncircleInfoFrame.nTBorder-rcUserInfoCtrl.Height()-EncircleInfoFrame.nBBorder,uFlags);
	//DeferWindowPos(hDwp,m_PlatFormNoticePublicize,NULL,238,9,540,18,uFlags);
	//左下角广告
	//DeferWindowPos(hDwp,m_PlatFormLeftPublicize,NULL,EncircleInfoFrame.nLBorder+EncircleInfoAffiche.nLBorder-4,EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()+EncircleInfoAffiche.nTBorder+nHeight-EncircleInfoAffiche.nTBorder-EncircleInfoAffiche.nBBorder-EncircleInfoFrame.nTBorder-rcUserInfoCtrl.Height()-EncircleInfoFrame.nBBorder+25,197,147,uFlags);
	DeferWindowPos(hDwp,m_Grid,NULL,EncircleInfoFrame.nLBorder+EncircleInfoAffiche.nLBorder-5,EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()+EncircleInfoAffiche.nTBorder+45,rcSplitter.left-EncircleInfoAffiche.nLBorder-EncircleInfoAffiche.nRBorder-EncircleInfoFrame.nLBorder+7,nHeight-42-EncircleInfoAffiche.nTBorder-EncircleInfoAffiche.nBBorder-EncircleInfoFrame.nTBorder-rcUserInfoCtrl.Height()-EncircleInfoFrame.nBBorder,uFlags);
	DeferWindowPos(hDwp,m_logo,NULL,4,1,229,130,uFlags);
	DeferWindowPos(hDwp,m_Publicizeurl,NULL,13,nHeight-191,204,166,uFlags);

	//结束调整
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//更新界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//调整资源
VOID CPlatformFrame::RectifyResource(INT nWidth, INT nHeight)
{
	//调整判断
	if ((nWidth!=0)&&(nHeight!=0))
	{
		//框架位置
		CRect rcFrame;
		rcFrame.SetRect(SHADOW_CX,SHADOW_CY,nWidth-SHADOW_CX,nHeight-SHADOW_CY);

		//窗口区域
		CRgn RegionWindow;
		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,ROUND_CX,ROUND_CY);

		//设置区域
		SetWindowRgn(RegionWindow,TRUE);

		//分层窗口
		if (m_SkinLayered.m_hWnd!=NULL)
		{
			//建立缓冲
			CImage ImageBuffer;
			ImageBuffer.Create(nWidth,nHeight,32);

			//创建 DC
			CImageDC ImageDC(ImageBuffer);
			CDC * pBufferDC=CDC::FromHandle(ImageDC);

			//绘画界面
			DrawControlView(pBufferDC,nWidth,nHeight);

			//更新分层
			m_SkinLayered.InitLayeredArea(pBufferDC,255,rcFrame,CPoint(ROUND_CX,ROUND_CX),false);
		}
	}

	return;
}

//绘画界面
VOID CPlatformFrame::DrawControlView(CDC * pDC, INT nWidth, INT nHeight)
{
	pDC->SetBkColor(RGB(255,0,255));
	//框架位置
	tagEncircleInfo FrameEncircleInfo;
	tagEncircleInfo ItemFrameEncircleInfo;
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);
	m_ItemFrameEncircle.GetEncircleInfo(ItemFrameEncircleInfo);

	//拆分位置
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//位置转换
	ScreenToClient(&rcSplitter);

	//控件位置
	CRect rcUserInfoCtrl;
	m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//框架区域
	CRect rcFrame;
	rcFrame.SetRect(0,0,nWidth,nHeight-4);

	//公告位置
	CRect rcAffiche;
	rcAffiche.right=rcSplitter.left;
	rcAffiche.left=FrameEncircleInfo.nLBorder;
	rcAffiche.top=FrameEncircleInfo.nTBorder+rcUserInfoCtrl.Height()+8;
	rcAffiche.bottom=nHeight-FrameEncircleInfo.nBBorder-4;
	m_nTop = 0;//rcAffiche.top+45;

	//子项区域
	CRect rcItemFrame;
	rcItemFrame.left=rcSplitter.right;
	rcItemFrame.top=FrameEncircleInfo.nTBorder;
	rcItemFrame.right=nWidth-FrameEncircleInfo.nRBorder;
	rcItemFrame.bottom=nHeight-FrameEncircleInfo.nBBorder-4;

	//绘画框架
	m_FrameEncircle.DrawEncircleFrame(pDC,rcFrame);
	//m_AfficheEncircle.DrawEncircleFrame(pDC,rcAffiche);
	//m_ItemFrameEncircle.DrawEncircleFrame(pDC,rcItemFrame);
	return;
}

//激活房间
bool CPlatformFrame::ActiveServerViewItem()
{
	//激活判断
	ASSERT(m_ServerViewItem.m_hWnd!=NULL);
	if (m_ServerViewItem.m_hWnd==NULL) return false;

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//游戏广场
	DeferWindowPos(hDwp,m_PlazaViewItem,NULL,m_rcViewItem.left+18,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags);
	DeferWindowPos(hDwp,m_ServerViewItem,NULL,m_rcViewItem.left,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags|SWP_SHOWWINDOW);

	//结束调整
	EndDeferWindowPos(hDwp);

	return true;
}

//删除房间
bool CPlatformFrame::DeleteServerViewItem()
{
	//激活判断
	ASSERT(m_ServerViewItem.m_hWnd!=NULL);
	if (m_ServerViewItem.m_hWnd==NULL) return false;

	//销毁房间
	m_ServerViewItem.DestroyWindow();

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//游戏广场
	DeferWindowPos(hDwp,m_PlazaViewItem,NULL,m_rcViewItem.left+18,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags|SWP_SHOWWINDOW);
	DeferWindowPos(hDwp,m_ServerViewItem,NULL,m_rcViewItem.left,m_rcViewItem.top,m_rcViewItem.Width(),m_rcViewItem.Height(),uFlags|SWP_HIDEWINDOW);

	//结束调整
	EndDeferWindowPos(hDwp);

	return true;
}

//进入房间
bool CPlatformFrame::EntranceServerItem(CGameServerItem * pGameServerItem)
{
	//效验参数
	ASSERT(pGameServerItem!=NULL);
	if (pGameServerItem==NULL) return NULL;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//登录判断
	if (pGlobalUserData->dwUserID==0L) return NULL;

	//变量定义
	tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
	tagGameKind * pGameKind=&pGameServerItem->m_pGameKindItem->m_GameKind;

	//当前判断
	if (m_ServerViewItem.m_hWnd!=NULL)
	{
		m_ServerViewItem.InitServerViewItem(pGameServerItem);
// 		//提示消息
// 		TCHAR szBuffer[256]=TEXT("");
// 		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("正在进入 [ %s ] 游戏房间中，请稍后等待片刻..."),m_ServerViewItem.GetServerName());
// 
// 		//提示消息
// 		CInformation Information(this);
// 		Information.ShowMessageBox(szBuffer,MB_OK|MB_ICONWARNING,30);

		return true;
	}


	//安装判断
	if (pGameServerItem->m_pGameKindItem->m_dwProcessVersion==0L)
	{
		//获取版本
		CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
		CWHService::GetModuleVersion(pGameKind->szProcessName,pGameKindItem->m_dwProcessVersion);

		//下载游戏
		if (pGameServerItem->m_pGameKindItem->m_dwProcessVersion==0L)
		{
			CGlobalUnits * pCGlobalUnits=CGlobalUnits::GetInstance();
			pCGlobalUnits->DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,pGameServer->wServerID);

			return NULL;
		}

		//更新列表
		OnGameItemUpdate(pGameKindItem);
	}

	//创建房间
	try
	{
		//配置房间
		m_ServerViewItem.Create(IDD_GAME_SERVER,this);
		m_ServerViewItem.InitServerViewItem(pGameServerItem);
		
	}
	catch (LPCTSTR pszString)
	{
		//销毁房间
		m_ServerViewItem.DestroyWindow();

		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(pGameServer->szServerName,pszString,MB_ICONERROR);

		return false;
	}

	return true;
}

//关闭消息
VOID CPlatformFrame::OnClose()
{

	if(!m_bKillSocket)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//关闭提示
		if (pGlobalUserData->dwUserID!=0L)
		{
			//显示窗口
			CDlgEnquire DlgEnquire;
			INT_PTR nResult=DlgEnquire.DoModal();

			//命令处理
			switch (nResult)
			{
			case IDCANCEL:				//取消动作
				{
					return;
				}
			case IDC_CLOSE_SERVER:		//退出房间
				{
					//关闭房间
					if (m_ServerViewItem.m_hWnd!=NULL)
					{
						DeleteServerViewItem();
					}

					return;
				}
			case IDC_SWITCH_ACCOUNTS:	//切换帐号
				{
					if(m_dlgHuiyuan.IsWindowVisible())
					{
						m_dlgHuiyuan.OnShowWindow(false,1);
					}
					else if(m_dlgUserAccount.IsWindowVisible())
					{
						m_dlgUserAccount.OnShowWindow(false,1);
					}
					else if(m_dlgTouZhu.IsWindowVisible())
					{
						m_dlgTouZhu.OnShowWindow(false,1);
					}

					m_WndUserInfoCtrl.ReleaseFace();
					//投递消息
					PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);
					SendToServer(6);

					return;
				}
			}
		}
		SendToServer(6);

	}

	//m_MissionManager.DeleteMissionItem(this);
//	m_MissionManager.ClearMissionItem();
	__super::OnClose();
}
void CPlatformFrame::Loadweb()
{

	CString strPath;  //文件路径
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	int pos = strPath.ReverseFind('\\'); 
	strPath = strPath.Left(pos); 

	m_logo.Navigate(strPath +_T("\\logo.html"));
	m_logo.EnableWindow(FALSE);

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\GamePlaza.ini"),szDirectory);

	TCHAR szUrl[126] = TEXT("");
	GetPrivateProfileString(TEXT("PlazaUrl"),TEXT("PlazaUrlLbAdd"),TEXT(""),szUrl,CountArray(szUrl),szFileName);

	m_Publicizeurl.Navigate(szUrl);



}


//绘画背景
BOOL CPlatformFrame::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建 DC
	CImageDC ImageDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(ImageDC);

// 	CBitImage Affiche;
// 	Affiche.LoadFromResource(AfxGetInstanceHandle(),IDB_AFFICHE_GROUP);
// 	Affiche.DrawImageTile(pDC,rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height());
	//绘画界面
	DrawControlView(pBufferDC,rcClient.Width(),rcClient.Height());

	//加载资源
	CPngImage StationLogo;
	StationLogo.LoadImage(AfxGetInstanceHandle(),TEXT("PLATFORM_LOGO"));
	StationLogo.DrawImage(pBufferDC,1,0);
// 	CPngImage TitleLogo;
// 	TitleLogo.LoadImage(AfxGetInstanceHandle(),TEXT("TITLE_LOGO"));
// 	TitleLogo.DrawImage(pBufferDC,1,0);


	DrawTopUsers(pBufferDC);

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//位置消息
VOID CPlatformFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	//调整资源
	RectifyResource(cx,cy);

	return;
}

//建立消息
INT CPlatformFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	m_DlgStatus.SetStatusViewSink(this);
	//设置窗口
	ModifyStyle(WS_CAPTION,WS_MINIMIZEBOX|WS_MAXIMIZEBOX,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	m_top8Font.CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	theTime = CTime::GetCurrentTime();
	m_bShutdowSocket = true;
	
	
	//创建广场
	CRect rcCreate(0,0,0,0);
	m_PlazaViewItem.Create(IDD_GAME_PLAZA,this);

// 	m_dlgUserAccount.Create(IDD_USER_ACCOUNT,this);
// 	m_dlgUserAccount.ShowWindow(SW_HIDE);
// 	m_dlgHuiyuan.Create(CHuiYuanDlg::IDD,this);
// 	m_dlgHuiyuan.ShowWindow(SW_HIDE);
// 	m_dlgChongzhi.Create(CChongZhiDlg::IDD,this);
// 	m_dlgChongzhi.ShowWindow(SW_HIDE);
// 	m_dlgQukuan.Create(CQuKuanDlg::IDD,this);
// 	m_dlgQukuan.ShowWindow(SW_HIDE);
// 	m_dlgHuodong.Create(CHuoDongDlg::IDD,this);
// 	m_dlgHuodong.ShowWindow(SW_HIDE);
// 	m_dlgTouZhu.Create(CZhangHuTzhLogDlg::IDD,this);
// 	m_dlgTouZhu.ShowWindow(SW_HIDE);
	//创建控件
	m_SkinSplitter.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN_SPLITTER);
	m_WndGameTypeCtrl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_GAME_TYPE_CTRL);
	m_WndUserInfoCtrl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_USER_INFO_CTRL);
	

	m_logo.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_LOGO);
	m_Publicizeurl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_LB_ADD);

	m_WndTopNews.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_LAST_TOP_NEWS);

	m_Grid.Create(rcCreate,this,IDC_TOP_NWES);
	m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
	m_Grid.ShowWindow(SW_HIDE);
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	//创建按钮
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MIN);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);
	m_btSound.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SOUND);
	m_btNet.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_NET);

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btMin.SetButtonImage(IDB_BT_MIN,hInstance,true,false);
	m_btClose.SetButtonImage(IDB_BT_CLOSE,hInstance,true,false);
	m_btSound.SetButtonImage(IDB_BT_SOUND,hInstance,true,false);
	m_btNet.SetButtonImage(IDB_BT_NET,hInstance,true,false);


	//导航按钮
	for (INT i=0;i<CountArray(m_btNavigation);i++)
	{
		m_btNavigation[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_NAVIGATION+i);
	}
	m_btHome.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_HOME);
	m_btHome.SetButtonImage(IDB_BK_HOME_BTN,hInstance,true,false);
	//导航按钮
	m_btNavigation[0].SetButtonImage(IDB_BK_NAVIGATION1,TEXT("BT_NAVIGATION_1"),hInstance,true,false);
	m_btNavigation[1].SetButtonImage(IDB_BK_NAVIGATION2,TEXT("BT_NAVIGATION_2"),hInstance,true,false);
	m_btNavigation[2].SetButtonImage(IDB_BK_NAVIGATION3,TEXT("BT_NAVIGATION_3"),hInstance,true,false);
	m_btNavigation[3].SetButtonImage(IDB_BK_NAVIGATION4,TEXT("BT_NAVIGATION_4"),hInstance,true,false);
	m_btNavigation[4].SetButtonImage(IDB_BK_NAVIGATION5,TEXT("BT_NAVIGATION_5"),hInstance,true,false);
	m_btNavigation[5].SetButtonImage(IDB_BK_NAVIGATION6,TEXT("BT_NAVIGATION_6"),hInstance,true,false);
	m_btNavigation[6].SetButtonImage(IDB_BK_NAVIGATION7,TEXT("BT_NAVIGATION_7"),hInstance,true,false);


	//游戏列表
	m_ServerListData.SetServerListDataSink(this);

	//提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btNavigation[0],TEXT("我要充值"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[1],TEXT("我要取款"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[2],TEXT("账号管理"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[3],TEXT("团队管理"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[4],TEXT("投注记录"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[5],TEXT("活动中心"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[6],TEXT("客服中心"));
	m_ToolTipCtrl.AddTool(&m_btMin,TEXT("最小化"));
	m_ToolTipCtrl.AddTool(&m_btClose,TEXT("关闭大厅"));
	m_ToolTipCtrl.AddTool(&m_btSound,TEXT("声音"));
	m_ToolTipCtrl.AddTool(&m_btNet,TEXT("网络"));

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);


	//设置位置
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//位置调整
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//移动窗口
	CRect rcFrame;
	rcFrame.top=(rcArce.Height()-SizeRestrict.cy)/2;
	rcFrame.left=(rcArce.Width()-SizeRestrict.cx)/2;
	rcFrame.right=(rcArce.Width()+SizeRestrict.cx)/2;
	rcFrame.bottom=(rcArce.Height()+SizeRestrict.cy)/2;
	SetWindowPos(NULL,rcFrame.left,rcFrame.top,rcFrame.Width(),rcFrame.Height(),SWP_NOZORDER);



	//创建分层
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	m_SkinLayered.CreateLayered(this,rcWindow);

	//调整资源
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyResource(rcClient.Width(),rcClient.Height());


	Loadweb();


	return 0;
}

//显示消息
VOID CPlatformFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//显示分层
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		m_nNewsYPos=362+360;
		m_nNewsYPos -= 290;
	}
	else
	{
		m_tishiDlg.StartKillTimer();
		KillTimer(TimerNews);
		KillTimer(TimerZhunBeiData);
		KillTimer(TimerJiShi);
		KillTimer(TimerNews1);
		KillTimer(TimerLuckyNum);
		KillTimer(TimerReconnect);
	//	KillTimer(TimerTopNews);
	}
	return;
}

//位置改变
VOID CPlatformFrame::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//移动分层
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//鼠标消息
VOID CPlatformFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟按标题
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}
LRESULT CPlatformFrame::OnQueryGameResult(WPARAM wParam, LPARAM lParam)
{
	return 1;
}
//显示菜单框
LRESULT CPlatformFrame::OnShowMenu(WPARAM wParam, LPARAM lParam)
{
	bool bShowMenu = wParam;
	WORD wShowMenuType = lParam;
	m_WndGameTypeCtrl.ShowMenu(bShowMenu,wShowMenuType);
	return 1;
}
//转换彩币
LRESULT CPlatformFrame::OnZhuanhuanCaibi(WPARAM wParam, LPARAM lParam)
{
	//全局设置
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	DWORD dwJine = (DWORD)lParam;
	DOUBLE fJine = 0.0;

	fJine = (DOUBLE)dwJine/1000;

	if(fJine > pGlobalUserData->lScore)
	{
		MyMessageBox(L"余额不足！");
		return 1;
	}

		m_fJiaoyiScore = fJine;
		
		SendToServer(3);
	return 1;
}
//转换金币
LRESULT CPlatformFrame::OnZhuanhuanJinbi(WPARAM wParam, LPARAM lParam)
{
	DWORD dwJine = (DWORD)lParam;
	DOUBLE fJine = 0.0;

	fJine = (DOUBLE)dwJine/1000;

		if(fJine > theAccount.yue)
		{
			MyMessageBox(L"余额不足！");
			return 1;
		}
 
		m_fJiaoyiScore = fJine;

		SendToServer(2);
	return 1;
}

LRESULT CPlatformFrame::OnUpdateButton(WPARAM wParam, LPARAM lParam)
{
	WORD wClickedType = wParam;
	m_WndGameTypeCtrl.SetClicked(wClickedType);
	return 1;
}
LRESULT CPlatformFrame::OnUpDateAccoount(WPARAM wParam, LPARAM lParam)
{
	m_WndUserInfoCtrl.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	return 1;
}
LRESULT CPlatformFrame::OnShowXgmm(WPARAM wParam, LPARAM lParam)
{
	ShowXgmm();
	return 1;
}
LRESULT CPlatformFrame::OnRefreshYue(WPARAM wParam, LPARAM lParam)
{
	SendToServer(4);
	return 1;
}
LRESULT CPlatformFrame::OnFreshGame(WPARAM wParam, LPARAM lParam)
{
	
	m_PlazaViewItem.FreshGame();
	return 1;
}
LRESULT CPlatformFrame::OnTouZhuTishi(WPARAM wParam, LPARAM lParam)
{
	if(!m_bCanSend)
		return 1;
	CMD_GP_GetTouzhuTishi GetTouzhuTishi;
	ZeroMemory(&GetTouzhuTishi,sizeof(GetTouzhuTishi));

	GetTouzhuTishi.dwUserID = theAccount.user_id;
	//发送数据
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_TOUZHU_TISHI ,&GetTouzhuTishi,sizeof(GetTouzhuTishi));

	return 1;
}
void CPlatformFrame::ShowXgmm()
{
	OnCommand(402,0);
	m_dlgUserAccount.OnBnClickedBtnModifypwd();
}
//事件消息
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//事件处理
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//登录完成
		{
			//显示窗口
			ShowWindow(SW_SHOW);
		//	SetForegroundWindow();
			BringWindowToTop();
			m_PlazaViewItem.ShowWindow(SW_SHOW);
			m_PlazaViewItem.ShowTypeItemView();
			m_PlazaViewItem.SetLogonSuccess(true);

			//更新人数
			m_MissionList.UpdateMissionStatus(true);

			m_WndUserInfoCtrl.SetClicked(1,0);
			m_bCanSend = true;
			//获取指针
			CGlobalUnits * pGlobalUnits = (CGlobalUnits *)CGlobalUnits::GetInstance();
			if(pGlobalUnits==NULL) return 0L;
			m_tishiDlg.Create(CTiShiDlg::IDD,this);

			//用户甜饼			
			pGlobalUnits->WriteUserCookie();

			//查询新闻
			SendToServer(1);

			SendToServer(5);
			SendToServer(7);
			SendToServer(8);


		//	m_tishiDlg.SetTiShiMessage(L"1111111111");

			m_PlazaViewItem.CreateDlgCaipiao(3);
// 			//独立线程的下载开奖数据
// 			AfxBeginThread(GetLuckNumber, this);

			OnCommand(IDC_HOME,0);
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			if(pGlobalUserInfo != NULL)
			{
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				if(pGlobalUserData!=NULL&&pGlobalUserData->lUserMedal == 1111)
				{
					CInformation information;
					int nResult = information.ShowMessageBox(_T("由于您密码设置过于简单，请设置复杂登陆密码和取款密码！"),MB_ICONWARNING,30);
					if(nResult==1)
					{
						ShowXgmm();
					}		
				}
			}
		//	SetTimer(TimerTopNews,300*1000,NULL);
			
			SetTimer(TimerZhunBeiData, 300*1000, NULL);
			SetTimer(TimerJiShi, 1000, NULL);
			SetTimer(TimerLuckyNum, 3000, NULL);
			return 0L;
		}
	case EVENT_USER_LOGOUT:			//注销成功
		{
			//隐藏窗口
			ShowWindow(SW_HIDE);

			//关闭房间
			DeleteServerViewItem();

			//显示登录
			m_MissionLogon.ShowLogon();
// 			AvtiveMainMissionItem();
			m_WndUserInfoCtrl.ReleaseFace();
			return 0L;
		}
	case EVENT_DOWN_LOAD_FINISH:	//下载完成
		{
			//获取参数
			WORD wKindID=LOWORD(lParam);
			WORD wServerID=HIWORD(lParam);

			//更新列表
			CServerListData * pServerListData=CServerListData::GetInstance();
			if (pServerListData!=NULL) pServerListData->OnEventDownLoadFinish(wKindID);

			//进入房间
			if ((wKindID!=0)&&(wServerID!=0))
			{
				CGameServerItem * pGameServerItem=m_ServerListData.SearchGameServer(wServerID);
				if (pGameServerItem!=NULL) EntranceServerItem(pGameServerItem);
			}

			return 0L;
		}
	case EVENT_UPDATE_ONLINE: //人数更新
		{
			//更新界面
			m_PlazaViewItem.InValidateWndView(VIEW_MODE_SERVER);

			return 0L;
		}
	}

	return 0L;
}

//银行更新
LRESULT CPlatformFrame::OnMessageInsureUpdate(WPARAM wParam, LPARAM lParam)
{
	//提取数据
	tagInsurePlazaUpdata * pInsurePlazaUpdate = (tagInsurePlazaUpdata *)wParam;
	ASSERT(pInsurePlazaUpdate!=NULL);
	if(pInsurePlazaUpdate==NULL) return 0L;

	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if(pGlobalUserData==NULL) return 0;
    
	//更新银行
	pGlobalUserData->lScore=pInsurePlazaUpdate->lUserScore;
	pGlobalUserData->lInsure=pInsurePlazaUpdate->lUserInsure;

	//平台事件
	CPlatformEvent * pPlatformEvent = CPlatformEvent::GetInstance();
	ASSERT(pPlatformEvent!=NULL);
	pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
//暂时放置新闻NEWS
void CPlatformFrame::DrawTopUsers(CDC* pDC)
{
	if(!m_bGame)
		return;

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	CFont *pOldFont = pDC->SelectObject(&m_top8Font);

	//	16,362,235,122
#ifdef _DEBUG
	m_vecTitle.clear();
	m_vecTitle.push_back(L"的冯绍峰沙发上发顺丰是范德萨发顺丰风额外热舞热污染人 污染物热污染污染物而额外热温热我认为玩热舞热舞而我污染物热污染而额外热舞");
	m_vecTitle.push_back(L"的冯绍峰沙发上发顺丰是范德萨发顺丰风额外热舞热污染人 污染物热污染污染物而额外热温热我认为玩热舞热舞而我污染物热污染而额外热舞");
	m_vecTitle.push_back(L"的冯绍峰沙发上发顺丰是范德萨发顺丰风额外热舞热污染人 污染物热污染污染物而额外热温热我认为玩热舞热舞而我污染物热污染而额外热舞");
	m_vecTitle.push_back(L"的冯绍峰沙发上发顺丰是范德萨发顺丰风额外热舞热污染人 污染物热污染污染物而额外热温热我认为玩热舞热舞而我污染物热污染而额外热舞");
	m_vecTitle.push_back(L"的冯绍峰沙发上发顺丰是范德萨发顺丰风额外热舞热污染人 污染物热污染污染物而额外热温热我认为玩热舞热舞而我污染物热污染而额外热舞");
#endif
	//拆分位置
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//位置转换
	ScreenToClient(&rcSplitter);
	tagEncircleInfo FrameEncircleInfo;
	tagEncircleInfo ItemFrameEncircleInfo;
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);
	m_ItemFrameEncircle.GetEncircleInfo(ItemFrameEncircleInfo);
	//控件位置
	CRect rcUserInfoCtrl;
	m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//公告位置
	CRect rcAffiche;
	rcAffiche.right=rcSplitter.left;
	rcAffiche.left=FrameEncircleInfo.nLBorder;
	rcAffiche.top=FrameEncircleInfo.nTBorder+rcUserInfoCtrl.Height()+15;
	rcAffiche.bottom=rcClient.Height()-FrameEncircleInfo.nBBorder-12;
	m_nTop = 0;//rcAffiche.top+45;

	//m_AfficheEncircle.DrawEncircleFrame(pDC,rcAffiche);
	CPngImage ImageNews;
	ImageNews.LoadImage(AfxGetInstanceHandle(),TEXT("BK_FRAME_NEWS"));
	ImageNews.DrawImage(pDC,rcAffiche.left,rcAffiche.top);

// 	CRect rcText(24, 407, 190, 362+380);
// 	
// 	//pDC->SetTextCharacterExtra(3);
// 	CString strText;
// 	COLORREF crTextColor= pDC->SetTextColor(RGB(88, 78, 77));
// 	for(unsigned int n=0; n<m_vecTitle.size(); n++)
// 	{
// 		strText += m_vecTitle[n].c_str();
// 		strText += L"\r\n";
// 	}
// 	pDC->DrawText( strText, &rcText, DT_LEFT|DT_WORDBREAK);

//	 pDC->SetTextColor(crTextColor);
	pDC->SelectObject(pOldFont);

	return;

}

//新闻公告
void CPlatformFrame::GridCtrlInit()
{
	return;
	m_Grid.SetGridLines(GVL_HORZ);
	m_Grid.ModifyStyle(WS_BORDER|WS_VSCROLL|WS_HSCROLL, NULL, NULL);
	m_Grid.EnableTitleTips();
	m_Grid.SetFixedBkColor(RGB(87,21,38));
	//	m_Grid.SetFixedTextColor(FONT_BG_COLOR);
	m_Grid.SetGridColor(RGB(73, 0, 0));
	m_Grid.SetGridBkColor(RGB(73, 0, 0));
	m_Grid.SetGridLineColor(RGB(127,127,123));
	m_Grid.SetBkColor(RGB(73, 0, 0));
	m_Grid.SetEditable(false);
	m_Grid.SetTextBkColor(RGB(73, 0, 0));			//背景色
	m_Grid.SetTextColor(RGB(255,255,255));				//字体颜色
	m_Grid.SetRowCount(5);								//初始为10行
	m_Grid.SetColumnCount(2);							//初始化为11列
	//    m_Grid.SetFixedRowCount(1);							//表头为一行
	//    m_Grid.SetFixedColumnCount(1);					//表头为一列
	m_Grid.SetColumnWidth(0,0);							//设置0列宽 
	m_Grid.SetColumnWidth(1,231);
	

	CString strTxtQiHao;
	for (int row = 0; row < m_Grid.GetRowCount(); row++)
	{
		m_Grid.SetItemState(row,0, m_Grid.GetItemState(row,0) | GVIS_READONLY);			//只读属性
		m_Grid.SetItemState(row,1, m_Grid.GetItemState(row,1) | GVIS_READONLY);			//只读属性
		for (int col = 0; col < m_Grid.GetColumnCount(); col++)
		{ 
			//设置表格显示属性
			GV_ITEM Item; 
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			m_Grid.SetRowHeight(row, 23);		//设置各行高          
			if(col ==0)					//第(0，0)格
			{
				Item.nFormat = DT_CENTER|DT_WORDBREAK;
				//Item.strText.Format(_T("%ld"),col+1);
			}
			else if (col == 1)					//设置0行表头显示
			{        
				Item.nFormat = DT_CENTER|DT_WORDBREAK;
				if(row < m_vecTitle.size())
					Item.strText.Format(_T("%s"), m_vecTitle[row].c_str());
			}	
			m_Grid.SetItem(&Item); 

		}
	}
}



void CPlatformFrame::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);


	CGridCell* pCell = (CGridCell*)m_Grid.GetCell(pItem->iRow, pItem->iColumn);
	if(!m_Grid.GetItemText(pItem->iRow, 1).IsEmpty())
		MyMessageBox(m_Grid.GetItemText(pItem->iRow, 1));
}

//鼠标消息
VOID CPlatformFrame::OnMouseMove(UINT nFlags, CPoint Point)
{

	return;
}

