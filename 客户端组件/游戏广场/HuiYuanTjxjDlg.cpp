#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuanTjxjDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"

static const int rio_width = 71;
static const int rio_height = 23;
static const int rio_daili_x = 228;
static const int rio_daili_y = 152;

static const int rio_huiyuan_x = 320;
static const int rio_huiyuan_y = 152;

static const int edit_width = 186;
static const int edit_height = 21;

static const int edit_x = 231;
static const int edit_fandian_y = 189;
static const int edit_zhanghu_y = 225;
static const int edit_dl_pwd_y = 262;
static const int edit_crm_dl_pwd_y = 297;
static const int edit_qk_pwd_y = 332;
static const int edit_crm_qk_pwd_y = 370;
static const int edit_qq_y = 407;

static const int btn_ok_x = 270;
static const int btn_ok_y = 468;

static const int btn_regurl_x = 527;
static const int btn_regurl_y = 72;

static CRect rcRegUrl(btn_regurl_x+80, btn_regurl_y, btn_regurl_x+600, btn_regurl_y+30);


static CRect rcText(edit_x-72, edit_qq_y + 30, edit_x+400-72, edit_qq_y + 30+20);

static const int web_fandian_x = 229;
static const int web_fandian_y = 118;

static const int set_fandian_x = 425;
static const int set_fandian_y = 120;

static const int set_mima_x = 425;
static const int set_mima_y = 250;
IMPLEMENT_DYNAMIC(CHuiYuanTjxjDlg, CDialog)

CHuiYuanTjxjDlg::CHuiYuanTjxjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuiYuanTjxjDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_type(0)
	, m_fandian(0)
	, m_zhanghu(_T(""))
	, m_password(_T(""))
	, m_crmpwd(_T(""))
	, m_qkpwd(_T(""))
	, m_crmqkpwd(_T(""))
	, m_strFandian(_T(""))
	, m_strWebFandian(_T(""))
	, m_strQQ(_T(""))
{
	m_bGetRegURL=false;
	m_AddHuiYuan=false;
	m_bSetWebFandian=false;
}

CHuiYuanTjxjDlg::~CHuiYuanTjxjDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CHuiYuanTjxjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FANDIAN, m_fandian);
	DDX_Text(pDX, IDC_EDIT_ZHANGHAO, m_zhanghu);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDX_Text(pDX, IDC_EDIT_CRM_PWD, m_crmpwd);
	DDX_Text(pDX, IDC_EDIT_QK_PWD, m_qkpwd);
	DDX_Text(pDX, IDC_EDIT_CRM_QK_PWD, m_crmqkpwd);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_RIO_DAILI, m_rioDaili);
	DDX_Control(pDX, IDC_RIO_HUIYUAN, m_rioHuiYuan);
	DDX_Control(pDX, IDC_COMBO_FANDIAN, m_cmbFandian);
	DDX_CBString(pDX, IDC_COMBO_FANDIAN, m_strFandian);
	DDX_Control(pDX, IDC_COMBO_WEB_FANDIAN, m_cmbWebFandian);
	DDX_CBString(pDX, IDC_COMBO_WEB_FANDIAN, m_strWebFandian);
	DDX_Control(pDX, IDC_BTN_CP_REGURL, m_btnRegUrl);
	DDX_Control(pDX, IDC_BTN_SET_WEB_FANDIAN, m_btnSetWebFandian);
	DDX_Control(pDX, IDC_BTN_SET_MOREN_MIMA, m_btnSetMorenMima);
	DDX_Control(pDX, IDC_EDIT_QQ, m_editQQ);
	DDX_Control(pDX, IDC_COMBO_WEB_REG_URL, m_cmbWebRegUrl);

	DDX_Control(pDX, IDC_EDIT_ZHANGHAO, m_editZhanghu);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EDIT_CRM_PWD, m_editCrmPwd);
	DDX_Control(pDX, IDC_EDIT_QK_PWD, m_editQkPwd);
	DDX_Control(pDX, IDC_EDIT_CRM_QK_PWD, m_editCrmQkPwd);

	DDX_Text(pDX, IDC_EDIT_QQ, m_strQQ);
}


