#include "StdAfx.h"
#include "ServerListManager.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameTypeItem::CGameTypeItem()
{
	//设置变量
	ZeroMemory(&m_GameType,sizeof(m_GameType));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameKindItem::CGameKindItem()
{
	//设置变量
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameServerItem::CGameServerItem()
{
	//设置变量
	m_dwVirtualCount=0;

	//设置变量
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerListManager::CServerListManager()
{
	//设置质数
	m_TypeItemMap.InitHashTable(PRIME_TYPE);
	m_KindItemMap.InitHashTable(PRIME_KIND);
	m_ServerItemMap.InitHashTable(PRIME_SERVER);

	return;
}

//析构函数
CServerListManager::~CServerListManager()
{
	//变量定义
	WORD wKey=0;

	//删除种类
	CGameTypeItem * pGameTypeItem=NULL;
	POSITION Position=m_TypeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);
		SafeDelete(pGameTypeItem);
	}
	for (INT_PTR i=0;i<m_TypeItemBuffer.GetCount();i++)
	{
		pGameTypeItem=m_TypeItemBuffer[i];
		SafeDelete(pGameTypeItem);
	}
	m_TypeItemMap.RemoveAll();
	m_TypeItemBuffer.RemoveAll();

	//删除类型
	CGameKindItem * pGameKindItem=NULL;
	Position=m_KindItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_KindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);
		SafeDelete(pGameKindItem);
	}
	for (INT_PTR i=0;i<m_KindItemBuffer.GetCount();i++)
	{
		pGameKindItem=m_KindItemBuffer[i];
		SafeDelete(pGameKindItem);
	}
	m_KindItemMap.RemoveAll();
	m_KindItemBuffer.RemoveAll();

	//删除房间
	CGameServerItem * pGameServerItem=NULL;
	Position=m_ServerItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_ServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);
		SafeDelete(pGameServerItem);
	}
	for (INT_PTR i=0;i<m_ServerItemBuffer.GetCount();i++)
	{
		pGameServerItem=m_ServerItemBuffer[i];
		SafeDelete(pGameServerItem);
	}
	m_ServerItemMap.RemoveAll();
	m_ServerItemBuffer.RemoveAll();

	return;
}

//重置列表
VOID CServerListManager::ResetServerList()
{
	//废弃列表
	DisuseKernelItem();
	DisuseServerItem();

	//清理列表
	CleanKernelItem();
	CleanServerItem();

	return;
}

//清理内核
VOID CServerListManager::CleanKernelItem()
{
	//变量定义
	POSITION PositionType=m_TypeItemMap.GetStartPosition();
	POSITION PositionKind=m_KindItemMap.GetStartPosition();

	//清理种类
	while (PositionType!=NULL)
	{
		//索引房间
		WORD wTypeID=0;
		CGameTypeItem * pGameTypeItem=NULL;
		m_TypeItemMap.GetNextAssoc(PositionType,wTypeID,pGameTypeItem);

		//删除判断
		if (pGameTypeItem->m_bDisuse==true)
		{
			m_TypeItemMap.RemoveKey(wTypeID);
			m_TypeItemBuffer.Add(pGameTypeItem);
		}
	}

	//清理类型
	while (PositionKind!=NULL)
	{
		//索引房间
		WORD wKindID=0;
		CGameKindItem * pGameKindItem=NULL;
		m_KindItemMap.GetNextAssoc(PositionKind,wKindID,pGameKindItem);

		//删除判断
		if (pGameKindItem->m_bDisuse==true)
		{
			m_KindItemMap.RemoveKey(wKindID);
			m_KindItemBuffer.Add(pGameKindItem);
		}
	}

	return;
}

