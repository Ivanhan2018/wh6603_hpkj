#include "StdAfx.h"
#include "Resource.h"
#include "SkinMenu.h"

//////////////////////////////////////////////////////////////////////////////////
//常量定义

//间隙变量
#define SPACE_LEFT					8									//空隙大小
#define SPACE_RIGHT					3									//空隙大小

//大小变量
#define MENU_BAR_CX					30									//标题高度
#define MENU_ITEM_CY				40									//子项高度
#define SEPARATOR_CY				3									//拆分高度

//////////////////////////////////////////////////////////////////////////////////

//菜单变量
CMenuImageArray						CSkinMenu::m_MenuItemImage;			//图形子项
CMenuStringArray					CSkinMenu::m_MenuItemString;		//字符子项
CMenuSeparatorArray					CSkinMenu::m_MenuItemSeparator;		//拆分子项

//内核变量
CSkinMenuAttribute					CSkinMenuKernel::m_SkinAttribute;	//菜单属性

//////////////////////////////////////////////////////////////////////////////////
static HHOOK g_hook=NULL; // 全局钩子
static LRESULT WINAPI CallWndProc(int, WPARAM, LPARAM); // 安装的钩子的窗口过程
static LRESULT WINAPI MenuWndProc(HWND, UINT, WPARAM, LPARAM); // 用来处理菜单的窗口过程

// 如果需要去除菜单的外部边框，需要通过安装钩子，设置外框属性并改变菜单大小
WNDPROC oldWndProc = NULL; // 用来保存被替换的窗口过程
LRESULT WINAPI CallWndProc(int code, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* pStruct = (CWPSTRUCT*)lParam;
	while (code == HC_ACTION)
	{
		HWND hWnd = pStruct->hwnd;
		// 捕捉创建消息WM_CREATE，后面筛选为是否是菜单的创建
		if ( pStruct->message != WM_CREATE)
			break;
		TCHAR sClassName[10];
		int Count = ::GetClassName(hWnd, sClassName, sizeof(sClassName)/sizeof(sClassName[0]));
		// 检查是否菜单窗口，#32768为菜单类名
		if ( Count != 6 ||  _tcscmp(sClassName, _T("#32768")) != 0 )   
			break;

		WNDPROC lastWndProc = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);   
		if (lastWndProc != MenuWndProc)   
		{   
			// 替换菜单窗口过程  
			SetWindowLong(hWnd, GWL_WNDPROC, (long)MenuWndProc);   
			// 保留原有的窗口过程   
			oldWndProc = lastWndProc;  
			UnhookWindowsHookEx(g_hook);
		}
		break;   
	}

	return CallNextHookEx((HHOOK)WH_CALLWNDPROC, code, wParam, lParam); 
}

LRESULT WINAPI MenuWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{   
	LRESULT lResult;
	switch (message)   
	{      
	case WM_CREATE:
		{   
			// 首先要去掉菜单窗口的一些扩展风格
			// 包括：WS_BORDER、WS_EX_DLGMODALFRAME、WS_EX_WINDOWEDGE
			lResult = CallWindowProc(oldWndProc, hWnd, message, wParam, lParam);   
			DWORD dwStyle = ::GetWindowLong(hWnd,   GWL_STYLE);   
			DWORD dwNewStyle = (dwStyle & ~WS_BORDER);
			::SetWindowLong(hWnd, GWL_STYLE, dwNewStyle);   
			DWORD dwExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
			DWORD dwNewExStyle = (dwExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE));   
			::SetWindowLong(hWnd, GWL_EXSTYLE, dwNewExStyle);   
			return lResult; 
		}   
	case   WM_PRINT: // 此处阻止非客户区地绘制
		return CallWindowProc( oldWndProc, hWnd, WM_PRINTCLIENT, wParam, lParam);

	case   WM_WINDOWPOSCHANGING:   
		{   
			// 最后,由于我们在MeasureItem里指定了菜单大小，而系统会自动替菜单加边框，
			// 因此必须去掉此部分额外地尺寸，将菜单大小改小
			LPWINDOWPOS lpPos = (LPWINDOWPOS)lParam;   
			lpPos->cx -= 2*GetSystemMetrics(SM_CXBORDER)+4;   
			lpPos->cy -= 2*GetSystemMetrics(SM_CYBORDER)+4;
			lResult = CallWindowProc(oldWndProc, hWnd, message, wParam, lParam);  
			return 0;
		}   
	case   WM_GETICON:
		return 0;    
	default:   
		return  CallWindowProc( oldWndProc, hWnd, message, wParam, lParam);   
	}   
}
//构造函数
CSkinMenuAttribute::CSkinMenuAttribute()
{
	//字体颜色
	m_crNormalText=RGB(0,0,0);	
	m_crSelectText=RGB(0,0,0);	

	//边框颜色
	m_crMenuBar=RGB(0,0,0);	
	m_crSeparator=RGB(0,0,0);	
	m_crNormalBack=RGB(0,0,0);
	m_crSelectBack=RGB(0,0,0);	
	m_crSelectBorder=RGB(0,0,0);

	return;
}

