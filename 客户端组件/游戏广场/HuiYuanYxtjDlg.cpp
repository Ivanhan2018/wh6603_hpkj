#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuanYxtjDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"

#include <map>
//////////////////////////////////////////////////////////////////////////////////
map<int ,DWORD> m_UserID2;

//起始时间
static const int tip_time_start_x = 18;
static const int tip_time_start_y = 49;
static const int tip_time_start_width = 70;
static const int tip_time_start_height = 20;
static CRect rc_time_start(tip_time_start_x, 
						   tip_time_start_y, 
						   tip_time_start_x + tip_time_start_width, 
						   tip_time_start_y + tip_time_start_height);

static const int time_start_x = 90;
static const int time_start_y = 49;
static const int time_start_width = 100;
static const int time_start_height = 20;

//截止时间
static const int tip_time_end_x = 190;
static const int tip_time_end_y = 49;
static const int tip_time_end_width = 20;
static const int tip_time_end_height = 20;
static CRect rc_time_end(tip_time_end_x, 
						 tip_time_end_y, 
						 tip_time_end_x + tip_time_end_width, 
						 tip_time_end_y + tip_time_end_height);

static const int time_end_x = 210;
static const int time_end_y = 49;
static const int time_end_width = 100;
static const int time_end_height = 20;

//账号
static const int tip_edit_act_x = 330;
static const int tip_edit_act_y = 49;
static const int tip_edit_act_width = 40;
static const int tip_edit_act_height = 20;
static CRect rc_edit_act(tip_edit_act_x, 
						 tip_edit_act_y, 
						 tip_edit_act_x+tip_edit_act_width, 
						 tip_edit_act_y+tip_edit_act_height);

static const int edit_act_x = 375;
static const int edit_act_y = 49;
static const int edit_act_width = 95;
static const int edit_act_height = 20;

//ID
static const int tip_edit_id_x = 480;
static const int tip_edit_id_y = 49;
static const int tip_edit_id_width = 40;
static const int tip_edit_id_height = 20;
static CRect rc_edit_id(tip_edit_id_x,
						tip_edit_id_y,
						tip_edit_id_x+tip_edit_id_width,
						tip_edit_id_y+tip_edit_id_height);

static const int edit_id_x = 525;
static const int edit_id_y = 49;
static const int edit_id_width = 95;
static const int edit_id_height = 20;

//确定按钮
static const int btn_ok_x = 640;
static const int btn_ok_y = 48;
//列表
static const int list_log_x = 10;
static const int list_log_y = 77;
static const int list_log_width = 987;
static const int list_log_height = 409;

//总页数
static const int tip_page_x = 20;
static const int tip_page_y = 490;
static const int tip_page_width = 400;
static const int tip_page_height = 20;
static CRect rc_tip_page(tip_page_x, 
						 tip_page_y, 
						 tip_page_x + tip_page_width, 
						 tip_page_y + tip_page_height);

//上一页
static const int btn_pre_page_x = 793;
static const int btn_pre_page_y = 490;

//下一页
static const int btn_next_page_x = 873;
static const int btn_next_page_y = 490;
static const int btn_chkxj_x = 2;
static const int btn_chkxj_y = 2;
static const int btn_chkxj_width = 114;
static const int btn_chkxj_height = 25;


static const int page_size = 19;

IMPLEMENT_DYNAMIC(CHuiYuanYxtjDlg, CDialog)

CHuiYuanYxtjDlg::CHuiYuanYxtjDlg(CWnd* pParent /*=NULL*/)
: CDialog(CHuiYuanYxtjDlg::IDD, pParent)
, m_bmpBk(NULL)
, m_tStart(CTime::GetCurrentTime())
, m_tEnd(CTime::GetCurrentTime())
, m_page(1)
, m_itemCount(0)
, m_maxPage(1)
, m_byTime(FALSE)
, m_user_id(0)
, m_strAct(_T(""))
, m_strID(_T(""))
{
	/*	m_tStart = CTime::GetCurrentTime();
	CString starttime ;

	starttime.Format(L"%d-%02d-%02d",m_tStart.GetYear(),m_tStart.GetMonth(),1);


	COleDateTime tm;
	tm.ParseDateTime(starttime);
	SYSTEMTIME st;
	tm.GetAsSystemTime(st);
	CTime t(st);
	m_tStart = t;
	*/
	m_bGetXjYkTjCount=false;
	m_bGetXjYkTj=false;
	m_bGetXjYkTjByID=false;
	m_bGetXjYkTjByAct=false;

}

CHuiYuanYxtjDlg::~CHuiYuanYxtjDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CHuiYuanYxtjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_START, m_dateStart);
	DDX_Control(pDX, IDC_DATE_END, m_dateEnd);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_PRE_PAGE, m_btnPrePage);
	DDX_Control(pDX, IDC_BTN_NEXT_PAGE, m_btnNextPage);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_tStart);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_tEnd);
	DDX_Control(pDX, IDC_EDIT_ACT, m_editAct);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Text(pDX, IDC_EDIT_ACT, m_strAct);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);

	DDX_Control(pDX, IDC_BTN_CHKXJ_1, m_btnChkxj[0]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_2, m_btnChkxj[1]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_3, m_btnChkxj[2]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_4, m_btnChkxj[3]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_5, m_btnChkxj[4]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_6, m_btnChkxj[5]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_7, m_btnChkxj[6]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_8, m_btnChkxj[7]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_9, m_btnChkxj[8]);
	DDX_Control(pDX, IDC_BTN_CHKXJ_10, m_btnChkxj[9]);

}

BEGIN_MESSAGE_MAP(CHuiYuanYxtjDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CHuiYuanYxtjDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CHuiYuanYxtjDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CHuiYuanYxtjDlg::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LOG, &CHuiYuanYxtjDlg::OnNMRClickListLog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LOG, &CHuiYuanYxtjDlg::OnNMLclickListLog)
	ON_COMMAND(ID_HYYK_CHKXJMX, &CHuiYuanYxtjDlg::OnXjyktjChkmx)
	ON_COMMAND(ID_HYYK_CHKXJYK, &CHuiYuanYxtjDlg::OnXjyktjChkxx)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_1, &CHuiYuanYxtjDlg::OnBnClickedBtnChkxj1)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_2, &CHuiYuanYxtjDlg::OnBnClickedBtnChkxj2)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_3, &CHuiYuanYxtjDlg::OnBnClickedBtnChkxj3)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_4, &CHuiYuanYxtjDlg::OnBnClickedBtnChkxj4)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_5, &CHuiYuanYxtjDlg::OnBnClickedBtnChkxj5)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_6, &CHuiYuanYxtjDlg::OnBnClickedBtnChkxj6)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_7, &CHuiYuanYxtjDlg::OnBnClickedBtnChkxj7)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_8, &CHuiYuanYxtjDlg::OnBnClickedBtnChkxj8)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_9, &CHuiYuanYxtjDlg::OnBnClickedBtnChkxj9)
	ON_BN_CLICKED(IDC_BTN_CHKXJ_10, &CHuiYuanYxtjDlg::OnBnClickedBtnChkxj10)

END_MESSAGE_MAP()


// CHuiYuanYxtjDlg 消息处理程序

void CHuiYuanYxtjDlg::OnPaint()
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

	cacheDC.DrawText(_T("起始时间"), rc_time_start, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("至"), rc_time_end, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("账户"), rc_edit_act, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("ID"), rc_edit_id, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

	//CString strTmp;
	//strTmp.Format(_T("第%d页/共%d页，每页%d条记录，共%d条记录"), 
	//	m_page, m_maxPage, page_size, m_itemCount);
	//cacheDC.DrawText(strTmp, rc_tip_page, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	CString strTmp;
	/*	if ((!m_strID.IsEmpty()) || (!m_strAct.IsEmpty()))
	{
	strTmp.Format(_T("共查找到%d条记录"), m_listLog.GetItemCount());
	}
	else
	*/	{
		strTmp.Format(_T("第%d页/共%d页，每页%d条记录，共%d条记录"), 
			m_page, m_maxPage, page_size, m_itemCount);
	}
	cacheDC.DrawText(strTmp, rc_tip_page, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CHuiYuanYxtjDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanYxtjDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

void CHuiYuanYxtjDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();
		for(int i = 1;i < CountArray(m_btnChkxj);i++)
		{
			m_btnChkxj[i].ShowWindow(SW_HIDE);
		}

		m_btnChkxj[m_nIndex].ShowWindow(SW_SHOW);
		m_btnChkxj[m_nIndex].SetWindowText(theAccount.account);

		m_byType=0;
		m_page = 1;
		m_user_id = theAccount.user_id;
		EnAbleCtrls(true);
	}
	else
	{
		m_listLog.DeleteAllItems();
		m_page = 1;
		m_editAct.SetWindowText(L"");
		m_editID.SetWindowText(L"");
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_dateStart.SetTime(&m_tStart);
		m_dateEnd.SetTime(&m_tEnd);
	}
}

void CHuiYuanYxtjDlg::OnBnClickedBtnOk()
{
	if(m_dwTickCount == 0)
	{
		m_dwTickCount = GetTickCount();
	}
	else if( GetTickCount()-m_dwTickCount <1000)
	{
		MyMessageBox(L"您点击过于频繁，请稍等片刻！");
		return;
	}
	m_dwTickCount = GetTickCount();

	UpdateData();
	if ((!m_strID.IsEmpty()))
	{
		int xj_id = _ttoi(m_strID);
		m_user_id = xj_id;
		if (xj_id <= 0) {
			MyMessageBox(_T("您输入的ID不对"));
			return;
		}
		m_byType=1;

	}
	else if (!m_strAct.IsEmpty())
	{
		m_byType=2;
	}
	else 
	{
		m_byType=0;
		m_byType=0;
		m_nIndex = 0;
		CString strText;

		for(int i = m_nIndex;i < CountArray(m_btnChkxj);i++)
		{
			if(i<=m_nIndex)
				m_btnChkxj[i].ShowWindow(SW_SHOW);
			else
				m_btnChkxj[i].ShowWindow(SW_HIDE);
		}

	}
	if (m_tStart > m_tEnd)
	{
		MyMessageBox(_T("选择的时间区间不对"));
		return;
	}
// 	else if(m_tStart == m_tEnd)
// 	{
// 		m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 
// 	}
	m_byTime = TRUE;
	m_listLog.DeleteAllItems();
	m_page = 1;
	SendToServer(1);
	SendToServer(2);
	m_btnOK.EnableWindow(FALSE);

	InvalidateRect(rc_tip_page);
}

void CHuiYuanYxtjDlg::OnBnClickedBtnPrePage()
{
	if(m_page == 1)
	{
		MyMessageBox(_T("已经是第一页了"));
		return;
	}

	m_page--;
	m_listLog.DeleteAllItems();
	SendToServer(2);
	InvalidateRect(rc_tip_page);

}

void CHuiYuanYxtjDlg::OnBnClickedBtnNextPage()
{
	if(m_page == m_maxPage)
	{
		MyMessageBox(_T("已经是最后一页了"));
		return;
	}

	m_page++;
	m_listLog.DeleteAllItems();
	SendToServer(2);
	InvalidateRect(rc_tip_page);
}

void CHuiYuanYxtjDlg::OnXjyktjChkxx()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		CString strType = m_listLog.GetItemText(nItem, 2);
		if (strType == _T("会员"))
		{
			MyMessageBox(_T("此人不是代理，无下级"));
		}
		else
		{
			m_user_id = m_listLog.GetItemData(nItem);
			CString  strAccount = m_listLog.GetItemText(nItem,1);
			if(m_nIndex<7 && m_user_id!=m_UserID2[m_nIndex])
			{
				m_nIndex++;
				m_btnChkxj[m_nIndex].ShowWindow(SW_SHOW);
				m_btnChkxj[m_nIndex].SetWindowText(strAccount);
				m_UserID2[m_nIndex] = m_user_id;

			}

			SendToServer(1);
			SendToServer(2);
		}
	}
}


void CHuiYuanYxtjDlg::OnXjyktjChkmx()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		int t_id = m_listLog.GetItemData(nItem);

		CWnd *pParent = GetParent();
		if(pParent!=NULL)
		{
			pParent->PostMessage(IDM_QUERY_XJ_YXYKMX,t_id,0);
		}
	}

}
void CHuiYuanYxtjDlg::OnNMRClickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{

	CPoint pt(0,0);
	GetCursorPos(&pt);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_CHKXJYK);
	CMenu* pMenu = menu.GetSubMenu(0);
	if (pMenu != NULL)
	{
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}

	*pResult = 0;
	return;
}
void CHuiYuanYxtjDlg::OnNMLclickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}

	CPoint pt(0,0);
	GetCursorPos(&pt);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_CHKXJYK);
	CMenu* pMenu = menu.GetSubMenu(0);
	if (pMenu != NULL)
	{
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}


	return;
}

BOOL CHuiYuanYxtjDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg3.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt.png"));
	m_btnPrePage.SetImage(CBmpUtil::GetExePath() + _T("skin\\syy_bt.png"));
	m_btnNextPage.SetImage(CBmpUtil::GetExePath() + _T("skin\\xyy_bt.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
	m_byType=0;
	InitListCtrl();
	m_dwTickCount=0;
	m_editAct.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(20,20,20));
	m_editID.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(20,20,20));
	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		m_btnChkxj[i].SetBkImage(CBmpUtil::GetExePath() + _T("skin\\btn_xj.png"));
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHuiYuanYxtjDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuiYuanYxtjDlg::OnOK()
{
	//CDialog::OnOK();
}

void CHuiYuanYxtjDlg::InitListCtrl()
{
	m_listLog.ModifyStyle(NULL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(95,82,81));

	m_listLog.InsertColumn(0, _T("用户ID"), LVCFMT_CENTER, 100);
	m_listLog.InsertColumn(1, _T("用户名"), LVCFMT_CENTER, 153);
	m_listLog.InsertColumn(2, _T("类型"), LVCFMT_CENTER, 100);
	m_listLog.InsertColumn(3, _T("代理ID"), LVCFMT_CENTER, 100);
	m_listLog.InsertColumn(4, _T("团队棋牌返点"), LVCFMT_CENTER, 130);
	m_listLog.InsertColumn(5, _T("团队棋牌盈亏"), LVCFMT_CENTER, 200);
	m_listLog.InsertColumn(6, _T("团队棋牌余额"), LVCFMT_CENTER, 200);
}

void CHuiYuanYxtjDlg::AdjustCtrls()
{
	if (m_dateStart.GetSafeHwnd() != NULL)
	{
		m_dateStart.SetWindowPos(NULL, time_start_x, time_start_y, time_start_width, time_start_height, SWP_NOZORDER);
	}
	if (m_dateEnd.GetSafeHwnd() != NULL)
	{
		m_dateEnd.SetWindowPos(NULL, time_end_x, time_end_y, time_end_width, time_end_height, SWP_NOZORDER);
	}
	if (m_listLog.GetSafeHwnd() != NULL)
	{
		m_listLog.SetWindowPos(NULL, list_log_x, list_log_y, list_log_width, list_log_height, SWP_NOZORDER);
	}
	if (m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}
	if (m_btnPrePage.GetSafeHwnd() != NULL)
	{
		m_btnPrePage.SetWindowPos(NULL, btn_pre_page_x, btn_pre_page_y, m_btnPrePage.Width(), m_btnPrePage.Height(), SWP_NOZORDER);
	}
	if (m_btnNextPage.GetSafeHwnd() != NULL)
	{
		m_btnNextPage.SetWindowPos(NULL, btn_next_page_x, btn_next_page_y, m_btnNextPage.Width(), m_btnNextPage.Height(), SWP_NOZORDER);
	}
	if (m_editAct.GetSafeHwnd() != NULL)
	{
		m_editAct.SetWindowPos(NULL, edit_act_x, edit_act_y, edit_act_width, edit_act_height, SWP_NOZORDER);
	}
	if (m_editID.GetSafeHwnd() != NULL)
	{
		m_editID.SetWindowPos(NULL, edit_id_x, edit_id_y, edit_id_width, edit_id_height, SWP_NOZORDER);
	}

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		m_btnChkxj[i].SetWindowPos(NULL,btn_chkxj_x+i*(btn_chkxj_width),btn_chkxj_y,btn_chkxj_width,btn_chkxj_height,SWP_NOZORDER);
		if(i>7)
		{
			m_btnChkxj[i].ShowWindow(SW_HIDE);
		}
	}

}

void CHuiYuanYxtjDlg::EnAbleCtrls(bool b)
{
	m_btnPrePage.EnableWindow(b);
	m_btnNextPage.EnableWindow(b);
}





//读取事件
bool CHuiYuanYxtjDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_XJYX_TJ_COUNT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetXJYXTjCountRet));
				if(wDataSize!=sizeof(CMD_GR_GetXJYXTjCountRet)) return false;

				CMD_GR_GetXJYXTjCountRet* pCountRet = (CMD_GR_GetXJYXTjCountRet*)pData;

				m_itemCount = pCountRet->dwCount;
				m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);
				m_maxPage = max(1,m_maxPage);

				break;
			}
		case SUB_GP_GET_XJYX_TJ_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_GetXJYXTjRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetXJYXTjRet)!=0) return false;

				m_btnOK.EnableWindow(TRUE);
				m_listLog.DeleteAllItems();
				WORD wCount = wDataSize/sizeof(CMD_GR_GetXJYXTjRet);
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetXJYXTjRet *pXJYKLogRet = ((CMD_GR_GetXJYXTjRet*)pData+i);

					if(pXJYKLogRet->n_t_id == theAccount.user_id)
					{
						CString strTmp;
						strTmp.Format(_T("%d"), pXJYKLogRet->n_t_id);
						int item = m_listLog.InsertItem(0, strTmp);
						m_listLog.SetItemData(0, pXJYKLogRet->n_t_id);
						if(m_byType!=0)
						{
							for(map<int ,DWORD >::iterator pos = m_UserID2.begin();pos!=m_UserID2.end();++pos)
							{
								if(pos->second == m_user_id)
								{
									m_nIndex = pos->first;
									break;
								}
								else
								{
									m_nIndex = 1;
								}
							}
							for(int i = 0;i < CountArray(m_btnChkxj);i++)
							{
								if(i<=m_nIndex)
								{
									m_btnChkxj[i].ShowWindow(SW_SHOW);
								}
								else
								{
									m_btnChkxj[i].ShowWindow(SW_HIDE);
								}
							}
							m_btnChkxj[m_nIndex].SetWindowText(pXJYKLogRet->s_t_account);
							m_UserID2[m_nIndex] = pXJYKLogRet->n_t_id;

						}

						m_listLog.SetItemText(0, 1,pXJYKLogRet->s_t_account);

						int t_type = pXJYKLogRet->n_t_type;
						switch (t_type)
						{
						case 0: strTmp = _T("会员"); break;
						case 1: strTmp = _T("代理"); break;
						default: strTmp = _T(""); break;
						}
						m_listLog.SetItemText(0, 2, strTmp);

						strTmp.Format(_T("%d"), pXJYKLogRet->n_t_daili_id);
						m_listLog.SetItemText(0, 3, strTmp);


						DOUBLE fandian = pXJYKLogRet->f_t_fandian_ze;



						strTmp.Format(_T("%.2lf"), fandian);
						m_listLog.SetItemText(0, 4, strTmp);

						strTmp.Format(_T("%.2lf"),pXJYKLogRet->f_t_qipai_ze);
						m_listLog.SetItemText(0, 5, strTmp);

						strTmp.Format(_T("%.2lf"), pXJYKLogRet->f_t_tuandui_ye);
						m_listLog.SetItemText(0, 6, strTmp);



					}
					else
					{
						CString strTmp;
						strTmp.Format(_T("%d"), pXJYKLogRet->n_t_id);
						int item = m_listLog.InsertItem(m_listLog.GetItemCount(), strTmp);
						m_listLog.SetItemData(item, pXJYKLogRet->n_t_id);
						if(m_byType!=0)
						{
							for(map<int ,DWORD >::iterator pos = m_UserID2.begin();pos!=m_UserID2.end();++pos)
							{
								if(pos->second == m_user_id)
								{
									m_nIndex = pos->first;
									break;
								}
								else
								{
									m_nIndex = 1;
								}
							}
							for(int i = 0;i < CountArray(m_btnChkxj);i++)
							{
								if(i<=m_nIndex)
								{
									m_btnChkxj[i].ShowWindow(SW_SHOW);
								}
								else
								{
									m_btnChkxj[i].ShowWindow(SW_HIDE);
								}
							}
							m_btnChkxj[m_nIndex].SetWindowText(pXJYKLogRet->s_t_account);
							m_UserID2[m_nIndex] = pXJYKLogRet->n_t_id;

						}

						m_listLog.SetItemText(item, 1,pXJYKLogRet->s_t_account);

						int t_type = pXJYKLogRet->n_t_type;
						switch (t_type)
						{
						case 0: strTmp = _T("会员"); break;
						case 1: strTmp = _T("代理"); break;
						default: strTmp = _T(""); break;
						}
						m_listLog.SetItemText(item, 2, strTmp);

						strTmp.Format(_T("%d"), pXJYKLogRet->n_t_daili_id);
						m_listLog.SetItemText(item, 3, strTmp);


						DOUBLE fandian = pXJYKLogRet->f_t_fandian_ze;

						strTmp.Format(_T("%.2lf"), fandian);
						m_listLog.SetItemText(item, 4, strTmp);

						strTmp.Format(_T("%.2lf"), pXJYKLogRet->f_t_qipai_ze);
						m_listLog.SetItemText(item, 5, strTmp);


						strTmp.Format(_T("%.2lf"), pXJYKLogRet->f_t_tuandui_ye);
						m_listLog.SetItemText(item, 6, strTmp);


					}

				}
				break;
			}

		}
	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CHuiYuanYxtjDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //下注记录数量
	{
		m_bGetXjYkTjCount = true;
		if(m_bGetXjYkTjCount)
		{
			m_bGetXjYkTjCount = false;
			CMD_GP_GetXJYXTjCount GetXJYXTjCount;
			ZeroMemory(&GetXJYXTjCount,sizeof(GetXJYXTjCount));

			GetXJYXTjCount.dwUserID = m_user_id;
			GetXJYXTjCount.n_t_type = m_byType;
			int xj_id = _ttoi(m_strID);
			GetXJYXTjCount.n_t_user_id = xj_id;
			lstrcpyn(GetXJYXTjCount.s_t_account,m_strAct.GetBuffer(),sizeof(GetXJYXTjCount.s_t_account));

			GetXJYXTjCount.bTime = m_byTime;
// 			if(m_tStart == m_tEnd)
// 			{
// 				m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}
// 			m_tEnd += CTimeSpan(0, 23, 59, 59);

			CopyMemory(GetXJYXTjCount.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetXJYXTjCount.szTimeStart));
			CopyMemory(GetXJYXTjCount.szTimeEnd , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetXJYXTjCount.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJYX_TJ_COUNT,&GetXJYXTjCount,sizeof(GetXJYXTjCount));
			return;
		}

		return;
	}

	if(nSendType == 2)
	{
		m_bGetXjYkTj = true;
		if(m_bGetXjYkTj)
		{
			m_bGetXjYkTj = false;
			CMD_GP_GetXJYXTj GetXJYXTj;
			ZeroMemory(&GetXJYXTj,sizeof(GetXJYXTj));

			GetXJYXTj.dwUserID = m_user_id;
			GetXJYXTj.n_t_type = m_byType;
			int xj_id = _ttoi(m_strID);
			GetXJYXTj.n_t_user_id = xj_id;
			lstrcpyn(GetXJYXTj.s_t_account,m_strAct.GetBuffer(),sizeof(GetXJYXTj.s_t_account));

			GetXJYXTj.nPage = m_page;
			GetXJYXTj.nSize = page_size;
			GetXJYXTj.bByTime = m_byTime;

			CopyMemory(GetXJYXTj.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetXJYXTj.szTimeStart));
			CopyMemory(GetXJYXTj.szTimeEnd  , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetXJYXTj.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJYX_TJ,&GetXJYXTj,sizeof(GetXJYXTj));
			return;

		}

	}


}

void CHuiYuanYxtjDlg::OnBnClickedBtnChkxj1()
{
	m_nIndex = 0;
	CString strText;

	for(int i = m_nIndex;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_page = 1;
	//m_btnChkxj[m_nIndex].ShowWindow(SW_HIDE);
	// 	m_btnChkxj[m_nIndex].GetWindowText(strText);
	// 
	// 	if(strText.IsEmpty())
	// 		return;
	m_editAct.SetWindowText(L"");
	m_editID.SetWindowText(L"");
	OnBnClickedBtnOk();
}
void CHuiYuanYxtjDlg::OnBnClickedBtnChkxj2()
{
	m_nIndex = 1;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	//m_btnChkxj[m_nIndex].ShowWindow(SW_HIDE);
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID2[m_nIndex]);
	m_user_id = m_UserID2[m_nIndex];
	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;

	m_byType=0;
	//OnBnClickedBtnOk();
	SendToServer(1);
	SendToServer(2);
}
void CHuiYuanYxtjDlg::OnBnClickedBtnChkxj3()
{
	m_nIndex = 2;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID2[m_nIndex]);
	m_user_id = m_UserID2[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	SendToServer(1);
	SendToServer(2);

}
void CHuiYuanYxtjDlg::OnBnClickedBtnChkxj4()
{
	m_nIndex = 3;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID2[m_nIndex]);
	m_user_id = m_UserID2[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	SendToServer(1);
	SendToServer(2);

}
void CHuiYuanYxtjDlg::OnBnClickedBtnChkxj5()
{
	m_nIndex = 4;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID2[m_nIndex]);
	m_user_id = m_UserID2[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	SendToServer(1);
	SendToServer(2);

}
void CHuiYuanYxtjDlg::OnBnClickedBtnChkxj6()
{
	m_nIndex = 5;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	//m_btnChkxj[m_nIndex].ShowWindow(SW_HIDE);
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID2[m_nIndex]);
	m_user_id = m_UserID2[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	SendToServer(1);
	SendToServer(2);
}
void CHuiYuanYxtjDlg::OnBnClickedBtnChkxj7()
{
	m_nIndex = 6;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID2[m_nIndex]);
	m_user_id = m_UserID2[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	SendToServer(1);
	SendToServer(2);
}
void CHuiYuanYxtjDlg::OnBnClickedBtnChkxj8()
{
	m_nIndex = 7;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID2[m_nIndex]);
	m_user_id = m_UserID2[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	SendToServer(1);
	SendToServer(2);
}
void CHuiYuanYxtjDlg::OnBnClickedBtnChkxj9()
{
	m_nIndex = 8;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	CString strID;
	strID.Format(L"%d",m_UserID2[m_nIndex]);
	m_user_id = m_UserID2[m_nIndex];

	m_editID.SetWindowText(strID);
	m_strID = strID;
	m_page = 1;
	m_byType=0;

	//	OnBnClickedBtnOk();
	SendToServer(1);
	SendToServer(2);
}
void CHuiYuanYxtjDlg::OnBnClickedBtnChkxj10()
{
	m_nIndex = 9;
	CString strText;

	for(int i = 0;i < CountArray(m_btnChkxj);i++)
	{
		if(i<=m_nIndex)
			m_btnChkxj[i].ShowWindow(SW_SHOW);
		else
			m_btnChkxj[i].ShowWindow(SW_HIDE);
	}
	m_btnChkxj[m_nIndex].GetWindowText(strText);

	if(strText.IsEmpty())
		return;
	m_editAct.SetWindowText(strText);
	m_editID.SetWindowText(L"");
	OnBnClickedBtnOk();
}