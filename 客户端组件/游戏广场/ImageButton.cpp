#include "stdafx.h"
#include "GamePlaza.h"
#include "ImageButton.h"
#include "gdipng/MemDC.h"

IMPLEMENT_DYNAMIC(CImageButton, CButton)

CImageButton::CImageButton()
: m_pIamge(NULL)
, m_bHaveBitmaps(false)
// , m_pCurBtn(NULL)
, m_width(0)
, m_height(0)
, m_bIsHovering(false)
, m_bIsTracking(false)
, m_bIsDisabled(false)
, m_bPushed(false)
{

}

CImageButton::~CImageButton()
{
	if(m_pIamge) 
	{
		delete m_pIamge;
		m_pIamge = NULL;
	}

}

void CImageButton::SetImage(LPCTSTR imgFile)
{
	m_pIamge = new Bitmap(imgFile);
	m_width = m_pIamge->GetWidth() / 4;
	m_height = m_pIamge->GetHeight();
}

void CImageButton::SetPushed(bool bPushed) 
{
	if (m_bPushed != bPushed) 
	{	
		m_bPushed = bPushed;
		Invalidate();
	}
}

BEGIN_MESSAGE_MAP(CImageButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
//	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE( WM_MOUSEHOVER, OnMouseHover )
	ON_MESSAGE( WM_MOUSELEAVE, OnMouseLeave ) 
	ON_WM_LBUTTONDOWN()

END_MESSAGE_MAP()

BOOL CImageButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CImageButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);
	CButton::PreSubclassWindow();
}

void CImageButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if(m_pIamge == NULL)
		return;

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	Gdiplus::Graphics graphics(pDC->m_hDC);
	graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), 0, 0, m_width, m_height, UnitPixel);

	if(m_bIsDisabled)
	{
		graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*3, 0, m_width, m_height, UnitPixel);
		return;
	}

	BOOL bIsPressed = (lpDIS->itemState & ODS_SELECTED);
	if (m_bPushed) 
	{
		graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*2, 0, m_width, m_height, UnitPixel);
	}
	else 
	{
		if(bIsPressed)
		{
			graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*2, 0, m_width, m_height, UnitPixel);
		}
		else if(m_bIsHovering)
		{
			graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width, 0, m_width, m_height, UnitPixel);
		}
		else
		{
			if (IsWindowEnabled()) {
				graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), 0, 0, m_width, m_height, UnitPixel);
			} else {
				graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*3, 0, m_width, m_height, UnitPixel);
			}
			
		}
	}

	// paint the button
//	PaintBtn(pDC);
	return;
}
// 
// void CImageButton::PaintBk(CDC *pDC)
// {
// 	CRect rect;
// 	GetClientRect(rect);
// 	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
// }
// 
// void CImageButton::PaintBtn(CDC *pDC)
// {
// 	CRect rect;
// 	GetClientRect(rect);
// 	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), m_pCurBtn, 0, 0, SRCCOPY);
// }
// 

LRESULT CImageButton::OnMouseHover(WPARAM wParam,LPARAM lParam)
{


	m_bIsHovering = true;
	Invalidate();
	return 1;
}
//鼠标消息
VOID CImageButton::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);
	TCHAR wav_path[MAX_PATH] = _T("");
	CBmpUtil::GetExePath(_T("skin\\game\\MouseDown.wav"), wav_path);
	::PlaySound(wav_path, NULL, SND_ASYNC);
	return;
}

LRESULT CImageButton::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	Invalidate();
	return 1;
}

void CImageButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = !!_TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}
