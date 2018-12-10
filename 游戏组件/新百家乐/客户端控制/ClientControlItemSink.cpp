// ClientControlItemSink.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "ClientControlItemSink.h"
#include ".\clientcontrolitemsink.h"

//绘画数字
CString NumberToStringWithSpace(LONGLONG lNumber)
{
	CString strNumber, strTempNum;
	strTempNum.Format(_T("%I64d"), (lNumber>0?lNumber:-lNumber));

	int nLength = strTempNum.GetLength();
	if (nLength <= 3)
		strNumber.Format(_T("%I64d"), lNumber);
	else
	{
		for (int i = 0; i < int((nLength-1)/3); i++)
			strTempNum.Insert(nLength - 3*(i+1), _T(","));
		strNumber.Format(_T("%s%s"), (lNumber<0?_T("-"):_T("")), strTempNum);
	}

	return strNumber;
}

// CClientControlItemSinkDlg 对话框

IMPLEMENT_DYNAMIC(CClientControlItemSinkDlg, IClientControlDlg)

CClientControlItemSinkDlg::CClientControlItemSinkDlg(CWnd* pParent /*=NULL*/)
	: IClientControlDlg(CClientControlItemSinkDlg::IDD, pParent)
{
	m_cbWinArea=0;				//赢牌区域
	m_cbExcuteTimes=0;			//执行次数
}

CClientControlItemSinkDlg::~CClientControlItemSinkDlg()
{
}

void CClientControlItemSinkDlg::DoDataExchange(CDataExchange* pDX)
{
	IClientControlDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientControlItemSinkDlg, IClientControlDlg)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnExcute)
	ON_BN_CLICKED(IDC_BUTTON_RESET,OnReSet)
	ON_BN_CLICKED(IDC_BUTTON_SYN,OnRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL,OnCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CClientControlItemSinkDlg 消息处理程序

//初始化
BOOL CClientControlItemSinkDlg::OnInitDialog()
{
	IClientControlDlg::OnInitDialog();

	SetDlgItemText(IDC_STATIC_TIMES,TEXT("控制局数:"));
	SetDlgItemText(IDC_STATIC_AREA, TEXT("区域控制:"));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("控制说明:"));
	SetDlgItemText(IDC_STATIC_TEXT, TEXT("区域输赢控制比游戏库存控制策略优先!"));
	SetDlgItemText(IDC_RADIO_LT,TEXT("闲对子"));
	SetDlgItemText(IDC_RADIO_LC,TEXT("  闲  "));		
	SetDlgItemText(IDC_RADIO_LB,TEXT("闲天王"));
	SetDlgItemText(IDC_RADIO_CC,TEXT("  平  "));
	SetDlgItemText(IDC_RADIO_CB,TEXT("同点平"));
	SetDlgItemText(IDC_RADIO_RT,TEXT("庄对子"));
	SetDlgItemText(IDC_RADIO_RC,TEXT("  庄  "));
	SetDlgItemText(IDC_RADIO_RB,TEXT("庄天王"));

	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));


	SetDlgItemText(IDC_BUTTON_RESET,TEXT("取消控制"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("本局"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("执行"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("取消"));

	return TRUE; 
}

void CClientControlItemSinkDlg::OnReSet()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_RESET_CONTROL;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}

void CClientControlItemSinkDlg::OnRefresh()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_PRINT_SYN;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}


void CClientControlItemSinkDlg::OnExcute()
{
	CMD_C_AdminReq adminReq;
	BYTE cbIndex=0;
	int nCheckID;
	for(cbIndex=0;cbIndex<8;cbIndex++)
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_LT+cbIndex))->GetCheck())
		{
			nCheckID=cbIndex+1;
		}
	}

	int nSelect=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();

	if(nCheckID>0&&nSelect>=0)
	{
		adminReq.cbReqType=RQ_SET_WIN_AREA;
		adminReq.cbExtendData[0]=(BYTE)nCheckID;
		adminReq.cbExtendData[1]=(BYTE)nSelect+1;
		m_cbWinArea=adminReq.cbExtendData[0];
		m_cbExcuteTimes=adminReq.cbExtendData[1];

		CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
	}
	else
	{
		AfxMessageBox(TEXT("请选择受控次数以及受控区域!"));
	}
	// TODO: 在此添加控件通知处理程序代码
}


