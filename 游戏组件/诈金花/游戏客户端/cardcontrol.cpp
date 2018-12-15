#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"
#include ".\cardcontrol.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
bool					CCardControl::m_bLoad=false;					//初始标志
CSize					CCardControl::m_CardSize;						//扑克大小
CPngImage				CCardControl::m_ImageCard;						//扑克图片

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

//构造函数
CCardControl::CCardControl()
{
	//配置变量
	m_bPositively=false;
	m_bDisplayHead=false;
	m_bLookCard=false;
	m_bCompareCard=false;
	m_bCompareBack=false;
	m_wCardColor=INVALID_CHAIR;
	m_dwCardHSpace=DEFAULT_PELS;
	m_wControlID=INVALID_CHAIR;
	m_wMessageID=INVALID_CHAIR;

	//状态变量
	m_bCaptureMouse=false;

	//位置变量
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;

	//资源变量
	if (m_bLoad==false)
	{
		//加载资源
		m_bLoad=true;
		m_ImageCard.LoadImage(AfxGetInstanceHandle(),TEXT("CARD"));

		//获取大小
		m_CardSize.cx=m_ImageCard.GetWidth()/13;
		m_CardSize.cy=m_ImageCard.GetHeight()/5;
	}

	return;
}

//析构函数
CCardControl::~CCardControl()
{
}


//重画函数
void CCardControl::DrawCardControl(CDC *pDC)
{

	//绘画扑克
	DWORD dwXImagePos,dwYImagePos;
	DWORD dwXScreenPos,dwYScreenPos;
	for (INT_PTR i=0;i<m_CardDataArray.GetCount();i++)
	{
		//变量定义
		BYTE cbCardData=m_CardDataArray[i];
		bool bDrawCard=(m_bDisplayHead==true);

		//图片位置
		if(m_wCardColor!=INVALID_CHAIR)
		{
			//放弃背景
			if(m_wCardColor==1)
			{
				dwXImagePos=m_CardSize.cx*3;
			}
			//输牌背景
			else if(m_wCardColor==2)
			{
				dwXImagePos=m_CardSize.cx*4;
			}
			//闪牌辅助
			else if(m_wCardColor==0)
			{
				dwXImagePos=m_CardSize.cx*2;
			}
			else if(m_wCardColor==3)
			{
				dwXImagePos=m_CardSize.cx*5;
			}
			else if(m_wCardColor==4)
			{
				dwXImagePos=m_CardSize.cx*0;
			}
			else if(m_wCardColor==5)
			{
				dwXImagePos=m_CardSize.cx*1;
			}
			dwYImagePos=m_CardSize.cy*4;
		}
		else if ((bDrawCard==true)&&(cbCardData!=0))
		{
			dwXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
			dwYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		}
		else
		{
			if(!m_bCompareBack)		//正常&看牌背景
			{
				dwXImagePos=m_CardSize.cx*((m_bLookCard)?6:2);
			}
			else					//比牌背景
			{
				dwXImagePos=m_CardSize.cx*5;
			}
			dwYImagePos=m_CardSize.cy*4;
		}

		//屏幕位置
		dwYScreenPos=0;
		dwXScreenPos=m_dwCardHSpace*i;

		//绘画扑克
		m_ImageCard.DrawImage(pDC,m_BenchmarkPos.x+dwXScreenPos,m_BenchmarkPos.y+dwYScreenPos,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos);
	}


	return;
}

//获取数目
DWORD CCardControl::GetCardCount()
{
	return (WORD)m_CardDataArray.GetCount();
}

//显示扑克
void CCardControl::SetDisplayHead(bool bDisplayHead)
{
	//状态判断
	if (m_bDisplayHead==bDisplayHead) return;

	//设置变量
	m_bDisplayHead=bDisplayHead;

	return;
}

//设置间距
void CCardControl::SetCardSpace(DWORD dwCardSpace)
{
	//设置变量
	m_dwCardHSpace=dwCardSpace;

	return;
}

//设置响应
void CCardControl::SetPositively(bool bPositively)
{
	//设置变量
	m_bPositively=bPositively;

	return;
}

//设置扑克
DWORD CCardControl::GetCardData(BYTE cbCardData[], DWORD dwMaxCount)
{
	//拷贝扑克
	DWORD dwCopyCount=__min((DWORD)m_CardDataArray.GetCount(),dwMaxCount);
	CopyMemory(cbCardData,m_CardDataArray.GetData(),sizeof(BYTE)*dwCopyCount);

	return dwCopyCount;
}

//设置扑克
DWORD CCardControl::SetCardData(const BYTE cbCardData[], DWORD dwCardCount)
{
	//设置扑克
	m_CardDataArray.SetSize(dwCardCount);
	CopyMemory(m_CardDataArray.GetData(),cbCardData,sizeof(BYTE)*dwCardCount);

	return dwCardCount;
}

//设置扑克
void CCardControl::SetCardColor(WORD wColor)
{
	m_wCardColor=wColor;
	return;
}

//设置扑克
void CCardControl::SetLookCard(bool bLook)
{
	m_bLookCard=bLook;

	return;
}

//设置扑克
void CCardControl::SetCompareCard(bool bCompare)
{
	m_bCompareCard=bCompare;

	return ;
}

//设置扑克
void CCardControl::SetCompareBack(bool bShow)
{
	m_bCompareBack=bShow;
	return ;
}

//设置扑克
void CCardControl::SetCommandID(WORD ControlID,WORD wMessageID)
{
	m_wMessageID=wMessageID;
	m_wControlID=ControlID;
	return ;
}

//基准位置
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	return;
}

//基准位置
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;
	return;
}

//获取位置
CPoint CCardControl::GetCardPos()
{
	//变量定义
	DWORD dwCardCount=(DWORD)m_CardDataArray.GetCount();

	//计算大小
	CSize ControlSize;
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;

	//横向位置
	int nXPos=0;
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
	case enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.cx; break; }
	}

	//竖向位置
	int nYPos=0;
	switch (m_YCollocateMode)
	{
	case enYTop:	{ nYPos=m_BenchmarkPos.y; break; }
	case enYCenter: { nYPos=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	CPoint cpTemp;
	cpTemp.x=nXPos;
	cpTemp.y=nYPos;

	return cpTemp;
}

BOOL CCardControl::OnClickJudge(CPoint Point)
{
	if(m_wControlID == INVALID_CHAIR || m_bCompareCard == false) 
		return FALSE;

	DWORD dwCardCount=(DWORD)m_CardDataArray.GetCount();

	//计算大小
	CSize ControlSize;
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;

	CRect rc(m_BenchmarkPos.x,m_BenchmarkPos.y,m_BenchmarkPos.x+m_CardSize.cx,m_BenchmarkPos.y+m_CardSize.cy);
	return rc.PtInRect(Point);
}


//鼠标左击
//void CCardControl::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	if(m_wControlID!=INVALID_CHAIR && m_bCompareCard)
//	{
//		//用户ID
//		//TCHAR obj[32]=TEXT("");
//		//_sntprintf(obj,sizeof(obj),TEXT("%I64d:%I64d"),m_wMessageID,m_wControlID);
//		//MessageBox(obj);
//		CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
//		if(pGameFrameEngine != NULL)pGameFrameEngine->SendMessage(m_wMessageID,m_wControlID,m_wControlID);
//	}
//
//}
//////////////////////////////////////////////////////////////////////////

