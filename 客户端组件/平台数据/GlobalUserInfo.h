#ifndef GLOBAL_USER_INFO_HEAD_FILE
#define GLOBAL_USER_INFO_HEAD_FILE

#pragma once

#include "PlatformDataHead.h"

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//用户信息
struct tagGlobalUserData
{
	//基本资料
	DWORD							dwUserID;							//用户 I D
	DWORD							dwExperience;						//用户经验
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码

	//用户资料
	WORD							wFaceID;							//头像索引
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器

	//用户成绩
	SCORE							lScore;								//用户积分
	SCORE							lInsure;							//用户银行
	SCORE                           lUserMedal;                         //用户奖牌

	//会员资料
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间

	//扩展信息
	TCHAR                           szAddrDescribe[LEN_ADDR_DESCRIBE];  //地址描述   
};

//扩展资料
struct tagIndividualUserData
{
	//用户信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//用户说明
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	//电话号码
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

	//联系资料
	TCHAR							szQQ[LEN_QQ];						//Q Q 号码
	TCHAR							szEMail[LEN_EMAIL];					//电子邮件
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
};

//////////////////////////////////////////////////////////////////////////////////

//用户信息
class PLATFORM_DATA_CLASS CGlobalUserInfo
{
	//用户信息
protected:
	tagGlobalUserData				m_GlobalUserData;						//用户资料
	tagIndividualUserData			m_IndividualUserData;					//扩展资料

	//静态变量
protected:
	static CGlobalUserInfo *		m_pGlobalUserInfo;						//用户信息

	//函数定义
public:
	//构造函数
	CGlobalUserInfo();
	//析构函数
	virtual ~CGlobalUserInfo();

	//功能函数
public:
	//重置资料
	VOID ResetUserInfoData();
	//用户资料
	tagGlobalUserData * GetGlobalUserData() { return &m_GlobalUserData; }
	//扩展资料
	tagIndividualUserData * GetIndividualUserData() { return &m_IndividualUserData; }

	//静态函数
public:
	//获取对象
	static CGlobalUserInfo * GetInstance() { return m_pGlobalUserInfo; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif