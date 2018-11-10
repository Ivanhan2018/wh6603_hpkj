// SkinProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include "SkinProgressCtrl.h"


// CSkinProgressCtrl

IMPLEMENT_DYNAMIC(CSkinProgressCtrl, CProgressCtrl)

CSkinProgressCtrl::CSkinProgressCtrl()
{
	m_bmTile.LoadBitmap(IDB_PROGRESS_TILE);
	m_bmBack.LoadBitmap(IDB_PROGRESS_BACK);
}

CSkinProgressCtrl::~CSkinProgressCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinProgressCtrl, CProgressCtrl)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSkinProgressCtrl message handlers
void CSkinProgressCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if (bCalcValidRects)
	{
		lpncsp->rgrc[2] = lpncsp->rgrc[0];
	}
	else
	{
		CProgressCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}

void CSkinProgressCtrl::OnNcPaint()
{
	//CDC* pDC = GetWindowDC();
	//ASSERT(pDC!=NULL);

	//CRect rcWindow;
	//GetWindowRect(&rcWindow);
	//rcWindow.OffsetRect(-rcWindow.TopLeft());

	//pDC->Draw3dRect(&rcWindow, RGB(190,190,190), RGB(190,190,190));
	//ReleaseDC(pDC);
}


VOID  CSkinProgressCtrl::LoadPngImage()
{
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("IMG_PROGRESS_BACK"));
	m_ImageTile.LoadImage(AfxGetInstanceHandle(),TEXT("IMG_PROGRESS_TILE"));
	m_ImageTileL.LoadImage(AfxGetInstanceHandle(),TEXT("IMG_PROGRESS_TILE_L"));
	m_ImageTileM.LoadImage(AfxGetInstanceHandle(),TEXT("IMG_PROGRESS_TILE_M"));
	m_ImageTileR.LoadImage(AfxGetInstanceHandle(),TEXT("IMG_PROGRESS_TILE_R"));
}

void CSkinProgressCtrl::OnPaint()
{
	CPaintDC dc(this);
	PPRODATA ppd = (PPRODATA)(LONG_PTR)GetWindowLongPtr(m_hWnd, 0);
	ASSERT(ppd!=NULL);
	if (ppd == NULL) 
		return;

	CRect rcClient, rcFill;
	int px = 0;
	int nMode;

	CDC MemDC;
	CBitmap* pOldBitmap;

	GetClientRect(&rcClient);
	rcClient.DeflateRect(1,1,1,1);

#if 1
	////创建缓冲
	//CBitImage ImageBuffer;
	//ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	////创建设备
	//CImageDC BufferDC(ImageBuffer);
	//CDC * pBufferDC=CDC::FromHandle(BufferDC);

	////设置缓冲
	//pBufferDC->SetBkMode(TRANSPARENT);
	//pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

    INT nXDest=0,nYDest=4;
	INT nSrcWidth=532,nSrcWidthL=12,nSrcWidthM=508,nSrcWidthR=12,nSrcHeight=28;
    INT nDestWidthM=0;
	// Vertical style
	if (ppd->dwStyle & PBS_VERTICAL)
	{
		px = MulDiv(rcClient.Height(), ppd->iHigh - ppd->iPos, ppd->iHigh - ppd->iLow);
		rcFill.SetRect(rcClient.left, px, rcClient.right, rcClient.bottom);
	}
	else
	{
		px = MulDiv(rcClient.Width(), ppd->iPos - ppd->iLow, ppd->iHigh - ppd->iLow);
		rcFill.SetRect(rcClient.left+1, rcClient.top, px, rcClient.bottom);
	}
    nDestWidthM=rcFill.Width()-nSrcWidthL-nSrcWidthR;
	if(nDestWidthM<0)
        nDestWidthM=0;
	if(nDestWidthM>nSrcWidthM)
        nDestWidthM=nSrcWidthM; 

	CString strLog;
	strLog.Format(_T("nDestWidthM=%d,w=%d,h=%d"),nDestWidthM,rcFill.Width(),rcFill.Height());
	OutputDebugString(strLog);

	m_ImageTileL.DrawImage(this->GetDC(),0,0,nSrcWidthL,nSrcHeight,0,0);
	m_ImageTileM.DrawImage(this->GetDC(),nSrcWidthL,0,nDestWidthM,nSrcHeight,0,0);
	m_ImageTileR.DrawImage(this->GetDC(),nSrcWidthL+nDestWidthM,0,nSrcWidthR,nSrcHeight,0,0);

	//绘画界面
	//dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);
#endif
	return;

	MemDC.CreateCompatibleDC(&dc);
	pOldBitmap = MemDC.SelectObject(&m_bmTile);

	nMode = dc.SetStretchBltMode(HALFTONE);
	// Vertical style
	if (ppd->dwStyle & PBS_VERTICAL)
	{
		px = MulDiv(rcClient.Height(), ppd->iHigh - ppd->iPos, ppd->iHigh - ppd->iLow);
		rcFill.SetRect(rcClient.left, px, rcClient.right, rcClient.bottom);
	}
	else
	{
		px = MulDiv(rcClient.Width(), ppd->iPos - ppd->iLow, ppd->iHigh - ppd->iLow);
		rcFill.SetRect(rcClient.left+1, rcClient.top, px, rcClient.bottom);
	}

	dc.StretchBlt(rcFill.left, rcFill.top, rcFill.Width(), rcFill.Height(), &MemDC, 0, 0, 40, 12, SRCCOPY);
	//dc.StretchBlt(rcFill.left, rcFill.top, 1, rcFill.Height(), &MemDC, 0, 0, 1, 14, SRCCOPY);
	//dc.StretchBlt(rcFill.left+1, rcFill.top, rcFill.Width()-2, rcFill.Height(), &MemDC, 1, 0, 98, 14, SRCCOPY);
	//dc.StretchBlt(rcFill.right-1, rcFill.top, 1, rcFill.Height(), &MemDC, 99, 0, 1, 14, SRCCOPY);

	dc.SetStretchBltMode(nMode);
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

BOOL CSkinProgressCtrl::OnEraseBkgnd(CDC* pDC)
{
	CDC MemDC;
	CBitmap* pOldBitmap;
	CRect rcClient;
	int nMode;
	GetClientRect(&rcClient);

#if 1

	//创建缓冲
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建设备
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置缓冲
	pBufferDC->SetBkMode(TRANSPARENT);
	//pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	m_ImageBack.DrawImage(this->GetDC(),0,0);

	//绘画界面
	//pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);
#endif
    return TRUE;

	nMode = pDC->SetStretchBltMode(HALFTONE);
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(&m_bmBack);

	pDC->StretchBlt(rcClient.left, rcClient.top, 1, rcClient.Height(), &MemDC, 0, 0, 1, 14, SRCCOPY);
	pDC->StretchBlt(rcClient.left+1, rcClient.top, rcClient.Width()-2, rcClient.Height(), &MemDC, 1, 0, 98, 14, SRCCOPY);
	pDC->StretchBlt(rcClient.right-1, rcClient.top, 1, rcClient.Height(), &MemDC, 99, 0, 1, 14, SRCCOPY);
	pDC->SetStretchBltMode(nMode);
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
	
	return TRUE;
	//return CProgressCtrl::OnEraseBkgnd(pDC);
}


