// DlgAddtional.cpp : 实现文件
//

#include "stdafx.h"
#include "GamePlaza.h"
#include "DlgAddtional.h"

static CRect rcZongJine(463,423,663,450);

// CDlgAddtional 对话框

IMPLEMENT_DYNAMIC(CDlgAddtional, CDialog)

CDlgAddtional::CDlgAddtional(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddtional::IDD, pParent)
{
		m_nFirst=8;
		m_nLast=3;
		m_bBeiTou=false;
		m_bAutoStop=true;
		m_fJine = 2.0f;
		m_nMostQishu= 120;
		m_ZongJine = 0.00f;
}

CDlgAddtional::~CDlgAddtional()
{
	if(m_bmpBk)
		delete m_bmpBk;
}
void CDlgAddtional::OnBnClickedOK()
{
	int nZhuihaoCount = 0;
	nZhuihaoCount = GetZhuiHaoQiShu();

	if(nZhuihaoCount <=0)
	{
		CInformation Information;
		if( Information.ShowMessageBox(L"温馨提醒",L"您追号期数为0 ，是否取消追期？",MB_YESNO,15) == IDYES)
		{
			OnCancel();
		}

	}
	else
	{
		OnOK();
	}
	return;
}
void CDlgAddtional::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//使窗口可以拖动
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}
void CDlgAddtional::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, ID_OK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
// 	DDX_Control(pDX, IDC_CHECK1, m_btCheckBeiTou);
// 	DDX_Control(pDX, IDC_CHECK2, m_btZhongj);

}


BEGIN_MESSAGE_MAP(CDlgAddtional, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()

	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgAddtional::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgAddtional::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgAddtional::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgAddtional::OnBnClickedCheck1)
	ON_BN_CLICKED(ID_OK, &CDlgAddtional::OnBnClickedOK)
	ON_BN_CLICKED(IDC_CHECK2, &CDlgAddtional::OnBnClickedCheck2)
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
//  	ON_NOTIFY(GVN_SELCHANGING, IDC_GRID, OnGridStartSelChange)
// 	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID, OnGridEndSelChange)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
END_MESSAGE_MAP()


// CDlgAddtional 消息处理程序

BOOL CDlgAddtional::OnInitDialog()
{
	CDialog::OnInitDialog();
//	SetIcon(m_hIcon, TRUE);   // 设置大图标
//	SetIcon(m_hIcon, FALSE);  // 设置小图标
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\bg_zhuihao.png"));

	CComboBox* pBtn = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if(pBtn)
	{
		pBtn->InsertString(0, L"--请选择期号--");
		pBtn->InsertString(1, L"5");
		pBtn->InsertString(2, L"15");
		pBtn->InsertString(3, L"20");
		pBtn->InsertString(4, L"25");
		pBtn->InsertString(5, L"全");
		pBtn->SetCurSel(0);
	}
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_EDIT1);
	if(pEdit)
		pEdit->SetWindowText(m_strTime);

	CButton* pBttnn = (CButton*)GetDlgItem(IDC_CHECK2);
	if(pBttnn)
		pBttnn->SetCheck(TRUE);
	m_font.CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnCancel.SetImage(CBmpUtil::GetExePath() + _T("skin\\quxiao_bt.png"));

	CRect rcClient;
	GetClientRect(&rcClient);
	int nWidth = rcClient.Width();
	int nHeight = rcClient.Height();

	m_btnOK.SetWindowPos(NULL,nWidth/2-105,nHeight-36,89,30, SWP_NOZORDER);
	m_btnCancel.SetWindowPos(NULL,nWidth/2+15,nHeight-36,89,30, SWP_NOZORDER);
	GridCtrlInit();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}




void CDlgAddtional::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	Graphics graphics(dc.m_hDC);

	graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 
		0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	dc.SetBkMode(TRANSPARENT);
	COLORREF oldTextColor = dc.SetTextColor(RGB(197, 49, 48));

	CFont * pOldFont = dc.SelectObject(&m_font);
	CString strZongJine;
	strZongJine.Format(L"所选金额:%.2lf元",m_ZongJine);
	dc.DrawText(strZongJine,rcZongJine,SWP_NOZORDER);
	dc.SelectObject(pOldFont);
	dc.SetTextColor(oldTextColor);

	// Do not call CDialog::OnPaint() for painting messages
}




