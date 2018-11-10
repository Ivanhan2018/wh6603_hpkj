#pragma once

#include "afxwin.h"
#include "ImageButton.h"
#include "HuoDongHuanLeLingDlg.h"
#include "HuoDongZaJinDanDlg.h"
#include "HuoDongDaliBaoDlg.h"
#include "HuodongZhuanzhuan.h"
#include "HuoDongDaili.h"
#define  IDM_RETURN_GAME 11118
//活动专区
class CHuoDongDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuoDongDlg)

public:
	CHuoDongDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHuoDongDlg();

// 对话框数据
	enum { IDD = IDD_DLG_HUODONG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnClose();

	afx_msg void OnBnClickedBtnDalibao();
	afx_msg void OnBnClickedBtnZajindan();
	afx_msg void OnBnClickedBtnHuanlesong();
	afx_msg void OnBnClickedBtnZhuanzhuan();
	afx_msg void OnBnClickedBtnDaili();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	void AdjustCtrls();

private:
	Bitmap*					m_bmpBk;
	Bitmap*					m_bmpTip;
	CImageButton			m_btnDalibao;		//大礼包按钮
	CImageButton			m_btnZaJinDan;		//砸金蛋按钮
	CImageButton			m_btnHuanlesong;	//欢乐颂
	CImageButton			m_btnZhuanZhuan;	//幸运转转
	CImageButton			m_btnDaili;			//代理回馈
public:
	CHuoDongDaliBaoDlg		m_dalibao;		//大礼包
	CHuoDongZaJinDanDlg		m_zajindan;		//砸金蛋
	CHuoDongHuanLeSongDlg	m_huanlesong;	//欢乐颂
	HuodongZhuanzhuan		m_zhuanzhuan;	//幸运转转
	CHuoDongDaili			m_daili;		//代理
	CImageButton m_btnClose;
};
