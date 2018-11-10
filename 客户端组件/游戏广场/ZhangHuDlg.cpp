#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhanghuDlg.h"
#include "BmpUtil.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

static const int btn_top = 34;
static const int btn_x = 8;
static const int btn_space = 92;
// static const int btn_width = 151;
// static const int btn_height = 34;
//基本信息
static const int btn_baseinfo_x = btn_x;
static const int btn_baseinfo_y = btn_top;
// static const int btn_baseinfo_width = btn_width;
// static const int btn_baseinfo_height = btn_height;

//修改密码
static const int btn_modifypwd_x = btn_x+ btn_space * 1;
static const int btn_modifypwd_y = btn_top ;
// static const int btn_modifypwd_width = btn_width;
// static const int btn_modifypwd_height = btn_height;

//设置账号保护
static const int btn_zhanghaobaohu_x = btn_x + btn_space * 2;
static const int btn_zhanghaobaohu_y = btn_top;
// static const int btn_zhanghaobaohu_width = btn_width;
// static const int btn_zhanghaobaohu_height = btn_height;

//设置提款账号
static const int btn_tikuanzhanghao_x = btn_x + btn_space * 3;
static const int btn_tikuanzhanghao_y = btn_top;
// static const int btn_tikuanzhanghao_width = btn_width;
// static const int btn_tikuanzhanghao_height = btn_height;

//投注记录
static const int btn_touzhujilu_x = btn_x + btn_space * 4;
static const int btn_touzhujilu_y = btn_top;
// static const int btn_touzhujilu_width = btn_width;
// static const int btn_touzhujilu_height = btn_height;

//提现记录
static const int btn_tixianjilu_x = btn_x+ btn_space * 5;
static const int btn_tixianjilu_y = btn_top ;
// static const int btn_tixianjilu_width = btn_width;
// static const int btn_tixianjilu_height = btn_height;

//充值记录
static const int btn_chongzhijilu_x = btn_x+ btn_space * 6;
static const int btn_chongzhijilu_y = btn_top ;
// static const int btn_chongzhijilu_width = btn_width;
// static const int btn_chongzhijilu_height = btn_height;

//盈亏记录
static const int btn_yingkuijilu_x = btn_x + btn_space * 7;
static const int btn_yingkuijilu_y = btn_top;
// static const int btn_yingkuijilu_width = btn_width;
// static const int btn_yingkuijilu_height = btn_height;
//盈亏明细
static const int btn_yingkuimingxi_x = btn_x + btn_space * 8;
static const int btn_yingkuimingxi_y = btn_top;

//棋牌盈亏
static const int btn_qipaiyingkui_x = btn_x + btn_space * 9;
static const int btn_qipaiyingkui_y = btn_top;

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


IMPLEMENT_DYNAMIC(CZhangHuDlg, CDialog)

CZhangHuDlg::CZhangHuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZhangHuDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_bmpTip(NULL)
	, m_showType(BASEINFO)
{
	m_font.CreateFont(22,0,0,0,800,0,0,0,0,0,0,0,0,TEXT("微软雅黑"));
}

CZhangHuDlg::~CZhangHuDlg()
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

void CZhangHuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_BASEINFO, m_btnBaseInfo);
	DDX_Control(pDX, IDC_BTN_MODIFYPWD, m_btnModifyPwd);
	DDX_Control(pDX, IDC_BTN_ACCOUNTBAOHU, m_btnZhangHaoBaoHu);
	DDX_Control(pDX, IDC_BTN_TIKUANACT, m_btnTiKuanZhangHao);
	DDX_Control(pDX, IDC_BTN_TOUZHUJILU, m_btnTouZhuJiLu);
	DDX_Control(pDX, IDC_BTN_TIXIANJILU, m_btnTiXianJiLu);
	DDX_Control(pDX, IDC_BTN_CHONGZHIJILU, m_btnChongZhiJiLu);
	DDX_Control(pDX, IDC_BTN_YINGKUIJILU, m_btnYingKuiJiLu);
	DDX_Control(pDX, IDC_BTN_YINGKUIMINGXI, m_btnYingKuiMingXi);
	DDX_Control(pDX, IDC_BTN_QIPAI_YINGKUI, m_btnQipaiYingkui);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
}


