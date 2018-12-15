#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0501 //为 Windows Me 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0501	//为 IE 5.0 及更新版本改变为适当的值。
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>
#include <AtlBase.h>
#include <AtlDbcli.h>
#include <AfxDtctl.h>

//////////////////////////////////////////////////////////////////////////////////


//系统环境

#include "..\..\..\客户端组件\游戏框架\GameFrameHead.h"

#ifndef _DEBUG
#pragma comment (lib,"../../../链接库/Unicode/WHImage.lib")
#pragma comment (lib,"../../../链接库/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../链接库/Unicode/GameEngine.lib")
#pragma comment (lib,"../../../链接库/Unicode/SkinControl.lib")
#pragma comment (lib,"../../../链接库/Unicode/ServiceCore.lib")
#else
#pragma comment (lib,"../../../链接库/Unicode/WHImageD.lib")
#pragma comment (lib,"../../../链接库/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../链接库/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../../链接库/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../../链接库/Unicode/ServiceCoreD.lib")
#endif


#define  __TEST__ 
#define  __BANKER___
#define  __SPECIAL___
//////////////////////////////////////////////////////////////////////////
#include "..\消息定义\CMD_OxBattle.h"


//////////////////////////////////////////////////////////////////////////
//
