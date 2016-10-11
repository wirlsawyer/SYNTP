#include "stdafx.h"
#include "SYNTPManger.h"

SYNTPManger* SYNTPManger::m_instance = NULL;

SYNTPManger* SYNTPManger::Instance()
{
	if (m_instance == NULL){
		m_instance = new SYNTPManger(); 
	}
	return m_instance;
}

SYNTPManger::SYNTPManger()
{
	m_hModule					= NULL;
	m_DLLFuncTestAdd			= NULL;
	m_DLLFuncGetTimeWithHost	= NULL;

	//-------------------------------------------------------------------------------
	m_hModule = LoadLibrary(L".\\SYNTP.dll");
	//-------------------------------------------------------------------------------
	if ( m_hModule != NULL )
	{
		//-------------------------------------------------------------------------------
		m_DLLFuncTestAdd = (SYNTP_TestAdd)GetProcAddress( m_hModule, "TestAdd" );
		if (m_DLLFuncTestAdd == NULL)
		{
			OutputDebugString(L"GetProc fault:TestAdd\n");
			::MessageBox(NULL, L"GetProc fault:TestAdd", L"", MB_OK);
			FreeLibrary(m_hModule);
			return;
		}

		//-------------------------------------------------------------------------------
		
		m_DLLFuncGetTimeWithHost = (SYNTP_GetTimeWithHost)GetProcAddress( m_hModule, "GetTimeWithHost" );
		if (m_DLLFuncGetTimeWithHost == NULL)
		{
			OutputDebugString(L"GetProc fault:GetTimeWithHost\n");
			::MessageBox(NULL, L"GetProc fault:GetTimeWithHost", L"", MB_OK);
			FreeLibrary(m_hModule);
			return;
		}

	}else{
		OutputDebugString(L"SYNTPManger fault:Can't find SYNTP.dll\n");
	}
}

SYNTPManger::~SYNTPManger()
{
	FreeLibrary(m_hModule);
}


int SYNTPManger::TestAdd(int a, int b)
{
	return m_DLLFuncTestAdd(a, b);
}


time_t SYNTPManger::GetTimeWithHostW(wchar_t *pwszHost, wchar_t *pwszPort)
{
	char *pHost = NULL;	
	pHost = (char *)new char[2 * wcslen(pwszHost)+1] ;
	memset(pHost , 0 , 2 * wcslen(pwszHost)+1 );
	int   nLen   =   (int)wcslen(pwszHost)+1;
	WideCharToMultiByte(CP_ACP,   0,   pwszHost,   nLen,   pHost,   2*nLen,   NULL,   NULL);

	char *pPort = NULL;	
	pPort = (char *)new char[2 * wcslen(pwszPort)+1] ;
	memset(pPort , 0 , 2 * wcslen(pwszPort)+1 );
	nLen   =   (int)wcslen(pwszPort)+1;
	WideCharToMultiByte(CP_ACP,   0,   pwszPort,   nLen,   pPort,   2*nLen,   NULL,   NULL);

	//  delete pAniFilename;
	time_t result = m_DLLFuncGetTimeWithHost(pHost, pPort);

	delete pHost;
	delete pPort;
	return result;
}


time_t SYNTPManger::GetTimeWithHost(char *pszHost, char *pszPort)
{
	/*
	wchar_t wszBuf[256];
	memset(wszBuf, 0, sizeof(wszBuf));
	wsprintf(wszBuf, L"Time:%s\n", _wctime(&result));
	OutputDebugString(wszBuf);
	*/
	return m_DLLFuncGetTimeWithHost(pszHost, pszPort);
}

int SYNTPManger::GetYearWithTime(time_t time)
{
	struct tm p;
#ifdef BCB
	p = *localtime(&time);
#else
	localtime_s(&p, &time);
#endif
	return  (1900+p.tm_year);
}

int SYNTPManger::GetMonthWithTime(time_t time)
{
	struct tm p;
#ifdef BCB
	p = *localtime(&time);
#else
	localtime_s(&p, &time);
#endif
	return (1+p.tm_mon);
}

int SYNTPManger::GetDayWithTime(time_t time)
{
	struct tm p;
#ifdef BCB
	p = *localtime(&time);
#else
	localtime_s(&p, &time);
#endif
	return p.tm_mday;
}

int SYNTPManger::GetHourWithTime(time_t time)
{
	struct tm p;
#ifdef BCB
	p = *localtime(&time);
#else
	localtime_s(&p, &time);
#endif
	return p.tm_hour;
}

int SYNTPManger::GetMinWithTime(time_t time)
{
	struct tm p;
#ifdef BCB
	p = *localtime(&time);
#else
	localtime_s(&p, &time);
#endif
	return p.tm_min;
}

int SYNTPManger::GetSecWithTime(time_t time)
{
	struct tm p;
#ifdef BCB
	p = *localtime(&time);
#else
	localtime_s(&p, &time);
#endif
	return p.tm_sec;
}

int SYNTPManger::GetTimeZone(void)
{
// Get the local system time.
	SYSTEMTIME LocalTime = { 0 };
	GetSystemTime( &LocalTime );

	// Get the timezone info.
	TIME_ZONE_INFORMATION TimeZoneInfo;
	GetTimeZoneInformation( &TimeZoneInfo );

	// Convert local time to UTC.
	SYSTEMTIME GmtTime = { 0 };
	TzSpecificLocalTimeToSystemTime( &TimeZoneInfo,
		&LocalTime,
		&GmtTime );

	// GMT = LocalTime + TimeZoneInfo.Bias
	// TimeZoneInfo.Bias is the difference between local time
	// and GMT in minutes. 

	// Local time expressed in terms of GMT bias.
	float TimeZoneDifference = -( float(TimeZoneInfo.Bias) / 60 );
	return (int)TimeZoneDifference;
}

void SYNTPManger::SetSystemTimeWithTime(time_t time)
{
	struct tm p;
#ifdef BCB
	p = *localtime(&time);
#else
	localtime_s(&p, &time);
#endif

	SYSTEMTIME st;
	
	st.wYear = (1900+p.tm_year);
	st.wMonth = (1+p.tm_mon);
	st.wDay = p.tm_mday;
	st.wMinute = p.tm_min;
	st.wSecond = p.tm_sec;
	st.wMilliseconds = 0;
	st.wDayOfWeek = p.tm_wday;

	st.wHour = p.tm_hour-GetTimeZone();
	if ( st.wHour <0 ) st.wHour+=24;
		

	SetSystemTime(&st); //時區問題
}




