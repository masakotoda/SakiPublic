
// SakiSnakeClientDlg.h : header file
//

#pragma once

#include "ISakiClientEngine.h"
class CSakiTcpClient;
namespace SakiSnakeCommon
{
	struct HelloMsg;
	struct JoinedMsg;
	struct UpdateBoardMsg;
	struct CountDownMsg;
	struct GameOverMsg;
};

// CSakiSnakeClientDlg dialog
class CSakiSnakeClientDlg : public CDialogEx, public ISakiClientEngine
{
// Construction
public:
	CSakiSnakeClientDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CSakiSnakeClientDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SakiSnakeClient_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonJoin();
	afx_msg void OnBnClickedButtonReady();

	DECLARE_MESSAGE_MAP()

	// Implementation ISakiClientEngine interface
public:
	virtual void OnReceive(unsigned char* data, int length) override;
	virtual void OnClose() override;

	// Data bound to controls
private:
	CString m_strStatus;
	DWORD m_ipaddress = 0;

	// Drawing methods
private:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	HRESULT OnRender();

private:
	bool HandleKeydown(unsigned short key);
	void HandleMsg(SakiSnakeCommon::HelloMsg& msg);
	void HandleMsg(SakiSnakeCommon::JoinedMsg& msg);
	void HandleMsg(SakiSnakeCommon::UpdateBoardMsg& msg);
	void HandleMsg(SakiSnakeCommon::CountDownMsg& msg);
	void HandleMsg(SakiSnakeCommon::GameOverMsg& msg);

private:
	struct CSakiPlayer
	{
		unsigned char m_playerId;
		ID2D1SolidColorBrush* m_brush;
		D2D1_COLOR_F m_color;
	};

	enum
	{
		stateWaiting,
		stateRunning,
		stateGameOver,
	};

	CSakiTcpClient* m_pTcpClient = nullptr;

	int m_state = stateWaiting;
	int m_width = 0;
	int m_height = 0;
	unsigned char m_me = 0;
	std::map<unsigned char, CSakiPlayer> m_players;
	std::vector<unsigned char> m_board;

	HWND m_hwnd = nullptr;
	ID2D1Factory* m_pDirect2dFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	ID2D1SolidColorBrush* m_pMarkerBrush = nullptr;
	ID2D1SolidColorBrush* m_pGridBrush = nullptr;
};
