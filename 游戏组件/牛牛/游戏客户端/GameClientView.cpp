#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientView.h"
#include "GameClientEngine.h"

#ifdef DEBUG
//#define ALONE
#endif

//////////////////////////////////////////////////////////////////////////
//宏定义

//按钮标识
#define IDC_HINT_OX						130								//提示按钮
#define IDC_START						100								//开始按钮
#define IDC_MAX_SCORE					104								//最大按钮
#define IDC_MIN_SCORE					105								//最少按钮
#define IDC_ADD_SCORE					110								//加注按钮

#define IDC_READY						111								//摊牌按钮
#define IDC_RESORT						112								//重排按钮
#define IDC_OX							113								//牛牛按钮

#define IDC_SHORTCUT					114								//快捷键按钮
#define IDC_IDLER						115								//闲家按钮
#define IDC_BANKER						116								//庄家按钮
#define IDC_SCORE						117								//查分按钮
#define IDC_YU_YIN						118								//语音按钮
 #define IDC_ADMIN						119									//按钮标识

//定时器标识
#define IDI_SEND_CARD					98								//发牌定时器
#define IDI_GOLD						97								//发牌定时器
#define IDI_USER_ACTION					101								//动作定时器
#define IDI_UPDATA_GAME					102								//更新定时器
#define IDI_JETTON_ACTION				103								//动作定时器

//定时器时间
#define TIME_USER_ACTION				4000							//动作定时器

//移动速度
#define	SEND_PELS						80								//发牌速度
#define	TIME_SENDSPEED					50								//间隔速度


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SCORE, OnBnClickedScore)
	ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)

	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView()
{
	//动画变量
	m_cbIndex=0;
	m_bJettonAction=false;
	m_cbDynamicJoin=FALSE;
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_bOxValue[i]=0xff;
		m_SendEndingPos[i].SetPoint(0,0);
	}
	m_wSendCount=0;
	m_wSendIndex=0;
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();
	ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));
	ZeroMemory(m_lMoveGold,sizeof(m_lMoveGold));

	//数据变量
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bOpenCardUser,sizeof(m_bOpenCardUser));
	ZeroMemory(m_bShowSocre,sizeof(m_bShowSocre));
	m_wBankerUser=INVALID_CHAIR;
	m_bCallUser=0xff;
	m_bShow=false;
	m_bInvest=false;
	m_bOpenCard=false;
	m_bUserAction=false;
	m_bLookOnUser=false;
	m_bShowScore=false;
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	return;
}

//析构函数
CGameClientView::~CGameClientView()
{
	m_ViewFontEx.DeleteFont();
}

//消息解释
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageScore.LoadFromResource(this,hInstance,IDB_SCORE);
	m_ImageJeton.LoadFromResource(this,hInstance,IDB__MONEY);
	m_ImageCard.LoadFromResource(this,hInstance,IDB_CARD);
	m_ImageViewBack.LoadFromResource(this,hInstance,IDB_VIEW_BACK);
	m_ImageViewCenter.LoadFromResource(this,hInstance,IDB_VIEW_FILL);

	//加载资源
	m_PngReady.LoadImage(this,hInstance,TEXT("READY"));
	m_PngUserBack.LoadImage(this,hInstance,TEXT("USER_HEAD_BACK"));
	m_ImageOxValueZero.LoadImage(this,hInstance,TEXT("COW_VALUE_ZERO"));
	m_ImageDoulbeOx.LoadImage(this,hInstance,TEXT("DOUBLE_COW"));
	m_ImageNumber.LoadImage(this,hInstance,TEXT("NUMBER"));
	m_ImageOxValue.LoadImage(this,hInstance,TEXT("COW_VALUE"));
	m_ImageWaitCall.LoadImage(this,hInstance,TEXT("CALLBANKER"));
	m_ImageTimeBack.LoadImage(this,hInstance,TEXT("TIME_BACK"));
	m_ImageTimeNumber.LoadImage(this,hInstance,TEXT("TIME_NUMBER"));
	m_ImageBanker.LoadImage(this,hInstance,TEXT("BANKER_FLAG"));
	m_PngAutoOpen.LoadImage(this,hInstance,TEXT("AUTO_OPEN"));
	m_PngWaitBet.LoadImage(this,hInstance,TEXT("WAIT_BET"));
	m_PngWaitOpen.LoadImage(this,hInstance,TEXT("WAIT_OPEN"));
	m_PngOpenTag.LoadImage(this,hInstance,TEXT("OPEN_TAG"));
	m_PngHistoryScore.LoadImage(this,hInstance,TEXT("HISTORY_SCORE"));
	m_PngOffLine.LoadImage(this,hInstance,TEXT("OFF_LINE"));
	
	//创建控件
	CRect rcCreate(0,0,0,0);
	m_btYuYin.Create(NULL,WS_CHILD,rcCreate,this,IDC_YU_YIN);
	m_btScore.Create(NULL,WS_CHILD,rcCreate,this,IDC_SCORE);
	m_btHintOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_HINT_OX);
	m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_START);
	m_btOpenCard.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_READY);
	m_btOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OX);
	m_btBanker.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BANKER);
	m_btIdler.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_IDLER);

	//加载位图
	m_btYuYin.SetButtonImage(IDB_BT_YU_YIN,hInstance,false,false);
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);
	m_btHintOx.SetButtonImage(IDB_HINT_OX,hInstance,false,false);
	m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
	m_btOpenCard.SetButtonImage(IDB_OPENCARD,hInstance,false,false);
	m_btOx.SetButtonImage(IDB_COW,hInstance,false,false);
	m_btBanker.SetButtonImage(IDB_BANKER,hInstance,false,false);
	m_btIdler.SetButtonImage(IDB_IDLER,hInstance,false,false);

	//创建控件
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+i);
		m_CardControlOx[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+GAME_PLAYER+i);
		m_CardControlOx[i].SetDrawHeight(DRAW_HEIGHT);
	}

    //按钮提示
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);


	//配置控件
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_CardControlOx[i].SetDisplayFlag(true);
		m_CardControlOx[i].SetCardSpace(18);
		m_CardControl[i].SetCardSpace(18);
	}

	m_JettonView.InitView(this);

	//获取大小
	m_sizeHistory.SetSize(m_PngHistoryScore.GetWidth(),m_PngHistoryScore.GetHeight());
	m_SizeFace.SetSize(48,48);

	//创建字体
	m_ViewFontEx.CreateFont(this,TEXT("宋体"),12,400);
		//控制按钮
	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,10,10),this,IDC_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	return 0;
}

//命令函数
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_HINT_OX:			//提示按钮
		{
			//发送消息
			SendEngineMessage(IDM_HINT_OX,0,0);
			return TRUE;
		}
	case IDC_START:				//开始按钮
		{
			//发送消息
			SendEngineMessage(IDM_START,0,0);
			return TRUE;
		}
	case IDC_READY:				//摊牌按钮
		{
			//发送消息
			SendEngineMessage(IDM_READY,0,0);
			return TRUE;
		}
	case IDC_OX:				//牛牛按钮
		{
			//发送消息
			SendEngineMessage(IDM_OX,0,0);
			return TRUE;
		}
	case IDC_RESORT:			//重排按钮
		{
			//发送消息
			SendEngineMessage(IDM_RESORT,0,0);
			return TRUE;
		}
	case IDC_SHORTCUT:			//快捷键按钮
		{
			//发送消息
			SendEngineMessage(IDM_SHORTCUT_KEY,0,0);
			return TRUE;
		}
	case IDC_BANKER:			//庄家按钮
		{
			//发送消息
			SendEngineMessage(IDM_BANKER,1,1);
			return TRUE;
		}
	case IDC_IDLER:				//闲家按钮
		{
			//发送消息
			SendEngineMessage(IDM_BANKER,0,0);
			return TRUE;
		}
	case IDC_YU_YIN:
		{
			//发送消息
			SendEngineMessage(IDM_YU_YIN,0,0);
			return TRUE;
		}
	}

	if(m_JettonView.OnCommand(wParam,lParam)==TRUE)
	{
		CRect rc=m_JettonView.GetRect();
		InvalidGameView(rc.left,rc.right,rc.Width(),rc.Height());
		return TRUE;
	}
	return __super::OnCommand(wParam, lParam);
}

//重置界面
VOID  CGameClientView::ResetGameView()
{
	//动画变量
	m_cbIndex=0;
	m_bJettonAction=false;
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_bOxValue[i]=0xff;
	}
	m_wSendCount=0;
	m_wSendIndex=0;
	m_cbDynamicJoin=FALSE;
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();
	ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));
	ZeroMemory(m_lMoveGold,sizeof(m_lMoveGold));

	//数据变量
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bOpenCardUser,sizeof(m_bOpenCardUser));
	ZeroMemory(m_bShowSocre,sizeof(m_bShowSocre));
	m_wBankerUser=INVALID_CHAIR;				
	m_bCallUser=0xff;
	m_bShow=false;
	m_bInvest=false;
	m_bOpenCard=false;
	m_bLookOnUser=false;
	m_bUserAction=false;
	m_bShowScore=false;
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	//删除定时器
	KillTimer(IDI_SEND_CARD);

	//隐藏按钮
	m_btScore.ShowWindow(SW_HIDE);
	m_btHintOx.ShowWindow(SW_HIDE);
	m_btStart.ShowWindow(SW_HIDE);
//	m_btReSort.ShowWindow(SW_HIDE);
	m_btOpenCard.ShowWindow(SW_HIDE);
	m_btOx.ShowWindow(SW_HIDE);

//	m_btShortcut.ShowWindow(SW_HIDE);
	m_btBanker.ShowWindow(SW_HIDE);
	m_btIdler.ShowWindow(SW_HIDE);
	m_btYuYin.EnableWindow(FALSE);

	//还原按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);

	//隐藏控件
//	m_ScoreView.ShowWindow(SW_HIDE);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_CardControl[i].SetDisplayFlag(false);
		//m_CardControlOx[i].SetDisplayFlag(false);
		m_CardControl[i].SetCardData(NULL,0);
		m_CardControlOx[i].SetCardData(NULL,0);
	}



	return ;
}

//调整控件
void CGameClientView::RectifyControl(int nWidth, int nHeight)
{
	SetTimer(IDI_UPDATA_GAME,1000,NULL);
	


	//发牌位置
	m_SendStartPos.SetPoint(nWidth/2,nHeight/2-30);

	m_ptAvatar[0].x=nWidth/2-m_SizeFace.cx/2;
	m_ptAvatar[0].y=9;
	m_ptNickName[0].x=nWidth/2+32;
	m_ptNickName[0].y=9; 
	m_ptJeton[0].x=nWidth/2-36;
	m_ptJeton[0].y=206;
	m_ptCard[0].x=m_ptJeton[0].x;
	m_ptCard[0].y=m_ptJeton[0].y-100; 
	m_ptReady[0].x=m_ptAvatar[0].x-10-69-4;
	m_ptReady[0].y=m_ptAvatar[0].y+(m_SizeFace.cy-48)/2+8;
	m_ptClock[0].x=m_ptAvatar[0].x+(m_SizeFace.cx-65)/2+32;
	m_ptClock[0].y=m_ptAvatar[0].y+m_SizeFace.cy+80;
	m_PointBanker[0].x = m_ptAvatar[0].x - 55;
	m_PointBanker[0].y =m_ptAvatar[0].y + 14;
	m_ptValue[0].x=nWidth/2+85;
	m_ptValue[0].y=129;

	m_ptAvatar[2].x=nWidth/2-m_SizeFace.cx/2;
	m_ptAvatar[2].y=nHeight-62;
	m_ptNickName[2].x=nWidth/2+32;
	m_ptNickName[2].y=nHeight-62;
	m_ptJeton[2].x=nWidth/2-36;
	m_ptJeton[2].y=nHeight-285;
	m_ptCard[2].x=m_ptJeton[2].x;
	m_ptCard[2].y=m_ptJeton[2].y+90;
	m_ptReady[2].x=m_ptAvatar[2].x-69-10-4;
	m_ptReady[2].y=m_ptAvatar[2].y+(m_SizeFace.cy-48)/2+8;
	m_ptClock[2].x=m_ptAvatar[2].x+(m_SizeFace.cx-65)/2+32;
	m_ptClock[2].y=m_ptAvatar[2].y+-57-140;
	m_PointBanker[2].x = m_ptAvatar[2].x - 54;
	m_PointBanker[2].y =m_ptAvatar[2].y +m_SizeFace.cy-33;
	m_ptValue[2].x=nWidth/2+85;
	m_ptValue[2].y=nHeight-161;

	m_ptAvatar[1].x=nWidth-m_SizeFace.cx-16;
	m_ptAvatar[1].y=nHeight/2-m_SizeFace.cy/2+5;
	m_ptNickName[1].x=m_ptAvatar[1].x;  
	m_ptNickName[1].y=m_ptAvatar[1].y+m_SizeFace.cy+6;
	m_ptJeton[1].x=nWidth-328+25;
	m_ptJeton[1].y=nHeight/2-49;
	m_ptCard[1].x=m_ptJeton[1].x+100; 
	m_ptCard[1].y=m_ptJeton[1].y;
	m_ptReady[1].x=m_ptAvatar[1].x-10-69-4;
	m_ptReady[1].y=m_ptAvatar[1].y+(m_SizeFace.cy-48)/2+8;
	m_ptClock[1].x=m_ptAvatar[1].x-80;
	m_ptClock[1].y=m_ptAvatar[1].y+(m_SizeFace.cy-57)/2+29;
	m_PointBanker[1].x = m_ptAvatar[1].x+5;
	m_PointBanker[1].y = m_ptAvatar[1].y-40;
	m_ptValue[1].x=nWidth-180;
	m_ptValue[1].y=nHeight/2+40;

	m_ptAvatar[3].x=16;
	m_ptAvatar[3].y=nHeight/2-m_SizeFace.cy/2+5;
	m_ptNickName[3].x=m_ptAvatar[3].x;
	m_ptNickName[3].y=m_ptAvatar[3].y+m_SizeFace.cy+6;
	m_ptJeton[3].x=263-30;
	m_ptJeton[3].y=nHeight/2-49;
	m_ptCard[3].x=m_ptJeton[3].x-100; 
	m_ptCard[3].y=m_ptJeton[3].y;
	m_ptReady[3].x=m_ptAvatar[3].x+m_SizeFace.cx+10+4;
	m_ptReady[3].y=m_ptAvatar[3].y+(m_SizeFace.cy-48)/2+8;
	m_ptClock[3].x=m_ptAvatar[3].x+m_SizeFace.cx+80;
	m_ptClock[3].y=m_ptAvatar[3].y+(m_SizeFace.cy-57)/2+29;
	m_PointBanker[3].x =m_ptAvatar[3].x+5;
	m_PointBanker[3].y =m_ptAvatar[3].y-40;
	m_ptValue[3].x=99;
	m_ptValue[3].y=nHeight/2+40;


	m_ptOffLine[0].SetPoint(m_ptAvatar[0].x+155,m_ptAvatar[0].y);
	m_ptOffLine[1].SetPoint(m_ptAvatar[1].x,m_ptAvatar[1].y-m_PngOffLine.GetHeight()-40);
	m_ptOffLine[2].SetPoint(m_ptAvatar[2].x+155,m_ptAvatar[2].y);
	m_ptOffLine[3].SetPoint(m_ptAvatar[3].x,m_ptAvatar[3].y-m_PngOffLine.GetHeight()-40);

	m_CardControl[0].SetBenchmarkPos(nWidth/2,80,enXCenter,enYTop); 
	m_CardControlOx[0].SetBenchmarkPos(nWidth/2,80-DRAW_HEIGHT,enXCenter,enYTop);
	m_CardControl[2].SetBenchmarkPos(nWidth/2,nHeight-68,enXCenter,enYBottom);
	m_CardControlOx[2].SetBenchmarkPos(nWidth/2,nHeight-68-DRAW_HEIGHT,enXCenter,enYBottom);
	m_CardControl[1].SetBenchmarkPos(nWidth-70,nHeight/2-30,enXRight,enYCenter);
	m_CardControlOx[1].SetBenchmarkPos(nWidth-80,nHeight/2-30-DRAW_HEIGHT,enXRight,enYCenter);
	m_CardControl[3].SetBenchmarkPos(74,nHeight/2-30,enXLeft,enYCenter);
	m_CardControlOx[3].SetBenchmarkPos(84,nHeight/2-30-DRAW_HEIGHT,enXLeft,enYCenter);


	//结束位置
	m_SendEndingPos[0].x=m_CardControl[0].GetCardPos().x;
	m_SendEndingPos[0].y=m_CardControl[0].GetCardPos().y;
	m_SendEndingPos[1].x=m_CardControl[1].GetCardPos().x;
	m_SendEndingPos[1].y=m_CardControl[1].GetCardPos().y;
	m_SendEndingPos[2].x=m_CardControl[2].GetCardPos().x;
	m_SendEndingPos[2].y=m_CardControl[2].GetCardPos().y;
	m_SendEndingPos[3].x=m_CardControl[3].GetCardPos().x;
	m_SendEndingPos[3].y=m_CardControl[3].GetCardPos().y;


	//牛值位置
	for(WORD i=0;i<GAME_PLAYER;i++)m_ptOpenTag[i]=m_ptValue[i]; 

	//按钮控件
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//开始按钮
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2-44,nHeight-170-56-20,0,0,uFlags);

	//控制按钮 
	m_btOpenCard.GetWindowRect(&rcButton);
	//DeferWindowPos(hDwp,m_btReSort,NULL,nWidth/2-32,nHeight-237,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOpenCard,NULL,nWidth/2+15,nHeight-237,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOx,NULL,nWidth/2-118,nHeight-237,0,0,uFlags);
	DeferWindowPos(hDwp,m_btHintOx,NULL,nWidth/2-102,nHeight-237,0,0,uFlags);

	//选庄按钮
	DeferWindowPos(hDwp,m_btBanker,NULL,nWidth/2-99,nHeight-198+10,0,0,uFlags);
	DeferWindowPos(hDwp,m_btIdler,NULL,nWidth/2+10,nHeight-198+10,0,0,uFlags);

	m_btYuYin.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btYuYin,NULL,nWidth/2,nHeight-rcButton.Height()-5,0,0,uFlags);


	//查分按钮
	CRect rcScore;
	m_btScore.GetWindowRect(&rcScore);
	DeferWindowPos(hDwp,m_btScore,NULL,nWidth-66,5,0,0,uFlags);

	//结束移动
	EndDeferWindowPos(hDwp);


	m_JettonView.RectifyControl(nWidth,nHeight);
	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景 
	DrawViewImage(pDC,m_ImageViewCenter,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);

	//绘画用户

	CRect rcUserInfo(0,0,0,0);
	UINT nTextFormat = DT_LEFT|DT_TOP|DT_END_ELLIPSIS|DT_NOCLIP;
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wUserTimer=GetUserClock(i);
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);

#ifdef DEBUG
		//int nTextWidth = (i%2==0?100:64);	
		//rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+18);
		//m_ViewFontEx.DrawText(pDC,TEXT("4564s54f5a4f4fd86f"),&rcUserInfo,RGB(255,255,255),nTextFormat);

		//rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+37,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+55);
		//m_ViewFontEx.DrawText(pDC,TEXT("4564s54f5a4f4fd86f"),&rcUserInfo,RGB(255,255,255),nTextFormat);

		//rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+19,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+37);
		//m_ViewFontEx.DrawText(pDC,TEXT("4564s54f5a4f4fd86f"),&rcUserInfo,RGB(255,255,255),nTextFormat);

		//m_PngOffLine.DrawImage(pDC,m_ptOffLine[i].x,m_ptOffLine[i].y);
#endif
		//绘画用户
		if (pIClientUserItem!=NULL)
		{
			TCHAR szBuffer[128]=TEXT("");

			//用户名字
			int nTextWidth = (i%2==0?100:64);		
			ZeroMemory(szBuffer,sizeof(szBuffer));
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),pIClientUserItem->GetNickName());
			rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+18);
			m_ViewFontEx.DrawText(pDC,szBuffer,&rcUserInfo,RGB(255,255,255),nTextFormat);
			
			//用户金币			
			ZeroMemory(szBuffer,sizeof(szBuffer));
			_sntprintf(szBuffer,CountArray(szBuffer),SCORE_STRING,pIClientUserItem->GetUserScore());
			rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+19,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+37);
			m_ViewFontEx.DrawText(pDC,szBuffer,&rcUserInfo,RGB(255,255,255),nTextFormat);

// 			rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+37,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+55);
// 			m_ViewFontEx.DrawText(pDC,pIClientUserItem->GetAddrDescribe(),&rcUserInfo,RGB(255,255,255),nTextFormat);

			CGameClientEngine *pGameClientDlg=(CGameClientEngine*)CGameFrameEngine::GetInstance();
			IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;

			if(CUserRight::IsGameCheatUser(pIClientKernel->GetUserAttribute()->dwUserRight))
			{
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%d"),pIClientUserItem->GetUserID());
				rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+65,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+83);
				m_ViewFontEx.DrawText(pDC,szBuffer,&rcUserInfo,RGB(255,255,255),nTextFormat);
			}

			//其他信息
			if (wUserTimer!=0)
			{
				if (m_bCallUser==0xff && (pIClientUserItem->GetUserStatus()==US_PLAYING||m_cbDynamicJoin))
					DrawUserTimerEx(pDC,nWidth/2,nHeight/2-60,wUserTimer,99L);
				else 
				{
					if(i==2&&pIClientUserItem->GetUserStatus()!=US_PLAYING)
						DrawUserTimerEx(pDC,m_ptAvatar[i].x-65-30-4,m_ptAvatar[2].y+(m_SizeFace.cy-57)/2+29,wUserTimer,99L);
					else
						DrawUserTimerEx(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer,99L);
				}
			}
			if (pIClientUserItem->GetUserStatus()==US_READY) 
			{
				DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
			}

			if(pIClientUserItem->GetUserStatus()==US_OFFLINE)
			{
				m_PngOffLine.DrawImage(pDC,m_ptOffLine[i].x,m_ptOffLine[i].y);
			}

			//绘画属性
			pDC->FillSolidRect(m_ptAvatar[i].x-2,m_ptAvatar[i].y-2,52,52,RGB(255,255,255));
			DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pIClientUserItem);
			//m_PngUserBack.DrawImage( pDC,m_ptAvatar[i].x-4,m_ptAvatar[i].y-4 );
			m_PngUserBack.AlphaDrawImage( pDC,m_ptAvatar[i].x-4,m_ptAvatar[i].y-4,255);
		}
	}

	//等待下注
	if (m_bInvest)
	{
		//m_PngWaitBet.DrawImage(pDC,nWidth/2-m_PngWaitBet.GetWidth()/2,nHeight/2-24);
		m_PngWaitBet.AlphaDrawImage(pDC,nWidth/2-m_PngWaitBet.GetWidth()/2,nHeight/2-24,255);
	}

	//等待摊牌
	if (m_bOpenCard)
	{
		//m_PngWaitOpen.DrawImage(pDC,nWidth/2-m_PngWaitOpen.GetWidth()/2,nHeight/2-24);
		m_PngWaitOpen.AlphaDrawImage(pDC,nWidth/2-m_PngWaitOpen.GetWidth()/2,nHeight/2-24,255);
	}

	//特殊牌型
	if (m_bUserAction)
	{
		//m_PngAutoOpen.DrawImage(pDC,nWidth/2-m_PngAutoOpen.GetWidth()/2,nHeight/2+90);//特殊牌型,系统自动为你配牌
		m_PngAutoOpen.AlphaDrawImage(pDC,nWidth/2-m_PngAutoOpen.GetWidth()/2,nHeight/2+90,255);
	}

	//等待叫庄
	if(m_bCallUser!=0xff/* && (m_bCallUser!=2 || m_bLookOnUser)*/)
	{
		//m_ImageWaitCall.DrawImage(pDC,nWidth/2-m_ImageWaitCall.GetWidth()/2,nHeight/2-24);
		m_ImageWaitCall.AlphaDrawImage(pDC,nWidth/2-m_ImageWaitCall.GetWidth()/2,nHeight/2-24,m_ImageWaitCall.GetWidth(),m_ImageWaitCall.GetHeight(),0,0,255);
	}

	//庄家信息
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		IClientUserItem * pIClientUserItem=GetClientUserItem(m_wBankerUser);
		if (pIClientUserItem!=NULL) 
			//m_ImageBanker.DrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y);
			m_ImageBanker.AlphaDrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y,255);
	}
 
	//绘画积分
	for (BYTE i=0;i<GAME_PLAYER;i++) 
	{
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		//m_lTableScore[i]=1000;
		if (pIClientUserItem!=NULL &&(m_lMoveGold[i]>COMPARE_ZERO||m_lMoveGold[i]<-COMPARE_ZERO|| m_lTableScore[i]>COMPARE_ZERO|| m_lTableScore[i]<-COMPARE_ZERO|| m_bShowSocre[i])) 
		{
			//调整位置
			int iPos=1;
			if(m_bShowSocre[i])
			{
				if(m_lTableScore[i]>COMPARE_ZERO)iPos=0;
				else iPos=2;
			}

			INT nX = m_ptJeton[i].x;
			INT nY =m_ptJeton[i].y;

			//绘画背景
			m_ImageScore.TransDrawImage(pDC,nX,nY,m_ImageScore.GetWidth()/3,
				m_ImageScore.GetHeight(),m_ImageScore.GetWidth()/3*iPos,0,RGB(255,0,255));

			//创建字体
			CFont ViewFont;
			ViewFont.CreateFont(-12,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("宋体"));
	
			//设置 DC
			pDC->SetTextAlign(TA_CENTER);
			CFont * pOldFont=pDC->SelectObject(&ViewFont);

			//绘画信息
			TCHAR tc[LEN_ACCOUNTS]=TEXT("");
			_sntprintf(tc,CountArray(tc),SCORE_STRING,m_lTableScore[i]);
			CRect rcDraw(nX,nY+6,nX+73,nY+28);
			DrawTextString(pDC,tc,RGB(250,250,0),RGB(0,0,0),&rcDraw);

			//清理资源
			pDC->SelectObject(pOldFont);
			ViewFont.DeleteObject();
		}
	}

	//显示摊牌标志
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		if (m_bOpenCardUser[i]==TRUE && i!=2) m_PngOpenTag.AlphaDrawImage(pDC,m_ptOpenTag[i].x,m_ptOpenTag[i].y,255);
	}

	//绘画扑克
	if (m_SendCardItemArray.GetCount()>0)
	{
		//变量定义
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//获取大小
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//绘画扑克
		int nXPos=m_SendCardPos.x-nItemWidth/2;
		int nYPos=m_SendCardPos.y-nItemHeight/2+10;
		m_ImageCard.TransDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
	}

	//显示点数
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bOxValue[i]!=0xff)
		{
			if(m_bShow || i==MY_VIEW_CHAIRID)
			{
				INT nXpt = m_ptValue[i].x;
				INT nypt = m_ptValue[i].y;
				if(m_bOxValue[i]!=0)
				{
					m_ImageOxValue.AlphaDrawImage(pDC,nXpt,nypt,255);
					if(m_bOxValue[i]==10 || m_bOxValue[i]>=OX_THREE_SAME)
					{
						m_ImageDoulbeOx.AlphaDrawImage(pDC,nXpt,nypt,255);
					}	
					else if(m_bOxValue[i]<10)
					{
						m_ImageNumber.AlphaDrawImage(pDC,nXpt+40,nypt+7,
							m_ImageNumber.GetWidth()/9,m_ImageNumber.GetHeight(),
							m_ImageNumber.GetWidth()/9*((int)m_bOxValue[i]-1),0,255);
					}
				}
				else if(m_bOxValue[i]==0)
				{				
					m_ImageOxValueZero.AlphaDrawImage(pDC,nXpt,nypt,255);
				}
			}
		}
	}

	//历史积分
	if (m_bShowScore==true)
	{
		//积分背景
		m_PngHistoryScore.AlphaDrawImage(pDC,nWidth-66-m_sizeHistory.cx,5,255);

		//计算位置
		INT nYBenchmark=5;
		INT nXBenchmark=nWidth-66-m_sizeHistory.cx;
		UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

		//绘画属性
		pDC->SetTextAlign(TA_LEFT|TA_TOP);

		//绘画信息
		for (WORD i=0;i<GAME_PLAYER;i++)
		{			
			//获取用户
			IClientUserItem * pIClientUserItem=GetClientUserItem(i);			
			//test
#ifdef _DEBUG
			//pIClientUserItem = GetClientUserItem(MYSELF_VIEW_ID);
#endif
			//end test
			if ((pIClientUserItem==NULL)||(m_pHistoryScore[i]==NULL)) continue;

			//变量定义
			COLORREF TextColor;

			//位置计算
			INT nY1=27,nY2=nY1+18,nSpa=20;
			CRect rcAccounts(nXBenchmark+4,nYBenchmark+nY1+i*nSpa,nXBenchmark+68+7,nYBenchmark+nY2+i*nSpa);
			CRect rcTurnScore(nXBenchmark+70+15,nYBenchmark+nY1+i*nSpa,nXBenchmark+121+15,nYBenchmark+nY2+i*nSpa);
			CRect rcCollectScore(nXBenchmark+140,nYBenchmark+nY1+i*nSpa,nXBenchmark+191,nYBenchmark+nY2+i*nSpa);

			//构造信息
			TCHAR szTurnScore[16]=TEXT(""),szCollectScore[16]=TEXT("");
			_sntprintf(szTurnScore,CountArray(szTurnScore),SCORE_STRING,m_pHistoryScore[i]->lTurnScore);
			_sntprintf(szCollectScore,CountArray(szCollectScore),SCORE_STRING,m_pHistoryScore[i]->lCollectScore);

			//绘画信息
			if(i==2) TextColor = RGB(255,255,0);
			else TextColor = RGB(255,255,255);
			m_ViewFontEx.DrawText(pDC,pIClientUserItem->GetNickName(),&rcAccounts,TextColor,nFormat);
			
			if(i==2) TextColor = RGB(255,153,0);
			else TextColor = RGB(0,255,153);
			m_ViewFontEx.DrawText(pDC,szTurnScore,&rcTurnScore,TextColor,nFormat);
			m_ViewFontEx.DrawText(pDC,szCollectScore,&rcCollectScore,TextColor,nFormat);
		}
	}

	m_JettonView.DrawJettonView(pDC,&m_ViewFontEx);
	return;
}

//绘画时间
void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
//#ifdef _DEBUG
//	return;
//#endif
	//获取属性
	const INT nNumberHeight=m_ImageTimeNumber.GetHeight();
	const INT nNumberWidth=m_ImageTimeNumber.GetWidth()/10;

	//计算数目
	LONG lNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		lNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	//位置定义
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

	//绘画背景
	CSize SizeTimeBack(m_ImageTimeBack.GetWidth(),m_ImageTimeBack.GetHeight());
	m_ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

	//绘画号码
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		WORD wCellNumber=wTime%10;
		m_ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//设置变量
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	}

	return;
}

//提示信息
void CGameClientView::SetUserAction(bool bWait)
{
	if(m_bUserAction!=bWait)
	{
		m_bUserAction=bWait;

		//更新界面
		InvalidGameView(0,0,0,0);
	}

	//设置时间
	if(bWait)
	{
		SetTimer(IDI_USER_ACTION,TIME_USER_ACTION,NULL);
	}
	else 
	{
		KillTimer(IDI_USER_ACTION);
	}

	return;
}

//动画判断
bool CGameClientView::IsDispatchCard()
{
	return m_SendCardItemArray.GetCount()>0;
}

//完成发牌
void CGameClientView::FinishDispatchCard()
{
	//完成判断
	if (m_SendCardItemArray.GetCount()==0) return;

	//完成动画
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//获取数据
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//获取扑克
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//设置扑克
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

	//删除处理
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

//发送扑克
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//效验参数
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//设置变量
	tagSendCardItem SendCardItem;
	SendCardItem.cbCardData=cbCardData;
	SendCardItem.wChairID=wChairID;

	//插入数组
	m_SendCardItemArray.Add(SendCardItem);

	//启动动画
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//设置位置
		m_SendCardPos.x=m_SendStartPos.x;
		m_SendCardPos.y=m_SendStartPos.y;

		//位移计算
		int nXCount=abs(m_SendEndingPos[SendCardItem.wChairID].x-m_SendStartPos.x)/SEND_PELS;
		int nYCount=abs(m_SendEndingPos[SendCardItem.wChairID].y-m_SendStartPos.y)/SEND_PELS;

		//设置次数
		m_wSendIndex=0;
		m_wSendCount=__max(1,__max(nXCount,nYCount));

		//设置定时器
		SetTimer(IDI_SEND_CARD,TIME_SENDSPEED,NULL);

		//播放声音
		CGameFrameEngine * pGameClientEngine=CGameClientEngine::GetInstance();
		if(pGameClientEngine!=NULL)
		pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//设置下注
void CGameClientView::SetUserTableScore(WORD wChairID, SCORE lTableScore,bool bMove)
{
	//设置数据
	if (wChairID<GAME_PLAYER) 
	{
		if(bMove)DrawMoveAnte(wChairID,lTableScore);
		else m_lTableScore[wChairID]=lTableScore;
	}
	else ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//更新界面
	InvalidGameView(0,0,0,0);
	return;
}

//庄家标志
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	m_wBankerUser=wBankerUser;

	//更新界面
	InvalidGameView(0,0,0,0);
	return;
}

//等待叫庄
void CGameClientView::SetWaitCall(BYTE bCallUser)
{
	m_bCallUser=bCallUser;

	//更新界面
	InvalidGameView(0,0,0,0);

	return ;
}

//等待标志
void CGameClientView::SetWaitInvest(bool bInvest)
{
	if(m_bInvest!=bInvest)
	{
		m_bInvest=bInvest;

		//更新界面
		InvalidGameView(0,0,0,0);
	}
	return;
}

//左上信息
//void CGameClientView::SetScoreInfo(LONGLONG lTurnMaxScore,LONGLONG lTurnLessScore)
//{
//	m_lTurnMaxScore=lTurnMaxScore;
//	//m_lTurnLessScore=lTurnLessScore;
//
//	//更新界面
//	InvalidGameView(0,0,0,0);
//	return;
//}

//显示牌型
void CGameClientView::DisplayTypeEx(bool bShow)
{
	if(m_bShow!=bShow)
	{
		m_bShow=bShow;

		//更新界面
		InvalidGameView(0,0,0,0);
	}

	return;
}

//显示点数
void CGameClientView::SetUserOxValue(WORD wChiarID,BYTE bValue)
{
	//牛牛数据
	m_bOxValue[wChiarID]=bValue;

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

//显示摊牌
void CGameClientView::ShowOpenCard(WORD wChiarID,BOOL bShow)
{
	//摊牌用户
	m_bOpenCardUser[wChiarID]=bShow;

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

//显示积分
void CGameClientView::ShowScore(WORD wChairID,bool bShow)
{
	//显示标志
	m_bShowSocre[wChairID]=bShow;

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//变量定义
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	//变量定义
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		m_ViewFontEx.DrawText(pDC,pszString,&rcDraw,crFrame,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//绘画字体
	rcDraw.CopyRect(lpRect);
	pDC->SetTextColor(crText);
	m_ViewFontEx.DrawText(pDC,pszString,&rcDraw,crText,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//积分按钮
VOID CGameClientView::OnBnClickedScore()
{
	//设置变量
	m_bShowScore=!m_bShowScore;

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage((m_bShowScore==true)?IDB_BT_CLOSE_SCORE:IDB_BT_SHOW_SCORE,hInstance,false,false);

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

//设置积分
bool CGameClientView::SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore)
{
	//效验参数
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//设置变量
	m_pHistoryScore[wChairID]=pHistoryScore;

	//更新界面
	InvalidGameView(0,0,0,0);

	return true;
}

//绘制动画, 0: 底注动画, 1: 加注动画, 2: 加注合并到底注, 3: 赢家收筹码
void CGameClientView::DrawMoveAnte( WORD wViewChairID, SCORE lTableScore)
{
	//动画步数
	int nAnimeStep = 60;

	m_JettonStatus[wViewChairID].wMoveIndex = 0;
	m_lMoveGold[wViewChairID]=lTableScore;

	m_JettonStatus[wViewChairID].ptFrom = m_ptCard[wViewChairID]; 
	m_JettonStatus[wViewChairID].ptDest = m_ptJeton[wViewChairID];
	m_JettonStatus[wViewChairID].ptCourse= m_ptCard[wViewChairID];
	m_JettonStatus[wViewChairID].lGold =lTableScore;

	//位移计算
	INT nXCount=abs(m_JettonStatus[wViewChairID].ptDest.x-m_JettonStatus[wViewChairID].ptFrom.x)/nAnimeStep+rand()%8;
	INT nYCount=abs(m_JettonStatus[wViewChairID].ptDest.y-m_JettonStatus[wViewChairID].ptFrom.y)/nAnimeStep+rand()%8;
	m_JettonStatus[wViewChairID].wMoveCount = __max(1,__max(nXCount,nYCount));;

	//设置时间
	if(!m_bJettonAction)
	{
		m_bJettonAction = true;
		SetTimer(IDI_JETTON_ACTION,50,NULL);	//--------以前是50
	}

	return;
}

//移动筹码
bool CGameClientView::MoveJetton()
{
	bool bAllClean = true;

	//设置变量
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		//移动步数
		if(m_JettonStatus[i].wMoveIndex<m_JettonStatus[i].wMoveCount)
		{
			bAllClean = false;
			m_JettonStatus[i].wMoveIndex++;
			WORD wMoveIndex = m_JettonStatus[i].wMoveIndex;
			WORD wMoveCount = m_JettonStatus[i].wMoveCount;
			m_JettonStatus[i].ptCourse.x = m_JettonStatus[i].ptFrom.x + (m_JettonStatus[i].ptDest.x-m_JettonStatus[i].ptFrom.x)*wMoveIndex/wMoveCount;
			m_JettonStatus[i].ptCourse.y = m_JettonStatus[i].ptFrom.y + (m_JettonStatus[i].ptDest.y-m_JettonStatus[i].ptFrom.y)*wMoveIndex/wMoveCount;
		}
		else if(m_JettonStatus[i].wMoveCount>0)
		{
			//筹码处理
			m_lTableScore[i] = m_JettonStatus[i].lGold ;

			//清理信息
			m_lMoveGold[i]=0;
			ZeroMemory(&m_JettonStatus[i],sizeof(m_JettonStatus[i]));
		}
	}

	if(bAllClean) m_bJettonAction = false;

	//更新界面
	InvalidGameView(0,0,0,0);

	return !bAllClean;
}

//绘制用户图像
void CGameClientView::DrawUserReadyEx(CDC *pDC,int nXPos,int nYPos)
{
	m_PngReady.DrawImage(pDC,nXPos,nYPos);
}


//定时器消息
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//发送处理
	switch(nIDEvent)
	{
	case IDI_JETTON_ACTION:	//筹码动作
		{
			bool bKillTime = true;

			//筹码移动
			if(m_bJettonAction && MoveJetton()) bKillTime = false;

			//筹码移动
			//if(m_bCardAction && MoveCard()) bKillTime = false;

			//删除定时器
			if(bKillTime)
			{
				m_bJettonAction = false;
				//m_bCardAction = false;
				KillTimer(IDI_JETTON_ACTION);
			}

			return;
		}
	case IDI_UPDATA_GAME:	//更新游戏
		{
			m_cbIndex++;

			//更新界面
			InvalidGameView(0,0,0,0);

			return ;
		}
	case IDI_SEND_CARD:		//移动扑克
		{
			//设置索引
			m_wSendIndex++;

			//设置位置
			tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
			if(pSendCardItem->wChairID%2!=0)
				m_SendCardPos.x=m_SendStartPos.x+(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)*m_wSendIndex/m_wSendCount;
			else
				m_SendCardPos.y=m_SendStartPos.y+(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)*m_wSendIndex/m_wSendCount;

			//停止判断
			if (m_wSendIndex>=m_wSendCount)
			{
				//获取扑克
				BYTE cbCardData[MAX_COUNT];
				WORD wChairID=pSendCardItem->wChairID;
				BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

				//设置扑克
				cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
				m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);

				//删除动画
				m_SendCardItemArray.RemoveAt(0);

				//继续动画
				if (m_SendCardItemArray.GetCount()>0)
				{
					//变量定义
					tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

					//设置位置
					m_SendCardPos.x=m_SendStartPos.x;
					m_SendCardPos.y=m_SendStartPos.y;

					//位移计算
					int nXCount=abs(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)/SEND_PELS;
					int nYCount=abs(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)/SEND_PELS;

					//设置次数
					m_wSendIndex=0;
					m_wSendCount=__max(1,__max(nXCount,nYCount));

					//播放声音
					CGameFrameEngine * pGameClientEngine=CGameClientEngine::GetInstance();
					if(pGameClientEngine!=NULL)
					pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
				}
				else 
				{
					//完成处理
					KillTimer(IDI_SEND_CARD);
					SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);
				}
			}
			
			//更新界面
			InvalidGameView(0,0,0,0);

			return ;
		}
	case IDI_USER_ACTION: 	//用户动作
		{
			//清理数据
			m_bUserAction = false;
			KillTimer(IDI_USER_ACTION);

			//更新界面
			InvalidGameView(0,0,0,0);

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
//管理员控制
void CGameClientView::OpenAdminWnd()
{
	//如果有此权限
	CGameClientEngine *pGameClientDlg=(CGameClientEngine*)CGameFrameEngine::GetInstance();
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;
	if(CUserRight::IsGameCheatUser(pIClientKernel->GetUserAttribute()->dwUserRight))
	{
		if (NULL==m_AdminDlg.m_hWnd) m_AdminDlg.Create(IDD_DIALOG_ADMIN,this);

		if(!m_AdminDlg.IsWindowVisible()) m_AdminDlg.ShowWindow(SW_SHOW);
		else m_AdminDlg.ShowWindow(SW_HIDE);
	}
}
