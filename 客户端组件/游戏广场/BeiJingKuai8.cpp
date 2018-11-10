// BeiJingKuai8.cpp : 实现文件
//

#include "stdafx.h"
#include "GamePlaza.h"
#include "BeiJingKuai8.h"
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
CString   CBeiJingKuai8::m_strHaoma = _T("");


extern int g_arr[MAXN];
extern int g_nPos[512000][5];
extern int g_Counts;



//大开奖号
static const int kj_big_haoma_x = 209;
static const int kj_big_haoma_y = 40;

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
static const int kj_small_haoma_x = 637;
static const int kj_small_haoma_y = 35;
static const int kj_small_haoma_col_span = 1;
static const int kj_small_haoma_row_span = 2;

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
static const int wanfa_col_width = 56;
static const int wanfa_row_height = 32;

//玩法子分类数据
static const int wanfa_sel_x = 20;
static const int wanfa_sel_y = 145;
static const int wanfa_sel_width = 71;
static const int wanfa_sel_height = 22;


//号码数据
static const int haoma_btn_x = 100;
static const int haoma_btn_y = 226;
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


// CBeiJingKuai8 对话框

IMPLEMENT_DYNAMIC(CBeiJingKuai8, CDialog)

CBeiJingKuai8::CBeiJingKuai8(CWnd* pParent /*=NULL*/)
	: CDialog(CBeiJingKuai8::IDD, pParent)
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

	m_gameKind = enBJK8_RX_1;
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
VOID CBeiJingKuai8::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}


CBeiJingKuai8::~CBeiJingKuai8()
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
	if (m_numImageList4 != NULL)
	{
		m_numImageList4=NULL;
		delete m_numImageList4;
	}

	if (m_kjNumBig != NULL) 
	{
		m_kjNumBig=NULL;
		delete m_kjNumBig;
	}
	if (m_kjNumBig2 != NULL) 
	{
		m_kjNumBig2=NULL;
		delete m_kjNumBig;
	}
	if (m_kjAdd != NULL) 
	{
		m_kjAdd=NULL;
		delete m_kjNumBig;
	}

	if (m_kjNumSmall != NULL) 
	{
		m_kjNumSmall=NULL;
		delete m_kjNumSmall;
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

void CBeiJingKuai8::ConnectMainDlg(CPlazaViewItem* luckMeDlg)
{
	m_pLuckMeDlg = luckMeDlg;
}

void CBeiJingKuai8::FlushZongjine()
{
	InvalidateRect(&rcZongJinE);
}

void CBeiJingKuai8::OnBnClickedBtnYuan()
{
	m_btnYuan.SetPushed(true);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Yuan ;

	InvalidateRect(rcZongJinE);

	return;
}
void CBeiJingKuai8::OnBnClickedBtnJiao()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(true);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Jiao ;
	InvalidateRect(rcZongJinE);

	return;
}
void CBeiJingKuai8::OnBnClickedBtnFen()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(true);
	m_btnLi.SetPushed(false);
	theAccount.Scoretype=MoShi_Fen ;
	InvalidateRect(rcZongJinE);
	return;

}
void CBeiJingKuai8::OnBnClickedBtnLi()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(true);
	theAccount.Scoretype=MoShi_Li ;
	InvalidateRect(rcZongJinE);

	return;
}

void CBeiJingKuai8::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TIP_WXZX,m_btWxzxTip);

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

	DDX_Control(pDX, IDC_BTN_BJ_SHANG, m_btnShang);
	DDX_Control(pDX, IDC_BTN_BJ_ZHONG, m_btnZhong);
	DDX_Control(pDX, IDC_BTN_BJ_XIA, m_btnXia);
	DDX_Control(pDX, IDC_BTN_BJ_JI, m_btnJi);
	DDX_Control(pDX, IDC_BTN_BJ_HE, m_btnHe);
	DDX_Control(pDX, IDC_BTN_BJ_OU, m_btnOu);
	DDX_Control(pDX, IDC_BTN_BJ_DADAN, m_btnDaDan1);
	DDX_Control(pDX, IDC_BTN_BJ_DASHUANG, m_btnDaShuang1);
	DDX_Control(pDX, IDC_BTN_BJ_XIAODAN, m_btnXiaoDan1);
	DDX_Control(pDX, IDC_BTN_BJ_XIAOSHUANG, m_btnXiaoShuang1);

	DDX_Control(pDX, IDC_BTN_BJ_1, m_btnHuanLe_01);
	DDX_Control(pDX, IDC_BTN_BJ_2, m_btnHuanLe_02);
	DDX_Control(pDX, IDC_BTN_BJ_3, m_btnHuanLe_03);
	DDX_Control(pDX, IDC_BTN_BJ_4, m_btnHuanLe_04);
	DDX_Control(pDX, IDC_BTN_BJ_5, m_btnHuanLe_05);
	DDX_Control(pDX, IDC_BTN_BJ_6, m_btnHuanLe_06);
	DDX_Control(pDX, IDC_BTN_BJ_7, m_btnHuanLe_07);
	DDX_Control(pDX, IDC_BTN_BJ_8, m_btnHuanLe_08);
	DDX_Control(pDX, IDC_BTN_BJ_9, m_btnHuanLe_09);
	DDX_Control(pDX, IDC_BTN_BJ_10, m_btnHuanLe_10);
	DDX_Control(pDX, IDC_BTN_BJ_11, m_btnHuanLe_11);
	DDX_Control(pDX, IDC_BTN_BJ_12, m_btnHuanLe_12);
	DDX_Control(pDX, IDC_BTN_BJ_13, m_btnHuanLe_13);
	DDX_Control(pDX, IDC_BTN_BJ_14, m_btnHuanLe_14);
	DDX_Control(pDX, IDC_BTN_BJ_15, m_btnHuanLe_15);
	DDX_Control(pDX, IDC_BTN_BJ_16, m_btnHuanLe_16);
	DDX_Control(pDX, IDC_BTN_BJ_17, m_btnHuanLe_17);
	DDX_Control(pDX, IDC_BTN_BJ_18, m_btnHuanLe_18);
	DDX_Control(pDX, IDC_BTN_BJ_19, m_btnHuanLe_19);
	DDX_Control(pDX, IDC_BTN_BJ_20, m_btnHuanLe_20);
	DDX_Control(pDX, IDC_BTN_BJ_21, m_btnHuanLe_21);
	DDX_Control(pDX, IDC_BTN_BJ_22, m_btnHuanLe_22);
	DDX_Control(pDX, IDC_BTN_BJ_23, m_btnHuanLe_23);
	DDX_Control(pDX, IDC_BTN_BJ_24, m_btnHuanLe_24);
	DDX_Control(pDX, IDC_BTN_BJ_25, m_btnHuanLe_25);
	DDX_Control(pDX, IDC_BTN_BJ_26, m_btnHuanLe_26);
	DDX_Control(pDX, IDC_BTN_BJ_27, m_btnHuanLe_27);
	DDX_Control(pDX, IDC_BTN_BJ_28, m_btnHuanLe_28);
	DDX_Control(pDX, IDC_BTN_BJ_29, m_btnHuanLe_29);
	DDX_Control(pDX, IDC_BTN_BJ_30, m_btnHuanLe_30);
	DDX_Control(pDX, IDC_BTN_BJ_31, m_btnHuanLe_31);
	DDX_Control(pDX, IDC_BTN_BJ_32, m_btnHuanLe_32);
	DDX_Control(pDX, IDC_BTN_BJ_33, m_btnHuanLe_33);
	DDX_Control(pDX, IDC_BTN_BJ_34, m_btnHuanLe_34);
	DDX_Control(pDX, IDC_BTN_BJ_35, m_btnHuanLe_35);
	DDX_Control(pDX, IDC_BTN_BJ_36, m_btnHuanLe_36);
	DDX_Control(pDX, IDC_BTN_BJ_37, m_btnHuanLe_37);
	DDX_Control(pDX, IDC_BTN_BJ_38, m_btnHuanLe_38);
	DDX_Control(pDX, IDC_BTN_BJ_39, m_btnHuanLe_39);
	DDX_Control(pDX, IDC_BTN_BJ_40, m_btnHuanLe_40);
	DDX_Control(pDX, IDC_BTN_BJ_41, m_btnHuanLe_41);
	DDX_Control(pDX, IDC_BTN_BJ_42, m_btnHuanLe_42);
	DDX_Control(pDX, IDC_BTN_BJ_43, m_btnHuanLe_43);
	DDX_Control(pDX, IDC_BTN_BJ_44, m_btnHuanLe_44);
	DDX_Control(pDX, IDC_BTN_BJ_45, m_btnHuanLe_45);
	DDX_Control(pDX, IDC_BTN_BJ_46, m_btnHuanLe_46);
	DDX_Control(pDX, IDC_BTN_BJ_47, m_btnHuanLe_47);
	DDX_Control(pDX, IDC_BTN_BJ_48, m_btnHuanLe_48);
	DDX_Control(pDX, IDC_BTN_BJ_49, m_btnHuanLe_49);
	DDX_Control(pDX, IDC_BTN_BJ_50, m_btnHuanLe_50);
	DDX_Control(pDX, IDC_BTN_BJ_51, m_btnHuanLe_51);
	DDX_Control(pDX, IDC_BTN_BJ_52, m_btnHuanLe_52);
	DDX_Control(pDX, IDC_BTN_BJ_53, m_btnHuanLe_53);
	DDX_Control(pDX, IDC_BTN_BJ_54, m_btnHuanLe_54);
	DDX_Control(pDX, IDC_BTN_BJ_55, m_btnHuanLe_55);
	DDX_Control(pDX, IDC_BTN_BJ_56, m_btnHuanLe_56);
	DDX_Control(pDX, IDC_BTN_BJ_57, m_btnHuanLe_57);
	DDX_Control(pDX, IDC_BTN_BJ_58, m_btnHuanLe_58);
	DDX_Control(pDX, IDC_BTN_BJ_59, m_btnHuanLe_59);
	DDX_Control(pDX, IDC_BTN_BJ_60, m_btnHuanLe_60);
	DDX_Control(pDX, IDC_BTN_BJ_61, m_btnHuanLe_61);
	DDX_Control(pDX, IDC_BTN_BJ_62, m_btnHuanLe_62);
	DDX_Control(pDX, IDC_BTN_BJ_63, m_btnHuanLe_63);
	DDX_Control(pDX, IDC_BTN_BJ_64, m_btnHuanLe_64);
	DDX_Control(pDX, IDC_BTN_BJ_65, m_btnHuanLe_65);
	DDX_Control(pDX, IDC_BTN_BJ_66, m_btnHuanLe_66);
	DDX_Control(pDX, IDC_BTN_BJ_67, m_btnHuanLe_67);
	DDX_Control(pDX, IDC_BTN_BJ_68, m_btnHuanLe_68);
	DDX_Control(pDX, IDC_BTN_BJ_69, m_btnHuanLe_69);
	DDX_Control(pDX, IDC_BTN_BJ_70, m_btnHuanLe_70);
	DDX_Control(pDX, IDC_BTN_BJ_71, m_btnHuanLe_71);
	DDX_Control(pDX, IDC_BTN_BJ_72, m_btnHuanLe_72);
	DDX_Control(pDX, IDC_BTN_BJ_73, m_btnHuanLe_73);
	DDX_Control(pDX, IDC_BTN_BJ_74, m_btnHuanLe_74);
	DDX_Control(pDX, IDC_BTN_BJ_75, m_btnHuanLe_75);
	DDX_Control(pDX, IDC_BTN_BJ_76, m_btnHuanLe_76);
	DDX_Control(pDX, IDC_BTN_BJ_77, m_btnHuanLe_77);
	DDX_Control(pDX, IDC_BTN_BJ_78, m_btnHuanLe_78);
	DDX_Control(pDX, IDC_BTN_BJ_79, m_btnHuanLe_79);
	DDX_Control(pDX, IDC_BTN_BJ_80, m_btnHuanLe_80);

	DDX_Control(pDX, IDC_RIO_SZX,m_rioSZX);
	DDX_Control(pDX, IDC_RIO_JHO,m_rioJHO);
	DDX_Control(pDX, IDC_RIO_HZDXDS,m_rioHZDXDS);

	DDX_Control(pDX, IDC_BTN_K8RX, m_btnRenXuan);		//快8任选
	DDX_Control(pDX, IDC_BTN_TEMA, m_btnTeMa);			
	DDX_Control(pDX, IDC_BTN_DXDS, m_btnDaXiaoDanShuang);
	DDX_Control(pDX, IDC_BTN_FUNNY, m_btnFunny);
	DDX_Control(pDX, IDC_BTN_JIZHI, m_btnJiZhi);
	DDX_Control(pDX, IDC_BTN_RX6, m_btnRenXuan6);		//快8任选
	DDX_Control(pDX, IDC_BTN_RX7, m_btnRenXuan7);			
	DDX_Control(pDX, IDC_BTN_RX8, m_btnRenXuan8);
	DDX_Control(pDX, IDC_BTN_RX9, m_btnRenXuan9);
	DDX_Control(pDX, IDC_BTN_RX10, m_btnRenXuan10);
	DDX_Control(pDX, IDC_BTN_QW, m_btnQuWei);


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



	DDX_Control(pDX,IDC_BTN_BEITOU_JIA,m_btnBeiTouJia);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIAN,m_btnBeiTouJian);

	DDX_Control(pDX, IDC_BTN_YUAN,m_btnYuan);
	DDX_Control(pDX, IDC_BTN_JIAO,m_btnJiao);
	DDX_Control(pDX, IDC_BTN_FEN,m_btnFen);
	DDX_Control(pDX, IDC_BTN_LI,m_btnLi);
}


