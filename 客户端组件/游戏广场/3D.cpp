#include "stdafx.h"
#include "GamePlaza.h"
#include "PaiLie3.h"
#include "BmpUtil.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
#include "QrTzhDlg.h"
#include "GDIpng/MemDC.h"
#include "DlgFandian.h"
#include "DlgAddtional.h"
#include <map>
using		namespace		std;
extern map <int,CString> mapFandian;
extern map <int,float> mapPercent;
extern map <int,float> mapDanweiFandian;
extern map <int,float> mapBonus;
extern vector<wstring>	vecData1, vecData2, vecData3;

extern vector<wstring> vecAllNums;
//玩法分类数据
static const int wanfa_x = 9;
static const int wanfa_y = 109;
static const int wanfa_col_width = 64;
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
static const int kj_big_haoma_x = 276;
static const int kj_big_haoma_y = 38;

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

static const int timespan_kj_shj = 24 * 60 * 60;
static const int timer_id_kaijiangzhong=3;
static const int TimerJiShi = 4;
CString   C3D::m_strHaoma = _T("");
//排列3

IMPLEMENT_DYNAMIC(C3D, CDialog)

C3D::C3D(CWnd* pParent /*=NULL*/)
	: CDialog(C3D::IDD, pParent)
	, m_bmpBk(NULL)
	, m_numImageList(NULL)
	, m_kjNumBig(NULL)
	, m_kjNumSmall(NULL)
	, m_bmpNumTip(NULL)
	, m_gameKind(enWF_ZhixuanFushi)
	, m_zongZhuShu(0)
	, m_beishu(1)
	, m_bigNumWidth(0)
	, m_bigNumHeight(0)
	, m_smallNumWidth(0)
	, m_smallNumHeight(0)
	, m_kjXinxiCont(0)
	, m_pLuckMeDlg(NULL)
	, m_bmpDxdsh(NULL)
{
	for (int i=0; i<Kj_XinXi_Count; i++) 
	{
		memset(&m_kjXinxi[i], 0, sizeof(KjXinXi));
	}
	m_nFrameIndex=0;
	m_nFrameCount=0;
	m_nZhuihaoCount=0;
	m_fBonus = 0.0;
	m_fPercent = 0.0;
	fDanweiFandian=0.0f;
	nBonus = 0.0f;
	m_bZhuiZjtz = true;
	_tcscpy_s(m_lastExpect, KJ_QIHAO_LENGTH, _T(""));
	m_bKaiJiangzhong = false;
	m_DlgStatus.SetStatusViewSink(this);
	m_nTzhSign = 1;
	m_nTzhSign1 = 0;
	InitWanFaMoShi();
}

C3D::~C3D()
{
	if (m_bmpBk != NULL) 
	{
		delete m_bmpBk;
	}
	if (m_bmpDxdsh != NULL) 
	{
		delete m_bmpDxdsh;
	}

	if (m_numImageList != NULL) 
	{
		delete m_numImageList;
	}

	if (m_kjNumBig != NULL) 
	{
		delete m_kjNumBig;
	}

	if (m_kjNumSmall != NULL) 
	{
		delete m_kjNumSmall;
	}

	if (m_bmpNumTip != NULL) 
	{
		delete m_bmpNumTip;
	}

	if (m_imgKaijiang != NULL) 
	{
		m_imgKaijiang=NULL;
		delete m_imgKaijiang;
	}

}

//取消连接
VOID C3D::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

void C3D::ConnectMainDlg(CPlazaViewItem* luckMeDlg)
{
	m_pLuckMeDlg = luckMeDlg;
}

void C3D::FlushZongjine()
{
	InvalidateRect(&rcZongJinE);
}

void C3D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIA,m_btnBeiTouJia);
	DDX_Control(pDX,IDC_BTN_BEITOU_JIAN,m_btnBeiTouJian);
	DDX_Control(pDX,IDC_TIP_WXZX,m_btWxzxTip);
	DDX_Control(pDX, IDC_BTN_MORE_RECORD, m_btnMoreRecord);

	DDX_Control(pDX, IDC_BTN_BAI_3D_0, m_btn3DBai0);
	DDX_Control(pDX, IDC_BTN_BAI_3D_1, m_btn3DBai1);
	DDX_Control(pDX, IDC_BTN_BAI_3D_2, m_btn3DBai2);
	DDX_Control(pDX, IDC_BTN_BAI_3D_3, m_btn3DBai3);
	DDX_Control(pDX, IDC_BTN_BAI_3D_4, m_btn3DBai4);
	DDX_Control(pDX, IDC_BTN_BAI_3D_5, m_btn3DBai5);
	DDX_Control(pDX, IDC_BTN_BAI_3D_6, m_btn3DBai6);
	DDX_Control(pDX, IDC_BTN_BAI_3D_7, m_btn3DBai7);
	DDX_Control(pDX, IDC_BTN_BAI_3D_8, m_btn3DBai8);
	DDX_Control(pDX, IDC_BTN_BAI_3D_9, m_btn3DBai9);

	DDX_Control(pDX, IDC_BTN_SHI_3D_0, m_btn3DShi0);
	DDX_Control(pDX, IDC_BTN_SHI_3D_1, m_btn3DShi1);
	DDX_Control(pDX, IDC_BTN_SHI_3D_2, m_btn3DShi2);
	DDX_Control(pDX, IDC_BTN_SHI_3D_3, m_btn3DShi3);
	DDX_Control(pDX, IDC_BTN_SHI_3D_4, m_btn3DShi4);
	DDX_Control(pDX, IDC_BTN_SHI_3D_5, m_btn3DShi5);
	DDX_Control(pDX, IDC_BTN_SHI_3D_6, m_btn3DShi6);
	DDX_Control(pDX, IDC_BTN_SHI_3D_7, m_btn3DShi7);
	DDX_Control(pDX, IDC_BTN_SHI_3D_8, m_btn3DShi8);
	DDX_Control(pDX, IDC_BTN_SHI_3D_9, m_btn3DShi9);

	DDX_Control(pDX, IDC_BTN_GE_3D_0, m_btn3DGe0);
	DDX_Control(pDX, IDC_BTN_GE_3D_1, m_btn3DGe1);
	DDX_Control(pDX, IDC_BTN_GE_3D_2, m_btn3DGe2);
	DDX_Control(pDX, IDC_BTN_GE_3D_3, m_btn3DGe3);
	DDX_Control(pDX, IDC_BTN_GE_3D_4, m_btn3DGe4);
	DDX_Control(pDX, IDC_BTN_GE_3D_5, m_btn3DGe5);
	DDX_Control(pDX, IDC_BTN_GE_3D_6, m_btn3DGe6);
	DDX_Control(pDX, IDC_BTN_GE_3D_7, m_btn3DGe7);
	DDX_Control(pDX, IDC_BTN_GE_3D_8, m_btn3DGe8);
	DDX_Control(pDX, IDC_BTN_GE_3D_9, m_btn3DGe9);
	DDX_Control(pDX, IDC_BTN_3D_SXZHX, m_btnSanXingZhiXuan);
	DDX_Control(pDX, IDC_BTN_3D_SXZX, m_btnSanXingZuXuan);
	DDX_Control(pDX, IDC_BTN_3D_QYZHX, m_btnQianYiZhiXuan);
	DDX_Control(pDX, IDC_BTN_3D_QEZHX, m_btnQianErZhiXuan);
	DDX_Control(pDX, IDC_BTN_3D_SXBDW, m_btnSanXingBuDingWei);
	DDX_Control(pDX, IDC_BTN_3D_HYZHX, m_btnHouYiZhiXuan);
	DDX_Control(pDX, IDC_BTN_3D_HEZHX, m_btnHouErZhiXuan);
	DDX_Control(pDX, IDC_BTN_3D_DWD, m_btnDingWeiDan);
	DDX_Control(pDX, IDC_BTN_3D_DWDXDS, m_btnDaXiaoDanShuang);
	DDX_Control(pDX, IDC_BTN_3D_SXZHXHZH, m_btnZhiXuanHeZhi);
	DDX_Control(pDX, IDC_BTN_BAI_3D_QUAN, m_btn3DBaiQuan);
	DDX_Control(pDX, IDC_BTN_BAI_3D_DA, m_btn3DBaiDa);
	DDX_Control(pDX, IDC_BTN_BAI_3D_XIAO, m_btn3DBaiXiao);
	DDX_Control(pDX, IDC_BTN_BAI_3D_DAN, m_btn3DBaiDan);
	DDX_Control(pDX, IDC_BTN_BAI_3D_SHUANG, m_btn3DBaiShuang);
	DDX_Control(pDX, IDC_BTN_BAI_3D_QING, m_btn3DBaiQing);
	DDX_Control(pDX, IDC_BTN_SHI_3D_QUAN, m_btn3DShiQuan);
	DDX_Control(pDX, IDC_BTN_SHI_3D_DA, m_btn3DShiDa);
	DDX_Control(pDX, IDC_BTN_SHI_3D_XIAO, m_btn3DShiXiao);
	DDX_Control(pDX, IDC_BTN_SHI_3D_DAN, m_btn3DShiDan);
	DDX_Control(pDX, IDC_BTN_SHI_3D_SHUANG, m_btn3DShiShuang);
	DDX_Control(pDX, IDC_BTN_SHI_3D_QING, m_btn3DShiQing);
	DDX_Control(pDX, IDC_BTN_GE_3D_QUAN, m_btn3DGeQuan);
	DDX_Control(pDX, IDC_BTN_GE_3D_DA, m_btn3DGeDa);
	DDX_Control(pDX, IDC_BTN_GE_3D_XIAO, m_btn3DGeXiao);
	DDX_Control(pDX, IDC_BTN_GE_3D_DAN, m_btn3DGeDan);
	DDX_Control(pDX, IDC_BTN_GE_3D_SHUANG, m_btn3DGeShuang);
	DDX_Control(pDX, IDC_BTN_GE_3D_QING, m_btn3DGeQing);
	DDX_Control(pDX, IDC_STATIC_BAI, m_staticBai);
	DDX_Control(pDX, IDC_STATIC_SHI, m_staticShi);
	DDX_Control(pDX, IDC_STATIC_GE, m_staticGe);
	DDX_Control(pDX, IDC_EDIT_DANSHI_HAOMA, m_editDanShiHaoMa);
	DDX_Control(pDX, IDC_RICHEDIT_DANSHI, m_richDanshiHaoma);

	DDX_Control(pDX, IDC_BTN_3D_ADD, m_btn3DAdd);
	DDX_Control(pDX, IDC_BTN_LOCK, m_btnLock);
	DDX_Control(pDX, IDC_BTN_DEL_SEL, m_btnDelSel);
	DDX_Control(pDX, IDC_BTN_CLS_LIST, m_btnClsList);
	DDX_Control(pDX, IDC_LIST_3D, m_list3D);
	DDX_Control(pDX, IDC_EDIT_BEISHU, m_editBeiShu);
	DDX_Control(pDX, IDC_BTN_TOUZHU, m_btnTouZhu);
	DDX_Control(pDX, IDC_BTN_ZHUIHAO, m_btnZhuiHao);

	DDX_Control(pDX, IDC_RIO_SXZHX_FUSHI, m_rioSxzhxFuShi);
	DDX_Control(pDX, IDC_RIO_SXZHX_DANSHI, m_rioSxzhxDanShi);
	DDX_Control(pDX, IDC_RIO_HEZHX_FUSHI, m_rioHeZhxFuShi);
	DDX_Control(pDX, IDC_RIO_HEZHX_DANSHI, m_rioHeZhxDanShi);
	DDX_Control(pDX, IDC_RIO_SXZS_FUSHI, m_rioSxzsFuShi);
	DDX_Control(pDX, IDC_RIO_SXZS_DANSHI, m_rioSxzsDanShi);
	DDX_Control(pDX, IDC_RIO_SXZL_FUSHI, m_rioSxzlFuShi);
	DDX_Control(pDX, IDC_RIO_SXZL_DANSHI, m_rioSxzlDanShi);
	DDX_Control(pDX, IDC_RIO_SXZHX_HEZHI, m_rioSxzhxHeZhi);
	DDX_Control(pDX, IDC_RIO_QYZHX, m_rioQyZhx);
	DDX_Control(pDX, IDC_RIO_QEZHX_FUSHI, m_rioQeZhxFuShi);
	DDX_Control(pDX, IDC_RIO_QEZHX_DANSHI, m_rioQeZhxDanShi);
	DDX_Control(pDX, IDC_RIO_SXBDW, m_rioSxbdw);
	DDX_Control(pDX, IDC_RIO_HYZHX, m_rioHyzhx);
	DDX_Control(pDX, IDC_RIO_DWD, m_rioDwd);
	DDX_Control(pDX, IDC_RIO_DWDXDS, m_rioDwDxds);
	DDX_Control(pDX, IDC_STATIC_ZUXUAN, m_staticZuXuan);
	DDX_Control(pDX, IDC_STATIC_BUDINGWEI, m_staticBuDingWei);
	DDX_Text(pDX, IDC_EDIT_BEISHU, m_beishu);
	

	DDX_Control(pDX, IDC_BTN_DA_BAI, m_btnDaBai);
	DDX_Control(pDX, IDC_BTN_XIAO_BAI, m_btnXiaoBai);
	DDX_Control(pDX, IDC_BTN_DAN_BAI, m_btnDanBai);
	DDX_Control(pDX, IDC_BTN_SHUANG_BAI, m_btnShuangBai);

	DDX_Control(pDX, IDC_BTN_DA_SHI, m_btnDaShi);
	DDX_Control(pDX, IDC_BTN_XIAO_SHI, m_btnXiaoShi);
	DDX_Control(pDX, IDC_BTN_DAN_SHI, m_btnDanShi);
	DDX_Control(pDX, IDC_BTN_SHUANG_SHI, m_btnShuangShi);
	DDX_Control(pDX, IDC_BTN_DA_GE, m_btnDaGe);
	DDX_Control(pDX, IDC_BTN_XIAO_GE, m_btnXiaoGe);
	DDX_Control(pDX, IDC_BTN_DAN_GE, m_btnDanGe);
	DDX_Control(pDX, IDC_BTN_SHUANG_GE, m_btnShuangGe);

	DDX_Control(pDX, IDC_BTN_YUAN,m_btnYuan);
	DDX_Control(pDX, IDC_BTN_JIAO,m_btnJiao);
	DDX_Control(pDX, IDC_BTN_FEN,m_btnFen);
	DDX_Control(pDX, IDC_BTN_LI,m_btnLi);
}


BEGIN_MESSAGE_MAP(C3D, CDialog)
	ON_BN_CLICKED(IDC_BTN_MORE_RECORD, &C3D::OnBnClickedBtnMoreRecord)
	ON_BN_CLICKED(IDC_BTN_YUAN, &C3D::OnBnClickedBtnYuan)
	ON_BN_CLICKED(IDC_BTN_JIAO, &C3D::OnBnClickedBtnJiao)
	ON_BN_CLICKED( IDC_BTN_FEN, &C3D::OnBnClickedBtnFen)
	ON_BN_CLICKED(IDC_BTN_LI, &C3D::OnBnClickedBtnLi)
	ON_BN_CLICKED(IDC_BTN_3D_SXZHX, &C3D::OnBnClickedBtn3dSxzhx)
	ON_BN_CLICKED(IDC_BTN_3D_SXZX, &C3D::OnBnClickedBtn3dSxzx)
	ON_BN_CLICKED(IDC_BTN_3D_QYZHX, &C3D::OnBnClickedBtn3dQyzhx)
	ON_BN_CLICKED(IDC_BTN_3D_QEZHX, &C3D::OnBnClickedBtn3dQezhx)
	ON_BN_CLICKED(IDC_BTN_3D_SXBDW, &C3D::OnBnClickedBtn3dSxbdw)
	ON_BN_CLICKED(IDC_BTN_3D_HYZHX, &C3D::OnBnClickedBtn3dHyzhx)
	ON_BN_CLICKED(IDC_BTN_3D_HEZHX, &C3D::OnBnClickedBtn3dHezhx)
	ON_BN_CLICKED(IDC_BTN_3D_DWD, &C3D::OnBnClickedBtn3dDwd)
	ON_BN_CLICKED(IDC_BTN_3D_DWDXDS, &C3D::OnBnClickedBtn3dDxds)
	ON_BN_CLICKED(IDC_BTN_BAI_3D_QUAN, &C3D::OnBnClickedBtnBai3dQuan)
	ON_BN_CLICKED(IDC_BTN_BAI_3D_DA, &C3D::OnBnClickedBtnBai3dDa)
	ON_BN_CLICKED(IDC_BTN_BAI_3D_XIAO, &C3D::OnBnClickedBtnBai3dXiao)
	ON_BN_CLICKED(IDC_BTN_BAI_3D_DAN, &C3D::OnBnClickedBtnBai3dDan)
	ON_BN_CLICKED(IDC_BTN_BAI_3D_SHUANG, &C3D::OnBnClickedBtnBai3dShuang)
	ON_BN_CLICKED(IDC_BTN_BAI_3D_QING, &C3D::OnBnClickedBtnBai3dQing)
	ON_BN_CLICKED(IDC_BTN_SHI_3D_QUAN, &C3D::OnBnClickedBtnShi3dQuan)
	ON_BN_CLICKED(IDC_BTN_SHI_3D_DA, &C3D::OnBnClickedBtnShi3dDa)
	ON_BN_CLICKED(IDC_BTN_SHI_3D_XIAO, &C3D::OnBnClickedBtnShi3dXiao)
	ON_BN_CLICKED(IDC_BTN_SHI_3D_DAN, &C3D::OnBnClickedBtnShi3dDan)
	ON_BN_CLICKED(IDC_BTN_SHI_3D_SHUANG, &C3D::OnBnClickedBtnShi3dShuang)
	ON_BN_CLICKED(IDC_BTN_SHI_3D_QING, &C3D::OnBnClickedBtnShi3dQing)
	ON_BN_CLICKED(IDC_BTN_GE_3D_QUAN, &C3D::OnBnClickedBtnGe3dQuan)
	ON_BN_CLICKED(IDC_BTN_GE_3D_DA, &C3D::OnBnClickedBtnGe3dDa)
	ON_BN_CLICKED(IDC_BTN_GE_3D_XIAO, &C3D::OnBnClickedBtnGe3dXiao)
	ON_BN_CLICKED(IDC_BTN_GE_3D_DAN, &C3D::OnBnClickedBtnGe3dDan)
	ON_BN_CLICKED(IDC_BTN_GE_3D_SHUANG, &C3D::OnBnClickedBtnGe3dShuang)
	ON_BN_CLICKED(IDC_BTN_GE_3D_QING, &C3D::OnBnClickedBtnGe3dQing)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RIO_SXZHX_FUSHI, &C3D::OnBnClickedRioSxzhxFushi)
	ON_BN_CLICKED(IDC_RIO_SXZHX_DANSHI, &C3D::OnBnClickedRioSxzhxDanshi)
	ON_BN_CLICKED(IDC_RIO_HEZHX_FUSHI, &C3D::OnBnClickedRioHezhxFushi)
	ON_BN_CLICKED(IDC_RIO_HEZHX_DANSHI, &C3D::OnBnClickedRioHezhxDanshi)
	ON_BN_CLICKED(IDC_RIO_SXZS_FUSHI, &C3D::OnBnClickedRioSxzsFushi)
	ON_BN_CLICKED(IDC_RIO_SXZS_DANSHI, &C3D::OnBnClickedRioSxzsDanshi)
	ON_BN_CLICKED(IDC_RIO_SXZL_FUSHI, &C3D::OnBnClickedRioSxzlFushi)
	ON_BN_CLICKED(IDC_RIO_SXZL_DANSHI, &C3D::OnBnClickedRioSxzlDanshi)
	ON_BN_CLICKED(IDC_RIO_QYZHX, &C3D::OnBnClickedRioQyzhx)
	ON_BN_CLICKED(IDC_RIO_QEZHX_FUSHI, &C3D::OnBnClickedRioQezhxFushi)
	ON_BN_CLICKED(IDC_RIO_QEZHX_DANSHI, &C3D::OnBnClickedRioQezhxDanshi)
	ON_BN_CLICKED(IDC_RIO_SXBDW, &C3D::OnBnClickedRioSxbdw)
	ON_BN_CLICKED(IDC_RIO_HYZHX, &C3D::OnBnClickedRioHyzhx)
	ON_BN_CLICKED(IDC_RIO_DWD, &C3D::OnBnClickedRioDwd)
	ON_BN_CLICKED(IDC_RIO_DWDXDS, &C3D::OnBnClickedRioDwDxds)
	ON_BN_CLICKED(IDC_RIO_SXZHX_HEZHI, &C3D::OnBnClickedRioSxzhxHezhi)
	ON_BN_CLICKED(IDC_BTN_3D_ADD, &C3D::OnBnClickedBtn3dAdd)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_DEL_SEL, &C3D::OnBnClickedBtnDelSel)
	ON_BN_CLICKED(IDC_BTN_CLS_LIST, &C3D::OnBnClickedBtnClsList)
	ON_BN_CLICKED(IDC_BTN_TOUZHU, &C3D::OnBnClickedBtnTouzhu)
	ON_BN_CLICKED(IDC_BTN_ZHUIHAO, &C3D::OnBnClickedBtnZhuihao)
	ON_EN_CHANGE(IDC_EDIT_BEISHU, &C3D::OnEnChangeEditBeishu)
	ON_EN_CHANGE(IDC_EDIT_DANSHI_HAOMA, &C3D::OnEnChangeEditDanshiInput)
	ON_BN_CLICKED(IDC_BTN_LOCK, &C3D::OnBnClickedBtnLock)
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIA, &C3D::OnBnClickedBeiTouJia)
	ON_BN_CLICKED(IDC_BTN_BEITOU_JIAN, &C3D::OnBnClickedBeiTouJian)

	ON_MESSAGE( WM_BN_CLICK, &C3D::onBnCLick)
	ON_EN_CHANGE(IDC_RICHEDIT_DANSHI, &C3D::OnEnChangeRichEditDanshiInput)
	ON_COMMAND(ID_RBTN_CPY, &C3D::OnRichEditCopy)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void C3D::OnRichEditCopy()
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

// C3D 消息处理程序
void C3D::OnPaint()
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

	//绘制开奖号
	DrawLastKjHaoma(&cacheDC, graphics);
	DrawTopKjHaoma(&cacheDC, graphics);

	//绘制总注数和总金额
	DrawZongZhushuJinE(&cacheDC);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheDC.DeleteDC();
	cacheBmp.DeleteObject();
}

BOOL C3D::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void C3D::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

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
void C3D::OnBnClickedBtnYuan()
{
	m_btnYuan.SetPushed(true);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);

	theAccount.Scoretype=MoShi_Yuan ;

	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);

	return;
}
void C3D::OnBnClickedBtnMoreRecord()
{
	SendToServer(8);
	return;
}

void C3D::OnBnClickedBtnJiao()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(true);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(false);

	theAccount.Scoretype=MoShi_Jiao ;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);
	return;
}
void C3D::OnBnClickedBtnFen()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(true);
	m_btnLi.SetPushed(false);

	theAccount.Scoretype=MoShi_Fen ;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);
	return;

}
void C3D::OnBnClickedBtnLi()
{
	m_btnYuan.SetPushed(false);
	m_btnJiao.SetPushed(false);
	m_btnFen.SetPushed(false);
	m_btnLi.SetPushed(true);

	theAccount.Scoretype=MoShi_Li ;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);
	return;
}

void C3D::OnTimer(UINT_PTR nIDEvent)
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

	CDialog::OnTimer(nIDEvent);
}


void C3D::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;
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

		SendToServer(6);
		OnBnClickedBtn3dSxzhx();
		SetTimer(timer_id_kaijiangzhong, 100, NULL);

	}
	else
	{
		KillTimer(timer_id_kaijiangzhong);
		m_DlgOpenRecord.ShowWindow(SW_HIDE);

	}
}

HBRUSH C3D::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() == IDC_EDIT_DANSHI_HAOMA) {
		pDC->SetBkMode(TRANSPARENT); 
		HBRUSH B = CreateSolidBrush(RGB(180, 218, 237)); 
		return (HBRUSH) B; 
	}
	HBRUSH hBrush=NULL;

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

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


//三星直选
void C3D::OnBnClickedBtn3dSxzhx()
{
	CheckWanFaBtn(m_btnSanXingZhiXuan);

	HideAllRio();

	ShowCtrl(IDC_RIO_SXZHX_FUSHI);
	ShowCtrl(IDC_RIO_SXZHX_DANSHI);
	OnBnClickedRioSxzhxFushi();
}

//三星组选
void C3D::OnBnClickedBtn3dSxzx()
{
	CheckWanFaBtn(m_btnSanXingZuXuan);

	HideAllRio();

	ShowCtrl(IDC_RIO_SXZS_FUSHI);
	ShowCtrl(IDC_RIO_SXZS_DANSHI);
	ShowCtrl(IDC_RIO_SXZL_FUSHI);
	ShowCtrl(IDC_RIO_SXZL_DANSHI);
	OnBnClickedRioSxzsFushi();
}

//前一直选
void C3D::OnBnClickedBtn3dQyzhx()
{
	m_gameKind = enWF_QianyizhixuanFushi;
	CheckWanFaBtn(m_btnQianYiZhiXuan);

	HideAllRio();

	ShowCtrl(IDC_RIO_QYZHX);
	OnBnClickedRioQyzhx();
}

//前二直选
void C3D::OnBnClickedBtn3dQezhx()
{
	CheckWanFaBtn(m_btnQianErZhiXuan);

	HideAllRio();

	ShowCtrl(IDC_RIO_QEZHX_DANSHI);
	ShowCtrl(IDC_RIO_QEZHX_FUSHI);
	OnBnClickedRioQezhxFushi();
}

//三星不定位
void C3D::OnBnClickedBtn3dSxbdw()
{
	m_gameKind = enWF_Budingwei;
	CheckWanFaBtn(m_btnSanXingBuDingWei);

	HideAllRio();

	ShowCtrl(IDC_RIO_SXBDW);
	OnBnClickedRioSxbdw();
}

//后一直选
void C3D::OnBnClickedBtn3dHyzhx()
{
	m_gameKind = enWF_HouyizhixuanFushi;
	CheckWanFaBtn(m_btnHouYiZhiXuan);

	HideAllRio();

	ShowCtrl(IDC_RIO_HYZHX);
	OnBnClickedRioHyzhx();
}

//后二直选
void C3D::OnBnClickedBtn3dHezhx()
{
	CheckWanFaBtn(m_btnHouErZhiXuan);

	HideAllRio();

	ShowCtrl(IDC_RIO_HEZHX_DANSHI);
	ShowCtrl(IDC_RIO_HEZHX_FUSHI);
	OnBnClickedRioHezhxFushi();
}
//定位胆
void C3D::OnBnClickedBtn3dDwd()
{
	CheckWanFaBtn(m_btnDingWeiDan);

	HideAllRio();

	ShowCtrl(IDC_RIO_DWD);

	OnBnClickedRioDwd();
}
//大小单双
void C3D::OnBnClickedBtn3dDxds()
{
	CheckWanFaBtn(m_btnDaXiaoDanShuang);

	HideAllRio();

	ShowCtrl(IDC_RIO_DWDXDS);

	OnBnClickedRioDwDxds();
}


