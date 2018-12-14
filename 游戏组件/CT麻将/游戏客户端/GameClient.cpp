#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//应用程序对象
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientApp::CGameClientApp()
{
}

//析构函数
CGameClientApp::~CGameClientApp() 
{
}

CGameFrameEngine * CGameClientApp::GetGameFrameEngine(DWORD dwSDKVersion) 
{ 
	return new CGameClientEngine; 
}
//////////////////////////////////////////////////////////////////////////