HBRUSH CDlgAddtional::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC) {
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(95,82,81));
		HBRUSH B = CreateSolidBrush(RGB(189,186,185)); 
		return (HBRUSH) B; 
	}

	if(nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() == IDC_EDIT1)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(0,0,0));
		HBRUSH B = CreateSolidBrush(RGB(255,255,255)); 
		return (HBRUSH) B;
	}
	else if( pWnd->GetDlgCtrlID() == IDC_COMBO1)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(95,82,81));
		HBRUSH B = CreateSolidBrush(RGB(200,200,200)); 
		return (HBRUSH) B;
	}
	else if( (pWnd->GetDlgCtrlID() ==  IDC_CHECK1 || pWnd->GetDlgCtrlID() == IDC_CHECK2)|| pWnd->GetDlgCtrlID() == IDC_STATIC_BEITOU|| pWnd->GetDlgCtrlID() == IDC_STATIC_STOP)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(95,82,81));
		
		HBRUSH B = CreateSolidBrush(RGB(189,186,185)); 
		return (HBRUSH) B;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

CString CDlgAddtional::GetNextQiHao()
{
	CString strFirst = m_strQiHao.Left(m_nFirst);
	CString strLast = m_strQiHao.Right(m_nLast);
	int nLast = _wtoi(strLast);
	if(m_nLast == 2)
	{
		if(nLast>=m_nMostQishu)
		{
			int a,b,c ;
			if(m_nFirst == 6)
			{
				a = _ttoi(m_strQiHao.Left(2));
				a+=2000;
				b = _ttoi(m_strQiHao.Mid(2,2));
				c = _ttoi(m_strQiHao.Mid(4,2));

			}
			else
			{
				a = _ttoi(m_strQiHao.Left(4));
				b = _ttoi(m_strQiHao.Mid(4,2));
				c = _ttoi(m_strQiHao.Mid(6,2));
			}
			CTime time(a,b,c,0,0,0);
			time += CTimeSpan(1L,0,0,0);
			if(m_nFirst == 6)
				m_strQiHao.Format(L"%d%02d%02d01", time.GetYear()-2000,time.GetMonth(),time.GetDay());
			else
				m_strQiHao.Format(L"%d%02d%02d01", time.GetYear(),time.GetMonth(),time.GetDay());
		}
		else
			m_strQiHao.Format(L"%s%02ld", strFirst, nLast+1);
	}
	else
	{
		if(nLast>=m_nMostQishu)
		{
			if(m_nGameType == CZ_QiXingCai || m_nGameType ==CZ3D || m_nGameType == CZPaiLie3)
			{
				int a = 0;
				a = _ttoi(strFirst);
				a+=1;

				m_strQiHao.Format(L"%d001", a);
			}
			else if(m_nGameType == CZ_PK10 ||m_nGameType == CZKUAILE8 ||m_nGameType == CZXingYun28)
			{
				m_strQiHao.Format(L"000001");
			}
			else
			{
				int a,b,c ;
				a = _ttoi(m_strQiHao.Left(4));
				b = _ttoi(m_strQiHao.Mid(4,2));
				c = _ttoi(m_strQiHao.Mid(6,2));

				CTime time(a,b,c,0,0,0);
				time += CTimeSpan(1L,0,0,0);
				if(m_nLast==3)
					m_strQiHao.Format(L"%d%02d%02d001", time.GetYear(),time.GetMonth(),time.GetDay());
				else 
					m_strQiHao.Format(L"%d%02d%02d0001", time.GetYear(),time.GetMonth(),time.GetDay());

			}

		}
		else
		{
			if(m_nLast == 3)
				m_strQiHao.Format(L"%s%03ld", strFirst, nLast+1);
			else
				m_strQiHao.Format(L"%s%04ld", strFirst, nLast+1);
		}
	}

	return m_strQiHao;
}
CString CDlgAddtional::GetTzhJine(int nRow)
{	
	CString str ;
	str = m_Grid.GetItemText(nRow,1);
	int nBeishu = _ttoi(str);
	m_strTzhJine.Format(L"%0.2lf",m_fJine*nBeishu);
	return m_strTzhJine;
}
CString CDlgAddtional::GetNextTime(int nPlus)
{
	CString strRet;
	CTime	tmNow = CTime::GetCurrentTime();
	tmNow = tmNow + nPlus*5*60*1000;
	strRet = tmNow.Format(L"(%Y-%m-%d %H.%M.%S)");
	return strRet;
}

