#include "StdAfx.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "ServerListView.h"

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define ITEM_SIZE					18									//子项高度
#define ITEM_HEIGHT					26									//子项高度

//滚动定义
#define WHELL_TIMES					2									//滚轮倍数
#define SCROLL_POS_LINE				10									//滚动像素

//////////////////////////////////////////////////////////////////////////////////

//时间定义
#define IDI_UPDATE_ONLINE			100									//更新人数
#define TIME_UPDATE_ONLINE			30000								//更新人数

//////////////////////////////////////////////////////////////////////////////////

//图标索引
#define IND_ROOT					0									//列表顶项
#define IND_TYPE					1									//游戏类型
#define IND_NODE					2									//游戏节点
#define IND_KIND_NODOWN				3									//还没下载
#define IND_KIND_UNKNOW				4									//未知类型
#define IND_SERVER_UPDATE			5									//收藏房间
#define IND_SERVER_ATTACH			6									//收藏房间
#define IND_SERVER_NORMAL			7									//正常房间
#define IND_SERVER_ENTRANCE			8									//当前使用
#define IND_SERVER_EVERENTRANCE		9									//曾经房间
#define IND_BROWSE					10									//浏览子项
#define IND_FUNCTION				11									//功能子项
#define IDC_GAME_TYPE_CTRL			310									//类型框架
//////////////////////////////////////////////////////////////////////////////////

#define SHOW_LAST_COUNT             6                                   //最近游戏数

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerListView, CWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMLClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)

	//自定消息
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerListView::CServerListView()
{
	//设置变量
	m_nXScroll=0;
	m_nYScroll=0;
	m_nScrollPos=0;
	m_nScrollPosMax=0;

	m_nShowType=1;

	//状态变量
	m_bHovering=false;
	m_bHoveKind=false;
	m_bLMouseDown=false;

	//辅助变量
	m_pItemMouseHover=NULL;
	m_pItemMouseDown=NULL;
	m_pServerItemHover=NULL;
	m_pServerItemDown=NULL;

	//内核变量
	m_GameImageMap.InitHashTable(PRIME_KIND);

	//创建字体
	//m_FontBold.CreateFont(-12,0,0,0,600,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));

	return;
}

//析构函数
CServerListView::~CServerListView()
{
	//保存记录
	SaveLastPlayGame();
}

//命令函数
BOOL CServerListView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	/*UINT nCommandID=LOWORD(wParam);

	//菜单命令
	switch (nCommandID)
	{
	case IDM_ENTER_SERVER:		//进入房间
		{
			//获取树项
			HTREEITEM hTreeItem=GetSelectedItem();

			//选择判断
			if (hTreeItem==NULL) return TRUE;

			//获取数据
			CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

			//命令处理
			if (pGameListItem!=NULL)
			{
				switch (pGameListItem->GetItemGenre())
				{
				case ItemGenre_Kind:	//游戏种类
					{
						//进入房间
						CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
						pPlatformFrame->EntranceServerItem((CGameKindItem *)pGameListItem);

						break;
					}
				case ItemGenre_Server:	//游戏房间
					{
						//进入房间
						CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
						pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameListItem);

						break;
					}
				}
			}

			return TRUE;
		}
	case IDM_SHRINK_LIST:		//收缩列表
		{
			//收缩列表
			HTREEITEM hCurrentItem=GetSelectedItem();
			if (hCurrentItem!=NULL)	Expand(hCurrentItem,TVE_COLLAPSE);

			return TRUE;
		}
	case IDM_EXPAND_LIST:		//展开列表
		{
			//展开列表
			HTREEITEM hCurrentItem=GetSelectedItem();
			if (hCurrentItem!=NULL)	Expand(hCurrentItem,TVE_EXPAND);

			return TRUE;
		}
	}*/

	return __super::OnCommand(wParam,lParam);
}

//窗口函数
LRESULT CServerListView::DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//双击消息
	switch (uMessage)
	{
	case WM_LBUTTONDOWN:		//鼠标消息
		{
			//鼠标坐标
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			//设置变量
			//m_hTreeClickExpand=NULL;

			////点击测试
			//HTREEITEM hTreeItem=HitTest(MousePoint);

			////消息处理
			//if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			//{
			//	//选择树项
			//	SetFocus();
			//	Select(hTreeItem,TVGN_CARET);

			//	//获取位置
			//	CRect rcTreeItem;
			//	GetItemRect(hTreeItem,&rcTreeItem,TRUE);

			//	//消息处理
			//	if (rcTreeItem.PtInRect(MousePoint)==TRUE)
			//	{
			//		//展开列表
			//		if (ExpandVerdict(hTreeItem)==false)
			//		{
			//			m_hTreeClickExpand=hTreeItem;
			//			Expand(m_hTreeClickExpand,TVE_EXPAND);
			//		}

			//		return 0;
			//	}
			//}

			break;
		}
	case WM_LBUTTONDBLCLK:		//鼠标消息
		{
			//鼠标坐标
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			////点击测试
			//HTREEITEM hTreeItem=HitTest(MousePoint);

			////下载判断
			//if (hTreeItem!=NULL)
			//{
			//	//获取数据
			//	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);
			//	if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Kind))
			//	{
			//		//变量定义
			//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

			//		//下载判断
			//		if (pGameKindItem->m_dwProcessVersion==0L)
			//		{
			//			//获取版本
			//			tagGameKind * pGameKind=&pGameKindItem->m_GameKind;
			//			CWHService::GetModuleVersion(pGameKind->szProcessName,pGameKindItem->m_dwProcessVersion);

			//			//下载判断
			//			if (pGameKindItem->m_dwProcessVersion==0L)
			//			{
			//				CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
			//				pGlobalUnits->DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,0);
			//			}
			//			else
			//			{
			//				OnGameItemUpdate(pGameKindItem);
			//			}
			//		}
			//	}
			//}

			////消息处理
			//if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			//{
			//	//选择树项
			//	SetFocus();
			//	Select(hTreeItem,TVGN_CARET);

			//	//位置判断
			//	CRect rcTreeItem;
			//	GetItemRect(hTreeItem,&rcTreeItem,TRUE);
			//	if (rcTreeItem.PtInRect(MousePoint)==FALSE) break;

			//	//展开判断
			//	if ((m_hTreeClickExpand!=hTreeItem)&&(ExpandVerdict(hTreeItem)==true))
			//	{
			//		//设置变量
			//		m_hTreeClickExpand=NULL;

			//		//展开列表
			//		Expand(hTreeItem,TVE_COLLAPSE);
			//	}

			//	return 0;
			//}

			break;
		}
	}

	return __super::DefWindowProc(uMessage,wParam,lParam);
}

//配置函数
VOID CServerListView::InitServerTreeView()
{
	CRect rcCreate(0,0,0,0);

	m_WndGameTypeCtrl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_GAME_TYPE_CTRL);

	m_WndGameTypeCtrl.ShowWindow(SW_HIDE);
	//设置列表
	m_ServerList.RemoveAll();

 	//设置滚动
 	m_SkinScrollBar.InitScroolBar(this);

	//设置字体
	SetFont(&CSkinResourceManager::GetInstance()->GetDefaultFont());

	return;
}

