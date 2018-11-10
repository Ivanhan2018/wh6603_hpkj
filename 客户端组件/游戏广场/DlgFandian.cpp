// DlgFandian.cpp : 实现文件
//

#include "stdafx.h"
#include "GamePlaza.h"
#include "DlgFandian.h"
#include "math.h"

// CDlgFandian 对话框
static const int btn_ok_x = 195;
static const int btn_ok_y = 133;

static const int btn_cancel_x = 410;
static const int btn_cancel_y = 133;


IMPLEMENT_DYNAMIC(CDlgFandian, CDialog)

CDlgFandian::CDlgFandian(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFandian::IDD, pParent)
{
	m_nBonus=0;
	m_nBonusPercent=0.0;
}


CDlgFandian::~CDlgFandian()
{
	if(m_bmpBk)
		delete m_bmpBk;
}

void CDlgFandian::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnPlus);
	DDX_Control(pDX, IDC_BUTTON2, m_btnMinus);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);

}


BEGIN_MESSAGE_MAP(CDlgFandian, CDialog)
	ON_WM_SIZE()

	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgFandian::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgFandian::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgFandian::OnBnClickedButton2)
END_MESSAGE_MAP()

////fDanweiFandian:单位返点，就是1个百分比返点对应多少MONEY
void	CDlgFandian::SetBonus(float nBonus, float nBonusDefault,float nPercent, float fPercentDefault, float fDanweiFandian)
{
	m_nBonus=nBonusDefault;
	m_nBonusPercent=fPercentDefault;
	m_ndata1 = nBonus;
	m_ndata2 = fPercentDefault;
	m_fDanweiFandian=fDanweiFandian;
}

void	CDlgFandian::GetBonus(float &nBonus, float& nPercent)
{
	nBonus = m_ndata1;
	nPercent = m_ndata2;
}

// CDlgFandian 消息处理程序
BOOL CDlgFandian::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\bg_fandian.png"));

	m_ImageBack.LoadImage(CBmpUtil::GetExePath() + _T("skin\\game\\bg_fandian.png"));

	CString strTextImg = CBmpUtil::GetExePath() + _T("skin\\game\\btn_minus.png");
	m_btnPlus.SetImage(strTextImg);
	strTextImg =  CBmpUtil::GetExePath() + _T("skin\\game\\btn_plus.png");
	m_btnMinus.SetImage(strTextImg);

	//m_BkImage.LoadImage(AfxGetInstanceHandle(),TEXT("BK_FANDIAN"));
	CString strBonus, strBonusPercent;
	strBonus.Format(L"%.2f", m_nBonus);
	strBonusPercent.Format(L"%.2f", m_nBonusPercent);
	CStatic* pSta = (CStatic*)GetDlgItem(IDC_STATIC2);
	if(pSta )
		pSta->SetWindowText(strBonus);
	
	pSta = (CStatic*)GetDlgItem(IDC_STATIC1);
	if(pSta)
		pSta->SetWindowText(strBonusPercent);

	int nMax = int(m_nBonusPercent*10+0.5);			//+0.5是为了四色五入
	CSliderCtrl   *pSlidCtrl=(CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	pSlidCtrl->SetRange(0, nMax);
	pSlidCtrl->SetTicFreq(1);

	float fpos = (m_ndata1-m_nBonus)*10/(m_fDanweiFandian);
	int nPos = (int)(fpos+0.1);
	pSlidCtrl->SetPos(nPos);

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\quxiao_bt.png"));

	OutputDebugString(L"DLGFANDIAN OnInitDialog");

// 	m_btnPlus.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnPlus.Width(), m_btnPlus.Height(), SWP_NOZORDER);
// 	m_btnMinus.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnMinus.Width(), m_btnMinus.Height(), SWP_NOZORDER);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	CenterWindow();

	return true;
}

BOOL CDlgFandian::OnEraseBkgnd(CDC* pDC)
{
//	CPaintDC dc(this); // device context for painting
//  	CRect rect;
//  	GetClientRect(&rect);
//   	Graphics graphics(pDC->m_hDC);
//   
//   	graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 
//   		0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);
// 
	OutputDebugString(L"DLGFANDIAN OnEraseBkgnd");

	return TRUE;
}
void CDlgFandian::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

 	CDC cacheDC;
 	cacheDC.CreateCompatibleDC(&dc);
 	CBitmap cacheBmp;
 	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	OutputDebugString(L"DLGFANDIAN OnPaint");

	CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);

  	Graphics graphics(cacheDC.m_hDC);
  
	graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

 	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);
 
 	cacheDC.SelectObject(pOldCacheBmp);
 	cacheBmp.DeleteObject();
 	cacheDC.DeleteDC();

	// Do not call CDialog::OnPaint() for painting messages
}

HBRUSH CDlgFandian::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	DWORD dwCtrlID = pWnd->GetDlgCtrlID();

	if (	dwCtrlID == IDC_STATIC1 ||
			dwCtrlID == IDC_STATIC2	)
	{
			pDC->SetBkMode(TRANSPARENT); 
			HBRUSH B = CreateSolidBrush(RGB(249,241,227)); 
			pDC->SetTextColor(RGB(88,78,77));
			return (HBRUSH) B;
	}

	else if(dwCtrlID == IDC_BUTTON1	||
			dwCtrlID == IDC_BUTTON2	)			
	{
//		pDC->SetBkMode(TRANSPARENT); 
//		HBRUSH B = CreateSolidBrush(RGB(0, 141, 193)); 
//		return (HBRUSH) B; 
	}
	else if(dwCtrlID == IDC_SLIDER1) 
	{
		pDC->SetBkMode(TRANSPARENT); 
		HBRUSH B = CreateSolidBrush(RGB(249,241,227));
		return (HBRUSH)B;
	}
	OutputDebugString(L"DLGFANDIAN OnCtlColor");

	HBRUSH bRush = CreateSolidBrush(RGB(249,241,227)); 
	return bRush;
}

void CDlgFandian::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//使窗口可以拖动
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgFandian::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	CSliderCtrl   *pSlidCtrl=(CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	int nPos  = pSlidCtrl->GetPos();
	CString strBonus, strBonusPercent;
	//刻度是0.1所以要除以10
	strBonus.Format(L"%.2f", m_nBonus+nPos*m_fDanweiFandian/10.0);
	strBonusPercent.Format(L"%1.2f %%", fabs(m_nBonusPercent - nPos/10.0));

	m_ndata1 = _wtof(strBonus);
	m_ndata2 = _wtof(strBonusPercent);

	CStatic* 	pSta = (CStatic*)GetDlgItem(IDC_STATIC1);
	if(pSta)
		pSta->SetWindowText(strBonusPercent);

	pSta = (CStatic*)GetDlgItem(IDC_STATIC2);
	if(pSta )
		pSta->SetWindowText(strBonus);


	OutputDebugString(L"DLGFANDIAN OnNMCustomdrawSlider1");
	*pResult = 0;
}

// --
void CDlgFandian::OnBnClickedButton1()
{
	CSliderCtrl   *pSlidCtrl=(CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	int nPos  = pSlidCtrl->GetPos();
	pSlidCtrl->SetPos(nPos-1);
}

//++
void CDlgFandian::OnBnClickedButton2()
{
	CSliderCtrl   *pSlidCtrl=(CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	int nPos  = pSlidCtrl->GetPos();
	pSlidCtrl->SetPos(nPos+1);}


void CDlgFandian::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	OutputDebugString(L"DLGFANDIAN OnSize");

	if(m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}

	if(m_btnCancel.GetSafeHwnd() != NULL)
	{
		m_btnCancel.SetWindowPos(NULL, btn_cancel_x, btn_cancel_y, m_btnCancel.Width(), m_btnCancel.Height(), SWP_NOZORDER);
	}

}