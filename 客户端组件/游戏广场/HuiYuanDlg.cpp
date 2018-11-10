#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuandlg.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

static const int btn_top = 34;
static const int btn_x = 8;
static const int btn_space = 92;
// static const int btn_width = 151;
// static const int btn_height = 34;
//会员信息
static const int btn_huiyuanxinxi_x = btn_x;
//添加下级
static const int btn_tianjiaxiaji_x = btn_x + btn_space * 1;
//下级投注统计
static const int btn_xiajitouzhutongji_x = btn_x + btn_space * 2;
//查看下级投注
static const int btn_chakanxiajitouzhu_x = btn_x + btn_space * 3;
//会员数据
static const int btn_huiyuanshuju_x = btn_x + btn_space * 4;
//盈亏统计
static const int btn_yingkuitongji_x = btn_x + btn_space * 5;
//盈亏记录
static const int btn_yingkuijilu_x = btn_x + btn_space * 6;
//充值记录
static const int btn_chongzhijilu_x = btn_x + btn_space * 9;
//提现记录
static const int btn_tixianjilu_x = btn_x + btn_space * 10;
//下级游戏盈亏
static const int btn_youxiyingkui_x = btn_x + btn_space * 7;
//下级游戏记录
static const int btn_youxijilu_x = btn_x + btn_space * 8;

//子窗口数据
static const int bk_zhanghu_x = 11;
static const int bk_zhanghu_y = 68;
static const int bk_zhanghu_width = 1003;
static const int bk_zhanghu_height = 515;

//子窗口数据2
static const int bk_zhanghu_x2 = 11;
static const int bk_zhanghu_y2 = 68;
static const int bk_zhanghu_width2 = 1003;
static const int bk_zhanghu_height2 = 515;

IMPLEMENT_DYNAMIC(CHuiYuanDlg, CDialog)

CHuiYuanDlg::CHuiYuanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuiYuanDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_bmpTip(NULL)
{
	m_font.CreateFont(22,0,0,0,800,0,0,0,0,0,0,0,0,TEXT("微软雅黑"));

}

CHuiYuanDlg::~CHuiYuanDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
		m_bmpBk = NULL;
	}
	if (m_bmpTip != NULL)
	{
		delete m_bmpTip;
		m_bmpTip = NULL;
	}
}

void CHuiYuanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_HUIYUANXINXI, m_btnHuiYuanXinXi);
	DDX_Control(pDX, IDC_BTN_TIANJIAXIAJI, m_btnTianJiaXiaJi);
	DDX_Control(pDX, IDC_BTN_XIAJITOUZHUTONGJI, m_btnXiaJiTouZhuTongJi);
	DDX_Control(pDX, IDC_BTN_CHAKANXIAJITOUZHU, m_btnChaKanXiaJiTouZhu);
	DDX_Control(pDX, IDC_BTN_HUIYUANSHUJU, m_btnHuiYuanShuJu);
	DDX_Control(pDX, IDC_BTN_YINGKUITONGJI, m_btnYingKuiTongJi);
	DDX_Control(pDX, IDC_BTN_YINGKUIJILU, m_btnYingKuiJiLu);
	DDX_Control(pDX, IDC_BTN_CHONGZHIJILU, m_btnChongZhiJiLu);
	DDX_Control(pDX, IDC_BTN_TIXIANJILU, m_btnTiXianJiLu);
	DDX_Control(pDX, IDC_BTN_XJYXYJ, m_btnXjYouxiYk);
	DDX_Control(pDX, IDC_BTN_XJYXJL, m_btnXjYouxiJl);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
}