//绘画子项
VOID CServerListView::DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox)
{
	//变量定义
	tagServerListItem *pServerListItem=NULL;
	tagServerListItem *pPreServerListItem=NULL;
	WORD nPreItemPosY=0;
	POSITION pos=m_ServerList.GetHeadPosition();

	//工作路径
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(10,10,10));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	int nShowBack=0;
	//查找父项
	while(pos!=NULL)
	{
		pServerListItem=m_ServerList.GetNext(pos);
		if(pServerListItem)
		{
			if(m_nShowType==0||pServerListItem->pGameKindItem->m_GameKind.wTypeID==m_nShowType)
			{

				//资源目录
				TCHAR szResDirectory[LEN_KIND]=TEXT("");
				GetGameResDirectory(pServerListItem->pGameKindItem->m_GameKind,szResDirectory,CountArray(szResDirectory));

				//图片路径
				TCHAR szImageFile[MAX_PATH]=TEXT("");
				_sntprintf(szImageFile,CountArray(szImageFile),TEXT("%s\\GameKind\\%s.png"),szDirectory,szResDirectory);

				//类型图片
				TCHAR szImageTypeFile[MAX_PATH]=TEXT("");
				_sntprintf(szImageTypeFile,CountArray(szImageFile),TEXT("%s\\GameKind\\GameKind_%d.bmp"),szDirectory,nShowBack);
				
				CPngImage PngGame;
				if(!PngGame.LoadImage(szImageFile))
				{
					_sntprintf(szImageFile,CountArray(szImageFile),TEXT("%s\\GameKind\\UnInstall.png"),szDirectory);
					PngGame.LoadImage(szImageFile);
					_sntprintf(szImageTypeFile,CountArray(szImageFile),TEXT("%s\\GameKind\\GameKind_5.bmp"),szDirectory);
				}
				//加载图片
				CBitImage ImageKind;
				if(FAILED(ImageKind.Load(szImageTypeFile)))
				{
					ImageKind.LoadFromResource(AfxGetInstanceHandle(), IDB_GAMEKIND_NORMAL);
				}

				CPngImage PngGameStatus;
				PngGameStatus.LoadImage(AfxGetInstanceHandle(),TEXT("SERVER_ITEM_STATUS"));

			    
				//绘制kind
				INT nImageW=ImageKind.GetWidth()/2;
				INT nImageH=ImageKind.GetHeight();
				INT nIndex = ((m_bHoveKind&&m_pServerItemHover==NULL&&m_pItemMouseHover==pServerListItem)?1:0);
				ImageKind.Draw(pDC->GetSafeHdc(),pServerListItem->nPosX,pServerListItem->nPosY-m_nScrollPos,nImageW,nImageH,nIndex*nImageW,0,nImageW,nImageH);
                PngGame.DrawImage(pDC,pServerListItem->nPosX,pServerListItem->nPosY-m_nScrollPos-2);
				//绘制房间
				if(pServerListItem->bShowServer)
				{
					//加载资源
					CPngImage ImageItemT,ImageItemM;
					ImageItemT.LoadImage(AfxGetInstanceHandle(), TEXT("SERVER_ITEM_T"));
					ImageItemM.LoadImage(AfxGetInstanceHandle(), TEXT("SERVER_ITEM_M"));

					//绘画界面
					INT nStartX=pServerListItem->nPosX;
					INT nStartY=pServerListItem->nPosY+47-m_nScrollPos;
					//ImageItemT.DrawImage(pDC, nStartX, nStartY);

					//下载提示

					if(pServerListItem->ServerList.GetCount()==0)
					{
						//绘制背景
						ImageItemM.DrawImage(pDC, nStartX, nStartY,ImageItemM.GetWidth(),ImageItemM.GetHeight()/2,0,0,ImageItemM.GetWidth(),ImageItemM.GetHeight()/2);
						//pDC->FillSolidRect(nStartX+2,nStartY+19,206,1,RGB(146,143,143));
						
						//绘制名称
						pDC->SetTextColor(RGB(10,10,10));
						CRect rcText(nStartX+42, nStartY, pServerListItem->nPosX+210, nStartY+29);
						pDC->DrawText(TEXT("没有可用游戏房间"), -1, &rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
						continue;
					}

					//绘制房间
					CGameServerItem *pServerItem=NULL;
					POSITION ItemPos=pServerListItem->ServerList.GetHeadPosition();
					//nStartY+=2;
					while(ItemPos!=NULL)
					{
						pServerItem=pServerListItem->ServerList.GetNext(ItemPos);
						if(pServerItem)
						{
							//绘制背景
							//ImageItemM.DrawImage(pDC, nStartX, nStartY);
							//pDC->FillSolidRect(nStartX+2,nStartY+19,206,1,RGB(146,143,143));
							pDC->SetTextColor(RGB(255,213,127));
							if(m_pServerItemHover && m_pServerItemHover==pServerItem)
							{
								//pDC->FillSolidRect(nStartX, nStartY, 210, 21, RGB(221,255,255));
								
								ImageItemM.DrawImage(pDC, nStartX, nStartY,ImageItemM.GetWidth(),ImageItemM.GetHeight()/2,0,ImageItemM.GetHeight()/2,ImageItemM.GetWidth(),ImageItemM.GetHeight()/2);
							}
							else
							{
								ImageItemM.DrawImage(pDC, nStartX, nStartY,ImageItemM.GetWidth(),ImageItemM.GetHeight()/2,0,0,ImageItemM.GetWidth(),ImageItemM.GetHeight()/2);
								
							}
							/////
							/////SERVER_ITEM_STATUS
							LONG dwPer=pServerItem->m_GameServer.dwOnLineCount*100/100;///*pServerItem->m_GameServer.dwFullCount*/  暂时修改为100
							if(dwPer > 80)
							{
								PngGameStatus.DrawImage(pDC, nStartX+125, nStartY+6,PngGameStatus.GetWidth()/4,PngGameStatus.GetHeight(),PngGameStatus.GetWidth()/4*2,0,PngGameStatus.GetWidth()/4,PngGameStatus.GetHeight());
							}
							else if(dwPer > 40)
							{
								PngGameStatus.DrawImage(pDC, nStartX+125, nStartY+6,PngGameStatus.GetWidth()/4,PngGameStatus.GetHeight(),PngGameStatus.GetWidth()/4*1,0,PngGameStatus.GetWidth()/4,PngGameStatus.GetHeight());
							}
							else if(dwPer > 10)
							{
								PngGameStatus.DrawImage(pDC, nStartX+125, nStartY+6,PngGameStatus.GetWidth()/4,PngGameStatus.GetHeight(),PngGameStatus.GetWidth()/4*0,0,PngGameStatus.GetWidth()/4,PngGameStatus.GetHeight());
							}
							
							else
							{
								PngGameStatus.DrawImage(pDC, nStartX+125, nStartY+6,PngGameStatus.GetWidth()/4,PngGameStatus.GetHeight(),PngGameStatus.GetWidth()/4*3,0,PngGameStatus.GetWidth()/4,PngGameStatus.GetHeight());
							}
			
							//获取标题
							TCHAR szTitle[64]=TEXT("");
							GetGameItemTitle(pServerItem,szTitle,CountArray(szTitle));

							//绘制名称
							CRect rcText(nStartX+42, nStartY, pServerListItem->nPosX+210, nStartY+29);
							pDC->DrawText(szTitle, -1, &rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
							nStartY += 29;
						}
					}
				}

				nShowBack++;
				if(nShowBack>=5)
					nShowBack=0;
			}
			
		}
		
	}

	return;
}

//绘画背景
VOID CServerListView::DrawTreeBack(CDC * pDC, CRect & rcClient, CRect & rcClipBox)
{
	CString strLog;
	strLog.Format(L"LISTVIEW cx:%d,cy:%d",rcClient.Width(),rcClient.Height());
	OutputDebugString(strLog);
	//绘画背景
	pDC->FillSolidRect(&rcClient,RGB(19,81,44));

	//绘制横线
	//for (INT nYPos=m_nYScroll/ITEM_HEIGHT*ITEM_HEIGHT;nYPos<rcClient.Height();nYPos+=ITEM_HEIGHT)
	//{
	//	pDC->FillSolidRect(0,nYPos,rcClient.Width(),1,RGB(223,223,223));
	//}

	return;
}

//绘制文本
VOID CServerListView::DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected)
{
	//变量定义
	//COLORREF crString=RGB(150,150,150);
	//CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	////颜色定义
	//if (pGameListItem!=NULL)
	//{
	//	//选择字体
	//	switch (pGameListItem->GetItemGenre())
	//	{
	//	case ItemGenre_Type:		//种类子项
	//		{
	//			//设置颜色
	//			crString=RGB(0,0,0);

	//			//设置字体
	//			pDC->SelectObject(m_FontBold);

	//			break;
	//		}
	//	case ItemGenre_Inside:		//内部子项
	//		{
	//			//设置颜色
	//			crString=RGB(0,0,0);

	//			//设置字体
	//			CGameInsideItem * pGameInsideItem=(CGameInsideItem *)pGameListItem;
	//			pDC->SelectObject((pGameInsideItem->m_dwInsideID==0)?m_FontBold:CSkinResourceManager::GetInstance()->GetDefaultFont());

	//			break;
	//		}
	//	default:					//其他子项
	//		{
	//			//设置颜色
	//			crString=RGB(0,0,0);

	//			//设置字体
	//			pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	//		}
	//	}
	//}
	//else
	//{
	//	//设置颜色
	//	crString=RGB(0,0,0);

	//	//设置字体
	//	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	//}

	////设置环境
	//pDC->SetBkMode(TRANSPARENT);
	//pDC->SetTextColor(crString);

	////绘画字体
	//rcRect.right += 5;
	//CString strString=GetItemText(hTreeItem);
	//pDC->DrawText(strString,rcRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	return;
}

