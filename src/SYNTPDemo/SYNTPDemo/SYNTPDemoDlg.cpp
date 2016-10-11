// SYNTPDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SYNTPDemo.h"
#include "SYNTPDemoDlg.h"

#include "SYNTPManger.h"
SYNTPManger *g_syNtpMgr = NULL;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSYNTPDemoDlg dialog




CSYNTPDemoDlg::CSYNTPDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSYNTPDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSYNTPDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSYNTPDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CSYNTPDemoDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSYNTPDemoDlg message handlers

BOOL CSYNTPDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	g_syNtpMgr = SYNTPManger::Instance();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSYNTPDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSYNTPDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSYNTPDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSYNTPDemoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	delete g_syNtpMgr;
}

void CSYNTPDemoDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	time_t result = g_syNtpMgr->GetTimeWithHost();
	
	wchar_t wszBuf[256];
	memset(wszBuf, 0, sizeof(wszBuf));
	wsprintf(wszBuf, L"Time:%d/%02d/%02d %02d:%02d:%02d\n", g_syNtpMgr->GetYearWithTime(result),
															g_syNtpMgr->GetMonthWithTime(result),
															g_syNtpMgr->GetDayWithTime(result),
															g_syNtpMgr->GetHourWithTime(result),
															g_syNtpMgr->GetMinWithTime(result),
															g_syNtpMgr->GetSecWithTime(result));
	OutputDebugString(wszBuf);

	g_syNtpMgr->SetSystemTimeWithTime(result);

}
