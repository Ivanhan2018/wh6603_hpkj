#ifndef PLAZA_VIEW_ITEM_HEAD_FILE
#define PLAZA_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"
#include "ChongQingSSC.h"
//#include "JiangXiSSC.h"
//#include "XinJiangSSC.h"
#include "GuangDong11X5.h"
//#include "ChongQing11X5.h"
//#include "JiangXi11X5.h"
//#include "FenFenCai.h"
//#include "WuFenCai.h"
//#include "ShanDong11X5.h"
#include "LiuHeCai.h"
//#include "HeiLongJiang11X5.h"
#include "QiXingCai.h"
#include "PaiLie3.h"
#include "3D.h"
#include "BeiJingPK10.h"
#include "BeiJingKuai8.h"
#include "XingYun28.h"
#include "PlatformPublicize.h"
//////////////////////////////////////////////////////////////////////////////////

#define IDM_SHOW_MENU				1110								//显示菜单
#define IDM_CLICKED_TYPE			1111								//更新按钮状态
#define IDM_SEND_QUERY				1112								//查询开奖
//常量定义
#define VIEW_MODE_NONE				0									//无效模式
#define VIEW_MODE_KIND				1									//类型模式
#define VIEW_MODE_SERVER			2									//房间模式
#define VIEW_MODE_GAME				3									//游戏模式
#define VIEW_MODE_TYPE				4									//类型模式

//按钮标识
#define BT_ENTER_KIND				2									//进入类型
#define BT_ENTER_SERVER				3									//进入房间
#define BT_OPEN_LOTTERY				4									//开奖记录

//////////////////////////////////////////////////////////////////////////////////

//类型定义
 struct tagGameKindInfo
 {
 	WORD							wSortID;							//排序标识
 	CPngImage						ImageKindItem;						//类型图片
 	CGameKindItem *					pGameKindItem;						//类型对象
 };

//房间定义
struct tagGameServerInfo
{
	WORD							wSortID;							//排序标识
	WORD							wServerID;							//房间标识
	CGameServerItem *				pGameServerItem;					//房间对象
};


//数组定义
typedef CWHArray<tagGameKindInfo *>	CGameKindInfoArray;					//类型数组
typedef CWHArray<tagGameServerInfo *> CGameServerInfoArray;				//房间数组
#define  IDM_RETURN_GAME 11118

//////////////////////////////////////////////////////////////////////////////////

//游戏广场
class CPlazaViewItem : public CDialog, public IUnknownEx
{
	//状态变量
protected:
	bool							m_bHovering;						//盘旋标志
	bool							m_bCreateFlag;						//创建标志
	BYTE							m_cbShowItemMode;					//显示模式

	CRect							m_GameListRect;						//列表区域
	bool							m_bShowMenu;						//是否显示菜单
	int								m_nMenuXPos;
	int								m_nMenuYPos;
	BYTE							m_cbMenuType;						//菜单类别
	CPlatformPublicize					m_logo;			//LOGO IE控件
	TCHAR 							m_strWebUrl[126];
	int								m_nOldTypeID;							//上一个游戏ID
	CRect							m_rcTypeRect[3];						//类型区域

	int								m_nRecordTypeID;					//返回按钮响应的ID
	int								m_nRecordKindID;					//类型ID 
	DWORD							m_dwTanChuangTick;
	//时时彩
public:
	CFont							m_Font;
// 
// 	CFont							m_Font1;							//重庆时时彩字体
// 	CFont							m_Font2;							//重庆时时彩字体
// 	CFont							m_Font3;							//重庆时时彩字体
// 	CFont							m_Font4;							//重庆时时彩字体
	
	//移动位置
protected:
	BYTE							m_cbButtonDown;						//按钮掩码
	BYTE							m_cbButtonHover;					//按钮掩码
	WORD							m_wViewItemDown;					//点击子项
	WORD							m_wViewItemHover;					//盘旋子项
	CImageButton					m_btnReturn;						//返回

	//位置变量
protected:
	WORD							m_wKindXCount;						//类型列数
	WORD							m_wKindYCount;						//类型行数
	WORD							m_wKindExcursion;					//类型偏移
	WORD							m_wKindPageCount;					//类型页数
	WORD							m_wKindPageCurrent;					//当前页数
	WORD                            m_wKindTypeCurrentID;               //当前类型
	CMissionManager*				m_MissionManager;					//任务管理
	//位置变量
protected:
	WORD							m_wServerXCount;					//房间列数
	WORD							m_wServerYCount;					//房间行数
	WORD							m_wServerExcursion;					//房间偏移
	WORD							m_wServerPageCount;					//房间页数
	WORD							m_wServerPageCurrent;				//当前页数

