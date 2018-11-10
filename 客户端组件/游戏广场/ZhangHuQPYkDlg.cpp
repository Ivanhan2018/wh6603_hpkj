#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhangHuQPYkDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
//起始时间
static const int tip_time_start_x = 20;
static const int tip_time_start_y = 5;
static const int tip_time_start_width = 70;
static const int tip_time_start_height = 20;
static CRect rc_time_start(tip_time_start_x, 
						   tip_time_start_y, 
						   tip_time_start_x + tip_time_start_width, 
						   tip_time_start_y + tip_time_start_height);

static const int time_start_x = 90;
static const int time_start_y = 5;
static const int time_start_width = 100;
static const int time_start_height = 20;

//截止时间
static const int tip_time_end_x = 190;
static const int tip_time_end_y = 5;
static const int tip_time_end_width = 20;
static const int tip_time_end_height = 20;
static CRect rc_time_end(tip_time_end_x, 
						 tip_time_end_y, 
						 tip_time_end_x + tip_time_end_width, 
						 tip_time_end_y + tip_time_end_height);

static const int time_end_x = 210;
static const int time_end_y = 5;
static const int time_end_width = 100;
static const int time_end_height = 20;

static const int edit_qihao_x = 430;
static const int edit_qihao_y = 1;
static const int edit_qihao_width = 60;
static const int edit_qihao_height = 20;

//确定按钮
static const int btn_ok_x = 520;
static const int btn_ok_y = 1;

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

//总盈亏
static const int yk_ze_x = 430;
static const int yk_ze_y = 519;
static const int yk_ze_width = 400;
static const int yk_ze_height = 20;
static CRect rc_yk_ze(yk_ze_x, 
						 yk_ze_y, 
						 yk_ze_x + yk_ze_width, 
						 yk_ze_y + yk_ze_height);


static const int page_size = 19;
// CZhangHuQPYkDlg 对话框

IMPLEMENT_DYNAMIC(CZhangHuQPYkDlg, CDialog)

CZhangHuQPYkDlg::CZhangHuQPYkDlg(CWnd* pParent /*=NULL*/)
: CDialog(CZhangHuQPYkDlg::IDD, pParent)
, m_bmpBk(NULL)
, m_tStart(CTime::GetCurrentTime())
, m_tEnd(CTime::GetCurrentTime())
, m_page(1)
, m_itemCount(0)
, m_maxPage(1)
, m_byTime(TRUE)
{
	m_fZongYingkui = SCORE_ZERO;
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
	m_bGetYingkuiMxCount = false;
	m_bGetYingkuiMx = false;
}

CZhangHuQPYkDlg::~CZhangHuQPYkDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CZhangHuQPYkDlg::DoDataExchange(CDataExchange* pDX)
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
}


BEGIN_MESSAGE_MAP(CZhangHuQPYkDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CZhangHuQPYkDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CZhangHuQPYkDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CZhangHuQPYkDlg::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CZhangHuQPYkDlg 消息处理程序

void CZhangHuQPYkDlg::OnPaint()
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

	CString strTmp;
	strTmp.Format(_T("第%d页/共%d页，每页%d条记录，共%d条记录"), 
		m_page, m_maxPage, page_size, m_itemCount);
	cacheDC.DrawText(strTmp, rc_tip_page, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strTmp.Format(_T("本页总盈亏:%.2lf"),m_fZongYingkui);
	cacheDC.DrawText(strTmp, rc_yk_ze, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CZhangHuQPYkDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CZhangHuQPYkDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

void CZhangHuQPYkDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_page = 1;
		SendToServer(1);
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

void CZhangHuQPYkDlg::OnBnClickedBtnOk()
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
	//m_tEnd += CTimeSpan(1L, 0, 0, 0);
	m_page = 1;
	SendToServer(1);
	SendToServer(2);

	InvalidateRect(rc_tip_page);
}

void CZhangHuQPYkDlg::OnBnClickedBtnPrePage()
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

void CZhangHuQPYkDlg::OnBnClickedBtnNextPage()
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

BOOL CZhangHuQPYkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg3.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt.png"));
	m_btnPrePage.SetImage(CBmpUtil::GetExePath() + _T("skin\\syy_bt.png"));
	m_btnNextPage.SetImage(CBmpUtil::GetExePath() + _T("skin\\xyy_bt.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
	m_dwTickCount = 0;
	InitListCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CZhangHuQPYkDlg::InitListCtrl()
{
	m_listLog.ModifyStyle(NULL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(95,82,81));

	m_listLog.InsertColumn(0, _T("ID"), LVCFMT_CENTER, 105);
	m_listLog.InsertColumn(1, _T("用户名"), LVCFMT_CENTER, 108);
	m_listLog.InsertColumn(2, _T("房间名"), LVCFMT_CENTER, 150);
	m_listLog.InsertColumn(3, _T("记录ID"), LVCFMT_CENTER, 150);
	m_listLog.InsertColumn(4, _T("进入时间"), LVCFMT_CENTER, 150);
	m_listLog.InsertColumn(5, _T("盈亏"), LVCFMT_CENTER, 130);
	m_listLog.InsertColumn(6, _T("备注"), LVCFMT_CENTER,190);
}

void CZhangHuQPYkDlg::AdjustCtrls()
{
	if (m_dateStart.GetSafeHwnd() != NULL)
	{
		m_dateStart.SetWindowPos(NULL, time_start_x, time_start_y, time_start_width, time_start_height, SWP_NOZORDER);
	}
	if (m_dateEnd.GetSafeHwnd() != NULL)
	{
		m_dateEnd.SetWindowPos(NULL, time_end_x, time_end_y, time_end_width, time_end_height, SWP_NOZORDER);
	}
	//if (m_editQihao.GetSafeHwnd() != NULL)
	//{
	//	m_editQihao.SetWindowPos(NULL, edit_qihao_x, edit_qihao_y, edit_qihao_width, edit_qihao_height, SWP_NOZORDER);
	//}
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

void CZhangHuQPYkDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CZhangHuQPYkDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

//读取事件
bool CZhangHuQPYkDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_QP_YINGKUI_COUNT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetQiPaiYingkuiCountRet));
				if(wDataSize!=sizeof(CMD_GR_GetQiPaiYingkuiCountRet)) return false;

				CMD_GR_GetQiPaiYingkuiCountRet* pCountRet = (CMD_GR_GetQiPaiYingkuiCountRet*)pData;

				m_itemCount = pCountRet->dwCount;
				m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);
				m_maxPage = max(1,m_maxPage);

				break;
			}
		case SUB_GP_GET_QP_YINGKUI_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_GetQiPaiYingkuiRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetQiPaiYingkuiRet)!=0) return false;

				m_fZongYingkui = SCORE_ZERO;
				m_listLog.DeleteAllItems();
				WORD wCount = wDataSize/sizeof(CMD_GR_GetQiPaiYingkuiRet);
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetQiPaiYingkuiRet *pYingkuiLogRet = ((CMD_GR_GetQiPaiYingkuiRet*)pData+i);



					DOUBLE f_t_yingkui = 0.0f;
					CString strTmp;
					strTmp.Format(_T("%d"), pYingkuiLogRet->n_t_userid);

					int item= m_listLog.InsertItem(m_listLog.GetItemCount(),strTmp);
					m_listLog.SetItemData(item,  pYingkuiLogRet->n_t_userid);



					m_listLog.SetItemText(item, 1,  pYingkuiLogRet->s_t_accounts);


					m_listLog.SetItemText(item, 2, pYingkuiLogRet->s_t_servername);

					strTmp.Format(_T("%d"), pYingkuiLogRet->n_t_drawid);
					m_listLog.SetItemText(item, 3, strTmp);

					strTmp.Format(_T("%d-%d-%d %d:%d:%d"), pYingkuiLogRet->n_t_playtime.wYear,pYingkuiLogRet->n_t_playtime.wMonth,pYingkuiLogRet->n_t_playtime.wDay,pYingkuiLogRet->n_t_playtime.wHour,pYingkuiLogRet->n_t_playtime.wMinute,pYingkuiLogRet->n_t_playtime.wSecond);
					m_listLog.SetItemText(item, 4,strTmp);

					strTmp.Format(_T("%.2lf"), pYingkuiLogRet->f_t_score);

					m_fZongYingkui += pYingkuiLogRet->f_t_score;

					m_listLog.SetItemText(item, 5, strTmp);

					strTmp.Format(_T(""));

					m_listLog.SetItemText(item, 6, strTmp);


				}
				break;
			}
		}
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CZhangHuQPYkDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //下注记录数量
	{
		m_bGetYingkuiMxCount = true;
		if(m_bGetYingkuiMxCount)
		{
			m_bGetYingkuiMxCount = false;
			CMD_GP_GetQiPaiYingkuiCount GetQiPaiYingkuiCount;
			ZeroMemory(&GetQiPaiYingkuiCount,sizeof(GetQiPaiYingkuiCount));

			GetQiPaiYingkuiCount.dwUserID = theAccount.user_id;
			GetQiPaiYingkuiCount.nTypeID = (m_byTime)?2:1;
			GetQiPaiYingkuiCount.nByTime = (m_byTime)?1:0;
// 			if(m_tStart == m_tEnd)
// 			{
// 				m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}
// 			m_tEnd += CTimeSpan(0, 23, 59, 59);

			CopyMemory(GetQiPaiYingkuiCount.szTimeStart , m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetQiPaiYingkuiCount.szTimeStart));
			CopyMemory(GetQiPaiYingkuiCount.szTimeEnd ,m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetQiPaiYingkuiCount.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_QP_YINGKUI_COUNT,&GetQiPaiYingkuiCount,sizeof(GetQiPaiYingkuiCount));
			return;
		}


	}

	if(nSendType == 2)//下注记录 
	{
		m_bGetYingkuiMx = true;
		if(m_bGetYingkuiMx)
		{
			m_bGetYingkuiMx = false;

			CMD_GP_GetQiPaiYingkui QiPaiYingkui;
			ZeroMemory(&QiPaiYingkui,sizeof(QiPaiYingkui));

			QiPaiYingkui.dwUserID = theAccount.user_id;
			QiPaiYingkui.bByTime = m_byTime?1:0;
			QiPaiYingkui.nPage = m_page;
			QiPaiYingkui.nSize = page_size;
// 			if(m_tStart == m_tEnd)
// 			{
// 				m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}
			CopyMemory(QiPaiYingkui.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(QiPaiYingkui.szTimeStart));
			CopyMemory(QiPaiYingkui.szTimeEnd  , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(QiPaiYingkui.szTimeEnd));


			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_QP_YINGKUI,&QiPaiYingkui,sizeof(QiPaiYingkui));
			return;
		}

	}

}