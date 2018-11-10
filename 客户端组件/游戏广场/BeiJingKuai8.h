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
#include	"GridCtrl/GridCtrl.h"
#include "DlgStatus.h"
class CPlazaViewItem;
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"

typedef CWHArray<CString>	CStringArr;					//类型数组



#define IDM_LUCKY_NUM_SSC			10001
#define IDM_SHOW_MENU_SSC			11110								//显示菜单
#define IDM_CLICKED_TYPE_SSC		11111								//点击游戏
#define IDM_UPDATE_ACCOUNT			11113								//更新玩家资料
#define IDM_SHOW_XGMM				11117								//修改密码

#define IDM_TANCHUANG				900									//弹窗



// CBeiJingKuai8 对话框

class CBeiJingKuai8 : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CBeiJingKuai8)

public:
	CDlgStatus						m_DlgStatus;						//状态窗口
	//取消连接
	virtual VOID OnStatusCancel();


	enum {
		Kj_XinXi_Count = 5
	};
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件
	CImageButton					m_btWxzxTip;
	virtual BOOL PreTranslateMessage(MSG * pMsg);


public:
	CBeiJingKuai8(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBeiJingKuai8();



	HBRUSH hBrush;

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int GetKl8Zhushu(int nTypeID,int nKindID);
	int				m_singlezhushu;
	DOUBLE			m_zongJine;

	CString GetScoreMoshi();
	int GetMoshiRet(CString moshi);

// 对话框数据
	enum { IDD = IDD_BEIJINGKUAI8 };

	int							m_SocketID;
	int							m_nTzhSign;
	int							m_nTzhSign1;
public:
	void ConnectMainDlg(CPlazaViewItem* luckMeDlg);
	void FlushZongjine();
	bool	CheckInput();//校验输入
	CString		ChangeStringToT(CStringA strInput);


	BeiJingKuai8Kind GetGameKindByDesc(const CString& desc);




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

	//玩法按钮
	afx_msg void OnBnClinckedBtnRenXuan();	//任选

	afx_msg void OnBnClinckedBtnTeMa();	//特码
	afx_msg void OnBnClinckedBtnDaXiaoDanShuang();	//大小单双 
	afx_msg void OnBnClinckedBtnFunny();//趣味玩法
	afx_msg void OnBnClinckedBtnJiZhi();	//极值

	afx_msg void OnBnClinckedBtnRenXuan6();	//任选
	afx_msg void OnBnClinckedBtnRenXuan7();	//特码
	afx_msg void OnBnClinckedBtnRenXuan8();	//大小单双 
	afx_msg void OnBnClinckedBtnRenXuan9();//趣味玩法
	afx_msg void OnBnClinckedBtnRenXuan10();	//极值
	afx_msg void OnBnClinckedBtnQuWei();	//趣味

	afx_msg void OnRioDuiZi();	//对子
	afx_msg void OnRioShunZi();	//顺子
	afx_msg void OnRioBaoZi();	//豹子

	//afx_msg void OnRioMax();		//极大
	//afx_msg void OnRioMin();		//极小
	

	//玩法操作

	////大小单双
	//afx_msg void OnBnClinckedBtnDa();
	//afx_msg void OnBnClinckedBtnXiao();
	//afx_msg void OnBnClinckedBtnDan();
	//afx_msg void OnBnClinckedBtnShuang();
	//afx_msg void OnBnClinckedBtnXiaoDan();	//小单
	//afx_msg void OnBnClinckedBtnXiaoShuang();	//小双
	//afx_msg void OnBnClinckedBtnDaDan();	//大单
	//afx_msg void OnBnClinckedBtnDaShuang();	//大双

	//afx_msg void OnBnClinckedBtnMax();		//极大
	//afx_msg void OnBnClinckedBtnMin();		//极小





	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	CImgRioButton m_rioMouseInput;		//鼠标输入
	CImgRioButton m_rioKeyboardInput;	//键盘输入

	afx_msg void OnBnClickedBtnChqAdd();//添加号码
	afx_msg void OnBnClickedBtnZhuihao();//追号
	afx_msg void OnBnClickedBtnTouzhu();//投注
	afx_msg void OnBnClickedBtnLock();//返点修改
	afx_msg void OnBnClickedBtnDelSel();//删除
	afx_msg void OnBnClickedBeiTouJia();
	afx_msg void OnBnClickedBeiTouJian();
	CImageButton m_btnBeiTouJia;
	CImageButton m_btnBeiTouJian;

	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );

public:
	afx_msg void OnBnClickedBtnClsList();//清空

	afx_msg void OnEnChangeEditDanshiInput();//注数修改。
	afx_msg void OnEnChangeEditBeishu();//倍数 修改


	DECLARE_MESSAGE_MAP()

	TCHAR				m_szMD5Haoma[33];

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	CString GetSZXDesc(int num);
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

	//CString GetDxdshStringDesc();
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	CString GetDxdshStringDesc(int nNum);
	CString GetJHODesc(int nNum);
	CString GetHZDXDSDesc(int nNum);




	//界面控件
private:
	Bitmap* m_bmpBk;
	Bitmap* m_numImageList;
	Bitmap* m_numImageList1;
	Bitmap* m_numImageList2;
	Bitmap* m_numImageList3;
	Bitmap* m_numImageList4;
	Bitmap* m_kjNumBig;
	Bitmap* m_kjNumBig2;
	Bitmap* m_kjAdd;

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

	BeiJingKuai8Kind m_gameKind;




	//幸运28数字
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

	CNumerButton m_btnShang;
	CNumerButton m_btnZhong;
	CNumerButton m_btnXia;

	CNumerButton m_btnJi;
	CNumerButton m_btnHe;
	CNumerButton m_btnOu;

	CNumerButton m_btnDaDan1;
	CNumerButton m_btnDaShuang1;
	CNumerButton m_btnXiaoDan1;
	CNumerButton m_btnXiaoShuang1;
	//北京快乐8
	CNumerButton	m_btnHuanLe_01;
	CNumerButton	m_btnHuanLe_02;
	CNumerButton	m_btnHuanLe_03;
	CNumerButton	m_btnHuanLe_04;
	CNumerButton	m_btnHuanLe_05;
	CNumerButton	m_btnHuanLe_06;
	CNumerButton	m_btnHuanLe_07;
	CNumerButton	m_btnHuanLe_08;
	CNumerButton	m_btnHuanLe_09;
	CNumerButton	m_btnHuanLe_10;
	CNumerButton	m_btnHuanLe_11;
	CNumerButton	m_btnHuanLe_12;
	CNumerButton	m_btnHuanLe_13;
	CNumerButton	m_btnHuanLe_14;
	CNumerButton	m_btnHuanLe_15;
	CNumerButton	m_btnHuanLe_16;
	CNumerButton	m_btnHuanLe_17;
	CNumerButton	m_btnHuanLe_18;
	CNumerButton	m_btnHuanLe_19;
	CNumerButton	m_btnHuanLe_20;
	CNumerButton	m_btnHuanLe_21;
	CNumerButton	m_btnHuanLe_22;
	CNumerButton	m_btnHuanLe_23;
	CNumerButton	m_btnHuanLe_24;
	CNumerButton	m_btnHuanLe_25;
	CNumerButton	m_btnHuanLe_26;
	CNumerButton	m_btnHuanLe_27;
	CNumerButton	m_btnHuanLe_28;
	CNumerButton	m_btnHuanLe_29;
	CNumerButton	m_btnHuanLe_30;
	CNumerButton	m_btnHuanLe_31;
	CNumerButton	m_btnHuanLe_32;
	CNumerButton	m_btnHuanLe_33;
	CNumerButton	m_btnHuanLe_34;
	CNumerButton	m_btnHuanLe_35;
	CNumerButton	m_btnHuanLe_36;
	CNumerButton	m_btnHuanLe_37;
	CNumerButton	m_btnHuanLe_38;
	CNumerButton	m_btnHuanLe_39;
	CNumerButton	m_btnHuanLe_40;
	CNumerButton	m_btnHuanLe_41;
	CNumerButton	m_btnHuanLe_42;
	CNumerButton	m_btnHuanLe_43;
	CNumerButton	m_btnHuanLe_44;
	CNumerButton	m_btnHuanLe_45;
	CNumerButton	m_btnHuanLe_46;
	CNumerButton	m_btnHuanLe_47;
	CNumerButton	m_btnHuanLe_48;
	CNumerButton	m_btnHuanLe_49;
	CNumerButton	m_btnHuanLe_50;
	CNumerButton	m_btnHuanLe_51;
	CNumerButton	m_btnHuanLe_52;
	CNumerButton	m_btnHuanLe_53;
	CNumerButton	m_btnHuanLe_54;
	CNumerButton	m_btnHuanLe_55;
	CNumerButton	m_btnHuanLe_56;
	CNumerButton	m_btnHuanLe_57;
	CNumerButton	m_btnHuanLe_58;
	CNumerButton	m_btnHuanLe_59;
	CNumerButton	m_btnHuanLe_60;
	CNumerButton	m_btnHuanLe_61;
	CNumerButton	m_btnHuanLe_62;
	CNumerButton	m_btnHuanLe_63;
	CNumerButton	m_btnHuanLe_64;
	CNumerButton	m_btnHuanLe_65;
	CNumerButton	m_btnHuanLe_66;
	CNumerButton	m_btnHuanLe_67;
	CNumerButton	m_btnHuanLe_68;
	CNumerButton	m_btnHuanLe_69;
	CNumerButton	m_btnHuanLe_70;
	CNumerButton	m_btnHuanLe_71;
	CNumerButton	m_btnHuanLe_72;
	CNumerButton	m_btnHuanLe_73;
	CNumerButton	m_btnHuanLe_74;
	CNumerButton	m_btnHuanLe_75;
	CNumerButton	m_btnHuanLe_76;
	CNumerButton	m_btnHuanLe_77;
	CNumerButton	m_btnHuanLe_78;
	CNumerButton	m_btnHuanLe_79;
	CNumerButton	m_btnHuanLe_80;


	CTextButton m_btnRenXuan;//任选1

	CTextButton m_btnTeMa;					//特码
	CTextButton m_btnDaXiaoDanShuang;		//大小单双
	CTextButton m_btnJiZhi;					//极值
	CTextButton m_btnFunny;					//趣味
	CTextButton m_btnRenXuan6;//任选6
	CTextButton m_btnRenXuan7;//任选7
	CTextButton m_btnRenXuan8;//任选8
	CTextButton m_btnRenXuan9;//任选9
	CTextButton m_btnRenXuan10;//任选10
	CTextButton m_btnQuWei;//任选10

	CImgRioButton m_rioDuiZi;					//对子
	CImgRioButton m_rioShunZi;					//顺子
	CImgRioButton m_rioBaoZi;					//豹子

	CNumerButton m_btnMax;					//极大
	CNumerButton m_btnMin;					//极小
	
	CImgRioButton m_rioSZX;					//上中下
	CImgRioButton m_rioJHO;					//奇偶和
	CImgRioButton m_rioHZDXDS;					//和值大小单双



	//大小单双
	CNumerButton m_btnDa;
	CNumerButton m_btnXiao;
	CNumerButton m_btnDan;
	CNumerButton m_btnShuang;
	CNumerButton m_btnXiaoDan;				//小单
	CNumerButton m_btnXiaoShuang;			//小双
	CNumerButton m_btnDaDan;					//大单
	CNumerButton m_btnDaShuang;				//大双




	CImageButton m_btnDelSel;	//删除
	CImageButton m_btnClsList;	//清空
	CImageButton m_btnTouZhu;	//投注
	CImageButton m_btnZhuiHao;	//追号
	CImageButton m_btnChqAdd;		//添加按钮
	CTextButton m_btnLock;			//返点按钮



	CSkinEditEx m_editBeiTou;		//倍数输入
	CEdit m_editDanShiHaoMa;

	CRichEditCtrl m_richDanshiHaoma;			//单式号码

	bool			m_RenxuanKeyboardInput;		//任选N的键盘输入



	CListCtrlCl m_listChqSSC;		//下注列表
	CGridCtrl	m_GridList;			//

	int				m_nZongZhuShu;				//单倍总注数
	UINT			m_beishu;					//倍数
	int				m_nFrameIndex;				//GIF动画的当前帧数
	int				m_nFrameCount;				//GIF动画总共帧数


	CString m_StrFunny;
	static CString m_strHaoma;					//号码

	//最后几次开奖信息
	TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	int	m_kjXinxiCont;
	KjXinXi m_kjXinxi[Kj_XinXi_Count];

	//绘制小的开奖号时使用的字体
	CFont m_smallNumfont;
	//总金额总注数使用的字体
	CFont m_zongFont;
	//开奖信息使用字体
	CFont m_KjInfoFont;
	CFont m_KjInfoFont1;
	afx_msg void OnBnClickedRioMouseInput();
	afx_msg void OnBnClickedRioKeyboardInput();

	
	CPlazaViewItem* m_pLuckMeDlg;		//
	CKuaiLe8RUle m_chqRule;				//时间控件

	void ShowShangZhongXia();
	void HideShangZhongXia();
	void ShowJiHeOu();
	void HideJiHeOu();
	void ShowDaXiaoDanShuang1();
	void HideDaXiaoDanShuang1();

	//界面控制
