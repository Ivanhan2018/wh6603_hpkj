// ButtonEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ListCtrlCl.h"
#include "ButtonEx.h"

#include "GamePlaza.h"
// CButtonEx

IMPLEMENT_DYNAMIC(CButtonEx, CButton)

CButtonEx::CButtonEx()
{

}

CButtonEx::CButtonEx( int nItem, int nSubItem, CRect rect, HWND hParent )
{
	m_inItem = nItem;
	m_inSubItem = nSubItem;
	m_rect = rect;
	m_hParent = hParent;
	bEnable = TRUE;

	m_pIamge = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\del.png"));
	m_width = m_pIamge->GetWidth() / 5;
	m_height = m_pIamge->GetHeight();

}

CButtonEx::~CButtonEx()
{
	if(m_pIamge) 
	{
		delete m_pIamge;
		m_pIamge = NULL;
	}

}


BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CButtonEx::OnBnClicked)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CButtonEx メッセージ ハンドラ

void CButtonEx::SetImage(LPCTSTR imgFile)
{
	m_pIamge = new Bitmap(imgFile);
	m_width = m_pIamge->GetWidth() / 4;
	m_height = m_pIamge->GetHeight();
}

void CButtonEx::OnBnClicked()
{
	::SendMessage( m_hParent, WM_BN_CLICK, m_inItem, m_inSubItem );
}
void CButtonEx::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);
	CButton::PreSubclassWindow();
}

void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if(m_pIamge == NULL)
		return;

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	Gdiplus::Graphics graphics(pDC->m_hDC);
	graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), 0, 0, m_width, m_height, UnitPixel);

//	if(m_bIsDisabled)
// 	{
// 		graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*3, 0, m_width, m_height, UnitPixel);
// 		return;
// 	}
// 
// 	BOOL bIsPressed = (lpDIS->itemState & ODS_SELECTED);
// 	if (m_bPushed) 
// 	{
// 		graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*2, 0, m_width, m_height, UnitPixel);
// 	}
// 	else 
// 	{
// 		if(bIsPressed)
// 		{
// 			graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*2, 0, m_width, m_height, UnitPixel);
// 		}
// 		else if(m_bIsHovering)
// 		{
// 			graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width, 0, m_width, m_height, UnitPixel);
// 		}
// 		else
// 		{
// 			if (IsWindowEnabled()) {
// 				graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), 0, 0, m_width, m_height, UnitPixel);
// 			} else {
// 				graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*3, 0, m_width, m_height, UnitPixel);
// 			}
// 
// 		}
// 	}
// 
	// paint the button
	//	PaintBtn(pDC);
	return;
}