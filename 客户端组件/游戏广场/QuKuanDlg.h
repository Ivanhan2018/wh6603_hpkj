#pragma once
#include "afxwin.h"
#include "ImageButton.h"

//class CLuckMeDlg;

//取款中心
class CQuKuanDlg : public CDialog
{
	DECLARE_DYNAMIC(CQuKuanDlg)

public:
	CQuKuanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQuKuanDlg();

// 对话框数据
	enum { IDD = IDD_DLG_QUKUAN };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnClose();

	afx_msg void OnBnClickedBtnQukuan();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	Bitmap* m_bmpBk;
	Bitmap* m_bmpQuKuanBk;

	CSkinEditEx m_editQkAct;
	CSkinEditEx m_editQkPwd;
	CImageButton m_btnQuKuan;
	CImageButton m_btnClose;

	CString m_yinhang;
	CString m_kaihuren;
	CString m_zhanghu;

	CFont m_font;
	double m_tikuane;
	CString m_tikuan_pwd;

	BYTE m_cbIsVip;
	//重载函数
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetTiKuan;						//获取提款信息
	bool							m_bDoTiKuan;
	bool							m_bGetLastYue;
protected:
	VOID SendToServer(BYTE nSendType);
};