BEGIN_MESSAGE_MAP(CHuiYuanDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()

	ON_MESSAGE(IDM_QUERY_XJ_YKMX,OnQueryXJykmx)
	ON_MESSAGE(IDM_QUERY_XJ_TZH,OnQueryXJTzh)
	ON_MESSAGE(IDM_QUERY_XJ_YXYKMX,OnQueryXJYxYk)
	ON_BN_CLICKED(IDC_BTN_HUIYUANXINXI, &CHuiYuanDlg::OnBnClickedBtnHuiyuanxinxi)
	ON_BN_CLICKED(IDC_BTN_TIANJIAXIAJI, &CHuiYuanDlg::OnBnClickedBtnTianjiaxiaji)
	ON_BN_CLICKED(IDC_BTN_XIAJITOUZHUTONGJI, &CHuiYuanDlg::OnBnClickedBtnXiajitouzhutongji)
	ON_BN_CLICKED(IDC_BTN_CHAKANXIAJITOUZHU, &CHuiYuanDlg::OnBnClickedBtnChakanxiajitouzhu)
	ON_BN_CLICKED(IDC_BTN_HUIYUANSHUJU, &CHuiYuanDlg::OnBnClickedBtnHuiyuanshuju)
	ON_BN_CLICKED(IDC_BTN_YINGKUITONGJI, &CHuiYuanDlg::OnBnClickedBtnYingkuitongji)
	ON_BN_CLICKED(IDC_BTN_YINGKUIJILU, &CHuiYuanDlg::OnBnClickedBtnYingkuijilu)
	ON_BN_CLICKED(IDC_BTN_CHONGZHIJILU, &CHuiYuanDlg::OnBnClickedBtnChongzhijilu)
	ON_BN_CLICKED(IDC_BTN_TIXIANJILU, &CHuiYuanDlg::OnBnClickedBtnTixianjilu)
	ON_BN_CLICKED(IDC_BTN_XJYXYJ, &CHuiYuanDlg::OnBnClickedBtnXjYouXiYk)
	ON_BN_CLICKED(IDC_BTN_XJYXJL, &CHuiYuanDlg::OnBnClickedBtnXjYouXiJl)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CHuiYuanDlg::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CHuiYuandlg 消息处理程序
void CHuiYuanDlg::OnBnClickedBtnClose()
{
	CWnd* pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->PostMessage(IDM_RETURN_GAME,1,0);
	}

	return;
}

BOOL CHuiYuanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg.png"));
	m_bmpTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\tdgl.png"));

	InitBtns();
	m_btnClose.SetImage(CBmpUtil::GetExePath() + _T("skin\\return_bt.png"));

	m_dwTickCount=0;
	m_hyxxDlg.Create(CHuiYuanHyxxDlg::IDD, this);
	m_tjxjDlg.Create(CHuiYuanTjxjDlg::IDD, this);
	m_xjtzhtjDlg.Create(CHuiYuanXjtzhtjDlg::IDD, this);
	m_chkxjtzhDlg.Create(CHuiYuanChkxjtzhDlg::IDD, this);
	m_hyshjDlg.Create(CHuiYuanHyshjDlg::IDD, this);
	m_yktjDlg.Create(CHuiYuanYktjDlg::IDD, this);
	m_yklogDlg.Create(CHuiYuanYkLogDlg::IDD, this);
	m_chzhlogDlg.Create(CHuiYuanChzhLogDlg::IDD, this);
	m_txlogDlg.Create(CHuiYuanTxLogDlg::IDD, this);
	m_xjyxjlDlg.Create(CHuiYuanXjYouXiJlDlg::IDD, this);
	m_yxtjlDlg.Create(CHuiYuanYxtjDlg::IDD, this);

	m_hyxxDlg.ShowWindow(SW_HIDE);
	
	//OnBnClickedBtnHuiyuanshuju();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CHuiYuanDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		OnBnClickedBtnHuiyuanxinxi();
	}
	else
	{
		m_hyxxDlg.ShowWindow(SW_HIDE);
		m_xjtzhtjDlg.ShowWindow(SW_HIDE);
		m_chkxjtzhDlg.ShowWindow(SW_HIDE);
		m_hyshjDlg.ShowWindow(SW_HIDE);
		m_yktjDlg.ShowWindow(SW_HIDE);
		m_yklogDlg.ShowWindow(SW_HIDE);
		m_chzhlogDlg.ShowWindow(SW_HIDE);
		m_txlogDlg.ShowWindow(SW_HIDE);
		m_xjyxjlDlg.ShowWindow(SW_HIDE);
		m_yxtjlDlg.ShowWindow(SW_HIDE);
		if(m_hyxxDlg.IsWindowVisible())
		{
			m_hyxxDlg.OnShowWindow(FALSE,1);
		}
		else if(m_xjtzhtjDlg.IsWindowVisible())
		{
			m_xjtzhtjDlg.OnShowWindow(FALSE,1);
		}
		else if(m_chkxjtzhDlg.IsWindowVisible())
		{
			m_chkxjtzhDlg.OnShowWindow(FALSE,1);
		}
		else if(m_hyshjDlg.IsWindowVisible())
		{
			m_hyshjDlg.OnShowWindow(FALSE,1);
		}
		else if(m_yktjDlg.IsWindowVisible())
		{
			m_yktjDlg.OnShowWindow(FALSE,1);
		}
		else if(m_yklogDlg.IsWindowVisible())
		{
			m_yklogDlg.OnShowWindow(FALSE,1);
		}
		else if(m_chzhlogDlg.IsWindowVisible())
		{
			m_chzhlogDlg.OnShowWindow(FALSE,1);
		}
		else if(m_txlogDlg.IsWindowVisible())
		{
			m_txlogDlg.OnShowWindow(FALSE,1);
		}
		else if(m_xjyxjlDlg.IsWindowVisible())
		{
			m_xjyxjlDlg.OnShowWindow(FALSE,1);
		}
		else if(m_yxtjlDlg.IsWindowVisible())
		{
			m_yxtjlDlg.OnShowWindow(FALSE,1);
		}

	}
}

void CHuiYuanDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);

	Graphics graphics(dc.m_hDC);

	graphics.DrawImage(m_bmpBk, Rect(0, 0,  m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);
	graphics.DrawImage(m_bmpTip, Rect(16, 5,  m_bmpTip->GetWidth(), m_bmpTip->GetHeight()), 0, 0, m_bmpTip->GetWidth(), m_bmpTip->GetHeight(), UnitPixel);

	//dc.TextOut(300, 300, _T("会员管理"));
	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CHuiYuanDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	if(m_btnClose.GetSafeHwnd() != NULL)
	{
		m_btnClose.SetWindowPos(NULL, cx - m_btnClose.Width() - 11, -1, m_btnClose.Width(), m_btnClose.Height(), SWP_NOZORDER);
	}

	if(m_btnHuiYuanXinXi.GetSafeHwnd() != NULL)
	{
		m_btnHuiYuanXinXi.SetWindowPos(NULL, btn_huiyuanxinxi_x, btn_top, m_btnHuiYuanXinXi.Width(), m_btnHuiYuanXinXi.Height(), SWP_NOZORDER);
	}
	if(m_btnTianJiaXiaJi.GetSafeHwnd() != NULL)
	{
		m_btnTianJiaXiaJi.SetWindowPos(NULL, btn_tianjiaxiaji_x, btn_top, m_btnTianJiaXiaJi.Width(), m_btnTianJiaXiaJi.Height(), SWP_NOZORDER);
	}
	if(m_btnXiaJiTouZhuTongJi.GetSafeHwnd() != NULL)
	{
		m_btnXiaJiTouZhuTongJi.SetWindowPos(NULL, btn_xiajitouzhutongji_x, btn_top, m_btnXiaJiTouZhuTongJi.Width(), m_btnXiaJiTouZhuTongJi.Height(), SWP_NOZORDER);
	}
	if(m_btnChaKanXiaJiTouZhu.GetSafeHwnd() != NULL)
	{
		m_btnChaKanXiaJiTouZhu.SetWindowPos(NULL, btn_chakanxiajitouzhu_x, btn_top, m_btnChaKanXiaJiTouZhu.Width(), m_btnChaKanXiaJiTouZhu.Height(), SWP_NOZORDER);
	}
	if(m_btnHuiYuanShuJu.GetSafeHwnd() != NULL)
	{
		m_btnHuiYuanShuJu.SetWindowPos(NULL, btn_huiyuanshuju_x, btn_top, m_btnHuiYuanShuJu.Width(), m_btnHuiYuanShuJu.Height(), SWP_NOZORDER);
	}
	if(m_btnYingKuiTongJi.GetSafeHwnd() != NULL)
	{
		m_btnYingKuiTongJi.SetWindowPos(NULL, btn_yingkuitongji_x, btn_top, m_btnYingKuiTongJi.Width(), m_btnYingKuiTongJi.Height(), SWP_NOZORDER);
	}
	if(m_btnYingKuiJiLu.GetSafeHwnd() != NULL)
	{
		m_btnYingKuiJiLu.SetWindowPos(NULL, btn_yingkuijilu_x, btn_top, m_btnYingKuiJiLu.Width(), m_btnYingKuiJiLu.Height(), SWP_NOZORDER);
	}
	if(m_btnChongZhiJiLu.GetSafeHwnd() != NULL)
	{
		m_btnChongZhiJiLu.SetWindowPos(NULL, btn_chongzhijilu_x, btn_top, m_btnChongZhiJiLu.Width(), m_btnChongZhiJiLu.Height(), SWP_NOZORDER);
		//m_btnChongZhiJiLu.ShowWindow(SW_HIDE);
	}
	if(m_btnTiXianJiLu.GetSafeHwnd() != NULL)
	{
		m_btnTiXianJiLu.SetWindowPos(NULL, btn_tixianjilu_x, btn_top, m_btnTiXianJiLu.Width(), m_btnTiXianJiLu.Height(), SWP_NOZORDER);
		//m_btnTiXianJiLu.ShowWindow(SW_HIDE);
	}

	if(m_btnXjYouxiYk.GetSafeHwnd() != NULL)
	{
		m_btnXjYouxiYk.SetWindowPos(NULL, btn_youxiyingkui_x, btn_top, m_btnXjYouxiYk.Width(), m_btnXjYouxiYk.Height(), SWP_NOZORDER);
	}
	if(m_btnXjYouxiJl.GetSafeHwnd() != NULL)
	{
		m_btnXjYouxiJl.SetWindowPos(NULL, btn_youxijilu_x, btn_top, m_btnXjYouxiJl.Width(), m_btnXjYouxiJl.Height(), SWP_NOZORDER);
	}

	if (m_hyxxDlg.GetSafeHwnd() != NULL)
	{
		m_hyxxDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}
	if (m_tjxjDlg.GetSafeHwnd() != NULL)
	{
		m_tjxjDlg.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}
	if (m_xjtzhtjDlg.GetSafeHwnd() != NULL)
	{
		m_xjtzhtjDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}
	if (m_chkxjtzhDlg.GetSafeHwnd() != NULL)
	{
		m_chkxjtzhDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}
	if (m_hyshjDlg.GetSafeHwnd() != NULL)
	{
		m_hyshjDlg.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}
	if (m_yktjDlg.GetSafeHwnd() != NULL)
	{
		m_yktjDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}
	if (m_yklogDlg.GetSafeHwnd() != NULL)
	{
		m_yklogDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}
	if (m_chzhlogDlg.GetSafeHwnd() != NULL)
	{
		m_chzhlogDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}
	if (m_txlogDlg.GetSafeHwnd() != NULL)
	{
		m_txlogDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}
	if (m_xjyxjlDlg.GetSafeHwnd() != NULL)
	{
		m_xjyxjlDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}
	if (m_yxtjlDlg.GetSafeHwnd() != NULL)
	{
		m_yxtjlDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}
}

