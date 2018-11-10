#include "stdafx.h"
#include "GamePlaza.h"
#include "LiuHeCai.h"
#include "BmpUtil.h"
/*#include "LuckMeDlg.h"*/
#include "MessageDlg.h"
#include "DlgAddtional.h"
#include "PlatformFrame.h"
#include "DlgFandian.h"
#include "GDIpng/MemDC.h"
#include   <afxinet.h> 
#include <map>
#include <wininet.h>
#include <fstream>
using namespace std;
map <BYTE,DOUBLE> mapJetton;
extern map <int,CString> mapFandian;
extern map <int,float> mapPercent;
extern map <int,float> mapDanweiFandian;
extern map <int,float> mapBonus;

IMPLEMENT_DYNAMIC(CLiuHeCai, CDialog)

static const int nian_left = 264;
static const int nian_top = 6;

static CRect rc_time_kj(593, 8, 593+200, 12 + 20);
static CRect rc_time_rest(44, 510, 200, 510 + 20);
//static CRect rcYuGao();
static const int btn_startX = 60;			//玩法按钮
static const int btn_startY = 33;			//玩法按钮

static const int btn_jetton_X = 220;			//玩法按钮

static const int btn_jetton_ok_X = 590;			//玩法按钮
static const int btn_jetton_ok_Y = 490;			//玩法按钮

static const int redraw_rc_left = 0;
static const int redraw_rc_top = 0;
static const int redraw_rc_width = 756;
static const int redraw_rc_height = 183;

static CRect rcRedraw(redraw_rc_left, redraw_rc_top, redraw_rc_left + redraw_rc_width, redraw_rc_top + redraw_rc_height);

char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr)
		delete[] wstr;
	return str;
}

static const int TimerFandian = 5;
static const int timer_id_kaijiangzhong=3;
static const int timer_update_resttime = 6;
#define IDC_JETTON_BUTTON_1		200									//按钮标识
#define IDC_JETTON_BUTTON_5		201									//按钮标识
#define IDC_JETTON_BUTTON_10	202									//按钮标识
#define IDC_JETTON_BUTTON_50	203									//按钮标识
#define IDC_JETTON_BUTTON_100	204									//按钮标识
#define IDC_JETTON_BUTTON_1000	205									//按钮标识

#define IDC_JETTON_OK			206									//按钮标识
#define IDC_JETTON_CANCEL		207									//按钮标识
#define IDC_JETTON_FANDIAN		208									//按钮标识

CLiuHeCai::CLiuHeCai(CWnd* pParent)
	: CDialog(CLiuHeCai::IDD, pParent)
	, m_bmpBk(NULL)
	, m_bmpTmdm(NULL)
	, m_bmpTmds(NULL)
	, m_bmpTmdx(NULL)
	, m_bmpTmsx(NULL)
	, m_bmpTmbs(NULL)
	, m_kjXinxiCont(0)
{
	m_DlgStatus.SetStatusViewSink(this);

	m_gameKind = LiuHeCai_Tmdm;
	m_fCurrentBet = 0.0f;
	for (int i=0; i<Kj_XinXi_Count; i++) 
	{
		memset(&m_kjXinxi[i], 0, sizeof(KjXinXi));
	}
	_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, _T(""));
	m_fBonus = 0.0;
	m_fPercent = 0.0;

	InitWanFaMoShi();

}
//取消连接
VOID CLiuHeCai::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}


CLiuHeCai::~CLiuHeCai()
{
	if (m_bmpBk != NULL) 
	{
		delete m_bmpBk;
	}
	if (m_bmpTmdm != NULL) 
	{
		delete m_bmpTmdm;
	}
	if (m_bmpTmds != NULL) 
	{
		delete m_bmpTmds;
	}
	if (m_bmpTmdx != NULL) 
	{
		delete m_bmpTmdx;
	}
	if (m_bmpTmsx != NULL) 
	{
		delete m_bmpTmsx;
	}
	if (m_bmpTmbs != NULL) 
	{
		delete m_bmpTmbs;
	}

}



void CLiuHeCai::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_TMDM, m_btnTmdm);
	DDX_Control(pDX, IDC_BTN_TMDS, m_btnTmds);
	DDX_Control(pDX, IDC_BTN_TMDX, m_btnTmdx);
	DDX_Control(pDX, IDC_BTN_TMSX, m_btnTmsx);
	DDX_Control(pDX, IDC_BTN_TMBS, m_btnTmbs);
}

BEGIN_MESSAGE_MAP(CLiuHeCai, CDialog)

	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BTN_TMDM, &CLiuHeCai::OnBnClickedBtnTmdm)
	ON_BN_CLICKED(IDC_BTN_TMDS, &CLiuHeCai::OnBnClickedBtnTmds)
	ON_BN_CLICKED(IDC_BTN_TMDX, &CLiuHeCai::OnBnClickedBtnTmdx)
	ON_BN_CLICKED(IDC_BTN_TMSX, &CLiuHeCai::OnBnClickedBtnTmsx)
	ON_BN_CLICKED(IDC_BTN_TMBS, &CLiuHeCai::OnBnClickedBtnTmbs)

	ON_BN_CLICKED(IDC_JETTON_BUTTON_1, &CLiuHeCai::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_5, &CLiuHeCai::OnBnClickedBtn5)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_10, &CLiuHeCai::OnBnClickedBtn10)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_50, &CLiuHeCai::OnBnClickedBtn50)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100, &CLiuHeCai::OnBnClickedBtn100)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000, &CLiuHeCai::OnBnClickedBtn1000)

	ON_BN_CLICKED(IDC_JETTON_OK, &CLiuHeCai::OnBnClickedJettonOK)
	ON_BN_CLICKED(IDC_JETTON_CANCEL, &CLiuHeCai::OnBnClickedCancelJetton)
	ON_BN_CLICKED(IDC_JETTON_FANDIAN, &CLiuHeCai::OnBnClickedFandian)

END_MESSAGE_MAP()


// CLiuHeCai 消息处理程序
void CLiuHeCai::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	CDC cacheDC;
	cacheDC.CreateCompatibleDC(&dc);

	CBitmap cacheBmp;
	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);

	Graphics graphics(cacheDC.m_hDC);
	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);
	cacheDC.SetBkMode(TRANSPARENT);
	cacheDC.SetTextColor(RGB(88, 78, 77));

	DrawTmdm(&cacheDC,graphics);
	DrawTmds(&cacheDC,graphics);
	DrawTmdx(&cacheDC,graphics);
	DrawTmsx(&cacheDC,graphics);
	DrawTmbs(&cacheDC,graphics);

	DrawFlashFrame(&cacheDC,rect.Width(), rect.Height());
	DrawJetton(&cacheDC,rect.Width(), rect.Height());
	DrawLuckNum(&cacheDC,rect.Width(), rect.Height());

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheDC.DeleteDC();
	cacheBmp.DeleteObject();

}

