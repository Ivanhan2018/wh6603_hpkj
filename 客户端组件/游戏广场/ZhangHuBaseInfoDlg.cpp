#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhangHuBaseInfoDlg.h"
#include "BmpUtil.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

static const int tip_width = 100;
static const int tip_height = 16;

//上次IP
static const int last_ip_x = 160;
static const int last_ip_y = 40;
static CRect rc_last_ip(last_ip_x, last_ip_y, last_ip_x+tip_width, last_ip_y+tip_height);

//上次时间
static const int last_time_x = 460;
static const int last_time_y = 40;
static CRect rc_last_time(last_time_x, last_time_y, last_time_x+tip_width, last_time_y+tip_height);

//此次IP
static const int currect_ip_x = 135;
static const int currect_ip_y = 62;
static CRect rc_currect_ip(currect_ip_x, currect_ip_y, currect_ip_x+tip_width, currect_ip_y+tip_height);

//账户信息
static const int account_x = 188;
static const int account_y = 122;
static CRect rc_account(account_x, account_y, account_x+tip_width, account_y+tip_height);

//ID号
static const int id_x = 188;
static const int id_y = 153;
static CRect rc_id(id_x, id_y, id_x+tip_width, id_y+tip_height);

//返点
static const int fandian_x = 188;
static const int fandian_y = 183;
static CRect rc_fandian(fandian_x, fandian_y, fandian_x+tip_width, fandian_y+tip_height);

//个人QQ
static const int qq_x = 188;
static const int qq_y = 212;
static CRect rc_qq(qq_x, qq_y, qq_x+tip_width, qq_y+tip_height);

static const int edit_qq_x = 188;
static const int edit_qq_y = 212;
static const int edit_qq_width = 120;
static const int edit_qq_height = 20;

static const int btn_xgqq_x = 337;
static const int btn_xgqq_y = 208;

//上级QQ
static const int shangji_x = 188;
static const int shangji_y = 289;
static CRect rc_shangji(shangji_x, shangji_y, shangji_x+tip_width, shangji_y+tip_height);
//注册时间
static const int reg_time_x = 188;
static const int reg_time_y = 319;
static CRect rc_regtime(reg_time_x, reg_time_y, reg_time_x+tip_width, reg_time_y+tip_height);

//棋牌余额
static const int qipai_x = 188;
static const int qipai_y = 275;
static CRect rc_qipai(qipai_x, qipai_y, qipai_x+tip_width, qipai_y+tip_height);

//佣金时间
static const int yongjinsj_x = 188;
static const int yongjinsj_y = 349;
static CRect rc_yongjin_time(yongjinsj_x, yongjinsj_y, yongjinsj_x+tip_width+200, yongjinsj_y+tip_height);

//佣金
static const int yongjin_x = 190;
static const int yongjin_y = 341;
static CRect rc_yongjin(yongjin_x, yongjin_y, yongjin_x+tip_width+200, yongjin_y+tip_height);

//佣金总额
static const int yongjinze_x = 390;
static const int yongjinze_y = 341;
static CRect rc_yongjinze(yongjinze_x, yongjinze_y, yongjinze_x+tip_width+200, yongjinze_y+tip_height);

//分红时间
static const int kuisunyongjinsj_x = 90;
static const int kuisunyongjinsj_y = 371;
static CRect rc_kuisunyongjin_time(kuisunyongjinsj_x, kuisunyongjinsj_y, kuisunyongjinsj_x+tip_width+200, kuisunyongjinsj_y+tip_height);

//佣金
static const int kuisunyongjin_x = 190;
static const int kuisunyongjin_y = 371;
static CRect rc_kuisunyongjin(kuisunyongjin_x, kuisunyongjin_y, kuisunyongjin_x+tip_width+200, kuisunyongjin_y+tip_height);

