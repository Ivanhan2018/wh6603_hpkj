#include "stdafx.h"
#include "GamePlaza.h"
#include "ListCtrlCl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
};
// CListCtrlCl
#define IDC_BUTTON_ID 0x1235
IMPLEMENT_DYNAMIC(CListCtrlCl, CListCtrl)

CListCtrlCl::CListCtrlCl()
: m_nRowHeight(0)
, m_fontHeight(22)
, m_fontWith(0)
{
	m_color = RGB(5,0,0);
	NewPen.CreatePen(PS_SOLID, 1, RGB(127,125,113));
	m_bSetPenColor = false;
	m_uID = IDC_BUTTON_ID;
}

CListCtrlCl::~CListCtrlCl()
{
	NewPen.DeleteObject();
	for (POSITION pos = m_colTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_colTextColor.GetNext(pos);
		if (pColor != NULL) {
			delete pColor;
			pColor = NULL;
		}
	}

	for (POSITION pos = m_ItemTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ItemTextColor.GetNext(pos);
		if (pColor != NULL) {
			delete pColor;
			pColor = NULL;
		}
	}

	for (POSITION pos = m_ptrListItem.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListItem.GetNext(pos);
		if (pColor != NULL) {
			delete pColor;
			pColor = NULL;
		}
	}

	for (POSITION pos = m_ptrListCol.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListCol.GetNext(pos);
		if (pColor != NULL) {
			delete pColor;
			pColor = NULL;
		}
	}

	button_map::iterator iter;
	iter = m_mButton.begin();
	while ( iter != m_mButton.end() )
	{
		delete iter->second;
		iter->second = NULL;
		iter++;
	}

}
void CListCtrlCl::updateListCtrlButtonPos()
{
	int iTopIndex = GetTopIndex();
	int nItem = iTopIndex;
	button_map::iterator iter;
	button_map::iterator iterUp;
	int iLine = 0;
	iter = m_mButton.find( iTopIndex );
	iterUp = m_mButton.begin();
	while ( iter != m_mButton.end() )
	{
		iterUp->second->EnableWindow( iter->second->bEnable );
		iter ++;
		iterUp++;
	}

}

void CListCtrlCl::deleteItemEx( int nItem )
{
	int iCount = GetItemCount();
	button_map::iterator iter;
	button_map::iterator iterNext;

	iter = m_mButton.find( iCount - 1 );
	if ( iter != m_mButton.end() )
	{
		delete iter->second;
		iter->second = NULL;
		m_mButton.erase( iter );
		updateListCtrlButtonPos();
	}
	DeleteItem( nItem );
}
void CListCtrlCl::deleteAllItemEx()
{
	int iCount = GetItemCount();
	button_map::iterator iter;
	button_map::iterator iterNext;

	DeleteAllItems();
	//iter = m_mButton.find(iCount-1);

	for (map<int,CButtonEx*>::iterator pos = m_mButton.begin(); pos != m_mButton.end();/*++pos*/)
	{
		delete pos->second;
		pos->second = NULL;
		m_mButton.erase( pos ++);

	}
	updateListCtrlButtonPos();
}
void CListCtrlCl::createItemButton( int nItem, int nSubItem, HWND hMain )
{
	CRect rect;
	int offset = 4;

	// Make sure that the item is visible
	if( !EnsureVisible(nItem, TRUE)) 
		return ;

	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}

	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nSubItem);
	if(rect.right > rcClient.right) 
		rect.right = rcClient.right;
	//basic code end

	rect.bottom = rect.top + rect.Height();

	int iPageCout = GetCountPerPage();
	if ( nItem >= iPageCout )
	{
		rect.top += rect.Height();
		rect.bottom += rect.Height();
	}

	DWORD dwStyle =  WS_CHILD | WS_VISIBLE;
	CButtonEx *pButton = new CButtonEx(nItem,nSubItem,rect,hMain);
	m_uID++;
	pButton->Create(_T("×"),dwStyle, rect, this, m_uID);
//	pButton->SetBitmap(IDB_BT_DEL);
	m_mButton.insert( make_pair( nItem, pButton ) );

	int iTopIndex = GetTopIndex();
	if ( iTopIndex > 0 )
	{
		updateListCtrlButtonPos();
	}

	return;
}

void CListCtrlCl::Default()
{
	SetColColor(0,RGB(44,8,60)); //设置列背景色
	SetColColor(2,RGB(44,8,60)); //设置列背景色
	SetBkColor(RGB(44,8,60));        //设置背景色
	SetItemColor(1,1,RGB(44,8,60)); //设置指定单元背景色
	SetRowHeigt(30);               //设置行高度
	SetHeaderHeight(1);          //设置头部高度
	SetHeaderFontHW(16,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	SetHeaderTextColor(RGB(40,40,40)); //设置头部字体颜色
	SetTextColor(RGB(95,82,81));  //设置文本颜色
	SetHeaderBKColor(44,8,60,0); //设置头部背景色
	SetFontHW(20,0);               //设置字体高度，和宽度,0表示缺省宽度
	SetColTextColor(2,RGB(255,0,100)); //设置列文本颜色
	SetItemTextColor(3,1,RGB(255,0,0));  //设置单元格字体颜色
}

BEGIN_MESSAGE_MAP(CListCtrlCl, CListCtrl)
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_PAINT()
END_MESSAGE_MAP()




void CListCtrlCl::SetPenColor(COLORREF color)
{
	SetPen.CreatePen(PS_SOLID, 1, color);
	m_bSetPenColor = TRUE;
	return;
}
void CListCtrlCl::OnPaint() 
{
	// First let the control do its default drawing.
	const MSG *msg = GetCurrentMessage();
	DefWindowProc( msg->message, msg->wParam, msg->lParam );


	// Draw the lines only for LVS_REPORT mode
	if( (GetStyle() & LVS_TYPEMASK) == LVS_REPORT )
	{
		// Get the number of columns
		CClientDC dc(this );
		CPen *oldPen = dc.SelectObject(&NewPen);
		if(m_bSetPenColor)
		{
			dc.SelectObject(&SetPen);
		}
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int nColumnCount = pHeader->GetItemCount();

		// The bottom of the header corresponds to the top of the line 
		RECT rect;
		pHeader->GetClientRect( &rect );
		int top = rect.bottom;

		// Now get the client rect so we know the line length and
		// when to stop
		GetClientRect( &rect );

		// The border of the column is offset by the horz scroll
		int borderx = 0 - GetScrollPos( SB_HORZ );
		 int i = 0;
		for( i = 0; i < nColumnCount; i++ )
		{
			// Get the next border
			borderx += GetColumnWidth( i );

			// if next border is outside client area, break out
			if( borderx >= rect.right ) break;

			// Draw the line.
			dc.MoveTo( borderx, top);
			dc.LineTo( borderx, rect.bottom );
		}

		// Draw the horizontal grid lines

		// First get the height
// 		if( !GetItemRect( 0, &rect, LVIR_BOUNDS ))
// 			return;

		int height = 24;//rect.bottom - rect.top;

		GetClientRect( &rect );
		int width = 990;//rect.right;

		for( i = 1; i <= 19; i++ )
		{
// 			if(i%2 == 1)
// 			{
// 				CRect rcItem;
// 				rcItem.left = 0;
// 				rcItem.right = width;
// 				rcItem.top = top + height*i-1;
// 				rcItem.bottom = rcItem.top+height;
// 				COLORREF color;
// 				color = RGB(201,198,197);
// 
// 				dc.FillSolidRect(rcItem,color);
// 
// 
// 			}
			dc.MoveTo( 0, top + height*i-1);
			dc.LineTo( width, top + height*i-1 );
		}
		
		dc.SelectObject(oldPen);
	}

	// Do not call CListCtrl::OnPaint() for painting messages
}
void CListCtrlCl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0,LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());

	SetBkColor(RGB(216,214,213));
	SetTextColor(RGB(95,82,81));  //设置文本颜色
	SetRowHeigt(24);               //设置行高度
	SetFontHW(20,0);

}