void	CDlgAddtional::SetCurrentQihaoTime(CString strQihao, CString strTime,int nFirst,int nLast,DOUBLE fJine,int nMostQishu,int nTypeID)
{
	m_strQiHao = strQihao;
	m_strTime = strTime;
	m_nFirst = nFirst;
	m_nLast = nLast;
	m_fJine=fJine;

	m_nMostQishu = nMostQishu;
	m_nGameType=nTypeID;
	m_Grid.Invalidate();


}

void CDlgAddtional::GridCtrlInit()
{
	m_Grid.EnableTitleTips();
	m_Grid.SetFixedBkColor(RGB(41,0,0));
	m_Grid.SetFixedTextColor(FONT_BG_COLOR);
	m_Grid.SetGridColor(RGB(41,0,0));
	m_Grid.SetGridBkColor(RGB(41,0,0));
	m_Grid.SetGridLineColor(RGB(41,0,0));
	m_Grid.ModifyStyle(WS_BORDER, NULL, NULL);
	m_Grid.SetBkColor(RGB(255,255,255));
    m_Grid.SetEditable(true);
    m_Grid.SetTextBkColor(RGB(200,200,200));			//背景色
	m_Grid.SetTextColor(RGB(95,82,81));				//字体颜色
    m_Grid.SetRowCount(50);								//初始为10行
    m_Grid.SetColumnCount(4);							//初始化为11列
    m_Grid.SetFixedRowCount(1);							//表头为一行
//    m_Grid.SetFixedColumnCount(1);					//表头为一列
	m_Grid.SetColumnWidth(0,100);						//设置0列宽 
	m_Grid.SetColumnWidth(1,128);						//设置各列宽
	m_Grid.SetColumnWidth(2,128);						//设置各列宽
	CRect rcGridRect;

	m_Grid.GetWindowRect(&rcGridRect);
	int nGridWidth = rcGridRect.Width();

	nGridWidth-=378;
	m_Grid.SetColumnWidth(3,nGridWidth);						//设置各列宽
	CString strTxtQiHao;
    for (int row = 0; row < m_Grid.GetRowCount(); row++)
	{
		if(row>0)
			m_Grid.SetCellType(row,0, RUNTIME_CLASS(CGridCellCheck));
		m_Grid.SetItemState(row,2, m_Grid.GetItemState(row,2) | GVIS_READONLY);			//只读属性
		m_Grid.SetItemState(row,3, m_Grid.GetItemState(row,3) | GVIS_READONLY);			//只读属性
		for (int col = 0; col < m_Grid.GetColumnCount(); col++)
		{ 
			//设置表格显示属性
			GV_ITEM Item; 
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			//先处理列头
			if(row==0)
			{
				m_Grid.SetRowHeight(row,25);		//设置各行高          
				if(col ==0)					//第(0，0)格
				{
					Item.nFormat = DT_CENTER|DT_WORDBREAK;
					Item.strText.Format(_T("选择期号"),col);
				}
				else if (col == 1)					//设置0行表头显示
				{        
					Item.nFormat = DT_CENTER|DT_WORDBREAK;
					Item.strText.Format(_T("倍  数"),col);
				}
				else if (col == 2)					//设置0列表头显示
				{
					Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
					Item.strText.Format(_T("期  号"));
				}
				else if(col == 3)
				{
					Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
					Item.strText.Format(_T("金额"),2);
				}
			}
			else
			{
				m_Grid.SetItemBkColour(row,1, RGB(255,255,255));			//只读属性
				m_Grid.SetItemFgColour(row,1, RGB(0,0,0));			//只读属性

				if(col == 1)
				{
					
					Item.strText = L"0";
				}
				else if(col == 2)
				{
					strTxtQiHao=GetNextQiHao();
					Item.strText = strTxtQiHao;
				}
				else if(col == 3)
				{
					CString str;//=GetNextTime(row);
					str.Format(L"%0.3f",m_fJine);
					Item.strText = GetTzhJine(row);//m_strTime;
				}
			}
			m_Grid.SetItem(&Item); 
// 			if(strTxtQiHao.Right(m_nLast) >= L"120" )
// 				return;
		}
	}
}
//期号选择
void CDlgAddtional::OnCbnSelchangeCombo1()
{
	int nIndex=0;
	CComboBox* pBtn = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if(pBtn)
		nIndex = pBtn->GetCurSel();

	if(nIndex>0)
	{
		CString strText;
		pBtn->GetLBText(nIndex, strText);

		int nTotalSel=0;
		if(_wtoi(strText) > 0)
			nTotalSel = _wtoi(strText);
		else
			nTotalSel = 50 - 1;		
		for(int n=1; n<= nTotalSel; n++)
		{
			CString strQishu = m_Grid.GetItemText(n,2);
			if(strQishu.IsEmpty())
				continue;
			CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(n, 0);

			if(pCellCheck->IsKindOf(RUNTIME_CLASS(CGridCellCheck)))
				pCellCheck->SetCheck(TRUE);
			m_Grid.SetItemText(n,1,m_strTime);

			CString strText,str,strLog;
			strText = m_Grid.GetItemText(n,1);
			int nBeishu = _ttoi(strText);
			str.Format(L"%0.3lf",m_fJine*nBeishu);

			m_Grid.SetItemText(n, 3, str);

		//	m_Grid.Invalidate();
		}
		for(int n=nTotalSel+1; n<m_Grid.GetRowCount(); n++)
		{
			CString strQishu = m_Grid.GetItemText(n,2);
			if(strQishu.IsEmpty())
				continue;
			CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(n, 0);
			if(pCellCheck->IsKindOf(RUNTIME_CLASS(CGridCellCheck)))
				pCellCheck->SetCheck(FALSE);			
			m_Grid.SetItemText(n,1,TEXT("0"));

			CString strText,str,strLog;
			strText = m_Grid.GetItemText(n,1);
			int nBeishu = _ttoi(strText);
			str.Format(L"0.00");

			m_Grid.SetItemText(n, 3, str);

		//	m_Grid.Invalidate();
		}
	}
	else
	{
		for(int n=1; n<m_Grid.GetRowCount(); n++)
		{
			CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(n, 0);
			if(pCellCheck->IsKindOf(RUNTIME_CLASS(CGridCellCheck)))
				pCellCheck->SetCheck(FALSE);
			m_Grid.SetItemText(n,1,TEXT("0"));

			CString strText,str,strLog;
			strText = m_Grid.GetItemText(n,1);
			int nBeishu = _ttoi(strText);
			str.Format(L"0.00");

			m_Grid.SetItemText(n, 3, str);

		}
	}
	int nCount = 0;
	nCount = m_Grid.GetRowCount();
	m_ZongJine= 0.00;
	for (int i = 1;i < nCount;i++)
	{
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(i, 0);
		if(pCellCheck->GetCheck() == TRUE)
		{
			DOUBLE fJine = 0.0f;
			CString strJine = m_Grid.GetItemText(i,3);
			fJine = _wtof(strJine.GetBuffer(0));

			m_ZongJine += fJine;

		}
	}
	m_Grid.Invalidate();
	InvalidateRect(rcZongJine);
}
//追期期数
void CDlgAddtional::OnEnChangeEdit2()
{
	CString strText,str;
	int nQi = 0;
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_EDIT2);
	pEdit->GetWindowText(strText);
	nQi = _ttoi(strText);
	if(nQi <= 0)
	{
		MyMessageBox(L"追期期数至少为1！");
		strText = _T("1");
		pEdit->SetWindowText(strText);
	}

	int nCount = 0;
	nCount = m_Grid.GetRowCount();
	m_ZongJine= 0.00;
	for (int i = 1;i < nCount;i++)
	{
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(i, 0);
		if(i<=nQi)
		{
			if(pCellCheck->IsKindOf(RUNTIME_CLASS(CGridCellCheck)))
				pCellCheck->SetCheck();

			m_Grid.SetItemText(i,1,m_strTime);

			CString strText,str,strLog;
			strText = m_Grid.GetItemText(i,1);
			int nBeishu = _ttoi(strText);
			str.Format(L"%0.3lf",m_fJine*nBeishu);

			m_Grid.SetItemText(i, 3, str);


			DOUBLE fJine = 0.0f;
			CString strJine = m_Grid.GetItemText(i,3);
			fJine = _wtof(strJine.GetBuffer(0));

			m_ZongJine += fJine;

		}
		else
		{
			pCellCheck->SetCheck(FALSE);
		}
	}
	OnEnChangeEdit1();

	m_Grid.Invalidate();
	InvalidateRect(rcZongJine);
}

