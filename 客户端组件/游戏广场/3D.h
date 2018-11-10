#pragma once
#include "afxwin.h"
#include "NumerButton.h"
#include "ImageButton.h"
#include "TextButton.h"
#include "ImgRioButton.h"
#include "afxcmn.h"
#include "ImgStatic.h"
#include "DlgStatus.h"
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"
#include "OpenRecord.h"
#include "resource.h"
#include "comm.h"
// C3D 对话框

class CPlazaViewItem;


class C3D : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(C3D)
// 	typedef enum 
// 	{
// 		InValid,
// 		//SanXingZhiXuan,		//三星直选
// 		enWF_ZhixuanFushi,	
// 		enWF_ZhixuanDanshi,
// 
// 		//SanXingZuXuan,		//三星组选
// 		enWF_QiansanzusanFushi,
// 		enWF_QiansanzusanDanshi,
// 		enWF_QiansanzuliuFushi,
// 		enWF_QiansanzuliuDanshi,
// 
// 		enWF_QianyizhixuanFushi,		//前一直选
// 
// 		//QianErZhiXuan,		//前二直选
// 		enWF_QianerzhixuanFushi,
// 		enWF_QianerzhixuanDanshi,
// 
// 		enWF_Budingwei,	//三星不定位
// 
// 		enWF_HouyizhixuanFushi,		//后一直选
// 
// 		//HouErZhiXuan,		//后二直选
// 		enWF_HouerzhixuanFushi,
// 		enWF_HouerzhixuanDanshi,
// 
// 		//SanXingZhiXuanHeZhi,//三星直选和值
// 
// 		WanFaCount
// 	}tagEnumTreeNumbers;
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件
	CImageButton					m_btWxzxTip;

	enum {
		Kj_XinXi_Count = 5
	};

public:
	C3D(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~C3D();
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	CImageButton m_btnMoreRecord;
// 对话框数据
	enum { IDD = IDD_DLG_PAILIE3 };

public:
	void ConnectMainDlg(CPlazaViewItem* luckMeDlg);
	void FlushZongjine();
	CString GetScoreMoshi();
	int GetMoshiRet(CString moshi);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnRichEditCopy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );
	afx_msg void OnEnChangeRichEditDanshiInput();
	int GetDanShiZhusu(int nKind);

	afx_msg void OnBnClickedBtn3dSxzhx();
	afx_msg void OnBnClickedBtn3dSxzx();
	afx_msg void OnBnClickedBtn3dQyzhx();
	afx_msg void OnBnClickedBtn3dQezhx();
	afx_msg void OnBnClickedBtn3dSxbdw();
	afx_msg void OnBnClickedBtn3dHyzhx();
	afx_msg void OnBnClickedBtn3dHezhx();
	afx_msg void OnBnClickedBtn3dDwd();
	afx_msg void OnBnClickedBtn3dDxds();
	afx_msg void OnBnClickedBtnBai3dQuan();
	afx_msg void OnBnClickedBtnBai3dDa();
	afx_msg void OnBnClickedBtnBai3dXiao();
	afx_msg void OnBnClickedBtnBai3dDan();
	afx_msg void OnBnClickedBtnBai3dShuang();
	afx_msg void OnBnClickedBtnBai3dQing();
	afx_msg void OnBnClickedBtnShi3dQuan();
	afx_msg void OnBnClickedBtnShi3dDa();
	afx_msg void OnBnClickedBtnShi3dXiao();
	afx_msg void OnBnClickedBtnShi3dDan();
	afx_msg void OnBnClickedBtnShi3dShuang();
	afx_msg void OnBnClickedBtnShi3dQing();
	afx_msg void OnBnClickedBtnGe3dQuan();
	afx_msg void OnBnClickedBtnGe3dDa();
	afx_msg void OnBnClickedBtnGe3dXiao();
	afx_msg void OnBnClickedBtnGe3dDan();
	afx_msg void OnBnClickedBtnGe3dShuang();
	afx_msg void OnBnClickedBtnGe3dQing();

	afx_msg void OnBnClickedRioSxzhxFushi();
	afx_msg void OnBnClickedRioSxzhxDanshi();
	afx_msg void OnBnClickedRioHezhxFushi();
	afx_msg void OnBnClickedRioHezhxDanshi();
	afx_msg void OnBnClickedRioSxzsFushi();
	afx_msg void OnBnClickedRioSxzsDanshi();
	afx_msg void OnBnClickedRioSxzlFushi();
	afx_msg void OnBnClickedRioSxzlDanshi();
	afx_msg void OnBnClickedRioQyzhx();
	afx_msg void OnBnClickedRioQezhxFushi();
	afx_msg void OnBnClickedRioQezhxDanshi();
	afx_msg void OnBnClickedRioSxbdw();
	afx_msg void OnBnClickedRioHyzhx();
	afx_msg void OnBnClickedRioSxzhxHezhi();
	afx_msg void OnBnClickedRioDwd();
	afx_msg void OnBnClickedRioDwDxds();

	afx_msg void OnBnClickedBtn3dAdd();
	afx_msg void OnBnClickedBtnDelSel();
	afx_msg void OnBnClickedBtnTouzhu();
	afx_msg void OnBnClickedBtnZhuihao();

	afx_msg void OnEnChangeEditDanshiInput();
	afx_msg void OnEnChangeEditBeishu();
