#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "ImgRioButton.h"

// CHuiYuanTjxjDlg 对话框

class CHuiYuanTjxjDlg : public CDialog//,public CMissionItem
{
	DECLARE_DYNAMIC(CHuiYuanTjxjDlg)

public:
	CHuiYuanTjxjDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHuiYuanTjxjDlg();

// 对话框数据
	enum { IDD = IDD_DLG_HUIYUAN_TJXJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnBnClickedRioDaili();
	afx_msg void OnBnClickedRioHuiyuan();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedSetWebFandian();
	afx_msg void OnBnClickedSetMorenMima();
	afx_msg void OnBnClickedBtnCpRegurl();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	void AdjustCtrls();
	void EnableCtrls(BOOL b=TRUE);
	void InitFanDianCmb();

private:
	Bitmap* m_bmpBk;
	CString m_strPeie;
	int m_type;	//下级身份
	double m_fandian;
	CString m_zhanghu;
	CString m_password;
	CString m_crmpwd;
	CString m_qkpwd;
	CString m_crmqkpwd;

	CSkinEditEx m_editZhanghu;
	CSkinEditEx	m_editPassword;
	CSkinEditEx m_editCrmPwd;
	CSkinEditEx	m_editQkPwd;
	CSkinEditEx m_editCrmQkPwd;

	CImageButton m_btnOK;

	CImgRioButton m_rioDaili;
	CImgRioButton m_rioHuiYuan;

	CComboBox m_cmbFandian;
	CString m_strFandian;
	CComboBox m_cmbWebFandian;
	CString m_strWebFandian;
	CComboBox m_cmbWebRegUrl;

	CImageButton m_btnRegUrl;
	CImageButton m_btnSetWebFandian;
	CImageButton m_btnSetMorenMima;
	CString m_strRegUrl;
	CString m_strWebFandian1;
	CFont m_font;
	CSkinEditEx m_editQQ;
	CString m_strQQ;

	//CMissionManager					m_MissionManager;					//任务管理
 	//重载函数
 public:
//  	//连接事件
//  	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
//  	//关闭事件
//  	virtual bool OnEventMissionShut(BYTE cbShutReason);
 	//读取事件
 	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
 protected:
 	bool							m_bGetRegURL;						//获取注册链接
 	bool							m_AddHuiYuan;							//添加下级
	bool							m_bSetWebFandian;//设置网页注册返点
 protected:
 	VOID SendToServer(int nSendType);

};