//追期倍数
void CDlgAddtional::OnEnChangeEdit1()
{
	CString strText,str;
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_EDIT1);
	pEdit->GetWindowText(strText);
	if(_ttoi(strText) <= 0)
	{
		MyMessageBox(L"追期倍数至少为1倍！");
		strText = _T("1");
		pEdit->SetWindowText(strText);
	}

	m_strTime = strText;
	int nSelIndex=0;
	for(int n=1; n<m_Grid.GetRowCount(); n++)
	{
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(n, 0);
		if(m_bBeiTou)
		{
			//自动倍投
			if(pCellCheck->GetCheck())
			{
				if(nSelIndex == 0)
				{
					nSelIndex ++;
					nSelIndex = nSelIndex *  _wtoi(strText);
				}
				else
					nSelIndex = nSelIndex *2;
				nSelIndex = min(nSelIndex, 999);

				CString strData;
				strData.Format(L"%ld",	nSelIndex);
				m_Grid.SetItemText(n, 1, strData);
				str.Format(L"%0.3lf",m_fJine*nSelIndex);
				m_Grid.SetItemText(n, 3, str);
				if(_ttoi(strText) <= 0)
				{
					pCellCheck->SetCheck(FALSE);
				}
			}
			else
			{
				strText = _T("0");
				m_Grid.SetItemText(n, 1, strText);
				int nBeishu = _ttoi(strText);
				str.Format(L"%0.2lf",m_fJine*nBeishu);
				m_Grid.SetItemText(n, 3, str);
			}
		}
		else
		{
			if(pCellCheck->GetCheck())
			{
				m_Grid.SetItemText(n, 1, strText);
				if(_ttoi(strText) <= 0)
				{
					pCellCheck->SetCheck(FALSE);
				}
			}
			else
			{
				strText = _T("0");
				m_Grid.SetItemText(n, 1, strText);

			}
			int nBeishu = _ttoi(strText);
			str.Format(L"%0.3lf",m_fJine*nBeishu);
			m_Grid.SetItemText(n, 3, str);
		}

	}
	int nCount = 0;
	nCount = m_Grid.GetRowCount();
	m_ZongJine= 0.00;
	for (int i = 1;i < nCount;i++)
	{
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(i, 0);
		if(pCellCheck->GetCheck() == TRUE)
		{
			DOUBLE fJine = 0.0f;
			CString strJine = m_Grid.GetItemText(i,3);
			fJine = _wtof(strJine.GetBuffer(0));

			m_ZongJine += fJine;

		}
	}
	m_Grid.Invalidate();
	InvalidateRect(rcZongJine);
}