//获取通知
VOID CServerListView::OnGameItemFinish()
{
	ModifyListPos();

	//更新人数
	ASSERT(CMissionList::GetInstance()!=NULL);
	CMissionList::GetInstance()->UpdateOnLineInfo();
	SetTimer(IDI_UPDATE_ONLINE,TIME_UPDATE_ONLINE,NULL);

	CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
	pPlatformFrame->OnGameItemFinish();
	return;
}

//获取通知
VOID CServerListView::OnGameKindFinish(WORD wKindID)
{
	//获取类型
	//CServerListData * pServerListData=CServerListData::GetInstance();
	//CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(wKindID);

	////类型处理
	//if (pGameKindItem!=NULL)
	//{
	//	//变量定义
	//	LPCTSTR pszTitle=TEXT("没有可用游戏房间");
	//	HTREEITEM hItemAttachUpdate=pGameKindItem->m_hItemAttachUpdate;
	//	HTREEITEM hItemNormalUpdate=pGameKindItem->m_hItemNormalUpdate;

	//	//更新标题
	//	if (hItemAttachUpdate!=NULL) SetItemText(hItemAttachUpdate,pszTitle);
	//	if (hItemNormalUpdate!=NULL) SetItemText(hItemNormalUpdate,pszTitle);
	//}

	//
	ModifyListPos();

	return;
}

//更新通知
VOID CServerListView::OnGameItemUpdateFinish()
{
	//更新人数
	CPlatformFrame::GetInstance()->UpDataAllOnLineCount();

	return;
}

//插入通知
VOID CServerListView::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//插入处理
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//游戏种类
		{
			//插入种类
			m_WndGameTypeCtrl.InsertGameType((CGameTypeItem *)pGameListItem);

			m_WndGameTypeCtrl.SetClicked(1);
			break;
		}
	case ItemGenre_Kind:	//游戏类型
		{
			//变量定义
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			
			//test
			//if(pGameKindItem->m_GameKind.wKindID != 200)break;

			//构造数据
			tagServerListItem *pServerListItemNew = new tagServerListItem;
			pServerListItemNew->pGameKindItem = pGameKindItem;

			//变量定义
			bool bInsert=true;
			tagServerListItem *pServerListItem=NULL;
			POSITION pos=m_ServerList.GetHeadPosition();

			//查找父项
			while(pos!=NULL)
			{
				POSITION posTemp=pos;
				pServerListItem=m_ServerList.GetNext(pos);
				if(pServerListItem && (pServerListItem->pGameKindItem->m_GameKind.wSortID > pGameKindItem->m_GameKind.wSortID))
				{
					bInsert=false;
					m_ServerList.InsertBefore(posTemp, pServerListItemNew);
					break;
				}
			}

			//插入新项
			if(bInsert) m_ServerList.AddTail(pServerListItemNew);

			break;
		}
	case ItemGenre_Node:	//游戏节点
		{
			break;
		}
	case ItemGenre_Page:	//定制子项
		{
			break;
		}
	case ItemGenre_Server:	//游戏房间
		{
			//变量定义
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

			//变量定义
			tagServerListItem *pServerListItem=NULL;
			WORD wKindID=pGameServerItem->m_GameServer.wKindID;
			POSITION pos=m_ServerList.GetHeadPosition();

			//查找父项
			while(pos!=NULL)
			{
				pServerListItem=m_ServerList.GetNext(pos);
				if(pServerListItem && pServerListItem->pGameKindItem->m_GameKind.wKindID == wKindID)
				{
					break;
				}
			}

			//插入新项
			if (pServerListItem!=NULL)
			{
				//插入处理
				TCHAR szTitle[64]=TEXT("");
				GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

				//常规插入
				pServerListItem->ServerList.AddTail(pGameServerItem);
			}

			break;
		}
	}

	return;
}

