#include "stdafx.h"
#include "GamePlaza.h"
#include "TzhLogDlg.h"
#include "TzhxxDlg.h"
#include "GameRule.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"

//起始时间
static const int tip_time_start_x = 140;
static const int tip_time_start_y = 43;
static const int tip_time_start_width = 70;
static const int tip_time_start_height = 20;
static CRect rc_time_start(tip_time_start_x, 
						   tip_time_start_y, 
						   tip_time_start_x + tip_time_start_width, 
						   tip_time_start_y + tip_time_start_height);

static const int time_start_x = 210;
static const int time_start_y = 43;
static const int time_start_width = 100;
static const int time_start_height = 20;

//截止时间
static const int tip_time_end_x = 310;
static const int tip_time_end_y = 43;
static const int tip_time_end_width = 20;
static const int tip_time_end_height = 20;
static CRect rc_time_end(tip_time_end_x, 
						 tip_time_end_y, 
						 tip_time_end_x + tip_time_end_width, 
						 tip_time_end_y + tip_time_end_height);

static const int time_end_x = 330;
static const int time_end_y = 43;
static const int time_end_width = 100;
static const int time_end_height = 20;


//期号
static const int tip_edit_qihao_x = 480;
static const int tip_edit_qihao_y = 36;
static const int tip_edit_qihao_width = 70;
static const int tip_edit_qihao_height = 20;
static CRect rc_edit_qihao(tip_edit_qihao_x, 
						   tip_edit_qihao_y, 
						   tip_edit_qihao_x + tip_edit_qihao_width, 
						   tip_edit_qihao_y + tip_edit_qihao_height);

static const int edit_qihao_x = 550;
static const int edit_qihao_y = 36;
static const int edit_qihao_width = 60;
static const int edit_qihao_height = 20;

//确定按钮
static const int btn_ok_x = 640;
static const int btn_ok_y = 42;

//列表
static const int list_log_x = 31;
static const int list_log_y = 69;
static const int list_log_width = 966;
static const int list_log_height = 485;

//总页数
static const int tip_page_x = 140;
static const int tip_page_y = 555;
static const int tip_page_width = 400;
static const int tip_page_height = 20;
static CRect rc_tip_page(tip_page_x, 
						 tip_page_y, 
						 tip_page_x + tip_page_width, 
						 tip_page_y + tip_page_height);

//上一页
static const int btn_pre_page_x = 720;
static const int btn_pre_page_y = 555;

//下一页
static const int btn_next_page_x = 800;
static const int btn_next_page_y = 555;

static const int page_size = 19;

IMPLEMENT_DYNAMIC(CTzhLogDlg, CDialog)

CTzhLogDlg::CTzhLogDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTzhLogDlg::IDD, pParent)
, m_bmpBk(NULL)
, m_tStart(CTime::GetCurrentTime())
, m_tEnd(CTime::GetCurrentTime())
, m_qihao(0)
, m_page(1)
, m_itemCount(0)
, m_maxPage(1)
, m_byTime(TRUE)
{
	/*	m_tStart = CTime::GetCurrentTime();
	CString starttime ;

	starttime.Format(L"%d-%02d-%02d",m_tStart.GetYear(),m_tStart.GetMonth(),1);


	COleDateTime tm;
	tm.ParseDateTime(starttime);
	SYSTEMTIME st;
	tm.GetAsSystemTime(st);
	CTime t(st);
	m_tStart = t;*/

	//任务组件

	m_bGetTouzhuLog = false;
	m_bGetTouzhuLogCount = false;
	m_bCancelTouzhu = false;
	m_nCancelID = 0;
	m_nCancelKind = 0;
	m_strQihao.Empty();
}

CTzhLogDlg::~CTzhLogDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CTzhLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_START, m_dateStart);
	DDX_Control(pDX, IDC_DATE_END, m_dateEnd);
	DDX_Control(pDX, IDC_EDIT_QIHAO, m_editQihao);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_PRE_PAGE, m_btnPrePage);
	DDX_Control(pDX, IDC_BTN_NEXT_PAGE, m_btnNextPage);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_tStart);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_tEnd);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Text(pDX, IDC_EDIT_QIHAO, m_qihao);
}


BEGIN_MESSAGE_MAP(CTzhLogDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CTzhLogDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CTzhLogDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CTzhLogDlg::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOG, &CTzhLogDlg::OnNMDblclkListLog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LOG, &CTzhLogDlg::OnNMRclickListLog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LOG, &CTzhLogDlg::OnNMLclickListLog)
	ON_COMMAND(ID_TOUZHU_LOG_CHAKANXX, &CTzhLogDlg::OnTouzhuLogChakanxx)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CTzhLogDlg::OnBnClickedBtnClose)
	ON_COMMAND(ID_TOUZHU_LOG_CHEXIAO, &CTzhLogDlg::OnTouzhuLogChexiao)
END_MESSAGE_MAP()


// CTzhLogDlg 消息处理程序

void CTzhLogDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0,  rect.Width(), rect.Height()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));

	cacheDC.DrawText(_T("起始时间"), rc_time_start, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(_T("至"), rc_time_end, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//cacheDC.DrawText(_T("投注期号"), rc_edit_qihao, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

	CString strTmp;
	strTmp.Format(_T("第%d页/共%d页，每页%d条记录，共%d条记录"), 
		m_page, m_maxPage, page_size, m_itemCount);
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

BOOL CTzhLogDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
// CZhanghuDlg 消息处理程序
void CTzhLogDlg::OnBnClickedBtnClose()
{
	CWnd* pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->PostMessage(IDM_RETURN_GAME,1,0);
	}

	return;
}

void CTzhLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}
	if(m_btnClose.GetSafeHwnd() != NULL)
	{
		m_btnClose.SetWindowPos(NULL, cx - m_btnClose.Width() - 8, -1, m_btnClose.Width(), m_btnClose.Height(), SWP_NOZORDER);
	}

	AdjustCtrls();
}

void CTzhLogDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_page = 1;

		SendToServer(1); //查询日志数量
		SendToServer(2);
	}
	else
	{
		m_listLog.DeleteAllItems();
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_dateStart.SetTime(&m_tStart);
		m_dateEnd.SetTime(&m_tEnd);
		m_page = 1;

	}
}

void CTzhLogDlg::OnBnClickedBtnOk()
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

	if (m_tStart > m_tEnd)
	{
		MyMessageBox(_T("选择的时间区间不对"));
		return;
	}

	m_byTime = TRUE;
	m_listLog.DeleteAllItems();
//	m_tEnd += CTimeSpan(1L, 0, 0, 0);
	m_page = 1;
	SendToServer(1);
	SendToServer(2);
	InvalidateRect(rc_tip_page);
}

void CTzhLogDlg::OnBnClickedBtnPrePage()
{
	if(m_page == 1)
	{
		MyMessageBox(_T("已经是第一页了"));
		return;
	}

	//数据库暂时屏蔽 Alter by a'long

	m_page--;
	m_listLog.DeleteAllItems();
	SendToServer(2);
	InvalidateRect(rc_tip_page);
}

void CTzhLogDlg::OnBnClickedBtnNextPage()
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

void CTzhLogDlg::OnNMDblclkListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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

void CTzhLogDlg::OnNMRclickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}

	int nItem = m_listLog.GetNextSelectedItem(pos);
	CString sState = m_listLog.GetItemText(nItem, 6);

	POINT pt = {0, 0};
	GetCursorPos(&pt);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_TOUZHU_LOG);

	CMenu* pMenu = menu.GetSubMenu(0);
	if (pMenu != NULL)
	{
		if ((sState != _T("未开奖"))&&(sState != _T("追号")))
		{
			pMenu->EnableMenuItem(ID_TOUZHU_LOG_CHEXIAO, MF_DISABLED|MF_GRAYED);
		}
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
	*pResult = 0;
}
void CTzhLogDlg::OnNMLclickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}

	int nItem = m_listLog.GetNextSelectedItem(pos);
	CString sState = m_listLog.GetItemText(nItem, 6);

	POINT pt = {0, 0};
	GetCursorPos(&pt);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_TOUZHU_LOG);

	CMenu* pMenu = menu.GetSubMenu(0);
	if (pMenu != NULL)
	{
		if ((sState != _T("未开奖"))&&(sState != _T("追号")))
		{
			pMenu->EnableMenuItem(ID_TOUZHU_LOG_CHEXIAO, MF_DISABLED|MF_GRAYED);
		}
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
	*pResult = 0;
}

void CTzhLogDlg::OnTouzhuLogChakanxx()
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

void CTzhLogDlg::OnTouzhuLogChexiao()
{
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = m_listLog.GetNextSelectedItem(pos);
		CString sGameType = m_listLog.GetItemText(nItem, 1);
		CString sQihao = m_listLog.GetItemText(nItem, 3);
		CString sState = m_listLog.GetItemText(nItem,6);
		int nState = _ttoi(sState);
		if(nState > 0 )
		{
			MyMessageBox(_T("亲，开奖前一分钟不支持撤单哦！"));
			return;

		}
		int nGameKind = 0;

		bool bCheDan = true;
		if (sGameType == _T("重庆时时彩"))
		{
			CChqSSCRule rule;
			bCheDan = rule.IsCanCancel(sQihao);
			nGameKind = CZChongQingSSC;
		}
		else if (sGameType == _T("天津时时彩"))
		{
			CJxSSCRule rule;
			bCheDan = rule.IsCanCancel(sQihao);
			nGameKind = CZ_TianJinSSC;
		}
		else if (sGameType == _T("新疆时时彩"))
		{
			CXJSSCRule rule;
			bCheDan = rule.IsCanCancel(sQihao);
			nGameKind = CZXinJiangSSC;
		}
		else if (sGameType == _T("吉利分分彩"))
		{
			CFenFenCaiRule rule;
			bCheDan = rule.IsCanCancel(sQihao);
			nGameKind = CZ_FENFEN_CAI;
		}
		else if (sGameType == _T("吉利五分彩"))
		{
			CWuFenCaiRule rule;
			bCheDan = rule.IsCanCancel(sQihao);
			nGameKind = CZ_WUFEN_CAI;
		}
		else if (sGameType == _T("韩国1.5分彩"))
		{
			CHgydwfcRule rule;
			bCheDan = rule.IsCanCancel(sQihao);
			nGameKind = CZ_HGYDWFC;
		}
		else if (sGameType == _T("广东11选5"))
		{
			CGD11X5Rule rule;
			bCheDan = rule.IsCanCancel(sQihao);
			nGameKind = CZGD11Xuan5;
		}
		else if (sGameType == _T("山东11选5"))
		{
			CSD11X5Rule rule;
			bCheDan = rule.IsCanCancel(sQihao);
			nGameKind = CZSD11Xuan5;

		}
		else if (sGameType == _T("江西11选5"))
		{
			CJX11X5Rule rule;
			bCheDan = rule.IsCanCancel(sQihao);
			nGameKind = CZJX11Xuan5;

		}
		else if (sGameType == _T("北京PK10"))
		{
			CBJPK10Rule rule;
			bCheDan = rule.IsCanCancel(sQihao);
			nGameKind = CZ_PK10;
		}
		else if(sGameType == _T("3D彩"))
		{
			nGameKind = CZ3D;
		}
		else if(sGameType == _T("排列三"))
		{
			nGameKind = CZPaiLie3;
		}
		else if(sGameType == _T("七星彩"))
		{
			nGameKind = CZ_QiXingCai;
		}
		else if(sGameType == _T("北京快8"))
		{
			nGameKind = CZKUAILE8;
		}
		else if(sGameType == _T("幸运28"))
		{
			nGameKind = CZXingYun28;
		}

		if (!bCheDan)
		{
			MyMessageBox(_T("亲，开奖前一分钟不支持撤单哦！"));
			return;
		}

		int t_id = m_listLog.GetItemData(nItem);
		m_nCancelID=t_id;
		m_nCancelKind = nGameKind;
		m_strQihao = sQihao;
		SendToServer(3);
	}
}

BOOL CTzhLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg1.png"));
	m_btnClose.SetImage(CBmpUtil::GetExePath() + _T("skin\\return_bt.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt.png"));
	m_btnPrePage.SetImage(CBmpUtil::GetExePath() + _T("skin\\syy_bt.png"));
	m_btnNextPage.SetImage(CBmpUtil::GetExePath() + _T("skin\\xyy_bt.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	InitListCtrl();
	m_dwTickCount = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTzhLogDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CTzhLogDlg::OnOK()
{
	//CDialog::OnOK();
}

void CTzhLogDlg::InitListCtrl()
{
	m_listLog.ModifyStyle(WS_HSCROLL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_UNDERLINEHOT);
	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(95,82,81));
	//m_listLog.SetHeaderTextColor(RGB(210,210,210));

	m_listLog.InsertColumn(0, _T("订单ID"), LVCFMT_CENTER, 90);
	m_listLog.InsertColumn(1, _T("投注彩种"), LVCFMT_CENTER, 130);
	m_listLog.InsertColumn(2, _T("投注模式"), LVCFMT_CENTER, 132);
	m_listLog.InsertColumn(3, _T("投注期号"), LVCFMT_CENTER, 120);
	m_listLog.InsertColumn(4, _T("投注金额"), LVCFMT_CENTER, 120);
	//m_listLog.InsertColumn(5, _T("盈亏记录"), LVCFMT_LEFT, 80);
	m_listLog.InsertColumn(5, _T("投注日期"), LVCFMT_CENTER, 170);
	m_listLog.InsertColumn(6, _T("状态"), LVCFMT_CENTER, 80);
	m_listLog.InsertColumn(7, _T("实际盈亏"), LVCFMT_CENTER, 120);
}

void CTzhLogDlg::AdjustCtrls()
{
	if (m_dateStart.GetSafeHwnd() != NULL)
	{
		m_dateStart.SetWindowPos(NULL, time_start_x, time_start_y, time_start_width, time_start_height, SWP_NOZORDER);
	}
	if (m_dateEnd.GetSafeHwnd() != NULL)
	{
		m_dateEnd.SetWindowPos(NULL, time_end_x, time_end_y, time_end_width, time_end_height, SWP_NOZORDER);
	}
	if (m_editQihao.GetSafeHwnd() != NULL)
	{
		m_editQihao.SetWindowPos(NULL, edit_qihao_x, edit_qihao_y, edit_qihao_width, edit_qihao_height, SWP_NOZORDER);
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
}

//读取事件
bool CTzhLogDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_TOUZHU_LOG_COUNT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetTouzhuLogCountRet));
				if(wDataSize!=sizeof(CMD_GR_GetTouzhuLogCountRet))
					return false;

				CMD_GR_GetTouzhuLogCountRet* pCountRet = (CMD_GR_GetTouzhuLogCountRet*)pData;

				m_itemCount = pCountRet->dwCount;
				m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);
				m_maxPage = max(1,m_maxPage);

				break;
			}
		case SUB_GP_GET_TOUZHU_LOG_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_GetTouzhuLogRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetTouzhuLogRet)!=0)
					return false;

				WORD wCount = wDataSize/sizeof(CMD_GR_GetTouzhuLogRet);
				m_listLog.DeleteAllItems();
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetTouzhuLogRet *pTouzhuLogRet = ((CMD_GR_GetTouzhuLogRet*)pData+i);

					CString strTmp;
					strTmp.Format(_T("%d"), pTouzhuLogRet->n_t_id);
					int item = m_listLog.InsertItem(m_listLog.GetItemCount(), strTmp);
					m_listLog.SetItemData(item, pTouzhuLogRet->n_t_id);

					m_listLog.SetItemText(item, 1, theGameType[pTouzhuLogRet->n_t_type]);
					int t_kind = pTouzhuLogRet->n_t_kind;
					m_listLog.SetItemText(item, 2, theGameKind[(pTouzhuLogRet->n_t_type << 16) | (unsigned short)t_kind]);

					m_listLog.SetItemText(item, 3, pTouzhuLogRet->szQishu);
					double danzhujine = 2.0;
					if(pTouzhuLogRet->n_t_moshi == MoShi_Yuan)
						danzhujine=2.0;
					else if(pTouzhuLogRet->n_t_moshi ==  MoShi_Jiao)
						danzhujine=0.2;
					else if(pTouzhuLogRet->n_t_moshi == MoShi_Fen)
						danzhujine=0.02;
					else if(pTouzhuLogRet->n_t_moshi == MoShi_Li)
						danzhujine=0.002;

					if(pTouzhuLogRet->n_t_type == CZ_LIUHECAI)
						strTmp.Format(_T("%.3lf"), pTouzhuLogRet->n_t_zhushu * pTouzhuLogRet->n_t_beishu*1.000 );
					else
						strTmp.Format(_T("%.3lf"), pTouzhuLogRet->n_t_zhushu * pTouzhuLogRet->n_t_beishu * danzhujine);
					m_listLog.SetItemText(item, 4, strTmp);

					CTime ct(pTouzhuLogRet->n_t_time);
					m_listLog.SetItemText(item, 5, ct.Format(_T("%Y%m%d %H:%M:%S")));


					int t_state = pTouzhuLogRet->n_t_state;

					int t_winzhushu = pTouzhuLogRet->n_t_winzhushu;
					if (0 == t_state)
					{

						bool bKaijiang = false;
						CString strNextQishu;
						if(pTouzhuLogRet->n_t_type == CZChongQingSSC)
						{
							strNextQishu = m_chqRule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZ_TianJinSSC)
						{
							strNextQishu = m_jxRule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZXinJiangSSC)
						{
							strNextQishu = m_xjRule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZ_WUFEN_CAI)
						{
							strNextQishu = m_wfcRule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZ_FENFEN_CAI)
						{
							strNextQishu = m_ffcRule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZ_HGYDWFC)
						{
							strNextQishu = m_hgRule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZCQ11Xuan5)
						{
							strNextQishu = m_chq11x5Rule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZGD11Xuan5)
						{
							strNextQishu = m_gd11x5Rule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZJX11Xuan5)
						{
							strNextQishu = m_jx11x5Rule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZSD11Xuan5)
						{
							strNextQishu = m_sd11x5Rule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZ_PK10)
						{
							strNextQishu = m_pk10Rule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZHLJ11Xuan5)
						{
							strNextQishu = m_hlj11x5Rule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZ_QiXingCai)
						{
							strNextQishu = m_qxcRule.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZPaiLie3)
						{
							strNextQishu = m_dlgPaiLie3.GetNextExpect();
						}
						else if(pTouzhuLogRet->n_t_type == CZ3D)
						{
							strNextQishu = m_dlg3D.GetNextExpect();
						}

						__int64  nQishu = 0L;
						nQishu = _ttoi64(strNextQishu);
						nQishu-=1;
						CString strNowQishu;

						strNowQishu.Format(L"%I64d",nQishu);
						CString strQihao;
						strQihao.Format(L"%s", pTouzhuLogRet->szQishu);

						if(strQihao<strNowQishu)
						{
							bKaijiang = true;
						}

						if(bKaijiang)
							strTmp = _T("未派奖");
						else
						{
							if(pTouzhuLogRet->n_t_zhuihao>=2)
								strTmp = _T("追号");
							else
								strTmp = _T("未开奖");
						}
					}
					else if (1 == t_state)
					{
						TCHAR szWinzhu[33]=TEXT("");
						_sntprintf(szWinzhu,CountArray(szWinzhu),TEXT("中奖%d注"),t_winzhushu);
						strTmp = t_winzhushu==0? _T("未中奖"): szWinzhu;
					}
					else if (2 == t_state)
					{
						if(pTouzhuLogRet->n_t_zhuihao>2)
							strTmp = _T("追号撤单");
						else
							strTmp = _T("撤单");
					}
					m_listLog.SetItemText(item, 6, strTmp);

					if (2 == t_state || 0 == t_state)
						strTmp.Format(L"%.3f",0.000);
					else
						strTmp.Format(L"%.3f", pTouzhuLogRet->f_t_yingkui);
					m_listLog.SetItemText(item, 7, strTmp);

				}
				break;
				//return true;
			}
		case SUB_GR_CANCEL_TOUZHU_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_CancelTouzhuRet));
				if(wDataSize!=sizeof(CMD_GR_CancelTouzhuRet))
					return false;

				CMD_GR_CancelTouzhuRet* pCancelTouzhu = (CMD_GR_CancelTouzhuRet*)pData;

				int nResult = pCancelTouzhu->nResult;

				if (!nResult)
				{
					CString msg;
					msg.Format(_T("您取消了订单[%d]"), m_nCancelID);
					MyMessageBox(msg);
					SendToServer(2);
				}
				else
				{
					CString strLog;
					strLog.Format(_T("亲，开奖前一分钟不支持撤单哦！"));
					MyMessageBox(strLog);
				}
				break;
				//return true;
			}
		}
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CTzhLogDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //下注记录数量
	{
		m_bGetTouzhuLogCount = true;
		if(m_bGetTouzhuLogCount)
		{
			m_bGetTouzhuLogCount = false;
			CMD_GP_GetTouzhuLogCount GetTouzhuLogCount;
			ZeroMemory(&GetTouzhuLogCount,sizeof(GetTouzhuLogCount));

			GetTouzhuLogCount.dwUserID = theAccount.user_id;
			GetTouzhuLogCount.bTime = m_byTime;
// 			CTime tEnd;
// 			tEnd = m_tEnd;
// 			if(m_tStart == m_tEnd)
// 			{
// 				tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}

			CopyMemory(GetTouzhuLogCount.szTimeStart , m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetTouzhuLogCount.szTimeStart));
			CopyMemory(GetTouzhuLogCount.szTimeEnd , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetTouzhuLogCount.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_TOUZHU_LOG_COUNT,&GetTouzhuLogCount,sizeof(GetTouzhuLogCount));
			return;
		}


	}

	if(nSendType == 2)//下注记录 
	{
		m_bGetTouzhuLog = true;
		if(m_bGetTouzhuLog)
		{
			m_bGetTouzhuLog = false;

			CMD_GP_GetTouzhuLog TouzhuLog;
			ZeroMemory(&TouzhuLog,sizeof(TouzhuLog));

			TouzhuLog.dwUserID = theAccount.user_id;
			TouzhuLog.nPage = m_page;
			TouzhuLog.nSize = page_size;
			TouzhuLog.bByTime = m_byTime;
			CopyMemory(TouzhuLog.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(TouzhuLog.szTimeStart));
			CopyMemory(TouzhuLog.szTimeEnd  , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(TouzhuLog.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_TOUZHU_LOG,&TouzhuLog,sizeof(TouzhuLog));
			return;
		}
	}
	if(nSendType == 3)
	{
		m_bCancelTouzhu = true;

		if(m_bCancelTouzhu)
		{
			m_bCancelTouzhu=false;
			CMD_GP_CancelTouzhu CancelTouzhu;
			ZeroMemory(&CancelTouzhu,sizeof(CancelTouzhu));
			CancelTouzhu.n_t_id = m_nCancelID;
			CancelTouzhu.n_t_kindid = m_nCancelKind;
			CancelTouzhu.n_t_userid = theAccount.user_id;
	//		_sntprintf(CancelTouzhu.s_t_qihao ,CountArray(CancelTouzhu.s_t_qihao),TEXT("%s"), m_strQihao);
			CopyMemory(CancelTouzhu.s_t_qihao  ,m_strQihao,sizeof(CancelTouzhu.s_t_qihao));
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_CANCEL_TOUZHU,&CancelTouzhu,sizeof(CancelTouzhu));
			return;


		}
	}

}