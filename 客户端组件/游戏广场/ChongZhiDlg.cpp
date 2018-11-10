#include "stdafx.h"
#include "GamePlaza.h"
#include "ChongzhiDlg.h"
#include "MessageDlg.h"
#include "PlatformFrame.h"
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

static const int bk_chongzhi_x = 220;
static const int bk_chongzhi_y = 50;
static const int bk_chongzhi_width = 725;
static const int bk_chongzhi_height = 415;

static const int logo_chongzhi_x = 0;
static const int logo_chongzhi_y = 0;
static const int logo_chongzhi_width = 220;
static const int logo_chongzhi_height = 617;

//银行按钮数据
static const int btn_yinhang_width = 150;
static const int btn_yinhang_height = 34;
static const int btn_yinhang_y = 115;
static const int btn_space = 150;
static const int bnt_left = 143;

//工商银行
static const int btn_gongshang_x = bnt_left;
//支付宝
static const int btn_zhifubao_x = bnt_left + btn_space * 1;
//财付通
static const int btn_caifutong_x = bnt_left + btn_space * 2;
//建设银行
static const int btn_jianshe_x = bnt_left + btn_space * 3;
//农业银行
static const int btn_nongye_x = bnt_left + btn_space * 4;

//打开网银充值按钮数据
static const int	btn_chongzhipage_x = 430;
static const int	btn_chongzhipage_y = 505;
static const int	btn_chongzhipage_width = 175;
static const int	btn_chongzhipage_height = 50;

//复制按钮数据
static const int btn_copy_x = 710;
static const int btn_copy_width = 73;
static const int btn_copy_height = 29;
//复制账号
static const int btn_cp_zhanghu_y = 191;
//复制开户人
static const int btn_cp_kaihuren_y = 237;
//复制开户银行
static const int btn_cp_yinhang_y = 280;
//复制游戏ID
static const int btn_cp_gameid_y = 324;

//账户信息数据
static const CRect rc_zhanghu(407, 160, 650, 255);
static const CRect rc_kaihuren(407, 207, 650, 300);
static const CRect rc_kaihuhang(407,248, 650, 346);
static const CRect rc_gameid(407, 297, 650, 316+70);
static const CRect rc_invalidate(225, 190, 920, 768);
static const CRect rc_BeiZhu(225, 368, 920, 768);

IMPLEMENT_DYNAMIC(CChongZhiDlg, CDialog)

CChongZhiDlg::CChongZhiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChongZhiDlg::IDD, pParent)
	, m_bmpBk(NULL)
	, m_bmpChongZhiBk(NULL)
// 	, m_bmpChongZhiLogo(NULL)
	, m_ChongZhiType(CZ_GONGSHANG)
{
	m_bGetChongzhiType = false;
	m_bGetAllYinHangName = false;
}

CChongZhiDlg::~CChongZhiDlg()
{
	if (m_bmpBk != NULL)
	{
		delete m_bmpBk;
		m_bmpBk = NULL;
	}

	if (m_bmpChongZhiBk != NULL)
	{
		delete m_bmpChongZhiBk;
		m_bmpChongZhiBk = NULL;
	}

}

void CChongZhiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_GONGSHANG, m_btnGongShang);
	DDX_Control(pDX, IDC_BTN_ZHIFUBAO, m_btnZhiFuBao);
	DDX_Control(pDX, IDC_BTN_CAIFUTONG, m_btnCaiFuTong);
	DDX_Control(pDX, IDC_BTN_JIANSHE, m_btnJianShe);
	DDX_Control(pDX, IDC_BTN_NONGYE, m_btnNongYe);
	DDX_Control(pDX, IDC_BTN_CHONGZHI_PAGE, m_btnChongZhiPage);
	DDX_Control(pDX, IDC_BTN_CP_ZHANGHU, m_btnCPZhangHu);
	DDX_Control(pDX, IDC_BTN_CP_KAIHUREN, m_btnCPKaiHuRen);
	DDX_Control(pDX, IDC_BTN_CP_YINHANG, m_btnCPYinHang);
	DDX_Control(pDX, IDC_BTN_CP_GANEID, m_btnCPGameID);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);

}


BEGIN_MESSAGE_MAP(CChongZhiDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_GONGSHANG, &CChongZhiDlg::OnBnClickedBtnGongshang)
	ON_BN_CLICKED(IDC_BTN_ZHIFUBAO, &CChongZhiDlg::OnBnClickedBtnZhifubao)
	ON_BN_CLICKED(IDC_BTN_CAIFUTONG, &CChongZhiDlg::OnBnClickedBtnCaifutong)
	ON_BN_CLICKED(IDC_BTN_JIANSHE, &CChongZhiDlg::OnBnClickedBtnJianshe)
	ON_BN_CLICKED(IDC_BTN_NONGYE, &CChongZhiDlg::OnBnClickedBtnNongye)
	ON_BN_CLICKED(IDC_BTN_CHONGZHI_PAGE, &CChongZhiDlg::OnBnClickedBtnChongzhiPage)
	ON_BN_CLICKED(IDC_BTN_CP_ZHANGHU, &CChongZhiDlg::OnBnClickedBtnCpZhanghu)
	ON_BN_CLICKED(IDC_BTN_CP_KAIHUREN, &CChongZhiDlg::OnBnClickedBtnCpKaihuren)
	ON_BN_CLICKED(IDC_BTN_CP_YINHANG, &CChongZhiDlg::OnBnClickedBtnCpYinhang)
	ON_BN_CLICKED(IDC_BTN_CP_GANEID, &CChongZhiDlg::OnBnClickedBtnCpGaneid)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CChongZhiDlg::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CChongzhiDlg 消息处理程序
void CChongZhiDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(theAccount.user_id <= 0)
			return;

		SendToServer(1);

	//	OnBnClickedBtnGongshang();
		//m_btnGongShang.ShowWindow(SW_HIDE);
		//m_btnNongYe.ShowWindow(SW_HIDE);
	}

}

BOOL CChongZhiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\n_bg1.png"));
	m_bmpChongZhiBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\czzx_bg.png"));

	InitBtns();
	m_ChongZhiType=CZ_GONGSHANG;
	m_font.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,0,0,0,0,FF_SWISS,TEXT("微软雅黑"));

	m_btnClose.SetImage(CBmpUtil::GetExePath() + _T("skin\\return_bt.png"));
	//设置按钮初始状态
	return TRUE;  
}

void CChongZhiDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);

	Graphics graphics(dc.m_hDC);
	CFont* pOldFont = dc.SelectObject(&m_font);

	graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()),0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);
	graphics.DrawImage(m_bmpChongZhiBk, Rect(15, 15,   m_bmpChongZhiBk->GetWidth(), m_bmpChongZhiBk->GetHeight()), 
		0, 0, m_bmpChongZhiBk->GetWidth(), m_bmpChongZhiBk->GetHeight(), UnitPixel);
// 	graphics.DrawImage(m_bmpChongZhiLogo, Rect(logo_chongzhi_x, logo_chongzhi_y, logo_chongzhi_width, logo_chongzhi_height), 
// 		0, 0, m_bmpChongZhiLogo->GetWidth(), m_bmpChongZhiLogo->GetHeight(), UnitPixel);

 	int oldBkMode = dc.SetBkMode(TRANSPARENT);

	COLORREF crTextColor=dc.SetTextColor(RGB(255,0,0));
// 	switch (m_ChongZhiType) {
// 	case CZ_GONGSHANG:
// 		{
			CString strBeizhu;
			strBeizhu.Format(L"最低充值50元起，单笔最高充值10万元。");

			DrawMultLineText(&dc,&CRect(rc_BeiZhu),5, DT_LEFT|DT_WORDBREAK,strBeizhu);
