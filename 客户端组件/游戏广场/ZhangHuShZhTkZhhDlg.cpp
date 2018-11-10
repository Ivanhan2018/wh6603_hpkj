#include "stdafx.h"
#include "GamePlaza.h"
#include "ZhangHuShZhTkZhhDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
static const int edit_width = 186;
static const int edit_height = 21;

//游戏账号
static const int account_x = 198;
static const int account_y = 163;
static CRect rc_account(account_x, account_y, account_x+edit_width, account_y+edit_height);

//ID
static const int game_id_x = 198;
static const int game_id_y = 197;
static CRect rc_game_id(game_id_x, game_id_y, game_id_x+edit_width, game_id_y+edit_height);

//开户银行
static const int edit_khyh_x = 198 ;
static const int edit_khyh_y = 237;

//开户人
static const int edit_khr_x = 198;
static const int edit_khr_y = 271;

//银行账号
static const int edit_yhzhh_x = 198;
static const int edit_yhzhh_y = 307;

//取款密码
static const int edit_qkmm_x = 198;
static const int edit_qkmm_y = 345;

//确定按钮
static const int btn_ok_x = 196;
static const int btn_ok_y = 376;

IMPLEMENT_DYNAMIC(CZhangHuShZhTkZhhDlg, CDialog)

CZhangHuShZhTkZhhDlg::CZhangHuShZhTkZhhDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZhangHuShZhTkZhhDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_khyh(_T(""))
	, m_khr(_T(""))
	, m_yhzhh(_T(""))
	, m_qkmm(_T(""))
{
	m_bSetQuKuanZhanghao=false;
	m_bQueryYinHang = false;
	m_bQueryMyYinHang=false;
}

CZhangHuShZhTkZhhDlg::~CZhangHuShZhTkZhhDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
		m_bmpBk = NULL;
	}
}

void CZhangHuShZhTkZhhDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_KHYH, m_khyh);
	DDX_Text(pDX, IDC_COMBO_KHYH, m_khyh);
	DDX_Text(pDX, IDC_EDIT_KHR, m_khr);
	DDX_Text(pDX, IDC_EDIT_YHZHH, m_yhzhh);
	DDX_Text(pDX, IDC_EDIT_QKMM, m_qkmm);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_COMBO_KHYH, m_cmbKhyh);

	DDX_Control(pDX, IDC_EDIT_KHR, m_editKhr);
	DDX_Control(pDX, IDC_EDIT_YHZHH, m_editYhzhh);
	DDX_Control(pDX, IDC_EDIT_QKMM, m_editQkmm);
}


BEGIN_MESSAGE_MAP(CZhangHuShZhTkZhhDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &CZhangHuShZhTkZhhDlg::OnBnClickedBtnOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CZhangHuShZhTkZhhDlg 消息处理程序

void CZhangHuShZhTkZhhDlg::OnPaint()
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

	cacheDC.DrawText(theAccount.account, rc_account, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	CString strTmp;
	strTmp.Format(_T("%d"), theAccount.user_id);
	cacheDC.DrawText(strTmp, rc_game_id, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheBmp.DeleteObject();
	cacheDC.DeleteDC();

	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CZhangHuShZhTkZhhDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CZhangHuShZhTkZhhDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	AdjustCtrls();
}

void CZhangHuShZhTkZhhDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <=0)
			return;

		m_btnOK.EnableWindow(FALSE);
		SendToServer(2);
		SendToServer(3);
		m_qkmm = _T("");
		m_cmbKhyh.EnableWindow(FALSE);
		SetYHCmbSel(m_khyh);
		UpdateData(FALSE);
	}

}

