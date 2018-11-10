#pragma once

//////////////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0400
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0400
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////
#include "targetver.h"

// add by hxh 20160801
#define POINTER_64 __ptr64

typedef void *PVOID;
typedef void * POINTER_64 PVOID64;

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//MFC 文件
#include <Nb30.h>
#include <AfxInet.h>

#include <string>
using namespace std;


//////////////////////////////////////////////////////////////////////////////////
//平台文件
enum	enMoShi{
	MoShi_Yuan,
	MoShi_Jiao,
	MoShi_Fen,
	MoShi_Li
};


//平台文件
#include "..\..\全局定义\Platform.h"

//网络命令
#include "..\..\消息定义\CMD_Commom.h"
#include "..\..\消息定义\CMD_GameServer.h"
#include "..\..\消息定义\CMD_LogonServer.h"

//进程命令
#include "..\..\消息定义\IPC_GameFrame.h"

//组件文件
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"
#include "..\..\公共组件\进程控制\ProcessControlHead.h"

//组件文件
#include "..\..\客户端组件\图像控件\WHImageHead.h"
#include "..\..\客户端组件\下载组件\DownLoadHead.h"
#include "..\..\客户端组件\界面控件\SkinControlHead.h"
#include "..\..\客户端组件\用户服务\UserServiceHead.h"
#include "..\..\客户端组件\平台数据\PlatformDataHead.h"
#include "..\..\客户端组件\游戏控件\ShareControlHead.h"
#include "..\..\客户端组件\形象组件\AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//链接代码

//多媒体库
#pragma comment (lib,"Winmm.lib")

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../链接库/Ansi/WHImage.lib")
	#pragma comment (lib,"../../链接库/Ansi/DownLoad.lib")
	#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../链接库/Ansi/SkinControl.lib")
	#pragma comment (lib,"../../链接库/Ansi/ShareControl.lib")
	#pragma comment (lib,"../../链接库/Ansi/PlatformData.lib")
	#pragma comment (lib,"../../链接库/Ansi/AvatarControl.lib")
#else
	#pragma comment (lib,"../../链接库/Unicode/WHImage.lib")
	#pragma comment (lib,"../../链接库/Unicode/DownLoad.lib")
	#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../链接库/Unicode/SkinControl.lib")
	#pragma comment (lib,"../../链接库/Unicode/ShareControl.lib")
	#pragma comment (lib,"../../链接库/Unicode/PlatformData.lib")
	#pragma comment (lib,"../../链接库/Unicode/AvatarControl.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../链接库/Ansi/WHImageD.lib")
	#pragma comment (lib,"../../链接库/Ansi/DownLoadD.lib")
	#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../链接库/Ansi/SkinControlD.lib")
	#pragma comment (lib,"../../链接库/Ansi/ShareControlD.lib")
	#pragma comment (lib,"../../链接库/Ansi/PlatformDataD.lib")
	#pragma comment (lib,"../../链接库/Ansi/AvatarControlD.lib")
#else
	#pragma comment (lib,"../../链接库/Unicode/WHImageD.lib")
	#pragma comment (lib,"../../链接库/Unicode/DownLoadD.lib")
	#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../链接库/Unicode/SkinControlD.lib")
	#pragma comment (lib,"../../链接库/Unicode/ShareControlD.lib")
	#pragma comment (lib,"../../链接库/Unicode/PlatformDataD.lib")
	#pragma comment (lib,"../../链接库/Unicode/AvatarControlD.lib")
#endif
#endif

#include "xml/Markup.h"
#include "BmpUtil.h"
#include <afxhtml.h>
#include "ListCtrlCl.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
//////////////////////////////////////////////////////////////////////////////////
