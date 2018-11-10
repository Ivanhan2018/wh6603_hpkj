#include "StdAfx.h"
#include "GamePlaza.h"
#include "ServerViewItem.h"

//数据控件
#include "GlobalUnits.h"
#include "PlatformEvent.h"

//界面控件
#include "PlatformFrame.h"
#include "DlgServerManager.h"
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_SERVER_TABLE			100									//桌子控件
#define IDC_WEB_PUBLICIZE           101                                 //浏览控件

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerViewItem, CDialog)
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerViewItem::CServerViewItem()
{
	//界面变量
	m_bCreateFlag=false;

	//用户属性
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//房间属性
	m_wServerType=0L;
	m_dwServerRule=0L;
	m_ServiceStatus=ServiceStatus_Unknow;

	//控件变量
	m_pDlgInsureServer=NULL;
	m_pDlgServerManager=NULL;

	//设置组件
	m_DlgStatus.SetStatusViewSink(this);

	//配置信息
	m_cbColumnCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));

	//房间属性
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	return;
}

//析构函数
CServerViewItem::~CServerViewItem()
{
}

//接口查询
VOID * CServerViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableViewFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerViewItem,Guid,dwQueryVer);
	return NULL;
}

//创建函数
BOOL CServerViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//设置变量
	m_bCreateFlag=true;

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_TableViewFrame.CreateTableFrame(this,IDC_SERVER_TABLE,QUERY_ME_INTERFACE(IUnknownEx));

	m_TableViewFrame.ShowWindow(SW_HIDE);
	//创建组件
	if (m_GameLevelParserModule.CreateInstance()==false) throw TEXT("等级解释组件创建失败！");
	if (m_PlazaUserManagerModule.CreateInstance()==false) throw TEXT("用户管理组件创建失败！");

	//设置组件
	m_ProcessManager.SetProcessManagerSink(QUERY_ME_INTERFACE(IUnknownEx));

	//配置组件
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	if (m_PlazaUserManagerModule->SetUserManagerSink(pIUnknownEx)==false) throw TEXT("用户管理组件配置失败！");
	if (m_PlazaUserManagerModule->SetUserInformation(pUserInformation)==false) throw TEXT("用户管理组件配置失败！");

	return TRUE;
}

//消息过虑
BOOL CServerViewItem::PreTranslateMessage(MSG * pMsg)
{
	//按键消息
	if (pMsg->message==WM_KEYDOWN)
	{
		//取消按钮
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}

		//回车按钮
		if (pMsg->wParam==VK_RETURN)
		{
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CServerViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//按钮命令
	switch (nCommandID)
	{
	case IDC_BT_QUIT_SERVER:		//退出房间
		{
			//发送命令
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);

			return TRUE;
		}
	}

	//菜单命令
	switch (nCommandID)
	{
	case IDM_DELETE_SERVER_ITEM:	//关闭房间
		{
			//中断连接
			if (m_TCPSocketModule.GetInterface()!=NULL)
			{
				m_TCPSocketModule->CloseSocket();
				m_ServiceStatus=ServiceStatus_NetworkDown;
			}

			//关闭房间
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->DeleteServerViewItem();

			return TRUE;
		}
	case IDM_MANAGER_SERVER:		//房间管理
		{
			//变量定义
			CDlgServerManager DlgServerManager;
			DlgServerManager.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

			//设置变量
			m_pDlgServerManager=&DlgServerManager;

			//显示窗口
			DlgServerManager.DoModal();

			//设置变量
			m_pDlgServerManager=NULL;

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//桌子按钮
VOID CServerViewItem::OnHitTableButton(BYTE cbButton)
{
	//按钮处理
	switch (cbButton)
	{
	case BT_ENTER_GAME:		//进入游戏
		{
			//自动分组
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				//权限判断
				if(CheckUserRight()==false) break;
				
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				if(pGlobalUserData->lScore < m_GameServer.lMinServerScore)
				{
					CInformation Information(this);
					Information.ShowMessageBox(L"您的游戏币不够加入条件，请充值");
					//关闭房间
					PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

					break;
				}
				//启动进程
				bool bSuccess=m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);

				//错误处理
				if (bSuccess==false)
				{
					//中断连接
					m_TCPSocketModule->CloseSocket();
					m_ServiceStatus=ServiceStatus_NetworkDown;

					return;
				}

				//发送分组
				PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR);
			}

			break;
		}
	case BT_CLOSE_SERVER:	//关闭房间
		{
			//退出判断
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetUserStatus()==US_PLAYING))
			{
				//提示消息
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("您正在游戏中，强行退出将被扣分，确实要强退吗？"),MB_ICONQUESTION|MB_YESNO,0)!=IDYES)
				{
					return;
				}
			}

			//关闭房间
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

			break;
		}
	}

	return;
}