BEGIN_MESSAGE_MAP(CBeiJingKuai8, CDialog)
	ON_BN_CLICKED(IDC_BTN_YUAN, &CBeiJingKuai8::OnBnClickedBtnYuan)
	ON_BN_CLICKED(IDC_BTN_JIAO, &CBeiJingKuai8::OnBnClickedBtnJiao)
	ON_BN_CLICKED( IDC_BTN_FEN, &CBeiJingKuai8::OnBnClickedBtnFen)
	ON_BN_CLICKED(IDC_BTN_LI, &CBeiJingKuai8::OnBnClickedBtnLi)

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

	ON_BN_CLICKED(IDC_RIO_MOUSE_INPUT, &CBeiJingKuai8::OnBnClickedRioMouseInput)		//鼠标输入
	ON_BN_CLICKED(IDC_RIO_KEYBOARD_INPUT, &CBeiJingKuai8::OnBnClickedRioKeyboardInput)	//键盘输入


	//玩法选择
	ON_BN_CLICKED(IDC_BTN_K8RX, &CBeiJingKuai8::OnBnClinckedBtnRenXuan)//快8任选
	ON_BN_CLICKED(IDC_BTN_TEMA, &CBeiJingKuai8::OnBnClinckedBtnTeMa)		//特码
	ON_BN_CLICKED(IDC_BTN_DXDS, &CBeiJingKuai8::OnBnClinckedBtnDaXiaoDanShuang)//大小单双
	ON_BN_CLICKED(IDC_BTN_FUNNY, &CBeiJingKuai8::OnBnClinckedBtnFunny)	//趣味玩法
	ON_BN_CLICKED(IDC_BTN_JIZHI, &CBeiJingKuai8::OnBnClinckedBtnJiZhi)//极值
	ON_BN_CLICKED(IDC_BTN_RX6, &CBeiJingKuai8::OnBnClinckedBtnRenXuan6)//快8任选
	ON_BN_CLICKED(IDC_BTN_RX7, &CBeiJingKuai8::OnBnClinckedBtnRenXuan7)		//特码
	ON_BN_CLICKED(IDC_BTN_RX8, &CBeiJingKuai8::OnBnClinckedBtnRenXuan8)//大小单双
	ON_BN_CLICKED(IDC_BTN_RX9, &CBeiJingKuai8::OnBnClinckedBtnRenXuan9)	//趣味玩法
	ON_BN_CLICKED(IDC_BTN_RX10, &CBeiJingKuai8::OnBnClinckedBtnRenXuan10)//极值
	ON_BN_CLICKED(IDC_BTN_QW, &CBeiJingKuai8::OnBnClinckedBtnQuWei)//趣味


	ON_BN_CLICKED(IDC_RIO_DUIZI, &CBeiJingKuai8::OnRioDuiZi)
	ON_BN_CLICKED(IDC_RIO_SHUNZI, &CBeiJingKuai8::OnRioShunZi)
	ON_BN_CLICKED(IDC_RIO_BAOZI, &CBeiJingKuai8::OnRioBaoZi)

	ON_BN_CLICKED(IDC_RIO_SZX, &CBeiJingKuai8::OnBnClickedBtnSZX)
	ON_BN_CLICKED(IDC_RIO_JHO, &CBeiJingKuai8::OnBnClickedBtnJHO)
	ON_BN_CLICKED(IDC_RIO_HZDXDS, &CBeiJingKuai8::OnBnClickedBtnHZDXDS)
	

	//控制按钮
	ON_BN_CLICKED(IDC_BTN_ZHUIHAO, &CBeiJingKuai8::OnBnClickedBtnZhuihao)//追号
	ON_BN_CLICKED(IDC_BTN_TOUZHU, &CBeiJingKuai8::OnBnClickedBtnTouzhu)//投注
	ON_BN_CLICKED(IDC_BTN_CHQ_ADD, &CBeiJingKuai8::OnBnClickedBtnChqAdd)//添加号码
	ON_BN_CLICKED(IDC_BTN_DEL_SEL, &CBeiJingKuai8::OnBnClickedBtnDelSel)//删除
	ON_BN_CLICKED(IDC_BTN_CLS_LIST, &CBeiJingKuai8::OnBnClickedBtnClsList)//清空
	ON_BN_CLICKED(IDC_BTN_LOCK, &CBeiJingKuai8::OnBnClickedBtnLock)//设置返点
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIA, &CBeiJingKuai8::OnBnClickedBeiTouJia)
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIAN, &CBeiJingKuai8::OnBnClickedBeiTouJian)
	ON_MESSAGE( WM_BN_CLICK, &CBeiJingKuai8::onBnCLick)


	//投注修改
	ON_EN_CHANGE(IDC_EDIT_BEISHU, &CBeiJingKuai8::OnEnChangeEditBeishu)//修改倍数
	ON_EN_CHANGE(IDC_EDIT_DANSHI_HAOMA, &CBeiJingKuai8::OnEnChangeEditDanshiInput)//修改单数


END_MESSAGE_MAP()





// CBeiJingKuai8 消息处理程序
void CBeiJingKuai8::OnPaint()
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

BOOL CBeiJingKuai8::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CBeiJingKuai8::OnSize(UINT nType, int cx, int cy)
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
bool CBeiJingKuai8::CheckInput()
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
void CBeiJingKuai8::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;
		SendToServer(6);
		m_bKaiJiangzhong = false; 
		
		SetTimer(timer_id_kj_yugao, 1000, NULL);
		SetTimer(timer_id_kaijiangzhong, 100, NULL);
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



		OnBnClinckedBtnRenXuan();
	}

	InvalidateRect(CRect(0,0,1000,1000),true);
}

 //定时器
void CBeiJingKuai8::OnTimer(UINT_PTR nIDEvent)
{
	if (timer_id_kj_yugao == nIDEvent)
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
				int nLeft=kj_big_haoma_x-8 /*+ n *( m_bigNumWidth+8)*/;
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

HBRUSH CBeiJingKuai8::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CBeiJingKuai8::OnBnClickedBtnChqAdd()
{
	//m_DlgStatus.ShowStatusWindow(TEXT("正在添加数据，请稍候..."));
	int zhushu = 1;
	CString strHaoMa;

	if(m_gameKind == enBJK8_RX_1)
	{
		CString strNum;
		strNum = GetBjNum();

		zhushu=0;
		int nCount  = strNum.GetLength();
		if (nCount == 0)
		{
			MyMessageBox(L"请选择数字！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		if (nCount>26)
		{
			MyMessageBox(L"您选择的数字太多,最多选13注！");
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
				zhushu++;
			}
		}

		zhushu++;

	}
	else if(m_gameKind == enBJK8_RX_2)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 4)
		{
			MyMessageBox(L"格式不正确，请重新选择！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		if (nCount>26)
		{
			MyMessageBox(L"您选择的数字太多,最多选13注！");
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
		int count = nCount/2;
		zhushu = (count*(count-1))/2;

	}
	else if(m_gameKind == enBJK8_RX_3)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 6)
		{
			MyMessageBox(L"格式不正确，请重新选择！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		if (nCount>26)
		{
			MyMessageBox(L"您选择的数字太多,最多选13注！");
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
		int count = nCount/2;
		zhushu = (count*(count-1)*(count-2))/6;

	}
	else if(m_gameKind == enBJK8_RX_4)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 8)
		{
			MyMessageBox(L"格式不正确，请重新选择！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		if (nCount>26)
		{
			MyMessageBox(L"您选择的数字太多,最多选13注！");
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
		int count = nCount/2;
		zhushu = (count*(count-1)*(count-2)*(count-3))/24;

	}
	else if(m_gameKind == enBJK8_RX_5)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 10)
		{
			MyMessageBox(L"格式不正确，请重新选择！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		if (nCount>26)
		{
			MyMessageBox(L"您选择的数字太多,最多选13注！");
			m_DlgStatus.HideStatusWindow();
			return;
		}

		for(int i = 0;i < nCount;i++)
		{
			strHaoMa += strNum.GetAt(i);
			//strHaoMa += strNum.GetAt(i*2+1);
			if(i%2 != 0 && i!= nCount-1 )
			{
				strHaoMa+=_T(",");
			}
		}
		int count = nCount/2;
		zhushu = (count*(count-1)*(count-2)*(count-3)*(count-4))/120;

	}
	else if(m_gameKind == enBJK8_RX_6)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 12)
		{
			MyMessageBox(L"格式不正确，请重新选择！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		if (nCount>26)
		{
			MyMessageBox(L"您选择的数字太多,最多选13注！");
			m_DlgStatus.HideStatusWindow();
			return;
		}

		for(int i = 0;i < nCount;i++)
		{
			strHaoMa += strNum.GetAt(i);
			//strHaoMa += strNum.GetAt(i*2+1);
			if(i%2 != 0 && i!= nCount-1 )
			{
				strHaoMa+=_T(",");
			}
		}
		int count = nCount/2;
		zhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5))/720;

	}
	else if(m_gameKind == enBJK8_RX_7)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 14)
		{
			MyMessageBox(L"格式不正确，请重新选择！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		if (nCount>26)
		{
			MyMessageBox(L"您选择的数字太多,最多选13注！");
			m_DlgStatus.HideStatusWindow();
			return;
		}

		for(int i = 0;i < nCount;i++)
		{
			strHaoMa += strNum.GetAt(i);
			//strHaoMa += strNum.GetAt(i*2+1);
			if(i%2 != 0 && i!= nCount-1 )
			{
				strHaoMa+=_T(",");
			}
		}
		int count = nCount/2;
		zhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6))/5040;

	}
	else if(m_gameKind == enBJK8_RX_8)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 16)
		{
			MyMessageBox(L"格式不正确，请重新选择！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		if (nCount>26)
		{
			MyMessageBox(L"您选择的数字太多,最多选13注！");
			m_DlgStatus.HideStatusWindow();
			return;
		}

		for(int i = 0;i < nCount;i++)
		{
			strHaoMa += strNum.GetAt(i);
			//strHaoMa += strNum.GetAt(i*2+1);
			if(i%2 != 0 && i!= nCount-1 )
			{
				strHaoMa+=_T(",");
			}
		}
		int count = nCount/2;
		zhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6)*(count-7))/40320;

	}
	else if(m_gameKind == enBJK8_RX_9)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 18)
		{
			MyMessageBox(L"格式不正确，请重新选择！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		if (nCount>26)
		{
			MyMessageBox(L"您选择的数字太多,最多选13注！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		for(int i = 0;i < nCount;i++)
		{
			strHaoMa += strNum.GetAt(i);
			//strHaoMa += strNum.GetAt(i*2+1);
			if(i%2 != 0 && i!= nCount-1 )
			{
				strHaoMa+=_T(",");
			}
		}
		int count = nCount/2;
		zhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6)*(count-7)*(count-8))/362880;

	}
	else if(m_gameKind == enBJK8_RX_10)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 20)
		{
			MyMessageBox(L"格式不正确，请重新选择！");
			m_DlgStatus.HideStatusWindow();
			return;
		}
		if (nCount>26)
		{
			MyMessageBox(L"您选择的数字太多,最多选13注！");
			m_DlgStatus.HideStatusWindow();
			return;
		}

		for(int i = 0;i < nCount;i++)
		{
			strHaoMa += strNum.GetAt(i);
			//strHaoMa += strNum.GetAt(i*2+1);
			if(i%2 != 0 && i!= nCount-1 )
			{
				strHaoMa+=_T(",");
			}
		}
		int count = nCount/2;
		zhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6)*(count-7)*(count-8)*(count-9))/3628800;

	}
	else if(m_gameKind == enBJK8_ShangXia)
	{
		CString strShang = GetShangZhongXiaString();
		if(strShang.GetLength()<=0)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(L"请选择号码！");
			return;
		}

		zhushu = strShang.GetLength();

		for(int i = 0;i < zhushu;i++)
		{
			strHaoMa += strShang.GetAt(i);
			if(i != (zhushu-1))
			{
				strHaoMa += _T(",");
			}
		}
		//strHaoMa = strShang;
	}
	else if(m_gameKind == enBJK8_JiOu)
	{
		CString strJiHeOu = GetJiHeOuString();
		if(strJiHeOu.GetLength()<=0)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(L"请选择号码！");
			return;
		}

		zhushu = strJiHeOu.GetLength();

		for(int i = 0;i < zhushu;i++)
		{
			strHaoMa += strJiHeOu.GetAt(i);
			if(i != (zhushu-1))
			{
				strHaoMa += _T(",");
			}
		}
		//strHaoMa = strShang;
	}
	else if(m_gameKind == enBJK8_HeZhiDXDS)
	{
		CString strDaXiaoDanShuang = GetDaXiaoDanShuang1String();
		if(strDaXiaoDanShuang.GetLength()<=0)
		{
			m_DlgStatus.HideStatusWindow();
			MyMessageBox(L"请选择号码！");
			return;
		}

		zhushu = strDaXiaoDanShuang.GetLength();

		for(int i = 0;i < zhushu;i++)
		{
			strHaoMa += strDaXiaoDanShuang.GetAt(i);
			if(i != (zhushu-1))
			{
				strHaoMa += _T(",");
			}
		}
		//strHaoMa = strShang;
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


		}
		else
		{

			m_strHaoma.Empty();
			int nCount = m_listChqSSC.GetItemCount();

			wstring strdata;
			strdata = strHaoMa.GetBuffer(0);
			//vecAllNums.insert(vecAllNums.begin(),1,strdata);
			vecAllNums.insert(vecAllNums.begin(),1,strdata);

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
			else if(m_gameKind == enBJK8_ShangXia)
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
					CString strDesc = GetSZXDesc(nNum);
					strAddHaoma += strDesc;

				} while (nAddIndex<1000);

				m_listChqSSC.InsertItem(nCount, strAddHaoma);

			}
			else if(m_gameKind == enBJK8_JiOu)
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
					CString strDesc = GetJHODesc(nNum);
					strAddHaoma += strDesc;

				} while (nAddIndex<1000);

				m_listChqSSC.InsertItem(nCount, strAddHaoma);

			}
			else if(m_gameKind == enBJK8_HeZhiDXDS)
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
					CString strDesc = GetHZDXDSDesc(nNum);
					strAddHaoma += strDesc;

				} while (nAddIndex<1000);

				m_listChqSSC.InsertItem(nCount, strAddHaoma);

			}
			else
			{
				m_listChqSSC.InsertItem(nCount, strHaoMa);
			}

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
			fJine = m_nZongZhuShu*m_beishu*danzhujine;
			CString strZongzhu ;
			strZongzhu.Format(L"￥%.3lf",fJine);
			m_listChqSSC.SetItemText(nCount, 2, strZongzhu);


			m_listChqSSC.SetItemText(nCount, 3, GetWanDesc());

			m_nZongZhuShu += (arrLen*zhushu);
			CString strTmp;
			strTmp.Format(_T("%d"), m_nZongZhuShu);
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


			m_nZongZhuShu += (arrLen*zhushu);
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
	return;
}
CString CBeiJingKuai8::GetSZXDesc(int num)
{
	CString str;

	switch(num)
	{
	case 0:
		{
			str = _T("上");
		}
		break;
	case 1:
		{
			str = _T("中");
		}
		break;
	case 2:
		{
			str = _T("下");
		}
		break;
	}
	return str;
}
CString CBeiJingKuai8::GetJHODesc(int num)
{
	CString str;

	switch(num)
	{
	case 0:
		{
			str = _T("奇");
		}
		break;
	case 1:
		{
			str = _T("和");
		}
		break;
	case 2:
		{
			str = _T("偶");
		}
		break;
	}
	return str;
}
CString CBeiJingKuai8::GetHZDXDSDesc(int num)
{
	CString str;

	switch(num)
	{
	case 0:
		{
			str = _T("大单");
		}
		break;
	case 1:
		{
			str = _T("大双");
		}
		break;
	case 2:
		{
			str = _T("小单");
		}
		break;
	case 3:
		{
			str = _T("小双");
		}
		break;
	}
	return str;
}
//追号
void CBeiJingKuai8::OnBnClickedBtnZhuihao()
{

	if(!CheckInput())
		return ;
	CDlgAddtional	dlgZhuiHao;
	CString strBeishu ;
	strBeishu.Format(L"%d",m_beishu);

	//if(m_bKaiJiangzhong)
	dlgZhuiHao.SetCurrentQihaoTime(m_chqRule.GetNextExpect(-1)/* m_kjXinxi[0].qihao*/, strBeishu,0,6, m_zongJine,1000000000,CZKUAILE8);
	// 	else
	// 		dlgZhuiHao.SetCurrentQihaoTime( m_kjXinxi[0].qihao, strBeishu,8,3, m_zongZhuShu * danzhujine,120);
	if(dlgZhuiHao.DoModal() == IDOK)
	{
		//判断是否临近开奖点
		//重新获取服务器时间
		//theApp.GetTimeDiff();
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
	//m_nZongZhuShu = 0;

	m_beishu = 1;
	//UpdateData(FALSE);
OnBnClickedBtnClsList();
// 	CRect rcInvali;
// 	rcInvali.CopyRect(rcZongZhuShu);
// 	rcInvali.right+=400;
// 	InvalidateRect(&rcInvali);
// 	InvalidateRect(&rcZongJinE);
}
//投注
void CBeiJingKuai8::OnBnClickedBtnTouzhu()
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
	qrDlg.m_gamekind = _T("北京快8");
	qrDlg.m_zhushu = m_nZongZhuShu * m_beishu;
	qrDlg.m_moshi = theAccount.Scoretype;
	qrDlg.m_zongjine = m_zongJine;
	if (qrDlg.DoModal() != IDOK)
	{
		return;
	}

	//判断是否临近开奖点
	//重新获取服务器时间
	//theApp.GetTimeDiff();
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

	//m_nZongZhuShu = 0;

	m_beishu = 1;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

}
//删除
void CBeiJingKuai8::OnBnClickedBtnDelSel()
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
void CBeiJingKuai8::OnEnChangeEditDanshiInput()
{
	m_richDanshiHaoma.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
}

