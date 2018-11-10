#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "TableViewFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//按钮大小
#define ENTER_GAME_CX				142									//按钮宽度
#define ENTER_GAME_CY				52									//按钮高度

//按钮大小
#define CLOSE_SERVER_CX				142									//按钮宽度
#define CLOSE_SERVER_CY				52									//按钮高度

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTableViewFrame, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableView::CTableView()
{
	//组件接口
	m_pITableViewFrame=NULL;

	//状态变量
	ZeroMemory(&m_TableAttribute,sizeof(m_TableAttribute));

	return;
}

//析构函数
CTableView::~CTableView()
{
}

//接口查询
VOID * CTableView::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableView,Guid,dwQueryVer);
	return NULL;
}

//空椅子数
WORD CTableView::GetNullChairCount(WORD & wNullChairID)
{
	//设置变量
	wNullChairID=INVALID_CHAIR;

	//寻找位置
	WORD wNullCount=0;
	for (WORD i=0;i<m_TableAttribute.wChairCount;i++)
	{
		if (m_TableAttribute.pIClientUserItem[i]==NULL)
		{
			//设置数目
			wNullCount++;

			//设置结果
			if (wNullChairID==INVALID_CHAIR) wNullChairID=i;
		}
	}

	return wNullCount;
}

//配置函数
VOID CTableView::InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableViewFrame)
{
	//设置属性
	m_TableAttribute.wTableID=wTableID;
	m_TableAttribute.wChairCount=wChairCount;

	//设置接口
	m_pITableViewFrame=QUERY_OBJECT_PTR_INTERFACE(pITableViewFrame,ITableViewFrame);

	return;
}

//获取用户
IClientUserItem * CTableView::GetClientUserItem(WORD wChairID)
{
	//效验参数
	ASSERT(wChairID<m_TableAttribute.wChairCount);
	if (wChairID>=m_TableAttribute.wChairCount) return NULL;

	//获取用户
	return m_TableAttribute.pIClientUserItem[wChairID];
}

//设置信息
bool CTableView::SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem)
{
	//效验参数
	ASSERT(wChairID<m_TableAttribute.wChairCount);
	if (wChairID>=m_TableAttribute.wChairCount) return false;

	//设置用户
	m_TableAttribute.pIClientUserItem[wChairID]=pIClientUserItem;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableViewFrame::CTableViewFrame()
{
	//属性变量
	m_wTableCount=0;
	m_wChairCount=0;

	//按钮变量
	m_cbButtonDown=0;
	m_cbButtonHover=0;
	m_bHovering=false;

	//设置指针
	m_pITableViewFrameSink=NULL;

	return;
}

//析构函数
CTableViewFrame::~CTableViewFrame()
{
	//删除桌子
	for (INT_PTR i=0;i<m_TableViewArray.GetCount();i++)
	{
		CTableView * pTableView=m_TableViewArray[i];
		if (pTableView!=NULL) SafeDelete(pTableView);
	}

	//删除数组
	m_TableViewArray.RemoveAll();

	return;
}

//接口查询
VOID * CTableViewFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableViewFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableViewFrame,Guid,dwQueryVer);
	return NULL;
}

//配置函数
bool CTableViewFrame::ConfigTableFrame(WORD wTableCount, WORD wChairCount)
{
	//效验参数
	ASSERT(wChairCount<=MAX_CHAIR);

	//设置变量
	m_wTableCount=wTableCount;
	m_wChairCount=wChairCount;
	m_TableViewArray.SetSize(m_wTableCount);
	ZeroMemory(m_TableViewArray.GetData(),m_wTableCount*sizeof(CTableView *));

	//创建桌子
	for (WORD i=0;i<m_wTableCount;i++)
	{
		m_TableViewArray[i]=new CTableView;
		m_TableViewArray[i]->InitTableView(i,wChairCount,this);
	}

	return true;
}

//创建函数
bool CTableViewFrame::CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx)
{
	//设置变量
	m_cbButtonDown=0;
	m_cbButtonHover=0;
	m_bHovering=false;

	//设置接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableViewFrameSink)!=NULL);
	m_pITableViewFrameSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableViewFrameSink);

	//创建窗口
	CRect rcCreate(0,0,0,0);
	Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,pParentWnd,uWndID);

	return true;
}

//获取用户
IClientUserItem * CTableViewFrame::GetClientUserItem(WORD wTableID, WORD wChairID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取用户
	if (pITableView!=NULL)
	{
		return pITableView->GetClientUserItem(wChairID);
	}

	return NULL;
}

//设置信息
bool CTableViewFrame::SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem)
{
	ITableView * pITableView=GetTableViewItem(wTableID);
	if (pITableView!=NULL) pITableView->SetClientUserItem(wChairID,pIClientUserItem);
	return true;
}

//获取桌子
ITableView * CTableViewFrame::GetTableViewItem(WORD wTableID)
{
	//获取桌子
	if (wTableID!=INVALID_TABLE)
	{
		//效验参数
		ASSERT(wTableID<m_TableViewArray.GetCount());
		if (wTableID>=m_TableViewArray.GetCount()) return NULL;

		//获取桌子
		ITableView * pITableView=m_TableViewArray[wTableID];

		return pITableView;
	}

	return NULL;
}

//空椅子数
WORD CTableViewFrame::GetNullChairCount(WORD wTableID, WORD & wNullChairID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取状态
	if (pITableView!=NULL)
	{
		return pITableView->GetNullChairCount(wNullChairID);
	}

	return 0;
}

