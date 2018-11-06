#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerListManager.h"
#include "DataBaseEngineSink.h"
//////////////////////////////////////////////////////////////////////////////////
//数据定义

//连接类型
#define CLIENT_KIND_FALSH			1									//网页类型
#define CLIENT_KIND_MOBILE			2									//手机类型
#define CLIENT_KIND_COMPUTER		3									//电脑类型

//开奖信息
#define	KJ_QIHAO_LENGTH	30
#define	KJ_HAOMA_LENGTH	45
#define	KJ_SHIJIAN_LENGTH	30

struct tagLuckNumXinXi
{
	WORD  wKindID;
	TCHAR qihao[KJ_QIHAO_LENGTH];
	TCHAR haoma[KJ_HAOMA_LENGTH];
	TCHAR shijian[KJ_SHIJIAN_LENGTH];
};
struct tagLuckNum
{
	tagLuckNumXinXi LuckNum[5];
};
//绑定参数
struct tagBindParameter
{
	//网络参数
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwActiveTime;						//激活时间

	//连接信息
	WORD							wModuleID;							//模块标识
	BYTE							cbClientKind;						//连接类型
	DWORD							dwPlazaVersion;						//大厅版本
};


//////////////////////////////////////////////////////////////////////////////////
#include <map>


//调度钩子
class CAttemperEngineSink : public IAttemperEngineSink
{
	//友元定义
	friend class CServiceUnits;
private:
	CTimeSpan						m_timespan ;						//数据库和服务器的时间差
	map<int,DWORD>							m_dwLuckyNumTickCount;
	int								m_nLuckNumID;
	//状态变量
protected:
	bool							m_bNeekCorrespond;					//协调标志
	bool                            m_bShowServerStatus;                //显示服务器状态
	//变量定义
protected:
	tagBindParameter *				m_pBindParameter;					//辅助数组
//	AllLuckNum						m_AllLuckNum;
	tagLuckNum						m_MyLuckNum[30] ;					//缓存
	CString							m_strNewsTime;						//新闻时间
	//组件变量
protected:
	CServerListManager				m_ServerListManager;				//列表管理

	//组件接口
protected:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	IDataBaseEngine *				m_pIDataBaseEngine;					//数据引擎
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//网络引擎
	ITCPSocketService *				m_pITCPSocketService;				//协调服务

	//函数定义
public:
	CInitParameter *				m_pInitParameter;					//配置参数
	CDataBaseEngineSink	*			m_DataBaseEngineSink[4];
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
	CString ChangeStringToT(CStringA strInput);
	//网络事件
public:
	//应答事件
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//关闭事件
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//读取事件
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//连接处理
protected:
	bool OnGetSqlserverTimer();
	bool OnGetNews();
	bool OnGetWinUserID();
	bool OnGetMapBonusRet();

