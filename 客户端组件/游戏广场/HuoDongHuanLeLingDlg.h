#pragma once

#include "ImageButton.h"

// CHuoDongHuanLeSongDlg 对话框
//欢乐颂
class CHuoDongHuanLeSongDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuoDongHuanLeSongDlg)
public:
	void	GetUserHuanlesongInfo();
public:
	CHuoDongHuanLeSongDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHuoDongHuanLeSongDlg();

// 对话框数据
	enum { IDD = IDD_DLG_HUODONG_HUANLELING };

	void AdjustLingjiang();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	double		m_dJinriXiafei;			//今日消费
	double		m_dWinPrize;			//中奖金额		
	int			m_nSignedCount;			//签到次数
	int			m_XXCount;				//剩余签到次数

private:
	Bitmap* m_bmpBk;
	CFont m_font;

	CImageButton m_btnQiandao;
	CImageButton m_btnLingJiang;

	CImageButton m_btnQiandaoDay[7];

	afx_msg void OnBnClickedBtnQiandao();
	afx_msg void OnBnClickedBtnLingjiang();

	//重载函数
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bUserQianDao;						//获取日志数量
	bool							m_bGetUserHuanlesongInfo;						//获取日志数量
	bool							m_bUserHuanlesongLingjiang;						//获取日志数量通过ID
public:
	VOID SendToServer(int nSendType);
};
