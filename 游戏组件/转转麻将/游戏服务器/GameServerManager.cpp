#include "StdAfx.h"
#include "Tableframesink.h"
#include "GameServerManager.h"

//////////////////////////////////////////////////////////////////////////

//全局变量
static CGameServiceManager			g_GameServiceManager;				//管理变量

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//机器定义
#ifndef _DEBUG
	#define ANDROID_SERVICE_DLL_NAME	TEXT("SparrowZZAndroidService.dll")	//组件名字
#else
	#define ANDROID_SERVICE_DLL_NAME	TEXT("SparrowZZAndroidServiceD.dll")	//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameServiceManager::CGameServiceManager()
{
	//设置属性
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	////m_GameServiceAttrib.cbJoinInGame=FALSE;
	m_GameServiceAttrib.cbDynamicJoin=FALSE;

	m_GameServiceAttrib.wSupporType=(GAME_GENRE_GOLD|GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_EDUCATE);////////////////
	m_GameServiceAttrib.cbAndroidUser=TRUE;////////////
	m_GameServiceAttrib.cbOffLineTrustee=TRUE;/////////
	m_GameServiceAttrib.dwServerVersion=VERSION_SERVER;/////////
	m_GameServiceAttrib.dwClientVersion=VERSION_CLIENT;//////////

	lstrcpyn(m_GameServiceAttrib.szGameName,GAME_NAME,CountArray(m_GameServiceAttrib.szGameName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("QPTREASUREDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("SparrowZZ.EXE"),CountArray(m_GameServiceAttrib.szClientEXEName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("SparrowZZServer.DLL"),CountArray(m_GameServiceAttrib.szServerDLLName));

	return;
}

//析构函数
CGameServiceManager::~CGameServiceManager()
{
}

//接口查询
void *  CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

////创建游戏桌
//void *  CGameServiceManager::CreateTableFrameSink(const IID & Guid, DWORD dwQueryVer)
//{
//	//建立对象
//	CTableFrameSink * pTableFrameSink=NULL;
//	try
//	{
//		pTableFrameSink=new CTableFrameSink();
//		if (pTableFrameSink==NULL)
//			throw TEXT("创建失败");
//		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
//		if (pObject==NULL) 
//			throw TEXT("接口查询失败");
//		return pObject;
//	}
//	catch (...) {}
//
//	//清理对象
//	SafeDelete(pTableFrameSink);
//
//	return NULL;
//}
//创建游戏桌
VOID *  CGameServiceManager::CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)
{
	//建立对象
	CTableFrameSink * pTableFrameSink=NULL;
	try
	{
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("创建失败");
		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pTableFrameSink);

	return NULL;
}

//获取属性
bool  CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	GameServiceAttrib=m_GameServiceAttrib;
	return true;
}

//参数修改
bool  CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{
	//////效验参数
	////ASSERT(GameServiceOption!=NULL);
	////if (GameServiceOption==NULL) return false;

	//单元积分
	GameServiceOption.lCellScore=__max(1L,GameServiceOption.lCellScore);

	//积分下限
	if (GameServiceOption.wServerType!=GAME_GENRE_SCORE)
	{
		GameServiceOption.lMinTableScore=__max(GameServiceOption.lCellScore*20L*2L,GameServiceOption.lMinTableScore);
	}

	//积分上限
	if (GameServiceOption.lRestrictScore!=0L)
	{
		GameServiceOption.lRestrictScore=__max(GameServiceOption.lRestrictScore,GameServiceOption.lMinTableScore);
	}

	//积分上限 lMaxEnterScore
	if (GameServiceOption.wServerType!=GAME_GENRE_SCORE)
	{
		////if(GameServiceOption.lMaxScore<=GameServiceOption.lMinTableScore)
		////	GameServiceOption.lMaxScore=0L;
		////else if(GameServiceOption.lRestrictScore>0)
		////	GameServiceOption.lMaxScore=__min(GameServiceOption.lMaxScore,GameServiceOption->lRestrictScore);
		if(GameServiceOption.lMaxEnterScore<=GameServiceOption.lMinTableScore)
			GameServiceOption.lMaxEnterScore=0L;
		else if(GameServiceOption.lRestrictScore>0)
			GameServiceOption.lMaxEnterScore=__min(GameServiceOption.lMaxEnterScore,GameServiceOption.lRestrictScore);
	}

	return true;
}
//创建机器
VOID *  CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	try
	{
		//创建组件
		if( m_AndroidServiceHelper.GetInterface() == NULL )
		{
			////m_AndroidServiceHelper.SetModuleCreateInfo(TEXT("SparrowGBAndroid.dll"),TEXT("CreateAndroidService"));
			m_AndroidServiceHelper.SetModuleCreateInfo(ANDROID_SERVICE_DLL_NAME,"CreateGameServiceManager");

			if( !m_AndroidServiceHelper.CreateInstance() ) throw 0;
		}

		//创建机器人
		VOID *pAndroidObject = m_AndroidServiceHelper->CreateAndroidUserItemSink(Guid,dwQueryVer);
		if( pAndroidObject == NULL ) throw TEXT("创建机器人失败");

		return pAndroidObject;
	}
	catch(...) {}

	return NULL;
}

////////////
void *  CGameServiceManager::CreateGameDataBaseEngineSink(const IID & Guid, DWORD dwQueryVer)
{
	return NULL;
}



//////////////////////////////////////////////////////////////////////////

//建立对象函数
extern "C" __declspec(dllexport) void *  CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	return g_GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////
