#pragma once
#include "Resource.h"
#include "../游戏客户端/ClientControl.h"


// CClientControlItemSinkDlg 对话框

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
	BYTE m_cbWinArea;				//赢牌区域
	BYTE m_cbExcuteTimes;			//执行次数

public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClientControlItemSinkDlg();

// 对话框数据
	enum { IDD = IDD_CLIENT_CONTROL_EX };
	LONGLONG  m_lAllUserBet[8];				//用户下注
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	//重置下注
	virtual void ResetUserBet();
	//玩家下注
	virtual void SetUserBetScore(BYTE cbArea,LONGLONG lScore);
	//申请结果
	virtual bool ReqResult(const void * pBuffer);

	virtual BOOL OnInitDialog();
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();

protected:
	virtual void OnCancel();

public:
	afx_msg void OnTimer(UINT nIDEvent);
};