BOOL CLiuHeCai::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CLiuHeCai::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL) 
	{
		return;
	}

	int nSpace = m_btnTmdm.Width() + 3;
	if(m_btnTmdm.GetSafeHwnd() != NULL)
	{
		m_btnTmdm.SetWindowPos(NULL, btn_startX, btn_startY, m_btnTmdm.Width(), m_btnTmdm.Height(), SWP_NOZORDER);
	}
	if(m_btnTmds.GetSafeHwnd() != NULL)
	{
		m_btnTmds.SetWindowPos(NULL, btn_startX + nSpace, btn_startY, m_btnTmds.Width(), m_btnTmds.Height(), SWP_NOZORDER);
	}
	if(m_btnTmdx.GetSafeHwnd() != NULL)
	{
		m_btnTmdx.SetWindowPos(NULL, btn_startX + nSpace*2, btn_startY, m_btnTmdx.Width(), m_btnTmdx.Height(), SWP_NOZORDER);
	}
	if(m_btnTmsx.GetSafeHwnd() != NULL)
	{
		m_btnTmsx.SetWindowPos(NULL, btn_startX + nSpace*3, btn_startY, m_btnTmsx.Width(), m_btnTmsx.Height(), SWP_NOZORDER);
	}
	if(m_btnTmbs.GetSafeHwnd() != NULL)
	{
		m_btnTmbs.SetWindowPos(NULL, btn_startX + nSpace*4, btn_startY, m_btnTmbs.Width(), m_btnTmbs.Height(), SWP_NOZORDER);
	}

	CRect rcJetton;
	m_btJetton1.GetWindowRect(&rcJetton);

	nSpace = rcJetton.Width()+10;
	int btn_jetton_Y = cy-rcJetton.Height()-37;
	if(m_btJetton1.GetSafeHwnd() != NULL)
	{
		m_btJetton1.SetWindowPos(NULL, btn_jetton_X, btn_jetton_Y, rcJetton.Width(), rcJetton.Height(), SWP_NOZORDER);
	}
	if(m_btJetton5.GetSafeHwnd() != NULL)
	{
		m_btJetton5.SetWindowPos(NULL, btn_jetton_X + nSpace*1, btn_jetton_Y, rcJetton.Width(), rcJetton.Height(), SWP_NOZORDER);
	}
	if(m_btJetton10.GetSafeHwnd() != NULL)
	{
		m_btJetton10.SetWindowPos(NULL, btn_jetton_X + nSpace*2, btn_jetton_Y, rcJetton.Width(), rcJetton.Height(), SWP_NOZORDER);
	}
	if(m_btJetton50.GetSafeHwnd() != NULL)
	{
		m_btJetton50.SetWindowPos(NULL, btn_jetton_X + nSpace*3, btn_jetton_Y, rcJetton.Width(), rcJetton.Height(), SWP_NOZORDER);
	}
	if(m_btJetton100.GetSafeHwnd() != NULL)
	{
		m_btJetton100.SetWindowPos(NULL, btn_jetton_X + nSpace*4, btn_jetton_Y, rcJetton.Width(), rcJetton.Height(), SWP_NOZORDER);
	}
	if(m_btJetton1000.GetSafeHwnd() != NULL)
	{
		m_btJetton1000.SetWindowPos(NULL, btn_jetton_X + nSpace*5, btn_jetton_Y, rcJetton.Width(), rcJetton.Height(), SWP_NOZORDER);
	}
	CRect rcJetton1;

	m_btnJetton.GetWindowRect(&rcJetton1);
	nSpace = rcJetton1.Width()+10;

	if(m_btnJetton.GetSafeHwnd() != NULL)
	{
		m_btnJetton.SetWindowPos(NULL, btn_jetton_ok_X, btn_jetton_ok_Y, rcJetton1.Width(), rcJetton1.Height(), SWP_NOZORDER);
	}
	if(m_btnCancelJetton.GetSafeHwnd() != NULL)
	{
		m_btnCancelJetton.SetWindowPos(NULL, btn_jetton_ok_X + nSpace, btn_jetton_ok_Y, rcJetton1.Width(), rcJetton1.Height(), SWP_NOZORDER);
	}
	if(m_btnFandian.GetSafeHwnd() != NULL)
	{
		m_btnFandian.SetWindowPos(NULL, btn_jetton_ok_X, btn_jetton_ok_Y+rcJetton1.Height()+13, rcJetton1.Width(), rcJetton1.Height(), SWP_NOZORDER);
	}

}

void CLiuHeCai::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		m_btnFandian.EnableWindow(false);

		m_nMoshi = theAccount.Scoretype;
		DOUBLE fyue = theAccount.yue;

		SendToServer(6);

		if(m_nMoshi == MoShi_Yuan)
		{
			fyue = theAccount.yue;
		}
		else if(m_nMoshi == MoShi_Jiao)
		{
			fyue = theAccount.yue*10;
		}
		else if(m_nMoshi == MoShi_Fen)
		{
			fyue = theAccount.yue*100;
		}
		EnableJetton(fyue);
		ResetJetton();
		if(fyue > 1)
		{
			m_fCurrentBet = 1;
		}

		SetTimer(timer_update_resttime,1000,NULL);
	}
}
void CLiuHeCai::FlushZongjine()
{
	DOUBLE fyue = 0.0f;
	DOUBLE fAcyue = 0.0f;
	for(int i = 1;i < AREA_TMDM_MAX;i++)
	{
		fyue += mapJetton[i];
	}

	if(m_nMoshi == MoShi_Yuan)
	{
		fAcyue = theAccount.yue;
	}
	else if(m_nMoshi == MoShi_Jiao)
	{
		fAcyue = theAccount.yue*10;
	}
	else if(m_nMoshi == MoShi_Fen)
	{
		fAcyue = theAccount.yue*100;
	}

	fyue = (fAcyue - fyue);

	//DOUBLE fJetton = 0.0f;
	if(theAccount.Scoretype > m_nMoshi)
	{
		if((theAccount.Scoretype - m_nMoshi)==1)
		{
			fyue = fyue*10;
		}
		else if((theAccount.Scoretype - m_nMoshi)==2)
		{
			fyue = fyue*100;
		}
	}
	else if(theAccount.Scoretype < m_nMoshi)
	{
		if((m_nMoshi - theAccount.Scoretype  )==1)
			fyue = fyue/10;
		else if((m_nMoshi - theAccount.Scoretype)==2)
			fyue = fyue/100;
	}


	EnableJetton(fyue);
	for(int i = 1;i < AREA_TMDM_MAX;i++)
	{
		if(theAccount.Scoretype > m_nMoshi)
		{
			if((theAccount.Scoretype - m_nMoshi)==1)
			{
				mapJetton[i] = mapJetton[i]*10;
			}
			else if((theAccount.Scoretype - m_nMoshi)==2)
			{
				mapJetton[i] = mapJetton[i]*100;
			}
		}
		else if(theAccount.Scoretype < m_nMoshi)
		{
			if((m_nMoshi - theAccount.Scoretype  )==1)
				mapJetton[i] = mapJetton[i]/10;
			else if((m_nMoshi - theAccount.Scoretype)==2)
				mapJetton[i] = mapJetton[i]/100;
		}
	}


	m_nMoshi = theAccount.Scoretype;
	InvalidateRect(FALSE);
}

void CLiuHeCai::EnableJetton(DOUBLE fyue)
{
	m_btJetton1.EnableWindow((fyue>=1)?TRUE:FALSE);
	m_btJetton5.EnableWindow((fyue>=5)?TRUE:FALSE);
	m_btJetton10.EnableWindow((fyue>=10)?TRUE:FALSE);
	m_btJetton50.EnableWindow((fyue>50)?TRUE:FALSE);
	m_btJetton100.EnableWindow((fyue>100)?TRUE:FALSE);
	m_btJetton1000.EnableWindow((fyue>1000)?TRUE:FALSE);

}
HBRUSH CLiuHeCai::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLiuHeCai::OnTimer(UINT_PTR nIDEvent)
{

	if(TimerFandian == nIDEvent)
	{
		SendToServer(6);
	}
	else if(nIDEvent == timer_update_resttime)
	{
		InvalidateRect(&rc_time_rest);
	}
	CDialog::OnTimer(nIDEvent);

}