//倍数 修改
void CBeiJingKuai8::OnEnChangeEditBeishu()
{
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
WORD CBeiJingKuai8::GetGameHoverIndex(CPoint MousePoint)
{
	if(!m_GameListRect.PtInRect(MousePoint))
	{
		return INVALID_WORD;
	}
	WORD wHoverItem = INVALID_WORD;

	return wHoverItem;
}

//鼠标消息
VOID CBeiJingKuai8::OnMouseMove(UINT nFlags, CPoint Point)
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
VOID CBeiJingKuai8::OnLButtonDown(UINT nFlags, CPoint Point)
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
VOID CBeiJingKuai8::OnLButtonUp(UINT nFlags, CPoint Point)
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
LRESULT CBeiJingKuai8::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置状态
	m_bHovering=false;
	return 0;
}


//清空
void CBeiJingKuai8::OnBnClickedBtnClsList()
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
	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);
}

//设置返点
void CBeiJingKuai8::OnBnClickedBtnLock()
{
	int index = (CZKUAILE8 << 16) | (unsigned short)m_gameKind;


	CDlgFandian	dlg;
	dlg.SetBonus(mapBonus[m_gameKind], theBonus[index], mapPercent[m_gameKind], theAccount.fandian*100.0, mapDanweiFandian[m_gameKind]);
	if(IDOK == dlg.DoModal())
	{
		dlg.GetBonus(m_fBonus,  m_fPercent);

		SendToServer(5);
	}
}
//倍投加
void CBeiJingKuai8::OnBnClickedBeiTouJia()
{
	CString strBeishu;

	m_beishu += 1;

	strBeishu.Format(L"%d",m_beishu);

	m_editBeiTou.SetWindowText(strBeishu);
	return;

}
//倍投减
void CBeiJingKuai8::OnBnClickedBeiTouJian()
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






void CBeiJingKuai8::OnBnClinckedBtnDaXiaoDanShuang()		//大小单双
{
	CheckWanFaBtn(m_btnDaXiaoDanShuang);
	m_gameKind = enBJK8_RX_3;
	SendToServer(3);

	ResetAllNums();
	HideAllRio();
	HideJiZhi();	
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();
	HideShangZhongXia();
	HideJiHeOu();
	HideDaXiaoDanShuang1();
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);

	

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);

}


//北京快8
void CBeiJingKuai8::OnBnClinckedBtnRenXuan()	//任选
{

	CheckWanFaBtn(m_btnRenXuan);

	//设置当前模式
	m_gameKind = enBJK8_RX_1;

	SendToServer(3);

	ResetAllNums();
	HideAllRio();
	HideShangZhongXia();
	HideJiHeOu();
	HideDaXiaoDanShuang1();
	HideJiZhi();
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();

	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);

	

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);

}

//特码
void CBeiJingKuai8::OnBnClinckedBtnTeMa()	//特码
{
	CheckWanFaBtn(m_btnTeMa);


	m_gameKind = enBJK8_RX_2;
	SendToServer(3);

	ResetAllNums();

	HideJiZhi();
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();
	HideAllRio();
	HideShangZhongXia();
	HideJiHeOu();
	HideDaXiaoDanShuang1();
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);

	

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);
}
//趣味玩法
void CBeiJingKuai8::OnBnClinckedBtnFunny()
{
	CheckWanFaBtn(m_btnFunny);
	m_gameKind = enBJK8_RX_5;
	SendToServer(3);

	ResetAllNums();

	HideJiZhi();
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();
	HideShangZhongXia();
	HideJiHeOu();
	HideDaXiaoDanShuang1();
	HideAllRio();
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);

	

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);

}

//任选6
void CBeiJingKuai8::OnBnClinckedBtnRenXuan6()
{
	CheckWanFaBtn(m_btnRenXuan6);
	m_gameKind = enBJK8_RX_6;
	SendToServer(3);

	ResetAllNums();

	HideJiZhi();
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();

	HideAllRio();
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideShangZhongXia();
	HideJiHeOu();
	HideDaXiaoDanShuang1();
	

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);

}

//任选7
void CBeiJingKuai8::OnBnClinckedBtnRenXuan7()
{
	CheckWanFaBtn(m_btnRenXuan7);
	m_gameKind = enBJK8_RX_7;
	SendToServer(3);


	ResetAllNums();

	HideJiZhi();
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();
	HideJiHeOu();


	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);

	
	HideAllRio();
	HideShangZhongXia();
	HideDaXiaoDanShuang1();
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);

}
//任选8
void CBeiJingKuai8::OnBnClinckedBtnRenXuan8()
{
	CheckWanFaBtn(m_btnRenXuan8);
	m_gameKind = enBJK8_RX_8;

	SendToServer(3);

	ResetAllNums();

	HideJiZhi();
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();
	HideDaXiaoDanShuang1();
	HideJiHeOu();

	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideAllRio();
	HideShangZhongXia();
	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);

}
//任选9
void CBeiJingKuai8::OnBnClinckedBtnRenXuan9()
{
	CheckWanFaBtn(m_btnRenXuan9);
	m_gameKind = enBJK8_RX_9;

	SendToServer(3);

	ResetAllNums();

	HideJiZhi();
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();
	HideShangZhongXia();
	HideAllRio();
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);
	HideJiHeOu();
	HideDaXiaoDanShuang1();
	

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);

}

//任选10
void CBeiJingKuai8::OnBnClinckedBtnRenXuan10()
{
	CheckWanFaBtn(m_btnRenXuan10);
	m_gameKind = enBJK8_RX_10;
	SendToServer(3);


	ResetAllNums();
	HideAllRio();
	ShowShangZhongXia();
	HideJiZhi();
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();
	HideShangZhongXia();
	HideJiHeOu();
	HideDaXiaoDanShuang1();
	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);

	

	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);

}
//趣味
void CBeiJingKuai8::OnBnClinckedBtnQuWei()
{
	CheckWanFaBtn(m_btnQuWei);

	ResetAllNums();


	ShowCtrl(IDC_RIO_SZX);
	ShowCtrl(IDC_RIO_JHO);
	ShowCtrl(IDC_RIO_HZDXDS);

	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);

	OnBnClickedBtnSZX();

	Invalidate(false);

}
//上中下
void CBeiJingKuai8::OnBnClickedBtnSZX()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = enBJK8_ShangXia;

	SendToServer(3);

	m_rioSZX.SetPushed(true);
	m_rioJHO.SetPushed(false);
	m_rioHZDXDS.SetPushed(false);
	HideJiHeOu();
	HideDaXiaoDanShuang1();
	HideJiZhi();
	HideRio();
	HideBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();
	ShowShangZhongXia();
}
//奇偶和
void CBeiJingKuai8::OnBnClickedBtnJHO()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = enBJK8_JiOu;

	SendToServer(3);
	ShowJiHeOu();
	m_rioSZX.SetPushed(false);
	m_rioJHO.SetPushed(true);
	m_rioHZDXDS.SetPushed(false);	
	HideShangZhongXia();
	HideJiZhi();
	HideRio();
	HideBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();
	HideDaXiaoDanShuang1();
}
//和值大小单双
void CBeiJingKuai8::OnBnClickedBtnHZDXDS()
{
	ResetAllNums();
	SetLockText();

	m_gameKind = enBJK8_HeZhiDXDS;
	HideShangZhongXia();
	HideJiHeOu();

	SendToServer(3);

	m_rioSZX.SetPushed(false);
	m_rioJHO.SetPushed(false);
	m_rioHZDXDS.SetPushed(true);

	HideJiZhi();
	HideRio();
	HideBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();
	ShowDaXiaoDanShuang1();
}
//对子
void CBeiJingKuai8::OnRioDuiZi()
{
	SelectRio(m_rioDuiZi);
	m_StrFunny = "14";
}

//顺子
void CBeiJingKuai8::OnRioShunZi()
{
	SelectRio(m_rioShunZi);
	m_StrFunny = "15";
}

//豹子
void CBeiJingKuai8::OnRioBaoZi()
{
	SelectRio(m_rioBaoZi);
	m_StrFunny = "16";
}

//任选4
void CBeiJingKuai8::OnBnClinckedBtnJiZhi()
{
	CheckWanFaBtn(m_btnJiZhi);


	m_gameKind = enBJK8_RX_4;
	SendToServer(3);

	ResetAllNums();
	HideAllRio();
	HideShangZhongXia();
	HideDaXiaoDanShuang1();
	HideJiHeOu();
	HideJiZhi();
	HideRio();
	ShowBJNums();//显示1-80数字
	HideHeZhiNums();
	HideDanShiEdit();//隐藏键盘输入(进入鼠标模式+
	HideDaXiaoDanShuang();


	HideCtrl(IDC_RIO_KEYBOARD_INPUT);
	HideCtrl(IDC_RIO_MOUSE_INPUT);


	m_rioMouseInput.SetPushed(true);m_RenxuanKeyboardInput = false;
	m_rioKeyboardInput.SetPushed(false);
	Invalidate(false);
}



BeiJingKuai8Kind CBeiJingKuai8::GetGameKindByDesc(const CString& desc)
{
	if (desc == _T("任选10")) 
	{
		return enBJK8_RX_10;
	} 
	else if (desc == _T("任选9")) 
	{
		return enBJK8_RX_9;
	} 
	else if (desc == _T("任选8")) 
	{
		return enBJK8_RX_8;
	} 
	else if (desc == _T("任选7")) 
	{
		return enBJK8_RX_7;
	}
	else if (desc == _T("任选6")) 
	{
		return enBJK8_RX_6;
	}
	else if (desc == _T("任选5")) 
	{
		return enBJK8_RX_5;
	}
	else if (desc == _T("任选4")) 
	{
		return enBJK8_RX_4;
	}
	else if (desc == _T("任选3")) 
	{
		return enBJK8_RX_3;
	}
	else if (desc == _T("任选2")) 
	{
		return enBJK8_RX_2;
	}
	else if (desc == _T("任选1")) 
	{
		return enBJK8_RX_1;
	}
	else if (desc == _T("上中下")) 
	{
		return enBJK8_ShangXia;
	}
	else if (desc == _T("奇和偶"))
	{
		return enBJK8_JiOu;
	}
	else if (desc == _T("和值大小单双")) 
	{
		return enBJK8_HeZhiDXDS;
	}

	return enBJK8_RX_1;
}

//初始化玩法信息
void CBeiJingKuai8::InitWanFaMoShi()
{
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_RX_10] = _T("任选10");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_RX_9] = _T("任选9");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_RX_8] = _T("任选8");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_RX_7] = _T("任选7");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_RX_6] = _T("任选6");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_RX_5] = _T("任选5");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_RX_4] = _T("任选4");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_RX_3] = _T("任选3");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_RX_2] = _T("任选2");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_RX_1] = _T("任选1");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_ShangXia] = _T("上中下");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_JiOu] = _T("奇和偶");
	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enBJK8_HeZhiDXDS] = _T("和值大小单双");


// 	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enXY28_TeMa] = _T("特码");
// 	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enXY28_DXDS] = _T("大小单双");
// 	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enXY28_Funny] = _T("小单");
// 	theGameKind[(CZKUAILE8 << 16) | (unsigned short)enXY28_JiZhi] = _T("极值");
}



