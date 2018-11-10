// CheckBox.cpp : implementation file  
//  

#include "stdafx.h"  
//#include "cat8637_brand.h" 
#include "MyCheckBox.h"  


// CMyCheckBox  

IMPLEMENT_DYNAMIC(CMyCheckBox, CButton)  

CMyCheckBox::CMyCheckBox()  
{  
	m_clrFore = RGB(88, 78, 77);  
	m_clrBK = RGB(61,47,42);  
	m_fontSize = 110;  
}  

CMyCheckBox::~CMyCheckBox()  
{  
}  


BEGIN_MESSAGE_MAP(CMyCheckBox, CButton)  
	ON_WM_PAINT()  
END_MESSAGE_MAP()  



// CMyCheckBox message handlers  
void CMyCheckBox::OnPaint()  
{  
	CPaintDC dc(this); // device context for painting  
	// TODO: Add your message handler code here  
	// Do not call CButton::OnPaint() for painting messages  

	//Draw box  
	RECT rect;  
	GetClientRect(&rect);  
	rect.right = rect.left + 20;  
//  	CMFCVisualManager::GetInstance()->DrawCheckBox(  
//  		&dc  
//  		, rect  
//  		, false                               // highlighted  
//  		, GetCheck() == TRUE ? 1 : 0 // state  
//  		, true                                // enabled  
//  		, false                               // pressed  
//  		);  

	//draw text  
	GetClientRect(&rect);  
	rect.left += 20;  

	CString text;  
	GetWindowText(text);  
	if (text.GetLength() > 0)  
	{  
		COLORREF oldClrFore = dc.SetTextColor(m_clrFore);  
		COLORREF oldClrBK = dc.SetBkColor(m_clrBK);  

		CFont *fontOld = NULL;  
		if (m_fontSize > 0)  
		{  
			CFont font;  
			font.CreatePointFont(m_fontSize, L"ËÎÌו", &dc);  

			fontOld = dc.SelectObject(&font);  
		}  

		CSize size = dc.GetTextExtent(text);  
		rect.top = ((rect.bottom - rect.top) - size.cy) / 2 + rect.top;  
		dc.DrawText(text.GetBuffer(), &rect, DT_LEFT);  

		if (m_fontSize > 0 && fontOld != NULL)  
		{  
			dc.SelectObject(fontOld);  
		}  

		dc.SetBkColor(oldClrBK);  
		dc.SetTextColor(oldClrFore);  
	}  
}  