//是否倍投
void CDlgAddtional::OnBnClickedCheck1()
{
	bool bBeiTou = false;
	CButton* pBttnn = (CButton*)GetDlgItem(IDC_CHECK1);
	if(pBttnn)
	{
		m_bBeiTou = pBttnn->GetCheck();
	}
	//按照期号重新计算
	OnEnChangeEdit1();
	int nCount = 0;
	nCount = m_Grid.GetRowCount();
	m_ZongJine= 0.00;
	for (int i = 1;i < nCount;i++)
	{
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(i, 0);
		if(pCellCheck->GetCheck() == TRUE)
		{
			DOUBLE fJine = 0.0f;
			CString strJine = m_Grid.GetItemText(i,3);
			fJine = _wtof(strJine.GetBuffer(0));

			m_ZongJine += fJine;

		}
	}
	m_Grid.Invalidate();
	InvalidateRect(rcZongJine);
}

//中奖后自动停止追号
void CDlgAddtional::OnBnClickedCheck2()
{
	CButton* pBttnn = (CButton*)GetDlgItem(IDC_CHECK2);
	if(pBttnn)
	{
		m_bAutoStop = pBttnn->GetCheck();
	}
}

// GVN_SELCHANGING
void CDlgAddtional::OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
//	MessageBox(_T("OnGridStartSelChange"));

    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
    TRACE(_T("Start Selection Change on row %d, col %d (%d Selected)\n"), 
		pItem->iRow, pItem->iColumn, m_Grid.GetSelectedCount());
}