//析构函数
CSkinMenuAttribute::~CSkinMenuAttribute() 
{
}

//配置资源
bool CSkinMenuAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//字体颜色
	m_crNormalText=RGB(243,214,180);//pSkinRenderManager->RenderColor(0.99,0.29);
	m_crSelectText=RGB(252,200,189);//pSkinRenderManager->RenderColor(1.95,0.75);

	//边框颜色
	m_crMenuBar=RGB(98,5,24);//pSkinRenderManager->RenderColor(0.35,0.93);
	m_crSeparator=RGB(208,186,189);//pSkinRenderManager->RenderColor(0.95,0.90);
	m_crNormalBack=RGB(98,5,24);//pSkinRenderManager->RenderColor(0.00,1.00);
	m_crSelectBack=RGB(253,157,134);//pSkinRenderManager->RenderColor(0.13,0.98);
	m_crSelectBorder=RGB(253,157,134);//pSkinRenderManager->RenderColor(0.66,0.87);

	
	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinMenuKernel::CSkinMenuKernel()
{
	m_ImageBackMid.LoadImage(GetModuleHandle(SKIN_CONTROL_DLL_NAME),TEXT("GAME_ITEM_BACK_MID"));
	m_ImageBackHover.LoadImage(GetModuleHandle(SKIN_CONTROL_DLL_NAME),TEXT("GAME_ITEM_HOVER"));

}

//析构函数
CSkinMenuKernel::~CSkinMenuKernel()
{
}

//绘画函数
VOID CSkinMenuKernel::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//效验参数
// 	ASSERT(lpDrawItemStruct->CtlType==ODT_MENU);
// 	if (lpDrawItemStruct->CtlType!=ODT_MENU) return;

	//变量定义
	CRect rcItem=lpDrawItemStruct->rcItem;
	CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	CSkinMenuItem * pSkinMenuItem=(CSkinMenuItem *)lpDrawItemStruct->itemData;

	//状态变量
	bool bChecked=((lpDrawItemStruct->itemState&ODS_CHECKED)!=0);
	bool bSelected=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);
	bool bGrayed=((lpDrawItemStruct->itemState&ODS_DISABLED)||(lpDrawItemStruct->itemState&ODS_GRAYED));

	//绘画背景
	COLORREF crMenuBar=m_SkinAttribute.m_crMenuBar;
	COLORREF crBorder=m_SkinAttribute.m_crSelectBorder;
	COLORREF crNormalBack=m_SkinAttribute.m_crNormalBack;
	//pDC->FillSolidRect(0,rcItem.top,MENU_BAR_CX,rcItem.Height(),m_SkinAttribute.m_crMenuBar);
	pDC->FillSolidRect(rcItem.left/*+MENU_BAR_CX*/,rcItem.top,rcItem.Width()/*-MENU_BAR_CX*/,rcItem.Height(),m_SkinAttribute.m_crNormalBack);

	m_ImageBackMid.DrawImage(pDC,rcItem.left,rcItem.top/*,m_ImageBackMid.GetWidth(),m_ImageBackMid.GetWidth(),0,0,m_ImageBackMid.GetWidth(),m_ImageBackMid.GetHeight()*/);
	//焦点边框
	if ((bSelected==true)&&(bGrayed==false)&&(pSkinMenuItem!=NULL)&&(pSkinMenuItem->m_MenuItemType!=MenuItemType_Separator))
	{
		m_ImageBackHover.DrawImage(pDC,rcItem.left,rcItem.top/*,m_ImageBackMid.GetWidth(),m_ImageBackMid.GetWidth(),0,0,m_ImageBackMid.GetWidth(),m_ImageBackMid.GetHeight()*/);
// 		pDC->Draw3dRect(rcItem.left+1,rcItem.top+1,rcItem.Width()-2,rcItem.Height()-2,crBorder,crBorder);
// 		pDC->FillSolidRect(rcItem.left+2,rcItem.top+2,rcItem.Width()-4,rcItem.Height()-4,m_SkinAttribute.m_crSelectBack);
	}

	////选择标志
	//if (bChecked==true)
	//{
	//	//加载资源
	//	CBitImage ImageMenuFlags;
	//	ImageMenuFlags.LoadFromResource(GetModuleHandle(SKIN_CONTROL_DLL_NAME),IDB_SKIN_MENU_FLAGS);

	//	//渲染资源
	//	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	//	if (pSkinRenderManager!=NULL) pSkinRenderManager->RenderImage(ImageMenuFlags);

	//	//绘画界面
	//	ImageMenuFlags.BitBlt(pDC->m_hDC,rcItem.left+1,rcItem.top+1,ImageMenuFlags.GetWidth()/2,
	//		ImageMenuFlags.GetHeight(),((bSelected==true)&&(bGrayed==false))?ImageMenuFlags.GetWidth()/2:0,0);
	//}

	////其他菜单
	//if (pSkinMenuItem==NULL)
	//{
	//	//获取信息
	//	MENUITEMINFO MenuItemInfo;
	//	MenuItemInfo.cbSize=sizeof(MenuItemInfo);
	//	MenuItemInfo.fMask=MIIM_FTYPE|MIIM_BITMAP;
	//	GetMenuItemInfo(lpDrawItemStruct->itemID,&MenuItemInfo);

	//	return;
	//}

	//界面菜单
	switch (pSkinMenuItem->m_MenuItemType)
	{
// 	case MenuItemType_Image:		//图形菜单
// 		{
// 			//变量定义
// 			CSkinMenuImage * pSkinMenuImage=(CSkinMenuImage *)pSkinMenuItem;
// 
// 			//创建 DC
// 			CDC ImageDC;
// 			ImageDC.CreateCompatibleDC(pDC);
// 			ImageDC.SelectObject(pSkinMenuImage->m_hBitmap);
// 
// 			//获取信息
// 			BITMAP BitmapInfo;
// 			GetObject(pSkinMenuImage->m_hBitmap,sizeof(BitmapInfo),&BitmapInfo);
// 
// 			//绘画位图
// 			INT nYPos=rcItem.top+3;
// 			INT nXPos=rcItem.left+MENU_BAR_CX+SPACE_LEFT;
// 			pDC->BitBlt(nXPos,nYPos,BitmapInfo.bmWidth,BitmapInfo.bmHeight,&ImageDC,0,0,SRCCOPY);
// 
// 			//释放 DC
// 			ImageDC.DeleteDC();
// 
// 			break;
// 		}
	case MenuItemType_String:		//字符菜单
		{
			//变量定义
			CSkinMenuString * pSkinMenuString=(CSkinMenuString *)pSkinMenuItem;

			//设置颜色
			pDC->SetBkMode(TRANSPARENT);
			pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

			//设置颜色
			if (bGrayed==true) pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
			else if (bSelected==true) pDC->SetTextColor(m_SkinAttribute.m_crSelectText);
			else pDC->SetTextColor(m_SkinAttribute.m_crNormalText);

			int nLength = pSkinMenuString->m_strString.GetLength();
			//计算长度
			CSize SizeString;
			GetTextExtentPoint32(pDC->m_hDC,pSkinMenuString->m_strString,pSkinMenuString->m_strString.GetLength(),&SizeString);

			//绘画字符
			CRect rcString;
			rcString.top=rcItem.top;
			rcString.bottom=rcItem.bottom;
			rcString.left=(rcItem.Width()-SizeString.cx)/2+rcItem.left/*+MENU_BAR_CX+SPACE_LEFT*/;
			rcString.right=rcString.left+nLength;
			pDC->DrawText(pSkinMenuString->m_strString,nLength,&rcString,DT_SINGLELINE|DT_VCENTER|DT_NOCLIP);

			break;
		}
	case MenuItemType_Separator:	//拆分菜单
		{
			//绘画拆分
			pDC->FillSolidRect(rcItem.left/*+MENU_BAR_CX+SPACE_LEFT*/,rcItem.top+1,rcItem.Width()/*-MENU_BAR_CX-SPACE_LEFT-SPACE_RIGHT*/,
				rcItem.Height()-2,m_SkinAttribute.m_crSeparator);

			break;
		}
	}


	return;
}

//测量位置
VOID CSkinMenuKernel::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//效验参数
	ASSERT(lpMeasureItemStruct->CtlType==ODT_MENU);
	if (lpMeasureItemStruct->CtlType!=ODT_MENU) return;

	//获取数据
	CSkinMenuItem * pSkinMenuItem=(CSkinMenuItem *)lpMeasureItemStruct->itemData;
	if (pSkinMenuItem==NULL)
	{
		//获取信息
		MENUITEMINFO MenuItemInfo;
		MenuItemInfo.cbSize=sizeof(MenuItemInfo);
		MenuItemInfo.fMask=MIIM_FTYPE|MIIM_STRING;
		GetMenuItemInfo(lpMeasureItemStruct->itemID,&MenuItemInfo);

		return;
	}

	//界面菜单
	switch (pSkinMenuItem->m_MenuItemType)
	{
	case MenuItemType_Image:		//图形菜单
		{
			//变量定义
			CSkinMenuImage * pSkinMenuImage=(CSkinMenuImage *)pSkinMenuItem;

			//获取信息
			BITMAP BitmapInfo;
			GetObject(pSkinMenuImage->m_hBitmap,sizeof(BitmapInfo),&BitmapInfo);

			//计算大小
			lpMeasureItemStruct->itemHeight=BitmapInfo.bmHeight+6;
			lpMeasureItemStruct->itemWidth=BitmapInfo.bmWidth+MENU_BAR_CX+SPACE_LEFT+SPACE_RIGHT;

			break;
		}
	case MenuItemType_String:		//字符菜单
		{
			//变量定义
			CSkinMenuString * pSkinMenuString=(CSkinMenuString *)pSkinMenuItem;

			//设置 DC
			HDC hDC=GetDC(NULL);
			SelectObject(hDC,CSkinResourceManager::GetInstance()->GetDefaultFont());

			//计算长度
			CSize SizeString;
			GetTextExtentPoint32(hDC,pSkinMenuString->m_strString,pSkinMenuString->m_strString.GetLength(),&SizeString);

			//计算位置
			lpMeasureItemStruct->itemHeight=MENU_ITEM_CY;
			lpMeasureItemStruct->itemWidth=110;//SizeString.cx+MENU_BAR_CX+SPACE_LEFT+SPACE_RIGHT;

			//释放资源
			ReleaseDC(NULL,hDC);

			break;
		}
	case MenuItemType_Separator:	//拆分菜单
		{
			lpMeasureItemStruct->itemWidth=0;
			lpMeasureItemStruct->itemHeight=SEPARATOR_CY;

			break;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinMenu::CSkinMenu()
{
	m_bRemoveBorder=true;
}

//析构函数
CSkinMenu::~CSkinMenu()
{
	DestroyMenu();
}

//创建菜单
bool CSkinMenu::CreateMenu()
{
	return m_SkinMenuKernel.CreatePopupMenu()?true:false;
}

//销毁菜单
bool CSkinMenu::DestroyMenu()
{
	//销毁菜单
	m_SkinMenuKernel.DestroyMenu();

	//释放子项
	for (INT_PTR i=0;i<m_MenuItemActive.GetCount();i++)
	{
		FreeMenuItem(m_MenuItemActive[i]);
	}
	m_MenuItemActive.RemoveAll();

	return true;
}
void CSkinMenu::RemoveMenuBorder(HINSTANCE hInst, BOOL bRemove /* = TRUE */)
{
	m_bRemoveBorder = bRemove; // 标识

	// 需要移除边框时，要安装钩子
	if (m_bRemoveBorder)
	{
		DWORD id = ::GetCurrentThreadId(); // 获取当前线程的ID
		g_hook = SetWindowsHookEx(WH_CALLWNDPROC,CallWndProc,hInst,id);
	}
}
//弹出菜单
bool CSkinMenu::TrackPopupMenu(CWnd * pWnd)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	//弹出菜单
	m_SkinMenuKernel.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,MousePoint.x,MousePoint.y,pWnd);

	return true;
}

