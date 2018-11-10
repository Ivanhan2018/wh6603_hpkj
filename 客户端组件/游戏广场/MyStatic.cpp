#include "stdafx.h"
#include "MyStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticTrans

CStaticTrans::CStaticTrans()
{
	m_TextColor = RGB(0, 0, 0);
	m_BackColor = TRANS_BACK;    
}

CStaticTrans::~CStaticTrans()
{
}


BEGIN_MESSAGE_MAP(CStaticTrans, CStatic)
	//{{AFX_MSG_MAP(CStaticTrans)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticTrans message handlers

HBRUSH CStaticTrans::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	m_Brush.DeleteObject();

	if (m_BackColor == TRANS_BACK) {
		m_Brush.CreateStockObject(HOLLOW_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
	}
	else {
		m_Brush.CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);
	}

	pDC->SetTextColor(m_TextColor);

	return (HBRUSH)m_Brush;
}

void CStaticTrans::UpdateCtrl()
{
	CWnd* pParent = GetParent();
	CRect rect;

	GetWindowRect(rect);
	pParent->ScreenToClient(rect);
	rect.DeflateRect(2, 2);

	pParent->InvalidateRect(rect, FALSE);    
}