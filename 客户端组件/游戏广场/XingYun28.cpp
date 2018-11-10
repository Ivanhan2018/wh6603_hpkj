// BeiJingKuai8.cpp : 实现文件
//

#include "stdafx.h"
#include "GamePlaza.h"
#include "XingYun28.h"
#include "DlgFandian.h"
#include "MessageDlg.h"
#include "DlgAddtional.h"
#include "PlatformFrame.h"
#include "GDIpng/MemDC.h"
#include <map>

using		namespace		std;


extern map <int,CString> mapFandian;
extern map <int,float> mapPercent;
extern map <int,float> mapDanweiFandian;
extern map <int,float> mapBonus;


extern vector<wstring>	vecData1, vecData2, vecData3;


#define      MAXN			100
extern vector<wstring> vecAllNums;
CString   CXingYun28::m_strHaoma = _T("");


extern int g_arr[MAXN];
extern int g_nPos[512000][5];
extern int g_Counts;


//大开奖号
static const int kj_big_haoma_x = 220;
static const int kj_big_haoma_y = 28;

static const int czh_rc_left = 207;
static const int czh_rc_top = 4;
static const int czh_rc_width = 365;
static const int czh_rc_height = 30;
static CRect rcCzh(czh_rc_left, czh_rc_top, czh_rc_left + czh_rc_width, czh_rc_top + czh_rc_height);

static const int qihao_rc_left = 325;
static const int qihao_rc_top = 4;
static const int qihao_rc_width = 125;
static const int qihao_rc_height = 30;
static CRect rcQiHao(qihao_rc_left, qihao_rc_top, qihao_rc_left + qihao_rc_width, qihao_rc_top + qihao_rc_height);

static const int yugao_rc_left = 3;
static const int yugao_rc_top = 4;
static const int yugao_rc_width = 196;
static const int yugao_rc_height = 30;
static CRect rcYuGao(yugao_rc_left, yugao_rc_top, yugao_rc_left + yugao_rc_width, yugao_rc_top + yugao_rc_height);


//小开奖号
static const int kj_small_haoma_x = 61;
static const int kj_small_haoma_y = 46;
static const int kj_small_haoma_col_span = 5;
static const int kj_small_haoma_row_span = 6;

//总注数
static const int zongzhushu_x = 71;
static const int zongzhushu_y = 411;
static const int zongzhushu_width = 150;
static const int zongzhushu_height = 25;
static CRect rcZongZhuShu(zongzhushu_x, zongzhushu_y, zongzhushu_x+zongzhushu_width, zongzhushu_y+zongzhushu_height);

//总金额
static const int zongjine_x = 295;
static const int zongjine_y = 428;
static const int zongjine_width = 150;
static const int zongjine_height = 25;
static CRect rcZongJinE(zongjine_x, zongjine_y, zongjine_x+zongjine_width, zongjine_y+zongjine_height);

//总注数
static const int allzongzhushu_x = 0;
static const int allzongzhushu_y = 444;
static const int allzongzhushu_width = 150;
static const int allzongzhushu_height = 25;
static CRect rcAllZongzhushu(allzongzhushu_x, allzongzhushu_y, allzongzhushu_x+allzongzhushu_width, allzongzhushu_y+allzongzhushu_height);




//单式选号编辑框
static const int edit_haomao_danshi_x = 20;
static const int edit_haomao_danshi_y = 210;
static const int edit_haomao_danshi_width = 752;
static const int edit_haomao_danshi_height = 195;


/////////////////////////////////////////////////////////////////控制位置
//鼠标输入
static const int mouseinput_x = 597;
static const int mouseinput_y = 175;
//键盘输入
static const int keyboardinput_x = 670;
static const int keyboardinput_y = 175;


//玩法分类数据
static const int wanfa_x = 9;
static const int wanfa_y = 109;
static const int wanfa_col_width = 48;
static const int wanfa_row_height = 32;

//玩法子分类数据
static const int wanfa_sel_x = 20;
static const int wanfa_sel_y = 145;
static const int wanfa_sel_width = 71;
static const int wanfa_sel_height = 22;


//号码数据
static const int haoma_btn_x = 100;
static const int haoma_btn_y = 114;
static const int haoma_btn_col_wdith = 40;
static const int haoma_btn_row_height = 35;



//添加号码按钮
static const int btn_chq_add_x = 571;
static const int btn_chq_add_y = 415;
//
static const int btn_lock_x = 450;
static const int btn_lock_y = 417;



//选号显示区域数据
//删除选中按钮
static const int btn_del_sel_x = 470;
static const int btn_del_sel_y = 543;
//清空列表按钮
static const int btn_cls_list_x = 645;
static const int btn_cls_list_y = 415;
//号码列表
static const int list_haomao_x = 43;
static const int list_haomao_y = 469;
static const int list_haomao_width = 592;
static const int list_haomao_height = 72;
//倍投编辑框
static const int edit_beitou_x = 16;
static const int edit_beitou_y = 481;
static const int edit_beitou_width = 78;
static const int edit_beitou_height = 34;
//确认投注按钮
static const int btn_touzhu_x = 638;
static const int btn_touzhu_y = 490;
//追号按钮
static const int btn_zhuihao_x = 719;
static const int btn_zhuihao_y = 415;
//更多记录
static const int btn_more_x = 712;
static const int btn_more_y = 92;

static const int btn_yuan_x = 10;
static const int btn_yuan_y = 415;
static const int btn_jiao_x = 45;
static const int btn_jiao_y = 415;
static const int btn_fen_x = 80;
static const int btn_fen_y = 415;
static const int btn_li_x = 115;
static const int btn_li_y = 415;
////////////////////////////////////////////////////////////////////////信息位置
static const int timer_id_get_luck_num = 1;		//获取开奖号码
static const int timer_id_kj_yugao = 2;			//开奖预告

static const int timer_id_kaijiangzhong=3;		//检查是否开奖中
//计时
static const int TimerJiShi = 4;				
static const int TimerStatus = 6;

//这个是用来本地刷新开奖数据的
static const int TimerZhunBeiData = 5;

static const int TimerFandian = 6;

static const int redraw_rc_left = 0;
static const int redraw_rc_top = 0;
static const int redraw_rc_width = 756;
static const int redraw_rc_height = 103;
static CRect rcRedraw(redraw_rc_left, redraw_rc_top, redraw_rc_left + redraw_rc_width, redraw_rc_top + redraw_rc_height);


// CXingYun28 对话框

IMPLEMENT_DYNAMIC(CXingYun28, CDialog)

CXingYun28::CXingYun28(CWnd* pParent /*=NULL*/)
	: CDialog(CXingYun28::IDD, pParent)
	, m_bmpBk(NULL)
	, m_numImageList(NULL)
	, m_numImageList1(NULL)
	, m_numImageList2(NULL)
	, m_kjNumBig(NULL)
	, m_kjNumSmall(NULL)
	, m_kjNumSmall1(NULL)
	, m_bmpDxdsh(NULL)
	, m_bmpNumTip(NULL)
	, m_bmpNumTip1(NULL)
	, m_bigNumWidth(0)
	, m_bigNumHeight(0)
	, m_smallNumWidth(0)
	, m_smallNumHeight(0)
	, m_nZongZhuShu(0)
	//, m_zongJinE(0.0)
	, m_beishu(1)
	, m_kjXinxiCont(0)
	/*, m_t1_start(0)
	, m_t1_end(2)
	, m_t2_start(10)
	, m_t2_end(22)	
	, m_t3_start(22)
	, m_t3_end(24)*/
	, m_pLuckMeDlg(NULL)
{

	m_gameKind = enBJK8_1;
	m_SocketID=0;
	m_nTzhSign = 1;
	m_nTzhSign1 = 0;
	m_nFrameIndex=0;
	m_nFrameCount=0;
	for (int i=0; i<Kj_XinXi_Count; i++) 
	{
		memset(&m_kjXinxi[i], 0, sizeof(KjXinXi));
	}

	m_RenxuanKeyboardInput=false;
	_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, _T(""));

	//任务组件
	m_bGetUserFandian=false;
	m_Font.CreateFont(14,0,0,0,700,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("微软雅黑"));
	m_wGameHoverItem=INVALID_WORD;
	m_cbMenuType=0xff;
	InitWanFaMoShi();
	m_bQueryTime=false;
	m_bGetCPUserInfo=false;
	m_bQueryLuckyNum=false;
	m_bTouzhu=false;
	m_bGetTime=false;
	m_bHovering=false;
	nPercent=0;
	m_bSetBonus=false;
	m_bGetLastYue=false;
	m_bGetMapBonus=false;
	m_fBonus = 0.0;
	m_fPercent = 0.0;
	fDanweiFandian=0.0f;
	nBonus = 0.0f;
	m_nTouzhuCount=0;
	m_nZhuihaoCount = 0;
	m_bZhuihao=false;
	//设置组件
	m_DlgStatus.SetStatusViewSink(this);

}


//取消连接
VOID CXingYun28::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}


CXingYun28::~CXingYun28()
{
	if (m_bmpBk != NULL) 
	{
		m_bmpBk=NULL;
		delete m_bmpBk;
	}

	if (m_numImageList != NULL) 
	{
		m_numImageList=NULL;
		delete m_numImageList;
	}
	if (m_numImageList1 != NULL) 
	{
		m_numImageList1=NULL;
		delete m_numImageList1;
	}
	if (m_numImageList2 != NULL) 
	{
		m_numImageList2=NULL;
		delete m_numImageList2;
	}
	if (m_numImageList3 != NULL)
	{
		m_numImageList3=NULL;
		delete m_numImageList3;
	}

	if (m_kjNumBig != NULL) 
	{
		m_kjNumBig=NULL;
		delete m_kjNumBig;
	}
	if (m_kjNumBig2 != NULL) 
	{
		m_kjNumBig2=NULL;
		delete m_kjNumBig2;
	}
	if (m_kjAdd != NULL) 
	{
		m_kjAdd=NULL;
		delete m_kjAdd;
	}
	if (m_kjAdd1 != NULL) 
	{
		m_kjAdd1=NULL;
		delete m_kjAdd1;
	}

	if (m_kjNumSmall != NULL) 
	{
		m_kjNumSmall=NULL;
		delete m_kjNumSmall;
	}
	if (m_kjNumSmall1 != NULL) 
	{
		m_kjNumSmall1=NULL;
		delete m_kjNumSmall1;
	}

	if (m_imgKaijiang != NULL) 
	{
		m_imgKaijiang=NULL;
		delete m_imgKaijiang;
	}

	if (m_bmpDxdsh != NULL) 
	{
		m_bmpDxdsh=NULL;
		delete m_bmpDxdsh;
	}

	if (m_bmpNumTip != NULL) 
	{
		m_bmpNumTip=NULL;
		delete m_bmpNumTip;
	}
	if (m_bmpNumTip1 != NULL) 
	{
		m_bmpNumTip1=NULL;
		delete m_bmpNumTip1;
	}
}

void CXingYun28::ConnectMainDlg(CPlazaViewItem* luckMeDlg)
{
	m_pLuckMeDlg = luckMeDlg;
}

void CXingYun28::FlushZongjine()
{
	InvalidateRect(&rcZongJinE);
}

void CXingYun28::OnBnClickedBtnYuan()
{
	m_btnYuan.SetPushed(true);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Yuan ;

	InvalidateRect(rcZongJinE);

	return;
}
void CXingYun28::OnBnClickedBtnJiao()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(true);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Jiao ;
	InvalidateRect(rcZongJinE);

	return;
}
void CXingYun28::OnBnClickedBtnFen()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(true);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Fen ;
	InvalidateRect(rcZongJinE);
	return;

}
void CXingYun28::OnBnClickedBtnLi()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(true);
	theAccount.Scoretype=MoShi_Li ;
	InvalidateRect(rcZongJinE);

	return;
}

void CXingYun28::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_BTN_BEITOU_JIA,m_btnBeiTouJia);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIAN,m_btnBeiTouJian);

	DDX_Control(pDX, IDC_BTN_HEZHI_0, m_btnHeZhi_0);
	DDX_Control(pDX, IDC_BTN_HEZHI_1, m_btnHeZhi_1);
	DDX_Control(pDX, IDC_BTN_HEZHI_2, m_btnHeZhi_2);
	DDX_Control(pDX, IDC_BTN_HEZHI_3, m_btnHeZhi_3);
	DDX_Control(pDX, IDC_BTN_HEZHI_4, m_btnHeZhi_4);
	DDX_Control(pDX, IDC_BTN_HEZHI_5, m_btnHeZhi_5);
	DDX_Control(pDX, IDC_BTN_HEZHI_6, m_btnHeZhi_6);
	DDX_Control(pDX, IDC_BTN_HEZHI_7, m_btnHeZhi_7);
	DDX_Control(pDX, IDC_BTN_HEZHI_8, m_btnHeZhi_8);
	DDX_Control(pDX, IDC_BTN_HEZHI_9, m_btnHeZhi_9);
	DDX_Control(pDX, IDC_BTN_HEZHI_10, m_btnHeZhi_10);
	DDX_Control(pDX, IDC_BTN_HEZHI_11, m_btnHeZhi_11);
	DDX_Control(pDX, IDC_BTN_HEZHI_12, m_btnHeZhi_12);
	DDX_Control(pDX, IDC_BTN_HEZHI_13, m_btnHeZhi_13);
	DDX_Control(pDX, IDC_BTN_HEZHI_14, m_btnHeZhi_14);
	DDX_Control(pDX, IDC_BTN_HEZHI_15, m_btnHeZhi_15);
	DDX_Control(pDX, IDC_BTN_HEZHI_16, m_btnHeZhi_16);
	DDX_Control(pDX, IDC_BTN_HEZHI_17, m_btnHeZhi_17);
	DDX_Control(pDX, IDC_BTN_HEZHI_18, m_btnHeZhi_18);
	DDX_Control(pDX, IDC_BTN_HEZHI_19, m_btnHeZhi_19);
	DDX_Control(pDX, IDC_BTN_HEZHI_20, m_btnHeZhi_20);
	DDX_Control(pDX, IDC_BTN_HEZHI_21, m_btnHeZhi_21);
	DDX_Control(pDX, IDC_BTN_HEZHI_22, m_btnHeZhi_22);
	DDX_Control(pDX, IDC_BTN_HEZHI_23, m_btnHeZhi_23);
	DDX_Control(pDX, IDC_BTN_HEZHI_24, m_btnHeZhi_24);
	DDX_Control(pDX, IDC_BTN_HEZHI_25, m_btnHeZhi_25);
	DDX_Control(pDX, IDC_BTN_HEZHI_26, m_btnHeZhi_26);
	DDX_Control(pDX, IDC_BTN_HEZHI_27, m_btnHeZhi_27);



	DDX_Control(pDX, IDC_BTN_K8RX, m_btnRenXuan);		//快8任选
	DDX_Control(pDX, IDC_BTN_TEMA, m_btnTeMa);			
	DDX_Control(pDX, IDC_BTN_DXDS, m_btnDaXiaoDanShuang);
	DDX_Control(pDX, IDC_BTN_FUNNY, m_btnFunny);
	DDX_Control(pDX, IDC_BTN_JIZHI, m_btnJiZhi);


	DDX_Control(pDX, IDC_RIO_DUIZI,	m_rioDuiZi);
	DDX_Control(pDX, IDC_RIO_SHUNZI,	m_rioShunZi);
	DDX_Control(pDX, IDC_RIO_BAOZI,	m_rioBaoZi);



	//大小单双选择
	DDX_Control(pDX, IDC_BTN_DA_GE, m_btnDa);
	DDX_Control(pDX, IDC_BTN_XIAO_GE, m_btnXiao);
	DDX_Control(pDX, IDC_BTN_DAN_GE, m_btnDan);
	DDX_Control(pDX, IDC_BTN_SHUANG_GE, m_btnShuang);
	DDX_Control(pDX, IDC_BTN_XIAODAN, m_btnXiaoDan);
	DDX_Control(pDX, IDC_BTN_XIAOSHUANG, m_btnXiaoShuang);
	DDX_Control(pDX, IDC_BTN_DADAN, m_btnDaDan);
	DDX_Control(pDX, IDC_BTN_DASHUANG, m_btnDaShuang);

	DDX_Control(pDX, IDC_BTN_MAX,	m_btnMax);
	DDX_Control(pDX, IDC_BTN_MIN,	m_btnMin);



	DDX_Control(pDX, IDC_LIST_CHQSSC, m_listChqSSC);
	DDX_Control(pDX, IDC_GRID_LIST, m_GridList);
	DDX_Control(pDX, IDC_BTN_CHQ_ADD, m_btnChqAdd);
	DDX_Control(pDX, IDC_BTN_DEL_SEL, m_btnDelSel);
	DDX_Control(pDX, IDC_BTN_CLS_LIST, m_btnClsList);
	DDX_Control(pDX, IDC_BTN_TOUZHU, m_btnTouZhu);
	DDX_Control(pDX, IDC_BTN_ZHUIHAO, m_btnZhuiHao);
	DDX_Control(pDX, IDC_EDIT_BEISHU, m_editBeiTou);






	DDX_Text(pDX, IDC_EDIT_BEISHU, m_beishu);
	DDX_Control(pDX, IDC_EDIT_DANSHI_HAOMA, m_editDanShiHaoMa);
	DDX_Control(pDX, IDC_RICHEDIT_DANSHI, m_richDanshiHaoma);
	DDX_Control(pDX, IDC_BTN_LOCK, m_btnLock);

	DDX_Control(pDX, IDC_RIO_MOUSE_INPUT,m_rioMouseInput);
	DDX_Control(pDX, IDC_RIO_KEYBOARD_INPUT,m_rioKeyboardInput);



	
	DDX_Control(pDX, IDC_BTN_YUAN,m_btnYuan);
	DDX_Control(pDX, IDC_BTN_JIAO,m_btnJiao);
	DDX_Control(pDX, IDC_BTN_FEN,m_btnFen);
	DDX_Control(pDX, IDC_BTN_LI,m_btnLi);
}


BEGIN_MESSAGE_MAP(CXingYun28, CDialog)
	ON_BN_CLICKED(IDC_BTN_YUAN, &CXingYun28::OnBnClickedBtnYuan)
	ON_BN_CLICKED(IDC_BTN_JIAO, &CXingYun28::OnBnClickedBtnJiao)
	ON_BN_CLICKED( IDC_BTN_FEN, &CXingYun28::OnBnClickedBtnFen)
	ON_BN_CLICKED(IDC_BTN_LI, &CXingYun28::OnBnClickedBtnLi)
	ON_MESSAGE( WM_BN_CLICK, &CXingYun28::onBnCLick)

	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()

	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)

	ON_BN_CLICKED(IDC_RIO_MOUSE_INPUT, &CXingYun28::OnBnClickedRioMouseInput)		//鼠标输入
	ON_BN_CLICKED(IDC_RIO_KEYBOARD_INPUT, &CXingYun28::OnBnClickedRioKeyboardInput)	//键盘输入


	//玩法选择
	ON_BN_CLICKED(IDC_BTN_K8RX, &CXingYun28::OnBnClinckedBtnRenXuan)//快8任选
	ON_BN_CLICKED(IDC_BTN_TEMA, &CXingYun28::OnBnClinckedBtnTeMa)		//特码
	ON_BN_CLICKED(IDC_BTN_DXDS, &CXingYun28::OnBnClinckedBtnDaXiaoDanShuang)//大小单双
	ON_BN_CLICKED(IDC_BTN_FUNNY, &CXingYun28::OnBnClinckedBtnFunny)	//趣味玩法
	ON_BN_CLICKED(IDC_BTN_JIZHI, &CXingYun28::OnBnClinckedBtnJiZhi)//极值


	ON_BN_CLICKED(IDC_RIO_DUIZI, &CXingYun28::OnRioDuiZi)
	ON_BN_CLICKED(IDC_RIO_SHUNZI, &CXingYun28::OnRioShunZi)
	ON_BN_CLICKED(IDC_RIO_BAOZI, &CXingYun28::OnRioBaoZi)

	

	//控制按钮
	ON_BN_CLICKED(IDC_BTN_ZHUIHAO, &CXingYun28::OnBnClickedBtnZhuihao)//追号
	ON_BN_CLICKED(IDC_BTN_TOUZHU, &CXingYun28::OnBnClickedBtnTouzhu)//投注
	ON_BN_CLICKED(IDC_BTN_CHQ_ADD, &CXingYun28::OnBnClickedBtnChqAdd)//添加号码
	ON_BN_CLICKED(IDC_BTN_DEL_SEL, &CXingYun28::OnBnClickedBtnDelSel)//删除
	ON_BN_CLICKED(IDC_BTN_CLS_LIST, &CXingYun28::OnBnClickedBtnClsList)//清空
	ON_BN_CLICKED(IDC_BTN_LOCK, &CXingYun28::OnBnClickedBtnLock)//设置返点
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIA, &CXingYun28::OnBnClickedBeiTouJia)
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIAN, &CXingYun28::OnBnClickedBeiTouJian)

	//投注修改
	ON_EN_CHANGE(IDC_EDIT_BEISHU, &CXingYun28::OnEnChangeEditBeishu)//修改倍数
	ON_EN_CHANGE(IDC_EDIT_DANSHI_HAOMA, &CXingYun28::OnEnChangeEditDanshiInput)//修改单数


END_MESSAGE_MAP()





// CXingYun28 消息处理程序
void CXingYun28::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(!IsWindowVisible())
		return;

	// TODO: 在此处添加消息处理程序代码
	if(!::IsWindow(m_hWnd))
	{
		return;
	}
	CRect rect;
	GetClientRect(&rect);

	CDC cacheDC;
	cacheDC.CreateCompatibleDC(&dc);

	CBitmap cacheBmp;
	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);

	Graphics graphics(cacheDC.m_hDC);
	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	//绘制开奖号
	DrawLastKjHaoma(&cacheDC, graphics);
	DrawTopKjHaoma(&cacheDC, graphics);

	//绘制总注数和总金额
	DrawZongZhushuJinE(&cacheDC);

	cacheDC.SetBkMode(TRANSPARENT);
	cacheDC.SetTextColor(RGB(88, 78, 77));
	CFont* pOldFont = cacheDC.SelectObject(&m_Font);

	m_FandianView.DrawView(this,&cacheDC,rect.Width(),rect.Height());

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldFont);
	cacheDC.SelectObject(pOldCacheBmp);
	cacheDC.DeleteDC();
	cacheBmp.DeleteObject();

	return;
}

BOOL CXingYun28::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CXingYun28::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL) 
	{
		return;
	}

	AdjustWanFa();
	AdjustRadio();
	AdjustNumBtn();
	AdjustNumView();
	AdjustDanShi();
	AdjustAdd();
}

//校验输入
bool CXingYun28::CheckInput()
{
	if (_tcslen(m_lastExpect) == 0)
	{
		MyMessageBox(_T("正在获取数据…… "));
		return false;
	}

	CString str;
	m_editBeiTou.GetWindowText(str);
	if (str.IsEmpty())
	{
		MyMessageBox(_T("投注倍数不可为空"));
		return false;
	}

	int itm_cnt = m_listChqSSC.GetItemCount();
	if (m_strHaoma.IsEmpty() && vecAllNums.size()==0) 
	{
		MyMessageBox(_T("您并未选择号码"));
		return false;
	}

	//判断余额是否充足
	double fCellPrice=0.0;
	if(theAccount.Scoretype == MoShi_Yuan)
		fCellPrice = 2.0;
	else if(theAccount.Scoretype == MoShi_Jiao)
		fCellPrice = 0.2;
	else
		fCellPrice = 0.02;
	if(theAccount.yue < m_nZongZhuShu * m_beishu * fCellPrice)
	{
		MyMessageBox(_T("您余额不足，请充值后再试"));
		return false;
	}
	//判断是否临近开奖点
	//重新获取服务器时间
	//theApp.GetTimeDiff();
	long kjing = m_chqRule.GetKjShjDiff(); 
	if (kjing < 60)								// || kjing > 540
	{
		MyMessageBox(_T("此期封单,请稍后再试"));
		return false;
	}



	return true;
}

//显示
void CXingYun28::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;
		SendToServer(6);
		m_btnYuan.SetPushed(false);
		m_btnJiao.SetPushed(false);
		m_btnFen.SetPushed(false);
		m_btnLi.SetPushed(false);
		if(theAccount.Scoretype==MoShi_Yuan )
		{
			m_btnYuan.SetPushed(true);
		}
		else if(theAccount.Scoretype == MoShi_Jiao)
		{
			m_btnJiao.SetPushed(true);
		}
		else if(theAccount.Scoretype == MoShi_Fen)
		{
			m_btnFen.SetPushed(true);
		}
		else if(theAccount.Scoretype == MoShi_Li)
		{
			m_btnLi.SetPushed(true);
		}

		m_bKaiJiangzhong = false;
		SetTimer(timer_id_kj_yugao, 1000, NULL);
		SetTimer(timer_id_kaijiangzhong, 100, NULL);


		OnBnClinckedBtnTeMa();
	}

	//InvalidateRect(CRect(0,0,1000,1000),true);
}

 //定时器
void CXingYun28::OnTimer(UINT_PTR nIDEvent)
{
	if (timer_id_kj_yugao == nIDEvent)
	{
		CRect rcRect;
		rcRect.CopyRect(rcYuGao);
	//	rcRect.right += 80;
		rcRect.bottom += 50;
		InvalidateRect(&rcRect);
	}
	else if(TimerJiShi == nIDEvent)
	{
		DWORD newCount = ::GetTickCount();
		int i = 0;
		while (newCount - theBeginCount > 1000) {
			theTimeCS.Lock();
			theTime += CTimeSpan(0, 0, 0, 1);

			theTimeCS.Unlock();
			newCount -= 1000;
			i ++;
		}
		theBeginCount += i * 1000;

	}
	else if(TimerFandian == nIDEvent)
	{
		SendToServer(6);
	}
	else if(timer_id_kaijiangzhong == nIDEvent)
	{
		//处理开奖GIF动画
		int nTimeLeft = GetKjShjDiff();
		if(m_bKaiJiangzhong )
		{
			CDC* pDC = GetDC();
			if(pDC==NULL)
				return;
			//for(int n=0; n<m_nFrameCount; n++)
			{
				int nLeft=kj_big_haoma_x-18 /*+ n *( m_bigNumWidth+8)*/;
				CMemDC	mDC(pDC, CRect(nLeft, kj_big_haoma_y+9, nLeft+m_imgKaijiang->GetWidth(), kj_big_haoma_y+m_imgKaijiang->GetHeight()));
				Graphics gh(mDC.m_hDC);
				gh.DrawImage(m_imgKaijiang, nLeft, kj_big_haoma_y+9, m_imgKaijiang->GetWidth(), m_imgKaijiang->GetHeight());
			}
			GUID Guid = FrameDimensionTime;
			m_imgKaijiang->SelectActiveFrame(&Guid, m_nFrameIndex++);
			if(m_nFrameIndex == m_nFrameCount)
				m_nFrameIndex=0;
			ReleaseDC(pDC);
		}
		else
			KillTimer(timer_id_kaijiangzhong);
	}
	else if(nIDEvent == TimerStatus)
	{
		KillTimer(TimerStatus);
		m_DlgStatus.HideStatusWindow();
		CInformation Information;
		Information.ShowMessageBox(L"温馨提醒",L"服务器繁忙，投注失败",MB_OK,5);
	}


	CDialog::OnTimer(nIDEvent);
}

HBRUSH CXingYun28::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() == IDC_EDIT_DANSHI_HAOMA) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(70,0, 0));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(70,0, 0));
		}

		return hBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RICHEDIT_DANSHI) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(70,0, 0));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(70,0, 0));
		}

		return hBrush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//添加号码
