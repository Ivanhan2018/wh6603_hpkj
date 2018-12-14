#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientEngine.h"


//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FLASH_WINNER			100									//闪动标识
#define IDI_SHOW_CHANGE_BANKER		101									//轮换庄家
#define IDI_DISPATCH_CARD			102									//发牌标识 
#define IDI_SHOWDISPATCH_CARD_TIP	103									//发牌提示
#define IDI_OPENCARD				104									//发牌提示
#define IDI_MOVECARD_END			105									//移動牌結束
#define IDI_POSTCARD				106									//发牌提示

#define	IDI_SICE					107									///骰子动画
#define IDI_WITER_SEND				108									///庄家动画
//按钮标识
#define IDC_JETTON_BUTTON_100		200									//按钮标识
#define IDC_JETTON_BUTTON_1000		201									//按钮标识
#define IDC_JETTON_BUTTON_10000		202									//按钮标识
#define IDC_JETTON_BUTTON_100000	203									//按钮标识
#define IDC_JETTON_BUTTON_1000000	204									//按钮标识
#define IDC_JETTON_BUTTON_5000000	205									//按钮标识
#define IDC_APPY_BANKER				206									//按钮标识
#define IDC_CANCEL_BANKER			207									//按钮标识
#define IDC_SCORE_MOVE_L			209									//按钮标识
#define IDC_SCORE_MOVE_R			210									//按钮标识
#define IDC_VIEW_CHART				211									//按钮标识
#define IDC_JETTON_BUTTON_50000	    219									//按钮标识
#define IDC_JETTON_BUTTON_500000	212									//按钮标识
#define IDC_AUTO_OPEN_CARD			213									//按钮标识
#define IDC_OPEN_CARD				214									//按钮标识
#define IDC_BANK					215									//按钮标识
#define IDC_CONTINUE_CARD			216									//按钮标识
#define IDC_UP						223									//按钮标识
#define IDC_DOWN					224									//按钮标识
#define IDC_LAST					225									//按钮标识
#define IDC_NEXT					226									//按钮标识

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_UP,OnUp)
	ON_BN_CLICKED(IDC_DOWN,OnDown)
	ON_BN_CLICKED(IDC_OPEN_CARD,OnOpenCard)
	ON_BN_CLICKED(IDC_BANK_DRAW,OnBankDraw)
	ON_BN_CLICKED(IDC_BT_ADMIN,OpenAdminWnd)
	ON_BN_CLICKED(IDC_APPY_BANKER,OnApplyBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L,OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R,OnScoreMoveR)
	ON_BN_CLICKED(IDC_BANK_STORAGE,OnBankStorage)
	ON_BN_CLICKED(IDC_CANCEL_BANKER,OnCancelBanker)
	ON_BN_CLICKED(IDC_AUTO_OPEN_CARD,OnAutoOpenCard)
	ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)
	ON_BN_CLICKED(IDC_LAST, OnLastMusic)
	ON_BN_CLICKED(IDC_NEXT, OnNextMusic)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView() 
{
	//下注信息
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//全体下注
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	for(int i = 0;i < AREA_COUNT+1;i++)
	{
		m_cbUserCardType[i] = INVALID_CHAIR;
	}
	//庄家信息
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//当局成绩
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//状态信息
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bWinTianMen=false;
	m_bWinHuangMen=false;
	m_bWinXuanMen=false;
	m_bFlashResult=false;
	m_blMoveFinish = false;
	m_blAutoOpenCard = true;
	m_enDispatchCardTip=enDispatchCardTip_NULL;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//位置信息
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//历史成绩
	m_lMeStatisticScore=0;

	//控件变量
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);

	m_pClientControlDlg = NULL;
	m_hInst = NULL;

	/*const BYTE CGameLogic::m_cbCardListData[CARD_COUNT]=
	{
			0x0C,0x2C,
			0x0B,0x2B,
			0x0A,0x1A,0x2A,0x3A,
			0x09,0x29,
			0x07,0x17,0x27,0x37,
			0x08,0x18,0x28,0x38,
			0x06,0x16,0x26,0x36,
			0x05,0x25,
			0x04,0x14,0x24,0x34,
			0x02,0x22,
			0x42,
			0x23

			QQ＞22＞大王和3＞红8对＞红4对＞红10对、红6对、黑4对（以上3对为一样大）
			＞黑10对 黑6对、红7对、红J对 （以上4对一样大）黑8对、黑7对、红5对、红九对（以上4对一样大）
			如遇一样大对子 那么不管点数都为庄家大 Q+ 9组合都算＞  Q+8 组合都算(包括黑8在内) ＞2＋8组合都算（包括黑8） 
			>Q＞2＞红8＞红4＞红10=红6=黑4=黑10=黑6=红7=红J=黑8=黑7=红5=红九>大王==3
	};*/
	//m_cbTableCardArray[SHUN_MEN_INDEX ][0]=0x0A;
	//m_cbTableCardArray[SHUN_MEN_INDEX ][1]=0x2A;
	//m_cbTableCardArray[ BANKER_INDEX][0]=0x1A;
	//m_cbTableCardArray[BANKER_INDEX][1]=0x3A;

	BYTE bcTmp[5]={0x2B,0x2C,0x0D,0x3B,0x1D};
	////	BYTE bcTmp1[5]={0x05,0x35,0x25,0x15,0x13};
	emCardType itype = (emCardType)m_GameLogic.GetCardType(bcTmp,5);

	m_lUserCardType[0] = itype;
	//BYTE Byte;
	//bool bWinShunMen=m_GameLogic.CompareCard(bcTmp,5,bcTmp1,5,Byte)==1?true:false;

	return;
}

//析构函数
CGameClientView::~CGameClientView(void)
{
	if( m_pClientControlDlg )
	{
		delete m_pClientControlDlg;
		m_pClientControlDlg = NULL;
	}

	if( m_hInst )
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
}

void CGameClientView::SetMoveCardTimer()
{
	//KillTimer(IDI_POSTCARD);
	KillCardTime();
	m_OpenCardIndex = 1;
	m_bcShowCount = 0;
	m_PostStartIndex =  m_GameLogic.GetCardValue(m_bcfirstShowCard[0]) +  m_GameLogic.GetCardValue(m_bcfirstShowCard[1]);
	if(m_PostStartIndex>1)
		m_PostStartIndex  = (m_PostStartIndex-1)%5;
	m_PostCardIndex=m_PostStartIndex;
	for (int i = 0;i<5;i++)
	{
		m_CardControl[i].m_blAdd = true;
		m_CardControl[i].m_blAuto = m_blAutoOpenCard;
		m_CardControl[i].m_blGameEnd = true;
		m_CardControl[i].m_blShowResult = false;
		m_CardControl[i].m_blShowLineResult = false;
		m_CardControl[i].ResetFrame();
	}
	SetTimer(IDI_POSTCARD,400,NULL);				///发牌
	//SetTimer(IDI_DISPATCH_CARD,25000,NULL);
	SetTimer(IDI_WITER_SEND,400,NULL);
	//m_Zhuang.SetFrameIndex((m_Zhuang.GetBeginIndex() -1)*8);			///每个动画8帧。第四个动画


}
//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_GameRecord.Create(IDD_DLG_GAME_RECORD,this);
	m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this );
	m_GameRecord.ShowWindow(SW_HIDE);
	
	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.LoadFromResource(this,hInstance,IDB_VIEW_FILL);
	//m_ImageViewBack.LoadFromResource(this,hInstance,IDB_VIEW_BACK);
	m_ImageViewBack.LoadBitmap(IDB_VIEW_BACK);//加载位图
	//m_ImageWinFlags.LoadFromResource(this,hInstance,IDB_WIN_FLAGS);
	m_ImageWinFlags.LoadBitmap(IDB_WIN_FLAGS);
	//m_ImageJettonView.LoadFromResource(this,hInstance,IDB_JETTOM_VIEW);
	m_ImageJettonView.LoadBitmap(IDB_JETTOM_VIEW);//加载位图
	m_ImageScoreNumber.LoadFromResource(this,hInstance,IDB_SCORE_NUMBER);
	//m_ImageMeScoreNumber.LoadFromResource(this,hInstance,IDB_ME_SCORE_NUMBER);
	m_ImageMeScoreNumber.LoadBitmap(IDB_ME_SCORE_NUMBER);

	//边框资源
	//m_ImageFrameTianMen.LoadFromResource(this,hInstance,IDB_FRAME_TIAN_MEN_EX);
	//m_ImageFrameDiMen.LoadFromResource(this,hInstance,IDB_FRAME_DI_MEN_EX);
	//m_ImageFrameHuangMen.LoadFromResource(this,hInstance,IDB_FRAME_HUANG_MEN_EX);
	//m_ImageFrameXuanMen.LoadFromResource(this,hInstance,IDB_FRAME_XUAN_MEN_EX);

	m_ImageFrameTianMen.LoadBitmap(IDB_FRAME_TIAN_MEN_EX);
	m_ImageFrameDiMen.LoadBitmap(IDB_FRAME_DI_MEN_EX);
	m_ImageFrameHuangMen.LoadBitmap(IDB_FRAME_HUANG_MEN_EX);
	m_ImageFrameXuanMen.LoadBitmap(IDB_FRAME_XUAN_MEN_EX);

	
	for (int i = 0;i<5;i++)//m_ImageCardType[i].LoadFromResource(this,hInstance,IDB_CARDTYPE);
	{
		//m_ImageCardType[i].LoadFromResource(this,hInstance,IDB_CARDTYPE);
		m_ImageCardType[i].LoadBitmap(IDB_CARDTYPE);
	}
	//m_ImageGameEnd.LoadFromResource(this,hInstance, IDB_GAME_END);
	m_ImageGameEnd.LoadBitmap(IDB_GAME_END);

	m_ImageMeBanker.LoadFromResource(this,hInstance, IDB_ME_BANKER);
	m_ImageChangeBanker.LoadFromResource(this,hInstance, IDB_CHANGE_BANKER);
	m_ImageNoBanker.LoadFromResource(this,hInstance, IDB_NO_BANKER);	

	//m_ImageTimeFlag.LoadFromResource(this,hInstance,IDB_TIME_FLAG);
	m_ImageTimeFlag.LoadBitmap(IDB_TIME_FLAG);

	//下注按钮
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton50000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_50000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
	
	m_btJetton500000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_500000);
	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5000000);
		
	//申请按钮
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	m_btAutoOpenCard.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_AUTO_OPEN_CARD);
	m_btOpenCard.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_OPEN_CARD);

	m_btBank.Create(NULL,WS_CHILD,rcCreate,this,IDC_BANK);
	m_btContinueCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_CONTINUE_CARD);

	m_btUp.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_UP);
	m_btDown.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_DOWN);

	m_btLast.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_LAST);
	m_btNext.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_NEXT);

//	m_btBankerStorage.EnableWindow(TRUE);
	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_BT_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	//设置按钮
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false,false);
	m_btJetton50000.SetButtonImage(IDB_BT_JETTON_50000,hResInstance,false,false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false,false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false,false);
	m_btJetton500000.SetButtonImage(IDB_BT_JETTON_500000,hResInstance,false,false);
	m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,hResInstance,false,false);
	m_btJetton5000000.SetButtonImage(IDB_BT_JETTON_5000000,hResInstance,false,false);

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false,false);

	m_btUp.SetButtonImage(IDB_BT_BT_S,hResInstance,false,false);
	m_btDown.SetButtonImage(IDB_BT_BT_X,hResInstance,false,false);
	m_btLast.SetButtonImage(IDB_BT_LAST,hResInstance,false,false);
	m_btNext.SetButtonImage(IDB_BT_NEXT,hResInstance,false,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false,false);

	m_btAutoOpenCard.SetButtonImage(IDB_BT_AUTO_OPEN_CARD,hResInstance,false,false);
	m_btOpenCard.SetButtonImage(IDB_BT_OPEN_CARD,hResInstance,false,false);

	m_btAutoOpenCard.ShowWindow(SW_HIDE);
	m_btOpenCard.ShowWindow(SW_HIDE);
	m_btBank.SetButtonImage(IDB_BT_BANK,hResInstance,false,false);
	m_btContinueCard.SetButtonImage(IDB_BT_CONTINUE_CARD,hResInstance,false,false);

// 	m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false,false);
// 	m_btBankerDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false,false);

	// 字体
	m_FontValleysListOne.CreateFont( this, TEXT("宋体"), 16, 600 );
	m_FontValleysListTwo.CreateFont( this, TEXT("宋体"), 14, 400 );

#ifdef __BANKER___
	//if(m_DlgBank.m_hWnd==NULL) m_DlgBank.Create(IDD_BANK_STORAGE,this);

// 	m_btBankerStorage.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_STORAGE);
// 	m_btBankerDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);
// 
// 	m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false,false);
// 	m_btBankerDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false,false);
#endif

	//扑克控件
	for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].SetDisplayFlag(true);

	//控制
	m_hInst = NULL;
	m_pClientControlDlg = NULL;
	m_hInst = LoadLibrary(_T("OxBattleClientControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(CWnd* pParentWnd); 
		CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,("CreateClientControl")); 
		if ( ClientControl )
		{
			m_pClientControlDlg = static_cast<IClientControlDlg*>(ClientControl(this));
			m_pClientControlDlg->ShowWindow( SW_HIDE );
		}
	}

	return 0;
}

//重置界面
VOID CGameClientView::ResetGameView()
{
	//下注信息
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//全体下注
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	for(int i = 0;i < AREA_COUNT+1;i++)
	{
		m_cbUserCardType[i] = INVALID_CHAIR;
	}

	//庄家信息
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//当局成绩
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//状态信息
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bWinTianMen=false;
	m_bWinHuangMen=false;
	m_bWinXuanMen=false;
	m_bFlashResult=false;
	m_bShowGameResult=false;
	m_enDispatchCardTip=enDispatchCardTip_NULL;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//位置信息
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//历史成绩
	m_lMeStatisticScore=0;

	//清空列表
	m_ApplyUser.ClearAll();

	//清除桌面
	CleanUserJetton();

	//设置按钮
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

	return;
}

//调整控件
VOID CGameClientView::RectifyControl(int nWidth, int nHeight)
{
	//位置信息
	m_nWinFlagsExcursionX = nWidth/2-260-41 + 85;	
	m_nWinFlagsExcursionY = nHeight/2 + 246-21 + 16;

	CSize Size;
	m_CardControl[0].GetControlSize(Size,3);
	Size.cy = Size.cy/2;

	int iWidth =405;

	int LifeWidth = nWidth/2;
	int TopHeight = nHeight/2;

	//区域位置
	int nCenterX=nWidth/2, nCenterY=nHeight/2, nBorderWidth=4;
	m_rcTianMen.left=LifeWidth-303;
	m_rcTianMen.top=TopHeight-115+77;
	m_rcTianMen.right=m_rcTianMen.left+145;
	m_rcTianMen.bottom=m_rcTianMen.top+200;

	m_rcDimen.left=LifeWidth-153;
	m_rcDimen.top=TopHeight-115+77;
	m_rcDimen.right=m_rcDimen.left+145;
	m_rcDimen.bottom=m_rcDimen.top+200;

	m_rcXuanMen.left=LifeWidth-3;
	m_rcXuanMen.top=TopHeight-115+77;
	m_rcXuanMen.right=m_rcXuanMen.left+145;
	m_rcXuanMen.bottom=m_rcXuanMen.top+200;

	m_rcHuangMen.left=LifeWidth+147;
	m_rcHuangMen.top=TopHeight-115+77;
	m_rcHuangMen.right=m_rcHuangMen.left+145;
	m_rcHuangMen.bottom=m_rcHuangMen.top+200;

	m_Sice[0].SetPointBase(m_rcDimen.left + 65,m_rcDimen.top + 65);
	m_Sice[0].SetSize(2170/31,80);
	m_Sice[1].SetPointBase(m_rcXuanMen.left + 65,m_rcXuanMen.top+65);
	m_Sice[1].SetSize(2170/31,80);

	m_Zhuang.SetPointBase(LifeWidth-20-59 ,TopHeight - 190 - 160+70);

	//筹码数字
	RECT rtTian = {0};
	rtTian.left=LifeWidth-310;
	rtTian.top=TopHeight - 80;
	RECT rtDi = {0};
	rtDi.left=LifeWidth-150;
	rtDi.top=TopHeight - 80;
	RECT rtXuan = {0};
	rtXuan.left=LifeWidth-10;
	rtXuan.top=TopHeight - 80;
	RECT rtHuang = {0};
	rtHuang.left=LifeWidth+135;
	rtHuang.top=TopHeight - 80;

	m_PointJettonNumber[ID_TIAN_MEN-1].SetPoint(m_rcTianMen.left + 60, m_rcTianMen.top + 80);
	m_PointJettonNumber[ID_DI_MEN -1].SetPoint(m_rcDimen.left + 60, m_rcDimen.top+ 80);
	m_PointJettonNumber[ID_XUAN_MEN-1].SetPoint(m_rcXuanMen.left + 60, m_rcXuanMen.top+ 80);
	m_PointJettonNumber[ID_HUANG_MEN-1].SetPoint(m_rcHuangMen.left + 60, m_rcHuangMen.top+ 80);

	//筹码位置
	m_PointJetton[ID_TIAN_MEN-1].SetPoint(m_rcTianMen.left, m_rcTianMen.top);
	m_PointJetton[ID_DI_MEN-1].SetPoint(m_rcDimen.left, m_rcDimen.top);
	m_PointJetton[ID_XUAN_MEN-1].SetPoint(m_rcXuanMen.left, m_rcXuanMen.top);
	m_PointJetton[ID_HUANG_MEN -1].SetPoint(m_rcHuangMen.left, m_rcHuangMen.top);

	m_CardControl[0].SetBenchmarkPos(CPoint(nWidth/2-175,nHeight/2-160),enXCenter,enYTop);
	m_CardTypePoint[0] =CPoint(nWidth/2-30,nHeight/2-150);

	m_CardControl[1].SetBenchmarkPos(CPoint(m_rcTianMen.left+65,m_rcTianMen.top+35),enXCenter,enYTop);
	m_CardTypePoint[1] =CPoint(m_rcTianMen.left+35,m_rcTianMen.top+145);

	m_CardControl[2].SetBenchmarkPos(CPoint(m_rcDimen.left+65,m_rcDimen.top+35),enXCenter,enYTop);
	m_CardTypePoint[2]=CPoint(m_rcDimen.left+35,m_rcDimen.top+145);

	m_CardControl[3].SetBenchmarkPos(CPoint(m_rcXuanMen.left+65,m_rcXuanMen.top+35),enXCenter,enYTop);
	m_CardTypePoint[3] =CPoint(m_rcXuanMen.left+35,m_rcXuanMen.top+145);

	m_CardControl[4].SetBenchmarkPos(CPoint(m_rcHuangMen.left+65,m_rcHuangMen.top+35),enXCenter,enYTop);
	m_CardTypePoint[4] =CPoint(m_rcHuangMen.left+35,m_rcHuangMen.top+145);

	
	for(int i=0; i<5; i++)
	{
		m_CardControl[i].SetPtSrc(CPoint(m_rcXuanMen.left + 27,m_rcXuanMen.top-40));
	}

	//m_CardControl[5].SetBenchmarkPos(CPoint(m_rcHuangMen.left+65,m_rcHuangMen.top+135),enXCenter,enYTop);
	//m_CardTypePoint[5] =CPoint(0,0);

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(33);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	m_ApplyUser.m_viewHandle = m_hWnd;

	//列表控件
	DeferWindowPos(hDwp,m_ApplyUser,NULL,LifeWidth+245+20+245-210+5-125,TopHeight+10+9-200+15-26-2-112,175,92,uFlags);
	DeferWindowPos(hDwp,m_btUp,NULL,LifeWidth+245+20+310-210+5+5+92-111-1+1+1-1,/*TopHeight+10+10-30*/TopHeight+10+9-200+15-26-20-112+8-1,80/5,17,uFlags);
	DeferWindowPos(hDwp,m_btDown,NULL,LifeWidth+245+20+310-210+5+5+92-111,/*TopHeight+10+12+88-20-30*/TopHeight+10+9-200+15-26+81-112-1,80/5,17,uFlags);

	//JJWillDo 删除下面 下下面true改false
	/*tagApplyUser ap;
	for (int i = 0; i < 8 ; i++)
	{
		ap.lUserScore = i+1;
		ap.strUserName.Format(_T("a%d"), i+1);
		m_ApplyUser.InserUser(ap);
	}*/
	m_btUp.ShowWindow(SW_SHOW);
	m_btUp.EnableWindow(false);
	m_btDown.ShowWindow(SW_SHOW);
	m_btDown.EnableWindow(false);

	m_MeInfoRect.left = LifeWidth-82;
	m_MeInfoRect.top = 10;

	//筹码按钮
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	
	int nXPos = nWidth/2-200;
	int nYPos = nHeight/2+185;

	int nSpace = 2;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton50000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace * 4 + rcJetton.Width() * 4,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton500000,NULL,nXPos + nSpace * 5 + rcJetton.Width() * 5,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + nSpace * 6 + rcJetton.Width() * 6,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos + nSpace * 7 + rcJetton.Width() *7,nYPos,0,0,uFlags|SWP_NOSIZE);

	//上庄按钮
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nWidth/2+277-80 + 165-83,nHeight/2-340+74 + 180-107,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nWidth/2+277-80 + 165-83,nHeight/2-340+74 + 180-107,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btLast,NULL,nWidth/2+277-80 + 165-83,nHeight/2-340+74 + 180-60,103,43,uFlags);
	DeferWindowPos(hDwp,m_btNext,NULL,nWidth/2+277-80 + 165-83,nHeight/2-340+74 + 180-13,103,43,uFlags);

	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,LifeWidth+58+23+4,TopHeight+594+9,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,LifeWidth+58+28+403+2,TopHeight+594+9,0,0,uFlags|SWP_NOSIZE);

	//开牌按钮
	DeferWindowPos(hDwp,m_btAutoOpenCard,NULL,nWidth/2+277-80 + 165,nHeight/2-340+74 + 180 + 180,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btOpenCard,NULL,		nWidth/2+277-80 + 165,nHeight/2-340+74 + 180 + 180 + 35,0,0,uFlags|SWP_NOSIZE);

	//其他按钮
	DeferWindowPos(hDwp,m_btBank,NULL,nWidth/2+290,nHeight/2-340+74,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btContinueCard,NULL,LifeWidth+624+20,TopHeight+198-30,0,0,uFlags|SWP_NOSIZE);		///连续发牌按钮

	//银行按钮