//更新通知
VOID CServerListView::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//效验参数
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////插入处理
	//switch (pGameListItem->GetItemGenre())
	//{
	//case ItemGenre_Type:	//游戏种类
	//	{
	//		//变量定义
	//		CGameTypeItem * pGameTypeItem=(CGameTypeItem *)pGameListItem;

	//		//设置子项
	//		if (pGameTypeItem->m_hTreeItemAttach!=NULL)
	//		{
	//			LPCTSTR pszTypeName(pGameTypeItem->m_GameType.szTypeName);
	//			ModifyGameListItem(pGameTypeItem->m_hTreeItemAttach,pszTypeName,IND_TYPE);
	//		}

	//		//设置子项
	//		if (pGameTypeItem->m_hTreeItemNormal!=NULL)
	//		{
	//			LPCTSTR pszTypeName(pGameTypeItem->m_GameType.szTypeName);
	//			ModifyGameListItem(pGameTypeItem->m_hTreeItemNormal,pszTypeName,IND_TYPE);
	//		}

	//		break;
	//	}
	//case ItemGenre_Kind:	//游戏类型
	//	{
	//		//变量定义
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

	//		//构造数据
	//		TCHAR szTitle[64]=TEXT("");
	//		UINT uNormalImage=GetGameImageIndex(pGameKindItem);
	//		GetGameItemTitle(pGameKindItem,szTitle,CountArray(szTitle));

	//		//设置子项
	//		if (pGameKindItem->m_hTreeItemAttach!=NULL)
	//		{
	//			ModifyGameListItem(pGameKindItem->m_hTreeItemAttach,szTitle,uNormalImage);
	//		}

	//		//设置子项
	//		if (pGameKindItem->m_hTreeItemNormal!=NULL)
	//		{
	//			ModifyGameListItem(pGameKindItem->m_hTreeItemNormal,szTitle,uNormalImage);
	//		}

	//		break;
	//	}
	//case ItemGenre_Node:	//游戏节点
	//	{
	//		//变量定义
	//		CGameNodeItem * pGameNodeItem=(CGameNodeItem *)pGameListItem;

	//		//设置子项
	//		if (pGameNodeItem->m_hTreeItemAttach!=NULL)
	//		{
	//			LPCTSTR pszNodeName(pGameNodeItem->m_GameNode.szNodeName);
	//			ModifyGameListItem(pGameNodeItem->m_hTreeItemAttach,pszNodeName,IND_NODE);
	//		}

	//		//设置子项
	//		if (pGameNodeItem->m_hTreeItemNormal!=NULL)
	//		{
	//			LPCTSTR pszNodeName(pGameNodeItem->m_GameNode.szNodeName);
	//			ModifyGameListItem(pGameNodeItem->m_hTreeItemNormal,pszNodeName,IND_NODE);
	//		}

	//		break;
	//	}
	//case ItemGenre_Page:	//定制子项
	//	{
	//		//变量定义
	//		CGamePageItem * pGamePageItem=(CGamePageItem *)pGameListItem;

	//		//设置子项
	//		if (pGamePageItem->m_hTreeItemAttach!=NULL)
	//		{
	//			LPCTSTR pszDisplayName(pGamePageItem->m_GamePage.szDisplayName);
	//			ModifyGameListItem(pGamePageItem->m_hTreeItemAttach,pszDisplayName,IND_BROWSE);
	//		}

	//		//设置子项
	//		if (pGamePageItem->m_hTreeItemNormal!=NULL)
	//		{
	//			LPCTSTR pszDisplayName(pGamePageItem->m_GamePage.szDisplayName);
	//			ModifyGameListItem(pGamePageItem->m_hTreeItemNormal,pszDisplayName,IND_BROWSE);
	//		}

	//		break;
	//	}
	//case ItemGenre_Server:	//游戏房间
	//	{
	//		//变量定义
	//		CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

	//		//构造数据
	//		TCHAR szTitle[64]=TEXT("");
	//		UINT uImageIndex=GetGameImageIndex(pGameServerItem);
	//		GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

	//		//设置子项
	//		if (pGameServerItem->m_hTreeItemAttach!=NULL)
	//		{
	//			ModifyGameListItem(pGameServerItem->m_hTreeItemAttach,szTitle,uImageIndex);
	//		}

	//		//设置子项
	//		if (pGameServerItem->m_hTreeItemNormal!=NULL)
	//		{
	//			ModifyGameListItem(pGameServerItem->m_hTreeItemNormal,szTitle,uImageIndex);
	//		}

	//		//最近插入
	//		if(IsLastPlayGame(pGameServerItem->m_GameServer.wServerID))
	//		{
	//			HTREEITEM hTreeServerItem = EmunGameServerItem(m_LastServerItem.m_hTreeItemNormal, pGameServerItem->m_GameServer.wServerID);
	//			if(hTreeServerItem!=NULL)
	//			{
	//				ModifyGameListItem(hTreeServerItem,szTitle,uImageIndex);
	//			}
	//		}

	//		break;
	//	}
	//}

	return;
}

//删除通知
VOID CServerListView::OnGameItemDelete(CGameListItem * pGameListItem)
{
	//效验参数
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////删除树项
	//if (pGameListItem->m_hTreeItemAttach!=NULL) DeleteItem(pGameListItem->m_hTreeItemAttach);
	//if (pGameListItem->m_hTreeItemNormal!=NULL) DeleteItem(pGameListItem->m_hTreeItemNormal);

	////删除树项
	//if(pGameListItem->GetItemGenre() == ItemGenre_Server)
	//{
	//	HTREEITEM hTreeitem=EmunGameServerItem(m_LastServerItem.m_hTreeItemNormal,((CGameServerItem *)pGameListItem)->m_GameServer.wServerID);
	//	if(hTreeitem!=NULL) DeleteItem(hTreeitem);
	//}

	return;
}

//获取图标
UINT CServerListView::GetGameImageIndex(CGameKindItem * pGameKindItem)
{
	//安装判断
	//if (pGameKindItem->m_dwProcessVersion==0L) return IND_KIND_NODOWN;

	////寻找现存
	//UINT uImageIndxe=0;
	//tagGameKind * pGameKind=&pGameKindItem->m_GameKind;
	//if (m_GameImageMap.Lookup(pGameKind->wKindID,uImageIndxe)==TRUE) return uImageIndxe;

	////加载图标
	//if (pGameKindItem->m_dwProcessVersion!=0L)
	//{
	//	//加载资源
	//	LPCTSTR strProcessName(pGameKind->szProcessName);
	//	HINSTANCE hInstance=AfxLoadLibrary(strProcessName);

	//	//加载图形
	//	CBitmap GameLogo;
	//	AfxSetResourceHandle(hInstance);
	//	if (GameLogo.LoadBitmap(TEXT("GAME_LOGO"))) uImageIndxe=m_ImageList.Add(&GameLogo,RGB(255,0,255));
	//	AfxSetResourceHandle(GetModuleHandle(NULL));

	//	//释放资源
	//	AfxFreeLibrary(hInstance);

	//	//保存信息
	//	if (uImageIndxe!=0L)
	//	{
	//		m_GameImageMap[pGameKind->wKindID]=uImageIndxe;
	//		return uImageIndxe;
	//	}
	//}
	//
	return IND_KIND_NODOWN;
}

//获取图标
UINT CServerListView::GetGameImageIndex(CGameServerItem * pGameServerItem)
{
	//获取图标
	if (pGameServerItem->m_ServerStatus==ServerStatus_Normal) return IND_SERVER_NORMAL;
	if (pGameServerItem->m_ServerStatus==ServerStatus_Entrance) return IND_SERVER_ENTRANCE;
	if (pGameServerItem->m_ServerStatus==ServerStatus_EverEntrance) return IND_SERVER_EVERENTRANCE;

	return IND_SERVER_NORMAL;
}

//获取标题
LPCTSTR CServerListView::GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount)
{
	//变量定义
	LPCTSTR pszKindName(pGameKindItem->m_GameKind.szKindName);
	DWORD dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//负载信息
	/*TCHAR szLoadInfo[32] = {0};
	if(pParameterGlobal->m_bShowServerStatus)
		GetLoadInfoOfServer(&(pGameKindItem->m_GameKind), szLoadInfo, CountArray(szLoadInfo));
	else
		_sntprintf(szLoadInfo,CountArray(szLoadInfo),TEXT("%ld"),dwOnLineCount);

	//构造标题
	if (pGameKindItem->m_dwProcessVersion!=0L)
	{
		_sntprintf(pszTitle,uMaxCount,TEXT("%s"),(LPCTSTR)pszKindName);
	}
	else
	{
		_sntprintf(pszTitle,uMaxCount,TEXT("%s （双击下载）"),pszKindName);
	}
*/
	_sntprintf(pszTitle,uMaxCount,TEXT("%s"),(LPCTSTR)pszKindName);
	return pszTitle; 
}

