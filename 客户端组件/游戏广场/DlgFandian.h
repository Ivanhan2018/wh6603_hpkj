#pragma once
#include "ImageButton.h"

// CDlgFandian 对话框

//CBitmapSlider	m_sliderBird;
//DDX_Control(pDX, IDC_SLIDER_BIRD, m_sliderBird);
//	m_sliderBird.SetBitmapThumb( IDB_BIRD, NULL, TRUE );
//	m_sliderBird.SetVertical();
//	m_sliderBird.DrawFocusRect( FALSE );
class CDlgFandian : public CDialog
{
	DECLARE_DYNAMIC(CDlgFandian)
public:
	CSliderCtrl		m_slider;
	float			m_nBonus;
	float			m_nBonusPercent;				//这个是我本身的返点，调节单位是0.1
	Bitmap*			m_bmpBk;

	CPngImage		m_BkImage;
	CImageButton	m_btnPlus;						//加按钮
	CImageButton	m_btnMinus;						//减按钮
	///////////////////////////////////////////////////////////////////////////////
	float			m_ndata1;
	float			m_ndata2;
	float			m_fDanweiFandian;				//单位返点，就是1个百分比返点对应多少MONEY
	CImageButton m_btnOK;
	CImageButton m_btnCancel;

	CPngImage	m_ImageBack;

public:
	CDlgFandian(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFandian();

public:
	void	SetBonus(float nBonus, float nBonusDefault,float nPercent, float fPercentDefault, float fDanweiFandian);
	void	GetBonus(float &nBonus, float& nPercent);
// 对话框数据
	enum { IDD = IDD_DLG_FANDIAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
	DECLARE_MESSAGE_MAP()
};
