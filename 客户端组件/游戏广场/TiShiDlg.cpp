#include "stdafx.h"
#include "GamePlaza.h"
#include "TiShiDlg.h"


static const int wnd_width = 258;
static const int wnd_height = 235;

static const int btn_ok_x = 110;
static const int btn_ok_y = 140;

static const int close_x = 290;
static const int close_y = 3;

static const int icon_x = 20;
static const int icon_y = 70;

static CRect rc_title(0, 10, wnd_width, 32);
static CRect rc_msg(117, 140, wnd_width, wnd_height - 32);



static const int timer_show_tishi = 1;
static const int timer_get_tishi = 2;
static const int timer_auto_close = 3;

IMPLEMENT_DYNAMIC(CTiShiDlg, CDialog)
//时间标识
#define IDI_MESSAGE					100									//消息标识


map<int ,int> m_mapTypeID;
map<int ,BYTE> m_mapMoshi;
map<int ,DOUBLE> m_mapZhushu;		//投注金额
map<int ,DOUBLE> m_mapZj	;		//中奖金额			
map<int ,int> m_mapBeiShu;
map<int ,CString> m_mapQiHao;
map<int ,DOUBLE> m_mapYingkui;

CTiShiDlg::CTiShiDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTiShiDlg::IDD, pParent)
, m_bmpBk(NULL)
, m_bmpIcon(NULL)
{
	//任务组件
//	m_MissionManager.InsertMissionItem(this);
	m_bGetChongzhiTishi=false;
	m_bTixianTishi=false;
	m_bTouzhuTishi=false;
	m_nElapse=30000;

}

CTiShiDlg::~CTiShiDlg()
{
	if(m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}

	if (m_bmpIcon != NULL)
	{
		delete m_bmpIcon;
	}
}

void CTiShiDlg::ShowTiShi(const CString& title, const CString& msg)
{
	m_title = title;
	m_msg = msg;
	ShowWindow(SW_SHOW);
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	SetTimer(IDI_MESSAGE,5000,NULL);

}

void CTiShiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CL, m_btnClose);
}

BEGIN_MESSAGE_MAP(CTiShiDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CL, &CTiShiDlg::OnBnClickedBtnClose)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTiShiDlg 消息处理程序

void CTiShiDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 
		0, 0, m_bmpBk->GetWidth(), m_bmpBk-> GetHeight(), UnitPixel);
// 	graphics.DrawImage(m_bmpIcon, Rect(icon_x, icon_y, m_bmpIcon->GetWidth(), m_bmpIcon-> GetHeight()), 
// 		0, 0, m_bmpIcon->GetWidth(), m_bmpIcon-> GetHeight(), UnitPixel);

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_fontTitle);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(251,245,219));

// 	cacheDC.DrawText(m_title.IsEmpty()? szProduct: m_title, 
// 		rc_title, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cacheDC.SetTextColor(RGB(255,255,255));
	cacheDC.SelectObject(&m_fontMsg);
	
	
	map<int, int>::iterator iter = m_mapTypeID.begin();
	int nFirst = iter->first;
	int nTypeID = m_mapTypeID[nFirst];
	TCHAR szTemp[126] = TEXT("");
	_sntprintf(szTemp,CountArray(szTemp),TEXT("%s"),theGameType[nTypeID]);
	cacheDC.DrawText(szTemp, rc_msg, DT_LEFT);

	CRect rcTemp;
	rcTemp.CopyRect(rc_msg);
	rcTemp.top += 17;
	rcTemp.bottom += 17;
	_sntprintf(szTemp,CountArray(szTemp),TEXT("%s"),m_mapQiHao[nFirst]);
	cacheDC.DrawText(szTemp, rcTemp, DT_LEFT);

	DOUBLE fTouzhu =  0.00f;
	DOUBLE fDanzhujine = 0.00f;

	BYTE cbMoshi = m_mapMoshi[0];
	if(cbMoshi == MoShi_Yuan)
	{	
		fDanzhujine = 2.0;
	}
	else if (cbMoshi == MoShi_Jiao)
	{
		fDanzhujine = 0.2;
	}
	else if(cbMoshi == MoShi_Fen)
	{
		fDanzhujine = 0.02;
	}
	else if(cbMoshi == MoShi_Li)
		fDanzhujine = 0.002;

	fTouzhu = m_mapZhushu[nFirst];
	rcTemp.top += 17;
	rcTemp.bottom += 17;
	_sntprintf(szTemp,CountArray(szTemp),TEXT("%.2lf元"),fTouzhu);
	cacheDC.DrawText(szTemp, rcTemp, DT_LEFT);
	rcTemp.top += 17;
	rcTemp.bottom += 17;
	_sntprintf(szTemp,CountArray(szTemp),TEXT("%.2lf元"),m_mapZj[nFirst]);
	cacheDC.DrawText(szTemp, rcTemp, DT_LEFT);
	rcTemp.top += 17;
	rcTemp.bottom += 17;
	_sntprintf(szTemp,CountArray(szTemp),TEXT("%.2lf元"),m_mapYingkui[nFirst]);
	cacheDC.DrawText(szTemp, rcTemp, DT_LEFT);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CTiShiDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CTiShiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\dialog\\msg_bk_ts.png"));
	m_bmpIcon = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\dialog\\msg_Ordi.png"));

	width = m_bmpBk->GetWidth();
	height = m_bmpBk->GetHeight();
	m_btnClose.SetImage(CBmpUtil::GetExePath() + _T("skin\\dialog\\win_close.png"));

	m_fontTitle.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_fontMsg.CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	SetPos();

	//SetTimer(IDI_MESSAGE,3000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CTiShiDlg::StartSetTimer()
{
// 	SetTimer(timer_show_tishi, 1000, NULL);
// 	SetTimer(timer_get_tishi, 60000, NULL);
	//设置时间
}
void CTiShiDlg::StartKillTimer()
{
	KillTimer(timer_show_tishi);
	KillTimer(timer_get_tishi);
	KillTimer(IDI_MESSAGE);
}

void CTiShiDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	if (m_btnClose.GetSafeHwnd() != NULL) 
	{
		m_btnClose.SetWindowPos(NULL, width - m_btnClose.Width()-5, close_y, m_btnClose.Width(), m_btnClose.Height(), SWP_NOZORDER);
	}
}

void CTiShiDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == timer_show_tishi)
	{
		if(/*!IsWindowVisible() &&*/ m_mapTypeID.size() > 0)
		{
			ShowTiShi(szProduct, m_tishi.GetAt(0));

// 			m_csTishi.Lock();
// 			m_tishi.RemoveAll();
// 			m_csTishi.Unlock();
		}
//		else
		{
			KillTimer(timer_show_tishi);
		}
	}
	else if (nIDEvent == timer_get_tishi) {
		m_csTishi.Lock();
//// 		SendToServer(1);
//// 		SendToServer(2);
//		SendToServer(3);
		CWnd *pParent = GetParent();
		if(pParent!=NULL)
		{
			pParent->PostMessage(IDM_TOUZHU_TISHI);
		}

		m_csTishi.Unlock();
	}
	//倒数时间
	else if (nIDEvent==IDI_MESSAGE)
	{
		KillTimer(IDI_MESSAGE);
		int nSize = m_mapTypeID.size();
		if( m_mapTypeID.size() > 0)
		{
			map<int, int>::iterator iter = m_mapTypeID.begin();
			int nFirst = iter->first;
			m_mapTypeID.erase(nFirst);
			m_mapMoshi.erase(nFirst);
			m_mapBeiShu.erase(nFirst);
			m_mapZhushu.erase(nFirst);
			m_mapQiHao.erase(nFirst);
			m_mapZj.erase(nFirst);
			m_mapYingkui.erase(nFirst);

		}

		 nSize = m_mapTypeID.size();

		if(/*!IsWindowVisible() &&*/ m_mapTypeID.size() > 0)
		{
			ShowWindow(SW_HIDE);
			SetTimer(timer_show_tishi, 1000, NULL);
		}
		else
		{
			OnBnClickedBtnClose();

		}
		return;
	}

	CDialog::OnTimer(nIDEvent);
}
void	CTiShiDlg::OnLButtonUp(UINT	nFlags, CPoint	point)
{
	OnBnClickedBtnClose();

}

void CTiShiDlg::OnDestroy()
{
	CDialog::OnDestroy();

	KillTimer(timer_show_tishi);
	KillTimer(timer_get_tishi);
}

void CTiShiDlg::OnBnClickedBtnClose()
{
	//::AnimateWindow(m_hWnd, 500, AW_HIDE|AW_SLIDE|AW_VER_POSITIVE);
	ShowWindow(SW_HIDE);
}

void CTiShiDlg::SetPos()
{
	int bottom = 0;
	HWND hWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	if (hWnd) {
		RECT rc  = {0, 0, 0, 0};
		::GetWindowRect(hWnd, &rc);
		bottom = GetSystemMetrics(SM_CYSCREEN) - (rc.bottom-rc.top);
	}
	else{
		bottom = GetSystemMetrics(SM_CYFULLSCREEN);
	}

	int wndX = GetSystemMetrics(SM_CXSCREEN) - width;
	int wndY = bottom - height;

	SetWindowPos(NULL, wndX, wndY, width,height, SWP_HIDEWINDOW);
}


//读取事件
bool CTiShiDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch(Command.wSubCmdID)
		{
		case SUB_GP_GET_CHONGZHI_TISHI_RET:
			{
				ASSERT((wDataSize% sizeof(CMD_GR_ChongzhiTishiRet))==0);
				if(( wDataSize% sizeof(CMD_GR_ChongzhiTishiRet))!=0) return false;

				int nCount = wDataSize/sizeof(CMD_GR_ChongzhiTishiRet);
				for (int i = 0;i < nCount;i++)
				{
					CMD_GR_ChongzhiTishiRet* pChongzhiTishiRet = ((CMD_GR_ChongzhiTishiRet*)pData+i);

					if (pChongzhiTishiRet->nState == 1) {
						CString msg;
						msg.Format(_T("您成功充值%0.2f元"), pChongzhiTishiRet->fJine);
						m_tishi.Add(msg);
					}
				}
				return true;
			}	
		case SUB_GP_GET_TIXIAN_TISHI_RET:
			{
				ASSERT((wDataSize% sizeof(CMD_GR_TixianTishiRet))==0);
				if(( wDataSize% sizeof(CMD_GR_TixianTishiRet))!=0) return false;

				int nCount = wDataSize/sizeof(CMD_GR_TixianTishiRet);
				for (int i = 0;i < nCount;i++)
				{
					CMD_GR_TixianTishiRet* pTixianTishiRet = ((CMD_GR_TixianTishiRet*)pData+i);

					int state = pTixianTishiRet->nState;
					DOUBLE jine = pTixianTishiRet->fJine;
					CString msg;
					if (state == 1)
					{
						msg.Format(_T("%0.2f元已打入您的账户"), jine);
					}
					else if (state == 2)
					{
						msg.Format(_T("%0.2f元提现申请被拒绝"), jine);
					}
					else {
						msg.Format(_T("%0.2f元提现申请正在处理"), jine);
					}
					m_tishi.Add(msg);

				}
				return true;

			}
		case SUB_GP_GET_TOUZHU_TISHI_RET:
			{
				ASSERT((wDataSize% sizeof(CMD_GR_TouzhuTishiRet))==0);
				if(( wDataSize% sizeof(CMD_GR_TouzhuTishiRet))!=0) return false;

				int nCount = wDataSize/sizeof(CMD_GR_TouzhuTishiRet);

// 				m_mapTypeID.clear();
// 				m_mapZhushu.clear();
// 				m_mapQiHao.clear();
// 
// 				m_mapYingkui.clear();
// 				m_mapZj.clear();


				bool bFresh = false;
				for (int i = 0;i < nCount;i++)
				{
					CMD_GR_TouzhuTishiRet* pTouzhuTishiRet = ((CMD_GR_TouzhuTishiRet*)pData+i);

					CString strTemp;
					strTemp.Format(L"%s", pTouzhuTishiRet->szQishu);
					if(strTemp.IsEmpty())
						continue;

					int nSize = m_mapTypeID.size();
					
					CString strQihao;
					strQihao.Format(L"%s",pTouzhuTishiRet->szQishu);

					m_mapTypeID.insert(map<int, int>::value_type(nSize, pTouzhuTishiRet->nType));
					//m_mapMoshi.insert(map<int, BYTE>::value_type(nSize, pTouzhuTishiRet->cbMoshi));
					m_mapZhushu.insert(map<int, DOUBLE>::value_type(nSize, pTouzhuTishiRet->nZhushu));
					//m_mapBeiShu.insert(map<int, int>::value_type(nSize, pTouzhuTishiRet->nBeishu));
					m_mapQiHao.insert(map<int, CString>::value_type(nSize, strQihao));

					m_mapYingkui.insert(map<int, DOUBLE>::value_type(nSize, pTouzhuTishiRet->f_yingkui));
					m_mapZj.insert(map<int, DOUBLE>::value_type(nSize, pTouzhuTishiRet->nWinZhushu));


					
					 nSize = m_mapTypeID.size();


 					CString msg;
 					if(pTouzhuTishiRet->nWinZhushu <= 0) {
 						msg.Format(_T("没中奖，继续努力！\n%s\n%s期"), theGameType[pTouzhuTishiRet->nType], pTouzhuTishiRet->szQishu);
 					} else {
 						bFresh = true;
 						msg.Format(_T("恭喜您，中奖了！！！\n%s\n%s期"), theGameType[pTouzhuTishiRet->nType], pTouzhuTishiRet->szQishu);
 					}
 					m_tishi.Add(msg);
				}
				SetTimer(timer_show_tishi, 1000, NULL);
				//ShowTiShi(szProduct, m_tishi.GetAt(0));
				return true;

			}
		
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}


VOID CTiShiDlg::SetTiShiMessage(CString strMsg)
{
	m_tishi.Add(strMsg);

	SetTimer(timer_show_tishi, 1000, NULL);

	m_nTypeID = 1;
	m_cbMoshi = 1;
	m_nZhushu = 10;
	m_nBeiShu = 2;
	_sntprintf(m_szQiHao,CountArray(m_szQiHao),TEXT("20160527310"));

	m_fYingKui = 10.00f;
}
