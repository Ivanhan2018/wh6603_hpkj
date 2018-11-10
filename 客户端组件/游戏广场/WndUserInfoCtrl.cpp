#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgService.h"
#include "PlatformFrame.h"
#include "DlgLockMachine.h"
#include "WndUserInfoCtrl.h"
#include "ZhuanhuanDlg.h"
//////////////////////////////////////////////////////////////////////////////////

//控制按钮
#define IDC_USER_INFO_1				100									//用户按钮
#define IDC_USER_INFO_2				101									//用户按钮
#define IDC_USER_INFO_3				102									//用户按钮
#define IDC_BTN_REFRESH				103									//刷新按钮
#define IDC_USER_INFO_4				104									//用户按钮
#define IDC_BTN_CHANGE				105									//转换按钮

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndUserInfoCtrl, CWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()

	//自定消息
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

	

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWndUserInfoCtrl::CWndUserInfoCtrl()
{
	m_bHitUserInfo1 = true;
	m_bHitUserInfo2 = false;
	m_bHitUserInfo3 = false;
	m_bHitUserInfo4 = false;
	m_dwTickCount = 0;
	//变量定义
	m_pDlgInsurePlaza = NULL;
	m_fUserScore=0.0f;
	m_cbScoreType=1;
	theAccount.Scoretype=MoShi_Yuan;
}

//析构函数
CWndUserInfoCtrl::~CWndUserInfoCtrl()
{
	if (m_Face != NULL)
	{
		delete m_Face;
		m_Face = NULL;
	}

}
void CWndUserInfoCtrl::ReleaseFace()
{
	if (m_Face != NULL)
	{
		delete m_Face;
		m_Face = NULL;
	}

}
void CWndUserInfoCtrl::SetClicked(BOOL bShow, UINT nStatus)
{

	OnCommand(100,0);

}
//命令函数
BOOL CWndUserInfoCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//功能按钮
	switch (nCommandID)
	{
	case IDC_USER_INFO_1:		//个人中心
		{
			theAccount.Scoretype=MoShi_Yuan;
			m_cbScoreType=1;
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
 			m_bHitUserInfo1 = true;
 			m_bHitUserInfo2 = false;
 			m_bHitUserInfo3 = false;
			m_bHitUserInfo4 = false;


			CWnd* pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_FRESH_GAME,0,0);
			}

						
			return TRUE;
		}
	case IDC_USER_INFO_2:		//财务信息
		{
			theAccount.Scoretype=MoShi_Jiao;
			m_cbScoreType=2;
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

 			m_bHitUserInfo1 = false;
 			m_bHitUserInfo2 = true;
 			m_bHitUserInfo3 = false;
			m_bHitUserInfo4 = false;


			CWnd* pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_FRESH_GAME,0,0);
			}

			break;
		}
	case IDC_USER_INFO_3:		//历史记录
		{
			theAccount.Scoretype=MoShi_Fen;
			m_cbScoreType=3;
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
 			m_bHitUserInfo1 = false;
 			m_bHitUserInfo2 = false;
 			m_bHitUserInfo3 = true;
			m_bHitUserInfo4 = false;


			CWnd* pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_FRESH_GAME,0,0);
			}

			return TRUE;
		}
	case IDC_USER_INFO_4:		//历史记录
		{
			theAccount.Scoretype=MoShi_Li;
			m_cbScoreType=4;
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
			m_bHitUserInfo1 = false;
			m_bHitUserInfo2 = false;
			m_bHitUserInfo3 = false;
			m_bHitUserInfo4 = true;


			CWnd* pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_FRESH_GAME,0,0);
			}

			return TRUE;
		}
	case IDC_BTN_REFRESH:
		{
			DWORD dwTickCount = GetTickCount();
			if ((dwTickCount-m_dwTickCount)<3000)
			{
				return TRUE;
			}
			m_dwTickCount = dwTickCount;

			CWnd* pParent = GetParent();
			if(pParent!=NULL)
			{
				pParent->PostMessage(IDM_REFRESH_YUE,0,0);
			}
			return TRUE;
		}
	case IDC_BTN_CHANGE:
		{
			//用户信息
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			CZhuanhuanDlg Zhuanhuan;
			Zhuanhuan.m_fJine = theAccount.yue;
			Zhuanhuan.m_fQpJine = pGlobalUserData->lScore;
			Zhuanhuan.m_nType = 1;
			if(Zhuanhuan.DoModal() == IDOK)
			{
				DOUBLE fJine = 0.0;
				int nType = 0;
				fJine = Zhuanhuan.m_fJiaoYiJine;
				nType = Zhuanhuan.m_nType;

				CWnd* pParent = GetParent();
				if(pParent!=NULL)
				{
					if(nType == 1)
					{
						pParent->PostMessage(IDM_ZHUANHUAN_TO_CAIPIAO,nType,fJine*1000);
					}
					else if(nType == 2)
					{
						pParent->PostMessage(IDM_ZHUANHUAN_TO_QIPAI,nType,fJine*1000);

					}
				}

			}

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//位置消息
VOID CWndUserInfoCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//建立消息
INT CWndUserInfoCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	m_Font.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("微软雅黑"));

	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//加载资源
	CPngImage ImageTitleM;
	ImageTitleM.LoadImage(AfxGetInstanceHandle(),TEXT("USER_INFO_MALE"));
	if(pGlobalUserData!=NULL)
	{
		if(pGlobalUserData->cbGender == 1)
			ImageTitleM.LoadImage(AfxGetInstanceHandle(),TEXT("USER_INFO_FEMALE"));
	}

	//创建按钮
	CRect rcCreate(0,0,0,0);

	m_btnFlush.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_REFRESH);
	m_btnChange.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BTN_CHANGE);

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();

	m_btnFlush.SetButtonImage(IDB_BK_FRESH,TEXT("BT_USER_FRESH"),hInstance,true,false);
	m_btnChange.SetButtonImage(IDB_BT_CHANGE,TEXT("BT_USER_CHANGE"),hInstance,true,false);

	//移动窗口
	CRect rcUserInfo;
	GetWindowRect(&rcUserInfo);
	SetWindowPos(NULL,0,0,rcUserInfo.Width(),ImageTitleM.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//调整控件
VOID CWndUserInfoCtrl::RectifyControl(INT nWidth, INT nHeight)
{
	//变量定义
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER|SWP_NOSIZE;


	DeferWindowPos(hDwp,m_btnFlush,NULL,183,136,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnChange,NULL,72,194,0,0,uFlags);
	//结束调整
	EndDeferWindowPos(hDwp);

	return;
}


//转换字符
VOID CWndUserInfoCtrl::SwitchScoreFormat(SCORE lScore, UINT uSpace,UINT uDecimalCount,LPTSTR pszBuffer, WORD wBufferSize)
{
	//转换数值
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%0.3f"),lScore);

	//变量定义
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore)-uDecimalCount-1;

	//转换字符
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//字符比较
		if(szSwitchScore[wSourceIndex]=='.') break;

		//拷贝字符
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

	}

	//补充小数
	for(BYTE i=0;i<uDecimalCount+1;i++)
		pszBuffer[wTargetIndex++] = szSwitchScore[wSourceIndex++];

	//结束字符
	pszBuffer[wTargetIndex++]=0;

	return;
}

//绘画背景
BOOL CWndUserInfoCtrl::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建 DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//变量定义
	HINSTANCE hInstance=AfxGetInstanceHandle();
	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("USER_INFO_MALE"));
	ImageBack.DrawImage(pBufferDC,0,0);
	if(pGlobalUserData!=NULL)
	{
		Graphics graphics(pBufferDC->m_hDC);

		if(m_Face == NULL)
		{
			if(theAccount.cbgender == 1)
				m_Face = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\female.png"));
			else if (theAccount.cbgender == 0)
			{
				m_Face = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\male.png"));
			}

		}

		CRect rcRect;
		rcRect.SetRect(0,0, m_Face->GetWidth(),  m_Face->GetHeight());
		graphics.DrawImage(m_Face, Rect(18,12, m_Face->GetWidth(),  m_Face->GetHeight()), 0, 0, m_Face->GetWidth(),  m_Face->GetHeight(), UnitPixel);
	}

	//设置环境
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SetTextColor(RGB(244,248,161));
	pBufferDC->SelectObject(m_Font);


	//绘画用户
	if (pGlobalUserData->dwUserID!=0L)
	{
		m_fUserScore = theAccount.dongjie;
		//构造信息
		TCHAR szScore[32]=TEXT("");
		TCHAR szInsure[32]=TEXT("");
		TCHAR szGameScore[32]=TEXT("");
		_sntprintf(szInsure,CountArray(szInsure),TEXT("%ld"),pGlobalUserData->dwUserID);
		SwitchScoreFormat(pGlobalUserData->lScore,3,2,szScore,CountArray(szGameScore));
		_tcscat(szScore ,TEXT("金币"));

		m_fUserScore = theAccount.yue;
		SwitchScoreFormat(m_fUserScore,3,3,szGameScore,CountArray(szGameScore));
		_tcscat(szGameScore ,TEXT("元"));
		//冻结
		CRect rcDongjie;
		rcDongjie.SetRect(85,168,rcClient.Width()-18,189);
		pBufferDC->DrawText(szScore,lstrlen(szScore),&rcDongjie,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//余额
		CRect rcYue;
		rcYue.SetRect(85,139,rcClient.Width()-18,160);
		pBufferDC->DrawText(szGameScore,lstrlen(szGameScore),&rcYue,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		pBufferDC->SetTextColor(RGB(255,255,255));

		//游戏币 
		CRect rcScore;
		rcYue.SetRect(85,110,rcClient.Width()-18,131);
		pBufferDC->DrawText(szInsure,lstrlen(szInsure),&rcYue,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//输出帐号
		CRect rcAccounts;
		rcAccounts.SetRect(85,82,rcClient.Width()-18,103);
		pBufferDC->DrawText(pGlobalUserData->szAccounts,lstrlen(pGlobalUserData->szAccounts),&rcAccounts,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//事件消息
LRESULT CWndUserInfoCtrl::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//登录成功
		{
			//更新窗口
			Invalidate();


			return 0L;
		}
	case EVENT_USER_LOGOUT:			//注销成功
		{
			//更新窗口
			Invalidate();


			return 0L;
		}
	case EVENT_USER_INFO_UPDATE:	//资料更新
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			ASSERT(pGlobalUserData!=NULL);

			//更新银行
			if(m_pDlgInsurePlaza!=NULL) 
				m_pDlgInsurePlaza->OnEventUpdateInsurePlaza(pGlobalUserData->lScore,pGlobalUserData->lInsure);

			//更新窗口
			Invalidate();

			return 0L;
		}
	}

	return 0L;
}


//////////////////////////////////////////////////////////////////////////////////
