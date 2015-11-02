
// SakiColorGameClientDlg.h : header file
//

#pragma once


// CSakiColorGameClientDlg dialog
class CSakiColorGameTcpClient;
class CSakiColorGameClientEngine;

class CSakiColorGameClientDlg : public CDialogEx
{
// Construction
public:
	CSakiColorGameClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SAKICOLORGAMECLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConnect();

	CSakiColorGameClientEngine* m_pGameEngine;
	CSakiColorGameTcpClient* m_pClient;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonRequestRed();
	afx_msg void OnStnClickedPicture();
	afx_msg void OnBnClickedButtonRequestGreen();
	afx_msg void OnBnClickedButtonRequestBlue();
};
