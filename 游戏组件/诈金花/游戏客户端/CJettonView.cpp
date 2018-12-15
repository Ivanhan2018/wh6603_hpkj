#include "stdafx.h"
#include "CJettonView.h"


//////////////////////////////////////////////////////////////////////////
#define IDC_CELL_ONE			402
#define IDC_CELL_TWO			403
#define IDC_CELL_THREE			404
#define IDC_CELL_FOUR			405
#define IDC_CELL_FIVE			406
#define IDC_CELL_CLEAN			407

//#define IDC_SOCRE_MAX
//#define IDC_SOCRE_MIN
//#define IDC_SOCRE_ACCPET
#define IDC_SOCRE_ADD			408
#define IDC_SOCRE_MINUS			409


//////////////////////////////////////////////////////////////////////////
	//构造函数
CJettonView::CJettonView()
{
	m_sCellScore=1;
	m_bMouseDown=false;
	m_bShowView=FALSE;
	m_ptBasePoint.SetPoint(0,0);
	m_rcCheckScore.SetRect(0,0,0,0);
	m_font.CreateFont(14,0,0,0,800,0,0,0,0,0,0,0,0,TEXT("宋体"));
	m_Color=RGB(0,0,0);
	EmptyData();
}
	//析构函数
CJettonView::~CJettonView()
{
	m_font.DeleteObject();
}
	//初始化
BOOL	CJettonView::InitView(CWnd *pParent)
{
	HINSTANCE hInst= AfxGetInstanceHandle();
	m_PngBackView.LoadImage(hInst,TEXT("USER_CONTROL"));
	m_ImageDrag.LoadImage(hInst,TEXT("ARROW_CONTROL"));

	m_sizeDrag.SetSize(m_ImageDrag.GetWidth(), m_ImageDrag.GetHeight());

	CRect rcCreate(0,0,0,0);
	
	for (BYTE i=0;i<CountArray(m_JettonButton)-1;i++)
	{
		m_JettonButton[i].Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,pParent,IDC_CELL_ONE+i);
		m_JettonButton[i].SetButtonImage(IDB_BT_JETTON,hInst,false,false);
		m_JettonButton[i].SetButtonColor(m_Color);
		m_JettonButton[i].SetFont(&m_font);
	}	
	m_JettonButton[5].Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,pParent,IDC_CELL_CLEAN);
	m_JettonButton[5].SetButtonImage(IDB_BT_CLEAN,hInst,false,false);
	
	return TRUE;
}
	//命令函数
BOOL	CJettonView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_CELL_ONE:
		{
			AddScore(m_sCellScore*(m_bMingZhu==TRUE?2:1));
			return TRUE;
		}
	case IDC_CELL_TWO:
		{
			AddScore(m_sCellScore*2*(m_bMingZhu==TRUE?2:1));
			return TRUE;
		}
	case IDC_CELL_THREE:
		{
			AddScore(m_sCellScore*5*(m_bMingZhu==TRUE?2:1));
			return TRUE;
		}
	case IDC_CELL_FOUR:
		{
			AddScore(m_sCellScore*10*(m_bMingZhu==TRUE?2:1));
			return TRUE;
		}	
	case IDC_CELL_FIVE:
		{
			AddScore(m_sCellScore*20*(m_bMingZhu==TRUE?2:1));
			return TRUE;
		}
	case IDC_CELL_CLEAN:
		{
			CleanScore();
			return TRUE;
		}
	case IDC_SOCRE_ADD:
		{
			AddScore(m_sCellScore);
			return TRUE;
		}
	case IDC_SOCRE_MINUS:
		{
			MinusScore(m_sCellScore);
			return TRUE;
		}
	}
	return FALSE;
}
	// 消息解释