BEGIN_MESSAGE_MAP(CZhangHuDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_BASEINFO, &CZhangHuDlg::OnBnClickedBtnBaseinfo)
	ON_BN_CLICKED(IDC_BTN_MODIFYPWD, &CZhangHuDlg::OnBnClickedBtnModifypwd)
	ON_BN_CLICKED(IDC_BTN_ACCOUNTBAOHU, &CZhangHuDlg::OnBnClickedBtnAccountbaohu)
	ON_BN_CLICKED(IDC_BTN_TIKUANACT, &CZhangHuDlg::OnBnClickedBtnTikuanact)
	ON_BN_CLICKED(IDC_BTN_TOUZHUJILU, &CZhangHuDlg::OnBnClickedBtnTouzhujilu)
	ON_BN_CLICKED(IDC_BTN_TIXIANJILU, &CZhangHuDlg::OnBnClickedBtnTixianjilu)
	ON_BN_CLICKED(IDC_BTN_CHONGZHIJILU, &CZhangHuDlg::OnBnClickedBtnChongzhijilu)
	ON_BN_CLICKED(IDC_BTN_YINGKUIJILU, &CZhangHuDlg::OnBnClickedBtnYingkuijilu)
	ON_BN_CLICKED(IDC_BTN_YINGKUIMINGXI, &CZhangHuDlg::OnBnClickedBtnYingkuimingxi)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CZhangHuDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_QIPAI_YINGKUI, &CZhangHuDlg::OnBnClickedBtnQipaiYingkui)
	ON_MESSAGE(RELEASE_FACE,&CZhangHuDlg::OnReleaseFace)

	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CZhanghuDlg 消息处理程序
void CZhangHuDlg::OnBnClickedBtnClose()
{
	CWnd* pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->PostMessage(IDM_RETURN_GAME,1,0);
	}

	return;
}
LRESULT CZhangHuDlg::OnReleaseFace(WPARAM wParam, LPARAM lParam)
{
	CWnd* pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->PostMessage(IDM_RELEASE_FACE,1,0);
	}
	return TRUE;
}
BOOL CZhangHuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg.png"));
	m_bmpTip = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\zhgl.png"));

	InitBtns();
	m_btnClose.SetImage(CBmpUtil::GetExePath() + _T("skin\\return_bt.png"));

	m_baseInfoDlg.Create(CZhangHuBaseInfoDlg::IDD, this);
	m_xgmmDlg.Create(CZhangHuXgmmDlg::IDD, this);
	m_zhhbhDlg.Create(CZhangHuZhhbhDlg::IDD, this);
	m_shzhTkzhh.Create(CZhangHuShZhTkZhhDlg::IDD, this);
	m_tzhlogDlg.Create(CZhangHuTzhLogDlg::IDD, this);
	m_txlogDlg.Create(CZhangHuTxLogDlg::IDD, this);
	m_chzhlogdlg.Create(CZhangHuChzhLogDlg::IDD, this);
	m_yklogdlg.Create(CZhangHuYkLogDlg::IDD, this);
	m_ykmxdlg.Create(CZhangHuYkMXDlg::IDD, this);
	m_qpykdlg.Create(CZhangHuQPYkDlg::IDD, this);

	m_dwTickCount = 0;

	SetWindowPos(NULL,0,0,m_bmpBk->GetWidth(),m_bmpBk->GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);

	//OnBnClickedBtnBaseinfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CZhangHuDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CZhangHuDlg::OnOK()
{
	//CDialog::OnOK();
}

void CZhangHuDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);

	Graphics graphics(dc.m_hDC);

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);
	graphics.DrawImage(m_bmpTip, Rect(16, 5, m_bmpTip->GetWidth(), m_bmpTip->GetHeight()), 0, 0, m_bmpTip->GetWidth(), m_bmpTip->GetHeight(), UnitPixel);

	//switch (m_showType)
	//{
	//case BASEINFO:
	//	//dc.TextOut(300, 300, _T("基本信息"));
	//	break;
	//case MODIFYPWD:
	//	//dc.TextOut(300, 300, _T("修改密码"));
	//	break;
	//case ZHANGHAOBAOHU:
	//	dc.TextOut(300, 300, _T("设置账号保护"));
	//	break;
	//case TIXIANZHANGHU:
	//	dc.TextOut(300, 300, _T("设置体现账户"));
	//	break;
	//case TOUZHUJILU:
	//	dc.TextOut(300, 300, _T("投注记录"));
	//	break;
	//case TIXIANJILU:
	//	dc.TextOut(300, 300, _T("提现记录"));
	//	break;
	//case CHONGZHIJILU:
	//	dc.TextOut(300, 300, _T("充值记录"));
	//	break;
	//case YINGKUIJILU:
	//	dc.TextOut(300, 300, _T("盈亏记录"));
	//	break;
	//default:
	//	break;
	//}
	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CZhangHuDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CZhangHuDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	if(m_btnClose.GetSafeHwnd() != NULL)
	{
		m_btnClose.SetWindowPos(NULL, cx - m_btnClose.Width() - 9, -1, m_btnClose.Width(), m_btnClose.Height(), SWP_NOZORDER);
	}

	if(m_btnBaseInfo.GetSafeHwnd() != NULL)
	{
		m_btnBaseInfo.SetWindowPos(NULL, btn_baseinfo_x, btn_baseinfo_y, m_btnBaseInfo.Width(), m_btnBaseInfo.Height(), SWP_NOZORDER);
	}

	if(m_btnModifyPwd.GetSafeHwnd() != NULL)
	{
		m_btnModifyPwd.SetWindowPos(NULL, btn_modifypwd_x, btn_modifypwd_y, m_btnModifyPwd.Width(), m_btnModifyPwd.Height(), SWP_NOZORDER);
	}

	if(m_btnZhangHaoBaoHu.GetSafeHwnd() != NULL)
	{
		m_btnZhangHaoBaoHu.SetWindowPos(NULL, btn_zhanghaobaohu_x, btn_zhanghaobaohu_y, m_btnZhangHaoBaoHu.Width(), m_btnZhangHaoBaoHu.Height(), SWP_NOZORDER);
	}

	if(m_btnTiKuanZhangHao.GetSafeHwnd() != NULL)
	{
		m_btnTiKuanZhangHao.SetWindowPos(NULL, btn_tikuanzhanghao_x, btn_tikuanzhanghao_y, m_btnTiKuanZhangHao.Width(), m_btnTiKuanZhangHao.Height(), SWP_NOZORDER);
	}

	if(m_btnTouZhuJiLu.GetSafeHwnd() != NULL)
	{
		m_btnTouZhuJiLu.SetWindowPos(NULL, btn_touzhujilu_x, btn_touzhujilu_y, m_btnTouZhuJiLu.Width(), m_btnTouZhuJiLu.Height(), SWP_NOZORDER);
		//m_btnTouZhuJiLu.ShowWindow(SW_HIDE);
	}

	if(m_btnTiXianJiLu.GetSafeHwnd() != NULL)
	{
		m_btnTiXianJiLu.SetWindowPos(NULL, btn_tixianjilu_x, btn_tixianjilu_y, m_btnTiXianJiLu.Width(), m_btnTiXianJiLu.Height(), SWP_NOZORDER);
	}

	if(m_btnChongZhiJiLu.GetSafeHwnd() != NULL)
	{
		m_btnChongZhiJiLu.SetWindowPos(NULL, btn_chongzhijilu_x, btn_chongzhijilu_y, m_btnChongZhiJiLu.Width(), m_btnChongZhiJiLu.Height(), SWP_NOZORDER);
	}

	if(m_btnYingKuiJiLu.GetSafeHwnd() != NULL)
	{
		m_btnYingKuiJiLu.SetWindowPos(NULL, btn_yingkuijilu_x, btn_yingkuijilu_y, m_btnYingKuiJiLu.Width(), m_btnYingKuiJiLu.Height(), SWP_NOZORDER);
	}

	if(m_btnYingKuiMingXi.GetSafeHwnd() != NULL)
	{
		m_btnYingKuiMingXi.SetWindowPos(NULL, btn_yingkuimingxi_x, btn_yingkuimingxi_y, m_btnYingKuiMingXi.Width(), m_btnYingKuiMingXi.Height(), SWP_NOZORDER);
	}

	if(m_btnQipaiYingkui.GetSafeHwnd() != NULL)
	{
		m_btnQipaiYingkui.SetWindowPos(NULL, btn_qipaiyingkui_x, btn_qipaiyingkui_y, m_btnQipaiYingkui.Width(), m_btnQipaiYingkui.Height(), SWP_NOZORDER);
	}

	if(m_baseInfoDlg.GetSafeHwnd() != NULL)
	{
		m_baseInfoDlg.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}

	if(m_ykmxdlg.GetSafeHwnd() != NULL)
	{
		m_ykmxdlg.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}

	if(m_qpykdlg.GetSafeHwnd() != NULL)
	{
		m_qpykdlg.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}

	if(m_xgmmDlg.GetSafeHwnd() != NULL)
	{
		m_xgmmDlg.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}

	if(m_zhhbhDlg.GetSafeHwnd() != NULL)
	{
		m_zhhbhDlg.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}

	if (m_shzhTkzhh.GetSafeHwnd() != NULL)
	{
		m_shzhTkzhh.SetWindowPos(NULL, bk_zhanghu_x, bk_zhanghu_y, bk_zhanghu_width, bk_zhanghu_height, SWP_NOZORDER);
	}

	if (m_tzhlogDlg.GetSafeHwnd() != NULL)
	{
		m_tzhlogDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
		//m_tzhlogDlg.ShowWindow(SW_HIDE);
	}

	if (m_txlogDlg.GetSafeHwnd() != NULL)
	{
		m_txlogDlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}

	if (m_chzhlogdlg.GetSafeHwnd() != NULL)
	{
		m_chzhlogdlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}

	if (m_yklogdlg.GetSafeHwnd() != NULL)
	{
		m_yklogdlg.SetWindowPos(NULL, bk_zhanghu_x2, bk_zhanghu_y2, bk_zhanghu_width2, bk_zhanghu_height2, SWP_NOZORDER);
	}
}

void CZhangHuDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow) 
	{
//		static bool first = true;
//		if (first) 
		{
			ResetDlg();
			OnBnClickedBtnBaseinfo();
			//OnBnClickedBtnTouzhujilu();
		//	first = false;
		}
	}
	else
	{
		ResetDlg();
	}

}

void CZhangHuDlg::ResetDlg()
{
	m_btnBaseInfo.SetPushed(false);
	m_btnModifyPwd.SetPushed(false);
	m_btnZhangHaoBaoHu.SetPushed(false);
	m_btnTiKuanZhangHao.SetPushed(false);
	m_btnTouZhuJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnYingKuiMingXi.SetPushed(false);
	m_btnQipaiYingkui.SetPushed(false);

	m_showType = BASEINFO;
	m_baseInfoDlg.ShowWindow(SW_HIDE);
	m_xgmmDlg.ShowWindow(SW_HIDE);
	m_zhhbhDlg.ShowWindow(SW_HIDE);
	m_shzhTkzhh.ShowWindow(SW_HIDE);
	m_tzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_chzhlogdlg.ShowWindow(SW_HIDE);
	m_yklogdlg.ShowWindow(SW_HIDE);
	m_ykmxdlg.ShowWindow(SW_HIDE);
	m_qpykdlg.ShowWindow(SW_HIDE);
}

void CZhangHuDlg::OnBnClickedBtnBaseinfo()
{
	if(m_dwTickCount == 0)
	{
		m_dwTickCount = GetTickCount();
	}
	else if( GetTickCount()-m_dwTickCount <100)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();
	m_btnBaseInfo.SetPushed(true);
	m_btnModifyPwd.SetPushed(false);
	m_btnZhangHaoBaoHu.SetPushed(false);
	m_btnTiKuanZhangHao.SetPushed(false);
	m_btnTouZhuJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnYingKuiMingXi.SetPushed(false);
	m_btnQipaiYingkui.SetPushed(false);

	m_showType = BASEINFO;
	m_baseInfoDlg.ShowWindow(SW_SHOW);
	m_xgmmDlg.ShowWindow(SW_HIDE);
	m_zhhbhDlg.ShowWindow(SW_HIDE);
	m_shzhTkzhh.ShowWindow(SW_HIDE);
	m_tzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_chzhlogdlg.ShowWindow(SW_HIDE);
	m_yklogdlg.ShowWindow(SW_HIDE);
	m_ykmxdlg.ShowWindow(SW_HIDE);
	m_qpykdlg.ShowWindow(SW_HIDE);
}

