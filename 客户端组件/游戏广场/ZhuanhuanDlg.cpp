#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhuanhuanDlg.h"
#include "Bmputil.h"
#include "MessageDlg.h"

static const int wnd_width = 332;
static const int wnd_height = 264;

static const int btn_ok_x = 108;
static const int btn_ok_y = 287;

static const int btn_cancel_x = 284;
static const int btn_cancel_y = 287;

//玩法
static const int tip_wanfa_x = 130;
static const int tip_wanfa_y = 47;
static const int tip_wanfa_width = 70;
static const int tip_wanfa_height = 20;
static CRect rc_wanfa(tip_wanfa_x, 
						   tip_wanfa_y, 
						   tip_wanfa_x + tip_wanfa_width, 
						   tip_wanfa_y + tip_wanfa_height);

//注数
static const int tip_zhushu_x = 130;
static const int tip_zhushu_y = 87;
static const int tip_zhushu_width = 70;
static const int tip_zhushu_height = 20;
static CRect rc_zhushu(tip_zhushu_x, 
						   tip_zhushu_y, 
						   tip_zhushu_x + tip_zhushu_width, 
						   tip_zhushu_y + tip_zhushu_height);
//注数
static const int tip_jine_x = 110;
static const int tip_jine_y = 246;
static const int tip_jine_width = 470;
static const int tip_jine_height = 20;
static CRect rc_jine(tip_jine_x, 
						   tip_jine_y, 
						   tip_jine_x + tip_jine_width, 
						   tip_jine_y + tip_jine_height);

//彩票金额
static const int tip_danzhujine_x = 110;
static const int tip_danzhujine_y = 43;
static const int tip_danzhujine_width = 170;
static const int tip_danzhujine_height = 20;
static CRect rc_danzhujine(tip_danzhujine_x, 
						   tip_danzhujine_y, 
						   tip_danzhujine_x + tip_danzhujine_width, 
						   tip_danzhujine_y + tip_danzhujine_height);
//棋牌金额
static const int tip_qipaijine_x = 110;
static const int tip_qipaijine_y = 71;
static const int tip_qipaijine_width = 170;
static const int tip_qipaijine_height = 20;
static CRect rc_qipaijine(tip_qipaijine_x, 
						   tip_qipaijine_y, 
						   tip_qipaijine_x + tip_danzhujine_width, 
						   tip_qipaijine_y + tip_danzhujine_height);

//总金额
static const int tip_zongjine_x = 315;
static const int tip_zongjine_y = 304;
static const int tip_zongjine_width = 170;
static const int tip_zongjine_height = 30;
static CRect rc_zongjine(tip_zongjine_x, 
						   tip_zongjine_y, 
						   tip_zongjine_x + tip_zongjine_width, 
						   tip_zongjine_y + tip_zongjine_height);
static const int btn_yuan_x = 107;
static const int btn_yuan_y = 131;
static const int btn_yuan_span_x = 91;
static const int btn_yuan_span_y = 35;
// CZhuanhuanDlg 对话框

IMPLEMENT_DYNAMIC(CZhuanhuanDlg, CDialog)

CZhuanhuanDlg::CZhuanhuanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZhuanhuanDlg::IDD, pParent)
	, m_bmpBk(NULL)
	,m_fJine(0.0)
	,m_fJiaoYiJine(0.0)
{
	m_nType=0;
}

CZhuanhuanDlg::~CZhuanhuanDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CZhuanhuanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_JINE, m_editJine);

	DDX_Control(pDX, IDC_RIO_TO_CAIPIAO, m_rioToCaiPiao);
	DDX_Control(pDX, IDC_RIO_TO_QIPAI, m_rioToQiPai);

	DDX_Control(pDX, IDC_50_YUAN, m_btButton[0]);
	DDX_Control(pDX, IDC_100_YUAN, m_btButton[1]);
	DDX_Control(pDX, IDC_300_YUAN, m_btButton[2]);
	DDX_Control(pDX, IDC_500_YUAN, m_btButton[3]);
	DDX_Control(pDX, IDC_1000_YUAN, m_btButton[4]);
	DDX_Control(pDX, IDC_3000_YUAN, m_btButton[5]);
	DDX_Control(pDX, IDC_5000_YUAN, m_btButton[6]);
	DDX_Control(pDX, IDC_10000_YUAN, m_btButton[7]);
	DDX_Control(pDX, IDC_HALF_YUAN, m_btButton[8]);
	DDX_Control(pDX, IDC_ALL_YUAN, m_btButton[9]);

}


BEGIN_MESSAGE_MAP(CZhuanhuanDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_JINE, OnLvnItemChangedEdit)
	ON_WM_LBUTTONDOWN()

	ON_BN_CLICKED(IDC_RIO_TO_CAIPIAO, &CZhuanhuanDlg::OnBnClickedRioToCaiPiao)
	ON_BN_CLICKED(IDC_RIO_TO_QIPAI, &CZhuanhuanDlg::OnBnClickedRioToQiPai)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_EDIT_JINE, OnLvnItemChangedEdit)
END_MESSAGE_MAP()

void CZhuanhuanDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//使窗口可以拖动
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}
BOOL CZhuanhuanDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);
	switch(nCommandID)
	{
	case IDC_50_YUAN:
		{
			m_fJiaoYiJine = 50;
			break;
		}
	case IDC_100_YUAN:
		{
			m_fJiaoYiJine = 100;
			break;
		}
	case IDC_300_YUAN:
		{
			m_fJiaoYiJine = 300;
			break;
		}
	case IDC_500_YUAN:
		{
			m_fJiaoYiJine = 500;
			break;
		}
	case IDC_1000_YUAN:
		{
			m_fJiaoYiJine = 1000;
			break;
		}
	case IDC_3000_YUAN:
		{
			m_fJiaoYiJine = 3000;
			break;
		}
	case IDC_5000_YUAN:
		{
			m_fJiaoYiJine = 5000;
			break;
		}
	case IDC_10000_YUAN:
		{
			m_fJiaoYiJine = 10000;
			break;
		}
	case IDC_HALF_YUAN:
		{
		//	m_fJiaoYiJine = m_fJine/2;

			if(m_nType == 1)
			{
				m_fJiaoYiJine = m_fQpJine/2;
			}
			else if(m_nType == 2)
			{
				m_fJiaoYiJine = m_fJine/2;
			}

			break;
		}
	case IDC_ALL_YUAN:
		{
		//	m_fJiaoYiJine = m_fJine;
			if(m_nType == 1)
			{
				m_fJiaoYiJine = m_fQpJine;
			}
			else if(m_nType == 2)
			{
				m_fJiaoYiJine = m_fJine;
			}
			break;
		}
	default:
		{
			return __super::OnCommand(wParam,lParam);
		}
	}
	DOUBLE fJine;
	if(m_nType == 1)
	{
		fJine = m_fQpJine;
	}
	else if(m_nType == 2)
	{
		fJine = m_fJine;
	}

	if(m_fJiaoYiJine > fJine)
	{
		MyMessageBox(L"余额不足，请重新填写");
		//m_editJine.SetSel(0,-1);
		m_fJiaoYiJine = 0;
		return  __super::OnCommand(wParam,lParam);;
	}

	InvalidateRect(rc_jine);

	return  __super::OnCommand(wParam,lParam);;
}