BOOL CLiuHeCai::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\pngCenter.png"));
	//m_Num2 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\nums_2.png"));
	m_bmpTmdm = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmdm.png"));
	m_bmpTmds = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmds.png"));
	m_bmpTmdx = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmdx.png"));
	m_bmpTmsx = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmsx.png"));
	m_bmpTmbs = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\tmbs.png"));

// 	m_Num2Width = m_Num2->GetWidth() / 10;
// 	m_Num2Height = m_Num2->GetHeight();

	m_nFrameX = 10000;
	m_nFrameY = 10000;
	m_nTzhSign=1;
	m_nTzhSign1=0;
	m_bKaiJiangzhong=false;
	m_cbCurrentArea = 0xff;
	m_cbOldArea = 0xff;
	m_bShowFail = true;
	//下注按钮
	CRect rcCreate(0,0,0,0);

	m_btJetton1.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1);
	m_btJetton5.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5);
	m_btJetton10.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10);
	m_btJetton50.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_50);
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);

	m_btnJetton.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_OK);
	m_btnCancelJetton.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_CANCEL);
	m_btnFandian.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_FANDIAN);

	//设置按钮
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageTmdmFrame.LoadImage(hResInstance,TEXT("IMAGE_TMDM_FRAME"));
	m_ImageTmdsFrame.LoadImage(hResInstance,TEXT("IMAGE_TMDS_FRAME"));
	m_ImageTmsxFrame.LoadImage(hResInstance,TEXT("IMAGE_TMSX_FRAME"));
	m_ImageTmbsFrame.LoadImage(hResInstance,TEXT("IMAGE_TMBS_FRAME"));
	m_btJetton1.SetButtonImage(IDB_BT_JETTON_1,hResInstance,false,false);
	m_btJetton5.SetButtonImage(IDB_BT_JETTON_5,hResInstance,false,false);
	m_btJetton10.SetButtonImage(IDB_BT_JETTON_10,hResInstance,false,false);
	m_btJetton50.SetButtonImage(IDB_BT_JETTON_50,hResInstance,false,false);
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false,false);

	m_btnJetton.SetButtonImage(IDB_BT_JETTON,hResInstance,false,false);
	m_btnCancelJetton.SetButtonImage(IDB_BT_JETTON_CANCEL,hResInstance,false,false);
	m_btnFandian.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_lock.png"));
	m_btnFandian.SetTextColor(RGB(88, 78, 77));
	m_ImgNum.LoadImage(hResInstance,TEXT("LHC_NUM"));
	m_ImageLuckNum.LoadImage(hResInstance,TEXT("LHC_LUCK_NUM"));
	m_ImageLuckBall.LoadImage(hResInstance,TEXT("LHC_LUCK_BALL"));
	m_Num2.LoadImage(hResInstance,TEXT("LHC_QI_NUM"));
	InitBtns();
	//区域位置
	CPoint ptBenchmark( 0, 70 );
	CRect rectArea;

	for ( int i = 0; i < AREA_TMDM_MAX; ++i )
	{
		m_ArrayTmdmBetArea[i].RemoveAll();
	}

	for ( int i = 1; i < AREA_TMDM_MAX; ++i )
	{
		//闲对子
		rectArea.SetRect( ptBenchmark.x , ptBenchmark.y , ptBenchmark.x + 153, ptBenchmark.y + 42);
		m_ArrayTmdmBetArea[i].Add(rectArea);

		mapJetton.insert(make_pair(i,0.00f));
		ptBenchmark.x += 153;

		if(i % 5 == 0 )
		{
			ptBenchmark.x=0;
			ptBenchmark.y += 41;
		}

	}

	ptBenchmark.x = 29;
	ptBenchmark.y = 160;
	for ( int i = 0; i < AREA_TMDS_MAX; ++i )
	{
		m_ArrayTmdsBetArea[i].RemoveAll();
	}
	for ( int i = 1; i < AREA_TMDS_MAX; ++i )
	{
		//闲对子
		rectArea.SetRect( ptBenchmark.x , ptBenchmark.y , ptBenchmark.x + 333, ptBenchmark.y + 164);
		m_ArrayTmdsBetArea[i].Add(rectArea);

		ptBenchmark.x += 372;

	}

	ptBenchmark.x = 29;
	ptBenchmark.y = 162;
	for ( int i = 0; i < AREA_TMDX_MAX; ++i )
	{
		m_ArrayTmdxBetArea[i].RemoveAll();
	}
	for ( int i = 1; i < AREA_TMDX_MAX; ++i )
	{
		//闲对子
		rectArea.SetRect( ptBenchmark.x , ptBenchmark.y , ptBenchmark.x + 333, ptBenchmark.y + 164);
		m_ArrayTmdxBetArea[i].Add(rectArea);

		ptBenchmark.x += 372;

	}

	ptBenchmark.x = 4;
	ptBenchmark.y = 90;
	for ( int i = 0; i < AREA_TMSX_MAX; ++i )
	{
		m_ArrayTmsxBetArea[i].RemoveAll();
	}
	for ( int i = 1; i < AREA_TMSX_MAX; ++i )
	{
		//闲对子
		rectArea.SetRect( ptBenchmark.x , ptBenchmark.y , ptBenchmark.x + 376, ptBenchmark.y + 50);
		m_ArrayTmsxBetArea[i].Add(rectArea);

		ptBenchmark.y += 55;
		if(i == 6)
		{
			ptBenchmark.x += 382;
			ptBenchmark.y = 90;
		}

	}

	ptBenchmark.x = 5;
	ptBenchmark.y = 78;
	for ( int i = 0; i < AREA_TMBS_MAX; ++i )
	{
		m_ArrayTmbsBetArea[i].RemoveAll();
	}
	for ( int i = 1; i < AREA_TMBS_MAX; ++i )
	{
		//闲对子
		rectArea.SetRect( ptBenchmark.x , ptBenchmark.y , ptBenchmark.x + 690, ptBenchmark.y + 112);
		m_ArrayTmbsBetArea[i].Add(rectArea);

		ptBenchmark.y += 112;
	}

	return TRUE;
}
CString CLiuHeCai::GetWebData()
{
	TCHAR szUrl[] = _T("http://www.33789.com/kj.htm");
	TCHAR szAgent[] = _T("");
	HINTERNET hInternet1 =  InternetOpen(_T("Microsoft Internet Explorer"),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if (NULL == hInternet1)
	{
		InternetCloseHandle(hInternet1);
		return FALSE;
	}
	HINTERNET hInternet2 =  InternetOpenUrl(hInternet1,szUrl,NULL,NULL,INTERNET_FLAG_NO_CACHE_WRITE,NULL);
	if (NULL == hInternet2)
	{
		InternetCloseHandle(hInternet2);
		InternetCloseHandle(hInternet1);
		return FALSE;
	}

	char szBuffer[1024] = {0};
	DWORD dwByteRead = 0;

	//	ofstream ofs(_T("page.html"));

	CString strContent;
	// 循环读取缓冲区内容直到结束
	while (InternetReadFile(hInternet2, szBuffer, 1023, &dwByteRead) && dwByteRead > 0)
	{
		CString strTemp;
		//ofs<<szBuffer;
		strTemp=szBuffer;
		strContent += strTemp;
		// 清空缓冲区以备下一次读取
		ZeroMemory(szBuffer, sizeof(szBuffer));
	}
	//	ofs.close();
	InternetCloseHandle(hInternet2);
	InternetCloseHandle(hInternet1); 

	strContent.Replace(L"<b>",L"");
	strContent.Replace(L"</b>",L"");
	strContent.Replace(L"&nbsp;",L"");
	int nMid = strContent.Find(L"开奖时间");

	strContent = strContent.Mid(nMid-6,18);
	return strContent;

}
//获取区域
BYTE CLiuHeCai::GetBetArea(CPoint MousePoint)
{
	if(m_gameKind == LiuHeCai_Tmdm)
	{
		for ( int i = 1 ; i < AREA_TMDM_MAX; ++i )
		{
			for ( int n = 0; n < m_ArrayTmdmBetArea[i].GetCount(); ++n )
			{
				if ( m_ArrayTmdmBetArea[i][n].PtInRect(MousePoint))
				{
					return i;
				}
			}
		}
	}
	else if(m_gameKind == LiuHeCai_Tmds)
	{
		for ( int i = 1 ; i < AREA_TMDS_MAX; ++i )
		{
			for ( int n = 0; n < m_ArrayTmdsBetArea[i].GetCount(); ++n )
			{
				if ( m_ArrayTmdsBetArea[i][n].PtInRect(MousePoint))
				{
					return i;
				}
			}
		}

	}
	else if(m_gameKind == LiuHeCai_Tmdx)
	{
		for ( int i = 1 ; i < AREA_TMDX_MAX; ++i )
		{
			for ( int n = 0; n < m_ArrayTmdxBetArea[i].GetCount(); ++n )
			{
				if ( m_ArrayTmdxBetArea[i][n].PtInRect(MousePoint))
				{
					return i;
				}
			}
		}

	}
	else if(m_gameKind == LiuHeCai_Tmsx)
	{
		for ( int i = 1 ; i < AREA_TMSX_MAX; ++i )
		{
			for ( int n = 0; n < m_ArrayTmsxBetArea[i].GetCount(); ++n )
			{
				if ( m_ArrayTmsxBetArea[i][n].PtInRect(MousePoint))
				{
					return i;
				}
			}
		}

	}
	else if(m_gameKind == LiuHeCai_Tmbs)
	{
		for ( int i = 1 ; i < AREA_TMBS_MAX; ++i )
		{
			for ( int n = 0; n < m_ArrayTmbsBetArea[i].GetCount(); ++n )
			{
				if ( m_ArrayTmbsBetArea[i][n].PtInRect(MousePoint))
				{
					return i;
				}
			}
		}

	}
	return 0xFF;
}

void CLiuHeCai::DrawNum2(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_Num2Width, 0, m_Num2Width, m_Num2Height, UnitPixel);
}


