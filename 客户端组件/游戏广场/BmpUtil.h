#pragma once

class CBmpUtil
{
public:
	CBmpUtil(void);
	~CBmpUtil(void);

public:
	static HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010);

	static void GetExePath(LPCTSTR lpctstrFileName, LPTSTR lptstrNewFileName);

	static CString& GetExePath();

	static bool IsNumber(const CString& strNum);
};
