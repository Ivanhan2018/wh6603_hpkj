#include "stdafx.h"
#ifdef	_DEBUG

	#define		OUT_TEXT	OutputDebugString

#else
	#define	OUT_TEXT	
#endif

class LogFile
{
public:
	LogFile();
	~LogFile();

	int		LogText(LPCTSTR lpszFormat,...);
	int		LogText(CString& strLog);


	TCHAR*			GetWorkFolder();
	static	LogFile &instance();
private:

	TCHAR	m_szPath[255];

};