// 			strBeizhu.Format(L"2、收款账户名和收款账号会不定期更换，请在获取最新信息后充\r\n值，否则充值将无法到账。");
// 
// 			CRect BeiZhu;
// 			BeiZhu.CopyRect(rc_BeiZhu);
// 			BeiZhu.top += 45;
// 			BeiZhu.bottom += 45;
// 			DrawMultLineText(&dc,&CRect(BeiZhu),5, DT_LEFT|DT_WORDBREAK,strBeizhu);
// 			strBeizhu.Format(L"3、工行汇款，登录网银，选择e-mail转账汇款。");
// 			BeiZhu.top += 26;
// 			BeiZhu.bottom += 26;
// 
// 			DrawMultLineText(&dc,&CRect(BeiZhu),5, DT_LEFT|DT_WORDBREAK,strBeizhu);
// 			//dc.DrawText(strBeizhu, &CRect(rc_BeiZhu), DT_LEFT|DT_WORDBREAK);
// 		}
// 		break;
// 	case CZ_ZHIFUBAO:
// 		{
// 			CString strBeizhu;
// 			strBeizhu.Format(L"请务必在转账备注信息中填写此游戏ID，您也可以使用复制");
// 
// 			DrawMultLineText(&dc,&CRect(rc_BeiZhu),5, DT_LEFT|DT_WORDBREAK,strBeizhu);
// 			strBeizhu.Format(L"1、请务必复制“附言”准确填写到“工行”汇款页面的“附言栏”、“招行”汇款页面的“备注栏”、“建行”汇款页面的“附言栏”、“农行”汇款页面的“转账用途栏”、“交行”汇款页面的“汇款附言栏”中，否则将无法充值到账。");
// 
// 			CRect BeiZhu;
// 			BeiZhu.CopyRect(rc_BeiZhu);
// 			BeiZhu.top += 26;
// 			BeiZhu.bottom += 26;
// 			DrawMultLineText(&dc,&CRect(BeiZhu),5, DT_LEFT|DT_WORDBREAK,strBeizhu);
// 			strBeizhu.Format(L"2、附言是自动到账的唯一标识，填写他人生成的附言导致的资金损失，平台概不负责。");
// 			BeiZhu.top += 66;
// 			BeiZhu.bottom += 66;
// 			DrawMultLineText(&dc,&CRect(BeiZhu),5, DT_LEFT|DT_WORDBREAK,strBeizhu);
// 			strBeizhu.Format(L"3、跨行充值1000元以上，返充值金额的0.2%%，上不封顶。");
// 			BeiZhu.top += 26;
// 			BeiZhu.bottom += 26;
// 			DrawMultLineText(&dc,&CRect(BeiZhu),5, DT_LEFT|DT_WORDBREAK,strBeizhu);
// 		}
// 		break;
// 	case CZ_CAIFUTONG:
// 		{
// 
// 		}
// 		break;
// 	case CZ_JIANSHE:
// 		{
// 			CString strBeizhu;
// 			strBeizhu.Format(L"1、务必将“充值编号”正确填写到汇款页面的“附言”栏中(复制->粘帖[CTRL+V])，否则充值将无法到账。");
// 
// 			DrawMultLineText(&dc,&CRect(rc_BeiZhu),5, DT_LEFT|DT_WORDBREAK,strBeizhu);
// 			strBeizhu.Format(L"2、收款账户名和收款账号会不定期更换，请在获取最新信息后充值，否则充值将无法到账。");
// 
// 			CRect BeiZhu;
// 			BeiZhu.CopyRect(rc_BeiZhu);
// 			BeiZhu.top += 45;
// 			BeiZhu.bottom += 45;
// 			DrawMultLineText(&dc,&CRect(BeiZhu),5, DT_LEFT|DT_WORDBREAK,strBeizhu);
// 			strBeizhu.Format(L"3、充值金额低于100 不享受“充值即返手续费”的优惠政策。");
// 			BeiZhu.top += 26;
// 			BeiZhu.bottom += 26;
// 
// 			DrawMultLineText(&dc,&CRect(BeiZhu),5, DT_LEFT|DT_WORDBREAK,strBeizhu);
// 		}
// 		break;
// 	case CZ_NONGYE:
// 		{
// // 			dc.DrawText(m_cz_nongye.m_strZhangHu, &CRect(rc_zhanghu), DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// // 			dc.DrawText(m_cz_nongye.m_strKaiHuRen, &CRect(rc_kaihuren), DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// // 			dc.DrawText(m_cz_nongye.m_strKaiHuHang, &CRect(rc_kaihuhang), DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// // 			dc.DrawText(m_cz_nongye.m_strGameID, &CRect(rc_gameid), DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 		}
// 		break;
// 
// 	default:
// 		break;
// 	}
	dc.SetTextColor(RGB(88, 78, 77));
	dc.DrawText(m_chongzhi.m_strZhangHu, &CRect(rc_zhanghu), DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	dc.DrawText(m_chongzhi.m_strKaiHuRen, &CRect(rc_kaihuren), DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	dc.DrawText(m_chongzhi.m_strKaiHuHang, &CRect(rc_kaihuhang), DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	if(m_ChongZhiType != CZ_CAIFUTONG)
	{
		CString sUserID;
		sUserID.Format(_T("%d"), theAccount.user_id);
		dc.DrawText(sUserID, &CRect(rc_gameid), DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	}
	dc.SetBkMode(oldBkMode);
	dc.SelectObject(pOldFont);
	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CChongZhiDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CChongZhiDlg::DrawMultLineText(CDC *pDC, CRect rect, int nRowDis, UINT nFromat, CString strText)
{

	if( strText.GetLength() <= 0 )
		return;

	WCHAR* pText = strText.GetBuffer(strText.GetLength());
	int nCount = strText.GetLength();
	CRect rtChar;
	CSize size = pDC->GetTextExtent(pText + 0, 1);
	int nRowHeight = size.cy + nRowDis;
	rtChar.top = rect.top;
	rtChar.left = rect.left;
	rtChar.bottom  = rtChar.top + nRowDis + size.cy;
	rtChar.right  = rtChar.left + size.cx; 
	CString strChar;
	for (int nCharIndex = 0; nCharIndex < nCount; nCharIndex++)
	{
		if( rtChar.right > rect.right )
		{
			rtChar.top = rtChar.bottom;
			rtChar.bottom += nRowHeight;
			size = pDC->GetTextExtent(pText + nCharIndex, 1);
			rtChar.left = rect.left;
			rtChar.right = rtChar.left + size.cx;
			if( rtChar.bottom > rect.bottom )
				break;
		}
		strChar = pText[nCharIndex];
		pDC->DrawText(strChar, rtChar, nFromat);
		size = pDC->GetTextExtent(pText + nCharIndex + 1, 1);
		rtChar.left = rtChar.right;
		rtChar.right += size.cx;
	}
}
void CChongZhiDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL) {
		return;
	}
	if(m_btnClose.GetSafeHwnd() != NULL)
	{
		m_btnClose.SetWindowPos(NULL, cx - m_btnClose.Width() - 9, -1, m_btnClose.Width(), m_btnClose.Height(), SWP_NOZORDER);
	}
	if(m_btnGongShang.GetSafeHwnd() != NULL)
	{
		m_btnGongShang.SetWindowPos(NULL, btn_gongshang_x, btn_yinhang_y, btn_yinhang_width, btn_yinhang_height, SWP_NOZORDER);
	}

	if(m_btnZhiFuBao.GetSafeHwnd() != NULL)
	{
		m_btnZhiFuBao.SetWindowPos(NULL, btn_zhifubao_x, btn_yinhang_y, btn_yinhang_width, btn_yinhang_height, SWP_NOZORDER);
	}

	if(m_btnCaiFuTong.GetSafeHwnd() != NULL)
	{
		m_btnCaiFuTong.SetWindowPos(NULL, btn_caifutong_x, btn_yinhang_y, btn_yinhang_width, btn_yinhang_height, SWP_NOZORDER);
	}

	if(m_btnJianShe.GetSafeHwnd() != NULL)
	{
		m_btnJianShe.SetWindowPos(NULL, btn_jianshe_x, btn_yinhang_y, btn_yinhang_width, btn_yinhang_height, SWP_NOZORDER);
	}

	if(m_btnNongYe.GetSafeHwnd() != NULL)
	{
		m_btnNongYe.SetWindowPos(NULL, btn_nongye_x, btn_yinhang_y, btn_yinhang_width, btn_yinhang_height, SWP_NOZORDER);
	}

	if(m_btnChongZhiPage.GetSafeHwnd() != NULL)
	{
		m_btnChongZhiPage.SetWindowPos(NULL, btn_chongzhipage_x, btn_chongzhipage_y, btn_chongzhipage_width, btn_chongzhipage_height, SWP_NOZORDER);
	}

	if(m_btnCPZhangHu.GetSafeHwnd() != NULL)
	{
		m_btnCPZhangHu.SetWindowPos(NULL, btn_copy_x, btn_cp_zhanghu_y, btn_copy_width, btn_copy_height, SWP_NOZORDER);
	}

	if(m_btnCPKaiHuRen.GetSafeHwnd() != NULL)
	{
		m_btnCPKaiHuRen.SetWindowPos(NULL, btn_copy_x, btn_cp_kaihuren_y, btn_copy_width, btn_copy_height, SWP_NOZORDER);
	}

	if(m_btnCPYinHang.GetSafeHwnd() != NULL)
	{
		m_btnCPYinHang.SetWindowPos(NULL, btn_copy_x, btn_cp_yinhang_y, btn_copy_width, btn_copy_height, SWP_NOZORDER);
	}

	if(m_btnCPGameID.GetSafeHwnd() != NULL)
	{
		m_btnCPGameID.SetWindowPos(NULL, btn_copy_x, btn_cp_gameid_y, btn_copy_width, btn_copy_height, SWP_NOZORDER);
	}
}
void CChongZhiDlg::OnBnClickedBtnGongshang()
{
	m_btnGongShang.SetPushed(true);
	m_btnZhiFuBao.SetPushed(false);
	m_btnCaiFuTong.SetPushed(false);
	m_btnJianShe.SetPushed(false);
	m_btnNongYe.SetPushed(false);
	if(m_ChongZhiType == CZ_CAIFUTONG)
	{
		m_bmpChongZhiBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\czzx_bg.png"));
		m_btnCPYinHang.ShowWindow(SW_SHOW);
		m_btnCPKaiHuRen.ShowWindow(SW_SHOW);
		m_btnCPZhangHu.ShowWindow(SW_SHOW);
		m_btnCPGameID.ShowWindow(SW_SHOW);
		m_btnChongZhiPage.SetWindowPos(NULL, btn_chongzhipage_x, btn_chongzhipage_y, btn_chongzhipage_width, btn_chongzhipage_height, SWP_NOZORDER);

		Invalidate(FALSE);
	}


	m_ChongZhiType = CZ_GONGSHANG;
	m_chongzhi.m_strKaiHuHang = m_ChongZhiPage[m_ChongZhiType].m_strKaiHuHang;
	m_chongzhi.m_strKaiHuRen = m_ChongZhiPage[m_ChongZhiType].m_strKaiHuRen;
	m_chongzhi.m_strZhangHu = m_ChongZhiPage[m_ChongZhiType].m_strZhangHu;
	m_chongzhi.m_strWeb = m_ChongZhiPage[m_ChongZhiType].m_strWeb;

	RedrawWindow(&rc_invalidate,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
}

void CChongZhiDlg::OnBnClickedBtnZhifubao()
{
	m_btnGongShang.SetPushed(false);
	m_btnZhiFuBao.SetPushed(true);
	m_btnCaiFuTong.SetPushed(false);
	m_btnJianShe.SetPushed(false);
	m_btnNongYe.SetPushed(false);

	if(m_ChongZhiType == CZ_CAIFUTONG)
	{
		m_bmpChongZhiBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\czzx_bg.png"));
		m_btnCPYinHang.ShowWindow(SW_SHOW);
		m_btnCPKaiHuRen.ShowWindow(SW_SHOW);
		m_btnCPZhangHu.ShowWindow(SW_SHOW);
		m_btnCPGameID.ShowWindow(SW_SHOW);
		m_btnChongZhiPage.SetWindowPos(NULL, btn_chongzhipage_x, btn_chongzhipage_y, btn_chongzhipage_width, btn_chongzhipage_height, SWP_NOZORDER);

		Invalidate(FALSE);
	}

	m_ChongZhiType=CZ_ZHIFUBAO;
	m_chongzhi.m_strKaiHuHang = m_ChongZhiPage[m_ChongZhiType].m_strKaiHuHang;
	m_chongzhi.m_strKaiHuRen = m_ChongZhiPage[m_ChongZhiType].m_strKaiHuRen;
	m_chongzhi.m_strZhangHu = m_ChongZhiPage[m_ChongZhiType].m_strZhangHu;
	m_chongzhi.m_strWeb = m_ChongZhiPage[m_ChongZhiType].m_strWeb;

	RedrawWindow(&rc_invalidate,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

}

void CChongZhiDlg::OnBnClickedBtnCaifutong()
{

	m_btnGongShang.SetPushed(false);
	m_btnZhiFuBao.SetPushed(false);
	m_btnCaiFuTong.SetPushed(true);
	m_btnJianShe.SetPushed(false);
	m_btnNongYe.SetPushed(false);

	if(m_ChongZhiType != CZ_CAIFUTONG)
	{
		m_bmpChongZhiBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\czzx_bg1.png"));
		m_btnChongZhiPage.SetWindowPos(NULL, btn_chongzhipage_x, btn_chongzhipage_y-200, btn_chongzhipage_width, btn_chongzhipage_height, SWP_NOZORDER);

	//	Invalidate(FALSE);
	}
	m_ChongZhiType=CZ_CAIFUTONG;
	m_chongzhi.m_strKaiHuHang.Empty();
	m_chongzhi.m_strKaiHuRen.Empty();
	m_chongzhi.m_strZhangHu.Empty();
	m_chongzhi.m_strWeb = m_ChongZhiPage[m_ChongZhiType].m_strWeb;

	m_btnCPYinHang.ShowWindow(SW_HIDE);
	m_btnCPKaiHuRen.ShowWindow(SW_HIDE);
	m_btnCPZhangHu.ShowWindow(SW_HIDE);
	m_btnCPGameID.ShowWindow(SW_HIDE);
	RedrawWindow(&rc_invalidate,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
}

void CChongZhiDlg::OnBnClickedBtnJianshe()
{
	m_btnGongShang.SetPushed(false);
	m_btnZhiFuBao.SetPushed(false);
	m_btnCaiFuTong.SetPushed(false);
	m_btnJianShe.SetPushed(true);
	m_btnNongYe.SetPushed(false);

	if(m_ChongZhiType == CZ_CAIFUTONG)
	{
		m_bmpChongZhiBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\czzx_bg.png"));
		m_btnCPYinHang.ShowWindow(SW_SHOW);
		m_btnCPKaiHuRen.ShowWindow(SW_SHOW);
		m_btnCPZhangHu.ShowWindow(SW_SHOW);
		m_btnCPGameID.ShowWindow(SW_SHOW);
		m_btnChongZhiPage.SetWindowPos(NULL, btn_chongzhipage_x, btn_chongzhipage_y, btn_chongzhipage_width, btn_chongzhipage_height, SWP_NOZORDER);

		Invalidate(FALSE);
	}


	m_ChongZhiType = CZ_JIANSHE;
	m_chongzhi.m_strKaiHuHang = m_ChongZhiPage[m_ChongZhiType].m_strKaiHuHang;
	m_chongzhi.m_strKaiHuRen = m_ChongZhiPage[m_ChongZhiType].m_strKaiHuRen;
	m_chongzhi.m_strZhangHu = m_ChongZhiPage[m_ChongZhiType].m_strZhangHu;
	m_chongzhi.m_strWeb = m_ChongZhiPage[m_ChongZhiType].m_strWeb;

	//ShellExecute(NULL, _T("open"), m_chongzhi.m_strWeb, NULL, NULL, SW_SHOWNORMAL);


	RedrawWindow(&rc_invalidate,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

}

void CChongZhiDlg::OnBnClickedBtnNongye()
{
	m_btnGongShang.SetPushed(false);
	m_btnZhiFuBao.SetPushed(false);
	m_btnCaiFuTong.SetPushed(false);
	m_btnJianShe.SetPushed(false);
	m_btnNongYe.SetPushed(true);


	if(m_ChongZhiType == CZ_CAIFUTONG)
	{
		m_bmpChongZhiBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\czzx_bg.png"));
		m_btnCPYinHang.ShowWindow(SW_SHOW);
		m_btnCPKaiHuRen.ShowWindow(SW_SHOW);
		m_btnCPZhangHu.ShowWindow(SW_SHOW);
		m_btnCPGameID.ShowWindow(SW_SHOW);
		m_btnChongZhiPage.SetWindowPos(NULL, btn_chongzhipage_x, btn_chongzhipage_y, btn_chongzhipage_width, btn_chongzhipage_height, SWP_NOZORDER);

		Invalidate(FALSE);
	}

	m_ChongZhiType = CZ_NONGYE;
	m_chongzhi.m_strKaiHuHang = m_ChongZhiPage[m_ChongZhiType].m_strKaiHuHang;
	m_chongzhi.m_strKaiHuRen = m_ChongZhiPage[m_ChongZhiType].m_strKaiHuRen;
	m_chongzhi.m_strZhangHu = m_ChongZhiPage[m_ChongZhiType].m_strZhangHu;
	m_chongzhi.m_strWeb = m_ChongZhiPage[m_ChongZhiType].m_strWeb;

	RedrawWindow(&rc_invalidate,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

}

void CChongZhiDlg::OnBnClickedBtnChongzhiPage()
{
	char szYInHang[1024] = "";
	switch (m_ChongZhiType) {
	case CZ_GONGSHANG:
//		ShellExecute(NULL, _T("open"), _T("IEXPLORE"), _T("http://www.icbc.com.cn/icbc/"), NULL, SW_SHOWNORMAL);
		break;
	case CZ_ZHIFUBAO:
//		ShellExecute(NULL, _T("open"), _T("IEXPLORE"), _T("http://www.alipay.com/"), NULL, SW_SHOWNORMAL);
		break;
	case CZ_CAIFUTONG:
		{
			CString strWeb;
			strWeb.Format(L"%s?userid=%d",m_chongzhi.m_strWeb,theAccount.user_id);
			//ShellExecute(NULL, _T("open"), _T("IEXPLORE"), strWeb, NULL, SW_SHOWNORMAL);
			ShellExecute(NULL, _T("open"), strWeb, NULL, NULL, SW_SHOWNORMAL);
			return;

		}
	case CZ_JIANSHE:
//		ShellExecute(NULL, _T("open"), _T("IEXPLORE"), _T("http://www.ccb.com/cn/home/index.html"), NULL, SW_SHOWNORMAL);
		break;
	case CZ_NONGYE:
//		ShellExecute(NULL, _T("open"), _T("IEXPLORE"), _T("http://www.95599.cn/cn/default.htm"), NULL, SW_SHOWNORMAL);
		break;
	default:
	//	ShellExecute(NULL, _T("open"), _T("IEXPLORE"), _T("http://www.icbc.com.cn/icbc/"), NULL, SW_SHOWNORMAL);
		ShellExecute(NULL, _T("open"), _T("http://www.icbc.com.cn/icbc/"), NULL, NULL, SW_SHOWNORMAL);
		break;
	}

	CString strNag;
	strNag.Format(L"%s?userid=%d",m_chongzhi.m_strWeb,theAccount.user_id);

	//ShellExecute(NULL, _T("open"), _T("IEXPLORE"), m_chongzhi.m_strWeb, NULL, SW_SHOWNORMAL);
	ShellExecute(NULL, _T("open"), strNag, NULL, NULL, SW_SHOWNORMAL);

}

void CChongZhiDlg::OnBnClickedBtnCpZhanghu()
{
	const int bufLen = 1024;
	TCHAR text[bufLen] = _T("");
	/*switch (m_ChongZhiType)
	{
		case CZ_GONGSHANG:
			_tcscpy_s(text, bufLen, m_cz_gongshang.m_strZhangHu);
			break;
		case CZ_ZHIFUBAO:
			_tcscpy_s(text, bufLen, m_cz_zhifubao.m_strZhangHu);
			break;
		case CZ_CAIFUTONG:
			_tcscpy_s(text, bufLen, m_cz_caifutong.m_strZhangHu);
			break;
		case CZ_JIANSHE:
			_tcscpy_s(text, bufLen, m_cz_jianshe.m_strZhangHu);
			break;
		case CZ_NONGYE:
			_tcscpy_s(text, bufLen, m_cz_nongye.m_strZhangHu);
			break;
		default:
			break;
	}*/

	_tcscpy_s(text, bufLen, m_chongzhi.m_strZhangHu);
	SetTextToClipboard(text);
}

void CChongZhiDlg::OnBnClickedBtnCpKaihuren()
{
	const int bufLen = 1024;
	TCHAR text[bufLen] = _T("");
	/*switch (m_ChongZhiType) 
	{
	case CZ_GONGSHANG:
		_tcscpy_s(text, bufLen, m_cz_gongshang.m_strKaiHuRen);
		break;
	case CZ_ZHIFUBAO:
		_tcscpy_s(text, bufLen, m_cz_zhifubao.m_strKaiHuRen);
		break;
	case CZ_CAIFUTONG:
		_tcscpy_s(text, bufLen, m_cz_caifutong.m_strKaiHuRen);
		break;
	case CZ_JIANSHE:
		_tcscpy_s(text, bufLen, m_cz_jianshe.m_strKaiHuRen);
		break;
	case CZ_NONGYE:
		_tcscpy_s(text, bufLen, m_cz_nongye.m_strKaiHuRen);
		break;
	default:
		break;
	}*/

	_tcscpy_s(text, bufLen, m_chongzhi.m_strKaiHuRen);
	SetTextToClipboard(text);
}

void CChongZhiDlg::OnBnClickedBtnCpYinhang()
{
	const int bufLen = 1024;
	TCHAR text[bufLen] = _T("");
	//switch (m_ChongZhiType) 
	//{
	//case CZ_GONGSHANG:
	//	_tcscpy_s(text, bufLen, m_cz_gongshang.m_strKaiHuHang);
	//	break;
	//case CZ_ZHIFUBAO:
	//	_tcscpy_s(text, bufLen, m_cz_zhifubao.m_strKaiHuHang);
	//	break;
	//case CZ_CAIFUTONG:
	//	_tcscpy_s(text, bufLen, m_cz_caifutong.m_strKaiHuHang);
	//	break;
	//case CZ_JIANSHE:
	//	_tcscpy_s(text, bufLen, m_cz_jianshe.m_strKaiHuHang);
	//	break;
	//case CZ_NONGYE:
	//	_tcscpy_s(text, bufLen, m_cz_nongye.m_strKaiHuHang);
	//	break;
	//default:
	//	break;
	//}

	_tcscpy_s(text, bufLen, m_chongzhi.m_strKaiHuHang);
	SetTextToClipboard(text);
}
void CChongZhiDlg::OnBnClickedBtnClose()
{
	CWnd* pParent = GetParent();
	if(pParent!=NULL)
	{
		pParent->PostMessage(IDM_RETURN_GAME,1,0);
	}
	
	return;
}
void CChongZhiDlg::OnBnClickedBtnCpGaneid()
{
	const int bufLen = 1024;
	TCHAR text[bufLen] = _T("");
	/*switch (m_ChongZhiType) 
	{
	case CZ_GONGSHANG:
		_tcscpy_s(text, bufLen, m_cz_gongshang.m_strGameID);
		break;
	case CZ_ZHIFUBAO:
		_tcscpy_s(text, bufLen, m_cz_zhifubao.m_strGameID);
		break;
	case CZ_CAIFUTONG:
		_tcscpy_s(text, bufLen, m_cz_caifutong.m_strGameID);
		break;
	case CZ_JIANSHE:
		_tcscpy_s(text, bufLen, m_cz_jianshe.m_strGameID);
		break;
	case CZ_NONGYE:
		_tcscpy_s(text, bufLen, m_cz_nongye.m_strGameID);
		break;
	default:
		break;
	}*/

	CString sUserID;
	sUserID.Format(_T("%d"), theAccount.user_id);
	_tcscpy_s(text, bufLen, sUserID);
	SetTextToClipboard(text);
}

void CChongZhiDlg::SetTextToClipboard(CString text)
{
	if (::OpenClipboard(m_hWnd)) 
	{
		::EmptyClipboard();

		HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, (_tcslen(text) + 1) * sizeof(TCHAR));
		LPTSTR pData = (LPTSTR)::GlobalLock(hData);
		_tcscpy_s(pData, _tcslen(text) + 1, text);
		::GlobalUnlock(hData);

		SetClipboardData(CF_UNICODETEXT, hData);
		::CloseClipboard();

		CString strTip;
		strTip.Format(_T("已复制【%s】到剪贴板"), text);
		MyMessageBox(strTip);
	}
	else
	{
		MyMessageBox(_T("未能复制，请手工填写"));
	}
}

void CChongZhiDlg::InitBtns()
{
	CString textBk = CBmpUtil::GetExePath() + _T("skin\\yinhang_bt.png");
	m_btnGongShang.SetBkImage(textBk);
	m_btnZhiFuBao.SetBkImage(textBk);
	m_btnCaiFuTong.SetBkImage(textBk);
	m_btnJianShe.SetBkImage(textBk);
	m_btnNongYe.SetBkImage(textBk);

	m_btnChongZhiPage.SetImage(CBmpUtil::GetExePath() + _T("skin\\dkwycz.png"));
	CString cpBtnBk = CBmpUtil::GetExePath() + _T("skin\\fz_bt.png");
	m_btnCPZhangHu.SetImage(cpBtnBk);
	m_btnCPKaiHuRen.SetImage(cpBtnBk);
	m_btnCPYinHang.SetImage(cpBtnBk);
	m_btnCPGameID.SetImage(cpBtnBk);

	m_btnGongShang.SetFont(&m_font);
}
void CChongZhiDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CChongZhiDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

VOID CChongZhiDlg::ShowButton(bool bShow)
{
	m_btnGongShang.EnableTextButton(!bShow);
	m_btnZhiFuBao.EnableTextButton(!bShow);
	m_btnCaiFuTong.EnableTextButton(!bShow);
	m_btnJianShe.EnableTextButton(!bShow);
	m_btnNongYe.EnableTextButton(!bShow);
}

//读取事件
bool CChongZhiDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_CHONG_ZHI_TYPE_RET:
			{
				ASSERT(wDataSize % sizeof(CMD_GP_GetChongzhiTypeRet)==0);
				if(wDataSize % sizeof(CMD_GP_GetChongzhiTypeRet)!=0) return false;

				for (int i = 0;i < 5;i++)
				{
					m_ChongZhiPage[i].m_strKaiHuHang.Empty();
					m_ChongZhiPage[i].m_strKaiHuRen.Empty();
					m_ChongZhiPage[i].m_strZhangHu.Empty();
					m_ChongZhiPage[i].m_strWeb.Empty();

				}

				m_btnGongShang.ShowWindow(SW_HIDE);
				m_btnZhiFuBao.ShowWindow(SW_HIDE);
				m_btnCaiFuTong.ShowWindow(SW_HIDE);
				m_btnJianShe.ShowWindow(SW_HIDE);
				m_btnNongYe.ShowWindow(SW_HIDE);
				WORD wCount = wDataSize/sizeof(CMD_GP_GetChongzhiTypeRet);

				int m_nFirstType = -1;

				for(int i = 0;i < wCount;i++)
				{
					CMD_GP_GetChongzhiTypeRet *pChongzhiTypeRet = ((CMD_GP_GetChongzhiTypeRet*)pData+i);


					m_ChongZhiPage[i].m_strKaiHuHang.Format(L"%s",pChongzhiTypeRet->s_t_yinhang);
					m_ChongZhiPage[i].m_strKaiHuRen.Format(L"%s",pChongzhiTypeRet->s_t_kaihuren);
					m_ChongZhiPage[i].m_strZhangHu.Format(L"%s",pChongzhiTypeRet->s_t_zhanghao);
					m_ChongZhiPage[i].m_strWeb.Format(L"%s",pChongzhiTypeRet->s_t_web);

					bool bEnable = IsEnableZhanghu(m_ChongZhiPage[i]);
 					if(!bEnable)
 						continue;
					if(m_nFirstType == -1)
					{
						m_nFirstType = i;
					}
					switch(i)
					{
					case  CZ_GONGSHANG:
						{
							m_btnGongShang.SetWindowText(m_ChongZhiPage[i].m_strKaiHuHang);
							m_btnGongShang.EnableTextButton(!bEnable);
							m_btnGongShang.ShowWindow(SW_SHOW);
							break;
						}
					case  CZ_ZHIFUBAO:
						{
							m_btnZhiFuBao.SetWindowText(m_ChongZhiPage[i].m_strKaiHuHang);
							m_btnZhiFuBao.EnableTextButton(!bEnable);
							m_btnZhiFuBao.ShowWindow(SW_SHOW);
							break;
						}
					case  CZ_CAIFUTONG:
						{
							m_btnCaiFuTong.SetWindowText(m_ChongZhiPage[i].m_strKaiHuHang);
							m_btnCaiFuTong.EnableTextButton(!bEnable);
							m_btnCaiFuTong.ShowWindow(SW_SHOW);
							break;
						}
					case  CZ_JIANSHE:
						{
							m_btnJianShe.SetWindowText(m_ChongZhiPage[i].m_strKaiHuHang);
							m_btnJianShe.EnableTextButton(!bEnable);
							m_btnJianShe.ShowWindow(SW_SHOW);
							break;
						}
					case  CZ_NONGYE:
						{
							m_btnNongYe.SetWindowText(m_ChongZhiPage[i].m_strKaiHuHang);
							m_btnNongYe.EnableTextButton(!bEnable);
							m_btnNongYe.ShowWindow(SW_SHOW);
							break;
						}
					default:
						break;
					}
				}

				switch(m_nFirstType)
				{
				case  CZ_GONGSHANG:
					{
						OnBnClickedBtnGongshang();
						break;
					}
				case  CZ_ZHIFUBAO:
					{
						OnBnClickedBtnZhifubao();
						break;
					}
				case  CZ_CAIFUTONG:
					{
						OnBnClickedBtnCaifutong();
						break;
					}
				case  CZ_JIANSHE:
					{
						OnBnClickedBtnJianshe();
						break;
					}
				case  CZ_NONGYE:
					{
						OnBnClickedBtnNongye();
						break;
					}
				default:
					break;
				}
				return true;
			}
			break;
		}
	}

// 	CRect rcRedraw;
// 	rcRedraw.left = 393;
// 	rcRedraw.right = rcRedraw.left +300;
// 	rcRedraw.top = 190;
// 	rcRedraw.bottom = rcRedraw.top+165;
// 	RedrawWindow(&rcRedraw,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

	//错误断言
	ASSERT(FALSE);

	return true;
}
bool CChongZhiDlg::IsEnableZhanghu(ChongZhi& chongzhi)
{
	if(chongzhi.m_strKaiHuHang.IsEmpty()||chongzhi.m_strKaiHuRen.IsEmpty()||chongzhi.m_strZhangHu.IsEmpty()||chongzhi.m_strWeb.IsEmpty())
	{
		return false;
	}
	return true;
}

VOID CChongZhiDlg::SendToServer(BYTE nSendType)
{

	ShowButton(false);
	if(nSendType == 1)
	{
		m_bGetChongzhiType = true;
		if(m_bGetChongzhiType)
		{
			CMD_GP_GetChongzhiType GetChongzhiType;
			ZeroMemory(&GetChongzhiType,sizeof(GetChongzhiType));
			GetChongzhiType.nUserID = theAccount.user_id;
			GetChongzhiType.cbType = m_cbChongzhiType;

			CPlatformFrame *pPlatformFrame = CPlatformFrame::GetInstance();
			pPlatformFrame->m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_CHONG_ZHI_TYPE,&GetChongzhiType,sizeof(GetChongzhiType));
			m_bGetChongzhiType = false;
		}
		return;
	}


}