VOID CBeiJingKuai8::SendToServer(int nSendType)
{
	if(nSendType == 1)
	{
		m_bTouzhu = true;
		if(m_bTouzhu)
		{
			m_bTouzhu = false;


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
				CString strWanfa = m_listChqSSC.GetItemText(i, 3);
				CString strBeishu = m_listChqSSC.GetItemText(i, 5);
				int nKind = GetGameKindByDesc(strWanfa);
				int nBeishu = _ttoi(strBeishu);
				CString strMoshi = m_listChqSSC.GetItemText(i, 1);

				int nMoshi = GetMoshiRet(strMoshi);
				if(nNowKind == nKind && nNowBeishu == nBeishu && nNowMoshi == nMoshi && nNowMoshi!= 10000 && nNowBeishu != 100000 && nNowKind != 10000)
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
						if(nKind == enBJK8_ShangXia||nKind == enBJK8_JiOu||nKind == enBJK8_HeZhiDXDS)
						{
							haoma =  vecAllNums[itm_cnt-j-1].c_str();
						}
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
				TouzhuCQSSC.nGameType = CZKUAILE8;
				TouzhuCQSSC.nMoshi = nNowMoshi;

				TouzhuCQSSC.nSign = m_nTzhSign;

				CString strQiHao;
				strQiHao = m_chqRule.GetNextExpect();


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
				if(m_nZongZhuShu<=0)
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

		m_btWxzxTip.ShowWindow(SW_SHOW);
		if(m_gameKind == enBJK8_RX_1)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从01-80之间选择一个或一个以上的数字，最多选13个数字，开奖号码中包含所选数字，即为中奖"));
		}
		else if(m_gameKind == enBJK8_RX_2)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从01-80之间选择两个或两个以上的数字，最多选13个数字，开奖号码中包含所选数字，即为中奖"));
		}
		else if(m_gameKind == enBJK8_RX_3)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从01-80之间选择三个或三个以上的数字，最多选13个数字，开奖号码中包含所选数字，即为中奖"));
		}
		else if(m_gameKind == enBJK8_RX_4)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从01-80之间选择四个或四个以上的数字，最多选13个数字，开奖号码中包含所选数字，即为中奖"));
		}
		else if(m_gameKind == enBJK8_RX_5)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从01-80之间选择五个或五个以上的数字，最多选13个数字，开奖号码中包含所选数字，即为中奖"));
		}
		else if(m_gameKind == enBJK8_RX_6)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从01-80之间选择六个或六个以上的数字，最多选13个数字，开奖号码中包含所选数字，即为中奖"));
		}
		else if(m_gameKind == enBJK8_RX_7)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从01-80之间选择七个或七个以上的数字，最多选13个数字，开奖号码中包含所选数字，即为中奖"));
		}
		else if(m_gameKind == enBJK8_RX_8)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从01-80之间选择八个或八个以上的数字，最多选13个数字，开奖号码中包含所选数字，即为中奖"));
		}
		else if(m_gameKind == enBJK8_RX_9)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从01-80之间选择九个或九个以上的数字，最多选13个数字，开奖号码中包含所选数字，即为中奖"));
		}
		else if(m_gameKind == enBJK8_RX_10)
		{
			m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从01-80之间选择十个或十个以上的数字，最多选13个数字，开奖号码中包含所选数字，即为中奖"));
		}
		else if(m_gameKind == enBJK8_ShangXia || m_gameKind == enBJK8_JiOu || enBJK8_HeZhiDXDS)
		{
			m_btWxzxTip.ShowWindow(SW_HIDE);
		}

	//	OnBnClickedBtnClsList();

		m_bGetUserFandian = true;
		//m_nZongZhuShu = 0;
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

					CMD_GP_TouZhuCQSSC_Zhuihao TouzhuCQSSC;
					ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
					TouzhuCQSSC.dwUserID = theAccount.user_id;
					TouzhuCQSSC.nGameKind = m_gameKind;
					TouzhuCQSSC.nGameType = CZKUAILE8;
					TouzhuCQSSC.nMoshi = theAccount.Scoretype;

					TouzhuCQSSC.nSign = m_nTzhSign;
					if(m_bZhuiZjtz)
						TouzhuCQSSC.bZhuihao = GetTickCount();
					else
						TouzhuCQSSC.bZhuihao = 2;

					int nItemCount = m_listChqSSC.GetItemCount();

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

						int nZhushu = 0;
						CString strZhu = m_listChqSSC.GetItemText(n, 4);
						nZhushu += _ttoi(strZhu);
						if(nZhushu<=0)
						{
							m_DlgStatus.HideStatusWindow();
							OnBnClickedBtnClsList();
							return;
						}

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
			SetUserBonus.n_t_type_id = CZKUAILE8;
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
			GetUserFandian.n_t_type_id = CZKUAILE8;
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



bool CBeiJingKuai8::ChangeStringToA(CString strInput, string&  strOutput)
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
bool CBeiJingKuai8::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
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
					m_bKaiJiangzhong=false;									//开奖结束
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
						//m_listChqSSC.DeleteAllItems();
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
					if(m_bShowFail)
					{
						MyMessageBox(_T("有订单未成功提交！"));
						m_bShowFail=false;
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


void CBeiJingKuai8::SetLockText(float fBonus, float fBonusPercent)
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
 void CBeiJingKuai8::DrawLastKjHaoma(CDC* pDC, Graphics& graphics)
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
 	pDC->DrawText(_T("北京快8"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
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


	pDC->SelectObject(&m_KjInfoFont);
	strYg.Format(_T("%s"), m_chqRule.GetKjShjDiffDesc(20));
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
 
 	if (m_gameKind<=enBJK8_10)
 	{
 		int j = 0;
 		int index = 0;
 		CString strTemp;
 		for (int i=0; i<__min(str.GetLength(),40); i++) 
 		{
 			if (iswdigit(str.GetAt(i))) 
 			{
 				strTemp+=str.GetAt(i);
 				int num = _ttoi(CString(str.GetAt(i)));
 
 				if(strTemp.GetLength()>=2)
 				{
 					num = _ttoi(strTemp);
 
 					if(m_bKaiJiangzhong )
 					{
 						KillTimer(timer_id_kaijiangzhong);
 						SetTimer(timer_id_kaijiangzhong, 100, NULL);
 					}
 					else
 					{
 						DrawBigNum(graphics, 
 							m_kjNumBig2, 
 							Rect(kj_big_haoma_x + ((index>=10)?(index-10):index)* m_bigNumWidth,
 							kj_big_haoma_y+((index>=10)?m_bigNumHeight:0),
 							m_bigNumWidth,
 							m_bigNumHeight),
 							num-1,m_bigNumWidth,m_bigNumHeight);
 					}
 				}
 				j++;
 				if(strTemp.GetLength()>=2)
 				{
 					//			num = _ttoi(strTemp);
 					index ++;
 					strTemp.Empty();
 				}
 
 			}
 		}
 	}
 }
 
//开奖号码
void CBeiJingKuai8::DrawTopKjHaoma(CDC* pDC, Graphics& graphics)
{
//	return;
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_smallNumfont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(250 ,250 ,250));

	int nCol = 0;
	
	for (int cnt=0; cnt<2; cnt++) 
	{
		CString str = m_kjXinxi[cnt].qihao;
		if(str.IsEmpty())
		{
			continue;
		}

		CRect rcText(kj_small_haoma_x - 80, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 80, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span) + m_smallNumHeight);
		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		str = m_kjXinxi[cnt].haoma;
		CString temp;
		temp = str.Left(2);

		if(_ttoi(temp) > 80)
		{
			//	index ++;
			continue;
		}

		int index = 0;
		int j = 0;
		CString strTemp;
		for (int i=0; i<str.GetLength(); i++) 
		{
			if (iswdigit(str.GetAt(i))) 
			{
				strTemp+=str.GetAt(i);
				int num = _ttoi(CString(str.GetAt(i)));
				if(strTemp.GetLength()>=2)
				{
					num = _ttoi(strTemp);
					//	strTemp.Empty();
				}
				if (num >0 && num < 81&& strTemp.GetLength()>=2) 
				{

// 					Rect(kj_big_haoma_x + ((index>=10)?(index-10):index)* m_bigNumWidth,
// 						kj_big_haoma_y+((index>=10)?m_bigNumHeight:0),
// 						m_bigNumWidth,
// 						m_bigNumHeight)

					Rect rc(kj_small_haoma_x + ((index>=10)?(index-10):index)* (m_smallNumWidth+kj_small_haoma_col_span)-28,
						kj_small_haoma_y + ((index>=10)?m_smallNumHeight:0) -10 +cnt*(m_smallNumHeight+kj_small_haoma_row_span)*2,      
						m_smallNumWidth, m_smallNumHeight);
					DrawSmallNum(graphics, m_kjNumSmall, rc, num-1);
				}

				j++;
				if(strTemp.GetLength()>=2)
				{
					//			num = _ttoi(strTemp);
					index ++;
					strTemp.Empty();
				}
			}
		}
	}

	pDC->DrawText(strText, &rcZongJinE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}


void CBeiJingKuai8::DrawZongZhushuJinE(CDC* pDC)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_zongFont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(139, 137, 136));
	strText.Format(_T("你选择了"));

	CRect rcZhu;
	rcZhu.CopyRect(rcZongZhuShu);
	rcZhu.left += 225;
	rcZhu.right += 475;
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(160, 8, 8));
	strText.Format(_T("%d"), m_singlezhushu);
	rcZhu.left += 56;
	rcZhu.right += 56;
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(139, 137, 136));
	rcZhu.left += strText.GetLength()*9;
	rcZhu.right += strText.GetLength()*9;
	strText.Format(_T("注"));
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

	strText.Format(_T("共:"));
	pDC->DrawText(strText, &rcZongJinE, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	rcRect.CopyRect(rcZongJinE);
	rcRect.left += 17;
	rcRect.right += 17;
	pDC->SetTextColor(RGB(160, 8, 8));

	strText.Format(_T("%.3lf"), m_singlezhushu * m_beishu * danzhujine);
	pDC->DrawText(strText, &rcRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(139, 137, 136));

	rcRect.left += strText.GetLength()*8;
	rcRect.right += strText.GetLength()*8;
	strText.Format(_T("元"));
	pDC->DrawText(strText, &rcRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strText.Format(_T("注数:"),m_nZongZhuShu);
	pDC->DrawText(strText, &rcAllZongzhushu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	rcRect.CopyRect(rcAllZongzhushu);
	rcRect.left += 32;
	rcRect.right += 32;
	pDC->SetTextColor(RGB(160, 8, 8));

	strText.Format(_T("%d"),m_nZongZhuShu);
	pDC->DrawText(strText, &rcRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(139, 137, 136));

	rcZhu.CopyRect(rcAllZongzhushu);
	rcZhu.top += 20;
	rcZhu.bottom += 20;
	strText.Format(_T("合计:"));
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(160, 8, 8));
	rcZhu.left += 32;
	rcZhu.right += 32;
	strText.Format(_T("%.3lf"),m_zongJine);
	pDC->DrawText(strText, &rcZhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);

}

long CBeiJingKuai8::GetKjShjDiff()
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

void CBeiJingKuai8::DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num,int nWidth,int nHeight)
{
	graphics.DrawImage(img, rc_dest, num*nWidth, 0, nWidth, nHeight, UnitPixel);
}

void CBeiJingKuai8::DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_smallNumWidth, 0, m_smallNumWidth, m_smallNumHeight, UnitPixel);
}

//鼠标输入
void CBeiJingKuai8::OnBnClickedRioMouseInput()
{
// 	if(m_gameKind == enBJK8_RX_1)
// 	{
// 		OnBnClinckedBtnRenXuan();
// 	}
// 	else if(m_gameKind == enXY28_TeMa)
// 	{
// 		OnBnClinckedBtnTeMa();
// 	}


	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);
	ResetAllNums();
	m_rioMouseInput.SetPushed(true);
	m_rioKeyboardInput.SetPushed(false);

	HideDanShiEdit();
}

//键盘输入（任选2，任选3，任选4）
void CBeiJingKuai8::OnBnClickedRioKeyboardInput()
{
	ShowCtrl(IDC_RIO_MOUSE_INPUT);
	ShowCtrl(IDC_RIO_KEYBOARD_INPUT);


	HideHeZhiNums();
	HideBJNums();

	ResetAllNums();
	m_rioMouseInput.SetPushed(false);
	m_rioKeyboardInput.SetPushed(true);


	ShowDanShiEdit();
}
void CBeiJingKuai8::ShowCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_SHOW);
	}
}
void CBeiJingKuai8::HideCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}
void CBeiJingKuai8::CheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(true);
}
void CBeiJingKuai8::UnCheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(false);
}
bool CBeiJingKuai8::IsNumBtnCheck(CNumerButton& btn)
{
	return btn.IsSelect();
}

void CBeiJingKuai8::ResetAllNums(void)
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

	UnCheckNumBtn(m_btnShang);
	UnCheckNumBtn(m_btnZhong);
	UnCheckNumBtn(m_btnXia);
	UnCheckNumBtn(m_btnJi);
	UnCheckNumBtn(m_btnHe);
	UnCheckNumBtn(m_btnOu);
	UnCheckNumBtn(m_btnDaDan1);
	UnCheckNumBtn(m_btnDaShuang1);
	UnCheckNumBtn(m_btnXiaoDan1);
	UnCheckNumBtn(m_btnXiaoShuang1);
	//北京快乐8
	UnCheckNumBtn(m_btnHuanLe_01);
	UnCheckNumBtn(m_btnHuanLe_02);
	UnCheckNumBtn(m_btnHuanLe_03);
	UnCheckNumBtn(m_btnHuanLe_04);
	UnCheckNumBtn(m_btnHuanLe_05);
	UnCheckNumBtn(m_btnHuanLe_06);
	UnCheckNumBtn(m_btnHuanLe_07);
	UnCheckNumBtn(m_btnHuanLe_08);
	UnCheckNumBtn(m_btnHuanLe_09);
	UnCheckNumBtn(m_btnHuanLe_10);
	UnCheckNumBtn(m_btnHuanLe_11);
	UnCheckNumBtn(m_btnHuanLe_12);
	UnCheckNumBtn(m_btnHuanLe_13);
	UnCheckNumBtn(m_btnHuanLe_14);
	UnCheckNumBtn(m_btnHuanLe_15);
	UnCheckNumBtn(m_btnHuanLe_16);
	UnCheckNumBtn(m_btnHuanLe_17);
	UnCheckNumBtn(m_btnHuanLe_18);
	UnCheckNumBtn(m_btnHuanLe_19);
	UnCheckNumBtn(m_btnHuanLe_20);
	UnCheckNumBtn(m_btnHuanLe_21);
	UnCheckNumBtn(m_btnHuanLe_22);
	UnCheckNumBtn(m_btnHuanLe_23);
	UnCheckNumBtn(m_btnHuanLe_24);
	UnCheckNumBtn(m_btnHuanLe_25);
	UnCheckNumBtn(m_btnHuanLe_26);
	UnCheckNumBtn(m_btnHuanLe_27);
	UnCheckNumBtn(m_btnHuanLe_28);
	UnCheckNumBtn(m_btnHuanLe_29);
	UnCheckNumBtn(m_btnHuanLe_30);
	UnCheckNumBtn(m_btnHuanLe_31);
	UnCheckNumBtn(m_btnHuanLe_32);
	UnCheckNumBtn(m_btnHuanLe_33);
	UnCheckNumBtn(m_btnHuanLe_34);
	UnCheckNumBtn(m_btnHuanLe_35);
	UnCheckNumBtn(m_btnHuanLe_36);
	UnCheckNumBtn(m_btnHuanLe_37);
	UnCheckNumBtn(m_btnHuanLe_38);
	UnCheckNumBtn(m_btnHuanLe_39);
	UnCheckNumBtn(m_btnHuanLe_40);
	UnCheckNumBtn(m_btnHuanLe_41);
	UnCheckNumBtn(m_btnHuanLe_42);
	UnCheckNumBtn(m_btnHuanLe_43);
	UnCheckNumBtn(m_btnHuanLe_44);
	UnCheckNumBtn(m_btnHuanLe_45);
	UnCheckNumBtn(m_btnHuanLe_46);
	UnCheckNumBtn(m_btnHuanLe_47);
	UnCheckNumBtn(m_btnHuanLe_48);
	UnCheckNumBtn(m_btnHuanLe_49);
	UnCheckNumBtn(m_btnHuanLe_50);
	UnCheckNumBtn(m_btnHuanLe_51);
	UnCheckNumBtn(m_btnHuanLe_52);
	UnCheckNumBtn(m_btnHuanLe_53);
	UnCheckNumBtn(m_btnHuanLe_54);
	UnCheckNumBtn(m_btnHuanLe_55);
	UnCheckNumBtn(m_btnHuanLe_56);
	UnCheckNumBtn(m_btnHuanLe_57);
	UnCheckNumBtn(m_btnHuanLe_58);
	UnCheckNumBtn(m_btnHuanLe_59);
	UnCheckNumBtn(m_btnHuanLe_60);
	UnCheckNumBtn(m_btnHuanLe_61);
	UnCheckNumBtn(m_btnHuanLe_62);
	UnCheckNumBtn(m_btnHuanLe_63);
	UnCheckNumBtn(m_btnHuanLe_64);
	UnCheckNumBtn(m_btnHuanLe_65);
	UnCheckNumBtn(m_btnHuanLe_66);
	UnCheckNumBtn(m_btnHuanLe_67);
	UnCheckNumBtn(m_btnHuanLe_68);
	UnCheckNumBtn(m_btnHuanLe_69);
	UnCheckNumBtn(m_btnHuanLe_70);
	UnCheckNumBtn(m_btnHuanLe_71);
	UnCheckNumBtn(m_btnHuanLe_72);
	UnCheckNumBtn(m_btnHuanLe_73);
	UnCheckNumBtn(m_btnHuanLe_74);
	UnCheckNumBtn(m_btnHuanLe_75);
	UnCheckNumBtn(m_btnHuanLe_76);
	UnCheckNumBtn(m_btnHuanLe_77);
	UnCheckNumBtn(m_btnHuanLe_78);
	UnCheckNumBtn(m_btnHuanLe_79);
	UnCheckNumBtn(m_btnHuanLe_80);


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
void CBeiJingKuai8::HideRio()
{
	m_rioDuiZi.ShowWindow(SW_HIDE);
	m_rioShunZi.ShowWindow(SW_HIDE);
	m_rioBaoZi.ShowWindow(SW_HIDE);
}
//选择玩法
void CBeiJingKuai8::SelectRio(CImgRioButton &rio)
{
	m_rioDuiZi.SetPushed(false);
	m_rioShunZi.SetPushed(false);
	m_rioBaoZi.SetPushed(false);

	rio.SetPushed(true);
}