void CHuiYuanDlg::OnBnClickedBtnHuiyuanxinxi()
{
	if(m_dwTickCount == 0)
	{
		m_dwTickCount = GetTickCount();
	}
	else if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(true);
	m_btnTianJiaXiaJi.SetPushed(false);
	m_btnXiaJiTouZhuTongJi.SetPushed(false);
	m_btnChaKanXiaJiTouZhu.SetPushed(false);
	m_btnHuiYuanShuJu.SetPushed(false);
	m_btnYingKuiTongJi.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnXjYouxiYk.SetPushed(false);
	m_btnXjYouxiJl.SetPushed(false);

	m_tjxjDlg.ShowWindow(SW_HIDE);
	m_xjtzhtjDlg.ShowWindow(SW_HIDE);
	m_chkxjtzhDlg.ShowWindow(SW_HIDE);
	m_hyshjDlg.ShowWindow(SW_HIDE);
	m_yktjDlg.ShowWindow(SW_HIDE);
	m_yklogDlg.ShowWindow(SW_HIDE);
	m_chzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_xjyxjlDlg.ShowWindow(SW_HIDE);
	m_yxtjlDlg.ShowWindow(SW_HIDE);
	m_hyxxDlg.ShowWindow(SW_SHOW);

// 	m_hyxxDlg.OnShowWindow(true,1);
// 	m_tjxjDlg.OnShowWindow(false,1);
// 	m_xjtzhtjDlg.OnShowWindow(false,1);
// 	m_chkxjtzhDlg.OnShowWindow(false,1);
// 	m_hyshjDlg.OnShowWindow(false,1);
// 	m_yktjDlg.OnShowWindow(false,1);
// 	m_yklogDlg.OnShowWindow(false,1);
// 	m_chzhlogDlg.OnShowWindow(false,1);
// 	m_txlogDlg.OnShowWindow(false,1);
// 	m_xjyxjlDlg.OnShowWindow(false,1);
// 	m_yxtjlDlg.OnShowWindow(false,1);

}

