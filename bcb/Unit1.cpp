//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "../bin/SYNTPManger.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	SYNTPManger *g_syNtpMgr = SYNTPManger::Instance();
	time_t result = SYNTPManger::Instance()->GetTimeWithHost();
	wchar_t wszBuf[256];
	memset(wszBuf, 0, sizeof(wszBuf));
	wsprintf(wszBuf, L"Time:%s\n", _wctime(&result));
	OutputDebugString(wszBuf);

	//SYNTPManger::Instance()->SetSystemTimeWithTime(result);

	memset(wszBuf, 0, sizeof(wszBuf));
	wsprintf(wszBuf, L"Time:%d/%02d/%02d %02d:%02d:%02d\n", g_syNtpMgr->GetYearWithTime(result),
															g_syNtpMgr->GetMonthWithTime(result),
															g_syNtpMgr->GetDayWithTime(result),
															g_syNtpMgr->GetHourWithTime(result),
															g_syNtpMgr->GetMinWithTime(result),
															g_syNtpMgr->GetSecWithTime(result));
	OutputDebugString(wszBuf);
}
//---------------------------------------------------------------------------