BOOL	CJettonView::PreTranslateMessage(MSG * pMsg)
{
	if(m_bShowView==FALSE) return FALSE;
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
	//绘制界面
VOID	CJettonView::DrawView(CWnd * pWnd,CDC * pDC, INT nWidth, INT nHeight)
{
	if(!m_bShowView) return;

	m_PngBackView.DrawImage(pDC,m_ptBasePoint.x,m_ptBasePoint.y);
	
	int nDragX = m_lDragSite;
	int nOffSet = m_sizeDrag.cx / 2;
	if ( nDragX < m_rectDrag.left + nOffSet )
		nDragX = m_rectDrag.left + nOffSet;
	else if ( nDragX > m_rectDrag.right - nOffSet )
		nDragX = m_rectDrag.right - nOffSet;
	m_ImageDrag.DrawImage( pDC, nDragX - nOffSet, m_rectDrag.top );

	CString strCheckScore;
	strCheckScore.Format(TEXT("")SCORE_STRING,m_sCheck/100.00);

	CDFontEx::DrawText(pWnd, pDC, 14, 800,strCheckScore,&m_rcCheckScore, m_Color, DT_CENTER|DT_END_ELLIPSIS);
	

	return;
}
	//调整控件
VOID	CJettonView::RectifyControl(INT nWidth, INT nHeight)
{
	m_ptBasePoint.SetPoint(nWidth/2 + 100, nHeight/2+78);

	//按钮控件
	CRect rcButton;
	m_JettonButton[0].GetWindowRect(&rcButton);

	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	for (BYTE i=0;i<CountArray(m_JettonButton);i++)
	{
		DeferWindowPos( hDwp, m_JettonButton[i],  NULL, m_ptBasePoint.x + 20+(i%2!=0?85:0), m_ptBasePoint.y + 65 + (rcButton.Height()+4)*(i/2), 0, 0, uFlags );
	}

	//结束移动
	EndDeferWindowPos(hDwp);
	CRect rcButtonOver;
	m_JettonButton[0].GetWindowRect(&rcButtonOver);

	m_lDragSite += rcButtonOver.left - rcButton.left;

	m_rectDrag.SetRect( m_ptBasePoint.x + 26, m_ptBasePoint.y + 33, m_ptBasePoint.x + 182, m_ptBasePoint.y + 33+ m_ImageDrag.GetHeight() );

	m_rcCheckScore.SetRect(m_ptBasePoint.x+30, m_ptBasePoint.y+15,m_ptBasePoint.x+180, m_ptBasePoint.y+31);

}
	//显示控制
VOID	CJettonView::ShowWindows(BOOL bshow)
{
	if(bshow==m_bShowView) return;
	m_bShowView=bshow;
	if(m_bShowView)
	{
		for (BYTE i=0;i<CountArray(m_JettonButton);i++)
		{
			//if(i==4) continue;
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
VOID	CJettonView::EmptyData()
{
	m_sLimitMax=0L;	
	m_sLimitMin=0L;			
	m_sCheck=0L;
	m_sCellScore=0L;
	return;
}
	//设置信息
VOID	CJettonView::SetSocreInfo(LONGLONG slimitmax,LONGLONG slimitmin,BOOL	bMingZhu)
{
	m_bMingZhu = bMingZhu;
	SetLimitMinScore(slimitmin*(m_bMingZhu==TRUE?2:1));
	SetLimitMaxScore(slimitmax*(m_bMingZhu==TRUE?2:1));
	SetCheckScore(slimitmin*(m_bMingZhu==TRUE?2:1));
	UpdataDrag();
	return;
}
	//加注操作
VOID	CJettonView::AddScore(LONGLONG saddscore)
{
	if(m_sCheck+saddscore<m_sLimitMax)
		m_sCheck+=saddscore;
	else
		m_sCheck=m_sLimitMax;
	UpdataDrag();
	return;
}
	//减注操作
VOID	CJettonView::MinusScore(LONGLONG sminusscore)
{
	if(m_sCheck-sminusscore>m_sLimitMin)
		m_sCheck-=sminusscore;
	else
		m_sCheck=m_sLimitMin;
	UpdataDrag();
	return;
}
	//还原最小
VOID	CJettonView::CleanScore()
{
	m_sCheck=m_sLimitMin;
	UpdataDrag();
	return;
}
	//显示区域
CRect	 CJettonView::GetRect()
{
	CRect rc(0,0,0,0);
	if(!m_PngBackView.IsNull())
		rc.SetRect(m_ptBasePoint.x,m_ptBasePoint.y,m_ptBasePoint.x+m_PngBackView.GetWidth(),m_ptBasePoint.y+m_PngBackView.GetHeight());
	return rc;
}
	//更新滑块
VOID	 CJettonView::UpdataDrag()
{
	if(m_sLimitMax==m_sLimitMin)
		m_lDragSite=m_rectDrag.left;
	else
		m_lDragSite=m_rectDrag.left+ m_rectDrag.Width()*(m_sCheck-m_sLimitMin)/(m_sLimitMax - m_sLimitMin);
}
	//拖动设置
VOID    CJettonView::SetDragScore()
{



	int nWidth = m_rectDrag.Width() - m_sizeDrag.cx;
	int nOffSet = m_sizeDrag.cx / 2;
	int nDragX = m_lDragSite;
	if ( nDragX < m_rectDrag.left + nOffSet )
		nDragX = m_rectDrag.left + nOffSet;
	else if ( nDragX > m_rectDrag.right - nOffSet )
		nDragX = m_rectDrag.right - nOffSet;

	nDragX -= m_rectDrag.left + nOffSet;

	LONGLONG lCurrentScore = 0;
	//梦成网络修正((SCORE)nDragX/(SCORE)nWidth)) 原来是整形除出来的数不能为小数所有这里不行
	lCurrentScore = ((m_sLimitMax - m_sLimitMin)*((SCORE)nDragX/(SCORE)nWidth)) + m_sLimitMin;

	//以倍数显示
	LONGLONG l=lCurrentScore/m_sCellScore*m_sCellScore;
	//if(lCurrentScore>=m_sCellScore)		l=m_sCellScore;
	//if(lCurrentScore>=2*m_sCellScore)		l=2*m_sCellScore;
	//if(lCurrentScore>=3*m_sCellScore)		l=3*m_sCellScore;
	//if(lCurrentScore>=4*m_sCellScore)		l=4*m_sCellScore;
	//if(lCurrentScore>=5*m_sCellScore)		l=5*m_sCellScore;
	//if(lCurrentScore>=6*m_sCellScore)		l=6*m_sCellScore;
	//if(lCurrentScore>=10*m_sCellScore)	l=10*m_sCellScore;
	//if(lCurrentScore>=20*m_sCellScore)	l=20*m_sCellScore;
	//LONGLONG l=lCurrentScore;
	
	
	LONGLONG c=m_sCellScore;

	


	SetCheckScore(m_sCellScore*l/c);

	//CString ss;
	//ss.Format(_T("%d"),m_sCellScore*l/c);
	//AfxMessageBox(ss);


}
	//设置下注
VOID	CJettonView::SetCheckScore(LONGLONG scheck)
{

	if(scheck<m_sLimitMin)
		m_sCheck=m_sLimitMin;
	else if(scheck>m_sLimitMax)
		m_sCheck=m_sLimitMax;	
	else
		m_sCheck=scheck;
	return;
}
	//设置上限
VOID	CJettonView::SetLimitMaxScore(LONGLONG slimitmax)
{
	if(slimitmax>=m_sLimitMin)
		m_sLimitMax=slimitmax;
	else
		m_sLimitMax=m_sLimitMin;
	return;
}
	//设置下限
VOID	CJettonView::SetLimitMinScore(LONGLONG slimitmin)
{
	m_sLimitMin=slimitmin;
	return;
}

	//单元积分
VOID	CJettonView::SetCellScore(LONGLONG	sCellScore)
{
	ASSERT(sCellScore>0);
	m_sCellScore=__max(1,sCellScore);
	CString strButtonText;
	LONGLONG d[5]={1,2,5,10,20};
	for (BYTE i=0;i<CountArray(m_JettonButton)-1;i++)
	{
		strButtonText.Format(TEXT("")SCORE_STRING,(sCellScore*d[i])/100.);
		m_JettonButton[i].SetWindowText(strButtonText);
	}	
}

	//获取下注
LONGLONG	CJettonView::GetCheckScore()
{
	return m_sCheck;
}

VOID CJettonView::UpdateButton(BOOL bShow[4])
{
	if(bShow==NULL)
	{
		for (BYTE i=0;i<CountArray(m_JettonButton)-2;i++)
		{
			m_JettonButton[i].EnableWindow(FALSE);
		}	
	}
	else
	{
		for (BYTE i=0;i<CountArray(bShow);i++)
		{
			m_JettonButton[i].EnableWindow(bShow[i]);
		}	
	}
}