void CXingYun28::OnBnClickedBtnChqAdd()
{
//	m_DlgStatus.ShowStatusWindow(TEXT("正在添加数据，请稍候..."));
	int zhushu = 1;
	CString strHaoMa;




	if (m_gameKind == enXY28_Funny)
	{

		strHaoMa = m_StrFunny;

		zhushu = 1;

	}
	else if (m_gameKind == enXY28_JiZhi)
	{
		CString strShi = GetJiZhiString();

		if (strShi.IsEmpty()) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("请选择"));
			return;
		}

		int nLength = strShi.GetLength();

		if(nLength>2)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("格式不正确，请重新选择！"));
			return;

		}
		for(int i = 0;i < nLength;i++)
		{
			strHaoMa += strShi.GetAt(i);
			if(i%2!=0 &&i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}


		zhushu = strShi.GetLength();
	}
	else if (m_gameKind == enXY28_DXDS)
	{
		CString strShi = GetDxdshString();

		if (strShi.IsEmpty()) 
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("请选择"));
			return;
		}
		
		bool bCheckDa = IsNumBtnCheck(m_btnDa);
		bool bCheckXiao = IsNumBtnCheck(m_btnXiao);
		bool bCheckDan = IsNumBtnCheck(m_btnDan);
		bool bCheckShuang = IsNumBtnCheck(m_btnShuang);

		bool bChecked[4] = {bCheckDa,bCheckXiao,bCheckDan,bCheckShuang};
		int nFirst = 0;
		int nSecond = 0;
		for (int i = 0;i < CountArray(bChecked);i++)
		{
			if(bChecked[i])
				nFirst++;
		}
		if(nFirst>1)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("大小单双只能选择一个！"));
			return;
		}
		bCheckDa = IsNumBtnCheck(m_btnDaDan);
		bCheckXiao = IsNumBtnCheck(m_btnXiaoDan);
		bCheckDan = IsNumBtnCheck(m_btnDaShuang);
		bCheckShuang = IsNumBtnCheck(m_btnXiaoShuang);

		bool bChecked1[4] = {bCheckDa,bCheckXiao,bCheckDan,bCheckShuang};
		for (int i = 0;i < CountArray(bChecked1);i++)
		{
			if(bChecked1[i])
				nSecond++;
		}
		if(nSecond>1)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(_T("大单、小单、大双、小双只能选择一个！"));
			return;
		}

		zhushu = 0;
		CString temp = TEXT(",");
		for(int i = 0;i < strShi.GetLength()-1;i++)
		{
			if (strShi.GetAt(i) == temp)
			{
				zhushu++;
			}
			strHaoMa += strShi.GetAt(i);
		}

		zhushu++;//最后个消除了， 所以+1
		//zhushu = strShi.GetLength();

	}
	else if(m_gameKind == enXY28_TeMa)//幸运28特码
	{
		if (m_RenxuanKeyboardInput)
		{
			if(!GetDanShiHaoma(strHaoMa,20,1))
			{
				m_DlgStatus.HideStatusWindow();
				CString strTips;
				strTips.Format(L"您的号码输入有误，请重新输入！");
				MyMessageBox(strTips);
				return;
			}

			int nCount = strHaoMa.GetLength()/3;
			nCount = (strHaoMa.GetLength()-nCount+1)/2;
			zhushu = (nCount-1)*nCount/2;
		}
		else
		{
			CString strNum;
			strNum = GetHezhiString();

			int nCount  = strNum.GetLength();
			if (nCount == 0)
			{
				MyMessageBox(L"请选择数字！");
				m_DlgStatus.HideStatusWindow();
				return;
			}
			if (nCount > 46)
			{
				MyMessageBox(L"选择注数过多！");
				m_DlgStatus.HideStatusWindow();
				return;
			}

			for(int i = 0;i < nCount/2;i++)
			{
				strHaoMa += strNum.GetAt(i*2);
				strHaoMa += strNum.GetAt(i*2+1);
				if(i*2 !=nCount-2)
				{
					strHaoMa+=_T(",");
				}
			}

			m_gameKind = enXY28_TeMa;

			zhushu = nCount/2;

		}

	}




	UpdateData();

	CStringArray strArr;


	if (!strHaoMa.IsEmpty()) 
	{
		if(strHaoMa.GetLength()>1024)
		{
			m_DlgStatus.ShowStatusWindow(TEXT("正在添加数据，请稍候..."));
		}

		if(m_RenxuanKeyboardInput)
		{
			if(strHaoMa.GetLength()>10240)
			{

				if((!m_strHaoma.IsEmpty())||(m_nZongZhuShu!=0))
				{
					CInformation Information;
					if(Information.ShowMessageBox(L"温馨提醒",L"本操作将清除已添加号码，请确认",MB_YESNO)==IDYES)
					{
						OnBnClickedBtnClsList();
					}
					else
					{
						m_DlgStatus.HideStatusWindow();
						return ;
					}

				}
				m_strHaoma = strHaoMa;


				memset(m_szMD5Haoma, 0, sizeof(m_szMD5Haoma));
				CString strTempHaoma = m_strHaoma.Left(3000);
				CMD5Encrypt::EncryptData(strTempHaoma,m_szMD5Haoma);

				int nIndex = 0;
				CString strHaomaTemp;
				int nZongzhushu = 0;


				do
				{

					if(!AfxExtractSubString(strHaomaTemp, strHaoMa, nIndex++, '|'))
						break;
					if(strHaomaTemp.IsEmpty())
						break;

					nZongzhushu ++;

				}
				while(nIndex<80000);
				int nCount = m_listChqSSC.GetItemCount();

				m_listChqSSC.InsertItem(nCount, L"号码添加成功");
				m_listChqSSC.SetItemText(nCount, 1, GetWanDesc());
				CString strZongzhu ;
				strZongzhu.Format(L"%d",nZongzhushu);
				m_listChqSSC.SetItemText(nCount, 2, strZongzhu);
				strZongzhu.Format(_T("%d"), m_beishu);
				m_listChqSSC.SetItemText(nCount, 3, strZongzhu);
				m_listChqSSC.createItemButton(nCount, 4, this->GetSafeHwnd());

				m_nZongZhuShu += nZongzhushu;
			}
			else
			{
				int nCount =m_listChqSSC.GetItemCount();
				int nIndex = 0;
				do
				{
					CString strHaomaTemp;

					if(!AfxExtractSubString(strHaomaTemp, strHaoMa, nIndex++, '|'))
						break;
					if(strHaomaTemp.IsEmpty())
						break;
					wstring strdata;
					strdata = strHaomaTemp.GetBuffer(0);
					vecAllNums.insert(vecAllNums.begin(),1,strdata);

					m_listChqSSC.InsertItem(nCount, strHaomaTemp);
					m_listChqSSC.SetItemText(nCount, 1, GetWanDesc());


					zhushu = 1;


					CString strTmp;
					strTmp.Format(_T("%d"), zhushu);
					m_listChqSSC.SetItemText(nCount, 2, strTmp);
					strTmp.Format(_T("%d"), m_beishu);
					m_listChqSSC.SetItemText(nCount, 3, strTmp);
					m_listChqSSC.createItemButton(nCount, 4, this->GetSafeHwnd());

					m_nZongZhuShu += zhushu;

				}
				while(nIndex < 80000);
				CString strMD5Haoma;
				for(int i = 0;i < vecAllNums.size();i++)
				{
					strMD5Haoma += vecAllNums[i].c_str();
					strMD5Haoma += L"|";
				}

				memset(m_szMD5Haoma, 0, sizeof(m_szMD5Haoma));
				CMD5Encrypt::EncryptData(strMD5Haoma,m_szMD5Haoma);

			}


		}
		else
		{

			m_strHaoma.Empty();

			wstring strdata;
			strdata = strHaoMa.GetBuffer(0);
			//vecAllNums.insert(vecAllNums.begin(),1,strdata);
			vecAllNums.insert(vecAllNums.begin(),1,strdata);
			int nCount = m_listChqSSC.GetItemCount();

			if(m_gameKind == enXY28_DXDS || m_gameKind == enXY28_JiZhi)
			{
				CString strAddHaoma;
				int nAddLength = strHaoMa.GetLength();
				int nAddIndex = 0;
				do 
				{
					CString strAdd;
					if(!AfxExtractSubString(strAdd, strHaoMa, nAddIndex++, ','))
						break;
					if(nAddIndex!=1)
					{
						strAddHaoma += _T(",");
					}
					int nNum = _ttoi(strAdd);
					CString strDesc = GetDxdshStringDesc(nNum);
					strAddHaoma += strDesc;

				} while (nAddIndex<1000);

				m_listChqSSC.InsertItem(nCount, strAddHaoma);
			}
			else if (m_gameKind == enXY28_Funny)
			{
				int nNum = _ttoi(strHaoMa);
				CString strDesc = GetFunnyDesc(nNum);

				m_listChqSSC.InsertItem(nCount, strDesc);
			}
			else
			{
				m_listChqSSC.InsertItem(nCount, strHaoMa);
			}
			m_listChqSSC.SetItemText(nCount, 1, GetWanDesc());
			CString strTmp;
			strTmp.Format(_T("%d"), zhushu);
			m_listChqSSC.SetItemText(nCount, 2, strTmp);
			m_listChqSSC.SetItemText(nCount, 3, strTmp);
			m_listChqSSC.createItemButton(nCount, 4, this->GetSafeHwnd());
			CString strMD5Haoma;
			for(int i = 0;i < vecAllNums.size();i++)
			{
				strMD5Haoma += vecAllNums[i].c_str();
				strMD5Haoma += L"|";
			}

			m_nZongZhuShu += zhushu;


			memset(m_szMD5Haoma, 0, sizeof(m_szMD5Haoma));
			CMD5Encrypt::EncryptData(strMD5Haoma,m_szMD5Haoma);

		}
		strHaoMa = _T("");
	}

	int arrLen = strArr.GetCount();
	if (arrLen > 0) 
	{
		int nLen = m_strHaoma.GetLength();
		for (int i=0; i<arrLen; i++) 
		{
			m_strHaoma += strArr.GetAt(i);
			m_strHaoma += L"|";

		}
		if(m_strHaoma.GetLength()>10240)
		{

			//CString strListHaoma = m_listChqSSC.GetItemText(0,0);
			if((nLen!=0)||(m_nZongZhuShu!=0))
			{
				CInformation Information;
				if(Information.ShowMessageBox(L"温馨提醒",L"本操作将清除已添加号码，请确认",MB_YESNO)==IDYES)
				{
					OnBnClickedBtnClsList();
				}
				else
				{
					m_DlgStatus.HideStatusWindow();
					return ;
				}

			}

			memset(m_szMD5Haoma, 0, sizeof(m_szMD5Haoma));
			CString strTempHaoma = m_strHaoma.Left(3000);
			CMD5Encrypt::EncryptData(strTempHaoma,m_szMD5Haoma);

			int nCount = m_listChqSSC.GetItemCount();

			m_listChqSSC.InsertItem(nCount, L"号码添加成功");
			m_listChqSSC.SetItemText(nCount, 1, GetWanDesc());

			m_nZongZhuShu += (arrLen*zhushu);
			CString strTmp;
			strTmp.Format(_T("%d"), m_nZongZhuShu);
			m_listChqSSC.SetItemText(nCount, 2, strTmp);
			m_listChqSSC.SetItemText(nCount, 3, strTmp);
			m_listChqSSC.createItemButton(nCount, 4, this->GetSafeHwnd());
			strArr.RemoveAll();

			memset(&g_arr, 0, sizeof(g_arr));
			memset(&g_nPos, 0, sizeof(g_nPos));
			g_Counts=0;

		}
		else
		{
			m_strHaoma.Empty();
			int nCount = m_listChqSSC.GetItemCount();

			CString strArrAt;
			for (int i=0; i<arrLen; i++) 
			{
				strArrAt = strArr.GetAt(i);
				m_listChqSSC.InsertItem(nCount+i, strArrAt);
				m_listChqSSC.SetItemText(nCount+i, 1, GetWanDesc());
				CString strTmp;
				strTmp.Format(_T("%d"), zhushu);
				m_listChqSSC.SetItemText(nCount+i, 2, strTmp);
				strTmp.Format(_T("%d"), m_beishu);
				m_listChqSSC.SetItemText(nCount+i, 3, strTmp);
				m_listChqSSC.createItemButton(nCount+i, 4, this->GetSafeHwnd());
				wstring strdata;
				strdata = strArrAt.GetBuffer();
				vecAllNums.insert(vecAllNums.begin(),1,strdata);//.push_back(strdata);
			}

			CString strMD5Haoma;
			for(int i = 0;i < vecAllNums.size();i++)
			{
				strMD5Haoma += vecAllNums[i].c_str();
				strMD5Haoma += L"|";
			}

			memset(m_szMD5Haoma, 0, sizeof(m_szMD5Haoma));
			CMD5Encrypt::EncryptData(strMD5Haoma,m_szMD5Haoma);

			m_nZongZhuShu += (arrLen*zhushu);
			strArr.RemoveAll();
			strArrAt.ReleaseBuffer();

			memset(&g_arr, 0, sizeof(g_arr));
			memset(&g_nPos, 0, sizeof(g_nPos));
			g_Counts=0;
		}

	}


	double danzhujine = 2.0;

	if(theAccount.Scoretype == MoShi_Yuan)
	{	
		danzhujine = 2.0;
	}
	else if (theAccount.Scoretype == MoShi_Jiao)
	{
		danzhujine = 0.2;
	}
	else if(theAccount.Scoretype == MoShi_Fen)
	{
		danzhujine = 0.02;
	}
	else if(theAccount.Scoretype == MoShi_Li)
		danzhujine = 0.002;
	int nItemCount = m_listChqSSC.GetItemCount();
	m_zongJine = 0.00f;
	for(int i = 0;i < nItemCount;i++)
	{
		CString strBeiSHu = m_listChqSSC.GetItemText(i,3);
		CString strZhuSHu = m_listChqSSC.GetItemText(i,2);
		int beishu = _ttoi(strBeiSHu);
		int zhushu = _ttoi(strZhuSHu);


		m_zongJine += zhushu*beishu*danzhujine;
	}


	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);

	m_DlgStatus.HideStatusWindow();

	ResetAllNums();

}

//追号
void CXingYun28::OnBnClickedBtnZhuihao()
{
	if(!CheckInput())
		return ;
	CDlgAddtional	dlgZhuiHao;
	double danzhujine = 2.0;

	if(theAccount.Scoretype == MoShi_Yuan)
	{	
		danzhujine = 2.0;
	}
	else if (theAccount.Scoretype == MoShi_Jiao)
	{
		danzhujine = 0.2;
	}
	else if(theAccount.Scoretype == MoShi_Fen)
	{
		danzhujine = 0.02;
	}
	CString strBeishu ;
	strBeishu.Format(L"%d",m_beishu);

	//if(m_bKaiJiangzhong)
	dlgZhuiHao.SetCurrentQihaoTime(m_chqRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,0,6, m_nZongZhuShu  * danzhujine,10000000,CZXingYun28);
	// 	else
	// 		dlgZhuiHao.SetCurrentQihaoTime( m_kjXinxi[0].qihao, strBeishu,8,3, m_zongZhuShu * danzhujine,120);
	if(dlgZhuiHao.DoModal() == IDOK)
	{
		m_nZhuihaoCount = dlgZhuiHao.GetZhuiHaoQiShu();
		dlgZhuiHao.GetZhuiHaoData(vecData1, vecData2, vecData3);


		m_bZhuiZjtz = dlgZhuiHao.GetPrizeStop();

		DOUBLE fZongJine = 0.0;
		for(int i = 0;i < m_nZhuihaoCount;i++)
		{
			int nBeishu = _ttoi(vecData1[i].c_str());
			fZongJine += m_nZongZhuShu*nBeishu*danzhujine;
		}

		if(fZongJine > theAccount.yue)
		{
			MyMessageBox(_T("您余额不足，请充值后再试"));
		}
		else
			SendToServer(4);
	}
	else
		return;

	//初始化参数
	m_nZongZhuShu = 0;

	m_beishu = 1;
	//UpdateData(FALSE);
OnBnClickedBtnClsList();
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
}
//投注
void CXingYun28::OnBnClickedBtnTouzhu()
{
	CString strHao = m_listChqSSC.GetItemText(0,0);
	if(strHao.IsEmpty())
	{
		OnBnClickedBtnChqAdd();
	}
	//校验输入
	if(!CheckInput())
		return;

	//请用户确认信息
	CQrTzhDlg qrDlg;
	qrDlg.m_gamekind = _T("幸运28");
	qrDlg.m_zhushu = m_nZongZhuShu * m_beishu;
	qrDlg.m_moshi = theAccount.Scoretype;
	if (qrDlg.DoModal() != IDOK)
	{
		return;
	}


	bool bSucc = true;
	int itm_cnt = m_listChqSSC.GetItemCount();
	m_nTouzhuCount=0;
	m_nTouzhuCount = itm_cnt;

	SendToServer(1);
	OnBnClickedBtnClsList();


	m_nZongZhuShu = 0;

	m_beishu = 1;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);

	InvalidateRect(&rcZongJinE);

}
//删除
void CXingYun28::OnBnClickedBtnDelSel()
{
	if(!m_strHaoma.IsEmpty())
	{
		m_strHaoma.Empty();
	}
	POSITION pos = m_listChqSSC.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listChqSSC.GetNextSelectedItem(pos);

		int zhushu = _ttoi(m_listChqSSC.GetItemText(nItem, 2));
		m_nZongZhuShu -= zhushu;
		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=400;
		InvalidateRect(&rcInvali);

		InvalidateRect(&rcZongJinE);

		m_listChqSSC.DeleteItem(nItem);

		if(vecAllNums.size() >0)
		{
			vector<wstring>::iterator   iter = vecAllNums.begin()+nItem;
			vecAllNums.erase(iter);
		}
	}

	CString strMD5Haoma;
	for(int i = 0;i < vecAllNums.size();i++)
	{
		strMD5Haoma += vecAllNums[i].c_str();
		strMD5Haoma += L"|";
	}

	memset(m_szMD5Haoma, 0, sizeof(m_szMD5Haoma));
	CMD5Encrypt::EncryptData(strMD5Haoma,m_szMD5Haoma);

}
//单数修改
void CXingYun28::OnEnChangeEditDanshiInput()
{
	m_richDanshiHaoma.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
}

//倍数 修改
void CXingYun28::OnEnChangeEditBeishu()
{
	CString str;
	m_editBeiTou.GetWindowText(str);
	if ( (_ttoi(str) < 1)) 
	{
		if(!str.IsEmpty())
		{
			MyMessageBox(_T("投注金额必须是一个正整数！"));
			m_beishu = 1;
			UpdateData(FALSE);
		}
	}
	else if(_ttoi(str)>theAccount.yue)
	{
		MyMessageBox(_T("投注金额不能超过自己的本金！"));
		m_beishu = 1;
		UpdateData(FALSE);

	}
	else
	{
		UpdateData();

		GetXy28Zhushu(CZXingYun28,m_gameKind);
		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=400;
		InvalidateRect(&rcInvali);
		InvalidateRect(&rcZongJinE);
		rcInvali.CopyRect(rcAllZongzhushu);
		rcInvali.bottom += 20;
		InvalidateRect(&rcInvali);
	}

}





//对象索引
WORD CXingYun28::GetGameHoverIndex(CPoint MousePoint)
{
	if(!m_GameListRect.PtInRect(MousePoint))
	{
		return INVALID_WORD;
	}
	WORD wHoverItem = INVALID_WORD;

	wHoverItem = (WORD)MousePoint.y/m_ImageItemBackMid.GetHeight();
	if(MousePoint.y%m_ImageItemBackMid.GetHeight()>0)
	{
		wHoverItem+=1;
	}
	wHoverItem-=1;
	return wHoverItem;
}

//鼠标消息
VOID CXingYun28::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

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

}

//鼠标消息
VOID CXingYun28::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//设置焦点
	SetFocus();

	if(m_GameListRect.PtInRect(Point))
	{
		WORD wDownItem = GetGameHoverIndex(Point);
		if(wDownItem == INVALID_WORD) return;

		//m_cbButtonDown = wDownItem;
		m_wViewItemDown = wDownItem;
	}

	return;
}
//鼠标消息
VOID CXingYun28::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);
	WORD wButtonUp = GetGameHoverIndex(Point);
	if((m_wViewItemDown!=INVALID_WORD)&&(m_wViewItemDown == wButtonUp))
	{
		ShowWindow(SW_HIDE);
		CWnd* pParent = GetParent();
		if(pParent!=NULL)
		{
			pParent->SendMessage(IDM_CLICKED_TYPE_SSC,m_wViewItemDown,m_cbMenuType);
		}
	}
}
//鼠标消息
LRESULT CXingYun28::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置状态
	m_bHovering=false;
	return 0;
}


//清空
void CXingYun28::OnBnClickedBtnClsList()
{
	m_nZongZhuShu = 0;
	m_zongJine = 0.00f;
	m_listChqSSC.deleteAllItemEx();
	vecAllNums.clear();
	vecData1.clear();
	vecData2.clear();
	vecData3.clear();
	m_editBeiTou.SetWindowText(L"1");
	m_richDanshiHaoma.SetWindowText(L"");
	m_strHaoma.Empty();
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);

	InvalidateRect(&rcZongJinE);
}

//设置返点
void CXingYun28::OnBnClickedBtnLock()
{
	int index = (CZXingYun28 << 16) | (unsigned short)m_gameKind;


	CDlgFandian	dlg;
	dlg.SetBonus(mapBonus[m_gameKind], theBonus[index], mapPercent[m_gameKind], theAccount.fandian*100.0, mapDanweiFandian[m_gameKind]);
	if(IDOK == dlg.DoModal())
	{
		dlg.GetBonus(m_fBonus,  m_fPercent);

		SendToServer(5);
	}
}


//倍投加
void CXingYun28::OnBnClickedBeiTouJia()
{
	CString strBeishu;

	m_beishu += 1;

	strBeishu.Format(L"%d",m_beishu);

	m_editBeiTou.SetWindowText(strBeishu);
	return;

}
//倍投减
void CXingYun28::OnBnClickedBeiTouJian()
{
	CString strBeishu;

	m_beishu -= 1;

	if(m_beishu<1)
	{
		m_beishu = 1;
	}
	strBeishu.Format(L"%d",m_beishu);

	m_editBeiTou.SetWindowText(strBeishu);
	return;
}




void CXingYun28::OnBnClinckedBtnDaXiaoDanShuang()		//大小单双
{
	CheckWanFaBtn(m_btnDaXiaoDanShuang);
	m_gameKind = enXY28_DXDS;
	SendToServer(3);

	ResetAllNums();
	ShowDaXiaoDanShuang();
	HideBJNums();
	HideRio();
	HideHeZhiNums();
	HideJiZhi();
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);


	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	OnBnClickedBtnClsList();
	HideDanShiEdit();
	Invalidate(false);

}


//北京快8
void CXingYun28::OnBnClinckedBtnRenXuan()	//任选
{

	CheckWanFaBtn(m_btnRenXuan);

	//设置当前模式
	m_gameKind = enBJK8_1;
	SendToServer(3);

	ResetAllNums();

	HideJiZhi();
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();


	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);

	OnBnClickedBtnClsList();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);

}

