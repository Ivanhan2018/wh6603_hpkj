#include "Stdafx.h"
#include "GameScore.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////

//按钮标识
#define IDC_CLOSE_SCORE				100									//关闭成绩

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScore, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameScore::CGameScore() : CDialog(IDD_GAME_SCORE)
{
	//初始化变量
	ZeroMemory( &m_ScoreInfo,sizeof(m_ScoreInfo) );
	m_cbWeaveCount = 0;
	m_ChiHuRight.SetEmpty();

	HINSTANCE hInst = AfxGetInstanceHandle();
	m_ImageBack.LoadFromResource( hInst,IDB_GAME_SCORE );
	m_ImageFangPao.LoadImage( hInst,TEXT("FANG_PAO") );
	m_ImageZiMo.LoadImage( hInst,TEXT("ZI_MO") );
}

//析构函数
CGameScore::~CGameScore()
{
}

//控件绑定
void CGameScore::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btCloseScore);
}

//初始化函数
BOOL CGameScore::OnInitDialog()
{
	__super::OnInitDialog();

	//删除窗口标题和获取属性
	ModifyStyle(WS_CAPTION,0,0);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//加载资源
	//CImageHandle ImageHandle(&m_ImageBack);

	//创建区域
	CRgn RgnControl;
	m_ImageBack.CreateImageRegion(RgnControl,RGB(255,0,255));

	//设置窗口
	if (RgnControl.GetSafeHandle()!=NULL)
	{
		//移动窗口
		SetWindowRgn(RgnControl,TRUE);
		SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE);
	}

	m_btCloseScore.SetButtonImage(IDB_BT_SCORE_CLOSE,AfxGetInstanceHandle(),false,false);

	//调整按钮
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rcBtClose;
	m_btCloseScore.GetWindowRect(&rcBtClose);
	m_btCloseScore.SetWindowPos(NULL,(rcClient.Width()-rcBtClose.Width())/2,rcClient.Height()-rcBtClose.Height()-10,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//创建字体
	LOGFONT lg;
	ZeroMemory( &lg,sizeof(lg) );
	lg.lfCharSet = GB2312_CHARSET;
	lstrcpy( lg.lfFaceName,_T("黑体"));
	lg.lfHeight = -20;
	if( !m_InfoFont.CreateFontIndirect( &lg ) )
		m_InfoFont.Attach(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//设置透明
	ModifyStyleEx(0,0x00080000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL")); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//取得SetLayeredWindowAttributes（）函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)fun(this->GetSafeHwnd(),0,220,2); 
		FreeLibrary(hInst); 
	}

	return TRUE;
}

//复位数据
void CGameScore::RestorationData()
{
	//设置变量
	m_cbWeaveCount=0;
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));

	//组合变量
	for (BYTE i=0;i<4;i++)
	{
		m_WeaveCard[i].SetCardData(NULL,0);
	}

	//隐藏窗口
	if (m_hWnd!=NULL) ShowWindow(SW_HIDE);

	return;
}

//设置积分
void CGameScore::SetScoreInfo(const tagScoreInfo & ScoreInfo, const tagWeaveInfo & WeaveInfo, const CChiHuRight &ChiHuRight)
{
	//设置变量
	m_ScoreInfo=ScoreInfo;
	m_cbWeaveCount=WeaveInfo.cbWeaveCount;
	m_ChiHuRight = ChiHuRight;

	//组合变量
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		bool bPublicWeave=(WeaveInfo.cbPublicWeave[i]==TRUE);
		m_WeaveCard[i].SetCardData(WeaveInfo.cbCardData[i],WeaveInfo.cbCardCount[i]);
		m_WeaveCard[i].SetDisplayItem(true);
	}

	for (BYTE j=0;j<m_cbWeaveCount;j++)
	{
		if(m_WeaveCard[j].GetCardCount()==4)
		{
			for (BYTE t=j+1;t<m_cbWeaveCount;t++)
			{
				m_WeaveCard[t].OffPoint();
			}
		}

	}
	//显示窗口
	ShowWindow(SW_SHOW);

	return;
}

//关闭按钮
void CGameScore::OnOK()
{
	//隐藏窗口
	RestorationData();

	__super::OnOK();
}

