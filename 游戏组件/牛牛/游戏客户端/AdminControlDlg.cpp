// AdminControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "gameclient.h"
#include "AdminControlDlg.h"


// CAdminControlDlg 对话框

IMPLEMENT_DYNAMIC(CAdminControlDlg, CDialog)
CAdminControlDlg::CAdminControlDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAdminControlDlg::IDD, pParent)
{
	m_dwGameID=0;
}

CAdminControlDlg::~CAdminControlDlg()
{
}

void CAdminControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CAdminControlDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnExcute)
	ON_BN_CLICKED(IDC_BUTTON_RESET,OnReSet)
	ON_BN_CLICKED(IDC_BUTTON_SYN,OnRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL,OnCancel)
	ON_BN_CLICKED(IDC_QUERY_USER,OnQueryUser)
	ON_WM_TIMER()	
END_MESSAGE_MAP()


// CAdminControlDlg 消息处理程序
BOOL CAdminControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();
	SetDlgItemText(IDC_STATIC_TIMES,TEXT("控制次数:"));
	SetDlgItemText(IDC_STATIC_ID,TEXT("玩家UserID:"));
	SetDlgItemText(IDC_RADIO_WIN,TEXT("赢"));
	SetDlgItemText(IDC_RADIO_LOST,TEXT("输"));

	((CButton*)(GetDlgItem(IDC_RADIO_WIN)))->SetCheck(1);
	((CButton*)(GetDlgItem(IDC_RADIO_LOST)))->SetCheck(0);


	//控制次数
	for(int i=0;i<100;i++)
	{

		CString NumInt;
		NumInt.Format(TEXT("%d"),i);
		((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->InsertString(i,NumInt);
	
	}


	SetDlgItemText(IDC_BUTTON_RESET,TEXT("取消控制"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("查询"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("执行"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("取消"));
	SetDlgItemText(IDC_QUERY_USER,TEXT("获取"));

	SetDlgItemText(IDC_STATIC,TEXT("注:如果作弊玩家本局没有下注或本局下注没输也没赢,那么该局将不会记录在本周期内。"));


	return TRUE;
}

void CAdminControlDlg::OnReSet()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_OPTION_CANCLE;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}

void CAdminControlDlg::OnRefresh()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_OPTION_QUERYING;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}

void CAdminControlDlg::OnExcute()
{
	CMD_C_AdminReq adminReq={0};


	int nSelect=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();


	//梦成网络修正
	CString strCount;
	GetDlgItem(IDC_COMBO_GAMEID)->GetWindowText(strCount);
	_sntscanf(strCount.GetBuffer(),strCount.GetLength(),_TEXT("%d"),&m_dwGameID);
	
	if(strCount.IsEmpty()){


	int nSelect1=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->GetLBText(nSelect1,strCount);

	}


	//用户名转用户ID

	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(wcscmp(strCount,m_szUserAccounts[i])==0)
		{
			m_dwGameID = m_wUserGameID[i];
		}
	}

	////////////////////

	if(nSelect>=0&&nSelect<=100 && m_dwGameID!=0)
	{
		adminReq.cbReqType=RQ_OPTION_SETING;
		adminReq.dwGameID=m_dwGameID;
		adminReq.cbCheatCount=nSelect;

		if(((CButton*)(GetDlgItem(IDC_RADIO_WIN)))->GetCheck()!=0)
		{
			adminReq.cbCheatType=CHEAT_TYPE_WIN;
		}
		else
		{
			adminReq.cbCheatType=CHEAT_TYPE_LOST;
		}


		CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
	}
	else
	{
		AfxMessageBox(TEXT("请输入正常的参数!"));
	}
	// TODO: 在此添加控件通知处理程序代码
}

void CAdminControlDlg::OnQueryUser()
{
	// TODO: 在此添加专用代码和/或调用基类
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_QUERYUSER,0,0);
}


void CAdminControlDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	ShowWindow(SW_HIDE);
	//	CDialog::OnCancel();
}

void CAdminControlDlg::OnTimer(UINT nIDEvent)
{

}



bool CAdminControlDlg::OnSubAdminCommand(const void * pBuffer, WORD wDataSize)
{

	////效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_AdminStore));
	//if (wDataSize!=sizeof(CMD_S_AdminStore)) return false;

	////消息处理
	//CMD_S_AdminStore * pAdminStore=(CMD_S_AdminStore *)pBuffer;

	//switch(pAdminStore->cbReqType)
	//{
	//case RQ_SERCHSTORE:
	//	{
	//		CString strStore;
	//		strStore.Format(TEXT("当前库存量：%d"),pAdminStore->lStorageStart);

	//		SetDlgItemText(IDC_STATIC_STORE,strStore);
	//		break;
	//	}
	//case RQ_REVIEWSTORE:
	//	{
	//		CString strStore;
	//		strStore.Format(TEXT("当前库存量：%d"),pAdminStore->lStorageStart);

	//		SetDlgItemText(IDC_STATIC_STORE,strStore);
	//		AfxMessageBox(TEXT("修改成功"));
	//		break;
	//	}
	//}
	return true;
}
void CAdminControlDlg::SetUserInfo(TCHAR m_szAccounts[GAME_PLAYER][LEN_ACCOUNTS],DWORD m_wGameID[GAME_PLAYER])
{
	memset(m_wUserGameID,0,sizeof(m_wUserGameID));
	ZeroMemory(m_szUserAccounts,sizeof(m_szUserAccounts));

	((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->ResetContent();

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		lstrcpyn(m_szUserAccounts[i],m_szAccounts[i],CountArray(m_szAccounts[i]));
		m_wUserGameID[i] = m_wGameID[i];
	}

	int num = 0;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(m_wUserGameID[i]!=0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_GAMEID))->InsertString(num,m_szUserAccounts[i]);
			num++;
		}
	}
}