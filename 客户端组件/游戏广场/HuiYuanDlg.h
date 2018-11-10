#pragma once

#include "afxwin.h"
#include "TextButton.h"
#include "HuiYuanHyxxDlg.h"
#include "HuiYuanTjxjDlg.h"
#include "HuiYuanXjtzhtjDlg.h"
#include "HuiYuanChkxjtzhDlg.h"
#include "HuiYuanHyshjDlg.h"
#include "HuiYuanYktjDlg.h"
#include "HuiYuanYkLogDlg.h"
#include "HuiYuanChzhLogDlg.h"
#include "HuiYuanTxLogDlg.h"
#include "HuiYuanXjYouXiJlDlg.h"
#include "HuiYuanYxtjDlg.h"
#include "MessageDlg.h"
#define  IDM_RETURN_GAME 11118

//会员管理
class CHuiYuanDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuiYuanDlg)

public:
	CHuiYuanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHuiYuanDlg();

// 对话框数据
	enum { IDD = IDD_DLG_HUIYUAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedBtnHuiyuanxinxi();
	afx_msg void OnBnClickedBtnTianjiaxiaji();
	afx_msg void OnBnClickedBtnXiajitouzhutongji();
	afx_msg void OnBnClickedBtnChakanxiajitouzhu();
	afx_msg void OnBnClickedBtnHuiyuanshuju();
	afx_msg void OnBnClickedBtnYingkuitongji();
	afx_msg void OnBnClickedBtnYingkuijilu();
	afx_msg void OnBnClickedBtnChongzhijilu();
	afx_msg void OnBnClickedBtnTixianjilu();
	afx_msg void OnBnClickedBtnXjYouXiYk();
	afx_msg void OnBnClickedBtnXjYouXiJl();
	afx_msg void OnBnClickedBtnClose();

	LRESULT OnQueryXJykmx(WPARAM wParam, LPARAM lParam);
	LRESULT OnQueryXJTzh(WPARAM wParam, LPARAM lParam);
	LRESULT OnQueryXJYxYk(WPARAM wParam, LPARAM lParam);

	CImageButton m_btnClose;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

protected:
	virtual BOOL OnInitDialog();
	CFont m_font;

private:
	void InitBtns();
	DWORD m_dwTickCount;

private:
	Bitmap* m_bmpBk;
	Bitmap* m_bmpTip;

	CTextButton m_btnHuiYuanXinXi;
	CTextButton m_btnTianJiaXiaJi;
	CTextButton m_btnXiaJiTouZhuTongJi;
	CTextButton m_btnChaKanXiaJiTouZhu;
	CTextButton m_btnHuiYuanShuJu;
	CTextButton m_btnYingKuiTongJi;
	CTextButton m_btnYingKuiJiLu;
	CTextButton m_btnChongZhiJiLu;
	CTextButton m_btnTiXianJiLu;
	CTextButton m_btnXjYouxiYk;
	CTextButton m_btnXjYouxiJl;
	
public:
	CHuiYuanHyxxDlg m_hyxxDlg;
	CHuiYuanTjxjDlg m_tjxjDlg;
	CHuiYuanXjtzhtjDlg m_xjtzhtjDlg;
	CHuiYuanChkxjtzhDlg m_chkxjtzhDlg;
	CHuiYuanHyshjDlg m_hyshjDlg;
	CHuiYuanYktjDlg m_yktjDlg;
	CHuiYuanYkLogDlg m_yklogDlg;
	CHuiYuanChzhLogDlg m_chzhlogDlg;
	CHuiYuanTxLogDlg m_txlogDlg;
	CHuiYuanXjYouXiJlDlg m_xjyxjlDlg;
	CHuiYuanYxtjDlg m_yxtjlDlg;
protected:
	virtual void OnCancel();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};
