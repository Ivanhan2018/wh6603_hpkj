#pragma once

#include "Stdafx.h"
// CButtonEx

class CButtonEx : public CButton
{
	DECLARE_DYNAMIC(CButtonEx)

public:
	CButtonEx();
	CButtonEx( int nItem, int nSubItem, CRect rect, HWND hParent );
	virtual ~CButtonEx();
	//»æ»­º¯Êý
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PreSubclassWindow();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
	void SetImage(LPCTSTR imgFile);
	int m_inItem;
	int m_inSubItem;
	CRect m_rect;
	HWND m_hParent;
	Bitmap* m_pIamge;
	int		m_width;
	int		m_height;

	BOOL bEnable;
};