	//注册事件
	bool OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//服务信息
	bool OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//远程服务
	bool OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//网络事件
protected:
	//登录处理
	bool OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//列表处理
	bool OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//服务处理
	bool OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//远程处理
	bool OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//手机事件
protected:
	//登录处理
	bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//列表处理
	bool OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	bool GetUserYueInfo(int nUserID,DWORD dwSocketID);
	//网络事件
protected:
	//帐号登录
	bool OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号注册
	bool OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//获取彩票用户信息
	bool OnTCPNetworkSubPCGetCpUserInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//手机事件
protected:
	//帐号登录
	bool OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号注册
	bool OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//游戏事件
protected:
	//登录成功
	bool OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//登录失败
	bool OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//用户头像
	bool OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//用户信息
	bool OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行信息
	bool OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行成功
	bool OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行失败
	bool OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//操作成功
	bool OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//操作失败
	bool OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取彩票用户信息
	bool OnGetCpUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取彩票用户信息
	bool OnGetMyUserMapBonus(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取彩票用户返点信息
	bool OnGetUserFandianRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置彩票用户返点信息
	bool OnSetUserBonusRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取MAPBONUS信息
	bool OnGetMapBonusRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取客服URL
	bool OnGetKefuUrlRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//系统时间返回
	bool OnGetSysTimeRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取中奖用户ID返回
	bool OnGetWinUserIDRet(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//手机事件
protected:
	//登录成功
	bool OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//登录失败
	bool OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
protected:
	//查询游戏结果返回
	bool OnDBQueryOpenResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询银行返回
	bool OnDBQueryYinHangResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下注返回
	bool OnDBQueryTouzhuResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下注日志数量返回
	bool OnDBQueryTouzhuLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//注册链接返回
	bool OnDBQueryRegUrlResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取配额返回
	bool OnDBQueryPeieResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取下级配额返回
	bool OnDBQueryXjPeieResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置下级配额返回
	bool OnDBQuerySetXjPeieResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提现日志数量返回
	bool OnDBQueryTixianLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//充值日志数量返回
	bool OnDBQueryChongzhiLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//会员信息日志数量返回
	bool OnDBQueryHYXXLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//盈亏日志数量返回
	bool OnDBQueryYingkuiLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//盈亏明细数量返回
	bool OnDBQueryYingkuiMxCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//棋牌盈亏明细数量返回
	bool OnDBQueryQiPaiYingkuiCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级投注日志数量返回
	bool OnDBQueryXITZHLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级投注日志数量返回
	bool OnDBCHKXITZHLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级盈亏日志数量返回
	bool OnDBXJYKLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级盈亏统计数量返回
	bool OnDBXJYKTjCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级游戏盈亏统计数量返回
	bool OnDBXJYXTjCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级充值日志数量返回
	bool OnDBXJCHZHLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级提现日志数量返回
	bool OnDBXJTxLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//充值信息返回
	bool OnDBGetChongzhiXinxiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//取款信息返回
	bool OnDBGetQukuanInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//余额信息返回
	bool OnDBGetYueInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//更多开奖记录返回
	bool OnDBGetMoreRecordResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//代理回馈返回
	bool OnDBGetDailiHuikuiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//代理领奖返回
	bool OnDBDailiLingjiangResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//退出游戏返回
	bool OnDBQuitGameResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//签到返回
	bool OnDBUserQiandaoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//欢乐送返回
	bool OnDBUserHuanlesongResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//领奖返回
	bool OnDBUserLingjiangResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//取款返回
	bool OnDBDoQukuanResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下注日志返回
	bool OnDBGetTouzhuLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取新闻返回
	bool OnDBGetNewsResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//转换返回
	bool OnDBZhuanhuanResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级充值日志返回
	bool OnDBGetXJCHZHLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级提现日志返回
	bool OnDBGetXJTxLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下家盈亏日志返回
	bool OnDBGetXJYKLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下家盈亏统计返回
	bool OnDBGetXJYKTjResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下家游戏盈亏统计返回
	bool OnDBGetXJYXTjResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级下注日志返回
	bool OnDBGetXJTZHLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级下注日志返回
	bool OnDBCHKXJTZHLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提现日志返回
	bool OnDBGetTixianLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//会员信息日志返回
	bool OnDBGetHYXXLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//充值日志返回
	bool OnDBGetChongzhiLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//盈亏日志返回
	bool OnDBGetYingkuiLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//盈亏明细返回
	bool OnDBGetYingkuiMxResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//棋牌盈亏明细返回
	bool OnDBGetQiPaiYingkuiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//撤单
	bool OnDBCancelTouzhuResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//添加会员返回
	bool OnDBAddHuiyuanResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取六合彩期号返回
	bool OnDBGetLhcQihaoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置网页注册返点返回
	bool OnDBSetWebFandianResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//玩家信息返回
	bool OnDBGetUserInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改QQ返回
	bool OnDBXGqqResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改性别返回
	bool OnDBXGGenderResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改会员返点返回
	bool OnDBXGHYFandianResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设为代理返回
	bool OnDBSzhidailiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//会员信息转账返回
	bool OnDBHyxxZhuanZhangResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改QQ返回
	bool OnDBXGLoginPassResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//锁定机器返回
	bool OnDBLockMachineResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改取款密码返回
	bool OnDBXGQukuanPassResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置取款保护返回
	bool OnDBSetQukuanProtectResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取我的银行账户返回
	bool OnDBQueryMyYinhangResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取我的账号保护返回
	bool OnDBQueryMyProtectResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置取款账户返回
	bool OnDBSetQukuanZhanghaoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取投注详细信息返回
	bool OnDBGetTouzhuXXResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取手机投注详细信息返回
	bool OnDBMBGetTouzhuXXResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取会员数据返回
	bool OnDBGetHyShjResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取大礼包返回
	bool OnDBGetDaLiBaoRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取砸金蛋返回
	bool OnDBGetZaJinDanRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取幸运转盘返回
	bool OnDBGetLuckyZhuanRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取大礼包资格返回
	bool OnDBCanDaLiBaoRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取大礼包资格返回
	bool OnDBCanZaJinDanRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取幸运转盘资格返回
	bool OnDBCanLuckyZhuanRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//充值提示返回
	bool OnDBChongzhiTishiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//提现提示返回
	bool OnDBTixianTishiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//投注提示返回
	bool OnDBTouzhuTishiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//列表事件
protected:
	//游戏种类
	bool OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏类型
	bool OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//虚拟人数
	bool OnDBPCLoadVirtualCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏列表
	bool OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//辅助函数
protected:
	//版本检测
	bool CheckPlazaVersion(DWORD dwPlazaVersion, DWORD dwSocketID);
	//发送请求
	bool SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize);

	//发送列表
protected:
	//发送类型
	VOID SendGameTypeInfo(DWORD dwSocketID);
	//发送种类
	VOID SendGameKindInfo(DWORD dwSocketID);
	//发送房间
	VOID SendGameServerInfo(DWORD dwSocketID);

	//手机列表
protected:
	//发送类型
	VOID SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID);
	//发送房间
	VOID SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID);
};

//////////////////////////////////////////////////////////////////////////////////

#endif