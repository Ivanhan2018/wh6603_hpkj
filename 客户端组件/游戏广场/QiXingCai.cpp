#include "stdafx.h"
#include "GamePlaza.h"
#include "QiXingCai.h"
#include "BmpUtil.h"
/*#include "LuckMeDlg.h"*/
#include "MessageDlg.h"
#include "DlgAddtional.h"
#include "PlatformFrame.h"
#include "DlgFandian.h"
#include "GDIpng/MemDC.h"
#include <map>
extern map <int,CString> mapFandian;
extern map <int,float> mapPercent;
extern map <int,float> mapDanweiFandian;
extern map <int,float> mapBonus;

extern vector<wstring>	vecData1, vecData2, vecData3;
using		namespace		std;

#define      MAXN			100
extern vector<wstring> vecAllNums;
CString   CQiXingCai::m_strHaoma = _T("");

extern int g_arr[MAXN];
extern int g_nPos[512000][5];
extern int g_Counts;
//得到组合后的
extern	int GetComboArray(int DigitnCount, int nPosCount, int* szInput, int szOutput[10][5]);
extern	int InsertDouhaoAfter(int nDouhaoIndexAfter, wstring&  strLine);
//得到某一行带逗号的字符串
extern	wstring	GetXiaZhuString(int nLines, int nCols, int nIndex, int nValue [10][5], int nInsertCol=100);

extern void      Combo(int m,int k);


//任选？倍数
extern int nRenXuan2[6];
extern int nRenXuan3[6];
extern int nRenXuan4[6];

//玩法分类数据
static const int wanfa_x = 9;
static const int wanfa_y = 109;
static const int wanfa_col_width = 64;
static const int wanfa_row_height = 32;

//玩法子分类数据
static const int wanfa_sel_x = 20;
static const int wanfa_sel_y = 145;
static const int wanfa_sel_width = 102;
static const int wanfa_sel_height = 22;

//号码数据
static const int haoma_btn_x = 83;
static const int haoma_btn_y = 226;
static const int haoma_btn_col_wdith = 40;
static const int haoma_btn_row_height = 35;

//号码提示数据
static const int haoma_tip_x = 20;
static const int haoma_tip_y = haoma_btn_y;

//单式选号编辑框
static const int edit_haomao_danshi_x = 20;
static const int edit_haomao_danshi_y = 210;
static const int edit_haomao_danshi_width = 752;
static const int edit_haomao_danshi_height = 195;

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
static const int edit_beitou_x = 214;
static const int edit_beitou_y = 423;
static const int edit_beitou_width = 48;
static const int edit_beitou_height = 21;
//确认投注按钮
static const int btn_touzhu_x = 638;
static const int btn_touzhu_y = 490;
//追号按钮
static const int btn_zhuihao_x = 719;
static const int btn_zhuihao_y = 415;
//更多记录
static const int btn_more_x = 712;
static const int btn_more_y = 92;

//大开奖号
static const int kj_big_haoma_x = 261;
static const int kj_big_haoma_y = 41;

static const int czh_rc_left = 243;
static const int czh_rc_top = 4;
static const int czh_rc_width = 65;
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
static const int kj_small_haoma_x = 630;
static const int kj_small_haoma_y = 28;
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
static const int allzongzhushu_x = 638;
static const int allzongzhushu_y = 444;
static const int allzongzhushu_width = 150;
static const int allzongzhushu_height = 25;
static CRect rcAllZongzhushu(allzongzhushu_x, allzongzhushu_y, allzongzhushu_x+allzongzhushu_width, allzongzhushu_y+allzongzhushu_height);

//鼠标输入
static const int mouseinput_x = 597;
static const int mouseinput_y = 175;
//键盘输入
static const int keyboardinput_x = 670;
static const int keyboardinput_y = 175;

static const int redraw_rc_left = 0;
static const int redraw_rc_top = 0;
static const int redraw_rc_width = 756;
static const int redraw_rc_height = 103;
static CRect rcRedraw(redraw_rc_left, redraw_rc_top, redraw_rc_left + redraw_rc_width, redraw_rc_top + redraw_rc_height);
static const int btn_yuan_x = 10;
static const int btn_yuan_y = 415;
static const int btn_jiao_x = 45;
static const int btn_jiao_y = 415;
static const int btn_fen_x = 80;
static const int btn_fen_y = 415;
static const int btn_li_x = 115;
static const int btn_li_y = 415;

static const int timer_id_get_luck_num = 1;
static const int timer_id_kj_yugao = 2;

static const int timer_id_kaijiangzhong=3;
//计时
static const int TimerJiShi = 4;
static const int TimerStatus = 6;

//这个是用来本地刷新开奖数据的
static const int TimerZhunBeiData = 5;

static const int TimerFandian = 6;


IMPLEMENT_DYNAMIC(CQiXingCai, CDialog)

CQiXingCai::CQiXingCai(CWnd* pParent /*=NULL*/)
	: CDialog(CQiXingCai::IDD, pParent)
	, m_bmpBk(NULL)
	, m_numImageList(NULL)
	, m_numImageList1(NULL)
	, m_numImageList2(NULL)
	, m_kjNumBig(NULL)
	, m_kjNumSmall(NULL)
	, m_bmpDxdsh(NULL)
	, m_bmpNumTip(NULL)
	, m_bmpNumTip1(NULL)
	, m_bigNumWidth(0)
	, m_bigNumHeight(0)
	, m_smallNumWidth(0)
	, m_smallNumHeight(0)
	, m_zongZhuShu(0)
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
	m_gameKind = QiXingCai_LiangDing;
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
	m_zongJine = 0.0f;
	nBonus = 0.0f;
	m_nTouzhuCount=0;
	m_nZhuihaoCount = 0;
	m_bZhuihao=false;
	//设置组件
	m_DlgStatus.SetStatusViewSink(this);

}
//取消连接
VOID CQiXingCai::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

BOOL CQiXingCai::PreTranslateMessage(MSG* pMsg)
{
	//定义变量
	BOOL  bReturn=TRUE; 
	if(pMsg-> message   ==WM_RBUTTONDOWN) 
	{
		if(pMsg->hwnd == m_richDanshiHaoma.m_hWnd)
		{
			CPoint pt(0,0);
			GetCursorPos(&pt);
			CMenu menu;
			menu.LoadMenu(IDR_MENU_RBTN);
			CMenu* pMenu = menu.GetSubMenu(0);
			if (pMenu != NULL)
			{
				pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
			}

			return true;

		}
	}
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}
	return __super::PreTranslateMessage(pMsg);
}

CQiXingCai::~CQiXingCai()
{
	if (m_bmpBk != NULL) 
	{
		delete m_bmpBk;
	}

	if (m_numImageList != NULL) 
	{
		delete m_numImageList;
	}
	if (m_numImageList1 != NULL) 
	{
		delete m_numImageList1;
	}
	if (m_numImageList2 != NULL) 
	{
		delete m_numImageList2;
	}

	if (m_kjNumBig != NULL) 
	{
		delete m_kjNumBig;
	}

	if (m_kjNumSmall != NULL) 
	{
		delete m_kjNumSmall;
	}

	if (m_imgKaijiang != NULL) 
	{
		delete m_imgKaijiang;
	}

	if (m_bmpDxdsh != NULL) 
	{
		delete m_bmpDxdsh;
	}

	if (m_bmpNumTip != NULL) 
	{
		delete m_bmpNumTip;
	}
	if (m_bmpNumTip1 != NULL) 
	{
		delete m_bmpNumTip1;
	}
}

void CQiXingCai::ConnectMainDlg(CPlazaViewItem* luckMeDlg)
{
	m_pLuckMeDlg = luckMeDlg;
}

void CQiXingCai::FlushZongjine()
{
	InvalidateRect(&rcZongJinE);
}

void CQiXingCai::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_MORE_RECORD, m_btnMoreRecord);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIA,m_btnBeiTouJia);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIAN,m_btnBeiTouJian);
	DDX_Control(pDX,IDC_TIP_WXZX,m_btWxzxTip);

	DDX_Control(pDX, IDC_RIO_LIANGDING,	m_btnLiangDing);
	DDX_Control(pDX, IDC_RIO_SANDING,	m_btnSanDing);
	DDX_Control(pDX, IDC_RIO_SIDING,	m_btnSiDing);
	DDX_Control(pDX, IDC_RIO_YIDING,	m_btnYiDing);
	DDX_Control(pDX, IDC_RIO_LIANGZIXIAN,	m_btnLiangZiXian);
	DDX_Control(pDX, IDC_RIO_SANZIXIAN, m_btnSanZiXian);

	DDX_Control(pDX, IDC_BTN_LOCK, m_btnLock);

	DDX_Control(pDX, IDC_RIO_MOUSE_INPUT,m_rioMouseInput);
	DDX_Control(pDX, IDC_RIO_KEYBOARD_INPUT,m_rioKeyboardInput);


	DDX_Control(pDX, IDC_BTN_WAN_0, m_btnWan_0);
	DDX_Control(pDX, IDC_BTN_WAN_1, m_btnWan_1);
	DDX_Control(pDX, IDC_BTN_WAN_2, m_btnWan_2);
	DDX_Control(pDX, IDC_BTN_WAN_3, m_btnWan_3);
	DDX_Control(pDX, IDC_BTN_WAN_4, m_btnWan_4);
	DDX_Control(pDX, IDC_BTN_WAN_5, m_btnWan_5);
	DDX_Control(pDX, IDC_BTN_WAN_6, m_btnWan_6);
	DDX_Control(pDX, IDC_BTN_WAN_7, m_btnWan_7);
	DDX_Control(pDX, IDC_BTN_WAN_8, m_btnWan_8);
	DDX_Control(pDX, IDC_BTN_WAN_9, m_btnWan_9);

	DDX_Control(pDX, IDC_BTN_QIAN_0, m_btnQian_0);
	DDX_Control(pDX, IDC_BTN_QIAN_1, m_btnQian_1);
	DDX_Control(pDX, IDC_BTN_QIAN_2, m_btnQian_2);
	DDX_Control(pDX, IDC_BTN_QIAN_3, m_btnQian_3);
	DDX_Control(pDX, IDC_BTN_QIAN_4, m_btnQian_4);
	DDX_Control(pDX, IDC_BTN_QIAN_5, m_btnQian_5);
	DDX_Control(pDX, IDC_BTN_QIAN_6, m_btnQian_6);
	DDX_Control(pDX, IDC_BTN_QIAN_7, m_btnQian_7);
	DDX_Control(pDX, IDC_BTN_QIAN_8, m_btnQian_8);
	DDX_Control(pDX, IDC_BTN_QIAN_9, m_btnQian_9);

	DDX_Control(pDX, IDC_BTN_BAI_0, m_btnBai_0);
	DDX_Control(pDX, IDC_BTN_BAI_1, m_btnBai_1);
	DDX_Control(pDX, IDC_BTN_BAI_2, m_btnBai_2);
	DDX_Control(pDX, IDC_BTN_BAI_3, m_btnBai_3);
	DDX_Control(pDX, IDC_BTN_BAI_4, m_btnBai_4);
	DDX_Control(pDX, IDC_BTN_BAI_5, m_btnBai_5);
	DDX_Control(pDX, IDC_BTN_BAI_6, m_btnBai_6);
	DDX_Control(pDX, IDC_BTN_BAI_7, m_btnBai_7);
	DDX_Control(pDX, IDC_BTN_BAI_8, m_btnBai_8);
	DDX_Control(pDX, IDC_BTN_BAI_9, m_btnBai_9);

	DDX_Control(pDX, IDC_BTN_SHI_0, m_btnShi_0);
	DDX_Control(pDX, IDC_BTN_SHI_1, m_btnShi_1);
	DDX_Control(pDX, IDC_BTN_SHI_2, m_btnShi_2);
	DDX_Control(pDX, IDC_BTN_SHI_3, m_btnShi_3);
	DDX_Control(pDX, IDC_BTN_SHI_4, m_btnShi_4);
	DDX_Control(pDX, IDC_BTN_SHI_5, m_btnShi_5);
	DDX_Control(pDX, IDC_BTN_SHI_6, m_btnShi_6);
	DDX_Control(pDX, IDC_BTN_SHI_7, m_btnShi_7);
	DDX_Control(pDX, IDC_BTN_SHI_8, m_btnShi_8);
	DDX_Control(pDX, IDC_BTN_SHI_9, m_btnShi_9);

	DDX_Control(pDX, IDC_BTN_GE_0, m_btnGe_0);
	DDX_Control(pDX, IDC_BTN_GE_1, m_btnGe_1);
	DDX_Control(pDX, IDC_BTN_GE_2, m_btnGe_2);
	DDX_Control(pDX, IDC_BTN_GE_3, m_btnGe_3);
	DDX_Control(pDX, IDC_BTN_GE_4, m_btnGe_4);
	DDX_Control(pDX, IDC_BTN_GE_5, m_btnGe_5);
	DDX_Control(pDX, IDC_BTN_GE_6, m_btnGe_6);
	DDX_Control(pDX, IDC_BTN_GE_7, m_btnGe_7);
	DDX_Control(pDX, IDC_BTN_GE_8, m_btnGe_8);
	DDX_Control(pDX, IDC_BTN_GE_9, m_btnGe_9);
	DDX_Text(pDX, IDC_EDIT_BEISHU, m_beishu);
	

	DDX_Control(pDX, IDC_EDIT_DANSHI_HAOMA, m_editDanShiHaoMa);
	DDX_Control(pDX, IDC_RICHEDIT_DANSHI, m_richDanshiHaoma);

	DDX_Control(pDX, IDC_STATIC_WAN, m_staticWan);
	DDX_Control(pDX, IDC_STATIC_QIAN, m_staticQian);
	DDX_Control(pDX, IDC_STATIC_BAI, m_staticBai);
	DDX_Control(pDX, IDC_STATIC_SHI, m_staticShi);
	DDX_Control(pDX, IDC_STATIC_GE, m_staticGe);
	DDX_Control(pDX, IDC_STATIC_NIU, m_staticNiu);

	DDX_Control(pDX, IDC_BTN_WAN_QUAN, m_btnWanQuan);
	DDX_Control(pDX, IDC_BTN_WAN_DA, m_btnWanDa);
	DDX_Control(pDX, IDC_BTN_WAN_XIAO, m_btnWanXiao);
	DDX_Control(pDX, IDC_BTN_WAN_DAN, m_btnWanDan);
	DDX_Control(pDX, IDC_BTN_WAN_SHUANG, m_btnWanShuang);
	DDX_Control(pDX, IDC_BTN_WAN_QING, m_btnWanQing);

	DDX_Control(pDX, IDC_BTN_QIAN_QUAN, m_btnQianQuan);
	DDX_Control(pDX, IDC_BTN_QIAN_DA, m_btnQianDa);
	DDX_Control(pDX, IDC_BTN_QIAN_XIAO, m_btnQianXiao);
	DDX_Control(pDX, IDC_BTN_QIAN_DAN, m_btnQianDan);
	DDX_Control(pDX, IDC_BTN_QIAN_SHUANG, m_btnQianShuang);
	DDX_Control(pDX, IDC_BTN_QIAN_QING, m_btnQianQing);

	DDX_Control(pDX, IDC_BTN_BAI_QUAN, m_btnBaiQuan);
	DDX_Control(pDX, IDC_BTN_BAI_DA, m_btnBaiDa);
	DDX_Control(pDX, IDC_BTN_BAI_XIAO, m_btnBaiXiao);
	DDX_Control(pDX, IDC_BTN_BAI_DAN, m_btnBaiDan);
	DDX_Control(pDX, IDC_BTN_BAI_SHUANG, m_btnBaiShuang);
	DDX_Control(pDX, IDC_BTN_BAI_QING, m_btnBaiQing);

	DDX_Control(pDX, IDC_BTN_SHI_QUAN, m_btnShiQuan);
	DDX_Control(pDX, IDC_BTN_SHI_DA, m_btnShiDa);
	DDX_Control(pDX, IDC_BTN_SHI_XIAO, m_btnShiXiao);
	DDX_Control(pDX, IDC_BTN_SHI_DAN, m_btnShiDan);
	DDX_Control(pDX, IDC_BTN_SHI_SHUANG, m_btnShiShuang);
	DDX_Control(pDX, IDC_BTN_SHI_QING, m_btnShiQing);

	DDX_Control(pDX, IDC_BTN_GE_QUAN, m_btnGeQuan);
	DDX_Control(pDX, IDC_BTN_GE_DA, m_btnGeDa);
	DDX_Control(pDX, IDC_BTN_GE_XIAO, m_btnGeXiao);
	DDX_Control(pDX, IDC_BTN_GE_DAN, m_btnGeDan);
	DDX_Control(pDX, IDC_BTN_GE_SHUANG, m_btnGeShuang);
	DDX_Control(pDX, IDC_BTN_GE_QING, m_btnGeQing);



	DDX_Control(pDX, IDC_LIST_CHQSSC, m_listChqSSC);
	DDX_Control(pDX, IDC_GRID_LIST, m_GridList);
	DDX_Control(pDX, IDC_BTN_CHQ_ADD, m_btnChqAdd);
	DDX_Control(pDX, IDC_BTN_DEL_SEL, m_btnDelSel);
	DDX_Control(pDX, IDC_BTN_CLS_LIST, m_btnClsList);
	DDX_Control(pDX, IDC_BTN_TOUZHU, m_btnTouZhu);
	DDX_Control(pDX, IDC_BTN_ZHUIHAO, m_btnZhuiHao);
	DDX_Control(pDX, IDC_EDIT_BEISHU, m_editBeiTou);



	DDX_Control(pDX, IDC_STATIC_BUDINGWEI, m_staticBuDingWei);




	DDX_Control(pDX, IDC_BTN_YUAN,m_btnYuan);
	DDX_Control(pDX, IDC_BTN_JIAO,m_btnJiao);
	DDX_Control(pDX, IDC_BTN_FEN,m_btnFen);
	DDX_Control(pDX, IDC_BTN_LI,m_btnLi);
}

BEGIN_MESSAGE_MAP(CQiXingCai, CDialog)
	ON_BN_CLICKED(IDC_BTN_YUAN, &CQiXingCai::OnBnClickedBtnYuan)
	ON_BN_CLICKED(IDC_BTN_JIAO, &CQiXingCai::OnBnClickedBtnJiao)
	ON_BN_CLICKED( IDC_BTN_FEN, &CQiXingCai::OnBnClickedBtnFen)
	ON_BN_CLICKED(IDC_BTN_LI, &CQiXingCai::OnBnClickedBtnLi)
	ON_BN_CLICKED(IDC_RIO_LIANGDING, &CQiXingCai::OnBnClickedRioLiangDing)
	ON_BN_CLICKED(IDC_RIO_SANDING, &CQiXingCai::OnBnClickedRioSanDing)
	ON_BN_CLICKED(IDC_RIO_SIDING, &CQiXingCai::OnBnClickedRioSiDing)
	ON_BN_CLICKED(IDC_RIO_YIDING, &CQiXingCai::OnBnClickedRioYiDing)
	ON_BN_CLICKED(IDC_RIO_LIANGZIXIAN, &CQiXingCai::OnBnClickedRioLiangZiXian)
	ON_BN_CLICKED(IDC_RIO_SANZIXIAN, &CQiXingCai::OnBnClickedRioSanZiXian)
	ON_MESSAGE( WM_BN_CLICK, &CQiXingCai::onBnCLick)

	ON_BN_CLICKED(IDC_BTN_MORE_RECORD, &CQiXingCai::OnBnClickedBtnMoreRecord)
	ON_BN_CLICKED(IDC_BTN_WAN_QUAN, &CQiXingCai::OnBnClickedBtnWanQuan)
	ON_BN_CLICKED(IDC_BTN_WAN_DA, &CQiXingCai::OnBnClickedBtnWanDa)
	ON_BN_CLICKED(IDC_BTN_WAN_XIAO, &CQiXingCai::OnBnClickedBtnWanXiao)
	ON_BN_CLICKED(IDC_BTN_WAN_DAN, &CQiXingCai::OnBnClickedBtnWanDan)
	ON_BN_CLICKED(IDC_BTN_WAN_SHUANG, &CQiXingCai::OnBnClickedBtnWanShuang)
	ON_BN_CLICKED(IDC_BTN_WAN_QING, &CQiXingCai::OnBnClickedBtnWanQing)
	ON_BN_CLICKED(IDC_BTN_QIAN_QUAN, &CQiXingCai::OnBnClickedBtnQianQuan)
	ON_BN_CLICKED(IDC_BTN_QIAN_DA, &CQiXingCai::OnBnClickedBtnQianDa)
	ON_BN_CLICKED(IDC_BTN_QIAN_XIAO, &CQiXingCai::OnBnClickedBtnQianXiao)
	ON_BN_CLICKED(IDC_BTN_QIAN_DAN, &CQiXingCai::OnBnClickedBtnQianDan)
	ON_BN_CLICKED(IDC_BTN_QIAN_SHUANG, &CQiXingCai::OnBnClickedBtnQianShuang)
	ON_BN_CLICKED(IDC_BTN_QIAN_QING, &CQiXingCai::OnBnClickedBtnQianQing)
	ON_BN_CLICKED(IDC_BTN_BAI_QUAN, &CQiXingCai::OnBnClickedBtnBaiQuan)
	ON_BN_CLICKED(IDC_BTN_BAI_DA, &CQiXingCai::OnBnClickedBtnBaiDa)
	ON_BN_CLICKED(IDC_BTN_BAI_XIAO, &CQiXingCai::OnBnClickedBtnBaiXiao)
	ON_BN_CLICKED(IDC_BTN_BAI_DAN, &CQiXingCai::OnBnClickedBtnBaiDan)
	ON_BN_CLICKED(IDC_BTN_BAI_SHUANG, &CQiXingCai::OnBnClickedBtnBaiShuang)
	ON_BN_CLICKED(IDC_BTN_BAI_QING, &CQiXingCai::OnBnClickedBtnBaiQing)
	ON_BN_CLICKED(IDC_BTN_SHI_QUAN, &CQiXingCai::OnBnClickedBtnShiQuan)
	ON_BN_CLICKED(IDC_BTN_SHI_DA, &CQiXingCai::OnBnClickedBtnShiDa)
	ON_BN_CLICKED(IDC_BTN_SHI_XIAO, &CQiXingCai::OnBnClickedBtnShiXiao)
	ON_BN_CLICKED(IDC_BTN_SHI_DAN, &CQiXingCai::OnBnClickedBtnShiDan)
	ON_BN_CLICKED(IDC_BTN_SHI_SHUANG, &CQiXingCai::OnBnClickedBtnShiShuang)
	ON_BN_CLICKED(IDC_BTN_SHI_QING, &CQiXingCai::OnBnClickedBtnShiQing)
	ON_BN_CLICKED(IDC_BTN_GE_QUAN, &CQiXingCai::OnBnClickedBtnGeQuan)
	ON_BN_CLICKED(IDC_BTN_GE_DA, &CQiXingCai::OnBnClickedBtnGeDa)
	ON_BN_CLICKED(IDC_BTN_GE_XIAO, &CQiXingCai::OnBnClickedBtnGeXiao)
	ON_BN_CLICKED(IDC_BTN_GE_DAN, &CQiXingCai::OnBnClickedBtnGeDan)
	ON_BN_CLICKED(IDC_BTN_GE_SHUANG, &CQiXingCai::OnBnClickedBtnGeShuang)
	ON_BN_CLICKED(IDC_BTN_GE_QING, &CQiXingCai::OnBnClickedBtnGeQing)
	//ON_BN_CLICKED(IDC_BTN_NIU_QUAN, &CQiXingCai::OnBnClickedBtnNiuQuan)
	//ON_BN_CLICKED(IDC_BTN_NIU_DA, &CQiXingCai::OnBnClickedBtnNiuDa)
	//ON_BN_CLICKED(IDC_BTN_NIU_XIAO, &CQiXingCai::OnBnClickedBtnNiuXiao)
	//ON_BN_CLICKED(IDC_BTN_NIU_DAN, &CQiXingCai::OnBnClickedBtnNiuDan)
	//ON_BN_CLICKED(IDC_BTN_NIU_SHUANG, &CQiXingCai::OnBnClickedBtnNiuShuang)
	//ON_BN_CLICKED(IDC_BTN_NIU_QING, &CQiXingCai::OnBnClickedBtnNiuQing)
	ON_BN_CLICKED(IDC_BTN_CHQ_ADD, &CQiXingCai::OnBnClickedBtnChqAdd)
	ON_COMMAND(ID_RBTN_CPY, &CQiXingCai::OnRichEditCopy)


	ON_BN_CLICKED(IDC_BTN_ZHUIHAO, &CQiXingCai::OnBnClickedBtnZhuihao)
	ON_BN_CLICKED(IDC_BTN_TOUZHU, &CQiXingCai::OnBnClickedBtnTouzhu)
	ON_BN_CLICKED(IDC_BTN_DEL_SEL, &CQiXingCai::OnBnClickedBtnDelSel)
	ON_BN_CLICKED(IDC_BTN_CLS_LIST, &CQiXingCai::OnBnClickedBtnClsList)
	ON_EN_CHANGE(IDC_EDIT_BEISHU, &CQiXingCai::OnEnChangeEditBeishu)
	ON_EN_CHANGE(IDC_EDIT_DANSHI_HAOMA, &CQiXingCai::OnEnChangeEditDanshiInput)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEMOVE()
 	ON_WM_LBUTTONUP()
 	ON_WM_LBUTTONDOWN()
 	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_BN_CLICKED(IDC_RIO_MOUSE_INPUT, &CQiXingCai::OnBnClickedRioMouseInput)
	ON_BN_CLICKED(IDC_RIO_KEYBOARD_INPUT, &CQiXingCai::OnBnClickedRioKeyboardInput)
	ON_BN_CLICKED(IDC_CHK_WAN, &CQiXingCai::OnBnClickedChkWan)
	ON_BN_CLICKED(IDC_CHK_QIAN, &CQiXingCai::OnBnClickedChkQian)
	ON_BN_CLICKED(IDC_CHK_BAI, &CQiXingCai::OnBnClickedChkBai)
	ON_BN_CLICKED(IDC_CHK_SHI, &CQiXingCai::OnBnClickedChkShi)
	ON_BN_CLICKED(IDC_CHKGE, &CQiXingCai::OnBnClickedChkge)
	ON_BN_CLICKED(IDC_BTN_LOCK, &CQiXingCai::OnBnClickedBtnLock)
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIA, &CQiXingCai::OnBnClickedBeiTouJia)
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIAN, &CQiXingCai::OnBnClickedBeiTouJian)

	////LLY
	//ON_BN_CLICKED(IDC_RIO_LIANGDING, &CQiXingCai::OnBnClickedRioLiangDing)
	//ON_BN_CLICKED(IDC_RIO_SANDING, &CQiXingCai::OnBnClickedRioSanDing)
	//ON_BN_CLICKED(IDC_RIO_SIDING, &CQiXingCai::OnBnClickedRioSiDing)
	//ON_BN_CLICKED(IDC_RIO_YIDING, &CQiXingCai::OnBnClickedRioYiDing)
	//ON_BN_CLICKED(IDC_RIO_LIANGZIXIAN, &CQiXingCai::OnBnClickedRioLiangZiXian)
	//ON_BN_CLICKED(IDC_RIO_SANZIXIAN, &CQiXingCai::OnBnClickedRioSanZiXian)

