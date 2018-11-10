#ifndef SERVER_VIEW_ITEM_HEAD_FILE
#define SERVER_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgStatus.h"
#include "ServerListData.h"
#include "ProcessManager.h"
#include "TableViewFrame.h"
#include "DlgServerManager.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//服务状态
enum enServiceStatus
{
	ServiceStatus_Unknow,			//未知状态
	ServiceStatus_Entering,			//进入状态
	ServiceStatus_Validate,			//验证状态
	ServiceStatus_RecvInfo,			//读取状态
	ServiceStatus_ServiceIng,		//服务状态
	ServiceStatus_NetworkDown,		//中断状态
};

//////////////////////////////////////////////////////////////////////////////////

//游戏房间
class CServerViewItem : public CDialog, public IServerViewItem, public ITCPSocketSink,
	public IUserManagerSink, public IProcessManagerSink, public IStatusViewSink,
	public ITableViewFrameSink
{
	//界面变量
protected:
	bool							m_bCreateFlag;						//创建标志

	//用户属性
protected:
	DWORD							m_dwUserRight;						//用户权限
	DWORD							m_dwMasterRight;					//管理权限
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针

	//房间属性
protected:
	BOOL                            m_bAllowDistribute;                 //允许分组
	WORD							m_wServerType;						//房间类型
	DWORD							m_dwServerRule;						//房间规则
	enServiceStatus					m_ServiceStatus;					//房间状态

	//房间属性
protected:
	tagGameKind						m_GameKind;							//类型信息
	tagGameServer					m_GameServer;						//房间信息

	//配置信息
protected:
	BYTE							m_cbColumnCount;					//列表数目
	tagColumnItem					m_ColumnItem[MAX_COLUMN];			//列表描述

	//功能组件
protected:
	CDlgStatus						m_DlgStatus;						//状态窗口
	CProcessManager					m_ProcessManager;					//进程管理
	CTableViewFrame					m_TableViewFrame;					//桌子框架

	//控件变量
protected:
	CDlgInsureServer *				m_pDlgInsureServer;					//银行对象
	CDlgServerManager *				m_pDlgServerManager;				//管理对象
	CPlatformPublicize				m_PlatformPublicize;				//浏览控件

	//组件变量
protected:
	CTCPSocketHelper				m_TCPSocketModule;					//网络连接
	CGameLevelParserHelper			m_GameLevelParserModule;			//游戏等级
	CPlazaUserManagerHelper			m_PlazaUserManagerModule;			//用户管理


	//函数定义
public:
	//构造函数
	CServerViewItem();
	//析构函数
	virtual ~CServerViewItem();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//重载函数
protected:
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//桌子接口
public:
	//桌子按钮
	virtual VOID OnHitTableButton(BYTE cbButton);
	//鼠标双击
	virtual VOID OnDButtonHitTable(WORD wTableID);
	//左键按下
	virtual VOID OnLButtonHitTable(WORD wTableID, WORD wChairID);
	//右键按下
	virtual VOID OnRButtonHitTable(WORD wTableID, WORD wChairID);

	//网络接口
protected:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//进程接口
protected:
	//进程错误
	virtual bool OnGameProcessError();
	//进程关闭
	virtual bool OnGameProcessClose(DWORD dwExitCode);
	//进程启动
	virtual bool OnGameProcessCreate(bool bAlreadyExist);
	//进程数据
	virtual bool OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//用户接口
public:
	//用户激活
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem);
	//用户删除
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem);
	//用户更新
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserSpreadInfo & LastSpreadInfo);

	//事件接口
protected:
	//取消连接
	virtual VOID OnStatusCancel();

	//信息函数
public:
	//房间标识
	virtual WORD GetServerID() { return m_GameServer.wServerID; }
	//房间名字
	virtual LPCTSTR GetServerName() { return m_GameServer.szServerName; }
	//服务状态
	virtual enServiceStatus GetServiceStatus() { return m_ServiceStatus; }

	//组件函数
public:
	//网络组件
	ITCPSocket * GetTCPSocket() { return m_TCPSocketModule.GetInterface(); }
	//用户组件
	IPlazaUserManager * GetPlazaUserManager() { return m_PlazaUserManagerModule.GetInterface(); }

	//功能函数
public:
	//显示银行
	VOID ShowInsureView();

	//网络事件
protected:
	//用户处理
	bool OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//登录处理
	bool OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//配置处理
	bool OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//银行处理
	bool OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//管理处理
	bool OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//系统处理
	bool OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏处理
	bool OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//网络事件
protected:
	//用户进入
	bool OnSocketSubUserEnter(VOID * pData, WORD wDataSize);
	//用户积分
	bool OnSocketSubUserScore(VOID * pData, WORD wDataSize);
	//用户状态
	bool OnSocketSubUserStatus(VOID * pData, WORD wDataSize);
	//用户信息
	bool OnSocketSubUserSpreadInfo(VOID * pData, WORD wDataSize);
	//登录完成
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//登录成功
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//登录失败
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//更新提示
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);
	//请求失败
	bool OnSocketSubRequestFailure(VOID * pData, WORD wDataSize);
	//系统消息
	bool OnSocketSubSystemMessage(VOID * pData, WORD wDataSize);
	//动作消息
	bool OnSocketSubActionMessage(VOID * pData, WORD wDataSize);

	//界面函数
public:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//配置房间
	VOID InitServerViewItem(CGameServerItem * pGameServerItem);
	//资源目录
	VOID GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount);

	//网络命令
public:
	BOOL GetHostIP(TCHAR * inData) ;
	//发送登录
	bool SendLogonPacket();
	//发送旁观
	bool SendLookonPacket(WORD wTableID, WORD wChairID);
	//发送坐下
	bool SendSitDownPacket(WORD wTableID, WORD wChairID);
	//发送起立
	bool SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave);

	//动作处理
public:
	//执行旁观
	bool PerformLookonAction(WORD wTableID, WORD wChairID);
	//执行起立
	bool PerformStandUpAction(WORD wTableID, WORD wChairID);
	//执行坐下
	bool PerformSitDownAction(WORD wTableID, WORD wChairID);

	//功能函数
public:
	//权限检查
	bool CheckUserRight();

	//消息函数
protected:
	//销毁消息
	VOID OnNcDestroy();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif