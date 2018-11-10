#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// 设置提款账户 对话框

class CZhangHuShZhTkZhhDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuShZhTkZhhDlg)

public:
	CZhangHuShZhTkZhhDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZhangHuShZhTkZhhDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ZHANGHU_SHZTKZHH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnBnClickedBtnOk();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

private:
	void AdjustCtrls();
	void EnableCtrls(BOOL b=TRUE);
	void SetYHCmbSel(const CString& khyh);

private:
	Bitmap* m_bmpBk;
	DWORD m_dwTickCount;
	CComboBox m_cmbKhyh;
	CString m_khyh;	//开户银行
	CString m_khr;	//开户人
	CString m_yhzhh;//银行账号
	CString m_qkmm;	//取款密码

	
	CSkinEditEx	m_editKhr;
	CSkinEditEx	m_editYhzhh;
	CSkinEditEx	m_editQkmm;
	CImageButton m_btnOK;

	CFont m_font;
protected:
	virtual void OnOK();
	virtual void OnCancel();
	//重载函数
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bSetQuKuanZhanghao;						//设置取款保护
	bool							m_bQueryYinHang;						//查询银行
	bool							m_bQueryMyYinHang;						//查询银行
protected:
	VOID SendToServer(int nSendType);

};
