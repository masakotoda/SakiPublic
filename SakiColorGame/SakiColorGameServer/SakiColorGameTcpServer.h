#pragma once

class CSakiColorGameTcpVisitor : public CAsyncSocket
{
public:
	CSakiColorGameTcpVisitor(int nVisitorId);
	virtual ~CSakiColorGameTcpVisitor();

	int ReceiveRequest(void* lpBuf, int nBufLen);
	int SendResponse(void* lpBuf, int nBufLen);

	void StartThread();
	void StopThread();

public:
	virtual void OnReceive(int nErrorCode) override;

private:

	static DWORD WINAPI VisitorThread(LPVOID lpParam);

	enum
	{
		statusNone,
		statusRunning,
		statusStopRequested,
	};

	const int m_nVisitorId;
	HANDLE m_hThread;
	int m_nStatus;	
};

class CSakiColorGameTcpServer : public CAsyncSocket
{
public:
	CSakiColorGameTcpServer(void);
	virtual ~CSakiColorGameTcpServer(void);

	bool RunServer();
	bool ShutdownServer();

public:
	virtual void OnAccept(int nErrorCode);

private:
	void CloseVisitors();

private:
	std::vector<CSakiColorGameTcpVisitor*> m_visitors;
};