#ifdef __BANKER___
// 	m_btBankerDraw.GetWindowRect(&rcJetton);
// 	DeferWindowPos(hDwp,m_btBankerStorage,NULL,nWidth/2+313,nHeight/2+228,0,0,uFlags|SWP_NOSIZE);
// 	DeferWindowPos(hDwp,m_btBankerDraw,NULL,nWidth/2+313,nHeight/2+228+53,0,0,uFlags|SWP_NOSIZE);
#endif
	
	//结束移动
	EndDeferWindowPos(hDwp);

	return;
}

//绘画界面
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//绘画背景
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);

	CDC memDC;//定义一个设备上下文

	memDC.CreateCompatibleDC(pDC);//创建兼容的设备上下文

	memDC.SelectObject(&m_ImageViewBack);//选中位图对象

	pDC->BitBlt(-3,-2,779,710,&memDC,0,0,SRCCOPY);

	//获取状态
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//状态提示
	CFont static InfoFont;
	InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));
	CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,234,0));
	
	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();

	//时间提示
	//CImageHandle ImageHandleTimeFlag(&m_ImageTimeFlag);
	int nTimeFlagWidth = 160/3;
	int nFlagIndex=0;
	if (cbGameStatus==GAME_STATUS_FREE) nFlagIndex=0;
	else if (cbGameStatus==GS_PLACE_JETTON) 
	{
		nFlagIndex=1;
	}
	else if (cbGameStatus==GS_GAME_END) nFlagIndex=2;
	///绘状态信息，也就是处于什么时间，如开牌时间。
	memDC.SelectObject(&m_ImageTimeFlag);//选中位图对象
	BitBlt(pDC->GetSafeHdc(), nWidth/2-470+115, nHeight/2-125 + 48-85, nTimeFlagWidth,17,memDC,
		nFlagIndex * nTimeFlagWidth, 0,SRCCOPY);

	WORD wUserTimer=GetUserClock(m_wMeChairID);

	///剩余时间
	CString stTtime;
	stTtime.Format(_T("%d"),wUserTimer );
	RECT rt;
	rt.top =nHeight/2+197-372 + 100 -85;
	rt.left = nWidth/2-323+49 - 148+115 ;
	rt.right = rt.left + 100;
	rt.bottom = rt.top + 50;
	pDC->DrawText(stTtime,&rt,DT_CENTER);

	//胜利边框
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//筹码资源
	CSize static SizeJettonItem(405/9,45);

	//绘画筹码
	for (INT i=0;i<AREA_COUNT;i++)
	{
		//变量定义
		LONGLONG lScoreCount=0L;
		LONGLONG static lScoreJetton[JETTON_COUNT]={1L,5L,10L,50L,100L,200L,500L,1000L};
		int static nJettonViewIndex=0;

		//绘画筹码
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//获取信息
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//累计数字
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//图片索引
			if (1==pJettonInfo->cbJettonIndex || 2==pJettonInfo->cbJettonIndex) nJettonViewIndex=pJettonInfo->cbJettonIndex+1;
			else if (0==pJettonInfo->cbJettonIndex) nJettonViewIndex=pJettonInfo->cbJettonIndex;
			else nJettonViewIndex=pJettonInfo->cbJettonIndex+1;

			//绘画筹码
			memDC.SelectObject(&m_ImageJettonView);//选中位图对象
			pDC->TransparentBlt(pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y,
				SizeJettonItem.cx,
				SizeJettonItem.cy,
				&memDC,
				SizeJettonItem.cx*pJettonInfo->cbJettonIndex,0,SizeJettonItem.cx,SizeJettonItem.cy,RGB(255,0,255));
		}

		//绘画数字
		if (lScoreCount>0L)	DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
	}

	if (m_Sice[0].IsVisable())
		m_Sice[0].DrawCardControl(pDC);
	if (m_Sice[1].IsVisable())
		m_Sice[1].DrawCardControl(pDC);
	

	//绘画庄家
	DrawBankerInfo(pDC,nWidth,nHeight);

	//绘画用户
	DrawMeInfo(pDC,nWidth,nHeight);

	//切换庄家
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 ;

		//由我做庄
		if ( m_wMeChairID == m_wBankerUser )
		{
			//CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			//CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else
		{
			//CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
	}

    //发牌提示 重新发牌 还是 连续发牌
	if (enDispatchCardTip_NULL!=m_enDispatchCardTip)
	{
		/*if (m_ImageDispatchCardTip.IsNull()==false) m_ImageDispatchCardTip.Destroy();
		if (enDispatchCardTip_Dispatch==m_enDispatchCardTip)
			m_ImageDispatchCardTip.LoadFromResource(this,AfxGetInstanceHandle(),IDB_DISPATCH_CARD);
		else
			m_ImageDispatchCardTip.LoadFromResource(this,AfxGetInstanceHandle(),IDB_CONTINUE_CARD);
		m_ImageDispatchCardTip.BitBlt(pDC->GetSafeHdc(), (nWidth-m_ImageDispatchCardTip.GetWidth())/2, nHeight/2);*/

		if (m_ImageDispatchCardTip.m_hObject != NULL) m_ImageDispatchCardTip.DeleteObject();
		if (enDispatchCardTip_Dispatch == m_enDispatchCardTip)
		{
			//m_ImageDispatchCardTip.LoadFromResource(this,AfxGetInstanceHandle(),IDB_DISPATCH_CARD);
			m_ImageDispatchCardTip.LoadBitmap(IDB_DISPATCH_CARD);
		}
		else
			m_ImageDispatchCardTip.LoadBitmap(IDB_CONTINUE_CARD);
		//m_ImageDispatchCardTip.BitBlt(pDC->GetSafeHdc(), (nWidth-m_ImageDispatchCardTip.GetWidth())/2, nHeight/2);
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(m_ImageDispatchCardTip);
		pDC->BitBlt((nWidth-267)/2, nHeight/2,267,110,&memDC,0,0,SRCCOPY);
		memDC.DeleteDC();
	}

	//我的下注
	DrawMeJettonNumber(pDC);

	/*test*/
	if (0)
	{
		//绘画扑克
		for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].DrawCardControl(pDC);
	}

	//胜利标志
	DrawWinFlags(pDC);

	/*if(1)
	{
		m_bShowGameResult= 1;
		DrawType(pDC,0);
	}*/
	

	//结束状态
	if (cbGameStatus==GS_GAME_END)
	{
		//绘画扑克
		for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].DrawCardControl(pDC);

	}

	///绘画发牌动画
	m_Zhuang.DrawCardControl(pDC);

	if(m_blMoveFinish)
	{

		ShowGameResult(pDC, nWidth, nHeight);
	}

//	if(m_blMoveFinish)
//	{
		//显示结果   牛几 
		DrawType(pDC,0);
		
//	}

	memDC.DeleteDC();

	return;
}

///状态改变
void CGameClientView:: OnStateChange(BYTE state)
{
	//if(state != GS_GAME_END)
	//{
		//SetTimer(F, 100, NULL);
		//AfxMessageBox(_T("open"));
	//}
	/*m_Sice[0].SetVisable(TRUE);
	m_Sice[1].SetVisable(TRUE);
	m_Zhuang.SetVisable(TRUE);
	SetTimer(IDI_SICE,50,NULL);*/
}

//设置信息
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//设置变量
		m_lMeMaxScore=lMeMaxScore;
	}

	return;
}

//最大下注
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//设置变量
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//设置筹码
void CGameClientView::SetCurrentJetton(LONGLONG lCurrentJetton)
{
	//设置变量
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;

	return;
}

//历史记录
void CGameClientView::SetGameHistory(bool bWinTian, bool bWinDi, bool bWinXuan,bool bWinHuang)
{
	//设置数据
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinTianMen=bWinTian;
	GameRecord.bWinDiMen =bWinDi;
	GameRecord.bWinXuanMen=bWinXuan;
	GameRecord.bWinHuangMen=bWinHuang;

	//操作类型
	if (0==m_lUserJettonScore[ID_TIAN_MEN]) GameRecord.enOperateTianMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_TIAN_MEN] > 0 && true==bWinTian) GameRecord.enOperateTianMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_TIAN_MEN] > 0 && false==bWinTian) GameRecord.enOperateTianMen=enOperateResult_Lost;

	if (0==m_lUserJettonScore[ID_DI_MEN]) GameRecord.enOperateDiMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_DI_MEN] > 0 && true==bWinDi) GameRecord.enOperateDiMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_DI_MEN] >0 && false==bWinDi) GameRecord.enOperateDiMen=enOperateResult_Lost;

	if (0==m_lUserJettonScore[ID_XUAN_MEN]) GameRecord.enOperateXuanMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_XUAN_MEN] > 0 && true==bWinXuan) GameRecord.enOperateXuanMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_XUAN_MEN] >0 && false==bWinXuan) GameRecord.enOperateXuanMen=enOperateResult_Lost;


	if (0==m_lUserJettonScore[ID_HUANG_MEN]) GameRecord.enOperateHuangMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_HUANG_MEN] > 0 && true==bWinHuang) GameRecord.enOperateHuangMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_HUANG_MEN] >0 && false==bWinHuang) GameRecord.enOperateHuangMen=enOperateResult_Lost;

	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( MAX_FALG_COUNT < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//移到最新记录
	if ( MAX_FALG_COUNT < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - MAX_FALG_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
		m_btScoreMoveL.EnableWindow(TRUE);
		m_btScoreMoveR.EnableWindow(FALSE);
	}

	return;
}

//清理筹码
void CGameClientView::CleanUserJetton()
{
	//清理数组
	for (BYTE i=0;i<CountArray(m_JettonInfoArray);i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	//下注信息
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//全体下注
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//更新界面
	RefreshGameView();

	return;
}

//个人下注
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<=ID_HUANG_MEN);
	if (cbViewIndex>ID_HUANG_MEN) return;

	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//更新界面
	RefreshGameView();
}

