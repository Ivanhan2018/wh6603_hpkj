#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhangHuYkMXDlg.h"
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

static const int page_size = 19;
// CZhangHuYkMXDlg 对话框

IMPLEMENT_DYNAMIC(CZhangHuYkMXDlg, CDialog)

CZhangHuYkMXDlg::CZhangHuYkMXDlg(CWnd* pParent /*=NULL*/)
: CDialog(CZhangHuYkMXDlg::IDD, pParent)
, m_bmpBk(NULL)
, m_tStart(CTime::GetCurrentTime())
, m_tEnd(CTime::GetCurrentTime())
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
	m_tStart = t;

	*/	
	m_bGetYingkuiMxCount = false;
	m_bGetYingkuiMx = false;
}

CZhangHuYkMXDlg::~CZhangHuYkMXDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CZhangHuYkMXDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CZhangHuYkMXDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CZhangHuYkMXDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CZhangHuYkMXDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CZhangHuYkMXDlg::OnBnClickedBtnNextPage)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CZhangHuYkMXDlg 消息处理程序

void CZhangHuYkMXDlg::OnPaint()
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

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CZhangHuYkMXDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CZhangHuYkMXDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

void CZhangHuYkMXDlg::OnShowWindow(BOOL bShow, UINT nStatus)
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

void CZhangHuYkMXDlg::OnBnClickedBtnOk()
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

void CZhangHuYkMXDlg::OnBnClickedBtnPrePage()
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

void CZhangHuYkMXDlg::OnBnClickedBtnNextPage()
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

BOOL CZhangHuYkMXDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg3.png"));

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

void CZhangHuYkMXDlg::InitListCtrl()
{
	m_listLog.ModifyStyle(NULL, LVS_SINGLESEL);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listLog.SetFont(&m_font);
	m_listLog.SetTextColor(RGB(95,82,81));

	m_listLog.InsertColumn(0, _T("用户名"), LVCFMT_CENTER, 110);
	m_listLog.InsertColumn(1, _T("订单ID"), LVCFMT_CENTER, 110);
	m_listLog.InsertColumn(2, _T("盈亏"), LVCFMT_CENTER, 110);
	m_listLog.InsertColumn(3, _T("彩票余额"), LVCFMT_CENTER, 133);
	m_listLog.InsertColumn(4, _T("盈亏时间"), LVCFMT_CENTER, 160);
	m_listLog.InsertColumn(5, _T("类型"), LVCFMT_CENTER, 130);
	m_listLog.InsertColumn(6, _T("摘要"), LVCFMT_CENTER,230);
}

void CZhangHuYkMXDlg::AdjustCtrls()
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

void CZhangHuYkMXDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CZhangHuYkMXDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

//读取事件
bool CZhangHuYkMXDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_YINGKUI_MX_COUNT_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetYingkuiMxCountRet));
				if(wDataSize!=sizeof(CMD_GR_GetYingkuiMxCountRet)) return false;

				CMD_GR_GetYingkuiMxCountRet* pCountRet = (CMD_GR_GetYingkuiMxCountRet*)pData;

				m_itemCount = pCountRet->dwCount;
				m_maxPage = (m_itemCount % page_size == 0)? m_itemCount / page_size: (m_itemCount / page_size + 1);

				m_maxPage = max(1,m_maxPage);
				break;
			}
		case SUB_GP_GET_YINGKUI_MX_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_GetYingkuiMxRet)==0);
				if(wDataSize % sizeof(CMD_GR_GetYingkuiMxRet)!=0) return false;

				m_listLog.DeleteAllItems();
				WORD wCount = wDataSize/sizeof(CMD_GR_GetYingkuiMxRet);
				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_GetYingkuiMxRet *pYingkuiLogRet = ((CMD_GR_GetYingkuiMxRet*)pData+i);


					DOUBLE f_t_yingkui = 0.0f;
					CString strTmp;

					int item= m_listLog.InsertItem(m_listLog.GetItemCount(), theAccount.account);
					m_listLog.SetItemData(item, theAccount.user_id);

					strTmp.Format(L"%ld",pYingkuiLogRet->n_t_dingdan);

					m_listLog.SetItemText(item, 1, strTmp);

					strTmp.Format(_T("%.3lf"), pYingkuiLogRet->f_t_yingkui);
					m_listLog.SetItemText(item, 2, strTmp);

					strTmp.Format(_T("%.3lf"), pYingkuiLogRet->f_t_yue);
					m_listLog.SetItemText(item, 3, strTmp);

					strTmp.Format(_T("%d-%d-%d %d:%d:%d"), pYingkuiLogRet->t_t_yingkuitime.wYear,pYingkuiLogRet->t_t_yingkuitime.wMonth,pYingkuiLogRet->t_t_yingkuitime.wDay,pYingkuiLogRet->t_t_yingkuitime.wHour,pYingkuiLogRet->t_t_yingkuitime.wMinute,pYingkuiLogRet->t_t_yingkuitime.wSecond);
					m_listLog.SetItemText(item, 4,strTmp);

					m_listLog.SetItemText(item, 5, pYingkuiLogRet->s_t_type);

					CString strZhaiyao;
					strZhaiyao.Format(L"%s",pYingkuiLogRet->s_t_zhaiyao);

					if((_tcscmp(pYingkuiLogRet->s_t_type,_T("投注")) == 0)||(_tcscmp(pYingkuiLogRet->s_t_type,_T("返点")) == 0)||(_tcscmp(pYingkuiLogRet->s_t_type,_T("下级返点")) == 0)||(_tcscmp(pYingkuiLogRet->s_t_type,_T("中奖")) == 0)||(_tcscmp(pYingkuiLogRet->s_t_type,_T("撤单")) == 0))
					{
						int nLength = strZhaiyao.Find(L"|");
						int nLength1 = strZhaiyao.Find(L"|",nLength);
						CString strQihao  = strZhaiyao.Left(nLength);
						CString strType = strZhaiyao.Mid(nLength+1,nLength1);
						CString wanfa = strZhaiyao.Right(strZhaiyao.GetLength()-nLength-3);

						if(strQihao.IsEmpty())
							break;
						int nType = _ttoi(strType);

						strTmp = theGameKind[nType];
// 						if(nType == CZChongQingSSC)
// 						{
// 							strTmp = _T("重庆彩");
// 						}
// 						else if(nType == CZJiangXiSSC)
// 						{
// 							strTmp = _T("江西彩");
// 						}
// 						else if(nType == CZ_TianJinSSC)
// 						{
// 							strTmp = _T("天津彩");
// 						}
// 						else if(nType == CZXinJiangSSC)
// 						{
// 							strTmp = _T("新疆彩");
// 						}
// 						else if(nType == CZ_FENFEN_CAI)
// 						{
// 							strTmp = _T("吉利分分彩");
// 						}
// 						else if(nType == CZ_WUFEN_CAI)
// 						{
// 							strTmp = _T("吉利五分彩");
// 						}
// 						else if(nType == CZGD11Xuan5)
// 						{
// 							strTmp = _T("广东11选5");
// 						}
// 						else if(nType == CZHLJ11Xuan5)
// 						{
// 							strTmp = _T("黑龙江11选5");
// 						}
// 						else if(nType == CZCQ11Xuan5)
// 						{
// 							strTmp = _T("重庆11选5");
// 						}
// 						else if(nType == CZSD11Xuan5)
// 						{
// 							strTmp = _T("山东11选5");
// 						}
// 						else if(nType == CZ_PK10)
// 						{
// 							strTmp = _T("北京PK10");
// 						}
// 						else if(nType == CZJX11Xuan5)
// 						{
// 							strTmp = _T("江西11选5");
// 						}
// 						else if(nType == CZ_QiXingCai)
// 						{
// 							strTmp = _T("七星彩");
// 						}
// 						else if(nType == CZPaiLie3)
// 						{
// 							strTmp = _T("排列三");
// 						}
// 						else if(nType == CZ3D)
// 						{
// 							strTmp = _T("3D彩");
// 						}

						CString str1;
						if(nType == CZ_FENFEN_CAI)
							str1.Format(L"[%s%s期]",strQihao.Left(8),strQihao.Right(strQihao.GetLength()-8));
						else
							str1.Format(L"[%s%s期]",strQihao.Left(8),strQihao.Right(strQihao.GetLength()-8-1));
						strTmp += str1;
						int nWanfa = _ttoi(wanfa);

						strTmp += theGameKind[(nType << 16) | (unsigned short)nWanfa];
					}
					else
					{
						strTmp.Format(L"%s",pYingkuiLogRet->s_t_zhaiyao); 
					}
					m_listLog.SetItemText(item, 6,strTmp);

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

VOID CZhangHuYkMXDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //下注记录数量
	{
		m_bGetYingkuiMxCount = true;
		if(m_bGetYingkuiMxCount)
		{
			m_bGetYingkuiMxCount = false;
			CMD_GP_GetYingkuiMxCount GetYingkuiMxCount;
			ZeroMemory(&GetYingkuiMxCount,sizeof(GetYingkuiMxCount));

			GetYingkuiMxCount.dwUserID = theAccount.user_id;
// 			if(m_tStart == m_tEnd)
// 			{
// 				m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}
// 			m_tEnd += CTimeSpan(0, 23, 59, 59);

			CopyMemory(GetYingkuiMxCount.szTimeStart , m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetYingkuiMxCount.szTimeStart));
			CopyMemory(GetYingkuiMxCount.szTimeEnd ,m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetYingkuiMxCount.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_YINGKUI_MX_COUNT,&GetYingkuiMxCount,sizeof(GetYingkuiMxCount));
		}

		return;
	}

	if(nSendType == 2)//下注记录 
	{
		m_bGetYingkuiMx = true;
		if(m_bGetYingkuiMx)
		{
			m_bGetYingkuiMx = false;

			CMD_GP_GetYingkuiMx YingkuiMx;
			ZeroMemory(&YingkuiMx,sizeof(YingkuiMx));

			YingkuiMx.dwUserID = theAccount.user_id;
			YingkuiMx.bByTime = m_byTime;
			YingkuiMx.nPage = m_page;
			YingkuiMx.nSize = page_size;
// 			if(m_tStart == m_tEnd)
// 			{
// 				m_tEnd += CTimeSpan(1L, 0, 0, 0);
// 			}
			CopyMemory(YingkuiMx.szTimeStart ,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(YingkuiMx.szTimeStart));
			CopyMemory(YingkuiMx.szTimeEnd  , m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(YingkuiMx.szTimeEnd));


			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_YINGKUI_MX,&YingkuiMx,sizeof(YingkuiMx));

			return;
		}
	}

}