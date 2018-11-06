#ifndef GAME_PROPERTY_MANAGER_HEAD_FILE
#define GAME_PROPERTY_MANAGER_HEAD_FILE

#pragma once

#include "GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////





class 	GAME_SERVICE_CLASS CINIFile  
{
  CString	m_lpFileName ;
  DWORD		m_mMaxSize ;
public:
	//判断文件是否存在
	static bool IsFileExist(CString fileName);

	//判断目录是否存在
	static bool	IsFolderExist(CString strFolder);

	//复制文件
	static void CopyFileTo(CString destFileName,CString srcFileName);
	
	//设置INI文件名称
	void SetINIFileName(CString fileName);

	//获得系统路径
	static CString GetWinSysPath();
	
	//获得应用程序路径（EXE）
	static CString GetAppPath();

	//根据KEY获得INI文件的值
	int GetKeyVal(CString secName,CString keyName,int lpDefault);

	//根据KEY获得INI文件的值
//	double GetKeyVal(CString secName,CString keyName,double ldDefault);

	//根据Key获得INI文件的值
	CString GetKeyVal(CString secName,CString keyName,LPCTSTR lpDefault);
	
	//写INI文件
	void SetKeyValString(CString secName,CString keyName,CString Val);

	//构造函数		
	CINIFile(CString FileName,int maxsize=255);

	//析构函数
	~CINIFile();
};

//////////////////////////////////////////////////////////////////////////////////

#endif