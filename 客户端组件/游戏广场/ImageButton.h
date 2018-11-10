#pragma once

//具有四种状态的图片的按钮
class CImageButton : public CButton
{
	DECLARE_DYNAMIC(CImageButton)

public:
	CImageButton();
	virtual ~CImageButton();

public:
	void SetImage(LPCTSTR imgFile);
	void SetPushed(bool bPushed);
	void SetBtnSize(int width, int height) {m_width = width; m_height = height;}
	int Width() {return m_width;}
	int Height() {return m_height;}


protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

// private:
// 	void PaintBk(CDC* pDC);
// 	void PaintBtn(CDC* pDC);

private:
	Bitmap* m_pIamge;

	bool	m_bHaveBitmaps;

// 	CDC		m_dcBk;			// button background
// 
// 	CDC		m_dcStd;		// standard button
// 	CDC		m_dcStdP;		// standard button pressed
// 	CDC		m_dcStdH;		// standard button hot
// 	CDC		m_dcGS;			// grayscale button (does not have a hot or pressed state)
// 
// 	CDC*	m_pCurBtn;	// current pointer to one of the above

	int		m_width;
	int		m_height;

	bool m_bIsHovering;
	bool m_bIsTracking;
	bool m_bIsDisabled;

	bool m_bPushed;
};