END_MESSAGE_MAP()

void CQiXingCai::OnRichEditCopy()
{
	char *buffer = NULL;
	CString fromClipboard;
	if(OpenClipboard())
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		buffer = (char*)GlobalLock(hData);
		fromClipboard = buffer;
		GlobalUnlock(hData);
		CloseClipboard();
	}

	m_richDanshiHaoma.SetWindowText(fromClipboard);
	//MyMessageBox(fromClipboard);
	return;
}
void CQiXingCai::OnBnClickedBtnYuan()
{
	m_btnYuan.SetPushed(true);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Yuan ;

	InvalidateRect(rcZongJinE);

	return;
}

void CQiXingCai::OnBnClickedBtnMoreRecord()
{
	SendToServer(8);
	return;
}

void CQiXingCai::OnBnClickedBtnJiao()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(true);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Jiao ;
	InvalidateRect(rcZongJinE);

	return;
}
void CQiXingCai::OnBnClickedBtnFen()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(true);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Fen ;
	InvalidateRect(rcZongJinE);
	return;

}
void CQiXingCai::OnBnClickedBtnLi()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(true);
	theAccount.Scoretype=MoShi_Li ;
	InvalidateRect(rcZongJinE);

	return;
}

// CQiXingCai 消息处理程序
void CQiXingCai::OnPaint()
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

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CQiXingCai::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CQiXingCai::OnSize(UINT nType, int cx, int cy)
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

void CQiXingCai::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
		{
			return;
		}
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

		LuckyNumCQSSC();

		SendToServer(6);
		m_bKaiJiangzhong = false;
// 		SetTimer(timer_id_get_luck_num, 10000, NULL);
		SetTimer(timer_id_kj_yugao, 1000, NULL);
		SetTimer(timer_id_kaijiangzhong, 100, NULL);

		OnBnClickedRioLiangDing();
	}
	else
	{
		KillTimer(timer_id_get_luck_num);
		KillTimer(timer_id_kj_yugao);
		KillTimer(timer_id_kaijiangzhong);
		m_DlgOpenRecord.ShowWindow(SW_HIDE);
	}

}

HBRUSH CQiXingCai::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() == IDC_EDIT_DANSHI_HAOMA) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}

		return hBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RICHEDIT_DANSHI) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}

		return hBrush;
	}
	//混合模式，提示信息
	if(CTLCOLOR_STATIC==nCtlColor && (pWnd->GetDlgCtrlID() == IDC_STATIC_HUNHE_TIP || pWnd->GetDlgCtrlID() == IDC_STATIC_NIUNIU_TIP))
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(61,47,42));
		}

		return hBrush;
	}
	//任选STATIC
	if(CTLCOLOR_STATIC==nCtlColor && pWnd->GetDlgCtrlID() == IDC_STATIC_RENXUAN_TIP)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(88, 78, 77));
		if(hBrush)
		{
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(245,229,203));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(245,229,203));
		}

		return hBrush;
	}
	//任选，键盘输入的多选框
//	if(CTLCOLOR_BTN == nCtlColor )
//	{
		if((pWnd->GetDlgCtrlID() == IDC_CHK_WAN)	||
			(pWnd->GetDlgCtrlID() ==IDC_CHK_QIAN)	||
			(pWnd->GetDlgCtrlID() ==IDC_CHK_BAI)	||
			(pWnd->GetDlgCtrlID() ==IDC_CHK_SHI)	||
			(pWnd->GetDlgCtrlID() ==IDC_CHKGE))
		{
			pDC->SetBkMode(TRANSPARENT); 
			pDC->SetTextColor(RGB(88, 78, 77));
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = CreateSolidBrush(RGB(245, 229, 203));
			}
			else
			{
				hBrush = CreateSolidBrush(RGB(245, 229, 203));
			}

			return hBrush;
		}