void CHuiYuanDlg::OnBnClickedBtnTianjiaxiaji()
{
	if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(false);
	m_btnTianJiaXiaJi.SetPushed(true);
	m_btnXiaJiTouZhuTongJi.SetPushed(false);
	m_btnChaKanXiaJiTouZhu.SetPushed(false);
	m_btnHuiYuanShuJu.SetPushed(false);
	m_btnYingKuiTongJi.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnXjYouxiYk.SetPushed(false);
	m_btnXjYouxiJl.SetPushed(false);

	m_hyxxDlg.ShowWindow(SW_HIDE);
	m_tjxjDlg.ShowWindow(SW_SHOW);
	m_xjtzhtjDlg.ShowWindow(SW_HIDE);
	m_chkxjtzhDlg.ShowWindow(SW_HIDE);
	m_hyshjDlg.ShowWindow(SW_HIDE);
	m_yktjDlg.ShowWindow(SW_HIDE);
	m_yklogDlg.ShowWindow(SW_HIDE);
	m_chzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_xjyxjlDlg.ShowWindow(SW_HIDE);
	m_yxtjlDlg.ShowWindow(SW_HIDE);
}

void CHuiYuanDlg::OnBnClickedBtnXiajitouzhutongji()
{
	if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(false);
	m_btnTianJiaXiaJi.SetPushed(false);
	m_btnXiaJiTouZhuTongJi.SetPushed(true);
	m_btnChaKanXiaJiTouZhu.SetPushed(false);
	m_btnHuiYuanShuJu.SetPushed(false);
	m_btnYingKuiTongJi.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnXjYouxiYk.SetPushed(false);
	m_btnXjYouxiJl.SetPushed(false);

	m_hyxxDlg.ShowWindow(SW_HIDE);
	m_tjxjDlg.ShowWindow(SW_HIDE);
	m_xjtzhtjDlg.ShowWindow(SW_SHOW);
	m_chkxjtzhDlg.ShowWindow(SW_HIDE);
	m_hyshjDlg.ShowWindow(SW_HIDE);
	m_yktjDlg.ShowWindow(SW_HIDE);
	m_yklogDlg.ShowWindow(SW_HIDE);
	m_chzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_xjyxjlDlg.ShowWindow(SW_HIDE);
	m_yxtjlDlg.ShowWindow(SW_HIDE);
}