// CZhuanhuanDlg 消息处理程序
void CZhuanhuanDlg::OnBnClickedRioToCaiPiao()
{
	m_rioToCaiPiao.SetPushed(TRUE);
	m_rioToQiPai.SetPushed(FALSE);
	m_nType = 1;
	return;
}
void CZhuanhuanDlg::OnBnClickedRioToQiPai()
{
	m_rioToCaiPiao.SetPushed(FALSE);
	m_rioToQiPai.SetPushed(TRUE);
	m_nType = 2;
	return;
}
void CZhuanhuanDlg::OnPaint()
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

	Graphics graphics(cacheDC.m_hDC);

	graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(98,88,84));

	CString strTmp;
	strTmp.Format(_T("%.2lf"), m_fJine);
	cacheDC.DrawText(strTmp, rc_danzhujine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.2lf"), m_fQpJine);
	cacheDC.DrawText(strTmp, rc_qipaijine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 	if(m_nType == 1)
// 	{
// 		strTmp.Format(_T("转换类型:彩票转游戏"));
// 	}
// 	else if(m_nType == 2)
// 	{
// 		strTmp.Format(L"转换类型:游戏转彩票");
// 	}
// 	else
// 		strTmp.Empty();
// 
// 	cacheDC.DrawText(strTmp, rc_zongjine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strTmp.Format(L"");
	CRect rcStatic;
	rcStatic.CopyRect(rc_zongjine);
	rcStatic.top +=38;
	rcStatic.bottom +=38;
	cacheDC.DrawText(strTmp, rcStatic, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(RGB(250, 0, 0));
	strTmp.Format(L"%0.2lf",m_fJiaoYiJine);
	cacheDC.DrawText(strTmp, rc_jine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);


	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CZhuanhuanDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CZhuanhuanDlg::SetJine(DOUBLE fJine)
{
	m_fJine = fJine;
	
	return;
}
VOID CZhuanhuanDlg::OnLvnItemChangedEdit()
{
	TCHAR szJiaoyi[30];
	GetDlgItemText(IDC_EDIT_JINE,szJiaoyi,sizeof(szJiaoyi));

	for(int i = 0;i < 30;i++)
	{
		CString str;
		str.Format(L"DQSZ %d",szJiaoyi[i]);
		if(szJiaoyi[i] == 0)
			break;
		if((szJiaoyi[i]!=8 )&&(szJiaoyi[i]!=46)&&(szJiaoyi[i]<48||szJiaoyi[i] >57))
		{
			GetDlgItem(IDC_EDIT_JINE)->SetWindowText(L"");
			return;
		}
	}
	m_fJiaoYiJine = _tstof(szJiaoyi);

	DOUBLE fJine;
	if(m_nType == 1)
	{
		fJine = m_fQpJine;
	}
	else if(m_nType == 2)
	{
		fJine = m_fJine;
	}

	if(m_fJiaoYiJine > fJine)
	{
		MyMessageBox(L"余额不足，请重新填写");
		m_editJine.SetSel(0,-1);
		return;
	}

	InvalidateRect(rc_jine);
	return;

}
VOID CZhuanhuanDlg::OnOK()
{
// 	TCHAR szJiaoyi[30];
// 	GetDlgItemText(IDC_EDIT_JINE,szJiaoyi,sizeof(szJiaoyi));
// 
// 	m_fJiaoYiJine = _tstof(szJiaoyi);

	DOUBLE fJine;
	if(m_nType == 1)
	{
		fJine = m_fQpJine;
	}
	else if(m_nType == 2)
	{
		fJine = m_fJine;
	}

	if(m_fJiaoYiJine < 0.000001)
	{
		MyMessageBox(L"交易金额为0.00元，请重新填写！");
		m_editJine.SetSel(0,-1);
		return;

	}

	if(m_fJiaoYiJine > fJine)
	{
		MyMessageBox(L"余额不足，请重新填写！");
		m_editJine.SetSel(0,-1);
		return;
	}

// 	if(m_fJiaoYiJine > 500000)
// 	{
// 		MyMessageBox(L"转换每次最多500000!");
// 		m_editJine.SetSel(0,-1);
// 		m_fJiaoYiJine = 0;
// 		InvalidateRect(rc_jine);
// 		return;
// 	}

	return CDialog::OnOK();
}
VOID CZhuanhuanDlg::OnCancel()
{
	return CDialog::OnCancel();
}
BOOL CZhuanhuanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\zhuanhuan.png"));

	//m_font.CreateFont(20,0,0,0,FW_NORMAL,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("微软雅黑"));
	m_font.CreateFont(21, 0, 0, 0,FW_NORMAL , 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	m_btnfont.CreateFont(17,0,0,0,FW_NORMAL,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("Arial"));
	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\qdzz.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\qxzz.png"));

	SetWindowPos(NULL, 0, 0, wnd_width, wnd_height, SWP_NOMOVE|SWP_NOZORDER);
	CenterWindow();

	m_editJine.SetFocus();

	m_rioToCaiPiao.SetFont(&m_font);

	for(BYTE i = 0;i < CountArray(m_btButton);i++)
	{
		m_btButton[i].SetBkImage(CBmpUtil::GetExePath() + _T("skin\\yuan.png"));
		m_btButton[i].SetTextFont(&m_btnfont);
		m_btButton[i].SetTextColor(RGB(98,88,84));

	}

	m_rioToCaiPiao.SetPushed(TRUE);
	m_rioToQiPai.SetPushed(FALSE);
	m_rioToCaiPiao.SetImage(CBmpUtil::GetExePath() + _T("skin\\yezh.png"));
	m_rioToQiPai.SetImage(CBmpUtil::GetExePath() + _T("skin\\yezh.png"));
	m_rioToCaiPiao.SetPaintTextColor(RGB(98,88,84));
	m_rioToQiPai.SetPaintTextColor(RGB(98,88,84));

	m_editJine.SetEnableColor(RGB(98,88,84),RGB(251,243,229),RGB(251,243,229));
	m_editJine.SetFont(&m_font);
	SetWindowPos(NULL,0,0,m_bmpBk->GetWidth(),m_bmpBk->GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);

	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CZhuanhuanDlg::OnSize(UINT nType, int cx, int cy)
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

	if(m_editJine.GetSafeHwnd() != NULL)
	{
		m_editJine.SetWindowPos(NULL, btn_cancel_x+40, btn_cancel_y-82,80, 23, SWP_NOZORDER);
	}

	if(m_rioToCaiPiao.GetSafeHwnd() != NULL)
	{
		m_rioToCaiPiao.SetWindowPos(NULL, tip_zongjine_x+10-221, tip_zongjine_y-71-126-11,100, 23, SWP_NOSIZE);
	}
	if(m_rioToQiPai.GetSafeHwnd() != NULL)
	{
		m_rioToQiPai.SetWindowPos(NULL, tip_zongjine_x+180-221, tip_zongjine_y-71-126-11,100, 30, SWP_NOSIZE);
	}

	for(int i = 0 ;i < CountArray(m_btButton);i++)
	{
		int x_span = 0;
		int y_span = 0;
		if(m_btButton[i].GetSafeHwnd()!=NULL)
		{
			x_span = (i%4)*btn_yuan_span_x;
			y_span = (i/4)*btn_yuan_span_y;
			m_btButton[i].SetWindowPos(NULL,btn_yuan_x+x_span,btn_yuan_y+y_span,m_btButton[i].Width(),m_btButton[i].Height(),SWP_NOZORDER);
		}
	}
}