//	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CQiXingCai::OnTimer(UINT_PTR nIDEvent)
{
	if(timer_id_get_luck_num == nIDEvent)
	{
		LuckyNumCQSSC();
	}

	else if (timer_id_kj_yugao == nIDEvent)
	{
		CRect rcRect;
		rcRect.CopyRect(rcYuGao);
		//rcRect.right += 80;
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
				int nLeft=kj_big_haoma_x -15/*+ n *( m_bigNumWidth+8)*/;
				CMemDC	mDC(pDC, CRect(nLeft, kj_big_haoma_y, nLeft+m_imgKaijiang->GetWidth(), kj_big_haoma_y+m_imgKaijiang->GetHeight()));
				Graphics gh(mDC.m_hDC);
				gh.DrawImage(m_imgKaijiang, nLeft, kj_big_haoma_y, m_imgKaijiang->GetWidth(), m_imgKaijiang->GetHeight());
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



BOOL CQiXingCai::OnInitDialog()
{
	CDialog::OnInitDialog();

	ZeroMemory(m_szMD5Haoma,sizeof(m_szMD5Haoma));
	hBrush = NULL;
	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\game_bk3.png"));
	m_numImageList = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num.png"));
	m_numImageList1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num2.png"));
	m_numImageList2 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num3.png"));
	m_kjNumBig = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big.png"));
	m_imgKaijiang = new Bitmap( CBmpUtil::GetExePath() + _T("skin\\game\\KaiJiang.gif"));
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
	m_bigNumWidth = m_kjNumBig->GetWidth() / 10;
	m_bigNumHeight = m_kjNumBig->GetHeight();
	m_kjNumSmall = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small.png"));
	m_smallNumWidth = m_kjNumSmall->GetWidth() / 10;
	m_smallNumHeight = m_kjNumSmall->GetHeight();
	m_bmpDxdsh = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx2.png"));
	m_bmpNumTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit.png"));
	m_bmpNumTip1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit2.png"));
	m_btnMoreRecord.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_more.png"));
	m_bCanSend=true;
	InitNumberBtns();
	InitListCtrl();
	m_DlgOpenRecord.Create(COpenRecord::IDD,this);
	m_smallNumfont.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_zongFont.CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_KjInfoFont1.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_KjInfoFont.CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	m_btnDelSel.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_delate.png"));
	m_btnClsList.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_clear.png"));
	m_btnTouZhu.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_bet.png"));
	m_btnZhuiHao.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_append.png"));
	m_btnChqAdd.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_addnum.png"));
	m_btnLock.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_lock.png"));
	m_btnLock.SetTextColor(RGB(51, 45, 42));
	m_btnBeiTouJia.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jia.png"));
	m_btnBeiTouJian.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jian.png"));
	m_btnLock.SetTextFont(&m_zongFont);
	m_editBeiTou.SetEnableColor(RGB(0,0,0),RGB(250,243,227),RGB(250,243,227));
	m_editBeiTou.SetFont(&m_zongFont);

	m_richDanshiHaoma.LimitText(-1);
	m_richDanshiHaoma.SetBackgroundColor(false,RGB(200,200,200));
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(88, 78, 77);
	m_richDanshiHaoma.SetDefaultCharFormat(cf);

	LuckyNumCQSSC();
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.SetMaxTipWidth(150);
	m_btWxzxTip.SetImage(CBmpUtil::GetExePath() + _T("skin\\shili.png"));

// 	SendQuerySystemTime();
// 	SendQueryLuckyNumCQSSC();

	//SetTimer(TimerZhunBeiData, 2000, NULL);

	m_bKaiJiangzhong=false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CQiXingCai::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CQiXingCai::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}



void CQiXingCai::OnBnClickedBtnWanQuan()
{
	CheckNumBtn(m_btnWan_0);
	CheckNumBtn(m_btnWan_1);
	CheckNumBtn(m_btnWan_2);
	CheckNumBtn(m_btnWan_3);
	CheckNumBtn(m_btnWan_4);
	CheckNumBtn(m_btnWan_5);
	CheckNumBtn(m_btnWan_6);
	CheckNumBtn(m_btnWan_7);
	CheckNumBtn(m_btnWan_8);
	CheckNumBtn(m_btnWan_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnWanDa()
{
	UnCheckNumBtn(m_btnWan_0);
	UnCheckNumBtn(m_btnWan_1);
	UnCheckNumBtn(m_btnWan_2);
	UnCheckNumBtn(m_btnWan_3);
	UnCheckNumBtn(m_btnWan_4);
	CheckNumBtn(m_btnWan_5);
	CheckNumBtn(m_btnWan_6);
	CheckNumBtn(m_btnWan_7);
	CheckNumBtn(m_btnWan_8);
	CheckNumBtn(m_btnWan_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnWanXiao()
{
	CheckNumBtn(m_btnWan_0);
	CheckNumBtn(m_btnWan_1);
	CheckNumBtn(m_btnWan_2);
	CheckNumBtn(m_btnWan_3);
	CheckNumBtn(m_btnWan_4);
	UnCheckNumBtn(m_btnWan_5);
	UnCheckNumBtn(m_btnWan_6);
	UnCheckNumBtn(m_btnWan_7);
	UnCheckNumBtn(m_btnWan_8);
	UnCheckNumBtn(m_btnWan_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnWanDan()
{
	UnCheckNumBtn(m_btnWan_0);
	CheckNumBtn(m_btnWan_1);
	UnCheckNumBtn(m_btnWan_2);
	CheckNumBtn(m_btnWan_3);
	UnCheckNumBtn(m_btnWan_4);
	CheckNumBtn(m_btnWan_5);
	UnCheckNumBtn(m_btnWan_6);
	CheckNumBtn(m_btnWan_7);
	UnCheckNumBtn(m_btnWan_8);
	CheckNumBtn(m_btnWan_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnWanShuang()
{
	CheckNumBtn(m_btnWan_0);
	UnCheckNumBtn(m_btnWan_1);
	CheckNumBtn(m_btnWan_2);
	UnCheckNumBtn(m_btnWan_3);
	CheckNumBtn(m_btnWan_4);
	UnCheckNumBtn(m_btnWan_5);
	CheckNumBtn(m_btnWan_6);
	UnCheckNumBtn(m_btnWan_7);
	CheckNumBtn(m_btnWan_8);
	UnCheckNumBtn(m_btnWan_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnWanQing()
{
	UnCheckNumBtn(m_btnWan_0);
	UnCheckNumBtn(m_btnWan_1);
	UnCheckNumBtn(m_btnWan_2);
	UnCheckNumBtn(m_btnWan_3);
	UnCheckNumBtn(m_btnWan_4);
	UnCheckNumBtn(m_btnWan_5);
	UnCheckNumBtn(m_btnWan_6);
	UnCheckNumBtn(m_btnWan_7);
	UnCheckNumBtn(m_btnWan_8);
	UnCheckNumBtn(m_btnWan_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnQianQuan()
{
	CheckNumBtn(m_btnQian_0);
	CheckNumBtn(m_btnQian_1);
	CheckNumBtn(m_btnQian_2);
	CheckNumBtn(m_btnQian_3);
	CheckNumBtn(m_btnQian_4);
	CheckNumBtn(m_btnQian_5);
	CheckNumBtn(m_btnQian_6);
	CheckNumBtn(m_btnQian_7);
	CheckNumBtn(m_btnQian_8);
	CheckNumBtn(m_btnQian_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnQianDa()
{
	UnCheckNumBtn(m_btnQian_0);
	UnCheckNumBtn(m_btnQian_1);
	UnCheckNumBtn(m_btnQian_2);
	UnCheckNumBtn(m_btnQian_3);
	UnCheckNumBtn(m_btnQian_4);
	CheckNumBtn(m_btnQian_5);
	CheckNumBtn(m_btnQian_6);
	CheckNumBtn(m_btnQian_7);
	CheckNumBtn(m_btnQian_8);
	CheckNumBtn(m_btnQian_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnQianXiao()
{
	CheckNumBtn(m_btnQian_0);
	CheckNumBtn(m_btnQian_1);
	CheckNumBtn(m_btnQian_2);
	CheckNumBtn(m_btnQian_3);
	CheckNumBtn(m_btnQian_4);
	UnCheckNumBtn(m_btnQian_5);
	UnCheckNumBtn(m_btnQian_6);
	UnCheckNumBtn(m_btnQian_7);
	UnCheckNumBtn(m_btnQian_8);
	UnCheckNumBtn(m_btnQian_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnQianDan()
{
	UnCheckNumBtn(m_btnQian_0);
	CheckNumBtn(m_btnQian_1);
	UnCheckNumBtn(m_btnQian_2);
	CheckNumBtn(m_btnQian_3);
	UnCheckNumBtn(m_btnQian_4);
	CheckNumBtn(m_btnQian_5);
	UnCheckNumBtn(m_btnQian_6);
	CheckNumBtn(m_btnQian_7);
	UnCheckNumBtn(m_btnQian_8);
	CheckNumBtn(m_btnQian_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnQianShuang()
{
	CheckNumBtn(m_btnQian_0);
	UnCheckNumBtn(m_btnQian_1);
	CheckNumBtn(m_btnQian_2);
	UnCheckNumBtn(m_btnQian_3);
	CheckNumBtn(m_btnQian_4);
	UnCheckNumBtn(m_btnQian_5);
	CheckNumBtn(m_btnQian_6);
	UnCheckNumBtn(m_btnQian_7);
	CheckNumBtn(m_btnQian_8);
	UnCheckNumBtn(m_btnQian_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnQianQing()
{
	UnCheckNumBtn(m_btnQian_0);
	UnCheckNumBtn(m_btnQian_1);
	UnCheckNumBtn(m_btnQian_2);
	UnCheckNumBtn(m_btnQian_3);
	UnCheckNumBtn(m_btnQian_4);
	UnCheckNumBtn(m_btnQian_5);
	UnCheckNumBtn(m_btnQian_6);
	UnCheckNumBtn(m_btnQian_7);
	UnCheckNumBtn(m_btnQian_8);
	UnCheckNumBtn(m_btnQian_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnBaiQuan()
{
	CheckNumBtn(m_btnBai_0);
	CheckNumBtn(m_btnBai_1);
	CheckNumBtn(m_btnBai_2);
	CheckNumBtn(m_btnBai_3);
	CheckNumBtn(m_btnBai_4);
	CheckNumBtn(m_btnBai_5);
	CheckNumBtn(m_btnBai_6);
	CheckNumBtn(m_btnBai_7);
	CheckNumBtn(m_btnBai_8);
	CheckNumBtn(m_btnBai_9);

	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnBaiDa()
{
	UnCheckNumBtn(m_btnBai_0);
	UnCheckNumBtn(m_btnBai_1);
	UnCheckNumBtn(m_btnBai_2);
	UnCheckNumBtn(m_btnBai_3);
	UnCheckNumBtn(m_btnBai_4);
	CheckNumBtn(m_btnBai_5);
	CheckNumBtn(m_btnBai_6);
	CheckNumBtn(m_btnBai_7);
	CheckNumBtn(m_btnBai_8);
	CheckNumBtn(m_btnBai_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnBaiXiao()
{
	CheckNumBtn(m_btnBai_0);
	CheckNumBtn(m_btnBai_1);
	CheckNumBtn(m_btnBai_2);
	CheckNumBtn(m_btnBai_3);
	CheckNumBtn(m_btnBai_4);
	UnCheckNumBtn(m_btnBai_5);
	UnCheckNumBtn(m_btnBai_6);
	UnCheckNumBtn(m_btnBai_7);
	UnCheckNumBtn(m_btnBai_8);
	UnCheckNumBtn(m_btnBai_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnBaiDan()
{
	UnCheckNumBtn(m_btnBai_0);
	CheckNumBtn(m_btnBai_1);
	UnCheckNumBtn(m_btnBai_2);
	CheckNumBtn(m_btnBai_3);
	UnCheckNumBtn(m_btnBai_4);
	CheckNumBtn(m_btnBai_5);
	UnCheckNumBtn(m_btnBai_6);
	CheckNumBtn(m_btnBai_7);
	UnCheckNumBtn(m_btnBai_8);
	CheckNumBtn(m_btnBai_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnBaiShuang()
{
	CheckNumBtn(m_btnBai_0);
	UnCheckNumBtn(m_btnBai_1);
	CheckNumBtn(m_btnBai_2);
	UnCheckNumBtn(m_btnBai_3);
	CheckNumBtn(m_btnBai_4);
	UnCheckNumBtn(m_btnBai_5);
	CheckNumBtn(m_btnBai_6);
	UnCheckNumBtn(m_btnBai_7);
	CheckNumBtn(m_btnBai_8);
	UnCheckNumBtn(m_btnBai_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnBaiQing()
{
	UnCheckNumBtn(m_btnBai_0);
	UnCheckNumBtn(m_btnBai_1);
	UnCheckNumBtn(m_btnBai_2);
	UnCheckNumBtn(m_btnBai_3);
	UnCheckNumBtn(m_btnBai_4);
	UnCheckNumBtn(m_btnBai_5);
	UnCheckNumBtn(m_btnBai_6);
	UnCheckNumBtn(m_btnBai_7);
	UnCheckNumBtn(m_btnBai_8);
	UnCheckNumBtn(m_btnBai_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnShiQuan()
{
	CheckNumBtn(m_btnShi_0);
	CheckNumBtn(m_btnShi_1);
	CheckNumBtn(m_btnShi_2);
	CheckNumBtn(m_btnShi_3);
	CheckNumBtn(m_btnShi_4);
	CheckNumBtn(m_btnShi_5);
	CheckNumBtn(m_btnShi_6);
	CheckNumBtn(m_btnShi_7);
	CheckNumBtn(m_btnShi_8);
	CheckNumBtn(m_btnShi_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnShiDa()
{
	UnCheckNumBtn(m_btnShi_0);
	UnCheckNumBtn(m_btnShi_1);
	UnCheckNumBtn(m_btnShi_2);
	UnCheckNumBtn(m_btnShi_3);
	UnCheckNumBtn(m_btnShi_4);
	CheckNumBtn(m_btnShi_5);
	CheckNumBtn(m_btnShi_6);
	CheckNumBtn(m_btnShi_7);
	CheckNumBtn(m_btnShi_8);
	CheckNumBtn(m_btnShi_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnShiXiao()
{
	CheckNumBtn(m_btnShi_0);
	CheckNumBtn(m_btnShi_1);
	CheckNumBtn(m_btnShi_2);
	CheckNumBtn(m_btnShi_3);
	CheckNumBtn(m_btnShi_4);
	UnCheckNumBtn(m_btnShi_5);
	UnCheckNumBtn(m_btnShi_6);
	UnCheckNumBtn(m_btnShi_7);
	UnCheckNumBtn(m_btnShi_8);
	UnCheckNumBtn(m_btnShi_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnShiDan()
{
	UnCheckNumBtn(m_btnShi_0);
	CheckNumBtn(m_btnShi_1);
	UnCheckNumBtn(m_btnShi_2);
	CheckNumBtn(m_btnShi_3);
	UnCheckNumBtn(m_btnShi_4);
	CheckNumBtn(m_btnShi_5);
	UnCheckNumBtn(m_btnShi_6);
	CheckNumBtn(m_btnShi_7);
	UnCheckNumBtn(m_btnShi_8);
	CheckNumBtn(m_btnShi_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnShiShuang()
{
	CheckNumBtn(m_btnShi_0);
	UnCheckNumBtn(m_btnShi_1);
	CheckNumBtn(m_btnShi_2);
	UnCheckNumBtn(m_btnShi_3);
	CheckNumBtn(m_btnShi_4);
	UnCheckNumBtn(m_btnShi_5);
	CheckNumBtn(m_btnShi_6);
	UnCheckNumBtn(m_btnShi_7);
	CheckNumBtn(m_btnShi_8);
	UnCheckNumBtn(m_btnShi_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnShiQing()
{
	UnCheckNumBtn(m_btnShi_0);
	UnCheckNumBtn(m_btnShi_1);
	UnCheckNumBtn(m_btnShi_2);
	UnCheckNumBtn(m_btnShi_3);
	UnCheckNumBtn(m_btnShi_4);
	UnCheckNumBtn(m_btnShi_5);
	UnCheckNumBtn(m_btnShi_6);
	UnCheckNumBtn(m_btnShi_7);
	UnCheckNumBtn(m_btnShi_8);
	UnCheckNumBtn(m_btnShi_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);

}

void CQiXingCai::OnBnClickedBtnGeQuan()
{
	CheckNumBtn(m_btnGe_0);
	CheckNumBtn(m_btnGe_1);
	CheckNumBtn(m_btnGe_2);
	CheckNumBtn(m_btnGe_3);
	CheckNumBtn(m_btnGe_4);
	CheckNumBtn(m_btnGe_5);
	CheckNumBtn(m_btnGe_6);
	CheckNumBtn(m_btnGe_7);
	CheckNumBtn(m_btnGe_8);
	CheckNumBtn(m_btnGe_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnGeDa()
{
	UnCheckNumBtn(m_btnGe_0);
	UnCheckNumBtn(m_btnGe_1);
	UnCheckNumBtn(m_btnGe_2);
	UnCheckNumBtn(m_btnGe_3);
	UnCheckNumBtn(m_btnGe_4);
	CheckNumBtn(m_btnGe_5);
	CheckNumBtn(m_btnGe_6);
	CheckNumBtn(m_btnGe_7);
	CheckNumBtn(m_btnGe_8);
	CheckNumBtn(m_btnGe_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnGeXiao()
{
	CheckNumBtn(m_btnGe_0);
	CheckNumBtn(m_btnGe_1);
	CheckNumBtn(m_btnGe_2);
	CheckNumBtn(m_btnGe_3);
	CheckNumBtn(m_btnGe_4);
	UnCheckNumBtn(m_btnGe_5);
	UnCheckNumBtn(m_btnGe_6);
	UnCheckNumBtn(m_btnGe_7);
	UnCheckNumBtn(m_btnGe_8);
	UnCheckNumBtn(m_btnGe_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnGeDan()
{
	UnCheckNumBtn(m_btnGe_0);
	CheckNumBtn(m_btnGe_1);
	UnCheckNumBtn(m_btnGe_2);
	CheckNumBtn(m_btnGe_3);
	UnCheckNumBtn(m_btnGe_4);
	CheckNumBtn(m_btnGe_5);
	UnCheckNumBtn(m_btnGe_6);
	CheckNumBtn(m_btnGe_7);
	UnCheckNumBtn(m_btnGe_8);
	CheckNumBtn(m_btnGe_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnGeShuang()
{
	CheckNumBtn(m_btnGe_0);
	UnCheckNumBtn(m_btnGe_1);
	CheckNumBtn(m_btnGe_2);
	UnCheckNumBtn(m_btnGe_3);
	CheckNumBtn(m_btnGe_4);
	UnCheckNumBtn(m_btnGe_5);
	CheckNumBtn(m_btnGe_6);
	UnCheckNumBtn(m_btnGe_7);
	CheckNumBtn(m_btnGe_8);
	UnCheckNumBtn(m_btnGe_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}

void CQiXingCai::OnBnClickedBtnGeQing()
{
	UnCheckNumBtn(m_btnGe_0);
	UnCheckNumBtn(m_btnGe_1);
	UnCheckNumBtn(m_btnGe_2);
	UnCheckNumBtn(m_btnGe_3);
	UnCheckNumBtn(m_btnGe_4);
	UnCheckNumBtn(m_btnGe_5);
	UnCheckNumBtn(m_btnGe_6);
	UnCheckNumBtn(m_btnGe_7);
	UnCheckNumBtn(m_btnGe_8);
	UnCheckNumBtn(m_btnGe_9);
	GetQXCZhushu(CZ_QiXingCai,m_gameKind);
}
bool CQiXingCai::GetKeyBoardHaomaXian(CString& haoma, int size, int nLeast)
{
	CStringArray strArr;
	CString line;

	m_richDanshiHaoma.SetSel(0,-1);
	line = m_richDanshiHaoma.GetSelText();

	int nFind = line.Find(L"，");
	if(nFind != -1)
	{
		line.Replace(_T("，"),_T(","));
	}

	line.Replace(L"\r",L" ");
	line.Replace(L"\n",L" ");

	if (!GetKeyHaomaStrsXian(strArr, line, size,nLeast)) 
	{
		return false;
	}

	int count = strArr.GetCount();
	CString strTemp;
	for(int i=0; i<count; i++)
	{
		strTemp = strArr.GetAt(i);
		int nWeishu = strTemp.GetLength();
		if(nWeishu > nLeast)
		{
			MyMessageBox(_T("您输入的号码太多了"));	
			return false;

		}
		for(int j = 0; j<strTemp.GetLength();j++)
		{
			haoma += strTemp.GetAt(j);
			if ((j!=nWeishu-1))
			{
				haoma += _T(",");
			}
			if(j == nWeishu-1)
			{
				haoma +=_T("|");
			}
		}
	}
	return true;
}

bool CQiXingCai::GetKeyHaomaStrsXian(CStringArray& strArr, CString line, int size, int nLeast)
{
	int len = line.GetLength();
	CString subStr;
	CString strTemp;
	int NumCount = 0;

	for (int i=0; i<len; i++) 
	{
		CString str;
		str=line.GetAt(i);

		int nTemp = 10;
		if (iswdigit(line.GetAt(i))&&!str.IsEmpty()/*&&str.Compare(L"\r\n")!=0*/)
		{
			subStr += line.GetAt(i);
			strTemp+= line.GetAt(i);
			//校验数字有效性
			nTemp = _ttoi(strTemp);
			strTemp=_T("");
			if(nTemp<0||nTemp>9)
			{
				MyMessageBox(L"您的输入有误，数字只能在0～9之间 ");
				return false;
			}
			//校验数字重复
			for (int j=0;j<subStr.GetLength()-1;j++)
			{
				strTemp = subStr.GetAt(j);
				if (nTemp == _ttoi(strTemp))
				{
					CString msgtemp;
					msgtemp.Format(L"您的输入有误，第%d组有相同数字",(1+strArr.GetCount()));
					MyMessageBox(msgtemp.GetBuffer());
					return false;
				}
			}

			NumCount++;
			strTemp=_T("");
			//一组保存
			if (NumCount == size)
			{
				NumCount = 0;
				strArr.Add(subStr);
				subStr.Empty();
			}
		}
	}

	if(strArr.GetCount()==0)
	{
		MyMessageBox(_T("请输入号码！"));	
		return false;
	}
	return true;
}


//添加号码
void CQiXingCai::OnBnClickedBtnChqAdd()
{
//	m_DlgStatus.ShowStatusWindow(TEXT("正在添加数据，请稍候..."));
	CString strWan = GetWanString();
	CString strQian = GetQianString();
	CString strBai = GetBaiString();
	CString strShi = GetShiString();
	CString strGe = GetGeString();
	CString strHezhi = GetHezhiString();
	CString strNiuNiu = GetNiuString();
	CString strNiuNum = GetNiuNumString();
	CString strHaoMa;
	int zhushu = 1;
	if (QiXingCai_LiangDing == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,3,2))
			{
				CString strTips;
				strTips.Format(L"您的号码输入有误，请重新输入！");
				MyMessageBox(strTips);
				m_DlgStatus.HideStatusWindow();

				return;
			}

			int nLength = strHaoMa.GetLength();
			if(nLength > 4800)
			{
				MyMessageBox(L"你输入的号码超过限制注数，请重新输入！");
				m_DlgStatus.HideStatusWindow();
				return;
			}
			zhushu = 1;

		}
		else
		{
			int nTempHaoMaCount =0;
			if (!strQian.IsEmpty())//千
				nTempHaoMaCount+=1;
			if (!strBai.IsEmpty())//百
				nTempHaoMaCount+=1;
			if (!strShi.IsEmpty())//十
				nTempHaoMaCount+=1;
			if (!strGe.IsEmpty())//个
				nTempHaoMaCount+=1;


			if(nTempHaoMaCount<2) 
			{
				MyMessageBox(_T("您选择的号码个数太少"));
				m_DlgStatus.HideStatusWindow();

				return;
			}
			if(nTempHaoMaCount>2) 
			{
				MyMessageBox(_T("您选择的号码个数太多"));
				m_DlgStatus.HideStatusWindow();

				return;
			}

			strHaoMa = strQian + _T(",") + strBai + _T(",") + strShi + _T(",") + strGe;


			zhushu = _MAX(strQian.GetLength(),1) * _MAX(strBai.GetLength(),1) * _MAX(strShi.GetLength(),1) * _MAX(strGe.GetLength(),1);
		}
	}
	else if (QiXingCai_SanDing == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,3,3))
			{
				CString strTips;
				strTips.Format(L"您的号码输入有误，请重新输入！");
				MyMessageBox(strTips);
				m_DlgStatus.HideStatusWindow();

				return;
			}

			int nLength = strHaoMa.GetLength();
			if(nLength > 4800)
			{
				MyMessageBox(L"你输入的号码超过限制注数，请重新输入！");
				m_DlgStatus.HideStatusWindow();
				return;
			}
			zhushu = 1;

		}
		else
		{
			int nTempHaoMaCount =0;
			if (!strQian.IsEmpty())//千
				nTempHaoMaCount+=1;
			if (!strBai.IsEmpty())//百
				nTempHaoMaCount+=1;
			if (!strShi.IsEmpty())//十
				nTempHaoMaCount+=1;
			if (!strGe.IsEmpty())//个
				nTempHaoMaCount+=1;


			if(nTempHaoMaCount<3) 
			{
				MyMessageBox(_T("您选择的号码个数太少"));
				m_DlgStatus.HideStatusWindow();

				return;
			}
			if(nTempHaoMaCount>3) 
			{
				MyMessageBox(_T("您选择的号码个数太多"));
				m_DlgStatus.HideStatusWindow();

				return;
			}

			strHaoMa = strQian + _T(",") + strBai + _T(",") + strShi + _T(",") + strGe;

			zhushu = _MAX(strQian.GetLength(),1) * _MAX(strBai.GetLength(),1) * _MAX(strShi.GetLength(),1) * _MAX(strGe.GetLength(),1);
		}


	}
	else if (QiXingCai_SiDing == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,3,4))
			{
				CString strTips;
				strTips.Format(L"您的号码输入有误，请重新输入！");
				MyMessageBox(strTips);
				m_DlgStatus.HideStatusWindow();

				return;
			}

			int nLength = strHaoMa.GetLength();
			if(nLength > 4800)
			{
				MyMessageBox(L"你输入的号码超过限制注数，请重新输入！");
				m_DlgStatus.HideStatusWindow();
				return;
			}
			zhushu = 1;

		}
		else
		{
			int nTempHaoMaCount =0;
			if (!strQian.IsEmpty())//千
				nTempHaoMaCount+=1;
			if (!strBai.IsEmpty())//百
				nTempHaoMaCount+=1;
			if (!strShi.IsEmpty())//十
				nTempHaoMaCount+=1;
			if (!strGe.IsEmpty())//个
				nTempHaoMaCount+=1;


			if(nTempHaoMaCount<4) 
			{
				MyMessageBox(_T("您选择的号码个数太少"));
				m_DlgStatus.HideStatusWindow();

				return;
			}

			strHaoMa = strQian + _T(",") + strBai + _T(",") + strShi + _T(",") + strGe ;

			zhushu = _MAX(strQian.GetLength(),1) * _MAX(strBai.GetLength(),1) * _MAX(strShi.GetLength(),1) * _MAX(strGe.GetLength(),1);
		}

	}
	else if (QiXingCai_YiDing == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaoma(strHaoMa,3,1))
			{
				CString strTips;
				strTips.Format(L"您的号码输入有误，请重新输入！");
				MyMessageBox(strTips);
				m_DlgStatus.HideStatusWindow();

				return;
			}

			int nLength = strHaoMa.GetLength();
			if(nLength > 4800)
			{
				MyMessageBox(L"你输入的号码超过限制注数，请重新输入！");
				m_DlgStatus.HideStatusWindow();
				return;
			}
			zhushu = 1;

		}
		else
		{
			int nTempHaoMaCount =0;
			if (!strQian.IsEmpty())//千
				nTempHaoMaCount+=1;
			if (!strBai.IsEmpty())//百
				nTempHaoMaCount+=1;
			if (!strShi.IsEmpty())//十
				nTempHaoMaCount+=1;
			if (!strGe.IsEmpty())//个
				nTempHaoMaCount+=1;


			if(nTempHaoMaCount<1) 
			{
				MyMessageBox(_T("您选择的号码个数太少"));
				m_DlgStatus.HideStatusWindow();

				return;
			}
			if(nTempHaoMaCount>1) 
			{
				MyMessageBox(_T("您选择的号码个数太多"));
				m_DlgStatus.HideStatusWindow();

				return;
			}

			strHaoMa = strQian + _T(",") + strBai + _T(",") + strShi + _T(",") + strGe;

			zhushu = _MAX(strQian.GetLength(),1) * _MAX(strBai.GetLength(),1) * _MAX(strShi.GetLength(),1) * _MAX(strGe.GetLength(),1);
		}
	}
	else if (QiXingCai_LiangZiXian == m_gameKind) 
	{

		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaomaXian(strHaoMa,2,4))
			{
				CString strTips;
				strTips.Format(L"您的号码输入有误，请重新输入！");
				MyMessageBox(strTips);
				m_DlgStatus.HideStatusWindow();
				return;
			}

			int nLength = strHaoMa.GetLength();
			if(nLength > 4800)
			{
				MyMessageBox(L"你输入的号码超过限制注数，请重新输入！");
				m_DlgStatus.HideStatusWindow();
				return;
			}
			zhushu = nLength/3;
		}
		else
		{
			if(strBai.GetLength()<2) 
			{
				MyMessageBox(_T("您选择的号码个数太少"));
				m_DlgStatus.HideStatusWindow();

				return;
			}
			if(strBai.GetLength()>2) 
			{
				MyMessageBox(_T("您选择的号码个数太多"));
				m_DlgStatus.HideStatusWindow();

				return;
			}
			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);

				if (i != strBai.GetLength()-1)
				{
					strHaoMa += _T(",");
				}
			}
			zhushu = 1;
		}
	}
	else if (QiXingCai_SanZiXian == m_gameKind) 
	{
		if(m_RenxuanKeyboardInput)
		{
			if(!GetKeyBoardHaomaXian(strHaoMa,3,4))
			{
				CString strTips;
				strTips.Format(L"您的号码输入有误，请重新输入！");
				MyMessageBox(strTips);
				m_DlgStatus.HideStatusWindow();
				return;
			}

			int nLength = strHaoMa.GetLength();
			if(nLength > 4800)
			{
				MyMessageBox(L"你输入的号码超过限制注数，请重新输入！");
				m_DlgStatus.HideStatusWindow();
				return;
			}
			zhushu = nLength/4;
		}
		else
		{
			if(strBai.GetLength()<3) 
			{
				MyMessageBox(_T("您选择的号码个数太少"));
				m_DlgStatus.HideStatusWindow();
				return;
			}
			if(strBai.GetLength()>3) 
			{
				MyMessageBox(_T("您选择的号码个数太多"));
				m_DlgStatus.HideStatusWindow();
				return;
			}
			for(int i = 0;i < strBai.GetLength();i++)
			{
				strHaoMa += strBai.GetAt(i);
				if (i != strBai.GetLength()-1)
				{
					strHaoMa += _T(",");
				}
			}
			zhushu = 1;
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

				if((!m_strHaoma.IsEmpty())||(m_zongZhuShu!=0))
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

				int nIndex = 0;
				CString strHaomaTemp;
				int nZongzhushu = 0;

				if(m_gameKind == WuxingZhixuan)
				{
					nZongzhushu += strHaoMa.GetLength()/6;
				}
				else if(m_gameKind == RenXuan4)
				{
					BOOL	 bWan=false, bQian=false, bBai=false, bShi=false, bGe=false;
					int nCount=GetRenXuanCheckBox(bWan, bQian, bBai, bShi, bGe);

					int nZhushu = strHaoMa.GetLength()/5;
					nZongzhushu = nZhushu *nRenXuan4[nCount];
				}
				else
				{
					do
					{

						if(!AfxExtractSubString(strHaomaTemp, strHaoMa, nIndex++, '|'))
							break;
						if(strHaomaTemp.IsEmpty())
							break;

						nZongzhushu ++;

					}
					while(nIndex<80000);

				}
				int nCount = m_listChqSSC.GetItemCount();
				m_listChqSSC.InsertItem(nCount, L"号码添加成功");

				m_listChqSSC.SetItemText(nCount, 1, GetScoreMoshi());
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
				DOUBLE fJine = 0.0f;
				fJine = nZongzhushu*m_beishu*danzhujine;
				CString strZongzhu ;
				strZongzhu.Format(L"￥%.3lf",fJine);
				m_listChqSSC.SetItemText(nCount, 2, strZongzhu);


				m_listChqSSC.SetItemText(nCount, 3, GetWanDesc());
				//CString strZongzhu ;
				strZongzhu.Format(L"%d",nZongzhushu);
				m_listChqSSC.SetItemText(nCount, 4, strZongzhu);
				strZongzhu.Format(_T("%d"), m_beishu);
				m_listChqSSC.SetItemText(nCount, 5, strZongzhu);
				m_listChqSSC.createItemButton(nCount, 6, this->GetSafeHwnd());

				m_zongZhuShu += nZongzhushu;
			}
			else
			{
				int nCount = m_listChqSSC.GetItemCount();
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

					m_listChqSSC.SetItemText(nCount, 1, GetScoreMoshi());
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
					DOUBLE fJine = 0.0f;
					fJine = zhushu*m_beishu*danzhujine;
					CString strZongzhu ;
					strZongzhu.Format(L"￥%.3lf",fJine);
					m_listChqSSC.SetItemText(nCount, 2, strZongzhu);

					m_listChqSSC.SetItemText(nCount, 3, GetWanDesc());


					zhushu = 1;


					CString strTmp;
					strTmp.Format(_T("%d"), zhushu);
					m_listChqSSC.SetItemText(nCount, 4, strTmp);
					strTmp.Format(_T("%d"), m_beishu);
					m_listChqSSC.SetItemText(nCount, 5, strTmp);
					m_listChqSSC.createItemButton(nCount++, 6, this->GetSafeHwnd());

					m_zongZhuShu += zhushu;

				}
				while(nIndex < 80000);

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

			m_listChqSSC.InsertItem(nCount, strHaoMa);
			m_listChqSSC.SetItemText(nCount, 1, GetScoreMoshi());
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
			DOUBLE fJine = 0.0f;
			fJine = zhushu*m_beishu*danzhujine;
			CString strZongzhu ;
			strZongzhu.Format(L"￥%.3lf",fJine);
			m_listChqSSC.SetItemText(nCount, 2, strZongzhu);

			m_listChqSSC.SetItemText(nCount, 3, GetWanDesc());
			CString strTmp;
			strTmp.Format(_T("%d"), zhushu);
			m_listChqSSC.SetItemText(nCount, 4, strTmp);
			strTmp.Format(_T("%d"), m_beishu);
			m_listChqSSC.SetItemText(nCount, 5, strTmp);
			m_listChqSSC.createItemButton(nCount, 6, this->GetSafeHwnd());
			CString strMD5Haoma;
			for(int i = 0;i < vecAllNums.size();i++)
			{
				strMD5Haoma += vecAllNums[i].c_str();
				strMD5Haoma += L"|";
			}

			memset(m_szMD5Haoma, 0, sizeof(m_szMD5Haoma));
			CMD5Encrypt::EncryptData(strMD5Haoma,m_szMD5Haoma);

			m_zongZhuShu += zhushu;
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
			if((nLen!=0)||(m_zongZhuShu!=0))
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

			int nCount = m_listChqSSC.GetItemCount();

			m_listChqSSC.InsertItem(nCount, L"号码添加成功");

			m_listChqSSC.SetItemText(nCount, 1, GetScoreMoshi());
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
			DOUBLE fJine = 0.0f;
			m_zongZhuShu += (arrLen*zhushu);
			fJine = m_zongZhuShu*m_beishu*danzhujine;
			CString strZongzhu ;
			strZongzhu.Format(L"￥%.3lf",fJine);
			m_listChqSSC.SetItemText(nCount, 2, strZongzhu);

			m_listChqSSC.SetItemText(nCount, 3, GetWanDesc());

			CString strTmp;
			strTmp.Format(_T("%d"), m_zongZhuShu);
			m_listChqSSC.SetItemText(nCount, 4, strTmp);
			strTmp.Format(_T("%d"), m_beishu);
			m_listChqSSC.SetItemText(nCount, 5, strTmp);
			m_listChqSSC.createItemButton(nCount, 6, this->GetSafeHwnd());
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
				m_listChqSSC.SetItemText(nCount+i, 1, GetScoreMoshi());
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
				DOUBLE fJine = 0.0f;
				fJine = zhushu*m_beishu*danzhujine;
				CString strZongzhu ;
				strZongzhu.Format(L"￥%.3lf",fJine);
				m_listChqSSC.SetItemText(nCount+i, 2, strZongzhu);


				m_listChqSSC.SetItemText(nCount+i, 3, GetWanDesc());
				CString strTmp;
				strTmp.Format(_T("%d"), zhushu);
				m_listChqSSC.SetItemText(nCount+i, 4, strTmp);
				strTmp.Format(_T("%d"), m_beishu);
				m_listChqSSC.SetItemText(nCount+i, 5, strTmp);
				m_listChqSSC.createItemButton(nCount+i, 6, this->GetSafeHwnd());
				wstring strdata;
				strdata = strArrAt.GetBuffer();
				vecAllNums.insert(vecAllNums.begin(),1,strdata);//.push_back(strdata);
			}


			m_zongZhuShu += (arrLen*zhushu);
			strArr.RemoveAll();
			strArrAt.ReleaseBuffer();

			memset(&g_arr, 0, sizeof(g_arr));
			memset(&g_nPos, 0, sizeof(g_nPos));
			g_Counts=0;
		}

	}


	int nItemCount = m_listChqSSC.GetItemCount();
	m_zongJine = 0.00f;
	for(int i = 0;i < nItemCount;i++)
	{
		CString strBeiSHu = m_listChqSSC.GetItemText(i,5);
		CString strZhuSHu = m_listChqSSC.GetItemText(i,4);
		CString strMoshi = m_listChqSSC.GetItemText(i,1);
		int beishu = _ttoi(strBeiSHu);
		int zhushu = _ttoi(strZhuSHu);

		int nScoreType = GetMoshiRet(strMoshi);
		double danzhujine = 2.0;

		if(nScoreType == MoShi_Yuan)
		{	
			danzhujine = 2.0;
		}
		else if (nScoreType == MoShi_Jiao)
		{
			danzhujine = 0.2;
		}
		else if(nScoreType == MoShi_Fen)
		{
			danzhujine = 0.02;
		}
		else if(nScoreType == MoShi_Li)
			danzhujine = 0.002;

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
CString CQiXingCai::GetScoreMoshi()
{
	if(theAccount.Scoretype == MoShi_Yuan)
	{
		return _T("元");
	}
	else 	if(theAccount.Scoretype == MoShi_Jiao)
	{
		return _T("角");
	}
	else 	if(theAccount.Scoretype == MoShi_Fen)
	{
		return _T("分");
	}
	else 	if(theAccount.Scoretype == MoShi_Li)
	{
		return _T("厘");
	}

	return _T("元");
}
int CQiXingCai::GetMoshiRet(CString moshi)
{
	if(moshi == _T("元"))
	{
		return MoShi_Yuan;
	}
	else if(moshi == _T("角"))
	{
		return MoShi_Jiao;
	}
	else if(moshi == _T("分"))
	{
		return MoShi_Fen;
	}
	else if (moshi == _T("厘"))
	{
		return MoShi_Li;
	}

	return MoShi_Yuan;

}

//初始化号码按钮
void CQiXingCai::InitNumberBtns()
{
	
	m_btnWan_0.SetImage(m_numImageList, 0, 10);
	m_btnWan_1.SetImage(m_numImageList, 1, 10);
	m_btnWan_2.SetImage(m_numImageList, 2, 10);
	m_btnWan_3.SetImage(m_numImageList, 3, 10);
	m_btnWan_4.SetImage(m_numImageList, 4, 10);
	m_btnWan_5.SetImage(m_numImageList, 5, 10);
	m_btnWan_6.SetImage(m_numImageList, 6, 10);
	m_btnWan_7.SetImage(m_numImageList, 7, 10);
	m_btnWan_8.SetImage(m_numImageList, 8, 10);
	m_btnWan_9.SetImage(m_numImageList, 9, 10);

	m_btnQian_0.SetImage(m_numImageList, 0, 10);
	m_btnQian_1.SetImage(m_numImageList, 1, 10);
	m_btnQian_2.SetImage(m_numImageList, 2, 10);
	m_btnQian_3.SetImage(m_numImageList, 3, 10);
	m_btnQian_4.SetImage(m_numImageList, 4, 10);
	m_btnQian_5.SetImage(m_numImageList, 5, 10);
	m_btnQian_6.SetImage(m_numImageList, 6, 10);
	m_btnQian_7.SetImage(m_numImageList, 7, 10);
	m_btnQian_8.SetImage(m_numImageList, 8, 10);
	m_btnQian_9.SetImage(m_numImageList, 9, 10);

	m_btnBai_0.SetImage(m_numImageList, 0, 10);
	m_btnBai_1.SetImage(m_numImageList, 1, 10);
	m_btnBai_2.SetImage(m_numImageList, 2, 10);
	m_btnBai_3.SetImage(m_numImageList, 3, 10);
	m_btnBai_4.SetImage(m_numImageList, 4, 10);
	m_btnBai_5.SetImage(m_numImageList, 5, 10);
	m_btnBai_6.SetImage(m_numImageList, 6, 10);
	m_btnBai_7.SetImage(m_numImageList, 7, 10);
	m_btnBai_8.SetImage(m_numImageList, 8, 10);
	m_btnBai_9.SetImage(m_numImageList, 9, 10);

	m_btnShi_0.SetImage(m_numImageList, 0, 10);
	m_btnShi_1.SetImage(m_numImageList, 1, 10);
	m_btnShi_2.SetImage(m_numImageList, 2, 10);
	m_btnShi_3.SetImage(m_numImageList, 3, 10);
	m_btnShi_4.SetImage(m_numImageList, 4, 10);
	m_btnShi_5.SetImage(m_numImageList, 5, 10);
	m_btnShi_6.SetImage(m_numImageList, 6, 10);
	m_btnShi_7.SetImage(m_numImageList, 7, 10);
	m_btnShi_8.SetImage(m_numImageList, 8, 10);
	m_btnShi_9.SetImage(m_numImageList, 9, 10);

	m_btnGe_0.SetImage(m_numImageList, 0, 10);
	m_btnGe_1.SetImage(m_numImageList, 1, 10);
	m_btnGe_2.SetImage(m_numImageList, 2, 10);
	m_btnGe_3.SetImage(m_numImageList, 3, 10);
	m_btnGe_4.SetImage(m_numImageList, 4, 10);
	m_btnGe_5.SetImage(m_numImageList, 5, 10);
	m_btnGe_6.SetImage(m_numImageList, 6, 10);
	m_btnGe_7.SetImage(m_numImageList, 7, 10);
	m_btnGe_8.SetImage(m_numImageList, 8, 10);
	m_btnGe_9.SetImage(m_numImageList, 9, 10);

	//m_staticZuXuan.SetImage(m_bmpNumTip, 0, 8);
	m_staticBuDingWei.SetImage(m_bmpNumTip, 1, 10);
	m_staticWan.SetImage(m_bmpNumTip, 2, 10);
	m_staticQian.SetImage(m_bmpNumTip, 3, 10);
	m_staticBai.SetImage(m_bmpNumTip, 4, 10);
	m_staticShi.SetImage(m_bmpNumTip, 5, 10);
	m_staticGe.SetImage(m_bmpNumTip, 6, 10);
	//m_staticNiu.SetImage(m_bmpNumTip, 7, 8);

	//m_staticZu120.SetImage(m_bmpNumTip1, 0, 6);
	//m_staticDanHao.SetImage(m_bmpNumTip1, 1, 6);
	//m_staticErChong.SetImage(m_bmpNumTip1, 2, 6);
	//m_staticErChong1.SetImage(m_bmpNumTip1, 2, 6);
	//m_staticSanChong.SetImage(m_bmpNumTip1, 3, 6);
	//m_staticSiChong.SetImage(m_bmpNumTip1, 4, 6);
	//m_staticXuanHao.SetImage(m_bmpNumTip1, 5, 6);


	//m_btnDaShi.SetImage(m_bmpDxdsh, 0, 4);
	//m_btnXiaoShi.SetImage(m_bmpDxdsh, 1, 4);
	//m_btnDanShi.SetImage(m_bmpDxdsh, 2, 4);
	//m_btnShuangShi.SetImage(m_bmpDxdsh, 3, 4);
	//m_btnDaGe.SetImage(m_bmpDxdsh, 0, 4);
	//m_btnXiaoGe.SetImage(m_bmpDxdsh, 1, 4);
	//m_btnDanGe.SetImage(m_bmpDxdsh, 2, 4);
	//m_btnShuangGe.SetImage(m_bmpDxdsh, 3, 4);

	CString numTextBk = CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx.png");
	m_btnWanQuan.SetBkImage(numTextBk);
	m_btnWanDa.SetBkImage(numTextBk);
	m_btnWanXiao.SetBkImage(numTextBk);
	m_btnWanDan.SetBkImage(numTextBk);
	m_btnWanShuang.SetBkImage(numTextBk);
	m_btnWanQing.SetBkImage(numTextBk);

	m_btnQianQuan.SetBkImage(numTextBk);
	m_btnQianDa.SetBkImage(numTextBk);
	m_btnQianXiao.SetBkImage(numTextBk);
	m_btnQianDan.SetBkImage(numTextBk);
	m_btnQianShuang.SetBkImage(numTextBk);
	m_btnQianQing.SetBkImage(numTextBk);

	m_btnBaiQuan.SetBkImage(numTextBk);
	m_btnBaiDa.SetBkImage(numTextBk);
	m_btnBaiXiao.SetBkImage(numTextBk);
	m_btnBaiDan.SetBkImage(numTextBk);
	m_btnBaiShuang.SetBkImage(numTextBk);
	m_btnBaiQing.SetBkImage(numTextBk);

	m_btnNiuQuan.SetBkImage(numTextBk);
	m_btnNiuDa.SetBkImage(numTextBk);
	m_btnNiuXiao.SetBkImage(numTextBk);
	m_btnNiuDan.SetBkImage(numTextBk);
	m_btnNiuShuang.SetBkImage(numTextBk);
	m_btnNiuQing.SetBkImage(numTextBk);

	m_btnShiQuan.SetBkImage(numTextBk);
	m_btnShiDa.SetBkImage(numTextBk);
	m_btnShiXiao.SetBkImage(numTextBk);
	m_btnShiDan.SetBkImage(numTextBk);
	m_btnShiShuang.SetBkImage(numTextBk);
	m_btnShiQing.SetBkImage(numTextBk);

	m_btnGeQuan.SetBkImage(numTextBk);
	m_btnGeDa.SetBkImage(numTextBk);
	m_btnGeXiao.SetBkImage(numTextBk);
	m_btnGeDan.SetBkImage(numTextBk);
	m_btnGeShuang.SetBkImage(numTextBk);
	m_btnGeQing.SetBkImage(numTextBk);

	m_btnWanQuan.SetTextColor(RGB(97,76,59));
	m_btnWanDa.SetTextColor(RGB(97,76,59));
	m_btnWanXiao.SetTextColor(RGB(97,76,59));
	m_btnWanDan.SetTextColor(RGB(97,76,59));
	m_btnWanShuang.SetTextColor(RGB(97,76,59));
	m_btnWanQing.SetTextColor(RGB(97,76,59));

	m_btnQianQuan.SetTextColor(RGB(97,76,59));
	m_btnQianDa.SetTextColor(RGB(97,76,59));
	m_btnQianXiao.SetTextColor(RGB(97,76,59));
	m_btnQianDan.SetTextColor(RGB(97,76,59));
	m_btnQianShuang.SetTextColor(RGB(97,76,59));
	m_btnQianQing.SetTextColor(RGB(97,76,59));

	m_btnBaiQuan.SetTextColor(RGB(97,76,59));
	m_btnBaiDa.SetTextColor(RGB(97,76,59));
	m_btnBaiXiao.SetTextColor(RGB(97,76,59));
	m_btnBaiDan.SetTextColor(RGB(97,76,59));
	m_btnBaiShuang.SetTextColor(RGB(97,76,59));
	m_btnBaiQing.SetTextColor(RGB(97,76,59));

	m_btnShiQuan.SetTextColor(RGB(97,76,59));
	m_btnShiDa.SetTextColor(RGB(97,76,59));
	m_btnShiXiao.SetTextColor(RGB(97,76,59));
	m_btnShiDan.SetTextColor(RGB(97,76,59));
	m_btnShiShuang.SetTextColor(RGB(97,76,59));
	m_btnShiQing.SetTextColor(RGB(97,76,59));

	m_btnGeQuan.SetTextColor(RGB(97,76,59));
	m_btnGeDa.SetTextColor(RGB(97,76,59));
	m_btnGeXiao.SetTextColor(RGB(97,76,59));
	m_btnGeDan.SetTextColor(RGB(97,76,59));
	m_btnGeShuang.SetTextColor(RGB(97,76,59));
	m_btnGeQing.SetTextColor(RGB(97,76,59));


}

void CQiXingCai::InitListCtrl()
{
	m_listChqSSC.SetExtendedStyle(m_listChqSSC.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);

	m_listChqSSC.ModifyStyle(0, LVS_NOCOLUMNHEADER);

	m_listChqSSC.SetPenColor(RGB(247,233,216));
	m_listChqSSC.InsertColumn(0, _T("号码"), LVCFMT_CENTER, 240);
	m_listChqSSC.InsertColumn(1, _T("模式"), LVCFMT_CENTER, 25);
	m_listChqSSC.InsertColumn(2, _T("金额"), LVCFMT_CENTER, 95);
	m_listChqSSC.InsertColumn(3, _T("玩法"), LVCFMT_CENTER, 80);
	m_listChqSSC.InsertColumn(4, _T("注法"), LVCFMT_CENTER, 55);
	m_listChqSSC.InsertColumn(5, _T("倍数"), LVCFMT_CENTER, 50);
	m_listChqSSC.InsertColumn(6, _T("删除"), LVCFMT_CENTER, 31);

}
//两定玩法
void CQiXingCai::OnBnClickedRioLiangDing()
{
	ResetAllNums();

	m_gameKind = QiXingCai_LiangDing;

	CheckWanFaBtn(m_btnLiangDing);

	SendToServer(3);
	SetLockText();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);


	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);


	HideDaXiaoDanShuang();
	HideHeZhiNums();
	HideWanNums();//隐藏万
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();

	HideDanShiEdit();
}
//三定
void CQiXingCai::OnBnClickedRioSanDing()
{
	ResetAllNums();
	m_gameKind = QiXingCai_SanDing;

	CheckWanFaBtn(m_btnSanDing);

	SendToServer(3);
	SetLockText();


	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);

	HideDaXiaoDanShuang();
	HideHeZhiNums();
	HideWanNums();//隐藏万
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();


	HideDanShiEdit();
}
//四定
void CQiXingCai::OnBnClickedRioSiDing()
{
	ResetAllNums();
	m_gameKind = QiXingCai_SiDing;

	CheckWanFaBtn(m_btnSiDing);

	SendToServer(3);
	SetLockText();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);

	HideDaXiaoDanShuang();
	HideHeZhiNums();
	HideWanNums();//隐藏万
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();

	HideDanShiEdit();
}
//一定
void CQiXingCai::OnBnClickedRioYiDing()
{
	ResetAllNums();

	m_gameKind = QiXingCai_YiDing;

	CheckWanFaBtn(m_btnYiDing);


	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);

	HideDaXiaoDanShuang();
	HideHeZhiNums();
	HideNiuNums();
	HideWanNums();//隐藏万
	ShowQianNums();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();

	HideDanShiEdit();
}

//两字现
void CQiXingCai::OnBnClickedRioLiangZiXian()
{
	ResetAllNums();

	m_gameKind = QiXingCai_LiangZiXian;

	CheckWanFaBtn(m_btnLiangZiXian);

	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);

	HideDaXiaoDanShuang();
	HideHeZhiNums();
	HideNiuNums();
	HideWanNums();//隐藏万
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	m_staticBai.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_SHOW);

	HideDanShiEdit();
}

//三字现
void CQiXingCai::OnBnClickedRioSanZiXian()
{
	ResetAllNums();

	m_gameKind = QiXingCai_SanZiXian;
	CheckWanFaBtn(m_btnSanZiXian);


	SendToServer(3);
	SetLockText();
	HideNiuNums();

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);

	HideDaXiaoDanShuang();
	HideHeZhiNums();
	HideNiuNums();
	HideWanNums();//隐藏万
	HideQianNums();
	ShowBaiNums();
	HideShiNums();
	HideGeNums();

	m_staticBai.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_SHOW);

	HideDanShiEdit();
}

//追号
void CQiXingCai::OnBnClickedBtnZhuihao()
{
	if(!CheckInput())
		return ;
	CDlgAddtional	dlgZhuiHao;

	CString strBeishu ;
	strBeishu.Format(L"%d",m_beishu);

	dlgZhuiHao.SetCurrentQihaoTime(m_chqRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,4,3, m_zongJine,156,CZ_QiXingCai);
	if(dlgZhuiHao.DoModal() == IDOK)
	{
		//判断是否临近开奖点
		long kjing = m_chqRule.GetKjShjDiff(); 
		if (kjing < 60)								// || kjing > 540
		{
			MyMessageBox(_T("此期封单,请稍后再试"));
			return ;
		}

		m_nZhuihaoCount = dlgZhuiHao.GetZhuiHaoQiShu();
		dlgZhuiHao.GetZhuiHaoData(vecData1, vecData2, vecData3);


		m_bZhuiZjtz = dlgZhuiHao.GetPrizeStop();

		DOUBLE fZongJine = 0.0;
		for(int i = 0;i < m_nZhuihaoCount;i++)
		{
			int nBeishu = _ttoi(vecData1[i].c_str());
			fZongJine += m_zongJine*nBeishu;
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
	m_zongZhuShu = 0;

	m_beishu = 1;
	//UpdateData(FALSE);
OnBnClickedBtnClsList();
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
}

//校验输入
bool	CQiXingCai::CheckInput()
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

	if(theAccount.yue < m_zongJine)
	{
		MyMessageBox(_T("您余额不足，请充值后再试"));
		return false;
	}
	//判断是否临近开奖点
	long kjing = m_chqRule.GetKjShjDiff(); 
	if (kjing < 60)								// || kjing > 540
	{
		MyMessageBox(_T("此期封单,请稍后再试"));
		return false;
	}

	return true;
}
//普通下注
void CQiXingCai::OnBnClickedBtnTouzhu()
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
	qrDlg.m_gamekind = _T("七星彩");
	qrDlg.m_zhushu = m_zongZhuShu * m_beishu;
	qrDlg.m_moshi = theAccount.Scoretype;
	if (qrDlg.DoModal() != IDOK)
	{
		return;
	}

	//判断是否临近开奖点
	long kjing = m_chqRule.GetKjShjDiff(); 
	if (kjing < 60)								// || kjing > 540
	{
		MyMessageBox(_T("此期封单,请稍后再试"));
		return ;
	}

	bool bSucc = true;
	int itm_cnt = m_listChqSSC.GetItemCount();
	m_nTouzhuCount=0;
	m_nTouzhuCount = itm_cnt;

	SendToServer(1);

	OnBnClickedBtnClsList();

	m_zongZhuShu = 0;

	m_beishu = 1;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);

	InvalidateRect(&rcZongJinE);

}

void CQiXingCai::OnBnClickedBtnDelSel()
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
		m_zongZhuShu -= zhushu;
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

}

void CQiXingCai::OnBnClickedBtnClsList()
{
	m_zongZhuShu = 0;
	m_zongJine=0;

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
	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);

	InvalidateRect(&rcZongJinE);
}

void CQiXingCai::OnEnChangeEditDanshiInput()
{
	//m_editDanShiHaoMa.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
	m_richDanshiHaoma.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
}

void CQiXingCai::OnEnChangeEditBeishu()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString str;
	m_editBeiTou.GetWindowText(str);
	if ( (_ttoi(str) < 1)) 
	{
		if(!str.IsEmpty())
		{
			MyMessageBox(_T("投注倍数必须是一个正整数"));
			m_beishu = 1;
			UpdateData(FALSE);
		}
	}
	else
	{
		UpdateData();
		GetQXCZhushu(CZ_QiXingCai,m_gameKind);
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

void CQiXingCai::CheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(true);
}

void CQiXingCai::UnCheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(false);
}

bool CQiXingCai::IsNumBtnCheck(CNumerButton& btn)
{
	return btn.IsSelect();
}

void CQiXingCai::HideCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}
void CQiXingCai::ShowCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_SHOW);
	}
}

void CQiXingCai::HideAllRio()
{
	////任选2,3,4
	//HideCtrl(IDC_RIO_RENXUAN2);
	//HideCtrl(IDC_RIO_RENXUAN3);
	//HideCtrl(IDC_RIO_RENXUAN4);

	////后三
	//HideCtrl(IDC_RIO_HSZHX_FUSHI);
	//HideCtrl(IDC_RIO_HSZHX_DANSHI);

	//HideCtrl(IDC_RIO_YFFS);				//一帆风顺
	//HideCtrl(IDC_RIO_HSCS);				//好事成双
	//HideCtrl(IDC_RIO_SXBX);				//三星报喜
	//HideCtrl(IDC_RIO_SJFC);				//四季发财

	//HideCtrl(IDC_RIO_WXZX);				//五星直选
	//HideCtrl(IDC_RIO_WXQS);				//五星前四
	//HideCtrl(IDC_RIO_WXHS);				//五星后四
	//HideCtrl(IDC_RIO_WXZS);				//五星中三
	//HideCtrl(IDC_RIO_QS_HEZHI);				//五星后四
	//HideCtrl(IDC_RIO_HS_HEZHI);				//五星中三
	//HideCtrl(IDC_RIO_NS);				//牛数
	//HideCtrl(IDC_RIO_NN);				//牛牛
	//HideCtrl(IDC_RIO_WN);				//无牛
	//HideCtrl(IDC_STATIC_NIUNIU_TIP);				//牛数

	////前三
	//HideCtrl(IDC_RIO_QSZHX_FUSHI);
	//HideCtrl(IDC_RIO_QSZHX_DANSHI);

	//HideCtrl(IDC_RIO_HSZS_FUSHI);
	//HideCtrl(IDC_RIO_HSZS_DANSHI);
	//HideCtrl(IDC_RIO_HSZL_FUSHI);
	//HideCtrl(IDC_RIO_HSZL_DANSHI);

	//HideCtrl(IDC_RIO_QSZS_FUSHI);
	//HideCtrl(IDC_RIO_QSZS_DANSHI);
	//HideCtrl(IDC_RIO_QSZL_FUSHI);
	//HideCtrl(IDC_RIO_QSZL_DANSHI);

	//HideCtrl(IDC_RIO_ZSZS_FUSHI);
	//HideCtrl(IDC_RIO_QSZS_DANSHI);
	//HideCtrl(IDC_RIO_ZSZL_FUSHI);

	//HideCtrl(IDC_RIO_HEZHX_FUSHI);
	//HideCtrl(IDC_RIO_HEZHX_DANSHI);

	//HideCtrl(IDC_RIO_QEZHX_FUSHI);
	//HideCtrl(IDC_RIO_QEZHX_DANSHI);

	//HideCtrl(IDC_RIO_HEZX_FUSHI);
	//HideCtrl(IDC_RIO_HEZX_DANSHI);

	//HideCtrl(IDC_RIO_QEZX_FUSHI);
	//HideCtrl(IDC_RIO_QEZX_DANSHI);

	//HideCtrl(IDC_RIO_ZX_120);
	//HideCtrl(IDC_RIO_ZX_60);
	//HideCtrl(IDC_RIO_ZX_30);
	//HideCtrl(IDC_RIO_ZX_20);
	//HideCtrl(IDC_RIO_ZX_10);
	//HideCtrl(IDC_RIO_ZX_5);

	//HideCtrl(IDC_RIO_YFFS);
	//HideCtrl(IDC_RIO_HSCS);
	//HideCtrl(IDC_RIO_SXBX);
	//HideCtrl(IDC_RIO_SJFC);
	//HideCtrl(IDC_RIO_QS_HEZHI);				//五星后四
	//HideCtrl(IDC_RIO_HS_HEZHI);				//五星中三

	//HideCtrl(IDC_RIO_DXDSH);

	//HideCtrl(IDC_RIO_HSZHXHZH);

	//HideCtrl(IDC_RIO_HSBDW);
	//HideCtrl(IDC_RIO_QSBDW);
	//HideCtrl(IDC_RIO_HEBDW);
	//HideCtrl(IDC_RIO_QEBDW);

	//HideCtrl(IDC_RIO_DWD);

	//HideCtrl(IDC_RIO_QSHUNHE);
	//HideCtrl(IDC_RIO_ZSHUNHE);
	//HideCtrl(IDC_RIO_HSHUNHE);
	//HideCtrl(IDC_STATIC_HUNHE_TIP);
	////这个是鼠标输入OR键盘输入
	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);

	////任选2,3,4时，键盘输入的万，千，百，十，个
	//HideCtrl(IDC_CHK_WAN);
	//HideCtrl(IDC_CHK_QIAN);
	//HideCtrl(IDC_CHK_BAI);
	//HideCtrl(IDC_CHK_SHI);
	//HideCtrl(IDC_CHKGE);
	//HideCtrl(IDC_STATIC_RENXUAN_TIP);
}

//void CQiXingCai::SelctRadioBtn(int ctrl_id)
//{
//	CButton* pRadio = (CButton*)GetDlgItem(ctrl_id);
//	if (pRadio != NULL) 
//	{
//		pRadio->SetCheck(BST_CHECKED);
//	}
//}
CString CQiXingCai::GetNiuString(int nNiu)
{
	CString str;
	switch(nNiu)
	{
	case 0:
		{
			str = _T("牛牛");
			break;
		}
	case 1:
		{
			str = _T("牛一");
			break;
		}
	case 2:
		{
			str = _T("牛二");
			break;
		}
	case 3:
		{
			str = _T("牛三");
			break;
		}
	case 4:
		{
			str = _T("牛四");
			break;
		}
	case 5:
		{
			str = _T("牛五");
			break;
		}
	case 6:
		{
			str = _T("牛六");
			break;
		}
	case 7:
		{
			str = _T("牛七");
			break;
		}
	case 8:
		{
			str = _T("牛八");
			break;
		}
	case 9:
		{
			str = _T("牛九");
			break;
		}
	}

	return str;
}
CString CQiXingCai::GetNiuString()
{
	CString str;
	if (IsNumBtnCheck(m_btnNiu_0))
	{
		str += _T("无牛");
	}
	if (IsNumBtnCheck(m_btnNiu_1))
	{
		str += _T("牛一");
	}
	if (IsNumBtnCheck(m_btnNiu_2))
	{
		str += _T("牛二");
	}
	if (IsNumBtnCheck(m_btnNiu_3))
	{
		str += _T("牛三");
	}
	if (IsNumBtnCheck(m_btnNiu_4))
	{
		str += _T("牛四");
	}
	if (IsNumBtnCheck(m_btnNiu_5))
	{
		str += _T("牛五");
	}
	if (IsNumBtnCheck(m_btnNiu_6))
	{
		str += _T("牛六");
	}
	if (IsNumBtnCheck(m_btnNiu_7))
	{
		str += _T("牛七");
	}
	if (IsNumBtnCheck(m_btnNiu_8))
	{
		str += _T("牛八");
	}
	if (IsNumBtnCheck(m_btnNiu_9))
	{
		str += _T("牛九");
	}
	if (IsNumBtnCheck(m_btnNiu_10))
	{
		str += _T("牛牛");
	}

	return str;
}
CString CQiXingCai::GetNiuNumString()
{
	CString str;
	if (IsNumBtnCheck(m_btnNiu_0))
	{
		str += _T("00");
	}
	if (IsNumBtnCheck(m_btnNiu_1))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnNiu_2))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnNiu_3))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnNiu_4))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnNiu_5))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnNiu_6))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnNiu_7))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnNiu_8))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnNiu_9))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnNiu_10))
	{
		str += _T("10");
	}

	return str;
}
CString CQiXingCai::GetHezhiString()
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


CString CQiXingCai::GetWanString()
{
	CString str;
	if (IsNumBtnCheck(m_btnWan_0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btnWan_1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btnWan_2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btnWan_3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btnWan_4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btnWan_5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btnWan_6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btnWan_7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btnWan_8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btnWan_9))
	{
		str += _T("9");
	}
	return str;
}

CString CQiXingCai::GetQianString()
{
	CString str;
	if (IsNumBtnCheck(m_btnQian_0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btnQian_1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btnQian_2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btnQian_3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btnQian_4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btnQian_5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btnQian_6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btnQian_7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btnQian_8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btnQian_9))
	{
		str += _T("9");
	}
	return str;
}

CString CQiXingCai::GetBaiString()
{
	CString str;
	if (IsNumBtnCheck(m_btnBai_0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btnBai_1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btnBai_2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btnBai_3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btnBai_4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btnBai_5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btnBai_6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btnBai_7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btnBai_8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btnBai_9))
	{
		str += _T("9");
	}
	return str;
}

CString CQiXingCai::GetShiString()
{
	CString str;
	if (IsNumBtnCheck(m_btnShi_0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btnShi_1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btnShi_2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btnShi_3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btnShi_4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btnShi_5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btnShi_6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btnShi_7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btnShi_8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btnShi_9))
	{
		str += _T("9");
	}
	return str;
}

CString CQiXingCai::GetGeString()
{
	CString str;
	if (IsNumBtnCheck(m_btnGe_0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btnGe_1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btnGe_2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btnGe_3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btnGe_4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btnGe_5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btnGe_6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btnGe_7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btnGe_8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btnGe_9))
	{
		str += _T("9");
	}
	return str;
}

CString CQiXingCai::GetShiDxdshString()
{
	CString strShi;
	if (IsNumBtnCheck(m_btnDaShi)) 
	{
		strShi += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoShi)) 
	{
		strShi += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanShi)) 
	{
		strShi += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangShi)) 
	{
		strShi += _T("2");
	}

	return strShi;
}

CString CQiXingCai::GetGeDxdshString()
{
	CString strGe;
	if (IsNumBtnCheck(m_btnDaGe)) 
	{
		strGe += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoGe)) 
	{
		strGe += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanGe)) 
	{
		strGe += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangGe)) 
	{
		strGe += _T("2");
	}

	return strGe;
}

CString CQiXingCai::GetWanDesc()
{
	return theGameKind[(CZ_QiXingCai << 16) | (unsigned short)m_gameKind];
}

QiXingCaiKind CQiXingCai::GetGameKindByDesc(const CString& desc)
{
	if (desc == _T("两定玩法")) 
	{
		return QiXingCai_LiangDing;
	} 
	else if (desc == _T("三定玩法")) 
	{
		return QiXingCai_SanDing;
	} 
	else if (desc == _T("四定玩法")) 
	{
		return QiXingCai_SiDing;
	}
	else if (desc == _T("两字现")) 
	{
		return QiXingCai_LiangZiXian;
	} 
	else if (desc == _T("三字现")) 
	{
		return QiXingCai_SanZiXian;
	} 
	return QiXingCai_LiangDing;
}

void CQiXingCai::AdjustWanFa()
{
	if (m_btnLiangDing.GetSafeHwnd() != NULL) 
	{
		m_btnLiangDing.SetWindowPos(NULL, wanfa_x, wanfa_y, m_btnLiangDing.Width(), m_btnLiangDing.Height(), SWP_NOZORDER);
	}

	if (m_btnYiDing.GetSafeHwnd() != NULL) 
	{
		m_btnYiDing.SetWindowPos(NULL, wanfa_x + wanfa_col_width*3, wanfa_y, m_btnYiDing.Width(), m_btnYiDing.Height(), SWP_NOZORDER);
	}
	if (m_btnSanDing.GetSafeHwnd() != NULL) 
	{
		m_btnSanDing.SetWindowPos(NULL, wanfa_x + wanfa_col_width/**2*/, wanfa_y /*+ wanfa_row_height*/, m_btnSanDing.Width(), m_btnSanDing.Height(), SWP_NOZORDER);
	}
	if (m_btnSiDing.GetSafeHwnd() != NULL) 
	{
		m_btnSiDing.SetWindowPos(NULL, wanfa_x + wanfa_col_width*2, wanfa_y /*+ wanfa_row_height*/, m_btnSiDing.Width(), m_btnSiDing.Height(), SWP_NOZORDER);
	}
	if (m_btnSanZiXian.GetSafeHwnd() != NULL) 
	{
		m_btnSanZiXian.SetWindowPos(NULL, wanfa_x + wanfa_col_width*5-10*2, wanfa_y/* + wanfa_row_height*/, m_btnSanZiXian.Width(), m_btnSanZiXian.Height(), SWP_NOZORDER);
	}

	if (m_btnLiangZiXian.GetSafeHwnd() != NULL) 
	{
		m_btnLiangZiXian.SetWindowPos(NULL, wanfa_x + wanfa_col_width*4-10, wanfa_y, m_btnLiangZiXian.Width(), m_btnLiangZiXian.Height(), SWP_NOZORDER);
	}
}

