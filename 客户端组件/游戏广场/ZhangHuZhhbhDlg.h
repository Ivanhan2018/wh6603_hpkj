#pragma once
#include "afxwin.h"
#include "ImageButton.h"

#include "MessageDlg.h"
// CZhangHuZhhbhDlg 对话框

class CZhangHuZhhbhDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuZhhbhDlg)

public:
	CZhangHuZhhbhDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZhangHuZhhbhDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ZHANGHU_ZHHBH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

private:
	void AdjustCtrls();

private:
	Bitmap* m_bmpBk;
	CImageButton m_btnOK;
	CFont m_font;
	DWORD m_dwTickCount;
	CString m_pwd_qk;
	CString m_mb_ask;
	CString m_mb_answer;

	CSkinEditEx  m_editPwdQk;
	CSkinEditEx	m_editMbAsk;
	CSkinEditEx	m_editMbAnswer;
protected:
	virtual void OnCancel();
	virtual void OnOK();
	//重载函数
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bSetQuKuanProtect;						//设置取款保护
	bool							m_bQueryMyProtect;						//设置取款保护
	
protected:
	VOID SendToServer(int nSendType);

};
