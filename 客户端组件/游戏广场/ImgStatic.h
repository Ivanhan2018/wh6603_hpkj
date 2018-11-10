#pragma once


// CImgStatic

class CImgStatic : public CWnd
{
	DECLARE_DYNAMIC(CImgStatic)

public:
	CImgStatic();
	virtual ~CImgStatic();

public:
	void SetImage(Bitmap* bitmap, int index, int size);
	void SetBtnSize(int width, int height) {m_imgWidth = width; m_imgHeight = height;}
	int Width() {return m_imgWidth;}
	int Height() {return m_imgHeight;}

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreSubclassWindow();

private:
	Bitmap* m_pImage;

	int m_imgWidth;
	int m_imgHeight;

	int m_imgIndex;
};


