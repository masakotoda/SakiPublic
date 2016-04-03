
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

namespace
{

struct VertEdge
{
	VertEdge() : y0(0) , y1(0) {}
	VertEdge(int _y0, int _y1) : y0(_y0) , y1(_y1) {}

	int y0; // from
	int y1; // to
};

void pushRect(int l, int r, int h, std::map<int, VertEdge>& buffer)
{
	// Get left bound
	std::pair<int, VertEdge> leftBound(-1, VertEdge());
	{
		auto it = buffer.lower_bound(l);
		if (it != buffer.end() && it != buffer.begin())
		{
			it--;
			if (it->second.y1 > 0)
			{
				leftBound = *it;
			}
		}
	}
	
	// Get right bound
	std::pair<int, VertEdge> rightBound(-1, VertEdge());
	{
		auto it = buffer.upper_bound(r);
		if (it != buffer.end())
		{
			it--;
			if (it != buffer.end())
			{
				rightBound = *it;
			}
		}
	}

	// Fix left height if there is overlap.
	int leftH = h;
	auto leftOverlap = buffer.find(l);
	if (leftOverlap != buffer.end())
	{
		leftH = max(leftH, max(leftOverlap->second.y0, leftOverlap->second.y1));
	}

	// Fix right height if there is overlap.
	int rightH = h;
	auto rightOverlap = buffer.find(r);
	if (rightOverlap != buffer.end())
	{
		rightH = max(rightH, max(rightOverlap->second.y0, rightOverlap->second.y1));
	}

	// Push left side
	if (leftBound.first < 0)
	{
		buffer[l] = VertEdge(0, leftH);
	}
	else if (leftOverlap != buffer.end())
	{
		buffer[l] = VertEdge(leftOverlap->second.y0, max(h, leftOverlap->second.y1));
	}
	else if (leftBound.second.y1 < h)
	{
		buffer[l] = VertEdge(leftBound.second.y1, h);
	}

	// Push right side
	if (rightBound.first < 0)
	{
		buffer[r] = VertEdge(rightH, 0);
	}
	else if (rightOverlap != buffer.end())
	{
		buffer[r] = VertEdge(max(h, rightOverlap->second.y0), rightOverlap->second.y1);
	}
	else if (rightBound.second.y1 < h)
	{
		buffer[r] = VertEdge(h, rightBound.second.y1);
	}

	// Raise the vertics to new horizontal line if they are lower.
	auto lower = buffer.lower_bound(l);
	auto upper = buffer.upper_bound(r);
	for (auto it = lower; it != upper; it++)
	{
		if (it->first > l && it->first < r)
		{
			it->second.y1 = max(it->second.y1, h);
			it->second.y0 = max(it->second.y0, h);
		}
	}
}

CDC* prepDC(CDialog* dialog, int resId, COLORREF color)
{
	CWnd* item = dialog->GetDlgItem(resId);
	CDC* pDC = item->GetDC();
	CRect rect;
	item->GetClientRect(rect);
	rect.InflateRect(1, 1);
	CBrush bg(color);
	pDC->FillRect(rect, &bg);

	return pDC;
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

	// This is what I'm expecting to see.
	{
		auto pDC = prepDC(this, IDC_STATIC1, RGB(255, 0, 0));
		CBrush brush(RGB(0, 0, 0));
		for (int i = 0; i < count; i++)
		{
			int l = min(lefts[i], rights[i]);
			int r = max(lefts[i], rights[i]);
			int h = heights[i];
			if (l != r)
			{
				CRect rect(l, max_height - h, r, max_height);
				pDC->FillRect(rect, &brush);
			}
		}
		ReleaseDC(pDC);
	}


	// Generate the skyline and draw it.
	{
		std::map<int, VertEdge> buffer;
		for (int i = 0; i < count; i++)
		{
			int l = min(lefts[i], rights[i]);
			int r = max(lefts[i], rights[i]);
			int h = heights[i];
			if (l != r)
			{
				pushRect(l, r, h, buffer);
			}
		}

		auto pDC = prepDC(this, IDC_STATIC2, RGB(0, 255, 0));
		pDC->MoveTo(0, max_height);
		std::for_each (buffer.begin(), buffer.end(), 
			[pDC, max_height](std::pair<int, VertEdge> p) 
			{
				if (p.second.y0 != p.second.y1)
				{
					pDC->LineTo(p.first, max_height - p.second.y0);
					pDC->LineTo(p.first, max_height - p.second.y1);
				}
			}
		);
		pDC->LineTo(0, max_height);

		ReleaseDC(pDC);
	}
}
