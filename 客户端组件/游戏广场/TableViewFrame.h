#ifndef TABLE_VIEW_FRAME_HEAD_FILE
#define TABLE_VIEW_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GamePlazaHead.h"

//////////////////////////////////////////////////////////////////////////////////

//按钮标识
#define BT_ENTER_GAME				1									//进入游戏
#define BT_CLOSE_SERVER				2									//关闭房间

//桌子属性
struct tagTableAttribute
{
	//桌子状态
	WORD							wWatchCount;						//旁观数目
	DWORD							dwTableOwnerID;						//桌主索引

	//属性变量
	WORD							wTableID;							//桌子号码
	WORD							wChairCount;						//椅子数目
	IClientUserItem *				pIClientUserItem[MAX_CHAIR];		//用户信息
};

//////////////////////////////////////////////////////////////////////////////////

//桌子视图
class CTableView : public ITableView
{
	//状态变量
protected:
	tagTableAttribute				m_TableAttribute;					//桌子属性

	//组件接口
protected:
	ITableViewFrame *				m_pITableViewFrame;					//桌子接口

	//函数定义
public:
	//构造函数
	CTableView();
	//析构函数
	virtual ~CTableView();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//功能接口
public:
	//空椅子数
	virtual WORD GetNullChairCount(WORD & wNullChairID);
	//配置函数
	virtual VOID InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableViewFrame);

	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(WORD wChairID);
	//设置信息
	virtual bool SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem);
};

//////////////////////////////////////////////////////////////////////////////////

//数组定义
typedef CWHArray<CTableView *>		CTableViewArray;					//桌子数组

//游戏桌子
class CTableViewFrame : public CWnd, public ITableViewFrame
{
	//友元定义
	friend class CTableView;

	//属性变量
protected:
	WORD							m_wTableCount;						//游戏桌数
	WORD							m_wChairCount;						//椅子数目

	//按钮变量
protected:
	bool							m_bHovering;						//盘旋标志
	BYTE							m_cbButtonDown;						//按钮掩码
	BYTE							m_cbButtonHover;					//按钮掩码

	//控件变量
protected:
	CTableViewArray					m_TableViewArray;					//桌子数组

	//组件接口
protected:
	ITableViewFrameSink *			m_pITableViewFrameSink;				//通知接口

	//函数定义
public:
	//构造函数
	CTableViewFrame();
	//析构函数
	virtual ~CTableViewFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//配置桌子
	virtual bool ConfigTableFrame(WORD wTableCount, WORD wChairCount);
	//创建桌子
	virtual bool CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx);

	//信息接口
public:
	//桌子数目
	virtual WORD GetTableCount() { return m_wTableCount; }
	//椅子数目
	virtual WORD GetChairCount() { return m_wChairCount; }

	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(WORD wTableID, WORD wChairID);
	//设置信息
	virtual bool SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem);

	//功能接口
public:
	//获取桌子
	virtual ITableView * GetTableViewItem(WORD wTableID);
	//空椅子数
	virtual WORD GetNullChairCount(WORD wTableID, WORD & wNullChairID);

	//内部函数
protected:
	//按钮测试
	BYTE GetHoverButton(CPoint MousePoint);

	//消息函数
protected:
	//重画消息
	VOID OnPaint();
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif