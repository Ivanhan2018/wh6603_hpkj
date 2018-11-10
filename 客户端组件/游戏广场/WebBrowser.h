#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CHtmlView。"
#endif 

// CWebBrowser Html 视图

class CWebBrowserLogo : public CHtmlView
{
	DECLARE_DYNCREATE(CWebBrowserLogo)

public:
	CWebBrowserLogo();           // 动态创建所使用的受保护的构造函数
	virtual ~CWebBrowserLogo();

public:
	void Navigate(LPCTSTR pszUrl);
	//启用网页
	bool EnableBrowser(bool bEnableBrowser);

	CString m_strNavigateUrl;					//页面地址
	bool m_bEnableBrowser;					//是否启用

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//消息预处理
	BOOL	PreTranslateMessage(MSG*	pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


