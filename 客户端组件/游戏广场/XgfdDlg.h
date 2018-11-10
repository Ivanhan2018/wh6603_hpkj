#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// CXgfdDlg 对话框

class CXgfdDlg : public CDialog
{
	DECLARE_DYNAMIC(CXgfdDlg)

public:
	CXgfdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CXgfdDlg();

// 对话框数据
	enum { IDD = IDD_DLG_XGFD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	void InitFanDianCmb();

public:
	int m_user_id;
	CString m_account;
	double m_old_fandian;

	Bitmap* m_bmpBk;
	CFont m_font;

	CImageButton m_btnOK;
	CImageButton m_btnCancel;

	double m_fandian;
	afx_msg void OnBnClickedOk();
	CComboBox m_cmbFandian;
	CString m_strFandian;
};
