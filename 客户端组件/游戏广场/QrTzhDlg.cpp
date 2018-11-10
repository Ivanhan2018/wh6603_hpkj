#include "stdafx.h"
#include "GamePlaza.h"
#include "QrTzhDlg.h"
#include "Bmputil.h"


static const int wnd_width = 332;
static const int wnd_height = 264;

static const int btn_ok_x = 105;
static const int btn_ok_y = 190;

static const int btn_cancel_x = 229;
static const int btn_cancel_y = 190;

//玩法
static const int tip_wanfa_x = 210;
static const int tip_wanfa_y = 47;
static const int tip_wanfa_width = 170;
static const int tip_wanfa_height = 20;
static CRect rc_wanfa(tip_wanfa_x, 
						   tip_wanfa_y, 
						   tip_wanfa_x + tip_wanfa_width, 
						   tip_wanfa_y + tip_wanfa_height);

//注数
static const int tip_zhushu_x = 210;
static const int tip_zhushu_y = 75;
static const int tip_zhushu_width = 70;
static const int tip_zhushu_height = 20;
static CRect rc_zhushu(tip_zhushu_x, 
						   tip_zhushu_y, 
						   tip_zhushu_x + tip_zhushu_width, 
						   tip_zhushu_y + tip_zhushu_height);

//单注金额
static const int tip_danzhujine_x = 210;
static const int tip_danzhujine_y = 108;
static const int tip_danzhujine_width = 70;
static const int tip_danzhujine_height = 20;
static CRect rc_danzhujine(tip_danzhujine_x, 
						   tip_danzhujine_y, 
						   tip_danzhujine_x + tip_danzhujine_width, 
						   tip_danzhujine_y + tip_danzhujine_height);

//总金额
static const int tip_zongjine_x = 210;
static const int tip_zongjine_y = 139;
static const int tip_zongjine_width = 70;
static const int tip_zongjine_height = 20;
static CRect rc_zongjine(tip_zongjine_x, 
						   tip_zongjine_y, 
						   tip_zongjine_x + tip_zongjine_width, 
						   tip_zongjine_y + tip_zongjine_height);

// CQrTzhDlg 对话框

IMPLEMENT_DYNAMIC(CQrTzhDlg, CDialog)

CQrTzhDlg::CQrTzhDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQrTzhDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_zhushu(0)
	, m_gamekind(_T("重庆彩"))
	, m_moshi(MoShi_Yuan)
{

}

CQrTzhDlg::~CQrTzhDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CQrTzhDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CQrTzhDlg, CDialog)
	ON_WM_LBUTTONDOWN()

	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CQrTzhDlg 消息处理程序
void CQrTzhDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//使窗口可以拖动
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CQrTzhDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);

	CDC cacheDC;
	cacheDC.CreateCompatibleDC(&dc);
	CBitmap cacheBmp;
	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);

	//Graphics graphics(cacheDC.m_hDC);

	//graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	m_ImageBack.DrawImage(&cacheDC,0,0);
	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));

	CString strTmp;
	cacheDC.DrawText(m_gamekind, rc_wanfa, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%d"), m_zhushu);
	cacheDC.DrawText(strTmp, rc_zhushu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	double danzhujine =  2.00;
	if(m_moshi == MoShi_Yuan)
	{
		danzhujine = 2.00;
	}
	else if(m_moshi == MoShi_Jiao)
	{
		danzhujine = 0.2;
	}
	else if(m_moshi == MoShi_Fen)
	{
		danzhujine = 0.02;
	}
	else if(m_moshi == MoShi_Li)
	{
		danzhujine = 0.002;
	}
	if(m_gamekind ==  _T("香港六合彩"))
		danzhujine = 1.000;

	strTmp.Format(_T("%.3lf"), danzhujine);
	cacheDC.DrawText(strTmp, rc_danzhujine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	if(m_zongjine>0.000001)
		strTmp.Format(_T("%.3lf"), m_zongjine);
	else
		strTmp.Format(_T("%.3lf"), m_zhushu * danzhujine);
	cacheDC.DrawText(strTmp, rc_zongjine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CQrTzhDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CQrTzhDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\qrtz.png"));

	m_ImageBack.LoadImage(CBmpUtil::GetExePath() + _T("skin\\qrtz.png"));
	m_font.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\quxiao_bt.png"));

	SetWindowPos(NULL, 0, 0, m_ImageBack.GetWidth(), m_ImageBack.GetHeight(), SWP_NOMOVE|SWP_NOZORDER);
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CQrTzhDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	if(m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}

	if(m_btnCancel.GetSafeHwnd() != NULL)
	{
		m_btnCancel.SetWindowPos(NULL, btn_cancel_x, btn_cancel_y, m_btnCancel.Width(), m_btnCancel.Height(), SWP_NOZORDER);
	}
}