//获取标题
LPCTSTR CServerListView::GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount)
{
	//变量定义
	LPCTSTR pszServerName(pGameServerItem->m_GameServer.szServerName);
	DWORD dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//负载信息
	/*TCHAR szLoadInfo[32] = {0};
	if(pParameterGlobal->m_bShowServerStatus)
		GetLoadInfoOfServer(&(pGameServerItem->m_GameServer), szLoadInfo, CountArray(szLoadInfo));
	else
		_sntprintf(szLoadInfo,CountArray(szLoadInfo),TEXT("%ld"),dwOnLineCount);*/

	//构造标题
	/*_sntprintf(pszTitle,uMaxCount,TEXT("%s [ %s ]"),pszServerName,szLoadInfo);*/
	_sntprintf(pszTitle,uMaxCount,TEXT("%s"),pszServerName);

	return pszTitle; 
}

//删除更新
VOID CServerListView::DeleteUpdateItem(CGameListItem * pGameListItem)
{
	//效验参数
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////删除更新
	//while (pGameListItem!=NULL)
	//{
	//	//类型判断
	//	if (pGameListItem->GetItemGenre()==ItemGenre_Kind)
	//	{
	//		//删除子项
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
	//		if (pGameKindItem->m_hItemAttachUpdate!=NULL) DeleteItem(pGameKindItem->m_hItemAttachUpdate);
	//		if (pGameKindItem->m_hItemNormalUpdate!=NULL) DeleteItem(pGameKindItem->m_hItemNormalUpdate);

	//		//设置变量
	//		pGameKindItem->m_hItemAttachUpdate=NULL;
	//		pGameKindItem->m_hItemNormalUpdate=NULL;

	//		break;
	//	}

	//	//获取父项
	//	pGameListItem=pGameListItem->GetParentListItem();
	//}

	return;
}

//修改子项
VOID CServerListView::ModifyGameListItem(HTREEITEM hTreeItem, LPCTSTR pszTitle, UINT uImage)
{
	//变量定义
	//TVITEM TVItem;
	//ZeroMemory(&TVItem,sizeof(TVItem));

	////子项属性
	//TVItem.hItem=hTreeItem;
	//TVItem.cchTextMax=64;
	//TVItem.iImage=uImage;
	//TVItem.iSelectedImage=uImage;
	//TVItem.pszText=(LPTSTR)pszTitle;
	//TVItem.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;

	////修改子项
	//SetItem(&TVItem);

	return;
}

//插入子项
HTREEITEM CServerListView::InsertInsideItem(LPCTSTR pszTitle, UINT uImage, DWORD dwInsideID, HTREEITEM hParentItem)
{
	//变量定义
	//TV_INSERTSTRUCT InsertStruct;
	//ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	////创建变量
	//CGameInsideItem * pGameInsideItem=new CGameInsideItem;
	//if (pGameInsideItem!=NULL) pGameInsideItem->m_dwInsideID=dwInsideID;

	////设置变量
	//InsertStruct.hParent=hParentItem;
	//InsertStruct.hInsertAfter=TVI_LAST;

	////子项属性
	//InsertStruct.item.cchTextMax=64;
	//InsertStruct.item.iImage=uImage;
	//InsertStruct.item.iSelectedImage=uImage;
	//InsertStruct.item.pszText=(LPTSTR)pszTitle;
	//InsertStruct.item.lParam=(LPARAM)pGameInsideItem;
	//InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	return NULL;//InsertItem(&InsertStruct);
}

//插入子项
HTREEITEM CServerListView::InsertGameListItem(LPCTSTR pszTitle, UINT uImage, CGameListItem * pGameListItem, HTREEITEM hParentItem)
{
	//变量定义
	//TV_INSERTSTRUCT InsertStruct;
	//ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	////设置变量
	//InsertStruct.hParent=hParentItem;
	//InsertStruct.hInsertAfter=TVI_FIRST;
	//InsertStruct.item.cchTextMax=64;
	//InsertStruct.item.iImage=uImage;
	//InsertStruct.item.iSelectedImage=uImage;
	//InsertStruct.item.pszText=(LPTSTR)pszTitle;
	//InsertStruct.item.lParam=(LPARAM)pGameListItem;
	//InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	////寻找子项
	//if ((hParentItem!=NULL)&&(pGameListItem!=NULL))
	//{
	//	//变量定义
	//	WORD wSortID=pGameListItem->GetSortID();
	//	HTREEITEM hTreeItem=GetChildItem(hParentItem);

	//	//枚举子项
	//	while (hTreeItem!=NULL)
	//	{
	//		//获取数据
	//		CGameListItem * pGameListTemp=(CGameListItem *)GetItemData(hTreeItem);

	//		//数据对比
	//		if ((pGameListTemp!=NULL)&&(pGameListTemp->GetSortID()>wSortID)) break;

	//		//设置结果
	//		InsertStruct.hInsertAfter=hTreeItem;

	//		//切换树项
	//		hTreeItem=GetNextItem(hTreeItem,TVGN_NEXT);
	//	} 
	//}

	return NULL;//InsertItem(&InsertStruct);
}

//清除列表
void CServerListView::ClearServerList()
{
	while(m_ServerList.GetCount()>0)
	{
		tagServerListItem * pServerListItem=m_ServerList.RemoveTail();
		if(pServerListItem) delete pServerListItem;
	}
}

//调整位置
void CServerListView::ModifyListPos()
{
	//变量定义
	tagServerListItem *pServerListItem=NULL;
	tagServerListItem *pPreServerListItem=NULL;
	WORD nPreItemPosY=0;
	POSITION pos=m_ServerList.GetHeadPosition();

	//查找父项
	while(pos!=NULL)
	{
		pServerListItem=m_ServerList.GetNext(pos);
		if(pServerListItem)
		{
			if(m_nShowType==0||pServerListItem->pGameKindItem->m_GameKind.wTypeID==m_nShowType)
			{
				INT nPreBottom=nPreItemPosY;
				if(pPreServerListItem)nPreBottom=nPreItemPosY+48;
				if(pPreServerListItem && pPreServerListItem->bShowServer)
				{
					if(pPreServerListItem->ServerList.GetCount()==0)
						nPreBottom=nPreBottom+(1*29)+4;
					else
						nPreBottom=nPreBottom+((INT)pPreServerListItem->ServerList.GetCount()*29)+4;
				}

				pServerListItem->nPosX=0;
				pServerListItem->nPosY=nPreBottom;

				nPreItemPosY=pServerListItem->nPosY;
				pPreServerListItem=pServerListItem;
			}

		}
	}

	//调整滚动
	//SetScrollMaxPos();
}

//调整位置
void CServerListView::SetScrollMaxPos()
{
	//列表判断
	//if(m_ServerList.GetCount()==0)
		return;

	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//计算位置
	tagServerListItem *pServerListItem =m_ServerList.GetTail();
	INT nTailBottom=pServerListItem->nPosY+52;
	if(pServerListItem->bShowServer)
	{
		if(pServerListItem->ServerList.GetCount()==0)
			nTailBottom=nTailBottom+(1*21)+7;
		else
			nTailBottom=nTailBottom+((INT)pServerListItem->ServerList.GetCount()*21)+7;
	}
	m_nScrollPosMax=nTailBottom;
	if(m_nScrollPosMax < rcClient.Height()) m_nScrollPosMax=rcClient.Height();

	//设置滚动
	SCROLLINFO ScrollInfoVert;
	ScrollInfoVert.cbSize=sizeof(ScrollInfoVert);
	ScrollInfoVert.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfoVert.nMin=0;
	ScrollInfoVert.nMax=m_nScrollPosMax;
	ScrollInfoVert.nPage=rcClient.Height();
	ScrollInfoVert.nPos=m_nScrollPos;
	SetScrollInfo(SB_VERT,&ScrollInfoVert,TRUE);
}

