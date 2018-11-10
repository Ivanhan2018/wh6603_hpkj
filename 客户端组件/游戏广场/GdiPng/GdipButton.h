#pragma once

class CGdiPlusBitmapResource;

class CGdipButton : public CButton
{
public:

	CGdipButton();
	virtual ~CGdipButton();

	BOOL LoadStdImage(UINT id, LPCTSTR pType, bool Text=false);
	BOOL LoadStdImage(LPCTSTR imgFile, bool Text=false);
	void SetBtnSize(int width, int height) {m_width = width; m_height = height;}
	void SetPushed(bool bPushed);

protected:
	void PaintBk(CDC* pDC);
	void PaintBtn(CDC* pDC);

	BOOL	m_bHaveBitmaps;

	BOOL	m_bIsDisabled;
	BOOL	m_bIsToggle;
	bool    m_bPushed;
	BOOL	m_bIsHovering;
	BOOL	m_bIsTracking;

	int		m_nCurType;

	CGdiPlusBitmapResource* m_pStdImage;

	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	afx_msg void OnEnable(BOOL bEnable);

	DECLARE_MESSAGE_MAP()

private:
	CDC		m_dcBk;			// button background
	
	CDC		m_dcStd;		// standard button
	CDC		m_dcStdP;		// standard button pressed
	CDC		m_dcStdH;		// standard button hot
	CDC		m_dcGS;			// grayscale button (does not have a hot or pressed state)

	CDC*	m_pCurBtn;		// current pointer to one of the above

	int		m_width;
	int		m_height;

	bool	m_bTextBtn;	// «∑Ò «TextButton
};
