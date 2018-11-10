#ifndef MISSION_LOGON_HEAD_FILE
#define MISSION_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgLogon.h"
#include "DlgStatus.h"

//////////////////////////////////////////////////////////////////////////////////

//登录任务
class CMissionLogon : public CDlgStatus, public CMissionItem, public IStatusViewSink
{
	//友元定义
	friend class CDlgLogon;
	friend class CDlgRegister;

	//变量定义
protected:
	int								m_nAutoLogon;						//自动登录次数
	bool							m_bRegister;						//注册标志
	bool							m_bRemPassword;						//记住密码
	bool							m_bGetCPUserInfo;
	bool							m_bKefu;								//客服
	//控件指针
protected:
	CDlgLogon *						m_pDlgLogon;						//登录窗口
	//CDlgRegister *					m_pDlgRegister;						//注册窗口

	//静态变量
protected:
	static CMissionLogon *			m_pMissionLogon;					//对象指针

	//函数定义
public:
	//构造函数
	CMissionLogon();
	//析构函数
	virtual ~CMissionLogon();
	bool							m_bKillSocket;

	//状态接口
public:
	//取消连接
	virtual VOID OnStatusCancel();

	//重载函数
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//功能函数
public:
	//显示登录
	VOID ShowLogon();
	//显示注册
	VOID ShowRegister();

	//辅助函数
protected:
	//更新用户
	VOID UpdateUserInfo();
	//执行登录
	bool PerformLogonMission(bool bRemPassword);
	bool CreatePlazaview();
	bool PerformKefu();
	//网络事件
protected:
	//登录成功
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//登录失败
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//登录完成
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//升级提示
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);

	//静态函数
public:
	//获取对象
	static CMissionLogon * GetInstance() { return m_pMissionLogon; }
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif