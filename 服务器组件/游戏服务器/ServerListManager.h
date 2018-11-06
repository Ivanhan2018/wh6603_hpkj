#ifndef SERVER_LIST_MANAGER_HEAD_FILE
#define SERVER_LIST_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"

//////////////////////////////////////////////////////////////////////////////////

//列表管理
namespace NServerListManager
{

	//////////////////////////////////////////////////////////////////////////////////

	//游戏子项
	class CGameListItem
	{
		//变量定义
	public:
		bool							m_bDisuse;							//废弃标志

		//函数定义
	protected:
		//构造函数
		CGameListItem() { m_bDisuse=false; }
	};

	//////////////////////////////////////////////////////////////////////////////////

	//种类结构
	class CGameTypeItem : public CGameListItem
	{
		//变量定义
	public:
		tagGameType						m_GameType;							//种类信息

		//函数定义
	public:
		//构造函数
		CGameTypeItem();
	};

	//////////////////////////////////////////////////////////////////////////////////

	//类型结构
	class CGameKindItem : public CGameListItem
	{
		//变量定义
	public:
		tagGameKind						m_GameKind;							//类型信息

		//函数定义
	public:
		//构造函数
		CGameKindItem();
	};

	//////////////////////////////////////////////////////////////////////////////////

	//房间结构
	class CGameServerItem : public CGameListItem
	{
		//变量定义
	public:
		tagGameServer					m_GameServer;						//房间信息

		//函数定义
	public:
		//构造函数
		CGameServerItem();
	};

	//////////////////////////////////////////////////////////////////////////////////

	//数组说明
	typedef CWHArray<CGameTypeItem *> CGameTypeItemArray;
	typedef CWHArray<CGameKindItem *> CGameKindItemArray;
	typedef CWHArray<CGameServerItem *>	CGameServerItemArray;

	//索引说明
	typedef CMap<WORD,WORD,CGameTypeItem *,CGameTypeItem * &> CTypeItemMap;
	typedef CMap<WORD,WORD,CGameKindItem *,CGameKindItem * &> CKindItemMap;
	typedef CMap<WORD,WORD,CGameServerItem *,CGameServerItem * &> CServerItemMap;

	//////////////////////////////////////////////////////////////////////////////////

	//列表服务
	class CServerListManager
	{
		//索引变量
	protected:
		CTypeItemMap					m_TypeItemMap;						//种类索引
		CKindItemMap					m_KindItemMap;						//类型索引
		CServerItemMap					m_ServerItemMap;					//房间索引

		//存储变量
	protected:
		CGameTypeItemArray				m_TypeItemBuffer;					//种类数组
		CGameKindItemArray				m_KindItemBuffer;					//类型数组
		CGameServerItemArray			m_ServerItemBuffer;					//房间数组

		//函数定义
	public:
		//构造函数
		CServerListManager();
		//析构函数
		virtual ~CServerListManager();

		//管理接口
	public:
		//重置列表
		VOID ResetServerList();
		//清理内核
		VOID CleanKernelItem();
		//清理房间
		VOID CleanServerItem();
		//废弃内核
		VOID DisuseKernelItem();
		//废弃房间
		VOID DisuseServerItem();

		//统计接口
	public:
		//统计人数
		DWORD CollectOnlineInfo();
		//类型在线
		DWORD CollectOnlineInfo(WORD wKindID);

		//插入接口
	public:
		//插入种类
		bool InsertGameType(tagGameType * pGameType);
		//插入类型
		bool InsertGameKind(tagGameKind * pGameKind);
		//插入房间
		bool InsertGameServer(tagGameServer * pGameServer);

		//删除接口
	public:
		//删除种类
		bool DeleteGameType(WORD wTypeID);
		//删除类型
		bool DeleteGameKind(WORD wKindID);
		//删除房间
		bool DeleteGameServer(WORD wServerID);

		//枚举接口
	public:
		//枚举种类
		CGameTypeItem * EmunGameTypeItem(POSITION & Position);
		//枚举类型
		CGameKindItem * EmunGameKindItem(POSITION & Position);
		//枚举房间
		CGameServerItem * EmunGameServerItem(POSITION & Position);

		//查找接口
	public:
		//查找种类
		CGameTypeItem * SearchGameType(WORD wTypeID);
		//查找类型
		CGameKindItem * SearchGameKind(WORD wKindID);
		//查找房间
		CGameServerItem * SearchGameServer(WORD wServerID);

		//数目接口
	public:
		//种类数目
		DWORD GetGameTypeCount() { return (DWORD)m_TypeItemMap.GetCount(); }
		//类型数目
		DWORD GetGameKindCount() { return (DWORD)m_KindItemMap.GetCount(); }
		//房间数目
		DWORD GetGameServerCount() { return (DWORD)m_ServerItemMap.GetCount(); }
	};

	//////////////////////////////////////////////////////////////////////////////////

};

using namespace NServerListManager;

#endif