//调整投注方式 单选框
void CQiXingCai::AdjustRadio()
{
	CWnd *pWnd;

	////lly
	//pWnd = GetDlgItem(IDC_RIO_LIANGDING);
	//if (pWnd != NULL) 
	//{
	//	pWnd->SetWindowPos(NULL, wanfa_sel_x,				wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	//}
	//pWnd = GetDlgItem(IDC_RIO_SANDING);
	//if (pWnd != NULL) 
	//{
	//	pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	//}
	//pWnd = GetDlgItem(IDC_RIO_SIDING);
	//if (pWnd != NULL) 
	//{
	//	pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	//}
	//pWnd = GetDlgItem(IDC_RIO_YIDING);
	//if (pWnd != NULL) 
	//{
	//	pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*3, wanfa_sel_y,wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	//}
	//pWnd = GetDlgItem(IDC_RIO_LIANGZIXIAN);
	//if (pWnd != NULL)  
	//{
	//	pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*4, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	//}
	//pWnd = GetDlgItem(IDC_RIO_SANZIXIAN);
	//if (pWnd != NULL) 
	//{
	//	pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*5, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	//}


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

//投注框：选择数字，5位数字，0--9的选择
void CQiXingCai::AdjustNumBtn()
{
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
		m_btnHeZhi_14.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_14.Width(), m_btnHeZhi_14.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_15.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_15.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_15.Width(), m_btnHeZhi_15.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_16.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_16.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_16.Width(), m_btnHeZhi_16.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_17.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_17.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_3.Width(), m_btnHeZhi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_18.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_18.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_4.Width(), m_btnHeZhi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_19.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_19.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_5.Width(), m_btnHeZhi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_20.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_20.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_6.Width(), m_btnHeZhi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_21.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_21.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_7.Width(), m_btnHeZhi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_22.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_22.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_8.Width(), m_btnHeZhi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_23.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_23.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_9.Width(), m_btnHeZhi_9.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_24.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_24.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_10.Width(), m_btnHeZhi_10.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_25.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_25.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_11.Width(), m_btnHeZhi_11.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_26.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_26.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_12.Width(), m_btnHeZhi_12.Height(), SWP_NOZORDER);
	}
	if(m_btnHeZhi_27.GetSafeHwnd() != NULL) 
	{
		m_btnHeZhi_27.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y+haoma_btn_row_height*2, m_btnHeZhi_13.Width(), m_btnHeZhi_13.Height(), SWP_NOZORDER);
	}
	//牛牛----------------------------
	if(m_btnNiu_0.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_0.Width(), m_btnNiu_0.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_1.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_1.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_1.Width(), m_btnNiu_1.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_2.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_2.Width(), m_btnNiu_2.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_3.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_3.Width(), m_btnNiu_3.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_4.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_4.Width(), m_btnNiu_4.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_5.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_5.Width(), m_btnNiu_5.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_6.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_6.Width(), m_btnNiu_6.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_7.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_7.Width(), m_btnNiu_7.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_8.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_8.Width(), m_btnNiu_8.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_9.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_9.Width(), m_btnNiu_9.Height(), SWP_NOZORDER);
	}
	if(m_btnNiu_10.GetSafeHwnd() != NULL) 
	{
		m_btnNiu_10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y + haoma_btn_row_height*2-10, m_btnNiu_10.Width(), m_btnNiu_10.Height(), SWP_NOZORDER);
	}

	if (m_btnNiuQuan.GetSafeHwnd() != NULL) 
	{
		m_btnNiuQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuQuan.Width(), m_btnNiuQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnNiuDa.GetSafeHwnd() != NULL) 
	{
		m_btnNiuDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuDa.Width(), m_btnNiuDa.Height(), SWP_NOZORDER);
	}
	if (m_btnNiuXiao.GetSafeHwnd() != NULL) 
	{
		m_btnNiuXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuXiao.Width(), m_btnNiuXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnNiuDan.GetSafeHwnd() != NULL) 
	{
		m_btnNiuDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuDan.Width(), m_btnNiuDan.Height(), SWP_NOZORDER);
	}
	if (m_btnNiuShuang.GetSafeHwnd() != NULL) 
	{
		m_btnNiuShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuShuang.Width(), m_btnNiuShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnNiuQing.GetSafeHwnd() != NULL) 
	{
		m_btnNiuQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnNiuQing.Width(), m_btnNiuQing.Height(), SWP_NOZORDER);
	}

	//万位------------------------------
	if (m_staticWan.GetSafeHwnd() != NULL) 
	{
		m_staticWan.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y, m_staticWan.Width(), m_staticWan.Height(), SWP_NOZORDER);
	}

	if(m_btnWan_0.GetSafeHwnd() != NULL) 
	{
		m_btnWan_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y, m_btnWan_0.Width(), m_btnWan_0.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_1.GetSafeHwnd() != NULL) 
	{
		m_btnWan_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y, m_btnWan_1.Width(), m_btnWan_1.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_2.GetSafeHwnd() != NULL) 
	{
		m_btnWan_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y, m_btnWan_2.Width(), m_btnWan_2.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_3.GetSafeHwnd() != NULL) 
	{
		m_btnWan_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y, m_btnWan_3.Width(), m_btnWan_3.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_4.GetSafeHwnd() != NULL) 
	{
		m_btnWan_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y, m_btnWan_4.Width(), m_btnWan_4.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_5.GetSafeHwnd() != NULL) 
	{
		m_btnWan_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y, m_btnWan_5.Width(), m_btnWan_5.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_6.GetSafeHwnd() != NULL) 
	{
		m_btnWan_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y, m_btnWan_6.Width(), m_btnWan_6.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_7.GetSafeHwnd() != NULL) 
	{
		m_btnWan_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y, m_btnWan_7.Width(), m_btnWan_7.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_8.GetSafeHwnd() != NULL) 
	{
		m_btnWan_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y, m_btnWan_8.Width(), m_btnWan_8.Height(), SWP_NOZORDER);
	}
	if(m_btnWan_9.GetSafeHwnd() != NULL) 
	{
		m_btnWan_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y, m_btnWan_9.Width(), m_btnWan_9.Height(), SWP_NOZORDER);
	}
	
	if (m_btnWanQuan.GetSafeHwnd() != NULL) 
	{
		m_btnWanQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+40, haoma_btn_y, m_btnWanQuan.Width(), m_btnWanQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnWanDa.GetSafeHwnd() != NULL) 
	{
		m_btnWanDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+40, haoma_btn_y, m_btnWanDa.Width(), m_btnWanDa.Height(), SWP_NOZORDER);
	}
	if (m_btnWanXiao.GetSafeHwnd() != NULL) 
	{
		m_btnWanXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+40, haoma_btn_y, m_btnWanXiao.Width(), m_btnWanXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnWanDan.GetSafeHwnd() != NULL) 
	{
		m_btnWanDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+47, haoma_btn_y, m_btnWanDan.Width(), m_btnWanDan.Height(), SWP_NOZORDER);
	}
	if (m_btnWanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnWanShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+47, haoma_btn_y, m_btnWanShuang.Width(), m_btnWanShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnWanQing.GetSafeHwnd() != NULL) 
	{
		m_btnWanQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+47, haoma_btn_y, m_btnWanQing.Width(), m_btnWanQing.Height(), SWP_NOZORDER);
	}


	//千位-------------------------------
	if (m_staticQian.GetSafeHwnd() != NULL) 
	{
		m_staticQian.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height, m_staticQian.Width(), m_staticQian.Height(), SWP_NOZORDER);
	}

	if(m_btnQian_0.GetSafeHwnd() != NULL) 
	{
		m_btnQian_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_0.Width(), m_btnQian_0.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_1.GetSafeHwnd() != NULL) 
	{
		m_btnQian_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_1.Width(), m_btnQian_1.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_2.GetSafeHwnd() != NULL) 
	{
		m_btnQian_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_2.Width(), m_btnQian_2.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_3.GetSafeHwnd() != NULL) 
	{
		m_btnQian_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_3.Width(), m_btnQian_3.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_4.GetSafeHwnd() != NULL) 
	{
		m_btnQian_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_4.Width(), m_btnQian_4.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_5.GetSafeHwnd() != NULL) 
	{
		m_btnQian_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_5.Width(), m_btnQian_5.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_6.GetSafeHwnd() != NULL) 
	{
		m_btnQian_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_6.Width(), m_btnQian_6.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_7.GetSafeHwnd() != NULL) 
	{
		m_btnQian_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_7.Width(), m_btnQian_7.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_8.GetSafeHwnd() != NULL) 
	{
		m_btnQian_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_8.Width(), m_btnQian_8.Height(), SWP_NOZORDER);
	}
	if(m_btnQian_9.GetSafeHwnd() != NULL) 
	{
		m_btnQian_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y + haoma_btn_row_height, m_btnQian_9.Width(), m_btnQian_9.Height(), SWP_NOZORDER);
	}

	if (m_btnQianQuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+40, haoma_btn_y + haoma_btn_row_height, m_btnQianQuan.Width(), m_btnQianQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnQianDa.GetSafeHwnd() != NULL) 
	{
		m_btnQianDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+40, haoma_btn_y + haoma_btn_row_height, m_btnQianDa.Width(), m_btnQianDa.Height(), SWP_NOZORDER);
	}
	if (m_btnQianXiao.GetSafeHwnd() != NULL) 
	{
		m_btnQianXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+40, haoma_btn_y + haoma_btn_row_height, m_btnQianXiao.Width(), m_btnQianXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnQianDan.GetSafeHwnd() != NULL) 
	{
		m_btnQianDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+47, haoma_btn_y + haoma_btn_row_height, m_btnQianDan.Width(), m_btnQianDan.Height(), SWP_NOZORDER);
	}
	if (m_btnQianShuang.GetSafeHwnd() != NULL) 
	{
		m_btnQianShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+40+7, haoma_btn_y + haoma_btn_row_height, m_btnQianShuang.Width(), m_btnQianShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnQianQing.GetSafeHwnd() != NULL) 
	{
		m_btnQianQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+40+7, haoma_btn_y + haoma_btn_row_height, m_btnQianQing.Width(), m_btnQianQing.Height(), SWP_NOZORDER);
	}

	//牛牛static
	if (m_staticNiu.GetSafeHwnd() != NULL) 
	{
		m_staticNiu.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticNiu.Width(), m_staticNiu.Height(), SWP_NOZORDER);
	}
	
	//百位-------------------------------
	//组选
	if (m_staticZuXuan.GetSafeHwnd() != NULL) 
	{
		m_staticZuXuan.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticDanHao.GetSafeHwnd() != NULL) 
	{
		m_staticDanHao.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*3, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticXuanHao.GetSafeHwnd() != NULL) 
	{
		m_staticXuanHao.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticZu120.GetSafeHwnd() != NULL) 
	{
		m_staticZu120.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticErChong.GetSafeHwnd() != NULL) 
	{
		m_staticErChong.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticErChong1.GetSafeHwnd() != NULL) 
	{
		m_staticErChong1.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*3, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticSanChong.GetSafeHwnd() != NULL) 
	{
		m_staticSanChong.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	if (m_staticSiChong.GetSafeHwnd() != NULL) 
	{
		m_staticSiChong.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}
	//不定位
	if (m_staticBuDingWei.GetSafeHwnd() != NULL) 
	{
		m_staticBuDingWei.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}

	if (m_staticBai.GetSafeHwnd() != NULL) 
	{
		m_staticBai.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
	}

	if(m_btnBai_0.GetSafeHwnd() != NULL) 
	{
		m_btnBai_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_0.Width(), m_btnBai_0.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_1.GetSafeHwnd() != NULL) 
	{
		m_btnBai_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_1.Width(), m_btnBai_1.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_2.GetSafeHwnd() != NULL) 
	{
		m_btnBai_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_2.Width(), m_btnBai_2.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_3.GetSafeHwnd() != NULL) 
	{
		m_btnBai_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_3.Width(), m_btnBai_3.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_4.GetSafeHwnd() != NULL) 
	{
		m_btnBai_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_4.Width(), m_btnBai_4.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_5.GetSafeHwnd() != NULL) 
	{
		m_btnBai_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_5.Width(), m_btnBai_5.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_6.GetSafeHwnd() != NULL) 
	{
		m_btnBai_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_6.Width(), m_btnBai_6.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_7.GetSafeHwnd() != NULL) 
	{
		m_btnBai_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_7.Width(), m_btnBai_7.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_8.GetSafeHwnd() != NULL) 
	{
		m_btnBai_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_8.Width(), m_btnBai_8.Height(), SWP_NOZORDER);
	}
	if(m_btnBai_9.GetSafeHwnd() != NULL) 
	{
		m_btnBai_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y + haoma_btn_row_height*2, m_btnBai_9.Width(), m_btnBai_9.Height(), SWP_NOZORDER);
	}

	if (m_btnBaiQuan.GetSafeHwnd() != NULL) 
	{
		m_btnBaiQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiQuan.Width(), m_btnBaiQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiDa.GetSafeHwnd() != NULL) 
	{
		m_btnBaiDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiDa.Width(), m_btnBaiDa.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiXiao.GetSafeHwnd() != NULL) 
	{
		m_btnBaiXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+20*2, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiXiao.Width(), m_btnBaiXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiDan.GetSafeHwnd() != NULL) 
	{
		m_btnBaiDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiDan.Width(), m_btnBaiDan.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiShuang.GetSafeHwnd() != NULL) 
	{
		m_btnBaiShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiShuang.Width(), m_btnBaiShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnBaiQing.GetSafeHwnd() != NULL) 
	{
		m_btnBaiQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+20*2+7, haoma_btn_y + haoma_btn_row_height*2, m_btnBaiQing.Width(), m_btnBaiQing.Height(), SWP_NOZORDER);
	}

	//大小单双
	if(m_btnDaShi.GetSafeHwnd() != NULL) 
	{
		m_btnDaShi.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*3, m_btnDaShi.Width(), m_btnDaShi.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoShi.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoShi.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*3, m_btnXiaoShi.Width(), m_btnXiaoShi.Height(), SWP_NOZORDER);
	}
	if(m_btnDanShi.GetSafeHwnd() != NULL) 
	{
		m_btnDanShi.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*3, m_btnDanShi.Width(), m_btnDanShi.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangShi.GetSafeHwnd() != NULL) 
	{
		m_btnShuangShi.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShuangShi.Width(), m_btnShuangShi.Height(), SWP_NOZORDER);
	}
	
	//十位-------------------------------
	if (m_staticShi.GetSafeHwnd() != NULL) 
	{
		m_staticShi.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*3, m_staticShi.Width(), m_staticShi.Height(), SWP_NOZORDER);
	}

	if(m_btnShi_0.GetSafeHwnd() != NULL) 
	{
		m_btnShi_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_0.Width(), m_btnShi_0.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_1.GetSafeHwnd() != NULL) 
	{
		m_btnShi_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_1.Width(), m_btnShi_1.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_2.GetSafeHwnd() != NULL) 
	{
		m_btnShi_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_2.Width(), m_btnShi_2.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_3.GetSafeHwnd() != NULL) 
	{
		m_btnShi_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_3.Width(), m_btnShi_3.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_4.GetSafeHwnd() != NULL) 
	{
		m_btnShi_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_4.Width(), m_btnShi_4.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_5.GetSafeHwnd() != NULL) 
	{
		m_btnShi_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_5.Width(), m_btnShi_5.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_6.GetSafeHwnd() != NULL) 
	{
		m_btnShi_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_6.Width(), m_btnShi_6.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_7.GetSafeHwnd() != NULL) 
	{
		m_btnShi_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_7.Width(), m_btnShi_7.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_8.GetSafeHwnd() != NULL) 
	{
		m_btnShi_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_8.Width(), m_btnShi_8.Height(), SWP_NOZORDER);
	}
	if(m_btnShi_9.GetSafeHwnd() != NULL) 
	{
		m_btnShi_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y + haoma_btn_row_height*3, m_btnShi_9.Width(), m_btnShi_9.Height(), SWP_NOZORDER);
	}

	if (m_btnShiQuan.GetSafeHwnd() != NULL) 
	{
		m_btnShiQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+20*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiQuan.Width(), m_btnShiQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnShiDa.GetSafeHwnd() != NULL) 
	{
		m_btnShiDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+20*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiDa.Width(), m_btnShiDa.Height(), SWP_NOZORDER);
	}
	if (m_btnShiXiao.GetSafeHwnd() != NULL) 
	{
		m_btnShiXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+20*2, haoma_btn_y + haoma_btn_row_height*3, m_btnShiXiao.Width(), m_btnShiXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnShiDan.GetSafeHwnd() != NULL) 
	{
		m_btnShiDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+20*2+7, haoma_btn_y + haoma_btn_row_height*3, m_btnShiDan.Width(), m_btnShiDan.Height(), SWP_NOZORDER);
	}
	if (m_btnShiShuang.GetSafeHwnd() != NULL) 
	{
		m_btnShiShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+20*2+7, haoma_btn_y + haoma_btn_row_height*3, m_btnShiShuang.Width(), m_btnShiShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnShiQing.GetSafeHwnd() != NULL) 
	{
		m_btnShiQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+20*2+7, haoma_btn_y + haoma_btn_row_height*3, m_btnShiQing.Width(), m_btnShiQing.Height(), SWP_NOZORDER);
	}

	//大小单双
	if(m_btnDaGe.GetSafeHwnd() != NULL) 
	{
		m_btnDaGe.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*4, m_btnDaGe.Width(), m_btnDaGe.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoGe.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoGe.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*4, m_btnXiaoGe.Width(), m_btnXiaoGe.Height(), SWP_NOZORDER);
	}
	if(m_btnDanGe.GetSafeHwnd() != NULL) 
	{
		m_btnDanGe.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*4, m_btnDanGe.Width(), m_btnDanGe.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangGe.GetSafeHwnd() != NULL) 
	{
		m_btnShuangGe.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*4, m_btnShuangGe.Width(), m_btnShuangGe.Height(), SWP_NOZORDER);
	}
	
	//个位-------------------------------
	if (m_staticGe.GetSafeHwnd() != NULL) 
	{
		m_staticGe.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*4, m_staticGe.Width(), m_staticGe.Height(), SWP_NOZORDER);
	}

	if(m_btnGe_0.GetSafeHwnd() != NULL) 
	{
		m_btnGe_0.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_0.Width(), m_btnGe_0.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_1.GetSafeHwnd() != NULL) 
	{
		m_btnGe_1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_1.Width(), m_btnGe_1.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_2.GetSafeHwnd() != NULL) 
	{
		m_btnGe_2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_2.Width(), m_btnGe_2.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_3.GetSafeHwnd() != NULL) 
	{
		m_btnGe_3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_3.Width(), m_btnGe_3.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_4.GetSafeHwnd() != NULL) 
	{
		m_btnGe_4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_4.Width(), m_btnGe_4.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_5.GetSafeHwnd() != NULL) 
	{
		m_btnGe_5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_5.Width(), m_btnGe_5.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_6.GetSafeHwnd() != NULL) 
	{
		m_btnGe_6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_6.Width(), m_btnGe_6.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_7.GetSafeHwnd() != NULL) 
	{
		m_btnGe_7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_7.Width(), m_btnGe_7.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_8.GetSafeHwnd() != NULL) 
	{
		m_btnGe_8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_8.Width(), m_btnGe_8.Height(), SWP_NOZORDER);
	}
	if(m_btnGe_9.GetSafeHwnd() != NULL) 
	{
		m_btnGe_9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9+20, haoma_btn_y + haoma_btn_row_height*4, m_btnGe_9.Width(), m_btnGe_9.Height(), SWP_NOZORDER);
	}

	if (m_btnGeQuan.GetSafeHwnd() != NULL) 
	{
		m_btnGeQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10+20*2, haoma_btn_y + haoma_btn_row_height*4, m_btnGeQuan.Width(), m_btnGeQuan.Height(), SWP_NOZORDER);
	}
	if (m_btnGeDa.GetSafeHwnd() != NULL) 
	{
		m_btnGeDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11+20*2, haoma_btn_y + haoma_btn_row_height*4, m_btnGeDa.Width(), m_btnGeDa.Height(), SWP_NOZORDER);
	}
	if (m_btnGeXiao.GetSafeHwnd() != NULL) 
	{
		m_btnGeXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12+20*2, haoma_btn_y + haoma_btn_row_height*4, m_btnGeXiao.Width(), m_btnGeXiao.Height(), SWP_NOZORDER);
	}
	if (m_btnGeDan.GetSafeHwnd() != NULL) 
	{
		m_btnGeDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13+20*2+7, haoma_btn_y + haoma_btn_row_height*4, m_btnGeDan.Width(), m_btnGeDan.Height(), SWP_NOZORDER);
	}
	if (m_btnGeShuang.GetSafeHwnd() != NULL) 
	{
		m_btnGeShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14+20*2+7, haoma_btn_y + haoma_btn_row_height*4, m_btnGeShuang.Width(), m_btnGeShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnGeQing.GetSafeHwnd() != NULL) 
	{
		m_btnGeQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15+20*2+7, haoma_btn_y + haoma_btn_row_height*4, m_btnGeQing.Width(), m_btnGeQing.Height(), SWP_NOZORDER);
	}

}

