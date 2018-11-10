#include "StdAfx.h"
#include "Resource.h"
#include "PlazaViewItem.h"
#include "GlobalUnits.h"
#include "WndTopNews.h"
#include "MessageDlg.h"
//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define MIN_TYPE_ID					1									//最小类型
#define MAX_TYPE_ID					6									//最大类型

static int nTimer_Fresh=				100;
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndTopNews, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()

END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWndTopNews::CWndTopNews()
{
	ImageItemBack.LoadFromResource(AfxGetInstanceHandle(),IDB_ITEM_BACK);
	//tagEncircleResource	EncircleItemFrame;
	//EncircleItemFrame.pszImageTL=MAKEINTRESOURCE(IDB_ITEM_NEWS_TL);
	//EncircleItemFrame.pszImageTM=MAKEINTRESOURCE(IDB_ITEM_NEWS_TM);
	//EncircleItemFrame.pszImageTR=MAKEINTRESOURCE(IDB_ITEM_NEWS_TR);
	//m_ItemFrameEncircle.InitEncircleResource(EncircleItemFrame,AfxGetInstanceHandle());
	m_nNewsLength = 0;
	m_nFreshLength = 0;
	m_nNewsX = 0;
	m_font.CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	return;
}

//析构函数
CWndTopNews::~CWndTopNews()
{

}

void CWndTopNews::SetTopNews(CString strNews)
{
	m_strNews = strNews;
	
	m_nNewsLength = m_strNews.GetLength();
	m_nFreshLength = m_nNewsLength;
	SetTimer(nTimer_Fresh,100,NULL);
	Invalidate(TRUE);
	return;
}
void CWndTopNews::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == nTimer_Fresh)
	{
		m_nNewsX -= 2;
		int nAllNewsLength = (m_nNewsLength*5);
		if((m_nNewsX + nAllNewsLength)<=0)
		{
			//获取位置
			CRect rcClient;
			GetClientRect(&rcClient);

			m_nNewsX = rcClient.Width();
		}
		Invalidate(TRUE);
	}
	__super::OnTimer(nIDEvent);

}
//绘画背景
BOOL CWndTopNews::OnEraseBkgnd(CDC * pDC)
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

	pBufferDC->SetBkMode(TRANSPARENT);

	ImageItemBack.DrawImageTile(pBufferDC,0,0,990,24);
	//m_ItemFrameEncircle.DrawEncircleFrame(pBufferDC,rcClient);

	CRect rcNews;
	rcNews.left = rcClient.left + m_nNewsX;
	rcNews.top = rcClient.top;
	rcNews.right = rcClient.right+m_nNewsLength*20+ m_nNewsX;
	rcNews.bottom = rcClient.bottom;
	CFont* pOldFont = pBufferDC->SelectObject(&m_font);

	COLORREF oldTextClr = pBufferDC->SetTextColor(RGB(242, 223, 196));
	pBufferDC->DrawText( m_strNews, &rcNews, DT_LEFT|DT_WORDBREAK|DT_SINGLELINE|DT_VCENTER);
	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);


	return TRUE;
}
//消息解释
BOOL CWndTopNews::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////////////
