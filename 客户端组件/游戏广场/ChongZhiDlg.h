#pragma once

#include "afxwin.h"
#include "TextButton.h"
#include "ImageButton.h"

//充值信息
typedef struct {
	CString m_strZhangHu;
	CString m_strKaiHuRen;
	CString m_strKaiHuHang;
	//CString m_strGameID;
	CString m_strWeb;
}ChongZhi;
#define  IDM_RETURN_GAME 11118
//充值中心
class CChongZhiDlg : public CDialog
{
	DECLARE_DYNAMIC(CChongZhiDlg)

	typedef enum {
		CZ_GONGSHANG,		//工商银行
		CZ_ZHIFUBAO,		//支付宝
		CZ_CAIFUTONG,		//财付通
		CZ_JIANSHE,			//建设银行
		CZ_NONGYE,			//农业银行
	}ChongZhiType;

public:
	CChongZhiDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChongZhiDlg();
	CFont m_font;
// 对话框数据
	enum { IDD = IDD_DLG_CHONGZHI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnGongshang();
	afx_msg void OnBnClickedBtnZhifubao();
	afx_msg void OnBnClickedBtnCaifutong();
	afx_msg void OnBnClickedBtnJianshe();
	afx_msg void OnBnClickedBtnNongye();
	afx_msg void OnBnClickedBtnChongzhiPage();
	afx_msg void OnBnClickedBtnCpZhanghu();
	afx_msg void OnBnClickedBtnCpKaihuren();
	afx_msg void OnBnClickedBtnCpYinhang();
	afx_msg void OnBnClickedBtnCpGaneid();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnClose();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	CImageButton m_btnClose;

private:
	void SetTextToClipboard(CString text);
	void InitBtns();
	void DrawMultLineText(CDC *pDC, CRect rect, int nRowDis, UINT nFromat, CString strText);

public:
	//ChongZhi	m_cz_gongshang;
	//ChongZhi	m_cz_zhifubao;
	//ChongZhi	m_cz_caifutong;
	//ChongZhi	m_cz_jianshe;
	//ChongZhi	m_cz_nongye;

	ChongZhi m_chongzhi;
	ChongZhi m_ChongZhiPage[5];
private:
	Bitmap* m_bmpBk;
	Bitmap* m_bmpChongZhiBk;
//	Bitmap* m_bmpChongZhiLogo;
	int m_ChongZhiType;

	CTextButton m_btnGongShang;
	CTextButton m_btnZhiFuBao;
	CTextButton m_btnCaiFuTong;
	CTextButton m_btnJianShe;
	CTextButton m_btnNongYe;
	
	//ChongZhiType	m_ChongZhiType;

	CImageButton m_btnChongZhiPage;
	CImageButton m_btnCPZhangHu;
	CImageButton m_btnCPKaiHuRen;
	CImageButton m_btnCPYinHang;
	CImageButton m_btnCPGameID;


	bool			m_bGetChongzhiType;
	bool			m_bGetAllYinHangName;


protected:
	virtual void OnCancel();
	virtual void OnOK();

	VOID  ShowButton(bool bShow);
	CMissionManager					m_MissionManager;					//任务管理
	//重载函数
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	BYTE							m_cbChongzhiType;						//充值类型	
protected:
	VOID SendToServer(BYTE nSendType);
	bool IsEnableZhanghu(ChongZhi& chongzhi);

};
