#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageButton.h"
#include "MessageDlg.h"
// 盈亏记录 对话框

class CZhangHuQPYkDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuQPYkDlg)

public:
	CZhangHuQPYkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZhangHuQPYkDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ZHANGHU_QPYK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnPrePage();
	afx_msg void OnBnClickedBtnNextPage();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

private:
	void InitListCtrl();
	void AdjustCtrls();

private:
	Bitmap* m_bmpBk;
	DWORD m_dwTickCount;
	CDateTimeCtrl m_dateStart;
	CDateTimeCtrl m_dateEnd;
	CListCtrlCl m_listLog;
	CImageButton m_btnOK;
	CImageButton m_btnPrePage;
	CImageButton m_btnNextPage;

	CFont m_font;
	CTime m_tStart;
	CTime m_tEnd;
	DOUBLE m_fZongYingkui;
	int m_page;
	int m_itemCount;
	int m_maxPage;
	BOOL m_byTime;
protected:
	virtual void OnCancel();
	virtual void OnOK();
	//重载函数
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetYingkuiMxCount;						//获取日志数量
	bool							m_bGetYingkuiMx;							//获取日志
protected:
	VOID SendToServer(int nSendType);
};

