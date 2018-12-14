#pragma once

//游戏控制基类
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

public:
	//更新控制
	virtual void __cdecl OnAllowControl(bool bEnable) = NULL;
	//申请结果
	virtual bool __cdecl ReqResult(const void * pBuffer) = NULL;
	virtual void __cdecl ResetUserBet() = NULL;

	virtual void __cdecl SetUserBetScore(BYTE cbArea,LONGLONG lScore) = NULL;
	virtual void __cdecl SetUserGameScore(BYTE cbArea,LONGLONG lScore) = NULL;
	virtual void __cdecl SetUserNickName(CString strNickName) = NULL;
	virtual void __cdecl ResetUserNickName() = NULL;
};



//
//事件：游戏代码6602转6603，使原来6602的游戏可以在6603上使用
//作者：王少江
//时间：2012.03.04 
//Q Q ：349126394
//QQ群：101494119
//邮箱：shaojiang216@163.com
//博客：http://shaojiang216.blog.163.com/ 