//设置扑克
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[5][5])
{
	if (cbTableCardArray!=NULL)
	{
		CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));

		//开始发牌
		DispatchCard();
	}
	else
	{
		ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	}
}

//设置筹码
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//效验参数
	ASSERT(cbViewIndex<=ID_HUANG_MEN);
	if (cbViewIndex>ID_HUANG_MEN) return;

	//变量定义
	bool bPlaceJetton=false;
	LONGLONG lScoreIndex[JETTON_COUNT]={1L,5L,10L,50L,100L,200L,500L,1000L};

	//边框宽度
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	switch (cbViewIndex)
	{
	case ID_TIAN_MEN:
		{ 
			m_lAllJettonScore[ID_TIAN_MEN] += lScoreCount;
			nFrameWidth = m_rcTianMen.right-m_rcTianMen.left;
			nFrameHeight = m_rcTianMen.bottom-m_rcTianMen.top;
			break;
		}
	case ID_DI_MEN:
		{
			m_lAllJettonScore[ID_DI_MEN] += lScoreCount;
			nFrameWidth = m_rcDimen.right-m_rcDimen.left;
			nFrameHeight = m_rcDimen.bottom-m_rcDimen.top;
			break;
		}
	case ID_HUANG_MEN :
		{ 
			m_lAllJettonScore[ID_HUANG_MEN] += lScoreCount;
			nFrameWidth = m_rcHuangMen.right-m_rcHuangMen.left;
			nFrameHeight = m_rcHuangMen.bottom-m_rcHuangMen.top;
			break;
		}
	case ID_XUAN_MEN:
		{ 
			m_lAllJettonScore[ID_XUAN_MEN] += lScoreCount;
			nFrameWidth = m_rcXuanMen.right-m_rcXuanMen.left;
			nFrameHeight = m_rcXuanMen.bottom-m_rcXuanMen.top;
			break;
		}
	default:
		{
			ASSERT(FALSE);
			return;
		}
	}

	nFrameWidth += nBorderWidth;
	nFrameHeight += nBorderWidth;

	//增加判断
	bool bAddJetton=lScoreCount>0?true:false;
	lScoreCount= (lScoreCount>0?lScoreCount:-lScoreCount);

	//增加筹码
	for (BYTE i=0;i<JETTON_COUNT;i++)
	{
		//计算数目
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONGLONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//插入过虑
		if (lCellCount==0L) continue;

		//加入筹码
		for (LONGLONG j=0;j<lCellCount;j++)
		{
			if (true==bAddJetton)
			{
				//构造变量
				tagJettonInfo JettonInfo;
				int nJettonSize=46;
				JettonInfo.cbJettonIndex=cbScoreIndex;
				JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize-20)+6;
				JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize-23-14)+6;

				//插入数组
				bPlaceJetton=true;
				m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
			}
			else
			{
				for (int nIndex=0; nIndex<m_JettonInfoArray[cbViewIndex-1].GetCount(); ++nIndex)
				{
					//移除判断
					tagJettonInfo &JettonInfo=m_JettonInfoArray[cbViewIndex-1][nIndex];
					if (JettonInfo.cbJettonIndex==cbScoreIndex)
					{
						m_JettonInfoArray[cbViewIndex-1].RemoveAt(nIndex);
						break;
					}
				}
			}
		}

		//减少数目
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	//更新界面
	if (bPlaceJetton==true) RefreshGameView();

	return;
}

//当局成绩
void CGameClientView::SetCurGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore, SCORE lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//设置胜方
void CGameClientView::SetWinnerSide(bool bWinTian, bool bWinDi, bool bWinXuan,bool bWinHuang, bool bSet)
{	
	//设置时间
	if (true==bSet)
	{
		//设置定时器
		SetTimer(IDI_FLASH_WINNER,500,NULL);

		//全胜判断
		if (false==bWinTian && false==bWinDi && false==bWinXuan &&false ==bWinHuang )
		{
			//清除资源
			m_ImageFrameTianMen.DeleteObject();
			m_ImageFrameDiMen.DeleteObject();
			m_ImageFrameHuangMen.DeleteObject();
			m_ImageFrameXuanMen.DeleteObject();

			//重设资源
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_ImageFrameTianMen.LoadBitmap(IDB_FRAME_TIAN_EMN);
			m_ImageFrameDiMen.LoadBitmap(IDB_FRAME_DI_EMN);
			m_ImageFrameHuangMen.LoadBitmap(IDB_FRAME_HUANG_EMN);
			m_ImageFrameXuanMen.LoadBitmap(IDB_FRAME_XUAN_EMN);


		}
	}
	else 
	{
		//清楚定时器
		KillTimer(IDI_FLASH_WINNER);

		//全胜判断
		if (false==m_bWinXuanMen && false==m_bWinTianMen && false==m_bWinHuangMen)
		{
			//清除资源
			m_ImageFrameTianMen.DeleteObject();
			m_ImageFrameXuanMen.DeleteObject();
			m_ImageFrameHuangMen.DeleteObject();
			m_ImageFrameXuanMen.DeleteObject();


			//重设资源
			HINSTANCE hInstance=AfxGetInstanceHandle();
		/*	m_ImageFrameTianMen.LoadFromResource(this,hInstance,IDB_FRAME_TIAN_MEN_EX);
			m_ImageFrameDiMen.LoadFromResource(this,hInstance,IDB_FRAME_DI_MEN_EX);
			m_ImageFrameHuangMen.LoadFromResource(this,hInstance,IDB_FRAME_HUANG_MEN_EX);
			m_ImageFrameXuanMen.LoadFromResource(this,hInstance,IDB_FRAME_XUAN_MEN_EX);*/

			m_ImageFrameTianMen.LoadBitmap(IDB_FRAME_TIAN_MEN_EX);
			m_ImageFrameDiMen.LoadBitmap(IDB_FRAME_DI_MEN_EX);
			m_ImageFrameHuangMen.LoadBitmap(IDB_FRAME_HUANG_MEN_EX);
			m_ImageFrameXuanMen.LoadBitmap(IDB_FRAME_XUAN_MEN_EX);
		}
	}

	//设置变量
	m_bWinTianMen=bWinTian;
	m_bWinHuangMen=bWinHuang;
	m_bWinXuanMen=bWinXuan;
	m_bWinDiMen=bWinDi;
	m_bFlashResult=bSet;
	m_bShowGameResult=bSet;
	m_cbAreaFlash=0xFF;

	//更新界面
	RefreshGameView();

	return;
}

//获取区域
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	if (m_rcTianMen.PtInRect(MousePoint))
	{
		return ID_TIAN_MEN;

	}else
	{
		if (m_rcDimen.PtInRect(MousePoint))
		{
			return ID_DI_MEN;

		}

	}
	if (m_rcHuangMen.PtInRect(MousePoint)) return ID_HUANG_MEN;	
	if (m_rcXuanMen.PtInRect(MousePoint)) return ID_XUAN_MEN ;	
	return 0xFF;
}
void CGameClientView::DrawType(CDC* pDC,WORD wChairID)
{
//	if (false==m_bShowGameResult) return;
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();
	
	if (cbGameStatus!=GS_GAME_END)
	{
		for(int i = 0;i < AREA_COUNT+1;i++)
		{
			m_cbUserCardType[i] = INVALID_CHAIR;
		}
		return;
	}


	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	//CImageHandle HandleImageCardType[5];
	for (int i = 0;i<5;i++)	
	{
		if(m_cbUserCardType[i] > 100)
			continue;
		int iIndex = 0;
		//iIndex = CT_SPECIAL_NIUNIUXW-2;
		//m_lUserCardType[IDC_BANK]
		if(m_cbUserCardType[i]!= CT_POINT)
		{
			iIndex = m_cbUserCardType[i]-2;
			if(iIndex>=13)
			{
				if(iIndex == 13)
				{
					iIndex = 12;
					//break;
				}
				if(iIndex == 14)
				{
					iIndex = 11;
					//break;
				}
				if(iIndex == 15)
				{
					iIndex = 13;
					//	break;
				}
			}
			else
			{
				if(iIndex>10)
				{
					iIndex = 10;
				}
				if(iIndex<0)
				{
					iIndex = 0;
				}
			}
			//iIndex = 11;
		}
		
		//CImageHandle HandleImageCardType(&m_ImageCardType[i]);
		//HandleImageCardType[i](&m_ImageCardType[i]);
		memDC.SelectObject(&m_ImageCardType[i]);
		pDC->TransparentBlt(m_CardTypePoint[i].x,m_CardTypePoint[i].y,1400/14,
			50,&memDC,1400/14*iIndex,0,1400/14,50,RGB(255,0,255));
		
		/*m_ImageCardType[i].TransDrawImage(pDC,m_CardTypePoint[i].x,m_CardTypePoint[i].y,m_ImageCardType[i].GetWidth()/14,
			m_ImageCardType[i].GetHeight(),m_ImageCardType[i].GetWidth()/14*iIndex,0,RGB(255,0,255));*/
	}
	memDC.DeleteDC();
}

//绘画数字
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//加载资源
	//CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	//CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	//CSize SizeScoreNumber(120/10,16);
	double dNumber = lNumber;
	CSize SizeScoreNumber(132/11,16);
	TCHAR scNumber[]=TEXT("0123456789.");
	CString strNumber;
	strNumber.Format(L"%.2f",dNumber);
	//if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());
	//if ( bMeScore ) SizeScoreNumber.SetSize(120/10,16);
	if ( bMeScore ) SizeScoreNumber.SetSize(132/11,16);

	//计算数目
	int lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//位置定义
	int nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	//int nXDrawPos=nXPos+lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;
	int nXDrawPos=nXPos+strNumber.GetLength()*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//绘画桌号
	//for (int i=0;i<lNumberCount;i++)
	for(int i= lNumberCount -1;i>=0;i--)
	{
		//绘画号码
		//int lCellNumber=(int)(lNumber%10);
		int lCellNumber=(int)(lNumber%11);

		CDC memDC;//定义一个设备上下文

		memDC.CreateCompatibleDC(pDC);//创建兼容的设备上下文

	//CBitmap bmp;//定义位图对象

		

		memDC.SelectObject(&m_ImageMeScoreNumber);//选中位图对象

	//pDC->BitBlt(nWidth/2 - 1006/2,nHeight/2 - 700/2,1006,700,&memDC,0,0,SRCCOPY);

		for ( INT j = 0; j < lstrlen(scNumber); ++j )
		{
			if ( strNumber[i] == scNumber[j] && strNumber[i] != '\0' )
			{
				lCellNumber = j;
			}
		}
		if ( bMeScore )
		{
			//m_ImageMeScoreNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				//lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
			pDC->TransparentBlt(nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,&memDC,lCellNumber*SizeScoreNumber.cx,0,132/11,16,RGB(255,0,255));
		}
		else
		{
			//m_ImageScoreNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				//lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
			pDC->TransparentBlt(nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,&memDC,lCellNumber*SizeScoreNumber.cx,0,132/11,16,RGB(255,0,255));
		}

		//设置变量
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;

		memDC.DeleteDC();
	};

	
	

	return;
}