//鼠标双击
VOID CServerViewItem::OnDButtonHitTable(WORD wTableID)
{
	return;
}

//左键按下
VOID CServerViewItem::OnLButtonHitTable(WORD wTableID, WORD wChairID)
{
	return;
}

//右键按下
VOID CServerViewItem::OnRButtonHitTable(WORD wTableID, WORD wChairID)
{
	return;
}

//连接事件
bool CServerViewItem::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//错误提示
	if (nErrorCode!=0L)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("很抱歉，游戏房间连接失败，请稍后再试或留意网站公告！"),MB_ICONSTOP);

		return true;
	}

	//发送登录
	SendLogonPacket();

	//设置状态
	m_ServiceStatus=ServiceStatus_Validate;

	return true;
}

//关闭事件
bool CServerViewItem::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//关闭状态
	m_DlgStatus.HideStatusWindow();

	//关闭游戏
	m_ProcessManager.CloseGameProcess(false);

	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		if (m_ServiceStatus==ServiceStatus_ServiceIng)
		{
			//关闭提示
			CInformation Information(this);
			INT_PTR nResult=Information.ShowMessageBox(m_GameServer.szServerName,TEXT("与游戏服务器的连接已经中断，是否继续游戏？"),
				MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2,0L);

			//关闭房间
			if (nResult==IDYES)
			{
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM);
			}
		}
		else
		{
			//关闭提示
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,TEXT("由于意外的原因，房间连接失败，请稍后重新尝试！"),MB_ICONERROR);

			//关闭房间
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
		}
	}

	//设置状态
	m_ServiceStatus=ServiceStatus_NetworkDown;

	return true;
}

