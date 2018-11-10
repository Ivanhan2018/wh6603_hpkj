#include "stdafx.h"
#include "FanDian.h"


//////////////////////////////////////////////////////////////////////////
#define IDC_CELL_ONE			402
#define IDC_CELL_TWO			403
#define IDC_CELL_THREE			404
#define IDC_CELL_FOUR			405
#define IDC_CELL_FIVE			406
#define IDC_CELL_CLEAN			407



//////////////////////////////////////////////////////////////////////////
	//构造函数
CFanDian::CFanDian()
{
	m_sCellScore=0.10;
	m_bMouseDown=false;
	m_bShowView=FALSE;
	m_ptBasePoint.SetPoint(0,0);
	m_rcCheckScore.SetRect(0,0,0,0);
	EmptyData();
	m_font.CreateFont(12,0,0,0,800,0,0,0,0,0,0,0,0,TEXT("宋体"));
	m_Color=RGB(69,28,0);
}
	//析构函数
CFanDian::~CFanDian()
{
	m_font.DeleteObject();
	m_PngBackView.DestroyImage();
	m_ImageDrag.DestroyImage();
}
	//初始化
BOOL	CFanDian::InitView(CWnd *pParent)
{
	HINSTANCE hInst= AfxGetInstanceHandle();
	//m_PngBackView.LoadImage(pParent,hInst,TEXT("USER_CONTROL"));
// 	m_ImageDrag.LoadImage(pParent,hInst,TEXT("ARROW_CONTROL"));
// 
// 	m_sizeDrag.SetSize(m_ImageDrag.GetWidth(), m_ImageDrag.GetHeight());


	return TRUE;
}
	//命令函数
BOOL	CFanDian::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_CELL_ONE:
		{
			AddScore(m_sCellScore);
			return TRUE;
		}
	case IDC_CELL_TWO:
		{
			AddScore(m_sCellScore*2);
			return TRUE;
		}
	case IDC_CELL_THREE:
		{
			AddScore(m_sCellScore*5);
			return TRUE;
		}
	case IDC_CELL_FOUR:
		{
			AddScore(m_sCellScore*10);
			return TRUE;
		}	
	case IDC_CELL_FIVE:
		{
			AddScore(m_sCellScore*20);
			return TRUE;
		}
	case IDC_CELL_CLEAN:
		{
			CleanScore();
			return TRUE;
		}

	}
	return FALSE;
}
	// 消息解释
BOOL	CFanDian::PreTranslateMessage(MSG * pMsg)
{
	if ( pMsg->message == WM_LBUTTONDOWN )
	{
		CPoint ptMouse(LOWORD(pMsg->lParam) , HIWORD(pMsg->lParam));
		if( m_rectDrag.PtInRect(ptMouse) )
		{
			m_bMouseDown = true;
			m_lDragSite = ptMouse.x;
			SetDragScore();
			return TRUE;
		}
	}
	else if ( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint ptMouse(LOWORD(pMsg->lParam) , HIWORD(pMsg->lParam));
		if ( m_bMouseDown  )
		{
			if(m_rectDrag.PtInRect(ptMouse))
			{
				m_lDragSite = ptMouse.x;
				SetDragScore();
				return TRUE;
			}else
			{
				m_bMouseDown=false;
				return FALSE;
			}
		}
	}
	else if ( pMsg->message == WM_LBUTTONUP || pMsg->message == WM_KILLFOCUS )
	{
		m_bMouseDown = false;
		return FALSE;
	}
	return FALSE;
}

//	按键消息
BOOL	CFanDian::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch( nChar ) {


		case VK_LEFT :
		case VK_UP :

			MinusScore( m_sCellScore);
			return TRUE;


		case VK_RIGHT :
		case VK_DOWN :

			AddScore( m_sCellScore );
			return TRUE;

// 		case VK_RETURN:
// 			if(CGameFrameView::GetInstance()!=NULL)
// 				CGameFrameView::GetInstance()->SendEngineMessage(IDM_OK_SCORE,0,0);
			return TRUE;
		}
	return FALSE;
}
	//绘制界面
VOID	CFanDian::DrawView(CWnd *pWnd,CDC * pDC, INT nWidth, INT nHeight)
{
	if(!m_bShowView) return;

	//m_PngBackView.DrawImage(pDC,m_ptBasePoint.x,m_ptBasePoint.y);
	
	int nDragX = m_lDragSite;
	int nOffSet = m_sizeDrag.cx / 2;
	if ( nDragX < m_rectDrag.left + nOffSet )
		nDragX = m_rectDrag.left + nOffSet;
	else if ( nDragX > m_rectDrag.right - nOffSet )
		nDragX = m_rectDrag.right - nOffSet;
	//m_ImageDrag.DrawImage( pDC, nDragX - nOffSet, m_rectDrag.top );


	CString strCheckScore;
	strCheckScore.Format(TEXT("")SCORE_STRING,m_sCheck);

	CDFontEx::DrawText(pWnd, pDC, 12, 800,strCheckScore,&m_rcCheckScore, m_Color, DT_CENTER|DT_END_ELLIPSIS);


	return;
}
	//调整控件
VOID	CFanDian::RectifyControl(INT nWidth, INT nHeight)
{
	m_ptBasePoint.SetPoint(nWidth/2 + 100, nHeight/2+78);

	//按钮控件
	CRect rcButton;
	m_JettonButton[0].GetWindowRect(&rcButton);

	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	for (BYTE i=0;i<CountArray(m_JettonButton);i++)
	{
		DeferWindowPos( hDwp, m_JettonButton[i],  NULL, m_ptBasePoint.x + 15+(i%2!=0?88:0), m_ptBasePoint.y + 62 + (rcButton.Height()+2)*(i/2), 0, 0, uFlags );
	}

	//结束移动
	EndDeferWindowPos(hDwp);

	m_lDragSite += m_ptBasePoint.x + 25;

//	m_rectDrag.SetRect( m_ptBasePoint.x + 25, m_ptBasePoint.y + 32, m_ptBasePoint.x + 183, m_ptBasePoint.y + m_ImageDrag.GetHeight()+32 );

	m_rcCheckScore.SetRect(m_ptBasePoint.x+25, m_ptBasePoint.y+15,m_ptBasePoint.x+183, m_ptBasePoint.y+ 31);

}
	//显示控制
VOID	CFanDian::ShowWindows(BOOL bshow)
{
	if(bshow==m_bShowView) return;
	m_bShowView=bshow;
	if(m_bShowView)
	{
		for (BYTE i=0;i<CountArray(m_JettonButton);i++)
		{
			m_JettonButton[i].ShowWindow(SW_SHOW);
			m_JettonButton[i].EnableWindow(TRUE);	
		}

	}
	else
	{
		for (BYTE i=0;i<CountArray(m_JettonButton);i++)
		{
			m_JettonButton[i].ShowWindow(SW_HIDE);	
		}				
	}
}
	//清空数据
VOID	CFanDian::EmptyData()
{
	m_sLimitMax=SCORE_ZERO;	
	m_sLimitMin=SCORE_ZERO;			
	m_sCheck=SCORE_ZERO;
	m_sCellScore=SCORE_ZERO;
	return;
}
	//设置信息
VOID	CFanDian::SetSocreInfo(SCORE slimitmax,SCORE slimitmin)
{
	SetLimitMinScore(slimitmin);
	SetLimitMaxScore(slimitmax);
	SetCheckScore(slimitmin);
	UpdataDrag();
	return;
}
	//加注操作
VOID	CFanDian::AddScore(SCORE saddscore)
{
	if(m_sLimitMax-m_sCheck-saddscore>0.001)
		m_sCheck+=saddscore;
	else
		m_sCheck=m_sLimitMax;
	UpdataDrag();
	return;
}
	//减注操作
VOID	CFanDian::MinusScore(SCORE sminusscore)
{
	if(m_sCheck-sminusscore-m_sLimitMin>0.001)
		m_sCheck-=sminusscore;
	else
		m_sCheck=m_sLimitMin;
	UpdataDrag();
	return;
}
	//还原最小
VOID	CFanDian::CleanScore()
{
	m_sCheck=m_sLimitMin;
	UpdataDrag();
	return;
}
	//显示区域
CRect	 CFanDian::GetRect()
{
	CRect rc(0,0,0,0);
	if(!m_PngBackView.IsNull())
		rc.SetRect(m_ptBasePoint.x,m_ptBasePoint.y,m_ptBasePoint.x+m_PngBackView.GetWidth(),m_ptBasePoint.y+m_PngBackView.GetHeight());
	return rc;
}
	//更新滑块
VOID	 CFanDian::UpdataDrag()
{
	if(m_sLimitMax - m_sLimitMin>0.001)
		m_lDragSite=m_rectDrag.left+ m_rectDrag.Width()*(m_sCheck-m_sLimitMin)/(m_sLimitMax - m_sLimitMin);
}
	//拖动设置
VOID    CFanDian::SetDragScore()
{
	int nWidth = m_rectDrag.Width() - m_sizeDrag.cx;
	int nOffSet = m_sizeDrag.cx / 2;
	int nDragX = m_lDragSite;
	if ( nDragX < m_rectDrag.left + nOffSet )
		nDragX = m_rectDrag.left + nOffSet;
	else if ( nDragX > m_rectDrag.right - nOffSet )
		nDragX = m_rectDrag.right - nOffSet;

	nDragX -= m_rectDrag.left + nOffSet;
	SCORE lCurrentScore = SCORE_ZERO;
	if(nDragX == 0)
	{
		lCurrentScore = m_sLimitMin;
	}
	else if (nDragX == nWidth)
	{
		lCurrentScore = m_sLimitMax;
	}
	else
	{
		lCurrentScore = KeepOne(((m_sLimitMax - m_sLimitMin) * ( (SCORE)nDragX/(SCORE)nWidth )) + m_sLimitMin);
	}
	

	SetCheckScore(lCurrentScore);
}
	//设置下注
VOID	CFanDian::SetCheckScore(SCORE scheck)
{

	if(m_sLimitMin-scheck>0.001)
		m_sCheck=m_sLimitMin;
	else if(scheck-m_sLimitMax>0.001)
		m_sCheck=m_sLimitMax;	
	else
		m_sCheck=scheck;
	return;
}
	//设置上限
VOID	CFanDian::SetLimitMaxScore(SCORE slimitmax)
{
	if(slimitmax-m_sLimitMin>0.001)
		m_sLimitMax=slimitmax;
	else
		m_sLimitMax=m_sLimitMin;
	return;
}
	//设置下限
VOID	CFanDian::SetLimitMinScore(SCORE slimitmin)
{
	m_sLimitMin=slimitmin;
	return;
}

	//单元积分
VOID	CFanDian::SetCellScore(SCORE	sCellScore)
{
	ASSERT(sCellScore>0.001);
	m_sCellScore=__max(0.1000,sCellScore);
	CString strButtonText;
	DOUBLE d[5]={1.000,2.000,5.000,10.000,20.000};
	for (BYTE i=0;i<CountArray(m_JettonButton)-1;i++)
	{
		strButtonText.Format(TEXT("")SCORE_STRING,sCellScore*d[i]);
		m_JettonButton[i].SetWindowText(strButtonText);
	}	
}

	//获取下注
SCORE	CFanDian::GetCheckScore()
{
	CString str;
	str.Format(TEXT("")SCORE_STRING,m_sCheck);
	return _tcstod(str,NULL);
}