// GVN_SELCHANGED
void CDlgAddtional::OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{

//	MessageBox(_T("OnGridEndSelChange"));

	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
		//选中了抬头
		if(pItem->iRow == -1 &&  pItem->iColumn == 0)
		{
			CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(0, 0);
			if(pCellCheck->GetCheck())
			{
				for(int n=1; n<m_Grid.GetRowCount(); n++)
				{
					CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(n, 0);
					if(pCellCheck->IsKindOf(RUNTIME_CLASS(CGridCellCheck)))
						pCellCheck->SetCheck(true);
					m_Grid.SetItemText(n,1,m_strTime);
				}
			}
			else
			{
				m_Grid.SetItemText(pItem->iRow,1,_T("0"));
			}
		}
		int nCount = 0;
		nCount = m_Grid.GetRowCount();
		m_ZongJine= 0.00;
		for (int i = 1;i < nCount;i++)
		{
			CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(i, 0);
			if(pCellCheck->GetCheck() == TRUE)
			{
				DOUBLE fJine = 0.0f;
				CString strJine = m_Grid.GetItemText(i,3);
				fJine = _wtof(strJine.GetBuffer(0));

				m_ZongJine += fJine;

			}
		}
		m_Grid.Invalidate();
		InvalidateRect(rcZongJine);

}
// GVN_SELCHANGED
void CDlgAddtional::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	//MessageBox(L"OnGridEndEdit");
	if(pItem->iRow != -1 && pItem->iColumn == 1)
	{
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(pItem->iRow, 0);
		if(!pCellCheck->GetCheck())
		{
			pCellCheck->SetCheck(TRUE);
		}

		CString strText,str,strLog;
		strText = m_Grid.GetItemText(pItem->iRow,pItem->iColumn);

		int nBeishu = _ttoi(strText);
		if(strText.IsEmpty()||nBeishu <=0)
		{
			str.Format(L"0");
			m_Grid.SetItemText(pItem->iRow, 1, str);
			str.Format(L"0.0");

			m_Grid.SetItemText(pItem->iRow, 3, str);
			pCellCheck->SetCheck(FALSE);	
		}
		else
		{
			str.Format(L"%0.3lf",m_fJine*nBeishu);

			m_Grid.SetItemText(pItem->iRow, 3, str);

		}
		int nCount = 0;
		nCount = m_Grid.GetRowCount();
		m_ZongJine= 0.00;
		for (int i = 1;i < nCount;i++)
		{
			CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(i, 0);
			if(pCellCheck->GetCheck() == TRUE)
			{
				DOUBLE fJine = 0.0f;
				CString strJine = m_Grid.GetItemText(i,3);
				fJine = _wtof(strJine.GetBuffer(0));

				m_ZongJine += fJine;

			}
		}
		m_Grid.Invalidate();
		InvalidateRect(rcZongJine);

	}
}
void CDlgAddtional::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{

//	MessageBox(_T("OnGridClick"));
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if(pItem->iRow == 0 &&  pItem->iColumn == 0)
	{
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(0, 0);
		//选中了抬头
		if(!pCellCheck->GetCheck())
		{
			for(int n=1; n<m_Grid.GetRowCount(); n++)
			{
				CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(n, 0);
				if(pCellCheck->IsKindOf(RUNTIME_CLASS(CGridCellCheck)))
				pCellCheck->SetCheck(false);

			}
		}
	}

	if(pItem->iRow != -1 &&  pItem->iColumn == 0)
	{
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(pItem->iRow, 0);
		if(!pCellCheck->GetCheck())
		{
			m_Grid.SetItemText(pItem->iRow,1,L"0");
			CString str;
			str.Format(L"0.00");
			m_Grid.SetItemText(pItem->iRow,3,str);
		}
		else
		{
			m_Grid.SetItemText(pItem->iRow,1,m_strTime);
			int nBeishu = _ttoi(m_strTime);
			CString str;
			str.Format(L"%0.3lf",m_fJine*nBeishu);
			m_Grid.SetItemText(pItem->iRow,3,str);
		}
		int nCount = 0;
		nCount = m_Grid.GetRowCount();
		m_ZongJine= 0.00;
		for (int i = 1;i < nCount;i++)
		{
			CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(i, 0);
			if(pCellCheck->GetCheck() == TRUE)
			{
				DOUBLE fJine = 0.0f;
				CString strJine = m_Grid.GetItemText(i,3);
				fJine = _wtof(strJine.GetBuffer(0));

				m_ZongJine += fJine;

			}
		}
		m_Grid.Invalidate();
		InvalidateRect(rcZongJine);

	}
}

//追期期数
int		CDlgAddtional::GetZhuiHaoQiShu()
{
	int nCout=0;
	for(int n=1; n<= m_Grid.GetRowCount(); n++)
	{
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(n, 0);
		if(pCellCheck && pCellCheck->GetCheck())
		{
			CString str1 = m_Grid.GetItemText(n, 1);
			int nstr = _ttoi(str1);

			if(nstr <= 0)
			{
				continue;
			}
		
			nCout++;
		}
	}
	return nCout;
}


//追期倍数
int		CDlgAddtional::GetZhuiQiBeiShu()
{
	CString strText;
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_EDIT1);
	pEdit->GetWindowText(strText);
	return _wtoi(strText);
}

//是否倍投
bool	CDlgAddtional::GetBeiTou()
{
	return m_bBeiTou;
}

//中奖后自动停止追号
bool	CDlgAddtional::GetPrizeStop()
{
	return m_bAutoStop;
}

//期号等数据
int		CDlgAddtional::GetZhuiHaoData(vector<wstring>&	vecData1, vector<wstring>& vecData2, vector<wstring>& vecData3)
{
	int nCount=0;
	vecData1.clear();
	vecData2.clear();
	vecData3.clear();

	for(int n=1; n< m_Grid.GetRowCount(); n++)
	{
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_Grid.GetCell(n, 0);
		if(pCellCheck && pCellCheck->GetCheck())
		{
			nCount++;
			CString str1 = m_Grid.GetItemText(n, 1);
			CString str2 = m_Grid.GetItemText(n, 2);
			CString str3 = m_Grid.GetItemText(n, 3);

			if(str2.IsEmpty())
				break;
			if(_ttoi(str1) <= 0)
			{
				continue;
			}


			wstring strdata;
			strdata = str1.GetBuffer(0);
			vecData1.push_back(strdata);
			vecData2.push_back(str2.GetBuffer(0));
			vecData3.push_back(str3.GetBuffer(0));

			str1.ReleaseBuffer();
			str2.ReleaseBuffer();
			str3.ReleaseBuffer();
		}
	}
	return nCount;
}