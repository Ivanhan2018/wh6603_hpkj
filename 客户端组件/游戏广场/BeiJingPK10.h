#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "comm.h"
#include "NumerButton.h"
#include "TextButton.h"
#include "ImageButton.h"
#include "ImgRioButton.h"
#include "ImgStatic.h"
#include "GameRule.h"
#include "resource.h"
// #include "GamePlaza.h"
 #include "QrTzhDlg.h"
#include "FanDian.h"
#include "DlgStatus.h"
#include "OpenRecord.h"
class CPlazaViewItem;


#define IDM_LUCKY_NUM_SSC			10001
#define IDM_SHOW_MENU_SSC				11110								//显示菜单
#define IDM_CLICKED_TYPE_SSC			11111								//点击游戏
#define IDM_SHOW_MENU				11112								//显示菜单
#define IDM_UPDATE_ACCOUNT			11113								//更新玩家资料
#define IDM_SHOW_XGMM				11117								//修改密码

// 重庆11选5 对话框
class CBeiJingPK10 : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CBeiJingPK10)
public:
	CDlgStatus						m_DlgStatus;						//状态窗口
	//取消连接
	virtual VOID OnStatusCancel();

public:


	enum {
		Kj_XinXi_Count = 5
	};
	CString GetDxdshStringDesc(int nNum);