//按钮调整：删除，清零，倍投，投注，追号
void CQiXingCai::AdjustNumView()
{
	if(m_btnBeiTouJia.GetSafeHwnd() != NULL) 
	{
		m_btnBeiTouJia.SetWindowPos(NULL,edit_beitou_x+edit_beitou_width , edit_beitou_y-8, m_btnBeiTouJia.Width(), m_btnBeiTouJia.Height(), SWP_NOZORDER);
	}
	if(m_btnBeiTouJian.GetSafeHwnd() != NULL) 
	{
		m_btnBeiTouJian.SetWindowPos(NULL,edit_beitou_x-m_btnBeiTouJian.Width() , edit_beitou_y-8, m_btnBeiTouJian.Width(), m_btnBeiTouJian.Height(), SWP_NOZORDER);
	}

	if(m_btnDelSel.GetSafeHwnd() != NULL) 
	{
		m_btnDelSel.SetWindowPos(NULL, btn_del_sel_x, btn_del_sel_y, m_btnDelSel.Width(), m_btnDelSel.Height(), SWP_NOZORDER);
		m_btnDelSel.ShowWindow(SW_HIDE);
	}

	if(m_btnClsList.GetSafeHwnd() != NULL) 
	{
		m_btnClsList.SetWindowPos(NULL, btn_cls_list_x, btn_cls_list_y, m_btnClsList.Width(), m_btnClsList.Height(), SWP_NOZORDER);
	}

	if (m_listChqSSC.GetSafeHwnd() != NULL) 
	{
		m_listChqSSC.SetWindowPos(NULL, list_haomao_x, list_haomao_y, list_haomao_width, list_haomao_height, SWP_NOZORDER);
	}

	if(m_editBeiTou.GetSafeHwnd() != NULL) 
	{
		m_editBeiTou.SetWindowPos(NULL, edit_beitou_x, edit_beitou_y, edit_beitou_width, edit_beitou_height, SWP_NOZORDER);
	}

	if(m_btnTouZhu.GetSafeHwnd() != NULL) 
	{
		m_btnTouZhu.SetWindowPos(NULL, btn_touzhu_x, btn_touzhu_y, m_btnTouZhu.Width(), m_btnTouZhu.Height(), SWP_NOZORDER);
	}

	if(m_btnZhuiHao.GetSafeHwnd() != NULL) 
	{
		m_btnZhuiHao.SetWindowPos(NULL, btn_zhuihao_x, btn_zhuihao_y, m_btnZhuiHao.Width(), m_btnZhuiHao.Height(), SWP_NOZORDER);
	}
	if(m_btnMoreRecord.GetSafeHwnd() != NULL) 
	{
		m_btnMoreRecord.SetWindowPos(NULL, btn_more_x, btn_more_y, m_btnMoreRecord.Width(), m_btnMoreRecord.Height(), SWP_NOZORDER);
	}

}

//单式调整
void CQiXingCai::AdjustDanShi()
{
	//if (m_staticDanShi.GetSafeHwnd() != NULL) 
	//{
	//	m_staticDanShi.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y, m_staticDanShi.Width(), m_staticDanShi.Height(), SWP_NOZORDER);
	//}

	//if (m_editDanShiHaoMa.GetSafeHwnd() != NULL) 
	//{
	//	m_editDanShiHaoMa.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y, edit_haomao_danshi_width, edit_haomao_danshi_height, SWP_NOZORDER);
	//}
 	if (m_richDanshiHaoma.GetSafeHwnd() != NULL) 
 	{
 		m_richDanshiHaoma.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y, edit_haomao_danshi_width, edit_haomao_danshi_height, SWP_NOZORDER);
 	}
	CWnd* pWnd = GetDlgItem(IDC_CHK_WAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y-24, 45, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_CHK_QIAN);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x+60, edit_haomao_danshi_y-24, 45, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_CHK_BAI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x+120, edit_haomao_danshi_y-24, 45, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_CHK_SHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x+180, edit_haomao_danshi_y-24, 45, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_CHKGE);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x+240, edit_haomao_danshi_y-24, 45, wanfa_sel_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_STATIC_RENXUAN_TIP);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_haomao_danshi_x+300, edit_haomao_danshi_y-24, 300, wanfa_sel_height, SWP_NOZORDER);
	}


}

//加注按钮
void CQiXingCai::AdjustAdd()
{
	if (m_btnChqAdd.GetSafeHwnd() != NULL) 
	{
		m_btnChqAdd.SetWindowPos(NULL, btn_chq_add_x, btn_chq_add_y, m_btnChqAdd.Width(), m_btnChqAdd.Height(), SWP_NOZORDER);
	}

	if (m_btnLock.GetSafeHwnd() != NULL) 
	{
		m_btnLock.SetWindowPos(NULL, btn_lock_x, btn_lock_y, m_btnLock.Width(), m_btnLock.Height(), SWP_NOZORDER);
	}

	if(m_btnYuan.GetSafeHwnd() != NULL)
	{
		m_btnYuan.SetWindowPos(NULL,btn_yuan_x,btn_yuan_y,m_btnYuan.Width(),m_btnYuan.Height(),SWP_NOZORDER);
	}
	if(m_btnJiao.GetSafeHwnd() != NULL)
	{
		m_btnJiao.SetWindowPos(NULL,btn_jiao_x,btn_jiao_y,m_btnJiao.Width(),m_btnJiao.Height(),SWP_NOZORDER);
	}
	if(m_btnFen.GetSafeHwnd() != NULL)
	{
		m_btnFen.SetWindowPos(NULL,btn_fen_x,btn_fen_y,m_btnFen.Width(),m_btnFen.Height(),SWP_NOZORDER);
	}
	if(m_btnLi.GetSafeHwnd() != NULL)
	{
		m_btnLi.SetWindowPos(NULL,btn_li_x,btn_li_y,m_btnLi.Width(),m_btnLi.Height(),SWP_NOZORDER);
	}

}
void CQiXingCai::HideHeZhiNums()
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
void CQiXingCai::ShowHeZhiNums()
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
void CQiXingCai::HideWanNums()
{
	m_btnWan_0.ShowWindow(SW_HIDE);
	m_btnWan_1.ShowWindow(SW_HIDE);
	m_btnWan_2.ShowWindow(SW_HIDE);
	m_btnWan_3.ShowWindow(SW_HIDE);
	m_btnWan_4.ShowWindow(SW_HIDE);
	m_btnWan_5.ShowWindow(SW_HIDE);
	m_btnWan_6.ShowWindow(SW_HIDE);
	m_btnWan_7.ShowWindow(SW_HIDE);
	m_btnWan_8.ShowWindow(SW_HIDE);
	m_btnWan_9.ShowWindow(SW_HIDE);

	m_staticWan.ShowWindow(SW_HIDE);

	m_btnWanQuan.ShowWindow(SW_HIDE);
	m_btnWanDa.ShowWindow(SW_HIDE);
	m_btnWanXiao.ShowWindow(SW_HIDE);
	m_btnWanDan.ShowWindow(SW_HIDE);
	m_btnWanShuang.ShowWindow(SW_HIDE);
	m_btnWanQing.ShowWindow(SW_HIDE);
}

void CQiXingCai::HideQianNums()
{
	m_btnQian_0.ShowWindow(SW_HIDE);
	m_btnQian_1.ShowWindow(SW_HIDE);
	m_btnQian_2.ShowWindow(SW_HIDE);
	m_btnQian_3.ShowWindow(SW_HIDE);
	m_btnQian_4.ShowWindow(SW_HIDE);
	m_btnQian_5.ShowWindow(SW_HIDE);
	m_btnQian_6.ShowWindow(SW_HIDE);
	m_btnQian_7.ShowWindow(SW_HIDE);
	m_btnQian_8.ShowWindow(SW_HIDE);
	m_btnQian_9.ShowWindow(SW_HIDE);

	m_staticQian.ShowWindow(SW_HIDE);

	m_btnQianQuan.ShowWindow(SW_HIDE);
	m_btnQianDa.ShowWindow(SW_HIDE);
	m_btnQianXiao.ShowWindow(SW_HIDE);
	m_btnQianDan.ShowWindow(SW_HIDE);
	m_btnQianShuang.ShowWindow(SW_HIDE);
	m_btnQianQing.ShowWindow(SW_HIDE);
}

void CQiXingCai::HideBaiNums()
{	
	m_btnBai_0.ShowWindow(SW_HIDE);
	m_btnBai_1.ShowWindow(SW_HIDE);
	m_btnBai_2.ShowWindow(SW_HIDE);
	m_btnBai_3.ShowWindow(SW_HIDE);
	m_btnBai_4.ShowWindow(SW_HIDE);
	m_btnBai_5.ShowWindow(SW_HIDE);
	m_btnBai_6.ShowWindow(SW_HIDE);
	m_btnBai_7.ShowWindow(SW_HIDE);
	m_btnBai_8.ShowWindow(SW_HIDE);
	m_btnBai_9.ShowWindow(SW_HIDE);

	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticErChong1.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

	m_btnBaiQuan.ShowWindow(SW_HIDE);
	m_btnBaiDa.ShowWindow(SW_HIDE);
	m_btnBaiXiao.ShowWindow(SW_HIDE);
	m_btnBaiDan.ShowWindow(SW_HIDE);
	m_btnBaiShuang.ShowWindow(SW_HIDE);
	m_btnBaiQing.ShowWindow(SW_HIDE);
}

void CQiXingCai::HideShiNums()
{
	m_btnShi_0.ShowWindow(SW_HIDE);
	m_btnShi_1.ShowWindow(SW_HIDE);
	m_btnShi_2.ShowWindow(SW_HIDE);
	m_btnShi_3.ShowWindow(SW_HIDE);
	m_btnShi_4.ShowWindow(SW_HIDE);
	m_btnShi_5.ShowWindow(SW_HIDE);
	m_btnShi_6.ShowWindow(SW_HIDE);
	m_btnShi_7.ShowWindow(SW_HIDE);
	m_btnShi_8.ShowWindow(SW_HIDE);
	m_btnShi_9.ShowWindow(SW_HIDE);

	m_staticShi.ShowWindow(SW_HIDE);
	m_staticDanHao.ShowWindow(SW_HIDE);
	m_btnShiQuan.ShowWindow(SW_HIDE);
	m_btnShiDa.ShowWindow(SW_HIDE);
	m_btnShiXiao.ShowWindow(SW_HIDE);
	m_btnShiDan.ShowWindow(SW_HIDE);
	m_btnShiShuang.ShowWindow(SW_HIDE);
	m_btnShiQing.ShowWindow(SW_HIDE);
}

void CQiXingCai::HideGeNums()
{
	m_btnGe_0.ShowWindow(SW_HIDE);
	m_btnGe_1.ShowWindow(SW_HIDE);
	m_btnGe_2.ShowWindow(SW_HIDE);
	m_btnGe_3.ShowWindow(SW_HIDE);
	m_btnGe_4.ShowWindow(SW_HIDE);
	m_btnGe_5.ShowWindow(SW_HIDE);
	m_btnGe_6.ShowWindow(SW_HIDE);
	m_btnGe_7.ShowWindow(SW_HIDE);
	m_btnGe_8.ShowWindow(SW_HIDE);
	m_btnGe_9.ShowWindow(SW_HIDE);

	m_staticGe.ShowWindow(SW_HIDE);

	m_btnGeQuan.ShowWindow(SW_HIDE);
	m_btnGeDa.ShowWindow(SW_HIDE);
	m_btnGeXiao.ShowWindow(SW_HIDE);
	m_btnGeDan.ShowWindow(SW_HIDE);
	m_btnGeShuang.ShowWindow(SW_HIDE);
	m_btnGeQing.ShowWindow(SW_HIDE);
}

void CQiXingCai::ShowWanNums()
{
	m_btnWan_0.ShowWindow(SW_SHOW);
	m_btnWan_1.ShowWindow(SW_SHOW);
	m_btnWan_2.ShowWindow(SW_SHOW);
	m_btnWan_3.ShowWindow(SW_SHOW);
	m_btnWan_4.ShowWindow(SW_SHOW);
	m_btnWan_5.ShowWindow(SW_SHOW);
	m_btnWan_6.ShowWindow(SW_SHOW);
	m_btnWan_7.ShowWindow(SW_SHOW);
	m_btnWan_8.ShowWindow(SW_SHOW);
	m_btnWan_9.ShowWindow(SW_SHOW);

	m_staticWan.ShowWindow(SW_SHOW);

	m_btnWanQuan.ShowWindow(SW_SHOW);
	m_btnWanDa.ShowWindow(SW_SHOW);
	m_btnWanXiao.ShowWindow(SW_SHOW);
	m_btnWanDan.ShowWindow(SW_SHOW);
	m_btnWanShuang.ShowWindow(SW_SHOW);
	m_btnWanQing.ShowWindow(SW_SHOW);
}

void CQiXingCai::ShowNiuNums()
{
	m_btnNiu_1.ShowWindow(SW_SHOW);
	m_btnNiu_2.ShowWindow(SW_SHOW);
	m_btnNiu_3.ShowWindow(SW_SHOW);
	m_btnNiu_4.ShowWindow(SW_SHOW);
	m_btnNiu_5.ShowWindow(SW_SHOW);
	m_btnNiu_6.ShowWindow(SW_SHOW);
	m_btnNiu_7.ShowWindow(SW_SHOW);
	m_btnNiu_8.ShowWindow(SW_SHOW);
	m_btnNiu_9.ShowWindow(SW_SHOW);
	m_btnNiu_10.ShowWindow(SW_SHOW);

	m_staticNiu.ShowWindow(SW_SHOW);

	m_btnNiuQuan.ShowWindow(SW_SHOW);
	m_btnNiuDa.ShowWindow(SW_SHOW);
	m_btnNiuXiao.ShowWindow(SW_SHOW);
	m_btnNiuDan.ShowWindow(SW_SHOW);
	m_btnNiuShuang.ShowWindow(SW_SHOW);
	m_btnNiuQing.ShowWindow(SW_SHOW);
}
void CQiXingCai::HideNiuNums()
{
	m_btnNiu_0.ShowWindow(SW_HIDE);
	m_btnNiu_1.ShowWindow(SW_HIDE);
	m_btnNiu_2.ShowWindow(SW_HIDE);
	m_btnNiu_3.ShowWindow(SW_HIDE);
	m_btnNiu_4.ShowWindow(SW_HIDE);
	m_btnNiu_5.ShowWindow(SW_HIDE);
	m_btnNiu_6.ShowWindow(SW_HIDE);
	m_btnNiu_7.ShowWindow(SW_HIDE);
	m_btnNiu_8.ShowWindow(SW_HIDE);
	m_btnNiu_9.ShowWindow(SW_HIDE);
	m_btnNiu_10.ShowWindow(SW_HIDE);

	m_staticNiu.ShowWindow(SW_HIDE);

	m_btnNiuQuan.ShowWindow(SW_HIDE);
	m_btnNiuDa.ShowWindow(SW_HIDE);
	m_btnNiuXiao.ShowWindow(SW_HIDE);
	m_btnNiuDan.ShowWindow(SW_HIDE);
	m_btnNiuShuang.ShowWindow(SW_HIDE);
	m_btnNiuQing.ShowWindow(SW_HIDE);
}

void CQiXingCai::ShowQianNums()
{
	m_btnQian_0.ShowWindow(SW_SHOW);
	m_btnQian_1.ShowWindow(SW_SHOW);
	m_btnQian_2.ShowWindow(SW_SHOW);
	m_btnQian_3.ShowWindow(SW_SHOW);
	m_btnQian_4.ShowWindow(SW_SHOW);
	m_btnQian_5.ShowWindow(SW_SHOW);
	m_btnQian_6.ShowWindow(SW_SHOW);
	m_btnQian_7.ShowWindow(SW_SHOW);
	m_btnQian_8.ShowWindow(SW_SHOW);
	m_btnQian_9.ShowWindow(SW_SHOW);

	m_staticQian.ShowWindow(SW_SHOW);

	m_btnQianQuan.ShowWindow(SW_SHOW);
	m_btnQianDa.ShowWindow(SW_SHOW);
	m_btnQianXiao.ShowWindow(SW_SHOW);
	m_btnQianDan.ShowWindow(SW_SHOW);
	m_btnQianShuang.ShowWindow(SW_SHOW);
	m_btnQianQing.ShowWindow(SW_SHOW);
}

void CQiXingCai::ShowBaiNums()
{
	m_btnBai_0.ShowWindow(SW_SHOW);
	m_btnBai_1.ShowWindow(SW_SHOW);
	m_btnBai_2.ShowWindow(SW_SHOW);
	m_btnBai_3.ShowWindow(SW_SHOW);
	m_btnBai_4.ShowWindow(SW_SHOW);
	m_btnBai_5.ShowWindow(SW_SHOW);
	m_btnBai_6.ShowWindow(SW_SHOW);
	m_btnBai_7.ShowWindow(SW_SHOW);
	m_btnBai_8.ShowWindow(SW_SHOW);
	m_btnBai_9.ShowWindow(SW_SHOW);

	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_SHOW);

	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticErChong1.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

	m_btnBaiQuan.ShowWindow(SW_SHOW);
	m_btnBaiDa.ShowWindow(SW_SHOW);
	m_btnBaiXiao.ShowWindow(SW_SHOW);
	m_btnBaiDan.ShowWindow(SW_SHOW);
	m_btnBaiShuang.ShowWindow(SW_SHOW);
	m_btnBaiQing.ShowWindow(SW_SHOW);
}

void CQiXingCai::ShowShiNums()
{
	m_btnShi_0.ShowWindow(SW_SHOW);
	m_btnShi_1.ShowWindow(SW_SHOW);
	m_btnShi_2.ShowWindow(SW_SHOW);
	m_btnShi_3.ShowWindow(SW_SHOW);
	m_btnShi_4.ShowWindow(SW_SHOW);
	m_btnShi_5.ShowWindow(SW_SHOW);
	m_btnShi_6.ShowWindow(SW_SHOW);
	m_btnShi_7.ShowWindow(SW_SHOW);
	m_btnShi_8.ShowWindow(SW_SHOW);
	m_btnShi_9.ShowWindow(SW_SHOW);

	m_staticShi.ShowWindow(SW_SHOW);
	m_staticDanHao.ShowWindow(SW_HIDE);
	m_btnShiQuan.ShowWindow(SW_SHOW);
	m_btnShiDa.ShowWindow(SW_SHOW);
	m_btnShiXiao.ShowWindow(SW_SHOW);
	m_btnShiDan.ShowWindow(SW_SHOW);
	m_btnShiShuang.ShowWindow(SW_SHOW);
	m_btnShiQing.ShowWindow(SW_SHOW);
	m_staticErChong1.ShowWindow(SW_HIDE);
}

void CQiXingCai::ShowGeNums()
{
	m_btnGe_0.ShowWindow(SW_SHOW);
	m_btnGe_1.ShowWindow(SW_SHOW);
	m_btnGe_2.ShowWindow(SW_SHOW);
	m_btnGe_3.ShowWindow(SW_SHOW);
	m_btnGe_4.ShowWindow(SW_SHOW);
	m_btnGe_5.ShowWindow(SW_SHOW);
	m_btnGe_6.ShowWindow(SW_SHOW);
	m_btnGe_7.ShowWindow(SW_SHOW);
	m_btnGe_8.ShowWindow(SW_SHOW);
	m_btnGe_9.ShowWindow(SW_SHOW);

	m_staticGe.ShowWindow(SW_SHOW);

	m_btnGeQuan.ShowWindow(SW_SHOW);
	m_btnGeDa.ShowWindow(SW_SHOW);
	m_btnGeXiao.ShowWindow(SW_SHOW);
	m_btnGeDan.ShowWindow(SW_SHOW);
	m_btnGeShuang.ShowWindow(SW_SHOW);
	m_btnGeQing.ShowWindow(SW_SHOW);
}

void CQiXingCai::ShowZuXuan()
{
	m_staticZuXuan.ShowWindow(SW_SHOW);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

	m_staticBai.ShowWindow(SW_HIDE);
}
void CQiXingCai::ShowDanHao()
{
	m_staticDanHao.ShowWindow(SW_SHOW);
	m_staticShi.ShowWindow(SW_HIDE);
	m_staticErChong1.ShowWindow(SW_HIDE);
}
void CQiXingCai::ShowErChong1()
{
	m_staticDanHao.ShowWindow(SW_HIDE);
	m_staticShi.ShowWindow(SW_HIDE);
	m_staticErChong1.ShowWindow(SW_SHOW);
}
void CQiXingCai::ShowXuanHao()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_SHOW);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

}
void CQiXingCai::ShowZu120()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_SHOW);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

}

// void CQiXingCai::HideZuXuan()
// {
// 	m_staticZuXuan.ShowWindow(SW_HIDE);
// 	m_staticBuDingWei.ShowWindow(SW_HIDE);
// 	m_staticBai.ShowWindow(SW_HIDE);
// }

void CQiXingCai::ShowBuDingWei()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_SHOW);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

}

void CQiXingCai::ShowErChong()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_SHOW);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_HIDE);

}

void CQiXingCai::ShowSanChong()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_SHOW);
	m_staticSiChong.ShowWindow(SW_HIDE);

}
void CQiXingCai::ShowSiChong()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticXuanHao.ShowWindow(SW_HIDE);
	m_staticZu120.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticErChong.ShowWindow(SW_HIDE);
	m_staticSanChong.ShowWindow(SW_HIDE);
	m_staticSiChong.ShowWindow(SW_SHOW);

}

void CQiXingCai::ShowDaXiaoDanShuang()
{
	m_staticShi.ShowWindow(SW_SHOW);
	m_staticGe.ShowWindow(SW_SHOW);

	m_btnDaShi.ShowWindow(SW_SHOW);
	m_btnXiaoShi.ShowWindow(SW_SHOW);
	m_btnDanShi.ShowWindow(SW_SHOW);
	m_btnShuangShi.ShowWindow(SW_SHOW);
	m_btnDaGe.ShowWindow(SW_SHOW);
	m_btnXiaoGe.ShowWindow(SW_SHOW);
	m_btnDanGe.ShowWindow(SW_SHOW);
	m_btnShuangGe.ShowWindow(SW_SHOW);
}

void CQiXingCai::HideDaXiaoDanShuang()
{
	m_btnDaShi.ShowWindow(SW_HIDE);
	m_btnXiaoShi.ShowWindow(SW_HIDE);
	m_btnDanShi.ShowWindow(SW_HIDE);
	m_btnShuangShi.ShowWindow(SW_HIDE);
	m_btnDaGe.ShowWindow(SW_HIDE);
	m_btnXiaoGe.ShowWindow(SW_HIDE);
	m_btnDanGe.ShowWindow(SW_HIDE);
	m_btnShuangGe.ShowWindow(SW_HIDE);
}

void CQiXingCai::HideDanShiEdit()
{
	m_RenxuanKeyboardInput = false;
	//m_editDanShiHaoMa.ShowWindow(SW_HIDE);
	m_richDanshiHaoma.ShowWindow(SW_HIDE);
}

void CQiXingCai::ShowDanShiEdit()
{
	m_RenxuanKeyboardInput = true;
	m_richDanshiHaoma.ShowWindow(SW_SHOW);
 //	m_editDanShiHaoMa.ShowWindow(SW_SHOW);
}

	//任选当中的万，千，百，十，个位数
void CQiXingCai::ShowRenxuanWeiShu()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WAN);
	if(pBtn)
		pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_QIAN);
	if(pBtn)
		pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_BAI);
	if(pBtn)
		pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_SHI);
	if(pBtn)
		pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_CHKGE);
	if(pBtn)
		pBtn->ShowWindow(SW_SHOW);
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_RENXUAN_TIP);
	if(pStatic)
		pStatic->ShowWindow(SW_SHOW);
	SetCheckRenXuan(false);
}

void CQiXingCai::HideRenxuanWeiShu()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WAN);
	if(pBtn)
		pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_QIAN);
	if(pBtn)
		pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_BAI);
	if(pBtn)
		pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_SHI);
	if(pBtn)
		pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_CHKGE);
	if(pBtn)
		pBtn->ShowWindow(SW_HIDE);
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_RENXUAN_TIP);
	if(pStatic)
		pStatic->ShowWindow(SW_HIDE);

	SetCheckRenXuan(false);
}