//按钮测试
BYTE CTableViewFrame::GetHoverButton(CPoint MousePoint)
{
	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//进入位置
	CRect rcEnterGame;
	rcEnterGame.top=400;
	rcEnterGame.bottom=400+ENTER_GAME_CY;
	rcEnterGame.right=rcClient.Width()/2-20;
	rcEnterGame.left=rcClient.Width()/2-20-ENTER_GAME_CX;
	if (rcEnterGame.PtInRect(MousePoint)==TRUE) return BT_ENTER_GAME;

	//关闭位置
	CRect rcCloseServer;
	rcCloseServer.top=400;
	rcCloseServer.bottom=400+CLOSE_SERVER_CY;
	rcCloseServer.left=rcClient.Width()/2+20;
	rcCloseServer.right=rcClient.Width()/2+20+CLOSE_SERVER_CX;
	if (rcCloseServer.PtInRect(MousePoint)==TRUE) return BT_CLOSE_SERVER;

	return 0;
}

//重画消息
VOID CTableViewFrame::OnPaint()
{
// 	//创建 DC
// 	CPaintDC dc(this);
// 
// 	//获取区域
// 	CRect rcClient;
// 	GetClientRect(&rcClient);
// 
// 	//创建缓冲
// 	CBitImage ImageBuffer;
// 	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);
// 
// 	//创建设备
// 	CImageDC BufferDC(ImageBuffer);
// 	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//加载资源
	//CBitImage TableLogo;
// 	CBitImage TableGround;
//	TableLogo.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_LOGO);
//	TableGround.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_GROUND);

	//绘画背景
//	TableGround.DrawImageTile(pBufferDC,0,0,rcClient.Width(),rcClient.Height());
	//TableLogo.BitBlt(BufferDC,(rcClient.Width()-TableLogo.GetWidth())/2,0,SRCCOPY);

// 	//加载资源
// 	CBitImage ImageEnterGame;
// 	CBitImage ImageCloseServer;
// 	ImageEnterGame.LoadFromResource(AfxGetInstanceHandle(),IDB_BT_ENTER_GAME);
// 	ImageCloseServer.LoadFromResource(AfxGetInstanceHandle(),IDB_BT_CLOSE_SERVER);
// 
// 	//获取大小
// 	CSize SizeEnterGame;
// 	CSize SizeCloseServer;
// 	SizeEnterGame.SetSize(ImageEnterGame.GetWidth()/5L,ImageEnterGame.GetHeight());
// 	SizeCloseServer.SetSize(ImageCloseServer.GetWidth()/5L,ImageCloseServer.GetHeight());
// 
// 	//关闭房间
// 	INT nXCloseIndex=0;
// 	bool bCloseButtonDown=(m_cbButtonDown==BT_CLOSE_SERVER);
// 	bool bCloseButtonHover=(m_cbButtonHover==BT_CLOSE_SERVER);
// 
// 	//关闭房间
// 	if ((bCloseButtonHover==true)&&(bCloseButtonDown==true)) nXCloseIndex=1;
// 	if ((bCloseButtonHover==true)&&(bCloseButtonDown==false)) nXCloseIndex=2;
// 	ImageCloseServer.BitBlt(BufferDC,rcClient.Width()/2+20,400,SizeCloseServer.cx,SizeCloseServer.cy,nXCloseIndex*SizeCloseServer.cx,0);
// 
// 	//进入游戏
// 	INT nXEnterIndex=0;
// 	bool bEnterButtonDown=(m_cbButtonDown==BT_ENTER_GAME);
// 	bool bEnterButtonHover=(m_cbButtonHover==BT_ENTER_GAME);
// 
// 	//查看规则
// 	if ((bEnterButtonHover==true)&&(bEnterButtonDown==true)) nXEnterIndex=1;
// 	if ((bEnterButtonHover==true)&&(bEnterButtonDown==false)) nXEnterIndex=2;
// 	ImageEnterGame.BitBlt(BufferDC,rcClient.Width()/2-SizeEnterGame.cx-20,400,SizeEnterGame.cx,SizeEnterGame.cy,nXEnterIndex*SizeEnterGame.cx,0);
// 
	//绘画界面
//	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return;
}

//鼠标消息
VOID CTableViewFrame::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//位置计算
	BYTE cbButtonHover=GetHoverButton(Point);

	//数据调整
	if ((m_cbButtonDown!=0)&&(cbButtonHover!=m_cbButtonDown))
	{
		cbButtonHover=0;
	}

	//盘旋变量
	if (m_cbButtonHover!=cbButtonHover)
	{
		//设置变量
		m_cbButtonHover=cbButtonHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//进入判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//鼠标消息
VOID CTableViewFrame::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//结果动作
	BYTE cbButtonDown=0;
	WORD wViewItemDown=INVALID_WORD;

	//设置变量
	if (m_cbButtonDown==m_cbButtonHover)
	{
		cbButtonDown=m_cbButtonDown;
	}

	//按起处理
	if (m_cbButtonDown!=0)
	{
		//释放鼠标
		ReleaseCapture();

		//设置变量
		m_cbButtonDown=0;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	}

	//执行动作
	if (cbButtonDown!=0)
	{
		ASSERT(m_pITableViewFrameSink!=NULL);
		if (m_pITableViewFrameSink!=NULL) m_pITableViewFrameSink->OnHitTableButton(cbButtonDown);
	}

	return;
}

//鼠标消息
VOID CTableViewFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//设置焦点
	SetFocus();

	//按下处理
	if (m_cbButtonHover!=0)
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_cbButtonDown=m_cbButtonHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	}

	return;
}

//鼠标消息
LRESULT CTableViewFrame::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置状态
	m_bHovering=false;

	//更新界面
	if (m_cbButtonHover!=0)
	{
		//设置变量
		m_cbButtonDown=0;
		m_cbButtonHover=0;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
