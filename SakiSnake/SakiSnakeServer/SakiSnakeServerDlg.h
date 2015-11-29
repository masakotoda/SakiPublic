
// SakiSnakeServerDlg.h : header file
//

#pragma once

class CSakiServerEngine;
class CSakiTcpServer;


// CSakiSnakeServerDlg dialog
class CSakiSnakeServerDlg : public CDialogEx
{
// Construction
public:
	CSakiSnakeServerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CSakiSnakeServerDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAKISNAKESERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strStatus;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CSakiServerEngine* m_pEngine;
	CSakiTcpServer* m_pTcpServer;
	afx_msg void OnBnClickedButtonRunserver();
	int m_width;
	int m_height;
private:
	DWORD m_ipaddress;
};
