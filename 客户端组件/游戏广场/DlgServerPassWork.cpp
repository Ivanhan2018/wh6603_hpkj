#include "stdafx.h"
#include "DlgServerPassWork.h"
//////////////////////////
////EWIN网络编写
////2013.4.16
////功能：验证房间密码
////出售转载请注明出处
///////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//屏幕位置
#define LAYERED_SIZE				5									//分层大小
#define CAPTION_SIZE				60									//标题大小

//////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgServerPassWork, CDialog)

	//系统消息
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()

	//按钮消息
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

CDlgServerPassWork::CDlgServerPassWork(CWnd* pParent /*=NULL*/): CDialog(CDlgServerPassWork::IDD, pParent)
{
	m_StrPassWork=TEXT("");
}

CDlgServerPassWork::~CDlgServerPassWork()
{
	m_StrPassWork=TEXT("");
}

//配置函数
BOOL CDlgServerPassWork::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("请输入房间密码"));

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_SERVERPASSWORK_BACK"));
	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);


	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btOk.SetButtonImage(IDB_BT_SERVER_OK,hInstance,false,false);
	m_btCancel.SetButtonImage(IDB_BT_SERVER_CANCEL,hInstance,false,false);

	//设置输入文本框
	m_ServerPassWork.SetEnableColor(RGB(255,255,0),RGB(11,60,96),RGB(11,60,96));


	//居中窗口
	CenterWindow(this);

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return FALSE;
}

void CDlgServerPassWork::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//按钮控件
	DDX_Control(pDX, IDC_OK, m_btOk);
	DDX_Control(pDX, IDC_CANCEL, m_btCancel);
	//房间密码控件
	DDX_Control(pDX, IDC_SERVER_PASSWORK, m_ServerPassWork);

}


//绘画背景
BOOL CDlgServerPassWork::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建 DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置缓冲
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_SERVERPASSWORK_BACK"));
	//绘画背景
	ImageBack.DrawImage(pBufferDC,0,0);

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//显示消息
VOID CDlgServerPassWork::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//显示分层
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//鼠标消息
VOID CDlgServerPassWork::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟标题
	if (Point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//位置改变
VOID CDlgServerPassWork::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//移动分层
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}


//设置房间密码
void CDlgServerPassWork::SetPassWork(CString StrPassWork)
{
	m_StrPassWork=StrPassWork;
	//AfxMessageBox(m_StrPassWork);
	return;
}

//确定密码
void CDlgServerPassWork::OnBnClickedOk()
{

	//获取密码
	CString strPassWork;
	GetDlgItemText(IDC_SERVER_PASSWORK,strPassWork);

	

	//输入密码与房间密码完全相同
	if(strPassWork==m_StrPassWork){
		
		EndDialog(99);
	
	}else{
	
		//提示输入密码错误
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您输入的房间密码不正确，请重新输入！"),MB_ICONERROR,0);
		m_ServerPassWork.SetFocus();
		//密码错误
		//EndDialog(0);
	
	}
	return;
}

//取消按扭
void CDlgServerPassWork::OnBnClickedCancel()
{
	//关闭窗口
	EndDialog(0);
	return;
}
//////////////////////////////////////////////////////////////////////////////////