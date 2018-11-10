#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageButton.h"
#include "MessageDlg.h"

#include "ImgRioButton.h"

// CHuiYuanHyshjDlg 对话框

class CHuiYuanHyshjDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuiYuanHyshjDlg)

public:
	CHuiYuanHyshjDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHuiYuanHyshjDlg();

// 对话框数据
	enum { IDD = IDD_DLG_HUIYUAN_HYSHJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnChkToday();
	afx_msg void OnBnClickedBtnOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	Bitmap* m_bmpBk;
	CFont m_font;
	CImageButton m_btnOK;
	bool	m_bChkToday;
	CString m_xiajirenshu;
	CString m_zaixianrenshu;
	CString m_huiyuanyue;
	CString m_jinrifandian;
	CString m_jinrichongzhi;
	CString m_jinriqukuan;
	CString m_jinritouzhu;
	CString m_jinrihuodong;

	CString m_jinrizhuce;
	CString m_chedanzonge;
	CString m_zhongjiangzonge;
	CString m_wodefandian;
	CString m_caipiaoyingkui;
	CString m_xianmaliang;
	CString m_qipaifandian;
	CString m_qipaiyingkui;
	CString m_myqipaifandian;

	CDateTimeCtrl m_dateStart;
	CDateTimeCtrl m_dateEnd;
	CTime m_tStart;
	CTime m_tEnd;

	CImgRioButton m_rioChkToday;
	void AdjustCtrls();
	//重载函数
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetHuiyuanShj;						//获取日志数量
protected:
	VOID SendToServer(int nSendType);

};