//清理房间
VOID CServerListManager::CleanServerItem()
{
	//变量定义
	POSITION PositionServer=m_ServerItemMap.GetStartPosition();

	//删除房间
	while (PositionServer!=NULL)
	{
		//索引房间
		WORD wServerID=0;
		CGameServerItem * pGameServerItem=NULL;
		m_ServerItemMap.GetNextAssoc(PositionServer,wServerID,pGameServerItem);

		//删除判断
		if (pGameServerItem->m_bDisuse==true)
		{
			//删除数据
			m_ServerItemMap.RemoveKey(wServerID);
			m_ServerItemBuffer.Add(pGameServerItem);

			//设置人数
			CGameKindItem * pGameKindItem=NULL;
			if (m_KindItemMap.Lookup(pGameServerItem->m_GameServer.wKindID,pGameKindItem)==TRUE)
			{
				tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
				pGameKindItem->m_GameKind.dwOnLineCount-=pGameServer->dwOnLineCount;
			}
		}
	}

	return;
}

//废弃内核
VOID CServerListManager::DisuseKernelItem()
{
	//变量定义
	WORD wKey=0;
	DWORD dwKey=0;

	//废弃种类
	CGameTypeItem * pGameTypeItem=NULL;
	POSITION Position=m_TypeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);
		pGameTypeItem->m_bDisuse=true;
	}

	//废弃类型
	CGameKindItem * pGameKindItem=NULL;
	Position=m_KindItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_KindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);
		pGameKindItem->m_bDisuse=true;
	}

	return;
}

//废弃房间
VOID CServerListManager::DisuseServerItem()
{
	//变量定义
	WORD wKey=0;
	CGameServerItem * pGameServerItem=NULL;

	//废弃房间
	POSITION Position=m_ServerItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_ServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);
		pGameServerItem->m_bDisuse=true;
	}

	return;
}

//统计人数
DWORD CServerListManager::CollectOnlineInfo()
{
	//变量定义
	DWORD dwOnLineCount=0L;
	POSITION PositionKind=m_KindItemMap.GetStartPosition();
	
	//统计人数
	while (PositionKind!=NULL)
	{
		//索引类型
		WORD wKindID=0;
		CGameKindItem * pGameKindItem=NULL;
		m_KindItemMap.GetNextAssoc(PositionKind,wKindID,pGameKindItem);

		//统计人数
		if (pGameKindItem!=NULL) dwOnLineCount+=pGameKindItem->m_GameKind.dwOnLineCount;
	}

	return dwOnLineCount;
}

//类型在线
DWORD CServerListManager::CollectOnlineInfo(WORD wKindID)
{
	//变量定义
	DWORD dwOnLineCount=0L;
	POSITION PositionServer=m_ServerItemMap.GetStartPosition();

	//枚举房间
	while (PositionServer!=NULL)
	{
		//索引房间
		WORD wServerID=0;
		CGameServerItem * pGameServerItem=NULL;
		m_ServerItemMap.GetNextAssoc(PositionServer,wServerID,pGameServerItem);

		//统计人数
		if (pGameServerItem->m_GameServer.wKindID==wKindID)
		{
			dwOnLineCount+=pGameServerItem->m_GameServer.dwOnLineCount;
		}
	}

	return dwOnLineCount;
}

//插入种类
bool CServerListManager::InsertGameType(tagGameType * pGameType)
{
	//效验参数
	ASSERT(pGameType!=NULL);
	if (pGameType==NULL) return false;

	//查找现存
	CGameTypeItem * pGameTypeItem=NULL;
	if (m_TypeItemMap.Lookup(pGameType->wTypeID,pGameTypeItem)==FALSE)
	{
		//创建对象
		try
		{
			INT_PTR nStroeCount=m_TypeItemBuffer.GetCount();
			if (nStroeCount>0)
			{
				pGameTypeItem=m_TypeItemBuffer[nStroeCount-1];
				m_TypeItemBuffer.RemoveAt(nStroeCount-1);
			}
			else
			{
				pGameTypeItem=new CGameTypeItem;
				if (pGameTypeItem==NULL) return false;
			}
		}
		catch (...) { return false; }

		//设置变量
		ZeroMemory(pGameTypeItem,sizeof(CGameTypeItem));
	}

	//设置数据
	pGameTypeItem->m_bDisuse=false;
	CopyMemory(&pGameTypeItem->m_GameType,pGameType,sizeof(tagGameType));

	//设置索引
	m_TypeItemMap[pGameType->wTypeID]=pGameTypeItem;

	return true;
}

