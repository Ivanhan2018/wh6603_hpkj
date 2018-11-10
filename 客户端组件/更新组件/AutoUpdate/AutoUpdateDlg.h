// AutoUpdateDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SkinProgressCtrl.h"
#include "SkinButton.h"
#include "SkinStatic.h"

// CAutoUpdateDlg dialog
class CAutoUpdateDlg : public CDialog, public IDownloadSink
{
// Implementation
protected:
	HICON					m_hIcon;				//图标句柄
	CString					m_strCurrentFile;		//当前文件
	CSkinProgressCtrl		m_Progress1;			//进度控件
	CSkinProgressCtrl		m_Progress2;			//进度控件
	CSkinStatic				m_PercentText;			//百分比文本
	CSkinStatic				m_StatusText;			//状态文本
	CSkinStatic				m_CountText;            //大小 
	CSkinStatic				m_CountText2;           //已下载
	CSkinButton				m_btCancel;				//彻消按钮
	CSkinButton				m_btRetry;				//重试按钮
	CSkinButton				m_btBySelf;				//手动下载

public:
	CDownloadMgr			m_DownloadMgr;			//下载管理
	CString					m_strCurrentDir;		//当前目录
	ULONG					m_ulCurentLenght;		//当前进度
	ULONG					m_ulTotalLength;		//总体进度

	// add by hxh 20160607
	int                     m_status;//下载状态 0:准备下载;1:正在下载;2:下载完成

public:
	CDC*					m_pMemDC;
	CBitmap*				m_pOldBitmap;
	CBitmap*				m_pBitmapMem;
	CBitmap					m_bmBack;
    CPngImage               m_BackImage;
    CPngImage               m_StatusImage[3];
    CFont                   m_font;
    CFont                   m_font2;
	//标准构造
public:
	CAutoUpdateDlg(CWnd* pParent = NULL);
	virtual ~CAutoUpdateDlg();
	// 加载PNG图片
	VOID  LoadPngImage();
	//重载函数
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	//接口函数
public:
	virtual void OnStatusCallback(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCTSTR pszStatusText);

protected:
	BOOL CheckVersionUpdate(CString& strUpdateURL);
	void BuildBkDC();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRetry();
	afx_msg void OnBnClickedSelf();
	afx_msg void OnClose();
	afx_msg LRESULT OnTransaprentBk( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
