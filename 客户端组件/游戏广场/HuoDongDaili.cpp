#include "stdafx.h"
#include "Gameplaza.h"
#include "HuoDongDaili.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
#define  IDC_BTN_QIANDAO	100
// CHuoDongDaliBao 对话框
static int qiandao_begin_x = 239;
static int qiandao_begin_y = 174;


static int reg_start_x = 192;
static int reg_start_y = 93;
static int reg_start_cx = 100;
static int reg_start_cy = 20;

static int reg_span_cx = 298;
static int reg_span_cy = 109;
IMPLEMENT_DYNAMIC(CHuoDongDaili, CDialog)

CHuoDongDaili::CHuoDongDaili(CWnd* pParent /*=NULL*/)
	: CDialog(CHuoDongDaili::IDD, pParent)
	, m_bmpBk(NULL)
{
	m_cbNewRegUser=0;
}

CHuoDongDaili::~CHuoDongDaili()
{	
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
	m_font.DeleteObject();
}

void CHuoDongDaili::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHuoDongDaili, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_QIANDAO, &CHuoDongDaili::OnBnClickedLingJiang)
	ON_BN_CLICKED(IDC_BTN_QIANDAO+1, &CHuoDongDaili::OnBnClickedLingJiang)
	ON_BN_CLICKED(IDC_BTN_QIANDAO+2, &CHuoDongDaili::OnBnClickedLingJiang)
	ON_BN_CLICKED(IDC_BTN_QIANDAO+3, &CHuoDongDaili::OnBnClickedLingJiang)
	ON_BN_CLICKED(IDC_BTN_QIANDAO+4, &CHuoDongDaili::OnBnClickedLingJiang)
	ON_BN_CLICKED(IDC_BTN_QIANDAO+5, &CHuoDongDaili::OnBnClickedLingJiang)
	ON_BN_CLICKED(IDC_BTN_QIANDAO+6, &CHuoDongDaili::OnBnClickedLingJiang)
END_MESSAGE_MAP()


// CHuoDongDaliBao 消息处理程序
BOOL CHuoDongDaili::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\hd_daili.png"));

	m_font.CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	CRect rcCreate(0,0,0,0);
	for (int i = 0;i < CountArray(m_btnQianDao);i++)
	{
		m_btnQianDao[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_QIANDAO+i);
		m_btnQianDao[i].SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_lingjiang2.png"));
		//m_btnQianDao[i].EnableWindow(FALSE);
	}
	AdjustLingjiang();
	m_nToday = 0;
	return TRUE;  
}

void CHuoDongDaili::OnPaint()
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

	cacheDC.SetBkMode(TRANSPARENT);

	COLORREF crTextColor = cacheDC.SetTextColor(RGB(88,78,131));
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	CString str;
	DOUBLE fScore[9] = {488.00,988.00,1988.00,5800.00,8800.00,9888.00,14888.00,16888.00,22888.00};
	for(WORD i = 0;i < 9;i++)
	{
		if(m_cbNewRegUser == 1)
			str = _T("是");
		else 
			str = _T("否");
		CRect rcReg;
		int nXPos = reg_start_x+(i%3)*reg_span_cx-25;
		int nYPos = reg_start_y+(i/3)*reg_span_cy+33;

		rcReg.left = nXPos;
		rcReg.top = nYPos;
		rcReg.right = rcReg.left+reg_start_cx;
		rcReg.bottom = rcReg.top+reg_start_cy;
		cacheDC.DrawText(str,rcReg,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
		rcReg.left -= 20;
		rcReg.top += 21;
		rcReg.bottom = rcReg.top+reg_start_cy;
		str.Format(L"%.2lf/%.2lf",abs(m_fTouZhuJine),fScore[i]);
		cacheDC.DrawText(str,rcReg,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

		rcReg.top += 20;
		rcReg.bottom = rcReg.top+reg_start_cy;
		str.Format(L"%.2lf/100",m_fChongZhiJine);
		cacheDC.DrawText(str,rcReg,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
	}
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SetTextColor(crTextColor);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheDC.SelectObject(pOldFont);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

}
void CHuoDongDaili::AdjustLingjiang()
{
	int n_space_x = 0;
	int n_space_y = 0;

	n_space_x =  298;
	n_space_y =  109;
	int nWidth = 83;
	int nHeight = 28;
	if(m_btnQianDao[0].GetSafeHwnd() != NULL)
	{
		m_btnQianDao[0].SetWindowPos(NULL, qiandao_begin_x, qiandao_begin_y, nWidth, nHeight, SWP_NOZORDER);
	}
	if(m_btnQianDao[1].GetSafeHwnd() != NULL)
	{
		m_btnQianDao[1].SetWindowPos(NULL, qiandao_begin_x+n_space_x, qiandao_begin_y,  nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQianDao[2].GetSafeHwnd() != NULL)
	{
		m_btnQianDao[2].SetWindowPos(NULL, qiandao_begin_x+n_space_x*2, qiandao_begin_y,  nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQianDao[3].GetSafeHwnd() != NULL)
	{
		m_btnQianDao[3].SetWindowPos(NULL, qiandao_begin_x, qiandao_begin_y+n_space_y,  nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQianDao[4].GetSafeHwnd() != NULL)
	{
		m_btnQianDao[4].SetWindowPos(NULL, qiandao_begin_x+n_space_x, qiandao_begin_y+n_space_y, nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQianDao[5].GetSafeHwnd() != NULL)
	{
		m_btnQianDao[5].SetWindowPos(NULL, qiandao_begin_x+n_space_x*2, qiandao_begin_y+n_space_y, nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQianDao[6].GetSafeHwnd() != NULL)
	{
		m_btnQianDao[6].SetWindowPos(NULL, qiandao_begin_x, qiandao_begin_y+n_space_y*2,nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQianDao[7].GetSafeHwnd() != NULL)
	{
		m_btnQianDao[7].SetWindowPos(NULL, qiandao_begin_x+n_space_x, qiandao_begin_y+n_space_y*2, nWidth,nHeight, SWP_NOZORDER);
	}
	if(m_btnQianDao[8].GetSafeHwnd() != NULL)
	{
		m_btnQianDao[8].SetWindowPos(NULL, qiandao_begin_x+n_space_x*2, qiandao_begin_y+n_space_y*2, nWidth,nHeight, SWP_NOZORDER);
	}

}
void CHuoDongDaili::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}
}

void CHuoDongDaili::OnBnClickedLingJiang()
{
	SendToServer(2);
}
//判断是否能够参加活动
void CHuoDongDaili::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		if(theAccount.user_id <= 0)
			return;
		SendToServer(1);
	}
}


//读取事件
bool CHuoDongDaili::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_DAILI_HUIKUI_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetDailiHuikuiRet));
				if(wDataSize != sizeof(CMD_GP_GetDailiHuikuiRet)) return false;

				CMD_GP_GetDailiHuikuiRet* pGetDailiHuikui = (CMD_GP_GetDailiHuikuiRet*)pData;
				
				m_cbNewRegUser = pGetDailiHuikui->cbNewRegUser;
				m_fTouZhuJine = pGetDailiHuikui->f_t_touzhu;
				m_fChongZhiJine = pGetDailiHuikui->f_t_chongzhi;
				
				for (int i = 0;i < CountArray(m_btnQianDao);i++)
				{
					m_btnQianDao[i].EnableWindow(FALSE);
					m_btnQianDao[i].SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_lingjiang3.png"));
				}
			//	MessageBox(L"EnableWindow");
				if(m_cbNewRegUser == 1)
				{

					if(m_fChongZhiJine < 100)
					{
						return TRUE;
					}

					DOUBLE fTouzhu[9] = {488.00,988.00,1988.00,5800.00,8800.00,9888.00,14888.00,16888.00,22888.00};

					for(int i = CountArray(fTouzhu)-1;i >= 0 ;i--)
					{
						if(m_fTouZhuJine>=fTouzhu[i])
						{
							m_btnQianDao[i].EnableWindow(TRUE);
							m_btnQianDao[i].SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_lingjiang2.png"));

							return TRUE;
						}
					}
				}
				Invalidate(FALSE);

				return TRUE;
			}
		case SUB_GP_DAILI_LJ_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_DailiLingjiangRet));
				if(wDataSize != sizeof(CMD_GP_DailiLingjiangRet)) return false;

				CMD_GP_DailiLingjiangRet* pDailiHuikui = (CMD_GP_DailiLingjiangRet*)pData;
				int nState = pDailiHuikui->n_t_state;
				
				if(nState == 0)
				{
					CString strMsg;
					strMsg.Format(L"恭喜您领奖【%.2lf】成功",pDailiHuikui->f_t_jine);
					MyMessageBox(strMsg);
					m_btnQianDao[m_nToday].EnableWindow(FALSE);
				}
				else if(nState == 1)
				{
					MyMessageBox(L"您不是48小时内的新注册用户！");
				}
				else if(nState == 2)
				{
					MyMessageBox(L"充值量未达到100，未达标！");

				}
				else if(nState == 3)
				{
					MyMessageBox(L"投注量未达标！");
				}
				else if(nState == 4)
				{
					MyMessageBox(L"您已领取首充大礼包！");
				}
				return true;
			}
		default:
			{
				return true;
			}
		}
	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CHuoDongDaili::SendToServer(int nSendType)
{
	if(nSendType == 1)
	{
		CMD_GP_GetDailiHuikui GetDailiHuikui;
		ZeroMemory(&GetDailiHuikui,sizeof(GetDailiHuikui));

		GetDailiHuikui.n_t_userid = theAccount.user_id;

		CPlatformFrame* pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_DAILI_HUIKUI,&GetDailiHuikui,sizeof(GetDailiHuikui));
		return;
	}

	if(nSendType == 2)
	{
		CMD_GP_DailiLingjiang DailiLingjiang;
		ZeroMemory(&DailiLingjiang,sizeof(DailiLingjiang));

		DailiLingjiang.n_t_userid = theAccount.user_id;

		CPlatformFrame* pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_DAILI_LJ,&DailiLingjiang,sizeof(DailiLingjiang));
		return;

	}
	return;

}