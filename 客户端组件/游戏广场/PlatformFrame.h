#ifndef PLATFORM_FRAME_HEAD_FILE
#define PLATFORM_FRAME_HEAD_FILE

#pragma once

//系统文件
#include "Stdafx.h"
#include "Resource.h"


//任务文件
#include "MissionList.h"
#include "MissionLogon.h"

//控件文件
#include "PlazaViewItem.h"
#include "ServerViewItem.h"
//窗口控件
#include "WndGameTypeCtrl.h"
#include "WndUserInfoCtrl.h"
#include "WndTopNews.h"
#include "PlatformPublicize.h"
#include "ZhuanhuanDlg.h"
#include "ZhangHuDlg.h"
#include "HuiYuanDlg.h"
#include "ChongZhiDlg.h"
#include "QuKuanDlg.h"
#include "HuoDongDlg.h"
#include "TzhLogDlg.h"
#include "WebBrowser.h"
#include	"GridCtrl/GridCtrl.h"
#include	"GridCtrl/GridCellCheck.h"
#include "TiShiDlg.h"
#include "DlgStatus.h"
#include <vector>
using namespace std;
#include "Other/GfxOutBarCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//热键定义
#define IDI_HOT_KEY_BOSS			0x0100								//老板热键
//////////////////////////////////////////////////////////////////////////////////

//平台框架
class CPlatformFrame : public CFrameWnd, public IServerListDataSink,public CMissionItem, public IStatusViewSink
{
	//状态变量
protected:
	bool							m_bRectify;							//调整标志
	CRect							m_rcViewItem;						//视图大小

	DOUBLE							m_fJiaoyiScore;						//交易金额
	bool							m_bToCaijin;						//转彩金
	bool							m_bToJinbi;							//转金币
	bool							m_bChaxunYue;						//刷新余额
	bool							m_bGetMapBonus;						//bonus
	bool							m_bGetLuckyNums;						//获取开奖数据
	int								m_nTopNewX;
	int								m_nTopNewY;
	CDlgStatus						m_DlgStatus;
	bool							m_bCreate;
	//取消连接
	virtual VOID OnStatusCancel();
	DWORD m_dwTickCount;
	
	//控制按钮
public:
	CSkinButton						m_btMin;							//最小按钮
	CSkinButton						m_btClose;							//关闭按钮
	CSkinButton						m_btSound;							//声音按钮
	CSkinButton						m_btNet;							//信号按钮
	CSkinButton						m_btNavigation[7];					//导航按钮
	CSkinButton						m_btHome;							//首页按钮
	static	UINT	GetLuckNumber(LPVOID lpParam);
	CTiShiDlg						m_tishiDlg;
	bool							m_bKillSocket;						//强制下线
	CGridCtrl						m_Grid;				//新闻公告
	bool							m_bLogonFail;
	bool							m_bSound;
	bool							m_bShutdowSocket;					//是否关闭网络
	//框架控件
protected:
	CWndGameTypeCtrl				m_WndGameTypeCtrl;					//类型子项
	CWndUserInfoCtrl				m_WndUserInfoCtrl;					//用户信息
	CWndTopNews						m_WndTopNews;						//新闻公告
// 	CGfxOutBarCtrl					m_WndBar;							//抽屉类
// 	CImageList						imaLarge, imaSmall;
	BYTE							m_cbShowNewsType;					//新闻位置
//控件变量
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件
	CSkinLayered					m_SkinLayered;						//分层窗口
	CSkinSplitter					m_SkinSplitter;						//拆分控件
	CServerListData					m_ServerListData;					//游戏数据
	//CPlatformPublicize              m_PlatformPublicize;				//浏览控件 
	//CPlatformPublicize              m_PlatFormNoticePublicize;          //浏览控件 
	//CPlatformPublicize              m_PlatFormLeftPublicize;          //浏览控件 
	bool							m_bGetNews;
	bool							m_bGetTime;						//获取系统时间
	bool							m_bQuitGame;					//退出
	CZhangHuDlg					m_dlgUserAccount;					//用户账户
	CHuiYuanDlg					m_dlgHuiyuan;						//会员管理
	CChongZhiDlg				m_dlgChongzhi;						//充值
	CHuoDongDlg					m_dlgHuodong;						//活动
	CQuKuanDlg					m_dlgQukuan;						//取款
	CTzhLogDlg			m_dlgTouZhu;						//投注记录
	//任务组件
protected:
	CMissionList					m_MissionList;						//列表任务
	CMissionLogon					m_MissionLogon;						//登录任务
public:
	CMissionManager					m_MissionManager;					//任务管理
	bool							m_bCanSend;							//是否可以发送
	//控件数组
public:
	CPlazaViewItem					m_PlazaViewItem;					//游戏广场
	CServerViewItem					m_ServerViewItem;					//当前房间