//插入类型
bool CServerListManager::InsertGameKind(tagGameKind * pGameKind)
{
	//效验参数
	ASSERT(pGameKind!=NULL);
	if (pGameKind==NULL) return false;

	//查找现存
	CGameKindItem * pGameKindItem=NULL;
	if (m_KindItemMap.Lookup(pGameKind->wKindID,pGameKindItem)==FALSE)
	{
		//创建对象
		try
		{
			INT_PTR nStroeCount=m_KindItemBuffer.GetCount();
			if (nStroeCount>0)
			{
				pGameKindItem=m_KindItemBuffer[nStroeCount-1];
				m_KindItemBuffer.RemoveAt(nStroeCount-1);
			}
			else
			{
				pGameKindItem=new CGameKindItem;
				if (pGameKindItem==NULL) return false;
			}
		}
		catch (...) { return false; }

		//设置变量
		ZeroMemory(pGameKindItem,sizeof(CGameKindItem));
	}

	//设置数据
	pGameKindItem->m_bDisuse=false;
	CopyMemory(&pGameKindItem->m_GameKind,pGameKind,sizeof(tagGameKind));

	//设置索引
	m_KindItemMap[pGameKind->wKindID]=pGameKindItem;

	return true;
}

//插入房间
bool CServerListManager::InsertGameServer(tagGameServer * pGameServer)
{
	//效验参数
	ASSERT(pGameServer!=NULL);
	if (pGameServer==NULL) return false;

	//查找房间
	WORD wKindID=0;
	DWORD dwOnLineCount=0L;
	CGameServerItem * pGameServerItem=NULL;

	//获取子项
	if (m_ServerItemMap.Lookup(pGameServer->wServerID,pGameServerItem)==FALSE)
	{
		//创建对象
		try
		{
			INT_PTR nStroeCount=m_ServerItemBuffer.GetCount();
			if (nStroeCount>0)
			{
				pGameServerItem=m_ServerItemBuffer[nStroeCount-1];
				m_ServerItemBuffer.RemoveAt(nStroeCount-1);
			}
			else
			{
				pGameServerItem=new CGameServerItem;
				if (pGameServerItem==NULL) return false;
			}
		}
		catch (...) { return false; }

		//设置变量
		ZeroMemory(pGameServerItem,sizeof(CGameServerItem));
	}
	else
	{
		//保存变量
		wKindID=pGameServerItem->m_GameServer.wKindID;
		dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
	}

	//设置数据
	pGameServerItem->m_bDisuse=false;
	CopyMemory(&pGameServerItem->m_GameServer,pGameServer,sizeof(tagGameServer));

	//设置人数
	if (wKindID!=pGameServerItem->m_GameServer.wKindID)
	{
		//变量定义
		CGameKindItem * pGameKindItemLast=NULL;
		CGameKindItem * pGameKindItemCurrent=NULL;

		//历史人数
		if ((wKindID!=0)&&(m_KindItemMap.Lookup(wKindID,pGameKindItemLast)==TRUE))
		{
			pGameKindItemLast->m_GameKind.dwOnLineCount-=dwOnLineCount;
		}

		//当前人数
		if (m_KindItemMap.Lookup(pGameServer->wKindID,pGameKindItemCurrent)==TRUE)
		{
			pGameKindItemCurrent->m_GameKind.dwOnLineCount+=pGameServer->dwOnLineCount;
		}
	}
	else
	{
		//查找类型
		CGameKindItem * pGameKindItem=NULL;
		m_KindItemMap.Lookup(wKindID,pGameKindItem);

		//设置人数
		if (pGameKindItem!=NULL)
		{
			pGameKindItem->m_GameKind.dwOnLineCount-=dwOnLineCount;
			pGameKindItem->m_GameKind.dwOnLineCount+=pGameServer->dwOnLineCount;
		}
	}

	//设置索引
	m_ServerItemMap[pGameServer->wServerID]=pGameServerItem;

	return true;
}