void CLiuHeCai::ResetJetton()
{
	for(BYTE i = 1;i < AREA_TMDM_MAX;i++)
	{
		mapJetton[i] = 0.0f;
	}
	CRect rcRect(0,70,769,485);
	RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	rcRect.SetRect(192,539,769,561);
	RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	return;
}
//特码单买
void CLiuHeCai::DrawTmdm(CDC* pDC, Graphics& graphics)
{
	if(m_gameKind != LiuHeCai_Tmdm)
		return;
	CRect rect;
	GetClientRect(&rect);

	graphics.DrawImage(m_bmpTmdm, Rect(0, 70, m_bmpTmdm->GetWidth(), m_bmpTmdm->GetHeight()), 0, 0, m_bmpTmdm->GetWidth(), m_bmpTmdm->GetHeight(), UnitPixel);

	return;
}
//特码单双
void CLiuHeCai::DrawTmds(CDC* pDC, Graphics& graphics)
{
	if(m_gameKind != LiuHeCai_Tmds)
		return;
	CRect rect;
	GetClientRect(&rect);

	graphics.DrawImage(m_bmpTmds, Rect(0, 70, m_bmpTmds->GetWidth(), m_bmpTmds->GetHeight()), 0, 0, m_bmpTmds->GetWidth(), m_bmpTmds->GetHeight(), UnitPixel);

	return;
}
//特码大小
void CLiuHeCai::DrawTmdx(CDC* pDC, Graphics& graphics)
{
	if(m_gameKind != LiuHeCai_Tmdx)
		return;
	CRect rect;
	GetClientRect(&rect);

	graphics.DrawImage(m_bmpTmdx, Rect(0, 70, m_bmpTmdx->GetWidth(), m_bmpTmdx->GetHeight()), 0, 0, m_bmpTmdx->GetWidth(), m_bmpTmdx->GetHeight(), UnitPixel);
	return;
}
//特码生肖
void CLiuHeCai::DrawTmsx(CDC* pDC, Graphics& graphics)
{
	if(m_gameKind != LiuHeCai_Tmsx)
		return;
	CRect rect;
	GetClientRect(&rect);

	graphics.DrawImage(m_bmpTmsx, Rect(0, 70, m_bmpTmsx->GetWidth(), m_bmpTmsx->GetHeight()), 0, 0, m_bmpTmsx->GetWidth(), m_bmpTmsx->GetHeight(), UnitPixel);
	return;
}
//特码波色
void CLiuHeCai::DrawTmbs(CDC* pDC, Graphics& graphics)
{
	if(m_gameKind != LiuHeCai_Tmbs)
		return;
	CRect rect;
	GetClientRect(&rect);

	graphics.DrawImage(m_bmpTmbs, Rect(0, 70, m_bmpTmbs->GetWidth(), m_bmpTmbs->GetHeight()), 0, 0, m_bmpTmbs->GetWidth(), m_bmpTmbs->GetHeight(), UnitPixel);
	return;
}

void CLiuHeCai::InitBtns()
{
	CString str = CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk2.png");
	m_btnTmdm.SetBkImage(str);
	m_btnTmds.SetBkImage(str);
	m_btnTmdx.SetBkImage(str);
	m_btnTmsx.SetBkImage(str);
	m_btnTmbs.SetBkImage(str);

	m_btnTmdm.SetPushed(true);
}
//特码单买
void CLiuHeCai::OnBnClickedBtnTmdm()
{
	m_cbOldArea= 0xff;
	m_cbCurrentArea = 0xff;
	m_gameKind = LiuHeCai_Tmdm;
	m_btnTmdm.SetPushed(true);
	m_btnTmds.SetPushed(false);
	m_btnTmdx.SetPushed(false);
	m_btnTmsx.SetPushed(false);
	m_btnTmbs.SetPushed(false);
	SendToServer(3);
	return;
}
//特码单双
void CLiuHeCai::OnBnClickedBtnTmds()
{
	m_cbOldArea= 0xff;
	m_cbCurrentArea = 0xff;
	m_gameKind = LiuHeCai_Tmds;
	m_btnTmdm.SetPushed(false);
	m_btnTmds.SetPushed(true);
	m_btnTmdx.SetPushed(false);
	m_btnTmsx.SetPushed(false);
	m_btnTmbs.SetPushed(false);

	SendToServer(3);
	return;
}
//特码大小
void CLiuHeCai::OnBnClickedBtnTmdx()
{
	m_cbOldArea= 0xff;
	m_cbCurrentArea = 0xff;
	m_gameKind = LiuHeCai_Tmdx;
	m_btnTmdm.SetPushed(false);
	m_btnTmds.SetPushed(false);
	m_btnTmdx.SetPushed(true);
	m_btnTmsx.SetPushed(false);
	m_btnTmbs.SetPushed(false);

	SendToServer(3);
	return;
}
//特码生肖
void CLiuHeCai::OnBnClickedBtnTmsx()
{
	m_cbOldArea= 0xff;
	m_cbCurrentArea = 0xff;
	m_gameKind = LiuHeCai_Tmsx;
	m_btnTmdm.SetPushed(false);
	m_btnTmds.SetPushed(false);
	m_btnTmdx.SetPushed(false);
	m_btnTmsx.SetPushed(true);
	m_btnTmbs.SetPushed(false);
	SendToServer(3);
	return;
}
//特码波色
void CLiuHeCai::OnBnClickedBtnTmbs()
{
	m_cbOldArea= 0xff;
	m_cbCurrentArea = 0xff;
	m_gameKind = LiuHeCai_Tmbs;
	m_btnTmdm.SetPushed(false);
	m_btnTmds.SetPushed(false);
	m_btnTmdx.SetPushed(false);
	m_btnTmsx.SetPushed(false);
	m_btnTmbs.SetPushed(true);
	SendToServer(3);
	return;

}

void CLiuHeCai::OnBnClickedBtn1()
{
	m_fCurrentBet = 1.0f;
}
void CLiuHeCai::OnBnClickedBtn5()
{
	m_fCurrentBet = 5.0f;
}
void CLiuHeCai::OnBnClickedBtn10()
{
	m_fCurrentBet = 10.0f;
}
void CLiuHeCai::OnBnClickedBtn50()
{
	m_fCurrentBet = 50.0f;
}
void CLiuHeCai::OnBnClickedBtn100()
{
	m_fCurrentBet = 100.0f;
}
void CLiuHeCai::OnBnClickedBtn1000()
{
	m_fCurrentBet = 1000.0f;
}
void CLiuHeCai::OnBnClickedJettonOK()
{
	int nZhushu = 0;
	for (int i = 1;i < AREA_TMDM_MAX;i++)
	{
		if(mapJetton[i] < 0.0000001)
			continue;
		nZhushu ++;
	}
	if(nZhushu <= 0)
	{
		MyMessageBox(L"请选择投注区域！");
		return;
	}

	CString strQiShu;
	strQiShu.Format(L"%s",m_kjXinxi[0].qihao);
	if (strQiShu.IsEmpty())
	{
		MyMessageBox(_T("正在获取数据…… "));
		return ;
	}

	CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
	ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
	TouzhuCQSSC.dwUserID = theAccount.user_id;
	TouzhuCQSSC.nBeitou = 1;
	TouzhuCQSSC.nGameKind = m_gameKind;
	TouzhuCQSSC.nGameType = CZ_LIUHECAI;
	TouzhuCQSSC.nMoshi = theAccount.Scoretype;
	TouzhuCQSSC.bZhuihao = 0;
	TouzhuCQSSC.nSign = m_nTzhSign;
	TouzhuCQSSC.nEnd = 0;
	TouzhuCQSSC.nIndex = 0;
	TouzhuCQSSC.nZhushu = 1;

	int nAreaCount = 0;
	CString strHaoma;
	if(m_gameKind == LiuHeCai_Tmdm)
	{
		nAreaCount = AREA_TMDM_MAX;
	}
	else if(m_gameKind == LiuHeCai_Tmds)
	{
		nAreaCount = AREA_TMDS_MAX;
	}
	else if(m_gameKind == LiuHeCai_Tmdx)
	{
		nAreaCount = AREA_TMDX_MAX;
	}
	else if(m_gameKind == LiuHeCai_Tmsx)
	{
		nAreaCount = AREA_TMSX_MAX;
	}
	else if(m_gameKind == LiuHeCai_Tmbs)
	{
		nAreaCount = AREA_TMBS_MAX;
	}

	// 	CString strQihao;
	// 	strQihao.Format(L"%s",m_kjXinxi[0].qihao);

	int nQihao = _ttoi(strQiShu.GetBuffer());
	nQihao+=1;

	strQiShu.Format(L"%d",nQihao);
	string strQi;
	ChangeStringToA(strQiShu, strQi);

	memcpy(TouzhuCQSSC.strQishu ,strQi.c_str(),strQi.length());
	m_bShowFail = true;
	for(int i = 1;i<nAreaCount;i++)
	{
		if(mapJetton[i]<0.000001)
			continue;
		CString str;
		if(m_gameKind == LiuHeCai_Tmsx)
		{
			BYTE cbArea = 1;
			if(i == 1)
			{
				cbArea = 8;
			}
			else if(i == 2)
			{
				cbArea = 7;
			}
			else if(i == 3)
			{
				cbArea = 6;
			}
			else if(i == 4)
			{
				cbArea = 5;
			}
			else if(i == 5)
			{
				cbArea = 4;
			}
			else if(i == 6)
			{
				cbArea = 3;
			}
			else if(i == 7)
			{
				cbArea = 2;
			}
			else if(i == 8)
			{
				cbArea = 1;
			}
			else if(i == 9)
			{
				cbArea = 12;
			}
			else if(i == 10)
			{
				cbArea = 11;
			}
			else if(i == 11)
			{
				cbArea = 10;
			}
			else if(i == 12)
			{
				cbArea = 9;
			}
			if(m_nMoshi == MoShi_Yuan)
				str.Format(L"%d,%.2lf|",cbArea,mapJetton[i]);
			else if (m_nMoshi == MoShi_Jiao)
			{
				str.Format(L"%d,%.2lf|",cbArea,mapJetton[i]/10);
			}
			else if(m_nMoshi == MoShi_Fen)
			{
				str.Format(L"%d,%.2lf|",cbArea,mapJetton[i]/100);
			}

		}
		else
		{
			if(m_nMoshi == MoShi_Yuan)
				str.Format(L"%d,%.2lf|",i,mapJetton[i]);
			else if (m_nMoshi == MoShi_Jiao)
			{
				str.Format(L"%d,%.2lf|",i,mapJetton[i]/10);
			}
			else if(m_nMoshi == MoShi_Fen)
			{
				str.Format(L"%d,%.2lf|",i,mapJetton[i]/100);
			}

		}
		strHaoma=str;

		string	strAllHaoma;
		ChangeStringToA(strHaoma, strAllHaoma);
		byte pDestBuffer[102400];
		int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));
		TouzhuCQSSC.nHaoMaLen = nLen;

		memset(TouzhuCQSSC.strHaoma, 0, sizeof(TouzhuCQSSC.strHaoma));
		int nSize = sizeof(TouzhuCQSSC.strHaoma);

		memcpy((byte*)TouzhuCQSSC.strHaoma, pDestBuffer, nSize);

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		bool bSuccess = pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_DAN,&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
		m_nTzhSign++;
	}


	strQiShu.ReleaseBuffer();
	ResetJetton();
	return;
}
bool	CLiuHeCai::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}

void CLiuHeCai::OnBnClickedCancelJetton()
{
	for ( int i = 1; i < AREA_TMDM_MAX; ++i )
	{
		mapJetton[i] = 0.00f;
	}
	CRect rcRect(0,70,769,485);
	RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

	DOUBLE fyue = theAccount.yue;

	if(m_nMoshi == MoShi_Yuan)
	{
		fyue = theAccount.yue;
	}
	else if(m_nMoshi == MoShi_Jiao)
	{
		fyue = theAccount.yue*10;
	}
	else if(m_nMoshi == MoShi_Fen)
	{
		fyue = theAccount.yue*100;
	}
	EnableJetton(fyue);


	return;
}
void CLiuHeCai::OnBnClickedFandian()
{
	int index = (CZ_LIUHECAI << 16) | (unsigned short)m_gameKind;

	CDlgFandian	dlg;

	dlg.SetBonus(mapBonus[m_gameKind], theBonus[index], mapPercent[m_gameKind], theAccount.fandian*100.0, mapDanweiFandian[m_gameKind]);
	if(IDOK == dlg.DoModal())
	{
		dlg.GetBonus(m_fBonus,  m_fPercent);

		CMD_GP_SetUserBonus SetUserBonus;
		ZeroMemory(&SetUserBonus,sizeof(SetUserBonus));

		SetUserBonus.n_t_userid = theAccount.user_id;
		SetUserBonus.n_t_type_id = CZ_LIUHECAI;
		SetUserBonus.n_t_kind_id = m_gameKind;
		SetUserBonus.f_t_bonus = m_fBonus;
		SetUserBonus.f_bonuspercent = m_fPercent;

		//发送数据
		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SET_USER_BONUS,&SetUserBonus,sizeof(SetUserBonus));

	}

	return;
}

BOOL CLiuHeCai::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if ( m_fCurrentBet > 0.000001)
	{
		//获取区域
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		BYTE cbArea = GetBetArea(MousePoint);

		if(cbArea > AREA_TMDM_MAX)
		{
			SetCursor( LoadCursor(NULL, IDC_ARROW) );
			return TRUE;

		}

		SetCursor( LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)) );
		return TRUE;
	}
	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}
//胜利边框
void CLiuHeCai::DrawFlashFrame(CDC *pDC, int nWidth, int nHeight)
{

 	if(m_cbCurrentArea == 0xff )
 		return;

	if( m_gameKind == LiuHeCai_Tmdm)
	{

		CRect rcRect(m_ArrayTmdmBetArea[m_cbCurrentArea][0]);
		m_nFrameX = rcRect.left;
		m_nFrameY = rcRect.top;

		m_ImageTmdmFrame.DrawImage(pDC, m_nFrameX,m_nFrameY);

	}
	else if( m_gameKind == LiuHeCai_Tmds)
	{

		CRect rcRect(m_ArrayTmdsBetArea[m_cbCurrentArea][0]);
		m_nFrameX = rcRect.left;
		m_nFrameY = rcRect.top;

		m_ImageTmdsFrame.DrawImage(pDC, m_nFrameX,m_nFrameY);

	}
	else if( m_gameKind == LiuHeCai_Tmdx)
	{

		CRect rcRect(m_ArrayTmdxBetArea[m_cbCurrentArea][0]);
		m_nFrameX = rcRect.left;
		m_nFrameY = rcRect.top;

		m_ImageTmdsFrame.DrawImage(pDC, m_nFrameX,m_nFrameY);

	}
	else if( m_gameKind == LiuHeCai_Tmsx)
	{

		CRect rcRect(m_ArrayTmsxBetArea[m_cbCurrentArea][0]);
		m_nFrameX = rcRect.left;
		m_nFrameY = rcRect.top;

		m_ImageTmsxFrame.DrawImage(pDC, m_nFrameX,m_nFrameY);

	}
	else if( m_gameKind == LiuHeCai_Tmbs)
	{

		CRect rcRect(m_ArrayTmbsBetArea[m_cbCurrentArea][0]);
		m_nFrameX = rcRect.left;
		m_nFrameY = rcRect.top;

		m_ImageTmbsFrame.DrawImage(pDC, m_nFrameX,m_nFrameY);
	}

}



//鼠标消息
VOID CLiuHeCai::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//获取区域
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	m_cbCurrentArea = GetBetArea(MousePoint);

	if(m_cbOldArea != m_cbCurrentArea && m_cbCurrentArea!=0xff)
	{
		m_cbOldArea = m_cbCurrentArea;

		CRect rcRect(0,70,769,485);
		RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

}


//鼠标消息
VOID CLiuHeCai::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	if(m_fCurrentBet>0.000001)
	{
		if(m_gameKind == LiuHeCai_Tmdm)
		{
			BYTE cbArea = GetBetArea(Point);
			if(cbArea > AREA_TMDM_MAX)
			{
				return ;
			}
			mapJetton[cbArea] += m_fCurrentBet;

			CRect rcRect(0,70,769,485);
			RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

			rcRect.SetRect(192,539,769,561);
			RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

			DOUBLE fyue = 0.0f;

			for(int i = 1;i < AREA_TMDM_MAX;i++)
			{
				fyue += mapJetton[i];
			}

			DOUBLE fAcyue = 0.0f;

			if(m_nMoshi == MoShi_Yuan)
			{
				fAcyue = theAccount.yue;
			}
			else if(m_nMoshi == MoShi_Jiao)
			{
				fAcyue = theAccount.yue*10;
			}
			else if(m_nMoshi == MoShi_Fen)
			{
				fAcyue = theAccount.yue*100;
			}


			fyue = (fAcyue - fyue);


			EnableJetton(fyue);
			if(fyue<1)
			{
				m_fCurrentBet = 0.0f;
				return;
			}

			DOUBLE fJetton[6] = {1,5,10,50,100,1000};

			int i = 0;
			for(i = 0;i<6;i++)
			{
				if(m_fCurrentBet == fJetton[i])
				{
					break;
				}
			}

			while(fyue < m_fCurrentBet)
			{
				if(i == 0)
				{
					m_fCurrentBet = 0.0f;
					return;
				}

				m_fCurrentBet = fJetton[--i];
			}
			return;
		}
		else if(m_gameKind == LiuHeCai_Tmds)
		{
			BYTE cbArea = GetBetArea(Point);
			if(cbArea > AREA_TMDS_MAX)
			{
				return ;
			}
			mapJetton[cbArea] += m_fCurrentBet;

			CRect rcRect(0,70,769,485);
			RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

			rcRect.SetRect(192,539,769,561);
			RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
		else if(m_gameKind == LiuHeCai_Tmdx)
		{
			BYTE cbArea = GetBetArea(Point);
			if(cbArea > AREA_TMDX_MAX)
			{
				return ;
			}
			mapJetton[cbArea] += m_fCurrentBet;

			CRect rcRect(0,70,769,485);
			RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

			rcRect.SetRect(192,539,769,561);
			RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
		else if(m_gameKind == LiuHeCai_Tmsx)
		{
			BYTE cbArea = GetBetArea(Point);
			if(cbArea > AREA_TMSX_MAX)
			{
				return ;
			}
			mapJetton[cbArea] += m_fCurrentBet;

			CRect rcRect(0,70,769,485);
			RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

			rcRect.SetRect(192,539,769,561);
			RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
		else if(m_gameKind == LiuHeCai_Tmbs)
		{
			BYTE cbArea = GetBetArea(Point);
			if(cbArea > AREA_TMBS_MAX)
			{
				return ;
			}
			mapJetton[cbArea] += m_fCurrentBet;

			CRect rcRect(0,70,769,485);
			RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

			rcRect.SetRect(192,539,769,561);
			RedrawWindow(rcRect,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}

	}
	return;
}
// 绘画数字
void CLiuHeCai::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%.2lf"),lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}
// 绘画数字
void CLiuHeCai::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, int lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%d"),lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// 绘画数字
void CLiuHeCai::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNumT[128] = {0};
	_sntprintf(szOutNumT,CountArray(szOutNumT),TEXT("%s"),szOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNumT, nXPos, nYPos, uFormat);
}


// 绘画数字
void CLiuHeCai::DrawNumber(CDC * pDC , CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_LEFT*/)
{
	// 加载资源
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * nNumberWidth;
	}

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}

VOID CLiuHeCai::DrawJetton(CDC *pDC, int nWidth, int nHeight)
{
	DOUBLE fZongTouzhu = 0.0f;
	if(m_gameKind == LiuHeCai_Tmdm)
	{
		for(BYTE i = 1;i < AREA_TMDM_MAX;i++)
		{
			CRect rcRect(m_ArrayTmdmBetArea[i][0]);
			int nFrameWidth = rcRect.Width();
			int nFrameHeight = rcRect.Height();
			int nFrameX = rcRect.left+nFrameWidth*4/7+3;
			int nFrameY = rcRect.top+(nFrameHeight-8)/2;

			fZongTouzhu += mapJetton[i];
			DrawNumber(pDC,&m_ImgNum,TEXT(",0123456789."),mapJetton[i],nFrameX,nFrameY, DT_LEFT); //jetton
		}
	}		
	else if(m_gameKind == LiuHeCai_Tmds)
	{
		for(BYTE i = 1;i < AREA_TMDS_MAX;i++)
		{
			CRect rcRect(m_ArrayTmdsBetArea[i][0]);
			int nFrameWidth = rcRect.Width();
			int nFrameHeight = rcRect.Height();
			int nFrameX = rcRect.left+210;
			int nFrameY = rcRect.top+(nFrameHeight-8)/2+5;
			if(i == 1)
			{
				nFrameX -= 30;
			}

			fZongTouzhu += mapJetton[i];
			DrawNumber(pDC,&m_ImgNum,TEXT(",0123456789."),mapJetton[i],nFrameX,nFrameY+163, DT_LEFT); //jetton
		}
	}
	else if(m_gameKind == LiuHeCai_Tmdx)
	{
		for(BYTE i = 1;i < AREA_TMDS_MAX;i++)
		{
			CRect rcRect(m_ArrayTmdxBetArea[i][0]);
			int nFrameWidth = rcRect.Width();
			int nFrameHeight = rcRect.Height();
			int nFrameX = rcRect.left+210;
			int nFrameY = rcRect.top+(nFrameHeight-8)/2+2;
			if(i == 1)
			{
				nFrameX -= 30;
			}

			fZongTouzhu += mapJetton[i];
			DrawNumber(pDC,&m_ImgNum,TEXT(",0123456789."),mapJetton[i],nFrameX,nFrameY+163, DT_LEFT); //jetton
		}
	}		
	else if(m_gameKind == LiuHeCai_Tmsx)
	{
		for(BYTE i = 1;i < AREA_TMSX_MAX;i++)
		{
			CRect rcRect(m_ArrayTmsxBetArea[i][0]);
			int nFrameWidth = rcRect.Width();
			int nFrameHeight = rcRect.Height();
			int nFrameX = rcRect.left+nFrameWidth*5/6+3;
			int nFrameY = rcRect.top+(nFrameHeight-8)/2;

			fZongTouzhu += mapJetton[i];
			DrawNumber(pDC,&m_ImgNum,TEXT(",0123456789."),mapJetton[i],nFrameX,nFrameY, DT_LEFT); //jetton
		}
	}
	else if(m_gameKind == LiuHeCai_Tmbs)
	{
		for(BYTE i = 1;i < AREA_TMBS_MAX;i++)
		{
			CRect rcRect(m_ArrayTmbsBetArea[i][0]);
			int nFrameWidth = rcRect.Width();
			int nFrameHeight = rcRect.Height();
			int nFrameX = rcRect.left+nFrameWidth*5/6+3;
			int nFrameY = rcRect.top+(nFrameHeight-8)/2;

			fZongTouzhu += mapJetton[i];
			DrawNumber(pDC,&m_ImgNum,TEXT(",0123456789."),mapJetton[i],nFrameX,nFrameY, DT_LEFT); //jetton
		}
	}
	DrawNumber(pDC,&m_ImgNum,TEXT(",0123456789."),fZongTouzhu,192,nHeight-30, DT_LEFT); //jetton

}
int CLiuHeCai::GetBoSe(int nNum)
{
	int nIndex = 0;
	int nHong[17] = {1,2,7,8,12,13,18,19,23,24,29,30,34,35,40,45,46};
	int nLan[16] = {3,4,9,10,14,15,20,25,26,31,36,37,41,42,47,48};
	int nLv[16] = {5,6,11,16,17,21,22,27,28,32,33,38,39,43,44,49};

	for(int i = 0;i < 17;i++)
	{
		if(nNum == nHong[i])
		{
			nIndex = 0;
			return nIndex;
		}
	}

	for(int i = 0;i < 16;i++)
	{
		if(nNum == nLan[i])
		{
			nIndex = 1;
			return nIndex;
		}
	}

	for(int i = 0;i < 16;i++)
	{
		if(nNum == nLv[i])
		{
			nIndex = 2;
			return nIndex;
		}
	}
	return 0;
}
VOID CLiuHeCai::DrawLuckNum(CDC *pDC, int nWidth, int nHeight)
{
// 	if(m_kjXinxiCont == 0)
// 		return;
	CTime t;
	theApp.GetTime(t);
	CString strKaijiang,strWebData;
	strWebData = GetWebData();

	int nYear = t.GetYear();

	int nSpace = 10;
	DrawNumber(pDC,&m_Num2,TEXT("0123456789"),nYear,nian_left+nSpace,nian_top,DT_LEFT);

	CString strTema,strQihao;
	strQihao.Format(L"%s",strWebData.Mid(1,3));
	strTema.Format(L"%s",m_kjXinxi[0].haoma);
	int nTema = _ttoi(strTema.Right(2));
	//int nQihao = _ttoi(strQihao.Right(3));

	DrawNumber(pDC,&m_Num2,TEXT("0123456789"),strQihao,nian_left+78,nian_top,DT_LEFT);

	int nBallWidth = m_ImageLuckBall.GetWidth()/3;
	int nBallHeight = m_ImageLuckBall.GetHeight();
	m_ImageLuckBall.DrawImage(pDC,rcRedraw.Width()-38,25,nBallWidth,nBallHeight,nBallWidth*GetBoSe(nTema),0,nBallWidth,nBallHeight);

	DrawNumber(pDC,&m_ImageLuckNum,TEXT("0123456789+-,"),nTema,rcRedraw.Width()-30,35,DT_LEFT);

	strKaijiang.Format( _T("%d年%s 21:30:00"),nYear,strWebData.Right(6));
	pDC->DrawText(strKaijiang, &rc_time_kj, DT_LEFT|DT_SINGLELINE);

	CString timestr = strKaijiang.Left(12);//"2000年04月05日";
	int year,month,day;
	BYTE tt[5];
	//get year
	memset(tt, 0, sizeof(tt));
	tt[0] = timestr[0];
	tt[1] = timestr[1];
	tt[2] = timestr[2];
	tt[3] = timestr[3];
	year= atoi((char *)tt);

	//get month
	memset(tt, 0, sizeof(tt));
	tt[0] = timestr[6];
	tt[1] = timestr[7];
	month = atoi((char *)tt);

	//get day
	memset(tt, 0, sizeof(tt));
	tt[0] = timestr[8];
	tt[1] = timestr[9];
	day = atoi((char *)tt);

	CTime time(year,month,day,21,30,0);

	CTimeSpan timespan = time-t;

	
	LONG lSeconds = timespan.GetTotalSeconds();
	int nDay = lSeconds/(24*3600);
	lSeconds -= (nDay*24*3600);
	int nHour = lSeconds/3600;
	lSeconds -= (nHour*3600);
	int nMinute = lSeconds/60;
	lSeconds -= (nMinute*60);
	CString strRest;
	strRest.Format(L"%d天%02d小时%02d分%02d秒",nDay,nHour,nMinute,lSeconds);
	pDC->DrawText(strRest, &rc_time_rest, DT_LEFT|DT_SINGLELINE);

	return;
}

//服务器端返回前N个开奖号码
bool CLiuHeCai::GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nIndex)
{
	_tcscpy_s(m_kjXinxi[nIndex].qihao, KJ_QIHAO_LENGTH, pResult->wPeriod);
	_tcscpy_s(m_kjXinxi[nIndex].haoma, KJ_HAOMA_LENGTH, pResult->szLotNum);
	_tcscpy_s(m_kjXinxi[nIndex].shijian, KJ_SHIJIAN_LENGTH, pResult->szShijian);

	if(nIndex+1 < TOP_KAIJIANG_NUMBERS)
		return true;

	m_kjXinxiCont = TOP_KAIJIANG_NUMBERS;
	if(!IsWindowVisible())
		return false;

	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
	{
		//第一次进入
		if(m_lastExpect[0] == 0)
			_tcscpy_s(m_lastExpect, KJ_HAOMA_LENGTH, m_kjXinxi[0].qihao);
		m_bKaiJiangzhong=false;											//开奖结束

		KillTimer(timer_id_kaijiangzhong);
	}
	if(m_bKaiJiangzhong)
	{
		CRect rcCnt;
		rcCnt.left = rcRedraw.left+200;
		rcCnt.right = rcRedraw.right;
		rcCnt.top = rcRedraw.top;
		rcCnt.bottom = rcRedraw.bottom;
		RedrawWindow(&rcCnt,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}
	else
	{
		RedrawWindow(&rcRedraw,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}
//读取事件
bool CLiuHeCai::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_SET_USER_BONUS_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_SetUserBonusRet));
				if(wDataSize < sizeof(CMD_GR_SetUserBonusRet)) return false;

				CMD_GR_SetUserBonusRet* pSetUserBonus = (CMD_GR_SetUserBonusRet*)pData;
				int nResult = pSetUserBonus->n_t_result;

				if(nResult==0)
				{
					SetLockText(m_fBonus, m_fPercent);
					MyMessageBox(L"返点数据修改成功！", L"温馨提示");
					SendToServer(6);

				}
				else
					MyMessageBox(L"返点数据修改失败！", L"提示");

				return true;
			}
		case SUB_GP_TOUZHU_CQSSC_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_TouzhuRes));
				if(wDataSize < sizeof(CMD_GR_TouzhuRes)) return false;

				CMD_GR_TouzhuRes* pTouzhuRes = (CMD_GR_TouzhuRes*)pData;

				LONG lResult = pTouzhuRes->lResult;

				m_DlgStatus.HideStatusWindow();
				if (lResult == 0)
				{
					if((pTouzhuRes->nSign != m_nTzhSign1))
					{
						m_nTzhSign1 = pTouzhuRes->nSign;
						ResetJetton();
						MyMessageBox(_T("成功投注！"), _T("【祝君好运 投注成功】"));
						SendToServer(7);
					}
				}
				else if(lResult == 8)
				{
					MyMessageBox(_T("余额不足，请充值后再投注！"));
				}
				else
				{
					if(	m_bShowFail == true)
					{
						m_bShowFail = false;

						MyMessageBox(_T("有订单未成功提交！"));
					}
				}


				return true;
			}
		case SUB_GP_GET_USER_FANDIAN_RET:
			{
				ASSERT((wDataSize % sizeof(CMD_GR_GetUserFandianRet))==0);
				if((wDataSize %sizeof(CMD_GR_GetUserFandianRet)) !=0)return false;

				KillTimer(TimerFandian);

				mapBonus.clear();
				mapDanweiFandian.clear();
				mapPercent.clear();
				mapFandian.clear();
				WORD wCount = wDataSize / sizeof(CMD_GR_GetUserFandianRet);
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetUserFandianRet* pGetUserFandian = ((CMD_GR_GetUserFandianRet*)pData+i);

					int nKindid = pGetUserFandian->n_t_kindid;
					nPercent = pGetUserFandian->f_t_bonusPercent;
					fDanweiFandian = pGetUserFandian->f_t_fandian;
					nBonus = pGetUserFandian->f_t_bonus;
					mapPercent.insert(make_pair(nKindid,nPercent));
					mapDanweiFandian.insert(make_pair(nKindid,fDanweiFandian));
					mapBonus.insert(make_pair(nKindid,nBonus));
					CString strText;
					strText.Format(L"%.2lf/%.2lf", nBonus, nPercent);

					strText+=_T("%");
					mapFandian.insert(make_pair(nKindid,strText));

				}
				m_btnFandian.SetWindowText(mapFandian[m_gameKind]);
				m_btnFandian.EnableWindow(true);
				return true;
			}
		default:
			break;
		}
		return true;
	}
	return true;
}
void CLiuHeCai::SetLockText(float fBonus, float fBonusPercent)
{
	const float EPSINON = 0.00001;
	if((fBonus >= - EPSINON) && (fBonus <= EPSINON))
	{
	}
	else
	{
		CString strText;
		strText.Format(L"%.2lf/%.2lf", fBonus, fBonusPercent);
		m_btnFandian.SetWindowText(strText + _T("%"));
	}
}

