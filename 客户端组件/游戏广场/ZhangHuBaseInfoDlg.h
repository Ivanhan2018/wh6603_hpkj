#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "MessageDlg.h"
#include "ImgRioButton.h"

#define  RELEASE_FACE  WM_USER+11123
//我的账户-基本信息
class CZhangHuBaseInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuBaseInfoDlg)

public:
	CZhangHuBaseInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZhangHuBaseInfoDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ZHANGHU_BASEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

	CImgRioButton m_ImgRioMan;
	CImgRioButton m_ImgRioWoman;
public:
	WDZHUserInfo m_userinfo;	
	DWORD m_dwTickCount;

private:
	Bitmap* m_bmpBk;
	CFont m_font;
	BYTE m_cbGender;
	CSkinEditEx m_editQQ;
	CImageButton m_btnXgqq;
	afx_msg void OnBnClickedBtnXgqq();
	afx_msg void OnBnClickedBtnXgGender();
	afx_msg void OnBnClickedRioBtnMan();
	afx_msg void OnBnClickedRioBtnWoman();
	CImageButton m_btnXggender;

	//重载函数
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetUserInfo;						//获取日志数量
	bool							m_bXgUserQQ;							//获取日志
protected:
	VOID SendToServer(int nSendType);

};