//重画函数
void CGameScore::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//设置 DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);
	DCBuffer.SelectObject(m_InfoFont);
	DCBuffer.SetTextColor(RGB(250,250,250));

	//绘画背景
	//CImageHandle ImageHandle(&m_ImageBack);
	if (m_ImageBack.IsNull()==false) m_ImageBack.BitBlt(DCBuffer,0,0);

	//位置变量
	int nCardSpace=5;
	int nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
	int nTotalWidth=m_cbWeaveCount*(nItemWidth*3+nCardSpace)+nItemWidth*m_ScoreInfo.cbCardCount+nCardSpace;

	//计算位置
	int nYCardPos=120-g_CardResource.m_ImageTableBottom.GetViewHeight();
	int nXCardPos=(rcClient.Width()-nTotalWidth)/2;

	//绘画组合
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		//绘画扑克
		m_WeaveCard[i].DrawCardControl(&DCBuffer,nXCardPos,nYCardPos,false);

		//设置位置
		nXCardPos+=(nCardSpace+nItemWidth*m_WeaveCard[i].GetCardCount());
	}

	//绘画扑克
	for (BYTE i=0;i<m_ScoreInfo.cbCardCount;i++)
	{
		//绘画扑克
		g_CardResource.m_ImageTableBottom.DrawCardItem(&DCBuffer,m_ScoreInfo.cbCardData[i],nXCardPos,nYCardPos,false);

		//设置位置
		nXCardPos+=nItemWidth;
		if ((i+2)==m_ScoreInfo.cbCardCount) nXCardPos+=nCardSpace;
	}

	TCHAR szBuffer[32] = TEXT("");
	//胡牌信息
	if( m_ScoreInfo.wProvideUser != INVALID_CHAIR )
	{
		CRect rcDraw;
		//放炮
		if( m_ScoreInfo.wProvideUser != m_ScoreInfo.wChiHuUser )
		{
			rcDraw.SetRect(15,128,190,155);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageFangPao.DrawImage( &DCBuffer,205,130 );
			rcDraw.SetRect(315,128,482,155);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );
		}
		//自摸
		else
		{
			rcDraw.SetRect(15,128,190,155);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageZiMo.DrawImage( &DCBuffer,215,130 );
		}
	}

	//得分信息
	DCBuffer.SetTextColor( RGB(250,250,250) );
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		CRect rcDrawPlayer(100,163+i*26,275,190+i*26);
		//玩家
		DCBuffer.DrawText( m_ScoreInfo.szUserName[i],lstrlen(m_ScoreInfo.szUserName[i]),
			&rcDrawPlayer,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );

		//分数
		CRect rcDrawScore(295,163+i*26,380,190+i*26);
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%I64d"),m_ScoreInfo.lGameScore[i] );
		DCBuffer.SetTextColor( RGB(252,255,0) );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore,DT_SINGLELINE|DT_BOTTOM|DT_RIGHT );

		DCBuffer.SetTextColor( RGB(250,250,250) );
		CRect rcDrawFen(385,163+i*26,512,190+i*26);
		DCBuffer.DrawText( CString(TEXT("分")),&rcDrawFen,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	}

	//牌型信息
	/*LPCTSTR pszKind[] = {
		TEXT("自摸")
	};
	//番数
	BYTE pcbFanCount[] = {1
	};
	//牌型权位
	DWORD pdwRight[] = {CHR_ZI_MO
	};
	//牌型区域
	CRect rcFanXing(34,271,159,297);
	//番数区域
	CRect rcFanShu(160,271,190,297);
	//番区域
	CRect rcFan(195,271,215,297);
	BYTE cbDrawCount = 0;
	for( BYTE i = 0; i < CountArray(pcbFanCount); i++ ) 
	{
		if( !(m_ChiHuRight&pdwRight[i]).IsEmpty() )
		{
			DCBuffer.DrawText( pszKind[i],lstrlen(pszKind[i]),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
			_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),pcbFanCount[i] );
			DCBuffer.SetTextColor( RGB(252,255,0) );
			DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
			DCBuffer.SetTextColor( RGB(250,250,250) );
			DCBuffer.DrawText( CString(TEXT("番")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );

			//移动绘画区域
			if( ++cbDrawCount%2 == 0 )
			{
				rcFanXing.OffsetRect( -245,26 );
				rcFanShu.OffsetRect( -245,26 );
				rcFan.OffsetRect( -245,26 );
			}
			else
			{
				rcFanXing.OffsetRect( 245,0 );
				rcFanShu.OffsetRect( 245,0 );
				rcFan.OffsetRect( 245,0 );
			}
		}
	}
	*/
	/*if( m_ScoreInfo.cbHuaCardCount > 0 )
	{
		DCBuffer.DrawText( TEXT("花"),lstrlen(TEXT("花")),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),m_ScoreInfo.cbHuaCardCount );
		DCBuffer.SetTextColor( RGB(252,255,0) );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
		DCBuffer.SetTextColor( RGB(250,250,250) );
		DCBuffer.DrawText( CString(TEXT("番")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
		//移动绘画区域
		if( ++cbDrawCount%2 == 0 )
		{
			rcFanXing.OffsetRect( -245,26 );
			rcFanShu.OffsetRect( -245,26 );
			rcFan.OffsetRect( -245,26 );
		}
		else
		{
			rcFanXing.OffsetRect( 245,0 );
			rcFanShu.OffsetRect( 245,0 );
			rcFan.OffsetRect( 245,0 );
		}
	}*/
	//总计
	/*if( m_ScoreInfo.cbFanCount > 0 )
	{
		DCBuffer.DrawText( TEXT("总计"),lstrlen(TEXT("总计")),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),m_ScoreInfo.cbFanCount );
		DCBuffer.SetTextColor( RGB(252,255,0) );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
		DCBuffer.SetTextColor( RGB(250,250,250) );
		DCBuffer.DrawText( CString(TEXT("番")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	}*/

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//清理资源
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//鼠标消息
void CGameScore::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//消息模拟
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//背景函数
BOOL CGameScore::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
