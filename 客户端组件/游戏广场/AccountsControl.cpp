#include "StdAfx.h"
#include "Resource.h"
#include "AccountsControl.h"

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define ITEM_CY						22									//子项大小

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAccountsCtrl, CSkinComboBox)
	//ON_WM_PAINT()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CAccountsCtrl::CAccountsCtrl()
{
	m_Font.CreateFont(26,0,0,0,0,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("Arial"));

}

//析构函数
CAccountsCtrl::~CAccountsCtrl()
{
}

//绑定函数
VOID CAccountsCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//设置控件
	m_SkinComboBoxEdit.LimitText(LEN_ACCOUNTS-1);
	m_SkinComboBoxEdit.SetEnableColor(RGB(254,254,254),RGB(73,62,61),RGB(73,62,61));
	//设置控件
	SetButtonImage(AfxGetInstanceHandle(),IDB_BT_COMBOBOX,CSize(21,19));

	
	return;
}
// VOID CAccountsCtrl::SetBackColor(COLORREF crBackColor)
// {
// 	SetBackColor(crBackColor);
// 	return;
// }

//测量子项
VOID CAccountsCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//设置变量
	lpMeasureItemStruct->itemWidth=0;
	lpMeasureItemStruct->itemHeight=ITEM_CY;

	return;
}
////重画消息
//VOID CAccountsCtrl::OnPaint()
//{
//	CPaintDC dc(this);
//
//	//获取位置
//	CRect rcClient;
//	GetClientRect(&rcClient);
//
//	CRect rcInv;
//	rcInv.CopyRect(rcClient);
//
//	rcInv.right -= 20;
//
//	dc.SetBkMode(TRANSPARENT);
//
//	//绘画背景
//	if (IsWindowEnabled()==TRUE)
//	{
//		dc.FillSolidRect(rcInv,RGB(73,62,61));
//	}
//	else
//	{
//		dc.FillSolidRect(rcInv,RGB(73,62,61));
//	}
//
//	return;
//}

//////////////////////////////////////////////////////////////////////////////////
