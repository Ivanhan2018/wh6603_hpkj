#ifndef STATICTRANS_H
#define STATICTRANS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CStaticTrans window

#define TRANS_BACK -1

class CStaticTrans : public CStatic
{
	// Construction
public:
	CStaticTrans();

private:
	COLORREF m_TextColor;
	COLORREF m_BackColor;
	CBrush   m_Brush;

	// Operations
public:
	void SetTextColor(COLORREF col) { m_TextColor = col;
	UpdateCtrl();      }
	void SetBackColor(COLORREF col) { m_BackColor = col;
	UpdateCtrl();      }

private:
	void UpdateCtrl();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticTrans)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CStaticTrans();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticTrans)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#endif // STATICTRANS_H