void CZhangHuDlg::OnBnClickedBtnModifypwd()
{
// 	if( GetTickCount()-m_dwTickCount <100)
// 	{
// 		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
// 		return;
// 	}
	m_dwTickCount = GetTickCount();

	m_btnBaseInfo.SetPushed(false);
	m_btnModifyPwd.SetPushed(true);
	m_btnZhangHaoBaoHu.SetPushed(false);
	m_btnTiKuanZhangHao.SetPushed(false);
	m_btnTouZhuJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnYingKuiMingXi.SetPushed(false);
	m_btnQipaiYingkui.SetPushed(false);

	m_showType = MODIFYPWD;
	m_baseInfoDlg.ShowWindow(SW_HIDE);
	m_xgmmDlg.ShowWindow(SW_SHOW);
	m_zhhbhDlg.ShowWindow(SW_HIDE);
	m_shzhTkzhh.ShowWindow(SW_HIDE);
	m_tzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_chzhlogdlg.ShowWindow(SW_HIDE);
	m_yklogdlg.ShowWindow(SW_HIDE);
	m_ykmxdlg.ShowWindow(SW_HIDE);
	m_qpykdlg.ShowWindow(SW_HIDE);
}


void CZhangHuDlg::OnBnClickedBtnAccountbaohu()
{
	if( GetTickCount()-m_dwTickCount <100)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnBaseInfo.SetPushed(false);
	m_btnModifyPwd.SetPushed(false);
	m_btnZhangHaoBaoHu.SetPushed(true);
	m_btnTiKuanZhangHao.SetPushed(false);
	m_btnTouZhuJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnYingKuiMingXi.SetPushed(false);
	m_btnQipaiYingkui.SetPushed(false);

	m_showType = ZHANGHAOBAOHU;
	m_baseInfoDlg.ShowWindow(SW_HIDE);
	m_xgmmDlg.ShowWindow(SW_HIDE);
	m_zhhbhDlg.ShowWindow(SW_SHOW);
	m_shzhTkzhh.ShowWindow(SW_HIDE);
	m_tzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_chzhlogdlg.ShowWindow(SW_HIDE);
	m_yklogdlg.ShowWindow(SW_HIDE);
	m_ykmxdlg.ShowWindow(SW_HIDE);
	m_qpykdlg.ShowWindow(SW_HIDE);
}

void CZhangHuDlg::OnBnClickedBtnTikuanact()
{
	if( GetTickCount()-m_dwTickCount <100)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnBaseInfo.SetPushed(false);
	m_btnModifyPwd.SetPushed(false);
	m_btnZhangHaoBaoHu.SetPushed(false);
	m_btnTiKuanZhangHao.SetPushed(true);
	m_btnTouZhuJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnYingKuiMingXi.SetPushed(false);
	m_btnQipaiYingkui.SetPushed(false);

	m_showType = TIXIANZHANGHU;
	m_baseInfoDlg.ShowWindow(SW_HIDE);
	m_xgmmDlg.ShowWindow(SW_HIDE);
	m_zhhbhDlg.ShowWindow(SW_HIDE);
	m_shzhTkzhh.ShowWindow(SW_SHOW);
	m_tzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_chzhlogdlg.ShowWindow(SW_HIDE);
	m_yklogdlg.ShowWindow(SW_HIDE);
	m_ykmxdlg.ShowWindow(SW_HIDE);
	m_qpykdlg.ShowWindow(SW_HIDE);
}