//资源目录
VOID CServerListView::GetGameResDirectory(tagGameKind &GameKind,TCHAR szResDirectory[], WORD wBufferCount)
{
	//变量定义
	WORD wStringIndex=0;

	//构造目录
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//字符终止
	szResDirectory[wStringIndex]=0;

	return;
}

//测试子项
tagServerListItem * CServerListView::HitTest(CPoint &MousePoint)
{
	//变量定义
	tagServerListItem *pServerListItem=NULL;
	tagServerListItem *pPreServerListItem=NULL;
	WORD nPreItemPosY=0;
	POSITION pos=m_ServerList.GetHeadPosition();

	//查找父项
	while(pos!=NULL)
	{
		pServerListItem=m_ServerList.GetNext(pos);
		if(pServerListItem)
		{
			if(m_nShowType==0||pServerListItem->pGameKindItem->m_GameKind.wTypeID==m_nShowType)
			{
				INT nBottom=pServerListItem->nPosY+52;;
				if(pServerListItem->bShowServer)
				{
					nBottom=nBottom+((INT)pServerListItem->ServerList.GetCount()*21)+7;
				}

				if(MousePoint.y>=(pServerListItem->nPosY-m_nScrollPos) && MousePoint.y <= (nBottom-m_nScrollPos))
				{
					return pServerListItem;
				}
			}
		}
	}

	return NULL;
}

//测试子项
CGameServerItem *CServerListView::HitSubTest(tagServerListItem *pServerListItem,CPoint &MousePoint)
{
	//参数效验
	if(pServerListItem==NULL) return NULL;
	if(pServerListItem->bShowServer==false) return NULL;

	//变量定义
	CGameServerItem *pGameServerItem=NULL;
	INT nPosY=pServerListItem->nPosY+52+7-m_nScrollPos;
	INT nBottom=nPosY+((INT)pServerListItem->ServerList.GetCount()*21);
	INT nIndex=(MousePoint.y-nPosY)/21;
	//if(nIndex>0) nIndex += (((MousePoint.y-nPosY)%21 > 0)?1:0);

	//范围判断
	if(MousePoint.y < nPosY || MousePoint.y > nBottom) return NULL;

	//房间判断
	INT nCount=0;
	POSITION pos=pServerListItem->ServerList.GetHeadPosition();
	while(pos!=NULL)
	{
		pGameServerItem = pServerListItem->ServerList.GetNext(pos);
		if(nCount==nIndex)
		{
			return pGameServerItem;
		}
		else
		{
			nCount++;
		}
	}

	return NULL;
}

//重画消息
VOID CServerListView::OnPaint()
{
	CPaintDC dc(this);

	//剪切位置
	CRect rcClip;
	dc.GetClipBox(&rcClip);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//设置 DC
	BufferDC.SelectObject(&BufferImage);

	//绘画控件
	DrawTreeBack(&BufferDC,rcClient,rcClip);
	DrawTreeItem(&BufferDC,rcClient,rcClip);

	//绘画背景
	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&BufferDC,rcClip.left,rcClip.top,SRCCOPY);

	//删除资源
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//时间消息
VOID CServerListView::OnTimer(UINT nIDEvent)
{
	//更新人数
	if (nIDEvent==IDI_UPDATE_ONLINE)
	{
		ASSERT(CMissionList::GetInstance()!=NULL);
		CMissionList::GetInstance()->UpdateOnLineInfo();

		return;
	}

	__super::OnTimer(nIDEvent);
}

//绘画背景
BOOL CServerListView::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//位置消息
VOID CServerListView::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

// 	//获取大小
// 	CRect rcClient;
// 	GetClientRect(&rcClient);
// 
// 	//获取信息
// 	SCROLLINFO ScrollInfoH;
// 	SCROLLINFO ScrollInfoV;
// 	ZeroMemory(&ScrollInfoH,sizeof(ScrollInfoH));
// 	ZeroMemory(&ScrollInfoV,sizeof(ScrollInfoV));
// 
// 	//获取信息
// 	GetScrollInfo(SB_HORZ,&ScrollInfoH,SIF_POS|SIF_RANGE);
// 	GetScrollInfo(SB_VERT,&ScrollInfoV,SIF_POS|SIF_RANGE);
// 
// 	//设置变量
// 	m_nXScroll=-ScrollInfoH.nPos;
// 	m_nYScroll=-ScrollInfoV.nPos;
// 
// 	//调整滚动
// 	SetScrollMaxPos();

	return;
}

//光标消息
BOOL CServerListView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

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

	//子项测试
	tagServerListItem * hItemMouseHover=HitTest(MousePoint);
	tagServerListItem * hItemLastHover = m_pItemMouseHover;
	CGameServerItem *   pServerLastHover = m_pServerItemHover;
	bool                bLastHoverKind=m_bHoveKind;
	if(hItemLastHover)
	{
		m_pServerItemHover=HitSubTest(hItemMouseHover, MousePoint);
	}

	//设置变量
	m_pItemMouseHover=hItemMouseHover;
	if(hItemMouseHover && MousePoint.y < (hItemMouseHover->nPosY+52-m_nScrollPos))m_bHoveKind=true;
	else m_bHoveKind=false;

	//重画判断
	if (hItemMouseHover!=hItemLastHover || pServerLastHover!=m_pServerItemHover || bLastHoverKind!=m_bHoveKind)
	{
		//重画界面
		Invalidate(FALSE);
	}

	//设置光标
	if (hItemMouseHover!=NULL)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//滚动消息
VOID CServerListView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
	return;
	//设置焦点
	SetFocus();

	//获取参数
	CRect rcClient;
	GetClientRect(&rcClient);
	INT nLastPos=m_nScrollPos;

	//移动坐标
	switch (nSBCode)
	{
	case SB_TOP:
		{
			m_nScrollPos=0;
			break;
		}
	case SB_BOTTOM:
		{
			m_nScrollPos=m_nScrollPosMax-rcClient.Height();
			break;
		}
	case SB_LINEUP:
		{
			m_nScrollPos-=SCROLL_POS_LINE;
			break;
		}
	case SB_PAGEUP:
		{
			m_nScrollPos-=rcClient.bottom;
			break;
		}
	case SB_LINEDOWN:
		{
			m_nScrollPos+=SCROLL_POS_LINE;
			break;
		}
	case SB_PAGEDOWN:
		{
			m_nScrollPos+=rcClient.bottom;
			break;
		}
	case SB_THUMBTRACK:
		{
			m_nScrollPos=nPos;
			break;
		}
	}

	//调整位置
	m_nScrollPos=__min(m_nScrollPos,m_nScrollPosMax-rcClient.bottom);
	if (m_nScrollPos<0) m_nScrollPos=0;

	//滚动窗口
	if (nLastPos!=m_nScrollPos)
	{
		SetScrollPos(SB_VERT,m_nScrollPos);
		ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);
	}

	InvalidateRect(NULL);

	return;
}

//鼠标消息
BOOL CServerListView::OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置滚动
	INT nLastPos=m_nScrollPos;
	INT nMaxPos=m_nScrollPosMax-rcClient.bottom;
	m_nScrollPos=__max(__min(m_nScrollPos-zDelta/WHELL_TIMES,nMaxPos),0L);

	//滚动窗口
	SetScrollPos(SB_VERT,m_nScrollPos);
	ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);
	//InvalidateRect(NULL);

	return TRUE;
}