public:
	afx_msg void OnBnClickedBtnLock();
	afx_msg void OnBnClickedBeiTouJia();
	afx_msg void OnBnClickedBeiTouJian();
	CImageButton m_btnBeiTouJia;
	CImageButton m_btnBeiTouJian;
	COpenRecord						m_DlgOpenRecord;					//更多开奖记录

	CTextButton m_btnYuan;
	CTextButton m_btnJiao;
	CTextButton m_btnFen;
	CTextButton m_btnLi;
	afx_msg void OnBnClickedBtnYuan();
	afx_msg void OnBnClickedBtnMoreRecord();
	afx_msg void OnBnClickedBtnJiao();
	afx_msg void OnBnClickedBtnFen();
	afx_msg void OnBnClickedBtnLi();

	afx_msg void OnBnClickedBtnClsList();
	DECLARE_MESSAGE_MAP()
public:
	//服务器端返回前N个开奖号码
	bool GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nDataSize);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	void CheckNumBtn(CNumerButton& btn);
	void UnCheckNumBtn(CNumerButton& btn);
	bool IsNumBtnCheck(CNumerButton& btn);

	void HideCtrl(int ctrl_id);
	void ShowCtrl(int ctrl_id);

	void HideAllRio();

	void AdjustWanFa();
	void AdjustRadio();
	void AdjustNumBtn();
	void AdjustNumView();
	void AdjustDanShi();
	void AdjustAdd();

	void CheckWanFaBtn(CTextButton& btn);

	void InitWanFaBtns();
	void InitNumberBtns();
	void InitListCtrl();

	void HideBaiNums();
	void HideShiNums();
	void HideGeNums();
	void ShowBaiNums();
	void ShowShiNums();
	void ShowGeNums();

	//显示隐藏组选和不定位
	void ShowZuXuan();
	void ShowBuDingWei();

	//显示隐藏单式
	void HideDanShiEdit();
	void ShowDanShiEdit();

	void ResetAllNums();

	CString GetBaiString();
	CString GetShiString();
	CString GetGeString();

	CString GetWanDesc();
	tagEnumTreeNumbers GetWanFaByDesc(const CString& desc);

	void DrawLastKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawTopKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawZongZhushuJinE(CDC* pDC);

	bool LuckyNumCQSSC();
