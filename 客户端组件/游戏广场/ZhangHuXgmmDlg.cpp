#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhangHuXgmmDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

static const int edit_width = 164;
static const int edit_height = 20;

//账户1
static const int dl_account_x = 301;
static const int dl_account_y = 132;
static CRect rc_dl_account(dl_account_x, dl_account_y, dl_account_x+edit_width, dl_account_y+edit_height);

//旧登陆密码
static const int dl_old_pwd_x = 281;
static const int dl_old_pwd_y = 167;
//新登陆密码
static const int dl_new_pwd_x = 281;
static const int dl_new_pwd_y = 202;
//确认新登陆密码
static const int dl_cfm_pwd_x = 281;
static const int dl_cfm_pwd_y = 238;
//确定按钮
static const int dl_ok_x = 297;
static const int dl_ok_y = 273;


//账户1
static const int qk_account_x = 658;
static const int qk_account_y = 132;
static CRect rc_qk_account(qk_account_x, qk_account_y, qk_account_x+edit_width, qk_account_y+edit_height);
//旧取款密码
static const int qk_old_pwd_x = 638;
static const int qk_old_pwd_y = 167;
//新取款密码
static const int qk_new_pwd_x = 638;
static const int qk_new_pwd_y = 202;
//确认新取款密码
static const int qk_cfm_pwd_x = 638;
static const int qk_cfm_pwd_y = 237;
//确定按钮
static const int qk_ok_x = 660;
static const int qk_ok_y = 273;


IMPLEMENT_DYNAMIC(CZhangHuXgmmDlg, CDialog)

CZhangHuXgmmDlg::CZhangHuXgmmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZhangHuXgmmDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_old_pwd(_T(""))
	, m_new_pwd(_T(""))
	, m_confirm_pwd(_T(""))
{
	m_bAlterLogonPass=false;
	m_bAlterQukuanPass=false;
}

CZhangHuXgmmDlg::~CZhangHuXgmmDlg()
{
	if(m_bmpBk != NULL)
	{
		delete m_bmpBk;
		m_bmpBk = NULL;
	}
}

void CZhangHuXgmmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OLD_PWD, m_old_pwd);
	DDX_Text(pDX, IDC_EDIT_NEW_PWD, m_new_pwd);
	DDX_Text(pDX, IDC_EDIT_CONFIRM, m_confirm_pwd);
	DDX_Control(pDX, IDC_EDIT_OLD_PWD, m_editOldPwd);
	DDX_Control(pDX, IDC_EDIT_NEW_PWD, m_editNewPwd);
	DDX_Control(pDX, IDC_EDIT_CONFIRM, m_editConfirmPwd);


	DDX_Control(pDX, IDC_BTN_OK, m_btnDlOK);
	DDX_Text(pDX, IDC_EDIT_OLD_PWD2, m_old_pwd_qk);
	DDX_Text(pDX, IDC_EDIT_NEW_PWD2, m_new_pwd_qk);
	DDX_Text(pDX, IDC_EDIT_CONFIRM2, m_confirm_pwd_qk);
	DDX_Control(pDX, IDC_EDIT_OLD_PWD2, m_editOldPwdQk);
	DDX_Control(pDX, IDC_EDIT_NEW_PWD2, m_editNewPwdQk);
	DDX_Control(pDX, IDC_EDIT_CONFIRM2, m_editConfirmPwdQk);
	DDX_Control(pDX, IDC_BTN_OK2, m_btnQkOK);
	DDX_Control(pDX, IDC_BTN_LOCKMACHINE, m_btnLockMachine);
}


BEGIN_MESSAGE_MAP(CZhangHuXgmmDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CZhangHuXgmmDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_OK2, &CZhangHuXgmmDlg::OnBnClickedBtnOk2)
	ON_BN_CLICKED(IDC_BTN_LOCKMACHINE, &CZhangHuXgmmDlg::OnBnClickedBtnLockMachine)
END_MESSAGE_MAP()


// CZhangHuXgmmDlg 消息处理程序
void CZhangHuXgmmDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.moormachine  == 0)
			m_btnLockMachine.SetImage(CBmpUtil::GetExePath() + _T("skin\\sdbj.png"));
		else if(theAccount.moormachine  == 1)
			m_btnLockMachine.SetImage(CBmpUtil::GetExePath() + _T("skin\\jsjq.png"));

	}
	else
	{
		m_old_pwd = _T("");
		m_new_pwd = _T("");
		m_confirm_pwd = _T("");
		UpdateData(FALSE);

	}
	return;
}

void CZhangHuXgmmDlg::OnPaint()
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

	cacheDC.DrawText(theAccount.account, rc_dl_account, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(theAccount.account, rc_qk_account, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);
	
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CZhangHuXgmmDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CZhangHuXgmmDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

BOOL CZhangHuXgmmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\xgmm_bg.png"));

	m_btnDlOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnQkOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));
	m_btnLockMachine.SetImage(CBmpUtil::GetExePath() + _T("skin\\sdbj.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_editOldPwd.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(250,242,228));
	m_editNewPwd.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(250,242,228));
	m_editConfirmPwd.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(250,242,228));

	m_editOldPwdQk.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(250,242,228));
	m_editNewPwdQk.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(250,242,228));
	m_editConfirmPwdQk.SetEnableColor(RGB(107,102,101),RGB(250,242,228),RGB(250,242,228));
	m_dwTickCount = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CZhangHuXgmmDlg::OnBnClickedBtnLockMachine()
{
	SendToServer(3);
	return;
}
void CZhangHuXgmmDlg::OnBnClickedBtnOk()
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

	if (m_old_pwd.IsEmpty()) 
	{
		MyMessageBox(_T("密码不能为空"));
		return;
	}

	if (m_old_pwd.GetLength() >= PASSWORD_LENGTH) 
	{
		CString msg;
		msg.Format(_T("密码限制长度在【%d】字符之内"), PASSWORD_LENGTH-1);
		MyMessageBox(msg);
		return;
	}

	if (m_new_pwd.IsEmpty()) 
	{
		MyMessageBox(_T("密码不能为空"));
		return;
	}

	if (m_new_pwd.GetLength() >= PASSWORD_LENGTH) 
	{
		CString msg;
		msg.Format(_T("密码限制长度在【%d】字符之内"), PASSWORD_LENGTH-1);
		MyMessageBox(msg);
		return;
	}

	if (m_new_pwd != m_confirm_pwd) 
	{
		MyMessageBox(_T("请确认新密码输入正确"));
		return;
	}

	SendToServer(1); //修改登录密码
	//数据库暂时屏蔽  Alter by a'long
// 	int res = theDBCmd.ModifyLoginPwd(theAccount.user_id, m_old_pwd, m_new_pwd);
// 	if (res == 0) 
// 	{
// 		CString msg;
// 		msg.Format(_T("您已经成功修改密码"));
// 		MyMessageBox(msg);
// 		
// 		m_old_pwd = _T("");
// 		m_new_pwd = _T("");
// 		m_confirm_pwd = _T("");
// 		UpdateData(FALSE);
// 	}
// 	else if (res == 1)
// 	{
// 		MyMessageBox(_T("旧密码不正确"));
// 		return;
// 	}
// 	else if(res == 2)
// 	{
// 		MyMessageBox(_T("修改密码失败，请重试"));
// 		return;
// 	}
}

void CZhangHuXgmmDlg::OnBnClickedBtnOk2()
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

	if (m_old_pwd_qk.IsEmpty()) 
	{
		MyMessageBox(_T("密码不能为空"));
		return;
	}

	if (m_old_pwd_qk.GetLength() >= PASSWORD_LENGTH) 
	{
		CString msg;
		msg.Format(_T("密码限制长度在【%d】字符之内"), PASSWORD_LENGTH-1);
		MyMessageBox(msg);
		return;
	}

	if (m_new_pwd_qk.IsEmpty()) 
	{
		MyMessageBox(_T("密码不能为空"));
		return;
	}

	if (m_new_pwd_qk.GetLength() >= PASSWORD_LENGTH) 
	{
		CString msg;
		msg.Format(_T("密码限制长度在【%d】字符之内"), PASSWORD_LENGTH-1);
		MyMessageBox(msg);
		return;
	}

	if (m_new_pwd_qk != m_confirm_pwd_qk) 
	{
		MyMessageBox(_T("请确认新密码输入正确"));
		return;
	}

	SendToServer(2);
	//数据库暂时屏蔽  Alter by a'long
