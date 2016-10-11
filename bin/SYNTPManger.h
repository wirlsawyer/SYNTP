#ifndef _SYNTPMANGER_H_
#define _SYNTPMANGER_H_
#include <windows.h>
#include <time.h>

typedef int (WINAPI* SYNTP_TestAdd)(int a, int b);
typedef time_t (WINAPI* SYNTP_GetTimeWithHost)(char *pszHost, char *pszPort);

//typedef bool (WINAPI* SYUSB_RegisterWithHwnd)(HWND hwnd);
//typedef bool (WINAPI* SYUSB_UnRegister)(void);
//typedef std::vector<SYWlanInfo*> *(WINAPI* SYWlan_Scan)(void);
//typedef bool (WINAPI* SYWlan_ConnetWithSSID)(WCHAR *wszSSID);

class SYNTPManger
{

public:	
	//creat
	static SYNTPManger* Instance(void);
	~SYNTPManger(void);
	//method
	int TestAdd(int a, int b);
	time_t GetTimeWithHost(char *pszHost="time.windows.com", char *pszPort="123");
	time_t GetTimeWithHostW(wchar_t *pwszHost=L"time.windows.com", wchar_t *pwszPort=L"123");
	int GetYearWithTime(time_t time);
	int GetMonthWithTime(time_t time);
	int GetDayWithTime(time_t time);
	int GetHourWithTime(time_t time);
	int GetMinWithTime(time_t time);
	int GetSecWithTime(time_t time);

	int GetTimeZone(void);
	void SetSystemTimeWithTime(time_t time);
private:
	//method
	SYNTPManger(void);	


private:
	//var
	//creat
	static SYNTPManger*	m_instance;		
	//dll
	HMODULE  				m_hModule;

	SYNTP_TestAdd			m_DLLFuncTestAdd;
	SYNTP_GetTimeWithHost	m_DLLFuncGetTimeWithHost;
							
};

#endif