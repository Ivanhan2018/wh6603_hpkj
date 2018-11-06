#include "StdAfx.h"
#include "TraceLog.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CTraceLog::CTraceLog()
{
	//设置变量
	m_bLoging = false;

	return;
}

//析构函数
CTraceLog::~CTraceLog()
{
	CloseLog();
}

//追踪信息
bool CTraceLog::TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	//输出日志
	if( m_bLoging && m_LogFile.is_open() )
	{
		if( TraceLevel == TraceLevel_Exception )
		{
			//获取时间
			SYSTEMTIME SystemTime;
			CHAR szTimeBuffer[64];
			GetLocalTime(&SystemTime);
			_snprintf(szTimeBuffer,CountArray(szTimeBuffer),"【 %04d-%02d-%02d %02d:%02d:%02d 】",
				SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

			m_LogFile << szTimeBuffer << (LPCSTR)CT2A(pszString) << "\n";

			m_LogFile.flush();
		}
	}

	return true;
}

//创建日志
bool CTraceLog::OpenLog(LPCTSTR lpszFile)
{
	//效验
	ASSERT(!m_bLoging);
	if( m_bLoging ) return false;

	ASSERT( !m_LogFile.is_open() );
	if( m_LogFile.is_open() )
		m_LogFile.close();

	m_bLoging = true;

	m_LogFile.open( (LPCSTR)CT2A(lpszFile),std::ios::out|std::ios::app );

	return m_bLoging;
}

//关闭日志
bool CTraceLog::CloseLog()
{
	if( m_LogFile.is_open() )
		m_LogFile.close();

	m_bLoging = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
