#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// CHuoDongDaliBao 对话框
//大礼包
class CHuoDongDaliBaoDlg : public CDialog,public CMissionItem
{
	DECLARE_DYNAMIC(CHuoDongDaliBaoDlg)

public:
	CHuoDongDaliBaoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHuoDongDaliBaoDlg();

// 对话框数据
	enum { IDD = IDD_DLG_HUODONG_DALIBAO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnBnClickedLingJiang();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();


private:
	Bitmap* m_bmpBk;
	CFont m_font;
	CImageButton m_btnLingJiang;

	double m_jinrixiaofei;
	int m_jine;

	//CMissionManager					m_MissionManager;					//任务管理
	//重载函数
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetDaLiBao;						//获取日志数量
	bool							m_bGetLastYue;						//获取日志数量
	bool							m_bCanDaLiBao;						//获取日志数量通过ID
	bool							m_bGetXjYkTjByAct;						//获取日志数量通过账户
public:
	VOID SendToServer(int nSendType);
};