public:
	CBeiJingPK10(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBeiJingPK10();
	bool	ChangeStringToA(CString strInput, string&  strOutput);
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件
	CImageButton					m_btWxzxTip;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int GetPk10Zhushu(int nTypeID,int nKindID);
	int				m_singlezhushu;
	DOUBLE			m_zongJine;
	CString GetScoreMoshi();
	int GetMoshiRet(CString moshi);

// 对话框数据
	enum { IDD = IDD_DLG_BJ_PK10 };
	int							m_nTzhSign;
	int							m_nTzhSign1;
	static CString m_strHaoma;
	bool							m_bCanSend	;						//是否发送
	TCHAR				m_szMD5Haoma[33];
	HBRUSH hBrush;
public:
	void ConnectMainDlg(CPlazaViewItem* luckMeDlg);
	void FlushZongjine();
	bool	CheckInput();
	CImageButton m_btnMoreRecord;
	COpenRecord						m_DlgOpenRecord;					//更多开奖记录
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedBtnMoreRecord();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CTextButton m_btnYuan;
	CTextButton m_btnJiao;
	CTextButton m_btnFen;
	CTextButton m_btnLi;

	CTextButton m_btnWanQuan;
	CTextButton m_btnWanDa;
	CTextButton m_btnWanXiao;
	CTextButton m_btnWanDan;
	CTextButton m_btnWanShuang;
	CTextButton m_btnWanQing;

	CTextButton m_btnQianQuan;
	CTextButton m_btnQianDa;
	CTextButton m_btnQianXiao;
	CTextButton m_btnQianDan;
	CTextButton m_btnQianShuang;
	CTextButton m_btnQianQing;

	CTextButton m_btnBaiQuan;
	CTextButton m_btnBaiDa;
	CTextButton m_btnBaiXiao;
	CTextButton m_btnBaiDan;
	CTextButton m_btnBaiShuang;
	CTextButton m_btnBaiQing;

	CTextButton m_btnShiQuan;
	CTextButton m_btnShiDa;
	CTextButton m_btnShiXiao;
	CTextButton m_btnShiDan;
	CTextButton m_btnShiShuang;
	CTextButton m_btnShiQing;

	CTextButton m_btnGeQuan;
	CTextButton m_btnGeDa;
	CTextButton m_btnGeXiao;
	CTextButton m_btnGeDan;
	CTextButton m_btnGeShuang;
	CTextButton m_btnGeQing;

	afx_msg void OnBnClickedBtnWanQuan();
	afx_msg void OnBnClickedBtnWanDa();
	afx_msg void OnBnClickedBtnWanXiao();
	afx_msg void OnBnClickedBtnWanDan();
	afx_msg void OnBnClickedBtnWanShuang();
	afx_msg void OnBnClickedBtnWanQing();
	afx_msg void OnBnClickedBtnQianQuan();
	afx_msg void OnBnClickedBtnQianDa();
	afx_msg void OnBnClickedBtnQianXiao();
	afx_msg void OnBnClickedBtnQianDan();
	afx_msg void OnBnClickedBtnQianShuang();
	afx_msg void OnBnClickedBtnQianQing();
	afx_msg void OnBnClickedBtnBaiQuan();
	afx_msg void OnBnClickedBtnBaiDa();
	afx_msg void OnBnClickedBtnBaiXiao();
	afx_msg void OnBnClickedBtnBaiDan();
	afx_msg void OnBnClickedBtnBaiShuang();
	afx_msg void OnBnClickedBtnBaiQing();
	afx_msg void OnBnClickedBtnShiQuan();
	afx_msg void OnBnClickedBtnShiDa();
	afx_msg void OnBnClickedBtnShiXiao();
	afx_msg void OnBnClickedBtnShiDan();
	afx_msg void OnBnClickedBtnShiShuang();
	afx_msg void OnBnClickedBtnShiQing();
	afx_msg void OnBnClickedBtnGeQuan();
	afx_msg void OnBnClickedBtnGeDa();
	afx_msg void OnBnClickedBtnGeXiao();
	afx_msg void OnBnClickedBtnGeDan();
	afx_msg void OnBnClickedBtnGeShuang();
	afx_msg void OnBnClickedBtnGeQing();
	void HideWanNumsQ();
	void HideQianNumsQ();
	void HideBaiNumsQ();
	void HideShiNumsQ();
	void HideGeNumsQ();
	int GetDanShiZhusu(int nKind);
	void ShowWanNumsQ();
	void ShowQianNumsQ();
	void ShowBaiNumsQ();
	void ShowShiNumsQ();
	void ShowGeNumsQ();

	afx_msg void OnEnChangeRichEditDanshiInput();

	afx_msg void OnBnClickedBtnYuan();
	afx_msg void OnBnClickedBtnJiao();
	afx_msg void OnBnClickedBtnFen();
	afx_msg void OnBnClickedBtnLi();

	afx_msg void OnBnClickedBtnHszhx();
	afx_msg void OnBnClickedBtnHszx();
	afx_msg void OnBnClickedBtnWxhe();
	afx_msg void OnBnClickedBtnHezx();
	afx_msg void OnBnClickedBtnDxdsh();
	afx_msg void OnBnClickedBtnBdw();
	afx_msg void OnBnClickedBtnQszhx();
	afx_msg void OnBnClickedBtnQszx();
	afx_msg void OnBnClickedBtnWxqe();
	afx_msg void OnBnClickedBtnQezx();
	afx_msg void OnBnClickedBtnQSDXDS();
	afx_msg void OnBnClickedBtnHszhxhzh();
	afx_msg void OnBnClickedBtnDwd();

	afx_msg void OnBnClickedBtnChqAdd();

	afx_msg void OnBnClickedRioRenXuan2();
	afx_msg void OnBnClickedRioHszhxFushi();
	afx_msg void OnBnClickedRioHszsFushi();
	afx_msg void OnBnClickedRioHszsFushiDan();
	afx_msg void OnBnClickedRioHszsDanshi();//猜亚军
	afx_msg void OnBnClickedRioQszsFushi();
	afx_msg void OnBnClickedRioQszsDanshi();//第八
	afx_msg void OnBnClickedRioHezhxFushi();
	afx_msg void OnBnClickedRioHezhxFushiDan();
	afx_msg void OnBnClickedRioHezhxDanshi();//猜第三
	afx_msg void OnBnClickedRioHezxFushi();
	afx_msg void OnBnClickedRioHezxFushiDan();
	afx_msg void OnBnClickedRioHezxDanshi();//猜第四
	afx_msg void OnBnClickedRioQezhxFushi();
	afx_msg void OnBnClickedRioQezhxDanshi();
	afx_msg void OnBnClickedRioQezxFushi();
	afx_msg void OnBnClickedRioQsDxds();//前十大小单双
	afx_msg void OnBnClickedRioQezxDanshi();
	afx_msg void OnBnClickedRioDxdsh();
	afx_msg void OnBnClickedRioDxdshDan();
	afx_msg void OnBnClickedRioQsbdw();//猜第五
	afx_msg void OnBnClickedRioHsbdw();
	afx_msg void OnBnClickedRioQsHunhe();//猜第六
	afx_msg void OnBnClickedRioRenXuan3();//猜第七

	afx_msg void OnBnClickedBtnZhuihao();
	afx_msg void OnBnClickedBtnTouzhu();

	afx_msg void OnBnClickedBtnDelSel();
	afx_msg void OnBnClickedBtnClsList();

	afx_msg void OnEnChangeEditDanshiInput();
	afx_msg void OnEnChangeEditBeishu();
	afx_msg void OnRichEditCopy();
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//组件变量
protected:
	bool							m_bShowFail ;

	WORD							m_wGameHoverItem;
	CRect							m_GameListRect;						//列表区域
	bool							m_bShowMenu;						//是否显示菜单
	BYTE							m_cbMenuType;						//菜单类别
	int								m_nMenuXPos;
	int								m_nMenuYPos;
	CFont							m_Font;
	WORD							m_wViewItemDown;					//点击子项

	CFanDian						m_FandianView;						//返点
	//对象索引
	WORD GetGameHoverIndex(CPoint MousePoint);


	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
// 	//鼠标离开
// 	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
// 

private:
	long GetKjShjDiff();
	//void InitWanFaBtns();
	void InitNumberBtns();
	void InitListCtrl();

	void CheckNumBtn(CNumerButton& btn);
	void UnCheckNumBtn(CNumerButton& btn);
	bool IsNumBtnCheck(CNumerButton& btn);

	void HideCtrl(int ctrl_id);
	void ShowCtrl(int ctrl_id);

	void HideAllRio();

	CString GetWanString();
	CString GetQianString();
	CString GetBaiString();
	CString GetShiString();
	CString GetGeString();

	CString GetLiuString();
	CString GetQiString();
	CString GetBaString();
	CString GetJiuString();
	CString GetShi10String();

	//获取大小单双
	CString GetShiDxdshString(); 
	CString GetGuanDxdshString();
	CString GetYaDxdshString();
	CString GetSanDxdshString();
	CString GetSiDxdshString();
	CString GetWuDxdshString();
	CString GetLiuDxdshString();
	CString GetQiDxdshString();
	CString GetBaDxdshString();
	CString GetJiuDxdshString();

	CString GetWanDesc();
	PK10GameKind GetGameKindByDesc(const CString& desc);

	void AdjustWanFa();
	void AdjustRadio();
	void AdjustNumBtn();
	void AdjustNumView();
	void AdjustDanShi();
	void AdjustAdd();

	void HideWanNums();
	void HideQianNums();
	void HideBaiNums();
	void HideShiNums();
	void HideGeNums();
	void HideLiuNums();
	void HideQiNums();
	void HideBaNums();
	void HideJiuNums();
	void HideShi10Nums();

	void ShowWanNums();
	void ShowQianNums();
	void ShowBaiNums();
	void ShowShiNums();
	void ShowGeNums();
	void ShowLiuNums();
	void ShowQiNums();
	void ShowBaNums();
	void ShowJiuNums();
	void ShowShi10Nums();

	//显示隐藏组选和不定位
	void ShowZuXuan();
	void ShowBuDingWei();

	//显示或隐藏大小单双
	void ShowDaXiaoDanShuang();
	void HideDaXiaoDanShuang();
	
	//显示或隐藏单式选号
	void HideDanShiEdit();
	void ShowDanShiEdit();
	//任选当中的万，千，百，十，个位数
	void ShowRenxuanWeiShu();
	void HideRenxuanWeiShu();
	void SetCheckRenXuan(bool bChecked);
	void CheckWanFaBtn(CTextButton& btn);
	void ValidateTips();
	//返回选中控件的个数
	int GetRenXuanCheckBox(BOOL& bWan, BOOL& bQian, BOOL& bBai, BOOL& bShi, BOOL& bGet);		//任选的万，千，百，十，个

	//从XML当中获取开奖号码
	bool LuckyNumCQSSC();

	void DrawLastKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawTopKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawZongZhushuJinE(CDC* pDC);
	void SetLockText(float fBonus=0.0f, float fBonusPercent=0.0f);
	void ResetAllNums();
	VOID SendQueryLuckyNumCQSSC();

	VOID SendToServer(int nSendType);

	//void SetLockText();

	//获取组三形式的号码
	bool GetZuSanHaoma(CStringArray& strArr);
	bool GetZuSanHaoma(CString& haoma, int& zhushu);
	bool IsZuSanHaoma(const CString& str);
	//获取单式号码
	bool GetDanShiHaoma(CStringArray& strArr, int size);
	bool GetDanShiHaoma(CString& haoma, int size, int nLeast);
	bool GetHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast);

	//初始化玩法信息
	void InitWanFaMoShi();
public:
	VOID SendQuerySystemTime();
	//服务器端返回前N个开奖号码
	bool GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nDataSize);
private:
	Bitmap* m_bmpBk;
	Bitmap* m_numImageList;
	Bitmap* m_kjNumBig;
	Bitmap* m_kjNumSmall;
	Bitmap* m_bmpDxdsh;
	Bitmap* m_bmpNumTip;
	Bitmap*	m_imgKaijiang;			//开奖动画
	int m_bigNumWidth;
	int m_bigNumHeight;
	int m_smallNumWidth;
	int m_smallNumHeight;
	bool	m_bKaiJiangzhong;		//正在开奖

	PK10GameKind m_gameKind;

	CNumerButton m_btnWan_0;
	CNumerButton m_btnWan_1;
	CNumerButton m_btnWan_2;
	CNumerButton m_btnWan_3;
	CNumerButton m_btnWan_4;
	CNumerButton m_btnWan_5;
	CNumerButton m_btnWan_6;
	CNumerButton m_btnWan_7;
	CNumerButton m_btnWan_8;
	CNumerButton m_btnWan_9;

	CNumerButton m_btnQian_0;
	CNumerButton m_btnQian_1;
	CNumerButton m_btnQian_2;
	CNumerButton m_btnQian_3;
	CNumerButton m_btnQian_4;
	CNumerButton m_btnQian_5;
	CNumerButton m_btnQian_6;
	CNumerButton m_btnQian_7;
	CNumerButton m_btnQian_8;
	CNumerButton m_btnQian_9;

	CNumerButton m_btnBai_0;
	CNumerButton m_btnBai_1;
	CNumerButton m_btnBai_2;
	CNumerButton m_btnBai_3;
	CNumerButton m_btnBai_4;
	CNumerButton m_btnBai_5;
	CNumerButton m_btnBai_6;
	CNumerButton m_btnBai_7;
	CNumerButton m_btnBai_8;
	CNumerButton m_btnBai_9;

	CNumerButton m_btnShi_0;
	CNumerButton m_btnShi_1;
	CNumerButton m_btnShi_2;
	CNumerButton m_btnShi_3;
	CNumerButton m_btnShi_4;
	CNumerButton m_btnShi_5;
	CNumerButton m_btnShi_6;
	CNumerButton m_btnShi_7;
	CNumerButton m_btnShi_8;
	CNumerButton m_btnShi_9;

	CNumerButton m_btnGe_0;
	CNumerButton m_btnGe_1;
	CNumerButton m_btnGe_2;
	CNumerButton m_btnGe_3;
	CNumerButton m_btnGe_4;
	CNumerButton m_btnGe_5;
	CNumerButton m_btnGe_6;
	CNumerButton m_btnGe_7;
	CNumerButton m_btnGe_8;
	CNumerButton m_btnGe_9;

