#include "stdafx.h"
#include "GamePlaza.h"
#include "XgfdDlg.h"

static CRect rc_account(30, 40, 230, 60);

static CRect rc_fandian(30, 75, 90, 95);

IMPLEMENT_DYNAMIC(CXgfdDlg, CDialog)

CXgfdDlg::CXgfdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXgfdDlg::IDD, pParent)
	, m_user_id(0)
	, m_old_fandian(0)
	, m_fandian(0)
	, m_strFandian(_T(""))
{

}

CXgfdDlg::~CXgfdDlg()
{
}

void CXgfdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//DDX_Text(pDX, IDC_EDIT_FANDIAN, m_fandian);
	DDX_Control(pDX, IDC_COMBO_FANDIAN, m_cmbFandian);
	DDX_CBString(pDX, IDC_COMBO_FANDIAN, m_strFandian);
}


BEGIN_MESSAGE_MAP(CXgfdDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CXgfdDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CXgfdDlg 消息处理程序

void CXgfdDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));

	cacheDC.DrawText(m_account, &rc_account, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("返点:"), &rc_fandian, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CXgfdDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CXgfdDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	if (m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, 0, 0, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER|SWP_NOMOVE);
	}

	if (m_btnCancel.GetSafeHwnd() != NULL)
	{
		m_btnCancel.SetWindowPos(NULL, 0, 0, m_btnCancel.Width(), m_btnCancel.Height(), SWP_NOZORDER|SWP_NOMOVE);
	}
}

BOOL CXgfdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg3.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\quxiao_bt.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	InitFanDianCmb();

	SetWindowPos(NULL, 0, 0, 260, 160, SWP_NOZORDER|SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CXgfdDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();

	//if (m_fandian < 0.1)
	//{
	//	MyMessageBox(_T("返点不能小于0.1%"));
	//	return;
	//}

	//if (m_fandian < m_old_fandian)
	//{
	//	MyMessageBox(_T("新的返点不能小于原来设置的返点"));
	//	return;
	//}

	//if (m_fandian > theAccount.fandian * 100)
	//{
	//	MyMessageBox(_T("新的返点不能大于自己的返点"));
	//	return;
	//}
	//m_cmbFandian.GetWindowText(m_strFandian);
	DWORD wSel = m_cmbFandian.GetCurSel();

	m_cmbFandian.GetLBText(wSel,m_strFandian);

	m_fandian = _tstof(m_strFandian);
	CDialog::OnOK();
}


void CXgfdDlg::OnBnClickedOk()
{
	OnOK();
}


void CXgfdDlg::InitFanDianCmb()
{
	if (theAccount.type == 1) {
		int i=0;
		if(theAccount.fandian*100 > 2.799999 && theAccount.fandian*100<2.8000001)
		{
			CString str;
			str.Format(_T("2.8"));
			m_cmbFandian.AddString(str);

		}

		double ffandian = 0.00f;

// 		if(theAccount.fandian*100 < 2.5999999)
// 			ffandian = theAccount.fandian*100;

		for(double df=(theAccount.fandian*100-0.1); df>=0.0000001; df-=0.1, i++) 
		{
			if(df>2.89999 && df<2.9000001) //排除 2.9
				continue;
			CString str;
			str.Format(_T("%.1lf"), df);
			m_cmbFandian.AddString(str);
			m_cmbFandian.SetCurSel(0);
		}
		CString str;
		str.Format(_T("%.1lf"), 0.0);
		m_cmbFandian.AddString(str);
		m_cmbFandian.SetCurSel(0);
	}
}
