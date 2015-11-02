
// SakiColorGameClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SakiColorGameClient.h"
#include "SakiColorGameClientDlg.h"
#include "afxdialogex.h"
#include "SakiColorGameTcpClient.h"
#include "SakiColorGameClientEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSakiColorGameClientDlg dialog



CSakiColorGameClientDlg::CSakiColorGameClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSakiColorGameClientDlg::IDD, pParent)
	, m_pClient(nullptr)
	, m_pGameEngine(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSakiColorGameClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSakiColorGameClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CSakiColorGameClientDlg::OnBnClickedButtonConnect)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_RED, &CSakiColorGameClientDlg::OnBnClickedButtonRequestRed)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_GREEN, &CSakiColorGameClientDlg::OnBnClickedButtonRequestGreen)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_BLUE, &CSakiColorGameClientDlg::OnBnClickedButtonRequestBlue)
	ON_STN_CLICKED(IDC_PICTURE, &CSakiColorGameClientDlg::OnStnClickedPicture)
END_MESSAGE_MAP()


// CSakiColorGameClientDlg message handlers

BOOL CSakiColorGameClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	m_pClient = new CSakiColorGameTcpClient();
	m_pGameEngine = new CSakiColorGameClientEngine();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSakiColorGameClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSakiColorGameClientDlg::OnPaint()
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
		CDialogEx::OnPaint();

		auto pDC = GetDlgItem(IDC_PICTURE)->GetDC();
		if (pDC != nullptr)
		{
			m_pGameEngine->Draw(pDC);

			ReleaseDC(pDC);
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSakiColorGameClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSakiColorGameClientDlg::OnBnClickedButtonConnect()
{
	// TODO: Add your control notification handler code here
	m_pClient->ConnectToServer();
}


void CSakiColorGameClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pClient->DisconnectFromServer();
	delete m_pClient;

	delete m_pGameEngine;
}


void CSakiColorGameClientDlg::OnBnClickedButtonRequestRed()
{
	// TODO: Add your control notification handler code here
	char request[] = "Red";
	m_pClient->Send((BYTE*)request, sizeof(request));
	m_pGameEngine->AcquiredRed();
}


void CSakiColorGameClientDlg::OnBnClickedButtonRequestGreen()
{
	// TODO: Add your control notification handler code here
	m_pGameEngine->AcquiredGreen();
}


void CSakiColorGameClientDlg::OnBnClickedButtonRequestBlue()
{
	// TODO: Add your control notification handler code here
	m_pGameEngine->AcquiredBlue();
}


void CSakiColorGameClientDlg::OnStnClickedPicture()
{
	CPoint pt;
	{
		::GetCursorPos(&pt);
		auto pWnd = GetDlgItem(IDC_PICTURE);
		pWnd->ScreenToClient(&pt);
	}

	m_pGameEngine->Click(pt);
	Invalidate();
}


