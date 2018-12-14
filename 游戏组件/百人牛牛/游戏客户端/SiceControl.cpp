#include "StdAfx.h"
#include "resource.h"
#include "sicecontrol.h"
#include "GameClientEngine.h"


CBitmap CSiceControl::m_ImageSice01;				//图片资源
CBitmap CSiceControl::m_ImageSice02;				//图片资源
CBitmap CSiceControl::m_ImageSice03;				//图片资源
CBitmap CSiceControl::m_ImageSice04;				//图片资源
CBitmap CSiceControl::m_ImageSice05;				//图片资源
CBitmap CSiceControl::m_ImageSice06;				//图片资源

CSiceControl::CSiceControl(void)
{
	m_SiceValue = 1;
	m_CurrentIndex = 0;
	m_pointOrigin.x = 0;
	m_pointOrigin.y = 0;
	m_bVisbale = false;					//是否可见


	//加载资源
	HINSTANCE hResInstance = AfxGetInstanceHandle();

	//if (m_ImageSice01.IsNull())
	//	m_ImageSice01.LoadImage(hResInstance, TEXT("SICE1"));
	//if (m_ImageSice02.IsNull())
	//	m_ImageSice02.LoadImage(hResInstance, TEXT("SICE2"));
	//if (m_ImageSice03.IsNull())
	//	m_ImageSice03.LoadImage(hResInstance, TEXT("SICE3"));
	//if (m_ImageSice04.IsNull())
	//	m_ImageSice04.LoadImage(hResInstance, TEXT("SICE4"));
	//if (m_ImageSice05.IsNull())
	//	m_ImageSice05.LoadImage(hResInstance, TEXT("SICE5"));
	//if (m_ImageSice06.IsNull())
	//	m_ImageSice06.LoadImage(hResInstance, TEXT("SICE6"));

	////if (m_ImageSice01.m_hObject == NULL)
	//	m_ImageSice01.LoadBitmap(IDB_BT_SICE_1);
	//
	////if (m_ImageSice02.m_hObject == NULL)
	//	m_ImageSice02.LoadBitmap(IDB_BT_SICE_2);
	////if (m_ImageSice03.m_hObject == NULL)
	//	m_ImageSice03.LoadBitmap(IDB_BT_SICE_3);
	////if (m_ImageSice04.m_hObject == NULL)
	//	m_ImageSice04.LoadBitmap(IDB_BT_SICE_4);
	////if (m_ImageSice05.m_hObject == NULL)
	//	m_ImageSice05.LoadBitmap(IDB_BT_SICE_5);
	////if (m_ImageSice06.m_hObject == NULL)
	//	m_ImageSice06.LoadBitmap(IDB_BT_SICE_6);

	if (m_ImageSice01.m_hObject == NULL)
	{
	/*m_ImageSice01.m_hObject = (HBITMAP)::LoadImage(NULL,_T("D:\\SICE_1.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	
	m_ImageSice02.m_hObject = (HBITMAP)::LoadImage(NULL,_T("D:\\SICE_2.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_ImageSice03.m_hObject = (HBITMAP)::LoadImage(NULL,_T("D:\\SICE_3.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_ImageSice04.m_hObject = (HBITMAP)::LoadImage(NULL,_T("D:\\SICE_4.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_ImageSice05.m_hObject = (HBITMAP)::LoadImage(NULL,_T("D:\\SICE_5.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_ImageSice06.m_hObject = (HBITMAP)::LoadImage(NULL,_T("D:\\SICE_6.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);*/
		m_ImageSice01.LoadBitmap(IDB_SICE_1);
		m_ImageSice02.LoadBitmap(IDB_SICE_2);
		m_ImageSice03.LoadBitmap(IDB_SICE_3);
		m_ImageSice04.LoadBitmap(IDB_SICE_4);
		m_ImageSice05.LoadBitmap(IDB_SICE_5);
		m_ImageSice06.LoadBitmap(IDB_SICE_6);
	}

	

	

	m_FrameSize.cx = 2170 / 31;
	m_FrameSize.cy = 128;
}

CSiceControl::~CSiceControl(void)
{
	SetVisable(false);
}

//设置点数
void CSiceControl::SetSiceValue(BYTE nValue)
{
	if (nValue <= 0)
		nValue = 1;

	if (nValue > 6)
		nValue = 6;

	if (m_SiceValue != nValue)
		m_SiceValue = nValue;

	//再随机
	m_pointOrigin.x = 0;
	m_pointOrigin.y = 0;


	m_CurrentIndex = 0;				//当前帧
}

//设置帧索引
void CSiceControl::SetFrameIndex(int nIndex)
{
	if (nIndex < 0)
		nIndex = 0;

	if (nIndex >= SICE_FLAME_COUNT)
		nIndex = SICE_FLAME_COUNT - 1;

	if (m_CurrentIndex != nIndex)
		m_CurrentIndex = nIndex;
}

//获取帧索引
int CSiceControl::GetFrameIndex()
{
	return m_CurrentIndex;
}

//显示完成
bool CSiceControl::IsFinish()
{
	return m_CurrentIndex >= SICE_FLAME_COUNT - 1;
}

//设置基准位置
void CSiceControl::SetPointBase(int x, int y)
{
	m_pointBase.x = x;
	m_pointBase.y = y;
}

//设置基准位置
void CSiceControl::SetPointBase(CPoint pos)
{
	m_pointBase = pos;
}

//设置显示范围
void CSiceControl::SetSize(int w, int h)
{
	if (w <= 0)
		w = 100;
	if (h <= 0)
		h = 100;

	m_sizeRECT = CSize(w, h);
}

//设置显示范围
void CSiceControl::SetSize(CSize size)
{
	if (size.cx <= 0)
		size.cx = 100;
	if (size.cy <= 0)
		size.cy = 100;

	m_sizeRECT = size;
}

//绘画扑克
VOID CSiceControl::DrawCardControl(CDC * pDC)
{
	//if (!IsVisable() || m_SiceValue <= 0 || m_SiceValue > 6 || m_CurrentIndex < 0 || m_SiceValue >= SICE_FLAME_COUNT) return;

	if (m_pointOrigin.x == 0 && m_pointOrigin.y == 0)
	{
		m_pointOrigin.x = rand() % m_sizeRECT.cx;
		m_pointOrigin.y = rand() % m_sizeRECT.cy;
	}

	CBitmap *pImage = NULL;
	switch (m_SiceValue)
	{
	case 1:
		pImage = &m_ImageSice01;
		break;
	case 2:
		pImage = &m_ImageSice02;
		break;
	case 3:
		pImage = &m_ImageSice03;
		break;
	case 4:
		pImage = &m_ImageSice04;
		break;
	case 5:
		pImage = &m_ImageSice05;
		break;
	case 6:
		pImage = &m_ImageSice06;
		break;
	default:
		return;
	}

	//绘画骰子
	
	
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(pImage);

	pDC->TransparentBlt(m_pointBase.x + m_pointOrigin.x, m_pointBase.y + m_pointOrigin.y,
		m_FrameSize.cx, 128,&memDC,m_CurrentIndex*m_FrameSize.cx,0, m_FrameSize.cx, m_FrameSize.cy,RGB(255,0,255));

	memDC.DeleteDC();
	//}

	//http://worldcup.qq.com/interview/live/index.htm
// 	if (m_CurrentIndex == 0)
// 	{
// 		CGameClientDlg *pGameClientDlg = (CGameClientDlg *)AfxGetMainWnd();
// 		if (pGameClientDlg)
// 			pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(), TEXT("DICE_GO"));
// 	}
}