BEGIN_MESSAGE_MAP(CHuiYuanTjxjDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RIO_DAILI, &CHuiYuanTjxjDlg::OnBnClickedRioDaili)
	ON_BN_CLICKED(IDC_RIO_HUIYUAN, &CHuiYuanTjxjDlg::OnBnClickedRioHuiyuan)
	ON_BN_CLICKED(IDC_BTN_OK, &CHuiYuanTjxjDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_SET_WEB_FANDIAN, &CHuiYuanTjxjDlg::OnBnClickedSetWebFandian)
	ON_BN_CLICKED(IDC_BTN_SET_MOREN_MIMA, &CHuiYuanTjxjDlg::OnBnClickedSetMorenMima)
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_CP_REGURL, &CHuiYuanTjxjDlg::OnBnClickedBtnCpRegurl)
END_MESSAGE_MAP()


// CHuiYuanTjxjDlg 消息处理程序

void CHuiYuanTjxjDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0,m_bmpBk->GetWidth(), m_bmpBk-> GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);

 	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
// 	CFont* pOldFont = cacheDC.SelectObject(&m_font);
// 	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));
	
// 	CString str;
// 	if (theAccount.type==1)
// 	{
// 		str = _T("推荐注册：");
// 		str += m_strRegUrl;
// 	}
// 	else
// 	{
// 		str = _T("申请成为代理会员后，即可得到自己的推广链接");
// 	}
// 	cacheDC.DrawText(str, rcRegUrl, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

// 	cacheDC.SetTextColor(oldTextColor);
// 	cacheDC.SelectObject(pOldFont);
// 	cacheDC.SetBkMode(oldBkMode);

	if (theAccount.fandian*100>2.799999)
	{
		COLORREF oldTextColor = cacheDC.SetTextColor(RGB(197, 49, 48));
		cacheDC.DrawText(m_strPeie,rcText,DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
		cacheDC.SetTextColor(oldTextColor);
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CHuiYuanTjxjDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanTjxjDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL) 
	{
		return;
	}

	AdjustCtrls();
}

void CHuiYuanTjxjDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		if(theAccount.user_id <= 0)
			return;


		EnableCtrls(theAccount.type == 1);
		InitFanDianCmb();


		int nSel = m_cmbWebFandian.FindString(0,m_strWebFandian1);
		if(nSel == -1)
		{
			m_cmbWebFandian.AddString(_T("0.0"));
			nSel = m_cmbWebFandian.FindString(0,m_strWebFandian1);
			m_cmbWebFandian.SetCurSel(nSel);
		}
		else
		{
			m_cmbWebFandian.SetCurSel(nSel);
		}

		SendToServer(1);
		SendToServer(4);
	}

}

void CHuiYuanTjxjDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(theAccount.type == 0)
	{
		MyMessageBox(_T("只有代理能够添加下级，您可以联系客服，申请成为代理"));
	}

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CHuiYuanTjxjDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\tjxj_bg.png"));
	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnRegUrl.SetImage(CBmpUtil::GetExePath() + _T("skin\\fz_bt.png"));
	m_btnSetWebFandian.SetImage(CBmpUtil::GetExePath() + _T("skin\\shezhi1.png"));
	m_btnSetMorenMima.SetImage(CBmpUtil::GetExePath() + _T("skin\\shezhi1.png"));
	

	((CEdit*)GetDlgItem(IDC_EDIT_ZHANGHAO))->LimitText(12);
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->LimitText(12);
	((CEdit*)GetDlgItem(IDC_EDIT_QK_PWD))->LimitText(12);
	((CEdit*)GetDlgItem(IDC_EDIT_QQ))->LimitText(12);
	m_font.CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	m_editZhanghu.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(58,27,20));
	m_editPassword.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(58,27,20));
	m_editCrmPwd.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(58,27,20));
	m_editQkPwd.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(58,27,20));
	m_editCrmQkPwd.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(58,27,20));
	m_editQQ.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(58,27,20));

	m_cmbWebFandian.SetFont(&m_font);
	m_cmbWebRegUrl.SetFont(&m_font);
	m_cmbFandian.SetFont(&m_font);
