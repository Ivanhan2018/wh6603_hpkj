#pragma once
#include "ImageButton.h"
#include "PlatformPublicize.h"
// HuodongZhuanzhuan 对话框
//幸运转转
class HuodongZhuanzhuan : public CDialog
{
	DECLARE_DYNAMIC(HuodongZhuanzhuan)

public:
	HuodongZhuanzhuan(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~HuodongZhuanzhuan();

// 对话框数据
	enum { IDD = IDD_DLG_HUODONG_ZHUANZHUAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedBtChouJiang();
private:
	Bitmap* m_bmpBk;
	CFont m_font;
	bool			m_bPrized;
	CImageButton	m_btnChouJiang;
	CPlatformPublicize  m_PublicizeWeb;
	//重载函数 
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetUserLuckyZhuan;						//获取日志数量
	bool							m_bGetLastYue;						//获取日志数量
	bool							m_bCanUserLuckyzhuanZhuan;						//获取日志数量通过ID
public:
	VOID SendToServer(int nSendType);
};
