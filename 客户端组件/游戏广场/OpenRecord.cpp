#include "stdafx.h"
#include "GamePlaza.h"
#include "OpenRecord.h"

static const int kj_small_haoma_x = 110;
static const int kj_small_haoma_y = 40;
static const int kj_small_haoma_col_span = 3;
static const int kj_small_haoma_row_span = 11;

static const int bt_close_x = 210;
static const int bt_close_y = 5;
static CRect rcClose(bt_close_x,bt_close_y,bt_close_x+29,bt_close_y+25);
IMPLEMENT_DYNAMIC(COpenRecord, CDialog)

COpenRecord::COpenRecord(CWnd* pParent /*=NULL*/)
	: CDialog(COpenRecord::IDD, pParent)
	, m_smallNumWidth(0)
	, m_smallNumHeight(0)
	, m_kjNumSmall(NULL)
	, m_kjNumSmall11xuan5(NULL)
	, m_btClose(NULL)
	, m_kjXinxiCont(0)
	, m_nCloseIndex(0)
{

}

COpenRecord::~COpenRecord()
{
	if (m_kjNumSmall != NULL) 
	{
		delete m_kjNumSmall;
	}
	if (m_kjNumSmall11xuan5 != NULL) 
	{
		delete m_kjNumSmall11xuan5;
	}
	if (m_btClose != NULL) 
	{
		delete m_btClose;
	}

}
//鼠标消息
VOID COpenRecord::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);
	if(rcClose.PtInRect(Point))
	{
		int nIndex = m_nCloseIndex;
		m_nCloseIndex = 3;
		if(nIndex == 0)
			InvalidateRect(rcClose);
	}
	else
	{
		int nIndex = m_nCloseIndex;
		m_nCloseIndex = 0;
		if(nIndex == 3)
			InvalidateRect(rcClose);
	}
	return ;
}
void COpenRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(COpenRecord, CDialog)
	ON_WM_PAINT()
	//ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDOK, &COpenRecord::OnBnClickedOk)
END_MESSAGE_MAP()


// COpenRecord 消息处理程序
BOOL COpenRecord::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

// 	CDC cacheDC;
// 	cacheDC.CreateCompatibleDC(pDC);
// 	CBitmap cacheBmp;
// 	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	//CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);

	Graphics graphics(pDC->m_hDC);

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);

	if(!m_b11Xuan5)
	{
		DrawTopKjHaoma(pDC,graphics);
	}
	else
	{
		DrawTopKjHaoma11Xuan5(pDC,graphics);
	}
	DrawCloseBtn(pDC,graphics);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);

	
	//cacheDC.SelectObject(pOldCacheBmp);

	return TRUE;
}

void COpenRecord::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
// 	CRect rect;
// 	GetClientRect(&rect);
// 
// 	CDC cacheDC;
// 	cacheDC.CreateCompatibleDC(&dc);
// 	CBitmap cacheBmp;
// 	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
// 
// 	CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);
// 
// 	Graphics graphics(cacheDC.m_hDC);
// 
// 	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);
// 
// 	DrawTopKjHaoma(&cacheDC,graphics);
// 
// 	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);
// 
// 	cacheDC.SelectObject(pOldCacheBmp);
	// 不为绘图消息调用 CDialog::OnPaint()
}
void COpenRecord::DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num)
{
	if(!m_b11Xuan5)
		graphics.DrawImage(img, rc_dest, (num)*m_smallNumWidth, 0, m_smallNumWidth, m_smallNumHeight, UnitPixel);
	else
		graphics.DrawImage(img, rc_dest, (num-1)*m_smallNumWidth, 0, m_smallNumWidth, m_smallNumHeight, UnitPixel);

}
void COpenRecord::DrawCloseBtn(CDC* pDC, Graphics& graphics)
{
	Rect rect(rcClose.left,rcClose.top,rcClose.Width(),rcClose.Height());
	graphics.DrawImage(m_btClose, rect, (m_nCloseIndex)*rcClose.Width(), 0, rcClose.Width(), rcClose.Height(), UnitPixel);

	return;
}
//开奖号码
void COpenRecord::DrawTopKjHaoma(CDC* pDC, Graphics& graphics)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_smallNumfont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(251, 245, 219));

	CRect rcTitle(kj_small_haoma_x-75,kj_small_haoma_y-30,kj_small_haoma_x+100,kj_small_haoma_y);
 	CString strTitle;
 	strTitle.Format(L"%s 开奖结果",theGameType[m_nGameType]);
 	pDC->DrawText(strTitle,&rcTitle, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	pDC->SetTextColor(RGB(88, 78, 77));
	pDC->SelectObject(&m_Numfont);
	for (int cnt=0; cnt<m_kjXinxiCont; cnt++) 
	{

		CString str = m_kjXinxi[cnt].qihao;
		CRect rcText(kj_small_haoma_x - 100, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 80, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span) + m_smallNumHeight);
		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		str = m_kjXinxi[cnt].haoma;
		CString temp;
		temp = str.GetAt(0);

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
					Rect rc(kj_small_haoma_x-20 + index * (m_smallNumWidth + kj_small_haoma_col_span), 
						kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
						m_smallNumWidth, m_smallNumHeight);
					DrawSmallNum(graphics, m_kjNumSmall, rc, num);
				}

				index ++;
			}
		}

	}

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}

//开奖号码
void COpenRecord::DrawTopKjHaoma11Xuan5(CDC* pDC, Graphics& graphics)
{
	CString strText;
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_smallNumfont);
	COLORREF oldTextClr = pDC->SetTextColor(RGB(251, 245, 219));
	CRect rcTitle(kj_small_haoma_x-75-50,kj_small_haoma_y-30,kj_small_haoma_x+150,kj_small_haoma_y);
	CString strTitle;
	strTitle.Format(L"%s 开奖结果",theGameType[m_nGameType]);
	pDC->DrawText(strTitle,&rcTitle, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	pDC->SetTextColor(RGB(88, 78, 77));
	for (int cnt=0; cnt<m_kjXinxiCont; cnt++) 
	{
		CString str = m_kjXinxi[cnt].qihao;
		CRect rcText(kj_small_haoma_x - 100, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
			kj_small_haoma_x + 80, 
			kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span) + m_smallNumHeight);
		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		str = m_kjXinxi[cnt].haoma;
		CString temp;
		temp = str.Left(2);

		if(_ttoi(temp) > 11)
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
				if (num >0 && num < 12 && strTemp.GetLength()>=2) 
				{
					int nX = kj_small_haoma_x;
					nX -= 5;
					Rect rc(nX + index * (m_smallNumWidth + kj_small_haoma_col_span), 
						kj_small_haoma_y + cnt * (m_smallNumHeight + kj_small_haoma_row_span), 
						m_smallNumWidth, m_smallNumHeight);
					DrawSmallNum(graphics, m_kjNumSmall11xuan5, rc, num);
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

	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
}

void COpenRecord::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//使窗口可以拖动
	if(rcClose.PtInRect(point))
	{
		PostMessage(WM_CLOSE);
		return;
	}
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

// void COpenRecord::OnLButtonUp(UINT nFlags, CPoint point)
// {
// 	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	//使窗口可以拖动
// 	if(rcClose.PtInRect(point))
// 	{
// 		PostMessage(WM_CLOSE);
// 		return;
// 	}
// //	PostMessage(WM_NCLBUTTONUP,HTCAPTION,MAKELPARAM(point.x,   point.y));
// 
// 	CDialog::OnLButtonUp(nFlags, point);
// }
void COpenRecord::OnSize(UINT nType, int cx, int cy)
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
		m_btnCancel.SetWindowPos(NULL, 20, 30, m_btnCancel.Width(), m_btnCancel.Height(), SWP_NOZORDER|SWP_NOMOVE);
	}
}
void COpenRecord::SetOpenRecord(CMD_GP_GetMoreRecordRet* pMoreRecord,int nIndex,bool b11xuan5)
{
	memset(&m_kjXinxi[nIndex], 0, sizeof(KjXinXi));
	m_kjXinxiCont = nIndex+1;
	_tcscpy_s(m_kjXinxi[nIndex].qihao, KJ_QIHAO_LENGTH, pMoreRecord->wPeriod);
	_tcscpy_s(m_kjXinxi[nIndex].haoma, KJ_HAOMA_LENGTH, pMoreRecord->szLotNum);
	_tcscpy_s(m_kjXinxi[nIndex].shijian, KJ_SHIJIAN_LENGTH, pMoreRecord->szShijian);
	m_nGameType = pMoreRecord->wKindID;

	m_b11Xuan5 = b11xuan5;

// 	CString strTitle;
// 	strTitle.Format(L"%s",theGameType[m_nGameType]);
// 	SetWindowText(strTitle);
	Invalidate(FALSE);

	return;
}
BOOL COpenRecord::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg4.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\close_bt.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\close_bt.png"));

	m_btnOK.ShowWindow(SW_HIDE);
	m_btnCancel.ShowWindow(SW_HIDE);
	m_kjNumSmall = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small4.png"));
	m_kjNumSmall11xuan5 = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\Num_small1.png"));
	m_smallNumWidth = m_kjNumSmall->GetWidth() / 10;
	m_smallNumHeight = m_kjNumSmall->GetHeight();
	m_btClose = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\close_bt.png"));

	m_smallNumfont.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 
	m_Numfont.CreateFont(18, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	for (int i = 0;i < 20;i++)
	{
		memset(&m_kjXinxi[i], 0, sizeof(KjXinXi));
	}
	//SetWindowPos(NULL, 0, 0, 260, 160, SWP_NOZORDER|SWP_NOMOVE);

	SetWindowPos(NULL,0,0,m_bmpBk->GetWidth(),m_bmpBk->GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COpenRecord::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();

	CDialog::OnOK();
}


void COpenRecord::OnBnClickedOk()
{
	OnOK();
}


