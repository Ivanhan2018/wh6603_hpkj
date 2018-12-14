#include "Stdafx.h"
#include "Resource.h"
#include "JettonView.h"



CJettonView::CJettonView()
{
	m_bShow=FALSE;
	m_ptBasePoint.SetPoint(0,0);
	m_sBetScore=SCORE_ZERO;
	m_sCellScore=SCORE_ZERO;
	m_sMaxCellScore=SCORE_ZERO;
}

CJettonView::~CJettonView()
{

}

//创建界面
VOID	CJettonView::InitView(CWnd	*pWnd)
{
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageScoreBack.LoadFromResource(hInstance,IDB_SCORE);
	CRect rcCreate(0,0,0,0);
	for (BYTE i=0;i<CountArray(m_btJetton);i++)
	{
		m_btJetton[i].Create(TEXT(""),WS_CHILD,rcCreate,pWnd,IDC_ONE_SCORE+i);
		m_btJetton[i].SetButtonImage(IDB__MONEY,hInstance,false,false);
		m_btJetton[i].SetButtonColor(RGB(0,0,0));
	}
	m_btAccept.Create(TEXT(" 确 认 "),WS_CHILD,rcCreate,pWnd,IDC_ACCEPT_SCORE);
	m_btAccept.SetButtonImage(IDB__MONEY,hInstance,false,false);
	m_btAccept.SetButtonColor(RGB(0,0,0));

	m_btClean.Create(TEXT(" 清 除 "),WS_CHILD,rcCreate,pWnd,IDC_CLEAN_SCORE);
	m_btClean.SetButtonImage(IDB__MONEY,hInstance,false,false);
	m_btClean.SetButtonColor(RGB(0,0,0));
}

//调整控件
VOID	CJettonView::RectifyControl(int nWidth, int nHeight)
{
	CRect rcButton;
	INT nXSpace=6;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btJetton[0].GetWindowRect(&rcButton);
	//筹码按钮
	DeferWindowPos(hDwp,m_btJetton[0],NULL,nWidth/2-rcButton.Width()*3-2*nXSpace-nXSpace/2,	nHeight-186,0,0,uFlags);
	DeferWindowPos(hDwp,m_btJetton[1],NULL,nWidth/2-rcButton.Width()*2-nXSpace-nXSpace/2,	nHeight-186,0,0,uFlags);
	DeferWindowPos(hDwp,m_btJetton[2],NULL,nWidth/2-rcButton.Width()-nXSpace/2,			nHeight-186,0,0,uFlags);
	DeferWindowPos(hDwp,m_btJetton[3],NULL,nWidth/2+nXSpace/2,								nHeight-186,0,0,uFlags);
	DeferWindowPos(hDwp,m_btJetton[4],NULL,nWidth/2+nXSpace/2+nXSpace+rcButton.Width(),		nHeight-186,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAccept,   NULL,nWidth/2+nXSpace/2+nXSpace*2+rcButton.Width()*2,	nHeight-186,0,0,uFlags);
	DeferWindowPos(hDwp,m_btClean,    NULL,nWidth/2+nXSpace/2+nXSpace*3+rcButton.Width()*3,	nHeight-186,0,0,uFlags);
	
	m_ptBasePoint.SetPoint(nWidth/2-m_ImageScoreBack.GetWidth()/6,nHeight-196-m_ImageScoreBack.GetHeight());
	//结束移动
	EndDeferWindowPos(hDwp);
}
BOOL	CJettonView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{

	case IDC_ONE_SCORE:
		{
			AddScore(m_sCellScore);
			return TRUE;
		}
	case IDC_TWO_SCORE:
		{
			AddScore(m_sCellScore*2);
			return TRUE;
		}
	case IDC_THREE_SCORE:
		{
			AddScore(m_sCellScore*5);
			return TRUE;
		}
	case IDC_FOUR_SCORE	:
		{
			AddScore(m_sCellScore*10);
			return TRUE;
		}
	case IDC_FIVE_SCORE	:
		{
			AddScore(m_sCellScore*20);
			return TRUE;
		}
	case IDC_ACCEPT_SCORE:
		{
			CGameFrameView *pwnd=CGameFrameView::GetInstance();
			if(pwnd!=NULL)
				pwnd->SendEngineMessage(IDM_ADD_SCORE,0,0);
			return TRUE;
		}
	case IDC_CLEAN_SCORE:
		{
			OnCleanScore();
			return TRUE;
		}
	}
	return FALSE;
}
//设置显示
VOID	CJettonView::ShowWindows(BOOL bShow)
{
	if(m_bShow==bShow) return;
	m_bShow=bShow;
	if(m_bShow)
	{
		for (BYTE i=0;i<CountArray(m_btJetton);i++)
		{
			m_btJetton[i].ShowWindow(SW_SHOW);
		}
		m_btAccept.ShowWindow(SW_SHOW);
		m_btClean.ShowWindow(SW_SHOW);
	}
	else
	{
		for (BYTE i=0;i<CountArray(m_btJetton);i++)
		{
			m_btJetton[i].ShowWindow(SW_HIDE);
		}
		m_btAccept.ShowWindow(SW_HIDE);
		m_btClean.ShowWindow(SW_HIDE);
	}
	
}
//绘制
VOID CJettonView::DrawJettonView(CDC *pDC,CDFontEx * pViewFont)
{
	if(m_bShow==FALSE) return;
	//绘画背景
	m_ImageScoreBack.TransDrawImage(pDC,m_ptBasePoint.x,m_ptBasePoint.y,m_ImageScoreBack.GetWidth()/3,
		m_ImageScoreBack.GetHeight(),m_ImageScoreBack.GetWidth()/3,0,RGB(255,0,255));
	CString strScore;
	strScore.Format(TEXT("")SCORE_STRING,m_sBetScore);
	CRect rc(m_ptBasePoint.x,m_ptBasePoint.y+12,m_ptBasePoint.x+m_ImageScoreBack.GetWidth()/3,m_ptBasePoint.y+28);
	pViewFont->DrawText(pDC,strScore,&rc,RGB(250,250,0),DT_CENTER|DT_END_ELLIPSIS);
}

//底注设置
VOID	CJettonView::SetCellScore(SCORE score)
{
	ASSERT(score>SCORE_ZERO);
	m_sCellScore=score;
	CString strscore;
	for (BYTE i=0;i<CountArray(m_btJetton);i++)
	{
		strscore.Format(TEXT("")SCORE_STRING,m_sCellScore*s_nmultiple[i]);
		m_btJetton[i].SetWindowText(strscore);
	}
}
//最大设置
VOID	CJettonView::SetMaxScore(SCORE score)
{
	ASSERT(score>SCORE_ZERO);
	ASSERT(score>m_sCellScore);
	m_sBetScore=m_sCellScore;
	m_sMaxCellScore=score;
	UpDateButton();
}
//获取下注
SCORE	CJettonView::GetBetScore()
{
	if(m_sCellScore-m_sBetScore>0.001)
		m_sBetScore=m_sCellScore;
	if(m_sBetScore-m_sMaxCellScore>0.001)
		m_sBetScore =  m_sMaxCellScore;
	return m_sBetScore;
}

//重置数据
VOID	CJettonView::RestData()
{
	m_sBetScore=SCORE_ZERO;
	m_sMaxCellScore=SCORE_ZERO;
}

//加注
VOID	CJettonView::AddScore(SCORE score)
{
	if(m_sBetScore+score<=m_sMaxCellScore)
	{
		m_sBetScore+=score;
		UpDateButton();
	}
}
VOID CJettonView::OnCleanScore()
{
	m_sBetScore = m_sCellScore;
	UpDateButton();
}
CRect	CJettonView::GetRect()
{
	CRect rc(0,0,0,0);

	rc.SetRect(m_ptBasePoint.x,m_ptBasePoint.y,m_ptBasePoint.x+m_ImageScoreBack.GetWidth()/3,m_ptBasePoint.y+m_ImageScoreBack.GetWidth());
	return  rc;
}

VOID CJettonView::UpDateButton()
{
	WORD wTimes[5]={1,2,5,10,20};
	for(WORD i =0;i<CountArray(m_btJetton);i++)
	{
		if(m_sMaxCellScore-(m_sBetScore+wTimes[i]*m_sCellScore)>0.001)
			m_btJetton[i].EnableWindow(TRUE);
		else
			m_btJetton[i].EnableWindow(FALSE);
	}
}