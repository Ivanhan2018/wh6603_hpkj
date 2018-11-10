#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "comm.h"
#include "NumerButton.h"
#include "TextButton.h"
#include "ImageButton.h"
#include "DlgLogon.h"
#include "ImgRioButton.h"
#include "ImgStatic.h"
#include "GameRule.h"
#include "resource.h"
// #include "GamePlaza.h"
 #include "QrTzhDlg.h"
#include "FanDian.h"
#include	"GridCtrl/GridCtrl.h"
#include "DlgStatus.h"
class CPlazaViewItem;
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"

#define			AREA_TMDM_MAX				 50
#define			AREA_TMDS_MAX				 3
#define			AREA_TMDX_MAX				 3
#define			AREA_TMSX_MAX				 13
#define			AREA_TMBS_MAX				 4



// 重庆时时彩 对话框
class CLiuHeCai : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CLiuHeCai)

public:
	CDlgStatus						m_DlgStatus;						//状态窗口
	//取消连接
	virtual VOID OnStatusCancel();


	enum {
		Kj_XinXi_Count = 5
	};

public:
	CLiuHeCai(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLiuHeCai();
	bool m_bShowFail;
	HBRUSH hBrush;
	int m_nTzhSign;
	int m_nTzhSign1;
// 对话框数据
	enum { IDD = IDD_DLG_LIUHECAI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	LiuHeCaiKind m_gameKind;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );

	afx_msg void OnBnClickedBtnTmdm();
	afx_msg void OnBnClickedBtnPmdm();
	afx_msg void OnBnClickedBtnTmds();
	afx_msg void OnBnClickedBtnTmdx();
	afx_msg void OnBnClickedBtnTmsx();
	afx_msg void OnBnClickedBtnTmbs();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnTouZhu();

	afx_msg void OnBnClickedJettonOK();
	afx_msg void OnBnClickedFandian();
	afx_msg void OnBnClickedBtnClsList();
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	afx_msg void OnBnClickedBtnZm1t();
	afx_msg void OnBnClickedBtnZm2t();
	afx_msg void OnBnClickedBtnZm3t();
	afx_msg void OnBnClickedBtnZm4t();
	afx_msg void OnBnClickedBtnZm5t();
	afx_msg void OnBnClickedBtnZm6t();
	DECLARE_MESSAGE_MAP()
	float						m_fBonus;						//设置的
	float						m_fPercent;						//设置的
	float	nPercent;
	float	fDanweiFandian;
	float	nBonus ;
	int	m_singlezhushu;
	DOUBLE m_fSingleJine;
	int m_zongZhuShu;
	DOUBLE m_zongJine;
	CFont m_font;
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	void ResetAllEdit();
	void InitWanFaMoShi();
	void SetLockText(float fBonus, float fBonusPercent);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, int lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	void DrawZongZhushuJinE(CDC* pDC);
	void ResetJetton();
	void SendToServer(int nSendType);
public:
	bool GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nDataSize);
	int GetBoSe(int nNum);
	bool ChangeStringToA(CString strInput, string&  strOutput);
	int GetLhcZhushu();
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//位置信息
protected:
	int		m_nMoshi;
	KjXinXi m_kjXinxi[Kj_XinXi_Count];
	bool	m_bKaiJiangzhong;
	int	m_kjXinxiCont;
	TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
public:
	void FlushZongjine();
	Bitmap* m_bmpBk;
	Bitmap* m_bmpTmdm;
	Bitmap* m_bmpTmpm;
	Bitmap* m_bmpTmds;
	Bitmap* m_bmpTmdx;
	Bitmap* m_bmpTmsx;
	Bitmap* m_bmpTmbs;

	CTextButton						 m_btnTmdm;		//特码单买
	CTextButton						 m_btnPmdm;		//平码单买
	CTextButton						 m_btnTmds;		//特码单双
	CTextButton						 m_btnTmdx;		//特码大小
	CTextButton						 m_btnTmsx;		//特码生肖
	CTextButton						 m_btnTmbs;		//特码波色

	CImageButton m_btn3DAdd;
	CImageButton m_btnClsList;
	CImageButton m_btnTouZhu;
	CTextButton m_btnLock;
	CFont m_zongFont;
	CFont m_KjInfoFont1;

	CTextButton				m_btnZmtm1;
	CTextButton				m_btnZmtm2;
	CTextButton				m_btnZmtm3;
	CTextButton				m_btnZmtm4;
	CTextButton				m_btnZmtm5;
	CTextButton				m_btnZmtm6;
	CListCtrlCl m_list3D;						
