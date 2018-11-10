#pragma once

#include "afxwin.h"
#include "TextButton.h"
#include "ZhangHuBaseInfoDlg.h"
#include "ZhangHuXgmmDlg.h"
#include "ZhangHuZhhbhDlg.h"
#include "ZhangHuShZhTkZhhDlg.h"
#include "ZhangHuTzhLogDlg.h"
#include "ZhangHuTxLogDlg.h"
#include "ZhangHuChzhLogDlg.h"
#include "ZhangHuYkLogDlg.h"
#include "ZhangHuYkMXDlg.h"
#include "ZhangHuQPYkDlg.h"
#define  IDM_RETURN_GAME 11118
#define IDM_RELEASE_FACE	11119
#include "MessageDlg.h"
//我的账户
class CZhangHuDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuDlg)

	typedef enum {
		BASEINFO,
		MODIFYPWD,
		ZHANGHAOBAOHU,
		TIXIANZHANGHU,
		TOUZHUJILU,
		TIXIANJILU,
		CHONGZHIJILU,
		YINGKUIJILU,
		YINGKUIMINGXI,
		QIPAIYINGKUI
	}ShowType;

public:
	CZhangHuDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZhangHuDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ZHANGHU };
	CImageButton m_btnClose;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnClose();

	afx_msg void OnBnClickedBtnBaseinfo();
	afx_msg void OnBnClickedBtnAccountbaohu();
	afx_msg void OnBnClickedBtnTikuanact();
	afx_msg void OnBnClickedBtnTouzhujilu();
	afx_msg void OnBnClickedBtnTixianjilu();
	afx_msg void OnBnClickedBtnChongzhijilu();
	afx_msg void OnBnClickedBtnYingkuijilu();
	afx_msg void OnBnClickedBtnYingkuimingxi();
	afx_msg void OnBnClickedBtnQipaiYingkui();

	LRESULT OnReleaseFace(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnModifypwd();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	void InitBtns();
	void ResetDlg();

private:
	DWORD m_dwTickCount;
	Bitmap* m_bmpBk;
	Bitmap* m_bmpTip;
	CFont m_font;
	CTextButton m_btnBaseInfo;		
	CTextButton m_btnModifyPwd;
	CTextButton m_btnZhangHaoBaoHu;
	CTextButton m_btnTiKuanZhangHao;
	CTextButton m_btnTouZhuJiLu;
	CTextButton m_btnTiXianJiLu;
	CTextButton m_btnChongZhiJiLu;
	CTextButton m_btnYingKuiJiLu;
	CTextButton m_btnYingKuiMingXi;		
	CTextButton m_btnQipaiYingkui;		

	ShowType m_showType;
public:
	CZhangHuBaseInfoDlg	m_baseInfoDlg;	//基本信息子窗口
	CZhangHuXgmmDlg	m_xgmmDlg;	//修改密码子窗口
	CZhangHuZhhbhDlg m_zhhbhDlg;	//账户保护子窗口
	CZhangHuShZhTkZhhDlg m_shzhTkzhh;	//设置提款账户
	CZhangHuTzhLogDlg m_tzhlogDlg;	//投注记录子窗口
	CZhangHuTxLogDlg m_txlogDlg;	//提现记录子窗口
	CZhangHuChzhLogDlg m_chzhlogdlg;	//充值记录子窗口
	CZhangHuYkLogDlg m_yklogdlg;	//充值记录子窗口
	CZhangHuYkMXDlg m_ykmxdlg;	//盈亏子窗口
	CZhangHuQPYkDlg m_qpykdlg;	//盈亏子窗口
};