public:
	void AdjustRadio();
	void AdjustWanFa();
	void AdjustNumView();
	void AdjustNumBtn();
	void AdjustDanShi();
	void AdjustAdd();

	void CheckWanFaBtn(CTextButton& btn);

	void HideAllRio();
	void ShowCtrl(int ctrl_id);
	void HideCtrl(int ctrl_id);

	//显示隐藏1-80
	void ShowBJNums();
	void HideBJNums();

	//显示隐藏1-27
	void ShowHezhiNums();
	void HideHeZhiNums();

	//重置所有状态
	void ResetAllNums(void);

	//选择数字
	void CheckNumBtn(CNumerButton& btn);
	//取消选择数字
	void UnCheckNumBtn(CNumerButton& btn);
	//该数字是否被选中
	bool IsNumBtnCheck(CNumerButton& btn);

	//显示键盘输入
	void ShowDanShiEdit();
	//隐藏键盘输入
	void HideDanShiEdit();

	//显示或隐藏大小单双
	void ShowDaXiaoDanShuang();
	void HideDaXiaoDanShuang();

	//隐藏玩法
	void HideRio();
	//选择玩法
	void SelectRio(CImgRioButton  &rio);
	//显示隐藏极值
	void HideJiZhi();
	void ShowJiZji();

	void OnBnClickedBtnSZX();
	void OnBnClickedBtnJHO();
	void OnBnClickedBtnHZDXDS();

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

	//辅助信息获取
