#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//定义文件
#include "Macro.h"
#include "Define.h"

//结构文件
#include "Struct.h"
#include "Packet.h"

//模板文件
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//////////////////////////////////////////////////////////////////////////////////

//程序版本
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//框架版本
#define VERSION_PLAZA				PROCESS_VERSION(29,0,16)				//大厅版本
#define VERSION_MOBILE				PROCESS_VERSION(6,0,3)				//手机版本

//开发库版本
#define VERSION_EFFICACY			0									//效验版本
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)				//框架版本

//////////////////////////////////////////////////////////////////////////////////
//发布版本

#ifndef _DEBUG
//平台常量
const TCHAR szProduct[]=TEXT("九亿娱乐");							//产品名字
const TCHAR szPlazaClass[]=TEXT("AQYGamePlaza");						//广场类名
const TCHAR szProductKey[]=TEXT("AQYGamePlatform");						//产品主键

//地址定义
const TCHAR szCookieUrl[]=TEXT("http://reg.hhsmtw.com");					//记录地址
//const TCHAR szLogonServer[]=TEXT("127.0.0.1");						//登录地址
//const TCHAR szLogonServer[]=TEXT("210.56.60.26");						//登录地址
const TCHAR szPlatformLink[]=TEXT("http://114.55.52.46:9999/");					//平台网站
const TCHAR szPlatformPublicize[]=TEXT("http://www.baidu.com/ads/GameIndex.aspx");		//公告网站
const TCHAR szPlatformTopPublicize[]=TEXT("http://www.baidu.com/ads/GameTopNotice.aspx");	//顶部公告
const TCHAR szPlatformLeftPublicize[]=TEXT("http://www.baidu.com/ads/PlazaIndex.html");	//左下角广告
const TCHAR szValidateLink[]=TEXT("UserService/ClientHandler.ashx?action=clientlogon&UID=%d&PWD=%s&URL=/"); //验证地址 


#else

//////////////////////////////////////////////////////////////////////////////////
//内测版本
//平台常量
const TCHAR szProduct[]=TEXT("三和棋牌");							//产品名字
const TCHAR szPlazaClass[]=TEXT("3HQPGamePlaza");						//广场类名
const TCHAR szProductKey[]=TEXT("3HQPGamePlatform");						//产品主键

//地址定义
const TCHAR szCookieUrl[]=TEXT("http://www.3hqipai.com");					//记录地址
const TCHAR szLogonServer[]=TEXT("www.3hqipai.com");						//登录地址
const TCHAR szPlatformLink[]=TEXT("http://www.3hqipai.com/");					//平台网站
const TCHAR szPlatformPublicize[]=TEXT("http://www.3hqipai.com/ads/GameIndex.aspx");		//公告网站
const TCHAR szPlatformTopPublicize[]=TEXT("http://www.3hqipai.com/ads/GameTopNotice.aspx");	//顶部公告
const TCHAR szPlatformLeftPublicize[]=TEXT("http://www.3hqipai.com/ads/PlazaIndex.html");	//左下角广告
const TCHAR szValidateLink[]=TEXT("UserService/ClientHandler.ashx?action=clientlogon&UID=%d&PWD=%s&URL=/"); //验证地址 

#endif
//////////////////////////////////////////////////////////////////////////////////

//数据库名
const TCHAR 		szPlatformDB[]=TEXT("QPPlatformDB");						//平台数据库
const TCHAR 		szAccountsDB[]=TEXT("QPAccountsDB");						//用户数据库
const TCHAR		szTreasureDB[]=TEXT("QPTreasureDB");						//财富数据库
const TCHAR		szEducateDB[]=TEXT("QPEducateDB");						//练习数据库 


//////////////////////////////////////////////////////////////////////////////////

//授权信息
const TCHAR szCompilation[]=TEXT("345E2FFA-891E-4021-A57E-80ECF3466896");


//////////////////////////////////////////////////////////////////////////////////

#endif