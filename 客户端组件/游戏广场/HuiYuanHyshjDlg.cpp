#include "stdafx.h"
#include "GamePlaza.h"
#include "HuiYuanHyshjDlg.h"
#include "PlatformFrame.h"
static const int tip_width = 200;
static const int tip_height = 20;
static const int tip_x = 152;
static const int time_start_x = 176;
static const int time_start_y = 38;
static const int time_start_width = 100;
static const int time_start_height = 20;
static const int time_end_x = 303;
static const int time_end_y = 38;
static const int time_end_width = 100;
static const int time_end_height = 20;
//确定按钮
static const int btn_ok_x = 521;
static const int btn_ok_y = 38;

static const int tip_span = 168;
//下级人数
static const int xiajirenshu_y = 163; 
static CRect rc_xiajirenshu(tip_x, 
																	xiajirenshu_y, 
																	tip_x + tip_width,
																	xiajirenshu_y + tip_height); 
//在线人数
static const int zaixianrenshu_y = 163;
static CRect rc_zaixianrenshu(tip_x + tip_span, 
																	zaixianrenshu_y, 
																	tip_x  + tip_span + tip_width,
																	zaixianrenshu_y + tip_height); 
//会员余额
static const int huiyuanyue_y = 121;
static CRect rc_huiyuanyue(tip_x , 
																	huiyuanyue_y, 
																	tip_x  + tip_span + tip_width,
																	huiyuanyue_y + tip_height); 
//今日注册
static const int jinrizhuce_y = 121;
static CRect rc_jinrizhuce(tip_x + tip_span, 
																	jinrizhuce_y, 
																	tip_x  + tip_span*2 + tip_width,
																	jinrizhuce_y + tip_height); 
//今日返点
static const int jinrifandian_y = 246;
static CRect rc_jinrifandian(tip_x + tip_span, 
																	jinrifandian_y, 
																	tip_x + tip_span + tip_width,
																	jinrifandian_y + tip_height); 
//撤单总额
static const int chedanzonge_y = 246;
static CRect rc_chedanzonge(tip_x + tip_span*2, 
																	chedanzonge_y, 
																	tip_x + tip_span*2 + tip_width,
																	chedanzonge_y + tip_height); 
//今日充值
static const int jinrichongzhi_y = 121;
static CRect rc_jinrichongzhi(tip_x + tip_span*2, 
																		jinrichongzhi_y, 
																		tip_x  + tip_span*3 + tip_width,
																		jinrichongzhi_y + tip_height); 
//今日取款
static const int jinriqukuan_y = 163;
static CRect rc_jinriqukuan(tip_x+ tip_span*2, 
																	jinriqukuan_y, 
																	tip_x+ tip_span*3 + tip_width,
																	jinriqukuan_y + tip_height); 
//今日投注
static const int jinritouzhu_y = 246;
static CRect rc_jinritouzhu(tip_x, 
																	jinritouzhu_y, 
																	tip_x + tip_width,
																	jinritouzhu_y + tip_height); 
//中奖总额
static const int zhongjiangzonge_y = 289;
static CRect rc_zhongjiangzonge(tip_x, 
																	zhongjiangzonge_y, 
																	tip_x + tip_width,
																	zhongjiangzonge_y + tip_height); 
//我的返点 - 彩票
static const int wodefandian_y = 289;
static CRect rc_wodefandian(tip_x+ tip_span, 
																	wodefandian_y, 
																	tip_x+ tip_span + tip_width,
																	wodefandian_y + tip_height); 
//彩票盈亏
static const int caipiaoyingkui_y = 289;
static CRect rc_caipiaoyingkui(tip_x+ tip_span*2, 
																	caipiaoyingkui_y, 
																	tip_x+ tip_span*2 + tip_width,
																	caipiaoyingkui_y + tip_height); 

//今日活动
static const int jinrihuodong = 168;
static CRect rc_jinrihuodong(tip_x+ tip_span*2, 
																	jinrihuodong, 
																	tip_x+ tip_span*2 + tip_width,
																	jinrihuodong + tip_height); 
//冼码
static const int xianmaliang_y = 371;
static CRect rc_xianmaliang(tip_x, 
																	xianmaliang_y, 
																	tip_x + tip_width,
																	xianmaliang_y + tip_height); 
//棋牌返点
static const int qipaifandian_y = 371;
static CRect rc_qipaifandian(tip_x+tip_span, 
																	qipaifandian_y, 
																	tip_x+tip_span + tip_width,
																	qipaifandian_y + tip_height); 
//棋牌盈亏
static const int qipaiyingkui_y = 414;
static CRect rc_qipaiyingkui(tip_x, 
																	qipaiyingkui_y, 
																	tip_x + tip_width,
																	qipaiyingkui_y + tip_height); 
//棋牌返点
static const int myqipaifandian_y = 414;
static CRect rc_myqipaifandian(tip_x+tip_span, 
																	myqipaifandian_y, 
																	tip_x+tip_span + tip_width,
																	myqipaifandian_y + tip_height); 
IMPLEMENT_DYNAMIC(CHuiYuanHyshjDlg, CDialog)

CHuiYuanHyshjDlg::CHuiYuanHyshjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuiYuanHyshjDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_tStart(CTime::GetCurrentTime())
	, m_tEnd(CTime::GetCurrentTime())
{
	m_bGetHuiyuanShj = false;
}

CHuiYuanHyshjDlg::~CHuiYuanHyshjDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CHuiYuanHyshjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DATE_START, m_dateStart);
	DDX_Control(pDX, IDC_DATE_END, m_dateEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_tStart);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_tEnd);

	DDX_Control(pDX, IDC_RIO_CHK_TODAY, m_rioChkToday);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
}

BEGIN_MESSAGE_MAP(CHuiYuanHyshjDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED(IDC_RIO_CHK_TODAY, &CHuiYuanHyshjDlg::OnBnClickedBtnChkToday)
	ON_BN_CLICKED(IDC_BTN_OK, &CHuiYuanHyshjDlg::OnBnClickedBtnOk)
END_MESSAGE_MAP()

void CHuiYuanHyshjDlg::OnBnClickedBtnChkToday()
{
	m_bChkToday = !m_bChkToday;
	if(m_bChkToday)
	{
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();
		m_dateStart.SetTime(&m_tStart);
		m_dateEnd.SetTime(&m_tEnd);
		UpdateData();

		m_rioChkToday.SetPushed(true);
	}
	else
		m_rioChkToday.SetPushed(false);
	return;
}
void CHuiYuanHyshjDlg::OnBnClickedBtnOk()
{
	if (m_tStart > m_tEnd)
	{
		MyMessageBox(_T("选择的时间区间不对"));
		return;
	}
	UpdateData();

	SendToServer(1);
	return;
}
void CHuiYuanHyshjDlg::AdjustCtrls()
{
	if (m_dateStart.GetSafeHwnd() != NULL)
	{
		m_dateStart.SetWindowPos(NULL, time_start_x, time_start_y, time_start_width, time_start_height, SWP_NOZORDER);
	}
	if (m_dateEnd.GetSafeHwnd() != NULL)
	{
		m_dateEnd.SetWindowPos(NULL, time_end_x, time_end_y, time_end_width, time_end_height, SWP_NOZORDER);
	}
	if (m_rioChkToday.GetSafeHwnd() != NULL)
	{
		m_rioChkToday.SetWindowPos(NULL, time_end_x+110, time_end_y, 71, 23, SWP_NOZORDER);
	}
	if (m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}

}
// CHuiYuanHyshjDlg 消息处理程序

void CHuiYuanHyshjDlg::OnPaint()
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

	cacheDC.DrawText(m_xiajirenshu, &rc_xiajirenshu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_zaixianrenshu, &rc_zaixianrenshu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_huiyuanyue, &rc_huiyuanyue, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinrizhuce, &rc_jinrizhuce, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinrifandian, &rc_jinrifandian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_chedanzonge, &rc_chedanzonge, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinrichongzhi, &rc_jinrichongzhi, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinriqukuan, &rc_jinriqukuan, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_jinritouzhu, &rc_jinritouzhu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
//	cacheDC.DrawText(m_jinrihuodong, &rc_jinrihuodong, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_zhongjiangzonge, &rc_zhongjiangzonge, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_wodefandian, &rc_wodefandian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_caipiaoyingkui, &rc_caipiaoyingkui, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_xianmaliang, &rc_xianmaliang, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_qipaifandian, &rc_qipaifandian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_qipaiyingkui, &rc_qipaiyingkui, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_myqipaifandian, &rc_myqipaifandian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CHuiYuanHyshjDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuiYuanHyshjDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		SendToServer(1);
	}
	else
	{
		m_tStart = CTime::GetCurrentTime();
		m_tEnd = CTime::GetCurrentTime();

		m_dateStart.SetTime(&m_tStart);
		m_dateEnd.SetTime(&m_tEnd);

	}
}

BOOL CHuiYuanHyshjDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hysj_bg.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 
	m_rioChkToday.SetPushed(TRUE);
	m_bChkToday=true;

//	m_rioChkToday.SetPaintTextColor(RGB(98,88,84));
	AdjustCtrls();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHuiYuanHyshjDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CHuiYuanHyshjDlg::OnOK()
{
	//CDialog::OnOK();
}


//读取事件
bool CHuiYuanHyshjDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_HYSHJ_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetHyShjRet));
				if(wDataSize!=sizeof(CMD_GR_GetHyShjRet)) return false;

				CMD_GR_GetHyShjRet* pCountRet = (CMD_GR_GetHyShjRet*)pData;
				
				m_xiajirenshu.Format(_T("%d"), pCountRet->n_t_xjrs);
				m_zaixianrenshu.Format(_T("%d"), pCountRet->n_t_xjzx);
				m_huiyuanyue.Format(_T("%.3lf"), pCountRet->f_t_hyye);
				m_jinrifandian.Format(_T("%.3lf"), pCountRet->f_t_jrfd);
				m_jinrichongzhi.Format(_T("%.3lf"), pCountRet->f_t_jrchz);
				m_jinriqukuan.Format(_T("%.3lf"), fabs(pCountRet->f_t_jrqk));
				m_jinritouzhu.Format(_T("%.3lf"), fabs(pCountRet->f_t_jrtzh));
				m_jinrihuodong.Format(_T("%.3lf"), fabs(pCountRet->f_t_jrhd));
				
				m_jinrizhuce.Format(_T("%d"),pCountRet->n_t_jinrizhuce);
				m_chedanzonge.Format(_T("%.3lf"), fabs(pCountRet->f_chedan_zonge));
				m_zhongjiangzonge.Format(_T("%.3lf"), fabs(pCountRet->f_zhongjiang_zonge));
				m_wodefandian.Format(_T("%.3lf"), fabs(pCountRet->f_wodefandian));
				m_caipiaoyingkui.Format(_T("%.3lf"),pCountRet->f_caipiao_yingkui);
				m_xianmaliang.Format(_T("%.3lf"), fabs(pCountRet->f_t_xianma));
				m_qipaifandian.Format(_T("%.3lf"), fabs(pCountRet->f_qp_fandian));
				m_qipaiyingkui.Format(_T("%.3lf"), pCountRet->f_qp_yingkui);
				m_myqipaifandian.Format(_T("%.3lf"), fabs(pCountRet->f_my_qp_fandian));
			}

		}
	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CHuiYuanHyshjDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //下注记录数量
	{
		m_bGetHuiyuanShj = true;
		if(m_bGetHuiyuanShj)
		{
			m_bGetHuiyuanShj = false;
			CMD_GP_GetHyShj GetHyShj;
			ZeroMemory(&GetHyShj,sizeof(GetHyShj));

			GetHyShj.dwUserID = theAccount.user_id;
			GetHyShj.cbOnlyToday = (m_bChkToday)?1:0;

			CopyMemory(GetHyShj.szTimeStart,m_tStart.Format(_T("%Y-%m-%d 00:00:00")),sizeof(GetHyShj.szTimeStart));
			CopyMemory(GetHyShj.szTimeEnd,m_tEnd.Format(_T("%Y-%m-%d 23:59:59.999")),sizeof(GetHyShj.szTimeEnd));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_HYSHJ,&GetHyShj,sizeof(GetHyShj));
			return;
		}

	}


}