//佣金总额
static const int kuisunyongjinze_x = 390;
static const int kuisunyongjinze_y = 371;
static CRect rc_kuisunyongjinze(kuisunyongjinze_x, kuisunyongjinze_y, kuisunyongjinze_x+tip_width+200, kuisunyongjinze_y+tip_height);

//分红时间
static const int fenhongsj_x = 90;
static const int fenhongsj_y = 401;
static CRect rc_fenhong_time(fenhongsj_x, fenhongsj_y, fenhongsj_x+tip_width+200, fenhongsj_y+tip_height);

//佣金
static const int fenhong_x = 190;
static const int fenhong_y = 401;
static CRect rc_fenhong(fenhong_x, fenhong_y, fenhong_x+tip_width+200, fenhong_y+tip_height);

//佣金总额
static const int fenhongze_x = 390;
static const int fenhongze_y = 401;
static CRect rc_fenhongze(fenhongze_x, fenhongze_y, fenhongze_x+tip_width+200, fenhongze_y+tip_height);

//投注总额
static const int touzhuze_x = 205;
static const int touzhuze_y = 365;
static CRect rc_touzhuze(touzhuze_x, touzhuze_y, touzhuze_x+tip_width, touzhuze_y+tip_height);

//充值总额
static const int chongzhize_x = 487;
static const int chongzhize_y = 365;
static CRect rc_chongzhize(chongzhize_x, chongzhize_y, chongzhize_x+tip_width, chongzhize_y+tip_height);

//中奖总额
static const int zhongjiangze_x = 205;
static const int zhongjiangze_y = 395;
static CRect rc_zhongjiangze(zhongjiangze_x, zhongjiangze_y, zhongjiangze_x+tip_width, zhongjiangze_y+tip_height);

//体现总额
static const int tixianze_x = 487;
static const int tixianze_y = 400;
static CRect rc_tixianze(tixianze_x, tixianze_y, tixianze_x+tip_width, tixianze_y+tip_height);

//投注返点总额
static const int fandianze_x = 205;
static const int fandianze_y = 430;
static CRect rc_fandianze(fandianze_x, fandianze_y, fandianze_x+tip_width, fandianze_y+tip_height);

//盈亏总额
static const int yingkuize_x = 487;
static const int yingkuize_y = 429;
static CRect rc_yingkuize(yingkuize_x, yingkuize_y, yingkuize_x+tip_width, yingkuize_y+tip_height);

IMPLEMENT_DYNAMIC(CZhangHuBaseInfoDlg, CDialog)

CZhangHuBaseInfoDlg::CZhangHuBaseInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZhangHuBaseInfoDlg::IDD, pParent)
	, m_bmpBk(NULL)
{
	memset(&m_userinfo, 0, sizeof(m_userinfo));
	m_bGetUserInfo=false;
	m_bXgUserQQ=false;
}

CZhangHuBaseInfoDlg::~CZhangHuBaseInfoDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
		m_bmpBk = NULL;
	}
}

void CZhangHuBaseInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_QQ, m_editQQ);
	DDX_Control(pDX, IDC_BTN_XGQQ, m_btnXgqq);
	DDX_Control(pDX, IDC_RIO_MAN, m_ImgRioMan);
	DDX_Control(pDX, IDC_RIO_WOMAN, m_ImgRioWoman);
	DDX_Control(pDX, IDC_BTN_XGGENDER, m_btnXggender);
}

BEGIN_MESSAGE_MAP(CZhangHuBaseInfoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_XGQQ, &CZhangHuBaseInfoDlg::OnBnClickedBtnXgqq)
	ON_BN_CLICKED(IDC_BTN_XGGENDER, &CZhangHuBaseInfoDlg::OnBnClickedBtnXgGender)
	ON_BN_CLICKED(IDC_RIO_MAN, &CZhangHuBaseInfoDlg::OnBnClickedRioBtnMan)
	ON_BN_CLICKED(IDC_RIO_WOMAN, &CZhangHuBaseInfoDlg::OnBnClickedRioBtnWoman)