private:
	void InitBtns();
	CString GetWanDesc();
	CSkinEditEx m_editTmdm1;
	CSkinEditEx m_editTmdm2;
	CSkinEditEx m_editTmdm3;
	CSkinEditEx m_editTmdm4;
	CSkinEditEx m_editTmdm5;
	CSkinEditEx m_editTmdm6;
	CSkinEditEx m_editTmdm7;
	CSkinEditEx m_editTmdm8;

	CSkinEditEx m_editTmdm9;
	CSkinEditEx m_editTmdm10;
	CSkinEditEx m_editTmdm11;
	CSkinEditEx m_editTmdm12;
	CSkinEditEx m_editTmdm13;
	CSkinEditEx m_editTmdm14;
	CSkinEditEx m_editTmdm15;
	CSkinEditEx m_editTmdm16;
	CSkinEditEx m_editTmdm17;
	CSkinEditEx m_editTmdm18;
	CSkinEditEx m_editTmdm19;
	CSkinEditEx m_editTmdm20;
	CSkinEditEx m_editTmdm21;
	CSkinEditEx m_editTmdm22;
	CSkinEditEx m_editTmdm23;
	CSkinEditEx m_editTmdm24;

	CSkinEditEx m_editTmdm25;
	CSkinEditEx m_editTmdm26;
	CSkinEditEx m_editTmdm27;
	CSkinEditEx m_editTmdm28;
	CSkinEditEx m_editTmdm29;
	CSkinEditEx m_editTmdm30;
	CSkinEditEx m_editTmdm31;
	CSkinEditEx m_editTmdm32;
	CSkinEditEx m_editTmdm33;
	CSkinEditEx m_editTmdm34;
	CSkinEditEx m_editTmdm35;
	CSkinEditEx m_editTmdm36;
	CSkinEditEx m_editTmdm37;
	CSkinEditEx m_editTmdm38;

	CSkinEditEx m_editTmdm39;
	CSkinEditEx m_editTmdm40;
	CSkinEditEx m_editTmdm41;
	CSkinEditEx m_editTmdm42;
	CSkinEditEx m_editTmdm43;
	CSkinEditEx m_editTmdm44;
	CSkinEditEx m_editTmdm45;
	CSkinEditEx m_editTmdm46;
	CSkinEditEx m_editTmdm47;
	CSkinEditEx m_editTmdm48;
	CSkinEditEx m_editTmdm49;

	CSkinEditEx m_editTmdxdsDa;
	CSkinEditEx m_editTmdxdsXiao;
	CSkinEditEx m_editTmdxdsDan;
	CSkinEditEx m_editTmdxdsShuang;

	CSkinEditEx m_editTmsxShu;
	CSkinEditEx m_editTmsxNiu;
	CSkinEditEx m_editTmsxHu;
	CSkinEditEx m_editTmsxTu;
	CSkinEditEx m_editTmsxLong;
	CSkinEditEx m_editTmsxShe;
	CSkinEditEx m_editTmsxMa;
	CSkinEditEx m_editTmsxYang;
	CSkinEditEx m_editTmsxHou;
	CSkinEditEx m_editTmsxJi;
	CSkinEditEx m_editTmsxGou;
	CSkinEditEx m_editTmsxZhu;
	CSkinEditEx m_editTmbshb;
	CSkinEditEx m_editTmbslb;
	CSkinEditEx m_editTmbslvb;
	void ShowTmdmCtrl();
	void HideTmdmCtrl();
	void ShowTmdxdsCtrl();
	void HideTmdxdsCtrl();
	void ShowTmsxCtrl();
	void HideTmsxCtrl();
	void ShowZmtCtrl();
	void HideZmtCtrl();
	void ShowTmbsCtrl();
	void HideTmbsCtrl();
	void DrawLastKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawTopKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	int GetGameKindByDesc(const CString& desc);
	CString strNextQihao;
	CTime m_NextTime;
	Bitmap* m_kjNumBig;
	int m_bigNumWidth;
	int m_bigNumHeight;
	Bitmap* m_kjNumSmall;
	int m_SmallNumWidth;
	int m_SmallNumHeight;


};