//读取事件
bool CServerViewItem::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON:		//登录消息
		{
			return OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_CONFIG:		//配置信息
		{
			return OnSocketMainConfig(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_USER:		//用户信息
		{
			return OnSocketMainUser(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_INSURE:		//银行消息
		{
			return OnSocketMainInsure(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_MANAGE:		//管理消息
		{
			return OnSocketMainManager(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_CM_SYSTEM:		//系统消息
		{
			return OnSocketMainSystem(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GF_GAME:		//游戏消息
	case MDM_GF_FRAME:		//框架消息
		{
			return OnSocketMainGameFrame(Command.wMainCmdID,Command.wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//进程错误
bool CServerViewItem::OnGameProcessError()
{
	return true;
}

//进程关闭
bool CServerViewItem::OnGameProcessClose(DWORD dwExitCode)
{
	//效验状态
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//变量定义
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();
	BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

	//退出游戏
	if ((wTableID!=INVALID_TABLE)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		SendStandUpPacket(wTableID,wChairID,TRUE);
		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	}

	return true;
}

//进程启动
bool CServerViewItem::OnGameProcessCreate(bool bAlreadyExist)
{
	//变量定义
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();

	//房间配置
	if (bAlreadyExist==false)
	{
		//获取等级
		tagLevelItem LevelItem[64];
		BYTE cbLevelCount=(BYTE)m_GameLevelParserModule->GetGameLevelItem(LevelItem,CountArray(LevelItem));

		//发送配置
		m_ProcessManager.SendLevelInfo(LevelItem,cbLevelCount);
		m_ProcessManager.SendColumnInfo(m_ColumnItem,m_cbColumnCount);
	}

	//房间信息
	m_ProcessManager.SendServerInfo(m_pIMySelfUserItem,m_dwUserRight,m_dwMasterRight,m_dwServerRule,m_wServerType,m_bAllowDistribute,m_GameServer);

	//发送用户
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		//变量定义
		WORD wEnumIndex=0;
		IPlazaUserManager * pIPlazaUserManager=m_PlazaUserManagerModule.GetInterface();

		//发送玩家
		while (true)
		{
			//获取用户
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//发送判断
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()==US_LOOKON) continue;

			//发送用户
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};

		//旁观用户
		wEnumIndex=0;
		while (true)
		{
			//获取用户
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//发送判断
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()!=US_LOOKON) continue;

			//发送用户
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};
	}

	//配置完成
	m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_CONFIG_FINISH);

	return true;
}

//进程数据
bool CServerViewItem::OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//网络消息
	if ((wMainCmdID==IPC_CMD_GF_SOCKET)&&(wSubCmdID==IPC_SUB_GF_SOCKET_SEND))
	{
		//效验数据
		ASSERT(wDataSize>=sizeof(TCP_Command));
		if (wDataSize<sizeof(TCP_Command)) return false;

		//变量定义
		IPC_GF_SocketSend * pSocketPackage=(IPC_GF_SocketSend *)pData;
		WORD wPacketSize=wDataSize-(sizeof(IPC_GF_SocketSend)-sizeof(pSocketPackage->cbBuffer));

		//处理数据
		if (wPacketSize==0)
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID);
		}
		else 
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID,pSocketPackage->cbBuffer,wPacketSize);
		}

		return true;
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//用户激活
VOID CServerViewItem::OnUserItemAcitve(IClientUserItem * pIClientUserItem)
{
	//判断自己
	if (m_pIMySelfUserItem==NULL)
	{
		m_pIMySelfUserItem=pIClientUserItem;
	}

	//设置桌子
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		WORD wTableID=pIClientUserItem->GetTableID();
		WORD wChairID=pIClientUserItem->GetChairID();
		m_TableViewFrame.SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	return;
}

//用户删除
VOID CServerViewItem::OnUserItemDelete(IClientUserItem * pIClientUserItem)
{
	//变量定义
	WORD wLastTableID=pIClientUserItem->GetTableID();
	WORD wLastChairID=pIClientUserItem->GetChairID();
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//变量定义
	ASSERT(CParameterGlobal::GetInstance());
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//离开处理
	if ((wLastTableID!=INVALID_TABLE)&&(wLastChairID!=INVALID_CHAIR))
	{
		//获取用户
		IClientUserItem * pITableUserItem=m_TableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_TableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,NULL);

		//离开通知
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.cbUserStatus=US_NULL;
			UserStatus.wTableID=INVALID_TABLE;
			UserStatus.wChairID=INVALID_CHAIR;
			m_ProcessManager.SendUserStatus(pIClientUserItem->GetUserID(),UserStatus);
		}
	}

	return;
}

//用户更新
VOID CServerViewItem::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	return;
}

//用户更新
VOID CServerViewItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//界面通知
	if ((m_wServerType&GAME_GENRE_GOLD)&&(pIClientUserItem==m_pIMySelfUserItem))
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		pGlobalUserData->lScore=pIClientUserItem->GetUserScore();
		pGlobalUserData->lInsure=pIClientUserItem->GetUserInsure();

		//更新银行
		if(m_pDlgInsureServer!=NULL) 
			m_pDlgInsureServer->OnEventUpdateInsureServer(pGlobalUserData->lScore,pGlobalUserData->lInsure);

		//事件通知
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//游戏通知
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//变量定义
		tagUserScore UserScore;
		ZeroMemory(&UserScore,sizeof(UserScore));

		//设置变量
		UserScore.lScore=pUserInfo->lScore;
		UserScore.dwWinCount=pUserInfo->dwWinCount;
		UserScore.dwLostCount=pUserInfo->dwLostCount;
		UserScore.dwDrawCount=pUserInfo->dwDrawCount;
		UserScore.dwFleeCount=pUserInfo->dwFleeCount;
		UserScore.dwExperience=pUserInfo->dwExperience;

		//发送数据
		m_ProcessManager.SendUserScore(pUserInfo->dwUserID,UserScore);
	}

	return;
}

//用户更新
VOID CServerViewItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserSpreadInfo & LastSpreadInfo)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//游戏通知
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//变量定义
		tagUserSpreadInfo UserSpreadInfo;
		ZeroMemory(&UserSpreadInfo,sizeof(UserSpreadInfo));

		//设置变量
		CopyMemory(UserSpreadInfo.szIpAddrDescribe,pIClientUserItem->GetAddrDescribe(),sizeof(UserSpreadInfo.szIpAddrDescribe));

		//发送数据
		m_ProcessManager.SendUserSpreadInfo(pUserInfo->dwUserID,UserSpreadInfo);
	}

	return;
}

//用户更新
VOID CServerViewItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	WORD wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	WORD wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	BYTE cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;

	//桌子离开
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		IClientUserItem * pITableUserItem=m_TableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_TableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,NULL);
	}

	//桌子加入
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		m_TableViewFrame.SetClientUserItem(wNowTableID,wNowChairID,pIClientUserItem);
	}

	//离开通知
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//游戏通知
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.wTableID=wNowTableID;
			UserStatus.wChairID=wNowChairID;
			UserStatus.cbUserStatus=cbNowStatus;
			m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);
		}

		return;
	}

	//加入处理
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//游戏通知
		if ((m_pIMySelfUserItem!=pIClientUserItem)&&(pMeUserInfo->wTableID==wNowTableID))
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			m_ProcessManager.SendUserItem(pIClientUserItem);
		}

		//启动进程
		if (m_pIMySelfUserItem==pIClientUserItem)
		{
			m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);
		}

		return;
	}

	//状态改变
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		//游戏通知
		tagUserStatus UserStatus;
		UserStatus.wTableID=wNowTableID;
		UserStatus.wChairID=wNowChairID;
		UserStatus.cbUserStatus=cbNowStatus;
		m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);

		return;
	}

	return;
}

