#include "StdAfx.h"
#include "LogFile.h"

LogFile::LogFile()
{
	memset(&m_szPath, 0, sizeof(m_szPath));
}

LogFile::~LogFile()
{

}

LogFile &LogFile::instance()
{
	static LogFile log;
	return log;
}


TCHAR*			LogFile::GetWorkFolder()
{
	if(m_szPath[0] == 0)
	{
		CString strDirectory;
		GetModuleFileName( NULL, m_szPath, 255 );
		strDirectory = m_szPath;
		wcscpy(m_szPath, strDirectory.Left(strDirectory.ReverseFind(TCHAR('\\'))));
	}
	return m_szPath;
}

int	LogFile::LogText(LPCTSTR lpszFormat,...)
{
	if(wcslen(lpszFormat) > 10240)
		return -1;
	CString strLog;	
	va_list argList;
	va_start(argList, lpszFormat);
	strLog.FormatV(lpszFormat, argList);

	va_end(argList);
	int nLen = strLog.GetLength();
#ifndef	_DEBUG
	LogText(strLog);
#else
	LogText(strLog);
#endif

	return nLen;
}



int	LogFile::LogText(CString& strLog)
{
	CString fname;
	CTime tm = CTime::GetCurrentTime();
	CString strToday = tm.Format(L"%Y-%m-%d");
	fname.Format(L"%s%s",strToday.GetBuffer(strToday.GetLength()), L".log");
	strToday.ReleaseBuffer();

	CString str_time = tm.Format(L"%Y-%m-%d %H:%M:%S, ");

	CStdioFile stdf;
	CString fullName;
	fullName.Format(L"%s\\log\\%s", GetWorkFolder(), (LPCTSTR)fname);
	CFileException ex;
	setlocale( LC_CTYPE, ("chs"));			//支持中文
	if( !stdf.Open( fullName, CStdioFile::modeReadWrite | CStdioFile::modeNoTruncate, &ex))
	{
		//		OUT_TEXT(fullName);
		Sleep(0);
		if(!stdf.Open( fullName, CStdioFile::modeReadWrite | CStdioFile::modeCreate| CStdioFile::modeNoTruncate, &ex))
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			OUT_TEXT(szError);
			return -1;
		}
	}

	stdf.SeekToEnd();
	stdf.WriteString(str_time);
	stdf.WriteString(strLog);
	stdf.WriteString(L"\n");
	stdf.Close();
	OUT_TEXT(strLog);
	//	int nlen = strLog.GetLength();
	return 0;
}