//弹出菜单
bool CSkinMenu::TrackPopupMenu(INT nXPos, INT nYPos, CWnd * pWnd)
{
	RemoveMenuBorder(GetModuleHandle(SKIN_CONTROL_DLL_NAME));
	//弹出菜单
	m_SkinMenuKernel.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,nXPos,nYPos,pWnd,NULL)?true:false;

	return true;
}

//插入拆分
bool CSkinMenu::AppendSeparator()
{
	//获取子项
	CSkinMenuSeparator * pSkinMenuItem=(CSkinMenuSeparator *)AcitveMenuItem(MenuItemType_Separator);

	//插入菜单
	if (pSkinMenuItem!=NULL)
	{
		m_SkinMenuKernel.AppendMenu(MF_OWNERDRAW,0,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//插入位图
bool CSkinMenu::AppendMenu(UINT nMenuID, HBITMAP hBitmap, UINT nFlags)
{
	//类型判断
	ASSERT((nFlags&MF_SEPARATOR)==0);
	if ((nFlags&MF_SEPARATOR)!=0) return false;

	//获取子项
	CSkinMenuImage * pSkinMenuItem=(CSkinMenuImage *)AcitveMenuItem(MenuItemType_Image);

	//插入菜单
	if (pSkinMenuItem!=NULL)
	{
		pSkinMenuItem->m_hBitmap=hBitmap;
		m_SkinMenuKernel.AppendMenu(MF_OWNERDRAW|nFlags,nMenuID,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//插入字符
bool CSkinMenu::AppendMenu(UINT nMenuID, LPCTSTR pszString, UINT nFlags)
{
	//类型判断
	ASSERT((nFlags&MF_SEPARATOR)==0);
	if ((nFlags&MF_SEPARATOR)!=0) return false;

	//获取子项
	CSkinMenuString * pSkinMenuItem=(CSkinMenuString *)AcitveMenuItem(MenuItemType_String);

	//插入菜单
	if (pSkinMenuItem!=NULL)
	{
		pSkinMenuItem->m_strString=pszString;
		m_SkinMenuKernel.AppendMenu(MF_OWNERDRAW|nFlags,nMenuID,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//插入拆分
bool CSkinMenu::InsertSeparator(UINT nPosition)
{
	//获取子项
	CSkinMenuSeparator * pSkinMenuItem=(CSkinMenuSeparator *)AcitveMenuItem(MenuItemType_Separator);

	//插入菜单
	if (pSkinMenuItem!=NULL)
	{
		m_SkinMenuKernel.InsertMenu(nPosition,MF_OWNERDRAW,0,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//插入位图
bool CSkinMenu::InsertMenu(UINT nMenuID, HBITMAP hBitmap, UINT nPosition, UINT nFlags)
{
	//类型判断
	ASSERT((nFlags&MF_SEPARATOR)==0);
	if ((nFlags&MF_SEPARATOR)!=0) return false;

	//获取子项
	CSkinMenuImage * pSkinMenuItem=(CSkinMenuImage *)AcitveMenuItem(MenuItemType_Image);

	//插入菜单
	if (pSkinMenuItem!=NULL)
	{
		pSkinMenuItem->m_hBitmap=hBitmap;
		m_SkinMenuKernel.InsertMenu(nPosition,MF_OWNERDRAW|nFlags,nMenuID,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//插入字符
bool CSkinMenu::InsertMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition, UINT nFlags)
{
	//类型判断
	ASSERT((nFlags&MF_SEPARATOR)==0);
	if ((nFlags&MF_SEPARATOR)!=0) return false;

	//获取子项
	CSkinMenuString * pSkinMenuItem=(CSkinMenuString *)AcitveMenuItem(MenuItemType_String);

	//插入菜单
	if (pSkinMenuItem!=NULL)
	{
		pSkinMenuItem->m_strString=pszString;
		m_SkinMenuKernel.InsertMenu(nPosition,MF_OWNERDRAW|nFlags,nMenuID,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//删除菜单
bool CSkinMenu::RemoveMenu(UINT nPosition, UINT nFlags)
{
	return m_SkinMenuKernel.RemoveMenu(nPosition,nFlags)?true:false;
}

//修改菜单
bool CSkinMenu::ModifyMenu(UINT nMenuID, HBITMAP hBitmap, UINT nPosition, UINT nFlags)
{
	return 0;
}

//修改菜单
bool CSkinMenu::ModifyMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition, UINT nFlags)
{
	return 0;
}

//设置默认
bool CSkinMenu::SetDefaultItem(UINT uItem, BOOL fByPos)
{
	return m_SkinMenuKernel.SetDefaultItem(uItem,fByPos)?true:false;
}

//选择控制
UINT CSkinMenu::CheckMenuItem(UINT nMenuID, UINT nCheck)
{
	return m_SkinMenuKernel.CheckMenuItem(nMenuID,nCheck)?true:false;
}

//启用控制
UINT CSkinMenu::EnableMenuItem(UINT nMenuID, UINT nEnable)
{
	return m_SkinMenuKernel.EnableMenuItem(nMenuID,nEnable)?true:false;
}
//鼠标移动
VOID CSkinMenu::OnMouseMove(UINT nFlags, CPoint MousePoint)
{
	
}
//释放子项
VOID CSkinMenu::FreeMenuItem(CSkinMenuItem * pSkinMenuItem)
{
	//效验参数
	ASSERT(pSkinMenuItem!=NULL);
	if (pSkinMenuItem==NULL) return;

	//清理变量
	switch (pSkinMenuItem->m_MenuItemType)
	{
	case MenuItemType_Image:		//图形类型
		{
			//变量定义
			CSkinMenuImage * pSkinMenuImage=(CSkinMenuImage *)pSkinMenuItem;

			//设置变量
			pSkinMenuImage->m_hBitmap=NULL;
			m_MenuItemImage.Add(pSkinMenuImage);

			break;
		}
	case MenuItemType_String:		//字符类型
		{
			//变量定义
			CSkinMenuString * pSkinMenuString=(CSkinMenuString *)pSkinMenuItem;

			//设置变量
			pSkinMenuString->m_strString.Empty();
			m_MenuItemString.Add(pSkinMenuString);

			break;
		}
	case MenuItemType_Separator:	//拆分类型
		{
			//变量定义
			CSkinMenuSeparator * pSkinMenuSeparator=(CSkinMenuSeparator *)pSkinMenuItem;

			//设置变量
			m_MenuItemSeparator.Add(pSkinMenuSeparator);

			break;
		}
	}

	return;
}

//获取子项
CSkinMenuItem * CSkinMenu::AcitveMenuItem(enMenuItemType MenuItemType)
{
	//变量定义
	CSkinMenuItem * pSkinMenuItem=NULL;

	//创建子项
	switch (MenuItemType)
	{
	case MenuItemType_Image:		//图形类型
		{
			if (m_MenuItemImage.GetCount()>0)
			{
				INT_PTR nItemCount=m_MenuItemImage.GetCount();
				pSkinMenuItem=m_MenuItemImage[nItemCount-1];
				m_MenuItemImage.RemoveAt(nItemCount-1);
			}
			else
			{
				pSkinMenuItem=new CSkinMenuImage;
				if (pSkinMenuItem==NULL) return NULL;
			}

			break;
		}
	case MenuItemType_String:		//字符类型
		{
			if (m_MenuItemString.GetCount()>0)
			{
				INT_PTR nItemCount=m_MenuItemString.GetCount();
				pSkinMenuItem=m_MenuItemString[nItemCount-1];
				m_MenuItemString.RemoveAt(nItemCount-1);
			}
			else
			{
				pSkinMenuItem=new CSkinMenuString;
				if (pSkinMenuItem==NULL) return NULL;
			}

			break;
		}
	case MenuItemType_Separator:	//拆分类型
		{
			if (m_MenuItemSeparator.GetCount()>0)
			{
				INT_PTR nItemCount=m_MenuItemSeparator.GetCount();
				pSkinMenuItem=m_MenuItemSeparator[nItemCount-1];
				m_MenuItemSeparator.RemoveAt(nItemCount-1);
			}
			else
			{
				pSkinMenuItem=new CSkinMenuSeparator;
				if (pSkinMenuItem==NULL) return NULL;
			}

			break;
		}
	default:
		{
			ASSERT(FALSE);
			return NULL;
		}
	}

	//加入队列
	m_MenuItemActive.Add(pSkinMenuItem);

	return pSkinMenuItem;
}

//////////////////////////////////////////////////////////////////////////////////