//取消连接
VOID CServerViewItem::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

//显示银行
VOID CServerViewItem::ShowInsureView()
{
	//效验参数
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//房间类型
	if((m_wServerType&GAME_GENRE_GOLD)==0)
	{
		//变量定义
		CInformation Information;
		Information.ShowMessageBox(TEXT("抱歉,您当前所在的房间不允许银行操作,请先退出房间!"));
        
		return;
	}

	//变量定义
	CDlgInsureServer DlgInsureServer;
	DlgInsureServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);
	
	//设置变量
	m_pDlgInsureServer=&DlgInsureServer;

	//显示窗口
	DlgInsureServer.DoModal();

	//设置变量
	m_pDlgInsureServer=NULL;

	return;
}

//用户处理
bool CServerViewItem::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_ENTER:			//用户进入
		{
			return OnSocketSubUserEnter(pData,wDataSize);
		}
	case SUB_GR_USER_SCORE:			//用户积分
		{
			return OnSocketSubUserScore(pData,wDataSize);
		}
	case SUB_GR_USER_STATUS:		//用户状态
		{
			return OnSocketSubUserStatus(pData,wDataSize);
		}
	case SUB_GR_USER_SPREADINFO:    //用户信息
		{
			return OnSocketSubUserSpreadInfo(pData,wDataSize);
		}
	case SUB_GR_REQUEST_FAILURE:	//请求失败
		{
			return OnSocketSubRequestFailure(pData,wDataSize);
		}
	}

	return true;
}

//登录处理
bool CServerViewItem::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:	//登录成功
		{
			return OnSocketSubLogonSuccess(pData,wDataSize);
		}
	case SUB_GR_LOGON_FAILURE:	//登录失败
		{
			return OnSocketSubLogonFailure(pData,wDataSize);
		}
	case SUB_GR_LOGON_FINISH:	//登录完成
		{
			return OnSocketSubLogonFinish(pData,wDataSize);
		}
	case SUB_GR_UPDATE_NOTIFY:	//更新提示
		{
			return OnSocketSubUpdateNotify(pData,wDataSize);
		}
	}

	return true;
}

//配置处理
bool CServerViewItem::OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:		//列表配置
		{
			//变量定义
			CMD_GR_ConfigColumn * pConfigColumn=(CMD_GR_ConfigColumn *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigColumn)-sizeof(pConfigColumn->ColumnItem);

			//效验参数
			ASSERT((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))==wDataSize);
			if ((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))!=wDataSize) return false;

			//保存信息
			m_cbColumnCount=__min(pConfigColumn->cbColumnCount,CountArray(m_ColumnItem));
			CopyMemory(m_ColumnItem,pConfigColumn->ColumnItem,sizeof(tagColumnItem)*m_cbColumnCount);

			return true;
		}
	case SUB_GR_CONFIG_SERVER:		//房间配置
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_GR_ConfigServer));
			if (wDataSize<sizeof(CMD_GR_ConfigServer)) return false;

			//消息处理
			CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)pData;

			//房间属性
			m_wServerType=pConfigServer->wServerType;
			m_dwServerRule=pConfigServer->dwServerRule;
			m_bAllowDistribute=pConfigServer->bAllowDistribute;

			//构造参数
			WORD wTableCount=pConfigServer->wTableCount;
			WORD wChairCount=pConfigServer->wChairCount;
			IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);

			//创建桌子
			m_TableViewFrame.ConfigTableFrame(wTableCount,wChairCount);

			return true;
		}
	case SUB_GR_CONFIG_FINISH:		//配置完成
		{
			//资源目录
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//游戏等级
			ASSERT(m_GameLevelParserModule.GetInterface()!=NULL);
			bool bSuccess=m_GameLevelParserModule->LoadGameLevelItem(m_GameKind.szKindName,szResDirectory,m_wServerType);

			//错误处理
			if (bSuccess==false)
			{
				//关闭连接
				m_TCPSocketModule->CloseSocket();

				//提示信息
				INT nResult=ShowInformation(TEXT("游戏等级配置读取失败或者格式错误，是否现在进行修复？"),MB_ICONERROR|MB_YESNO);

				//下载游戏
				if (nResult==IDYES)
				{
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//关闭房间
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	}

	return true;
}

//银行处理
bool CServerViewItem::OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(m_pDlgInsureServer!=NULL);
	if (m_pDlgInsureServer==NULL) return true;

	//消息处理
	if (m_pDlgInsureServer->OnServerInsureMessage(MDM_GR_INSURE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//管理处理
bool CServerViewItem::OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_OPTION_CURRENT:	//当前配置
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GR_OptionCurrent));
			if (wDataSize!=sizeof(CMD_GR_OptionCurrent)) return false;

			//消息处理
			if (m_pDlgServerManager!=NULL)
			{
				CMD_GR_OptionCurrent * pOptionCurrent=(CMD_GR_OptionCurrent *)pData;
				m_pDlgServerManager->SetServerOptionInfo(pOptionCurrent->ServerOptionInfo,pOptionCurrent->dwRuleMask);
			}

			return true;
		}
	}

	return true;
}