//删除种类
bool CServerListManager::DeleteGameType(WORD wTypeID)
{
	//查找种类
	CGameTypeItem * pGameTypeItem=NULL;
	if (m_TypeItemMap.Lookup(wTypeID,pGameTypeItem)==FALSE) return false;

	//删除数据
	m_TypeItemMap.RemoveKey(wTypeID);
	m_TypeItemBuffer.Add(pGameTypeItem);

	return true;
}

//删除类型
bool CServerListManager::DeleteGameKind(WORD wKindID)
{
	//查找类型
	CGameKindItem * pGameKindItem=NULL;
	if (m_KindItemMap.Lookup(wKindID,pGameKindItem)==FALSE) return false;

	//删除数据
	m_KindItemMap.RemoveKey(wKindID);
	m_KindItemBuffer.Add(pGameKindItem);

	//设置变量
	ZeroMemory(pGameKindItem,sizeof(CGameKindItem));

	return true;
}

//删除房间
bool CServerListManager::DeleteGameServer(WORD wServerID)
{
	//查找房间
	CGameServerItem * pGameServerItem=NULL;
	if (m_ServerItemMap.Lookup(wServerID,pGameServerItem)==FALSE) return false;

	//删除数据
	m_ServerItemMap.RemoveKey(wServerID);
	m_ServerItemBuffer.Add(pGameServerItem);

	//设置人数
	CGameKindItem * pGameKindItem=NULL;
	if (m_KindItemMap.Lookup(pGameServerItem->m_GameServer.wKindID,pGameKindItem)==TRUE)
	{
		tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
		pGameKindItem->m_GameKind.dwOnLineCount = __max(pGameKindItem->m_GameKind.dwOnLineCount-pGameServer->dwOnLineCount,0);
	}

	return false;
}

//枚举种类
CGameTypeItem * CServerListManager::EmunGameTypeItem(POSITION & Position)
{
	//变量定义
	WORD wKey=0;
	CGameTypeItem * pGameTypeItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_TypeItemMap.GetStartPosition();
	if (Position!=NULL) m_TypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);

	return pGameTypeItem;
}

//枚举类型
CGameKindItem * CServerListManager::EmunGameKindItem(POSITION & Position)
{
	//变量定义
	WORD wKey=0;
	CGameKindItem * pGameKindItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_KindItemMap.GetStartPosition();
	if (Position!=NULL) m_KindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);

	return pGameKindItem;
}

//枚举房间
CGameServerItem * CServerListManager::EmunGameServerItem(POSITION & Position)
{
	//变量定义
	WORD wKey=0;
	CGameServerItem * pGameServerItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_ServerItemMap.GetStartPosition();
	if (Position!=NULL) m_ServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);

	return pGameServerItem;
}

//查找种类
CGameTypeItem * CServerListManager::SearchGameType(WORD wTypeID)
{
	CGameTypeItem * pGameTypeItem=NULL;
	m_TypeItemMap.Lookup(wTypeID,pGameTypeItem);
	return pGameTypeItem;
}

//查找类型
CGameKindItem * CServerListManager::SearchGameKind(WORD wKindID)
{
	CGameKindItem * pGameKindItem=NULL;
	m_KindItemMap.Lookup(wKindID,pGameKindItem);
	return pGameKindItem;
}

//查找房间
CGameServerItem * CServerListManager::SearchGameServer(WORD wServerID)
{
	CGameServerItem * pGameServerItem=NULL;
	m_ServerItemMap.Lookup(wServerID,pGameServerItem);
	return pGameServerItem;
}

//////////////////////////////////////////////////////////////////////////////////
