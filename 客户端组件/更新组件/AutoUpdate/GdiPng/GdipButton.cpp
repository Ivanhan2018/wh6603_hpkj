#include "stdafx.h"
#include "GdipButton.h"

#include "CGdiPlusBitmap.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGdipButton::CGdipButton()
{
	m_pStdImage = NULL;

	m_bHaveBitmaps = FALSE;

	m_pCurBtn = NULL;

	m_bIsDisabled = FALSE;
	m_bIsToggle = TRUE;
	m_bPushed = FALSE;

	m_bIsHovering = FALSE;
	m_bIsTracking = FALSE;

	m_width = 0;
	m_height = 0;

	m_bTextBtn = false;
}

CGdipButton::~CGdipButton()
{
	if(m_pStdImage) delete m_pStdImage;
}

BEGIN_MESSAGE_MAP(CGdipButton, CButton)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ENABLE()
END_MESSAGE_MAP()



//=============================================================================
//
// LoadStdImage()
//
// Purpose:     The LoadStdImage() Loads the image for the button.  This 
//				function must be called at a minimum or the button wont do 
//				anything.
//
// Parameters:  
//		[IN]	id
//				resource id, one of the resources already imported with the 
//				resource editor, usually begins with IDR_  
//
//		[IN]	pType
//				pointer to string describing the resource type
//				
// Returns:     BOOL
//				Non zero if successful, otherwise zero
//
//=============================================================================
BOOL CGdipButton::LoadStdImage(UINT id, LPCTSTR pType, bool Text)
{
	m_bTextBtn = Text;
	m_pStdImage = new CGdiPlusBitmapResource;
	return m_pStdImage->Load(id, pType);
}

BOOL CGdipButton::LoadStdImage(LPCTSTR imgFile, bool Text)
{
	m_bTextBtn = Text;
	m_pStdImage = new CGdiPlusBitmapResource;
	return m_pStdImage->LoadFile(imgFile);
}

void CGdipButton::SetPushed(bool bPushed) 
{
	if(m_bPushed != bPushed) 
	{
		m_bPushed = bPushed;
		Invalidate();
	}
}

//=============================================================================
//
//	The framework calls this member function when a child control is about to 
//	be drawn.  All the bitmaps are created here on the first call. Every thing
//	is done with a memory DC except the background, which get's it's information 
//	from the parent. The background is needed for transparent portions of PNG 
//	images. An always on top app (such as Task Manager) that is in the way can 
//	cause it to get an incorrect background.  To avoid this, the parent should 
//	call the SetBkGnd function with a memory DC when it creates the background.
//				
//=============================================================================
HBRUSH CGdipButton::CtlColor(CDC* pScreenDC, UINT nCtlColor) 
{
	if(!m_bHaveBitmaps)
	{
		if(!m_pStdImage)
		{
			return NULL; // Load the standard image with LoadStdImage()
		}

		CBitmap bmp, *pOldBitmap;

		CRect rect;
		GetClientRect(rect);

		// do everything with mem dc
		CMemDC pDC(pScreenDC, rect);

		Gdiplus::Graphics graphics(pDC->m_hDC);

		// background
		if (m_dcBk.m_hDC == NULL)
		{

			CRect rect1;
			CClientDC clDC(GetParent());
			GetWindowRect(rect1);
			GetParent()->ScreenToClient(rect1);

			m_dcBk.CreateCompatibleDC(&clDC);
			bmp.CreateCompatibleBitmap(&clDC, rect.Width(), rect.Height());
			pOldBitmap = m_dcBk.SelectObject(&bmp);
			m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), &clDC, rect1.left, rect1.top, SRCCOPY);
			bmp.DeleteObject();
		}

		// standard image
		if (m_dcStd.m_hDC == NULL)
		{
			PaintBk(pDC);

			graphics.DrawImage(*m_pStdImage, Rect(0, 0, m_width, m_height), 0, 0, m_width, m_height, UnitPixel);
		
			if (m_bTextBtn) {
				CString str;
				GetWindowText(str);
				int oldBkMode = pDC.SetBkMode(TRANSPARENT);
				COLORREF oldTextClr = pDC.SetTextColor(RGB(255, 255, 255));
				pDC.DrawText(str, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				pDC.SetTextColor(oldTextClr);
				pDC.SetBkMode(oldBkMode);
			}
			
			m_dcStd.CreateCompatibleDC(pDC);
			bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
			pOldBitmap = m_dcStd.SelectObject(&bmp);
			m_dcStd.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
			bmp.DeleteObject();

			// standard image pressed
			if (m_dcStdH.m_hDC == NULL)
			{
				PaintBk(pDC);

				graphics.DrawImage(*m_pStdImage, Rect(0, 0, m_width, m_height), m_width, 0, m_width, m_height, UnitPixel);

				if (m_bTextBtn) {
					CString str;
					GetWindowText(str);
					int oldBkMode = pDC.SetBkMode(TRANSPARENT);
					COLORREF oldTextClr = pDC.SetTextColor(RGB(255, 255, 255));
					pDC.DrawText(str, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					pDC.SetTextColor(oldTextClr);
					pDC.SetBkMode(oldBkMode);
				}

				m_dcStdH.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcStdH.SelectObject(&bmp);
				m_dcStdH.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}

			// standard image pressed
			if (m_dcStdP.m_hDC == NULL)
			{
				PaintBk(pDC);

				graphics.DrawImage(*m_pStdImage, Rect(0, 0, m_width, m_height), m_width*2, 0, m_width, m_height, UnitPixel);

				if (m_bTextBtn) {
					CString str;
					GetWindowText(str);
					int oldBkMode = pDC.SetBkMode(TRANSPARENT);
					COLORREF oldTextClr = pDC.SetTextColor(RGB(255, 255, 255));
					pDC.DrawText(str, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					pDC.SetTextColor(oldTextClr);
					pDC.SetBkMode(oldBkMode);
				}

				m_dcStdP.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcStdP.SelectObject(&bmp);
				m_dcStdP.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}

			// grayscale image
			if(m_dcGS.m_hDC == NULL)
			{
				PaintBk(pDC);

				graphics.DrawImage(*m_pStdImage, Rect(0, 0, m_width, m_height), m_width*3, 0, m_width, m_height, UnitPixel);

				if (m_bTextBtn) {
					CString str;
					GetWindowText(str);
					int oldBkMode = pDC.SetBkMode(TRANSPARENT);
					COLORREF oldTextClr = pDC.SetTextColor(RGB(255, 255, 255));
					pDC.DrawText(str, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					pDC.SetTextColor(oldTextClr);
					pDC.SetBkMode(oldBkMode);
				}

				m_dcGS.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcGS.SelectObject(&bmp);
				m_dcGS.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}
		}

		if(m_pCurBtn == NULL)
		{
			m_pCurBtn = &m_dcStd;
		}

		m_bHaveBitmaps = TRUE;
	}

	return NULL;
}

//=============================================================================
// paint the background
//=============================================================================
void CGdipButton::PaintBk(CDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
}

//=============================================================================
// paint the bitmap currently pointed to with m_pCurBtn
//=============================================================================
void CGdipButton::PaintBtn(CDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), m_pCurBtn, 0, 0, SRCCOPY);
}

