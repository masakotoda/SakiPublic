#pragma once

class CSakiColorGameServerEngine;

class CSakiColorGameTcpVisitor : public CAsyncSocket
{
public:
	CSakiColorGameTcpVisitor(int nVisitorId, CSakiColorGameServerEngine* pEngine);
	virtual ~CSakiColorGameTcpVisitor();

	int ReceiveRequest(void* lpBuf, int nBufLen);
	int SendResponse(void* lpBuf, int nBufLen);

	void StartThread();
	void StopThread();
	int GetVisitorId();

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
	CSakiColorGameServerEngine* const m_pEngine;
};

class CSakiColorGameTcpServer : public CAsyncSocket
{
public:
	CSakiColorGameTcpServer(CSakiColorGameServerEngine* pEngine);
	virtual ~CSakiColorGameTcpServer(void);

	bool RunServer();
	bool ShutdownServer();

public:
	virtual void OnAccept(int nErrorCode);

private:
	void CloseVisitors();

private:
	std::vector<CSakiColorGameTcpVisitor*> m_visitors;
	CSakiColorGameServerEngine* const m_pEngine;
};

