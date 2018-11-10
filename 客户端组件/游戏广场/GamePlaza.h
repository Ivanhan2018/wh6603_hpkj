#pragma once

//系统文件
#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "PlatformEvent.h"
#include "ServerListData.h"
#include "comm.h"
//其他文件
#include "Other\IDispimp.h"
#include "Other\Custsite.h"
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"
//////////////////////////////////////////////////////////////////////////////////
#define  WM_BN_CLICK  WM_USER + 100
//应用程序
class CGamePlazaApp : public CWinApp
{
	//变量定义
public:
	CImpIDispatch *					m_pIDispatch;						//自动接口
	CCustomOccManager *				m_pCustomOccManager;				//管理对象

	//平台数据
protected:
	CGlobalUnits					m_GlobalUnits;						//全局单元
	CGlobalUserInfo					m_GlobalUserInfo;					//用户信息
		CString					m_strCurrentDir;		//当前目录
	//全局组件
protected:
	CPlatformEvent					m_PlatformEvent;					//平台事件
	CParameterGlobal				m_ParameterGlobal;					//全局配置
	CUserItemElement				m_UserItemElement;					//用户元素
	CUserInformation				m_UserInformation;					//用户信息

	//资源组件
public:
	CSkinRenderManager				m_SkinRenderManager;				//渲染管理
	CSkinResourceManager			m_SkinResourceManager;				//资源管理

	//组件变量
public:
	CFaceItemControlHelper			m_FaceItemControlModule;			//头像组件
	CUserOrderParserHelper			m_UserOrderParserModule;			//等级组件

	//函数定义
public:
	//构造函数
	CGamePlazaApp();
	BOOL CheckVersionUpdate(CString& strUpdateURL);

	//初始化彩种信息
	void InitCaiZhong();
	bool GetSystemT();
	void GetTime(CTime& time);
	bool	ChangeStringToA(CString strInput, string&  strOutput);

	//重载函数
public:
	//退出函数
	virtual INT ExitInstance();
	//进入函数
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//程序对象
extern CGamePlazaApp theApp;

//extern CDBCmd	theDBCmd;
extern CMap<unsigned int, unsigned int, double, double> theBonus;
extern CMap<int, int, CString, CString&> theGameType;
extern CMap<int, int, CString, CString&> theGameKind;
//extern long theTimeDiff;
extern CTime theTime;
extern DWORD theBeginCount;
extern CCriticalSection theTimeCS;
extern BaseUser	theAccount;
//extern CString theKeFuUrl;

//////////////////////////////////////////////////////////////////////////////////
