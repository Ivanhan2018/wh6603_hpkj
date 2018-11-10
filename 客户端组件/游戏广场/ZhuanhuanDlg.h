#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "ImgRioButton.h"
#include "TextButton.h"
// 确认投注 对话框

class CZhuanhuanDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhuanhuanDlg)

public:
	CZhuanhuanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZhuanhuanDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ZHUANHUAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedRioToCaiPiao();
	afx_msg void OnBnClickedRioToQiPai();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

public:
	CImgRioButton m_rioToCaiPiao;
	CImgRioButton m_rioToQiPai;

	CTextButton m_btButton[10];
	CString m_gamekind;
	DOUBLE	m_fJine;
	DOUBLE	m_fQpJine;
	CSkinEditEx m_editJine;



	DOUBLE m_fJiaoYiJine;
	int			m_nType;
	//选择改变
	VOID OnLvnItemChangedEdit();

	void SetJine(DOUBLE fJine);
private:
	Bitmap* m_bmpBk;
	CFont m_font;
	CFont m_btnfont;

	CImageButton m_btnOK;
	CImageButton m_btnCancel;
};
