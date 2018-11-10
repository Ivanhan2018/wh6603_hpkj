#pragma once  

/*  
Title:改变MFC CCheckBox控件默认字体的颜色  
Test Environment: VS2013Update3  
Author: kagula  
[cpp] view plaincopy在CODE上查看代码片派生到我的代码片
LastUpdateDate:2014-10-20  
*/  
class CMyCheckBox : public CButton  
{  
	DECLARE_DYNAMIC(CMyCheckBox)  

public:  
	CMyCheckBox();  
	virtual ~CMyCheckBox();  

protected:  
	DECLARE_MESSAGE_MAP()  
public:  
	COLORREF m_clrFore;  
	COLORREF m_clrBK;  
	int      m_fontSize;  
	afx_msg void OnPaint();  
}; 