#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// 确认投注 对话框

class CFenPeiPeie : public CDialog
{
	DECLARE_DYNAMIC(CFenPeiPeie)

public:
	CFenPeiPeie(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFenPeiPeie();

// 对话框数据
	enum { IDD = IDD_DLG_FPPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual VOID OnOK();

public:
	int m_nUserID;
	DOUBLE m_fFandian;
	int  m_nPeieSy1;
	int  m_nPeieSy2;
	int  m_nPeieSy3;
	CSkinEditEx	m_editPeie1;
	CSkinEditEx	m_editPeie2;
	CSkinEditEx	m_editPeie3;

	int	m_nSetPeie1;
	int	m_nSetPeie2;
	int	m_nSetPeie3;
	CString m_gamekind;
	int m_zhushu;
	int m_moshi;
	DOUBLE m_zongjine;
	CPngImage	m_ImageBack;
private:
	Bitmap* m_bmpBk;
	CFont m_font;

	CImageButton m_btnOK;
	CImageButton m_btnCancel;
};