//系统处理
bool CServerViewItem::OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CM_SYSTEM_MESSAGE:	//系统消息
		{
			return OnSocketSubSystemMessage(pData,wDataSize);
		}
	case SUB_CM_ACTION_MESSAGE:	//动作消息
		{
			return OnSocketSubActionMessage(pData,wDataSize);
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏处理
bool CServerViewItem::OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;

	//构造数据
	IPC_GF_SocketRecv SocketRecv;
	SocketRecv.CommandInfo.wSubCmdID=wSubCmdID;
	SocketRecv.CommandInfo.wMainCmdID=wMainCmdID;

	//叠加数据
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketRecv.cbBuffer,pData,wDataSize);
	}

	//发送数据
	WORD wSendSize=sizeof(SocketRecv)-sizeof(SocketRecv.cbBuffer)+wDataSize;
	m_ProcessManager.SendProcessData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_RECV,&SocketRecv,wSendSize);

	return true;
}

//用户进入
bool CServerViewItem::OnSocketSubUserEnter(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead)) return false;

	//变量定义
	tagUserInfo UserInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));

	//消息处理
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//用户属性
	UserInfo.wFaceID=pUserInfoHead->wFaceID;
	UserInfo.dwUserID=pUserInfoHead->dwUserID;
	UserInfo.cbGender=pUserInfoHead->cbGender;
	UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
	UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

	//用户状态
	UserInfo.wTableID=pUserInfoHead->wTableID;
	UserInfo.wChairID=pUserInfoHead->wChairID;
	UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

	//用户积分
	UserInfo.lScore=pUserInfoHead->lScore;
	UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
	UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
	UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
	UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;
	UserInfo.dwExperience=pUserInfoHead->dwExperience;

	//变量定义
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));

	//扩展信息
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GR_NICK_NAME:		//用户昵称
			{
				ASSERT(pDataBuffer!=NULL);
				ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
				if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
				{
					CopyMemory(&UserInfo.szNickName,pDataBuffer,DataDescribe.wDataSize);
					UserInfo.szNickName[CountArray(UserInfo.szNickName)-1]=0;
				}
				break;
			}
		case DTP_GR_ADDR_DESCRIBE: //地址描述
			{
				ASSERT(pDataBuffer!=NULL);
				ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szAddrDescribe));
				if (DataDescribe.wDataSize<=sizeof(UserInfo.szAddrDescribe))
				{
					CopyMemory(&UserInfo.szAddrDescribe,pDataBuffer,DataDescribe.wDataSize);
					UserInfo.szAddrDescribe[CountArray(UserInfo.szAddrDescribe)-1]=0;
				}

				break;
			}
		}
	}

	//激活用户
	ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==NULL) pIClientUserItem=m_PlazaUserManagerModule->ActiveUserItem(UserInfo);

	return true;
}

//用户积分
bool CServerViewItem::OnSocketSubUserScore(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore)) return false;

	//寻找用户
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserScore->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//更新用户
	m_PlazaUserManagerModule->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);

	return true;
}

//用户状态
bool CServerViewItem::OnSocketSubUserStatus(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//寻找用户
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserStatus->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//设置状态
	if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	{
		//删除用户
		m_PlazaUserManagerModule->DeleteUserItem(pIClientUserItem);
	}
	else
	{
		//更新用户
		m_PlazaUserManagerModule->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);
	}

	return true;
}

//用户信息
bool CServerViewItem::OnSocketSubUserSpreadInfo(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserSpreadInfo));
	if (wDataSize<sizeof(CMD_GR_UserSpreadInfo)) return false;

	//寻找用户
	CMD_GR_UserSpreadInfo * pUserSpreadInfo=(CMD_GR_UserSpreadInfo *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserSpreadInfo->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//更新用户
	m_PlazaUserManagerModule->UpdateUserItemSpreadInfo(pIClientUserItem,&pUserSpreadInfo->UserSpreadInfo);

	return true;
}

