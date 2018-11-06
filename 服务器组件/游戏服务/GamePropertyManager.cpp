#include "StdAfx.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////


//CINIFile文件
CINIFile::CINIFile(CString FileName,int maxsize)
{
	m_lpFileName=FileName;
	m_mMaxSize = maxsize;
}

CINIFile::~CINIFile()
{

}

void CINIFile::SetINIFileName(CString fileName)
{
	m_lpFileName=fileName;
}

	//判断文件是否存在
bool CINIFile::IsFileExist(CString fileName)
{
	WIN32_FIND_DATA  d;
	HANDLE  hd= FindFirstFile(fileName,&d);
	if(hd != INVALID_HANDLE_VALUE)
		return true;
	return false;
}

	//判断目录是否存在
bool	CINIFile::IsFolderExist(CString strFolder)
{
	WIN32_FIND_DATA    wfd;
    bool rValue = false;
	HANDLE hFind = FindFirstFile(strFolder, &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
		rValue = true;   
    }
    FindClose(hFind);
    return rValue;
}


void CINIFile::CopyFileTo(CString destFileName,CString srcFileName)
{
	bool bb=IsFileExist(srcFileName);
	if(!bb)return;
	CopyFile( srcFileName, destFileName, FALSE);
}
	
CString CINIFile::GetWinSysPath()
{
	TCHAR lpBuffer[255];
	GetSystemDirectory( lpBuffer, 255);
	CString ss=lpBuffer;
	ss += L"\\";
	return ss;
}
	
CString CINIFile::GetAppPath()
{
	TCHAR lpBuffer[MAX_PATH];
	memset(lpBuffer, 0x00, sizeof(lpBuffer)/sizeof(char));
//	//OutputDebugString(lpBuffer);
	GetModuleFileName( NULL, lpBuffer, MAX_PATH );
	CString		strDirectory = lpBuffer;
	lstrcpyn(lpBuffer, strDirectory.Left(strDirectory.ReverseFind(TCHAR('\\'))), MAX_PATH);
	strDirectory = lpBuffer;
	strDirectory += TEXT("\\");
//	//OutputDebugString(lpBuffer);
	return strDirectory;
}

int CINIFile::GetKeyVal(CString secName,CString keyName,int lpDefault)
{
	return (int) GetPrivateProfileInt(
				secName,// points to section name
				keyName,// points to key name
				lpDefault,       // return value if key name not found
				m_lpFileName//LPCTSTR lpFileName  // initialization file name
				);

}

/*
//根据KEY获得INI文件的值
double CINIFile::GetKeyVal(CString secName,CString keyName,double ldDefault)
{
	CString strValue;
	strValue = GetKeyVal(secName, keyName, TEXT(""));
	if(strValue.IsEmpty())
		return ldDefault;
	else
		return _wtof(strValue);
}
*/

CString CINIFile::GetKeyVal(CString secName,CString keyName,LPCTSTR lpDefault)
{ 
	TCHAR* re = new TCHAR[m_mMaxSize + 1];
	GetPrivateProfileString(
							secName,// points to section name
							keyName,// points to key name
							lpDefault,// points to default string
							re,// points to destination buffer
							m_mMaxSize,// size of destination buffer
							m_lpFileName // points to initialization filename
						);
	CString ss=re;
	delete []re;
	return ss;
}
	
void CINIFile::SetKeyValString(CString secName,CString keyName,CString Val)
{ 

	WritePrivateProfileString(
							secName,// pointer to section name
							keyName,// pointer to key name
							Val,// pointer to string to add
							m_lpFileName// pointer to initialization filename
							);

}