END_MESSAGE_MAP()


// CZhangHuBaseInfo 消息处理程序

BOOL CZhangHuBaseInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\wdzh_bg.png"));
	m_btnXgqq.SetImage(CBmpUtil::GetExePath() + _T("skin\\shezhi.png"));
	m_btnXggender.SetImage(CBmpUtil::GetExePath() + _T("skin\\shezhi.png"));

	m_font.CreateFont(19, 0, 0, 0,FW_NORMAL , 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	m_editQQ.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(58,27,20));
	m_dwTickCount=0;

// 	m_ImgRioMan.SetPaintTextColor(RGB(88,78,77));
// 	m_ImgRioWoman.SetPaintTextColor(RGB(88,78,77));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CZhangHuBaseInfoDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CZhangHuBaseInfoDlg::OnOK()
{
	//CDialog::OnOK();
}

void CZhangHuBaseInfoDlg::OnPaint()
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

	graphics.DrawImage(m_bmpBk, Rect(0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));

	CString strTmp;
	cacheDC.DrawText(m_userinfo.last_ip, rc_last_ip, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_userinfo.last_time, rc_last_time, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_userinfo.current_ip, rc_currect_ip, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.DrawText(m_userinfo.account, rc_account, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%d"), m_userinfo.user_id);
	cacheDC.DrawText(strTmp, rc_id, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.2lf"), m_userinfo.fandian * 100);
	cacheDC.DrawText(strTmp + _T("%"), rc_fandian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	//cacheDC.DrawText(m_userinfo.qq, rc_qq, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_userinfo.shangji_qq, rc_shangji, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strTmp.Format(_T("%d-%02d-%02d"), m_userinfo.t_reg_time.wYear, m_userinfo.t_reg_time.wMonth, m_userinfo.t_reg_time.wDay );
	cacheDC.DrawText(strTmp, rc_regtime, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strTmp.Format(_T("%.2lf"), m_userinfo.fandian * 100*20 );
	cacheDC.DrawText(strTmp+ _T("%"), rc_yongjin_time, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 
// 	if(theAccount.type == 1)
// 	{
// 		strTmp.Format(_T("%d-%02d-%02d"), m_userinfo.t_yj_time.wYear, m_userinfo.t_yj_time.wMonth, m_userinfo.t_yj_time.wDay );
// 		cacheDC.DrawText(strTmp, rc_yongjin_time, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 
// 		strTmp.Format(_T("消费佣金：%.2lf元"), m_userinfo.yongjin );
// 		cacheDC.DrawText(strTmp, rc_yongjin, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 
// 		strTmp.Format(_T("消费佣金总额：%.2lf元"), m_userinfo.yongjin_ze );
// 		cacheDC.DrawText(strTmp, rc_yongjinze, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 
// 		strTmp.Format(_T("%d-%02d-%02d"), m_userinfo.t_ksyj_time.wYear, m_userinfo.t_ksyj_time.wMonth, m_userinfo.t_ksyj_time.wDay );
// 		cacheDC.DrawText(strTmp, rc_kuisunyongjin_time, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 
// 		strTmp.Format(_T("盈亏佣金：%.2lf元"), m_userinfo.kuisunyongjin);
// 		cacheDC.DrawText(strTmp, rc_kuisunyongjin, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 
// 		strTmp.Format(_T("盈亏佣金总额：%.2lf元"), m_userinfo.kuisunyongjin_ze );
// 		cacheDC.DrawText(strTmp, rc_kuisunyongjinze, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 
// 
// 	}


	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CZhangHuBaseInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CZhangHuBaseInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL) {
		return;
	}

	if (m_editQQ.GetSafeHwnd() != NULL)
	{
		m_editQQ.SetWindowPos(NULL, edit_qq_x+2, edit_qq_y+4, edit_qq_width-6, edit_qq_height, SWP_NOZORDER);
	}

	if (m_btnXgqq.GetSafeHwnd() != NULL)
	{
		m_btnXgqq.SetWindowPos(NULL, btn_xgqq_x, btn_xgqq_y, m_btnXgqq.Width(), m_btnXgqq.Height(), SWP_NOZORDER);
	}

	if (m_ImgRioMan.GetSafeHwnd() != NULL)
	{
		m_ImgRioMan.SetWindowPos(NULL, edit_qq_x, edit_qq_y + edit_qq_height+22,  m_ImgRioMan.Width(), m_ImgRioMan.Height(), SWP_NOZORDER);
	}
	if (m_ImgRioWoman.GetSafeHwnd() != NULL)
	{
		m_ImgRioWoman.SetWindowPos(NULL, edit_qq_x+edit_qq_width-85, edit_qq_y + edit_qq_height+22, m_ImgRioWoman.Width(), m_ImgRioWoman.Height(), SWP_NOZORDER);
	}
	if (m_btnXggender.GetSafeHwnd() != NULL)
	{
		m_btnXggender.SetWindowPos(NULL, btn_xgqq_x, btn_xgqq_y+40, m_btnXggender.Width()+40, m_btnXggender.Height()+20, SWP_NOZORDER);
	}
}

void CZhangHuBaseInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow) 
	{
		if(theAccount.user_id <=0)
			return;

		m_userinfo.user_id = theAccount.user_id;
		_tcscpy_s(m_userinfo.account, ACCOUNT_LENGTH, theAccount.account);
		m_userinfo.fandian = theAccount.fandian;

		m_ImgRioWoman.SetPushed(FALSE);
		m_ImgRioMan.SetPushed(FALSE);
		if(theAccount.cbgender == 1)
			m_ImgRioWoman.SetPushed(TRUE);
		else
			m_ImgRioMan.SetPushed(TRUE);

		SendToServer(1);//获取用户信息
	}

}
void CZhangHuBaseInfoDlg::OnBnClickedRioBtnMan()
{
	m_ImgRioMan.SetPushed(true);
	m_ImgRioWoman.SetPushed(false);

	return;
}
void CZhangHuBaseInfoDlg::OnBnClickedRioBtnWoman()
{
	m_ImgRioMan.SetPushed(false);
	m_ImgRioWoman.SetPushed(true);

	return;
}
void CZhangHuBaseInfoDlg::OnBnClickedBtnXgGender()
{
	m_cbGender = m_ImgRioMan.GetChecked()?0:1;

	if(m_cbGender == theAccount.cbgender)
	{
		CString strMessage;
		strMessage.Format(L"你当前性别已为%s",(m_cbGender == 0)?L"男":L"女");
		MyMessageBox(strMessage);
		return;
	}
	SendToServer(3);
}

void CZhangHuBaseInfoDlg::OnBnClickedBtnXgqq()
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

	CString new_qq;
	m_editQQ.GetWindowText(new_qq);

	new_qq = new_qq.Trim();
	if (new_qq.IsEmpty())
	{
		MyMessageBox(_T("QQ不能为空"));
		return;
	}

	if (new_qq != m_userinfo.qq)
	{
		SendToServer(2);
	}
}

//读取事件
bool CZhangHuBaseInfoDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_GET_USER_INFO_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_GetUserInfoRet));
				if(wDataSize!=sizeof(CMD_GR_GetUserInfoRet))
					return false;

				CMD_GR_GetUserInfoRet* pCountRet = (CMD_GR_GetUserInfoRet*)pData;

				_tcscpy_s(m_userinfo.qq, QQ_LENGTH, pCountRet->s_t_qq);
				_tcscpy_s(m_userinfo.shangji_qq, QQ_LENGTH, pCountRet->s_t_sjqq);
				m_userinfo.t_reg_time = pCountRet->t_reg_time;
				m_userinfo.yongjin = fabs(pCountRet->f_t_yongjin);
				m_userinfo.yongjin_ze = fabs(pCountRet->f_t_yongjinze);
				m_userinfo.fenhong = fabs(pCountRet->f_t_fenhong);
				m_userinfo.fenhong_ze = fabs(pCountRet->f_t_fenhongze);
				m_userinfo.qipai_ye = fabs(pCountRet->f_t_qipaiye);
				m_userinfo.t_fh_time = pCountRet->t_fh_time;
				m_userinfo.t_yj_time = pCountRet->t_yj_time;
				m_userinfo.kuisunyongjin = fabs(pCountRet->f_t_kuisunyongjin);
				m_userinfo.kuisunyongjin_ze = fabs(pCountRet->f_t_kuisunyongjinze);
				m_userinfo.t_ksyj_time = pCountRet->t_ksyj_time;
				m_editQQ.SetWindowText(m_userinfo.qq);
				Invalidate();


				return true;
			}
		case SUB_GR_XG_QQ_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_XG_QQ_RET));
				if(wDataSize!=sizeof(CMD_GR_XG_QQ_RET))
					return false;

				CMD_GR_XG_QQ_RET* pXGqqRet = (CMD_GR_XG_QQ_RET*)pData;
				
				LONG lResult = pXGqqRet->lResult;
				if(lResult == 0)
				{
					MyMessageBox(L"设置QQ成功");
				}
				else
				{
					MyMessageBox(L"修改失败");			
				}
				return true;
			}
		case SUB_GP_ALTER_GENDER_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_AlterGenderRet));
				if(wDataSize!=sizeof(CMD_GP_AlterGenderRet))
					return false;

				CMD_GP_AlterGenderRet* pAlterGenderRet = (CMD_GP_AlterGenderRet*)pData;
				int nResult = pAlterGenderRet->n_t_return;

				if(nResult == 0)
				{
					theAccount.cbgender = m_cbGender;
					CWnd *pParent = GetParent();
					if(pParent!=NULL)
					{
						pParent->SendMessage(RELEASE_FACE,1,0);
					}
					MyMessageBox(L"性别设置成功！");
				}
				else
				{
					MyMessageBox(L"性别设置失败！");
				}
				return true;

			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CZhangHuBaseInfoDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //获取用户信息
	{
		m_bGetUserInfo = true;
		if(m_bGetUserInfo)
		{
			m_bGetUserInfo = false;
			CMD_GP_GetUserInfo GetUserInfo;
			ZeroMemory(&GetUserInfo,sizeof(GetUserInfo));

			GetUserInfo.dwUserID = theAccount.user_id;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_USER_INFO,&GetUserInfo,sizeof(GetUserInfo));
			return;
		}
	}

	if(nSendType == 2)//修改QQ 
	{
		m_bXgUserQQ = true;

		if(m_bXgUserQQ)
		{
			m_bXgUserQQ = false;

			CMD_GP_XGqq XGqq;
			ZeroMemory(&XGqq,sizeof(XGqq));
			XGqq.dwUserID = theAccount.user_id;

			CString new_qq;
			m_editQQ.GetWindowText(new_qq);

			new_qq = new_qq.Trim();
			if (new_qq.IsEmpty())
			{
				MyMessageBox(_T("QQ不能为空"));
				return ;
			}
			lstrcpyn(XGqq.szQQ,new_qq.GetBuffer(),sizeof(XGqq.szQQ));
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_XG_QQ,&XGqq,sizeof(XGqq));

			return;
		}	
	}

	if(nSendType == 3)
	{
		CMD_GP_AlterGender AlterGender;
		ZeroMemory(&AlterGender,sizeof(AlterGender));

		AlterGender.n_t_userid = theAccount.user_id;
		AlterGender.cbGender = m_cbGender;

		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_ALTER_GENDER,&AlterGender,sizeof(AlterGender));
		return;
	}

	return;

}