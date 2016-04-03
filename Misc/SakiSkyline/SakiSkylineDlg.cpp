
// SakiSkylineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SakiSkyline.h"
#include "SakiSkylineDlg.h"
#include "afxdialogex.h"
#include "../random_helper.h"

#include <vector>
#include <map>
#include <algorithm>

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


// CSakiSkylineDlg dialog



CSakiSkylineDlg::CSakiSkylineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSakiSkylineDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSakiSkylineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSakiSkylineDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSakiSkylineDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSakiSkylineDlg message handlers

BOOL CSakiSkylineDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSakiSkylineDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSakiSkylineDlg::OnPaint()
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
HCURSOR CSakiSkylineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


struct HeadTailY
{
	HeadTailY() : headY(0) , tailY(0) {}
	HeadTailY(int _minY, int _maxY) : headY(_minY) , tailY(_maxY) {}

	int headY;
	int tailY;
};

std::map<int, HeadTailY> buffer;

void pushRect(int l, int r, int h)
{
	int left = -1;
	{
		auto lower = buffer.lower_bound(l);
		if (lower != buffer.end() && lower != buffer.begin())
		{
			lower--;
			if (lower->second.tailY > 0)
			{
				left = lower->first;
			}
		}
	}
	
	int h1 = h;
	{
		auto t = buffer.find(l);
		if (t != buffer.end())
		{
			h1 = max(h1, t->second.headY);
			h1 = max(h1, t->second.tailY);
		}
	}


	HeadTailY up;
	int right = -1;
	{
		auto upper = buffer.upper_bound(r);
		if (upper != buffer.end())
		{
			upper--;
			if (upper != buffer.end())
			{
				right = upper->first;
				up = upper->second;
			}
		}
	}

	int h2 = h;
	{
		auto t = buffer.find(r);
		if (t != buffer.end())
		{
			h2 = max(h2, t->second.headY);
			h2 = max(h2, t->second.tailY);
		}
	}

	if (left < 0)
	{
		buffer[l] = HeadTailY(0, h1);
	}
	else
	{
		auto t = buffer.find(l);
		if (t != buffer.end())
		{
			if (t->second.headY > t->second.tailY) // down
				buffer[l] = HeadTailY(t->second.headY, max(h, t->second.tailY));
			else
				buffer[l] = HeadTailY(t->second.headY, max(h, t->second.tailY));
		}
		else
		{
			auto it = buffer.find(left);
			if (it->second.tailY < h)
			{
				buffer[l] = HeadTailY(it->second.tailY, h);
			}
		}
	}

	if (right < 0)
	{
		buffer[r] = HeadTailY(h2, 0);
	}
	else
	{
		auto it = buffer.find(right);
		if (it->first == r)
		{
			if (up.headY > up.tailY) // down
				buffer[r] = HeadTailY(max(h, up.headY), up.tailY);
			else
				buffer[r] = HeadTailY(max(h, up.headY), up.tailY);
		}
		else if (up.tailY < h)
		{
			buffer[r] = HeadTailY(h, up.tailY);
		}
	}

	auto lower = buffer.lower_bound(l);
	auto upper = buffer.upper_bound(r);

	for (auto it = lower; it != upper; it++)
	{
		if (it->first > l && it->first < r)
		{
			it->second.tailY = max(it->second.tailY, h);
			it->second.headY = max(it->second.headY, h);
		}
	}
}



void CSakiSkylineDlg::OnBnClickedButton1()
{
	int count = 25;
	int max_height = 100;
	int max_width = 200;

	auto lefts = getRandoms(0, max_width, count);
	auto rights = getRandoms(0, max_width, count);
	auto heights = getRandoms(10, max_height, count);

	auto count0 = std::count_if(lefts.begin(), lefts.end(), [](int val) -> bool { return val == 0; } );
	auto count100 = std::count_if(lefts.begin(), lefts.end(), [max_width](int val) -> bool { return val == max_width; } );

	{
		auto item = GetDlgItem(IDC_STATIC1);
		CDC* pDC = item->GetDC();
		CBrush bg(RGB(255, 0, 0));
		CRect r(0, 0, max_width, max_height);
		r.InflateRect(1, 1);
		pDC->FillRect(r, &bg);

		CBrush brush(RGB(0, 0, 0));
		for (int i = 0; i < count; i++)
		{
			int l = min(lefts[i], rights[i]);
			int r = max(lefts[i], rights[i]);
			if (l != r)
			{
				CRect rect(l, max_height - heights[i], r, max_height);
				pDC->FillRect(rect, &brush);
			}
		}
		ReleaseDC(pDC);
	}


	{
		buffer.clear();
		for (int i = 0; i < count; i++)
		{
			int l = min(lefts[i], rights[i]);
			int r = max(lefts[i], rights[i]);
			int h = heights[i];
			if (l != r)
			{
				pushRect(l, r, h);
			}
		}

		auto item = GetDlgItem(IDC_STATIC2);
		CDC* pDC = item->GetDC();
		CBrush bg(RGB(0, 255, 0));
		CRect rect(0, 0, max_width, max_height);
		rect.InflateRect(1, 1);
		pDC->FillRect(rect, &bg);

		pDC->MoveTo(0, max_height);
		std::for_each (buffer.begin(), buffer.end(), 
			[pDC, max_height](std::pair<int, HeadTailY> p) 
			{
				if (p.second.headY != p.second.tailY)
				{
					pDC->LineTo(p.first, max_height - p.second.headY);
					pDC->LineTo(p.first, max_height - p.second.tailY);
				}
			}
		);

		ReleaseDC(pDC);
	}
}