/////////////////////////////////////////////////////////////////////
	CNumerButton m_btnLiu_0;
	CNumerButton m_btnLiu_1;
	CNumerButton m_btnLiu_2;
	CNumerButton m_btnLiu_3;
	CNumerButton m_btnLiu_4;
	CNumerButton m_btnLiu_5;
	CNumerButton m_btnLiu_6;
	CNumerButton m_btnLiu_7;
	CNumerButton m_btnLiu_8;
	CNumerButton m_btnLiu_9;

	CNumerButton m_btnQi_0;
	CNumerButton m_btnQi_1;
	CNumerButton m_btnQi_2;
	CNumerButton m_btnQi_3;
	CNumerButton m_btnQi_4;
	CNumerButton m_btnQi_5;
	CNumerButton m_btnQi_6;
	CNumerButton m_btnQi_7;
	CNumerButton m_btnQi_8;
	CNumerButton m_btnQi_9;

	CNumerButton m_btnBa_0;
	CNumerButton m_btnBa_1;
	CNumerButton m_btnBa_2;
	CNumerButton m_btnBa_3;
	CNumerButton m_btnBa_4;
	CNumerButton m_btnBa_5;
	CNumerButton m_btnBa_6;
	CNumerButton m_btnBa_7;
	CNumerButton m_btnBa_8;
	CNumerButton m_btnBa_9;

	CNumerButton m_btnJiu_0;
	CNumerButton m_btnJiu_1;
	CNumerButton m_btnJiu_2;
	CNumerButton m_btnJiu_3;
	CNumerButton m_btnJiu_4;
	CNumerButton m_btnJiu_5;
	CNumerButton m_btnJiu_6;
	CNumerButton m_btnJiu_7;
	CNumerButton m_btnJiu_8;
	CNumerButton m_btnJiu_9;

	CNumerButton m_btnShi10_0;
	CNumerButton m_btnShi10_1;
	CNumerButton m_btnShi10_2;
	CNumerButton m_btnShi10_3;
	CNumerButton m_btnShi10_4;
	CNumerButton m_btnShi10_5;
	CNumerButton m_btnShi10_6;
	CNumerButton m_btnShi10_7;
	CNumerButton m_btnShi10_8;
	CNumerButton m_btnShi10_9;

	bool							m_bZhuiZjtz;						//是否中奖停追
//////////////////////////////////////////////
	CListCtrlCl m_listChqSSC;
	int				m_zongZhuShu;				//单倍总注数
	bool			m_RenxuanKeyboardInput;		//任选N的键盘输入

	//CArray<TouZhuOrder, TouZhuOrder&> m_arrOrder;
	UINT			m_beishu;					//倍数
	int				m_nFrameIndex;				//GIF动画的当前帧数
	int				m_nFrameCount;				//GIF动画总共帧数
	//CStatic m_staticDanShi;
	CEdit m_editDanShiHaoMa;
	CRichEditCtrl m_richDanshiHaoma;

	CImgStatic m_staticWan;
	CImgStatic m_staticQian;
	CImgStatic m_staticBai;
	CImgStatic m_staticShi;
	CImgStatic m_staticGe;
	CImgStatic m_staticLiu;
	CImgStatic m_staticQi;
	CImgStatic m_staticBa;
	CImgStatic m_staticJiu;
	CImgStatic m_staticShi10;

	CImageButton m_btnDelSel;
	CImageButton m_btnClsList;
	CImageButton m_btnTouZhu;
	CImageButton m_btnZhuiHao;

	CSkinEditEx m_editBeiTou;

	CImageButton m_btnChqAdd;
	CTextButton m_btnLock;

	CTextButton m_btnHouSanZhiXuan;
	CTextButton m_btnQianSanZhiXuan;
	CTextButton m_btnHouSanZuXuan;
	CTextButton m_btnQianSanZuXuan;
	CTextButton m_btnWuXingHouEr;
	CTextButton m_btnWuXingQianEr;
	CTextButton m_btnHouErZuXuan;
	CTextButton m_btnQianErZuXuan;
	CTextButton m_btnDaXiaoDanShuang;
	CTextButton m_btnQSDaXiaoDanShuang;
	CTextButton m_btnBuDingWei;
	CTextButton m_btnDingWeiDan;

	CMarkup m_xml;

	TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	int	m_kjXinxiCont;
	KjXinXi m_kjXinxi[Kj_XinXi_Count];

	CImgRioButton m_rioRenXuan2;
	CImgRioButton m_rioRenXuan3;
	CImgRioButton m_rioRenXuan4;
	CImgRioButton m_rioMouseInput;
	CImgRioButton m_rioKeyboardInput;
	//
	CButton		m_rioWan;
	CButton		m_rioQian;
	CButton		m_rioBai;
	CButton		m_rioShi;
	CButton		m_rioGe;

	CImgRioButton m_rioHsZhxFuShi;
	CImgRioButton m_rioHsZhxDanShi;
	CImgRioButton m_rioQsZhxFuShi;
	CImgRioButton m_rioQsZhxDanShi;
	CImgRioButton m_rioHsZsFuShi;
	CImgRioButton m_rioHsZsFuShiDanshi;
	CImgRioButton m_rioHsZsDanShi;
	CImgRioButton m_rioHsZlFuShi;
	//CImgRioButton m_rioHsZlDanShi;
	CImgRioButton m_rioQsZsFuShi;
	CImgRioButton m_rioQsZsDanShi;
	CImgRioButton m_rioQsZlFuShi;
	//CImgRioButton m_rioQsZlDanShi;
	CImgRioButton m_rioHeZhxFuShi;
	CImgRioButton m_rioHeZhxFuShiDan;
	CImgRioButton m_rioHeZhxDanShi;
	CImgRioButton m_rioQeZhxFuShi;
	CImgRioButton m_rioQeZhxDanShi;
	CImgRioButton m_rioHeZxFuShi;
	CImgRioButton m_rioHeZxFuShiDan;
	CImgRioButton m_rioHeZxDanShi;
	CImgRioButton m_rioQeZxFuShi;
	CImgRioButton m_rioQeZxDanShi;
	CImgRioButton m_rioQeDxdsh;
	CImgRioButton m_rioQeDxdshDan;
	CImgRioButton m_rioQsbdw;
	CImgRioButton m_rioHsbdw;
	CImgRioButton m_rioDwd;

	CImgRioButton m_rioQsHunhe;				//前三混合
	CImgRioButton m_rioZsHunhe;				//中三混合
	CImgRioButton m_rioHsHunhe;				//后三混合

	CStatic		m_staticHunheTip;

	//大小单双
	CNumerButton m_btnDaGUAN;
	CNumerButton m_btnXiaoGUAN;
	CNumerButton m_btnDanGUAN;
	CNumerButton m_btnShuangGUAN;
	CNumerButton m_btnDaYA;
	CNumerButton m_btnXiaoYA;
	CNumerButton m_btnDanYA;
	CNumerButton m_btnShuangYA;

	CNumerButton m_btnDaSAN;
	CNumerButton m_btnXiaoSAN;
	CNumerButton m_btnDanSAN;
	CNumerButton m_btnShuangSAN;
	CNumerButton m_btnDaSI;
	CNumerButton m_btnXiaoSI;
	CNumerButton m_btnDanSI;
	CNumerButton m_btnShuangSI;

	CNumerButton m_btnDaWU;
	CNumerButton m_btnXiaoWU;
	CNumerButton m_btnDanWU;
	CNumerButton m_btnShuangWU;
	CNumerButton m_btnDaLIU;
	CNumerButton m_btnXiaoLIU;
	CNumerButton m_btnDanLIU;
	CNumerButton m_btnShuangLIU;

	CNumerButton m_btnDaQI;
	CNumerButton m_btnXiaoQI;
	CNumerButton m_btnDanQI;
	CNumerButton m_btnShuangQI;
	CNumerButton m_btnDaBA;
	CNumerButton m_btnXiaoBA;
	CNumerButton m_btnDanBA;
	CNumerButton m_btnShuangBA;

	CNumerButton m_btnDaJIU;
	CNumerButton m_btnXiaoJIU;
	CNumerButton m_btnDanJIU;
	CNumerButton m_btnShuangJIU;
	CNumerButton m_btnDaShi;
	CNumerButton m_btnXiaoShi;
	CNumerButton m_btnDanShi;
	CNumerButton m_btnShuangShi;
	//绘制小的开奖号时使用的字体
	CFont m_smallNumfont;
	//总金额总注数使用的字体
	CFont m_zongFont;
	//开奖信息使用字体
	CFont m_KjInfoFont;
	CFont m_KjInfoFont1;
	afx_msg void OnBnClickedRioMouseInput();
	afx_msg void OnBnClickedRioKeyboardInput();
	afx_msg void OnBnClickedChkWan();
	afx_msg void OnBnClickedChkQian();
	afx_msg void OnBnClickedChkBai();
	afx_msg void OnBnClickedChkShi();
	afx_msg void OnBnClickedChkge();
	afx_msg void OnBnClickedBtnLock();
	CPlazaViewItem* m_pLuckMeDlg;
	afx_msg void OnBnClickedBeiTouJia();
	afx_msg void OnBnClickedBeiTouJian();
	CImageButton m_btnBeiTouJia;
	CImageButton m_btnBeiTouJian;

	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );

	CBJPK10Rule m_chqRule;
public:
	VOID AddOpenResult(CMD_GP_QueryLotResult* pQueryLotResult);
	//重载函数
public:
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
public:
	bool						m_bQueryTime;					//是否查询时间
	bool						m_bQueryLuckyNum;					//是否查询时间
	bool						m_bTouzhu;						//是否下注
	bool						m_bHovering;
	bool						m_bGetTime;
	bool						m_bGetCPUserInfo;				//获取彩票用户信息
	bool						m_bZhuihao;	
	bool						m_bGetUserFandian;				//获取返点
	bool						m_bGetMapBonus;
	bool						m_bGetLastYue;
	bool						m_bSetBonus;					//
	float						m_fBonus;						//设置的
	float						m_fPercent;						//设置的
	int							m_nZhuihaoCount;				//追号注数
	float	nPercent;
	float	fDanweiFandian;
	float	nBonus ;

};

