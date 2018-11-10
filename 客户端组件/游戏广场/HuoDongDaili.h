#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// CHuoDongDaliBao 对话框
//大礼包
class CHuoDongDaili : public CDialog
{
	DECLARE_DYNAMIC(CHuoDongDaili)

public:
	CHuoDongDaili(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHuoDongDaili();

// 对话框数据
	enum { IDD = IDD_DLG_HUODONG_DAILI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnBnClickedLingJiang();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	void AdjustLingjiang();
	CImageButton m_btnQianDao[9];
private:
	Bitmap* m_bmpBk;
	CFont m_font;
	int   m_nToday;
	//重载函数
public:
	//读取事件
	 bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetDaLiBao;						//获取日志数量
	bool							m_bGetLastYue;						//获取日志数量
	bool							m_bCanDaLiBao;						//获取日志数量通过ID
	bool							m_bGetXjYkTjByAct;						//获取日志数量通过账户
	BYTE							m_cbNewRegUser;						//新注册用户
	DOUBLE							m_fTouZhuJine;						//投注金额
	DOUBLE							m_fChongZhiJine;					//充值金额
public:
	VOID SendToServer(int nSendType);
};
