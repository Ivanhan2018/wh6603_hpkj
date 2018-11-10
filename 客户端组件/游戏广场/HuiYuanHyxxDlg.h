#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageButton.h"
#include "DlgLogon.h"
#include "MessageDlg.h"
#include "TextButton.h"
// CHuiYuanHyxxDlg 对话框
//单选控件
// class  CControlCheckButton
// {
// 	//状态变量
// protected:
// 	BYTE                           m_cbChecked;                        //记住密码
// 	CWnd *                         m_pParentSink;                      //父类指针
// 
// 	//位置变量
// protected:
// 	CPoint                         m_ptControlBenchmark;               //基准位置
// 
// 	//资源变量
// protected:
// 	CBitImage                      m_ImageBtnBack;                     //背景资源
// 
// 	//函数定义
// public:
// 	//构造函数
// 	CControlCheckButton();
// 	//析构函数
// 	virtual ~CControlCheckButton();
// 
// 	//辅助函数
// public:
// 	//控件区域
// 	CRect GetControlRect();
// 
// 	//辅助函数
// public:
// 	//基准位置
// 	inline VOID  SetControlBenchmark(INT nXPos,INT nYPos) { m_ptControlBenchmark.SetPoint(nXPos,nYPos); };
// 	//获取状态
// 	inline BYTE  GetButtonChecked() { return m_cbChecked; };
// 	//设置状态
// 	inline VOID  SetButtonChecked(BYTE cbChecked);
// 	//设置父类
// 	inline VOID  SetParentWndSink(CWnd * pParentSink) { m_pParentSink=pParentSink; };
// 
// 	//事件函数
// public:
// 	//绘制控件
// 	VOID  OnDrawControl(CDC * pDC);
// 	//点击事件
// 	VOID  OnClickControl(CPoint Point);
// 
// };

class CHuiYuanHyxxDlg : public CDialog//,public CMissionItem
{
	DECLARE_DYNAMIC(CHuiYuanHyxxDlg)

public:
	CHuiYuanHyxxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHuiYuanHyxxDlg();

// 对话框数据
	enum { IDD = IDD_DLG_HUIYUAN_HYXX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnPrePage();
	afx_msg void OnBnClickedBtnNextPage();

	afx_msg void OnBnClickedBtnChkxj1();
	afx_msg void OnBnClickedBtnChkxj2();
	afx_msg void OnBnClickedBtnChkxj3();
	afx_msg void OnBnClickedBtnChkxj4();
	afx_msg void OnBnClickedBtnChkxj5();
	afx_msg void OnBnClickedBtnChkxj6();
	afx_msg void OnBnClickedBtnChkxj7();
	afx_msg void OnBnClickedBtnChkxj8();
	afx_msg void OnBnClickedBtnChkxj9();
	afx_msg void OnBnClickedBtnChkxj10();

	afx_msg void OnHyxxChkxj();
	afx_msg void OnHyxxXgfd();
	afx_msg void OnHyxxSwdl();
	afx_msg void OnHyxxZhuanZ(); //转账
	afx_msg void OnHyxxFenPeipe(); //转账
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnNMRclickListLog(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	void InitListCtrl();
	void AdjustCtrls();
	void EnAbleCtrls(bool b);

private:

	CControlCheckButton m_CheckOnlineBtn;
	CControlCheckButton m_CheckUserYueBtn;
	Bitmap* m_bmpBk;
	DWORD m_dwTickCount;
	CDateTimeCtrl m_dateStart;
	CDateTimeCtrl m_dateEnd;
	CListCtrlCl m_listLog;
	CImageButton m_btnOK;
	CImageButton m_btnPrePage;
	CImageButton m_btnNextPage;
	CSkinComboBox m_cmbSort;
	CFont m_font;
	CTime m_tStart;
	CTime m_tEnd;
	CFont m_cmbfont;
	int m_nIndex;
	int m_nTargetUserID;
	DOUBLE m_fZhuanZhangJine;
	TCHAR		m_szPassWord[126];

	CString m_strName;
	int m_page;
	int m_itemCount;
	int m_maxPage;
	BOOL m_byTime;

	DOUBLE m_fXGFandian;
	int m_user_id;
	int m_byType;
	CSkinEditEx m_editAct;
	CSkinEditEx m_editID;
	CString m_strAct;
	CString m_strID;

	CTextButton	m_btnChkxj[10];
	int	m_nSetPeie1;
	int	m_nSetPeie2;
	int	m_nSetPeie3;

	//重载函数
public:
// 	//连接事件
// 	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
// 	//关闭事件
// 	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetHyxxLogCount;						//获取日志数量
	bool							m_bGetHYXXLog;							//获取日志
	bool							m_bGetHYXXByIDLog;						//获取日志通过ID
	bool							m_bGetHYXXByActLog;						//获取日志通过Act
	bool							m_bXgHyFandian;							//修改会员返点
	bool							m_bSetUserAgent;						//设置代理
	int								m_nItem;				
protected:
	VOID SendToServer(int nSendType);

};