//特码
void CXingYun28::OnBnClinckedBtnTeMa()	//特码
{
	CheckWanFaBtn(m_btnTeMa);


	m_gameKind = enXY28_TeMa;
	SendToServer(3);

	ResetAllNums();
	HideBJNums();
	ShowDaXiaoDanShuang();
	HideRio();
	HideJiZhi();


	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);


	OnBnClickedBtnClsList();

	HideDanShiEdit();
	ShowHezhiNums();
	//Invalidate(false);
}
//趣味玩法
void CXingYun28::OnBnClinckedBtnFunny()
{
	CheckWanFaBtn(m_btnFunny);
	m_gameKind = enXY28_Funny;
	SendToServer(3);

	ResetAllNums();

	HideJiZhi();
	HideRio();
	HideBJNums();
	HideHeZhiNums();
	HideDanShiEdit();
	HideDaXiaoDanShuang();
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);

	m_rioDuiZi.ShowWindow(SW_SHOW);
	m_rioShunZi.ShowWindow(SW_SHOW);
	m_rioBaoZi.ShowWindow(SW_SHOW);
	OnBnClickedBtnClsList();

	OnRioDuiZi();
	Invalidate(false);

}



//对子
void CXingYun28::OnRioDuiZi()
{
	SelectRio(m_rioDuiZi);
	m_StrFunny = "14";
}

//顺子
void CXingYun28::OnRioShunZi()
{
	SelectRio(m_rioShunZi);
	m_StrFunny = "15";
}

//豹子
void CXingYun28::OnRioBaoZi()
{
	SelectRio(m_rioBaoZi);
	m_StrFunny = "16";
}

//极值
void CXingYun28::OnBnClinckedBtnJiZhi()
{
	CheckWanFaBtn(m_btnJiZhi);


	m_gameKind = enXY28_JiZhi;
	SendToServer(3);

	ResetAllNums();

	HideRio();
	HideBJNums();
	HideHeZhiNums();
	HideDanShiEdit();
	HideDaXiaoDanShuang();
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);
	OnBnClickedBtnClsList();
	ShowJiZji();
}




//初始化玩法信息
void CXingYun28::InitWanFaMoShi()
{
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enBJK8_10] = _T("任选10");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enBJK8_9] = _T("任选9");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enBJK8_8] = _T("任选8");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enBJK8_7] = _T("任选7");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enBJK8_6] = _T("任选6");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enBJK8_5] = _T("任选5");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enBJK8_4] = _T("任选4");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enBJK8_3] = _T("任选3");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enBJK8_2] = _T("任选2");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enBJK8_1] = _T("任选1");


	theGameKind[(CZXingYun28 << 16) | (unsigned short)enXY28_TeMa] = _T("特码");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enXY28_DXDS] = _T("大小单双");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enXY28_Funny] = _T("趣味");
	theGameKind[(CZXingYun28 << 16) | (unsigned short)enXY28_JiZhi] = _T("极值");
}



VOID CXingYun28::SendToServer(int nSendType)
{
	if(nSendType == 1)
	{
		m_bTouzhu = true;
		if(m_bTouzhu)
		{
			m_bTouzhu = false;

			if(m_RenxuanKeyboardInput)
			{

				int nLen = m_strHaoma.GetLength();
				if(nLen > 10240)
				{
					int nSendCount = 0;
					int nEnd = 1;

					CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
					ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
					TouzhuCQSSC.dwUserID = theAccount.user_id;
					TouzhuCQSSC.nBeitou = m_beishu;
					TouzhuCQSSC.nGameKind = m_gameKind;
					TouzhuCQSSC.nGameType = CZXingYun28;
					TouzhuCQSSC.nMoshi = theAccount.Scoretype;

					TouzhuCQSSC.nSign = m_nTzhSign;

					if (m_gameKind == 0)
					{
						AfxGetMainWnd()->MessageBox(L"kindId = 0");
					}

					CString strQiHao = m_chqRule.GetNextExpect();

					if(m_strHaoma.IsEmpty())
					{
						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"你还未选择号码，请选择！");
						OnBnClickedBtnClsList();
						return ;
					}
					TCHAR szMD5Haoma[33];
					memset(szMD5Haoma, 0, sizeof(szMD5Haoma));

// 					CMD5Encrypt::EncryptData(m_strHaoma,szMD5Haoma);
// 
// 					if(lstrcmp(m_szMD5Haoma,szMD5Haoma)!=0)
// 					{
// 						m_DlgStatus.HideStatusWindow();
// 						OnBnClickedBtnClsList();
// 						return ;
// 					}

					string	strAllHaoma;
					ChangeStringToA(m_strHaoma, strAllHaoma);

					string strQi;
					ChangeStringToA(strQiHao, strQi);

					memcpy(TouzhuCQSSC.strQishu ,strQi.c_str(),strQi.length());


					byte pDestBuffer[102400];
					int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));

					int nSendSize = 0;
					TouzhuCQSSC.nEnd = 1;
					if(m_nZongZhuShu<=0)
					{
						m_DlgStatus.HideStatusWindow();
						OnBnClickedBtnClsList();
						return;
					}

					TouzhuCQSSC.nZhushu = m_nZongZhuShu;
					int nSize = sizeof(TouzhuCQSSC.strHaoma);
					DWORD dwTick=GetTickCount();

					while (nSendSize < nLen)
					{
						while ((GetTickCount() - dwTick ) < 30000)
						{
							MSG msg;
							if(GetMessage (&msg, NULL, 0, 0))
							{
								TranslateMessage (&msg) ;

								DispatchMessage (&msg) ;

							}
							if(m_bCanSend)
								break;

						}
						int nHaoMaLen = 0;
						nHaoMaLen = min(nSize,nLen-nSendSize);
						TouzhuCQSSC.nHaoMaLen = nHaoMaLen;	//nLen;

						TouzhuCQSSC.nIndex = nSendCount;
						memset(TouzhuCQSSC.strHaoma, 0, sizeof(TouzhuCQSSC.strHaoma));
						memcpy((byte*)TouzhuCQSSC.strHaoma, pDestBuffer+nSendSize, nSize);


						if(nSendSize + nSize >= nLen)
							TouzhuCQSSC.nEnd = 0;

						CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
						bool bSuccess = pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_DAN,&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
						nSendSize += nHaoMaLen;
						m_bCanSend = false;
						nSendCount++;
					}

					m_bCanSend = true;
					m_DlgStatus.ShowStatusWindow(TEXT("正在提交数据，请稍候..."));

				}
				else
				{
					m_strHaoma.Empty();

					for(int i = 0;i < vecAllNums.size();i++)
					{
						m_strHaoma += vecAllNums[i].c_str();
						m_strHaoma += L"|";
					}
					if(m_strHaoma.GetLength()>1024)
					{
						m_DlgStatus.ShowStatusWindow(TEXT("正在提交数据，请稍候..."));
					}

					TCHAR szMD5Haoma[33];
					memset(szMD5Haoma, 0, sizeof(szMD5Haoma));
// 					CMD5Encrypt::EncryptData(m_strHaoma,szMD5Haoma);
// 
// 					if(lstrcmp(m_szMD5Haoma,szMD5Haoma)!=0)
// 					{
// 						m_DlgStatus.HideStatusWindow();
// 						OnBnClickedBtnClsList();
// 						return ;
// 					}
// 
					int nSendCount = 0;
					int nEnd = 1;

					CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
					ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
					TouzhuCQSSC.dwUserID = theAccount.user_id;
					TouzhuCQSSC.nBeitou = m_beishu;
					TouzhuCQSSC.nGameKind = m_gameKind;
					TouzhuCQSSC.nGameType = CZXingYun28;
					TouzhuCQSSC.nMoshi = theAccount.Scoretype;

					TouzhuCQSSC.nSign = m_nTzhSign;

					CString strQiHao = m_chqRule.GetNextExpect();
					if(m_strHaoma.IsEmpty())
					{
						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"你还未选择号码，请选择！");
						OnBnClickedBtnClsList();
						return ;
					}

					string	strAllHaoma;
					ChangeStringToA(m_strHaoma, strAllHaoma);
					//int itm_cnt = strAllHaoma.length();


					string strQi;
					ChangeStringToA(strQiHao, strQi);

					memcpy(TouzhuCQSSC.strQishu ,strQi.c_str(),strQi.length());

					//					lstrcpyn(TouzhuCQSSC.strQishu ,strQiHao,CountArray(TouzhuCQSSC.strQishu));


					byte pDestBuffer[102400];
					int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));

					int nSendSize = 0;
					TouzhuCQSSC.nEnd = 1;
					if(m_nZongZhuShu<=0)
					{
						m_DlgStatus.HideStatusWindow();
						OnBnClickedBtnClsList();
						return;
					}

					TouzhuCQSSC.nZhushu = m_nZongZhuShu;
					int nSize = sizeof(TouzhuCQSSC.strHaoma);
					DWORD dwTick=GetTickCount();

					while (nSendSize < nLen)
					{
						while ((GetTickCount() - dwTick ) < 30000)
						{
							MSG msg;
							if(GetMessage (&msg, NULL, 0, 0))
							{
								TranslateMessage (&msg) ;

								DispatchMessage (&msg) ;

							}
							if(m_bCanSend)
								break;

						}
						int nHaoMaLen = 0;
						nHaoMaLen = min(nSize,nLen-nSendSize);
						TouzhuCQSSC.nHaoMaLen = nHaoMaLen;	//nLen;

						TouzhuCQSSC.nIndex = nSendCount;
						memset(TouzhuCQSSC.strHaoma, 0, sizeof(TouzhuCQSSC.strHaoma));
						memcpy((byte*)TouzhuCQSSC.strHaoma, pDestBuffer+nSendSize, nSize);

						if(nSendSize + nSize >= nLen)
							TouzhuCQSSC.nEnd = 0;

						CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
						bool bSuccess = pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_DAN,&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
						m_bShowFail=true;

						nSendSize += nHaoMaLen;
						m_bCanSend = false;
						nSendCount++;
					}

					m_bCanSend = true;
				}
			}
			else
			{
				m_strHaoma.Empty();

				for(int i = 0;i < vecAllNums.size();i++)
				{
					m_strHaoma += vecAllNums[i].c_str();
					m_strHaoma += L"|";
				}
				if(m_strHaoma.GetLength()>1024)
				{
					m_DlgStatus.ShowStatusWindow(TEXT("正在提交数据，请稍候..."));
				}

				TCHAR szMD5Haoma[33];
				memset(szMD5Haoma, 0, sizeof(szMD5Haoma));
// 				CMD5Encrypt::EncryptData(m_strHaoma,szMD5Haoma);
// 
// 				if(lstrcmp(m_szMD5Haoma,szMD5Haoma)!=0)
// 				{
// 					m_DlgStatus.HideStatusWindow();
// 					OnBnClickedBtnClsList();
// 					return ;
// 				}

				int nSendCount = 0;
				int nEnd = 1;

				CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
				ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
				TouzhuCQSSC.dwUserID = theAccount.user_id;
				TouzhuCQSSC.nBeitou = m_beishu;
				TouzhuCQSSC.nGameKind = m_gameKind;
				TouzhuCQSSC.nGameType = CZXingYun28;
				TouzhuCQSSC.nMoshi = theAccount.Scoretype;

				TouzhuCQSSC.nSign = m_nTzhSign;

				CString strQiHao = m_chqRule.GetNextExpect();

				string	strAllHaoma;
				ChangeStringToA(m_strHaoma, strAllHaoma);

				if (m_gameKind == 0)
				{
					AfxGetMainWnd()->MessageBox(L"kindId = 0");
				}

				string strQi;
				ChangeStringToA(strQiHao, strQi);

				memcpy(TouzhuCQSSC.strQishu ,strQi.c_str(),strQi.length());


				if(m_strHaoma.IsEmpty())
				{
					m_DlgStatus.HideStatusWindow();
					MyMessageBox(L"你还未选择号码，请选择！");

					return ;
				}

				byte pDestBuffer[102400];
				int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));

				int nSendSize = 0;
				TouzhuCQSSC.nEnd = 1;
				if(m_nZongZhuShu<=0)
				{
					m_DlgStatus.HideStatusWindow();
					OnBnClickedBtnClsList();
					return;
				}

				TouzhuCQSSC.nZhushu = m_nZongZhuShu;
				int nSize = sizeof(TouzhuCQSSC.strHaoma);
				DWORD dwTick=GetTickCount();

				while (nSendSize < nLen)
				{
					while ((GetTickCount() - dwTick ) < 30000)
					{
						MSG msg;
						if(GetMessage (&msg, NULL, 0, 0))
						{
							TranslateMessage (&msg) ;

							DispatchMessage (&msg) ;

						}
						if(m_bCanSend)
							break;

					}
					int nHaoMaLen = 0;
					nHaoMaLen = min(nSize,nLen-nSendSize);
					TouzhuCQSSC.nHaoMaLen = nHaoMaLen;	//nLen;

					TouzhuCQSSC.nIndex = nSendCount;
					memset(TouzhuCQSSC.strHaoma, 0, sizeof(TouzhuCQSSC.strHaoma));
					memcpy((byte*)TouzhuCQSSC.strHaoma, pDestBuffer+nSendSize, nSize);


					if(nSendSize + nSize >= nLen)
						TouzhuCQSSC.nEnd = 0;

					CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
					bool bSuccess = pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_DAN,&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
					m_bShowFail=true;

					nSendSize += nHaoMaLen;
					m_bCanSend = false;
					nSendCount++;
				}

				m_bCanSend = true;
			}

			m_nTzhSign++;
			SetTimer(TimerStatus,60000,NULL);
			//	m_strHaoma = _T("");
			OnBnClickedBtnClsList();


		}
		return;

	}

	if(nSendType == 2)   //没用
	{
		m_bGetCPUserInfo = true;
		return;
	}

	if(nSendType == 3)
	{
		m_btnLock.SetWindowText(mapFandian[m_gameKind]);


		OnBnClickedBtnClsList();

		m_bGetUserFandian = true;
		m_nZongZhuShu = 0;
		m_editBeiTou.SetWindowText(L"1");
		m_beishu=1;

		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=400;
		InvalidateRect(&rcInvali);

		InvalidateRect(&rcZongJinE);
		return;
	}

	if(nSendType == 4)
	{
		m_bZhuihao = true;
		if(m_bZhuihao)
		{
			m_bZhuihao = false;
			m_DlgStatus.ShowStatusWindow(TEXT("正在提交数据，请稍候..."));

			//if(m_RenxuanKeyboardInput/*&&m_strHaoma.GetLength() >10240*/)
			{
				if(m_strHaoma.IsEmpty() && vecAllNums.size()>0)
				{
					for(int i = 0;i < vecAllNums.size();i++)
					{
						m_strHaoma += vecAllNums[i].c_str();
						m_strHaoma += L"|";
					}

				}

				//for(int n=0; n< vecData2.size(); n++)
				{
					int nSendCount = 0;
					int nEnd = 1;

					CMD_GP_TouZhuCQSSC_Zhuihao TouzhuCQSSC;
					ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
					TouzhuCQSSC.dwUserID = theAccount.user_id;
					TouzhuCQSSC.nGameKind = m_gameKind;
					TouzhuCQSSC.nGameType = CZXingYun28;
					TouzhuCQSSC.nMoshi = theAccount.Scoretype;

					TouzhuCQSSC.nSign = m_nTzhSign;
					if(m_bZhuiZjtz)
						TouzhuCQSSC.bZhuihao = GetTickCount();
					else
						TouzhuCQSSC.bZhuihao = 2;

					if(m_strHaoma.IsEmpty())
					{
						m_DlgStatus.HideStatusWindow();
//						MyMessageBox(L"你还未选择号码，请选择！");
						MyMessageBox(L"你还未选择号码，请选择！");
						OnBnClickedBtnClsList();
						return ;
					}
					TCHAR szMD5Haoma[33];
					memset(szMD5Haoma, 0, sizeof(szMD5Haoma));

// 					CMD5Encrypt::EncryptData(m_strHaoma,szMD5Haoma);
// 
// 					if(lstrcmp(m_szMD5Haoma,szMD5Haoma)!=0)
// 					{
// 						OnBnClickedBtnClsList();
// 						return ;
// 					}



					string	strAllHaoma;
					ChangeStringToA(m_strHaoma, strAllHaoma);

					memset(TouzhuCQSSC.strQishu,0,sizeof(TouzhuCQSSC.strQishu));
					string sNowQi;
					ChangeStringToA(m_chqRule.GetNextExpect(),sNowQi);
					for(int o=0; o< vecData2.size(); o++)
					{
						string strQi;
						ChangeStringToA(vecData2[o].c_str(),strQi);
						if(strQi < sNowQi)
						{

							MyMessageBox(L"订单期号有误！可能原因，该期已经停止投注！");
							OnBnClickedBtnClsList();
							m_DlgStatus.HideStatusWindow();
							return ;
						}
						memcpy(TouzhuCQSSC.strQishu[o],strQi.c_str(),strQi.length());
						TouzhuCQSSC.nBeitou[o] = _ttoi(vecData1[o].c_str());;
					}


					byte pDestBuffer[102400];
					int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));

					int nSendSize = 0;
					TouzhuCQSSC.nEnd = 1;
					if(m_nZongZhuShu<=0)
					{
						m_DlgStatus.HideStatusWindow();
						OnBnClickedBtnClsList();
						return;
					}

					TouzhuCQSSC.nZhushu = m_nZongZhuShu;
					int nSize = sizeof(TouzhuCQSSC.strHaoma);
					DWORD dwTick=GetTickCount();

					while (nSendSize < nLen)
					{
						while ((GetTickCount() - dwTick ) < 30000)
						{
							MSG msg;
							if(GetMessage (&msg, NULL, 0, 0))
							{
								TranslateMessage (&msg) ;

								DispatchMessage (&msg) ;

							}
							if(m_bCanSend)
								break;

						}
						int nHaoMaLen = 0;
						nHaoMaLen = min(nSize,nLen-nSendSize);
						TouzhuCQSSC.nHaoMaLen = nHaoMaLen;	//nLen;

						TouzhuCQSSC.nIndex = nSendCount;
						memset(TouzhuCQSSC.strHaoma, 0, sizeof(TouzhuCQSSC.strHaoma));
						memcpy((byte*)TouzhuCQSSC.strHaoma, pDestBuffer+nSendSize, nSize);


						if(nSendSize + nSize >= nLen)
							TouzhuCQSSC.nEnd = 0;

						CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
						bool bSuccess = pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TOUZHU_CQSSC_ZHUIHAO,&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
						m_bShowFail=true;
						nSendSize += nHaoMaLen;
						m_bCanSend = false;
						nSendCount++;
					}

				}

			}

			m_nTzhSign++;


			SetTimer(TimerStatus,60000,NULL);
			m_strHaoma = _T("");
			OnBnClickedBtnClsList();


		}
		return;


	}

	if(nSendType == 5)
	{
		m_bSetBonus = true;
		if(m_bSetBonus)
		{
			m_bSetBonus=false;
			CMD_GP_SetUserBonus SetUserBonus;
			ZeroMemory(&SetUserBonus,sizeof(SetUserBonus));

			SetUserBonus.n_t_userid = theAccount.user_id;
			SetUserBonus.n_t_type_id = CZXingYun28;
			SetUserBonus.n_t_kind_id = m_gameKind;
			SetUserBonus.f_t_bonus = m_fBonus;
			SetUserBonus.f_bonuspercent = m_fPercent;

			//发送数据
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SET_USER_BONUS,&SetUserBonus,sizeof(SetUserBonus));
		}
		return;

	}

	if(nSendType == 6)
	{
		m_bGetUserFandian = true;
		//m_btnLock.EnableTextButton(true);
		if(m_bGetUserFandian)
		{
			m_bGetUserFandian=false;
			CMD_GP_GetUserFandian GetUserFandian;
			ZeroMemory(&GetUserFandian,sizeof(GetUserFandian));

			GetUserFandian.n_t_userid = theAccount.user_id;
			GetUserFandian.n_t_type_id = CZXingYun28;
			GetUserFandian.n_t_kind_id = m_gameKind;

			//发送数据
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_USER_FANDIAN,&GetUserFandian,sizeof(GetUserFandian));
		}

		return;
	}

	if(nSendType == 7)
	{
		// 		m_bGetLastYue =true;
		// 		if(m_bGetLastYue)
		// 		{
		// 			m_bGetLastYue = false;
		// 			CMD_GP_GetLastYue GetLastYue;
		// 			ZeroMemory(&GetLastYue,sizeof(GetLastYue));
		// 
		// 			GetLastYue.dwUserID = theAccount.user_id;
		// 
		// 			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		// 			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_LAST_YUE,&GetLastYue,sizeof(GetLastYue));
		// 		}
		return;

	}
}