void CQiXingCai::CheckWanFaBtn(CTextButton& btn)
{
	m_btnLiangDing.SetPushed(false);
	m_btnYiDing.SetPushed(false);
	m_btnSanDing.SetPushed(false);
	m_btnSiDing.SetPushed(false);
	m_btnSanZiXian.SetPushed(false);
	m_btnLiangZiXian.SetPushed(false);


	btn.SetPushed(true);
}
VOID CQiXingCai::SendQuerySystemTime()
{
	return;
}
VOID CQiXingCai::SendToServer(int nSendType)
{
	if(nSendType == 1)
	{
		m_bTouzhu = true;
		if(m_bTouzhu)
		{
			m_bTouzhu = false;


				m_strHaoma.Empty();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				int itm_cnt = m_listChqSSC.GetItemCount();
				int nNowKind = 10000;
				int nNowBeishu = 100000;
				int nNowMoshi = 10000;
				CString strTouZhuhaoma;

				map<int,int> mapHaoma;
				mapHaoma.clear();
				for(int i = 0;i < itm_cnt;i++)
				{
					strTouZhuhaoma.Empty();
					CString haoma = m_listChqSSC.GetItemText(i,0);
					if(haoma == _T("号码添加成功"))
						continue;
					CString strWanfa = m_listChqSSC.GetItemText(i, 3);
					CString strBeishu = m_listChqSSC.GetItemText(i, 5);
					CString strMoshi = m_listChqSSC.GetItemText(i, 1);
					int nKind = GetGameKindByDesc(strWanfa);
					int nBeishu = _ttoi(strBeishu);
					int nMoshi = GetMoshiRet(strMoshi);
					if(nNowKind == nKind && nNowBeishu == nBeishu && nNowMoshi == nMoshi && nNowMoshi!= 10000 && nNowBeishu != 100000 && nNowKind != 10000&&nNowKind != SSC_LongHu)
						continue;
					bool bContinue = false;
					for (map<int,int>::iterator pos = mapHaoma.begin(); pos != mapHaoma.end();++pos)
					{
						if(i == pos->second)
						{
							bContinue = true;
							break;
						}
					}
					if(bContinue)
						continue;
					nNowBeishu = nBeishu;
					nNowKind = nKind;
					nNowMoshi = nMoshi;
					int nZhushu = 0;
					for(int j = i;j<itm_cnt;j++)
					{
						haoma = m_listChqSSC.GetItemText(j,0);
						if(haoma == _T("号码添加成功"))
							continue;
						CString wanfa = m_listChqSSC.GetItemText(j, 3);
						int kind = GetGameKindByDesc(wanfa);
						CString sbeishu = m_listChqSSC.GetItemText(j, 5);
						int beishu = _ttoi(sbeishu);
						CString sMoshi = m_listChqSSC.GetItemText(j, 1);
						int scoremoshi = 0;
						scoremoshi = GetMoshiRet(sMoshi);
						if (kind == nNowKind && beishu == nNowBeishu && scoremoshi == nNowMoshi)
						{
							CString strZhu = m_listChqSSC.GetItemText(j, 4);
							nZhushu += _ttoi(strZhu);
							int nSize = mapHaoma.size();
							mapHaoma[nSize] = j;

							CString haoma = m_listChqSSC.GetItemText(j, 0);
							strTouZhuhaoma+= haoma;
							if(haoma[haoma.GetLength()-1] != '|')
								strTouZhuhaoma+=TEXT("|");
						}
					}

					int nSendCount = 0;
					int nEnd = 1;

					CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
					ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
					TouzhuCQSSC.dwUserID = theAccount.user_id;
					TouzhuCQSSC.nBeitou = nNowBeishu;
					TouzhuCQSSC.nGameKind = nNowKind;
					TouzhuCQSSC.nGameType = CZ_QiXingCai;
					TouzhuCQSSC.nMoshi = nNowMoshi;

					TouzhuCQSSC.nSign = m_nTzhSign;

				//	CString strQiHao;
					CString strQiHao = m_chqRule.GetNextExpect();



					string	strAllHaoma;
					ChangeStringToA(strTouZhuhaoma, strAllHaoma);

					string strQi;
					ChangeStringToA(strQiHao, strQi);

					memcpy(TouzhuCQSSC.strQishu ,strQi.c_str(),strQi.length());


					if(strTouZhuhaoma.IsEmpty())
					{
						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"你还未选择号码，请选择！");

						return ;
					}

					byte pDestBuffer[102400];
					int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));

					int nSendSize = 0;
					TouzhuCQSSC.nEnd = 1;
					if(m_zongZhuShu<=0)
					{
						m_DlgStatus.HideStatusWindow();
						OnBnClickedBtnClsList();
						return;
					}

					TouzhuCQSSC.nZhushu = nZhushu;
					int nSize = sizeof(TouzhuCQSSC.strHaoma);
					DWORD dwTick=GetTickCount();

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
					nSendCount++;

					m_bCanSend = true;
				}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 				for(int i = 0;i < vecAllNums.size();i++)
// 				{
// 					m_strHaoma += vecAllNums[i].c_str();
// 					m_strHaoma += L"|";
// 				}
// 				if(m_strHaoma.GetLength()>1024)
// 				{
// 					m_DlgStatus.ShowStatusWindow(TEXT("正在提交数据，请稍候..."));
// 				}

/*
				int nSendCount = 0;
				int nEnd = 1;

				CMD_GP_TouZhuCQSSC_Dan TouzhuCQSSC;
				ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Dan));
				TouzhuCQSSC.dwUserID = theAccount.user_id;
				TouzhuCQSSC.nBeitou = m_beishu;
				TouzhuCQSSC.nGameKind = m_gameKind;
				TouzhuCQSSC.nGameType = CZ_QiXingCai;
				TouzhuCQSSC.nMoshi = theAccount.Scoretype;

				TouzhuCQSSC.nSign = m_nTzhSign;

				CString strQiHao = m_chqRule.GetNextExpect();

				string	strAllHaoma;
				ChangeStringToA(m_strHaoma, strAllHaoma);
				//int itm_cnt = strAllHaoma.length();

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
				if(m_zongZhuShu<=0)
				{
					m_DlgStatus.HideStatusWindow();
					OnBnClickedBtnClsList();
					return;
				}

				TouzhuCQSSC.nZhushu = m_zongZhuShu;
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
*/
				m_bCanSend = true;
			

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
		

		m_btWxzxTip.ShowWindow(SW_HIDE);
		if(m_gameKind == QiXingCai_LiangDing)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT(""));
		}
		OnBnClickedBtnClsList();

		m_bGetUserFandian = true;
		m_zongZhuShu = 0;
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
/*			{
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
					TouzhuCQSSC.nGameType = CZ_QiXingCai;
					TouzhuCQSSC.nMoshi = theAccount.Scoretype;

					TouzhuCQSSC.nSign = m_nTzhSign;
					if(m_bZhuiZjtz)
						TouzhuCQSSC.bZhuihao = GetTickCount();
					else
						TouzhuCQSSC.bZhuihao = 2;

					if(m_strHaoma.IsEmpty())
					{
						m_DlgStatus.HideStatusWindow();
						MyMessageBox(L"你还未选择号码，请选择！");
						OnBnClickedBtnClsList();
						return ;
					}
					string	strAllHaoma;
					ChangeStringToA(m_strHaoma, strAllHaoma);

					memset(TouzhuCQSSC.strQishu,0,sizeof(TouzhuCQSSC.strQishu));
					string sNowQi;
					ChangeStringToA(m_chqRule.GetNextExpect(),sNowQi);
					for(int n=0; n< vecData2.size(); n++)
					{
						string strQi;
						ChangeStringToA(vecData2[n].c_str(),strQi);
						if(strQi < sNowQi)
						{

							MyMessageBox(L"订单期号有误！可能原因，该期已经停止投注！");
							OnBnClickedBtnClsList();
							m_DlgStatus.HideStatusWindow();
							return ;
						}
						memcpy(TouzhuCQSSC.strQishu[n],strQi.c_str(),strQi.length());
						TouzhuCQSSC.nBeitou[n] = _ttoi(vecData1[n].c_str());;
					}


					byte pDestBuffer[102400];
					int nLen = CWHService::LZCompressData((byte*)strAllHaoma.c_str(), strAllHaoma.length(), pDestBuffer, sizeof(pDestBuffer));

					int nSendSize = 0;
					TouzhuCQSSC.nEnd = 1;
					if(m_zongZhuShu<=0)
					{
						m_DlgStatus.HideStatusWindow();
						OnBnClickedBtnClsList();
						return;
					}

					TouzhuCQSSC.nZhushu = m_zongZhuShu;
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
*/
if(m_strHaoma.IsEmpty() && vecAllNums.size()>0)
{
	for(int i = 0;i < vecAllNums.size();i++)
	{
		m_strHaoma += vecAllNums[i].c_str();
		m_strHaoma += L"|";
	}

}

int nSel = m_listChqSSC.GetSelectionMark();
if(nSel<0)
nSel = 0;
int nItemCount = m_listChqSSC.GetItemCount();

CMD_GP_TouZhuCQSSC_Zhuihao TouzhuCQSSC;
ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
TouzhuCQSSC.dwUserID = theAccount.user_id;
TouzhuCQSSC.nGameType = CZ_QiXingCai;

TouzhuCQSSC.nSign = m_nTzhSign;
if(m_bZhuiZjtz)
TouzhuCQSSC.bZhuihao = GetTickCount();
else
TouzhuCQSSC.bZhuihao = 2;
for(int n=0; n< nItemCount; n++)
{
	int nSendCount = 0;
	int nEnd = 1;



	CString wanfa = m_listChqSSC.GetItemText(n, 3);
	int kind = GetGameKindByDesc(wanfa);

	TouzhuCQSSC.nGameKind = kind;
	CString moshi = m_listChqSSC.GetItemText(n, 1);
	int nScoreType = GetMoshiRet(moshi);
	TouzhuCQSSC.nMoshi = nScoreType;

	m_strHaoma = vecAllNums[nItemCount-n-1].c_str();
	if(m_strHaoma.IsEmpty())
	{
		m_DlgStatus.HideStatusWindow();
		MyMessageBox(L"你还未选择号码，请选择！");
		OnBnClickedBtnClsList();
		return ;
	}
	if(m_strHaoma[m_strHaoma.GetLength()-1] != '|')
		m_strHaoma+=TEXT("|");

	string	strAllHaoma;
	ChangeStringToA(m_strHaoma, strAllHaoma);

	memset(TouzhuCQSSC.strQishu,0,sizeof(TouzhuCQSSC.strQishu));

	CString strQiHao;
// 	if(m_TypeID == CZChongQingSSC)
// 		strQiHao = m_chqRule.GetNextExpect();
// 	else if(m_TypeID == CZ_TianJinSSC)
// 		strQiHao = m_jxRule.GetNextExpect();
// 	else if(m_TypeID == CZXinJiangSSC)
// 		strQiHao = m_xjRule.GetNextExpect();
// 	else if(m_TypeID == CZ_FENFEN_CAI)
// 		strQiHao = m_ffcRule.GetNextExpect();
// 	else if(m_TypeID == CZ_WUFEN_CAI)
		strQiHao = m_chqRule.GetNextExpect();

	string sNowQi;
	ChangeStringToA(strQiHao,sNowQi);


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
	if(m_zongZhuShu<=0)
	{
		m_DlgStatus.HideStatusWindow();
		OnBnClickedBtnClsList();
		return;
	}
	int nZhushu = 0;
	CString strZhu = m_listChqSSC.GetItemText(n, 4);
	nZhushu += _ttoi(strZhu);

	TouzhuCQSSC.nZhushu = nZhushu;


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
			SetUserBonus.n_t_type_id = CZ_QiXingCai;
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
			GetUserFandian.n_t_type_id = CZ_QiXingCai;
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
	if(nSendType == 8)
	{
		CMD_GP_GetMoreRecord GetMoreRecord;
		ZeroMemory(&GetMoreRecord,sizeof(GetMoreRecord));

		GetMoreRecord.n_t_type = CZ_QiXingCai;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_MORE_RECORD,&GetMoreRecord,sizeof(GetMoreRecord));

		return;
	}
}
VOID CQiXingCai::SendQueryLuckyNumCQSSC()
{
	return;
}


//服务器端返回前N个开奖号码
bool CQiXingCai::GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nIndex)
{
	_tcscpy_s(m_kjXinxi[nIndex].qihao, KJ_QIHAO_LENGTH, pResult->wPeriod);
	_tcscpy_s(m_kjXinxi[nIndex].haoma, KJ_HAOMA_LENGTH, pResult->szLotNum);
	_tcscpy_s(m_kjXinxi[nIndex].shijian, KJ_SHIJIAN_LENGTH, pResult->szShijian);

	if(nIndex+1 < TOP_KAIJIANG_NUMBERS)
	{
		return true;
	}

	m_kjXinxiCont = TOP_KAIJIANG_NUMBERS;
	if(!IsWindowVisible())
		return false;

	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
	{
		//第一次进入
		if(m_lastExpect[0] == 0)
			_tcscpy_s(m_lastExpect, KJ_HAOMA_LENGTH, m_kjXinxi[0].qihao);
		m_bKaiJiangzhong=false;											//开奖结束
		long secDiff = m_chqRule.GetKjShjDiff();

		KillTimer(timer_id_kaijiangzhong);
	}
// 	if(m_bKaiJiangzhong)
// 	{
// 		CRect rcCnt;
// 		rcCnt.left = rcRedraw.left+200;
// 		rcCnt.right = rcRedraw.right;
// 		rcCnt.top = rcRedraw.top;
// 		rcCnt.bottom = rcRedraw.bottom;
// 		RedrawWindow(&rcCnt,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
// 	}
// 	else
	{
		RedrawWindow(&rcRedraw,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

bool CQiXingCai::LuckyNumCQSSC()
{
	return true;

}


long CQiXingCai::GetKjShjDiff()
{
	long secDiff = m_chqRule.GetKjShjDiff();

	if (secDiff <= 60 )
	{
		return 0;
	}
 	else if(secDiff > (m_chqRule.GetQiSpan() ))
	{
 		return 0;
	}
	return secDiff;
}


//上期开奖号码
void CQiXingCai::DrawLastKjHaoma(CDC* pDC, Graphics& graphics)
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
	pDC->DrawText(_T("七星彩"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	CRect rcDi;
	rcDi.left = rcCzh.right+10;
	rcDi.right = rcDi.left+20;
	rcDi.top = rcCzh.top;
	rcDi.bottom = rcCzh.bottom;
	pDC->DrawText(_T("第"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(253, 191, 6));
	if(m_bKaiJiangzhong)
	{
		pDC->DrawText(m_chqRule.GetNextExpect(-1), &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else
		pDC->DrawText(m_lastExpect, &rcQiHao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(255, 255, 255));

	rcDi.left = rcQiHao.right-12;
	rcDi.right = rcDi.left+90;
	rcDi.top = rcCzh.top;
	rcDi.bottom = rcCzh.bottom;
	pDC->DrawText(_T("开奖号码"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(&m_KjInfoFont1);
	CString strYg;
	CRect rcYuGao1;
	//strYg.Format(_T("正在销售"));
	//pDC->DrawText(strYg, &rcYuGao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);


	strYg.Format(_T("第%s	下单时间"), m_chqRule.GetNextExpect());
	//pDC->SetTextColor(RGB(255, 125, 0));
	rcYuGao1.left = rcYuGao.left+15;
	rcYuGao1.top = rcYuGao.top+40;
	rcYuGao1.bottom = rcYuGao.bottom+70;
	rcYuGao1.right = rcYuGao.right+50;
	pDC->DrawText(strYg, &rcYuGao, DT_CENTER|DT_VCENTER|DT_SINGLELINE);


	// 	strYg.Format(_T("距封单还有"));
	// 	//pDC->SetTextColor(RGB(255, 226, 109));
	// 	rcYuGao1.left = rcYuGao1.left;
	// 	rcYuGao1.top = rcYuGao1.top+20;
	// 	rcYuGao1.bottom = rcYuGao1.bottom+20;
	// 	rcYuGao1.right = rcYuGao1.right;
	// 	pDC->DrawText(strYg, &rcYuGao1, DT_CENTER|DT_SINGLELINE);
	pDC->SelectObject(&m_KjInfoFont);
	strYg.Format(_T("%s"), GetKjShjDiffDesc(20));
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

	CString temp;
	temp.Format(L"%c",m_kjXinxi[0].haoma[0]);

	if(_ttoi(temp) > 9)
	{
		m_bKaiJiangzhong=true;
		KillTimer(timer_id_kaijiangzhong);
		SetTimer(timer_id_kaijiangzhong, 100, NULL);

		return;
	}
	int index = 0;
	for (int i=0; i<4; i++) 
	{
		if (iswdigit(str.GetAt(i))) 
		{
			int num = _ttoi(CString(str.GetAt(i)));
			if (num >=0 && num < 10) 
			{
				if(m_bKaiJiangzhong )
				{
					KillTimer(timer_id_kaijiangzhong);
					SetTimer(timer_id_kaijiangzhong, 100, NULL);
				}
				else
				{
					DrawBigNum(graphics, m_kjNumBig, Rect(kj_big_haoma_x + index * (m_bigNumWidth+2), kj_big_haoma_y, m_bigNumWidth, m_bigNumHeight), num);
				}
			}

			index ++;
		}
	}
}
CString CQiXingCai::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = m_chqRule.GetKjShjDiff();

	int hour = 0;
	int minute = 0;
	int second = 0;
	int nTemp = 0;
	int day = secDiff/(24*3600);
	nTemp = secDiff%(24*3600);
	hour = nTemp/3600;
	nTemp = secDiff%3600;
	minute = nTemp / 60 /*- 1*/;
	second = nTemp % 60;
	CString str;
	str.Format(_T("%d天%02d:%02d:%02d"),day,hour, minute, second);

	return str;


}
//开奖号码
void CQiXingCai::DrawTopKjHaoma(CDC* pDC, Graphics& graphics)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_smallNumfont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(250 ,250 ,250));
	CRect rcRect;
	rcRect.left = rcCzh.left+100;
	rcRect.top = rcCzh.top;
	rcRect.right = rcCzh.right+100;
	rcRect.bottom = rcCzh.bottom;
	//pDC->DrawText(_T("七星彩"), &rcRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	for (int cnt=0; cnt<4; cnt++) 
	{
		CString str = m_kjXinxi[cnt].qihao;
		if(str.IsEmpty())
		{
			continue;
		}

		CRect rcText(kj_small_haoma_x - 55, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 80, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span) + m_smallNumHeight);
		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		str = m_kjXinxi[cnt].haoma;
		CString temp;
		temp = str.GetAt(cnt);

		if(_ttoi(temp) > 9)
		{
			//	index ++;
			continue;
		}

		int index = 0;
		for (int i=0; i<str.GetLength(); i++) 
		{
			if (iswdigit(str.GetAt(i))) 
			{
				int num = _ttoi(CString(str.GetAt(i)));
				if (num >=0 && num < 10) 
				{
					Rect rc(kj_small_haoma_x+10 + index * (m_smallNumWidth + kj_small_haoma_col_span), 
						kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
						m_smallNumWidth, m_smallNumHeight);
					DrawSmallNum(graphics, m_kjNumSmall, rc, num);
				}

				index ++;
			}
		}


	}

	pDC->DrawText(strText, &rcZongJinE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}

void CQiXingCai::DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_bigNumWidth, 0, m_bigNumWidth, m_bigNumHeight, UnitPixel);
}

void CQiXingCai::DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_smallNumWidth, 0, m_smallNumWidth, m_smallNumHeight, UnitPixel);
}

void CQiXingCai::DrawZongZhushuJinE(CDC* pDC)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_zongFont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(160, 8, 8));
	strText.Format(_T("你选择了%d注"), m_singlezhushu);

	CRect rcZhu;
	rcZhu.CopyRect(rcZongZhuShu);
	rcZhu.left += 225;
	// 	rcZhu.top -= 17;
	// 	rcZhu.bottom -= 17;
	rcZhu.right += 475;
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);


	CRect rcRect;

	rcRect.left = rcZongZhuShu.left+27;
	rcRect.right = rcZongZhuShu.right+27;
	rcRect.top = rcZongZhuShu.top+15;
	rcRect.bottom = rcZongZhuShu.bottom+15;
	strText.Format(L"倍数：");
	pDC->DrawText(strText, &rcRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
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

	strText.Format(_T("共:%.3lf元"), m_singlezhushu * m_beishu * danzhujine);
	pDC->DrawText(strText, &rcZongJinE, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strText.Format(_T("共计%d注"),m_zongZhuShu);
	pDC->DrawText(strText, &rcAllZongzhushu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	rcZhu.CopyRect(rcAllZongzhushu);
	rcZhu.top += 20;
	rcZhu.bottom += 20;
	strText.Format(_T("共计:%.3lf元"),m_zongJine);
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}

void CQiXingCai::ResetAllNums()
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

	UnCheckNumBtn(m_btnNiu_0);
	UnCheckNumBtn(m_btnNiu_1);
	UnCheckNumBtn(m_btnNiu_2);
	UnCheckNumBtn(m_btnNiu_3);
	UnCheckNumBtn(m_btnNiu_4);
	UnCheckNumBtn(m_btnNiu_5);
	UnCheckNumBtn(m_btnNiu_6);
	UnCheckNumBtn(m_btnNiu_7);
	UnCheckNumBtn(m_btnNiu_8);
	UnCheckNumBtn(m_btnNiu_9);
	UnCheckNumBtn(m_btnNiu_10);

	UnCheckNumBtn(m_btnWan_0);
	UnCheckNumBtn(m_btnWan_1);
	UnCheckNumBtn(m_btnWan_2);
	UnCheckNumBtn(m_btnWan_3);
	UnCheckNumBtn(m_btnWan_4);
	UnCheckNumBtn(m_btnWan_5);
	UnCheckNumBtn(m_btnWan_6);
	UnCheckNumBtn(m_btnWan_7);
	UnCheckNumBtn(m_btnWan_8);
	UnCheckNumBtn(m_btnWan_9);

	UnCheckNumBtn(m_btnQian_0);
	UnCheckNumBtn(m_btnQian_1);
	UnCheckNumBtn(m_btnQian_2);
	UnCheckNumBtn(m_btnQian_3);
	UnCheckNumBtn(m_btnQian_4);
	UnCheckNumBtn(m_btnQian_5);
	UnCheckNumBtn(m_btnQian_6);
	UnCheckNumBtn(m_btnQian_7);
	UnCheckNumBtn(m_btnQian_8);
	UnCheckNumBtn(m_btnQian_9);

	UnCheckNumBtn(m_btnBai_0);
	UnCheckNumBtn(m_btnBai_1);
	UnCheckNumBtn(m_btnBai_2);
	UnCheckNumBtn(m_btnBai_3);
	UnCheckNumBtn(m_btnBai_4);
	UnCheckNumBtn(m_btnBai_5);
	UnCheckNumBtn(m_btnBai_6);
	UnCheckNumBtn(m_btnBai_7);
	UnCheckNumBtn(m_btnBai_8);
	UnCheckNumBtn(m_btnBai_9);

	UnCheckNumBtn(m_btnShi_0);
	UnCheckNumBtn(m_btnShi_1);
	UnCheckNumBtn(m_btnShi_2);
	UnCheckNumBtn(m_btnShi_3);
	UnCheckNumBtn(m_btnShi_4);
	UnCheckNumBtn(m_btnShi_5);
	UnCheckNumBtn(m_btnShi_6);
	UnCheckNumBtn(m_btnShi_7);
	UnCheckNumBtn(m_btnShi_8);
	UnCheckNumBtn(m_btnShi_9);

	UnCheckNumBtn(m_btnGe_0);
	UnCheckNumBtn(m_btnGe_1);
	UnCheckNumBtn(m_btnGe_2);
	UnCheckNumBtn(m_btnGe_3);
	UnCheckNumBtn(m_btnGe_4);
	UnCheckNumBtn(m_btnGe_5);
	UnCheckNumBtn(m_btnGe_6);
	UnCheckNumBtn(m_btnGe_7);
	UnCheckNumBtn(m_btnGe_8);
	UnCheckNumBtn(m_btnGe_9);

	UnCheckNumBtn(m_btnDaShi);
	UnCheckNumBtn(m_btnXiaoShi);
	UnCheckNumBtn(m_btnDanShi);
	UnCheckNumBtn(m_btnShuangShi);
	UnCheckNumBtn(m_btnDaGe);
	UnCheckNumBtn(m_btnXiaoGe);
	UnCheckNumBtn(m_btnDanGe);
	UnCheckNumBtn(m_btnShuangGe);
	m_richDanshiHaoma.SetWindowText(_T(""));

	//m_editDanShiHaoMa.SetWindowText(_T(""));
}

void CQiXingCai::SetLockText(float fBonus, float fBonusPercent)
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

//获取组三形式的号码
bool CQiXingCai::GetZuSanHaoma(CStringArray& strArr)
{
	CString line;
	//m_editDanShiHaoMa.GetWindowText(line);
	m_richDanshiHaoma.GetWindowText(line);
	if(line.GetLength() > 2999)
	{
//		MyMessageBox(_T("您输入的号码太多了"));
		MyMessageBox(_T("您输入的号码太多了"));
		return false;
	}
	if (!GetHaomaStrs(strArr, line, 3)) 
	{
		return false;
	}

	int count = strArr.GetCount();
	for(int i=0; i<count; i++)
	{
		if (!IsZuSanHaoma(strArr[i])) 
		{
			return false;
		}
	}

	return true;
}

bool CQiXingCai::GetZuSanHaoma(CString& haoma, int& zhushu)
{
	CStringArray strArr;
	
	if(!GetZuSanHaoma(strArr))
	{
		zhushu = 0;
		return false;
	}

	int count = strArr.GetCount();
	for(int i=0; i<count; i++)
	{
		haoma += strArr.GetAt(i);

		if (i < count - 1)
		{
			haoma += _T(",");
		}
	}
	zhushu = count;

	return true;
}

bool CQiXingCai::IsZuSanHaoma(const CString& str)
{
	if(str.GetLength() != 3)
	{
		return false;
	}

	TCHAR ch0 = str.GetAt(0);
	TCHAR ch1 = str.GetAt(1);
	TCHAR ch2 = str.GetAt(2);

	return (ch0 == ch1 || ch0 == ch2 || ch1 == ch2)
		&& (ch0 != ch1 || ch0 != ch2 || ch1 != ch2);
}

bool CQiXingCai::GetDanShiHaoma(CStringArray& strArr, int size)
{
	CString line;
	//m_editDanShiHaoMa.GetWindowText(line);
	m_richDanshiHaoma.GetWindowText(line);
	if(line.IsEmpty())
	{
		MyMessageBox(_T("请输入号码！"));	

		return false;
	}

	if(line.GetLength() > 2999)
	{
//		MyMessageBox(_T("您输入的号码太多了"));	
		MyMessageBox(_T("您输入的号码太多了"));	
		return false;
	}
	if (!GetHaomaStrs(strArr, line, size)) 
	{
		return false;
	}

	return true;
}

bool CQiXingCai::GetDanShiHaoma(CString& haoma, int size, int& zhushu, int nMax)
{
	CStringArray strArr;
	CString line;
	if(nMax == 0)
		nMax = 512000;
	m_richDanshiHaoma.GetWindowText(line);
	if(line.IsEmpty())
	{
		MyMessageBox(_T("请输入号码！"));	

		return false;
	}

	if(line.GetLength() > nMax)
	{
		MyMessageBox(_T("您输入的号码太多了"));	
		return false;
	}
	if (!GetHaomaStrs(strArr, line, size)) 
	{
		return false;
	}


	int count = strArr.GetCount();
	for(int i=0; i<count; i++)
	{
		if(m_gameKind == WuXingQianEr_DanShi || m_gameKind == WuXingHouEr_DanShi|| m_gameKind == QianErZuXuan_DanShi|| m_gameKind == HouErZuXuan_DanShi)
		{
			CString strTempHaoma;
			strTempHaoma += strArr.GetAt(i);

			int nLength = strTempHaoma.GetLength();
			for (int j = 0;j < nLength;j++)
			{
				haoma += strTempHaoma.GetAt(j);
				if(j!=nLength-1)
				{
					haoma+=_T(",");
				}
			}

		}
		else
		{
			haoma += strArr.GetAt(i);
		}
	//	if (i < count - 1)
		{
			haoma += _T("|");
		}
	}
	zhushu = count;


	return true;
}

bool CQiXingCai::GetHaomaStrs(CStringArray& strArr, CString line, int size)
{

	int len = line.GetLength();
	CString subStr,str;
	for (int i=0; i<len; i++) 
	{
		str = line.GetAt(i);
		if (iswdigit(line.GetAt(i))&&!str.IsEmpty()) 
		{
			subStr += line.GetAt(i);

			if(m_gameKind == QianErZuXuan_DanShi || m_gameKind == HouErZuXuan_DanShi)
			{
				if (subStr.GetLength() == size)
				{
					if(subStr[0] == subStr[1])
					{
						MyMessageBox(L"组选二不能选重复数字！");
						return false;
					}
				}
			}
			else if(m_gameKind == QianSanHunhe || m_gameKind == ZhongSanHunhe|| m_gameKind == HouSanHunhe)
			{
				if (subStr.GetLength() == size)
				{
					if(subStr[0] == subStr[1]&&subStr[2] == subStr[1])
					{
					//	MyMessageBox(L"混合不能选豹子数字！");

						return false;
					}
				}
			}
			if (subStr.GetLength() > size) 
			{
				return false;
			}
		}
		else
		{
			if (subStr.GetLength() != size && subStr.GetLength() != 0) 
			{
				return false;
			}

			if (subStr.GetLength() == size) 
			{
				strArr.Add(subStr);
				subStr = _T("");
			}
		}
	}
	if(( subStr.GetLength() == 0)&&(strArr.GetCount()==0))
	{
		MyMessageBox(_T("请输入号码！"));	

		return false;

	}

	if (subStr.GetLength() != size && subStr.GetLength() != 0) 
	{
		return false;
	}

	if (subStr.GetLength() == size) 
	{
		strArr.Add(subStr);
		subStr = _T("");
	}

	return true;
}

//初始化玩法信息
void CQiXingCai::InitWanFaMoShi()
{
	theGameKind[(CZ_QiXingCai << 16) | (unsigned short)QiXingCai_LiangDing] = _T("两定玩法");		
	theGameKind[(CZ_QiXingCai << 16) | (unsigned short)QiXingCai_SanDing] = _T("三定玩法");

	theGameKind[(CZ_QiXingCai << 16) | (unsigned short)QiXingCai_SiDing] = _T("四定玩法");
	theGameKind[(CZ_QiXingCai << 16) | (unsigned short)QiXingCai_YiDing] = _T("一定玩法");
	theGameKind[(CZ_QiXingCai << 16) | (unsigned short)QiXingCai_LiangZiXian] = _T("两字现");

	theGameKind[(CZ_QiXingCai << 16) | (unsigned short)QiXingCai_SanZiXian] = _T("三字现");
}

bool	CQiXingCai::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}
// bool	CQiXingCai::ChangeStringToA(CString strInput, string&  strOutput)
// {
// 	USES_CONVERSION;
// 	
// 	strOutput = T2A(strInput);
// 	return true;
// }

