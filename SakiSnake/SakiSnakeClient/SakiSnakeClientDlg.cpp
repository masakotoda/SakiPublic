
// SakiSnakeClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SakiSnakeClient.h"
#include "SakiSnakeClientDlg.h"
#include "afxdialogex.h"
#include "SakiTcpClient.h"
#include "SakiSnakeMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSakiSnakeClientDlg dialog
CSakiSnakeClientDlg::CSakiSnakeClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SAKISNAKECLIENT_DIALOG, pParent)
	, m_pTcpClient(nullptr)
	, m_pDirect2dFactory(nullptr)
	, m_pRenderTarget(nullptr)
	, m_pMarkerBrush(nullptr)
	, m_pGridBrush(nullptr) 
	, m_hwnd(nullptr)
	, m_width(0)
	, m_height(0)
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


CSakiSnakeClientDlg::~CSakiSnakeClientDlg()
{
	delete m_pTcpClient;

	SafeRelease(&m_pDirect2dFactory);

	DiscardDeviceResources();
}


void CSakiSnakeClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATUS, m_strStatus);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_ipaddress);
}

BEGIN_MESSAGE_MAP(CSakiSnakeClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_JOIN, &CSakiSnakeClientDlg::OnBnClickedButtonJoin)
	ON_BN_CLICKED(IDC_BUTTON_READY, &CSakiSnakeClientDlg::OnBnClickedButtonReady)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSakiSnakeClientDlg message handlers

BOOL CSakiSnakeClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HRESULT hr = CreateDeviceIndependentResources();
	ASSERT(SUCCEEDED(hr));

	m_hwnd = GetDlgItem(IDC_BOARD)->m_hWnd;

	return TRUE;  // return TRUE  unless you set the focus to a control
}


HRESULT CSakiSnakeClientDlg::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (m_pRenderTarget == nullptr)
    {
        RECT rc;
        ::GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
            );

        hr = m_pDirect2dFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &m_pRenderTarget
            );


        if (SUCCEEDED(hr))
        {
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::LightSlateGray),
                &m_pMarkerBrush
                );
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
                &m_pGridBrush
                );
        }

		for (auto& x : m_players)
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				x.second.m_color,
				&x.second.m_brush
				);
		}
    }

    return hr;
}


HRESULT CSakiSnakeClientDlg::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}


void CSakiSnakeClientDlg::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pMarkerBrush);
	SafeRelease(&m_pGridBrush);

	for (auto& x : m_players)
	{
		SafeRelease(&x.second.m_brush);
	}
}


HRESULT CSakiSnakeClientDlg::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		const float scaleFactor = 10.0f;

		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		// Draw a grid background.
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		for (int x = 0; x <= m_width * scaleFactor; x += static_cast<int>(scaleFactor))
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), m_height * scaleFactor),
				m_pMarkerBrush,
				0.5f
				);
		}

		for (int y = 0; y <= m_height * scaleFactor; y += static_cast<int>(scaleFactor))
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(m_width * scaleFactor, static_cast<FLOAT>(y)),
				m_pMarkerBrush,
				0.5f
				);
		}


		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				auto val = m_board.at(y * m_width + x);

				// Paint x,y with val & 0xf
				D2D1_RECT_F r = D2D1::RectF(x * scaleFactor, y * scaleFactor, (x + 1) * scaleFactor, (y + 1) * scaleFactor);
				if (m_players[val & 0xf].m_brush != nullptr)
				{
					m_pRenderTarget->FillRectangle(&r, m_players[val & 0xf].m_brush);
				}

				if (val == m_me)
				{
					//mark with square
					m_pRenderTarget->DrawRectangle(&r, m_pMarkerBrush);
				}
			}
		}

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}


void CSakiSnakeClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(cx, cy));
	}
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSakiSnakeClientDlg::OnPaint()
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

		OnRender();
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSakiSnakeClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSakiSnakeClientDlg::OnReceive(unsigned char* data, int length)
{
	SakiSnakeCommon::MsgHeader* header = reinterpret_cast<SakiSnakeCommon::MsgHeader*>(data);
	switch (header->_id)
	{
	case SakiSnakeCommon::msgHello:
	{
		auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgHello, SakiSnakeCommon::HelloMsg>(data, length);
		HandleMsg(msg._data);
		break;
	}
	case SakiSnakeCommon::msgJoined:
	{
		auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgJoined, SakiSnakeCommon::JoinedMsg>(data, length);
		HandleMsg(msg._data);
		break;
	}
	case SakiSnakeCommon::msgUpdateBoard:
	{
		auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgUpdateBoard, SakiSnakeCommon::UpdateBoardMsg>(data, length);
		HandleMsg(msg._data);
		break;
	}
	case SakiSnakeCommon::msgCountDown:
	{
		auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgCountDown, SakiSnakeCommon::CountDownMsg>(data, length);
		HandleMsg(msg._data);
		break;
	}
	case SakiSnakeCommon::msgGameOver:
	{
		auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgGameOver, SakiSnakeCommon::GameOverMsg>(data, length);
		HandleMsg(msg._data);
		break;
	}
	default:
		ASSERT(false);
		break;
	}
}


