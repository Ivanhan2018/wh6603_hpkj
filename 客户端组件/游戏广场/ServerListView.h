#ifndef SERVER_LIST_VIEW_HEAD_FILE
#define SERVER_LIST_VIEW_HEAD_FILE

#pragma once
#include "Stdafx.h"
#include "ServerListData.h"
#include "WndGameTypeCtrl.h"
//////////////////////////////////////////////////////////////////////////////////
//房间列表
typedef CList<CGameServerItem *>	 CGameServerList;				//游戏房间

//列表节点
struct tagServerListItem
{
	INT             nPosX;
	INT             nPosY;
	WORD            wKindHeight;
	bool            bShowServer;
	CGameKindItem * pGameKindItem;
	CGameServerList ServerList;

	tagServerListItem()
	{
		nPosX=0;
		nPosY=0;
		wKindHeight=52;
		bShowServer=false;
		pGameKindItem=NULL;
	}

	~tagServerListItem()
	{
		ServerList.RemoveAll();
	}
};

//游戏列表
typedef CList<tagServerListItem *>	 CServerList;				//游戏房间

//////////////////////////////////////////////////////////////////////////////////

//数组说明
typedef CMap<WORD,WORD,UINT,UINT>	CGameImageMap;						//游戏图标
typedef CList<WORD>	                CLastGameServerList;				//游戏房间

//////////////////////////////////////////////////////////////////////////////////

//房间列表
class CServerListView : public CWnd, public IServerListDataSink
{
	//位置变量
protected:
	INT								m_nXScroll;							//滚动偏移
	INT								m_nYScroll;							//滚动偏移
	INT								m_nScrollPos;						//滚动位置
	INT								m_nScrollPosMax;					//最大位置
	////显示类型
    INT                             m_nShowType;
	//状态信息
protected:
	bool							m_bHovering;						//盘旋标志
	bool							m_bHoveKind;						//盘旋标志
	bool							m_bLMouseDown;						//左键标志
	CWndGameTypeCtrl				m_WndGameTypeCtrl;					//类型子项
	//列表句柄
protected:
	CServerList                     m_ServerList;
    CServerList                     m_ServerListGameType;
	//资源变量
protected:
	CFont							m_FontBold;							//粗体字体
	CSkinScrollBar					m_SkinScrollBar;					//滚动条类

	//资源变量
protected:
	CGameImageMap					m_GameImageMap;						//图形索引
	CLastGameServerList             m_LastPlayGameList;                 //最近玩过游戏

	//辅助变量
protected:
	tagServerListItem *				m_pItemMouseHover;					//盘旋子项
	tagServerListItem *				m_pItemMouseDown;					//点击子项
	CGameServerItem *               m_pServerItemHover;                 //盘旋子项
	CGameServerItem *               m_pServerItemDown;                  //点击子项

	//字体变量
protected:

	//函数定义
public:
	//构造函数
	CServerListView();
	//析构函数
	virtual ~CServerListView();

	//状态通知
public:
	//获取通知
	virtual VOID OnGameItemFinish();
	//获取通知
	virtual VOID OnGameKindFinish(WORD wKindID);
	//更新通知
	virtual VOID OnGameItemUpdateFinish();

	//更新通知
public:
	//插入通知
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//重载函数
protected:
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//窗口函数
	virtual LRESULT DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//快速通道
	VOID InitAssistantItem();
	//配置函数
	VOID InitServerTreeView();

	//绘画函数
private:
	//绘画子项
	VOID DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox);
	//绘画背景
	VOID DrawTreeBack(CDC * pDC, CRect & rcClient, CRect & rcClipBox);

	//绘画辅助
private:
	//绘制文本
	VOID DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected);

	//图标函数
private:
	//获取图标
	UINT GetGameImageIndex(CGameKindItem * pGameKindItem);
	//获取图标
	UINT GetGameImageIndex(CGameServerItem * pGameServerItem);

	//标题函数
private:
	//获取标题
	LPCTSTR GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount);
	//获取标题
	LPCTSTR GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount);

	//辅助函数
private:
	//删除更新
	VOID DeleteUpdateItem(CGameListItem * pGameListItem);
	//修改子项
	VOID ModifyGameListItem(HTREEITEM hTreeItem, LPCTSTR pszTitle, UINT uImage);
	//插入子项
	HTREEITEM InsertInsideItem(LPCTSTR pszTitle, UINT uImage, DWORD dwInsideID, HTREEITEM hParentItem);
	//插入子项
	HTREEITEM InsertGameListItem(LPCTSTR pszTitle, UINT uImage, CGameListItem * pGameListItem, HTREEITEM hParentItem);
	//资源目录
	VOID GetGameResDirectory(tagGameKind &GameKind,TCHAR szResDirectory[], WORD wBufferCount);
	//测试子项
	tagServerListItem *HitTest(CPoint &MousePoint);
	//测试子项
	CGameServerItem *HitSubTest(tagServerListItem *pServerListItem,CPoint &MousePoint);
	//清除列表
	void ClearServerList();
	//调整位置
	void ModifyListPos();
	//调整位置
	void SetScrollMaxPos();

	//系统消息
protected:
	//重画消息
	VOID OnPaint();
	//时间消息
	VOID OnTimer(UINT nIDEvent);
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//滚动消息
	VOID OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	//列表消息
protected:
	//右键列表
	VOID OnNMRClick(NMHDR * pNMHDR, LRESULT * pResult);
	//左击列表
	VOID OnNMLClick(NMHDR * pNMHDR, LRESULT * pResult);
	//列表改变
	VOID OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult);
	//列表展开
	VOID OnTvnItemexpanding(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()

protected:
	//获得房间负载信息
	LPCTSTR GetLoadInfoOfServer(DWORD dwOnLineCount, DWORD dwMaxCount, LPTSTR pszBuffer, WORD wBufferSize);
	//获得房间负载信息
	LPCTSTR GetLoadInfoOfServer(tagGameServer * pGameServer, LPTSTR pszBuffer, WORD wBufferSize);
	//获得房间负载信息
	LPCTSTR GetLoadInfoOfServer(tagGameKind * pGameKind, LPTSTR pszBuffer, WORD wBufferSize);
	//加载记录
	void LoadLastPlayGame();
	//保存记录
	void SaveLastPlayGame();

public:
	VOID ShowGameType(WORD wType);
	VOID ShowGameServer();
};

//////////////////////////////////////////////////////////////////////////////////

#endif