void CBeiJingKuai8::HideAllRio()
{
	HideCtrl(IDC_RIO_SZX);
	HideCtrl(IDC_RIO_JHO);
	HideCtrl(IDC_RIO_HZDXDS);
}
//显示隐藏极值
void CBeiJingKuai8::HideJiZhi()
{
	m_btnMax.ShowWindow(SW_HIDE);
	m_btnMin.ShowWindow(SW_HIDE);
}
void CBeiJingKuai8::ShowJiZji()
{
	m_btnMax.ShowWindow(SW_SHOW);
	m_btnMin.ShowWindow(SW_SHOW);
}



void CBeiJingKuai8::ShowDanShiEdit()
{
	m_RenxuanKeyboardInput = true;
	m_richDanshiHaoma.ShowWindow(SW_SHOW);
}
void CBeiJingKuai8::HideDanShiEdit()
{
	m_RenxuanKeyboardInput = false;
	m_richDanshiHaoma.ShowWindow(SW_HIDE);
}

void CBeiJingKuai8::ShowDaXiaoDanShuang()
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

void CBeiJingKuai8::HideDaXiaoDanShuang()
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
void CBeiJingKuai8::ShowBJNums()
{
	m_btnHuanLe_01.ShowWindow(SW_SHOW);
	m_btnHuanLe_02.ShowWindow(SW_SHOW);
	m_btnHuanLe_03.ShowWindow(SW_SHOW);
	m_btnHuanLe_04.ShowWindow(SW_SHOW);
	m_btnHuanLe_05.ShowWindow(SW_SHOW);
	m_btnHuanLe_06.ShowWindow(SW_SHOW);
	m_btnHuanLe_07.ShowWindow(SW_SHOW);
	m_btnHuanLe_08.ShowWindow(SW_SHOW);
	m_btnHuanLe_09.ShowWindow(SW_SHOW);
	m_btnHuanLe_10.ShowWindow(SW_SHOW);
	m_btnHuanLe_11.ShowWindow(SW_SHOW);
	m_btnHuanLe_12.ShowWindow(SW_SHOW);
	m_btnHuanLe_13.ShowWindow(SW_SHOW);
	m_btnHuanLe_14.ShowWindow(SW_SHOW);
	m_btnHuanLe_15.ShowWindow(SW_SHOW);
	m_btnHuanLe_16.ShowWindow(SW_SHOW);
	m_btnHuanLe_17.ShowWindow(SW_SHOW);
	m_btnHuanLe_18.ShowWindow(SW_SHOW);
	m_btnHuanLe_19.ShowWindow(SW_SHOW);
	m_btnHuanLe_20.ShowWindow(SW_SHOW);
	m_btnHuanLe_21.ShowWindow(SW_SHOW);
	m_btnHuanLe_22.ShowWindow(SW_SHOW);
	m_btnHuanLe_23.ShowWindow(SW_SHOW);
	m_btnHuanLe_24.ShowWindow(SW_SHOW);
	m_btnHuanLe_25.ShowWindow(SW_SHOW);
	m_btnHuanLe_26.ShowWindow(SW_SHOW);
	m_btnHuanLe_27.ShowWindow(SW_SHOW);
	m_btnHuanLe_28.ShowWindow(SW_SHOW);
	m_btnHuanLe_29.ShowWindow(SW_SHOW);
	m_btnHuanLe_30.ShowWindow(SW_SHOW);
	m_btnHuanLe_31.ShowWindow(SW_SHOW);
	m_btnHuanLe_32.ShowWindow(SW_SHOW);
	m_btnHuanLe_33.ShowWindow(SW_SHOW);
	m_btnHuanLe_34.ShowWindow(SW_SHOW);
	m_btnHuanLe_35.ShowWindow(SW_SHOW);
	m_btnHuanLe_36.ShowWindow(SW_SHOW);
	m_btnHuanLe_37.ShowWindow(SW_SHOW);
	m_btnHuanLe_38.ShowWindow(SW_SHOW);
	m_btnHuanLe_39.ShowWindow(SW_SHOW);
	m_btnHuanLe_40.ShowWindow(SW_SHOW);
	m_btnHuanLe_41.ShowWindow(SW_SHOW);
	m_btnHuanLe_42.ShowWindow(SW_SHOW);
	m_btnHuanLe_43.ShowWindow(SW_SHOW);
	m_btnHuanLe_44.ShowWindow(SW_SHOW);
	m_btnHuanLe_45.ShowWindow(SW_SHOW);
	m_btnHuanLe_46.ShowWindow(SW_SHOW);
	m_btnHuanLe_47.ShowWindow(SW_SHOW);
	m_btnHuanLe_48.ShowWindow(SW_SHOW);
	m_btnHuanLe_49.ShowWindow(SW_SHOW);
	m_btnHuanLe_50.ShowWindow(SW_SHOW);
	m_btnHuanLe_51.ShowWindow(SW_SHOW);
	m_btnHuanLe_52.ShowWindow(SW_SHOW);
	m_btnHuanLe_53.ShowWindow(SW_SHOW);
	m_btnHuanLe_54.ShowWindow(SW_SHOW);
	m_btnHuanLe_55.ShowWindow(SW_SHOW);
	m_btnHuanLe_56.ShowWindow(SW_SHOW);
	m_btnHuanLe_57.ShowWindow(SW_SHOW);
	m_btnHuanLe_58.ShowWindow(SW_SHOW);
	m_btnHuanLe_59.ShowWindow(SW_SHOW);
	m_btnHuanLe_60.ShowWindow(SW_SHOW);
	m_btnHuanLe_61.ShowWindow(SW_SHOW);
	m_btnHuanLe_62.ShowWindow(SW_SHOW);
	m_btnHuanLe_63.ShowWindow(SW_SHOW);
	m_btnHuanLe_64.ShowWindow(SW_SHOW);
	m_btnHuanLe_65.ShowWindow(SW_SHOW);
	m_btnHuanLe_66.ShowWindow(SW_SHOW);
	m_btnHuanLe_67.ShowWindow(SW_SHOW);
	m_btnHuanLe_68.ShowWindow(SW_SHOW);
	m_btnHuanLe_69.ShowWindow(SW_SHOW);
	m_btnHuanLe_70.ShowWindow(SW_SHOW);
	m_btnHuanLe_71.ShowWindow(SW_SHOW);
	m_btnHuanLe_72.ShowWindow(SW_SHOW);
	m_btnHuanLe_73.ShowWindow(SW_SHOW);
	m_btnHuanLe_74.ShowWindow(SW_SHOW);
	m_btnHuanLe_75.ShowWindow(SW_SHOW);
	m_btnHuanLe_76.ShowWindow(SW_SHOW);
	m_btnHuanLe_77.ShowWindow(SW_SHOW);
	m_btnHuanLe_78.ShowWindow(SW_SHOW);
	m_btnHuanLe_79.ShowWindow(SW_SHOW);
	m_btnHuanLe_80.ShowWindow(SW_SHOW);
	
}
void CBeiJingKuai8::HideBJNums()
{
	m_btnHuanLe_01.ShowWindow(SW_HIDE);
	m_btnHuanLe_02.ShowWindow(SW_HIDE);
	m_btnHuanLe_03.ShowWindow(SW_HIDE);
	m_btnHuanLe_04.ShowWindow(SW_HIDE);
	m_btnHuanLe_05.ShowWindow(SW_HIDE);
	m_btnHuanLe_06.ShowWindow(SW_HIDE);
	m_btnHuanLe_07.ShowWindow(SW_HIDE);
	m_btnHuanLe_08.ShowWindow(SW_HIDE);
	m_btnHuanLe_09.ShowWindow(SW_HIDE);
	m_btnHuanLe_10.ShowWindow(SW_HIDE);
	m_btnHuanLe_11.ShowWindow(SW_HIDE);
	m_btnHuanLe_12.ShowWindow(SW_HIDE);
	m_btnHuanLe_13.ShowWindow(SW_HIDE);
	m_btnHuanLe_14.ShowWindow(SW_HIDE);
	m_btnHuanLe_15.ShowWindow(SW_HIDE);
	m_btnHuanLe_16.ShowWindow(SW_HIDE);
	m_btnHuanLe_17.ShowWindow(SW_HIDE);
	m_btnHuanLe_18.ShowWindow(SW_HIDE);
	m_btnHuanLe_19.ShowWindow(SW_HIDE);
	m_btnHuanLe_20.ShowWindow(SW_HIDE);
	m_btnHuanLe_21.ShowWindow(SW_HIDE);
	m_btnHuanLe_22.ShowWindow(SW_HIDE);
	m_btnHuanLe_23.ShowWindow(SW_HIDE);
	m_btnHuanLe_24.ShowWindow(SW_HIDE);
	m_btnHuanLe_25.ShowWindow(SW_HIDE);
	m_btnHuanLe_26.ShowWindow(SW_HIDE);
	m_btnHuanLe_27.ShowWindow(SW_HIDE);
	m_btnHuanLe_28.ShowWindow(SW_HIDE);
	m_btnHuanLe_29.ShowWindow(SW_HIDE);
	m_btnHuanLe_30.ShowWindow(SW_HIDE);
	m_btnHuanLe_31.ShowWindow(SW_HIDE);
	m_btnHuanLe_32.ShowWindow(SW_HIDE);
	m_btnHuanLe_33.ShowWindow(SW_HIDE);
	m_btnHuanLe_34.ShowWindow(SW_HIDE);
	m_btnHuanLe_35.ShowWindow(SW_HIDE);
	m_btnHuanLe_36.ShowWindow(SW_HIDE);
	m_btnHuanLe_37.ShowWindow(SW_HIDE);
	m_btnHuanLe_38.ShowWindow(SW_HIDE);
	m_btnHuanLe_39.ShowWindow(SW_HIDE);
	m_btnHuanLe_40.ShowWindow(SW_HIDE);
	m_btnHuanLe_41.ShowWindow(SW_HIDE);
	m_btnHuanLe_42.ShowWindow(SW_HIDE);
	m_btnHuanLe_43.ShowWindow(SW_HIDE);
	m_btnHuanLe_44.ShowWindow(SW_HIDE);
	m_btnHuanLe_45.ShowWindow(SW_HIDE);
	m_btnHuanLe_46.ShowWindow(SW_HIDE);
	m_btnHuanLe_47.ShowWindow(SW_HIDE);
	m_btnHuanLe_48.ShowWindow(SW_HIDE);
	m_btnHuanLe_49.ShowWindow(SW_HIDE);
	m_btnHuanLe_50.ShowWindow(SW_HIDE);
	m_btnHuanLe_51.ShowWindow(SW_HIDE);
	m_btnHuanLe_52.ShowWindow(SW_HIDE);
	m_btnHuanLe_53.ShowWindow(SW_HIDE);
	m_btnHuanLe_54.ShowWindow(SW_HIDE);
	m_btnHuanLe_55.ShowWindow(SW_HIDE);
	m_btnHuanLe_56.ShowWindow(SW_HIDE);
	m_btnHuanLe_57.ShowWindow(SW_HIDE);
	m_btnHuanLe_58.ShowWindow(SW_HIDE);
	m_btnHuanLe_59.ShowWindow(SW_HIDE);
	m_btnHuanLe_60.ShowWindow(SW_HIDE);
	m_btnHuanLe_61.ShowWindow(SW_HIDE);
	m_btnHuanLe_62.ShowWindow(SW_HIDE);
	m_btnHuanLe_63.ShowWindow(SW_HIDE);
	m_btnHuanLe_64.ShowWindow(SW_HIDE);
	m_btnHuanLe_65.ShowWindow(SW_HIDE);
	m_btnHuanLe_66.ShowWindow(SW_HIDE);
	m_btnHuanLe_67.ShowWindow(SW_HIDE);
	m_btnHuanLe_68.ShowWindow(SW_HIDE);
	m_btnHuanLe_69.ShowWindow(SW_HIDE);
	m_btnHuanLe_70.ShowWindow(SW_HIDE);
	m_btnHuanLe_71.ShowWindow(SW_HIDE);
	m_btnHuanLe_72.ShowWindow(SW_HIDE);
	m_btnHuanLe_73.ShowWindow(SW_HIDE);
	m_btnHuanLe_74.ShowWindow(SW_HIDE);
	m_btnHuanLe_75.ShowWindow(SW_HIDE);
	m_btnHuanLe_76.ShowWindow(SW_HIDE);
	m_btnHuanLe_77.ShowWindow(SW_HIDE);
	m_btnHuanLe_78.ShowWindow(SW_HIDE);
	m_btnHuanLe_79.ShowWindow(SW_HIDE);
	m_btnHuanLe_80.ShowWindow(SW_HIDE);


}
void CBeiJingKuai8::ShowShangZhongXia()
{
	m_btnShang.ShowWindow(SW_SHOW);
	m_btnZhong.ShowWindow(SW_SHOW);
	m_btnXia.ShowWindow(SW_SHOW);
}
void CBeiJingKuai8::HideShangZhongXia()
{
	m_btnShang.ShowWindow(SW_HIDE);
	m_btnZhong.ShowWindow(SW_HIDE);
	m_btnXia.ShowWindow(SW_HIDE);
}
void CBeiJingKuai8::ShowJiHeOu()
{
	m_btnJi.ShowWindow(SW_SHOW);
	m_btnHe.ShowWindow(SW_SHOW);
	m_btnOu.ShowWindow(SW_SHOW);
}
void CBeiJingKuai8::HideJiHeOu()
{
	m_btnJi.ShowWindow(SW_HIDE);
	m_btnHe.ShowWindow(SW_HIDE);
	m_btnOu.ShowWindow(SW_HIDE);
}
void CBeiJingKuai8::ShowDaXiaoDanShuang1()
{
	m_btnDaDan1.ShowWindow(SW_SHOW);
	m_btnDaShuang1.ShowWindow(SW_SHOW);
	m_btnXiaoDan1.ShowWindow(SW_SHOW);
	m_btnXiaoShuang1.ShowWindow(SW_SHOW);
}
void CBeiJingKuai8::HideDaXiaoDanShuang1()
{
	m_btnDaDan1.ShowWindow(SW_HIDE);
	m_btnDaShuang1.ShowWindow(SW_HIDE);
	m_btnXiaoDan1.ShowWindow(SW_HIDE);
	m_btnXiaoShuang1.ShowWindow(SW_HIDE);
}
void CBeiJingKuai8::HideHeZhiNums()
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
void CBeiJingKuai8::ShowHezhiNums()
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
void CBeiJingKuai8::AdjustRadio()
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

	pWnd = GetDlgItem(IDC_RIO_SZX);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x,wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_JHO);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HZDXDS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x+wanfa_sel_width*2, wanfa_sel_y,wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
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
void CBeiJingKuai8::AdjustWanFa()
{
	if (m_btnRenXuan.GetSafeHwnd() != NULL) 
	{
		m_btnRenXuan.SetWindowPos(NULL, wanfa_x, wanfa_y, m_btnRenXuan.Width(), m_btnRenXuan.Height(), SWP_NOZORDER);
	}
	if (m_btnTeMa.GetSafeHwnd() != NULL) 
	{
		m_btnTeMa.SetWindowPos(NULL, wanfa_x + wanfa_col_width, wanfa_y , m_btnTeMa.Width(), m_btnTeMa.Height(), SWP_NOZORDER);
	}
	if (m_btnDaXiaoDanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnDaXiaoDanShuang.SetWindowPos(NULL, wanfa_x + wanfa_col_width*2, wanfa_y, m_btnDaXiaoDanShuang.Width(), m_btnDaXiaoDanShuang.Height(), SWP_NOZORDER);
	}
	if (m_btnJiZhi.GetSafeHwnd() != NULL) 
	{
		m_btnJiZhi.SetWindowPos(NULL, wanfa_x+wanfa_col_width*3, wanfa_y, m_btnJiZhi.Width(), m_btnJiZhi.Height(), SWP_NOZORDER);
	}
	if (m_btnFunny.GetSafeHwnd() != NULL) 
	{
		m_btnFunny.SetWindowPos(NULL, wanfa_x+wanfa_col_width*4, wanfa_y, m_btnFunny.Width(), m_btnFunny.Height(), SWP_NOZORDER);
	}
	if (m_btnRenXuan6.GetSafeHwnd() != NULL) 
	{
		m_btnRenXuan6.SetWindowPos(NULL, wanfa_x+wanfa_col_width*5, wanfa_y, m_btnRenXuan6.Width(), m_btnRenXuan6.Height(), SWP_NOZORDER);
	}
	if (m_btnRenXuan7.GetSafeHwnd() != NULL) 
	{
		m_btnRenXuan7.SetWindowPos(NULL, wanfa_x + wanfa_col_width*6, wanfa_y , m_btnRenXuan7.Width(), m_btnRenXuan7.Height(), SWP_NOZORDER);
	}
	if (m_btnRenXuan8.GetSafeHwnd() != NULL) 
	{
		m_btnRenXuan8.SetWindowPos(NULL, wanfa_x + wanfa_col_width*7, wanfa_y, m_btnRenXuan8.Width(), m_btnRenXuan8.Height(), SWP_NOZORDER);
		m_btnRenXuan8.ShowWindow(SW_HIDE);
	}
	if (m_btnRenXuan9.GetSafeHwnd() != NULL) 
	{
		m_btnRenXuan9.SetWindowPos(NULL, wanfa_x+wanfa_col_width*8, wanfa_y, m_btnRenXuan9.Width(), m_btnRenXuan9.Height(), SWP_NOZORDER);
		m_btnRenXuan9.ShowWindow(SW_HIDE);
	}
	if (m_btnRenXuan10.GetSafeHwnd() != NULL) 
	{
		m_btnRenXuan10.SetWindowPos(NULL, wanfa_x+wanfa_col_width*9, wanfa_y, m_btnRenXuan10.Width(), m_btnRenXuan10.Height(), SWP_NOZORDER);
		m_btnRenXuan10.ShowWindow(SW_HIDE);
	}
	if (m_btnQuWei.GetSafeHwnd() != NULL) 
	{
		m_btnQuWei.SetWindowPos(NULL, wanfa_x+wanfa_col_width*7, wanfa_y, m_btnQuWei.Width(), m_btnQuWei.Height(), SWP_NOZORDER);
	}

}