void CZhangHuDlg::OnBnClickedBtnTouzhujilu()
{
	if( GetTickCount()-m_dwTickCount <100)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnBaseInfo.SetPushed(false);
	m_btnModifyPwd.SetPushed(false);
	m_btnZhangHaoBaoHu.SetPushed(false);
	m_btnTiKuanZhangHao.SetPushed(false);
	m_btnTouZhuJiLu.SetPushed(true);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnYingKuiMingXi.SetPushed(false);
	m_btnQipaiYingkui.SetPushed(false);

	m_showType = TOUZHUJILU;
	m_baseInfoDlg.ShowWindow(SW_HIDE);
	m_xgmmDlg.ShowWindow(SW_HIDE);
	m_zhhbhDlg.ShowWindow(SW_HIDE);
	m_shzhTkzhh.ShowWindow(SW_HIDE);
	m_tzhlogDlg.ShowWindow(SW_SHOW);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_chzhlogdlg.ShowWindow(SW_HIDE);
	m_yklogdlg.ShowWindow(SW_HIDE);
	m_ykmxdlg.ShowWindow(SW_HIDE);
	m_qpykdlg.ShowWindow(SW_HIDE);
}

void CZhangHuDlg::OnBnClickedBtnTixianjilu()
{
	if( GetTickCount()-m_dwTickCount <100)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnBaseInfo.SetPushed(false);
	m_btnModifyPwd.SetPushed(false);
	m_btnZhangHaoBaoHu.SetPushed(false);
	m_btnTiKuanZhangHao.SetPushed(false);
	m_btnTouZhuJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(true);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnYingKuiMingXi.SetPushed(false);
	m_btnQipaiYingkui.SetPushed(false);

	m_showType = TIXIANJILU;
	m_baseInfoDlg.ShowWindow(SW_HIDE);
	m_xgmmDlg.ShowWindow(SW_HIDE);
	m_zhhbhDlg.ShowWindow(SW_HIDE);
	m_shzhTkzhh.ShowWindow(SW_HIDE);
	m_tzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_SHOW);
	m_chzhlogdlg.ShowWindow(SW_HIDE);
	m_yklogdlg.ShowWindow(SW_HIDE);
	m_ykmxdlg.ShowWindow(SW_HIDE);
	m_qpykdlg.ShowWindow(SW_HIDE);
}

void CZhangHuDlg::OnBnClickedBtnChongzhijilu()
{
	if( GetTickCount()-m_dwTickCount <100)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnBaseInfo.SetPushed(false);
	m_btnModifyPwd.SetPushed(false);
	m_btnZhangHaoBaoHu.SetPushed(false);
	m_btnTiKuanZhangHao.SetPushed(false);
	m_btnTouZhuJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(true);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnYingKuiMingXi.SetPushed(false);
	m_btnQipaiYingkui.SetPushed(false);

	m_showType = CHONGZHIJILU;
	m_baseInfoDlg.ShowWindow(SW_HIDE);
	m_xgmmDlg.ShowWindow(SW_HIDE);
	m_zhhbhDlg.ShowWindow(SW_HIDE);
	m_shzhTkzhh.ShowWindow(SW_HIDE);
	m_tzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_chzhlogdlg.ShowWindow(SW_SHOW);
	m_yklogdlg.ShowWindow(SW_HIDE);
	m_ykmxdlg.ShowWindow(SW_HIDE);
	m_qpykdlg.ShowWindow(SW_HIDE);
}

void CZhangHuDlg::OnBnClickedBtnYingkuijilu()
{
	if( GetTickCount()-m_dwTickCount <100)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnBaseInfo.SetPushed(false);
	m_btnModifyPwd.SetPushed(false);
	m_btnZhangHaoBaoHu.SetPushed(false);
	m_btnTiKuanZhangHao.SetPushed(false);
	m_btnTouZhuJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(true);
	m_btnYingKuiMingXi.SetPushed(false);
	m_btnQipaiYingkui.SetPushed(false);

	m_showType = YINGKUIJILU;
	m_baseInfoDlg.ShowWindow(SW_HIDE);
	m_xgmmDlg.ShowWindow(SW_HIDE);
	m_zhhbhDlg.ShowWindow(SW_HIDE);
	m_shzhTkzhh.ShowWindow(SW_HIDE);
	m_tzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_chzhlogdlg.ShowWindow(SW_HIDE);
	m_yklogdlg.ShowWindow(SW_SHOW);
	m_ykmxdlg.ShowWindow(SW_HIDE);
	m_qpykdlg.ShowWindow(SW_HIDE);
}