bool CXingYun28::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}



//读取事件
bool CXingYun28::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
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
				m_btnLock.SetWindowText(mapFandian[m_gameKind]);
				m_btnLock.EnableTextButton(false);


				return true;

			}
		case SUB_GP_TOUZHU_CONTINUE_RET:
			{
				m_bCanSend = true;
				return true;
			}
		case SUB_GP_GET_MAP_BONUS_RET:
			{

				ASSERT(wDataSize % sizeof(CMD_GR_GetMapBonusRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetMapBonusRet)!=0) return false;

				int nCount = wDataSize/sizeof(CMD_GR_GetMapBonusRet);
				for(int i = 0;i < nCount;i++)
				{
					CMD_GR_GetMapBonusRet* pGetMapBonus = ((CMD_GR_GetMapBonusRet*)pData+i);

					int index = (pGetMapBonus->n_t_type_id << 16) | (unsigned short)pGetMapBonus->n_t_kind_id;
					theBonus[index] = pGetMapBonus->f_t_bonus;

				}
				return true;
			}
		case SUB_GP_QUERY_TIME_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_GetSystemTime));
				if(wDataSize < sizeof(CMD_GR_GetSystemTime)) return false;

				CMD_GR_GetSystemTime* pSystemTime = (CMD_GR_GetSystemTime*)pData;

				theTime = pSystemTime->ServerTime;

				//SendToServer(3);

				return true;
			}
		case SUB_GP_SET_USER_BONUS_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_SetUserBonusRet));
				if(wDataSize < sizeof(CMD_GR_SetUserBonusRet)) return false;

				CMD_GR_SetUserBonusRet* pSetUserBonus = (CMD_GR_SetUserBonusRet*)pData;
				int nResult = pSetUserBonus->n_t_result;

				if(nResult==0)
				{
					SendToServer(6);
					SetLockText(m_fBonus, m_fPercent);
					MyMessageBox(L"返点数据修改成功！", L"温馨提示");
				}
				else if(nResult == 1)
				{
					MyMessageBox(L"你修改的返点不符合！", L"提示");
				}
				else if(nResult == 2)
				{
					MyMessageBox(L"此玩法不支持修改返点！", L"提示");
				}
				else
				{
					MyMessageBox(L"返点修改失败！",L"提示");
				}

				return true;
			}
		case SUB_GP_QUERY_GAME_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GP_QueryLotResult)==0);
				if(wDataSize % sizeof(CMD_GP_QueryLotResult)!=0)
					return false;

				m_kjXinxiCont = 0;
				WORD wCount = wDataSize/sizeof(CMD_GP_QueryLotResult);
				wCount = __min(wCount,Kj_XinXi_Count);

				for(int i = 0;i < wCount;i++)
				{
					CMD_GP_QueryLotResult* pQueryLotResult  =  ((CMD_GP_QueryLotResult*)pData+i);

					CMD_GP_QueryLotResult QueryLotResult;
					ZeroMemory(&QueryLotResult,sizeof(QueryLotResult));

					QueryLotResult.wKindID = pQueryLotResult->wKindID;
					lstrcpyn(QueryLotResult.wPeriod,pQueryLotResult->wPeriod,CountArray(QueryLotResult.wPeriod));
					lstrcpyn(QueryLotResult.szLotNum,pQueryLotResult->szLotNum,CountArray(QueryLotResult.szLotNum));
					lstrcpyn(QueryLotResult.szShijian,pQueryLotResult->szShijian,CountArray(QueryLotResult.szShijian));

					_tcscpy_s(m_kjXinxi[m_kjXinxiCont].qihao, KJ_QIHAO_LENGTH, QueryLotResult.wPeriod);
					_tcscpy_s(m_kjXinxi[m_kjXinxiCont].haoma, KJ_HAOMA_LENGTH, QueryLotResult.szLotNum);

					_tcscpy_s(m_kjXinxi[m_kjXinxiCont].shijian, KJ_HAOMA_LENGTH, QueryLotResult.szShijian);
					m_kjXinxiCont++;
				}


				if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
				{
					//第一次进入
					if(m_lastExpect[0] == 0)
						_tcscpy_s(m_lastExpect, KJ_HAOMA_LENGTH, m_kjXinxi[0].qihao);
					m_bKaiJiangzhong=false;											//开奖结束
					long secDiff = m_chqRule.GetKjShjDiff();

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
		case SUB_GP_TOUZHU_CQSSC_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_TouzhuRes));
				if(wDataSize < sizeof(CMD_GR_TouzhuRes)) return false;

				KillTimer(TimerStatus);
				CMD_GR_TouzhuRes* pTouzhuRes = (CMD_GR_TouzhuRes*)pData;

				LONG lResult = pTouzhuRes->lResult;

				m_bCanSend = true;

				m_DlgStatus.HideStatusWindow();
				if (lResult == 0)
				{
					if((pTouzhuRes->nSign != m_nTzhSign1))
					{
						m_nTzhSign1 = pTouzhuRes->nSign;
						m_listChqSSC.DeleteAllItems();
						MyMessageBox(_T("成功投注！"), _T("【祝君好运 投注成功】"));
						SendToServer(7);						

					}
				}
				else if(lResult == 1)
				{
					MyMessageBox(_T("注数错误，或者投注内容不正确！"));
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("该期号已经开奖，投注失败！"));
				}
				else if(lResult == 3)
				{
					MyMessageBox(_T("该期号已经过期，投注失败！"));
				}
				else if(lResult == 4)
				{
					MyMessageBox(_T("由于期号出现错误，投注失败！"));
				}
				else if(lResult == 5)
				{
					MyMessageBox(_T("无法获取玩法对应的奖金，投注失败！"));
				}
				else if(lResult == 6)
				{
					MyMessageBox(_T("其他错误，有订单没有成功提交！"));
				}
				else if(lResult == 7)
				{
					MyMessageBox(_T("其他错误，有订单没有成功提交！"));
				}
				else if(lResult == 8)
				{
					MyMessageBox(_T("投注过程返回提示：余额不足"));
				}
				else if(lResult == 9)
				{
					MyMessageBox(_T("其他错误，有订单没有成功提交！"));
				}
				else if(lResult == 10)
				{
					MyMessageBox(_T("对不起，您的账户没有投注权限！"));
				}
				else if(lResult == 11)
				{
					MyMessageBox(_T("注数不正确，投注失败！"));
				}
				else if(lResult == 12)
				{
					MyMessageBox(_T("对不起，该玩法维护中，请选择其他玩法进行投注！"));
				}
				else if(lResult>=20 && lResult <= 30)
				{
					MyMessageBox(_T("该账户没有投注权限！"));

				}
				else if(lResult == 111)
				{
					CInformation information;
					int nResult = information.ShowMessageBox(_T("投注未成功！由于您密码设置过于简单，请设置复杂登陆密码和取款密码！"),MB_ICONWARNING,30);
					if(nResult==1)
					{
						CWnd* pParent = GetParent();
						if(pParent!=NULL)
						{
							pParent->SendMessage(IDM_SHOW_XGMM,0,0);
						}
					}		
				}
				else if(lResult > 18)
				{
					CString strLog;
					int nFandian = 1900+lResult*2;
					strLog.Format(L"抱歉返点在%d以上的玩家不能投注！",nFandian);
					MyMessageBox(strLog);
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
		case SUB_GP_GET_LAST_YUE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetLastYueRet));
				if(wDataSize!=sizeof(CMD_GP_GetLastYueRet)) return false;

				CMD_GP_GetLastYueRet* pCountRet = (CMD_GP_GetLastYueRet*)pData;

				theAccount.yue = pCountRet->f_t_yue;
				theAccount.dongjie = pCountRet->f_t_dongjie;

				CWnd* pParent = GetParent();
				if(pParent!=NULL)
				{
					pParent->SendMessage(IDM_UPDATE_ACCOUNT,0,0);
				}
				break;
			}

		case SUB_GP_GET_CP_USER_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_GetCpUserInfoRet));
				if(wDataSize < sizeof(CMD_GR_GetCpUserInfoRet)) return false;

				CMD_GR_GetCpUserInfoRet* pGetCpUser = (CMD_GR_GetCpUserInfoRet*)pData;

				lstrcpyn(theAccount.account,pGetCpUser->s_t_account,sizeof(theAccount.account));
				theAccount.type = pGetCpUser->n_t_type;
				theAccount.fandian = pGetCpUser->f_t_fandian;
				theAccount.yue = pGetCpUser->f_t_yue;
				theAccount.dongjie = pGetCpUser->f_t_dongjie;
				return true;
			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}


void CXingYun28::SetLockText(float fBonus, float fBonusPercent)
{
	const float EPSINON = 0.00001;
	if((fBonus >= - EPSINON) && (fBonus <= EPSINON))
	{
	}
	else
	{
		CString strText;
		strText.Format(L"%.2lf/%.2lf", fBonus, fBonusPercent);
		m_btnLock.SetWindowText(strText + _T("%"));
	}
}

//上期开奖号码
 void CXingYun28::DrawLastKjHaoma(CDC* pDC, Graphics& graphics)
 {
 	long secDiff = m_chqRule.GetKjShjDiff();
 	if(IsWindowVisible() && secDiff > 60 && secDiff <=65)
 	{
 		CPlatformFrame* pPlatforFrame = CPlatformFrame::GetInstance();
 		if(pPlatforFrame->m_bSound)
 			sndPlaySound(MAKEINTRESOURCE(IDR_CLOCK),  SND_RESOURCE |SND_ASYNC);
 	}
 
 	if(IsWindowVisible() && secDiff == 61)
 	{
 		if(m_listChqSSC.GetItemCount() != 0)
 		{
 			CWnd *pParent = GetParent();
 			if(pParent!=NULL)
 			{
 				pParent->PostMessage(IDM_TANCHUANG,0,0);
 			}
 
 		}
 	}
 	int nTimeLeft = GetKjShjDiff();
 	//时间用尽了，显示开奖动画;
 	//m_kjXinxi 存的是本地最新开奖号码
 	//更新上期的期号
 	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
 	{
 		_tcscpy_s(m_lastExpect, KJ_HAOMA_LENGTH, m_kjXinxi[0].qihao);
 	}
 
 	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
 	CFont* pOldFont = pDC->SelectObject(&m_KjInfoFont1);
 	COLORREF oldTextClr = pDC->SetTextColor(RGB(255, 255, 255));

	CString strTopQi;
	strTopQi.Format(_T("北京 NO.%s"),m_chqRule.GetNextExpect());
 	pDC->DrawText(strTopQi, &rcCzh, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
//  	CRect rcDi;
//  	rcDi.left = rcCzh.right+10;
//  	rcDi.right = rcDi.left+20;
//  	rcDi.top = rcCzh.top;
//  	rcDi.bottom = rcCzh.bottom;
//  	pDC->DrawText(_T("第"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
//  	pDC->SetTextColor(RGB(253, 191, 6));
//  	if(m_bKaiJiangzhong)
//  	{
//  		pDC->DrawText(m_chqRule.GetNextExpect(-1), &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
//  	}
//  	else
//  		pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
//  	pDC->SetTextColor(RGB(255, 255, 255));
// 	rcDi.left = rcQiHao.right-12;
// 	rcDi.right = rcDi.left+90;
// 	rcDi.top = rcCzh.top;
// 	rcDi.bottom = rcCzh.bottom;
// 	pDC->DrawText(_T("开奖号码"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(&m_KjInfoFont1);
	CString strYg;
	CRect rcYuGao1;
	//strYg.Format(_T("正在销售"));
	//pDC->DrawText(strYg, &rcYuGao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);


// 	strYg.Format(_T("第%s	下单时间"), m_chqRule.GetNextExpect());
// 	//pDC->SetTextColor(RGB(255, 125, 0));
// 	rcYuGao1.left = rcYuGao.left+15;
// 	rcYuGao1.top = rcYuGao.top+40;
// 	rcYuGao1.bottom = rcYuGao.bottom+70;
// 	rcYuGao1.right = rcYuGao.right+50;
// 	pDC->DrawText(strYg, &rcYuGao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);


	pDC->SelectObject(&m_KjInfoFont);
	strYg.Format(_T("%s"), m_chqRule.GetKjShjDiffDesc());
	pDC->SetTextColor(RGB(219, 219, 219));
	rcYuGao1.left = rcYuGao1.left-30;
	rcYuGao1.top = rcYuGao1.top+12;
	rcYuGao1.bottom = rcYuGao1.bottom+3;
	rcYuGao1.right = rcYuGao1.right-30;
	pDC->DrawText(strYg, &rcYuGao1,DT_CENTER|DT_SINGLELINE);

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(&pOldFont);
	pDC->SetBkMode(oldBkMode);
 	CString str = m_kjXinxi[0].haoma;
 	//CString temp;
 	//temp.Format(L"%s",str.Left(2));
 
 	//if(_ttoi(temp) > 11)
 	//{
 	//	m_bKaiJiangzhong=true;
 	//	KillTimer(timer_id_kaijiangzhong);
 	//	SetTimer(timer_id_kaijiangzhong, 100, NULL);
 
 	//	return;
 	//}
 
//  	if (m_gameKind<=enBJK8_10)
//  	{
//  		int j = 0;
//  		int index = 0;
//  		CString strTemp;
//  		for (int i=0; i<__min(str.GetLength(),40); i++) 
//  		{
//  			if (iswdigit(str.GetAt(i))) 
//  			{
//  				strTemp+=str.GetAt(i);
//  				int num = _ttoi(CString(str.GetAt(i)));
//  
//  				if(strTemp.GetLength()>=2)
//  				{
//  					num = _ttoi(strTemp);
//  
//  					if(m_bKaiJiangzhong )
//  					{
//  						KillTimer(timer_id_kaijiangzhong);
//  						SetTimer(timer_id_kaijiangzhong, 100, NULL);
//  					}
//  					else
//  					{
//  						DrawBigNum(graphics, 
//  							m_kjNumBig2, 
//  							Rect(kj_big_haoma_x + ((index>=10)?(index-10):index)* m_bigNumWidth,
//  							kj_big_haoma_y+((index>=10)?m_bigNumHeight:0),
//  							m_bigNumWidth,
//  							m_bigNumHeight),
//  							num-1,m_bigNumWidth,m_bigNumHeight);
//  					}
//  				}
//  				j++;
//  				if(strTemp.GetLength()>=2)
//  				{
//  					//			num = _ttoi(strTemp);
//  					index ++;
//  					strTemp.Empty();
//  				}
//  
//  			}
//  		}
//  	}
//  	else
 	{
 		int index = 0;
 		int Sum = 0;
 		int SumCount = 0;
 		int Sumall = 0;
 		CString strTemp;
 		for (int i=0; i<40; i++) 
 		{
 			if (iswdigit(str.GetAt(i)) && (i%2!=0)) 
 			{
 				strTemp=str.GetAt(i);
 				int num = _ttoi(strTemp);
 
 				if(m_bKaiJiangzhong )
 				{
 					KillTimer(timer_id_kaijiangzhong);
 					SetTimer(timer_id_kaijiangzhong, 100, NULL);
 				}
 				else
 				{
 					SumCount++;
 					Sum += num;
 					while(Sum>=10)
 						Sum = Sum%10;
 
 					if(SumCount == 6)
 					{
 						DrawBigNum(graphics, 
 							m_kjNumBig, 
 							Rect(kj_big_haoma_x + index* (m_bigNumWidth+33),kj_big_haoma_y+30,m_kjNumBig->GetWidth()/28,m_kjNumBig->GetHeight()),
 							Sum,m_kjNumBig->GetWidth()/28,m_kjNumBig->GetHeight());
 
 						Sumall+=Sum;
 
 						Sum = 0;
 						SumCount =0;
 						index ++;
 
//  						DrawBigNum(graphics, 
//  							m_kjAdd, 
//  							Rect(kj_big_haoma_x + index* (m_bigNumWidth+10),kj_big_haoma_y+30,m_bigNumWidth,m_bigNumHeight),
//  							(index/2>1?1:0),m_bigNumWidth,m_bigNumHeight);
 					//	index ++;
 					}
 				}
 			}
			else
			{
				continue;
			}
 
 		}
 
 
 		DrawBigNum(graphics, 
 			m_kjNumBig2, 
 			Rect(kj_big_haoma_x + index* (m_bigNumWidth+33),kj_big_haoma_y+30,m_bigNumWidth,m_bigNumHeight),
 			Sumall-1,m_bigNumWidth,m_bigNumHeight);
 
 	}	
 }
 
//开奖号码
void CXingYun28::DrawTopKjHaoma(CDC* pDC, Graphics& graphics)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_smallNumfont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(88, 78, 77));

	for (int cnt=0; cnt<3; cnt++) 
	{
		CString str = m_kjXinxi[cnt].qihao;
		if(str.IsEmpty())
		{
			continue;
		}

		CRect rcText(kj_small_haoma_x - 50, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 80, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span) + m_smallNumHeight);
		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		str = m_kjXinxi[cnt].haoma;
		int index = 0;
		int Sum = 0;
		int SumCount = 0;
		int Sumall = 0;
		CString strTemp;
		for (int i=0; i<40; i++) 
		{
			if (iswdigit(str.GetAt(i)) && (i%2!=0)) 
			{
				strTemp=str.GetAt(i);
				int num = _ttoi(strTemp);

				SumCount++;
				Sum += num;
				while(Sum>=10)
					Sum = Sum%10;

				if(SumCount == 6)
				{
					DrawBigNum(graphics, 
						m_kjNumSmall, 
						Rect(kj_small_haoma_x + index* (m_smallNumWidth)+12,kj_small_haoma_y+ cnt * (m_smallNumHeight + kj_small_haoma_row_span),m_smallNumWidth,m_smallNumHeight),
						Sum,m_smallNumWidth,m_smallNumHeight);

					Sumall+=Sum;

					Sum = 0;
					SumCount =0;
					index ++;

// 					DrawBigNum(graphics, 
// 						m_kjAdd1, 
// 						Rect(kj_small_haoma_x + index* (m_smallNumWidth+3),kj_small_haoma_y+ cnt * (m_smallNumHeight + kj_small_haoma_row_span),m_smallNumWidth,m_smallNumHeight),
// 						(index/2>1?1:0),m_smallNumWidth,m_smallNumHeight);
					index ++;
				}
			}
			else
			{
				continue;
			}

		}


		DrawBigNum(graphics, 
			m_kjNumSmall1, 
			Rect(kj_small_haoma_x + index* (m_smallNumWidth+3),kj_small_haoma_y+ cnt * (m_smallNumHeight + kj_small_haoma_row_span),m_kjNumSmall1->GetWidth()/80,m_kjNumSmall1->GetHeight()),
			Sumall-1,m_kjNumSmall1->GetWidth()/80,m_kjNumSmall1->GetHeight());

	}

	pDC->DrawText(strText, &rcZongJinE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}