// 	m_rioDaili.SetPaintTextColor(RGB(88,78,77));
// 	m_rioHuiYuan.SetPaintTextColor(RGB(88,78,77));

	//m_ImgRioMan.SetPaintTextColor(RGB(88,78,77));

	//CButton* pWnd = (CButton*)GetDlgItem(IDC_RIO_HUIYUAN);
	//if (pWnd != NULL) 
	//{
	//	pWnd->SetCheck(BST_CHECKED);
	//}
	OnBnClickedRioDaili();

	m_strWebFandian1.Format(L"0.0");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHuiYuanTjxjDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuiYuanTjxjDlg::OnOK()
{
	//CDialog::OnOK();
}

void CHuiYuanTjxjDlg::OnBnClickedRioDaili()
{
	m_type = 1;

	m_rioDaili.SetPushed(true);
	m_rioHuiYuan.SetPushed(false);
}

void CHuiYuanTjxjDlg::OnBnClickedRioHuiyuan()
{
	m_type = 0;

	m_rioDaili.SetPushed(false);
	m_rioHuiYuan.SetPushed(true);
}
void CHuiYuanTjxjDlg::OnBnClickedSetWebFandian()
{
	SendToServer(3);
	return;
}
void CHuiYuanTjxjDlg::OnBnClickedSetMorenMima()
{
	MyMessageBox(L"默认登陆密码为：a123456\r\n取款密码为：123456");
	CWnd *pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
	if(pWnd!=NULL)
	{
		pWnd->SetWindowText(L"a123456");
	}

	pWnd = GetDlgItem(IDC_EDIT_CRM_PWD);
	if(pWnd!=NULL)
	{
		pWnd->SetWindowText(L"a123456");
	}
	pWnd = GetDlgItem(IDC_EDIT_QK_PWD);
	if(pWnd!=NULL)
	{
		pWnd->SetWindowText(L"123456");
	}
	pWnd = GetDlgItem(IDC_EDIT_CRM_QK_PWD);
	if(pWnd!=NULL)
	{
		pWnd->SetWindowText(L"123456");
	}
	return;
}
void CHuiYuanTjxjDlg::OnBnClickedBtnOk()
{
	UpdateData();

	//if (m_fandian/100.0 < 0.0) 
	//{
	//	MyMessageBox(_T("返点不能小于零"));
	//	return;
	//}

	//if (m_fandian/100.0 > theAccount.fandian) 
	//{
	//	MyMessageBox(_T("返点不能大于自己的返点"));
	//	return;
	//}

	m_zhanghu = m_zhanghu.Trim();
	if (m_zhanghu.IsEmpty()) 
	{
		MyMessageBox(_T("用户名不能为空"));
		return;
	}

	if (m_zhanghu.GetLength() > 12) 
	{
		MyMessageBox(_T("用户名为6-12位"));
		return;
	}

	if (m_zhanghu.GetLength() < 6) 
	{
		MyMessageBox(_T("用户名为6-12位"));
		return;
	}

	m_password = m_password.Trim();
	if (m_password.IsEmpty()) 
	{
		MyMessageBox(_T("登录密码不能为空"));
		return;
	}
	if (m_password.GetLength() > 12) 
	{
		MyMessageBox(_T("登录密码为6-12位"));
		return;
	}
	if (m_password.GetLength() < 6) 
	{
		MyMessageBox(_T("登录密码为6-12位"));
		return;
	}

	m_crmpwd = m_crmpwd.Trim();
	if(m_crmpwd != m_password)
	{
		MyMessageBox(_T("两次输入登录密码不一致"));
		return;
	}

	if(m_qkpwd == m_password)
	{
		MyMessageBox(_T("取款密码不能登录密码一致"));
		return;

	}
	m_qkpwd = m_qkpwd.Trim();
	if (m_qkpwd.IsEmpty()) 
	{
		MyMessageBox(_T("取款密码不能为空"));
		return;
	}
	if (m_qkpwd.GetLength() > 12) 
	{
		MyMessageBox(_T("取款密码为6-12位"));
		return;
	}
	if (m_qkpwd.GetLength() < 6) 
	{
		MyMessageBox(_T("取款密码为6-12位"));
		return;
	}

	m_crmqkpwd = m_crmqkpwd.Trim();
	if(m_crmqkpwd != m_qkpwd)
	{
		MyMessageBox(_T("两次输入取款密码不一致"));
		return;
	}

	m_strQQ = m_strQQ.Trim();
 	if (!m_strQQ.IsEmpty())
 	{
		if(m_strQQ.GetLength()<5)
		{
 			MyMessageBox(_T("QQ号码不能小于5位"));
 			return;
		}
 	}

	double fandian = _tstof(m_strFandian);
	SendToServer(2);
}