// 	int res = theDBCmd.ModifyQuKuanPwd(theAccount.user_id, m_old_pwd_qk, m_new_pwd_qk);
// 	if (res == 0) 
// 	{
// 		CString msg;
// 		msg.Format(_T("您已经成功修改密码"));
// 		MyMessageBox(msg);
// 
// 		m_old_pwd_qk = _T("");
// 		m_new_pwd_qk = _T("");
// 		m_confirm_pwd_qk = _T("");
// 		UpdateData(FALSE);
// 	}
// 	else if (res == 1)
// 	{
// 		MyMessageBox(_T("旧密码不正确"));
// 		return;
// 	}
// 	else if(res == 2)
// 	{
// 		MyMessageBox(_T("修改密码失败，请重试"));
// 		return;
// 	}
}

void CZhangHuXgmmDlg::AdjustCtrls()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_OLD_PWD);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, dl_old_pwd_x, dl_old_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_NEW_PWD);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, dl_new_pwd_x, dl_new_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_CONFIRM);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, dl_cfm_pwd_x, dl_cfm_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}
	
	if(m_btnDlOK.GetSafeHwnd() != NULL)
	{
		m_btnDlOK.SetWindowPos(NULL, dl_ok_x-18, dl_ok_y, m_btnDlOK.Width(), m_btnDlOK.Height(), SWP_NOZORDER);
	}

	//--------------------------------------------------------------------------------
	pWnd = GetDlgItem(IDC_EDIT_OLD_PWD2);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, qk_old_pwd_x, qk_old_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_NEW_PWD2);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, qk_new_pwd_x, qk_new_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_CONFIRM2);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, qk_cfm_pwd_x, qk_cfm_pwd_y, edit_width, edit_height, SWP_NOZORDER);
	}

	if(m_btnQkOK.GetSafeHwnd() != NULL)
	{
		m_btnQkOK.SetWindowPos(NULL, qk_ok_x-23, qk_ok_y, m_btnQkOK.Width(), m_btnQkOK.Height(), SWP_NOZORDER);
	}
	if(m_btnLockMachine.GetSafeHwnd() != NULL)
	{
		m_btnLockMachine.SetWindowPos(NULL, qk_ok_x+ m_btnQkOK.Width()-10, qk_ok_y, m_btnLockMachine.Width(), m_btnLockMachine.Height(), SWP_NOZORDER);
	}
}

void CZhangHuXgmmDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CZhangHuXgmmDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}


//读取事件
bool CZhangHuXgmmDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GR_LOCK_MACHINE_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_LockMachine_RET));
				if(wDataSize!=sizeof(CMD_GR_LockMachine_RET))
					return false;

				CMD_GR_LockMachine_RET* pLockRet = (CMD_GR_LockMachine_RET*)pData;

				if(pLockRet->lResult == 0)
				{
					theAccount.moormachine = (theAccount.moormachine==1)?0:1;
					if(theAccount.moormachine  == 0)
					{
						m_btnLockMachine.SetImage(CBmpUtil::GetExePath() + _T("skin\\sdbj.png"));
						MyMessageBox(L"解锁成功！");
					}
					else if(theAccount.moormachine  == 1)
					{
						m_btnLockMachine.SetImage(CBmpUtil::GetExePath() + _T("skin\\jsjq.png"));

						MyMessageBox(L"锁定机器成功！");
					}
				}
				else 
				{
					MyMessageBox(L"锁定机器失败！");
				}
				return true;
				
			}
		case SUB_GR_XG_LOGIN_PASS_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_XG_Loginpass_RET));
				if(wDataSize!=sizeof(CMD_GR_XG_Loginpass_RET))
					return false;

				CMD_GR_XG_Loginpass_RET* pCountRet = (CMD_GR_XG_Loginpass_RET*)pData;

				int lResult = pCountRet->lResult;
				if(lResult == 0)
				{
					CString msg;
					msg.Format(_T("您已经成功修改密码"));
					MyMessageBox(msg);

					m_old_pwd = _T("");
					m_new_pwd = _T("");
					m_confirm_pwd = _T("");
					UpdateData(FALSE);

				}
				else if (lResult == 1)
				{
					MyMessageBox(_T("旧密码不正确"));
					return true;
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("修改密码失败，请重试"));
					return true;
				}

				return true;
			}
		case SUB_GR_XG_QUKUAN_PASS_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_XG_Qukuanpass_RET));
				if(wDataSize!=sizeof(CMD_GR_XG_Qukuanpass_RET))
					return false;

				CMD_GR_XG_Qukuanpass_RET* pXGqqRet = (CMD_GR_XG_Qukuanpass_RET*)pData;

				int lResult = pXGqqRet->lResult;
				if(lResult == 0)
				{
					CString msg;
					msg.Format(_T("您已经成功修改密码"));
					MyMessageBox(msg);

					m_old_pwd = _T("");
					m_new_pwd = _T("");
					m_confirm_pwd = _T("");
					UpdateData(FALSE);

				}
				else if (lResult == 1)
				{
					MyMessageBox(_T("旧密码不正确"));
					return true;
				}
				else if(lResult == 2)
				{
					MyMessageBox(_T("修改密码失败，请重试"));
					return true;
				}
				return true;
			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CZhangHuXgmmDlg::SendToServer(int nSendType)
{

	if(nSendType == 1)   //获取用户信息
	{
		m_bAlterLogonPass = true;
		if(m_bAlterLogonPass)
		{
			m_bAlterLogonPass = false;


			CMD_GP_XG_Loginpass XGLoginPass;
			ZeroMemory(&XGLoginPass,sizeof(XGLoginPass));

			XGLoginPass.dwUserID = theAccount.user_id;
			TCHAR szPassword[33];
			TCHAR szPasswordOld[33];
			memset(szPassword, 0, sizeof(szPassword));
			memset(szPasswordOld, 0, sizeof(szPasswordOld));
			CMD5Encrypt::EncryptData(m_new_pwd,szPassword);
			CMD5Encrypt::EncryptData(m_old_pwd,szPasswordOld);

			lstrcpyn(XGLoginPass.szOldPass,szPasswordOld,sizeof(XGLoginPass.szOldPass));
			lstrcpyn(XGLoginPass.szNewPass,szPassword,sizeof(XGLoginPass.szNewPass));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_XG_LOGIN_PASS,&XGLoginPass,sizeof(XGLoginPass));
			return;

		}


	}

	if(nSendType == 2)//修改QQ 
	{
		m_bAlterQukuanPass = true;
		if(m_bAlterQukuanPass)
		{
			m_bAlterQukuanPass = false;

			CMD_GP_XG_Qukuanpass XGQukuanpass;
			ZeroMemory(&XGQukuanpass,sizeof(XGQukuanpass));
			XGQukuanpass.dwUserID = theAccount.user_id;
			//变量定义
			TCHAR szPassword[33];
			TCHAR szPasswordOld[33];
			memset(szPassword, 0, sizeof(szPassword));
			memset(szPasswordOld, 0, sizeof(szPasswordOld));
			CMD5Encrypt::EncryptData(m_new_pwd_qk,szPassword);
			CMD5Encrypt::EncryptData(m_old_pwd_qk,szPasswordOld);

			lstrcpyn(XGQukuanpass.szOldPass,szPasswordOld,sizeof(XGQukuanpass.szOldPass));
			lstrcpyn(XGQukuanpass.szNewPass,szPassword,sizeof(XGQukuanpass.szNewPass));

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_XG_QUKUAN_PASS,&XGQukuanpass,sizeof(XGQukuanpass));

			return;
		}	
	}
	if(nSendType == 3)
	{
		CMD_GP_LockMachine LockMachine;
		ZeroMemory(&LockMachine,sizeof(LockMachine));
		LockMachine.dwUserID = theAccount.user_id;
		LockMachine.cbLock = (theAccount.moormachine == 0)?1:0;
		CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_LOCK_MACHINE,&LockMachine,sizeof(LockMachine));

		return;

	}
}