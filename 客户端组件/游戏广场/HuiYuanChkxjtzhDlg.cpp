#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuanChkxjtzhDlg.h"
#include "TzhxxDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
//起始时间
static const int tip_time_start_x = 18;
static const int tip_time_start_y = 10;
static const int tip_time_start_width = 70;
static const int tip_time_start_height = 20;
static CRect rc_time_start(tip_time_start_x, 
						   tip_time_start_y, 
						   tip_time_start_x + tip_time_start_width, 
						   tip_time_start_y + tip_time_start_height);

static const int time_start_x = 90;
static const int time_start_y = 8;
static const int time_start_width = 100;
static const int time_start_height = 20;

//截止时间
static const int tip_time_end_x = 190;
static const int tip_time_end_y = 10;
static const int tip_time_end_width = 20;
static const int tip_time_end_height = 20;
static CRect rc_time_end(tip_time_end_x, 
						 tip_time_end_y, 
						 tip_time_end_x + tip_time_end_width, 
						 tip_time_end_y + tip_time_end_height);

static const int time_end_x = 210;
static const int time_end_y = 8;
static const int time_end_width = 100;
static const int time_end_height = 20;

//账号
static const int tip_edit_act_x = 330;
static const int tip_edit_act_y = 8;
static const int tip_edit_act_width = 40;
static const int tip_edit_act_height = 20;
static CRect rc_edit_act(tip_edit_act_x, 
												 tip_edit_act_y, 
												 tip_edit_act_x+tip_edit_act_width, 
												 tip_edit_act_y+tip_edit_act_height);

static const int edit_act_x = 375;
static const int edit_act_y = 8;
static const int edit_act_width = 95;
static const int edit_act_height = 20;

//ID
static const int tip_edit_id_x = 480;
static const int tip_edit_id_y = 8;
static const int tip_edit_id_width = 40;
static const int tip_edit_id_height = 20;
static CRect rc_edit_id(tip_edit_id_x,
												tip_edit_id_y,
												tip_edit_id_x+tip_edit_id_width,
												tip_edit_id_y+tip_edit_id_height);

static const int edit_id_x = 525;
static const int edit_id_y = 8;
static const int edit_id_width = 95;
static const int edit_id_height = 20;


//确定按钮
static const int btn_ok_x = 640;
static const int btn_ok_y = 5;

//列表
static const int list_log_x = 10;
static const int list_log_y = 38;
static const int list_log_width = 987;
static const int list_log_height = 448;

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


static const int page_size = 19;

IMPLEMENT_DYNAMIC(CHuiYuanChkxjtzhDlg, CDialog)

CHuiYuanChkxjtzhDlg::CHuiYuanChkxjtzhDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuiYuanChkxjtzhDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_tStart(CTime::GetCurrentTime())
	, m_tEnd(CTime::GetCurrentTime())
	, m_page(1)
	, m_itemCount(0)
	, m_maxPage(1)
	, m_byTime(TRUE)
	, m_user_id(0)
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
	m_bGetXjtzhLogCount = false;
	m_bGetXjtzhLog = false;

}

CHuiYuanChkxjtzhDlg::~CHuiYuanChkxjtzhDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CHuiYuanChkxjtzhDlg::DoDataExchange(CDataExchange* pDX)
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
}


BEGIN_MESSAGE_MAP(CHuiYuanChkxjtzhDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CHuiYuanChkxjtzhDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CHuiYuanChkxjtzhDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CHuiYuanChkxjtzhDlg::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOG, &CHuiYuanChkxjtzhDlg::OnNMDblclkListLog)
	ON_COMMAND(ID_CHKXJTZH_CHKXJ, &CHuiYuanChkxjtzhDlg::OnChkxjtzhChkxj)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LOG, &CHuiYuanChkxjtzhDlg::OnNMRClickListLog)
	ON_COMMAND(ID_CHKXJTZH_CHKXX, &CHuiYuanChkxjtzhDlg::OnChkxjtzhChkxx)
END_MESSAGE_MAP()


// CHuiYuanChkxjtzhDlg 消息处理程序

void CHuiYuanChkxjtzhDlg::OnPaint()
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
// 	if /*((!m_strID.IsEmpty()) ||*/ (!m_strAct.IsEmpty())/*)*/
// 	{
// 		strTmp.Format(_T("共查找到%d条记录"), m_listLog.GetItemCount());
// 	}
// 	else
	{
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

BOOL CHuiYuanChkxjtzhDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanChkxjtzhDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

void CHuiYuanChkxjtzhDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

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


void CHuiYuanChkxjtzhDlg::OnBnClickedBtnOk()
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

	}
	if (m_tStart > m_tEnd)
	{
		MyMessageBox(_T("选择的时间区间不对"));
		return;
	}
// 	else if(m_tStart == m_tEnd)
// 		m_tEnd += CTimeSpan(1L, 0, 0, 0);
	m_page = 1;

	m_byTime = TRUE;
	m_listLog.DeleteAllItems();
	SendToServer(1);
	SendToServer(2);

	InvalidateRect(rc_tip_page);
}

void CHuiYuanChkxjtzhDlg::OnBnClickedBtnPrePage()
{
	if(m_page <= 1)
	{
		MyMessageBox(_T("已经是第一页了"));
		return;
	}

	m_page--;
	m_listLog.DeleteAllItems();
	SendToServer(2);
	InvalidateRect(rc_tip_page);
}

void CHuiYuanChkxjtzhDlg::OnBnClickedBtnNextPage()
{
	if(m_page >= m_maxPage)
	{
		MyMessageBox(_T("已经是最后一页了"));
		return;
	}

	m_page++;
	m_listLog.DeleteAllItems();
	SendToServer(2);
	InvalidateRect(rc_tip_page);
}

void CHuiYuanChkxjtzhDlg::OnNMDblclkListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		int t_id = m_listLog.GetItemData(nItem);

		CTzhxxDlg dlg;
		dlg.m_id = t_id;
		dlg.DoModal();
	}
	*pResult = 0;
}

void CHuiYuanChkxjtzhDlg::OnChkxjtzhChkxj()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		//CString strType = m_listLog.GetItemText(nItem, 3);
		//if (strType == _T("会员"))
		//{
		//	MyMessageBox(_T("此人不是代理，无下级"));
		//}
		//else
		//{
			m_user_id = m_listLog.GetItemData(nItem);
// 			theDBCmd.GetXjtzhLogCount(m_user_id, page_size, m_itemCount, m_maxPage);
// 			m_listLog.DeleteAllItems();
// 			theDBCmd.GetXjtzhLog(m_user_id, m_page-1, page_size, m_listLog);
		//}
	}
}

void CHuiYuanChkxjtzhDlg::OnChkxjtzhChkxx()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		int t_id = m_listLog.GetItemData(nItem);

		CTzhxxDlg dlg;
		dlg.m_id = t_id;
		dlg.DoModal();
	}
}

void CHuiYuanChkxjtzhDlg::OnNMRClickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}

	POINT pt = {0, 0};
	GetCursorPos(&pt);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_CHKXJTZH);

	CMenu* pMenu = menu.GetSubMenu(0);
	if (pMenu != NULL)
	{
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
	*pResult = 0;
}

BOOL CHuiYuanChkxjtzhDlg::OnInitDialog()
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
	m_dwTickCount=0;
	InitListCtrl();
	m_editAct.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(20,20,20));
	m_editID.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(20,20,20));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHuiYuanChkxjtzhDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuiYuanChkxjtzhDlg::OnOK()
{
	//CDialog::OnOK();
}

void CHuiYuanChkxjtzhDlg::InitListCtrl()
{
	m_listLog.ModifyStyle(NULL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(95,82,81));

	m_listLog.InsertColumn(0, _T("序列号"), LVCFMT_CENTER, 80);
	m_listLog.InsertColumn(1,_T("用户名"),LVCFMT_CENTER,130);
	m_listLog.InsertColumn(2, _T("投注彩种"), LVCFMT_CENTER,110);
	m_listLog.InsertColumn(3, _T("投注模式"), LVCFMT_CENTER, 100);
	m_listLog.InsertColumn(4, _T("投注期号"), LVCFMT_CENTER,110);
	m_listLog.InsertColumn(5, _T("投注金额"), LVCFMT_CENTER, 100);
	//m_listLog.InsertColumn(5, _T("盈亏记录"), LVCFMT_LEFT, 80);
	m_listLog.InsertColumn(6, _T("投注日期"), LVCFMT_CENTER, 140);
	m_listLog.InsertColumn(7, _T("状态"), LVCFMT_CENTER, 110);
	m_listLog.InsertColumn(8, _T("实际盈亏"), LVCFMT_CENTER, 103);
}

void CHuiYuanChkxjtzhDlg::AdjustCtrls()
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
}

void CHuiYuanChkxjtzhDlg::EnAbleCtrls(bool b)
{
	m_btnPrePage.EnableWindow(b);
	m_btnNextPage.EnableWindow(b);
}


//读取事件
bool CHuiYuanChkxjtzhDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_CHK_XJTZH_LOG_COUNT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_CHKXJTZHLogCountRet));
				if(wDataSize!=sizeof(CMD_GR_CHKXJTZHLogCountRet)) return false;

				CMD_GR_CHKXJTZHLogCountRet* pCountRet = (CMD_GR_CHKXJTZHLogCountRet*)pData;

				m_itemCount = pCountRet->dwCount;
				m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);
				m_maxPage = max(1,m_maxPage);
				//if(!m_strID.IsEmpty())
				
				//else if(!m_strAct.IsEmpty())
				//	SendToServer(2);
					break;
			}
		case SUB_GP_CHK_XJTZH_LOG_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_CHKXJTZHLogRet)==0);
				if(wDataSize % sizeof(CMD_GR_CHKXJTZHLogRet)!=0) return false;

				WORD wCount = wDataSize/sizeof(CMD_GR_CHKXJTZHLogRet);

				m_listLog.DeleteAllItems();
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_CHKXJTZHLogRet *pGetXJTZHLogRet = ((CMD_GR_CHKXJTZHLogRet*)pData+i);


					CString strTmp;
					strTmp.Format(_T("%d"), pGetXJTZHLogRet->n_t_id);
					int item = m_listLog.InsertItem(m_listLog.GetItemCount(), strTmp);
					m_listLog.SetItemData(item, pGetXJTZHLogRet->n_t_id);
					m_listLog.SetItemText(item, 1,pGetXJTZHLogRet->s_t_account);


					int t_type = pGetXJTZHLogRet->n_t_type;
					m_listLog.SetItemText(item, 2, theGameType[t_type]);

					int t_kind = pGetXJTZHLogRet->n_t_kind;
					m_listLog.SetItemText(item, 3, theGameKind[(t_type << 16) | (unsigned short)t_kind]);

					m_listLog.SetItemText(item, 4,pGetXJTZHLogRet->s_t_qishu);


					
					int Scoretype = pGetXJTZHLogRet->n_t_moshi;
					DOUBLE danzhujine = 2.0;
					if(Scoretype == MoShi_Yuan)
					{
						danzhujine = 2.0;
					}
					else if (Scoretype == MoShi_Jiao)
					{
						danzhujine = 0.2;
					}
					else if(Scoretype == MoShi_Fen)
					{
						danzhujine = 0.02;
					}
					else if(Scoretype == MoShi_Li)
						danzhujine = 0.002;

					strTmp.Format(_T("%.3lf"), pGetXJTZHLogRet->n_t_zhushu * pGetXJTZHLogRet->n_t_beishu * danzhujine);
					m_listLog.SetItemText(item, 5, strTmp);

					CTime ct(pGetXJTZHLogRet->n_t_time);
					m_listLog.SetItemText(item, 6, ct.Format(_T("%Y%m%d %H:%M:%S")));

					int t_state = pGetXJTZHLogRet->n_t_state;
					
					int t_winzhushu = pGetXJTZHLogRet->n_t_winzhushu;
					if (0 == t_state)
					{
						bool bKaijiang = false;
						CString strNextQishu;
						if(pGetXJTZHLogRet->n_t_type == CZChongQingSSC)
						{
							strNextQishu = m_chqRule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZ_TianJinSSC)
						{
							strNextQishu = m_jxRule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZXinJiangSSC)
						{
							strNextQishu = m_xjRule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZ_WUFEN_CAI)
						{
							strNextQishu = m_wfcRule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZ_FENFEN_CAI)
						{
							strNextQishu = m_ffcRule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZ_HGYDWFC)
						{
							strNextQishu = m_hgRule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZCQ11Xuan5)
						{
							strNextQishu = m_chq11x5Rule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZGD11Xuan5)
						{
							strNextQishu = m_gd11x5Rule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZJX11Xuan5)
						{
							strNextQishu = m_jx11x5Rule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZSD11Xuan5)
						{
							strNextQishu = m_sd11x5Rule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZ_PK10)
						{
							strNextQishu = m_pk10Rule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZHLJ11Xuan5)
						{
							strNextQishu = m_hlj11x5Rule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZ_QiXingCai)
						{
							strNextQishu = m_qxcRule.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZPaiLie3)
						{
							strNextQishu = m_dlgPaiLie3.GetNextExpect();
						}
						else if(pGetXJTZHLogRet->n_t_type == CZ3D)
						{
							strNextQishu = m_dlg3D.GetNextExpect();
						}

						__int64  nQishu = 0L;
						nQishu = _ttoi64(strNextQishu);
						nQishu-=1;
						CString strNowQishu;

						strNowQishu.Format(L"%I64d",nQishu);
						CString strQihao;
						strQihao.Format(L"%s", pGetXJTZHLogRet->s_t_qishu);

						if(strQihao<strNowQishu)
						{
							bKaijiang = true;
						}

						if(bKaijiang)
							strTmp = _T("未派奖");
						else
						{
							if(pGetXJTZHLogRet->n_t_zhuihao>=2)
								strTmp = _T("追号");
							else
								strTmp = _T("未开奖");
						}
					}
					else if (1 == t_state)
					{
						strTmp = t_winzhushu==0? _T("未中奖"): _T("中奖");
					}
					else if (2 == t_state)
					{
						if(pGetXJTZHLogRet->n_t_zhuihao>2)
							strTmp = _T("追号撤单");
						else
							strTmp = _T("撤单");
					}
					m_listLog.SetItemText(item, 7, strTmp);

					float fYingkui = pGetXJTZHLogRet->f_t_yingkui;
					if (2 == t_state)
						fYingkui = 0.000;
					strTmp.Format(L"%.3f", fYingkui);
					m_listLog.SetItemText(item, 8, strTmp);


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

VOID CHuiYuanChkxjtzhDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //下注记录数量
	{
		m_bGetXjtzhLogCount = true;
		if(m_bGetXjtzhLogCount)
		{
			m_bGetXjtzhLogCount = false;
			CMD_GP_CHKXJTZHLogCount GetXJTZHLogCount;
			ZeroMemory(&GetXJTZHLogCount,sizeof(GetXJTZHLogCount));

			GetXJTZHLogCount.dwUserID = theAccount.user_id;

			GetXJTZHLogCount.n_t_type = m_byType;

			int xj_id = _ttoi(m_strID);
			GetXJTZHLogCount.n_t_user_id = xj_id;
			lstrcpyn(GetXJTZHLogCount.s_t_account,m_strAct.GetBuffer(),sizeof(GetXJTZHLogCount.s_t_account));
			GetXJTZHLogCount.bTime = m_byTime;

// 			if(m_tStart == m_tEnd)
// 			{
// 				m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}
// 			m_tEnd += CTimeSpan(0, 23, 59, 59);

			lstrcpyn(GetXJTZHLogCount.szTimeStart,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetXJTZHLogCount.szTimeStart));
			lstrcpyn(GetXJTZHLogCount.szTimeEnd,m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetXJTZHLogCount.szTimeEnd));


			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_CHK_XJTZH_LOG_COUNT,&GetXJTZHLogCount,sizeof(GetXJTZHLogCount));
			return;
		}

	}

	if(nSendType == 2)
	{
		m_bGetXjtzhLog = true;
		if(m_bGetXjtzhLog)
		{
			m_bGetXjtzhLog=false;
			CMD_GP_CHKXJTZHLog GetXJTZHLog;
			ZeroMemory(&GetXJTZHLog,sizeof(GetXJTZHLog));

			GetXJTZHLog.dwUserID = theAccount.user_id;

			GetXJTZHLog.n_t_type = m_byType;

			int xj_id = _ttoi(m_strID);
			GetXJTZHLog.n_t_user_id = xj_id;
			lstrcpyn(GetXJTZHLog.s_t_account,m_strAct.GetBuffer(),sizeof(GetXJTZHLog.s_t_account));
			GetXJTZHLog.bByTime = m_byTime;
			GetXJTZHLog.nPage = m_page;
			GetXJTZHLog.nSize = page_size;
// 			if(m_tStart == m_tEnd)
// 			{
// 				m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}

			lstrcpyn(GetXJTZHLog.szTimeStart,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetXJTZHLog.szTimeStart));
			lstrcpyn(GetXJTZHLog.szTimeEnd,m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetXJTZHLog.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_CHK_XJTZH_LOG,&GetXJTZHLog,sizeof(GetXJTZHLog));
			return;
		}

	}


}

void CHuiYuanChkxjtzhDlg::QueryChkXjTzh(int n_t_id)
{
	m_strID.Format(L"%d",n_t_id);
	m_editID.SetWindowText(m_strID);
	m_byType=1;
	SendToServer(1);
		SendToServer(2);

}