void CListCtrlCl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	TCHAR lpBuffer[9000];

	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
	lvi.iItem = lpDrawItemStruct->itemID ; 
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer ;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));

	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CPen *oldPen = pDC->SelectObject(&NewPen);
	CRect rtClient;
	GetClientRect(&rtClient);
	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		if ( nCol > 0 ) 
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol-1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left; 
		}

		CRect rcItem;   
		if (!GetSubItemRect(lpDrawItemStruct->itemID,nCol,LVIR_LABEL,rcItem))   
			continue;   

		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
		CRect rcTemp;
		rcTemp = rcItem;

		if (nCol==0)
		{
			rcTemp.left -=2;
		}

		if ( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			pDC->FillSolidRect(&rcTemp, RGB(222,163,124)) ;
			pDC->SetTextColor(RGB(0,0,0)) ;
		}
		else
		{
			COLORREF color;
			color = GetBkColor();

 			if(lpDrawItemStruct->itemID%2 == 1)
			{
 				color = RGB(201,198,197);
				rcTemp.left -=2;
			}
			pDC->FillSolidRect(rcTemp,color);

			if (FindColColor(nCol,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			if (FindItemColor(nCol,lpDrawItemStruct->itemID,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			
			//pDC->SetTextColor(m_color);
		}

		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

		UINT   uFormat    = DT_CENTER ;
		if (m_Header.m_Format[nCol]=='0')
		{
			uFormat = DT_LEFT;
		}
		else if (m_Header.m_Format[nCol]=='1')
		{
			uFormat = DT_CENTER;
		}
		else if (m_Header.m_Format[nCol]=='2')
		{
			uFormat = DT_RIGHT;
		}
		TEXTMETRIC metric;
		pDC->GetTextMetrics(&metric);
		int ofst;
		ofst = rcItem.Height() - metric.tmHeight;
		rcItem.OffsetRect(0,ofst/2);
		if ( lpDrawItemStruct->itemState & ODS_SELECTED )
			pDC->SetTextColor(RGB(0,0,0));
		else
			pDC->SetTextColor(m_color);
		COLORREF color;
		if (FindColTextColor(nCol,color))
		{
			pDC->SetTextColor(color);
		}
		if (FindItemTextColor(nCol,lpDrawItemStruct->itemID,color))
		{
			pDC->SetTextColor(color);
		}
		CFont nFont ,* nOldFont; 
		nFont.CreateFont(m_fontHeight,m_fontWith,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("微软雅黑"));//创建字体 
		nOldFont = pDC->SelectObject(&nFont);

		CRect rcText ;
		rcText.CopyRect(rcItem);
		rcText.top -=3;
		rcText.bottom -= 3;
		DrawText(lpDrawItemStruct->hDC, lpBuffer, _tcslen(lpBuffer), &rcText, uFormat) ;

		pDC->SelectStockObject(SYSTEM_FONT) ;
		pDC->SelectObject(oldPen);
	}



}

void CListCtrlCl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
void CListCtrlCl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight>0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}
int CListCtrlCl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */)
{
	m_Header.m_HChar.Add(lpszColumnHeading);
	if (nFormat==LVCFMT_LEFT)
	{
		m_Header.m_Format = m_Header.m_Format + _T("0");
	}
	else if (nFormat==LVCFMT_CENTER)
	{
		m_Header.m_Format = m_Header.m_Format + _T("1");
	}
	else if (nFormat==LVCFMT_RIGHT)
	{
		m_Header.m_Format = m_Header.m_Format + _T("2");
	}
	else
	{
		m_Header.m_Format = m_Header.m_Format + _T("1");
	}
	return CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
}
// Gradient - 渐变系数，立体背景用,不用渐变设为0
void CListCtrlCl::SetHeaderBKColor(int R, int G, int B, int Gradient) //设置表头背景色
{
	m_Header.m_R = R;
	m_Header.m_G = G;
	m_Header.m_B = B;
	m_Header.m_Gradient = Gradient;
}

// 设置表头高度
void CListCtrlCl::SetHeaderHeight(float Height) //设置表头高度
{
	m_Header.m_Height = Height;
}
bool CListCtrlCl::FindColColor(int col,COLORREF &color) //查找列颜色
{
	int flag = 0;
	for (POSITION pos = m_ptrListCol.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListCol.GetNext(pos);
		if (pColor->nCol==col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
bool CListCtrlCl::FindItemColor(int col,int row,COLORREF &color) //查找颜色
{
	int flag = 0;
	for (POSITION pos = m_ptrListItem.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListItem.GetNext(pos);
		if (pColor->nCol==col&&pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
void CListCtrlCl::SetColColor(int col,COLORREF color) //设置列颜色
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_ptrListCol.AddTail(pColor);
}
void CListCtrlCl::SetItemColor(int col,int row,COLORREF color) //设置格子颜色
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ptrListItem.AddTail(pColor);
}
void CListCtrlCl::SetRowHeigt(int nHeight) //高置行高
{
	m_nRowHeight = nHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}
void CListCtrlCl::SetHeaderFontHW(int nHeight,int nWith) //设置头部字体宽和高
{
	m_Header.m_fontHeight = nHeight;
	m_Header.m_fontWith = nWith;
}
void CListCtrlCl::SetHeaderTextColor(COLORREF color) //设置头部字体颜色
{
	m_Header.m_color = color;
}
BOOL CListCtrlCl::SetTextColor(COLORREF cr)  //设置字体颜色
{
	m_color = cr;
	return TRUE;
}
void CListCtrlCl::SetFontHW(int nHeight,int nWith) //设置字体高和宽
{
	m_fontHeight = nHeight;
	m_fontWith = nWith;
}
void CListCtrlCl::SetColTextColor(int col,COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_colTextColor.AddTail(pColor);
}
bool CListCtrlCl::FindColTextColor(int col,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_colTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_colTextColor.GetNext(pos);
		if (pColor->nCol==col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
bool CListCtrlCl::FindItemTextColor(int col,int row,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_ItemTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ItemTextColor.GetNext(pos);
		if (pColor->nCol==col&&pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
void CListCtrlCl::SetItemTextColor(int col,int row,COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ItemTextColor.AddTail(pColor);
}