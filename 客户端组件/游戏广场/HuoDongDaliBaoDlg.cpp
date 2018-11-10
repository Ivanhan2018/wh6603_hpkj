#include "stdafx.h"
#include "Gameplaza.h"
#include "HuoDongDaliBaoDlg.h"
#include "MessageDlg.h"

const static int jinrixiaofei_x = 215+18;
const static int jinrixiaofei_y = 149-12;
const static int jinrixiaofei_width = 110;
const static int jinrixiaofei_height = 20;
CRect rc_jinrixiaofei(jinrixiaofei_x,
					  jinrixiaofei_y,
					  jinrixiaofei_x + jinrixiaofei_width,
					  jinrixiaofei_y + jinrixiaofei_height);

const static int zhanghuyue_x = 148+11;
const static int zhanghuyue_y = 202-15;
const static int zhanghuyue_width = 110;
const static int zhanghuyue_height = 20;
CRect rc_zhanghuyue(zhanghuyue_x,
					zhanghuyue_y,
					zhanghuyue_x + zhanghuyue_width,
					zhanghuyue_y + zhanghuyue_height);

const static int btn_lingjiang_x = 280;
const static int btn_lingjiang_y = 230;

// CHuoDongDaliBao 对话框

IMPLEMENT_DYNAMIC(CHuoDongDaliBaoDlg, CDialog)

CHuoDongDaliBaoDlg::CHuoDongDaliBaoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuoDongDaliBaoDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_jinrixiaofei(0.0)
	, m_jine(0)
{
	m_bGetLastYue=false;
	m_bGetDaLiBao=false;
	m_bCanDaLiBao=false;
}

CHuoDongDaliBaoDlg::~CHuoDongDaliBaoDlg()
{	
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
	}
	m_font.DeleteObject();
}

void CHuoDongDaliBaoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINHJIANG, m_btnLingJiang);
}


BEGIN_MESSAGE_MAP(CHuoDongDaliBaoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_LINHJIANG, &CHuoDongDaliBaoDlg::OnBnClickedLingJiang)
END_MESSAGE_MAP()


// CHuoDongDaliBao 消息处理程序
BOOL CHuoDongDaliBaoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\hd\\hd_hls.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_btnLingJiang.SetImage(CBmpUtil::GetExePath() + _T("skin\\hd\\btn_chongzhilingjiang.png"));

	m_btnLingJiang.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHuoDongDaliBaoDlg::OnPaint()
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

// 	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
// 	CFont* pOldFont = cacheDC.SelectObject(&m_font);
// 	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(255, 0, 0));
// 
// 	CString dText;
// 	dText.Format(_T("%.2lf元"), m_jinrixiaofei);
// 	cacheDC.DrawText(dText, rc_jinrixiaofei, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 	dText.Format(_T("%.2lf元"), theAccount.yue);
// 	cacheDC.DrawText(dText, rc_zhanghuyue, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 	cacheDC.SetTextColor(oldTextColor);
// 	cacheDC.SelectObject(pOldFont);
// 	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

void CHuoDongDaliBaoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetSafeHwnd() == NULL)
	{
		return;
	}

	if (m_btnLingJiang.GetSafeHwnd() != NULL)
	{
		m_btnLingJiang.SetWindowPos(NULL, btn_lingjiang_x, btn_lingjiang_y, m_btnLingJiang.Width(), m_btnLingJiang.Height(), SWP_NOZORDER);
	}
}

void CHuoDongDaliBaoDlg::OnBnClickedLingJiang()
{
	m_btnLingJiang.EnableWindow(FALSE);
	SendToServer(1);
	//a'long 暂时屏蔽
// 	if (!theDBCmd.GetDaLiBao(theAccount.user_id, m_jine)) 
// 	{
// 		MyMessageBox(_T("系统错误，请稍后再试"));
// 	} else {
// 		CString str;
// 		str.Format(_T("恭喜获得%d元奖金"), m_jine);
// 		MyMessageBox(str);
// 
// 		theDBCmd.GetLastYue(theAccount.user_id, theAccount.yue, theAccount.dongjie);
// 		InvalidateRect(rc_jinrixiaofei);
// 		InvalidateRect(rc_zhanghuyue);
// 	}
}
//判断是否能够参加活动
void CHuoDongDaliBaoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		if(theAccount.user_id <= 0)
			return;
	//	m_MissionManager.InsertMissionItem(this);

		SendToServer(3);
	}
	else
	{
		if(GetMissionManager()!=NULL)
			GetMissionManager()->DeleteMissionItem(this);		
	}
}

//连接事件
bool CHuoDongDaliBaoDlg::OnEventMissionLink(INT nErrorCode,INT nSocketID)
{
	return TRUE;


}

//关闭事件
bool CHuoDongDaliBaoDlg::OnEventMissionShut(BYTE cbShutReason)
{
	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
// 		CMissionManager * pMissionManager=GetMissionManager();
// 		if (pMissionManager->AvtiveMissionItem(this,true)==true) return true;

	}

	return true;
}

//读取事件
bool CHuoDongDaliBaoDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_DA_LI_BAO_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetDaLiBaoRet));
				if(wDataSize!=sizeof(CMD_GP_GetDaLiBaoRet)) return false;

				CMD_GP_GetDaLiBaoRet* pCountRet = (CMD_GP_GetDaLiBaoRet*)pData;

				CString str;
				str.Format(_T("恭喜获得%d元奖金"), pCountRet->f_t_jine);
				MyMessageBox(str);
				SendToServer(2);
// 				theDBCmd.GetLastYue(theAccount.user_id, theAccount.yue, theAccount.dongjie);

				break;
			}
		case SUB_GP_GET_LAST_YUE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_GetLastYueRet));
				if(wDataSize!=sizeof(CMD_GP_GetLastYueRet)) return false;

				CMD_GP_GetLastYueRet* pCountRet = (CMD_GP_GetLastYueRet*)pData;

				theAccount.yue = pCountRet->f_t_yue;
				theAccount.dongjie = pCountRet->f_t_dongjie;
 				InvalidateRect(rc_jinrixiaofei);
 				InvalidateRect(rc_zhanghuyue);
				break;
			}

		case SUB_GP_CAN_DA_LI_BAO_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_CanDaLiBaoRet));
				if(wDataSize!=sizeof(CMD_GP_CanDaLiBaoRet)) return false;

				CMD_GP_CanDaLiBaoRet* pCountRet = (CMD_GP_CanDaLiBaoRet*)pData;

				m_jinrixiaofei = pCountRet->f_t_xiaofei;
				theAccount.yue = pCountRet->f_t_yue;
				if (pCountRet->f_t_xiaofei >=5000.0 && pCountRet->f_t_chongzhi >= 1000.0 && pCountRet->n_t_got==0) {
					m_btnLingJiang.EnableWindow(TRUE);
				} else {
					m_btnLingJiang.EnableWindow(FALSE);
				}

			}

		}
	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CHuoDongDaliBaoDlg::SendToServer(int nSendType)
{
	return;

}