private:
	long GetKjShjDiff();//获取开奖时间
	void InitNumberBtns();//初始化数字按钮
	void InitListCtrl();//初始化列表控件

	//绘图辅助
	void DrawLastKjHaoma(CDC* pDC, Graphics& graphics);//最后开奖号码
	void DrawTopKjHaoma(CDC* pDC, Graphics& graphics);//最后几次的开奖号码
	void DrawZongZhushuJinE(CDC* pDC);					//总注数
	void DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num,int nWidth,int nHeight);
	void DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);


	void SetLockText(float fBonus=0.0f, float fBonusPercent=0.0f);
	VOID SendToServer(int nSendType);

	CString GetBjNum();//获取北京快8的按钮数值
	CString GetHezhiString();//获取幸运28数值
	CString GetWanDesc();//获取玩法名称
	CString GetDxdshString();//获取大小单双
	CString GetJiZhiString();//获取极值

	CString GetShangZhongXiaString();
	CString GetJiHeOuString();
	CString GetDaXiaoDanShuang1String();


	//获取单式号码
	//bool GetDanShiHaoma(CStringArray& strArr, int size);
	bool GetDanShiHaoma(CString& haoma, int size, int nLeast);
	bool GetHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast);


	//初始化玩法信息
	void InitWanFaMoShi();
public:
	VOID SendQuerySystemTime();
	//服务器端返回前N个开奖号码
	bool GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nDataSize);


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
