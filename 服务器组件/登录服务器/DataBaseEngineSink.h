#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"
#include <string>
using		namespace		std;

//////////////////////////////////////////////////////////////////////////////////

//数据库类
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//友元定义
	friend class CServiceUnits;

	//用户数据库
protected:
	CDataBaseAide					m_AccountsDBAide;					//用户数据库
	CDataBaseHelper					m_AccountsDBModule;					//用户数据库
	//金币数据库
protected:
	CDataBaseAide					m_TreasureDBAide;					//金币数据库
	CDataBaseHelper					m_TreasureDBModule;					//金币数据库

	//平台数据库
protected:
	CDataBaseAide					m_PlatformDBAide;					//平台数据库
	CDataBaseHelper					m_PlatformDBModule;					//平台数据库

	//游戏开奖数据库
protected:
	CDataBaseAide					m_GameSSCDBAide;					//平台数据库
	CDataBaseHelper					m_GameSSCDBModule;					//平台数据库

	//组件变量
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//数据事件

	//函数定义
public:
	//构造函数
	CDataBaseEngineSink();
	//析构函数
	virtual ~CDataBaseEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//系统事件
public:
	//启动事件
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//内核事件
public:
	//时间事件
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//控制事件
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	CString		ChangeStringToT(CStringA strInput);
	//帐号登录
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏投注
	bool OnTouzhuCQSSC(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//客服链接
	bool OnGetKefuUrl(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//校验号码
	bool CheckTouzhuNum(CString strHaoma,int nLength,int nUserID,CString strQihao);
	//游戏投注单
	bool OnTouzhuCQSSCDan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏投注追号
	bool OnTouzhuCQSSCZhuihao(DWORD dwContextID, VOID * pData, WORD wDataSize);
	CString AddDouHao(CString strHaoma); //标准化格式
	//手机投注追号
	bool OnMBTouzhuCQSSCZhuihao(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取投注数
	int GetTouZhuZongShu(int nTypeID,int nKindID,CString strHaoma);
	//获取时时彩投注数
	int GetSSCZhushu(int nTypeID,int nKindID,CString strHaoma);
	//获取11选5投注数
	int Get11X5Zhushu(int nTypeID,int nKindID,CString strHaoma);
	//获取3D投注数
	int Get3DZhushu(int nTypeID,int nKindID,CString strHaoma);
	//获取七星彩投注数
	int GetQXCZhushu(int nTypeID,int nKindID,CString strHaoma);
	//获取PK10投注数
	int GetPK10Zhushu(int nTypeID,int nKindID,CString strHaoma);
	//获取快乐8投注数
	int GetKL8Zhushu(int nTypeID,int nKindID,CString strHaoma);
	//获取幸运28投注数
	int GetXY28Zhushu(int nTypeID,int nKindID,CString strHaoma);
	//登录服务
protected:
	//帐号注册
	bool OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//获取彩票用户信息
	bool OnGetCPUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//手机登录
protected:
	//帐号登录
	bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏开奖查询
	bool OnQueryGameResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取系统时间
	bool OnQuerySystemTime(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取中奖用户
	bool OnQueryWinUserID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取MAPBONUS
	bool OnQueryMapBonus(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询玩家信息
	bool OnQueryCpUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取玩家bonus
	bool OnGetMyMapBonus(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询玩家返点
	bool OnGetUserFandian(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置玩家返点
	bool OnSetUserBonus(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//投注记录数量查询
	bool OnGetTouzhuLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提现记录数量查询
	bool OnGetTixianLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//充值记录数量查询
	bool OnGetChongzhiLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//会员信息记录数量查询
	bool OnGetHYXXLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//盈亏记录数量查询
	bool OnGetYingkuiLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//盈亏明细数量查询
	bool OnGetYingkuiMxCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//棋牌盈亏数量查询
	bool OnGetQiPaiYingkuiCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级投注数量查询
	bool OnGetXJTZHLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级投注数量查询
	bool OnCHKXJTZHLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级盈亏数量查询
	bool OnXJYKLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级盈亏统计数量查询
	bool OnXJYKTjCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级游戏盈亏统计数量查询
	bool OnXJYXTjCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级充值日志数量查询
	bool OnXJCHZHLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级提现日志数量查询
	bool OnXJTxLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询充值信息
	bool OnGetChongzhiXinxi(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询取款信息
	bool OnGetQukuanInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询取款信息
	bool OnQueryYinHangName(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询余额信息
	bool OnGetYueInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询更多开奖记录
	bool OnGetMoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询代理回馈
	bool OnGetDailiHuikui(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询代理回馈
	bool OnDailiLingjiang(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//退出游戏
	bool OnQuitGame(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//签到
	bool OnUserQiandao(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//欢乐送
	bool OnGetUserHuanlesong(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//领奖
	bool OnGetUserLingJiang(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//取款
	bool OnDoQukuan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//投注记录查询
	bool OnGetTouzhuLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级充值日志查询
	bool OnGetXJCHZHLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级提现日志查询
	bool OnGetXJTxLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下家盈亏记录查询
	bool OnGetXJYKLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下家盈亏统计查询
	bool OnGetXJYKTj(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下家游戏盈亏统计查询
	bool OnGetXJYXTj(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取大礼包
	bool OnGetDaLiBao(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取砸金蛋
	bool OnGetZaJinDan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取幸运转盘
	bool OnGetLuckyZhuan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取大礼包资格
	bool OnCanDaLiBao(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取砸金蛋资格
	bool OnCanZaJinDan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取幸运转盘资格
	bool OnCanLuckyZhuan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级投注记录查询
	bool OnGetXJTZHLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级投注记录查询
	bool OnCHKXJTZHLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级投注记录通过账户查询
	bool OnGetXJTZHLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级提现通过账户查询
	bool OnGetXJTxLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级投注记录通过ID查询
	bool OnGetXJTZHLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级提现记录通过ID查询
	bool OnGetXJTXLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级投注记录通过账户查询
	bool OnCHKXJTZHLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级盈亏记录通过ID查询
	bool OnGetXJYKLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级盈亏记录通过账户查询
	bool OnGetXJYKLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//通过ID查询下级充值日志
	bool OnGetXJCHZHLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//通过账户查询下级充值日志
	bool OnGetXJCHZHLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级盈亏统计通过ID查询
	bool OnGetXJYKTjByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级盈亏统计通过账户查询
	bool OnGetXJYKTjByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//下级投注记录通过ID查询
	bool OnCHKXJTZHLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取会员数据
	bool OnGetHyShj(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提现记录查询
	bool OnGetTixianLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//会员信息记录查询
	bool OnGetHYXXLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//会员信息记录查询
	bool OnGetHYXXList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//会员信息记录查询
	bool OnGetHYXXLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//会员信息记录查询
	bool OnGetHYXXLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改会员返点
	bool OnXGHYFandian(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置为代理
	bool OnSWDaili(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//会员信息转账
	bool OnHYXXZhuanZhang(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取下级配额
	bool OnHYXXXiaJiPeie(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置下级配额
	bool OnHYXXSetXiaJiPeie(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//充值记录查询
	bool OnGetChongzhiLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//盈亏记录查询
	bool OnGetYingkuiLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//盈亏明细查询
	bool OnGetYingkuiMx(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//棋牌盈亏明细查询
	bool OnGetQiPaiYingkui(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询注册链接
	bool OnGetRegUrl(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询配额
	bool OnGetPeie(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//撤单
	bool OnCancelTouzhu(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//添加会员
	bool OnAddHuiyuan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置网页注册返点
	bool OnSetWebFandian(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//充值提示
	bool OnChongzhiTishi(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提现提示
	bool OnTixianTishi(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//投注提示
	bool OnTouzhuTishi(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//获取玩家信息
	bool OnGetUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取新闻
	bool OnGetNewsInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//转换
	bool OnZhuanhuan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改QQ
	bool OnXGqq(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改性别
	bool OnXGGender(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改登录密码
	bool OnXGLoginPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改取款密码
	bool OnXGQukuanPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//锁定机器
	bool OnLockMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置取款保护
	bool OnSetQukuanProtectPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置取款账户
	bool OnSetQukuanZhanghuPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询我的取款账户
	bool OnQueryMyYinHang(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询我的账户保护
	bool OnQueryMyProtect(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//投注详细信息
	bool OnGetTouzhuXX(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//手机投注详细信息
	bool OnMBGetTouzhuXX(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取银行
	bool OnQueryYinhang(DWORD dwContextID, VOID * pData, WORD wDataSize);
	bool	ChangeStringToA(CString strInput, string&  strOutput);

	//账号服务
protected:
	//修改机器
	bool OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改密码
	bool OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改密码
	bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改头像
	bool OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改资料
	bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//银行服务
protected:
	//存入金币
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提取金币
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//转账金币
	bool OnRequestUserTransScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取六合彩期号
	bool OnRequestLhcQihao(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//信息查询
protected:
	//查询资料
	bool OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询银行
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//系统功能
protected:
	//加载列表
	bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//在线信息
	bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//虚拟在线
	bool OnRequestVirtualUserCount(DWORD dwContextID, VOID * pData, WORD wDataSize);

	void ReconnectSql();
	//结果处理
protected:
	//登录结果
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//银行结果
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//操作结果
	VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
};

//////////////////////////////////////////////////////////////////////////////////

#endif