//绘画数字
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber, strTempNum;
	strTempNum.Format(_T("%I64d"), (lNumber>0?lNumber:-lNumber));

	int nLength = strTempNum.GetLength();
	if (nLength <= 3)
		strNumber.Format(_T("%I64d"), lNumber);
	else
	{
		for (int i = 0; i < int((nLength-1)/3); i++)
			strTempNum.Insert(nLength - 3*(i+1), _T(","));
		strNumber.Format(_T("%s%s"), (lNumber<0?_T("-"):_T("")), strTempNum);
	}

	//输出数字
	pDC->TextOut(nXPos,nYPos,strNumber);
}

//绘画数字
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
	LONGLONG lTmpNumber = lNumber;
	CString strNumber;
	CString strTmpNumber1;
	CString strTmpNumber2;
	bool blfirst = true;
	bool bLongNum = false;
	int nNumberCount = 0;

	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();

	if ( lNumber == 0 )
		strNumber=TEXT("0");

	if ( lNumber < 0 ) 
		lNumber =- lNumber;

	if( lNumber >= 100 )
		bLongNum = true;

	while(lNumber > 0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if ( nNumberCount == 3 )
		{
			if(blfirst)
			{
				strTmpNumber2 += (TEXT("") +strNumber);
				blfirst = false;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
			}

			strNumber = strTmpNumber2;
			nNumberCount = 0;
			strTmpNumber2 = TEXT("");
		}
		lNumber /= 10;
	}

	if ( strTmpNumber2.IsEmpty() == FALSE )
	{
		if( bLongNum )
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber = strTmpNumber2;
	}

	if ( lTmpNumber < 0 ) 
		strNumber = TEXT("-") + strNumber;
	//输出数字
	if (nFormat==-1) pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
	else pDC->DrawText(strNumber,rcRect,nFormat);

	/*if (nFormat==-1) 
		CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcRect, RGB(255,234,0), DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else 
		CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcRect, RGB(255,234,0), nFormat);*/
}
void CGameClientView::DrawNumberStringWithDot(CDC *pDC,DOUBLE lScore,CRect rcRect,INT nFormat )
{
		//double dNumber = 0;
		//dNumber = lNumber/1000.0;
		//CString strNumber;
		//strNumber.Format(L"%.2f",dNumber);
		//if(nFormat == -1)	pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
		//else	pDC->DrawText(strNumber,rcRect,nFormat);
		CString strScore;
	CString strReturn;
	DOUBLE lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%0.2f"), lNumber);

	CString strDecimal;
	strDecimal = strScore.Right(3);
	strScore.Delete( strScore.Find('.'), 3);

	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%3) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}

	if ( lScore < 0 )
		strReturn.Insert(0, '-');

	//if ( bPlus && lScore > 0 )
	//	strReturn.Insert(0, '+');

	strReturn += strDecimal;

	if(nFormat == -1)	pDC->DrawText(strReturn,rcRect,DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
	else	pDC->DrawText(strReturn,rcRect,nFormat);

	//return strReturn;

}
void CGameClientView::KillCardTime()
{
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDI_POSTCARD);
	KillTimer(IDI_OPENCARD);
	//KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_SHOWDISPATCH_CARD_TIP);
	for (int i = 0;i<5;i++)
	{
	  m_CardControl[i].m_blShowResult = false;
	  m_CardControl[i].m_blShowLineResult = false;
	}
}
//定时器消息
void CGameClientView::OnTimer(UINT nIDEvent)
{
	if (IDI_SICE == nIDEvent)
	{
		m_Sice[0].SetVisable(true);
		m_Sice[1].SetVisable(true);
		RefreshGameView();
		if (m_Sice[0].GetFrameIndex() == 0)
		{
			//m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(), TEXT("DICE_GO"));
		}
		m_Sice[0].SetFrameIndex(m_Sice[0].GetFrameIndex() + 1);
		m_Sice[1].SetFrameIndex(m_Sice[1].GetFrameIndex() + 1);

		if (m_Sice[0].IsFinish())
		{
				KillTimer(IDI_SICE);	//杀掉自己
				//不再显示骰子
				m_Sice[0].SetVisable(false);
				m_Sice[1].SetVisable(false);

				m_Sice[0].SetFrameIndex(0);
				m_Sice[1].SetFrameIndex(0);
				//开始发牌
				SetMoveCardTimer();
			
		}
	}
	//闪动胜方
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//设置变量
		m_bFlashResult=!m_bFlashResult;
		//更新界面
		RefreshGameView();
		return;
	}
	if(IDI_POSTCARD ==nIDEvent)
	{
			bool blKillTimer = true;
			for (int i = 0;i<5;i++)
			{
				if(m_CardControl[i].m_CardItemArray.GetCount()!=m_CardControl[i].m_CardTempItemArray.GetCount())
				{
					blKillTimer = false;
					break;
				}
			}
			m_PostCardIndex = m_PostCardIndex%5;  // 0 ~ 4
			m_CardControl[m_PostCardIndex].OnCopyCard();
			m_Zhuang.SetFrameIndex(m_PostCardIndex*8+1);
			m_Zhuang.ResetPerFrame();
			m_PostCardIndex++;

			if(blKillTimer)
			{	
				KillTimer(IDI_POSTCARD);
				
			}
			else
				m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		m_bcShowCount++;		
	}
	if (IDI_WITER_SEND == nIDEvent)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<m_CardControl[i].GetCardNum(); ++j)
				m_CardControl[i].AddFrame(j);
		}
		SetTimer(IDI_WITER_SEND,50,NULL);
		m_Zhuang.SetFrameIndex(m_Zhuang.GetFrameIndex() + 1);
		m_Zhuang.AddFrame();			///增加每帧计数器
		
		if(m_bcShowCount >= 26 /*&& m_Zhuang.isPerFrameFinish()*/)
		{
			m_Zhuang.SetFrameIndex(0);
			m_Zhuang.ResetPerFrame();
			KillTimer(IDI_WITER_SEND);
			SetTimer(IDI_OPENCARD,30,NULL);
		}
		RefreshGameView();
	}
	if(IDI_OPENCARD==nIDEvent)	///开牌时间
	{
		if(m_blAutoOpenCard == false)
		{
			while (1)
			{
				int i;
				bool blCopy = false;
				for ( i = 0;i<5;i++)
				{
					if(m_CardControl[i].m_CardItemArray.GetCount()!=5)
					{
						blCopy = true;
						break;
					}
					if(m_CardControl[i].m_CardTempItemArray.GetCount()==0)
					{
						blCopy = false;
						break;
					}
				}
				if(false == blCopy)
				{
					break;
				}else
				{
					m_CardControl[i].OnCopyCard();
				}
			}
			for (int i = 0;i<5;i++)
			{
				m_CardControl[i].m_blGameEnd = false;
				m_CardControl[i].m_blhideOneCard = false;
			}
			//更新界面
			RefreshGameView();
			KillTimer(IDI_OPENCARD);
			return ;
		}
		
		if(m_CardControl[m_OpenCardIndex%5].m_MovePoint.x<40&&m_CardControl[m_OpenCardIndex%5].m_blAdd)
		{
			m_CardControl[m_OpenCardIndex%5].m_blGameEnd = false;
			m_CardControl[m_OpenCardIndex%5].m_MovePoint.x+=5;
			m_CardControl[m_OpenCardIndex%5].m_blhideOneCard = false;
			//更新界面
			RefreshGameView();
			OutputDebugString(L"IDI_OPENCARD 1111111111111111111111111111111111");
		}
		else
		{

			RefreshGameView();
			m_CardControl[m_OpenCardIndex%5].m_blAdd = false;
			m_CardControl[m_OpenCardIndex%5].m_blGameEnd = false;
			if(m_CardControl[m_OpenCardIndex%5].m_MovePoint.x>10)
				m_CardControl[m_OpenCardIndex%5].m_MovePoint.x-=2;
			m_CardControl[m_OpenCardIndex%5].m_blhideOneCard = false;
				
			if(m_CardControl[m_OpenCardIndex%5].m_MovePoint.x<=10)
			{
				m_cbUserCardType[m_OpenCardIndex%5] = m_GameLogic.GetCardType(m_cbTableCardArray[m_OpenCardIndex%5],5);
				int cbType = m_cbUserCardType[m_OpenCardIndex%5]-2;
				if(cbType>10)
				{
					cbType = 10;
				}
				else if(cbType<0)
				{
					cbType = 0;
				}
				CString strSound;
				strSound.Format(L"NEW_%d",cbType);
				PlayGameSound(AfxGetInstanceHandle(),strSound);
				OutputDebugString(strSound);
				m_CardControl[m_OpenCardIndex%5].m_tMoveCard.m_blMoveCard = true;
				m_CardControl[m_OpenCardIndex%5].m_tMoveCard.m_blMoveFinish = true;
				m_CardControl[m_OpenCardIndex%5].m_MovePoint.x = 0;
				m_CardControl[m_OpenCardIndex%5].m_MovePoint.y = 0;
				m_CardControl[m_OpenCardIndex%5].m_blAdd = true;
				m_CardControl[m_OpenCardIndex%5].m_blGameEnd = true;

				if(m_lUserCardType[m_OpenCardIndex%5]==CT_POINT&&m_lUserCardType[m_OpenCardIndex%5]==CT_SPECIAL_BOMEBOME)
					m_CardControl[m_OpenCardIndex%5].m_blShowLineResult = true;
				else
					m_CardControl[m_OpenCardIndex%5].m_blShowLineResult = false;
				m_CardControl[m_OpenCardIndex%5].SetCardData(m_cbTableSortCardArray[m_OpenCardIndex%5],5,false);
				m_CardControl[m_OpenCardIndex%5].m_blShowResult = true;
				m_OpenCardIndex++;

				if(m_OpenCardIndex==6)
				{
					KillTimer(IDI_OPENCARD);
					//完成开牌
					FinishDispatchCard(true);
						
					for (int i = 0;i<5;i++)
					{
						m_CardControl[i].m_blGameEnd = true;
						m_CardControl[i].m_blhideOneCard = false;

						if(m_lUserCardType[i]==CT_POINT||m_lUserCardType[i]==CT_SPECIAL_BOMEBOME)
							m_CardControl[i].m_blShowLineResult = true;
						else
							m_CardControl[i].m_blShowLineResult = false;

						//if (m_blAutoOpenCard==false)
						{
							m_CardControl[i].SetCardData(m_cbTableSortCardArray[i],5,false);
							m_CardControl[i].m_blShowResult = true;
						}
					}
						
					m_blMoveFinish = true;
				}
			}
		}
		
	}
	//轮换庄家
	else if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );

		return;
	}
	else if (nIDEvent==IDI_DISPATCH_CARD)
	{
		//更新界面
		RefreshGameView();
		return;
	}
	else if (IDI_SHOWDISPATCH_CARD_TIP==nIDEvent)
	{
		SetDispatchCardTip(enDispatchCardTip_NULL);

	}else if(nIDEvent==IDI_MOVECARD_END)
	{
		KillTimer(IDI_MOVECARD_END);
	}

	__super::OnTimer(nIDEvent);

}