void CHuiYuanTjxjDlg::OnBnClickedBtnCpRegurl()
{
	const int bufLen = 1024;
	TCHAR text[bufLen] = _T("");

	int nSel = m_cmbWebRegUrl.GetCurSel();

	m_cmbWebRegUrl.GetLBText(nSel,m_strRegUrl);

	
	_tcscpy_s(text, bufLen, m_strRegUrl);
	if (::OpenClipboard(m_hWnd)) 
	{
		::EmptyClipboard();

		HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, (_tcslen(text) + 1) * sizeof(TCHAR));
		LPTSTR pData = (LPTSTR)::GlobalLock(hData);
		_tcscpy_s(pData, _tcslen(text) + 1, text);
		::GlobalUnlock(hData);

		SetClipboardData(CF_UNICODETEXT, hData);
		::CloseClipboard();

		CString strTip;
		strTip.Format(_T("已复制链接到剪贴板"), text);
		MyMessageBox(strTip);
	}
	else
	{
		MyMessageBox(_T("未能复制，直接使用即可"));
	}
}

void CHuiYuanTjxjDlg::AdjustCtrls()
{
	if (m_rioDaili.GetSafeHwnd() != NULL) 
	{
		m_rioDaili.SetWindowPos(NULL, rio_daili_x, rio_daili_y, rio_width, rio_height, SWP_NOZORDER);
	}

	if (m_rioHuiYuan.GetSafeHwnd() != NULL) 
	{
		m_rioHuiYuan.SetWindowPos(NULL, rio_huiyuan_x, rio_huiyuan_y, rio_width, rio_height, SWP_NOZORDER);
	}

	//CWnd* pWnd = GetDlgItem(IDC_EDIT_FANDIAN);
	//if (pWnd != NULL) 
	//{
	//	pWnd->SetWindowPos(NULL, edit_x, edit_fandian_y, edit_width, edit_height, SWP_NOZORDER);
	//}

	if (m_cmbFandian.GetSafeHwnd() != NULL) 
	{
		m_cmbFandian.SetWindowPos(NULL, edit_x-2, edit_fandian_y-6, edit_width+3, edit_height+20, SWP_NOZORDER);
	}

	if (m_cmbWebFandian.GetSafeHwnd() != NULL) 
	{
		m_cmbWebFandian.SetWindowPos(NULL,  edit_x-2, web_fandian_y, edit_width+3, edit_height+20, SWP_NOZORDER);
	}
	if (m_cmbWebRegUrl.GetSafeHwnd() != NULL) 
	{
		m_cmbWebRegUrl.SetWindowPos(NULL,  edit_x-2, web_fandian_y-48, edit_width+100, edit_height+20, SWP_NOZORDER);
	}

	CWnd* pWnd = GetDlgItem(IDC_EDIT_ZHANGHAO);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_zhanghu_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_dl_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_CRM_PWD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_crm_dl_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_dl_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_QK_PWD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_qk_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_CRM_QK_PWD);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_crm_qk_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_QQ);
	if (pWnd != NULL) 
	{
		pWnd->SetWindowPos(NULL, edit_x, edit_qq_y, edit_width, edit_height-2, SWP_NOZORDER);
	}

	if (m_btnOK.GetSafeHwnd() != NULL) 
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}

	if(m_btnRegUrl.GetSafeHwnd() != NULL) 
	{
		m_btnRegUrl.SetWindowPos(NULL,btn_regurl_x , btn_regurl_y-2, m_btnRegUrl.Width(), m_btnRegUrl.Height(), SWP_NOZORDER);
	}
	if(m_btnSetWebFandian.GetSafeHwnd() != NULL) 
	{
		m_btnSetWebFandian.SetWindowPos(NULL, set_fandian_x, set_fandian_y-2, m_btnSetWebFandian.Width(), m_btnSetWebFandian.Height(), SWP_NOZORDER);
	}
	if(m_btnSetMorenMima.GetSafeHwnd() != NULL) 
	{
		m_btnSetMorenMima.SetWindowPos(NULL, set_mima_x, set_mima_y+5, m_btnSetWebFandian.Width(), m_btnSetWebFandian.Height(), SWP_NOZORDER);
	}
}

void CHuiYuanTjxjDlg::EnableCtrls(BOOL b)
{
	if (m_rioDaili.GetSafeHwnd() != NULL) 
	{
		m_rioDaili.EnableWindow(b);
	}

	if (m_rioHuiYuan.GetSafeHwnd() != NULL) 
	{
		m_rioHuiYuan.EnableWindow(b);
	}

	CWnd* pWnd = GetDlgItem(IDC_EDIT_FANDIAN);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_ZHANGHAO);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_CRM_PWD);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_QK_PWD);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	pWnd = GetDlgItem(IDC_EDIT_CRM_QK_PWD);
	if (pWnd != NULL) 
	{
		pWnd->EnableWindow(b);
	}

	if (m_btnOK.GetSafeHwnd() != NULL) 
	{
		m_btnOK.EnableWindow(b);
	}

	if (m_btnRegUrl.GetSafeHwnd() != NULL)
	{
		m_btnRegUrl.EnableWindow(b);
	}

	if (m_cmbFandian.GetSafeHwnd() != NULL) 
	{
		m_cmbFandian.EnableWindow(b);
	}
	if (m_cmbWebFandian.GetSafeHwnd() != NULL) 
	{
		m_cmbWebFandian.EnableWindow(b);
	}
}

void CHuiYuanTjxjDlg::InitFanDianCmb()
{
	m_cmbFandian.ResetContent();
	if (theAccount.fandian*100-0.1 < 0.0001)
	{
		m_cmbFandian.AddString(_T("0.0"));
		m_cmbFandian.SetCurSel(0);
		return;
	}


	if (theAccount.type == 1) {
		if(theAccount.fandian*100 > 2.799999 && theAccount.fandian*100<2.8000001)
		{
			CString str;
			str.Format(_T("2.8"));
			m_cmbFandian.AddString(str);

		}
		//int i=0;
		for(double f=theAccount.fandian*100-0.1; f>=-0.0000000001; f-=0.1/*, i++*/) 
		{
			if(f>2.89999 && f<2.9000001) //排除 2.9
				continue;
			CString str;
			str.Format(_T("%.1lf"), fabs(f));
			m_cmbFandian.AddString(str);
			m_cmbFandian.SetCurSel(0);
		}
	}
	m_cmbWebFandian.ResetContent();
	if (theAccount.fandian*100-0.1 < 0.0001)
	{
		m_cmbWebFandian.AddString(_T("0.0"));
		m_cmbWebFandian.SetCurSel(0);
		return;
	}

	if (theAccount.type == 1) {
		if(theAccount.fandian*100 > 2.799999 && theAccount.fandian*100<2.8000001)
		{
			CString str;
			str.Format(_T("2.8"));
			m_cmbWebFandian.AddString(str);

		}

		//int i=0;
		for(double f=theAccount.fandian*100-0.1; f>=-0.0000000001; f-=0.1/*, i++*/) 
		{
			if(f>2.89999 && f<2.9000001) //排除 2.9
 				continue;
			CString str;
			str.Format(_T("%.1lf"), fabs(f));
			m_cmbWebFandian.AddString(str);
			m_cmbWebFandian.SetCurSel(0);
		}
	}
}





