#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// CXgfdDlg 对话框

class COpenRecord : public CDialog
{
	DECLARE_DYNAMIC(COpenRecord)

public:
	COpenRecord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COpenRecord();
	KjXinXi m_kjXinxi[20];
	int m_smallNumWidth;
	int m_smallNumHeight;
	Bitmap* m_kjNumSmall;
	Bitmap* m_kjNumSmall11xuan5;
	Bitmap* m_btClose;
	CFont m_smallNumfont;
	CFont m_Numfont;
	int	m_kjXinxiCont;
	int m_nGameType;
	int m_nCloseIndex;
	bool m_b11Xuan5;
// 对话框数据
	enum { IDD = IDD_DLG_OPEN_RECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
	void DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawTopKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawTopKjHaoma11Xuan5(CDC* pDC, Graphics& graphics);
	void DrawCloseBtn(CDC* pDC, Graphics& graphics);
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	VOID OnMouseMove(UINT nFlags, CPoint Point);

public:
	void SetOpenRecord(CMD_GP_GetMoreRecordRet* pMoreRecord,int nIndex,bool b11xuan5 = false);
// 	int m_user_id;
// 	CString m_account;
// 	double m_old_fandian;
// 
 	Bitmap* m_bmpBk;
// 	CFont m_font;

	CImageButton m_btnOK;
	CImageButton m_btnCancel;

//	double m_fandian;
	afx_msg void OnBnClickedOk();
// 	CComboBox m_cmbFandian;
// 	CString m_strFandian;
};
