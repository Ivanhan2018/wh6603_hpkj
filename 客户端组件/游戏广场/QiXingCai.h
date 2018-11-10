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
 #include "QrTzhDlg.h"
#include "FanDian.h"
#include	"GridCtrl/GridCtrl.h"
#include "DlgStatus.h"
class CPlazaViewItem;
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"
#include "OpenRecord.h"
typedef CWHArray<CString>	CStringArr;					//类型数组

#define IDM_LUCKY_NUM_SSC			10001
#define IDM_SHOW_MENU_SSC				11110								//显示菜单
#define IDM_CLICKED_TYPE_SSC			11111								//点击游戏
#define IDM_UPDATE_ACCOUNT			11113								//更新玩家资料
#define IDM_SHOW_XGMM				11117								//修改密码

#define IDM_TANCHUANG				900									//弹窗
// 重庆时时彩 对话框
class CQiXingCai : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CQiXingCai)

public:
	CDlgStatus						m_DlgStatus;						//状态窗口
	//取消连接
	virtual VOID OnStatusCancel();

	// 	//七星彩玩法种类---lly

	enum {
		Kj_XinXi_Count = 5
	};

public:
	CQiXingCai(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQiXingCai();
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件
	CImageButton					m_btWxzxTip;
	CImageButton m_btnMoreRecord;
	COpenRecord						m_DlgOpenRecord;					//更多开奖记录
	HBRUSH hBrush;

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int GetQXCZhushu(int nTypeID,int nKindID);
	int				m_singlezhushu;
	DOUBLE			m_zongJine;
	CString GetScoreMoshi();
	int GetMoshiRet(CString moshi);
	QiXingCaiKind GetGameKindByDesc(const CString& desc);

// 对话框数据
	enum { IDD = IDD_DLG_QIXINGCAI };
	int							m_SocketID;
	int							m_nTzhSign;
	int							m_nTzhSign1;
public:
	void ConnectMainDlg(CPlazaViewItem* luckMeDlg);
	void FlushZongjine();
	bool	CheckInput();
	CString		ChangeStringToT(CStringA strInput);
	virtual BOOL PreTranslateMessage(MSG * pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CTextButton m_btnYuan;
	CTextButton m_btnJiao;
	CTextButton m_btnFen;
	CTextButton m_btnLi;
	afx_msg void OnBnClickedBtnYuan();
	afx_msg void OnBnClickedBtnJiao();
	afx_msg void OnBnClickedBtnFen();
	afx_msg void OnBnClickedBtnLi();
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRichEditCopy();


	afx_msg void OnBnClickedBtnWanQuan();
	afx_msg void OnBnClickedBtnMoreRecord();
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
	afx_msg void OnBnClickedBtnChqAdd();

	bool GetKeyHaomaStrsXian(CStringArray& strArr, CString line, int size, int nLeast);
	bool GetKeyBoardHaomaXian(CString& haoma, int size, int nLeast);

	afx_msg void OnBnClickedRioLiangDing();//双定
	afx_msg void OnBnClickedRioSanDing();//三定
	afx_msg void OnBnClickedRioSiDing();//四定
	afx_msg void OnBnClickedRioYiDing();//一定
	afx_msg void OnBnClickedRioLiangZiXian();//双字现
	afx_msg void OnBnClickedRioSanZiXian();//三字现



	afx_msg void OnBnClickedBtnZhuihao();
	afx_msg void OnBnClickedBtnTouzhu();

	afx_msg void OnBnClickedBtnDelSel();
public:
	afx_msg void OnBnClickedBtnClsList();

	afx_msg void OnEnChangeEditDanshiInput();
	afx_msg void OnEnChangeEditBeishu();
	DECLARE_MESSAGE_MAP()

	TCHAR				m_szMD5Haoma[33];
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//组件变量
protected:
	bool							m_bZhuiZjtz;						//是否中奖停追
	int								m_nTouzhuCount;						//下注数
	WORD							m_wGameHoverItem;
	CRect							m_GameListRect;						//列表区域
	bool							m_bShowMenu;						//是否显示菜单
	bool							m_bShowFail;						//显示失败订单
	BYTE							m_cbMenuType;						//菜单类别
	int								m_nMenuXPos;
	int								m_nMenuYPos;
	CFont							m_Font;
	WORD							m_wViewItemDown;					//点击子项
	bool							m_bCanSend	;						//是否发送
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

private:
	long GetKjShjDiff();
	void InitNumberBtns();
	void InitListCtrl();

	void CheckNumBtn(CNumerButton& btn);
	void UnCheckNumBtn(CNumerButton& btn);
	bool IsNumBtnCheck(CNumerButton& btn);

	void HideCtrl(int ctrl_id);
	void ShowCtrl(int ctrl_id);

	void HideAllRio();
	CString GetHezhiString();
	CString GetNiuString();
	CString GetNiuNumString();
	CString GetNiuString(int nNiu);
	CString GetWanString();
	CString GetQianString();
	CString GetBaiString();
	CString GetShiString();
	CString GetGeString();

	//获取大小单双
	CString GetShiDxdshString(); 
	CString GetGeDxdshString();

	CString GetWanDesc();

	void AdjustWanFa();
	void AdjustRadio();
	void AdjustNumBtn();
	void AdjustNumView();
	void AdjustDanShi();
	void AdjustAdd();

	void HideHeZhiNums();
	void ShowHeZhiNums();

	void HideWanNums();
	void HideQianNums();
	void HideBaiNums();
	void HideShiNums();
	void HideGeNums();
	void HideNiuNums();
	void ShowWanNums();
	void ShowQianNums();
	void ShowBaiNums();
	void ShowShiNums();
	void ShowGeNums();
	void ShowNiuNums();

	//显示隐藏组选和不定位
	void ShowZuXuan();
	void ShowDanHao();
	void ShowBuDingWei();
	void ShowXuanHao();
	void ShowZu120();
	void ShowErChong();
	void ShowErChong1();
	void ShowSanChong();
	void ShowSiChong();

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


	//获取组三形式的号码
	bool GetZuSanHaoma(CStringArray& strArr);
	bool GetZuSanHaoma(CString& haoma, int& zhushu);
	bool IsZuSanHaoma(const CString& str);
	//获取单式号码
	bool GetDanShiHaoma(CStringArray& strArr, int size);
	bool GetDanShiHaoma(CString& haoma, int size, int& zhushu, int nMax=0);
	bool GetHaomaStrs(CStringArray& strArr, CString line, int size);

	bool GetKeyBoardHaoma(CString& haoma, int size, int nLeast);
	bool GetKeyHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast);

	//初始化玩法信息
	void InitWanFaMoShi();
public:
	VOID SendQuerySystemTime();
	//服务器端返回前N个开奖号码
	bool GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nDataSize);