void CHuiYuanDlg::OnBnClickedBtnChakanxiajitouzhu()
{
	if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(false);
	m_btnTianJiaXiaJi.SetPushed(false);
	m_btnXiaJiTouZhuTongJi.SetPushed(false);
	m_btnChaKanXiaJiTouZhu.SetPushed(true);
	m_btnHuiYuanShuJu.SetPushed(false);
	m_btnYingKuiTongJi.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnXjYouxiYk.SetPushed(false);
	m_btnXjYouxiJl.SetPushed(false);

	m_hyxxDlg.ShowWindow(SW_HIDE);
	m_tjxjDlg.ShowWindow(SW_HIDE);
	m_xjtzhtjDlg.ShowWindow(SW_HIDE);
	m_chkxjtzhDlg.ShowWindow(SW_SHOW);
	m_hyshjDlg.ShowWindow(SW_HIDE);
	m_yktjDlg.ShowWindow(SW_HIDE);
	m_yklogDlg.ShowWindow(SW_HIDE);
	m_chzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_xjyxjlDlg.ShowWindow(SW_HIDE);
	m_yxtjlDlg.ShowWindow(SW_HIDE);
}

void CHuiYuanDlg::OnBnClickedBtnHuiyuanshuju()
{
	if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(false);
	m_btnTianJiaXiaJi.SetPushed(false);
	m_btnXiaJiTouZhuTongJi.SetPushed(false);
	m_btnChaKanXiaJiTouZhu.SetPushed(false);
	m_btnHuiYuanShuJu.SetPushed(true);
	m_btnYingKuiTongJi.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnXjYouxiYk.SetPushed(false);
	m_btnXjYouxiJl.SetPushed(false);


	m_hyxxDlg.ShowWindow(SW_HIDE);
	m_tjxjDlg.ShowWindow(SW_HIDE);
	m_xjtzhtjDlg.ShowWindow(SW_HIDE);
	m_chkxjtzhDlg.ShowWindow(SW_HIDE);
	m_hyshjDlg.ShowWindow(SW_SHOW);
	m_yktjDlg.ShowWindow(SW_HIDE);
	m_yklogDlg.ShowWindow(SW_HIDE);
	m_chzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_xjyxjlDlg.ShowWindow(SW_HIDE);
	m_yxtjlDlg.ShowWindow(SW_HIDE);
}

void CHuiYuanDlg::OnBnClickedBtnYingkuitongji()
{
	if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(false);
	m_btnTianJiaXiaJi.SetPushed(false);
	m_btnXiaJiTouZhuTongJi.SetPushed(false);
	m_btnChaKanXiaJiTouZhu.SetPushed(false);
	m_btnHuiYuanShuJu.SetPushed(false);
	m_btnYingKuiTongJi.SetPushed(true);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnXjYouxiYk.SetPushed(false);
	m_btnXjYouxiJl.SetPushed(false);

	m_hyxxDlg.ShowWindow(SW_HIDE);
	m_tjxjDlg.ShowWindow(SW_HIDE);
	m_xjtzhtjDlg.ShowWindow(SW_HIDE);
	m_chkxjtzhDlg.ShowWindow(SW_HIDE);
	m_hyshjDlg.ShowWindow(SW_HIDE);
	m_yktjDlg.ShowWindow(SW_SHOW);
	m_yklogDlg.ShowWindow(SW_HIDE);
	m_chzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_xjyxjlDlg.ShowWindow(SW_HIDE);
	m_yxtjlDlg.ShowWindow(SW_HIDE);
}

void CHuiYuanDlg::OnBnClickedBtnYingkuijilu()
{
	if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(false);
	m_btnTianJiaXiaJi.SetPushed(false);
	m_btnXiaJiTouZhuTongJi.SetPushed(false);
	m_btnChaKanXiaJiTouZhu.SetPushed(false);
	m_btnHuiYuanShuJu.SetPushed(false);
	m_btnYingKuiTongJi.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(true);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnXjYouxiYk.SetPushed(false);
	m_btnXjYouxiJl.SetPushed(false);

	m_hyxxDlg.ShowWindow(SW_HIDE);
	m_tjxjDlg.ShowWindow(SW_HIDE);
	m_xjtzhtjDlg.ShowWindow(SW_HIDE);
	m_chkxjtzhDlg.ShowWindow(SW_HIDE);
	m_hyshjDlg.ShowWindow(SW_HIDE);
	m_yktjDlg.ShowWindow(SW_HIDE);
	m_yklogDlg.ShowWindow(SW_SHOW);
	m_chzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_xjyxjlDlg.ShowWindow(SW_HIDE);
	m_yxtjlDlg.ShowWindow(SW_HIDE);
}

