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
	////CImageHandle ImageHandle(&m_ImageBack);

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

	//////创建字体
	////LOGFONT lg;
	////ZeroMemory( &lg,sizeof(lg) );
	////lg.lfCharSet = GB2312_CHARSET;
	////strcpy( lg.lfFaceName,TEXT("黑体") );
	////lg.lfHeight = -20;
	////if( !m_InfoFont.CreateFontIndirect( &lg ) )
	////	m_InfoFont.Attach(CSkinResourceManager::GetDefaultFont());

	//设置透明
	ModifyStyleEx(0,0x00080000);
	HINSTANCE hInst = LoadLibrary(TEXT("User32.DLL")); 
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
	////CImageHandle ImageHandle(&m_ImageBack);
	if (m_ImageBack.IsNull()==false) m_ImageBack.BitBlt(DCBuffer,0,0);

	//位置变量
	int nCardSpace=5;
	int nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
	int nTotalWidth=m_cbWeaveCount*(nItemWidth*3+nCardSpace)+nItemWidth*m_ScoreInfo.cbCardCount+nCardSpace;

	//计算位置
	int nYCardPos=140-g_CardResource.m_ImageTableBottom.GetViewHeight();
	int nXCardPos=(rcClient.Width()-nTotalWidth)/2;

	//绘画组合
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		//绘画扑克
		m_WeaveCard[i].DrawCardControl(&DCBuffer,nXCardPos,nYCardPos,false);

		//设置位置
		nXCardPos+=(nCardSpace+nItemWidth*3);
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
			rcDraw.SetRect(15,148,190,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageFangPao.DrawImage( &DCBuffer,205,150 );
			rcDraw.SetRect(295,148,482,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );
		}
		//自摸
		else
		{
			rcDraw.SetRect(15,148,190,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageZiMo.DrawImage( &DCBuffer,215,150 );
		}
	}

	//得分信息
	DCBuffer.SetTextColor( RGB(250,250,250) );
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		CRect rcDrawPlayer(100,183+i*26,275,210+i*26);
		//玩家
		DCBuffer.DrawText( m_ScoreInfo.szUserName[i],lstrlen(m_ScoreInfo.szUserName[i]),
			&rcDrawPlayer,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );

		//分数
		CRect rcDrawScore(265,183+i*26,355,210+i*26);
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+I64d"),m_ScoreInfo.lGameScore[i] );
		DCBuffer.SetTextColor( RGB(252,255,0) );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),&rcDrawScore,DT_SINGLELINE|DT_BOTTOM|DT_RIGHT|DT_END_ELLIPSIS );

		DCBuffer.SetTextColor( RGB(250,250,250) );
		CRect rcDrawFen(360,183+i*26,512,210+i*26);
		DCBuffer.DrawText( CString(TEXT("分")),&rcDrawFen,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	}

	//牌型信息
	LPCTSTR pszKind[] = {
		TEXT("自摸"),TEXT("单钓将"),TEXT("坎张"),TEXT("边张"),TEXT("无字"),TEXT("缺一门"),TEXT("明杠"),TEXT("幺九刻"),TEXT("老少副"),
		TEXT("连六"),TEXT("喜相逢"),TEXT("一般高"),TEXT("断幺"),TEXT("暗杠"),TEXT("双暗刻"),TEXT("双同刻"),
		TEXT("四归一"),TEXT("平和"),TEXT("门前清"),TEXT("门风刻"),TEXT("圈风刻"),TEXT("箭刻"),TEXT("绝张"),TEXT("双明杠"),TEXT("不求人"),
		TEXT("全带幺"),TEXT("双箭刻"),TEXT("双暗杠"),TEXT("全求人"),TEXT("五门齐"),TEXT("三色三步高"),TEXT("混一色"),TEXT("碰碰和"),TEXT("抢杠和"),
		TEXT("杠上开花"),TEXT("海底捞月"),TEXT("妙手回春"),TEXT("无番和"),TEXT("三色三节高"),TEXT("三色三同顺"),TEXT("推不倒"),TEXT("花龙"),TEXT("三风刻"),
		TEXT("小于五"),TEXT("大于五"),TEXT("组合龙"),TEXT("全不靠"),TEXT("三暗刻"),TEXT("三同刻"),TEXT("全带五"),TEXT("一色三步高"),TEXT("三色双龙会"),
		TEXT("清龙"),TEXT("全小"),TEXT("全中"),TEXT("全大"),TEXT("一色三节高"),TEXT("一色三同顺"),TEXT("清一色"),TEXT("全双"),TEXT("七星不靠"),
		TEXT("七对"),TEXT("混幺九"),TEXT("三杠"),TEXT("一色四步高"),TEXT("一色四节高"),TEXT("一色四同顺"),TEXT("一色双龙会"),TEXT("四暗刻"),TEXT("字一色"),
		TEXT("小三元"),TEXT("小四喜"),TEXT("清幺九"),TEXT("十三幺"),TEXT("连七对"),TEXT("四杠"),TEXT("九莲宝灯"),TEXT("绿一色"),TEXT("大三元"),
		TEXT("大四喜")
	};
	//番数
	BYTE pcbFanCount[] = {
		1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,
		2,2,2,2,2,2,2,2,2,
		4,6,6,6,6,6,6,6,8,
		8,8,8,8,8,8,8,8,12,
		12,12,12,12,16,16,16,16,16,
		16,24,24,24,24,24,24,24,24,
		24,32,32,32,48,48,64,64,64,
		64,64,64,88,88,88,88,88,88,
		88
	};
	//牌型权位
	DWORD pdwRight[] = {
		CHR_ZI_MO,CHR_DAN_DIAO_JIANG,CHR_KAN_ZHANG,CHR_BIAN_ZHANG,CHR_WU_ZI,CHR_QUE_YI_MEN,CHR_MING_GANG,CHR_YAO_JIU,CHR_LAO_SHAO_JIANG,CHR_LIAN_LIU,CHR_XI_XIANG_FENG,
		CHR_YI_BAN_GAO,CHR_DUAN_YAO,CHR_AN_GANG,CHR_SHUANG_AN_KE,CHR_SHUANG_TONG_KE,CHR_SHI_GUI_YI,CHR_PING_HU,CHR_MEN_QI_QING,CHR_MEN_FENG_KE,CHR_QUAN_FENG_KE,CHR_JIAN_KE,
		CHR_JUE_ZHANG,CHR_SHUANG_MING_GANG,CHR_BU_QIU_REN,CHR_QUAN_DAI_YAO,CHR_SHUANG_JIAN_KE,CHR_SHUANG_AN_GANG,CHR_QUAN_QIU_REN,CHR_WU_MEN_QI,CHR_SAN_SE_BU,
		CHR_HUN_YI_SE,CHR_PENG_PENG,CHR_QIANG_GANG,CHR_GANG_KAI,CHR_HAI_DI,CHR_MIAO_SHOU,CHR_WU_FAN,CHR_SAN_SE_JIE,CHR_SAN_SE_TONG,CHR_TUI_BU_DAO,CHR_HUA_LONG,
		CHR_SAN_FENG_KE,CHR_XIAO_WU,CHR_DA_WU,CHR_ZU_HE_LONG,CHR_QUAN_BU_KAO,CHR_SAN_AN_KE,CHR_SAN_TONG_KE,CHR_QUAN_DAI_WU,CHR_YI_SE_BU,CHR_SAN_SE_SHUANG,CHR_QING_LONG,
		CHR_QUAN_XIAO,CHR_QUAN_ZHONG,CHR_QUAN_DA,CHR_YI_SE_JIE,CHR_YI_SE_TONG,CHR_QING_YI_SE,CHR_QUAN_SHUANG,CHR_QI_XING_BU_KAO,CHR_QI_DUI,CHR_HUN_YAO,CHR_SAN_GANG,
		CHR_YI_SHI_BU,CHR_YI_SHI_JIE,CHR_YI_SHI_TONG,CHR_YI_SE_SHUANG,CHR_SI_AN_KE,CHR_ZI_YI_SE,CHR_XIAO_SAN_YUAN,CHR_XIAO_SHI_XI,CHR_QING_YAO_JIU,CHR_SHI_SAN_YAO,CHR_LIAN_QI_DUI,
		CHR_SI_GANG,CHR_JIU_LIAN_DENG,CHR_LU_YI_SE,CHR_DA_SAN_YUAN,CHR_DA_SHI_XI
	};
	//牌型区域
	CRect rcFanXing(34,301,159,327);
	//番数区域
	CRect rcFanShu(160,301,190,327);
	//番区域
	CRect rcFan(195,301,215,327);
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
	if( m_ScoreInfo.cbHuaCardCount > 0 )
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
	}
	//总计
	if( m_ScoreInfo.cbFanCount > 0 )
	{
		DCBuffer.DrawText( TEXT("总计"),lstrlen(TEXT("总计")),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),m_ScoreInfo.cbFanCount );
		DCBuffer.SetTextColor( RGB(252,255,0) );
		DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
		DCBuffer.SetTextColor( RGB(250,250,250) );
		DCBuffer.DrawText( CString(TEXT("番")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
	}

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