//按钮调整：删除，清零，倍投，投注，追号
void CBeiJingKuai8::AdjustNumView()
{
	if(m_btnBeiTouJia.GetSafeHwnd() != NULL) 
	{
		m_btnBeiTouJia.SetWindowPos(NULL,edit_beitou_x+edit_beitou_width , edit_beitou_y-8, m_btnBeiTouJia.Width(), m_btnBeiTouJia.Height(), SWP_NOZORDER);
	}
	if(m_btnBeiTouJian.GetSafeHwnd() != NULL) 
	{
		m_btnBeiTouJian.SetWindowPos(NULL, edit_beitou_x-m_btnBeiTouJian.Width(), edit_beitou_y-8, m_btnBeiTouJian.Width(), m_btnBeiTouJian.Height(), SWP_NOZORDER);
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
}
//单式调整
void CBeiJingKuai8::AdjustDanShi()
{
	if (m_richDanshiHaoma.GetSafeHwnd() != NULL) 
	{
		m_richDanshiHaoma.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y, edit_haomao_danshi_width, edit_haomao_danshi_height, SWP_NOZORDER);
	}
}

//加注按钮
void CBeiJingKuai8::AdjustAdd()
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
	if(m_btWxzxTip.GetSafeHwnd() != NULL)
	{
		m_btWxzxTip.SetWindowPos(NULL,keyboardinput_x,mouseinput_y-32,m_btWxzxTip.Width(),m_btWxzxTip.Height(),SWP_NOZORDER);
	}

}

//投注框：选择数字，5位数字，0--9的选择
void CBeiJingKuai8::AdjustNumBtn()
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

	if(m_btnShang.GetSafeHwnd() != NULL) 
	{
		m_btnShang.SetWindowPos(NULL, haoma_btn_x , haoma_btn_y+haoma_btn_row_height*2, m_btnShang.Width(), m_btnShang.Height(), SWP_NOZORDER);
	}
	if(m_btnZhong.GetSafeHwnd() != NULL) 
	{
		m_btnZhong.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height*2, m_btnZhong.Width(), m_btnZhong.Height(), SWP_NOZORDER);
	}
	if(m_btnXia.GetSafeHwnd() != NULL) 
	{
		m_btnXia.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+haoma_btn_row_height*2, m_btnXia.Width(), m_btnXia.Height(), SWP_NOZORDER);
	}

	if(m_btnJi.GetSafeHwnd() != NULL) 
	{
		m_btnJi.SetWindowPos(NULL, haoma_btn_x , haoma_btn_y+haoma_btn_row_height*2, m_btnJi.Width(), m_btnJi.Height(), SWP_NOZORDER);
	}
	if(m_btnHe.GetSafeHwnd() != NULL) 
	{
		m_btnHe.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height*2, m_btnHe.Width(), m_btnHe.Height(), SWP_NOZORDER);
	}
	if(m_btnOu.GetSafeHwnd() != NULL) 
	{
		m_btnOu.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+haoma_btn_row_height*2, m_btnOu.Width(), m_btnOu.Height(), SWP_NOZORDER);
	}

	if(m_btnDaDan1.GetSafeHwnd() != NULL) 
	{
		m_btnDaDan1.SetWindowPos(NULL, haoma_btn_x , haoma_btn_y+haoma_btn_row_height*2, m_btnDaDan1.Width(), m_btnDaDan1.Height(), SWP_NOZORDER);
	}
	if(m_btnDaShuang1.GetSafeHwnd() != NULL) 
	{
		m_btnDaShuang1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y+haoma_btn_row_height*2, m_btnDaShuang1.Width(), m_btnDaShuang1.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoDan1.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoDan1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y+haoma_btn_row_height*2, m_btnXiaoDan1.Width(), m_btnXiaoDan1.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoShuang1.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoShuang1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y+haoma_btn_row_height*2, m_btnXiaoShuang1.Width(), m_btnXiaoShuang1.Height(), SWP_NOZORDER);
	}


	//1-16
	if (m_btnHuanLe_01.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_01.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y, m_btnHuanLe_01.Width(), m_btnHuanLe_01.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_02.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_02.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y, m_btnHuanLe_02.Width(), m_btnHuanLe_02.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_03.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_03.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y, m_btnHuanLe_03.Width(), m_btnHuanLe_03.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_04.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_04.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y, m_btnHuanLe_04.Width(), m_btnHuanLe_04.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_05.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_05.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y, m_btnHuanLe_05.Width(), m_btnHuanLe_05.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_06.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_06.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y, m_btnHuanLe_06.Width(), m_btnHuanLe_06.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_07.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_07.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y, m_btnHuanLe_07.Width(), m_btnHuanLe_07.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_08.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_08.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y, m_btnHuanLe_08.Width(), m_btnHuanLe_08.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_09.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_09.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y, m_btnHuanLe_09.Width(), m_btnHuanLe_09.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_10.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_10.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y, m_btnHuanLe_10.Width(), m_btnHuanLe_10.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_11.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_11.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y, m_btnHuanLe_11.Width(), m_btnHuanLe_11.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_12.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_12.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y, m_btnHuanLe_12.Width(), m_btnHuanLe_12.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_13.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_13.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y, m_btnHuanLe_13.Width(), m_btnHuanLe_13.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_14.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_14.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y, m_btnHuanLe_14.Width(), m_btnHuanLe_14.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_15.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_15.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14, haoma_btn_y, m_btnHuanLe_15.Width(), m_btnHuanLe_15.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_16.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_16.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15, haoma_btn_y, m_btnHuanLe_16.Width(), m_btnHuanLe_16.Height(), SWP_NOZORDER);
	}

	//17-32
	if (m_btnHuanLe_17.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_17.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_17.Width(), m_btnHuanLe_17.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_18.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_18.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_18.Width(), m_btnHuanLe_18.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_19.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_19.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_19.Width(), m_btnHuanLe_19.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_20.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_20.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_20.Width(), m_btnHuanLe_20.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_21.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_21.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_21.Width(), m_btnHuanLe_21.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_22.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_22.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_22.Width(), m_btnHuanLe_22.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_23.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_23.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_23.Width(), m_btnHuanLe_23.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_24.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_24.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_24.Width(), m_btnHuanLe_24.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_25.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_25.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_25.Width(), m_btnHuanLe_25.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_26.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_26.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_26.Width(), m_btnHuanLe_26.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_27.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_27.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_27.Width(), m_btnHuanLe_27.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_28.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_28.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_28.Width(), m_btnHuanLe_28.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_29.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_29.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_29.Width(), m_btnHuanLe_29.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_30.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_30.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_30.Width(), m_btnHuanLe_30.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_31.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_31.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_31.Width(), m_btnHuanLe_31.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_32.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_32.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15, haoma_btn_y + haoma_btn_row_height, m_btnHuanLe_32.Width(), m_btnHuanLe_32.Height(), SWP_NOZORDER);
	}

	//33-48
	if (m_btnHuanLe_33.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_33.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_33.Width(), m_btnHuanLe_33.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_34.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_34.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_34.Width(), m_btnHuanLe_34.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_35.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_35.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_35.Width(), m_btnHuanLe_35.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_36.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_36.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_36.Width(), m_btnHuanLe_36.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_37.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_37.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_37.Width(), m_btnHuanLe_37.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_38.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_38.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_38.Width(), m_btnHuanLe_38.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_39.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_39.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_39.Width(), m_btnHuanLe_39.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_40.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_40.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_40.Width(), m_btnHuanLe_40.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_41.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_41.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_41.Width(), m_btnHuanLe_41.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_42.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_42.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_42.Width(), m_btnHuanLe_42.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_43.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_43.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_43.Width(), m_btnHuanLe_43.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_44.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_44.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_44.Width(), m_btnHuanLe_44.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_45.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_45.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_45.Width(), m_btnHuanLe_45.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_46.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_46.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_46.Width(), m_btnHuanLe_46.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_47.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_47.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_47.Width(), m_btnHuanLe_47.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_48.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_48.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15, haoma_btn_y + haoma_btn_row_height*2, m_btnHuanLe_48.Width(), m_btnHuanLe_48.Height(), SWP_NOZORDER);
	}



	//49-64
	if (m_btnHuanLe_49.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_49.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_49.Width(), m_btnHuanLe_49.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_50.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_50.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_50.Width(), m_btnHuanLe_50.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_51.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_51.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_51.Width(), m_btnHuanLe_51.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_52.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_52.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_52.Width(), m_btnHuanLe_52.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_53.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_53.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_53.Width(), m_btnHuanLe_53.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_54.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_54.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_54.Width(), m_btnHuanLe_54.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_55.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_55.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_55.Width(), m_btnHuanLe_55.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_56.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_56.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_56.Width(), m_btnHuanLe_56.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_57.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_57.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_57.Width(), m_btnHuanLe_57.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_58.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_58.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_58.Width(), m_btnHuanLe_58.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_59.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_59.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_59.Width(), m_btnHuanLe_59.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_60.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_60.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_60.Width(), m_btnHuanLe_60.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_61.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_61.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_61.Width(), m_btnHuanLe_61.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_62.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_62.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_62.Width(), m_btnHuanLe_62.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_63.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_63.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_63.Width(), m_btnHuanLe_63.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_64.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_64.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15, haoma_btn_y + haoma_btn_row_height*3, m_btnHuanLe_64.Width(), m_btnHuanLe_64.Height(), SWP_NOZORDER);
	}


	//65-80
	if (m_btnHuanLe_65.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_65.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_65.Width(), m_btnHuanLe_65.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_66.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_66.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_66.Width(), m_btnHuanLe_66.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_67.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_67.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_67.Width(), m_btnHuanLe_67.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_68.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_68.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_68.Width(), m_btnHuanLe_68.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_69.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_69.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_69.Width(), m_btnHuanLe_69.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_70.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_70.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_70.Width(), m_btnHuanLe_70.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_71.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_71.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_71.Width(), m_btnHuanLe_71.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_72.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_72.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_72.Width(), m_btnHuanLe_72.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_73.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_73.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_73.Width(), m_btnHuanLe_73.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_74.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_74.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_74.Width(), m_btnHuanLe_74.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_75.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_75.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_75.Width(), m_btnHuanLe_75.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_76.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_76.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_76.Width(), m_btnHuanLe_76.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_77.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_77.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_77.Width(), m_btnHuanLe_77.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_78.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_78.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_78.Width(), m_btnHuanLe_78.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_79.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_79.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_79.Width(), m_btnHuanLe_79.Height(), SWP_NOZORDER);
	}
	if (m_btnHuanLe_80.GetSafeHwnd() != NULL)
	{
		m_btnHuanLe_80.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15, haoma_btn_y + haoma_btn_row_height*4, m_btnHuanLe_80.Width(), m_btnHuanLe_80.Height(), SWP_NOZORDER);
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
		m_btnDaShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*3, m_btnDaShuang.Width(), m_btnDaShuang.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoDan.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*3, m_btnXiaoDan.Width(), m_btnXiaoDan.Height(), SWP_NOZORDER);
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
		m_btnMin.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*3, m_btnMin.Width(), m_btnMin.Height(), SWP_NOZORDER);
	}
}