//登录完成
bool CServerViewItem::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//关闭提示
	m_DlgStatus.HideStatusWindow();

	//设置状态
	m_ServiceStatus=ServiceStatus_ServiceIng;

	//激活房间
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	if (pPlatformFrame!=NULL) pPlatformFrame->ActiveServerViewItem();

	//重入判断
	if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()!=INVALID_TABLE))
	{
		//启动进程	
		bool bSuccess=m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);

		//错误处理
		if (bSuccess==false)
		{
			//中断连接
			m_TCPSocketModule->CloseSocket();
			m_ServiceStatus=ServiceStatus_NetworkDown;

			return false;
		}
	}
	OnHitTableButton(1);
	return true;
}

//登录成功
bool CServerViewItem::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_LogonSuccess));
	if (wDataSize!=sizeof(CMD_GR_LogonSuccess)) return false;

	//消息处理
	CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)pData;

	//设置状态
	m_ServiceStatus=ServiceStatus_RecvInfo;

	//玩家属性
	m_dwUserRight=pLogonSuccess->dwUserRight;
	m_dwMasterRight=pLogonSuccess->dwMasterRight;

	//关闭提示
	m_DlgStatus.ShowStatusWindow(TEXT("正在读取房间信息..."));

	return true;
}

//登录失败
bool CServerViewItem::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//效验参数
	CMD_GR_LogonFailure * pLogonFailure=(CMD_GR_LogonFailure *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//关闭提示
	m_DlgStatus.HideStatusWindow();

	//关闭处理
	m_TCPSocketModule->CloseSocket();

	//设置状态
	m_ServiceStatus=ServiceStatus_NetworkDown;

 	//显示消息
 	LPCTSTR pszDescribeString=pLogonFailure->szDescribeString;
 	if (lstrlen(pszDescribeString)>0) ShowInformation(pszDescribeString,MB_ICONERROR,60);

	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//更新提示
bool CServerViewItem::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GR_UpdateNotify)) return false;

	//变量定义
	CMD_GR_UpdateNotify * pUpdateNotify=(CMD_GR_UpdateNotify *)pData;

	//大厅更新
	if (pUpdateNotify->cbMustUpdatePlaza==TRUE)
	{
		//关闭处理
		m_DlgStatus.HideStatusWindow();
		m_TCPSocketModule->CloseSocket();

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//下载大厅
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("游戏广场"),0,0);

		return true;
	}

	//游戏更新
	if ((pUpdateNotify->cbMustUpdateClient==TRUE)||(pUpdateNotify->cbAdviceUpdateClient==TRUE))
	{
		//关闭处理
		if (pUpdateNotify->cbMustUpdateClient==TRUE)
		{
			m_DlgStatus.HideStatusWindow();
			m_TCPSocketModule->CloseSocket();
		}

		//构造提示
		if (pUpdateNotify->cbMustUpdateClient==FALSE)
		{
			//构造提示
			TCHAR szDescribe[512]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("“%s”已经更新了，我们强烈建议您进行更新，现在进行更新吗？"),m_GameKind.szKindName);

			//提示消息
			CInformation Information(this);
			if (Information.ShowMessageBox(szDescribe,MB_ICONINFORMATION|MB_YESNO,0)!=IDYES) return true;
		}

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//更新提示
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
	}

	return true;
}