void CXingYun28::DrawZongZhushuJinE(CDC* pDC)
{
	return;
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_zongFont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(160, 8, 8));
//	strText.Format(_T("你选择了%d注"), m_singlezhushu);

	CRect rcZhu;
// 	rcZhu.CopyRect(rcZongZhuShu);
// 	rcZhu.left += 225;
// 	// 	rcZhu.top -= 17;
// 	// 	rcZhu.bottom -= 17;
// 	rcZhu.right += 475;
// 	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 
// 
// 	CRect rcRect;

// 	rcRect.left = rcZongZhuShu.left+27;
// 	rcRect.right = rcZongZhuShu.right+27;
// 	rcRect.top = rcZongZhuShu.top+15;
// 	rcRect.bottom = rcZongZhuShu.bottom+15;
// 	strText.Format(L"倍数：");
// 	pDC->DrawText(strText, &rcRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
// 	double danzhujine = 2.0;
// 
// 	if(theAccount.Scoretype == MoShi_Yuan)
// 	{	
// 		danzhujine = 2.0;
// 	}
// 	else if (theAccount.Scoretype == MoShi_Jiao)
// 	{
// 		danzhujine = 0.2;
// 	}
// 	else if(theAccount.Scoretype == MoShi_Fen)
// 	{
// 		danzhujine = 0.02;
// 	}
// 	else if(theAccount.Scoretype == MoShi_Li)
// 		danzhujine = 0.002;
// 
// 	strText.Format(_T("共:%.3lf元"), m_singlezhushu * m_beishu * danzhujine);
// 	pDC->DrawText(strText, &rcZongJinE, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 
// 	strText.Format(_T("共计%d注"),m_nZongZhuShu);
// 	pDC->DrawText(strText, &rcAllZongzhushu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	rcZhu.CopyRect(rcAllZongzhushu);
	rcZhu.top += 40;
	rcZhu.bottom += 48;
	strText.Format(_T("%.3lf"),m_zongJine);
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}

long CXingYun28::GetKjShjDiff()
{
	long secDiff = m_chqRule.GetKjShjDiff();

	if (secDiff <= 60 )
	{
		return 0;
	}
	else if(secDiff > (m_chqRule.GetQiSpan() ))
	{
		m_bKaiJiangzhong=true;
		return 0;
	}
	return secDiff;
}

void CXingYun28::DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num,int nWidth,int nHeight)
{
	graphics.DrawImage(img, rc_dest, num*nWidth, 0, nWidth, nHeight, UnitPixel);
}

void CXingYun28::DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_smallNumWidth, 0, m_smallNumWidth, m_smallNumHeight, UnitPixel);
}

//鼠标输入
void CXingYun28::OnBnClickedRioMouseInput()
{
	OnBnClickedBtnClsList();
	if(m_gameKind == enBJK8_1)
	{
		OnBnClinckedBtnRenXuan();
	}
	else if(m_gameKind == enXY28_TeMa)
	{
		OnBnClinckedBtnTeMa();
	}


	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);
	ResetAllNums();
	m_rioMouseInput.SetPushed(true);
	m_rioKeyboardInput.SetPushed(false);

	HideDanShiEdit();
}

//键盘输入（任选2，任选3，任选4）
void CXingYun28::OnBnClickedRioKeyboardInput()
{
	OnBnClickedBtnClsList();
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);


	HideHeZhiNums();
	HideBJNums();

	ResetAllNums();
	m_rioMouseInput.SetPushed(false);
	m_rioKeyboardInput.SetPushed(true);


	ShowDanShiEdit();
}
void CXingYun28::ShowCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_SHOW);
	}
}
void CXingYun28::HideCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}
void CXingYun28::CheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(true);
}
void CXingYun28::UnCheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(false);
}
bool CXingYun28::IsNumBtnCheck(CNumerButton& btn)
{
	return btn.IsSelect();
}

void CXingYun28::ResetAllNums(void)
{
	m_singlezhushu = 0;
	UnCheckNumBtn(m_btnHeZhi_0);
	UnCheckNumBtn(m_btnHeZhi_1);
	UnCheckNumBtn(m_btnHeZhi_2);
	UnCheckNumBtn(m_btnHeZhi_3);
	UnCheckNumBtn(m_btnHeZhi_4);
	UnCheckNumBtn(m_btnHeZhi_5);
	UnCheckNumBtn(m_btnHeZhi_6);
	UnCheckNumBtn(m_btnHeZhi_7);
	UnCheckNumBtn(m_btnHeZhi_8);
	UnCheckNumBtn(m_btnHeZhi_9);
	UnCheckNumBtn(m_btnHeZhi_10);
	UnCheckNumBtn(m_btnHeZhi_11);
	UnCheckNumBtn(m_btnHeZhi_12);
	UnCheckNumBtn(m_btnHeZhi_13);
	UnCheckNumBtn(m_btnHeZhi_14);
	UnCheckNumBtn(m_btnHeZhi_15);
	UnCheckNumBtn(m_btnHeZhi_16);
	UnCheckNumBtn(m_btnHeZhi_17);
	UnCheckNumBtn(m_btnHeZhi_18);
	UnCheckNumBtn(m_btnHeZhi_19);
	UnCheckNumBtn(m_btnHeZhi_20);
	UnCheckNumBtn(m_btnHeZhi_21);
	UnCheckNumBtn(m_btnHeZhi_22);
	UnCheckNumBtn(m_btnHeZhi_23);
	UnCheckNumBtn(m_btnHeZhi_24);
	UnCheckNumBtn(m_btnHeZhi_25);
	UnCheckNumBtn(m_btnHeZhi_26);
	UnCheckNumBtn(m_btnHeZhi_27);


	UnCheckNumBtn(m_btnDa);
	UnCheckNumBtn(m_btnXiao);
	UnCheckNumBtn(m_btnDan);
	UnCheckNumBtn(m_btnShuang);
	UnCheckNumBtn(m_btnDaDan);
	UnCheckNumBtn(m_btnDaShuang);
	UnCheckNumBtn(m_btnXiaoDan);
	UnCheckNumBtn(m_btnXiaoShuang);

	UnCheckNumBtn(m_btnMax);
	UnCheckNumBtn(m_btnMin);

}

//隐藏玩法
void CXingYun28::HideRio()
{
	m_rioDuiZi.ShowWindow(SW_HIDE);
	m_rioShunZi.ShowWindow(SW_HIDE);
	m_rioBaoZi.ShowWindow(SW_HIDE);
}
//选择玩法
void CXingYun28::SelectRio(CImgRioButton &rio)
{
	m_rioDuiZi.SetPushed(false);
	m_rioShunZi.SetPushed(false);
	m_rioBaoZi.SetPushed(false);

	rio.SetPushed(true);
}


//显示隐藏极值
void CXingYun28::HideJiZhi()
{
	m_btnMax.ShowWindow(SW_HIDE);
	m_btnMin.ShowWindow(SW_HIDE);
}
void CXingYun28::ShowJiZji()
{
	m_btnMax.ShowWindow(SW_SHOW);
	m_btnMin.ShowWindow(SW_SHOW);
}



void CXingYun28::ShowDanShiEdit()
{
	m_RenxuanKeyboardInput = true;
	m_richDanshiHaoma.ShowWindow(SW_SHOW);
}
void CXingYun28::HideDanShiEdit()
{
	m_RenxuanKeyboardInput = false;
	m_richDanshiHaoma.ShowWindow(SW_HIDE);
}

void CXingYun28::ShowDaXiaoDanShuang()
{
	m_btnDa.ShowWindow(SW_SHOW);
	m_btnXiao.ShowWindow(SW_SHOW);
	m_btnDan.ShowWindow(SW_SHOW);
	m_btnShuang.ShowWindow(SW_SHOW);
	m_btnDaDan.ShowWindow(SW_SHOW);
	m_btnDaShuang.ShowWindow(SW_SHOW);
	m_btnXiaoDan.ShowWindow(SW_SHOW);
	m_btnXiaoShuang.ShowWindow(SW_SHOW);
}

void CXingYun28::HideDaXiaoDanShuang()
{
	m_btnDa.ShowWindow(SW_HIDE);
	m_btnXiao.ShowWindow(SW_HIDE);
	m_btnDan.ShowWindow(SW_HIDE);
	m_btnShuang.ShowWindow(SW_HIDE);
	m_btnDaDan.ShowWindow(SW_HIDE);
	m_btnDaShuang.ShowWindow(SW_HIDE);
	m_btnXiaoDan.ShowWindow(SW_HIDE);
	m_btnXiaoShuang.ShowWindow(SW_HIDE);
}
void CXingYun28::ShowBJNums()
{
}
void CXingYun28::HideBJNums()
{


}
void CXingYun28::HideHeZhiNums()
{
	m_btnHeZhi_0.ShowWindow(SW_HIDE);
	m_btnHeZhi_1.ShowWindow(SW_HIDE);
	m_btnHeZhi_2.ShowWindow(SW_HIDE);
	m_btnHeZhi_3.ShowWindow(SW_HIDE);
	m_btnHeZhi_4.ShowWindow(SW_HIDE);
	m_btnHeZhi_5.ShowWindow(SW_HIDE);
	m_btnHeZhi_6.ShowWindow(SW_HIDE);
	m_btnHeZhi_7.ShowWindow(SW_HIDE);
	m_btnHeZhi_8.ShowWindow(SW_HIDE);
	m_btnHeZhi_9.ShowWindow(SW_HIDE);
	m_btnHeZhi_10.ShowWindow(SW_HIDE);
	m_btnHeZhi_11.ShowWindow(SW_HIDE);
	m_btnHeZhi_12.ShowWindow(SW_HIDE);
	m_btnHeZhi_13.ShowWindow(SW_HIDE);
	m_btnHeZhi_14.ShowWindow(SW_HIDE);
	m_btnHeZhi_15.ShowWindow(SW_HIDE);
	m_btnHeZhi_16.ShowWindow(SW_HIDE);
	m_btnHeZhi_17.ShowWindow(SW_HIDE);
	m_btnHeZhi_18.ShowWindow(SW_HIDE);
	m_btnHeZhi_19.ShowWindow(SW_HIDE);
	m_btnHeZhi_20.ShowWindow(SW_HIDE);
	m_btnHeZhi_21.ShowWindow(SW_HIDE);
	m_btnHeZhi_22.ShowWindow(SW_HIDE);
	m_btnHeZhi_23.ShowWindow(SW_HIDE);
	m_btnHeZhi_24.ShowWindow(SW_HIDE);
	m_btnHeZhi_25.ShowWindow(SW_HIDE);
	m_btnHeZhi_26.ShowWindow(SW_HIDE);
	m_btnHeZhi_27.ShowWindow(SW_HIDE);
}
void CXingYun28::ShowHezhiNums()
{
	m_btnHeZhi_0.ShowWindow(SW_SHOW);
	m_btnHeZhi_1.ShowWindow(SW_SHOW);
	m_btnHeZhi_2.ShowWindow(SW_SHOW);
	m_btnHeZhi_3.ShowWindow(SW_SHOW);
	m_btnHeZhi_4.ShowWindow(SW_SHOW);
	m_btnHeZhi_5.ShowWindow(SW_SHOW);
	m_btnHeZhi_6.ShowWindow(SW_SHOW);
	m_btnHeZhi_7.ShowWindow(SW_SHOW);
	m_btnHeZhi_8.ShowWindow(SW_SHOW);
	m_btnHeZhi_9.ShowWindow(SW_SHOW);
	m_btnHeZhi_10.ShowWindow(SW_SHOW);
	m_btnHeZhi_11.ShowWindow(SW_SHOW);
	m_btnHeZhi_12.ShowWindow(SW_SHOW);
	m_btnHeZhi_13.ShowWindow(SW_SHOW);
	m_btnHeZhi_14.ShowWindow(SW_SHOW);
	m_btnHeZhi_15.ShowWindow(SW_SHOW);
	m_btnHeZhi_16.ShowWindow(SW_SHOW);
	m_btnHeZhi_17.ShowWindow(SW_SHOW);
	m_btnHeZhi_18.ShowWindow(SW_SHOW);
	m_btnHeZhi_19.ShowWindow(SW_SHOW);
	m_btnHeZhi_20.ShowWindow(SW_SHOW);
	m_btnHeZhi_21.ShowWindow(SW_SHOW);
	m_btnHeZhi_22.ShowWindow(SW_SHOW);
	m_btnHeZhi_23.ShowWindow(SW_SHOW);
	m_btnHeZhi_24.ShowWindow(SW_SHOW);
	m_btnHeZhi_25.ShowWindow(SW_SHOW);
	m_btnHeZhi_26.ShowWindow(SW_SHOW);
	m_btnHeZhi_27.ShowWindow(SW_SHOW);
}