//鼠标消息
LRESULT CServerListView::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_bHovering=false;

	//设置变量
	m_pItemMouseHover=NULL;
	m_pItemMouseDown=NULL;
	m_pServerItemHover=NULL;
	m_pServerItemDown=NULL;

	//设置界面
	InvalidateRect(NULL);

	return 0;
}

//鼠标消息
VOID CServerListView::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	//设置焦点
	SetFocus();

	//动作处理
	if ((m_bLMouseDown==false))
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_bLMouseDown=true;
		m_pItemMouseDown=HitTest(MousePoint);

		//动作处理
		if (m_pItemMouseDown!=NULL)
		{
			m_pServerItemDown=HitSubTest(m_pItemMouseDown, MousePoint);
		}
	}

	return;
}

//鼠标消息
VOID CServerListView::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	//结果动作
	BYTE cbButtonDown=0;
	WORD wViewItemDown=INVALID_WORD;

	//点击判断
	if (m_bLMouseDown==true)
	{
		//释放捕获
		ReleaseCapture();

		//设置变量
		m_bLMouseDown=false;

		//变量定义
		WORD wChairID=INVALID_CHAIR;
		CGameServerItem *pGameServerItem=NULL;
		tagServerListItem *pServerListItem=HitTest(MousePoint);

		//获取位置
		if ((pServerListItem!=NULL)&&(pServerListItem==m_pItemMouseDown))
		{
			//房间查找
			pGameServerItem=HitSubTest(pServerListItem, MousePoint);
		}

		WORD wKind = 0;
		if (pServerListItem!=NULL)
			wKind = pServerListItem->pGameKindItem->m_GameKind.wKindID;
		if((wKind == CZChongQingSSC)||(wKind == CZJiangXiSSC)||(wKind == CZXinJiangSSC)||(wKind == CZ_WUFEN_CAI)||(wKind == CZ_FENFEN_CAI)||(wKind == CZGD11Xuan5)||(wKind == CZSD11Xuan5)||(wKind == CZJX11Xuan5))
		{
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->SetBnEnterKind(wKind);
			return;
		}
		//结果处理
		if ((pGameServerItem!=NULL)&&(pServerListItem==m_pItemMouseDown)&&(m_pServerItemDown==pGameServerItem))
		{
			//设置变量
			m_pItemMouseDown=NULL;
			m_pServerItemDown=NULL;

			//进入房间
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameServerItem);
		}
		else
		{
			//仅点击游戏
			if(pServerListItem!=NULL && pServerListItem==m_pItemMouseDown)
			{
				pServerListItem->bShowServer = !pServerListItem->bShowServer;
				ModifyListPos();
				InvalidateRect(NULL);

// 				TCHAR szRuleLink[MAX_PATH]=TEXT("");
// 				WORD wKindID=pServerListItem->pGameKindItem->m_GameKind.wKindID;
// 				_sntprintf(szRuleLink,CountArray(szRuleLink),TEXT("%s/GameRule/%ld.html"),szPlatformLink,wKindID);
				//打开页面
// 				CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
// 				if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szRuleLink,false);
			}

			//设置变量
			m_pItemMouseDown=NULL;
			m_pServerItemDown=NULL;
		}
	}

	return;
}

//右键列表
VOID CServerListView::OnNMRClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	////获取选择
	//HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	////选择判断
	//if (hTreeItem==NULL) return;

	////设置选择
	//Select(hTreeItem,TVGN_CARET);

	//获取数据
/*	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);
	if (pGameListItem!=NULL)
	{
		switch (pGameListItem->GetItemGenre())
		{
		case ItemGenre_Kind:	//游戏种类
			{
				//变量定义
				TCHAR szBuffer[64]=TEXT("");
				CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

				//构造菜单
				CSkinMenu Menu;
				Menu.CreateMenu();

				//自动进入
				Menu.AppendMenu(IDM_ENTER_SERVER,TEXT("自动进入"));
				Menu.AppendSeparator();

				//下载游戏
				CW2CT pszKindName(pGameKindItem->m_GameKind.szKindName);
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("下载“%s”"),(LPCTSTR)pszKindName);
				Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,szBuffer);

				//当前版本
				if (pGameKindItem->m_dwProcessVersion!=0)
				{
					DWORD dwVersion=pGameKindItem->m_dwProcessVersion;
					_sntprintf(szBuffer,CountArray(szBuffer),TEXT("安装版本 %d.%d.%d.%d"),GetProductVer(dwVersion),
						GetMainVer(dwVersion),GetSubVer(dwVersion),GetBuildVer(dwVersion));
					Menu.AppendMenu(IDM_NULL_COMMAND,szBuffer,MF_GRAYED);
				}
				else Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,TEXT("没有安装（点击下载）"));

				//控制菜单
				Menu.AppendSeparator();
				bool bExpand=ExpandVerdict(hTreeItem);
				Menu.AppendMenu(bExpand?IDM_SHRINK_LIST:IDM_EXPAND_LIST,bExpand?TEXT("收缩列表"):TEXT("展开列表"));

				//弹出菜单
				Menu.TrackPopupMenu(this);

				return;
			}
		case ItemGenre_Server:	//游戏房间
			{
				//变量定义
				TCHAR szBuffer[64]=TEXT("");
				CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;
				CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;

				//构造菜单
				CSkinMenu Menu;
				Menu.CreateMenu();
				Menu.AppendMenu(IDM_ENTER_SERVER,TEXT("进入游戏房间"));
				Menu.AppendSeparator();
				Menu.AppendMenu(IDM_SET_COLLECTION,TEXT("设为常用服务器"));

				//下载游戏
				CW2CT pszKindName(pGameKindItem->m_GameKind.szKindName);
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("下载“%s”"),(LPCTSTR)pszKindName);
				Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,szBuffer);
				Menu.AppendSeparator();

				//当前版本
				if (pGameKindItem->m_dwProcessVersion!=0)
				{
					DWORD dwVersion=pGameKindItem->m_dwProcessVersion;
					_sntprintf(szBuffer,CountArray(szBuffer),TEXT("安装版本 %d.%d.%d.%d"),GetProductVer(dwVersion),
						GetMainVer(dwVersion),GetSubVer(dwVersion),GetBuildVer(dwVersion));
					Menu.AppendMenu(IDM_NULL_COMMAND,szBuffer,MF_GRAYED);
				}
				else Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,TEXT("没有安装（点击下载）"));

				//菜单设置
				bool Collection=false;//pGameServerItem->IsCollection();
				if (Collection==true) Menu.CheckMenuItem(IDM_SET_COLLECTION,MF_BYCOMMAND|MF_CHECKED);

				//弹出菜单
				Menu.TrackPopupMenu(this);

				return;
			}
		}
	}*/

	return;
}