void CZhangHuDlg::OnBnClickedBtnYingkuimingxi()
{
	if( GetTickCount()-m_dwTickCount <100)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnBaseInfo.SetPushed(false);
	m_btnModifyPwd.SetPushed(false);
	m_btnZhangHaoBaoHu.SetPushed(false);
	m_btnTiKuanZhangHao.SetPushed(false);
	m_btnTouZhuJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnYingKuiMingXi.SetPushed(true);
	m_btnQipaiYingkui.SetPushed(false);

	m_showType = YINGKUIMINGXI;
	m_baseInfoDlg.ShowWindow(SW_HIDE);
	m_xgmmDlg.ShowWindow(SW_HIDE);
	m_zhhbhDlg.ShowWindow(SW_HIDE);
	m_shzhTkzhh.ShowWindow(SW_HIDE);
	m_tzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_chzhlogdlg.ShowWindow(SW_HIDE);
	m_yklogdlg.ShowWindow(SW_HIDE);
	m_ykmxdlg.ShowWindow(SW_SHOW);
	m_qpykdlg.ShowWindow(SW_HIDE);
}
void CZhangHuDlg::OnBnClickedBtnQipaiYingkui()
{
	if( GetTickCount()-m_dwTickCount <100)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	m_btnBaseInfo.SetPushed(false);
	m_btnModifyPwd.SetPushed(false);
	m_btnZhangHaoBaoHu.SetPushed(false);
	m_btnTiKuanZhangHao.SetPushed(false);
	m_btnTouZhuJiLu.SetPushed(false);
	m_btnTiXianJiLu.SetPushed(false);
	m_btnChongZhiJiLu.SetPushed(false);
	m_btnYingKuiJiLu.SetPushed(false);
	m_btnYingKuiMingXi.SetPushed(false);
	m_btnQipaiYingkui.SetPushed(true);

	m_showType = QIPAIYINGKUI;
	m_baseInfoDlg.ShowWindow(SW_HIDE);
	m_xgmmDlg.ShowWindow(SW_HIDE);
	m_zhhbhDlg.ShowWindow(SW_HIDE);
	m_shzhTkzhh.ShowWindow(SW_HIDE);
	m_tzhlogDlg.ShowWindow(SW_HIDE);
	m_txlogDlg.ShowWindow(SW_HIDE);
	m_chzhlogdlg.ShowWindow(SW_HIDE);
	m_yklogdlg.ShowWindow(SW_HIDE);
	m_ykmxdlg.ShowWindow(SW_HIDE);
	m_qpykdlg.ShowWindow(SW_SHOW);
}

void CZhangHuDlg::InitBtns()
{
	CString str = CBmpUtil::GetExePath() + _T("skin\\btn_txt_bk1.png");
	m_btnBaseInfo.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\BaseInfo.png"));
	m_btnModifyPwd.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\ModifyPwd.png"));
	m_btnZhangHaoBaoHu.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\ZhangHaoBaoHu.png"));
	m_btnTiKuanZhangHao.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\TiKuanZhangHao.png"));
	m_btnTouZhuJiLu.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\touzhujilu.png"));
	m_btnTiXianJiLu.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\ZHTiXianJiLu.png"));
	m_btnChongZhiJiLu.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\ZHChongZhiJiLu.png"));
	m_btnYingKuiJiLu.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\ZHYingKuiJiLu.png"));
	m_btnYingKuiMingXi.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\YingKuiMingXi.png"));
	m_btnQipaiYingkui.SetBkImage(CBmpUtil::GetExePath() + _T("skin\\QipaiYingkui.png"));

	m_btnBaseInfo.SetTextFont(&m_font);
	m_btnModifyPwd.SetTextFont(&m_font);
	m_btnZhangHaoBaoHu.SetTextFont(&m_font);
	m_btnTiKuanZhangHao.SetTextFont(&m_font);
	m_btnTouZhuJiLu.SetTextFont(&m_font);
	m_btnTiXianJiLu.SetTextFont(&m_font);
	m_btnChongZhiJiLu.SetTextFont(&m_font);
	m_btnYingKuiJiLu.SetTextFont(&m_font);
	m_btnYingKuiMingXi.SetTextFont(&m_font);
	m_btnQipaiYingkui.SetTextFont(&m_font);
}

