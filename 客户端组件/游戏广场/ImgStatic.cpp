#include "stdafx.h"
#include "GamePlaza.h"
#include "ImgStatic.h"


// CImgStatic

IMPLEMENT_DYNAMIC(CImgStatic, CWnd)

CImgStatic::CImgStatic()
: m_pImage(NULL)
, m_imgWidth(0)
, m_imgHeight(0)
, m_imgIndex(0)
{

}

CImgStatic::~CImgStatic()
{

}

void CImgStatic::SetImage(Bitmap* bitmap, int index, int size)
{
	ASSERT(bitmap != NULL);

	m_pImage = bitmap;
	m_imgWidth = m_pImage->GetWidth();
	m_imgHeight = m_pImage->GetHeight() / size;

	m_imgIndex = index;
}

BEGIN_MESSAGE_MAP(CImgStatic, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CImgStatic 消息处理程序
void CImgStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);

	Graphics graphics(dc.m_hDC);
	graphics.DrawImage(m_pImage, Rect(0, 0, rect.Width(), rect.Height()), 0,  m_imgIndex * m_imgHeight, m_imgWidth, m_imgHeight , UnitPixel);
	
	// 不为绘图消息调用 CWnd::OnPaint()
}

BOOL CImgStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CImgStatic::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CWnd::PreSubclassWindow();
}
