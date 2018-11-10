#pragma once


class CNumerButton : public CButton
{
	DECLARE_DYNAMIC(CNumerButton)

public:
	CNumerButton();
	virtual ~CNumerButton();

public:
	void SetImage(Bitmap* imageList, int index, int size);
	void SetBtnSize(int width, int height) {m_width = width; m_height = height;}
	int Width() {return m_width;}
	int Height() {return m_height;}
	void SetSelect(bool bSel);
	bool IsSelect() {return b_select;}


protected:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
	LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

// private:
// 	void PaintBk(CDC* pDC);
// 	void PaintBtn(CDC* pDC);

private:
	Bitmap* m_pIamge;
	int m_index;

	bool	m_bHaveBitmaps;

// 	CDC		m_dcStdP;		// standard button pressed
// 	CDC		m_dcStdH;		// standard button hot
// 	CDC		m_dcGS;			// grayscale button (does not have a hot or pressed state)
// 
// 	CDC		m_dcBk;			// button background
// 
// 	CDC		m_dcStd;		// standard button
// 	CDC		m_dcStdSel;		// standard button pressed

//	CDC*	m_pCurBtn;	// current pointer to one of the above

	int		m_width;
	int		m_height;

	bool m_bIsHovering;
	bool m_bIsTracking;
	bool m_bIsDisabled;

	bool b_select;
};