void C3D::OnBnClickedBtnBai3dQuan()
{
	CheckNumBtn(m_btn3DBai0);
	CheckNumBtn(m_btn3DBai1);
	CheckNumBtn(m_btn3DBai2);
	CheckNumBtn(m_btn3DBai3);
	CheckNumBtn(m_btn3DBai4);
	CheckNumBtn(m_btn3DBai5);
	CheckNumBtn(m_btn3DBai6);
	CheckNumBtn(m_btn3DBai7);
	CheckNumBtn(m_btn3DBai8);
	CheckNumBtn(m_btn3DBai9);

	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnBai3dDa()
{
	UnCheckNumBtn(m_btn3DBai0);
	UnCheckNumBtn(m_btn3DBai1);
	UnCheckNumBtn(m_btn3DBai2);
	UnCheckNumBtn(m_btn3DBai3);
	UnCheckNumBtn(m_btn3DBai4);
	CheckNumBtn(m_btn3DBai5);
	CheckNumBtn(m_btn3DBai6);
	CheckNumBtn(m_btn3DBai7);
	CheckNumBtn(m_btn3DBai8);
	CheckNumBtn(m_btn3DBai9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnBai3dXiao()
{
	CheckNumBtn(m_btn3DBai0);
	CheckNumBtn(m_btn3DBai1);
	CheckNumBtn(m_btn3DBai2);
	CheckNumBtn(m_btn3DBai3);
	CheckNumBtn(m_btn3DBai4);
	UnCheckNumBtn(m_btn3DBai5);
	UnCheckNumBtn(m_btn3DBai6);
	UnCheckNumBtn(m_btn3DBai7);
	UnCheckNumBtn(m_btn3DBai8);
	UnCheckNumBtn(m_btn3DBai9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnBai3dDan()
{
	UnCheckNumBtn(m_btn3DBai0);
	CheckNumBtn(m_btn3DBai1);
	UnCheckNumBtn(m_btn3DBai2);
	CheckNumBtn(m_btn3DBai3);
	UnCheckNumBtn(m_btn3DBai4);
	CheckNumBtn(m_btn3DBai5);
	UnCheckNumBtn(m_btn3DBai6);
	CheckNumBtn(m_btn3DBai7);
	UnCheckNumBtn(m_btn3DBai8);
	CheckNumBtn(m_btn3DBai9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnBai3dShuang()
{
	CheckNumBtn(m_btn3DBai0);
	UnCheckNumBtn(m_btn3DBai1);
	CheckNumBtn(m_btn3DBai2);
	UnCheckNumBtn(m_btn3DBai3);
	CheckNumBtn(m_btn3DBai4);
	UnCheckNumBtn(m_btn3DBai5);
	CheckNumBtn(m_btn3DBai6);
	UnCheckNumBtn(m_btn3DBai7);
	CheckNumBtn(m_btn3DBai8);
	UnCheckNumBtn(m_btn3DBai9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnBai3dQing()
{
	UnCheckNumBtn(m_btn3DBai0);
	UnCheckNumBtn(m_btn3DBai1);
	UnCheckNumBtn(m_btn3DBai2);
	UnCheckNumBtn(m_btn3DBai3);
	UnCheckNumBtn(m_btn3DBai4);
	UnCheckNumBtn(m_btn3DBai5);
	UnCheckNumBtn(m_btn3DBai6);
	UnCheckNumBtn(m_btn3DBai7);
	UnCheckNumBtn(m_btn3DBai8);
	UnCheckNumBtn(m_btn3DBai9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnShi3dQuan()
{
	CheckNumBtn(m_btn3DShi0);
	CheckNumBtn(m_btn3DShi1);
	CheckNumBtn(m_btn3DShi2);
	CheckNumBtn(m_btn3DShi3);
	CheckNumBtn(m_btn3DShi4);
	CheckNumBtn(m_btn3DShi5);
	CheckNumBtn(m_btn3DShi6);
	CheckNumBtn(m_btn3DShi7);
	CheckNumBtn(m_btn3DShi8);
	CheckNumBtn(m_btn3DShi9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnShi3dDa()
{
	UnCheckNumBtn(m_btn3DShi0);
	UnCheckNumBtn(m_btn3DShi1);
	UnCheckNumBtn(m_btn3DShi2);
	UnCheckNumBtn(m_btn3DShi3);
	UnCheckNumBtn(m_btn3DShi4);
	CheckNumBtn(m_btn3DShi5);
	CheckNumBtn(m_btn3DShi6);
	CheckNumBtn(m_btn3DShi7);
	CheckNumBtn(m_btn3DShi8);
	CheckNumBtn(m_btn3DShi9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnShi3dXiao()
{
	CheckNumBtn(m_btn3DShi0);
	CheckNumBtn(m_btn3DShi1);
	CheckNumBtn(m_btn3DShi2);
	CheckNumBtn(m_btn3DShi3);
	CheckNumBtn(m_btn3DShi4);
	UnCheckNumBtn(m_btn3DShi5);
	UnCheckNumBtn(m_btn3DShi6);
	UnCheckNumBtn(m_btn3DShi7);
	UnCheckNumBtn(m_btn3DShi8);
	UnCheckNumBtn(m_btn3DShi9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnShi3dDan()
{
	UnCheckNumBtn(m_btn3DShi0);
	CheckNumBtn(m_btn3DShi1);
	UnCheckNumBtn(m_btn3DShi2);
	CheckNumBtn(m_btn3DShi3);
	UnCheckNumBtn(m_btn3DShi4);
	CheckNumBtn(m_btn3DShi5);
	UnCheckNumBtn(m_btn3DShi6);
	CheckNumBtn(m_btn3DShi7);
	UnCheckNumBtn(m_btn3DShi8);
	CheckNumBtn(m_btn3DShi9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnShi3dShuang()
{
	CheckNumBtn(m_btn3DShi0);
	UnCheckNumBtn(m_btn3DShi1);
	CheckNumBtn(m_btn3DShi2);
	UnCheckNumBtn(m_btn3DShi3);
	CheckNumBtn(m_btn3DShi4);
	UnCheckNumBtn(m_btn3DShi5);
	CheckNumBtn(m_btn3DShi6);
	UnCheckNumBtn(m_btn3DShi7);
	CheckNumBtn(m_btn3DShi8);
	UnCheckNumBtn(m_btn3DShi9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnShi3dQing()
{
	UnCheckNumBtn(m_btn3DShi0);
	UnCheckNumBtn(m_btn3DShi1);
	UnCheckNumBtn(m_btn3DShi2);
	UnCheckNumBtn(m_btn3DShi3);
	UnCheckNumBtn(m_btn3DShi4);
	UnCheckNumBtn(m_btn3DShi5);
	UnCheckNumBtn(m_btn3DShi6);
	UnCheckNumBtn(m_btn3DShi7);
	UnCheckNumBtn(m_btn3DShi8);
	UnCheckNumBtn(m_btn3DShi9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnGe3dQuan()
{
	CheckNumBtn(m_btn3DGe0);
	CheckNumBtn(m_btn3DGe1);
	CheckNumBtn(m_btn3DGe2);
	CheckNumBtn(m_btn3DGe3);
	CheckNumBtn(m_btn3DGe4);
	CheckNumBtn(m_btn3DGe5);
	CheckNumBtn(m_btn3DGe6);
	CheckNumBtn(m_btn3DGe7);
	CheckNumBtn(m_btn3DGe8);
	CheckNumBtn(m_btn3DGe9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnGe3dDa()
{
	UnCheckNumBtn(m_btn3DGe0);
	UnCheckNumBtn(m_btn3DGe1);
	UnCheckNumBtn(m_btn3DGe2);
	UnCheckNumBtn(m_btn3DGe3);
	UnCheckNumBtn(m_btn3DGe4);
	CheckNumBtn(m_btn3DGe5);
	CheckNumBtn(m_btn3DGe6);
	CheckNumBtn(m_btn3DGe7);
	CheckNumBtn(m_btn3DGe8);
	CheckNumBtn(m_btn3DGe9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnGe3dXiao()
{
	CheckNumBtn(m_btn3DGe0);
	CheckNumBtn(m_btn3DGe1);
	CheckNumBtn(m_btn3DGe2);
	CheckNumBtn(m_btn3DGe3);
	CheckNumBtn(m_btn3DGe4);
	UnCheckNumBtn(m_btn3DGe5);
	UnCheckNumBtn(m_btn3DGe6);
	UnCheckNumBtn(m_btn3DGe7);
	UnCheckNumBtn(m_btn3DGe8);
	UnCheckNumBtn(m_btn3DGe9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnGe3dDan()
{
	UnCheckNumBtn(m_btn3DGe0);
	CheckNumBtn(m_btn3DGe1);
	UnCheckNumBtn(m_btn3DGe2);
	CheckNumBtn(m_btn3DGe3);
	UnCheckNumBtn(m_btn3DGe4);
	CheckNumBtn(m_btn3DGe5);
	UnCheckNumBtn(m_btn3DGe6);
	CheckNumBtn(m_btn3DGe7);
	UnCheckNumBtn(m_btn3DGe8);
	CheckNumBtn(m_btn3DGe9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnGe3dShuang()
{
	CheckNumBtn(m_btn3DGe0);
	UnCheckNumBtn(m_btn3DGe1);
	CheckNumBtn(m_btn3DGe2);
	UnCheckNumBtn(m_btn3DGe3);
	CheckNumBtn(m_btn3DGe4);
	UnCheckNumBtn(m_btn3DGe5);
	CheckNumBtn(m_btn3DGe6);
	UnCheckNumBtn(m_btn3DGe7);
	CheckNumBtn(m_btn3DGe8);
	UnCheckNumBtn(m_btn3DGe9);
	Get3dZhushu(CZ3D,m_gameKind);
}

void C3D::OnBnClickedBtnGe3dQing()
{
	UnCheckNumBtn(m_btn3DGe0);
	UnCheckNumBtn(m_btn3DGe1);
	UnCheckNumBtn(m_btn3DGe2);
	UnCheckNumBtn(m_btn3DGe3);
	UnCheckNumBtn(m_btn3DGe4);
	UnCheckNumBtn(m_btn3DGe5);
	UnCheckNumBtn(m_btn3DGe6);
	UnCheckNumBtn(m_btn3DGe7);
	UnCheckNumBtn(m_btn3DGe8);
	UnCheckNumBtn(m_btn3DGe9);
	Get3dZhushu(CZ3D,m_gameKind);

}

void C3D::OnBnClickedRioSxzhxFushi()
{
	ResetAllNums();

	m_gameKind = enWF_ZhixuanFushi;

	SetLockText();

	m_rioSxzhxFuShi.SetPushed(true);
	m_rioSxzhxDanShi.SetPushed(false);

	HideDanShiEdit();
	HideDaXiaoDanShuang();
	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();
}

void C3D::OnBnClickedRioSxzhxDanshi()
{
	ResetAllNums();

	m_gameKind = enWF_ZhixuanDanshi;

	SetLockText();

	m_rioSxzhxFuShi.SetPushed(false);
	m_rioSxzhxDanShi.SetPushed(true);

	ShowDanShiEdit();
	HideDaXiaoDanShuang();

	HideBaiNums();
	HideShiNums();
	HideGeNums();
}

void C3D::OnBnClickedRioHezhxFushi()
{
	ResetAllNums();

	m_gameKind = enWF_HouerzhixuanFushi;
	HideDaXiaoDanShuang();

	SetLockText();

	m_rioHeZhxFuShi.SetPushed(true);
	m_rioHeZhxDanShi.SetPushed(false);

	HideDanShiEdit();

	HideBaiNums();
	ShowShiNums();
	ShowGeNums();
}

void C3D::OnBnClickedRioHezhxDanshi()
{
	ResetAllNums();

	m_gameKind = enWF_HouerzhixuanDanshi;
	HideDaXiaoDanShuang();

	SetLockText();

	m_rioHeZhxFuShi.SetPushed(false);
	m_rioHeZhxDanShi.SetPushed(true);

	ShowDanShiEdit();

	HideBaiNums();
	HideShiNums();
	HideGeNums();
}

void C3D::OnBnClickedRioSxzsFushi()
{
	ResetAllNums();

	m_gameKind = enWF_QiansanzusanFushi;
	HideDaXiaoDanShuang();

	SetLockText();

	m_rioSxzsFuShi.SetPushed(true);
	m_rioSxzsDanShi.SetPushed(false);
	m_rioSxzlFuShi.SetPushed(false);
	m_rioSxzlDanShi.SetPushed(false);

	HideDanShiEdit();

	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();
}

void C3D::OnBnClickedRioSxzsDanshi()
{
	ResetAllNums();

	m_gameKind = enWF_QiansanzusanDanshi;
	HideDaXiaoDanShuang();

	SetLockText();

	m_rioSxzsFuShi.SetPushed(false);
	m_rioSxzsDanShi.SetPushed(true);
	m_rioSxzlFuShi.SetPushed(false);
	m_rioSxzlDanShi.SetPushed(false);

	ShowDanShiEdit();

	HideBaiNums();
	HideShiNums();
	HideGeNums();
}

void C3D::OnBnClickedRioSxzlFushi()
{
	ResetAllNums();

	m_gameKind = enWF_QiansanzuliuFushi;

	SetLockText();
	HideDaXiaoDanShuang();

	m_rioSxzsFuShi.SetPushed(false);
	m_rioSxzsDanShi.SetPushed(false);
	m_rioSxzlFuShi.SetPushed(true);
	m_rioSxzlDanShi.SetPushed(false);

	HideDanShiEdit();

	ShowBaiNums();
	ShowZuXuan();
	HideShiNums();
	HideGeNums();
}

void C3D::OnBnClickedRioSxzlDanshi()
{
	ResetAllNums();

	m_gameKind = enWF_QiansanzuliuDanshi;

	SetLockText();
	HideDaXiaoDanShuang();

	m_rioSxzsFuShi.SetPushed(false);
	m_rioSxzsDanShi.SetPushed(false);
	m_rioSxzlFuShi.SetPushed(false);
	m_rioSxzlDanShi.SetPushed(true);

	ShowDanShiEdit();

	HideBaiNums();
	HideShiNums();
	HideGeNums();
}

void C3D::OnBnClickedRioQyzhx()
{
	ResetAllNums();

	m_gameKind = enWF_QianyizhixuanFushi;

	SetLockText();
	HideDaXiaoDanShuang();


	m_rioQyZhx.SetPushed(true);

	HideDanShiEdit();

	ShowBaiNums();
	HideShiNums();
	HideGeNums();
}

void C3D::OnBnClickedRioQezhxFushi()
{
	ResetAllNums();

	m_gameKind = enWF_QianerzhixuanFushi;

	SetLockText();
	HideDaXiaoDanShuang();

	m_rioQeZhxFuShi.SetPushed(true);
	m_rioQeZhxDanShi.SetPushed(false);

	HideDanShiEdit();

	ShowBaiNums();
	ShowShiNums();
	HideGeNums();
}

void C3D::OnBnClickedRioQezhxDanshi()
{
	ResetAllNums();

	m_gameKind = enWF_QianerzhixuanDanshi;

	SetLockText();
	HideDaXiaoDanShuang();

	m_rioQeZhxFuShi.SetPushed(false);
	m_rioQeZhxDanShi.SetPushed(true);

	ShowDanShiEdit();

	HideBaiNums();
	HideShiNums();
	HideGeNums();
}

void C3D::OnBnClickedRioSxbdw()
{
	ResetAllNums();

	m_gameKind = enWF_Budingwei;
	HideDaXiaoDanShuang();

	SetLockText();

	m_rioSxbdw.SetPushed(true);

	HideDanShiEdit();

	ShowBaiNums();
	ShowBuDingWei();
	HideShiNums();
	HideGeNums();
}

void C3D::OnBnClickedRioHyzhx()
{
	ResetAllNums();

	m_gameKind = enWF_HouyizhixuanFushi;
	HideDaXiaoDanShuang();

	SetLockText();

	m_rioHyzhx.SetPushed(true);

	HideDanShiEdit();

	HideBaiNums();
	HideShiNums();
	ShowGeNums();
}
void C3D::OnBnClickedRioDwd()
{
	ResetAllNums();

	m_gameKind = enWF_DingWeiDan;
	HideDaXiaoDanShuang();

	SetLockText();

	m_rioDwd.SetPushed(true);

	HideDanShiEdit();

	ShowBaiNums();
	ShowShiNums();
	ShowGeNums();
}
//定位大小单双
void C3D::OnBnClickedRioDwDxds()
{
	ResetAllNums();

	m_gameKind = enWF_DaxiaoDanshuang;

	SetLockText();

	m_rioDwDxds.SetPushed(true);

	HideDanShiEdit();

	HideBaiNums();
	HideShiNums();
	HideGeNums();
	ShowDaXiaoDanShuang();
}

void C3D::OnBnClickedRioSxzhxHezhi()
{
	// 	ResetAllNums();
	// 
	// 	m_wanfa = SanXingZhiXuanHeZhi;
	// 
	// 	m_rioSxzhxHeZhi.SetPushed(true);
	// 
	// 	HideDanShiEdit();
	// 
	// 	HideBaiNums();
	// 	HideShiNums();
	// 	HideGeNums();
}

//服务器端返回前N个开奖号码
bool C3D::GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nIndex)
{
	_tcscpy_s(m_kjXinxi[nIndex].qihao, KJ_QIHAO_LENGTH, pResult->wPeriod);
	_tcscpy_s(m_kjXinxi[nIndex].haoma, KJ_HAOMA_LENGTH, pResult->szLotNum);
	_tcscpy_s(m_kjXinxi[nIndex].shijian, KJ_SHIJIAN_LENGTH, pResult->szShijian);

	if(nIndex+1 < TOP_KAIJIANG_NUMBERS)
	{
		if(nIndex>1)
			return true;
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

void C3D::OnBnClickedBtn3dAdd()
{
	CString strBai = GetBaiString();
	CString strShi = GetShiString();
	CString strGe = GetGeString();

	CString strHaoMa; 
	int zhushu = 0;
	m_singlezhushu = 0;
	if(enWF_ZhixuanFushi == m_gameKind) //三星直选复式
	{
		if(strBai.IsEmpty() || strShi.IsEmpty() || strGe.IsEmpty()) 
		{
			MyMessageBox(_T("请选择合适的号码"));
			return;
		}

		strHaoMa = strBai + _T(",") + strShi + _T(",") + strGe;

		zhushu = strBai.GetLength() * strShi.GetLength() * strGe.GetLength();
		if(zhushu > 800)
		{
			MyMessageBox(_T("你选择的号码超过限制注数800注，请重新输入！"));
			return;
		}
	}
	else if (enWF_ZhixuanDanshi == m_gameKind) //三星直选dan式
	{
		if (!GetDanShiHaoma(strHaoMa, 3, zhushu)) 
		{
			MyMessageBox(_T("请填写合适的号码"));
			return;
		}
		if(zhushu > 800)
		{
			MyMessageBox(_T("你选择的号码超过限制注数800注，请重新输入！"));
			return;
		}
	}
	else if (enWF_DingWeiDan == m_gameKind) //定位胆
	{
		if(strBai.IsEmpty() && strShi.IsEmpty() && strGe.IsEmpty()) 
		{
			MyMessageBox(_T("请选择合适的号码"));
			return;
		}

		strHaoMa = strBai + _T(",") +strShi + _T(",") + strGe;

		zhushu = strBai.GetLength() + strShi.GetLength() + strGe.GetLength();
		if(zhushu > 24)
		{
			MyMessageBox(_T("你选择的号码超过限制注数24注，请重新输入！"));
			return;
		}
	}
	else if (enWF_QiansanzusanFushi == m_gameKind) //三星组三复式
	{
		if (strBai.GetLength() < 2) 
		{
			MyMessageBox(_T("请填写合适的号码"));
			return;
		}

		strHaoMa = strBai;

		zhushu = strBai.GetLength() * (strBai.GetLength() - 1);
	}
	else if (enWF_QiansanzusanDanshi == m_gameKind) //三星组三单式
	{
		if (!GetZuSanHaoma(strHaoMa, zhushu)) 
		{
			MyMessageBox(_T("请选择合适的号码"));
			return;
		}
	}
	else if (enWF_QiansanzuliuFushi == m_gameKind) //三星组六复式
	{
		if (strBai.GetLength() < 3) 
		{
			MyMessageBox(_T("请填写合适的号码"));
			return;
		}

		strHaoMa = strBai;

		zhushu = strBai.GetLength() * (strBai.GetLength() - 1) * (strBai.GetLength() - 2) / 6;
	}
	else if (enWF_QiansanzuliuDanshi == m_gameKind) //三星组六单式
	{
		if (!GetDanShiHaoma(strHaoMa, 3, zhushu)) 
		{
			MyMessageBox(_T("请选择合适的号码"));
			return;
		}
	}
	else if (enWF_QianyizhixuanFushi == m_gameKind) //前一直选
	{
		if (strBai.IsEmpty()) 
		{
			MyMessageBox(_T("请填写合适的号码"));
			return;
		}

		strHaoMa = strBai;
		zhushu = strBai.GetLength();
		if(zhushu > 8)
		{
			MyMessageBox(_T("你选择的号码超过限制注数8注，请重新输入！"));
			return;
		}
	}
	else if (enWF_QianerzhixuanFushi == m_gameKind) //前二直选复式
	{
		if(strBai.IsEmpty() || strShi.IsEmpty()) 
		{
			MyMessageBox(_T("请选择合适的号码"));
			return;
		}

		strHaoMa = strBai + _T(",") + strShi;

		zhushu = strBai.GetLength() * strShi.GetLength();
		if(zhushu > 80)
		{
			MyMessageBox(_T("你选择的号码超过限制注数80注，请重新输入！"));
			return;
		}

	}
	else if (enWF_QianerzhixuanDanshi == m_gameKind) //前二直选单式
	{
		if (!GetDanShiHaoma(strHaoMa, 2, zhushu)) 
		{
			MyMessageBox(_T("请选择合适的号码"));
			return;
		}
		if(zhushu > 80)
		{
			MyMessageBox(_T("你选择的号码超过限制注数80注，请重新输入！"));
			return;
		}
	}
	else if (enWF_Budingwei == m_gameKind) //三星不定位
	{
		if(strBai.IsEmpty()) 
		{
			MyMessageBox(_T("请选择合适的号码"));
			return;
		}

		int nLength = strBai.GetLength();
		for (int i = 0;i < nLength;i++)
		{
			strHaoMa+=strBai.GetAt(i);
			if(i!=nLength-1)
			{
				strHaoMa+=_T(",");
			}
		}


		zhushu = nLength;
	}
	else if (enWF_HouyizhixuanFushi == m_gameKind) //后一直选
	{
		if (strGe.IsEmpty()) 
		{
			MyMessageBox(_T("请填写合适的号码"));
			return;
		}

		strHaoMa = strGe;

		zhushu = strGe.GetLength();
		if(zhushu > 8)
		{
			MyMessageBox(_T("你选择的号码超过限制注数8注，请重新输入！"));
			return;
		}
	}
	else if (enWF_HouerzhixuanFushi == m_gameKind) //后二直选复式
	{
		if(strShi.IsEmpty() || strGe.IsEmpty()) 
		{
			MyMessageBox(_T("请选择合适的号码"));
			return;
		}

		strHaoMa = strShi + _T(",") + strGe;

		zhushu = strShi.GetLength() * strGe.GetLength();
		if(zhushu > 80)
		{
			MyMessageBox(_T("你选择的号码超过限制注数80注，请重新输入！"));
			return;
		}
	}
	else if (enWF_HouerzhixuanDanshi == m_gameKind) //后二直选单式
	{
		if (!GetDanShiHaoma(strHaoMa, 2, zhushu)) 
		{
			MyMessageBox(_T("请选择合适的号码"));
			return;
		}
		if(zhushu > 80)
		{
			MyMessageBox(_T("你选择的号码超过限制注数80注，请重新输入！"));
			return;
		}
	}
	else if(enWF_DaxiaoDanshuang == m_gameKind)
	{
		CString strBaiDXDS = GetBaiDxdshString();
		CString strShiDXDS = GetShiDxdshString();
		CString strGeDXDS = GetGeDxdshString();

		int nBaiLength = strBaiDXDS.GetLength();
		int nShiLength = strShiDXDS.GetLength();
		int nGeLength = strGeDXDS.GetLength();
		if(strBaiDXDS.IsEmpty()&& strShiDXDS.IsEmpty() && strGeDXDS.IsEmpty())
		{
			MyMessageBox(_T("请选择合适的号码"));
			return;
		}

		if(nBaiLength>1||nShiLength>1||nGeLength>1)
		{
			MyMessageBox(_T("大小单双每位最多只能选择一个号码"));
			return;
		}

		strHaoMa = strBaiDXDS + _T(",") + strShiDXDS + _T(",") + strGeDXDS;

		zhushu = nBaiLength + nShiLength + nGeLength;
	}
	// 	else if (SanXingZhiXuanHeZhi) //三星之选和值
	// 	{
	// 
	// 	}

	UpdateData();

	wstring strdata;
	strdata = strHaoMa.GetBuffer(0);
	//vecAllNums.insert(vecAllNums.begin(),1,strdata);
	vecAllNums.insert(vecAllNums.begin(),1,strdata);

	int nCount = m_list3D.GetItemCount();
	if(enWF_DaxiaoDanshuang == m_gameKind)
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

		m_list3D.InsertItem(nCount, strAddHaoma);

	}
	else
		m_list3D.InsertItem(nCount, strHaoMa);


	m_list3D.SetItemText(nCount, 1, GetScoreMoshi());
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
	m_list3D.SetItemText(nCount, 2, strZongzhu);

	m_list3D.SetItemText(nCount, 3, GetWanDesc());
	CString strTmp;
	strTmp.Format(_T("%d"), zhushu);
	m_list3D.SetItemText(nCount, 4, strTmp);
	strTmp.Format(_T("%d"), m_beishu);
	m_list3D.SetItemText(nCount, 5, strTmp);
	m_list3D.createItemButton(nCount, 6, this->GetSafeHwnd());

	//strTmp.Format(_T("%d"), m_beishu);
	//m_list3D.SetItemText(0, 3, strTmp);
	//strTmp.Format(_T("%d"), 2 * zhushu * m_beishu);
	//m_list3D.SetItemText(0, 4, strTmp);

	m_zongZhuShu += zhushu;
	//m_zongJinE += (2 * zhushu * m_beishu);

	int nItemCount = m_list3D.GetItemCount();
	m_zongJine = 0.00f;
	for(int i = 0;i < nItemCount;i++)
	{
		CString strBeiSHu = m_list3D.GetItemText(i,5);
		CString strZhuSHu = m_list3D.GetItemText(i,4);
		CString strMoshi = m_list3D.GetItemText(i,1);

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

	ResetAllNums();
//	m_DlgStatus.HideStatusWindow();
}
CString C3D::GetScoreMoshi()
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
int C3D::GetMoshiRet(CString moshi)
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

void C3D::OnBnClickedBtnDelSel()
{
	POSITION pos = m_list3D.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_list3D.GetNextSelectedItem(pos);

		int zhushu = _ttoi(m_list3D.GetItemText(nItem, 4));
		m_zongZhuShu -= zhushu;
		CRect rcInvali;
		rcInvali.CopyRect(rcZongZhuShu);
		rcInvali.right+=400;
		InvalidateRect(&rcInvali);
		InvalidateRect(&rcZongJinE);
		if(vecAllNums.size() > 0)
		{
			vector<wstring>::iterator   iter = vecAllNums.begin()+nItem;
			vecAllNums.erase(iter);

		}

		m_list3D.DeleteItem(nItem);
	}
}

void C3D::OnBnClickedBtnClsList()
{
	m_richDanshiHaoma.SetWindowText(L"");

	m_zongZhuShu = 0;
	m_singlezhushu = 0;
	m_zongJine = 0;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
	rcInvali.CopyRect(rcAllZongzhushu);
	rcInvali.bottom += 20;
	InvalidateRect(&rcInvali);
	ResetAllNums();

	m_list3D.deleteAllItemEx();
}

void C3D::OnBnClickedBtnTouzhu()
{

	if (_tcslen(m_lastExpect) == 0)
	{
		MyMessageBox(_T("正在获取数据……"));
		return;
	}

	CString strHao = m_list3D.GetItemText(0,0);
	if(strHao.IsEmpty())
	{
		OnBnClickedBtn3dAdd();
	}


	CString str;
	m_editBeiShu.GetWindowText(str);
	if (str.IsEmpty())
	{
		MyMessageBox(_T("投注倍数不可为空"));
		return;
	}

	int itm_cnt = m_list3D.GetItemCount();
	if (itm_cnt == 0 && vecAllNums.size()==0) 
	{
		MyMessageBox(_T("您并未选择号码"));
		return;
	}
	//判断余额是否充足
	if(theAccount.yue < m_zongJine)
	{
		MyMessageBox(_T("您余额不足，请充值后再试"));
		return;
	}

	//请用户确认信息
	CQrTzhDlg qrDlg;
	qrDlg.m_gamekind = _T("3D彩");
	qrDlg.m_zhushu = m_zongZhuShu * m_beishu;
	qrDlg.m_moshi = theAccount.Scoretype;
	qrDlg.m_zongjine = m_zongJine;
	if (qrDlg.DoModal() != IDOK)
	{
		return;
	}

	//判断是否临近开奖点
	long kjing = GetKjShjDiff(); 
	if (kjing < 0L)
	{
		MyMessageBox(_T("此期封单,请稍后再试"));
		return;
	}

	bool bSucc = true;

	SendToServer(1);
	OnBnClickedBtnClsList();

	m_zongZhuShu = 0;
	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	m_beishu = 1;
	UpdateData(FALSE);

	m_list3D.DeleteAllItems();

}

void C3D::OnBnClickedBtnZhuihao()
{
	int itm_cnt = m_list3D.GetItemCount();
	if (itm_cnt<=0)
	{
		MyMessageBox(L"请添加投注号码！");
		return;

	}

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
	else if(theAccount.Scoretype == MoShi_Li)
		danzhujine = 0.002;

	CString strBeishu ;
	strBeishu.Format(L"%d",m_beishu);

	bool bRunNian = FALSE;
	CTime t;
	theApp.GetTime(t);
	if(t.GetYear()%4 == 0 && t.GetYear()%100!=0)
	{
		bRunNian = true;
	}
	else if(t.GetYear()%100 == 0 && t.GetYear()%400 == 0)
	{
		bRunNian = true;
	}

	int nMostQishu = 0;
	if(bRunNian)
	{
		nMostQishu = 359;
	}
	else
	{
		nMostQishu = 358;
	}
	if(m_bKaiJiangzhong)
		dlgZhuiHao.SetCurrentQihaoTime(GetNextExpect(), strBeishu,4,3,m_zongJine,nMostQishu,CZ3D);
	else
		dlgZhuiHao.SetCurrentQihaoTime( m_kjXinxi[0].qihao, strBeishu,4,3,m_zongJine,nMostQishu,CZ3D);
	if(dlgZhuiHao.DoModal() == IDOK)
	{
		//判断是否临近开奖点
		long kjing = GetKjShjDiff(); 
		if (kjing < 0L)
		{
			MyMessageBox(_T("此期封单,请稍后再试"));
			return;
		}

		m_nZhuihaoCount = dlgZhuiHao.GetZhuiHaoQiShu();
		dlgZhuiHao.GetZhuiHaoData(vecData1, vecData2, vecData3);


		m_bZhuiZjtz = dlgZhuiHao.GetPrizeStop();

		DOUBLE fZongJine = 0.0;
		for(int i = 0;i < m_nZhuihaoCount;i++)
		{
			int nBeishu = _ttoi(vecData1[i].c_str());
			fZongJine += m_zongZhuShu*nBeishu*danzhujine;
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

	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=400;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);
	OnBnClickedBtnClsList();

}

void C3D::OnEnChangeEditDanshiInput()
{
//	m_editDanShiHaoMa.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
	m_richDanshiHaoma.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );

}

void C3D::OnEnChangeEditBeishu()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString str;
	m_editBeiShu.GetWindowText(str);
	if (!CBmpUtil::IsNumber(str) || (_ttoi(str) < 1)) 
	{
		if (!str.IsEmpty())
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

BOOL C3D::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\game_bk3.png"));
	m_numImageList = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\num.png"));

	m_bmpNumTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\digit.png"));

	m_kjNumBig = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_big.png"));
	m_bigNumWidth = m_kjNumBig->GetWidth() / 10;
	m_bigNumHeight = m_kjNumBig->GetHeight();
	m_kjNumSmall = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small.png"));
	m_smallNumWidth = m_kjNumSmall->GetWidth() / 10;
	m_smallNumHeight = m_kjNumSmall->GetHeight();

	m_imgKaijiang = new Bitmap( CBmpUtil::GetExePath() + _T("skin\\game\\KaiJiang1.gif"));
	int nCount = m_imgKaijiang->GetFrameDimensionsCount();			//获取帧维数
	GUID *pGuids = new GUID[nCount];								//定义一个GUID数组
	m_imgKaijiang->GetFrameDimensionsList(pGuids,nCount);			//获取图像帧的GUID
	m_nFrameCount=m_imgKaijiang->GetFrameCount(pGuids);				//获取GIF帧数
	
	InitWanFaBtns();
	InitNumberBtns();
	InitListCtrl();

	m_bShowFail = true;
	ZeroMemory(m_szMD5Haoma,sizeof(m_szMD5Haoma));
	m_smallNumfont.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_zongFont.CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_KjInfoFont1.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_KjInfoFont.CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_btnYuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_1.png"));
	m_btnJiao.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_2.png"));
	m_btnFen.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_3.png"));
	m_btnLi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\BT_USER_INFO_4.png"));

	m_btnDelSel.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_delate.png"));
	m_btnClsList.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_clear.png"));
	m_btnTouZhu.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_bet.png"));
	m_btnZhuiHao.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_append.png"));
	m_btn3DAdd.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_addnum.png"));
	m_btnLock.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_lock.png"));
	m_btnMoreRecord.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_more.png"));
	m_DlgOpenRecord.Create(COpenRecord::IDD,this);

	m_btnLock.SetTextColor(RGB(51, 45, 42));
	m_btnLock.SetTextFont(&m_zongFont);
	m_richDanshiHaoma.LimitText(-1);
	m_richDanshiHaoma.SetBackgroundColor(false,RGB(200,200,200));
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(88, 78, 77);
	m_richDanshiHaoma.SetDefaultCharFormat(cf);
	m_editBeiShu.SetEnableColor(RGB(0,0,0),RGB(250,243,227),RGB(40,29,27));
	m_editBeiShu.SetFont(&m_zongFont);
	m_btnBeiTouJia.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jia.png"));
	m_btnBeiTouJian.SetImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_jian.png"));
	m_richDanshiHaoma.SetEventMask(ENM_CHANGE);

	LuckyNumCQSSC();
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.SetMaxTipWidth(150);
	m_btWxzxTip.SetImage(CBmpUtil::GetExePath() + _T("skin\\bangzhu.png"));

	SetTimer(timer_id_get_luck_num, 10000, NULL);
	SetTimer(timer_id_kj_yugao, 1000, NULL);

	return TRUE;  
}

void C3D::OnCancel()
{
	//CDialog::OnCancel();
}

void C3D::OnOK()
{
	//CDialog::OnOK();
}

void C3D::CheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(true);
}

void C3D::UnCheckNumBtn(CNumerButton& btn)
{
	btn.SetSelect(false);
}

bool C3D::IsNumBtnCheck(CNumerButton& btn)
{
	return btn.IsSelect();
}

void C3D::HideCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}

void C3D::ShowCtrl(int ctrl_id)
{
	CWnd* pWnd = GetDlgItem(ctrl_id);
	if (pWnd != NULL) 
	{
		pWnd->ShowWindow(SW_SHOW);
	}
}

void C3D::HideAllRio()
{
	HideCtrl(IDC_RIO_SXZHX_FUSHI);
	HideCtrl(IDC_RIO_SXZHX_DANSHI);

	HideCtrl(IDC_RIO_SXZS_FUSHI);
	HideCtrl(IDC_RIO_SXZS_DANSHI);
	HideCtrl(IDC_RIO_SXZL_FUSHI);
	HideCtrl(IDC_RIO_SXZL_DANSHI);

	HideCtrl(IDC_RIO_QYZHX);

	HideCtrl(IDC_RIO_QEZHX_FUSHI);
	HideCtrl(IDC_RIO_QEZHX_DANSHI);

	HideCtrl(IDC_RIO_SXBDW);

	HideCtrl(IDC_RIO_HYZHX);
	HideCtrl(IDC_RIO_DWD);
	HideCtrl(IDC_RIO_DWDXDS);

	HideCtrl(IDC_RIO_HEZHX_FUSHI);
	HideCtrl(IDC_RIO_HEZHX_DANSHI);

	HideCtrl(IDC_RIO_SXZHX_HEZHI);
}

void C3D::AdjustWanFa()
{
	if (m_btnSanXingZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnSanXingZhiXuan.SetWindowPos(NULL, wanfa_x, wanfa_y, m_btnSanXingZhiXuan.Width(), m_btnSanXingZhiXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnSanXingZuXuan.GetSafeHwnd() != NULL) 
	{
		m_btnSanXingZuXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width, wanfa_y, m_btnSanXingZuXuan.Width(), m_btnSanXingZuXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianYiZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianYiZhiXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*2, wanfa_y, m_btnQianYiZhiXuan.Width(), m_btnQianYiZhiXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnQianErZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnQianErZhiXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*3, wanfa_y, m_btnQianErZhiXuan.Width(), m_btnQianErZhiXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnSanXingBuDingWei.GetSafeHwnd() != NULL) 
	{
		m_btnSanXingBuDingWei.SetWindowPos(NULL, wanfa_x + wanfa_col_width*4, wanfa_y, m_btnSanXingBuDingWei.Width(), m_btnSanXingBuDingWei.Height(), SWP_NOZORDER);
	}

	if (m_btnHouYiZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouYiZhiXuan.SetWindowPos(NULL, wanfa_x + wanfa_col_width*5+11, wanfa_y, m_btnHouYiZhiXuan.Width(), m_btnHouYiZhiXuan.Height(), SWP_NOZORDER);
	}

	if (m_btnHouErZhiXuan.GetSafeHwnd() != NULL) 
	{
		m_btnHouErZhiXuan.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*6+11, wanfa_y, m_btnHouErZhiXuan.Width(), m_btnHouErZhiXuan.Height(), SWP_NOZORDER);
	}
	if (m_btnDingWeiDan.GetSafeHwnd() != NULL) 
	{
		m_btnDingWeiDan.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*7+11, wanfa_y, m_btnDingWeiDan.Width(), m_btnDingWeiDan.Height(), SWP_NOZORDER);
	}
	if (m_btnDaXiaoDanShuang.GetSafeHwnd() != NULL) 
	{
		m_btnDaXiaoDanShuang.SetWindowPos(NULL, wanfa_x+ wanfa_col_width*8+1, wanfa_y, m_btnDaXiaoDanShuang.Width(), m_btnDaXiaoDanShuang.Height(), SWP_NOZORDER);
	}

	if (m_btnZhiXuanHeZhi.GetSafeHwnd() != NULL) 
	{
		m_btnZhiXuanHeZhi.SetWindowPos(NULL, wanfa_x + wanfa_col_width, wanfa_y+ wanfa_row_height, m_btnZhiXuanHeZhi.Width(), m_btnZhiXuanHeZhi.Height(), SWP_NOZORDER);
	}
}

void C3D::AdjustRadio()
{
	//-----------------------------------
	CWnd* pWnd = GetDlgItem(IDC_RIO_SXZHX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_SXZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_SXZS_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_SXZS_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_SXZL_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*2, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_SXZL_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width*3, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_QYZHX);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_QEZHX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_QEZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_SXBDW);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width+31, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_HYZHX);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_DWD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_DWDXDS);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_HEZHX_FUSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_RIO_HEZHX_DANSHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x + wanfa_sel_width, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}

	//-----------------------------------
	pWnd = GetDlgItem(IDC_RIO_SXZHX_HEZHI);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, wanfa_sel_x, wanfa_sel_y, wanfa_sel_width, wanfa_sel_height, SWP_NOZORDER);
	}
}


void C3D::AdjustNumBtn()
{
	//百位-------------------------------
	if (m_staticZuXuan.GetSafeHwnd() != NULL) 
	{
		m_staticZuXuan.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*2, m_staticBai.Width(), m_staticBai.Height(), SWP_NOZORDER);
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

	if(m_btn3DBai0.GetSafeHwnd() != NULL) 
	{
		m_btn3DBai0.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*2,  m_btn3DBai0.Width(), m_btn3DBai0.Height(), SWP_NOZORDER);
	}
	if(m_btn3DBai1.GetSafeHwnd() != NULL) 
	{
		m_btn3DBai1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*2,   m_btn3DBai1.Width(), m_btn3DBai1.Height(), SWP_NOZORDER);
	}
	if(m_btn3DBai2.GetSafeHwnd() != NULL) 
	{
		m_btn3DBai2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*2,   m_btn3DBai2.Width(), m_btn3DBai2.Height(), SWP_NOZORDER);
	}
	if(m_btn3DBai3.GetSafeHwnd() != NULL) 
	{
		m_btn3DBai3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*2,   m_btn3DBai3.Width(), m_btn3DBai3.Height(), SWP_NOZORDER);
	}
	if(m_btn3DBai4.GetSafeHwnd() != NULL) 
	{
		m_btn3DBai4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height*2,   m_btn3DBai4.Width(), m_btn3DBai4.Height(), SWP_NOZORDER);
	}
	if(m_btn3DBai5.GetSafeHwnd() != NULL) 
	{
		m_btn3DBai5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*2,   m_btn3DBai5.Width(), m_btn3DBai5.Height(), SWP_NOZORDER);
	}
	if(m_btn3DBai6.GetSafeHwnd() != NULL) 
	{
		m_btn3DBai6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*2,   m_btn3DBai6.Width(), m_btn3DBai6.Height(), SWP_NOZORDER);
	}
	if(m_btn3DBai7.GetSafeHwnd() != NULL) 
	{
		m_btn3DBai7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height*2,   m_btn3DBai7.Width(), m_btn3DBai7.Height(), SWP_NOZORDER);
	}
	if(m_btn3DBai8.GetSafeHwnd() != NULL) 
	{
		m_btn3DBai8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height*2,   m_btn3DBai8.Width(), m_btn3DBai8.Height(), SWP_NOZORDER);
	}
	if(m_btn3DBai9.GetSafeHwnd() != NULL) 
	{
		m_btn3DBai9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height*2,   m_btn3DBai9.Width(), m_btn3DBai9.Height(), SWP_NOZORDER);
	}

	if (m_btn3DBaiQuan.GetSafeHwnd() != NULL) 
	{
		m_btn3DBaiQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height*2,  m_btn3DBaiQuan.Width(), m_btn3DBaiQuan.Height(), SWP_NOZORDER);
	}
	if (m_btn3DBaiDa.GetSafeHwnd() != NULL) 
	{
		m_btn3DBaiDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y + haoma_btn_row_height*2,  m_btn3DBaiDa.Width(), m_btn3DBaiDa.Height(), SWP_NOZORDER);
	}
	if (m_btn3DBaiXiao.GetSafeHwnd() != NULL) 
	{
		m_btn3DBaiXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y + haoma_btn_row_height*2,  m_btn3DBaiXiao.Width(), m_btn3DBaiXiao.Height(), SWP_NOZORDER);
	}
	if (m_btn3DBaiDan.GetSafeHwnd() != NULL) 
	{
		m_btn3DBaiDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y + haoma_btn_row_height*2,  m_btn3DBaiDan.Width(), m_btn3DBaiDan.Height(), SWP_NOZORDER);
	}
	if (m_btn3DBaiShuang.GetSafeHwnd() != NULL) 
	{
		m_btn3DBaiShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14, haoma_btn_y + haoma_btn_row_height*2,  m_btn3DBaiShuang.Width(), m_btn3DBaiShuang.Height(), SWP_NOZORDER);
	}
	if (m_btn3DBaiQing.GetSafeHwnd() != NULL) 
	{
		m_btn3DBaiQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15, haoma_btn_y + haoma_btn_row_height*2,  m_btn3DBaiQing.Width(), m_btn3DBaiQing.Height(), SWP_NOZORDER);
	}

	//十位-------------------------------
	if (m_staticShi.GetSafeHwnd() != NULL) 
	{
		m_staticShi.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*3, m_staticShi.Width(), m_staticShi.Height(), SWP_NOZORDER);
	}

	if(m_btn3DShi0.GetSafeHwnd() != NULL) 
	{
		m_btn3DShi0.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShi0.Width(), m_btn3DShi0.Height(), SWP_NOZORDER);
	}
	if(m_btn3DShi1.GetSafeHwnd() != NULL) 
	{
		m_btn3DShi1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShi1.Width(), m_btn3DShi1.Height(), SWP_NOZORDER);
	}
	if(m_btn3DShi2.GetSafeHwnd() != NULL) 
	{
		m_btn3DShi2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShi2.Width(), m_btn3DShi2.Height(), SWP_NOZORDER);
	}
	if(m_btn3DShi3.GetSafeHwnd() != NULL) 
	{
		m_btn3DShi3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShi3.Width(), m_btn3DShi3.Height(), SWP_NOZORDER);
	}
	if(m_btn3DShi4.GetSafeHwnd() != NULL) 
	{
		m_btn3DShi4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShi4.Width(), m_btn3DShi4.Height(), SWP_NOZORDER);
	}
	if(m_btn3DShi5.GetSafeHwnd() != NULL) 
	{
		m_btn3DShi5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShi5.Width(), m_btn3DShi5.Height(), SWP_NOZORDER);
	}
	if(m_btn3DShi6.GetSafeHwnd() != NULL) 
	{
		m_btn3DShi6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShi6.Width(), m_btn3DShi6.Height(), SWP_NOZORDER);
	}
	if(m_btn3DShi7.GetSafeHwnd() != NULL) 
	{
		m_btn3DShi7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShi7.Width(), m_btn3DShi7.Height(), SWP_NOZORDER);
	}
	if(m_btn3DShi8.GetSafeHwnd() != NULL) 
	{
		m_btn3DShi8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShi8.Width(), m_btn3DShi8.Height(), SWP_NOZORDER);
	}
	if(m_btn3DShi9.GetSafeHwnd() != NULL) 
	{
		m_btn3DShi9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShi9.Width(), m_btn3DShi9.Height(), SWP_NOZORDER);
	}

	if (m_btn3DShiQuan.GetSafeHwnd() != NULL) 
	{
		m_btn3DShiQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShiQuan.Width(), m_btn3DShiQuan.Height(), SWP_NOZORDER);
	}
	if (m_btn3DShiDa.GetSafeHwnd() != NULL) 
	{
		m_btn3DShiDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShiDa.Width(), m_btn3DShiDa.Height(), SWP_NOZORDER);
	}
	if (m_btn3DShiXiao.GetSafeHwnd() != NULL) 
	{
		m_btn3DShiXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShiXiao.Width(), m_btn3DShiXiao.Height(), SWP_NOZORDER);
	}
	if (m_btn3DShiDan.GetSafeHwnd() != NULL) 
	{
		m_btn3DShiDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShiDan.Width(), m_btn3DShiDan.Height(), SWP_NOZORDER);
	}
	if (m_btn3DShiShuang.GetSafeHwnd() != NULL) 
	{
		m_btn3DShiShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShiShuang.Width(), m_btn3DShiShuang.Height(), SWP_NOZORDER);
	}
	if (m_btn3DShiQing.GetSafeHwnd() != NULL) 
	{
		m_btn3DShiQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15, haoma_btn_y + haoma_btn_row_height*3,  m_btn3DShiQing.Width(), m_btn3DShiQing.Height(), SWP_NOZORDER);
	}

	//个位-------------------------------
	if (m_staticGe.GetSafeHwnd() != NULL) 
	{
		m_staticGe.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y + haoma_btn_row_height*4, m_staticGe.Width(), m_staticGe.Height(), SWP_NOZORDER);
	}

	if(m_btn3DGe0.GetSafeHwnd() != NULL) 
	{
		m_btn3DGe0.SetWindowPos(NULL, haoma_btn_x, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGe0.Width(), m_btn3DGe0.Height(), SWP_NOZORDER);
	}
	if(m_btn3DGe1.GetSafeHwnd() != NULL) 
	{
		m_btn3DGe1.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGe1.Width(), m_btn3DGe1.Height(), SWP_NOZORDER);
	}
	if(m_btn3DGe2.GetSafeHwnd() != NULL) 
	{
		m_btn3DGe2.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGe2.Width(), m_btn3DGe2.Height(), SWP_NOZORDER);
	}
	if(m_btn3DGe3.GetSafeHwnd() != NULL) 
	{
		m_btn3DGe3.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGe3.Width(), m_btn3DGe3.Height(), SWP_NOZORDER);
	}
	if(m_btn3DGe4.GetSafeHwnd() != NULL) 
	{
		m_btn3DGe4.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*4, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGe4.Width(), m_btn3DGe4.Height(), SWP_NOZORDER);
	}
	if(m_btn3DGe5.GetSafeHwnd() != NULL) 
	{
		m_btn3DGe5.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*5, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGe5.Width(), m_btn3DGe5.Height(), SWP_NOZORDER);
	}
	if(m_btn3DGe6.GetSafeHwnd() != NULL) 
	{
		m_btn3DGe6.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*6, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGe6.Width(), m_btn3DGe6.Height(), SWP_NOZORDER);
	}
	if(m_btn3DGe7.GetSafeHwnd() != NULL) 
	{
		m_btn3DGe7.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*7, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGe7.Width(), m_btn3DGe7.Height(), SWP_NOZORDER);
	}
	if(m_btn3DGe8.GetSafeHwnd() != NULL) 
	{
		m_btn3DGe8.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*8, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGe8.Width(), m_btn3DGe8.Height(), SWP_NOZORDER);
	}
	if(m_btn3DGe9.GetSafeHwnd() != NULL) 
	{
		m_btn3DGe9.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*9, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGe9.Width(), m_btn3DGe9.Height(), SWP_NOZORDER);
	}

	if (m_btn3DGeQuan.GetSafeHwnd() != NULL) 
	{
		m_btn3DGeQuan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*10, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGeQuan.Width(), m_btn3DGeQuan.Height(), SWP_NOZORDER);
	}
	if (m_btn3DGeDa.GetSafeHwnd() != NULL) 
	{
		m_btn3DGeDa.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*11, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGeDa.Width(), m_btn3DGeDa.Height(), SWP_NOZORDER);
	}
	if (m_btn3DGeXiao.GetSafeHwnd() != NULL) 
	{
		m_btn3DGeXiao.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*12, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGeXiao.Width(), m_btn3DGeXiao.Height(), SWP_NOZORDER);
	}
	if (m_btn3DGeDan.GetSafeHwnd() != NULL) 
	{
		m_btn3DGeDan.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*13, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGeDan.Width(), m_btn3DGeDan.Height(), SWP_NOZORDER);
	}
	if (m_btn3DGeShuang.GetSafeHwnd() != NULL) 
	{
		m_btn3DGeShuang.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*14, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGeShuang.Width(), m_btn3DGeShuang.Height(), SWP_NOZORDER);
	}
	if (m_btn3DGeQing.GetSafeHwnd() != NULL) 
	{
		m_btn3DGeQing.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*15, haoma_btn_y + haoma_btn_row_height*4,  m_btn3DGeQing.Width(), m_btn3DGeQing.Height(), SWP_NOZORDER);
	}

	//大小单双  -- 百
	if(m_btnDaBai.GetSafeHwnd() != NULL) 
	{
		m_btnDaBai.SetWindowPos(NULL, haoma_btn_x+20, haoma_btn_y + haoma_btn_row_height*2, m_btnDaBai.Width(), m_btnDaBai.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaoBai.GetSafeHwnd() != NULL) 
	{
		m_btnXiaoBai.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith+20, haoma_btn_y + haoma_btn_row_height*2, m_btnXiaoBai.Width(), m_btnXiaoBai.Height(), SWP_NOZORDER);
	}
	if(m_btnDanBai.GetSafeHwnd() != NULL) 
	{
		m_btnDanBai.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*2+20, haoma_btn_y + haoma_btn_row_height*2, m_btnDanBai.Width(), m_btnDanBai.Height(), SWP_NOZORDER);
	}
	if(m_btnShuangBai.GetSafeHwnd() != NULL) 
	{
		m_btnShuangBai.SetWindowPos(NULL, haoma_btn_x + haoma_btn_col_wdith*3+20, haoma_btn_y + haoma_btn_row_height*2, m_btnShuangBai.Width(), m_btnShuangBai.Height(), SWP_NOZORDER);
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

}

void C3D::ShowDaXiaoDanShuang()
{
	m_staticBai.ShowWindow(SW_SHOW);
	m_staticShi.ShowWindow(SW_SHOW);
	m_staticGe.ShowWindow(SW_SHOW);

	m_btnDaBai.ShowWindow(SW_SHOW);
	m_btnXiaoBai.ShowWindow(SW_SHOW);
	m_btnDanBai.ShowWindow(SW_SHOW);
	m_btnShuangBai.ShowWindow(SW_SHOW);
	m_btnDaShi.ShowWindow(SW_SHOW);
	m_btnXiaoShi.ShowWindow(SW_SHOW);
	m_btnDanShi.ShowWindow(SW_SHOW);
	m_btnShuangShi.ShowWindow(SW_SHOW);
	m_btnDaGe.ShowWindow(SW_SHOW);
	m_btnXiaoGe.ShowWindow(SW_SHOW);
	m_btnDanGe.ShowWindow(SW_SHOW);
	m_btnShuangGe.ShowWindow(SW_SHOW);

}
void C3D::HideDaXiaoDanShuang()
{
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticShi.ShowWindow(SW_HIDE);
	m_staticGe.ShowWindow(SW_HIDE);

	m_btnDaBai.ShowWindow(SW_HIDE);
	m_btnXiaoBai.ShowWindow(SW_HIDE);
	m_btnDanBai.ShowWindow(SW_HIDE);
	m_btnShuangBai.ShowWindow(SW_HIDE);
	m_btnDaShi.ShowWindow(SW_HIDE);
	m_btnXiaoShi.ShowWindow(SW_HIDE);
	m_btnDanShi.ShowWindow(SW_HIDE);
	m_btnShuangShi.ShowWindow(SW_HIDE);
	m_btnDaGe.ShowWindow(SW_HIDE);
	m_btnXiaoGe.ShowWindow(SW_HIDE);
	m_btnDanGe.ShowWindow(SW_HIDE);
	m_btnShuangGe.ShowWindow(SW_HIDE);

}

void C3D::AdjustNumView()
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

	if (m_list3D.GetSafeHwnd() != NULL) 
	{
		m_list3D.SetWindowPos(NULL, list_haomao_x, list_haomao_y, list_haomao_width, list_haomao_height, SWP_NOZORDER);
	}

	if(m_editBeiShu.GetSafeHwnd() != NULL) 
	{
		m_editBeiShu.SetWindowPos(NULL, edit_beitou_x, edit_beitou_y, edit_beitou_width, edit_beitou_height, SWP_NOZORDER);
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

void C3D::AdjustDanShi()
{
	//if (m_staticDanShi.GetSafeHwnd() != NULL) 
	//{
	//	m_staticDanShi.SetWindowPos(NULL, haoma_tip_x, haoma_tip_y, haoma_tip_width, haoma_tip_height, SWP_NOZORDER);
	//}
	if (m_richDanshiHaoma.GetSafeHwnd() != NULL) 
	{
		m_richDanshiHaoma.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y, edit_haomao_danshi_width, edit_haomao_danshi_height, SWP_NOZORDER);
	}

// 	if (m_editDanShiHaoMa.GetSafeHwnd() != NULL) 
// 	{
// 		m_editDanShiHaoMa.SetWindowPos(NULL, edit_haomao_danshi_x, edit_haomao_danshi_y, edit_haomao_danshi_width, edit_haomao_danshi_height, SWP_NOZORDER);
// 	}
}

void C3D::AdjustAdd()
{
	if (m_btn3DAdd.GetSafeHwnd() != NULL) 
	{
		m_btn3DAdd.SetWindowPos(NULL, btn_chq_add_x, btn_chq_add_y, m_btn3DAdd.Width(), m_btn3DAdd.Height(), SWP_NOZORDER);
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
BOOL C3D::PreTranslateMessage(MSG* pMsg)
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
	if(pMsg-> message   ==ENM_CHANGE) 
	{
		if(pMsg->hwnd == m_richDanshiHaoma.m_hWnd)
		{
			CString strMessage;
			m_richDanshiHaoma.GetWindowText(strMessage);

			MyMessageBox(strMessage);
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

//void C3D::SelctRadioBtn(int ctrl_id)
//{
//	CImgRioButton* pRadio = (CImgRioButton*)GetDlgItem(ctrl_id);
//	if (pRadio != NULL) 
//	{
//		pRadio->SetPushed(true);
//	}
//}

void C3D::CheckWanFaBtn(CTextButton& btn)
{
	m_btnSanXingZhiXuan.SetPushed(false);
	m_btnSanXingZuXuan.SetPushed(false);
	m_btnQianYiZhiXuan.SetPushed(false);
	m_btnQianErZhiXuan.SetPushed(false);
	m_btnSanXingBuDingWei.SetPushed(false);
	m_btnHouYiZhiXuan.SetPushed(false);
	m_btnHouErZhiXuan.SetPushed(false);
	m_btnZhiXuanHeZhi.SetPushed(false);
	m_btnDingWeiDan.SetPushed(false);
	m_btnDaXiaoDanShuang.SetPushed(false);

	btn.SetPushed(true);
}

void C3D::InitWanFaBtns()
{
// 	m_btnSanXingZhiXuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk.png"));
// 	m_btnSanXingZuXuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk.png"));
// 	m_btnQianYiZhiXuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk.png"));
// 	m_btnQianErZhiXuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk.png"));
// 	m_btnSanXingBuDingWei.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk.png"));
// 	m_btnHouYiZhiXuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk.png"));
// 	m_btnHouErZhiXuan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk.png"));
// 	m_btnZhiXuanHeZhi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk.png"));
// 	m_btnDingWeiDan.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk.png"));
// 	m_btnDaXiaoDanShuang.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk.png"));
}

void C3D::InitNumberBtns()
{
	m_btn3DBai0.SetImage(m_numImageList, 0, 10);
	m_btn3DBai1.SetImage(m_numImageList, 1, 10);
	m_btn3DBai2.SetImage(m_numImageList, 2, 10);
	m_btn3DBai3.SetImage(m_numImageList, 3, 10);
	m_btn3DBai4.SetImage(m_numImageList, 4, 10);
	m_btn3DBai5.SetImage(m_numImageList, 5, 10);
	m_btn3DBai6.SetImage(m_numImageList, 6, 10);
	m_btn3DBai7.SetImage(m_numImageList, 7, 10);
	m_btn3DBai8.SetImage(m_numImageList, 8, 10);
	m_btn3DBai9.SetImage(m_numImageList, 9, 10);

	m_btn3DShi0.SetImage(m_numImageList, 0, 10);
	m_btn3DShi1.SetImage(m_numImageList, 1, 10);
	m_btn3DShi2.SetImage(m_numImageList, 2, 10);
	m_btn3DShi3.SetImage(m_numImageList, 3, 10);
	m_btn3DShi4.SetImage(m_numImageList, 4, 10);
	m_btn3DShi5.SetImage(m_numImageList, 5, 10);
	m_btn3DShi6.SetImage(m_numImageList, 6, 10);
	m_btn3DShi7.SetImage(m_numImageList, 7, 10);
	m_btn3DShi8.SetImage(m_numImageList, 8, 10);
	m_btn3DShi9.SetImage(m_numImageList, 9, 10);

	m_btn3DGe0.SetImage(m_numImageList, 0, 10);
	m_btn3DGe1.SetImage(m_numImageList, 1, 10);
	m_btn3DGe2.SetImage(m_numImageList, 2, 10);
	m_btn3DGe3.SetImage(m_numImageList, 3, 10);
	m_btn3DGe4.SetImage(m_numImageList, 4, 10);
	m_btn3DGe5.SetImage(m_numImageList, 5, 10);
	m_btn3DGe6.SetImage(m_numImageList, 6, 10);
	m_btn3DGe7.SetImage(m_numImageList, 7, 10);
	m_btn3DGe8.SetImage(m_numImageList, 8, 10);
	m_btn3DGe9.SetImage(m_numImageList, 9, 10);

	m_staticZuXuan.SetImage(m_bmpNumTip, 0, 10);
	m_staticBuDingWei.SetImage(m_bmpNumTip, 1, 10);
	m_staticBai.SetImage(m_bmpNumTip, 4, 10);
	m_staticShi.SetImage(m_bmpNumTip, 5, 10);
	m_staticGe.SetImage(m_bmpNumTip, 6, 10);

	CString numTextBk = CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx.png");
	m_btn3DBaiQuan.SetBkImage(numTextBk);
	m_btn3DBaiDa.SetBkImage(numTextBk);
	m_btn3DBaiXiao.SetBkImage(numTextBk);
	m_btn3DBaiDan.SetBkImage(numTextBk);
	m_btn3DBaiShuang.SetBkImage(numTextBk);
	m_btn3DBaiQing.SetBkImage(numTextBk);

	m_btn3DShiQuan.SetBkImage(numTextBk);
	m_btn3DShiDa.SetBkImage(numTextBk);
	m_btn3DShiXiao.SetBkImage(numTextBk);
	m_btn3DShiDan.SetBkImage(numTextBk);
	m_btn3DShiShuang.SetBkImage(numTextBk);
	m_btn3DShiQing.SetBkImage(numTextBk);

	m_btn3DGeQuan.SetBkImage(numTextBk);
	m_btn3DGeDa.SetBkImage(numTextBk);
	m_btn3DGeXiao.SetBkImage(numTextBk);
	m_btn3DGeDan.SetBkImage(numTextBk);
	m_btn3DGeShuang.SetBkImage(numTextBk);
	m_btn3DGeQing.SetBkImage(numTextBk);

	m_bmpDxdsh = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_dx2.png"));
	m_btnDaBai.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoBai.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanBai.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangBai.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaShi.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoShi.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanShi.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangShi.SetImage(m_bmpDxdsh, 3, 4);
	m_btnDaGe.SetImage(m_bmpDxdsh, 0, 4);
	m_btnXiaoGe.SetImage(m_bmpDxdsh, 1, 4);
	m_btnDanGe.SetImage(m_bmpDxdsh, 2, 4);
	m_btnShuangGe.SetImage(m_bmpDxdsh, 3, 4);

	m_btn3DBaiQuan.SetTextColor(RGB(97,76,59));
	m_btn3DBaiDa.SetTextColor(RGB(97,76,59));
	m_btn3DBaiXiao.SetTextColor(RGB(97,76,59));
	m_btn3DBaiDan.SetTextColor(RGB(97,76,59));
	m_btn3DBaiShuang.SetTextColor(RGB(97,76,59));
	m_btn3DBaiQing.SetTextColor(RGB(97,76,59));

	m_btn3DShiQuan.SetTextColor(RGB(97,76,59));
	m_btn3DShiDa.SetTextColor(RGB(97,76,59));
	m_btn3DShiXiao.SetTextColor(RGB(97,76,59));
	m_btn3DShiDan.SetTextColor(RGB(97,76,59));
	m_btn3DShiShuang.SetTextColor(RGB(97,76,59));
	m_btn3DShiQing.SetTextColor(RGB(97,76,59));

	m_btn3DGeQuan.SetTextColor(RGB(97,76,59));
	m_btn3DGeDa.SetTextColor(RGB(97,76,59));
	m_btn3DGeXiao.SetTextColor(RGB(97,76,59));
	m_btn3DGeDan.SetTextColor(RGB(97,76,59));
	m_btn3DGeShuang.SetTextColor(RGB(97,76,59));
	m_btn3DGeQing.SetTextColor(RGB(97,76,59));


}
CString C3D::GetBaiDxdshString()
{
	CString strBai;
	if (IsNumBtnCheck(m_btnDaBai)) 
	{
		strBai += _T("9");
	}
	if (IsNumBtnCheck(m_btnXiaoBai)) 
	{
		strBai += _T("1");
	}
	if (IsNumBtnCheck(m_btnDanBai)) 
	{
		strBai += _T("3");
	}
	if (IsNumBtnCheck(m_btnShuangBai)) 
	{
		strBai += _T("2");
	}

	return strBai;
}

CString C3D::GetShiDxdshString()
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

CString C3D::GetGeDxdshString()
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
CString C3D::GetDxdshStringDesc(int nNum)
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

void C3D::InitListCtrl()
{
	m_list3D.SetExtendedStyle(m_list3D.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);
	m_list3D.ModifyStyle(0, LVS_NOCOLUMNHEADER);

	m_list3D.SetPenColor(RGB(247,233,216));
// 	m_list3D.InsertColumn(0, _T("号码"), LVCFMT_CENTER, 313);
// 	m_list3D.InsertColumn(1, _T("模式"), LVCFMT_CENTER, 80);
// 	m_list3D.InsertColumn(2, _T("注数"), LVCFMT_CENTER, 70);
// 	m_list3D.InsertColumn(3, _T("倍数"), LVCFMT_CENTER, 70);
// 	m_list3D.InsertColumn(4, _T(""), LVCFMT_CENTER, 30);

	m_list3D.InsertColumn(0, _T("号码"), LVCFMT_CENTER, 240);
	m_list3D.InsertColumn(1, _T("模式"), LVCFMT_CENTER, 25);
	m_list3D.InsertColumn(2, _T("金额"), LVCFMT_CENTER, 95);
	m_list3D.InsertColumn(3, _T("玩法"), LVCFMT_CENTER, 80);
	m_list3D.InsertColumn(4, _T("注法"), LVCFMT_CENTER, 55);
	m_list3D.InsertColumn(5, _T("倍数"), LVCFMT_CENTER, 50);
	m_list3D.InsertColumn(6, _T("删除"), LVCFMT_CENTER, 31);

	//m_rioSxzhxFuShi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\game\\btn_rio_back.png"));
}

void C3D::HideBaiNums()
{
	m_staticBai.ShowWindow(SW_HIDE);
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);

	m_btn3DBai0.ShowWindow(SW_HIDE);
	m_btn3DBai1.ShowWindow(SW_HIDE);
	m_btn3DBai2.ShowWindow(SW_HIDE);
	m_btn3DBai3.ShowWindow(SW_HIDE);
	m_btn3DBai4.ShowWindow(SW_HIDE);
	m_btn3DBai5.ShowWindow(SW_HIDE);
	m_btn3DBai6.ShowWindow(SW_HIDE);
	m_btn3DBai7.ShowWindow(SW_HIDE);
	m_btn3DBai8.ShowWindow(SW_HIDE);
	m_btn3DBai9.ShowWindow(SW_HIDE);

	m_btn3DBaiQuan.ShowWindow(SW_HIDE);
	m_btn3DBaiDa.ShowWindow(SW_HIDE);
	m_btn3DBaiXiao.ShowWindow(SW_HIDE);
	m_btn3DBaiDan.ShowWindow(SW_HIDE);
	m_btn3DBaiShuang.ShowWindow(SW_HIDE);
	m_btn3DBaiQing.ShowWindow(SW_HIDE);
}

void C3D::HideShiNums()
{
	m_staticShi.ShowWindow(SW_HIDE);

	m_btn3DShi0.ShowWindow(SW_HIDE);
	m_btn3DShi1.ShowWindow(SW_HIDE);
	m_btn3DShi2.ShowWindow(SW_HIDE);
	m_btn3DShi3.ShowWindow(SW_HIDE);
	m_btn3DShi4.ShowWindow(SW_HIDE);
	m_btn3DShi5.ShowWindow(SW_HIDE);
	m_btn3DShi6.ShowWindow(SW_HIDE);
	m_btn3DShi7.ShowWindow(SW_HIDE);
	m_btn3DShi8.ShowWindow(SW_HIDE);
	m_btn3DShi9.ShowWindow(SW_HIDE);

	m_btn3DShiQuan.ShowWindow(SW_HIDE);
	m_btn3DShiDa.ShowWindow(SW_HIDE);
	m_btn3DShiXiao.ShowWindow(SW_HIDE);
	m_btn3DShiDan.ShowWindow(SW_HIDE);
	m_btn3DShiShuang.ShowWindow(SW_HIDE);
	m_btn3DShiQing.ShowWindow(SW_HIDE);
}

void C3D::HideGeNums()
{
	m_staticGe.ShowWindow(SW_HIDE);

	m_btn3DGe0.ShowWindow(SW_HIDE);
	m_btn3DGe1.ShowWindow(SW_HIDE);
	m_btn3DGe2.ShowWindow(SW_HIDE);
	m_btn3DGe3.ShowWindow(SW_HIDE);
	m_btn3DGe4.ShowWindow(SW_HIDE);
	m_btn3DGe5.ShowWindow(SW_HIDE);
	m_btn3DGe6.ShowWindow(SW_HIDE);
	m_btn3DGe7.ShowWindow(SW_HIDE);
	m_btn3DGe8.ShowWindow(SW_HIDE);
	m_btn3DGe9.ShowWindow(SW_HIDE);

	m_btn3DGeQuan.ShowWindow(SW_HIDE);
	m_btn3DGeDa.ShowWindow(SW_HIDE);
	m_btn3DGeXiao.ShowWindow(SW_HIDE);
	m_btn3DGeDan.ShowWindow(SW_HIDE);
	m_btn3DGeShuang.ShowWindow(SW_HIDE);
	m_btn3DGeQing.ShowWindow(SW_HIDE);
}

void C3D::ShowBaiNums()
{
	m_staticBai.ShowWindow(SW_SHOW);
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_HIDE);

	m_btn3DBai0.ShowWindow(SW_SHOW);
	m_btn3DBai1.ShowWindow(SW_SHOW);
	m_btn3DBai2.ShowWindow(SW_SHOW);
	m_btn3DBai3.ShowWindow(SW_SHOW);
	m_btn3DBai4.ShowWindow(SW_SHOW);
	m_btn3DBai5.ShowWindow(SW_SHOW);
	m_btn3DBai6.ShowWindow(SW_SHOW);
	m_btn3DBai7.ShowWindow(SW_SHOW);
	m_btn3DBai8.ShowWindow(SW_SHOW);
	m_btn3DBai9.ShowWindow(SW_SHOW);

	m_btn3DBaiQuan.ShowWindow(SW_SHOW);
	m_btn3DBaiDa.ShowWindow(SW_SHOW);
	m_btn3DBaiXiao.ShowWindow(SW_SHOW);
	m_btn3DBaiDan.ShowWindow(SW_SHOW);
	m_btn3DBaiShuang.ShowWindow(SW_SHOW);
	m_btn3DBaiQing.ShowWindow(SW_SHOW);
}

void C3D::ShowShiNums()
{
	m_staticShi.ShowWindow(SW_SHOW);

	m_btn3DShi0.ShowWindow(SW_SHOW);
	m_btn3DShi1.ShowWindow(SW_SHOW);
	m_btn3DShi2.ShowWindow(SW_SHOW);
	m_btn3DShi3.ShowWindow(SW_SHOW);
	m_btn3DShi4.ShowWindow(SW_SHOW);
	m_btn3DShi5.ShowWindow(SW_SHOW);
	m_btn3DShi6.ShowWindow(SW_SHOW);
	m_btn3DShi7.ShowWindow(SW_SHOW);
	m_btn3DShi8.ShowWindow(SW_SHOW);
	m_btn3DShi9.ShowWindow(SW_SHOW);

	m_btn3DShiQuan.ShowWindow(SW_SHOW);
	m_btn3DShiDa.ShowWindow(SW_SHOW);
	m_btn3DShiXiao.ShowWindow(SW_SHOW);
	m_btn3DShiDan.ShowWindow(SW_SHOW);
	m_btn3DShiShuang.ShowWindow(SW_SHOW);
	m_btn3DShiQing.ShowWindow(SW_SHOW);
}

void C3D::ShowGeNums()
{
	m_staticGe.ShowWindow(SW_SHOW);

	m_btn3DGe0.ShowWindow(SW_SHOW);
	m_btn3DGe1.ShowWindow(SW_SHOW);
	m_btn3DGe2.ShowWindow(SW_SHOW);
	m_btn3DGe3.ShowWindow(SW_SHOW);
	m_btn3DGe4.ShowWindow(SW_SHOW);
	m_btn3DGe5.ShowWindow(SW_SHOW);
	m_btn3DGe6.ShowWindow(SW_SHOW);
	m_btn3DGe7.ShowWindow(SW_SHOW);
	m_btn3DGe8.ShowWindow(SW_SHOW);
	m_btn3DGe9.ShowWindow(SW_SHOW);

	m_btn3DGeQuan.ShowWindow(SW_SHOW);
	m_btn3DGeDa.ShowWindow(SW_SHOW);
	m_btn3DGeXiao.ShowWindow(SW_SHOW);
	m_btn3DGeDan.ShowWindow(SW_SHOW);
	m_btn3DGeShuang.ShowWindow(SW_SHOW);
	m_btn3DGeQing.ShowWindow(SW_SHOW);
}

//显示隐藏组选和不定位
void C3D::ShowZuXuan()
{
	m_staticZuXuan.ShowWindow(SW_SHOW);
	m_staticBuDingWei.ShowWindow(SW_HIDE);
	m_staticBai.ShowWindow(SW_HIDE);
}

void C3D::ShowBuDingWei()
{
	m_staticZuXuan.ShowWindow(SW_HIDE);
	m_staticBuDingWei.ShowWindow(SW_SHOW);
	m_staticBai.ShowWindow(SW_HIDE);
}

void C3D::HideDanShiEdit()
{
	m_editDanShiHaoMa.ShowWindow(SW_HIDE);
	m_richDanshiHaoma.ShowWindow(SW_HIDE);

}

void C3D::ShowDanShiEdit()
{
	m_richDanshiHaoma.ShowWindow(SW_SHOW);
}

void C3D::ResetAllNums()
{
	m_singlezhushu = 0;
	UnCheckNumBtn(m_btn3DBai0);
	UnCheckNumBtn(m_btn3DBai1);
	UnCheckNumBtn(m_btn3DBai2);
	UnCheckNumBtn(m_btn3DBai3);
	UnCheckNumBtn(m_btn3DBai4);
	UnCheckNumBtn(m_btn3DBai5);
	UnCheckNumBtn(m_btn3DBai6);
	UnCheckNumBtn(m_btn3DBai7);
	UnCheckNumBtn(m_btn3DBai8);
	UnCheckNumBtn(m_btn3DBai9);

	UnCheckNumBtn(m_btn3DShi0);
	UnCheckNumBtn(m_btn3DShi1);
	UnCheckNumBtn(m_btn3DShi2);
	UnCheckNumBtn(m_btn3DShi3);
	UnCheckNumBtn(m_btn3DShi4);
	UnCheckNumBtn(m_btn3DShi5);
	UnCheckNumBtn(m_btn3DShi6);
	UnCheckNumBtn(m_btn3DShi7);
	UnCheckNumBtn(m_btn3DShi8);
	UnCheckNumBtn(m_btn3DShi9);

	UnCheckNumBtn(m_btn3DGe0);
	UnCheckNumBtn(m_btn3DGe1);
	UnCheckNumBtn(m_btn3DGe2);
	UnCheckNumBtn(m_btn3DGe3);
	UnCheckNumBtn(m_btn3DGe4);
	UnCheckNumBtn(m_btn3DGe5);
	UnCheckNumBtn(m_btn3DGe6);
	UnCheckNumBtn(m_btn3DGe7);
	UnCheckNumBtn(m_btn3DGe8);
	UnCheckNumBtn(m_btn3DGe9);

	UnCheckNumBtn(m_btnDaBai);
	UnCheckNumBtn(m_btnXiaoBai);
	UnCheckNumBtn(m_btnDanBai);
	UnCheckNumBtn(m_btnShuangBai);
	UnCheckNumBtn(m_btnDaShi);
	UnCheckNumBtn(m_btnXiaoShi);
	UnCheckNumBtn(m_btnDanShi);
	UnCheckNumBtn(m_btnShuangShi);
	UnCheckNumBtn(m_btnDaGe);
	UnCheckNumBtn(m_btnXiaoGe);
	UnCheckNumBtn(m_btnDanGe);
	UnCheckNumBtn(m_btnShuangGe);

//	m_editDanShiHaoMa.SetWindowText(_T(""));
	m_richDanshiHaoma.SetWindowText(_T(""));

}

CString C3D::GetBaiString()
{
	CString str;
	if (IsNumBtnCheck(m_btn3DBai0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btn3DBai1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btn3DBai2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btn3DBai3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btn3DBai4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btn3DBai5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btn3DBai6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btn3DBai7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btn3DBai8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btn3DBai9))
	{
		str += _T("9");
	}
	return str;
}

CString C3D::GetShiString()
{
	CString str;
	if (IsNumBtnCheck(m_btn3DShi0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btn3DShi1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btn3DShi2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btn3DShi3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btn3DShi4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btn3DShi5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btn3DShi6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btn3DShi7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btn3DShi8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btn3DShi9))
	{
		str += _T("9");
	}
	return str;
}

CString C3D::GetGeString()
{
	CString str;
	if (IsNumBtnCheck(m_btn3DGe0))
	{
		str += _T("0");
	}
	if (IsNumBtnCheck(m_btn3DGe1))
	{
		str += _T("1");
	}
	if (IsNumBtnCheck(m_btn3DGe2))
	{
		str += _T("2");
	}
	if (IsNumBtnCheck(m_btn3DGe3))
	{
		str += _T("3");
	}
	if (IsNumBtnCheck(m_btn3DGe4))
	{
		str += _T("4");
	}
	if (IsNumBtnCheck(m_btn3DGe5))
	{
		str += _T("5");
	}
	if (IsNumBtnCheck(m_btn3DGe6))
	{
		str += _T("6");
	}
	if (IsNumBtnCheck(m_btn3DGe7))
	{
		str += _T("7");
	}
	if (IsNumBtnCheck(m_btn3DGe8))
	{
		str += _T("8");
	}
	if (IsNumBtnCheck(m_btn3DGe9))
	{
		str += _T("9");
	}
	return str;
}

CString C3D::GetWanDesc()
{
	CString strWanfa;
	switch(m_gameKind)
	{
	case enWF_ZhixuanFushi:
		strWanfa = _T("三星直选复式"); break;		
	case enWF_ZhixuanDanshi:
		strWanfa = _T("三星直选单式"); break;
	case enWF_DingWeiDan:
		strWanfa = _T("定位胆"); break;
	case enWF_DaxiaoDanshuang:
		strWanfa = _T("大小单双"); break;

	case enWF_QiansanzusanFushi:
		strWanfa = _T("三星组选组三复式"); break;
	case enWF_QiansanzusanDanshi:
		strWanfa = _T("三星组选组三单式"); break;
	case enWF_QiansanzuliuFushi:
		strWanfa = _T("三星组选组六复式"); break;
	case enWF_QiansanzuliuDanshi:
		strWanfa = _T("三星组选组六单式"); break;

	case enWF_QianyizhixuanFushi:
		strWanfa = _T("前一直选"); break;

	case enWF_QianerzhixuanFushi:
		strWanfa = _T("前二直选复式"); break;
	case enWF_QianerzhixuanDanshi:
		strWanfa = _T("前二直选单式"); break;

	case enWF_Budingwei:
		strWanfa = _T("三星不定位"); break;

	case enWF_HouyizhixuanFushi:
		strWanfa = _T("后一直选"); break;

	case enWF_HouerzhixuanFushi:
		strWanfa = _T("后二直选复式"); break;
	case enWF_HouerzhixuanDanshi:
		strWanfa = _T("后二直选单式"); break;

		// 	case SanXingZhiXuanHeZhi:
		// 		strWanfa = _T("三星直选和值"); break;
	default:break;
	}

	return strWanfa;
}

tagEnumTreeNumbers C3D::GetWanFaByDesc(const CString& desc)
{
	if (desc == _T("三星直选复式")) 
	{
		return enWF_ZhixuanFushi;
	}
	else if(desc == _T("三星直选单式"))
	{
		return enWF_ZhixuanDanshi;
	}
	else if(desc == _T("三星组选组三复式"))
	{
		return enWF_QiansanzusanFushi;
	}
	else if(desc == _T("定位胆"))
	{
		return enWF_DingWeiDan;
	}
	else if(desc == _T("大小单双"))
	{
		return enWF_DaxiaoDanshuang;
	}
	else if(desc == _T("三星组选组三单式"))
	{
		return enWF_QiansanzusanDanshi;
	}
	else if(desc == _T("三星组选组六复式"))
	{
		return enWF_QiansanzuliuFushi;
	}
	else if(desc == _T("三星组选组六单式"))
	{
		return enWF_QiansanzuliuDanshi;
	}
	else if(desc == _T("前一直选"))
	{
		return enWF_QianyizhixuanFushi;
	}
	else if(desc == _T("前二直选复式"))
	{
		return enWF_QianerzhixuanFushi;
	}
	else if(desc == _T("前二直选单式"))
	{
		return enWF_QianerzhixuanDanshi;
	}
	else if(desc == _T("三星不定位"))
	{
		return enWF_Budingwei;
	}
	else if(desc == _T("后一直选"))
	{
		return enWF_HouyizhixuanFushi;
	}
	else if(desc == _T("后二直选复式"))
	{
		return enWF_HouerzhixuanFushi;
	}
	else if(desc == _T("后二直选单式"))
	{
		return enWF_HouerzhixuanDanshi;
	}
	else 
	{
		return Invalid;
	}
}

void C3D::DrawLastKjHaoma(CDC* pDC, Graphics& graphics)
{
	int nTimeLeft = GetKjShjDiff();

	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
	{
		_tcscpy_s(m_lastExpect, KJ_HAOMA_LENGTH, m_kjXinxi[0].qihao);
		m_bKaiJiangzhong=false;

	}
	else if(nTimeLeft == 0)
	{
		m_bKaiJiangzhong=true;
	}


	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&m_KjInfoFont1);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText(_T("3D彩"), &rcCzh, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	CRect rcDi;
	rcDi.left = rcCzh.right+10;
	rcDi.right = rcDi.left+20;
	rcDi.top = rcCzh.top;
	rcDi.bottom = rcCzh.bottom;
	pDC->DrawText(_T("第"), &rcDi, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(RGB(253, 191, 6));

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


	strYg.Format(_T("第%s	下单时间"), GetNextExpect());
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
	strYg.Format(_T("%s"), GetKjShjDiffDesc());
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
	int index = 0;
	for (int i=0; i<str.GetLength(); i++) 
	{
		if (iswdigit(str.GetAt(i))) 
		{
			int num = _ttoi(CString(str.GetAt(i)));
			if (num >=0 && num < 10) 
			{
				DrawBigNum(graphics, m_kjNumBig, Rect(kj_big_haoma_x + index * (m_bigNumWidth+2), kj_big_haoma_y, m_bigNumWidth, m_bigNumHeight), num);
			}

			index ++;
		}
	}
}

void C3D::DrawTopKjHaoma(CDC* pDC, Graphics& graphics)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_smallNumfont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(250 ,250 ,250));

	for (int cnt=0; cnt<4; cnt++) 
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

void C3D::DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_bigNumWidth, 0, m_bigNumWidth, m_bigNumHeight, UnitPixel);
}

void C3D::DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	graphics.DrawImage(img, rc_dest, num*m_smallNumWidth, 0, m_smallNumWidth, m_smallNumHeight, UnitPixel);
}

void C3D::DrawZongZhushuJinE(CDC* pDC)
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
	strText.Format(_T("注数:"),m_zongZhuShu);
	pDC->DrawText(strText, &rcAllZongzhushu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	rcRect.CopyRect(rcAllZongzhushu);
	rcRect.left += 32;
	rcRect.right += 32;
	pDC->SetTextColor(RGB(160, 8, 8));

	strText.Format(_T("%d"),m_zongZhuShu);
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

//江苏快3：http://www.cailele.com/static/K3/newlyopenlist.xml

bool C3D::LuckyNumCQSSC()
{
	CString xmlurl = _T("http://www.cailele.com/static/ps/newlyopenlist.xml");

	CString strPath;  //文件路径
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strPath.ReleaseBuffer(); 
	int pos = strPath.ReverseFind(_T('\\')); 
	strPath = strPath.Left(pos)+ _T("\\p3.xml"); 

//	if(FAILED(URLDownloadToFile(NULL, xmlurl, strPath, 0, NULL)) && _tcslen(m_lastExpect) != 0)
//	{
//		return false;
//	}

	CString strexpect;
	CString stropencode;
	CString stropentime;
	CString strall;

	if(!m_xml.Load(strPath))
	{
		return false;
	}

	m_xml.ResetMainPos();
	m_xml.FindElem();

	int count = 0;
	while( m_xml.FindChildElem( _T("row") ) && count < Kj_XinXi_Count )
	{
		strexpect = m_xml.GetChildAttrib(_T("expect"));			//期号
		stropencode = m_xml.GetChildAttrib(_T("opencode"));	//号码
		stropentime = m_xml.GetChildAttrib(_T("opentime"));	//开奖时间

		_tcscpy_s(m_kjXinxi[count].qihao, KJ_QIHAO_LENGTH, strexpect);
		_tcscpy_s(m_kjXinxi[count].haoma, KJ_HAOMA_LENGTH, stropencode);
		_tcscpy_s(m_kjXinxi[count].shijian, KJ_SHIJIAN_LENGTH, stropentime);

		count ++;
	}

	m_kjXinxiCont = count;

	if (_tcscmp(m_lastExpect, m_kjXinxi[0].qihao) != 0) 
	{
		_tcscpy_s(m_lastExpect, KJ_HAOMA_LENGTH, m_kjXinxi[0].qihao);
		Invalidate();
	}

	return true;
}

CString C3D::GetNextExpect()
{
	if (_tcslen(m_lastExpect) == 0)
	{
		return _T("---");
	}

	int iQh = _ttoi(m_lastExpect);
	iQh ++;
	if (GetKjShjDiff() < 1) 
	{
		iQh ++;	
	}

	CString rQh;
	rQh.Format(_T("%d"), iQh);

	return rQh;
}

CTime C3D::GetNextKjShj()
{
	CString lastKjShj = m_kjXinxi[0].shijian;

	if(lastKjShj.IsEmpty())
	{
		CTime t;
		theApp.GetTime(t);

		return t;
	}
	COleVariant vtime(lastKjShj);
	vtime.ChangeType(VT_DATE);
	COleDateTime time4 = vtime;

	SYSTEMTIME systime;   
	VariantTimeToSystemTime(time4, &systime);   
	CTime ctm(systime);

	ctm += timespan_kj_shj;


	return ctm;
}

long C3D::GetKjShjDiff()
{
	if (_tcslen(m_lastExpect) == 0)
	{
		return 0;
	}

	CTime t;
	theApp.GetTime(t);
	CTimeSpan span = GetNextKjShj() - t;
	return (long)span.GetTotalSeconds() - 10 * 60;
}

CString C3D::GetKjShjDiffDesc()
{
	if (_tcslen(m_lastExpect) == 0)
	{
		return _T("00:00");	
	}

	long secDiff = GetKjShjDiff();
	if (secDiff < 0L) 
	{
		return _T("00:00");
	}

	int hour = secDiff / 3600;
	int tmp = secDiff % 3600;
	int minute = tmp / 60;
	int second = tmp % 60;

	CString str;
	str.Format(_T("%02d:%02d:%02d"), hour, minute, second);

	return str;
}

void C3D::SetLockText()
{
	m_btnLock.SetWindowText(mapFandian[m_gameKind]);

	m_btWxzxTip.ShowWindow(SW_SHOW);
	if(m_gameKind == enWF_ZhixuanFushi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从百位、十位、个位至少各选择一个号码，所选号码与开奖号码相同，且顺序一致，即为中奖"));
	}
	else if(m_gameKind == enWF_ZhixuanDanshi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("手动输入至少一个三位数字，所选号码与开奖号码相同，且顺序一致，即为中奖"));
	}
	else if(m_gameKind == enWF_QiansanzusanFushi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从0-9中选择两个或两个以上的号码，所选号码与开奖号码相同，顺序不限，即为中奖"));
	}
	else if(m_gameKind == enWF_QiansanzusanDanshi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("手动输入至少一个三位数字，需要有一个重复数字 如：544，所选号码与开奖号码相同，且顺序一致，即为中奖"));
	}
	else if(m_gameKind == enWF_QiansanzuliuFushi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从0-9中选择三个或三个以上的号码，所选号码与开奖号码相同，顺序不限，即为中奖"));
	}
	else if(m_gameKind == enWF_QiansanzuliuDanshi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("手动输入至少一个三位数字，不能有重复数字，所选号码与开奖号码相同，且顺序一致，即为中奖"));
	}
	else if(m_gameKind == enWF_QianyizhixuanFushi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从0-9中至少选择一个号码，所选号码与开奖号码第一位相同，即为中奖"));
	}
	else if(m_gameKind == enWF_QianerzhixuanFushi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从十位和百位中各选择至少一个号码，所选号码与开奖号码前两位相同，且顺序一致，即为中奖"));
	}
	else if(m_gameKind == enWF_QianerzhixuanDanshi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("手动输入至少一个两位位数字，所选号码与开奖号码前两位相同，且顺序一致，即为中奖"));
	}
	else if(m_gameKind == enWF_Budingwei)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从0-9中选择至少一个号码，所选号码与开奖号码任意一位相同，即为中奖"));
	}
	else if(m_gameKind == enWF_HouyizhixuanFushi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从0-9中至少选择一个号码，所选号码与开奖号码最后一位相同，即为中奖"));
	}
	else if(m_gameKind == enWF_HouerzhixuanFushi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从十位和个位中各选择至少一个号码，所选号码与开奖号码后两位相同，且顺序一致，即为中奖"));
	}
	else if(m_gameKind == enWF_HouerzhixuanDanshi)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("手动输入至少一个两位位数字，所选号码与开奖号码后两位相同，且顺序一致，即为中奖"));
	}
	else if(m_gameKind == enWF_DingWeiDan)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从百位、十位、个位至少选择一个号码，所选号码与开奖号码对应位置相同，即为中奖"));
	}
	else if(m_gameKind == enWF_DaxiaoDanshuang)
	{
		m_ToolTipCtrl.AddTool(&m_btWxzxTip,TEXT("从百位、十位、个位选择一个号码，所选号码与开奖号码对应的大小单双相同，即为中奖"));
	}

//	OnBnClickedBtnClsList();
// 	CString str;
// 	int index = (CZ3D << 16) | (unsigned short)m_gameKind;
// 	str.Format(_T("%.2lf/%.2lf"), theBonus[index], theAccount.fandian*100.0);
// 
// 	m_btnLock.SetWindowText(str + _T("%"));
}

bool C3D::GetZuSanHaoma(CStringArray& strArr)
{
	CString line;
	m_richDanshiHaoma.GetWindowText(line);
	if(line.GetLength() > 2999)
	{
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

bool C3D::GetZuSanHaoma(CString& haoma, int& zhushu)
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
		CString strTemp;
		strTemp = strArr.GetAt(i);

		int nLength = strTemp.GetLength();
		for(int j = 0;j < nLength;j++)
		{
			haoma += strTemp.GetAt(j);
			if (j != (nLength - 1))
			{
				haoma += _T(",");
			}

			if (j == (nLength - 1) && i != count-1)
			{
				haoma += _T("|");
			}

		}
	}
	zhushu = count;

	return true;
}

bool C3D::IsZuSanHaoma(const CString& str)
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
#include <algorithm>
#include <functional> 
int isnum1(string strinfo)  
{  
	string strset="1234567890";  
	int first = strinfo.find_first_of(strset);  
	if(first == string::npos)   
	{  
		return -1;  
	}   

	return 0;  
}
int SplitString3(const string &srcStr, vector<string> &destVec, const string splitStr="|")
{
	if(srcStr.size()==0 || srcStr.empty())
	{   
		return false;
	}   
	size_t oldPos,newPos;
	oldPos=0;
	newPos=0;
	string tempData;
	while(1)
	{   
		newPos=srcStr.find(splitStr,oldPos);
		if(newPos!=string::npos)
		{   
			tempData = srcStr.substr(oldPos,newPos-oldPos);
			destVec.push_back(tempData);
			oldPos=newPos+splitStr.size();
		}   
		else if(oldPos<=srcStr.size())
		{   
			tempData= srcStr.substr(oldPos);
			destVec.push_back(tempData);
			break;
		}   
		else
		{   
			break;
		}   
	}   
	return true;
}


bool C3D::GetDanShiHaoma(CString& haoma, int size, int& zhushu)
{
	CStringArray strArr;
	CString line;
	//	m_editDanShiHaoMa.GetWindowText(line);
	m_richDanshiHaoma.GetWindowText(line);

	if(line.GetLength() > 2999)
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
		CString strTemp;
		strTemp = strArr.GetAt(i);

		int nLength = strTemp.GetLength();
		for(int j = 0;j < nLength;j++)
		{
			haoma += strTemp.GetAt(j);
			if (j != (nLength - 1))
			{
				haoma += _T(",");
			}

			if (j == (nLength - 1) && i != count-1)
			{
				haoma += _T("|");
			}

		}
	}
	zhushu = count;


	return true;
}

bool C3D::GetHaomaStrs(CStringArray& strArr, CString line, int size)
{
	int len = line.GetLength();
	CString subStr;
	int nTempHaoma[10];
	int nCount = 0;
	CString strTemp;

	for (int i=0; i<len; i++) 
	{
		int nTmp = 10;
		if (iswdigit(line.GetAt(i))) 
		{
			subStr += line.GetAt(i);
			strTemp = line.GetAt(i);
			nTmp = _ttoi(strTemp);
			if(m_gameKind == enWF_QiansanzuliuDanshi)
			{
				for(int a = 0;a < nCount;a++)
				{
					if(nTmp == nTempHaoma[a])
					{
						MyMessageBox(L"您输入的号码有重复数字 ");
						return false;
					}
				}
			}
			else if(m_gameKind == enWF_QiansanzusanDanshi && nCount == 2)
			{
				nTempHaoma[nCount++] = nTmp;
				bool bSuit = false;
				for(int a = 0;a < nCount;a++)
				{
					int nTempn = nTempHaoma[a];
					for(int b = a+1;b <nCount+1;b++)
					{
						int nTempb = nTempHaoma[b];
						if(nTempn == nTempHaoma[b])
						{
							bSuit = true;
							break;
						}

					}
				}

				if(!bSuit)
				{
					MyMessageBox(L"您输入的号码不符合！");
					return false;
				}
			}

			nTempHaoma[nCount++] = nTmp;
			if (subStr.GetLength() > size) 
			{
				return false;
			}
		}
		else
		{
			nTmp = 10;
			nCount=0;

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
void C3D::InitWanFaMoShi()
{
	theGameKind[(CZ3D << 16) | (unsigned short)enWF_ZhixuanFushi] = _T("三星直选复式");		
	theGameKind[(CZ3D << 16) | (unsigned short)enWF_ZhixuanDanshi] = _T("三星直选单式");
	theGameKind[(CZ3D << 16) | (unsigned short)enWF_DingWeiDan] = _T("定位胆");
	theGameKind[(CZ3D << 16) | (unsigned short)enWF_DaxiaoDanshuang] = _T("大小单双");

	theGameKind[(CZ3D << 16) | (unsigned short)enWF_QiansanzusanFushi] = _T("三星组选组三复式");
	theGameKind[(CZ3D << 16) | (unsigned short)enWF_QiansanzusanDanshi] = _T("三星组选组三单式");
	theGameKind[(CZ3D << 16) | (unsigned short)enWF_QiansanzuliuFushi] =  _T("三星组选组六复式");
	theGameKind[(CZ3D << 16) | (unsigned short)enWF_QiansanzuliuDanshi] = _T("三星组选组六单式");

	theGameKind[(CZ3D << 16) | (unsigned short)enWF_QianyizhixuanFushi] = _T("前一直选");

	theGameKind[(CZ3D << 16) | (unsigned short)enWF_QianerzhixuanFushi] = _T("前二直选复式");
	theGameKind[(CZ3D << 16) | (unsigned short)enWF_QianerzhixuanDanshi] = _T("前二直选单式");

	theGameKind[(CZ3D << 16) | (unsigned short)enWF_Budingwei] = _T("三星不定位");

	theGameKind[(CZ3D << 16) | (unsigned short)enWF_HouyizhixuanFushi] = _T("后一直选");

	theGameKind[(CZ3D << 16) | (unsigned short)enWF_HouerzhixuanFushi] = _T("后二直选复式");
	theGameKind[(CZ3D << 16) | (unsigned short)enWF_HouerzhixuanDanshi] = _T("后二直选单式");
}

//读取事件
bool C3D::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
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
		case SUB_GP_SET_USER_BONUS_RET:
			{
				ASSERT(wDataSize >= sizeof(CMD_GR_SetUserBonusRet));
				if(wDataSize < sizeof(CMD_GR_SetUserBonusRet)) return false;

				CMD_GR_SetUserBonusRet* pSetUserBonus = (CMD_GR_SetUserBonusRet*)pData;
				int nResult = pSetUserBonus->n_t_result;

				if(nResult==0)
				{
				//	SetLockText(m_fBonus, m_fPercent);
					MyMessageBox(L"返点数据修改成功！", L"温馨提示");
					SendToServer(6);

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

				CMD_GR_TouzhuRes* pTouzhuRes = (CMD_GR_TouzhuRes*)pData;

				LONG lResult = pTouzhuRes->lResult;

				m_DlgStatus.HideStatusWindow();
				if (lResult == 0)
				{
					if((pTouzhuRes->nSign != m_nTzhSign1))
					{
						m_nTzhSign1 = pTouzhuRes->nSign;
						m_list3D.DeleteAllItems();
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
		}
	}
	return false;
}

VOID C3D::SendToServer(int nSendType)
{

	if(nSendType == 1)
	{
		m_DlgStatus.ShowStatusWindow(TEXT("正在提交数据，请稍候..."));

		int itm_cnt = m_list3D.GetItemCount();
		int nNowKind = 10000;
		int nNowBeishu = 100000;
		int nNowMoshi = 10000;

		CString strTouZhuhaoma;

		map<int,int> mapHaoma;
		mapHaoma.clear();
		for(int i = 0;i < itm_cnt;i++)
		{
			strTouZhuhaoma.Empty();
			CString strWanfa = m_list3D.GetItemText(i, 3);
			CString strBeishu = m_list3D.GetItemText(i, 5);
			CString strMoshi = m_list3D.GetItemText(i, 1);
			int nKind = GetWanFaByDesc(strWanfa);
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
				CString wanfa = m_list3D.GetItemText(j, 3);
				int kind = GetWanFaByDesc(wanfa);
				CString sbeishu = m_list3D.GetItemText(j, 5);
				int beishu = _ttoi(sbeishu);
				CString sMoshi = m_list3D.GetItemText(j, 1);
				int scoremoshi = 0;
				scoremoshi = GetMoshiRet(sMoshi);

				if (kind == nNowKind && beishu == nNowBeishu&& scoremoshi == nNowMoshi)
				{
					CString strZhu = m_list3D.GetItemText(j, 4);
					nZhushu += _ttoi(strZhu);
					int nSize = mapHaoma.size();
					mapHaoma[nSize] = j;

					CString haoma = m_list3D.GetItemText(j, 0);

					if(nKind == enWF_DaxiaoDanshuang)
					{
						CString strTemp = haoma;
						strTemp.Replace(L"大",L"9");
						strTemp.Replace(L"小",L"1");
						strTemp.Replace(L"单",L"3");
						strTemp.Replace(L"双",L"2");

						haoma = strTemp;
					}

					strTouZhuhaoma+= haoma;
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
			TouzhuCQSSC.nGameType = CZ3D;
			TouzhuCQSSC.nMoshi = nNowMoshi;

			TouzhuCQSSC.nSign = m_nTzhSign;

			CString strQiHao = GetNextExpect();

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

			//m_bCanSend = true;
		}
		m_bShowFail = true;
		OnBnClickedBtnClsList();

		m_nTzhSign ++;

	}

	if(nSendType == 4)
	{
		int nItemCount = m_list3D.GetItemCount();

		CMD_GP_TouZhuCQSSC_Zhuihao TouzhuCQSSC;
		ZeroMemory(&TouzhuCQSSC,sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
		TouzhuCQSSC.dwUserID = theAccount.user_id;
		TouzhuCQSSC.nGameType = CZ3D;

		TouzhuCQSSC.nSign = m_nTzhSign;
		if(m_bZhuiZjtz)
			TouzhuCQSSC.bZhuihao = GetTickCount();
		else
			TouzhuCQSSC.bZhuihao = 2;
		for(int n=0; n< nItemCount; n++)
		{
			int nSendCount = 0;
			int nEnd = 1;



			CString wanfa = m_list3D.GetItemText(n, 3);
			int kind = GetWanFaByDesc(wanfa);

			TouzhuCQSSC.nGameKind = kind;
			CString moshi = m_list3D.GetItemText(n, 1);
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
			// 		strQiHao = m_wfcRule.GetNextExpect();
			// 
			// 	string sNowQi;
			// 	ChangeStringToA(strQiHao,sNowQi);

			string sNowQi;
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
			CString strZhu = m_list3D.GetItemText(n, 4);
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
					//			if(m_bCanSend)
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
				//m_bCanSend = false;
				nSendCount++;
			}

		}
			OnBnClickedBtnClsList();
			m_nTzhSign++;
			m_DlgStatus.ShowStatusWindow(TEXT("正在提交数据，请稍候..."));
			m_list3D.DeleteAllItems();

		return;
	}


	if(nSendType == 5)
	{
		CMD_GP_SetUserBonus SetUserBonus;
		ZeroMemory(&SetUserBonus,sizeof(SetUserBonus));

		SetUserBonus.n_t_userid = theAccount.user_id;
		SetUserBonus.n_t_type_id = CZ3D;
		SetUserBonus.n_t_kind_id = m_gameKind;
		SetUserBonus.f_t_bonus = m_fBonus;
		SetUserBonus.f_bonuspercent = m_fPercent;

		//发送数据
		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SET_USER_BONUS,&SetUserBonus,sizeof(SetUserBonus));
		return;
	}

	if(nSendType == 6)
	{
		CMD_GP_GetUserFandian GetUserFandian;
		ZeroMemory(&GetUserFandian,sizeof(GetUserFandian));

		GetUserFandian.n_t_userid = theAccount.user_id;
		GetUserFandian.n_t_type_id = CZ3D;
		GetUserFandian.n_t_kind_id = m_gameKind;

		//发送数据
		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_USER_FANDIAN,&GetUserFandian,sizeof(GetUserFandian));
		return;

	}

	if(nSendType == 8)
	{
		CMD_GP_GetMoreRecord GetMoreRecord;
		ZeroMemory(&GetMoreRecord,sizeof(GetMoreRecord));

		GetMoreRecord.n_t_type = CZ3D;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_MORE_RECORD,&GetMoreRecord,sizeof(GetMoreRecord));

		return;
	}

}

bool	C3D::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}
//设置返点
void C3D::OnBnClickedBtnLock()
{
	int index = (CZ3D << 16) | (unsigned short)m_gameKind;

	CDlgFandian	dlg;

	dlg.SetBonus(mapBonus[m_gameKind], theBonus[index], mapPercent[m_gameKind], theAccount.fandian*100.0, mapDanweiFandian[m_gameKind]);
	if(IDOK == dlg.DoModal())
	{
		dlg.GetBonus(m_fBonus,  m_fPercent);

		SendToServer(5);
	}
}
//倍投加
void C3D::OnBnClickedBeiTouJia()
{
	CString strBeishu;

	m_beishu += 1;

	strBeishu.Format(L"%d",m_beishu);

	m_editBeiShu.SetWindowText(strBeishu);
	return;

}
//倍投减
void C3D::OnBnClickedBeiTouJian()
{
	CString strBeishu;

	m_beishu -= 1;

	if(m_beishu<1)
	{
		m_beishu = 1;
	}
	strBeishu.Format(L"%d",m_beishu);

	m_editBeiShu.SetWindowText(strBeishu);
	return;
}
BOOL C3D::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	if((nCommandID >= IDC_BTN_BAI_3D_0 && nCommandID <= IDC_BTN_GE_3D_9)||(nCommandID >= IDC_BTN_DA_SHI && nCommandID <= IDC_BTN_SHUANG_GE)||(nCommandID >= IDC_BTN_DA_BAI && nCommandID <= IDC_BTN_SHUANG_BAI))
	{
		Get3dZhushu(CZ3D,m_gameKind);	
	}
	return  __super::OnCommand(wParam,lParam);;
}
int C3D::Get3dZhushu(int nTypeID,int nKindID)
{
	CString strBai = GetBaiString();
	CString strShi = GetShiString();
	CString strGe = GetGeString();
	if(nKindID == enWF_ZhixuanFushi)
	{
		m_singlezhushu = strBai.GetLength() * strShi.GetLength() * strGe.GetLength();
		if(m_singlezhushu > 800)
		{
			MyMessageBox(_T("你选择的号码超过限制注数800注，请重新输入！"));
			m_singlezhushu = 0;
			OnBnClickedBtnClsList();
			return m_singlezhushu;
		}
	}
	else if (enWF_QiansanzusanFushi == nKindID) //三星组三复式
	{
		if (strBai.GetLength() < 2) 
		{
			return 0;
		}
		m_singlezhushu = strBai.GetLength() * (strBai.GetLength() - 1);
	}
	else if (enWF_QiansanzuliuFushi == nKindID) //三星组六复式
	{
		if (strBai.GetLength() < 3) 
		{
			return 0;
		}

		m_singlezhushu = strBai.GetLength() * (strBai.GetLength() - 1) * (strBai.GetLength() - 2) / 6;
	}
	else if (enWF_QianyizhixuanFushi == nKindID) //前一直选
	{
		m_singlezhushu = strBai.GetLength();
		if(m_singlezhushu > 8)
		{
			MyMessageBox(_T("你选择的号码超过限制注数8注，请重新输入！"));
			return 0;
		}
	}
	else if (enWF_QianerzhixuanFushi == nKindID) //前二直选复式
	{

		m_singlezhushu = strBai.GetLength() * strShi.GetLength();
		if(m_singlezhushu > 80)
		{
			MyMessageBox(_T("你选择的号码超过限制注数80注，请重新输入！"));
			return 0;
		}

	}
	else if (enWF_DingWeiDan == nKindID) //定位胆
	{
		m_singlezhushu = strBai.GetLength() + strShi.GetLength() + strGe.GetLength();
		if(m_singlezhushu > 24)
		{
			MyMessageBox(_T("你选择的号码超过限制注数24注，请重新输入！"));
			return 0;
		}
	}
	else if(enWF_DaxiaoDanshuang == nKindID)
	{
		CString strBaiDXDS = GetBaiDxdshString();
		CString strShiDXDS = GetShiDxdshString();
		CString strGeDXDS = GetGeDxdshString();

		int nBaiLength = strBaiDXDS.GetLength();
		int nShiLength = strShiDXDS.GetLength();
		int nGeLength = strGeDXDS.GetLength();

		if(nBaiLength>1||nShiLength>1||nGeLength>1)
		{
			MyMessageBox(_T("大小单双每位最多只能选择一个号码"));
			return 0;
		}


		m_singlezhushu = nBaiLength + nShiLength + nGeLength;
	}
	else if (enWF_Budingwei == nKindID) //三星不定位
	{

		int nLength = strBai.GetLength();

		m_singlezhushu = nLength;
	}
	else if (enWF_HouyizhixuanFushi == nKindID) //后一直选
	{
		m_singlezhushu = strGe.GetLength();
		if(m_singlezhushu > 8)
		{
			MyMessageBox(_T("你选择的号码超过限制注数8注，请重新输入！"));
			return 0;
		}
	}
	else if (enWF_HouerzhixuanFushi == nKindID) //后二直选复式
	{
		m_singlezhushu = strShi.GetLength() * strGe.GetLength();
		if(m_singlezhushu > 80)
		{
			MyMessageBox(_T("你选择的号码超过限制注数80注，请重新输入！"));
			return 0;
		}
	}

	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=200;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	return m_singlezhushu;
}

LRESULT C3D::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	//OnBnClickedBtnDelSel();
	nSubItem--;

	int zhushu = _ttoi(m_list3D.GetItemText(nItem, 4));
	m_zongZhuShu -= zhushu;
	m_list3D.deleteItemEx( nItem );

	int nItemCount = m_list3D.GetItemCount();
	m_zongJine = 0.00f;
	for(int i = 0;i < nItemCount;i++)
	{
		CString strBeiSHu = m_list3D.GetItemText(i,5);
		CString strZhuSHu = m_list3D.GetItemText(i,4);
		int beishu = _ttoi(strBeiSHu);
		int zhushu = _ttoi(strZhuSHu);
		CString strMoshi = m_list3D.GetItemText(i,1);
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


void C3D::OnEnChangeRichEditDanshiInput()
{
	m_singlezhushu = GetDanShiZhusu(m_gameKind);


	return ;
}

int C3D::GetDanShiZhusu(int nKind)
{

	m_singlezhushu = 0;
	CString strHaoma;

	m_richDanshiHaoma.GetWindowText(strHaoma);

	strHaoma.Replace(L"\r",L" ");
	strHaoma.Replace(L"\n",L" ");

	if(nKind == enWF_ZhixuanDanshi)
	{
		string strGetHaoma;

		ChangeStringToA(strHaoma,strGetHaoma);

// 		string::iterator new_end = std::remove_if(strGetHaoma.begin(), strGetHaoma.end(), bind2nd(equal_to <char>(), '\r')); 
// 
// 		strGetHaoma.erase(new_end, strGetHaoma.end()); 
// 
		string splitStr=" ";
		bool bSuit = true;
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						m_singlezhushu = 0;
						bSuit = false;

						break;

					}
				}

				if (nSize > 3 || !bSuit)
				{
					m_singlezhushu = 0;
					break;
				}
				if (nSize != 3)
				{
					m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;

				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
					m_singlezhushu = 0;
					break;
				}
				if (nSize != 3)
				{
					m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}
	if(nKind == enWF_QiansanzuliuDanshi)
	{
		string strGetHaoma;

		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";
		bool bSuit = true;
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
					m_singlezhushu = 0;
					break;
				}
				if (nSize != 3)
				{
					m_singlezhushu = 0;
					break;
				}

				if(tempData[0] == tempData[1] || tempData[0] == tempData[2] || tempData[2] == tempData[1] )
				{
					m_singlezhushu = 0;
					break;
				}
				nZhushu = 1;


				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						bSuit = false;

						m_singlezhushu = 0;
						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
					m_singlezhushu = 0;
					break;
				}
				if (nSize != 3)
				{
					m_singlezhushu = 0;
					break;
				}
				if(tempData[0] == tempData[1] || tempData[0] == tempData[2] || tempData[2] == tempData[1] )
				{
					m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}

	if(nKind == enWF_QiansanzusanDanshi)
	{
		string strGetHaoma;

		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";
		bool bSuit = true;
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
					m_singlezhushu = 0;
					break;
				}
				if (nSize != 3)
				{
					m_singlezhushu = 0;
					break;
				}

				if(tempData[0] == tempData[1] && tempData[0] == tempData[2] && tempData[2] == tempData[1] )
				{
					m_singlezhushu = 0;
					break;
				}
				if(tempData[0]!= tempData[1] && tempData[0] != tempData[2] && tempData[2] != tempData[1] )
				{
					m_singlezhushu = 0;
					break;
				}
				nZhushu = 1;
				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						bSuit = false;

						m_singlezhushu = 0;
						break;
					}
				}

				if (nSize > 3 || !bSuit)
				{
					m_singlezhushu = 0;
					break;
				}
				if (nSize != 3)
				{
					m_singlezhushu = 0;
					break;
				}
				if(tempData[0] == tempData[1] && tempData[0] == tempData[2] && tempData[2] == tempData[1] )
				{
					m_singlezhushu = 0;
					break;
				}
				if(tempData[0]!= tempData[1] && tempData[0] != tempData[2] && tempData[2] != tempData[1] )
				{
					m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}

	if(nKind == enWF_QianerzhixuanDanshi||nKind == enWF_HouerzhixuanDanshi)
	{
		string strGetHaoma;

		ChangeStringToA(strHaoma,strGetHaoma);

		string splitStr=" ";
		bool bSuit = true;
		size_t oldPos,newPos;
		oldPos=0;
		newPos=0;
		string tempData;
		while(1)
		{   
			int nZhushu = 0;
			newPos=strGetHaoma.find(splitStr,oldPos);
			if(newPos!=string::npos)
			{   
				tempData = strGetHaoma.substr(oldPos,newPos-oldPos);
				if(tempData.empty())
				{
					oldPos=newPos+splitStr.size();
					continue;
				}

				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						m_singlezhushu = 0;
						bSuit = false;

						break;
					}
				}

				if (nSize > 2 || !bSuit)
				{
					m_singlezhushu = 0;
					break;
				}
				if (nSize != 2)
				{
					m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;
				m_singlezhushu += nZhushu;

				oldPos=newPos+splitStr.size();
			}   
			else if(oldPos<=strGetHaoma.size())
			{   
				tempData= strGetHaoma.substr(oldPos);

				if(tempData.empty())
				{
					break;
				}
				int nSize = tempData.size();
				for(string::size_type i=0;i!=nSize;++i) 
				{
					if(!isdigit(tempData[i]))
					{
						bSuit = false;

						m_singlezhushu = 0;
						break;
					}
				}

				if (nSize > 2 || !bSuit)
				{
					m_singlezhushu = 0;
					break;
				}
				if (nSize != 2)
				{
					m_singlezhushu = 0;
					break;
				}

				nZhushu = 1;

				m_singlezhushu += nZhushu;
				oldPos= oldPos+strGetHaoma.size();
				break;
			}   
			else
			{   
				break;
			}   
		}   
	}

	CRect rcInvali;
	rcInvali.CopyRect(rcZongZhuShu);
	rcInvali.right+=200;
	InvalidateRect(&rcInvali);
	InvalidateRect(&rcZongJinE);

	return m_singlezhushu;

}
