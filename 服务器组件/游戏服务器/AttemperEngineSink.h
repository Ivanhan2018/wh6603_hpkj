#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

// add by hxh 20160708
#define USE_RS_PRINT

#include "Stdafx.h"
#include "TableFrame.h"
#include "InitParameter.h"
#include "ServerListManager.h"
#include "DistributeManager.h"

//////////////////////////////////////////////////////////////////////////////////

//连接类型
#define CLIENT_KIND_FALSH			1									//网页类型
#define CLIENT_KIND_MOBILE			2									//手机类型
#define CLIENT_KIND_COMPUTER		3									//电脑类型

//绑定参数
struct tagBindParameter
{
	//连接属性
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwActiveTime;						//激活时间

	//视图模式
	WORD							wVisibleMode;						//视图模式
	WORD							wVisibleTableID;					//桌子号码
	WORD							wVisibleTableRand;					//桌子范围

	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本

	//用户属性
	BYTE							cbClientKind;						//连接类型
	IServerUserItem *				pIServerUserItem;					//用户接口
};

//用户状况
struct tagAndroidStatus
{
	WORD							wFreeUserCount;						//用户数目
	WORD							wPlayUserCount;						//用户数目
	WORD							wSitdownUserCount;					//用户数目
	IServerUserItem *				pServerUserFree[MAX_ANDROID];		//用户接口
	IServerUserItem *				pServerUserPlay[MAX_ANDROID];		//用户接口
	IServerUserItem *				pServerUserSitdown[MAX_ANDROID];	//用户接口
};

//桌子状况
struct tagTableStatusInfo
{
	WORD							wMinUserCount;						//最少人数
	WORD							wTableUserCount;					//用户数目
	WORD							wTableReadyCount;					//准备数目
	WORD							wTableAndroidCount;					//用户数目
};

////系统消息<by hxh>
//struct tagSystemMessage
//{
//	DWORD							dwLastTime;						   //发送时间
//	DBR_GR_SystemMessage            SystemMessage;                     //系统消息
//};

//数组说明
typedef CWHArray<CTableFrame *>		CTableFrameArray;					//桌子数组
//typedef CList<tagSystemMessage *>   CSystemMessageList;                 //系统消息<by hxh>
//////////////////////////////////////////////////////////////////////////////////

//调度钩子
class CAttemperEngineSink : public IAttemperEngineSink, public IMainServiceFrame,
	public IServerUserItemSink
{
	//友元定义
	friend class CServiceUnits;

	//状态变量
protected:
	bool							m_bCollectUser;						//汇总标志
	bool							m_bNeekCorrespond;					//协调标志

	//绑定信息
protected:
	tagBindParameter *				m_pNormalParameter;					//绑定信息
	tagBindParameter *				m_pAndroidParameter;				//绑定信息
	//TCHAR szServerPort[124] ;
	TCHAR szServerAddr[124] ;
	//配置信息
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	tagGameParameter *				m_pGameParameter;					//配置参数
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//服务属性
	tagGameServiceOption *			m_pGameServiceOption;				//服务配置

	//配置数据
protected:
	CMD_GR_ConfigColumn				m_DataConfigColumn;					//列表配置

	//组件变量
protected:
	CTableFrameArray				m_TableFrameArray;					//桌子数组
	CDistributeManager              m_DistributeManage;                 //分组管理

	//组件变量
protected:
	CServerListManager				m_ServerListManager;				//列表管理
	CServerUserManager				m_ServerUserManager;				//用户管理
	CAndroidUserManager				m_AndroidUserManager;				//机器管理
    //CSystemMessageList              m_SystemMessageList;                //系统消息<by hxh>

	//组件接口
protected:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	IAttemperEngine *				m_pIAttemperEngine;					//调度引擎
	ITCPSocketService *				m_pITCPSocketService;				//网络服务
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//网络引擎
	IGameServiceManager *			m_pIGameServiceManager;				//服务管理

	//数据引擎
protected:
	IDataBaseEngine *				m_pIRecordDataBaseEngine;			//数据引擎
	IDataBaseEngine *				m_pIKernelDataBaseEngine;			//数据引擎

	//函数定义
public:
	//构造函数
	CAttemperEngineSink();
	//析构函数
	virtual ~CAttemperEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//异步接口
public:
	//启动事件
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//事件接口
public:
	//控制事件
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//自定事件
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//内核事件
public:
	//时间事件
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//连接事件
public:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//网络事件
public:
	//应答事件
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//关闭事件
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//读取事件
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//功能接口
public:
	//插入用户
	virtual bool InsertDistribute(IServerUserItem * pIServerUserItem,bool bCheckPlazaType=true);

	//消息接口
public:
	//房间消息
	virtual bool SendRoomMessage(LPCTSTR lpszMessage, WORD wType);
	//游戏消息
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType);
	//房间消息
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);
	//游戏消息
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);

	//网络接口
public:
	//发送数据
	virtual bool SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//发送数据
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//发送数据
	virtual bool SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//用户接口
public:
	//用户积分
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem);
	//用户状态<by hxh 20160624>
#if 0
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem);
#else
    virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID=INVALID_TABLE, WORD wOldChairID=INVALID_CHAIR);
#endif
	//用户信息
	virtual bool OnEventUserItemSpreadInfo(IServerUserItem * pIServerUserItem);

	//数据事件
