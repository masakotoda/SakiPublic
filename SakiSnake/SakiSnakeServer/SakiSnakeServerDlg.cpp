
// SakiSnakeServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SakiSnakeServer.h"
#include "SakiSnakeServerDlg.h"
#include "afxdialogex.h"

#include "SakiTcpServer.h"
#include "SakiServerEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSakiSnakeServerDlg dialog

CSakiSnakeServerDlg::CSakiSnakeServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SAKISNAKESERVER_DIALOG, pParent)
	, m_strStatus(_T(""))
	, m_pEngine(nullptr)
	, m_pTcpServer(nullptr)
	, m_width(29)
	, m_height(17)
	, m_ipaddress(0)
{
	// 192.168.0.5
	m_ipaddress |= 192;
	m_ipaddress = (m_ipaddress << 8);
	m_ipaddress |= 168;
	m_ipaddress = (m_ipaddress << 8);
	m_ipaddress |= 0;
	m_ipaddress = (m_ipaddress << 8);
	m_ipaddress |= 5;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


CSakiSnakeServerDlg::~CSakiSnakeServerDlg()
{
	delete m_pEngine;
	delete m_pTcpServer;
}


void CSakiSnakeServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATUS, m_strStatus);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_ipaddress);
}

BEGIN_MESSAGE_MAP(CSakiSnakeServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RUNSERVER, &CSakiSnakeServerDlg::OnBnClickedButtonRunserver)
END_MESSAGE_MAP()


// CSakiSnakeServerDlg message handlers

BOOL CSakiSnakeServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSakiSnakeServerDlg::OnPaint()
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
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSakiSnakeServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSakiSnakeServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	m_pEngine->Tick();

	CDialogEx::OnTimer(nIDEvent);
}


void CSakiSnakeServerDlg::OnBnClickedButtonRunserver()
{
	UpdateData(TRUE);

	if (m_pEngine == nullptr)
	{
		m_pEngine = new CSakiServerEngine(m_width, m_height);
		m_pTcpServer = new CSakiTcpServer(m_pEngine);
		m_pTcpServer->RunServer(m_ipaddress);

		SetTimer(1, 500, nullptr);
	}
}