bool CClientControlItemSinkDlg::ReqResult(const void * pBuffer)
{

	const CMD_S_CommandResult*pResult=(CMD_S_CommandResult*)pBuffer;
	CString str;
	switch(pResult->cbAckType)
	{

	case ACK_SET_WIN_AREA:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				switch(m_cbWinArea)
				{
				case 0:str.Format(TEXT("无受控区域"));break;
				case 1:str.Format(TEXT("请求已接受,胜利区域:闲对子,执行次数:%d"),m_cbExcuteTimes);break;
				case 2:str.Format(TEXT("请求已接受,胜利区域:  闲  ,执行次数:%d"),m_cbExcuteTimes);break;
				case 3:str.Format(TEXT("请求已接受,胜利区域:闲天王,执行次数:%d"),m_cbExcuteTimes);break;
				case 4:str.Format(TEXT("请求已接受,胜利区域:  平  ,执行次数:%d"),m_cbExcuteTimes);break;
				case 5:str.Format(TEXT("请求已接受,胜利区域:同点平,执行次数:%d"),m_cbExcuteTimes);break;
				case 6:str.Format(TEXT("请求已接受,胜利区域:庄对子,执行次数:%d"),m_cbExcuteTimes);break;
				case 7:str.Format(TEXT("请求已接受,胜利区域:  庄  ,执行次数:%d"),m_cbExcuteTimes);break;
				case 8:str.Format(TEXT("请求已接受,胜利区域:庄天王,执行次数:%d"),m_cbExcuteTimes);break;
				default:break;
				}
			}
			else
			{
				str.Format(TEXT("请求失败!"));
				m_cbExcuteTimes=0;
				m_cbWinArea=0;
			}
			break;
		}
	case ACK_RESET_CONTROL:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str=TEXT("重置请求已接受!");
				for(int nIndex=0;nIndex<8;nIndex++)
					((CButton*)GetDlgItem(IDC_RADIO_LT+nIndex))->SetCheck(0);
				if(m_cbExcuteTimes>0)
					((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(-1);
				m_cbWinArea=0;
				m_cbExcuteTimes=0;
			}
			break;
		}
	case ACK_PRINT_SYN:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str=TEXT("服务器同步请求已接受!");
				for(int nIndex=0;nIndex<8;nIndex++)
					((CButton*)GetDlgItem(IDC_RADIO_LT+nIndex))->SetCheck(0);

				m_cbWinArea=pResult->cbExtendData[0];
				m_cbExcuteTimes=pResult->cbExtendData[1];
				if(m_cbWinArea>0&&m_cbWinArea<=8)
					((CButton*)GetDlgItem(IDC_RADIO_LT+m_cbWinArea-1))->SetCheck(1);
				if(m_cbExcuteTimes>0&&m_cbExcuteTimes<=5)
					((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(m_cbExcuteTimes-1);

				SetTimer(10,2000,0);
			}
			else
			{
				str.Format(TEXT("请求失败!"));
			}
			break;
		}

	default: break;
	}

	SetWindowText(str);
	return true;
}


void CClientControlItemSinkDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	ShowWindow(SW_HIDE);
	//	CDialog::OnCancel();
}

void CClientControlItemSinkDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(10);
	CMD_S_CommandResult Result;
	Result.cbAckType=ACK_SET_WIN_AREA;
	Result.cbResult=CR_ACCEPT;
	ReqResult(&Result);
	CDialog::OnTimer(nIDEvent);
}
////下注信息
void CClientControlItemSinkDlg::ResetUserBet()
{
	CString strPrint;
	memset(m_lAllUserBet,0,sizeof(LONGLONG)*8);
	for(int i=0;i<8;i++)
	{
		SetDlgItemText(IDC_STATIC1+i,_T("(0)"));
	}
/*	case 0: strPrint.Format(TEXT("闲家:%I64d"),lScore);break;
	case 1: strPrint.Format(TEXT("平家:%I64d"),lScore);break;
	case 2: strPrint.Format(TEXT("庄家:%I64d"),lScore);break;
	case 3: strPrint.Format(TEXT("闲天王:%I64d"),lScore);break;
	case 4: strPrint.Format(TEXT("庄天王:%I64d"),lScore);break;
	case 5: strPrint.Format(TEXT("同点平:%I64d"),lScore);break;
	case 6: strPrint.Format(TEXT("闲对子:%I64d"),lScore);break;
	case 7: strPrint.Format(TEXT("庄对子:%I64d"),lScore);break;
*/
}


////下注信息
void CClientControlItemSinkDlg::SetUserBetScore(BYTE cbArea,LONGLONG lScore)
{
	m_lAllUserBet[cbArea]+=lScore;
	CString strPrint;
	switch(cbArea)
	{
	/*case 0: strPrint.Format(TEXT("闲家%I64d"),m_lAllUserBet[cbArea]);break;
	case 1: strPrint.Format(TEXT("平家:%I64d"),m_lAllUserBet[cbArea]);break;
	case 2: strPrint.Format(TEXT("庄家:%I64d"),m_lAllUserBet[cbArea]);break;
	case 3: strPrint.Format(TEXT("闲天王:%I64d"),m_lAllUserBet[cbArea]);break;
	case 4: strPrint.Format(TEXT("庄天王:%I64d"),m_lAllUserBet[cbArea]);break;
	case 5: strPrint.Format(TEXT("同点平:%I64d"),m_lAllUserBet[cbArea]);break;
	case 6: strPrint.Format(TEXT("闲对子:%I64d"),m_lAllUserBet[cbArea]);break;
	case 7: strPrint.Format(TEXT("庄对子:%I64d"),m_lAllUserBet[cbArea]);break;*/

	case 0: strPrint.Format(TEXT("(%I64d)"),m_lAllUserBet[cbArea]);break;
	case 1: strPrint.Format(TEXT("(%I64d)"),m_lAllUserBet[cbArea]);break;
	case 2: strPrint.Format(TEXT("(%I64d)"),m_lAllUserBet[cbArea]);break;
	case 3: strPrint.Format(TEXT("(%I64d)"),m_lAllUserBet[cbArea]);break;
	case 4: strPrint.Format(TEXT("(%I64d)"),m_lAllUserBet[cbArea]);break;
	case 5: strPrint.Format(TEXT("(%I64d)"),m_lAllUserBet[cbArea]);break;
	case 6: strPrint.Format(TEXT("(%I64d)"),m_lAllUserBet[cbArea]);break;
	case 7: strPrint.Format(TEXT("(%I64d)"),m_lAllUserBet[cbArea]);break;


	default:false;
	}
	SetDlgItemText(IDC_STATIC1+cbArea,strPrint);
}


