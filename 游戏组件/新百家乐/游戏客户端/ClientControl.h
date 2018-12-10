#pragma once

#define IDM_ADMIN_COMMDN WM_USER+1000

//游戏控制基类
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

	virtual bool  ReqResult(const void * pBuffer)=NULL;
	virtual void  ResetUserBet() = NULL;
	virtual void  SetUserBetScore(BYTE cbArea,LONGLONG lScore) = NULL;
};