public:
	CString GetNextExpect();
	//下期开奖时间
	CTime GetNextKjShj();
	//离下次开奖时间还剩下的时间
	long GetKjShjDiff();
	CString GetKjShjDiffDesc();

	void SetLockText();

	bool GetZuSanHaoma(CStringArray& strArr);
	bool GetZuSanHaoma(CString& haoma, int& zhushu);
	bool IsZuSanHaoma(const CString& str);

	bool GetDanShiHaoma(CString& haoma, int size, int& zhushu);
	bool GetHaomaStrs(CStringArray& strArr, CString line, int size);

	void InitWanFaMoShi();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int Get3dZhushu(int nTypeID,int nKindID);

private:
	Bitmap* m_bmpBk;
	Bitmap* m_numImageList;
	Bitmap* m_kjNumBig;
	Bitmap* m_kjNumSmall;
	Bitmap* m_bmpNumTip;
	DOUBLE	m_zongJine;
	tagEnumTreeNumbers	m_gameKind;
	static CString m_strHaoma;
	int				m_singlezhushu;
	CNumerButton m_btn3DBai0;
	CNumerButton m_btn3DBai1;
	CNumerButton m_btn3DBai2;
	CNumerButton m_btn3DBai3;
	CNumerButton m_btn3DBai4;
	CNumerButton m_btn3DBai5;
	CNumerButton m_btn3DBai6;
	CNumerButton m_btn3DBai7;
	CNumerButton m_btn3DBai8;
	CNumerButton m_btn3DBai9;

	CNumerButton m_btn3DShi0;
	CNumerButton m_btn3DShi1;
	CNumerButton m_btn3DShi2;
	CNumerButton m_btn3DShi3;
	CNumerButton m_btn3DShi4;
	CNumerButton m_btn3DShi5;
	CNumerButton m_btn3DShi6;
	CNumerButton m_btn3DShi7;
	CNumerButton m_btn3DShi8;
	CNumerButton m_btn3DShi9;

	CNumerButton m_btn3DGe0;
	CNumerButton m_btn3DGe1;
	CNumerButton m_btn3DGe2;
	CNumerButton m_btn3DGe3;
	CNumerButton m_btn3DGe4;
	CNumerButton m_btn3DGe5;
	CNumerButton m_btn3DGe6;
	CNumerButton m_btn3DGe7;
	CNumerButton m_btn3DGe8;
	CNumerButton m_btn3DGe9;

	CNumerButton m_btnDaBai;
	CNumerButton m_btnXiaoBai;
	CNumerButton m_btnDanBai;
	CNumerButton m_btnShuangBai;
	CNumerButton m_btnDaShi;
	CNumerButton m_btnXiaoShi;
	CNumerButton m_btnDanShi;
	CNumerButton m_btnShuangShi;
	CNumerButton m_btnDaGe;
	CNumerButton m_btnXiaoGe;
	CNumerButton m_btnDanGe;
	CNumerButton m_btnShuangGe;


	CTextButton m_btnSanXingZhiXuan;
	CTextButton m_btnSanXingZuXuan;
	CTextButton m_btnQianYiZhiXuan;
	CTextButton m_btnQianErZhiXuan;
	CTextButton m_btnSanXingBuDingWei;
	CTextButton m_btnHouYiZhiXuan;
	CTextButton m_btnHouErZhiXuan;
	CTextButton m_btnZhiXuanHeZhi;
	CTextButton m_btnDingWeiDan;
	CTextButton m_btnDaXiaoDanShuang;

	CTextButton m_btn3DBaiQuan;
	CTextButton m_btn3DBaiDa;
	CTextButton m_btn3DBaiXiao;
	CTextButton m_btn3DBaiDan;
	CTextButton m_btn3DBaiShuang;
	CTextButton m_btn3DBaiQing;
	CTextButton m_btn3DShiQuan;
	CTextButton m_btn3DShiDa;
	CTextButton m_btn3DShiXiao;
	CTextButton m_btn3DShiDan;
	CTextButton m_btn3DShiShuang;
	CTextButton m_btn3DShiQing;
	CTextButton m_btn3DGeQuan;
	CTextButton m_btn3DGeDa;
	CTextButton m_btn3DGeXiao;
	CTextButton m_btn3DGeDan;
	CTextButton m_btn3DGeShuang;
	CTextButton m_btn3DGeQing;

	CImgStatic m_staticBai;
	CImgStatic m_staticShi;
	CImgStatic m_staticGe;
	CImgStatic m_staticZuXuan;
	CImgStatic m_staticBuDingWei;

	CEdit					m_editDanShiHaoMa;
	CRichEditCtrl m_richDanshiHaoma;

	CImageButton m_btn3DAdd;
	CTextButton m_btnLock;
	CImageButton m_btnDelSel;
	CImageButton m_btnClsList;

	CListCtrlCl m_list3D;
	int m_zongZhuShu;	//单倍总注数

	CSkinEditEx m_editBeiShu;

	CImageButton m_btnTouZhu;
	CImageButton m_btnZhuiHao;

	CImgRioButton m_rioSxzhxFuShi;
	CImgRioButton m_rioSxzhxDanShi;
	CImgRioButton m_rioHeZhxFuShi;
	CImgRioButton m_rioHeZhxDanShi;
	CImgRioButton m_rioSxzsFuShi;
	CImgRioButton m_rioSxzsDanShi;
	CImgRioButton m_rioSxzlFuShi;
	CImgRioButton m_rioSxzlDanShi;
	CImgRioButton m_rioSxzhxHeZhi;
	CImgRioButton m_rioQyZhx;
	CImgRioButton m_rioQeZhxFuShi;
	CImgRioButton m_rioQeZhxDanShi;
	CImgRioButton m_rioSxbdw;
	CImgRioButton m_rioHyzhx;
	CImgRioButton m_rioDwd;
	CImgRioButton m_rioDwDxds;

	UINT m_beishu;

	int m_bigNumWidth;
	int m_bigNumHeight;
	int m_smallNumWidth;
	int m_smallNumHeight;

	CMarkup m_xml;
	TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	int	m_kjXinxiCont;
	KjXinXi m_kjXinxi[Kj_XinXi_Count];

	//绘制小的开奖号时使用的字体
	CFont m_smallNumfont;
	//总金额总注数使用的字体
	CFont m_zongFont;
	//开奖信息使用字体
	CFont m_KjInfoFont;
	bool	m_bKaiJiangzhong;		//正在开奖
	Bitmap*	m_imgKaijiang;			//开奖动画
	int				m_nFrameIndex;				//GIF动画的当前帧数
	int				m_nFrameCount;				//GIF动画总共帧数
	CPlazaViewItem* m_pLuckMeDlg;
	//CFont m_KjInfoFont;

	CFont m_KjInfoFont1;
	int							m_nTzhSign;
	int							m_nTzhSign1;
	bool							m_bShowFail;						//显示失败订单
	float	nPercent;
	float	fDanweiFandian;
	float	nBonus ;

	float						m_fBonus;						//设置的
	float						m_fPercent;						//设置的
	TCHAR				m_szMD5Haoma[33];
	int							m_nZhuihaoCount;				//追号注数
	bool							m_bZhuiZjtz;						//是否中奖停追
	CDlgStatus						m_DlgStatus;						//状态窗口
	//取消连接
	virtual VOID OnStatusCancel();

	VOID SendToServer(int nSendType);
	bool	ChangeStringToA(CString strInput, string&  strOutput);
	CString GetBaiDxdshString(); 
	CString GetShiDxdshString(); 
	CString GetGeDxdshString();

	void ShowDaXiaoDanShuang();
	void HideDaXiaoDanShuang();
	Bitmap* m_bmpDxdsh;
	CString GetDxdshStringDesc(int nNum);
};