BOOL CBeiJingKuai8::OnInitDialog()
{
	CDialog::OnInitDialog();

	ZeroMemory(m_szMD5Haoma,sizeof(m_szMD5Haoma));
	hBrush = NULL;
	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\game_bk3.png"));
	m_numImageList = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num.png"));
	m_numImageList1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num2.png"));
	m_numImageList2 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num3.png"));
	m_numImageList3 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num5.png"));
	m_numImageList4 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx5.png"));
	m_kjNumBig = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big2.png"));
	m_kjNumBig2 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big3.png"));
	m_kjAdd = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Sign.png"));
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
	m_bigNumWidth = m_kjNumBig2->GetWidth() / 80;
	m_bigNumHeight = m_kjNumBig2->GetHeight();
	m_kjNumSmall = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small3.png"));
	m_smallNumWidth = m_kjNumSmall->GetWidth() / 80;
	m_smallNumHeight = m_kjNumSmall->GetHeight();
	m_bmpDxdsh = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx3.png"));
	m_bmpNumTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit.png"));
	m_bmpNumTip1 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit2.png"));

	m_bCanSend=true;
	InitNumberBtns();
	InitListCtrl();

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
	m_btnLock.SetTextFont(&m_zongFont);
	m_btnBeiTouJia.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jia.png"));
	m_btnBeiTouJian.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jian.png"));

	m_richDanshiHaoma.LimitText(-1);
	m_richDanshiHaoma.SetBackgroundColor(false,RGB(200,200,200));
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(88, 78, 77);
	m_richDanshiHaoma.SetDefaultCharFormat(cf);
	m_editBeiTou.SetEnableColor(RGB(0,0,0),RGB(250,243,227),RGB(250,243,227));
	m_editBeiTou.SetFont(&m_zongFont);


	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.SetMaxTipWidth(150);
	m_btWxzxTip.SetImage(CBmpUtil::GetExePath() + _T("skin\\bangzhu.png"));


	m_bKaiJiangzhong=false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBeiJingKuai8::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CBeiJingKuai8::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
//初始化号码按钮
void CBeiJingKuai8::InitNumberBtns()
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


	m_btnHuanLe_01.SetImage(m_numImageList3, 0, 80);
	m_btnHuanLe_02.SetImage(m_numImageList3, 1, 80);
	m_btnHuanLe_03.SetImage(m_numImageList3, 2, 80);
	m_btnHuanLe_04.SetImage(m_numImageList3, 3, 80);
	m_btnHuanLe_05.SetImage(m_numImageList3, 4, 80);
	m_btnHuanLe_06.SetImage(m_numImageList3, 5, 80);
	m_btnHuanLe_07.SetImage(m_numImageList3, 6, 80);
	m_btnHuanLe_08.SetImage(m_numImageList3, 7, 80);
	m_btnHuanLe_09.SetImage(m_numImageList3, 8, 80);
	m_btnHuanLe_10.SetImage(m_numImageList3, 9, 80);

	m_btnHuanLe_11.SetImage(m_numImageList3, 10, 80);
	m_btnHuanLe_12.SetImage(m_numImageList3, 11, 80);
	m_btnHuanLe_13.SetImage(m_numImageList3, 12, 80);
	m_btnHuanLe_14.SetImage(m_numImageList3, 13, 80);
	m_btnHuanLe_15.SetImage(m_numImageList3, 14, 80);
	m_btnHuanLe_16.SetImage(m_numImageList3, 15, 80);
	m_btnHuanLe_17.SetImage(m_numImageList3, 16, 80);
	m_btnHuanLe_18.SetImage(m_numImageList3, 17, 80);
	m_btnHuanLe_19.SetImage(m_numImageList3, 18, 80);
	m_btnHuanLe_20.SetImage(m_numImageList3, 19, 80);

	m_btnHuanLe_21.SetImage(m_numImageList3, 20, 80);
	m_btnHuanLe_22.SetImage(m_numImageList3, 21, 80);
	m_btnHuanLe_23.SetImage(m_numImageList3, 22, 80);
	m_btnHuanLe_24.SetImage(m_numImageList3, 23, 80);
	m_btnHuanLe_25.SetImage(m_numImageList3, 24, 80);
	m_btnHuanLe_26.SetImage(m_numImageList3, 25, 80);
	m_btnHuanLe_27.SetImage(m_numImageList3, 26, 80);
	m_btnHuanLe_28.SetImage(m_numImageList3, 27, 80);
	m_btnHuanLe_29.SetImage(m_numImageList3, 28, 80);
	m_btnHuanLe_30.SetImage(m_numImageList3, 29, 80);

	m_btnHuanLe_31.SetImage(m_numImageList3, 30, 80);
	m_btnHuanLe_32.SetImage(m_numImageList3, 31, 80);
	m_btnHuanLe_33.SetImage(m_numImageList3, 32, 80);
	m_btnHuanLe_34.SetImage(m_numImageList3, 33, 80);
	m_btnHuanLe_35.SetImage(m_numImageList3, 34, 80);
	m_btnHuanLe_36.SetImage(m_numImageList3, 35, 80);
	m_btnHuanLe_37.SetImage(m_numImageList3, 36, 80);
	m_btnHuanLe_38.SetImage(m_numImageList3, 37, 80);
	m_btnHuanLe_39.SetImage(m_numImageList3, 38, 80);
	m_btnHuanLe_40.SetImage(m_numImageList3, 39, 80);

	m_btnHuanLe_41.SetImage(m_numImageList3, 40, 80);
	m_btnHuanLe_42.SetImage(m_numImageList3, 41, 80);
	m_btnHuanLe_43.SetImage(m_numImageList3, 42, 80);
	m_btnHuanLe_44.SetImage(m_numImageList3, 43, 80);
	m_btnHuanLe_45.SetImage(m_numImageList3, 44, 80);
	m_btnHuanLe_46.SetImage(m_numImageList3, 45, 80);
	m_btnHuanLe_47.SetImage(m_numImageList3, 46, 80);
	m_btnHuanLe_48.SetImage(m_numImageList3, 47, 80);
	m_btnHuanLe_49.SetImage(m_numImageList3, 48, 80);
	m_btnHuanLe_50.SetImage(m_numImageList3, 49, 80);

	m_btnHuanLe_51.SetImage(m_numImageList3, 50, 80);
	m_btnHuanLe_52.SetImage(m_numImageList3, 51, 80);
	m_btnHuanLe_53.SetImage(m_numImageList3, 52, 80);
	m_btnHuanLe_54.SetImage(m_numImageList3, 53, 80);
	m_btnHuanLe_55.SetImage(m_numImageList3, 54, 80);
	m_btnHuanLe_56.SetImage(m_numImageList3, 55, 80);
	m_btnHuanLe_57.SetImage(m_numImageList3, 56, 80);
	m_btnHuanLe_58.SetImage(m_numImageList3, 57, 80);
	m_btnHuanLe_59.SetImage(m_numImageList3, 58, 80);
	m_btnHuanLe_60.SetImage(m_numImageList3, 59, 80);

	m_btnHuanLe_61.SetImage(m_numImageList3, 60, 80);
	m_btnHuanLe_62.SetImage(m_numImageList3, 61, 80);
	m_btnHuanLe_63.SetImage(m_numImageList3, 62, 80);
	m_btnHuanLe_64.SetImage(m_numImageList3, 63, 80);
	m_btnHuanLe_65.SetImage(m_numImageList3, 64, 80);
	m_btnHuanLe_66.SetImage(m_numImageList3, 65, 80);
	m_btnHuanLe_67.SetImage(m_numImageList3, 66, 80);
	m_btnHuanLe_68.SetImage(m_numImageList3, 67, 80);
	m_btnHuanLe_69.SetImage(m_numImageList3, 68, 80);
	m_btnHuanLe_70.SetImage(m_numImageList3, 69, 80);

	m_btnHuanLe_71.SetImage(m_numImageList3, 70, 80);
	m_btnHuanLe_72.SetImage(m_numImageList3, 71, 80);
	m_btnHuanLe_73.SetImage(m_numImageList3, 72, 80);
	m_btnHuanLe_74.SetImage(m_numImageList3, 73, 80);
	m_btnHuanLe_75.SetImage(m_numImageList3, 74, 80);
	m_btnHuanLe_76.SetImage(m_numImageList3, 75, 80);
	m_btnHuanLe_77.SetImage(m_numImageList3, 76, 80);
	m_btnHuanLe_78.SetImage(m_numImageList3, 77, 80);
	m_btnHuanLe_79.SetImage(m_numImageList3, 78, 80);
	m_btnHuanLe_80.SetImage(m_numImageList3, 79, 80);


	m_btnShang.SetImage(m_numImageList4,0,10);
	m_btnZhong.SetImage(m_numImageList4,1,10);
	m_btnXia.SetImage(m_numImageList4,2,10);
	m_btnJi.SetImage(m_numImageList4,3,10);
	m_btnHe.SetImage(m_numImageList4,4,10);
	m_btnOu.SetImage(m_numImageList4,5,10);
	m_btnDaDan1.SetImage(m_numImageList4,6,10);
	m_btnDaShuang1.SetImage(m_numImageList4,7,10);
	m_btnXiaoDan1.SetImage(m_numImageList4,8,10);
	m_btnXiaoShuang1.SetImage(m_numImageList4,9,10);

	m_btnDa.SetImage(m_bmpDxdsh, 0, 10);
	m_btnXiao.SetImage(m_bmpDxdsh, 1, 10);
	m_btnDan.SetImage(m_bmpDxdsh, 2, 10);
	m_btnShuang.SetImage(m_bmpDxdsh, 3, 10);
	m_btnDaDan.SetImage(m_bmpDxdsh, 4, 10);
	m_btnDaShuang.SetImage(m_bmpDxdsh, 5, 10);
	m_btnXiaoDan.SetImage(m_bmpDxdsh, 6, 10);
	m_btnXiaoShuang.SetImage(m_bmpDxdsh, 7, 10);

	m_btnMax.SetImage(m_bmpDxdsh, 8, 10);
	m_btnMin.SetImage(m_bmpDxdsh, 9, 10);

}

void CBeiJingKuai8::InitListCtrl()
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
CString CBeiJingKuai8::GetScoreMoshi()
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
int CBeiJingKuai8::GetMoshiRet(CString moshi)
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