protected:
	//登录成功
	bool OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//登录失败
	bool OnDBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//配置信息
	bool OnDBGameParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//列表信息
	bool OnDBGameColumnInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//机器信息
	bool OnDBGameAndroidInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行信息
	bool OnDBUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行成功
	bool OnDBUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行失败
	bool OnDBUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询成功
	bool OnDBQueryIpDescribeSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//连接处理
protected:
	//注册事件
	bool OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//列表事件
	bool OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//汇总事件
	bool OnTCPSocketMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//网络事件
protected:
	//用户处理
	bool OnTCPNetworkMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//登录处理
	bool OnTCPNetworkMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//游戏处理
	bool OnTCPNetworkMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//框架处理
	bool OnTCPNetworkMainFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//银行处理
	bool OnTCPNetworkMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//管理处理
	bool OnTCPNetworkMainManage(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//网络事件
protected:
	//I D 登录
	bool OnTCPNetworkSubLogonUserID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//手机登录
	bool OnTCPNetworkSubLogonMobile(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号登录
	bool OnTCPNetworkSubLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//用户命令
protected:
	//用户旁观
	bool OnTCPNetworkSubUserLookon(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户坐下
	bool OnTCPNetworkSubUserSitDown(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户起立
	bool OnTCPNetworkSubUserStandUp(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户聊天
	bool OnTCPNetworkSubUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户私聊
	bool OnTCPNetworkSubWisperChat(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户表情
	bool OnTCPNetworkSubUserExpression(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户表情
	bool OnTCPNetworkSubWisperExpression(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//请求更换位置<by hxh>
	bool OnTCPNetworkSubUserChairReq(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//请求椅子用户信息<by hxh>
	bool OnTCPNetworkSubChairUserInfoReq(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//银行命令
protected:
	//查询银行
	bool OnTCPNetworkSubQueryInsureInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//存款请求
	bool OnTCPNetworkSubSaveScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//取款请求
	bool OnTCPNetworkSubTakeScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//转账请求
	bool OnTCPNetworkSubTransferScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//管理命令
protected:
	//查询设置
	bool OnTCPNetworkSubQueryOption(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//房间设置
	bool OnTCPNetworkSubOptionServer(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	////发布消息<by hxh>
	//bool OnTCPNetworkSubSendMessage(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//内部事件
protected:
	//用户登录
	VOID OnEventUserLogon(IServerUserItem * pIServerUserItem);
	//用户登出
	VOID OnEventUserLogout(IServerUserItem * pIServerUserItem, DWORD dwLeaveReason);

	//执行功能
protected:
	//分配用户
	bool PerformDistribute();
	//玩家坐下
	bool PerformUserSitDown(IServerUserItem * pIServerUserItem,WORD wRequestTableID,WORD wRequestChairID );
	//玩家起立
	bool PerformUserStandUp(IServerUserItem * pIServerUserItem,BOOL cbForceLeave);
	//解锁金币
	bool PerformUnlockScore(DWORD dwUserID, DWORD dwInoutIndex, DWORD dwLeaveReason);
	//版本检查
	bool PerformCheckVersion(DWORD dwPlazaVersion, DWORD dwFrameVersion, DWORD dwClientVersion, DWORD dwSocketID,bool IsMobileUser);
	//切换连接
	bool SwitchUserItemConnect(IServerUserItem * pIServerUserItem, TCHAR szMachineID[LEN_MACHINE_ID], WORD wTargetIndex);

	//发送函数
protected:
	//用户信息
	bool SendUserInfoPacket(IServerUserItem * pIServerUserItem, DWORD dwSocketID);

	//辅助函数
protected:
	//登录失败
	bool SendLogonFailure(LPCTSTR pszString, LONG lErrorCode, DWORD dwSocketID);
	//银行失败
	bool SendInsureFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode);
	//请求失败
	bool SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, BYTE cbFailureCode,BYTE cbCommandCode);

	//辅助函数
protected:
	//用户状况
	WORD GetAndroidStatus(tagAndroidStatus & AndroidStatus);
	//桌子状况
	WORD GetTableStatusInfo(CTableFrame * pTableFrame, tagTableStatusInfo & TableStatusInfo);
	//最少人数
	WORD GetGameMinUserCount();
	//调整参数
	VOID RectifyDistributeParameter();

	//辅助函数
protected:
	//配置机器
	bool InitAndroidUser();
	//配置桌子
	bool InitTableFrameArray();
	//发送请求
	bool SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize);

	
	//群发数据<by hxh>
	virtual bool SendDataBatchToMobileUser(WORD wCmdTable, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//群发用户信息<by hxh>
	bool SendUserInfoPacketBatchToMobileUser(IServerUserItem * pIServerUserItem);
	//发可视用户信息<by hxh>
	bool SendViewTableUserInfoPacketToMobileUser(IServerUserItem * pIServerUserItem,DWORD dwUserIDReq);
	////发送系统消息<by hxh>
	//bool SendSystemMessage(CMD_GR_SendMessage * pSendMessage, WORD wDataSize);

	//辅助函数
public:
	//绑定用户
	IServerUserItem * GetBindUserItem(WORD wBindIndex);
	//绑定参数
	tagBindParameter * GetBindParameter(WORD wBindIndex);

};

//////////////////////////////////////////////////////////////////////////////////

#endif