	//资源变量
protected:
	//CBitImage						m_ImageFrame;						//框架图片
	CEncirclePNG					m_FrameEncircle;					//框架资源
	//CEncircleBMP					m_AfficheEncircle;					//框架资源
	CEncircleBMP					m_ItemFrameEncircle;				//框架资源
	vector<wstring>		m_vecTitle;
	bool				m_bGame;			//是否是游戏画面，true游戏，false管理
	int							m_nNewsYPos;	//新闻滚动时的Y坐标
	int						m_nTop;
	int							m_nreduce;
	int							m_nNewsHeight;
	CFont m_top8Font;
	CMarkup m_xml;

	//静态变量
protected:
	static CPlatformFrame *			m_pPlatformFrame;					//框架指针

	//函数定义
public:
	//构造函数
	CPlatformFrame();
	//析构函数
	virtual ~CPlatformFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//状态通知
public:
	//获取通知
	virtual VOID OnGameItemFinish();
	//获取通知
	virtual VOID OnGameKindFinish(WORD wKindID);

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
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//调整资源
	VOID RectifyResource(INT nWidth, INT nHeight);
	//绘画界面
	VOID DrawControlView(CDC * pDC, INT nWidth, INT nHeight);

	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);

	//房间管理
public:
	//激活房间
	bool ActiveServerViewItem();
	//删除房间
	bool DeleteServerViewItem();
	//进入房间
	bool EntranceServerItem(CGameServerItem * pGameServerItem);
	void DrawTopUsers(CDC* pDC);

	void	GridCtrlInit();				//初始化新闻公告
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//消息函数
protected:
	//关闭消息
	VOID OnClose();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//显示消息
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//位置改变
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//显示菜单框
	LRESULT OnShowMenu(WPARAM wParam, LPARAM lParam);
	//查询开奖数据
	LRESULT OnQueryGameResult(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpDateAccoount(WPARAM wParam, LPARAM lParam);
	LRESULT OnShowXgmm(WPARAM wParam, LPARAM lParam);
	LRESULT OnRefreshYue(WPARAM wParam, LPARAM lParam);
	LRESULT OnFreshGame(WPARAM wParam, LPARAM lParam);
	LRESULT OnTouZhuTishi(WPARAM wParam, LPARAM lParam);

	LRESULT OnZhuanhuanJinbi(WPARAM wParam, LPARAM lParam);
	LRESULT OnZhuanhuanCaibi(WPARAM wParam, LPARAM lParam);
	LRESULT ReturnTouzhu(WPARAM wParam, LPARAM lParam);
	LRESULT ReleaseFace(WPARAM wParam, LPARAM lParam);
	bool m_bCreateType4;
//自定消息
protected:
	//事件消息
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
	//银行更新
	LRESULT OnMessageInsureUpdate(WPARAM wParam, LPARAM lParam);

	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
public:
	VOID SendToServer(int nSendType);
	VOID AvtiveMainMissionItem();
	bool CreatePlazaview();
public:
	CPlatformPublicize			m_logo;			//LOGO IE控件
	//	CWebBrowser			*m_pnotice;		//公告信息

	CPlatformPublicize			m_Publicizeurl;			//LOGO IE控件
	void Loadweb();
	void ShowXgmm();

	//功能函数
public:
	//获取实例
	static CPlatformFrame * GetInstance() { return m_pPlatformFrame; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif