#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClient.h"

//////////////////////////////////////////////////////////////////////////
//按钮标识 

#define IDC_START						100								//开始按钮
#define IDC_HAI_DI						101								//海底按钮
#define IDC_NO_HAI_DI					102								//放弃按钮
#define IDC_TRUSTEE_CONTROL				104								//托管控制
#define IDC_TING_CARD                   105                             //听牌按钮
#define IDC_NO_TING_CARD				106								//不听按钮
#define IDC_AUTOPLAY_ON                 111                             //托管按钮
#define IDC_AUTOPLAY_OFF                112                             //取消托管
#define IDC_SORT_CARD                   113                             //排序按钮

//动作标识
#define IDI_BOMB_EFFECT					101								//动作标识
#define IDI_DISC_EFFECT					102								//丢弃效果
#define IDI_MOVE_CARD					103								//

//动作数目
#define BOMB_EFFECT_COUNT				12								//动作数目
#define DISC_EFFECT_COUNT				8								//丢弃效果		

#define MOVE_STEP_COUNT					4								//移动步数
#define TIME_MOVE_CARD					5								//牌动画定时时间

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL,OnStusteeControl)
	//ON_BN_CLICKED(IDC_HAI_DI, OnHaiDi)
	//ON_BN_CLICKED(IDC_NO_HAI_DI, OnNoHaiDi)
	//ON_BN_CLICKED(IDC_TING_CARD,OnTingCard)
	//ON_BN_CLICKED(IDC_NO_TING_CARD, OnNoTingCard)
	ON_BN_CLICKED(IDC_AUTOPLAY_ON, OnAutoPlayerOn)
	ON_BN_CLICKED(IDC_AUTOPLAY_OFF,OnAutoPlayerOff)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView()
{
	//标志变量
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

	//游戏属性
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbFengQuan = GAME_PLAYER;

	//动作动画
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//丢弃效果
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//用户状态
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//堆立变量
	m_wHeapHand = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;

	//出牌动画变量
	m_bEnableAnimate = true;
	m_bCardMoving = false;

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageWait.LoadImage( hInstance,TEXT("WAIT_TIP") );
	m_ImageBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	m_ImageUserFlag.LoadImage( hInstance,TEXT("USER_FLAG") );
	m_ImageOutCard.LoadImage( hInstance,TEXT("OUT_CARD_TIP") );
	m_ImageActionBack.LoadImage( hInstance,TEXT("ACTION_BACK") );
	m_ImageHuangZhuang.LoadFromResource(hInstance,IDB_HUANG_ZHUANG);
	m_ImageTrustee.LoadImage(hInstance,TEXT("TRUSTEE"));
	m_ImageActionAni.LoadImage(AfxGetInstanceHandle(),TEXT("ActionAni"));
	m_ImageDisc.LoadImage(AfxGetInstanceHandle(),TEXT("DISC"));
	m_ImageArrow.LoadImage(AfxGetInstanceHandle(),TEXT("ARROW"));
	m_ImageCenter.LoadFromResource(hInstance,IDB_VIEW_CENTER);
	m_PngFengQuan.LoadImage( hInstance,TEXT("DRAW_WIND") );
	m_PngListenFlag.LoadImage( hInstance,TEXT("LISTEN_FLAG") );
	m_ImageDiscKuang.LoadImage(hInstance,TEXT("DISCKUANG"));
	m_ImageZhuang.LoadImage(hInstance,TEXT("ZHUANGSAI"));
}

//析构函数
CGameClientView::~CGameClientView(void)
{
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//变量定义
	enDirection Direction[]={Direction_North,Direction_West,Direction_South,Direction_East};

	//设置控件
	for (WORD i=0;i<4;i++)
	{
		//用户扑克
		m_HeapCard[i].SetDirection(Direction[i]);
		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);
		m_WindCard[i].SetDirection(Direction[i]);

		//组合扑克
		for( BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++ )
		{
			m_WeaveCard[i][j].SetDisplayItem(true);
			m_WeaveCard[i][j].SetDirection(Direction[i]);
		}
	}

	//设置控件
	m_UserCard[0].SetDirection(Direction_North);
	m_UserCard[1].SetDirection(Direction_West);
	m_UserCard[2].SetDirection(Direction_East);

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);
	m_DrawSiceWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,11,NULL);
	m_DrawSiceWnd.SetWindowSink(AfxGetMainWnd());
	m_GameScore.Create( IDD_GAME_SCORE,this );

	//创建控件
	m_btStart.Create(NULL,WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false,false);

	//托管按钮
	//m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);
	//m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);

#ifdef VIDEO_GAME
	//创建视频
	for (WORD i=0;i<4;i++)
	{
		//创建视频
		m_DlgVedioService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,300+i);
		m_DlgVedioService[i].InitVideoService(i==2,true);

		//设置视频
		g_VedioServiceManager.SetVideoServiceControl(i,&m_DlgVedioService[i]);
	}
#endif

	return 0;
}

//重置界面
void CGameClientView::ResetGameView()
{
	//标志变量
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

	//游戏属性
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbFengQuan = GAME_PLAYER;

	//动作动画
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//丢弃效果
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//用户状态
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//堆立变量
	m_wHeapHand = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;

	//出牌动画变量
	m_bEnableAnimate = true;
	m_bCardMoving = false;

	//界面设置
	m_btStart.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameScore.RestorationData();
	
	//禁用控件
	//m_btStusteeControl.EnableWindow(FALSE);

	//扑克设置
	m_UserCard[0].SetCardData(0,false);
	m_UserCard[1].SetCardData(0,false);
	m_UserCard[2].SetCardData(0,false);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);
	m_HandCardControl.SetCardData(NULL,0,0);

	//扑克设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HeapCard[i].SetHeapCardInfo(0,0);
		m_TableCard[i].SetCardData(NULL,0);
		m_DiscardCard[i].SetCardData(NULL,0);
		m_WindCard[i].SetCardData(NULL,0);
		for( BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++ )
			m_WeaveCard[i][j].SetCardData(NULL,0);
	}

	//销毁定时器
	//KillTimer(IDI_DISC_EFFECT);
	KillTimer(IDI_BOMB_EFFECT);

	return;
}

//调整控件
void CGameClientView::RectifyControl(int nWidth, int nHeight)
{
	int m_nXFace = 50,m_nYFace = 50;//////////////
	int m_nXTimer = 60,m_nYTimer = 60;
	int m_nXBorder = 0,m_nYBorder = 0;

	//设置坐标
	m_ptAvatar[0].x=nWidth/2-m_nXFace;
	m_ptAvatar[0].y=5+m_nYBorder;
	m_ptNickName[0].x=nWidth/2+5;
	m_ptNickName[0].y=10+m_nYBorder;
	m_ptClock[0].x=nWidth/2-m_nXFace-m_nXTimer-2;
	m_ptClock[0].y=17+m_nYBorder;
	m_ptReady[0].x=nWidth/2-m_nXFace-110;
	m_ptReady[0].y=m_nYBorder+20;
	m_UserFlagPos[0].x=nWidth/2-m_nXFace-m_ImageUserFlag.GetWidth()/4-10;
	m_UserFlagPos[0].y=m_nYBorder+5;
	m_PointTrustee[0].x=nWidth/2-m_nXFace-105;
	m_PointTrustee[0].y=5+m_nYBorder+20;
	m_UserListenPos[0].x=nWidth/2-m_nXFace+5;
	m_UserListenPos[0].y=m_nYBorder+m_nYFace+20;
	//SetFlowerControlInfo( 0,m_ptAvatar[0].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptAvatar[0].y+m_nYFace );

	m_ptAvatar[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptAvatar[1].y=nHeight/2-m_nYFace-50;
	m_ptNickName[1].x=nWidth-m_nXBorder-5;
	m_ptNickName[1].y=nHeight/2-45;
	m_ptClock[1].x=nWidth-m_nXBorder-m_nXFace+5;
	m_ptClock[1].y=nHeight/2-m_nYFace-105;
	m_ptReady[1].x=nWidth-m_nXBorder-32;
	m_ptReady[1].y=nHeight/2-m_nYFace-90;
	m_UserFlagPos[1].x=m_ptAvatar[1].x-m_ImageUserFlag.GetWidth()/4-10;
	m_UserFlagPos[1].y=m_ptAvatar[1].y;
	m_PointTrustee[1].x=nWidth-m_nXBorder-44;
	m_PointTrustee[1].y=nHeight/2-m_nYFace-105;
	m_UserListenPos[1].x=nWidth-m_nXBorder-m_nXFace-85;
	m_UserListenPos[1].y=nHeight/2-m_nYFace-50;
	//SetFlowerControlInfo( 1,m_ptAvatar[1].x-BIG_FACE_WIDTH,m_ptAvatar[1].y+m_nYFace/2-BIG_FACE_HEIGHT/2 );

	m_ptAvatar[2].x=nWidth/2-m_nXFace;
	m_ptAvatar[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptNickName[2].x=nWidth/2+5;
	m_ptNickName[2].y=nHeight-m_nYBorder-m_nYFace;
	m_ptClock[2].x=nWidth/2-m_nXFace/2-m_nXTimer-2;
	m_ptClock[2].y=nHeight-m_nYBorder-m_nYTimer-8+40;
	m_ptReady[2].x=nWidth/2-m_nXFace-110;
	m_ptReady[2].y=nHeight-m_nYBorder-20;
	m_UserFlagPos[2].x=nWidth/2-m_nXFace-m_ImageUserFlag.GetWidth()/4-10;
	m_UserFlagPos[2].y=nHeight-m_nYBorder-m_ImageUserFlag.GetHeight();
	m_PointTrustee[2].x=nWidth/2-m_nXFace-105;
	m_PointTrustee[2].y=nHeight-m_nYBorder-23;
	m_UserListenPos[2].x=nWidth/2-m_nXFace;
	m_UserListenPos[2].y=nHeight-m_nYBorder-140;
	//SetFlowerControlInfo( 2,m_ptAvatar[2].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptAvatar[2].y-BIG_FACE_HEIGHT );

	m_ptAvatar[3].x=m_nXBorder+5;
	m_ptAvatar[3].y=nHeight/2-m_nYFace-50;
	m_ptNickName[3].x=m_nXBorder+5;
	m_ptNickName[3].y=nHeight/2-45;
	m_ptClock[3].x=m_nXBorder+22;
	m_ptClock[3].y=nHeight/2-m_nYFace-105;
	m_ptReady[3].x=m_nXBorder+32;
	m_ptReady[3].y=nHeight/2-m_nYFace-90;
	m_UserFlagPos[3].x=m_ptAvatar[3].x+m_nXFace+10;
	m_UserFlagPos[3].y=m_ptAvatar[3].y;
	m_PointTrustee[3].x=m_nXBorder+43;
	m_PointTrustee[3].y=nHeight/2-m_nYFace-105;
	m_UserListenPos[3].x=m_nXBorder+m_nXFace+15;
	m_UserListenPos[3].y=nHeight/2-m_nYFace-50;
	//SetFlowerControlInfo( 3,m_ptAvatar[3].x+m_nXFace,m_ptAvatar[3].y+m_nYFace/2-BIG_FACE_HEIGHT/2 );

	//用户扑克
	m_UserCard[0].SetControlPoint(nWidth/2-240,m_nYBorder+m_nYFace+15);
	m_UserCard[1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-99,nHeight/2-225);
	m_UserCard[2].SetControlPoint(m_nXBorder+m_nXFace+80,nHeight/2+170);
	m_HandCardControl.SetBenchmarkPos(nWidth/2+100-(939-nWidth)/2,nHeight-m_nYFace-m_nYBorder-17,enXCenter,enYBottom);

	//桌面扑克
	m_TableCard[0].SetControlPoint(nWidth/2+200,m_nYBorder+m_nYFace+10);
	m_TableCard[1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-102,nHeight/2+90);
	m_TableCard[2].SetControlPoint(nWidth/2-200,nHeight-m_nYFace-m_nYBorder-63);
	m_TableCard[3].SetControlPoint(m_nXBorder+m_nXFace+62+15,nHeight/2-145);

	//堆积扑克
	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2-40;
	m_HeapCard[0].SetControlPoint(nXCenter-165,nYCenter-190+15);
	m_HeapCard[1].SetControlPoint(nXCenter+205-10-15-20+2,nYCenter-140-5);
	m_HeapCard[2].SetControlPoint(nXCenter+155,nYCenter+190-15-10-10);
	m_HeapCard[3].SetControlPoint(nXCenter-237+30+10,nYCenter+155);

	//丢弃扑克
	m_DiscardCard[0].SetControlPoint(nXCenter+112,nYCenter-145);
	m_DiscardCard[1].SetControlPoint(nXCenter+133,nYCenter+105);
	m_DiscardCard[2].SetControlPoint(nXCenter-120,nYCenter+130);
	m_DiscardCard[3].SetControlPoint(nXCenter-168+3,nYCenter-80);

	//风牌扑克
	m_WindCard[0].SetControlPoint(nWidth/2+155,nHeight/2-245);
	m_WindCard[1].SetControlPoint(nWidth/2+198,nHeight/2+135);
	m_WindCard[2].SetControlPoint(nWidth/2-240,nHeight/2+150,true);
	m_WindCard[3].SetControlPoint(nWidth/2-228,nHeight/2-205);


	//组合扑克
	int nXControl = nWidth/2+205;
	int nYControl = m_nYBorder+m_nYFace+15;
	for( BYTE i = 0; i < CountArray(m_WeaveCard[0]); i++ )
	{
		m_WeaveCard[0][i].SetControlPoint(nXControl,nYControl);
		nXControl -= 75;
	}

	//组合扑克
	nXControl = nWidth-m_nXBorder-m_nXFace-105;
	nYControl = nHeight/2+130;
	for( BYTE i = 0; i < CountArray(m_WeaveCard[1]); i++ )
	{
		m_WeaveCard[1][i].SetControlPoint(nXControl,nYControl);
		nYControl -= 75;
	}

	//组合扑克
	nXControl = nWidth/2-320;
	nYControl = nHeight-m_nYFace-m_nYBorder-68;
	for( BYTE i = 0; i < CountArray(m_WeaveCard[2]); i++ )
	{
		m_WeaveCard[2][i].SetControlPoint(nXControl,nYControl);
		nXControl += 125;
	}

	//组合扑克
	nXControl = m_nXBorder+m_nXFace+77;
	nYControl = nHeight/2-200;
	for( BYTE i = 0; i < CountArray(m_WeaveCard[3]); i++ )
	{
		m_WeaveCard[3][i].SetControlPoint(nXControl,nYControl);
		nYControl += 75;
	}

	//设置牌距
	/*for (WORD i=0;i<GAME_PLAYER;i++)
	{
		for (BYTE j=0;j<4;j++)
		{
			if(m_WeaveCard[i][j].GetCardCount()==4 )
			{
				for (BYTE t=j+1;t<4;t++)
				{
					m_WeaveCard[i][t].OffPoint(i==2?true:false);
				}
			}

		}
	}*/


	//色子窗口
	CRect rcSice;
	m_DrawSiceWnd.GetWindowRect(&rcSice);
	m_DrawSiceWnd.SetBenchmarkPos((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2);
	m_DrawSiceWnd.MoveWindow((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2,rcSice.Width(),rcSice.Height());

	//移动按钮
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//移动调整
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-m_nYBorder-155,0,0,uFlags);
	//移动调整
	//DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-rcButton.Height(),0,0,uFlags);

	//控制窗口
	CRect rcControlWnd;
	m_ControlWnd.GetWindowRect(&rcControlWnd);
	DeferWindowPos(hDwp,m_ControlWnd,NULL,nWidth-rcControlWnd.Width()-12,nHeight-rcControlWnd.Height()-130,0,0,uFlags);

#ifdef VIDEO_GAME
	//视频窗口
	CRect rcAVDlg;
	m_DlgVedioService[0].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[1],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2+10,0,0,uFlags);
	DeferWindowPos(hDwp,m_DlgVedioService[3],NULL,m_nXBorder+5,nHeight/2+10,0,0,uFlags);
	DeferWindowPos(hDwp,m_DlgVedioService[0],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),5,0,0,uFlags);
	m_DlgVedioService[2].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[2],NULL,m_nXBorder+5,nHeight-m_nYBorder-135-rcAVDlg.Height(),0,0,uFlags);
#endif

	//结束移动
	EndDeferWindowPos(hDwp);

	CRect rcGameScore;
	m_GameScore.GetWindowRect(&rcGameScore);
	CPoint ptPos( (nWidth-rcGameScore.Width())/2,(nHeight-rcGameScore.Height())*2/5-30 );
	ClientToScreen( &ptPos );
	m_GameScore.SetWindowPos( NULL,ptPos.x,ptPos.y,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOCOPYBITS );

	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	int m_nXBorder = 0,m_nYBorder = 0;/////////

	//绘画背景
	DrawViewImage(pDC,m_ImageBack,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageCenter,DRAW_MODE_CENTENT);

	//绘画用户
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		//const tagUserData * pUserData=GetUserInfo(i);
		IClientUserItem * pUserData=GetClientUserItem(i);
		//test	
		
		/*tagUserData *pUserData = new tagUserData;
		lstrcpy(pUserData->szName,TEXT("SSSSSssssssssssss"));
		pUserData->cbUserStatus = US_READY;*/
		
		//end test
		if (pUserData!=NULL)
		{
			//用户名字
			pDC->SetTextAlign((i==1)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			DrawTextString(pDC,pUserData->GetNickName(),(i==2?RGB(152,204,253):RGB(254,230,92)),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y);

			//玩家积分
			CString strScore;
			strScore.Format(_T("%I64d"), (long long)pUserData->GetUserScore());
			DrawTextString(pDC,strScore,RGB(207,217,128),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y+15);

			//其他信息
			WORD wUserTimer=GetUserClock(i);
			//test
			//wUserTimer = 15;
			//end test
			/*if ((wUserTimer!=0)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				DrawUserTimerEx(pDC,nWidth/2,nHeight/2-10,wUserTimer);
				if(m_wCurrentUser==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-10,nHeight/2-15-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				else if(m_wCurrentUser==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4+5,nHeight/2-25,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				else if(m_wCurrentUser==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-10,nHeight/2-10+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				else if(m_wCurrentUser==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2-5,nHeight/2-25,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
			}
			if((wUserTimer!=0)&&(m_wCurrentUser==INVALID_CHAIR))
			{
				DrawUserTimerEx(pDC,nWidth/2,nHeight/2-10,wUserTimer);
				if(i==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-10,nHeight/2-15-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				else if(i==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4+5,nHeight/2-25,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				else if(i==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-10,nHeight/2-10+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				else if(i==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2-5,nHeight/2-25,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
			}
			if ((wUserTimer!=0)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				DrawUserTimerEx(pDC,nWidth/2,nHeight/2-10,wUserTimer);
				if(m_wCurrentUser==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-10,nHeight/2-15-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				else if(m_wCurrentUser==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4+5,nHeight/2-25,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				else if(m_wCurrentUser==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-10,nHeight/2-10+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				else if(m_wCurrentUser==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2-5,nHeight/2-25,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
			}
			*/
			if(wUserTimer!=0)
			{
				DrawUserTimerEx(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y,wUserTimer);
			}
			if (pUserData->GetUserStatus()==US_READY) 
				DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
			///DrawUserFace(pDC,pUserData,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserData->cbUserStatus==US_OFFLINE);

			//托管
			/*if(m_bTrustee[i])
			{
				m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
			}*/
		}
		//test
		//delete pUserData;
		//end test
	}

	//圈风标志
	/*if( m_cbFengQuan < GAME_PLAYER )
	{
		m_PngFengQuan.DrawImage( pDC,m_nXBorder+10,m_nYBorder+10,m_PngFengQuan.GetWidth(),m_PngFengQuan.GetHeight()/4,
			0,m_cbFengQuan*m_PngFengQuan.GetHeight()/4 );
	}*/

	//用户标志
	//test
	//m_wBankerUser = 3;
	//end test
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//加载位图
		int nImageWidth=m_ImageUserFlag.GetWidth()/4;
		int nImageHeight=m_ImageUserFlag.GetHeight();

		//绘画标志
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wIndex=(m_wBankerUser+GAME_PLAYER-i)%GAME_PLAYER;
			m_ImageUserFlag.DrawImage(pDC,m_UserFlagPos[wIndex].x,m_UserFlagPos[wIndex].y,nImageWidth,nImageHeight,nImageWidth*i,0);
		}

		/*if(m_wBankerUser%2==0)
		{
			m_ImageZhuang.DrawImage(pDC,m_ptReady[m_wBankerUser].x-55,m_ptReady[m_wBankerUser].y-10);
		}
		if(m_wBankerUser==1)
		{
			m_ImageZhuang.DrawImage(pDC,m_UserFlagPos[m_wBankerUser].x-40,m_UserFlagPos[m_wBankerUser].y);
		}
		if(m_wBankerUser==3)
		{
			m_ImageZhuang.DrawImage(pDC,m_UserFlagPos[m_wBankerUser].x+40,m_UserFlagPos[m_wBankerUser].y);
		}*/
	}

	//丢弃扑克
	m_DiscardCard[0].DrawCardControl( pDC );
	m_DiscardCard[1].DrawCardControl( pDC );
	m_DiscardCard[2].DrawCardControl( pDC );
	m_DiscardCard[3].DrawCardControl( pDC );

	//用户扑克
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[1].DrawCardControl(pDC);
	m_UserCard[2].DrawCardControl(pDC);
	
	//堆积扑克
	m_HeapCard[0].DrawCardControl(pDC);
	m_HeapCard[1].DrawCardControl(pDC);
	m_HeapCard[3].DrawCardControl(pDC);
	m_HeapCard[2].DrawCardControl(pDC);

	//花牌扑克
	m_WindCard[0].DrawCardControl(pDC);
	m_WindCard[1].DrawCardControl(pDC);
	m_WindCard[2].DrawCardControl(pDC);
	m_WindCard[3].DrawCardControl(pDC);


	//桌面扑克
	for (WORD i=0;i<4;i++)
	{
		m_TableCard[i].DrawCardControl(pDC);
		for( BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++ )
		{
			if(i==2)
				m_WeaveCard[i][j].DrawCardControl(pDC,true,true);
			else
				m_WeaveCard[i][j].DrawCardControl(pDC,true);

		}
	}

	//丢弃效果
	if(m_wDiscUser!=INVALID_CHAIR)
	{
		CPoint pt=m_DiscardCard[m_wDiscUser].GetLastCardPosition();

		if(pt.x!=0||pt.y!=0)
		{
			////绘画信息
			//m_ImageDisc.DrawImage(pDC,pt.x,pt.y,SizeDisc.cx,SizeDisc.cy,
			//	m_cbDiscFrameIndex*SizeDisc.cx,0,SizeDisc.cx,SizeDisc.cy);
			BYTE cbIndex=0;
			if(m_wDiscUser%2==0) cbIndex=1;
			m_ImageDiscKuang.DrawImage(pDC,pt.x,pt.y,m_ImageDiscKuang.GetWidth()/2,m_ImageDiscKuang.GetHeight(),cbIndex*m_ImageDiscKuang.GetWidth()/2,0,m_ImageDiscKuang.GetWidth()/2,m_ImageDiscKuang.GetHeight());
		}
	}

	//手上扑克
	m_HandCardControl.DrawCardControl(pDC);

	//出牌提示
	if (m_bOutCard==true)
	{
		m_ImageOutCard.DrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-165);
	}

	//等待提示
	if (m_bWaitOther==true)
	{
		m_ImageWait.DrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-165);
	}

	//荒庄标志
	if (m_bHuangZhuang==true)
	{
		//CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.AlphaDrawImage(pDC,(nWidth-m_ImageHuangZhuang.GetWidth())/2,nHeight/2-103,RGB(255,0,255));
	}

	//听牌标志
	/*
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bListenStatus[i]==true)
		{
			m_PngListenFlag.DrawImage( pDC,m_UserListenPos[i].x,m_UserListenPos[i].y );
		}
	}
	*/

	//用户状态
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((m_wOutCardUser==i)||(m_cbUserAction[i]!=0))
		{
			//计算位置
			int nXPos=0,nYPos=0;
			switch (i)
			{
			case 0:	//北向
				{
					nXPos=nWidth/2-32;
					nYPos=m_nYBorder+95;
					break;
				}
			case 1:	//东向
				{
					nXPos=nWidth-m_nXBorder-222;
					nYPos=nHeight/2-71;
					break;
				}
			case 2:	//南向
				{
					nXPos=nWidth/2-32;
					nYPos=nHeight-m_nYBorder-260;
					break;
				}
			case 3:	//西向
				{
					nXPos=m_nXBorder+167;
					nYPos=nHeight/2-71;
					break;
				}
			}

			//绘画动作
			if (m_cbUserAction[i]!=WIK_NULL)
			{	
				//绘画动作
				if (m_bBombEffect==true)
				{
					//绘画效果
					INT nItemHeight=m_ImageActionAni.GetHeight()/7;
					INT nItemWidth=m_ImageActionAni.GetWidth()/BOMB_EFFECT_COUNT;

					//绘画动作
					int nYImagePos=0;
					if (m_cbUserAction[i]&WIK_PENG) nYImagePos=nItemHeight;
					else if (m_cbUserAction[i]&WIK_GANG) nYImagePos=nItemHeight*2;
					else if( m_cbUserAction[i]&WIK_LISTEN ) nYImagePos = nItemHeight*3;
					else if (m_cbUserAction[i]&WIK_CHI_HU) nYImagePos=nItemHeight*4;
					else if( m_cbUserAction[i]&WIK_REPLACE ) nYImagePos = nItemHeight*6;
					else nYImagePos=0;
					m_ImageActionAni.DrawImage(pDC,nXPos-nItemWidth/2+54,nYPos+42-nItemHeight/2,nItemWidth,nItemHeight,
						nItemWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),nYImagePos,nItemWidth,nItemHeight);
				}
			}
			else
			{	
				//动作背景
				m_ImageActionBack.DrawImage(pDC,nXPos,nYPos);

				//绘画扑克
				g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos+20,nYPos+23,false);
			}

		}
	}

	//出牌或发牌动画
	if( m_bCardMoving && m_CurrentMoveItem.wViewChairId != INVALID_CHAIR )
	{
		switch( m_enMoveDirection )
		{
		case Direction_East:
			{
				//出牌
				if( m_CurrentMoveItem.bOutCard )
					g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,m_CurrentMoveItem.cbCardData,m_ptMovePos.x,m_ptMovePos.y,true );
				//发牌
				else
				{
					g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,m_ptMovePos.x,m_ptMovePos.y );
					g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,m_ptMovePos.x,m_ptMovePos.y );
				}
			}
			break;
		case Direction_South:
			{
				//出牌
				if( m_CurrentMoveItem.bOutCard )
					g_CardResource.m_ImageTableBottom.DrawCardItem( pDC,m_CurrentMoveItem.cbCardData,m_ptMovePos.x,m_ptMovePos.y,true );
				//发牌
				else
				{
					g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,m_ptMovePos.x,m_ptMovePos.y );
					g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,m_ptMovePos.x,m_ptMovePos.y );
				}
			}
			break;
		case Direction_West:
			{
				//出牌
				if( m_CurrentMoveItem.bOutCard )
					g_CardResource.m_ImageTableRight.DrawCardItem( pDC,m_CurrentMoveItem.cbCardData,m_ptMovePos.x,m_ptMovePos.y,true );
				//发牌
				else
				{
					g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,m_ptMovePos.x,m_ptMovePos.y );
					g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,m_ptMovePos.x,m_ptMovePos.y );
				}
			}
			break;
		case Direction_North:
			{
				//出牌
				if( m_CurrentMoveItem.bOutCard )
					g_CardResource.m_ImageTableTop.DrawCardItem( pDC,m_CurrentMoveItem.cbCardData,m_ptMovePos.x,m_ptMovePos.y,true );
				//发牌
				else
				{
					g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,m_ptMovePos.x,m_ptMovePos.y );
					g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,m_ptMovePos.x,m_ptMovePos.y );
				}
			}
		}
	}

	return;
}

//基础积分
void CGameClientView::SetCellScore(LONGLONG lCellScore)
{
	//设置扑克
	if (lCellScore!=m_lCellScore)
	{
		//设置变量
		m_lCellScore=lCellScore;

		//更新界面
		InvalidGameView(0,0,0,0);
	}

	return;
}

//海底扑克
void CGameClientView::SetHuangZhuang(bool bHuangZhuang)
{
	//设置扑克
	if (bHuangZhuang!=m_bHuangZhuang)
	{
		//设置变量
		m_bHuangZhuang=bHuangZhuang;

		//更新界面
		InvalidGameView(0,0,0,0);
	}

	return;
}

//庄家用户
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	//设置用户
	if (wBankerUser!=m_wBankerUser)
	{
		//设置变量
		m_wBankerUser=wBankerUser;

		//更新界面
		InvalidGameView(0,0,0,0);
	}

	return;
}

//状态标志
void CGameClientView::SetStatusFlag(bool bOutCard, bool bWaitOther)
{
	//设置变量
	m_bOutCard=bOutCard;
	m_bWaitOther=bWaitOther;

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

//出牌信息
void CGameClientView::SetOutCardInfo(WORD wViewChairID, BYTE cbCardData)
{
	//设置变量
	m_cbCardData=cbCardData;
	m_wOutCardUser=wViewChairID;

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

//动作信息
void CGameClientView::SetUserAction(WORD wViewChairID, BYTE bUserAction)
{
	//设置变量
	if (wViewChairID<GAME_PLAYER)
	{
		m_cbUserAction[wViewChairID]=bUserAction;
		SetBombEffect(true);
	}
	else 
	{
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		if(m_bBombEffect)
			SetBombEffect(false);
	}

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

//设置动作
bool CGameClientView::SetBombEffect(bool bBombEffect)
{
	if (bBombEffect==true)
	{
		//设置变量
		m_bBombEffect=true;
		m_cbBombFrameIndex=0;

		//启动时间
		SetTimer(IDI_BOMB_EFFECT,250,NULL);
	}
	else
	{
		//停止动画
		if (m_bBombEffect==true)
		{
			//删除时间
			KillTimer(IDI_BOMB_EFFECT);

			//设置变量
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;

			//更新界面
			InvalidGameView(0,0,0,0);
		}
	}

	return true;
}

//丢弃用户
void CGameClientView::SetDiscUser(WORD wDiscUser)
{
	if(m_wDiscUser != wDiscUser)
	{
		//更新变量
		m_wDiscUser=wDiscUser;

	/*	if( m_wDiscUser != INVALID_CHAIR )
			SetTimer( IDI_DISC_EFFECT,250,NULL );
		else KillTimer( IDI_DISC_EFFECT );
		*/

		//更新界面
		InvalidGameView(0,0,0,0);
	}
	return;
}

//定时玩家
void CGameClientView::SetCurrentUser(WORD wCurrentUser)
{
	if (m_wCurrentUser != wCurrentUser)
	{
		//更新变量 
		m_wCurrentUser=wCurrentUser;
		
		//更新界面
		InvalidGameView(0,0,0,0);
	}
	return;
}
//设置托管
void CGameClientView::SetTrustee(WORD wTrusteeUser,bool bTrustee)
{
	//校验数据 
	ASSERT(wTrusteeUser>=0&&wTrusteeUser<GAME_PLAYER);

	if(m_bTrustee[wTrusteeUser] !=bTrustee)	
	{
		//设置数据
		m_bTrustee[wTrusteeUser]=bTrustee;

		//更新界面
		InvalidGameView(0,0,0,0);
	}
	return;
}

//听牌标志
void CGameClientView::SetUserListenStatus(WORD wViewChairID, bool bListenStatus)
{
	//设置变量
	if (wViewChairID<GAME_PLAYER)
	{
		SetBombEffect(true);
		m_cbUserAction[wViewChairID]=WIK_LISTEN;
		m_bListenStatus[wViewChairID]=bListenStatus;
	}
	else 
		ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

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

//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//点击测试
	CRect rcRePaint;
	bool bHandle=m_HandCardControl.OnEventSetCursor(MousePoint,rcRePaint);

	//重画控制
	if (rcRePaint.IsRectEmpty()==false)
	{
		InvalidGameView(0,0,0,0);//UpdateGameView(&rcRePaint);
		AfxGetMainWnd()->PostMessage(IDM_SOUND,0,0);
	}

	//光标控制
	if (bHandle==false)
		return __super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//鼠标消息
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//扑克事件
	m_HandCardControl.OnEventLeftHitCard();

	//test
	//BYTE byCard[] = {
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,					
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09
	//};
	//BYTE cbHuaCard[]={0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};
	//BYTE cbGangCard[]={0x31,0x31,0x31,0x31};
	//BYTE byCardCount = 14;
	//static int n = 0;

	//if( ++n==1 )
	//{
	//	m_HandCardControl.SetDisplayItem(true);
	//	m_HandCardControl.SetPositively(true);
	//	m_HandCardControl.SetCardData(byCard,MAX_COUNT-1-3-3-3-3,byCard[MAX_COUNT-1]);
	//	for( BYTE i = 0; i < GAME_PLAYER; i++ )
	//	{
	//		m_HeapCard[i].SetHeapCardInfo(5,5);
	//		//m_TableCard[i].SetCardData(byCard,byCardCount-3-3-3-3);
	//		m_DiscardCard[i].SetCardData(byCard,5);

	//		m_WeaveCard[i][0].SetUserAction(WIK_GANG);
	//		m_WeaveCard[i][0].SetDisplayItem(true);
	//		m_WeaveCard[i][0].SetCardData(cbGangCard,4);

	//		m_WeaveCard[i][1].SetUserAction(WIK_GANG);
	//		m_WeaveCard[i][1].SetDisplayItem(false);
	//		m_WeaveCard[i][1].SetCardData(cbGangCard,4 );

	//	    m_WeaveCard[i][2].SetUserAction( WIK_GANG );
	//		m_WeaveCard[i][2].SetDisplayItem(true);
	//		m_WeaveCard[i][2].SetCardData( cbGangCard,4 );
	//				
	//		m_WeaveCard[i][3].SetUserAction(WIK_GANG );
	//		m_WeaveCard[i][3].SetDisplayItem(true);
	//		m_WeaveCard[i][3].SetCardData( cbGangCard,4 );



	//		m_WindCard[i].SetCardData(cbHuaCard,8);
	//		if( i < 3 )
	//			m_UserCard[i].SetCardData(MAX_COUNT-1-3-3-3-3,true);
	//		m_bTrustee[i] = true;
	//		m_cbUserAction[i] = WIK_REPLACE;
	//		m_bListenStatus[i] = true;
	//	}
	//	m_bWaitOther = true;
	//	m_bHuangZhuang = true;
	//	m_bOutCard = true;
	//	m_wOutCardUser = 3;
	//	m_cbCardData = 0x03;
	//	m_cbUserAction[3] = WIK_NULL;
	//	SetDiscUser( 3 );
	//	SetBombEffect(true);
	//	tagScoreInfo ScoreInfo;
	//	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	//	tagWeaveInfo WeaveInfo;
	//	ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));
	//	ScoreInfo.wChiHuUser = 0;
	//	ScoreInfo.wProvideUser = 0;
	//	ScoreInfo.cbFanCount = 88;
	//	ScoreInfo.cbCardCount = 13;
	//	CopyMemory( ScoreInfo.cbCardData,byCard,sizeof(BYTE)*ScoreInfo.cbCardCount );
	//	for( WORD i = 0; i < GAME_PLAYER; i++ )
	//	{
	//		lstrcpy( ScoreInfo.szUserName[i],TEXT("SSSSDFSDFSDF") );
	//		ScoreInfo.lGameScore[i] = 123L*((i%2)?-1:1);
	//	}
	//	CChiHuRight chr;
	//	chr |= CHR_ZI_MO;
	//	chr |= CHR_HUN_YI_SE;
	//	ScoreInfo.cbHuaCardCount = 3;
	//	ScoreInfo.cbFanCount = 23;
	//	m_GameScore.SetScoreInfo(ScoreInfo,WeaveInfo,chr);
	//	m_GameScore.ShowWindow( SW_SHOW );
	//	SetHuangZhuang(true);
	//	m_cbFengQuan = 0;
	//	m_btStart.ShowWindow( SW_SHOW );

	//	tagSelectCardInfo sci[MAX_WEAVE];
	//	ZeroMemory( sci,sizeof(sci) );
	//	sci[0].cbActionCard = 0x04;
	//	sci[0].cbActionMask = WIK_CENTER;
	//	sci[0].cbCardCount = 2;
	//	sci[0].cbCardData[0] = 0x03;
	//	sci[0].cbCardData[1] = 0x05;

	//	sci[1].cbActionCard = 0x03;
	//	sci[1].cbActionMask = WIK_CENTER;
	//	sci[1].cbCardCount = 2;
	//	sci[1].cbCardData[0] = 0x02;
	//	sci[1].cbCardData[1] = 0x04;

	//	sci[2].cbActionCard = 0x03;
	//	sci[2].cbActionMask = WIK_RIGHT;
	//	sci[2].cbCardCount = 2;
	//	sci[2].cbCardData[0] = 0x01;
	//	sci[2].cbCardData[1] = 0x02;
	//	//m_HandCardControl.OnEventUserAction( sci,1 );
	//	SetHeapCardInfo( 1,3 );
	//	m_ControlWnd.SetControlInfo( WIK_RIGHT );

	//	//m_DrawSiceWnd.SetSiceInfo( GetDC(),250,MAKEWORD(4,5),MAKEWORD(2,6) );
	//	//m_DrawSiceWnd.ShowWindow( SW_SHOW );

	//	//设置牌距
	//	for (WORD i=0;i<GAME_PLAYER;i++)
	//	{
	//		for (BYTE j=0;j<4;j++)
	//		{
	//			if(m_WeaveCard[i][j].GetCardCount()==4 )
	//			{
	//				for (BYTE t=j+1;t<4;t++)
	//				{
	//					m_WeaveCard[i][t].OffPoint(i==2?true:false);
	//				}
	//			}

	//		}
	//	}
	//}
	
	
	//else
	//{
	//	m_HandCardControl.SetCurrentCard( false );

	//	tagMoveCardItem mci;
	//	mci.bOutCard = false;
	//	mci.bReplace=false;
	//	mci.bSendTail = (n%2)==0?true:false;
	//	mci.cbCardData = 0x01;
	//	mci.wViewChairId = 2;
	//	OnUserMoveCard( mci );
	//	BeginMoveCard();
	//}
	////end test
	
	
	
	return;
}

//开始按钮
void CGameClientView::OnStart()
{
	//发送消息
	SendEngineMessage(IDM_START,0,0);

	return;
}

//拖管控制
void CGameClientView::OnStusteeControl()
{
	SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}

// 自动托管
void CGameClientView::OnAutoPlayerOn()
{
	m_btAutoPlayOn.ShowWindow(SW_HIDE);
	m_btAutoPlayOff.ShowWindow(SW_SHOW);
	SendEngineMessage(IDM_TRUSTEE_CONTROL,1,1);
	return;
}

// 取消托管
void CGameClientView::OnAutoPlayerOff()
{
	m_btAutoPlayOn.ShowWindow(SW_SHOW);
	m_btAutoPlayOff.ShowWindow(SW_HIDE);
	SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}

//定时器
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//动作动画
	if (nIDEvent==IDI_BOMB_EFFECT)
	{
		//停止判断
		if (m_bBombEffect==false)
		{
			KillTimer(IDI_BOMB_EFFECT);
			return;
		}

		//设置变量
		if ((m_cbBombFrameIndex+1)>=BOMB_EFFECT_COUNT)
		{
			//删除时间
			KillTimer(IDI_BOMB_EFFECT);

			//设置变量
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;
		}
		else m_cbBombFrameIndex++;

		//更新界面
		InvalidGameView(0,0,0,0);

		return;
	}
	else if (nIDEvent==IDI_DISC_EFFECT)
	{
		//设置变量
		if ((m_cbDiscFrameIndex+1)>=DISC_EFFECT_COUNT)
		{
			m_cbDiscFrameIndex=0;
		}
		else m_cbDiscFrameIndex++;

		//更新界面
		InvalidGameView(0,0,0,0);

		return;

	}
	else if( nIDEvent == IDI_MOVE_CARD )
	{
		//判断当前动画是否结束
		if( m_nStepCount == 0 )
		{
			//删除定时器
			KillTimer( IDI_MOVE_CARD );

			//如果是出牌,则添加到已丢弃牌
			if( m_CurrentMoveItem.bOutCard )
			{
				if(m_CurrentMoveItem.bReplace==false)
					m_DiscardCard[m_CurrentMoveItem.wViewChairId].AddCardItem( m_CurrentMoveItem.cbCardData );
				else
					m_WindCard[m_CurrentMoveItem.wViewChairId].AddCardItem( m_CurrentMoveItem.cbCardData );

			}

			//判断是否还有动画,如果只有当前一个
			if( m_MoveCardItemArray.GetCount() == 0 )
			{
				//置FALSE
				m_bCardMoving = false;

				//发送动画结束消息
				WPARAM wParam = MAKEWPARAM(m_CurrentMoveItem.wViewChairId,m_CurrentMoveItem.wChairId);
				AfxGetMainWnd()->PostMessage( m_CurrentMoveItem.bOutCard?IDM_OUT_CARD_FINISH:IDM_SEND_CARD_FINISH,
					wParam,m_CurrentMoveItem.cbCardData );

				//设置变量
				m_CurrentMoveItem.wViewChairId = INVALID_CHAIR;

				//清空绘画区域
				m_rcCardMove.SetRectEmpty();
			}
			//否则继续下一个动画
			else
			{
				//发送动画结束消息
				WPARAM wParam = MAKEWPARAM(m_CurrentMoveItem.wViewChairId,m_CurrentMoveItem.wChairId);
				AfxGetMainWnd()->PostMessage( m_CurrentMoveItem.bOutCard?IDM_OUT_CARD_FINISH:IDM_SEND_CARD_FINISH,
					wParam,m_CurrentMoveItem.cbCardData );

				//设置变量
				m_CurrentMoveItem.wViewChairId = INVALID_CHAIR;

				//开始新的动画
				BeginMoveCard();
			}
			//更新视图
			InvalidGameView(0,0,0,0);
		}
		//动画还没结束
		else
		{
			//减少步数
			m_nStepCount--;

			//设置绘画区域
			m_rcCardMove.SetRect( m_ptMovePos.x,m_ptMovePos.y,m_ptMovePos.x+50,m_ptMovePos.y+50 );

			//移动
			m_ptMovePos.x += m_nXStep;
			m_ptMovePos.y += m_nYStep;

			//联合绘画区域
			CRect rcDraw( m_ptMovePos,CSize(50,50) );
			m_rcCardMove.UnionRect( &m_rcCardMove,&rcDraw );

			//更新视图
			InvalidGameView(0,0,0,0);//UpdateGameView( &m_rcCardMove );
		}
		return;
	}

	__super::OnTimer(nIDEvent);
}

//设置风圈
void CGameClientView::SetFengQuan( BYTE cbFengQuan )
{
	if( m_cbFengQuan != cbFengQuan )
	{
		m_cbFengQuan = cbFengQuan;
		InvalidGameView(0,0,0,0);
	}
}

//玩家出牌或发牌动作
void CGameClientView::OnUserMoveCard( tagMoveCardItem &MoveCardItem )
{
	m_MoveCardItemArray.Add( MoveCardItem );
	return;
}

//开始动画
bool CGameClientView::BeginMoveCard()
{
	//判断是否禁止显示动画
	if( !m_bEnableAnimate )
	{
		return StopMoveCard();
	}

	//构造动画
	if( (!m_bCardMoving||m_CurrentMoveItem.wViewChairId==INVALID_CHAIR) && 
		ConstructMoveCard() )
	{
		m_bCardMoving = true;
		//设置定时器
		SetTimer( IDI_MOVE_CARD,TIME_MOVE_CARD,NULL );
		InvalidGameView(0,0,0,0);//UpdateGameView( &m_rcCardMove );
	}

	return m_bCardMoving;
}

//结束动画,并发送动画结束消息
bool CGameClientView::StopMoveCard()
{
	//判断
	if( m_MoveCardItemArray.GetCount() == 0 && !m_bCardMoving ) return false;

	KillTimer( IDI_MOVE_CARD );

	//当前动画
	if( m_bCardMoving )
	{
		//如果是出牌
		if( m_CurrentMoveItem.bOutCard )
		{
			if(m_CurrentMoveItem.bReplace==false)
				m_DiscardCard[m_CurrentMoveItem.wViewChairId].AddCardItem( m_CurrentMoveItem.cbCardData );
			else
				m_WindCard[m_CurrentMoveItem.wViewChairId].AddCardItem( m_CurrentMoveItem.cbCardData );
		}

		if( m_MoveCardItemArray.GetCount() == 0 ) m_bCardMoving = false;

		//发送消息
		WPARAM wParam = MAKEWPARAM( m_CurrentMoveItem.wViewChairId,m_CurrentMoveItem.wChairId );
		AfxGetMainWnd()->SendMessage( m_CurrentMoveItem.bOutCard?IDM_OUT_CARD_FINISH:IDM_SEND_CARD_FINISH,
			wParam,m_CurrentMoveItem.cbCardData );

		m_CurrentMoveItem.wViewChairId = INVALID_CHAIR;
	}

	//遍历动画数组
	while( m_MoveCardItemArray.GetCount() > 0 )
	{
		tagMoveCardItem mci = m_MoveCardItemArray[0];

		//如果是出牌
		if( mci.bOutCard )
		{
			if(mci.bReplace==false)
			{
				//添加到丢弃牌
				m_DiscardCard[mci.wViewChairId].AddCardItem( mci.cbCardData );
			}
			else
			{
				//添加到丢弃牌
				m_WindCard[mci.wViewChairId].AddCardItem( mci.cbCardData );

			}
		}
		//否则是发牌
		else
		{
			//从牌堆移除牌
			if( mci.bSendTail )
			{
				if( !m_HeapCard[m_wHeapTail].DeductionHeapCard(true) )
				{
					m_wHeapTail = (m_wHeapTail+GAME_PLAYER-1)%GAME_PLAYER;
					VERIFY( m_HeapCard[m_wHeapTail].DeductionHeapCard(true) );
				}
			}
			else
			{
				if( !m_HeapCard[m_wHeapHand].DeductionHeapCard(false) )
				{
					m_wHeapHand = (m_wHeapHand+1)%GAME_PLAYER;
					VERIFY( m_HeapCard[m_wHeapHand].DeductionHeapCard(false) );
				}
			}
		}

		m_MoveCardItemArray.RemoveAt(0);

		//发送消息
		WPARAM wParam = MAKEWPARAM( mci.wViewChairId,mci.wChairId );
		AfxGetMainWnd()->SendMessage( mci.bOutCard?IDM_OUT_CARD_FINISH:IDM_SEND_CARD_FINISH,
			wParam,mci.cbCardData );
	}

	m_bCardMoving = false;

	InvalidGameView(0,0,0,0);

	return true;
}

//构造动画
bool CGameClientView::ConstructMoveCard()
{
	//如果有动画数据
	if( m_MoveCardItemArray.GetCount() > 0 )
	{
		tagMoveCardItem &mci = m_MoveCardItemArray[0];

		//计算起始,结束点
		CPoint ptFrom,ptTo;
		//如果是出牌
		if( mci.bOutCard )
		{
			if( mci.wViewChairId == MYSELF_VIEW_ID )
				ptFrom = m_HandCardControl.GetDispatchCardPos();
			else
			{
				WORD wViewId = mci.wViewChairId>=3?2:mci.wViewChairId;
				ptFrom = m_UserCard[wViewId].GetDispatchCardPos();
			}
			if(mci.bReplace==false)
				ptTo = m_DiscardCard[mci.wViewChairId].GetDispatchCardPos();
			else
				ptTo = m_WindCard[mci.wViewChairId].GetDispatchCardPos();


		}
		//如果是发牌,则要从牌堆移除牌
		else
		{
			if( !mci.bSendTail )
			{
				ASSERT( m_wHeapHand != INVALID_CHAIR );
				ptFrom = m_HeapCard[m_wHeapHand].GetDispatchCardPos(false);
				if( !m_HeapCard[m_wHeapHand].DeductionHeapCard(false) )
				{
					m_wHeapHand = (m_wHeapHand+1)%GAME_PLAYER;
					ptFrom = m_HeapCard[m_wHeapHand].GetDispatchCardPos(false);
					VERIFY( m_HeapCard[m_wHeapHand].DeductionHeapCard(false) );
				}
			}
			else
			{
				ASSERT( m_wHeapTail != INVALID_CHAIR );
				ptFrom = m_HeapCard[m_wHeapTail].GetDispatchCardPos(true);
				if( !m_HeapCard[m_wHeapTail].DeductionHeapCard(true) )
				{
					m_wHeapTail = (m_wHeapTail+GAME_PLAYER-1)%GAME_PLAYER;
					ptFrom = m_HeapCard[m_wHeapTail].GetDispatchCardPos(true);
					VERIFY( m_HeapCard[m_wHeapTail].DeductionHeapCard(true) );
				}
			}
			if( mci.wViewChairId == MYSELF_VIEW_ID )
				ptTo = m_HandCardControl.GetDispatchCardPos();
			else
			{
				WORD wViewId = mci.wViewChairId>=3?2:mci.wViewChairId;
				ptTo = m_UserCard[wViewId].GetDispatchCardPos();
			}
		}
		
		//设置变量
		m_ptMovePos = ptFrom;
		m_nStepCount = MOVE_STEP_COUNT;
		m_nXStep = (ptTo.x-ptFrom.x)/m_nStepCount;
		m_nYStep = (ptTo.y-ptFrom.y)/m_nStepCount;
		switch( mci.wViewChairId )
		{
		case 0:
			m_enMoveDirection = Direction_North;
			break;
		case 1:
			m_enMoveDirection = Direction_West;
			break;
		case 2:
			m_enMoveDirection = Direction_South;
			break;
		case 3:
			m_enMoveDirection = Direction_East;
		}

		//设置绘画区域
		CRect rcDraw(ptFrom,CSize(50,50));
		rcDraw.InflateRect( 50,50 );
		m_rcCardMove.UnionRect( &m_rcCardMove,&rcDraw );

		m_CurrentMoveItem = m_MoveCardItemArray[0];
		m_MoveCardItemArray.RemoveAt(0);

		return true;
	}

	return false;
}

//设置牌堆信息
void CGameClientView::SetHeapCardInfo( WORD wHeapHead, WORD wHeapTail )
{
	m_wHeapHand = wHeapHead;
	m_wHeapTail = wHeapTail;
}

//绘画时间
void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	//加载资源
	CPngImage ImageTimeBack;
	CPngImage ImageTimeNumber;
	HINSTANCE hInst = AfxGetInstanceHandle();
	ImageTimeBack.LoadImage(hInst,TEXT("TIME_BACK"));
	ImageTimeNumber.LoadImage(hInst,TEXT("TIME_NUMBER"));

	//获取属性
	const INT nNumberHeight=ImageTimeNumber.GetHeight();
	const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

	//计算数目
	int nNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		nNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	//位置定义
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=nXPos+(nNumberCount*nNumberWidth)/2-nNumberWidth;

	//绘画背景
	CSize SizeTimeBack(ImageTimeBack.GetWidth(),ImageTimeBack.GetHeight());
	ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

	//绘画号码
	for (int i=0;i<nNumberCount;i++)
	{
		//绘画号码
		WORD wCellNumber=wTime%10;
		ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//设置变量
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//绘画准备
void CGameClientView::DrawUserReadyEx(CDC * pDC, int nXPos, int nYPos)
{
	//加载资源
	CPngImage ImageUserReady;
	ImageUserReady.LoadImage(AfxGetInstanceHandle(),TEXT("USER_READY"));

	//绘画准备
	CSize SizeImage(ImageUserReady.GetWidth(),ImageUserReady.GetHeight());
	ImageUserReady.DrawImage(pDC,nXPos-SizeImage.cx/2,nYPos-SizeImage.cy/2);

	return;
}

//////////////////////////////////////////////////////////////////////////