//调整 单选框
void CXingYun28::AdjustRadio()
{
	CWnd *pWnd;


	pWnd = GetDlgItem(IDC_RIO_DUIZI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x,				wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_SHUNZI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_BAOZI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*2, wanfa_sel_y,wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}









	pWnd = GetDlgItem(IDC_RIO_MOUSE_INPUT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, mouseinput_x, mouseinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_KEYBOARD_INPUT);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL,keyboardinput_x, keyboardinput_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}




}
//玩法按钮调整
void CXingYun28::AdjustWanFa()
{
	if (m_btnRenXuan.GetSafeHwnd() != NULL) 
	{
		m_btnRenXuan.SetWindowPos(NULL, wanfa_x, wanfa_y, m_btnRenXuan.Width(), m_btnRenXuan.Height(), SWP_NOZORDER);
		m_btnRenXuan.ShowWindow(SW_HIDE);
	}
	if (m_btnTeMa.GetSafeHwnd() != NULL) 
	{
		m_btnTeMa.SetWindowPos(NULL, wanfa_x , wanfa_y , m_btnTeMa.Width(), m_btnTeMa.Height(), SWP_NOZORDER);
		m_btnTeMa.ShowWindow(SW_HIDE);
	}
	if (m_btnDaXiaoDanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnDaXiaoDanShuang.SetWindowPos(NULL, wanfa_x + wanfa_col_width*1, wanfa_y, m_btnDaXiaoDanShuang.Width(), m_btnDaXiaoDanShuang.Height(), SWP_NOZORDER);
		m_btnDaXiaoDanShuang.ShowWindow(SW_HIDE);
	}
	if (m_btnJiZhi.GetSafeHwnd() != NULL) 
	{
		m_btnJiZhi.SetWindowPos(NULL, wanfa_x+wanfa_col_width*2+16, wanfa_y, m_btnJiZhi.Width(), m_btnJiZhi.Height(), SWP_NOZORDER);
		m_btnJiZhi.ShowWindow(SW_HIDE);
	}
	if (m_btnFunny.GetSafeHwnd() != NULL) 
	{
		m_btnFunny.SetWindowPos(NULL, wanfa_x+wanfa_col_width*3+16, wanfa_y, m_btnFunny.Width(), m_btnFunny.Height(), SWP_NOZORDER);
		m_btnFunny.ShowWindow(SW_HIDE);
	}

}

//按钮调整：删除，清零，倍投，投注，追号
void CXingYun28::AdjustNumView()
{
	if(m_btnBeiTouJia.GetSafeHwnd() != NULL) 
	{
		m_btnBeiTouJia.SetWindowPos(NULL,edit_beitou_x+edit_beitou_width , edit_beitou_y-8, m_btnBeiTouJia.Width(), m_btnBeiTouJia.Height(), SWP_NOZORDER);
		m_btnBeiTouJia.ShowWindow(SW_HIDE);
	}
	if(m_btnBeiTouJian.GetSafeHwnd() != NULL) 
	{
		m_btnBeiTouJian.SetWindowPos(NULL,edit_beitou_x-m_btnBeiTouJian.Width() , edit_beitou_y-8, m_btnBeiTouJian.Width(), m_btnBeiTouJian.Height(), SWP_NOZORDER);
		m_btnBeiTouJian.ShowWindow(SW_HIDE);
	}

	if(m_btnDelSel.GetSafeHwnd() != NULL) 
	{
		m_btnDelSel.SetWindowPos(NULL, btn_del_sel_x, btn_del_sel_y, m_btnDelSel.Width(), m_btnDelSel.Height(), SWP_NOZORDER);
		m_btnDelSel.ShowWindow(SW_HIDE);
	}

	if(m_btnClsList.GetSafeHwnd() != NULL) 
	{
		m_btnClsList.SetWindowPos(NULL, btn_cls_list_x, btn_cls_list_y, m_btnClsList.Width(), m_btnClsList.Height(), SWP_NOZORDER);
		m_btnClsList.ShowWindow(SW_HIDE);
	}

	if (m_listChqSSC.GetSafeHwnd() != NULL) 
	{
		m_listChqSSC.SetWindowPos(NULL, list_haomao_x, list_haomao_y, list_haomao_width, list_haomao_height, SWP_NOZORDER);
		m_listChqSSC.ShowWindow(SW_HIDE);
	}

	if(m_editBeiTou.GetSafeHwnd() != NULL) 
	{
		m_editBeiTou.SetWindowPos(NULL, edit_beitou_x, edit_beitou_y, edit_beitou_width, edit_beitou_height, SWP_NOZORDER);
		//m_editBeiTou.ShowWindow(SW_HIDE);
	}

	if(m_btnTouZhu.GetSafeHwnd() != NULL) 
	{
		m_btnTouZhu.SetWindowPos(NULL, btn_touzhu_x, btn_touzhu_y, m_btnTouZhu.Width(), m_btnTouZhu.Height(), SWP_NOZORDER);
	}

	if(m_btnZhuiHao.GetSafeHwnd() != NULL) 
	{
		m_btnZhuiHao.SetWindowPos(NULL, btn_zhuihao_x, btn_zhuihao_y, m_btnZhuiHao.Width(), m_btnZhuiHao.Height(), SWP_NOZORDER);
		m_btnZhuiHao.ShowWindow(SW_HIDE);
	}
}
//单式调整
void CXingYun28::AdjustDanShi()
{
	if (m_richDanshiHaoma.GetSafeHwnd() != NULL) 
	{
		m_richDanshiHaoma.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y, edit_haomao_danshi_width, edit_haomao_danshi_height, SWP_NOZORDER);
		m_richDanshiHaoma.ShowWindow(SW_HIDE);
	}
}

//加注按钮
void CXingYun28::AdjustAdd()
{
	if (m_btnChqAdd.GetSafeHwnd() != NULL) 
	{
		m_btnChqAdd.SetWindowPos(NULL, btn_chq_add_x, btn_chq_add_y, m_btnChqAdd.Width(), m_btnChqAdd.Height(), SWP_NOZORDER);
		m_btnChqAdd.ShowWindow(SW_HIDE);
	}

	if (m_btnLock.GetSafeHwnd() != NULL) 
	{
		m_btnLock.SetWindowPos(NULL, btn_lock_x, btn_lock_y, m_btnLock.Width(), m_btnLock.Height(), SWP_NOZORDER);
		m_btnLock.ShowWindow(SW_HIDE);
	}
	if(m_btnYuan.GetSafeHwnd() != NULL)
	{
		m_btnYuan.SetWindowPos(NULL,btn_yuan_x,btn_yuan_y,m_btnYuan.Width(),m_btnYuan.Height(),SWP_NOZORDER);
		m_btnYuan.ShowWindow(SW_HIDE);
	}
	if(m_btnJiao.GetSafeHwnd() != NULL)
	{
		m_btnJiao.SetWindowPos(NULL,btn_jiao_x,btn_jiao_y,m_btnJiao.Width(),m_btnJiao.Height(),SWP_NOZORDER);
		m_btnJiao.ShowWindow(SW_HIDE);
	}
	if(m_btnFen.GetSafeHwnd() != NULL)
	{
		m_btnFen.SetWindowPos(NULL,btn_fen_x,btn_fen_y,m_btnFen.Width(),m_btnFen.Height(),SWP_NOZORDER);
		m_btnFen.ShowWindow(SW_HIDE);
	}
	if(m_btnLi.GetSafeHwnd() != NULL)
	{
		m_btnLi.SetWindowPos(NULL,btn_li_x,btn_li_y,m_btnLi.Width(),m_btnLi.Height(),SWP_NOZORDER);
		m_btnLi.ShowWindow(SW_HIDE);
	}

}

//投注框：选择数字，5位数字，0--9的选择
void CXingYun28::AdjustNumBtn()
{
	int n_row_span_y = 30;
	//和值------------------------------
	if(m_btnHeZhi_0.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_0.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_0.Width(), m_btnHeZhi_0.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_1.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_1.Width(), m_btnHeZhi_1.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_2.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_2.Width(), m_btnHeZhi_2.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_3.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_3.Width(), m_btnHeZhi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_4.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_4.Width(), m_btnHeZhi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_5.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_5.Width(), m_btnHeZhi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_6.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_6.Width(), m_btnHeZhi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_7.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_7.Width(), m_btnHeZhi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_8.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_8.Width(), m_btnHeZhi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_9.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_9.Width(), m_btnHeZhi_9.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_10.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_10.Width(), m_btnHeZhi_10.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_11.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_11.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_11.Width(), m_btnHeZhi_11.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_12.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_12.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_12.Width(), m_btnHeZhi_12.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_13.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_13.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y+haoma_btn_row_height, m_btnHeZhi_13.Width(), m_btnHeZhi_13.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_14.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_14.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_14.Width(), m_btnHeZhi_14.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_15.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_15.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_15.Width(), m_btnHeZhi_15.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_16.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_16.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_16.Width(), m_btnHeZhi_16.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_17.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_17.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_3.Width(), m_btnHeZhi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_18.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_18.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_4.Width(), m_btnHeZhi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_19.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_19.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_5.Width(), m_btnHeZhi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_20.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_20.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_6.Width(), m_btnHeZhi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_21.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_21.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_7.Width(), m_btnHeZhi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_22.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_22.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_8.Width(), m_btnHeZhi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_23.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_23.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_9.Width(), m_btnHeZhi_9.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_24.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_24.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_10.Width(), m_btnHeZhi_10.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_25.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_25.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_11.Width(), m_btnHeZhi_11.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_26.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_26.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_12.Width(), m_btnHeZhi_12.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_27.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_27.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y+haoma_btn_row_height*2+n_row_span_y, m_btnHeZhi_13.Width(), m_btnHeZhi_13.Height(), SWP_NOZORDER);
	}


	//大小单双
	if(m_btnDa.GetSafeHwnd() != NULL) 
	{
		m_btnDa.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*3, m_btnDa.Width(), m_btnDa.Height(), SWP_NOZORDER);
	}
	if(m_btnXiao.GetSafeHwnd() != NULL) 
	{
		m_btnXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*3, m_btnXiao.Width(), m_btnXiao.Height(), SWP_NOZORDER);
	}
	if(m_btnDan.GetSafeHwnd() != NULL) 
	{
		m_btnDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*3, m_btnDan.Width(), m_btnDan.Height(), SWP_NOZORDER);
	}
	if(m_btnShuang.GetSafeHwnd() != NULL) 
	{
		m_btnShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*3, m_btnShuang.Width(), m_btnShuang.Height(), SWP_NOZORDER);
	}
	if(m_btnDaDan.GetSafeHwnd() != NULL) 
	{
		m_btnDaDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height*3, m_btnDaDan.Width(), m_btnDaDan.Height(), SWP_NOZORDER);
	}
	if(m_btnDaShuang.GetSafeHwnd() != NULL) 
	{
		m_btnDaShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*3, m_btnDaShuang.Width(), m_btnDaShuang.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoDan.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*3, m_btnXiaoDan.Width(), m_btnXiaoDan.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoShuang.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height*3, m_btnXiaoShuang.Width(), m_btnXiaoShuang.Height(), SWP_NOZORDER);
	}



	if(m_btnMax.GetSafeHwnd() != NULL) 
	{
		m_btnMax.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*3, m_btnMax.Width(), m_btnMax.Height(), SWP_NOZORDER);
	}
	if(m_btnMin.GetSafeHwnd() != NULL) 
	{
		m_btnMin.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*3, m_btnMin.Width(), m_btnMin.Height(), SWP_NOZORDER);
	}
}





BOOL CXingYun28::OnInitDialog()
{
	CDialog::OnInitDialog();
	ModifyStyle(WS_CLIPCHILDREN,0);
	//ZeroMemory(m_szMD5Haoma,sizeof(m_szMD5Haoma));
	hBrush = NULL;
	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\game_bk4.png"));
	m_numImageList = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num.png"));
	m_numImageList1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num2.png"));
	m_numImageList2 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num3.png"));
	m_numImageList3 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num5.png"));
	m_kjNumBig = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big6.png"));
	m_kjNumBig2 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big6.png"));
	m_kjAdd = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Sign.png"));
	m_kjAdd1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Sign1.png"));
	m_imgKaijiang = new Bitmap( CBmpUtil::GetExePath() + _T("skin\\game\\xy28_kaijiang.gif"));
	int nCount = m_imgKaijiang->GetFrameDimensionsCount();			//获取帧维数
	GUID *pGuids = new GUID[nCount];								//定义一个GUID数组
	m_imgKaijiang->GetFrameDimensionsList(pGuids,nCount);			//获取图像帧的GUID
	m_nFrameCount=m_imgKaijiang->GetFrameCount(pGuids);				//获取GIF帧数
	m_bZhuiZjtz = true;
	m_btnYuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_1.png"));
	m_btnJiao.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_2.png"));
	m_btnFen.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_3.png"));
	m_btnLi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_4.png"));

	m_bShowFail = true;
	m_bigNumWidth = m_kjNumBig2->GetWidth() / 28;
	m_bigNumHeight = m_kjNumBig2->GetHeight();
	m_kjNumSmall = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small2.png"));
	m_kjNumSmall1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small3.png"));
	m_smallNumWidth = m_kjNumSmall->GetWidth() / 10;
	m_smallNumHeight = m_kjNumSmall->GetHeight();
	m_bmpDxdsh = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num6.png"));
	m_bmpNumTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit.png"));
	m_bmpNumTip1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit2.png"));

	m_bCanSend=true;
	InitNumberBtns();
	InitListCtrl();

	m_smallNumfont.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_zongFont.CreateFont(30, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_KjInfoFont1.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_KjInfoFont.CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	m_btnDelSel.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_delate.png"));
	m_btnClsList.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_clear.png"));
	m_btnTouZhu.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_bet_xy28.png"));
	m_btnZhuiHao.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_append.png"));
	m_btnChqAdd.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_addnum.png"));
	m_btnLock.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_lock.png"));
	m_btnLock.SetTextColor(RGB(51, 45, 42));
	m_btnBeiTouJia.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jia.png"));
	m_btnBeiTouJian.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jian.png"));
	m_btnLock.SetTextFont(&m_zongFont);
	m_editBeiTou.SetEnableColor(RGB(250,250,250),RGB(163,153,147),RGB(163,153,147));
	m_editBeiTou.SetFont(&m_zongFont);

	m_ImageItemBackMid.LoadImage(AfxGetInstanceHandle(),TEXT("GAME_ITEM_BACK_MID"));

	m_richDanshiHaoma.LimitText(-1);
	m_richDanshiHaoma.SetBackgroundColor(false,RGB(200,200,200));
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(88, 78, 77);
	m_richDanshiHaoma.SetDefaultCharFormat(cf);




	m_bKaiJiangzhong=false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CXingYun28::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CXingYun28::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
//初始化号码按钮
void CXingYun28::InitNumberBtns()
{
	m_btnHeZhi_0.SetImage(m_numImageList1, 0, 28);
	m_btnHeZhi_1.SetImage(m_numImageList1, 1, 28);
	m_btnHeZhi_2.SetImage(m_numImageList1, 2, 28);
	m_btnHeZhi_3.SetImage(m_numImageList1, 3, 28);
	m_btnHeZhi_4.SetImage(m_numImageList1, 4, 28);
	m_btnHeZhi_5.SetImage(m_numImageList1, 5, 28);
	m_btnHeZhi_6.SetImage(m_numImageList1, 6, 28);
	m_btnHeZhi_7.SetImage(m_numImageList1, 7, 28);
	m_btnHeZhi_8.SetImage(m_numImageList1, 8, 28);
	m_btnHeZhi_9.SetImage(m_numImageList1, 9, 28);
	m_btnHeZhi_10.SetImage(m_numImageList1, 10, 28);
	m_btnHeZhi_11.SetImage(m_numImageList1, 11, 28);
	m_btnHeZhi_12.SetImage(m_numImageList1, 12, 28);
	m_btnHeZhi_13.SetImage(m_numImageList1, 13, 28);
	m_btnHeZhi_14.SetImage(m_numImageList1, 14, 28);
	m_btnHeZhi_15.SetImage(m_numImageList1, 15, 28);
	m_btnHeZhi_16.SetImage(m_numImageList1, 16, 28);
	m_btnHeZhi_17.SetImage(m_numImageList1, 17, 28);
	m_btnHeZhi_18.SetImage(m_numImageList1, 18, 28);
	m_btnHeZhi_19.SetImage(m_numImageList1, 19, 28);
	m_btnHeZhi_20.SetImage(m_numImageList1, 20, 28);
	m_btnHeZhi_21.SetImage(m_numImageList1, 21, 28);
	m_btnHeZhi_22.SetImage(m_numImageList1, 22, 28);
	m_btnHeZhi_23.SetImage(m_numImageList1, 23, 28);
	m_btnHeZhi_24.SetImage(m_numImageList1, 24, 28);
	m_btnHeZhi_25.SetImage(m_numImageList1, 25, 28);
	m_btnHeZhi_26.SetImage(m_numImageList1, 26, 28);
	m_btnHeZhi_27.SetImage(m_numImageList1, 27, 28);


	m_btnDa.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiao.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDan.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuang.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaDan.SetImage(m_bmpDxdsh, 4, 10);
	m_btnXiaoDan.SetImage(m_bmpDxdsh, 5, 10);
	m_btnDaShuang.SetImage(m_bmpDxdsh, 6, 10);
	m_btnXiaoShuang.SetImage(m_bmpDxdsh, 7, 10);

	m_btnMax.SetImage(m_bmpDxdsh, 8, 10);
	m_btnMin.SetImage(m_bmpDxdsh, 9, 10);

}

void CXingYun28::InitListCtrl()
{
	m_listChqSSC.SetExtendedStyle(m_listChqSSC.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);
	m_listChqSSC.ModifyStyle(0, LVS_NOCOLUMNHEADER);

	m_listChqSSC.SetPenColor(RGB(247,233,216));
	m_listChqSSC.InsertColumn(0, _T("号码"), LVCFMT_CENTER, 313);
	m_listChqSSC.InsertColumn(1, _T("模式"), LVCFMT_CENTER, 80);
	m_listChqSSC.InsertColumn(2, _T("注数"), LVCFMT_CENTER, 70);
	m_listChqSSC.InsertColumn(3, _T("倍数"), LVCFMT_CENTER, 70);
	m_listChqSSC.InsertColumn(4, _T(""), LVCFMT_CENTER, 30);

}


CString CXingYun28::GetBjNum()//获取北京快8的按钮数值
{
	CString str;
	return str;
}
CString CXingYun28::GetHezhiString()//获取幸运28数值
{
	CString str;
	if (IsNumBtnCheck(m_btnHeZhi_0))
	{
		str += _T("00");
	}
	if (IsNumBtnCheck(m_btnHeZhi_1))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnHeZhi_2))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnHeZhi_3))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnHeZhi_4))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnHeZhi_5))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnHeZhi_6))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnHeZhi_7))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnHeZhi_8))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnHeZhi_9))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnHeZhi_10))
	{
		str += _T("10");
	}
	if (IsNumBtnCheck(m_btnHeZhi_11))
	{
		str += _T("11");
	}
	if (IsNumBtnCheck(m_btnHeZhi_12))
	{
		str += _T("12");
	}
	if (IsNumBtnCheck(m_btnHeZhi_13))
	{
		str += _T("13");
	}
	if (IsNumBtnCheck(m_btnHeZhi_14))
	{
		str += _T("14");
	}
	if (IsNumBtnCheck(m_btnHeZhi_15))
	{
		str += _T("15");
	}
	if (IsNumBtnCheck(m_btnHeZhi_16))
	{
		str += _T("16");
	}
	if (IsNumBtnCheck(m_btnHeZhi_17))
	{
		str += _T("17");
	}
	if (IsNumBtnCheck(m_btnHeZhi_18))
	{
		str += _T("18");
	}
	if (IsNumBtnCheck(m_btnHeZhi_19))
	{
		str += _T("19");
	}
	if (IsNumBtnCheck(m_btnHeZhi_20))
	{
		str += _T("20");
	}
	if (IsNumBtnCheck(m_btnHeZhi_21))
	{
		str += _T("21");
	}
	if (IsNumBtnCheck(m_btnHeZhi_22))
	{
		str += _T("22");
	}
	if (IsNumBtnCheck(m_btnHeZhi_23))
	{
		str += _T("23");
	}
	if (IsNumBtnCheck(m_btnHeZhi_24))
	{
		str += _T("24");
	}
	if (IsNumBtnCheck(m_btnHeZhi_25))
	{
		str += _T("25");
	}
	if (IsNumBtnCheck(m_btnHeZhi_26))
	{
		str += _T("26");
	}
	if (IsNumBtnCheck(m_btnHeZhi_27))
	{
		str += _T("27");
	}
	return str;
}
CString CXingYun28::GetJiZhiString()
{
	CString strGe;
	if (IsNumBtnCheck(m_btnMax)) 
	{
		strGe += _T("18");
	}
	if (IsNumBtnCheck(m_btnMin)) 
	{
		strGe += _T("17");
	}

	return strGe;
}

