#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "resource.h"

// CMessageDlg 对话框

class CMessageDlg : public CDialog
{
	DECLARE_DYNAMIC(CMessageDlg)

public:
	CMessageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMessageDlg();

// 对话框数据
	enum { IDD = IDD_DLG_TMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedBtnClose();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

public:
	CString m_title;
	CString m_msg;

private:
	Bitmap* m_bmpBk;
	Bitmap* m_bmpIcon;
	CImageButton m_btnOK;
	CImageButton m_btnClose;

	CFont m_fontTitle;
	CFont m_fontMsg;
};

void MyMessageBox(const CString& msg, const CString& title=_T(""));