void CSakiSnakeClientDlg::OnClose()
{
	m_strStatus = _T("Lost connection.");
	UpdateData(FALSE);

	m_state = stateWaiting;
	delete m_pTcpClient;
	m_pTcpClient = nullptr;

	for (auto& x : m_players)
	{
		SafeRelease(&x.second.m_brush);
	}
	m_players.clear();
}


void CSakiSnakeClientDlg::HandleMsg(SakiSnakeCommon::HelloMsg& msg)
{
	m_strStatus.Format(_T("Connected. You are player %d."), msg._playerId);
	UpdateData(FALSE);

	m_me = msg._playerId;
	m_width = msg._width;
	m_height = msg._height;

	ASSERT(m_players.find(msg._playerId) == m_players.end());

	CSakiPlayer player;
	player.m_playerId = msg._playerId;
	player.m_color = D2D1::ColorF(msg._r, msg._g, msg._b, 1.0f);
	auto hr = m_pRenderTarget->CreateSolidColorBrush(
		player.m_color, &player.m_brush);
	m_players[msg._playerId] = player;
}


void CSakiSnakeClientDlg::HandleMsg(SakiSnakeCommon::JoinedMsg& msg)
{
	if (m_players.find(msg._playerId) == m_players.end())
	{
		CSakiPlayer player;
		player.m_playerId = msg._playerId;
		player.m_color = D2D1::ColorF(msg._r, msg._g, msg._b, 1.0f);
		auto hr = m_pRenderTarget->CreateSolidColorBrush(
			player.m_color, &player.m_brush);
		m_players[msg._playerId] = player;
	}
}


void CSakiSnakeClientDlg::HandleMsg(SakiSnakeCommon::UpdateBoardMsg& msg)
{
	ASSERT(msg._cells.size() == m_height * m_width);

	if (m_state == stateGameOver)
	{
		m_strStatus = _T("Click 'Ready' when you are ready.");
		UpdateData(FALSE);
	}

	m_board = msg._cells;

	Invalidate();
}


void CSakiSnakeClientDlg::HandleMsg(SakiSnakeCommon::CountDownMsg& msg)
{
	m_strStatus.Format(_T("%d second(s) to start..."), msg._second);

	if (msg._second == 0)
	{
		m_state = stateRunning;
	}

	UpdateData(FALSE);
}


void CSakiSnakeClientDlg::HandleMsg(SakiSnakeCommon::GameOverMsg& msg)
{
	m_state = stateGameOver;

	if (msg._winner == m_me)
	{
		m_strStatus.Format(_T("Game over. You are the winner!"));
	}
	else
	{
		m_strStatus.Format(_T("Game over. Winner is player %d"), msg._winner);
	}

	UpdateData(FALSE);
}


BOOL CSakiSnakeClientDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (HandleKeydown(pMsg->wParam))
		{
			return TRUE;
		}
	}
	
	return __super::PreTranslateMessage(pMsg);
}


bool CSakiSnakeClientDlg::HandleKeydown(unsigned short key)
{
	bool success = false;

	if (m_pTcpClient != nullptr && m_pTcpClient->IsConnected() && m_state == stateRunning)
	{
		std::map<unsigned short, unsigned char> dir;
		dir[VK_LEFT] = SakiSnakeCommon::directionWest;
		dir[VK_RIGHT] = SakiSnakeCommon::directionEast;
		dir[VK_DOWN] = SakiSnakeCommon::directionSouth;
		dir[VK_UP] = SakiSnakeCommon::directionNorth;

		if (dir.find(key) != dir.end())
		{
			SakiSnakeCommon::Msg<SakiSnakeCommon::msgSetDirection, SakiSnakeCommon::SetDirectionMsg> msg;
			msg._data._direction = dir[key];
			auto sent = m_pTcpClient->Send(&msg, sizeof(msg));
			success = (sent == sizeof(msg));
		}
	}

	return success;
}


void CSakiSnakeClientDlg::OnBnClickedButtonJoin()
{
	UpdateData(TRUE);

	if (m_pTcpClient == nullptr)
	{
		m_pTcpClient = new CSakiTcpClient(this);
	}

	if (!m_pTcpClient->IsConnected())
	{
		m_pTcpClient->ConnectToServer(m_ipaddress);
		m_strStatus = _T("Connecting...");
		UpdateData(FALSE);
	}
}


void CSakiSnakeClientDlg::OnBnClickedButtonReady()
{
	if (m_pTcpClient != nullptr && m_pTcpClient->IsConnected())
	{
		SakiSnakeCommon::Msg<SakiSnakeCommon::msgSetReady, SakiSnakeCommon::SetReadyMsg> msg;
		m_pTcpClient->Send(&msg, sizeof(msg));
	}
	else
	{
		AfxMessageBox(_T("'Join' first."));
	}
}
