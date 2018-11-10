#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuanTxLogDlg.h"
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
static const int tip_edit_act_y = 10;
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
static const int tip_edit_id_y = 10;
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

IMPLEMENT_DYNAMIC(CHuiYuanTxLogDlg, CDialog)

CHuiYuanTxLogDlg::CHuiYuanTxLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuiYuanTxLogDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_tStart(CTime::GetCurrentTime())
	, m_tEnd(CTime::GetCurrentTime())
	, m_page(1)
	, m_itemCount(0)
	, m_maxPage(1)
	, m_byTime(TRUE)
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
	m_bGetXjTxLogCount=false;
	m_bGetXjTxLog=false;
	m_bGetXjTxLogByID=false;
	m_bGetXjTxLogByAct=false;

}

CHuiYuanTxLogDlg::~CHuiYuanTxLogDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CHuiYuanTxLogDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CHuiYuanTxLogDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CHuiYuanTxLogDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CHuiYuanTxLogDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CHuiYuanTxLogDlg::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_XJTXLOG_CHKXJ, &CHuiYuanTxLogDlg::OnXjtxlogChkxj)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LOG, &CHuiYuanTxLogDlg::OnNMRClickListLog)
END_MESSAGE_MAP()


// CHuiYuanTxLogDlg 消息处理程序

void CHuiYuanTxLogDlg::OnPaint()
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
	if ((!m_strID.IsEmpty()) || (!m_strAct.IsEmpty()))
	{
		strTmp.Format(_T("共查找到%d条记录"), m_listLog.GetItemCount());
	}
	else
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

BOOL CHuiYuanTxLogDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanTxLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

void CHuiYuanTxLogDlg::OnShowWindow(BOOL bShow, UINT nStatus)
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
		SendToServer(1);
		SendToServer(2);

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

void CHuiYuanTxLogDlg::OnBnClickedBtnOk()
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

void CHuiYuanTxLogDlg::OnBnClickedBtnPrePage()
{
	if(m_page == 1)
	{
		MyMessageBox(_T("已经是第一页了"));
		return;
	}

	m_page--;
	SendToServer(2);
	m_listLog.DeleteAllItems();
	InvalidateRect(rc_tip_page);
}

void CHuiYuanTxLogDlg::OnBnClickedBtnNextPage()
{
	if(m_page == m_maxPage)
	{
		MyMessageBox(_T("已经是最后一页了"));
		return;
	}

	m_page++;
	SendToServer(2);
	m_listLog.DeleteAllItems();
	InvalidateRect(rc_tip_page);
}

void CHuiYuanTxLogDlg::OnXjtxlogChkxj()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		m_user_id = m_listLog.GetItemData(nItem);
	}
}

void CHuiYuanTxLogDlg::OnNMRClickListLog(NMHDR *pNMHDR, LRESULT *pResult)
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
	menu.LoadMenu(IDR_MENU_XJYKTJ);

	CMenu* pMenu = menu.GetSubMenu(0);
	if (pMenu != NULL)
	{
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
	*pResult = 0;
}

BOOL CHuiYuanTxLogDlg::OnInitDialog()
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

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHuiYuanTxLogDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuiYuanTxLogDlg::OnOK()
{
	//CDialog::OnOK();
}

void CHuiYuanTxLogDlg::InitListCtrl()
{
	m_listLog.ModifyStyle(NULL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(95,82,81));

	m_listLog.InsertColumn(0, _T("流水号"), LVCFMT_CENTER, 80);
	m_listLog.InsertColumn(1, _T("游戏ID"), LVCFMT_CENTER, 80);
	m_listLog.InsertColumn(2, _T("游戏账号"), LVCFMT_CENTER, 90);
	m_listLog.InsertColumn(3, _T("提现时间"), LVCFMT_CENTER, 150);
	m_listLog.InsertColumn(4, _T("收款开户行"), LVCFMT_CENTER, 163);
	m_listLog.InsertColumn(5, _T("提现金额"), LVCFMT_CENTER, 80);
	m_listLog.InsertColumn(6, _T("状态"), LVCFMT_CENTER, 90);
	m_listLog.InsertColumn(7, _T("提现状态"), LVCFMT_CENTER, 90);
	m_listLog.InsertColumn(8, _T("说明"), LVCFMT_CENTER, 160);
}

void CHuiYuanTxLogDlg::AdjustCtrls()
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

void CHuiYuanTxLogDlg::EnAbleCtrls(bool b)
{
	m_btnPrePage.EnableWindow(b);
	m_btnNextPage.EnableWindow(b);
}

//读取事件
bool CHuiYuanTxLogDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_XJTX_LOG_COUNT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetXjTxLogCountRet));
				if(wDataSize!=sizeof(CMD_GR_GetXjTxLogCountRet)) return false;

				CMD_GR_GetXjTxLogCountRet* pCountRet = (CMD_GR_GetXjTxLogCountRet*)pData;

				m_itemCount = pCountRet->dwCount;
				m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);
				m_maxPage = max(1,m_maxPage);

				break;
			}
		case SUB_GP_GET_XJTX_LOG_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_GetXJTxLogRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetXJTxLogRet)!=0) return false;
				m_listLog.DeleteAllItems();

				WORD wCount = wDataSize/sizeof(CMD_GR_GetXJTxLogRet);
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetXJTxLogRet *pXJYKLogRet = ((CMD_GR_GetXJTxLogRet*)pData+i);

					CString strTmp;
					strTmp.Format(_T("%d"), pXJYKLogRet->n_t_id);
					int item = m_listLog.InsertItem(m_listLog.GetItemCount(), strTmp);

					int t_user_id = pXJYKLogRet->n_t_user_id;
					strTmp.Format(_T("%d"), t_user_id);
					m_listLog.SetItemText(item, 1, strTmp);
					m_listLog.SetItemData(item, t_user_id);
					strTmp.Format(_T("%s"), pXJYKLogRet->s_t_account);
					strTmp.Trim();
					m_listLog.SetItemText(item, 2,strTmp );

					CTime ct(pXJYKLogRet->n_t_time);
					m_listLog.SetItemText(item, 3, ct.Format(_T("%Y%m%d %H:%M:%S")));
					strTmp.Format(_T("%s"), pXJYKLogRet->s_t_yinhang);
					strTmp.Trim();

					m_listLog.SetItemText(item, 4, strTmp);

					DOUBLE t_jine = pXJYKLogRet->f_t_jine;
					strTmp.Format(_T("%.2lf"), t_jine);
					m_listLog.SetItemText(item, 5, strTmp);

					strTmp.Format(_T("%s"), pXJYKLogRet->s_t_kaihuiren);
					strTmp.Trim();

					strTmp.Format(L"%s",strTmp);


					m_listLog.SetItemText(item,7, strTmp);

					int t_state = pXJYKLogRet->n_t_state;
					switch(t_state)
					{
					case 0: strTmp=_T("未处理"); break;
					case 1: strTmp=_T("已审核-通过"); break;
					case 2: strTmp=_T("已处理-拒绝"); break;
					case 3: strTmp=_T("已处理-完成"); break;
					default: strTmp=_T("未处理"); break;
					}
					m_listLog.SetItemText(item, 6, strTmp);


					m_listLog.SetItemText(item, 8, pXJYKLogRet->s_t_note);

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

VOID CHuiYuanTxLogDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //下注记录数量
	{
		m_bGetXjTxLogCount = true;
		if(m_bGetXjTxLogCount)
		{
			m_bGetXjTxLogCount = false;
			CMD_GP_GetXjTxLogCount GetXjTxLogCount;
			ZeroMemory(&GetXjTxLogCount,sizeof(GetXjTxLogCount));

			GetXjTxLogCount.dwUserID = theAccount.user_id;
			GetXjTxLogCount.n_t_type = m_byType;

			int xj_id = _ttoi(m_strID);
			GetXjTxLogCount.n_t_user_id = xj_id;
			lstrcpyn(GetXjTxLogCount.s_t_account,m_strAct.GetBuffer(),sizeof(GetXjTxLogCount.s_t_account));
			GetXjTxLogCount.bTime = m_byTime;
// 			if(m_tStart == m_tEnd)
// 			{
// 				m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}
// 			m_tEnd += CTimeSpan(0, 23, 59, 59);

			CopyMemory(GetXjTxLogCount.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetXjTxLogCount.szTimeStart));
			CopyMemory(GetXjTxLogCount.szTimeEnd , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetXjTxLogCount.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJTX_LOG_COUNT,&GetXjTxLogCount,sizeof(GetXjTxLogCount));
			return;
		}


	}

	if(nSendType == 2)
	{
		m_bGetXjTxLog = true;
		if(m_bGetXjTxLog)
		{
			m_bGetXjTxLog = false;
			CMD_GP_GetXJTxLog GetXJTxLog;
			ZeroMemory(&GetXJTxLog,sizeof(GetXJTxLog));

			GetXJTxLog.dwUserID = theAccount.user_id;
			GetXJTxLog.n_t_type = m_byType;

			int xj_id = _ttoi(m_strID);
			GetXJTxLog.n_t_user_id = xj_id;
			lstrcpyn(GetXJTxLog.s_t_account,m_strAct.GetBuffer(),sizeof(GetXJTxLog.s_t_account));
			GetXJTxLog.nPage = m_page;
			GetXJTxLog.nSize = page_size;
			GetXJTxLog.bByTime = m_byTime;

			CopyMemory(GetXJTxLog.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetXJTxLog.szTimeStart));
			CopyMemory(GetXJTxLog.szTimeEnd  , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetXJTxLog.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJTX_LOG,&GetXJTxLog,sizeof(GetXJTxLog));
			return;

		}


	}

	if(nSendType == 3)
	{
		m_bGetXjTxLogByID = true;
		if(m_bGetXjTxLogByID)
		{
			m_bGetXjTxLogByID = false;
			CMD_GP_GetXJTxLogByID GetXJTxLog;
			ZeroMemory(&GetXJTxLog,sizeof(GetXJTxLog));

			GetXJTxLog.dwUserID = theAccount.user_id;
			int xj_id = _ttoi(m_strID);
			GetXJTxLog.nXiaJiID = xj_id;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJTX_LOG_BY_ID,&GetXJTxLog,sizeof(GetXJTxLog));
			return;

		}

	}

	if(nSendType == 4)
	{
		m_bGetXjTxLogByAct = true;
		if(m_bGetXjTxLogByAct)
		{
			m_bGetXjTxLogByAct=false;
			CMD_GP_GetXJTxLogByAct Tj;
			ZeroMemory(&Tj,sizeof(Tj));

			Tj.dwUserID = theAccount.user_id;
			lstrcpyn(Tj.szAccount,m_strAct.GetBuffer(),sizeof(Tj.szAccount));
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_XJTX_LOG_BY_ACT,&Tj,sizeof(Tj));
			return;
		}
	}


}