CString CBeiJingKuai8::GetBjNum()//获取北京快8的按钮数值
{
	CString str;
	if (IsNumBtnCheck(m_btnHuanLe_01))
	{
		str += _T("01");
	}
	if (IsNumBtnCheck(m_btnHuanLe_02))
	{
		str += _T("02");
	}
	if (IsNumBtnCheck(m_btnHuanLe_03))
	{
		str += _T("03");
	}
	if (IsNumBtnCheck(m_btnHuanLe_04))
	{
		str += _T("04");
	}
	if (IsNumBtnCheck(m_btnHuanLe_05))
	{
		str += _T("05");
	}
	if (IsNumBtnCheck(m_btnHuanLe_06))
	{
		str += _T("06");
	}
	if (IsNumBtnCheck(m_btnHuanLe_07))
	{
		str += _T("07");
	}
	if (IsNumBtnCheck(m_btnHuanLe_08))
	{
		str += _T("08");
	}
	if (IsNumBtnCheck(m_btnHuanLe_09))
	{
		str += _T("09");
	}
	if (IsNumBtnCheck(m_btnHuanLe_10))
	{
		str += _T("10");
	}
	if (IsNumBtnCheck(m_btnHuanLe_11))
	{
		str += _T("11");
	}
	if (IsNumBtnCheck(m_btnHuanLe_12))
	{
		str += _T("12");
	}
	if (IsNumBtnCheck(m_btnHuanLe_13))
	{
		str += _T("13");
	}
	if (IsNumBtnCheck(m_btnHuanLe_14))
	{
		str += _T("14");
	}
	if (IsNumBtnCheck(m_btnHuanLe_15))
	{
		str += _T("15");
	}
	if (IsNumBtnCheck(m_btnHuanLe_16))
	{
		str += _T("16");
	}
	if (IsNumBtnCheck(m_btnHuanLe_17))
	{
		str += _T("17");
	}
	if (IsNumBtnCheck(m_btnHuanLe_18))
	{
		str += _T("18");
	}
	if (IsNumBtnCheck(m_btnHuanLe_19))
	{
		str += _T("19");
	}
	if (IsNumBtnCheck(m_btnHuanLe_20))
	{
		str += _T("20");
	}
	if (IsNumBtnCheck(m_btnHuanLe_21))
	{
		str += _T("21");
	}
	if (IsNumBtnCheck(m_btnHuanLe_22))
	{
		str += _T("22");
	}
	if (IsNumBtnCheck(m_btnHuanLe_23))
	{
		str += _T("23");
	}
	if (IsNumBtnCheck(m_btnHuanLe_24))
	{
		str += _T("24");
	}
	if (IsNumBtnCheck(m_btnHuanLe_25))
	{
		str += _T("25");
	}
	if (IsNumBtnCheck(m_btnHuanLe_26))
	{
		str += _T("26");
	}
	if (IsNumBtnCheck(m_btnHuanLe_27))
	{
		str += _T("27");
	}
	if (IsNumBtnCheck(m_btnHuanLe_28))
	{
		str += _T("28");
	}
	if (IsNumBtnCheck(m_btnHuanLe_29))
	{
		str += _T("29");
	}
	if (IsNumBtnCheck(m_btnHuanLe_30))
	{
		str += _T("30");
	}
	if (IsNumBtnCheck(m_btnHuanLe_31))
	{
		str += _T("31");
	}
	if (IsNumBtnCheck(m_btnHuanLe_32))
	{
		str += _T("32");
	}
	if (IsNumBtnCheck(m_btnHuanLe_33))
	{
		str += _T("33");
	}
	if (IsNumBtnCheck(m_btnHuanLe_34))
	{
		str += _T("34");
	}
	if (IsNumBtnCheck(m_btnHuanLe_35))
	{
		str += _T("35");
	}
	if (IsNumBtnCheck(m_btnHuanLe_36))
	{
		str += _T("36");
	}
	if (IsNumBtnCheck(m_btnHuanLe_37))
	{
		str += _T("37");
	}
	if (IsNumBtnCheck(m_btnHuanLe_38))
	{
		str += _T("38");
	}
	if (IsNumBtnCheck(m_btnHuanLe_39))
	{
		str += _T("39");
	}
	if (IsNumBtnCheck(m_btnHuanLe_40))
	{
		str += _T("40");
	}
	if (IsNumBtnCheck(m_btnHuanLe_41))
	{
		str += _T("41");
	}
	if (IsNumBtnCheck(m_btnHuanLe_42))
	{
		str += _T("42");
	}
	if (IsNumBtnCheck(m_btnHuanLe_43))
	{
		str += _T("43");
	}
	if (IsNumBtnCheck(m_btnHuanLe_44))
	{
		str += _T("44");
	}
	if (IsNumBtnCheck(m_btnHuanLe_45))
	{
		str += _T("45");
	}
	if (IsNumBtnCheck(m_btnHuanLe_46))
	{
		str += _T("46");
	}
	if (IsNumBtnCheck(m_btnHuanLe_47))
	{
		str += _T("47");
	}
	if (IsNumBtnCheck(m_btnHuanLe_48))
	{
		str += _T("48");
	}
	if (IsNumBtnCheck(m_btnHuanLe_49))
	{
		str += _T("49");
	}
	if (IsNumBtnCheck(m_btnHuanLe_50))
	{
		str += _T("50");
	}
	if (IsNumBtnCheck(m_btnHuanLe_51))
	{
		str += _T("51");
	}
	if (IsNumBtnCheck(m_btnHuanLe_52))
	{
		str += _T("52");
	}
	if (IsNumBtnCheck(m_btnHuanLe_53))
	{
		str += _T("53");
	}
	if (IsNumBtnCheck(m_btnHuanLe_54))
	{
		str += _T("54");
	}
	if (IsNumBtnCheck(m_btnHuanLe_55))
	{
		str += _T("55");
	}
	if (IsNumBtnCheck(m_btnHuanLe_56))
	{
		str += _T("56");
	}
	if (IsNumBtnCheck(m_btnHuanLe_57))
	{
		str += _T("57");
	}
	if (IsNumBtnCheck(m_btnHuanLe_58))
	{
		str += _T("58");
	}
	if (IsNumBtnCheck(m_btnHuanLe_59))
	{
		str += _T("59");
	}
	if (IsNumBtnCheck(m_btnHuanLe_60))
	{
		str += _T("60");
	}
	if (IsNumBtnCheck(m_btnHuanLe_61))
	{
		str += _T("61");
	}
	if (IsNumBtnCheck(m_btnHuanLe_62))
	{
		str += _T("62");
	}
	if (IsNumBtnCheck(m_btnHuanLe_63))
	{
		str += _T("63");
	}
	if (IsNumBtnCheck(m_btnHuanLe_64))
	{
		str += _T("64");
	}
	if (IsNumBtnCheck(m_btnHuanLe_65))
	{
		str += _T("65");
	}
	if (IsNumBtnCheck(m_btnHuanLe_66))
	{
		str += _T("66");
	}
	if (IsNumBtnCheck(m_btnHuanLe_67))
	{
		str += _T("67");
	}
	if (IsNumBtnCheck(m_btnHuanLe_68))
	{
		str += _T("68");
	}
	if (IsNumBtnCheck(m_btnHuanLe_69))
	{
		str += _T("69");
	}
	if (IsNumBtnCheck(m_btnHuanLe_70))
	{
		str += _T("70");
	}
	if (IsNumBtnCheck(m_btnHuanLe_71))
	{
		str += _T("71");
	}
	if (IsNumBtnCheck(m_btnHuanLe_72))
	{
		str += _T("72");
	}
	if (IsNumBtnCheck(m_btnHuanLe_73))
	{
		str += _T("73");
	}
	if (IsNumBtnCheck(m_btnHuanLe_74))
	{
		str += _T("74");
	}
	if (IsNumBtnCheck(m_btnHuanLe_75))
	{
		str += _T("75");
	}
	if (IsNumBtnCheck(m_btnHuanLe_76))
	{
		str += _T("76");
	}
	if (IsNumBtnCheck(m_btnHuanLe_77))
	{
		str += _T("77");
	}
	if (IsNumBtnCheck(m_btnHuanLe_78))
	{
		str += _T("78");
	}
	if (IsNumBtnCheck(m_btnHuanLe_79))
	{
		str += _T("79");
	}
	if (IsNumBtnCheck(m_btnHuanLe_80))
	{
		str += _T("80");
	}
	return str;
}
CString CBeiJingKuai8::GetShangZhongXiaString()
{
	CString str;

	if(IsNumBtnCheck(m_btnShang))
	{
		str += _T("0");
	}
	if(IsNumBtnCheck(m_btnZhong))
	{
		str += _T("1");
	}
	if(IsNumBtnCheck(m_btnXia))
	{
		str += _T("2");
	}
	return str;
}
CString CBeiJingKuai8::GetJiHeOuString()
{
	CString str;

	if(IsNumBtnCheck(m_btnJi))
	{
		str += _T("0");
	}
	if(IsNumBtnCheck(m_btnHe))
	{
		str += _T("1");
	}
	if(IsNumBtnCheck(m_btnOu))
	{
		str += _T("2");
	}
	return str;
}
CString CBeiJingKuai8::GetDaXiaoDanShuang1String()
{
	CString str;

	if(IsNumBtnCheck(m_btnDaDan1))
	{
		str += _T("0");
	}
	if(IsNumBtnCheck(m_btnDaShuang1))
	{
		str += _T("1");
	}
	if(IsNumBtnCheck(m_btnXiaoDan1))
	{
		str += _T("2");
	}
	if(IsNumBtnCheck(m_btnXiaoShuang1))
	{
		str += _T("3");
	}
	return str;
}
CString CBeiJingKuai8::GetHezhiString()//获取幸运28数值
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
CString CBeiJingKuai8::GetJiZhiString()
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

CString CBeiJingKuai8::GetDxdshString()
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
	if (IsNumBtnCheck(m_btnDaShuang)) 
	{
		strGe += _T("13,");
	}
	if (IsNumBtnCheck(m_btnXiaoDan)) 
	{
		strGe += _T("10,");
	}
	if (IsNumBtnCheck(m_btnXiaoShuang)) 
	{
		strGe += _T("11,");
	}


	return strGe;
}
CString CBeiJingKuai8::GetDxdshStringDesc(int nNum)
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

CString CBeiJingKuai8::GetWanDesc()
{
	return theGameKind[(CZKUAILE8 << 16) | (unsigned short)m_gameKind];
}

//服务器端返回前N个开奖号码
bool CBeiJingKuai8::GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nIndex)
{
	_tcscpy_s(m_kjXinxi[nIndex].qihao, KJ_QIHAO_LENGTH, pResult->wPeriod);
	_tcscpy_s(m_kjXinxi[nIndex].haoma, KJ_HAOMA_LENGTH, pResult->szLotNum);
	_tcscpy_s(m_kjXinxi[nIndex].shijian, KJ_SHIJIAN_LENGTH, pResult->szShijian);

	if(nIndex+1 < TOP_KAIJIANG_NUMBERS)
		return true;

	m_kjXinxiCont = TOP_KAIJIANG_NUMBERS;
	if(!IsWindowVisible())
		return false;

	return true;

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

BOOL CBeiJingKuai8::PreTranslateMessage(MSG* pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}
	return __super::PreTranslateMessage(pMsg);
}

void CBeiJingKuai8::CheckWanFaBtn(CTextButton& btn)
{
	m_btnRenXuan.SetPushed(false);
	m_btnTeMa.SetPushed(false);
	m_btnDaXiaoDanShuang.SetPushed(false);
	m_btnJiZhi.SetPushed(false);
	m_btnFunny.SetPushed(false);
	m_btnRenXuan6.SetPushed(false);
	m_btnRenXuan7.SetPushed(false);
	m_btnRenXuan8.SetPushed(false);
	m_btnRenXuan9.SetPushed(false);
	m_btnRenXuan10.SetPushed(false);

	m_btnQuWei.SetPushed(false);

	btn.SetPushed(true);
}

bool CBeiJingKuai8::GetDanShiHaoma(CString& haoma, int size, int nLeast)
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

bool CBeiJingKuai8::GetHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast)
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
LRESULT CBeiJingKuai8::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	//OnBnClickedBtnDelSel();
	nSubItem--;

	int zhushu = _ttoi(m_listChqSSC.GetItemText(nItem, 4));
	m_nZongZhuShu -= zhushu;
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


BOOL CBeiJingKuai8::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	if((nCommandID >= IDC_BTN_HEZHI_0 && nCommandID <= IDC_BTN_BJ_XIAOSHUANG)||(nCommandID >= IDC_BTN_DA_SHI && nCommandID <= IDC_BTN_WAN_2)||(nCommandID >= IDC_BTN_HEZHI_0 && nCommandID <= IDC_BTN_HEZHI_27)||(nCommandID >= IDC_BTN_DA_WAN && nCommandID <= IDC_BTN_SHUANG_BAI)||(nCommandID >= IDC_BTN_NIU_1 && nCommandID <= IDC_BTN_NIU_10))
	{
		int nZhushu = GetKl8Zhushu(CZKUAILE8,m_gameKind);
	}

	return  __super::OnCommand(wParam,lParam);;
}

int CBeiJingKuai8::GetKl8Zhushu(int nTypeID,int nKindID)
{
	int zhushu = 1;
	CString strHaoMa;

	if(m_gameKind == enBJK8_RX_1)
	{
		CString strNum;
		strNum = GetBjNum();

		zhushu=0;
		int nCount  = strNum.GetLength();
		if (nCount < 1 || nCount > 26)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		for(int i = 0;i < nCount/2;i++)
		{
			if(i*2 !=nCount-2)
			{
				zhushu++;
			}
		}

		zhushu++;

		m_singlezhushu = zhushu;

	}
	else if(m_gameKind == enBJK8_RX_2)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 4 || nCount > 26)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		int count = nCount/2;
		m_singlezhushu = (count*(count-1))/2;

	}
	else if(m_gameKind == enBJK8_RX_3)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 6 || nCount > 26)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		int count = nCount/2;
		m_singlezhushu = (count*(count-1)*(count-2))/6;

	}
	else if(m_gameKind == enBJK8_RX_4)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 8||nCount > 26)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		int count = nCount/2;
		m_singlezhushu = (count*(count-1)*(count-2)*(count-3))/24;

	}
	else if(m_gameKind == enBJK8_RX_5)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 10 || nCount > 26)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		int count = nCount/2;
		m_singlezhushu = (count*(count-1)*(count-2)*(count-3)*(count-4))/120;

	}
	else if(m_gameKind == enBJK8_RX_6)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 12 || nCount>26)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		int count = nCount/2;
		m_singlezhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5))/720;

	}
	else if(m_gameKind == enBJK8_RX_7)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 14 || nCount > 26)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		int count = nCount/2;
		m_singlezhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6))/5040;

	}
	else if(m_gameKind == enBJK8_RX_8)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 16 || nCount > 26)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}
		int count = nCount/2;
		m_singlezhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6)*(count-7))/40320;

	}
	else if(m_gameKind == enBJK8_RX_9)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 18 || nCount > 26)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		int count = nCount/2;
		m_singlezhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6)*(count-7)*(count-8))/362880;

	}
	else if(m_gameKind == enBJK8_RX_10)
	{
		CString strNum;
		strNum = GetBjNum();

		int nCount  = strNum.GetLength();
		if (nCount < 20 || nCount > 26)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		int count = nCount/2;
		zhushu = (count*(count-1)*(count-2)*(count-3)*(count-4)*(count-5)*(count-6)*(count-7)*(count-8)*(count-9))/3628800;

	}
	else if(m_gameKind == enBJK8_ShangXia)
	{
		CString strShang = GetShangZhongXiaString();
		if(strShang.GetLength()<=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = strShang.GetLength();

	}
	else if(m_gameKind == enBJK8_JiOu)
	{
		CString strJiHeOu = GetJiHeOuString();
		if(strJiHeOu.GetLength()<=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = strJiHeOu.GetLength();

	}
	else if(m_gameKind == enBJK8_HeZhiDXDS)
	{
		CString strDaXiaoDanShuang = GetDaXiaoDanShuang1String();
		if(strDaXiaoDanShuang.GetLength()<=0)
		{
			m_singlezhushu = 0;
			CRect rcInvali;
			rcInvali.CopyRect(rcZongZhuShu);
			rcInvali.right+=400;
			InvalidateRect(&rcInvali);
			InvalidateRect(&rcZongJinE);

			return m_singlezhushu;
		}

		m_singlezhushu = strDaXiaoDanShuang.GetLength();

	}



	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	return m_singlezhushu;
}