//鼠标消息
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{	//JJWillDo 删除下面
	/*int static a = 1;
	tagApplyUser ta;
	ta.strUserName.Format(_T("a%d"), a);
	m_ApplyUser.DeleteUser(ta);
	a++;*/

	for (int i = 0;i<5;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnLButtonDown(nFlags, Point);
		}
	}
		
	if (m_lCurrentJetton!=0L)
	{
		int iTimer = 1;
		//下注区域
		BYTE cbJettonArea=GetJettonArea(Point);
	
		//最大下注
		LONGLONG lMaxJettonScore=GetUserMaxJetton();

		if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
		{		
			return ;
		}
		//合法判断
		//if (lMaxJettonScore < m_lCurrentJetton*iTimer) return ;

		//发送消息
		if (cbJettonArea!=0xFF) SendEngineMessage(IDM_PLACE_JETTON,cbJettonArea,0);
	}
	RefreshGameView();

	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	
	for (int i = 0;i<5;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnLButtonUp(nFlags, Point);
			RefreshGameView();
		}
		
	}
	__super::OnLButtonUp(nFlags,Point);
}
//接受其他控件传来的消息
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
	CPoint *pPoint = (CPoint*)lParam;
	ScreenToClient(pPoint);
    OnLButtonUp(1,*pPoint);
	return 1;

}
//鼠标移动消息
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	for (int i = 0;i<5;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnMouseMove(nFlags, point);
			RefreshGameView();

		}

	}
	__super::OnMouseMove(nFlags,point);
}
//鼠标消息
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//设置变量
	m_lCurrentJetton=0L;

	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		RefreshGameView();
	}

	__super::OnLButtonDown(nFlags,Point);
}

//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton!=0L)
	{
		//获取区域
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//设置变量
		if ( m_cbAreaFlash!= cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
			RefreshGameView();
		}

		//区域判断
		if (cbJettonArea==0xFF)
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			return TRUE;
		}

		//最大下注
		LONGLONG lMaxJettonScore=GetUserMaxJetton();

		//合法判断
		int iTimer = 1;
		if ((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore || lMaxJettonScore < m_lCurrentJetton)
		{
			SetCursor(LoadCursor(NULL,IDC_NO));
				return TRUE;	
		}
	
		//设置光标
		switch (m_lCurrentJetton)
		{
		case 1:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
				return TRUE;
			}
		case 5:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)));
				return TRUE;
			}
		case 10:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000)));
				return TRUE;
			}
		case 50:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_50000)));
				return TRUE;
			}
		case 100:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)));
				return TRUE;
			}
		case 200:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_500000)));
				return TRUE;
			}
		case 500:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)));
				return TRUE;
			}
		case 1000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000000)));
				return TRUE;
			}		
		}
	}
	else
	{
		ClearAreaFlash();
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//轮换庄家
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{
	//轮换庄家
	if ( bChangeBanker )
	{
		SetTimer( IDI_SHOW_CHANGE_BANKER, 3000, NULL );
		m_bShowChangeBanker = true;
	}
	else
	{
		KillTimer( IDI_SHOW_CHANGE_BANKER );
		m_bShowChangeBanker = false ;
	}

	//更新界面
	RefreshGameView();
}

//上庄按钮
void CGameClientView::OnApplyBanker()
{
	SendEngineMessage(IDM_APPLY_BANKER,1,0);

}

//下庄按钮
void CGameClientView::OnCancelBanker()
{
	SendEngineMessage(IDM_APPLY_BANKER,0,0);
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
//庄家信息
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore) 
{
	//庄家椅子号
	WORD wBankerUser=INVALID_CHAIR;

	//查找椅子号
	if (0!=dwBankerUserID)
	{
		for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
		{
			//tagUserData const *pUserData=GetUserInfo(wChairID);
			IClientUserItem * pUserData=GetClientUserItem(wChairID);
			if (NULL!=pUserData && dwBankerUserID==pUserData->GetUserID())
			{
				wBankerUser=wChairID;
				break;
			}
		}
	}

	//切换判断
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lBankerWinScore=0L;	
		m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lBankerScore;
}

//绘画标识
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	//非空判断
	//m_nRecordLast = 14;
	if (m_nRecordLast==m_nRecordFirst) return;

	//资源变量
	//CImageHandle ImageHandleWinFlags(&m_ImageWinFlags);
	int nIndex = m_nScoreHead;
	COLORREF static clrOld ;

	clrOld = pDC->SetTextColor(RGB(255,234,0));
	int nDrawCount=0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < MAX_FALG_COUNT )
	{
		//胜利标识
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		bool  static bWinMen[4];
		bWinMen[0]=ClientGameRecord.bWinTianMen;
		bWinMen[1]=ClientGameRecord.bWinDiMen ;
		bWinMen[2]=ClientGameRecord.bWinXuanMen;
		bWinMen[3]=ClientGameRecord.bWinHuangMen;

		//操作结果
		enOperateResult static OperateResult[4];
		OperateResult[0]=ClientGameRecord.enOperateTianMen;
		OperateResult[1]=ClientGameRecord.enOperateDiMen;
		OperateResult[2]=ClientGameRecord.enOperateXuanMen;
		OperateResult[3]=ClientGameRecord.enOperateHuangMen;

		for (int i=0; i<4; ++i)
		{
			//位置变量
			int static nYPos=0,nXPos=0;
			nYPos=m_nWinFlagsExcursionY+i*24;
			nXPos=m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 31;

			//胜利标识
			int static nFlagsIndex=0;
			if (false==bWinMen[i])
			{
				nFlagsIndex=2;
			}
			else
			{
				nFlagsIndex=0;
			}
			if (OperateResult[i] != enOperateResult_NULL)
			{			
				nFlagsIndex+=1;

				if(nFlagsIndex>=4)
				{
					nFlagsIndex=3;
				}
			}
			//绘画标识
			/*m_ImageWinFlags.TransDrawImage( pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/4 , 
				m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/4 * nFlagsIndex, 0, RGB(255, 0, 255) );*/
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			memDC.SelectObject(&m_ImageWinFlags);
			pDC->TransparentBlt(nXPos-9, nYPos+5, 92/4 , 
				21,&memDC,92/4 * nFlagsIndex, 0,92/4,21,RGB(255,0,255));
			memDC.DeleteDC();
		}

		//移动下标
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}
	pDC->SetTextColor(clrOld);
}
//手工搓牌
afx_msg void CGameClientView::OnOpenCard()
{
	SendEngineMessage(IDM_OPEN_CARD,0,0);

}
//自动搓牌
afx_msg void CGameClientView::OnAutoOpenCard()
{
	SendEngineMessage(IDM_AUTO_OPEN_CARD,0,0);

}
//手工搓牌
afx_msg void CGameClientView::OnLastMusic()
{
	SendEngineMessage(IDM_CHANGE_MUSIC,1,0);

}
//自动搓牌
afx_msg void CGameClientView::OnNextMusic()
{
	SendEngineMessage(IDM_CHANGE_MUSIC,0,0);

}

//1
LRESULT CGameClientEngine::OnGetAccount( WPARAM wParam, LPARAM lParam )
{
	SendSocketData(SUB_C_GET_ACCOUNT);
	return true;
}

//2
LRESULT CGameClientEngine::OnCheakAccount( WPARAM wParam, LPARAM lParam )
{
	CMD_C_CheakAccount *pCheakAccount=(CMD_C_CheakAccount*)wParam;
	CMD_C_CheakAccount CheakAccount;
	myprintf(CheakAccount.dwUserAccount,sizeof(CheakAccount.dwUserAccount),_T("%s"),pCheakAccount->dwUserAccount);
	SendSocketData(SUB_C_CHEAK_ACCOUNT,&CheakAccount,sizeof(CheakAccount));
	return true;
}

//移动按钮
void CGameClientView::OnScoreMoveL()
{
	if ( m_nRecordFirst == m_nScoreHead ) return;

	m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( m_nScoreHead == m_nRecordFirst ) m_btScoreMoveL.EnableWindow(FALSE);

	m_btScoreMoveR.EnableWindow(TRUE);

	//更新界面
	RefreshGameView();
}

//移动按钮
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = ( m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount == MAX_FALG_COUNT ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == MAX_FALG_COUNT ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//更新界面
	RefreshGameView();
}

//显示结果
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//显示判断
	//if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END) return;
	if (false==m_bShowGameResult) return;

	int	nXPos = nWidth / 2 - 129;
	int	nYPos = nHeight / 2 - 178;
	//CImageHandle ImageHandleGameEnd(&m_ImageGameEnd);

	CRect rcAlpha(nXPos+2, nYPos+70, nXPos+2 + 260, nYPos+70+104);
	DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);
	//m_ImageGameEnd.TransDrawImage(pDC, nXPos+2, nYPos+70, RGB(255,0,255));
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(m_ImageGameEnd);
	pDC->TransparentBlt(nXPos+2, nYPos+70,260,104,&memDC,0,0,260,104,RGB(255,0,255));
	memDC.DeleteDC();


	pDC->SetTextColor(RGB(255,234,0));
	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40;
	rcMeWinScore.top = nYPos+70 + 32+5;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos+2 + 150;
	rcMeReturnScore.top = nYPos+70 + 32+5;
	rcMeReturnScore.right = rcMeReturnScore.left + 111;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	//DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithDot(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	//DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithDot(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40;
	rcBankerWinScore.top = nYPos+70 + 69+5-5;
	rcBankerWinScore.right = rcBankerWinScore.left + 111;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	CString strBankerCurGameScore;
	strBankerCurGameScore.Format(TEXT("%I64d"), m_lBankerCurGameScore);
	//DrawNumberStringWithSpace(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithDot(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

//透明绘画
bool CGameClientView::DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha)
{
	ASSERT(pDC != 0 && lpRect != 0 && clr != CLR_NONE);
	if(pDC == 0 || lpRect == 0 || clr == CLR_NONE)
	{
		return false;
	}
	//全透明
	if( abs(fAlpha) <= 0.000001 )
	{
		return true;
	}

	for(int l=lpRect->top; l<lpRect->bottom; l++)
	{
		for(int k=lpRect->left; k<lpRect->right; k++)
		{
			COLORREF clrBk = pDC->GetPixel(k, l);
			COLORREF clrBlend = RGB(GetRValue(clrBk)*(1-fAlpha)+GetRValue(clr)*fAlpha, 
								GetGValue(clrBk)*(1-fAlpha)+GetGValue(clr)*fAlpha,
								GetBValue(clrBk)*(1-fAlpha)+GetBValue(clr)*fAlpha);
			pDC->SetPixel(k, l, clrBlend);
		}
	}

	return true;
}

//最大下注
LONGLONG CGameClientView::GetUserMaxJetton()
{
	int iTimer = 5;
	//已下注额
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex]*iTimer;

	//庄家金币
	LONGLONG lBankerScore=2147483647;
	if (m_wBankerUser!=INVALID_CHAIR) lBankerScore=m_lBankerScore;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex]*iTimer;

	//区域限制
	LONGLONG lMeMaxScore;
	if((m_lMeMaxScore-lNowJetton)/iTimer>m_lAreaLimitScore)
	{
		lMeMaxScore= m_lAreaLimitScore*iTimer;

	}else
	{
		lMeMaxScore = m_lMeMaxScore-lNowJetton;
		lMeMaxScore = lMeMaxScore;
	}

	//庄家限制
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	lMeMaxScore /= iTimer; 

	//非零限制
	//ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//成绩设置
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

//绘画数字
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//绘画数字
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[nAreaIndex] > 0 )
			DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x,m_PointJettonNumber[nAreaIndex-1].y+25, true);
	}
}

//开始发牌
void CGameClientView::DispatchCard()
{
	//设置界面
	for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].m_CardItemArray.SetSize(0);
	//设置标识
	m_bNeedSetGameRecord=true;
}

//结束发牌
void CGameClientView::FinishDispatchCard(bool bNotScene)
{
	//完成判断
	if (m_bNeedSetGameRecord==false) return;

	//设置标识
	m_bNeedSetGameRecord=false;

	//删除定时器
	//KillTimer(IDI_DISPATCH_CARD);
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<5; ++j)
			m_CardControl[i].SetFrame(8,j);
	}


	////设置扑克
	//for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].SetCardData(m_cbTableCardArray[i],2);

	//推断赢家
	bool bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang;

	BYTE TianMultiple,diMultiple,TianXuanltiple,HuangMultiple;

	DeduceWinner(bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang,TianMultiple,diMultiple,TianXuanltiple,HuangMultiple);

	//保存记录
	if (bNotScene)
		SetGameHistory(bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang);

	//累计积分
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;

	//设置赢家
	SetWinnerSide(bWinTianMen, bWinDiMen, bWinXuanMen, bWinHuang, true);

	//播放声音
	if (m_lMeCurGameScore>0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
	else if (m_lMeCurGameScore<0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
	else m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
}

//终止动画
void CGameClientView::StopAnimal()
{
	//定时器
	KillTimer(IDI_POSTCARD);
	//KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDI_OPENCARD);

	FinishDispatchCard(true);

	for (int i = 0;i<4;i++)
	{
		m_CardControl[i].m_blGameEnd = true;
		m_CardControl[i].m_blhideOneCard = false;
	}
	m_blMoveFinish = true;
	m_bFlashResult = false;
}

//胜利边框
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	//合法判断
	if (m_cbAreaFlash==0xFF && false==m_bFlashResult) return;

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	//下注判断
	if (false==m_bFlashResult)
	{
		if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
		{
			switch (m_cbAreaFlash)
			{
			case ID_TIAN_MEN:	
				{
					//绘画图片
					//CImageHandle ImageHandle(&m_ImageFrameTianMen);
				
					//m_ImageFrameTianMen.TransDrawImage(pDC,m_rcTianMen.left, m_rcTianMen.top, RGB(255,0,255));
					memDC.SelectObject(&m_ImageFrameTianMen);
					pDC->TransparentBlt(m_rcTianMen.left, m_rcTianMen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
					break;
				}
			case ID_DI_MEN:
				{
					//绘画图片
					//CImageHandle ImageHandle(&m_ImageFrameDiMen);
					//m_ImageFrameDiMen.TransDrawImage(pDC,m_rcDimen.left, m_rcDimen.top, RGB(255,0,255));
					memDC.SelectObject(&m_ImageFrameDiMen);
					pDC->TransparentBlt(m_rcDimen.left, m_rcDimen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
					break;
				}
			case ID_XUAN_MEN:
				{	
					//绘画图片
					//CImageHandle ImageHandle(&m_ImageFrameXuanMen);	
					//m_ImageFrameXuanMen.TransDrawImage(pDC,m_rcXuanMen.left, m_rcXuanMen.top, RGB(255,0,255));
					memDC.SelectObject(&m_ImageFrameXuanMen);
					pDC->TransparentBlt(m_rcXuanMen.left, m_rcXuanMen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
					break;
				}
			case ID_HUANG_MEN:
				{
					//绘画图片
					//CImageHandle ImageHandle(&m_ImageFrameHuangMen);	
					//m_ImageFrameHuangMen.TransDrawImage(pDC,m_rcHuangMen.left, m_rcHuangMen.top, RGB(255,0,255));
					memDC.SelectObject(&m_ImageFrameHuangMen);
					pDC->TransparentBlt(m_rcHuangMen.left, m_rcHuangMen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
					break;
				}
			}
		}
		
	}
	else
	{
		//庄全胜判断
		if (false==m_bWinXuanMen && false==m_bWinTianMen && false==m_bWinHuangMen&&m_bWinDiMen == false)
		{
			//绘画图片
			/*m_ImageFrameTianMen.TransDrawImage(pDC,m_rcTianMen.left, m_rcTianMen.top, RGB(255,0,255));
			m_ImageFrameDiMen.TransDrawImage(pDC,m_rcDimen.left, m_rcDimen.top, RGB(255,0,255));	
			m_ImageFrameHuangMen.TransDrawImage(pDC,m_rcHuangMen.left, m_rcHuangMen.top, RGB(255,0,255));
			m_ImageFrameXuanMen.TransDrawImage(pDC,m_rcXuanMen.left, m_rcXuanMen.top, RGB(255,0,255));*/
			memDC.SelectObject(&m_ImageFrameTianMen);
			pDC->TransparentBlt(m_rcTianMen.left, m_rcTianMen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
			memDC.SelectObject(&m_ImageFrameDiMen);
			pDC->TransparentBlt(m_rcDimen.left, m_rcDimen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
			memDC.SelectObject(&m_ImageFrameHuangMen);
			pDC->TransparentBlt(m_rcHuangMen.left, m_rcHuangMen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
			memDC.SelectObject(&m_ImageFrameXuanMen);
			pDC->TransparentBlt(m_rcXuanMen.left, m_rcXuanMen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
		}
		else
		{
			//门判断
			if (true==m_bWinXuanMen)
			{
				//CImageHandle ImageHandleXuanMen(&m_ImageFrameXuanMen);
				//m_ImageFrameXuanMen.TransDrawImage(pDC,m_rcXuanMen.left, m_rcXuanMen.top, RGB(255,0,255));
				memDC.SelectObject(&m_ImageFrameXuanMen);
			pDC->TransparentBlt(m_rcXuanMen.left, m_rcXuanMen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
			}
			if (true==m_bWinTianMen)
			{
				//CImageHandle ImageHandleTianMen(&m_ImageFrameTianMen);
				//m_ImageFrameTianMen.TransDrawImage(pDC,m_rcTianMen.left, m_rcTianMen.top, RGB(255,0,255));
				memDC.SelectObject(&m_ImageFrameTianMen);
			pDC->TransparentBlt(m_rcTianMen.left, m_rcTianMen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
			}
			if (true==m_bWinHuangMen)
			{
				//CImageHandle ImageHandleHuangMen(&m_ImageFrameHuangMen);
				//m_ImageFrameHuangMen.TransDrawImage(pDC,m_rcHuangMen.left, m_rcHuangMen.top, RGB(255,0,255));
				memDC.SelectObject(&m_ImageFrameHuangMen);
			pDC->TransparentBlt(m_rcHuangMen.left, m_rcHuangMen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
			}
			if (true==m_bWinDiMen)
			{
				//CImageHandle ImageHandleDiMen(&m_ImageFrameDiMen);
				//m_ImageFrameDiMen.TransDrawImage(pDC,m_rcDimen.left, m_rcDimen.top, RGB(255,0,255));
				memDC.SelectObject(&m_ImageFrameDiMen);
			pDC->TransparentBlt(m_rcDimen.left, m_rcDimen.top,145,200,&memDC,0,0,145,200,RGB(255,0,255));
			}
		}
	}
	memDC.DeleteDC();
}


//推断赢家
void CGameClientView::DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple )
{
	//大小比较
	bWinTian=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[SHUN_MEN_INDEX],5,TianMultiple)==1?true:false;
	bWinDi=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[DUI_MEN_INDEX],5,diMultiple)==1?true:false;
	bWinXuan=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[DAO_MEN_INDEX],5,TianXuanltiple)==1?true:false;
	bWinHuan=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[HUAN_MEN_INDEX],5,HuangMultiple)==1?true:false;
}

//控件命令
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//获取ID
	WORD wControlID=LOWORD(wParam);

	//控件判断
	switch (wControlID)
	{
	case IDC_JETTON_BUTTON_100:
		{
			//设置变量
			m_lCurrentJetton=1L;
			break;
		}
	case IDC_JETTON_BUTTON_1000:
		{
			//设置变量
			m_lCurrentJetton=5L;
			break;
		}
	case IDC_JETTON_BUTTON_10000:
		{
			//设置变量
			m_lCurrentJetton=10L;
			break;
		}
	case IDC_JETTON_BUTTON_50000:
		{
			//设置变量
			m_lCurrentJetton=50L;
			break;
		}
	case IDC_JETTON_BUTTON_100000:
		{
			//设置变量
			m_lCurrentJetton=100L;
			break;
		}
	case IDC_JETTON_BUTTON_500000:
		{
			//设置变量
			m_lCurrentJetton=200L;
			break;
		}
	case IDC_JETTON_BUTTON_1000000:
		{
			//设置变量
			m_lCurrentJetton=500L;
			break;
		}
	case IDC_JETTON_BUTTON_5000000:
		{
			//设置变量
			m_lCurrentJetton=1000L;
			break;
		}
	case IDC_AUTO_OPEN_CARD:
		{
			break;
		}
	case IDC_OPEN_CARD:
		{
			break;
		}
	case IDC_BANK:
		{
			break;
		}
	case IDC_CONTINUE_CARD:
		{
			SendEngineMessage(IDM_CONTINUE_CARD,0,0);
			break;
		}
	}

	return CGameFrameView::OnCommand(wParam, lParam);
}

//我的位置
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
	//查找椅子号
	for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
	{
		//tagUserData const *pUserData=GetUserInfo(wChairID);
		IClientUserItem * pUserData=GetClientUserItem(wChairID);
		if (NULL!=pUserData && dwMeUserID==pUserData->GetUserID())
		{
			m_wMeChairID=wChairID;
			break;
		}
	}
}

//设置提示
void CGameClientView::SetDispatchCardTip(enDispatchCardTip DispatchCardTip)
{
	//设置变量
	m_enDispatchCardTip=DispatchCardTip;

	//设置定时器
	if (enDispatchCardTip_NULL!=DispatchCardTip) SetTimer(IDI_SHOWDISPATCH_CARD_TIP,2*1000,NULL);		
	else KillTimer(IDI_SHOWDISPATCH_CARD_TIP);

	//更新界面
	RefreshGameView();
}

//绘画庄家
void CGameClientView::DrawBankerInfo(CDC *pDC,int nWidth,int nHeight)
{
	//庄家信息																											
	pDC->SetTextColor(RGB(255,234,0));

	//获取玩家
	IClientUserItem * pUserData=m_wBankerUser==INVALID_CHAIR ? NULL :GetClientUserItem(m_wBankerUser);

	//位置信息
	CRect static StrRect;
	StrRect.left = nWidth/2-373+91;
	StrRect.top = nHeight/2 - 230-78;
	StrRect.right = StrRect.left + 104;
	StrRect.bottom = StrRect.top + 15;

	CString strNick;
	if(pUserData!=NULL)
	{
		strNick= pUserData->GetNickName();
		strNick = strNick.Left(4);
		strNick+=_T("****");
	}

	//庄家名字
	pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("系统坐庄"):TEXT("无人坐庄")):strNick, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );
	//CDFontEx::DrawText(this,pDC,  12, 400, pUserData==NULL?(m_bEnableSysBanker?TEXT("系统坐庄"):TEXT("无人坐庄")):pUserData->GetNickName(), StrRect, RGB(255,234,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);


	//庄家总分
	StrRect.left = nWidth/2-373-10-16+91;
	StrRect.top = nHeight/2 - 210+5-78;
	StrRect.right = StrRect.left + 104;
	StrRect.bottom = StrRect.top + 15;
	//DrawNumberStringWithSpace(pDC,pUserData==NULL?0:pUserData->GetUserScore(), StrRect);
	DrawNumberStringWithDot(pDC,pUserData==NULL?0:pUserData->GetUserScore(), StrRect);

	//庄家成绩
	StrRect.left = nWidth/2-373 -30+91;
	StrRect.top = nHeight/2 - 180-78;
	StrRect.right = StrRect.left + 104;
	StrRect.bottom = StrRect.top + 15;
	//DrawNumberStringWithSpace(pDC,m_lBankerWinScore,StrRect);
	DrawNumberStringWithDot(pDC,m_lBankerWinScore,StrRect);

	//庄家局数
	StrRect.left = nWidth/2-373-15+91;
	StrRect.top = nHeight/2 - 152-78;
	StrRect.right = StrRect.left + 104;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_wBankerTime,StrRect);
	//DrawNumberStringWithDot(pDC,m_wBankerTime,StrRect);
}

void CGameClientView::SetFirstShowCard(BYTE *bcCard)
{

	if (bcCard)
	{
		m_bcfirstShowCard[0] = bcCard[0];
		m_bcfirstShowCard[1] = bcCard[1];  //2-15 %5

		int nFirst = (m_GameLogic.GetCardValue(m_bcfirstShowCard[0]) +  m_GameLogic.GetCardValue(m_bcfirstShowCard[1]) - 1) % 5;

		/*m_Sice[0].SetSiceValue(m_bcfirstShowCard[0]);
		m_Sice[1].SetSiceValue(m_bcfirstShowCard[1]);*/

		//int nSum = bcCard[0] + bcCard[1];  2  3  4  5  6  7  8  9  10 11 12
		::srand(time(NULL));

		switch(nFirst)
		{
		case 0:		//庄  6  11
			if(rand() % 2 == 0)
			{
				int nFirstSice = rand()%5+1;
				m_Sice[0].SetSiceValue(nFirstSice);
				m_Sice[1].SetSiceValue(6-nFirstSice);
			}
			else
			{
				if(rand() % 2 == 0)
				{
					m_Sice[0].SetSiceValue(6);
					m_Sice[1].SetSiceValue(5);
				}
				else
				{
					m_Sice[0].SetSiceValue(5);
					m_Sice[1].SetSiceValue(6);
				}
			}
			break;
		case 1:		//东  2   7  12
			{
				int nRand = rand() % 3;
				if(nRand == 0)
				{
					m_Sice[0].SetSiceValue(1);
					m_Sice[1].SetSiceValue(1);
				}
				else if(nRand == 1) 
				{

					int nFirstSice = rand()%6+1;
					m_Sice[0].SetSiceValue(nFirstSice);
					m_Sice[1].SetSiceValue(7-nFirstSice);
				}
				else
				{
					m_Sice[0].SetSiceValue(6);
					m_Sice[1].SetSiceValue(6);
				}
				break;
			}
		case 2:		//南  3   8  
			if(rand() % 2 == 0)
			{
				int nFirstSice = rand()%2+1;
				m_Sice[0].SetSiceValue(nFirstSice);
				m_Sice[1].SetSiceValue(3-nFirstSice);
			}
			else
			{
				int nFirstSice = rand()%5+2;
				m_Sice[0].SetSiceValue(nFirstSice);
				m_Sice[1].SetSiceValue(8-nFirstSice);
			}
			break;
		case 3:		//西  4  9 
			if(rand() % 2 == 0)
			{
				int nFirstSice = rand()%4+1;
				m_Sice[0].SetSiceValue(nFirstSice);
				m_Sice[1].SetSiceValue(4-nFirstSice);
			}
			else
			{
				int nFirstSice = rand()%4+3;
				m_Sice[0].SetSiceValue(nFirstSice);
				m_Sice[1].SetSiceValue(9-nFirstSice);
			}
			break;
		case 4:		//北  5  10
			if(rand() % 2 == 0)
			{
				int nFirstSice = rand()%4+1;
				m_Sice[0].SetSiceValue(nFirstSice);
				m_Sice[1].SetSiceValue(5-nFirstSice);
			}
			else
			{
				int nFirstSice = rand()%3+4;
				m_Sice[0].SetSiceValue(nFirstSice);
				m_Sice[1].SetSiceValue(10-nFirstSice);
			}
			break;
		}
		SetTimer(IDI_SICE, 50, NULL);
	}
	/*else
		ZeroMemory(m_bcfirstShowCard, 0, sizeof(m_bcfirstShowCard));*/
}

//绘画玩家
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
	//合法判断
	if (INVALID_CHAIR==m_wMeChairID) return;

	IClientUserItem * pMeUserData=GetClientUserItem(m_wMeChairID);

	if ( pMeUserData != NULL )
	{
		//字体颜色
		pDC->SetTextColor(RGB(255,234,0));

		//总共下注
		LONGLONG lMeJetton=0L;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lMeJetton += m_lUserJettonScore[nAreaIndex];

		//设置位置
		CRect static rcDraw;
		rcDraw.left=m_MeInfoRect.left;
		rcDraw.top=m_MeInfoRect.top+13;
		rcDraw.right=rcDraw.left+114;
		rcDraw.bottom=rcDraw.top+15;

		//绘画帐号
		pDC->DrawText(pMeUserData->GetNickName(),lstrlen(pMeUserData->GetNickName()),rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);
		//CDFontEx::DrawText(this,pDC,  12, 400, pMeUserData->GetNickName(), rcDraw, RGB(255,234,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

		//绘画积分
		rcDraw.top=m_MeInfoRect.top+33 +6;
		rcDraw.bottom=rcDraw.top+15 ;
		//DrawNumberStringWithSpace(pDC,pMeUserData->GetUserScore()-lMeJetton,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);
		DrawNumberStringWithDot(pDC,pMeUserData->GetUserScore()/*-(SCORE)lMeJetton*/,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		//绘画成绩
		rcDraw.left = m_MeInfoRect.left+141;
		rcDraw.right = rcDraw.left+114;
		rcDraw.top=m_MeInfoRect.top+13;
		rcDraw.bottom=rcDraw.top+15;
		//DrawNumberStringWithSpace(pDC,m_lMeStatisticScore,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);
		DrawNumberStringWithDot(pDC,m_lMeStatisticScore,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		DOUBLE dJettonScore = 0;
		for(BYTE i = 0;i < AREA_COUNT+1;i++)
		{
			dJettonScore += m_lUserJettonScore[i];
		}
		//绘画成绩
// 		rcDraw.left = m_MeInfoRect.left+141;
// 		rcDraw.right = rcDraw.left+114;
		rcDraw.top=m_MeInfoRect.top+33 +6;
		rcDraw.bottom=rcDraw.top+15;
		//DrawNumberStringWithSpace(pDC,m_lMeStatisticScore,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);
		DrawNumberStringWithDot(pDC,dJettonScore,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);
	}
}

//银行存款
void CGameClientView::OnBankStorage()
{
	//获取接口
	//CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)m_pGameClientDlg->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		//m_btBankerStorage.GetWindowRect(&btRect);
//		ShowInsureSave(pIClientKernel,CPoint(btRect.left-200,btRect.top));
	}
}

//银行取款
void CGameClientView::OnBankDraw()
{
	//获取接口
	//CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)m_pGameClientDlg->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		//m_btBankerDraw.GetWindowRect(&btRect);
//		ShowInsureGet(pIClientKernel,CPoint(btRect.left-200,btRect.top));
	}
}

void CGameClientView::OnUp()
{
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEUP,0),NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);

}

void CGameClientView::OnDown()
{
	tagSCROLLINFO scrollInfo = {};
	m_ApplyUser.m_AppyUserList.GetScrollInfo(SB_VERT, &scrollInfo);
	int nPos = scrollInfo.nPos - scrollInfo.nMin;
	if(m_ApplyUser.m_AppyUserList.GetItemCount()>MAX_APPLY_DISPLAY)
	{
		if(nPos>m_ApplyUser.m_AppyUserList.GetItemCount()-7)
		{
			return ;
		}
	}
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEDOWN,0),NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);
};

void CGameClientView::OpenAdminWnd()
{
	//如果有此权限
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;
	if(CUserRight::IsGameCheatUser(pIClientKernel->GetUserAttribute()->dwUserRight))
	//if(m_pClientControlDlg != NULL && (GetUserInfo(m_wMeChairID)->dwUserRight&UR_GAME_CONTROL)!=0)
	{
		if(!m_pClientControlDlg->IsWindowVisible()) 
			m_pClientControlDlg->ShowWindow(SW_SHOW);
		else 
			m_pClientControlDlg->ShowWindow(SW_HIDE);
	}
}


//更新视图
void CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

	return;
}
///////////////////////////////////////////////////////////////////////////////////////////