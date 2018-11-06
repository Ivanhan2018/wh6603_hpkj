#pragma once

//////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT WINVER
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS WINVER
#endif

#ifndef _WIN32_IE
#define _WIN32_IE WINVER
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#include <AfxWin.h>
#include <AfxExt.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <AfxOLE.h>
#include <AfxDisp.h>
#include <AfxoDlgs.h>
#endif

#include <AfxDtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <AfxCmn.h>
#endif

//////////////////////////////////////////////////////////////////////////////////