	//资源变量
protected:
	//CPngImage						m_ImageTitle;						//房间图片
	int								m_nNowTypeID;		
	CPngImage						m_ImageServer;						//房间图片
	BYTE							m_bCreate;
	//变量定义
protected:
	CGameKindInfoArray				m_GameKindInfoActive;				//类型数组
	CGameKindInfoArray				m_GameKindInfoInterface;				//类型数组
	CGameKindInfoArray				m_GameKindInfoActiveMenu;			//类型数组
	CGameKindInfoArray				m_GameKindInfoBuffer;				//类型数组
	CGameServerInfoArray			m_GameServerInfoActive;				//房间数组
	CGameServerInfoArray			m_GameServerInfoBuffer;				//房间数组
	bool							m_bCreateType1;
	WORD							m_wGameHoverItem;
	//控件指针
protected:
	static CPlazaViewItem *			m_pPlazaViewItem;					//广场指针
public:
	CChongQingSSC					m_dlgChongQingSSC;						//重庆时时彩
	CChongQingSSC						m_dlgFenFenCai;							//分分彩
	CChongQingSSC						m_dlgWuFenCai;							//分分彩
	CChongQingSSC						m_dlgJiangXiSSC;						//江西时时彩
	CChongQingSSC					m_dlgXinjiangSSC;					//新疆时时彩
	CGuangDong11X5					m_dlgGuangdong11x5;					//广东11选5
	CGuangDong11X5					m_dlgChongQing11x5;					//重庆11选5
	CGuangDong11X5					m_dlgJiangXi11x5;					//江西11选5
	CGuangDong11X5					m_dlgShanDong11x5;					//山东11选5
	CLiuHeCai						m_dlgLiuHeCai;						//六合彩
	CGuangDong11X5				m_dlgHeiLongJiang11X5;				//黑龙江11选5		
	CQiXingCai						m_dlgQiXingCai;						//七星彩--lly
	CPaiLie3						m_dlgPaiLie3;						//排列三
	C3D								m_dlg3D;							//3D彩
	CBeiJingPK10								m_dlgBjPK10;							//pk10
	CBeiJingKuai8					m_dlgBjKuai8;						//快乐8
	CXingYun28						m_dlgXingYun28;						//幸运28
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
	void CreateDlgCaipiao(int nType);
	bool							m_bLogonSuccess;
	//函数定义
public:
	//构造函数
	CPlazaViewItem();
	//析构函数
	virtual ~CPlazaViewItem();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK() { return; };
	//取消函数
	virtual VOID OnCancel() { return; };

	//功能函数
public:
	//显示类型
	VOID ShowTypeItemView();

	//显示类型
	VOID ShowKindItemView(WORD wTypeID,BOOL bResetPageIndex=TRUE);
	//显示房间
	VOID ShowServerItemView(WORD wKindID,BOOL bResetPageIndex=FALSE);
	//界面更新
	VOID InValidateWndView(BYTE cbViewType);

	VOID ShowGameMenu(bool bShowMenu,WORD wMenuType,int nXPos,int nYPos);

	VOID ShowMenu(WORD wMenuType,CGameKindInfoArray& GameKindInfo);

	VOID FreshGame();

	VOID SetLogonSuccess(bool bSuccess);

	VOID SetMissionManager(CMissionManager* pMissionManager){m_MissionManager = pMissionManager;}
	//功能函数
public:
	//获取实例
	static CPlazaViewItem * GetInstance() { return m_pPlazaViewItem; }	

	//绘画函数
private:
	//绘画类型
	VOID DrawKindItem(CDC * pDC, WORD wViewIndex, INT nXPos, INT nYPos, tagGameKindInfo * pGameKindInfo);
	//绘画房间
	VOID DrawServerItem(CDC * pDC, WORD wViewIndex, INT nXPos, INT nYPos, tagGameServerInfo * pGameServerInfo);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	//内部函数
protected:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	VOID RectifyDlg();
	//资源目录
	VOID GetGameDirectory(TCHAR szDirectory[], WORD wBufferCount, tagGameKind & GameKind);

	//位置测试
protected:
	//对象索引
	WORD GetHoverIndex(CPoint MousePoint);
	//按钮测试
	BYTE GetHoverButton(WORD wViewIndex, CPoint MousePoint);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//对象索引
	WORD GetGameHoverIndex(CPoint MousePoint);
	//按钮消息
public:
	//查看规则
	VOID OnButtonViewRule(WORD wKindID);
	//进入类型
	VOID OnButtonEnterKind(WORD wKindID);
	//进入房间
	VOID OnButtonEnterServer(WORD wServerID);
	void ResetRecordTypeKind();

	void ReturnTouzhu();
	//按钮消息
protected:
	//上页按钮
	VOID OnBnClickedLastKind();
	//下页按钮
	VOID OnBnClickedNextKind();
	//返回按钮
	VOID OnBnClickedReturnHall();
	//上页按钮
	VOID OnBnClickedLastServer();
	//下页按钮
	VOID OnBnClickedNextServer();	
	afx_msg void OnBnClickedReturnUppage();

	//消息函数
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	LRESULT SendQueryGameResult(WPARAM wParam, LPARAM lParam);

	LRESULT OnUpdateAccount(WPARAM wParam, LPARAM lParam);
	//更新余额
	LRESULT OnUpdateYue(WPARAM wParam, LPARAM lParam);
	//更新余额
	LRESULT OnTanChuang(WPARAM wParam, LPARAM lParam);
	//修改密码
	LRESULT OnShowXgmm(WPARAM wParam, LPARAM lParam);
	//显示菜单框
	LRESULT OnShowMenu(WPARAM wParam, LPARAM lParam);
	//点击游戏
	LRESULT OnBnClickedGameType(WPARAM wParam, LPARAM lParam);

public:
	VOID AddOpenResult(CMD_GP_QueryLotResult* pQueryLotResult, int wDataSize);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif