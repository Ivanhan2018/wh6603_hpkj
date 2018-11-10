#include "stdafx.h"
#include "GamePlaza.h"
#include "HuoDongZaJinDanDlg.h"
#include "Mmsystem.h"
#include "MessageDlg.h"

//金蛋数据
static const int dan_width = 244;
static const int dan_height = 235;

static const int yindan_x = 25;
static const int yindan_y = 70;
static const int jindan_x = yindan_x + dan_width;
static const int jindan_y = yindan_y;
static const int tongdan_x = jindan_x + dan_width;
static const int tongdan_y = yindan_y;

static Rect rc_yindan(yindan_x, yindan_y, dan_width, dan_height);
static Rect rc_jindan(jindan_x, jindan_y, dan_width, dan_height);
static Rect rc_tongdan(tongdan_x, tongdan_y, dan_width, dan_height);

static CRect crc_yindan(yindan_x, yindan_y, yindan_x + dan_width, yindan_y + dan_height);
static CRect crc_jindan(jindan_x, jindan_y, jindan_x + dan_width, jindan_y + dan_height);
static CRect crc_tongdan(tongdan_x, tongdan_y, tongdan_x + dan_width, tongdan_y + dan_height);
static CRect rc_hit_yindan(yindan_x+40, yindan_y+20, yindan_x+40 + dan_width-100, yindan_y+20 + dan_height-40);
static CRect rc_hit_jindan(jindan_x+40, jindan_y+20, jindan_x+40 + dan_width-100, jindan_y+20 + dan_height-40);
static CRect rc_hit_tongdan(tongdan_x+40, tongdan_y+20, tongdan_x+40 + dan_width-100, tongdan_y+20 + dan_height-40);

//锤子数据
static const int chuizi_width = 190;
static const int chuizi_height = 180;
static Rect rc_yindan_chuizi(yindan_x+50, yindan_y, chuizi_width, chuizi_height);
static Rect rc_jindan_chuizi(jindan_x+50, jindan_y, chuizi_width, chuizi_height);
static Rect rc_tongdan_chuizi(tongdan_x+50, tongdan_y, chuizi_width, chuizi_height);

static CRect crc_yindan_chuizi(yindan_x+50, yindan_y, yindan_x+50 + chuizi_width, yindan_y + chuizi_height);
static CRect crc_jindan_chuizi(jindan_x+50, jindan_y, jindan_x+50 + chuizi_width, jindan_y + chuizi_height);
static CRect crc_tongdan_chuizi(tongdan_x+50, tongdan_y, tongdan_x+50 + chuizi_width, tongdan_y + chuizi_height);


#define TIMER_EVENT_JINDAN	1
#define TIMER_EVENT_YINDAN	2
#define TIMER_EVENT_TONGDAN	3

static const int timer_elapse = 150;

IMPLEMENT_DYNAMIC(CHuoDongZaJinDanDlg, CDialog)

CHuoDongZaJinDanDlg::CHuoDongZaJinDanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuoDongZaJinDanDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_yindanState(YinDan_Image_1)
	, m_jindanState(JinDan_Image_1)
	, m_tongdanState(TongDan_Image_1)
	, m_yindanChuiziState(ChuiZi_Image_Up)
	, m_jindanChuiziState(ChuiZi_Image_Up)
	, m_tongdanChuiziState(ChuiZi_Image_Up)
	, m_yindan_win(false)
	, m_jindan_win(false)
	, m_tongdan_win(false)
	, m_jinrixiaofei(0.0)
	, m_got(true)
	, m_bZhuiZi(true)
{
	m_bCanZaJinDan = false;
	m_bGetZaJinDan=false;
	m_cbJindanType = 0;
}

CHuoDongZaJinDanDlg::~CHuoDongZaJinDanDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
}

void CHuoDongZaJinDanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHuoDongZaJinDanDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CHuoDongZaJinDanDlg 消息处理程序

BOOL CHuoDongZaJinDanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\hd_yjdhk.png"));

// 	m_bmpTongDan[TongDan_Image_1] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\tongdan1.png"));
// 	m_bmpTongDan[TongDan_Image_2] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\tongdan2.png"));
// 	m_bmpTongDan[TongDan_Image_3] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\tongdan3.png"));
// 	m_bmpTongDan[TongDan_Image_4] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\tongdan4.png"));
// 	m_bmpTongDan[TongDan_Image_5] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\tongdan5.png"));
// 	m_bmpTongDan[TongDan_Image_6] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\tongdan6.png"));
// 
// 	m_bmpYinDan[YinDan_Image_1] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\yindan1.png"));
// 	m_bmpYinDan[YinDan_Image_2] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\yindan2.png"));
// 	m_bmpYinDan[YinDan_Image_3] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\yindan3.png"));
// 	m_bmpYinDan[YinDan_Image_4] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\yindan4.png"));
// 	m_bmpYinDan[YinDan_Image_5] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\yindan5.png"));
// 	m_bmpYinDan[YinDan_Image_6] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\yindan6.png"));
// 
// 	m_bmpJinDan[JinDan_Image_1] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\jindan1.png"));
// 	m_bmpJinDan[JinDan_Image_2] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\jindan2.png"));
// 	m_bmpJinDan[JinDan_Image_3] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\jindan3.png"));
// 	m_bmpJinDan[JinDan_Image_4] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\jindan4.png"));
// 	m_bmpJinDan[JinDan_Image_5] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\jindan5.png"));
// 	m_bmpJinDan[JinDan_Image_6] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\jindan6.png"));
// 
// 	m_bmpChuiZi[ChuiZi_Image_Up] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\chuiz1.png"));
// 	m_bmpChuiZi[ChuiZi_Image_Mid] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\chuiz2.png"));
// 	m_bmpChuiZi[ChuiZi_Image_Down] = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\chuiz3.png"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHuoDongZaJinDanDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

// 	DrawYinDanImage(graphics);
// 	DrawJinDanImage(graphics);
// 	DrawTongDanImage(graphics);
	
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CHuoDongZaJinDanDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHuoDongZaJinDanDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (TIMER_EVENT_JINDAN == nIDEvent)
	{
		if (!m_jindan_win)
		{
			if(m_jindanState == JinDan_Image_1)
			{
				if (m_jindanChuiziState == ChuiZi_Image_Up)
				{
					m_jindanChuiziState = ChuiZi_Image_Mid;
				}
				else if (m_jindanChuiziState == ChuiZi_Image_Mid)
				{
					m_jindanChuiziState = ChuiZi_Image_Down;
				}
				else if (m_jindanChuiziState == ChuiZi_Image_Down)
				{
					m_jindanState = JinDan_Image_6;
				}
				
				KillTimer(TIMER_EVENT_JINDAN);
				m_bZhuiZi = false;
				InvalidateRect(&crc_yindan);
				InvalidateRect(&crc_tongdan);
			}
		}
		else
		{
			if(m_jindanState == JinDan_Image_1)
			{
				if (m_jindanChuiziState == ChuiZi_Image_Up)
				{
					m_jindanChuiziState = ChuiZi_Image_Mid;
				}
				else if (m_jindanChuiziState == ChuiZi_Image_Mid)
				{
					m_jindanChuiziState = ChuiZi_Image_Down;
				}
				else if (m_jindanChuiziState == ChuiZi_Image_Down)
				{
					m_jindanState = JinDan_Image_2;
				}
			}
			else if(m_jindanState == JinDan_Image_2)
			{
				m_jindanState = JinDan_Image_3;
			}
			else if(m_jindanState == JinDan_Image_3)
			{
				m_jindanState = JinDan_Image_4;
			}
			else if(m_jindanState == JinDan_Image_4)
			{
				m_jindanState = JinDan_Image_5;
				KillTimer(TIMER_EVENT_JINDAN);
				m_bZhuiZi = false;
				InvalidateRect(&crc_yindan);
				InvalidateRect(&crc_tongdan);
			}
		}

		InvalidateRect(&crc_jindan);

		if(m_jindanState == JinDan_Image_5)
		{
			Getjiangjin(ZJD_JINDAN);
		}
	}
	else if (TIMER_EVENT_YINDAN == nIDEvent)
	{
		if (!m_yindan_win)
		{
			if(m_yindanState == YinDan_Image_1)
			{
				if (m_yindanChuiziState == ChuiZi_Image_Up)
				{
					m_yindanChuiziState = ChuiZi_Image_Mid;
				}
				else if (m_yindanChuiziState == ChuiZi_Image_Mid)
				{
					m_yindanChuiziState = ChuiZi_Image_Down;
				}
				else if (m_yindanChuiziState == ChuiZi_Image_Down)
				{
					m_yindanState = YinDan_Image_6;
				}
				KillTimer(TIMER_EVENT_YINDAN);
				m_bZhuiZi = false;
				InvalidateRect(&crc_jindan);
				InvalidateRect(&crc_tongdan);
			}
		}
		else 
		{
			if(m_yindanState == YinDan_Image_1)
			{
				if (m_yindanChuiziState == ChuiZi_Image_Up)
				{
					m_yindanChuiziState = ChuiZi_Image_Mid;
				}
				else if (m_yindanChuiziState == ChuiZi_Image_Mid)
				{
					m_yindanChuiziState = ChuiZi_Image_Down;
				}
				else if (m_yindanChuiziState == ChuiZi_Image_Down)
				{
					m_yindanState = YinDan_Image_2;
				}
			}
			else if(m_yindanState == YinDan_Image_2)
			{
				m_yindanState = YinDan_Image_3;
			}
			else if(m_yindanState == YinDan_Image_3)
			{
				m_yindanState = YinDan_Image_4;
			}
			else if(m_yindanState == YinDan_Image_4)
			{
				m_yindanState = YinDan_Image_5;
				KillTimer(TIMER_EVENT_YINDAN);
				m_bZhuiZi = false;
				InvalidateRect(&crc_jindan);
				InvalidateRect(&crc_tongdan);
			}
		}

		InvalidateRect(&crc_yindan);

		if(m_yindanState == YinDan_Image_5)
		{
			Getjiangjin(ZJD_YINDAN);
		}
	}
	else if (TIMER_EVENT_TONGDAN == nIDEvent)
	{
		if(!m_tongdan_win)
		{
			if(m_tongdanState == TongDan_Image_1)
			{
				if (m_tongdanChuiziState == ChuiZi_Image_Up)
				{
					m_tongdanChuiziState = ChuiZi_Image_Mid;
				}
				else if (m_tongdanChuiziState == ChuiZi_Image_Mid)
				{
					m_tongdanChuiziState = ChuiZi_Image_Down;
				}
				else if (m_tongdanChuiziState == ChuiZi_Image_Down)
				{
					m_tongdanState = TongDan_Image_6;
				}
				KillTimer(TIMER_EVENT_TONGDAN);
				m_bZhuiZi = false;
				InvalidateRect(&crc_jindan);
				InvalidateRect(&crc_yindan);
			}
		}
		else
		{
			if(m_tongdanState == TongDan_Image_1)
			{
				if (m_tongdanChuiziState == ChuiZi_Image_Up)
				{
					m_tongdanChuiziState = ChuiZi_Image_Mid;
				}
				else if (m_tongdanChuiziState == ChuiZi_Image_Mid)
				{
					m_tongdanChuiziState = ChuiZi_Image_Down;
				}
				else if (m_tongdanChuiziState == ChuiZi_Image_Down)
				{
					m_tongdanState = TongDan_Image_2;
				}
			}
			else if(m_tongdanState == TongDan_Image_2)
			{
				m_tongdanState = TongDan_Image_3;
			}
			else if(m_tongdanState == TongDan_Image_3)
			{
				m_tongdanState = TongDan_Image_4;
			}
			else if(m_tongdanState == TongDan_Image_4)
			{
				m_tongdanState = TongDan_Image_5;
				KillTimer(TIMER_EVENT_TONGDAN);
				m_bZhuiZi = false;
				InvalidateRect(&crc_jindan);
				InvalidateRect(&crc_yindan);
			}
		}

		InvalidateRect(&crc_tongdan);

		if (m_tongdanState == TongDan_Image_5)
		{
			Getjiangjin(ZJD_TONGDAN);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

//判断是否能够参加活动
void CHuoDongZaJinDanDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		if(theAccount.user_id <= 0)
			return;
		//m_MissionManager.InsertMissionItem(this);

		//SendToServer(3);
		m_bZhuiZi = !m_got;
	}
	else
	{
		if(GetMissionManager()!=NULL)
			GetMissionManager()->DeleteMissionItem(this);		
	}

}

void CHuoDongZaJinDanDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
// 	if (rc_hit_yindan.PtInRect(point))
// 	{
// 		if (m_got || m_jinrixiaofei < 5000.0 || m_yindanChuiziState != ChuiZi_Image_Up)
// 		{
// 			//MessageBox(_T("您今天已经砸过了"));
// 		}
// 		else
// 		{
// 			m_got = true;
// 
// 			TCHAR wav_path[MAX_PATH] = _T("");
// 			m_yindan_win = true;
// 			CBmpUtil::GetExePath(_T("skin\\hd\\zjd_win.wav"), wav_path);
// 			::PlaySound(wav_path, NULL, SND_ASYNC);
// 
// 			SetTimer(TIMER_EVENT_YINDAN, timer_elapse, NULL);
// 		}
// 	}
// 	else if (rc_hit_jindan.PtInRect(point))
// 	{
// 		if (m_got || m_jinrixiaofei < 10000.0 || m_jindanChuiziState != ChuiZi_Image_Up)
// 		{
// 			//MessageBox(_T("您今天已经砸过了"));
// 		}
// 		else
// 		{
// 			m_got = true;
// 
// 			TCHAR wav_path[MAX_PATH] = _T("");
// 			m_jindan_win = true;
// 			CBmpUtil::GetExePath(_T("skin\\hd\\zjd_win.wav"), wav_path);
// 			::PlaySound(wav_path, NULL, SND_ASYNC);
// 
// 			SetTimer(TIMER_EVENT_JINDAN, timer_elapse, NULL);
// 		}
// 	}
// 	else if (rc_hit_tongdan.PtInRect(point))
// 	{
// 		if (m_got || m_jinrixiaofei < 2000.0 || m_tongdanChuiziState != ChuiZi_Image_Up)
// 		{
// 			//MessageBox(_T("您今天已经砸过了"));
// 		}
// 		else
// 		{
// 			m_got = true;
// 
// 			TCHAR wav_path[MAX_PATH] = _T("");
// 			m_tongdan_win = true;
// 			CBmpUtil::GetExePath(_T("skin\\hd\\zjd_win.wav"), wav_path);
// 			::PlaySound(wav_path, NULL, SND_ASYNC);
// 
// 			SetTimer(TIMER_EVENT_TONGDAN, timer_elapse, NULL);
// 		}
// 	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CHuoDongZaJinDanDlg::DrawJinDanImage(Graphics& graphics)
{
	Bitmap* bmp = m_bmpJinDan[m_jindanState];
	graphics.DrawImage(bmp, rc_jindan, 0, 0, bmp->GetWidth(), bmp->GetHeight(), UnitPixel);

	if (m_jindanState == JinDan_Image_1 && m_jinrixiaofei >= 10000.0 && m_bZhuiZi)
	{
		bmp = m_bmpChuiZi[m_jindanChuiziState];
		graphics.DrawImage(bmp, rc_jindan_chuizi, 0, 0, bmp->GetWidth(), bmp->GetHeight(), UnitPixel);
	}
}

void CHuoDongZaJinDanDlg::DrawYinDanImage(Graphics& graphics)
{
	Bitmap* bmp = m_bmpYinDan[m_yindanState];
	graphics.DrawImage(bmp, rc_yindan, 0, 0, bmp->GetWidth(), bmp->GetHeight(), UnitPixel);

	if (m_yindanState == YinDan_Image_1 && m_jinrixiaofei >= 5000.0 && m_bZhuiZi)
	{
		bmp = m_bmpChuiZi[m_yindanChuiziState];
		graphics.DrawImage(bmp, rc_yindan_chuizi, 0, 0, bmp->GetWidth(), bmp->GetHeight(), UnitPixel);
	}
}

void CHuoDongZaJinDanDlg::DrawTongDanImage(Graphics& graphics)
{
	Bitmap* bmp = m_bmpTongDan[m_tongdanState];
	graphics.DrawImage(bmp, rc_tongdan, 0, 0, bmp->GetWidth(), bmp->GetHeight(), UnitPixel);

	if (m_tongdanState == TongDan_Image_1 && m_jinrixiaofei >= 2000.0 && m_bZhuiZi)
	{
		bmp = m_bmpChuiZi[m_tongdanChuiziState];
		graphics.DrawImage(bmp, rc_tongdan_chuizi, 0, 0, bmp->GetWidth(), bmp->GetHeight(), UnitPixel);
	}
}

void CHuoDongZaJinDanDlg::Getjiangjin(int dantupe)
{
	m_cbJindanType = dantupe;
	int jine = 0;
//	SendToServer(1);
}

//连接事件
bool CHuoDongZaJinDanDlg::OnEventMissionLink(INT nErrorCode,INT nSocketID)
{
	return true;
}

//关闭事件
bool CHuoDongZaJinDanDlg::OnEventMissionShut(BYTE cbShutReason)
{
	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{

	}

	return true;
}

//读取事件
bool CHuoDongZaJinDanDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_ZA_JIN_DAN_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetZaJinDanRet));
				if(wDataSize!=sizeof(CMD_GP_GetZaJinDanRet)) return false;

				CMD_GP_GetZaJinDanRet* pCountRet = (CMD_GP_GetZaJinDanRet*)pData;

				CString str;
				str.Format(_T("恭喜获得%d元奖金"), pCountRet->f_t_jine);
				MyMessageBox(str);
				//SendToServer(2);

				break;
			}
		case SUB_GP_GET_LAST_YUE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetLastYueRet));
				if(wDataSize!=sizeof(CMD_GP_GetLastYueRet)) return false;

				CMD_GP_GetLastYueRet* pCountRet = (CMD_GP_GetLastYueRet*)pData;

				theAccount.yue = pCountRet->f_t_yue;
				theAccount.dongjie = pCountRet->f_t_dongjie;
// 				InvalidateRect(rc_jinrixiaofei);
// 				InvalidateRect(rc_zhanghuyue);
				break;
			}

		case SUB_GP_CAN_ZA_JIN_DAN_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_CanZaJinDanRet));
				if(wDataSize!=sizeof(CMD_GP_CanZaJinDanRet)) return false;

				CMD_GP_CanZaJinDanRet* pCountRet = (CMD_GP_CanZaJinDanRet*)pData;
				m_jinrixiaofei = pCountRet->f_t_xiaofei;
				m_got = pCountRet->n_t_got;
				m_bZhuiZi = !m_got;
				break;

			}

		}
	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CHuoDongZaJinDanDlg::SendToServer(int nSendType)
{
	return ;

}