void	CQiXingCai::GetSubStringA(CStringA& strInput, int nPos, int nLen, CStringA& strOutput)
{

	strOutput = strInput.Mid(nPos, nLen);
	
}
CString	CQiXingCai::ChangeStringToT(CStringA strInput)
{
	CString  strOutput;
	USES_CONVERSION;
	strOutput = A2T(strInput);

	return strOutput;
}

//读取事件
bool CQiXingCai::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_MORE_RECORD_RET:
			{
				ASSERT((wDataSize % sizeof(CMD_GP_GetMoreRecordRet))==0);
				if((wDataSize %sizeof(CMD_GP_GetMoreRecordRet)) !=0)return false;

				int nCount = wDataSize/sizeof(CMD_GP_GetMoreRecordRet);
				for(int i = 0;i < nCount;i++)
				{
					CMD_GP_GetMoreRecordRet *pGetMoreRecord = ((CMD_GP_GetMoreRecordRet*)pData+i);

					m_DlgOpenRecord.SetOpenRecord(pGetMoreRecord,i);
				}

				m_DlgOpenRecord.ShowWindow(SW_SHOW);
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


//对象索引
WORD CQiXingCai::GetGameHoverIndex(CPoint MousePoint)
{
	if(!m_GameListRect.PtInRect(MousePoint))
	{
		return INVALID_WORD;
	}
	WORD wHoverItem = INVALID_WORD;

	return wHoverItem;
}

//鼠标消息
VOID CQiXingCai::OnMouseMove(UINT nFlags, CPoint Point)
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
VOID CQiXingCai::OnLButtonDown(UINT nFlags, CPoint Point)
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
VOID CQiXingCai::OnLButtonUp(UINT nFlags, CPoint Point)
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
LRESULT CQiXingCai::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{

	//设置状态
	m_bHovering=false;
// 
// 	m_wGameHoverItem=INVALID_WORD;
// 	CWnd* pParent = GetParent();
// 	if(pParent!=NULL)
// 	{
// 		pParent->SendMessage(IDM_SHOW_MENU_SSC,0,INVALID_WORD);
// 	}
// 	m_bShowMenu = false;
	//更新界面
//	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

	return 0;
}


//鼠标输入（任选2，任选3，任选4）
void CQiXingCai::OnBnClickedRioMouseInput()
{
	OnBnClickedBtnClsList();
	if(	m_gameKind == QiXingCai_LiangDing)
	{
		OnBnClickedRioLiangDing();
	}
	else if(m_gameKind == QiXingCai_SanDing)
	{
		OnBnClickedRioSanDing();
	}
	else if(m_gameKind == QiXingCai_SiDing)
	{
		OnBnClickedRioSiDing();
	}
	else if(m_gameKind == QiXingCai_YiDing)
	{
		OnBnClickedRioYiDing();
	}
	else if(m_gameKind == QiXingCai_LiangZiXian)
	{
		OnBnClickedRioLiangZiXian();
	}
	else if(m_gameKind == QiXingCai_SanZiXian)
	{
		OnBnClickedRioSanZiXian();
	}


	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);
	ResetAllNums();
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);


	HideDanShiEdit();
	m_RenxuanKeyboardInput=false;
}

//键盘输入（任选2，任选3，任选4）
void CQiXingCai::OnBnClickedRioKeyboardInput()
{
	OnBnClickedBtnClsList();
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);

	//任选2,3,4时，键盘输入的万，千，百，十，个
	if(m_gameKind == RenXuan2 || m_gameKind == RenXuan4||m_gameKind == RenXuan3)
		ShowRenxuanWeiShu();
	ResetAllNums();
	m_rioMouseInput.SetPushed(false);
	m_rioKeyboardInput.SetPushed(true);

	HideDaXiaoDanShuang();
	HideWanNums();
	HideQianNums();
	HideBaiNums();
	HideShiNums();
	HideGeNums();

	ShowDanShiEdit();
	m_RenxuanKeyboardInput=true;
}


//修改任选的 状态
void CQiXingCai::SetCheckRenXuan(bool bChecked)
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WAN);
	if(pBtn)
		pBtn->SetCheck(bChecked);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_QIAN);
	if(pBtn)
		pBtn->SetCheck(bChecked);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_BAI);
	if(pBtn)
		pBtn->SetCheck(bChecked);
	pBtn = (CButton*)GetDlgItem(IDC_CHK_SHI);
	if(pBtn)
		pBtn->SetCheck(bChecked);
	pBtn = (CButton*)GetDlgItem(IDC_CHKGE);
	if(pBtn)
		pBtn->SetCheck(bChecked);
	ValidateTips();
}

void CQiXingCai::OnBnClickedChkWan()
{
	ValidateTips();
}
void CQiXingCai::OnBnClickedChkQian()
{
	ValidateTips();
}

void CQiXingCai::OnBnClickedChkBai()
{
	ValidateTips();
}

void CQiXingCai::OnBnClickedChkShi()
{
	ValidateTips();
}

void CQiXingCai::OnBnClickedChkge()
{
	ValidateTips();
}

//更新提示
void CQiXingCai::ValidateTips()
{
	int nCheckCount=0;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WAN);
	if(pBtn && pBtn->GetCheck())
		nCheckCount++;
	pBtn = (CButton*)GetDlgItem(IDC_CHK_QIAN);
	if(pBtn && pBtn->GetCheck())
		nCheckCount++;
	pBtn = (CButton*)GetDlgItem(IDC_CHK_BAI);
	if(pBtn && pBtn->GetCheck())
		nCheckCount++;
	pBtn = (CButton*)GetDlgItem(IDC_CHK_SHI);
	if(pBtn && pBtn->GetCheck())
		nCheckCount++;
	pBtn = (CButton*)GetDlgItem(IDC_CHKGE);
	if(pBtn && pBtn->GetCheck())
		nCheckCount++;
	//判断是任选？
	CStatic* pSta = (CStatic*)GetDlgItem(IDC_STATIC_RENXUAN_TIP);
	if(RenXuan2 == m_gameKind)
	{
		if(nRenXuan2[nCheckCount] == 0)
		{
			if(pSta)
				pSta->SetWindowText(L"系统会自动根据勾选位置，组合对应的方数");
		}
		else
		{
			CString strTips;
			strTips.Format(L"您选择了%ld个位置，系统自动根据位置组合为%ld方", nCheckCount, nRenXuan2[nCheckCount]);
			if(pSta)
				pSta->SetWindowText(strTips);
		}
	}
	else if(RenXuan3 == m_gameKind)
	{
		if(nRenXuan3[nCheckCount] == 0)
		{
			if(pSta)
				pSta->SetWindowText(L"系统会自动根据勾选位置，组合对应的方数");
		}
		else
		{
			CString strTips;
			strTips.Format(L"您选择了%ld个位置，系统自动根据位置组合为%ld方", nCheckCount, nRenXuan3[nCheckCount]);
			if(pSta)
				pSta->SetWindowText(strTips);
		}
	}
	else if(RenXuan4 == m_gameKind)
	{
		if(nRenXuan4[nCheckCount] == 0)
		{
			if(pSta)
				pSta->SetWindowText(L"系统会自动根据勾选位置，组合对应的方数");
		}
		else
		{
			CString strTips;
			strTips.Format(L"您选择了%ld个位置，系统自动根据位置组合为%ld方", nCheckCount, nRenXuan4[nCheckCount]);
			if(pSta)
				pSta->SetWindowText(strTips);
		}
	}
}

//任选的万，千，百，十，个
int CQiXingCai::GetRenXuanCheckBox(BOOL& bWan, BOOL& bQian, BOOL& bBai, BOOL& bShi, BOOL& bGe)		
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WAN);
	if(pBtn)
		bWan = pBtn->GetCheck();
	pBtn = (CButton*)GetDlgItem(IDC_CHK_QIAN);
	if(pBtn)
		bQian = pBtn->GetCheck();
	pBtn = (CButton*)GetDlgItem(IDC_CHK_BAI);
	if(pBtn)
		bBai = pBtn->GetCheck();
	pBtn = (CButton*)GetDlgItem(IDC_CHK_SHI);
	if(pBtn)
		bShi = pBtn->GetCheck();
	pBtn = (CButton*)GetDlgItem(IDC_CHKGE);
	if(pBtn)
		bGe = pBtn->GetCheck();
	int nNums=0;
	nNums = bWan + bQian + bBai+ bShi+bGe;
	return nNums;
}

//设置返点
void CQiXingCai::OnBnClickedBtnLock()
{
	int index = (CZ_QiXingCai << 16) | (unsigned short)m_gameKind;

	
	CDlgFandian	dlg;
	dlg.SetBonus(mapBonus[m_gameKind], theBonus[index], mapPercent[m_gameKind], theAccount.fandian*100.0, mapDanweiFandian[m_gameKind]);
	if(IDOK == dlg.DoModal())
	{
		dlg.GetBonus(m_fBonus,  m_fPercent);

		SendToServer(5);
	}
}


//倍投加
void CQiXingCai::OnBnClickedBeiTouJia()
{
	CString strBeishu;

	m_beishu += 1;

	strBeishu.Format(L"%d",m_beishu);

	m_editBeiTou.SetWindowText(strBeishu);
	return;

}
//倍投减
void CQiXingCai::OnBnClickedBeiTouJian()
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




/////////////////////////////////////////////////////////////////////////////////////
bool CQiXingCai::GetKeyBoardHaoma(CString& haoma, int size, int nLeast)
{
	CStringArray strArr;
	CString line;

	m_richDanshiHaoma.SetSel(0,-1);
	line = m_richDanshiHaoma.GetSelText();

	int nFind = line.Find(L"，");
	if(nFind != -1)
	{
		line.Replace(_T("，"),_T(","));
	}

	line.Replace(L"\r",L" ");
	line.Replace(L"\n",L" ");

	if (!GetKeyHaomaStrs(strArr, line, size,nLeast)) 
	{
		return false;
	}

	//haoma = strArr.GetAt(0);

	int count = strArr.GetCount();
	CString strTemp;
	for(int i=0; i<count; i++)
	{
		haoma += strArr.GetAt(i);
		haoma +=_T("|");
		//strTemp = strArr.GetAt(i);
		//int nWeishu = strTemp.GetLength();
		//if(nWeishu > nLeast)
		//{
		//	MyMessageBox(_T("您输入的号码太多了"));	
		//	return false;

		//}
		//for(int j = 0; j<strTemp.GetLength();j++)
		//{
		//	haoma += strTemp.GetAt(j);
		//	//if (m_gameKind!=WuxingZhixuan && (j!=nWeishu-1))
		//	//{
		//	//	haoma += _T(",");
		//	//}

		//	//if(j == nWeishu-1)
		//	//{
		//	//	haoma +=_T("|");
		//	//}

		//}

	}

	return true;
}

bool CQiXingCai::GetKeyHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast)
{
	int len = line.GetLength();
	CString subStr;
	CString strTemp;
	int nWayCount = 0;
	int NumCount = 0;
	int LastNumCount = 0;

	for (int i=0; i<len; i++) 
	{
		CString str;
		str=line.GetAt(i);

		int nTemp = 10;
		if (iswdigit(line.GetAt(i))&&!str.IsEmpty()/*&&str.Compare(L"\r\n")!=0*/)
		{
			subStr += line.GetAt(i);

			strTemp+= line.GetAt(i);

			nTemp = _ttoi(strTemp);
			strTemp=_T("");
			if(nTemp<0||nTemp>9)
			{
				MyMessageBox(L"您的输入有误，数字只能在0～9之间 ");
				return false;
			}
		}
		else
		{
			NumCount++;

			if (subStr.GetLength()!=LastNumCount)//如果和上次比，增加了数字,表示有一位已填
			{
				nWayCount++;
			}

			if (NumCount > size )
			{
				if (nWayCount != nLeast)//需要对应位数 的号码。
				{
					return false;
				}
				NumCount = 0;
				strArr.Add(subStr);
				subStr = _T("");
			}
			else
			{
				subStr = _T(",");
			}
		}
	}

	if(( subStr.GetLength() == 0)&&(strArr.GetCount()==0))
	{
		MyMessageBox(_T("请输入号码！"));	

		return false;
	}
	if (NumCount != size && strArr.GetCount())
	{
		return false;
	}
	else if (subStr.GetLength() != 0) 
	{
		strArr.Add(subStr);
		subStr = _T("");
	}



	

	return true;
}
LRESULT CQiXingCai::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	//OnBnClickedBtnDelSel();
	nSubItem--;

	int zhushu = _ttoi(m_listChqSSC.GetItemText(nItem, 4));
	m_zongZhuShu -= zhushu;
	m_listChqSSC.deleteItemEx( nItem );
	int nItemCount = m_listChqSSC.GetItemCount();
	m_zongJine = 0.00f;
	for(int i = 0;i < nItemCount;i++)
	{
		CString strBeiSHu = m_listChqSSC.GetItemText(i,5);
		CString strZhuSHu = m_listChqSSC.GetItemText(i,4);
		CString strMoshi = m_listChqSSC.GetItemText(i,1);
		int nMoshi = GetMoshiRet(strMoshi);

		double danzhujine = 2.0;

		if(nMoshi == MoShi_Yuan)
		{	
			danzhujine = 2.0;
		}
		else if (nMoshi == MoShi_Jiao)
		{
			danzhujine = 0.2;
		}
		else if(nMoshi == MoShi_Fen)
		{
			danzhujine = 0.02;
		}
		else if(nMoshi == MoShi_Li)
			danzhujine = 0.002;

		int beishu = _ttoi(strBeiSHu);
		int zhushu = _ttoi(strZhuSHu);


		m_zongJine += zhushu*beishu*danzhujine;
	}
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


BOOL CQiXingCai::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	if((nCommandID >= IDC_BTN_WAN_0 && nCommandID <= IDC_BTN_GE_9)||(nCommandID >= IDC_BTN_DA_SHI && nCommandID <= IDC_BTN_WAN_2)||(nCommandID >= IDC_BTN_HEZHI_0 && nCommandID <= IDC_BTN_HEZHI_27)||(nCommandID >= IDC_BTN_DA_WAN && nCommandID <= IDC_BTN_SHUANG_BAI)||(nCommandID >= IDC_BTN_NIU_1 && nCommandID <= IDC_BTN_NIU_10))
	{
		int nZhushu = GetQXCZhushu(CZ_QiXingCai,m_gameKind);
	}

	return  __super::OnCommand(wParam,lParam);;
}

int CQiXingCai::GetQXCZhushu(int nTypeID,int nKindID)
{
	CString strWan = GetWanString();
	CString strQian = GetQianString();
	CString strBai = GetBaiString();
	CString strShi = GetShiString();
	CString strGe = GetGeString();
	m_singlezhushu = 0;

	if(m_gameKind == QiXingCai_LiangDing)
	{
		int zhushu = 1;
		int nTempHaoMaCount =0;
		if (!strQian.IsEmpty())//千
			nTempHaoMaCount+=1;
		if (!strBai.IsEmpty())//百
			nTempHaoMaCount+=1;
		if (!strShi.IsEmpty())//十
			nTempHaoMaCount+=1;
		if (!strGe.IsEmpty())//个
			nTempHaoMaCount+=1;


		if(nTempHaoMaCount!=2) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		} 

		m_singlezhushu = _MAX(strQian.GetLength(),1) * _MAX(strBai.GetLength(),1) * _MAX(strShi.GetLength(),1) * _MAX(strGe.GetLength(),1);
	}
	else if (m_gameKind == QiXingCai_SanDing)
	{
		int nTempHaoMaCount =0;
		if (!strQian.IsEmpty())//千
			nTempHaoMaCount+=1;
		if (!strBai.IsEmpty())//百
			nTempHaoMaCount+=1;
		if (!strShi.IsEmpty())//十
			nTempHaoMaCount+=1;
		if (!strGe.IsEmpty())//个
			nTempHaoMaCount+=1;


		if(nTempHaoMaCount!=3) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = _MAX(strQian.GetLength(),1) * _MAX(strBai.GetLength(),1) * _MAX(strShi.GetLength(),1) * _MAX(strGe.GetLength(),1);

	}
	else if(m_gameKind == QiXingCai_SiDing)
	{
		int nTempHaoMaCount =0;
		if (!strQian.IsEmpty())//千
			nTempHaoMaCount+=1;
		if (!strBai.IsEmpty())//百
			nTempHaoMaCount+=1;
		if (!strShi.IsEmpty())//十
			nTempHaoMaCount+=1;
		if (!strGe.IsEmpty())//个
			nTempHaoMaCount+=1;


		if(nTempHaoMaCount!=4) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = _MAX(strQian.GetLength(),1) * _MAX(strBai.GetLength(),1) * _MAX(strShi.GetLength(),1) * _MAX(strGe.GetLength(),1);

	}
	else if(m_gameKind == QiXingCai_YiDing)
	{
		int nTempHaoMaCount =0;
		if (!strQian.IsEmpty())//千
			nTempHaoMaCount+=1;
		if (!strBai.IsEmpty())//百
			nTempHaoMaCount+=1;
		if (!strShi.IsEmpty())//十
			nTempHaoMaCount+=1;
		if (!strGe.IsEmpty())//个
			nTempHaoMaCount+=1;


		if(nTempHaoMaCount!=1) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = _MAX(strQian.GetLength(),1) * _MAX(strBai.GetLength(),1) * _MAX(strShi.GetLength(),1) * _MAX(strGe.GetLength(),1);

	}
	else if (m_gameKind == QiXingCai_LiangZiXian)
	{
		if(strBai.GetLength()!=2) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = 1;

	}
	else if (m_gameKind == QiXingCai_SanZiXian)
	{
		if(strBai.GetLength()!=3) 
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = 1;

	}

	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	return m_singlezhushu;
}