//请求失败
bool CServerViewItem::OnSocketSubRequestFailure(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)pData;

	//效验参数
	ASSERT(wDataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	if (wDataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	//关闭处理
	if(pRequestFailure->cbCommandCode==REQUEST_FAILURE_CMD_CONCLUDE)
		m_ProcessManager.CloseGameProcess(false);

	//提示消息
	if (pRequestFailure->szDescribeString[0]!=0)
	{
		//提示消息
		CInformation Information;
		Information.ShowMessageBox(pRequestFailure->szDescribeString,MB_ICONINFORMATION,30L);
	}

	return true;
}

//系统消息
bool CServerViewItem::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR)))) return false;

	//关闭处理
	if ((pSystemMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭处理
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//状态设置
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//弹出消息
	if (pSystemMessage->wType&SMT_EJECT)
	{
		CInformation Information;
		Information.ShowMessageBox(pSystemMessage->szString,MB_ICONINFORMATION);
	}

	//关闭房间
	if (pSystemMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//动作消息
bool CServerViewItem::OnSocketSubActionMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize>=(wHeadSize+pActionMessage->wLength*sizeof(WCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize<(wHeadSize+pActionMessage->wLength*sizeof(WCHAR)))) return false;

	//关闭处理
	if ((pActionMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭处理
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//状态设置
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//弹出消息
	CInformation Information;
	INT nResultCode=Information.ShowMessageBox(pActionMessage->szString,MB_ICONINFORMATION);

	//变量定义
	WORD wExcursion=wHeadSize+pActionMessage->wLength*sizeof(TCHAR);

	//提取动作
	while (wExcursion<wDataSize)
	{
		//变量定义
		tagActionHead * pActionHead=(tagActionHead *)((BYTE *)pData+wExcursion);

		//效验参数
		ASSERT((wExcursion+sizeof(tagActionHead))<=wDataSize);
		ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=wDataSize);

		//效验参数
		if ((wExcursion+sizeof(tagActionHead))>wDataSize) return false;
		if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>wDataSize) return false;

		//动作出来
		if (nResultCode==pActionHead->uResponseID)
		{
			switch (pActionHead->cbActionType)
			{
			case ACT_BROWSE:	//浏览动作
				{
					//变量定义
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionBrowse * pActionBrowse=(tagActionBrowse *)((BYTE *)pData+wDataPos);

					//I E 浏览
					if (pActionBrowse->cbBrowseType&BRT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionBrowse->szBrowseUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			case ACT_DOWN_LOAD:	//下载动作
				{
					//变量定义
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((BYTE *)pData+wDataPos);

					//I E 下载
					if (pActionDownLoad->cbDownLoadMode&DLT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					//下载模块
					if (pActionDownLoad->cbDownLoadMode&DLT_MODULE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			}
		}

		//增加偏移
		wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	}

	//关闭房间
	if (pActionMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//调整控件
VOID CServerViewItem::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//环绕控件
	m_TableViewFrame.SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOZORDER);

	//重画界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

	return;
}

//配置房间
VOID CServerViewItem::InitServerViewItem(CGameServerItem * pGameServerItem)
{
	//设置状态
	m_ServiceStatus=ServiceStatus_Entering;

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//房间属性
	CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
	CopyMemory(&m_GameKind,&pGameKindItem->m_GameKind,sizeof(m_GameKind));
	CopyMemory(&m_GameServer,&pGameServerItem->m_GameServer,sizeof(m_GameServer));

	//关闭判断
	ASSERT(m_GameServer.wServerID!=0);
	if (m_GameServer.wServerID==0) throw TEXT("很抱歉，此游戏房间已经关闭了，不允许继续进入！");

	//创建组件
	if (m_TCPSocketModule.CreateInstance()==false) throw TEXT("无法创建网络连接组件，游戏房间进入失败！");

	//设置网络
	m_TCPSocketModule->SetTCPSocketSink(QUERY_ME_INTERFACE(IUnknownEx));
	m_TCPSocketModule->SetProxyServerInfo(pParameterGlobal->m_cbProxyType,pParameterGlobal->m_ProxyServer);

	//发起连接
	if (m_TCPSocketModule->Connect(m_GameServer.szServerAddr,m_GameServer.wServerPort)!=CONNECT_SUCCESS)
	{
		throw TEXT("游戏房间连接失败，请检查系统网络配置参数是否正确！");
		return;
	}

	//创建提示
	m_DlgStatus.ShowStatusWindow(TEXT("正在进入游戏房间，请稍后..."));

	return;
}

//资源目录
VOID CServerViewItem::GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount)
{
	//变量定义
	WORD wStringIndex=0;

	//构造目录
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,m_GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//字符终止
	szResDirectory[wStringIndex]=0;

	return;
}
BOOL CServerViewItem::GetHostIP(TCHAR * inData)  
{  
	char name[255];  
	if( gethostname ( name, sizeof(name)) == 0)  
	{  
		hostent* pHostent = gethostbyname(name);  
		hostent& he = *pHostent;  
		sockaddr_in sa;  
		for (int nAdapter=0; he.h_addr_list[nAdapter]; nAdapter++)   
		{  
			memcpy ( &sa.sin_addr.s_addr, he.h_addr_list[nAdapter],he.h_length);  
			CopyMemory(inData,inet_ntoa(*(struct in_addr *)he.h_addr_list[nAdapter]),strlen(name));  

		}       
	}   
	return TRUE;  
}
//发送登录
bool CServerViewItem::SendLogonPacket()
{
	//变量定义
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//变量定义
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//游戏版本
	LPCTSTR pszProcessName=m_GameKind.szProcessName;
	CWHService::GetModuleVersion(pszProcessName,LogonUserID.dwProcessVersion);

	//附加信息
	LogonUserID.dwPlazaVersion=pGlobalUnits->GetPlazaVersion();
	LogonUserID.dwFrameVersion=pGlobalUnits->GetFrameVersion();

	//登录信息
	LogonUserID.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(LogonUserID.szPassword,pGlobalUserData->szPassword,CountArray(LogonUserID.szPassword));

	TCHAR szIPAddr[255]=TEXT("");
	URLDownloadToFile(0,L"http://www.ip138.com/ips1388.asp",L"c://1.txt",0,NULL);

	char* pbuf = NULL;
	CString strBuf;
	CFile fp;
//	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
//	setlocale( LC_CTYPE, "chs" );
	if(fp.Open( L"c://1.txt", CFile::modeRead|CFile::typeText))
	{
		int nCount = fp.GetLength() ;
		pbuf = new char[nCount+1] ;
		pbuf[nCount] = '\0' ;
		fp.Read(pbuf,nCount);
		fp.Close();

		strBuf = pbuf;

		if (NULL != pbuf )
		{
			delete[] pbuf ;

			pbuf = NULL ;
		}
		int nLen = 0;

		nLen = strBuf.Find(L"地址是");
		strBuf = strBuf.Mid(nLen,nCount-nLen);

		int nLen1 = strBuf.Find(L"[");
		int nLen2 = strBuf.Find(L"]");
		strBuf=strBuf.Mid(nLen1+1,nLen2-nLen1-1);
		DeleteFile(L"c://1.txt");
	}

	CWHService::GetMachineID(LogonUserID.szMachineID);

	memcpy(LogonUserID.szIPAdrr,strBuf.GetBuffer(strBuf.GetLength()),CountArray(LogonUserID.szIPAdrr));
	strBuf.ReleaseBuffer();
	//广场类型
#ifdef	RELEASE_MANAGER
	LogonUserID.cbPlazaType = PLAZE_TYPE_MULTI;
#else
	LogonUserID.cbPlazaType = PLAZA_TYPE_NORMAL;
#endif

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//发送旁观
bool CServerViewItem::SendLookonPacket(WORD wTableID, WORD wChairID)
{
	//变量定义
	CMD_GR_UserLookon UserLookon;
	ZeroMemory(&UserLookon,sizeof(UserLookon));

	//构造数据
	UserLookon.wTableID=wTableID;
	UserLookon.wChairID=wChairID;

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_LOOKON,&UserLookon,sizeof(UserLookon));

	return true;
}

//发送坐下
bool CServerViewItem::SendSitDownPacket(WORD wTableID, WORD wChairID)
{
	//变量定义
	CMD_GR_UserSitDown UserSitDown;
	ZeroMemory(&UserSitDown,sizeof(UserSitDown));

	//构造数据
	UserSitDown.wTableID=wTableID;
	UserSitDown.wChairID=wChairID;

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&UserSitDown,sizeof(UserSitDown));

	return true;
}

//发送起立
bool CServerViewItem::SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
	//变量定义
	CMD_GR_UserStandUp UserStandUp;
	ZeroMemory(&UserStandUp,sizeof(UserStandUp));

	//构造数据
	UserStandUp.wTableID=wTableID;
	UserStandUp.wChairID=wChairID;
	UserStandUp.cbForceLeave=cbForceLeave;

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP,&UserStandUp,sizeof(UserStandUp));

	return true;
}

//执行旁观
bool CServerViewItem::PerformLookonAction(WORD wTableID, WORD wChairID)
{
	//效验数据
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;

	//自己状态
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

		return false;
	}

	//权限判断
	if (CUserRight::CanLookon(m_dwUserRight)==false)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("抱歉，您暂时没有旁观游戏的权限！"));

		return false;
	}

	//发送命令
	SendLookonPacket(wTableID,wChairID);

	return true;
}

