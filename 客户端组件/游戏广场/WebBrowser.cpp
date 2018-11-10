// WebBrowser.cpp : 实现文件
//

#include "stdafx.h"
//#include "LuckMe.h"
#include "WebBrowser.h"


// CWebBrowserLogo

IMPLEMENT_DYNCREATE(CWebBrowserLogo, CHtmlView)

CWebBrowserLogo::CWebBrowserLogo()
{

}

CWebBrowserLogo::~CWebBrowserLogo()
{
}


BOOL	CWebBrowserLogo::PreTranslateMessage(MSG*	pMsg)
{
	if ((pMsg->message == WM_RBUTTONDOWN)||(pMsg->message == WM_RBUTTONDBLCLK))
	{
		CPoint point(pMsg->pt);
		ScreenToClient(&point);

		IHTMLDocument2* pdoc2=NULL;
		IHTMLElement* pelement=NULL;
		IDispatch* pdisp=NULL;
		pdisp=GetHtmlDocument();
		pdisp->QueryInterface(IID_IHTMLDocument2,(void**)&pdoc2);
		pdisp->Release();

		pdoc2->elementFromPoint(point.x,point.y,&pelement);
		pdoc2->Release();
		if(pelement)
		{
			BSTR id;
			pelement->get_id(&id);
			pelement->Release();
			CString str=(LPCTSTR)(BSTR)id;
			if(str==_T("layer1")){
				//cmenu menu;
				//menu.loadmenu(idr_menu1 );
				//cmenu* Pmenu=menu.getsubmenu(0);
				//pMenu->trackpopupmenu(0,pmsg->pt.x,pmsg->pt.y,this);
			}
		}
		return true;//如果想完全屏蔽掉,不显示任何菜单,直接返回true就行,
		//上面这些代码演示了怎么对html中特定id的元素弹出自己想要显示的菜单 
	}	
	else if((pMsg->message == VK_F5))
		return true;
	else
		return CHtmlView::PreTranslateMessage(pMsg);
}

void CWebBrowserLogo::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

}

void CWebBrowserLogo::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWebBrowserLogo, CHtmlView)
END_MESSAGE_MAP()


// CWebBrowserLogo 诊断

#ifdef _DEBUG
void CWebBrowserLogo::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CWebBrowserLogo::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CMloadWeb 消息处理程序
void CWebBrowserLogo::Navigate(LPCTSTR pszURL)
{
	ASSERT(pszURL!=NULL);
	m_strNavigateUrl=pszURL;
	if (m_bEnableBrowser) __super::Navigate(m_strNavigateUrl,NULL,NULL,NULL,NULL);
	return;
}

//启用网页
bool CWebBrowserLogo::EnableBrowser(bool bEnableBrowser)
{
	if (m_bEnableBrowser!=bEnableBrowser)
	{
		m_bEnableBrowser=bEnableBrowser;
		if (m_bEnableBrowser==true)
		{
			if ((GetSafeHwnd()!=NULL)&&(m_strNavigateUrl.IsEmpty()==FALSE))
			{
				__super::Navigate(m_strNavigateUrl,NULL,NULL,NULL,NULL);
			}
		}
		else m_strNavigateUrl.Empty();
	}
	return m_bEnableBrowser;
}