//左击列表
VOID CServerListView::OnNMLClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//获取选择
	//HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	////选择判断
	//if (hTreeItem==NULL) return;

	////设置选择
	//Select(hTreeItem,TVGN_CARET);

	////获取数据
	//CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	////进入房间
	//if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Server))
	//{
	//	//进入房间
	//	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//	pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameListItem);

	//	return;
	//}

	////内部子项
	//if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Inside))
	//{
	//	//变量定义
	//	CGameInsideItem * pGameInsideItem=(CGameInsideItem *)pGameListItem;

	//	//工作目录
	//	TCHAR szDirectory[MAX_PATH]=TEXT("");
	//	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//	//构造路径
	//	TCHAR szAssistantPath[MAX_PATH]=TEXT("");
	//	_sntprintf(szAssistantPath,CountArray(szAssistantPath),TEXT("%s\\Collocate\\Collocate.INI"),szDirectory);

	//	//读取地址
	//	TCHAR szItemName[128]=TEXT(""),szAssistantLink[128]=TEXT("");
	//	_sntprintf(szItemName,CountArray(szItemName),TEXT("Assistant%ld"),pGameInsideItem->m_dwInsideID);
	//	GetPrivateProfileString(szItemName,TEXT("AssistantLink"),TEXT(""),szAssistantLink,CountArray(szAssistantLink),szAssistantPath);

	//	//连接地址
	//	if (szAssistantLink[0]!=0)
	//	{
	//		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//		if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szAssistantLink,false);
	//	}

	//	return;
	//}

	return;
}

//列表改变
VOID CServerListView::OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult)
{
	////获取选择
	//HTREEITEM hTreeItem=GetSelectedItem();

	////选择判断
	//if (hTreeItem==NULL) return;

	////获取数据
	//CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	////数据处理
	//if (pGameListItem!=NULL)
	//{
	//	switch (pGameListItem->GetItemGenre())
	//	{
	//	case ItemGenre_Kind:	//游戏种类
	//		{
	//			//变量定义
	//			WORD wGameID=((CGameKindItem *)pGameListItem)->m_GameKind.wGameID;

	//			//构造地址
	//			TCHAR szRuleLink[MAX_PATH]=TEXT("");
	//			_sntprintf(szRuleLink,CountArray(szRuleLink),TEXT("%s/GameRule.aspx?GameID=%ld"),szPlatformLink,wGameID);

	//			//打开页面
	//			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//			if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szRuleLink,false);

	//			return;
	//		}
	//	case ItemGenre_Page:	//定制类型
	//		{
	//			//变量定义
	//			WORD wPageID=((CGamePageItem *)pGameListItem)->m_GamePage.wPageID;

	//			//构造地址
	//			TCHAR szPageLink[MAX_PATH]=TEXT("");
	//			_sntprintf(szPageLink,CountArray(szPageLink),TEXT("%s/GamePage.aspx?PageID=%ld"),szPlatformLink,wPageID);

	//			//打开页面
	//			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//			if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szPageLink,false);

	//			return;
	//		}
	//	}
	//}

	//return;
}

//列表展开
VOID CServerListView::OnTvnItemexpanding(NMHDR * pNMHDR, LRESULT * pResult)
{
	////变量定义
	//LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	////动作处理
	//if (pNMTreeView->action==TVE_EXPAND)
	//{
	//	//获取子项
	//	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(pNMTreeView->itemNew.hItem);

	//	//子项处理
	//	if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Kind))
	//	{
	//		//变量定义
	//		DWORD dwNowTime=(DWORD)time(NULL);
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
	//		bool bTimeOut=(dwNowTime>=(pGameKindItem->m_dwUpdateTime+30L));

	//		//更新判断
	//		if ((pGameKindItem->m_bUpdateItem==false)||(bTimeOut==true))
	//		{
	//			//设置列表
	//			pGameKindItem->m_bUpdateItem=true;
	//			pGameKindItem->m_dwUpdateTime=(DWORD)time(NULL);

	//			//设置任务
	//			CMissionList * pMissionList=CMissionList::GetInstance();
	//			if (pMissionList!=NULL) pMissionList->UpdateServerInfo(pGameKindItem->m_GameKind.wKindID);
	//		}

	//		return;
	//	}
	//}

	return;
}

//获得房间负载信息
LPCTSTR CServerListView::GetLoadInfoOfServer(DWORD dwOnLineCount, DWORD dwMaxCount, LPTSTR pszBuffer, WORD wBufferSize)
{
	if(pszBuffer == NULL) return NULL;
	if(dwMaxCount == 0)dwMaxCount = 2;

	DWORD dwPer = (dwOnLineCount*100)/dwMaxCount;
	if(dwPer > 80)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("满员"));
	}
	else if(dwPer > 60)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("拥挤"));
	}
	else if(dwPer > 40)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("繁忙"));
	}
	else if(dwPer > 20)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("良好"));
	}
	else
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("空闲"));
	}

	return pszBuffer;
}

//获得房间负载信息
LPCTSTR CServerListView::GetLoadInfoOfServer(tagGameServer * pGameServer, LPTSTR pszBuffer, WORD wBufferSize)
{
	if(pGameServer == NULL || pszBuffer == NULL) return NULL;

	return GetLoadInfoOfServer(pGameServer->dwOnLineCount, 100/*pGameServer->dwFullCount*/, pszBuffer, wBufferSize);
}

//获得房间负载信息
LPCTSTR CServerListView::GetLoadInfoOfServer(tagGameKind * pGameKind, LPTSTR pszBuffer, WORD wBufferSize)
{
	if(pGameKind == NULL || pszBuffer == NULL) return NULL;

	return GetLoadInfoOfServer(pGameKind->dwOnLineCount,  100/*pGameServer->dwFullCount*/, pszBuffer, wBufferSize);
}

//加载记录
void CServerListView::LoadLastPlayGame()
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]={0};
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\ConstantlyPlayGame.wh"),szDirectory);

	//读取文件
	CFile file;
	if(file.Open(szFileName, CFile::modeRead))
	{
		//读取数据
		char buffer[128]={0};
		UINT uReadCount=file.Read(buffer, CountArray(buffer));
		uReadCount /= 2;

		//加入记录
		WORD *pServerIDArry = (WORD *)buffer;
		for(BYTE i=0; i<uReadCount; i++)
		{
			if(pServerIDArry[i]>0) m_LastPlayGameList.AddHead(pServerIDArry[i]);
		}

		//关闭文件
		file.Close();
	}

	return;
}

//保存记录
void CServerListView::SaveLastPlayGame()
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]={0};
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\ConstantlyPlayGame.wh"),szDirectory);

	//写入文件
	CFile file;
	if(file.Open(szFileName, CFile::modeCreate|CFile::modeWrite))
	{
		//设置数据
		POSITION Position=m_LastPlayGameList.GetHeadPosition();
		WORD wServerIDArry[SHOW_LAST_COUNT]={0};
		for(BYTE i=0; i<SHOW_LAST_COUNT; i++)
		{
			if(Position == NULL) break;

			wServerIDArry[i]=m_LastPlayGameList.GetNext(Position);
		}

		//写入数据
		file.Write(wServerIDArry, sizeof(wServerIDArry));

		//关闭文件
		file.Close();
	}

	return;
}
//////////////////////////////////////////////////////////////////////////////////
VOID CServerListView::ShowGameType(WORD wType)
{
	m_nShowType=wType;
	ModifyListPos();
	m_nScrollPos=0;
	//重画界面
	Invalidate(FALSE);
}
VOID CServerListView::ShowGameServer()
{
	//变量定义
	tagServerListItem *pServerListItem=NULL;

    POSITION pos=m_ServerList.GetHeadPosition();
	//查找父项
	while(pos!=NULL)
	{
		pServerListItem=m_ServerList.GetNext(pos);
		if(pServerListItem)
		{
			pServerListItem->bShowServer=!pServerListItem->bShowServer;
		}
	}
	ModifyListPos();
	m_nScrollPos=0;
	//重画界面
	Invalidate(FALSE);
}