//=============================================================================
// set the control to owner draw
//=============================================================================
void CGdipButton::PreSubclassWindow()
{
	// Set control to owner draw
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}

//=============================================================================
// disable double click 
//=============================================================================
BOOL CGdipButton::PreTranslateMessage(MSG* pMsg) 
{
	return CButton::PreTranslateMessage(pMsg);
}


//=============================================================================
// overide the erase function
//=============================================================================
BOOL CGdipButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

//=============================================================================
// Paint the button depending on the state of the mouse
//=============================================================================
void CGdipButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// handle disabled state
	if(m_bIsDisabled)
	{
		m_pCurBtn = &m_dcGS;
		PaintBtn(pDC);
		return;
	}

	BOOL bIsPressed = (lpDIS->itemState & ODS_SELECTED);

	if(bIsPressed)
	{
		//if (m_bIsToggle && m_bPushed)
		//{
		//	m_pCurBtn = &m_dcStd;
		//} 
		//else 
		//{
			m_pCurBtn = &m_dcStdP;
		//}
	}
	else if(m_bIsHovering)
	{
		if (!m_bPushed)
		{
			m_pCurBtn = &m_dcStdH;
		}
	}
	else
	{
		if (m_bIsToggle && m_bPushed)
		{
			m_pCurBtn = &m_dcStdP;
		} 
		else 
		{
			m_pCurBtn = &m_dcStd;
		}
	}

	// paint the button
	PaintBtn(pDC);
}

//=============================================================================
LRESULT CGdipButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
//=============================================================================
{
	m_bIsHovering = TRUE;
	Invalidate();

	return 0;
}


//=============================================================================
LRESULT CGdipButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
//=============================================================================
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	Invalidate();
	return 0;
}

//=============================================================================
void CGdipButton::OnMouseMove(UINT nFlags, CPoint point) 
//=============================================================================
{
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = _TrackMouseEvent(&tme);
	}
	
	CButton::OnMouseMove(nFlags, point);
}

void CGdipButton::OnEnable(BOOL bEnable)
{
	CButton::OnEnable(bEnable);

	m_bIsDisabled = !bEnable;
	Invalidate();
}