void CLiuHeCai::InitWanFaMoShi()
{
	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Tmdm] = _T("特码单买");		

	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Tmds] =  _T("特码单双");

	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Tmdx] = _T("特码大小");

	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Tmsx] = _T("特码生肖");

	theGameKind[(CZ_LIUHECAI << 16) | (unsigned short)LiuHeCai_Tmbs] = _T("特码波色");

}

void CLiuHeCai::SendToServer(int nSendType)
{
// 	CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
// 
// 	if(!pPlatformFrame->m_bCanSend)
// 	{
// 		if(nSendType == 6)
// 		{
// 			KillTimer(TimerFandian);
// 			SetTimer(TimerFandian, 1000, NULL);
// 
// 		}
// 		return;
// 	}

	if(nSendType == 3)
	{
		m_btnFandian.SetWindowText(mapFandian[m_gameKind]);
		ResetJetton();
		return;
	}


	if(nSendType == 6)
	{
		m_btnFandian.EnableWindow(false);
		CMD_GP_GetUserFandian GetUserFandian;
		ZeroMemory(&GetUserFandian,sizeof(GetUserFandian));

		GetUserFandian.n_t_userid = theAccount.user_id;
		GetUserFandian.n_t_type_id = CZ_LIUHECAI;
		GetUserFandian.n_t_kind_id = m_gameKind;

		//发送数据
		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_USER_FANDIAN,&GetUserFandian,sizeof(GetUserFandian));

		return;
	}

	if(nSendType == 7)
	{
// 		CMD_GP_GetLastYue GetLastYue;
// 		ZeroMemory(&GetLastYue,sizeof(GetLastYue));
// 
// 		GetLastYue.dwUserID = theAccount.user_id;
// 
// 		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
// 		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_LAST_YUE,&GetLastYue,sizeof(GetLastYue));
		return;
	}

}