#pragma once
#include "ImageButton.h"

// CTiShiDlg 对话框
#define  IDM_FRESH_YUE 114
#define  IDM_TOUZHU_TISHI 115
class CTiShiDlg : public CDialog
{
	DECLARE_DYNAMIC(CTiShiDlg)

public:
	CTiShiDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTiShiDlg();

// 对话框数据
	enum { IDD = IDD_DLG_TISHI };

public:
	void ShowTiShi(const CString& title, const CString& msg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void	OnLButtonUp(UINT	nFlags, CPoint	point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	afx_msg void OnBnClickedBtnClose();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

public:
	CString m_title;
	CString m_msg;
	int m_nElapse;
	void StartSetTimer();
	void StartKillTimer();
private:
	void SetPos();

private:
	Bitmap* m_bmpBk;
	Bitmap* m_bmpIcon;
	CImageButton m_btnClose;

	CFont m_fontTitle;
	CFont m_fontMsg;
	int width;
	int height;

	CStringArray m_tishi;
	CCriticalSection m_csTishi;
public:
	VOID SetTiShiMessage(CString strMsg);
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
private:
	bool								m_bGetChongzhiTishi;
	bool								m_bTixianTishi;
	bool								m_bTouzhuTishi;

	int									m_nTypeID;
	int									m_nKindID;
	BYTE								m_cbMoshi;
	int									m_nZhushu;
	int									m_nBeiShu;
	TCHAR								m_szQiHao[30];
	DOUBLE								m_fYingKui;

};

