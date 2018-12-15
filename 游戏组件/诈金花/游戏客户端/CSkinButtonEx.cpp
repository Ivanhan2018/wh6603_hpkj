#include "Stdafx.h"
#include "CSkinButtonEx.h"


BEGIN_MESSAGE_MAP(CSkinButtonEx,CSkinButton)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()


//构造函数
CSkinButtonEx::CSkinButtonEx()
{
	m_wID = INVALID_WORD;

}
//析构函数
CSkinButtonEx::~CSkinButtonEx()
{

}
//鼠标按下
void CSkinButtonEx::OnLButtonDown(UINT nFlags, CPoint Point)
{
	SetTimer(IDI_LONG_DOWN,500,NULL);
	__super::OnLButtonDown(nFlags,Point);
}
//鼠标释放
void CSkinButtonEx::OnLButtonUp(UINT nFlags, CPoint Point)
{
	KillTimer(IDI_LONG_DOWN);
	__super::OnLButtonUp(nFlags,Point);
}
//鼠标离开
LRESULT CSkinButtonEx::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	KillTimer(IDI_LONG_DOWN);
	return	__super::OnMouseLeave(wParam,lParam);
}
//鼠标消息
VOID CSkinButtonEx::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags,Point);
}
//定时器消息
void CSkinButtonEx::OnTimer(UINT nIDEvent)
{
	if(IDI_LONG_DOWN == nIDEvent&&m_wID!=INVALID_WORD)
	{
		//长按消息
		if(CGameFrameView::GetInstance()!=NULL)
		{
			CGameFrameView::GetInstance()->PostEngineMessage(IDM_LONG_DOWN,m_wID,0);
		}
		return;
	}
	__super::OnTimer(nIDEvent);
}