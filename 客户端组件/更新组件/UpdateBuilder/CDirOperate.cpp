///////////////////////////////////////////////////////
// CDirOperate.cpp文件
#include "stdAfx.h"
#include "CDirOperate.h"

CDirOperate::CDirOperate()
{
}

CDirOperate::~CDirOperate()
{
}

void CDirOperate::CreateAllDirectory(LPCTSTR lpszDir)
{
	int nPos = 0;
	CString strTemp = lpszDir;
	if(strTemp.Right(1)!="\\")
		strTemp += "\\";
	
	//逐层创建目录
	while((nPos = strTemp.Find('\\',nPos+1)) != -1)
	{
		if(nPos != 2) //排除第一个'\\',(如：C:\\)
		{
			if(!IsDirExist(strTemp.Left(nPos+1))) //判断是否存在
				::CreateDirectory(strTemp.Left(nPos+1),NULL);
		}
	}
}

//判断指定的目录是否存在
BOOL CDirOperate::IsDirExist(LPCTSTR lpszDir)
{
	BOOL bExist;
	int n;
	CString szTemp = lpszDir;
	CString szDir;
	n = szTemp.ReverseFind('\\');
	if(n<=0)
		return FALSE;
	szDir = szTemp.Left(n);

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(szDir,&wfd);
	if(hFind == INVALID_HANDLE_VALUE)
		bExist = FALSE;
	else
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			bExist = TRUE;//目录存在
		else
			bExist = FALSE;//目录不存在
		FindClose(hFind);
	}
	return bExist;
}

//判断指定的目录是否存在
BOOL CDirOperate::CheckFolderExist(LPCTSTR lpszDir)
{
    CString strTemp = lpszDir;
    strTemp=strTemp.TrimRight(_T("\\"));
	strTemp=strTemp.TrimRight(_T("/"));
	
	WIN32_FIND_DATA  wfd;
    BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strTemp.GetString(), &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        rValue = TRUE;   
    }
    FindClose(hFind);
    return rValue;
}


//删除给定目录下的所有文件及其子目录
void CDirOperate::RecursiveDelete(LPCTSTR lpszPath)
{
	CString szTemp = lpszPath;
	CString szPath; 
	if(szTemp.Right(1) != "\\")
		szTemp += "\\";
	szPath = szTemp; //保留原始目录供下面使用
	szTemp += "*.*";

	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	hFind = FindFirstFile(szTemp,&wfd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			//名称为"."的目录代表本目录，为".."的目录代表上一层目录
			if(wfd.cFileName[0] == '.')
				continue;

			if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString szFilePathName;//文件的全路径名
				//wfd.cFileName只有文件名，没有路径。szFilePathName保存了文件的全路径名。
				szFilePathName = szPath + wfd.cFileName;
				::SetFileAttributes(szFilePathName,FILE_ATTRIBUTE_NORMAL);
				::DeleteFile(szFilePathName);
			}
			else
			{
				CString szFilePathName;
				szFilePathName = szPath + wfd.cFileName;
				RecursiveDelete(szFilePathName);
				::SetFileAttributes(szFilePathName,FILE_ATTRIBUTE_NORMAL);
				//删除此目录(RemoveDirectory只能删除空目录)
				::RemoveDirectory(szFilePathName);
			}
		}while(::FindNextFile(hFind,&wfd));
		::FindClose(hFind);
	}
}

//删除整个目录
void CDirOperate::DeleteAllDirectory(LPCTSTR lpszDir)
{ 
	RecursiveDelete(lpszDir); //首先删除此目录下的所有文件及其子目录
	::RemoveDirectory(lpszDir); //删除此目录
}

//-------------------------------------------------------------------------
// Function Name    :IsFileExist
// Parameter(s)     :LPCTSTR lpszFilePath	File path
// Return           :
// Create			:2007-4-23 15:13	Jerry.Wang
// Memo             :verify the file exist
//-------------------------------------------------------------------------
BOOL CDirOperate::IsFileExist(LPCTSTR lpszFilePath)
{
	BOOL bExist = FALSE;
	HANDLE hFile = NULL;

	hFile = CreateFile( lpszFilePath
		, GENERIC_READ
		, FILE_SHARE_READ | FILE_SHARE_WRITE
		, NULL
		, OPEN_EXISTING
		, 0
		, 0
		);

	if( hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle( hFile );
		bExist = TRUE;
	}

	return bExist;
}