void CZhangHuShZhTkZhhDlg::OnBnClickedBtnOk()
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

	//m_khyh.Trim();
	//if (m_khyh.IsEmpty())
	//{
	//	MyMessageBox(_T("开户银行不能为空"));
	//	return;
	//}
	//if (m_khyh.GetLength() > 40)
	//{
	//	MyMessageBox(_T("开户银行名字过长"));
	//	return;
	//}
	
	m_khr.Trim();
	if (m_khr.IsEmpty())
	{
		MyMessageBox(_T("开户人不能为空"));
		return;
	}
	if (m_khr.GetLength() > 40)
	{
		MyMessageBox(_T("开户人名字过长"));
		return;
	}
	
	m_yhzhh.Trim();
	if (m_yhzhh.IsEmpty())
	{
		MyMessageBox(_T("银行账户不能为空"));
		return;
	}
	if (m_yhzhh.GetLength() > 40)
	{
		MyMessageBox(_T("银行账户过长"));
		return;
	}

	m_qkmm.Trim();
	if (m_qkmm.IsEmpty())
	{
		MyMessageBox(_T("取款密码不能为空"));
		return;
	}
	if (m_qkmm.GetLength() > PASSWORD_LENGTH - 1)
	{
		MyMessageBox(_T("取款密码过长"));
		return;
	}

	//数据库暂时屏蔽  Alter by a'long
	SendToServer(1);
// 	int res = theDBCmd.SetTiKuanZhangHao(
// 		theAccount.user_id, m_khyh, m_khr, m_yhzhh, m_qkmm);
// 
// 	if (res == 0)
// 	{
// 		MyMessageBox(_T("设置提款账号成功"));
// 		EnableCtrls(FALSE);
// 	}
// 	else if (res == 1)
// 	{
// 		MyMessageBox(_T("已设置提款账号"));
// 	}
// 	else if (res == 2)
// 	{
// 		MyMessageBox(_T("提款密码不正确"));
// 	}
// 	else if (res == 3)
// 	{
// 		MyMessageBox(_T("设置失败"));
// 	}
}

BOOL CZhangHuShZhTkZhhDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\tkzh_bg.png"));

	m_btnOK.SetImage(CBmpUtil::GetExePath() + _T("skin\\quedingt_bt1.png"));

	m_font.CreateFont(19, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("微软雅黑")); 

	m_cmbKhyh.InsertString(0, _T("支付宝"));
	m_cmbKhyh.InsertString(1, _T("财付通"));
	m_cmbKhyh.InsertString(2, _T("中国工商银行"));
	m_cmbKhyh.InsertString(3, _T("中国建设银行"));
	m_cmbKhyh.InsertString(4, _T("中国农业银行"));
	m_cmbKhyh.InsertString(5, _T("中国民生银行"));
	m_cmbKhyh.InsertString(6, _T("中国光大银行"));
	m_cmbKhyh.InsertString(7, _T("中国银行"));
	m_cmbKhyh.InsertString(8, _T("邮政储蓄"));
	m_cmbKhyh.InsertString(9, _T("交通银行"));
	m_cmbKhyh.InsertString(10, _T("中信银行"));
	m_cmbKhyh.InsertString(11, _T("招商银行"));
	m_cmbKhyh.InsertString(12, _T("平安银行"));
	m_cmbKhyh.InsertString(13, _T("华夏银行"));
	m_cmbKhyh.InsertString(14, _T("广发银行"));
	m_cmbKhyh.InsertString(15, _T("恒丰银行"));
	m_cmbKhyh.InsertString(16, _T("浙商银行"));
	m_cmbKhyh.InsertString(17, _T("农村信用合作社"));
	m_cmbKhyh.InsertString(18, _T("福建兴业银行"));
	m_cmbKhyh.InsertString(19, _T("上海浦东发展银行"));
	m_cmbKhyh.SetCurSel(0);

	m_cmbKhyh.SetFont(&m_font);
	m_editKhr.SetEnableColor(RGB(107,102,101),RGB(249,241,227),RGB(249,241,227));
	m_editYhzhh.SetEnableColor(RGB(107,102,101),RGB(249,241,227),RGB(249,241,227));
	m_editQkmm.SetEnableColor(RGB(107,102,101),RGB(249,241,227),RGB(249,241,227));

	m_dwTickCount = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CZhangHuShZhTkZhhDlg::AdjustCtrls()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_KHYH);
	//if (pWnd != NULL)
	//{
	//	pWnd->SetWindowPos(NULL, edit_khyh_x, edit_khyh_y, edit_width, edit_height, SWP_NOZORDER);
	//}
	if (m_cmbKhyh.GetSafeHwnd() != NULL)
	{
		m_cmbKhyh.SetWindowPos(NULL, edit_khyh_x, edit_khyh_y-3, edit_width, edit_height, SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_KHR);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, edit_khr_x, edit_khr_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_YHZHH);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, edit_yhzhh_x, edit_yhzhh_y, edit_width, edit_height, SWP_NOZORDER);
	}
	pWnd = GetDlgItem(IDC_EDIT_QKMM);
	if (pWnd != NULL)
	{
		pWnd->SetWindowPos(NULL, edit_qkmm_x, edit_qkmm_y, edit_width, edit_height, SWP_NOZORDER);
	}

	if(m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.SetWindowPos(NULL, btn_ok_x, btn_ok_y, m_btnOK.Width(), m_btnOK.Height(), SWP_NOZORDER);
	}
}

void CZhangHuShZhTkZhhDlg::EnableCtrls(BOOL b)
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_KHYH);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(b);
	}
	pWnd = GetDlgItem(IDC_EDIT_KHR);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(b);
	}
	pWnd = GetDlgItem(IDC_EDIT_YHZHH);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(b);
	}
	pWnd = GetDlgItem(IDC_EDIT_QKMM);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(b);
	}

	if(m_btnOK.GetSafeHwnd() != NULL)
	{
		m_btnOK.EnableWindow(b);
	}

	if (m_cmbKhyh.GetSafeHwnd() != NULL)
	{
		m_cmbKhyh.EnableWindow(b);
	}
}

void CZhangHuShZhTkZhhDlg::SetYHCmbSel(const CString& khyh) {

	if (khyh == _T("支付宝")) {
		m_cmbKhyh.SetCurSel(0);
	} 
	else if(khyh == _T("财付通")) {
		m_cmbKhyh.SetCurSel(1);
	}
	else if(khyh == _T("中国工商银行")) {
		m_cmbKhyh.SetCurSel(2);
	}
	else if(khyh == _T("中国建设银行")) {
		m_cmbKhyh.SetCurSel(3);
	}
	else if(khyh == _T("中国农业银行")) {
		m_cmbKhyh.SetCurSel(4);
	}
	else {
		m_cmbKhyh.SetCurSel(0);
	}
}

void CZhangHuShZhTkZhhDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

void CZhangHuShZhTkZhhDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

//读取事件
bool CZhangHuShZhTkZhhDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_QUERY_YINHANG_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GR_QueryYinhang_RET)==0);
				if(wDataSize%sizeof(CMD_GR_QueryYinhang_RET)!=0)
					return false;

				WORD wCount = wDataSize/sizeof(CMD_GR_QueryYinhang_RET);
				

				for(int i = 0;i < wCount;i++)
				{
					CMD_GR_QueryYinhang_RET* pQueryLotResult  =  ((CMD_GR_QueryYinhang_RET*)pData+i);

					int nItem = m_cmbKhyh.GetCount();
					m_cmbKhyh.InsertString(nItem,pQueryLotResult->szYinHangName);
					
				}


				m_cmbKhyh.SetCurSel(0);
				return true;
			}
		case SUB_GR_QUERY_MY_YINHANG_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_QueryMyYinHang_RET));
				if(wDataSize!=sizeof(CMD_GR_QueryMyYinHang_RET))
					return false;
				CMD_GR_QueryMyYinHang_RET* pQueryMyYinHang = (CMD_GR_QueryMyYinHang_RET*)pData;
				
				if(pQueryMyYinHang->lResult == 0)
				{
					m_btnOK.EnableWindow(FALSE);

// 					for (int i = 0;i < m_cmbKhyh.GetCount;i++)
// 					{
// 						//CString strYinHang = m_cmbKhyh.GetItemData(i);
// 
// 					}
					int nSel = m_cmbKhyh.FindString(0,pQueryMyYinHang->szKaihuYinghang);
					m_cmbKhyh.SetCurSel(nSel);

					m_cmbKhyh.EnableWindow(FALSE);
					CWnd *pWnd = GetDlgItem(IDC_EDIT_KHR);
					if(pWnd != NULL)
					{
						pWnd->EnableWindow(FALSE);
						CString strKaihR,strText;
						strKaihR.Format(L"%s",pQueryMyYinHang->szKaihuRen);

						strText.Format(L"%s*",strKaihR.Left(2));
						pWnd->SetWindowText(strText);
					}
					pWnd = GetDlgItem(IDC_EDIT_YHZHH);
					if(pWnd != NULL)
					{
						pWnd->EnableWindow(FALSE);
						CString strKaihR,strText;
						strKaihR.Format(L"%s",pQueryMyYinHang->szYinhangZhanghu);

						strText.Format(L"%s**********%s",strKaihR.Left(4),strKaihR.Right(4));
						pWnd->SetWindowText(strText);

					//	pWnd->SetWindowText(pQueryMyYinHang->szYinhangZhanghu);
					}
				}
				else
				{
					m_btnOK.EnableWindow(TRUE);
					m_cmbKhyh.EnableWindow(TRUE);
					EnableCtrls(TRUE);
				}

				return true;
			}
		case SUB_GR_SET_QUKUAN_ZHANGHAO_RET:
			{
				ASSERT(wDataSize == sizeof(CMD_GR_SetQukuanZhanghao_RET));
				if(wDataSize!=sizeof(CMD_GR_SetQukuanZhanghao_RET))
					return false;

				CMD_GR_SetQukuanZhanghao_RET* pCountRet = (CMD_GR_SetQukuanZhanghao_RET*)pData;

				int res = pCountRet->lResult;
				if (res == 0)
				{
					MyMessageBox(_T("设置提款账号成功"));
					EnableCtrls(FALSE);
				}
				else if (res == 1)
				{
					MyMessageBox(_T("已设置提款账号"));
				}
				else if (res == 2)
				{
					MyMessageBox(_T("提款密码不正确"));
				}
				else if (res == 3)
				{
					MyMessageBox(_T("设置失败"));
				}

				return true;
			}

		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

VOID CZhangHuShZhTkZhhDlg::SendToServer(int nSendType)
{
	if(nSendType == 1)   //获取用户信息
	{
		m_bSetQuKuanZhanghao = true;
		if(m_bSetQuKuanZhanghao)
		{
			m_bSetQuKuanZhanghao = false;


			CMD_GP_SetQukuanZhanghao SetQukuanZhanghao;
			ZeroMemory(&SetQukuanZhanghao,sizeof(SetQukuanZhanghao));

			SetQukuanZhanghao.dwUserID = theAccount.user_id;
			lstrcpyn(SetQukuanZhanghao.szKaihuYinghang,m_khyh.GetBuffer(),sizeof(SetQukuanZhanghao.szKaihuYinghang));
			lstrcpyn(SetQukuanZhanghao.szKaihuRen,m_khr.GetBuffer(),sizeof(SetQukuanZhanghao.szKaihuRen));
			lstrcpyn(SetQukuanZhanghao.szYinhangZhanghu,m_yhzhh.GetBuffer(),sizeof(SetQukuanZhanghao.szYinhangZhanghu));
			TCHAR szPassword[33];
			memset(szPassword, 0, sizeof(szPassword));
			CMD5Encrypt::EncryptData(m_qkmm,szPassword);

			lstrcpyn(SetQukuanZhanghao.szQukuanPass,szPassword,sizeof(SetQukuanZhanghao.szQukuanPass));
			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SET_QUKUAN_ZHANGHAO,&SetQukuanZhanghao,sizeof(SetQukuanZhanghao));
			return;

		}


	}
	else if(nSendType == 2)
	{
		m_bQueryYinHang = true;
		if(m_bQueryYinHang)
		{
			m_bQueryYinHang = false;	
			m_cmbKhyh.ResetContent();

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_YINHANG);
			return;

		}

	}
	else if(nSendType == 3)
	{
		m_bQueryMyYinHang = true;
		if(m_bQueryMyYinHang)
		{
			m_bQueryMyYinHang = false;
			CMD_GP_QueryMyYinHang QueryMyYinhang;
			ZeroMemory(&QueryMyYinhang,sizeof(QueryMyYinhang));

			QueryMyYinhang.dwUserID = theAccount.user_id;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_MY_YINHANG,&QueryMyYinhang,sizeof(QueryMyYinhang));
			return;
		}
	}

}