//执行起立
bool CServerViewItem::PerformStandUpAction(WORD wTableID, WORD wChairID)
{
	//效验数据
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;

	//自己状态
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

		return false;
	}

	//发送命令
	SendStandUpPacket(wTableID,wChairID,FALSE);

	return true;
}

//执行坐下
bool CServerViewItem::PerformSitDownAction(WORD wTableID, WORD wChairID)
{
	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;

	//自己状态
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

		return false;
	}

	//权限判断
	if(CheckUserRight()==false) return false;
	

	//发送命令
	SendSitDownPacket(wTableID,wChairID);

	return true;
}

//权限检查
bool CServerViewItem::CheckUserRight()
{
	//权限判断
	if (CUserRight::CanPlay(m_dwUserRight)==false)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("抱歉，您暂时没有加入游戏的权限！"));

		return false;
	}

	return true;
}

//销毁消息
VOID CServerViewItem::OnNcDestroy()
{
	__super::OnNcDestroy();

	//界面变量
	m_bCreateFlag=false;

	//用户属性
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//房间属性
	m_wServerType=0L;
	m_dwServerRule=0L;
	m_bAllowDistribute=FALSE;
	m_ServiceStatus=ServiceStatus_Unknow;

	//配置信息
	m_cbColumnCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));

	//房间属性
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	return;
}

//绘画背景
BOOL CServerViewItem::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//位置消息
VOID CServerViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