CString CXingYun28::GetDxdshString()
{
	CString strGe;
	if (IsNumBtnCheck(m_btnDa)) 
	{
		strGe += _T("9,");
	}
	if (IsNumBtnCheck(m_btnXiao)) 
	{
		strGe += _T("1,");
	}
	if (IsNumBtnCheck(m_btnDan)) 
	{
		strGe += _T("3,");
	}
	if (IsNumBtnCheck(m_btnShuang)) 
	{
		strGe += _T("2,");
	}
	if (IsNumBtnCheck(m_btnDaDan)) 
	{
		strGe += _T("12,");
	}
	if (IsNumBtnCheck(m_btnXiaoDan)) 
	{
		strGe += _T("10,");
	}
	if (IsNumBtnCheck(m_btnDaShuang)) 
	{
		strGe += _T("13,");
	}
	if (IsNumBtnCheck(m_btnXiaoShuang)) 
	{
		strGe += _T("11,");
	}


	return strGe;
}
CString CXingYun28::GetFunnyDesc(int nNum)
{
	CString strDesc;
	switch(nNum)
	{
	case 14:
		{
			strDesc = _T("对子");
			break;
		}
	case 15:
		{
			strDesc = _T("顺子");
			break;
		}
	case 16:
		{
			strDesc = _T("豹子");
			break;
		}
	}
	return strDesc;
}
CString CXingYun28::GetDxdshStringDesc(int nNum)
{
	CString strDesc;
	switch (nNum)
	{
	case 1:
		{
			strDesc = _T("小");
			break;
		}
	case 2:
		{
			strDesc = _T("双");
			break;
		}
	case 3:
		{
			strDesc = _T("单");
			break;
		}
	case 9:
		{
			strDesc = _T("大");
			break;
		}
	case 10:
		{
			strDesc = _T("小单");
			break;
		}
	case 11:
		{
			strDesc = _T("小双");
			break;
		}
	case 12:
		{
			strDesc = _T("大单");
			break;
		}
	case 13:
		{
			strDesc = _T("大双");
			break;
		}
	case 17:
		{
			strDesc = _T("极小");
			break;
		}
	case 18:
		{
			strDesc = _T("极大");
			break;
		}
	}
	return strDesc;
}

CString CXingYun28::GetWanDesc()
{
	return theGameKind[(CZXingYun28 << 16) | (unsigned short)m_gameKind];
}

//服务器端返回前N个开奖号码
bool CXingYun28::GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nIndex)
{
	_tcscpy_s(m_kjXinxi[nIndex].qihao, KJ_QIHAO_LENGTH, pResult->wPeriod);
	_tcscpy_s(m_kjXinxi[nIndex].haoma, KJ_HAOMA_LENGTH, pResult->szLotNum);
	_tcscpy_s(m_kjXinxi[nIndex].shijian, KJ_SHIJIAN_LENGTH, pResult->szShijian);

	if(nIndex+1 < TOP_KAIJIANG_NUMBERS)
		return true;

	m_kjXinxiCont = TOP_KAIJIANG_NUMBERS;
	if(!IsWindowVisible())
		return false;

	//return true;

	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
	{
		//第一次进入
		if(m_lastExpect[0] == 0)
			_tcscpy_s(m_lastExpect, KJ_HAOMA_LENGTH, m_kjXinxi[0].qihao);
		m_bKaiJiangzhong=false;											//开奖结束
		long secDiff = m_chqRule.GetKjShjDiff();

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


void CXingYun28::CheckWanFaBtn(CTextButton& btn)
{
	m_btnRenXuan.SetPushed(false);
	m_btnTeMa.SetPushed(false);
	m_btnDaXiaoDanShuang.SetPushed(false);
	m_btnJiZhi.SetPushed(false);
	m_btnFunny.SetPushed(false);


	btn.SetPushed(true);
}

bool CXingYun28::GetDanShiHaoma(CString& haoma, int size, int nLeast)
{
	CStringArray strArr;
	CString line;
	//m_editDanShiHaoMa.GetWindowText(line);
	m_richDanshiHaoma.GetWindowText(line);

	if(line.IsEmpty())
	{
		MyMessageBox(_T("请输入号码！"));	

		return false;
	}

	CString line1=line;
	CString temp;
	do 
	{
		int n = line1.Find(_T(","));
		if(n == -1)
		{
			n = line1.Find(_T(" "));
		}

		if(n == -1)
		{
			temp = line1;
		}
		else
		{
			temp = line1.Left(n);
		}
		if(temp.GetLength() > size)
		{
			MyMessageBox(_T("您输入的号码太多了"));	
			return false;
		}
		if(n == -1)
		{
			line1.Empty();
		}
		else
		{
			int nAll = line1.GetLength();
			line1 = line1.Right(nAll-n-1);
		}

	} while (!line1.IsEmpty());

	if (!GetHaomaStrs(strArr, line, size,nLeast)) 
	{
		return false;
	}

	int count = strArr.GetCount();
	CString strTemp;
	for(int i=0; i<count; i++)
	{
		strTemp = strArr.GetAt(i);

		int nWeishu = strTemp.GetLength();
		if(nWeishu/2 < nLeast)
		{
			//MyMessageBox(_T("您输入的号码太多了"));	
			return false;

		}
		for(int j = 0; j<strTemp.GetLength();j++)
		{
			haoma += strTemp.GetAt(j);
			if (j%2== 1 && (j!=nWeishu-1))
			{
				haoma += _T(",");
			}

			if(j%2== 1 &&j == nWeishu-1)
			{
				haoma +=_T("|");
			}

		}

	}

	return true;
}

bool CXingYun28::GetHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast)
{
	int len = line.GetLength();
	CString subStr;
	CString strTemp;
	int nTempHaoma[20];
	int nCount = 0;
	ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));


	for (int i=0; i<len; i++) 
	{
		CString str;
		str=line.GetAt(i);
		if (iswdigit(line.GetAt(i))&&!str.IsEmpty()) 
		{
			subStr += line.GetAt(i);
			strTemp+= line.GetAt(i);

			subStr += line.GetAt(++i);
			strTemp+= line.GetAt(i);

			int nTemp = _ttoi(strTemp);
			strTemp.Empty();

			if(nTemp<1||nTemp>80)
			{
				MyMessageBox(L"您的输入有误，数字只能在1～80之间 ");
				return false;
			}

			for (int n = 0;n<nCount;n++)
			{
				if(nTemp == nTempHaoma[n])
				{
					CString strLog;
					strLog.Format(L"%02d 和 %02d重复",nTemp,nTempHaoma[n]);
					MyMessageBox(strLog);
					return false;
				}
			}
			nTempHaoma[nCount++] = nTemp;

			if (subStr.GetLength() > nLeast*2) 
			{
				return false;
			}
		}
		else
		{
			// 			MyMessageBox(L"您的输入有误，输入不需要除数字以外的字符");
			// 			return false;
			nCount=0;

			ZeroMemory(&nTempHaoma,sizeof(nTempHaoma));

			if (subStr.GetLength() > size || subStr.GetLength() == 0 || subStr.GetLength()/2<nLeast) 
			{
				return false;
			}

			//if (subStr.GetLength() == size) 
			{
				strArr.Add(subStr);
				subStr = _T("");
			}
		}
	}

	// 	if (subStr.GetLength() != size && subStr.GetLength() != 0) 
	// 	{
	// 		return false;
	// 	}
	if(( subStr.GetLength() == 0)&&(strArr.GetCount()==0))
	{
		MyMessageBox(_T("请输入号码！"));	

		return false;

	}

	if (subStr.GetLength() != 0) 
	{
		strArr.Add(subStr);
		subStr = _T("");
	}

	return true;
}
LRESULT CXingYun28::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	//OnBnClickedBtnDelSel();
	nSubItem--;

	int zhushu = _ttoi(m_listChqSSC.GetItemText(nItem, 2));
	m_nZongZhuShu -= zhushu;
	m_listChqSSC.deleteItemEx( nItem );
	// 	double danzhujine = 2.0;
	// 
	// 	if(theAccount.Scoretype == MoShi_Yuan)
	// 	{	
	// 		danzhujine = 2.0;
	// 	}
	// 	else if (theAccount.Scoretype == MoShi_Jiao)
	// 	{
	// 		danzhujine = 0.2;
	// 	}
	// 	else if(theAccount.Scoretype == MoShi_Fen)
	// 	{
	// 		danzhujine = 0.02;
	// 	}
	// 	else if(theAccount.Scoretype == MoShi_Li)
	// 		danzhujine = 0.002;
	// 	int nItemCount = m_listChqSSC.GetItemCount();
	// 	m_zongJine = 0.00f;
	// 	for(int i = 0;i < nItemCount;i++)
	// 	{
	// 		CString strBeiSHu = m_listChqSSC.GetItemText(i,3);
	// 		CString strZhuSHu = m_listChqSSC.GetItemText(i,2);
	// 		int beishu = _ttoi(strBeiSHu);
	// 		int zhushu = _ttoi(strZhuSHu);
	// 
	// 
	// 		m_zongJine += zhushu*beishu*danzhujine;
	// 	}
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	// 	rcInvali.CopyRect(rcAllZongzhushu);
	// 	rcInvali.bottom += 20;
	// 	InvalidateRect(&rcInvali);

	//	m_listChqSSC.DeleteItem(nItem);

	if(vecAllNums.size() >0)
	{
		int nSize = vecAllNums.size();
		nSize -=1;

		vector<wstring>::iterator   iter = vecAllNums.begin()+nSize-nItem;

		vecAllNums.erase(iter);
	}



	return 0;
}


BOOL CXingYun28::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	if((nCommandID >= IDC_BTN_WAN_0 && nCommandID <= IDC_BTN_GE_9)||(nCommandID >= IDC_BTN_DA_SHI && nCommandID <= IDC_BTN_WAN_2)||(nCommandID >= IDC_BTN_HEZHI_0 && nCommandID <= IDC_BTN_HEZHI_27)||(nCommandID >= IDC_BTN_DA_WAN && nCommandID <= IDC_BTN_SHUANG_BAI)||(nCommandID >= IDC_BTN_NIU_1 && nCommandID <= IDC_BTN_NIU_10))
	{
		int nZhushu = GetXy28Zhushu(CZXingYun28,m_gameKind);
	}

	return  __super::OnCommand(wParam,lParam);;
}

int CXingYun28::GetXy28Zhushu(int nTypeID,int nKindID)
{
	m_singlezhushu = 0;

	int zhushu = 0;

	if (m_gameKind == enXY28_Funny)
	{
		m_singlezhushu = 1;
	}
	else if (m_gameKind == enXY28_JiZhi)
	{
		CString strShi = GetJiZhiString();

		if (strShi.IsEmpty()) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		int nLength = strShi.GetLength();

		if(nLength>2)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;

		}


		m_singlezhushu = strShi.GetLength();
	}
	else if (m_gameKind == enXY28_DXDS)
	{
		CString strShi = GetDxdshString();

		if (strShi.IsEmpty()) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		bool bCheckDa = IsNumBtnCheck(m_btnDa);
		bool bCheckXiao = IsNumBtnCheck(m_btnXiao);
		bool bCheckDan = IsNumBtnCheck(m_btnDan);
		bool bCheckShuang = IsNumBtnCheck(m_btnShuang);

		bool bChecked[4] = {bCheckDa,bCheckXiao,bCheckDan,bCheckShuang};
		int nFirst = 0;
		int nSecond = 0;
		for (int i = 0;i < CountArray(bChecked);i++)
		{
			if(bChecked[i])
				nFirst++;
		}
		if(nFirst>1)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		bCheckDa = IsNumBtnCheck(m_btnDaDan);
		bCheckXiao = IsNumBtnCheck(m_btnXiaoDan);
		bCheckDan = IsNumBtnCheck(m_btnDaShuang);
		bCheckShuang = IsNumBtnCheck(m_btnXiaoShuang);

		bool bChecked1[4] = {bCheckDa,bCheckXiao,bCheckDan,bCheckShuang};
		for (int i = 0;i < CountArray(bChecked1);i++)
		{
			if(bChecked1[i])
				nSecond++;
		}
		if(nSecond>1)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		zhushu = 0;

		zhushu++;//最后个消除了， 所以+1
		//zhushu = strShi.GetLength();

		m_singlezhushu = zhushu;

	}
	else if(m_gameKind == enXY28_TeMa)//幸运28特码
	{
			CString strNum;
			strNum = GetHezhiString();

			int nCount  = strNum.GetLength();
			if (nCount == 0)
			{
				m_singlezhushu = 0;
				CRect rcInvali;
				rcInvali.CopyRect(rcZongZhuShu);
				rcInvali.right+=400;
				InvalidateRect(&rcInvali);
				InvalidateRect(&rcZongJinE);

				return m_singlezhushu;
			}
			if (nCount > 46)
			{
				m_singlezhushu = 0;
				CRect rcInvali;
				rcInvali.CopyRect(rcZongZhuShu);
				rcInvali.right+=400;
				InvalidateRect(&rcInvali);
				InvalidateRect(&rcZongJinE);

				return m_singlezhushu;
			}

			m_singlezhushu = nCount/2;
	}


	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	return m_singlezhushu;
}