void CHuiYuanDlg::OnBnClickedBtnChongzhijilu()
{
	if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(false);
	m_btnTianJiaXiaJi.SetPushed(false);
	m_btnXiaJiTouZhuTongJi.SetPushed(false);
	m_btnChaKanXiaJiTouZhu.SetPushed(false);
	m_btnHuiYuanShuJu.SetPushed(false);
	m_btnYingKuiTongJi.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(true);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnXjYouxiYk.SetPushed(false);
	m_btnXjYouxiJl.SetPushed(false);

	m_hyxxDlg.ShowWindow(SW_HIDE);
	m_tjxjDlg.ShowWindow(SW_HIDE);
	m_xjtzhtjDlg.ShowWindow(SW_HIDE);
	m_chkxjtzhDlg.ShowWindow(SW_HIDE);
	m_hyshjDlg.ShowWindow(SW_HIDE);
	m_yktjDlg.ShowWindow(SW_HIDE);
	m_yklogDlg.ShowWindow(SW_HIDE);
	m_chzhlogDlg.ShowWindow(SW_SHOW);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_xjyxjlDlg.ShowWindow(SW_HIDE);
	m_yxtjlDlg.ShowWindow(SW_HIDE);
}

void CHuiYuanDlg::OnBnClickedBtnTixianjilu()
{
	if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(false);
	m_btnTianJiaXiaJi.SetPushed(false);
	m_btnXiaJiTouZhuTongJi.SetPushed(false);
	m_btnChaKanXiaJiTouZhu.SetPushed(false);
	m_btnHuiYuanShuJu.SetPushed(false);
	m_btnYingKuiTongJi.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(true);
	m_btnXjYouxiYk.SetPushed(false);
	m_btnXjYouxiJl.SetPushed(false);

	m_hyxxDlg.ShowWindow(SW_HIDE);
	m_tjxjDlg.ShowWindow(SW_HIDE);
	m_xjtzhtjDlg.ShowWindow(SW_HIDE);
	m_chkxjtzhDlg.ShowWindow(SW_HIDE);
	m_hyshjDlg.ShowWindow(SW_HIDE);
	m_yktjDlg.ShowWindow(SW_HIDE);
	m_yklogDlg.ShowWindow(SW_HIDE);
	m_chzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_SHOW);
	m_xjyxjlDlg.ShowWindow(SW_HIDE);
	m_yxtjlDlg.ShowWindow(SW_HIDE);
}
void CHuiYuanDlg::OnBnClickedBtnXjYouXiYk()
{
	if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(false);
	m_btnTianJiaXiaJi.SetPushed(false);
	m_btnXiaJiTouZhuTongJi.SetPushed(false);
	m_btnChaKanXiaJiTouZhu.SetPushed(false);
	m_btnHuiYuanShuJu.SetPushed(false);
	m_btnYingKuiTongJi.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnXjYouxiYk.SetPushed(true);
	m_btnXjYouxiJl.SetPushed(false);

	m_hyxxDlg.ShowWindow(SW_HIDE);
	m_tjxjDlg.ShowWindow(SW_HIDE);
	m_xjtzhtjDlg.ShowWindow(SW_HIDE);
	m_chkxjtzhDlg.ShowWindow(SW_HIDE);
	m_hyshjDlg.ShowWindow(SW_HIDE);
	m_yktjDlg.ShowWindow(SW_HIDE);
	m_yklogDlg.ShowWindow(SW_HIDE);
	m_chzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_xjyxjlDlg.ShowWindow(SW_HIDE);
	m_yxtjlDlg.ShowWindow(SW_SHOW);
}

void CHuiYuanDlg::OnBnClickedBtnXjYouXiJl()
{
	if( GetTickCount()-m_dwTickCount <10)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnHuiYuanXinXi.SetPushed(false);
	m_btnTianJiaXiaJi.SetPushed(false);
	m_btnXiaJiTouZhuTongJi.SetPushed(false);
	m_btnChaKanXiaJiTouZhu.SetPushed(false);
	m_btnHuiYuanShuJu.SetPushed(false);
	m_btnYingKuiTongJi.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnXjYouxiYk.SetPushed(false);
	m_btnXjYouxiJl.SetPushed(true);

	m_hyxxDlg.ShowWindow(SW_HIDE);
	m_tjxjDlg.ShowWindow(SW_HIDE);
	m_xjtzhtjDlg.ShowWindow(SW_HIDE);
	m_chkxjtzhDlg.ShowWindow(SW_HIDE);
	m_hyshjDlg.ShowWindow(SW_HIDE);
	m_yktjDlg.ShowWindow(SW_HIDE);
	m_yklogDlg.ShowWindow(SW_HIDE);
	m_chzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_xjyxjlDlg.ShowWindow(SW_SHOW);
	m_yxtjlDlg.ShowWindow(SW_HIDE);
}

void CHuiYuanDlg::InitBtns()
{
	CString str = CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk1.png");
	m_btnHuiYuanXinXi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\HuiYuanXinXi.png"));
	m_btnTianJiaXiaJi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\TianJiaXiaJi.png"));
	m_btnXiaJiTouZhuTongJi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\XiaJiTouZhuTongJi.png"));
	m_btnChaKanXiaJiTouZhu.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\ChaKanXiaJiTouZhu.png"));
	m_btnHuiYuanShuJu.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\HuiYuanShuJu.png"));
	m_btnYingKuiTongJi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\YingKuiTongJi.png"));
	m_btnYingKuiJiLu.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\YingKuiJiLu.png"));
	m_btnChongZhiJiLu.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\ChongZhiJiLu.png"));
	m_btnTiXianJiLu.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\TiXianJiLu.png"));
	m_btnXjYouxiYk.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\XjYouxiYk.png"));
	m_btnXjYouxiJl.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\XjYouxiJl.png"));

	m_btnHuiYuanXinXi.SetTextFont(&m_font);
	m_btnTianJiaXiaJi.SetTextFont(&m_font);
	m_btnXiaJiTouZhuTongJi.SetTextFont(&m_font);
	m_btnChaKanXiaJiTouZhu.SetTextFont(&m_font);
	m_btnHuiYuanShuJu.SetTextFont(&m_font);
	m_btnYingKuiTongJi.SetTextFont(&m_font);
	m_btnYingKuiJiLu.SetTextFont(&m_font);
	m_btnChongZhiJiLu.SetTextFont(&m_font);
	m_btnTiXianJiLu.SetTextFont(&m_font);
	m_btnXjYouxiYk.SetTextFont(&m_font);
	m_btnXjYouxiJl.SetTextFont(&m_font);

}

void CHuiYuanDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CHuiYuanDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}


LRESULT CHuiYuanDlg::OnQueryXJykmx(WPARAM wParam, LPARAM lParam)
{
	int n_t_id = (int)wParam;

	OnBnClickedBtnYingkuijilu();
	m_yklogDlg.QueryXJYKMX(n_t_id);
	return 1;
}
LRESULT CHuiYuanDlg::OnQueryXJTzh(WPARAM wParam, LPARAM lParam)
{
	int n_t_id = (int)wParam;

	OnBnClickedBtnChakanxiajitouzhu();
	m_chkxjtzhDlg.QueryChkXjTzh(n_t_id);
	return 1;
}
LRESULT CHuiYuanDlg::OnQueryXJYxYk(WPARAM wParam, LPARAM lParam)
{
	int n_t_id = (int)wParam;

	OnBnClickedBtnXjYouXiJl();
	m_xjyxjlDlg.QueryChkXjYxYk(n_t_id);
	return 1;
}