bool CHuiYuanTjxjDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_Peie_RET:
			{
				//效验参数
				ASSERT(wDataSize%sizeof(CMD_GetPeieRet)==0);
				if (wDataSize%sizeof(CMD_GetPeieRet)!=0) return false;

				CMD_GetPeieRet* pCountRet = (CMD_GetPeieRet*)pData;
				int n_peie_1 = pCountRet->n_t_peie_1;
				int n_peie_2 = pCountRet->n_t_peie_2;
				int n_peie_3 = pCountRet->n_t_peie_3;
				int n_peie_s_1 = pCountRet->n_t_peie_s_1;
				int n_peie_s_2 = pCountRet->n_t_peie_s_2;
				int n_peie_s_3 = pCountRet->n_t_peie_s_3;
				m_strPeie.Empty();
				if(theAccount.fandian*100 > 3.099999999999 && theAccount.fandian < 3.100000000000001)
				{
					m_strPeie.Format(L"剩余配额：【3.0】%d个 【2.8】%d个 【2.7】%d个",n_peie_1-n_peie_s_1,n_peie_2-n_peie_s_2,n_peie_3-n_peie_s_3);
				}
				else if(theAccount.fandian*100 > 2.799999999999 && theAccount.fandian*100 < 3.00000001)
				{
					m_strPeie.Format(L"剩余配额：【2.8】%d个 【2.7】%d个",n_peie_2-n_peie_s_2,n_peie_3-n_peie_s_3);
				}
				else 
				{
					return true;
				}

				RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_ERASE);
				return true;

			}
		case SUB_GR_GET_REG_URL_RET:
			{
				//效验参数
				ASSERT(wDataSize%sizeof(CMD_GetRegUrl)==0);
				if (wDataSize%sizeof(CMD_GetRegUrl)!=0) return false;

				int nCount = wDataSize/sizeof(CMD_GetRegUrl);

				m_cmbWebRegUrl.ResetContent();
				for(int i =0;i < nCount;i++)
				{
					CMD_GetRegUrl* pCountRet = (CMD_GetRegUrl*)pData+i;

					m_strRegUrl.Format(_T("%s%d"), pCountRet->s_reg_url, theAccount.user_id);
					m_cmbWebRegUrl.AddString(m_strRegUrl);

					m_strWebFandian1.Format(L"%.1lf",pCountRet->f_t_fandian*100);
				}

				int nSelReg = m_cmbWebRegUrl.FindString(0,m_strRegUrl);
				if(nSelReg == -1)
				{
					m_cmbWebRegUrl.AddString(_T("申请成为代理会员后，即可得到自己的推广链接"));
					nSelReg = m_cmbWebRegUrl.FindString(0,m_strRegUrl);
					m_cmbWebRegUrl.SetCurSel(nSelReg);

				}
				else
					m_cmbWebRegUrl.SetCurSel(nSelReg);

				CRect rcRect;
				rcRect.left = 0;
				rcRect.top = 0;
				rcRect.right = 750;
				rcRect.bottom = 300;


				//CString strFandian;

				int nSel = m_cmbWebFandian.FindString(0,m_strWebFandian1);
				if(nSel == -1)
				{
					m_cmbWebFandian.AddString(_T("0.0"));
					 nSel = m_cmbWebFandian.FindString(0,m_strWebFandian1);
					m_cmbWebFandian.SetCurSel(nSel);
				}
				else
				{
					m_cmbWebFandian.SetCurSel(nSel);
				}

				RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_ERASE);
				return true;
			}
		case SUB_GR_ADD_HY_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_AddHuiYuanRet));
				if(wDataSize!=sizeof(CMD_GR_AddHuiYuanRet)) return false;

				CMD_GR_AddHuiYuanRet* pAddHuiyuan = (CMD_GR_AddHuiYuanRet*)pData;
	
				int res =pAddHuiyuan->n_t_res;
				if(res == 0)
				{
					m_fandian = 0.0;
					m_zhanghu = _T("");
					m_password = _T("");
					m_crmpwd = _T("");
					m_qkpwd = _T("");
					m_crmqkpwd = _T("");
					m_strQQ = _T("");
					UpdateData(FALSE);
				}
// 				else if(res == 1)
// 				{
// 					MyMessageBox(m_zhanghu + _T("-已存在，请尝试其他用户名"));
// 				}
// 				else if(res == 2)
// 				{
// 					MyMessageBox(_T("添加下级失败"));
// 				}
				MyMessageBox(pAddHuiyuan->s_t_Desc);
				return true;
			}
		case SUB_GR_SET_WEB_FANDIAN_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_SetWebFandianRet));
				if(wDataSize!=sizeof(CMD_GR_SetWebFandianRet)) return false;

				CMD_GR_SetWebFandianRet* pSetWebFandianRet = (CMD_GR_SetWebFandianRet*)pData;

				int res =pSetWebFandianRet->n_t_res;
				if(res == 0)
				{
					MyMessageBox(_T("设置网页返点成功"));
				}
				else if(res == 2)
				{
					CString strLog;
					strLog.Format(L"抱歉，设置返点超过限制，最高设定%.2lf",pSetWebFandianRet->f_t_fandian);
					MyMessageBox(strLog);
				}
				else
				{
					MyMessageBox( _T("设置网页返点失败"));
				}

			}

		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CHuiYuanTjxjDlg::SendToServer(int nSendType)
{

	if(nSendType == 1)   //下注记录数量
	{
		m_bGetRegURL = true;
		if(m_bGetRegURL)
		{
			m_bGetRegURL = false;

			CMD_GetUrlFandian GetUrlFandian;
			ZeroMemory(&GetUrlFandian,sizeof(GetUrlFandian));

			GetUrlFandian.n_t_userid = theAccount.user_id;
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_REG_URL,&GetUrlFandian,sizeof(GetUrlFandian));
			return;
		}

	}

	if(nSendType == 2)//下注记录 
	{
		m_AddHuiYuan = true;
		if(m_AddHuiYuan)
		{
			m_AddHuiYuan=false;
			CMD_GP_AddHuiYuan AddHuiYuan;
			ZeroMemory(&AddHuiYuan,sizeof(AddHuiYuan));

			AddHuiYuan.n_t_daili_id = theAccount.user_id;
			AddHuiYuan.n_type = m_type;

			DWORD wSel = m_cmbFandian.GetCurSel();
			m_cmbFandian.GetLBText(wSel,m_strFandian);

			DOUBLE fandian = _tstof(m_strFandian);
			AddHuiYuan.f_t_fandian = fandian/100;

			CString dlPwd;
			CString qkPwd;

			TCHAR szDlPwd[33]=TEXT("");
			TCHAR szQkPwd[33]=TEXT("");


			CMD5Encrypt::EncryptData(m_password,szDlPwd);
			CMD5Encrypt::EncryptData(m_qkpwd,szQkPwd);

			lstrcpyn(AddHuiYuan.s_t_account,m_zhanghu.GetBuffer(),sizeof(AddHuiYuan.s_t_account));
			lstrcpyn(AddHuiYuan.s_t_login_pass,szDlPwd,sizeof(AddHuiYuan.s_t_login_pass));
			lstrcpyn(AddHuiYuan.s_t_qukuan_pass,szQkPwd,sizeof(AddHuiYuan.s_t_qukuan_pass));
			lstrcpyn(AddHuiYuan.s_t_qq,m_strQQ.GetBuffer(),sizeof(AddHuiYuan.s_t_qq));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_ADD_HY,&AddHuiYuan,sizeof(AddHuiYuan));
			return;
		}
	}

	if(nSendType == 3)
	{
		m_bSetWebFandian = true;
		if(m_bSetWebFandian)
		{
			m_bSetWebFandian=false;

			CMD_GP_SetWebFandian SetWebFandian;
			ZeroMemory(&SetWebFandian,sizeof(SetWebFandian));

			SetWebFandian.n_t_userid = theAccount.user_id;

			DWORD wSel = m_cmbWebFandian.GetCurSel();
			m_cmbWebFandian.GetLBText(wSel,m_strWebFandian);
			DOUBLE fandian = _tstof(m_strWebFandian);
			SetWebFandian.f_t_fandian = fandian/100;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SET_WEB_FANDIAN,&SetWebFandian,sizeof(SetWebFandian));
			return;
		}

	}
	if(nSendType == 4)   //下注记录数量
	{
		m_bGetRegURL = true;
		if(m_bGetRegURL)
		{
			m_bGetRegURL = false;

			CMD_GetPeie GetPeie;
			ZeroMemory(&GetPeie,sizeof(GetPeie));

			GetPeie.n_t_userid = theAccount.user_id;
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_Peie,&GetPeie,sizeof(GetPeie));
			return;
		}

	}


}