private:
	Bitmap* m_bmpBk;
	Bitmap* m_numImageList;
	Bitmap* m_numImageList1;
	Bitmap* m_numImageList2;
	Bitmap* m_kjNumBig;
	Bitmap* m_kjNumSmall;
	Bitmap* m_bmpDxdsh;
	Bitmap* m_bmpNumTip;
	Bitmap* m_bmpNumTip1;
	Bitmap*	m_imgKaijiang;			//开奖动画
	int m_bigNumWidth;
	int m_bigNumHeight;
	int m_smallNumWidth;
	int m_smallNumHeight;
	bool	m_bKaiJiangzhong;		//正在开奖

	QiXingCaiKind m_gameKind;

	CNumerButton m_btnHeZhi_0;
	CNumerButton m_btnHeZhi_1;
	CNumerButton m_btnHeZhi_2;
	CNumerButton m_btnHeZhi_3;
	CNumerButton m_btnHeZhi_4;
	CNumerButton m_btnHeZhi_5;
	CNumerButton m_btnHeZhi_6;
	CNumerButton m_btnHeZhi_7;
	CNumerButton m_btnHeZhi_8;
	CNumerButton m_btnHeZhi_9;
	CNumerButton m_btnHeZhi_10;
	CNumerButton m_btnHeZhi_11;
	CNumerButton m_btnHeZhi_12;
	CNumerButton m_btnHeZhi_13;
	CNumerButton m_btnHeZhi_14;
	CNumerButton m_btnHeZhi_15;
	CNumerButton m_btnHeZhi_16;
	CNumerButton m_btnHeZhi_17;
	CNumerButton m_btnHeZhi_18;
	CNumerButton m_btnHeZhi_19;
	CNumerButton m_btnHeZhi_20;
	CNumerButton m_btnHeZhi_21;
	CNumerButton m_btnHeZhi_22;
	CNumerButton m_btnHeZhi_23;
	CNumerButton m_btnHeZhi_24;
	CNumerButton m_btnHeZhi_25;
	CNumerButton m_btnHeZhi_26;
	CNumerButton m_btnHeZhi_27;
	
	CNumerButton m_btnNiu_0;
	CNumerButton m_btnNiu_1;
	CNumerButton m_btnNiu_2;
	CNumerButton m_btnNiu_3;
	CNumerButton m_btnNiu_4;
	CNumerButton m_btnNiu_5;
	CNumerButton m_btnNiu_6;
	CNumerButton m_btnNiu_7;
	CNumerButton m_btnNiu_8;
	CNumerButton m_btnNiu_9;
	CNumerButton m_btnNiu_10;


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

	CListCtrlCl m_listChqSSC;
	CGridCtrl	m_GridList;
	int				m_zongZhuShu;				//单倍总注数
	bool			m_RenxuanKeyboardInput;		//任选N的键盘输入

	UINT			m_beishu;					//倍数
	int				m_nFrameIndex;				//GIF动画的当前帧数
	int				m_nFrameCount;				//GIF动画总共帧数
	CEdit m_editDanShiHaoMa;
	CRichEditCtrl m_richDanshiHaoma;
	CImgStatic m_staticWan;
	CImgStatic m_staticQian;
	CImgStatic m_staticBai;
	CImgStatic m_staticShi;
	CImgStatic m_staticGe;
	CImgStatic m_staticNiu;

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

	CTextButton m_btnNiuQuan;
	CTextButton m_btnNiuDa;
	CTextButton m_btnNiuXiao;
	CTextButton m_btnNiuDan;
	CTextButton m_btnNiuShuang;
	CTextButton m_btnNiuQing;

	CImageButton m_btnDelSel;
	CImageButton m_btnClsList;
	CImageButton m_btnTouZhu;
	CImageButton m_btnZhuiHao;

	CSkinEditEx m_editBeiTou;

	CImageButton m_btnChqAdd;
	CTextButton m_btnLock;

	CTextButton m_btnLiangDing;
	CTextButton m_btnSanDing;
	CTextButton m_btnSiDing;
	CTextButton m_btnYiDing;
	CTextButton m_btnLiangZiXian;
	CTextButton m_btnSanZiXian;


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

 	CImgRioButton m_rioWuXingZX;
 	CImgRioButton m_rioWuXingQS;
 	CImgRioButton m_rioWuXingHS;
 	CImgRioButton m_rioWuXingZS;
	CImgRioButton m_rioNiuShu;
	CImgRioButton m_rioNiuNiu;
	CImgRioButton m_rioWuNiu;

	CImgRioButton m_rioHsZhxFuShi;
	CImgRioButton m_rioHsZhxDanShi;
	CImgRioButton m_rioQsZhxFuShi;
	CImgRioButton m_rioQsZhxDanShi;
	CImgRioButton m_rioHsZsFuShi;
	CImgRioButton m_rioHsZlFuShi;
	CImgRioButton m_rioQsZsFuShi;
	CImgRioButton m_rioQsZlFuShi;
	CImgRioButton m_rioHeZhxFuShi;
	CImgRioButton m_rioHeZhxDanShi;
	CImgRioButton m_rioQeZhxFuShi;
	CImgRioButton m_rioQeZhxDanShi;
	CImgRioButton m_rioHeZxFuShi;
	CImgRioButton m_rioHeZxDanShi;
	CImgRioButton m_rioQeZxFuShi;
	CImgRioButton m_rioQeZxDanShi;
	CImgRioButton m_rioQeDxdsh;
	CImgRioButton m_rioQsbdw;
	CImgRioButton m_rioHsbdw;
	CImgRioButton m_rioDwd;
	CImgRioButton m_rioHebdw;
	CImgRioButton m_rioQebdw;

	CImgRioButton m_rioZsZsFuShi;			//中三组三
	CImgRioButton m_rioZsZlFuShi;			//中三组六
	CImgRioButton m_rioZuXuan120;
	CImgRioButton m_rioZuXuan60;
	CImgRioButton m_rioZuXuan30;
	CImgRioButton m_rioZuXuan20;
	CImgRioButton m_rioZuXuan10;
	CImgRioButton m_rioZuXuan5;			

	CImgRioButton m_rioYffs;
	CImgRioButton m_rioHscs;
	CImgRioButton m_rioSxbx;
	CImgRioButton m_rioSjfc;	

	CImgRioButton m_rioQsHezhi;
	CImgRioButton m_rioHsHezhi;

	CImgRioButton m_rioQsHunhe;				//前三混合
	CImgRioButton m_rioZsHunhe;				//中三混合
	CImgRioButton m_rioHsHunhe;				//后三混合

	CImgStatic m_staticZuXuan;
	CImgStatic m_staticBuDingWei;
	CStatic		m_staticHunheTip;
	CImgStatic m_staticDanHao;
	CImgStatic m_staticXuanHao;
	CImgStatic m_staticZu120;
	CImgStatic m_staticErChong;
	CImgStatic m_staticErChong1;
	CImgStatic m_staticSanChong;
	CImgStatic m_staticSiChong;

	//大小单双
	CNumerButton m_btnDaShi;
	CNumerButton m_btnXiaoShi;
	CNumerButton m_btnDanShi;
	CNumerButton m_btnShuangShi;
	CNumerButton m_btnDaGe;
	CNumerButton m_btnXiaoGe;
	CNumerButton m_btnDanGe;
	CNumerButton m_btnShuangGe;



	CImageButton m_btnBeiTouJia;
	CImageButton m_btnBeiTouJian;



	static CString m_strHaoma;

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
	CString GetKjShjDiffDesc(int nSecond);
	CQiXingCaiRule m_chqRule;
public:
	VOID AddOpenResult(CMD_GP_QueryLotResult* pQueryLotResult);
	//重载函数
public:
	bool	ChangeStringToA(CString strInput, string&  strOutput);
	void	GetSubStringA(CStringA& strInput, int nPos, int nLen, CStringA& strOutput);

	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
	bool							m_bLogonSuccess;

	VOID SetLogonSuccess(bool bSuccess)
	{
		m_bLogonSuccess = bSuccess;
	}
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
