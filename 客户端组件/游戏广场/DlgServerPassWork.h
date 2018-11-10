#ifndef DLG_SERVER_PASSWORK_HEAD_FILE
#define DLG_SERVER_PASSWORK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

////////////////////////////////////////////////////////////////////////////////

class CDlgServerPassWork : public CDialog
{


public:
	CDlgServerPassWork(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgServerPassWork();

// 对话框数据
	enum { IDD = IDD_DLG_SERVER_PASSWORK };

private:
	CString							m_StrPassWork;						//房间密码
		//控件变量
protected:
	CSkinButton						m_btOk;								//确认操作
	CSkinButton						m_btCancel;							//取消操作
	CSkinEditEx						m_ServerPassWork;					//登录帐号
	//界面资源
protected:
	CSkinLayered					m_SkinLayered;						//分层窗口
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//配置函数
	virtual BOOL OnInitDialog();
	//用户自定义函数
public:
	void SetPassWork(CString StrPassWork);
	//消息函数
public:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//显示消息
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//位置改变
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//按钮消息
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	DECLARE_MESSAGE_MAP()
};




////////////////////////////////////////////////////////////////////////////////



#endif