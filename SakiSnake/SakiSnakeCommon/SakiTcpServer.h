#pragma once

class ISakiServerEngine;

class CSakiTcpVisitor : public CAsyncSocket
{
public:
	CSakiTcpVisitor(int nVisitorId, ISakiServerEngine* pEngine);
	virtual ~CSakiTcpVisitor();

	int ReceiveRequest(void* lpBuf, int nBufLen);
	int SendResponse(void* lpBuf, int nBufLen);

	void StartThread();
	void StopThread();

public:
	virtual void OnReceive(int nErrorCode) override;
	virtual void OnClose(int nErrorCode) override;

private:

	static DWORD WINAPI VisitorThread(LPVOID lpParam);

	enum
	{
		statusNone,
		statusRunning,
		statusStopRequested,
	};

	HANDLE m_hThread;
	int m_nStatus;
	ISakiServerEngine* const m_pEngine;
};

class CSakiTcpServer : public CAsyncSocket
{
public:
	CSakiTcpServer(ISakiServerEngine* pEngine);
	virtual ~CSakiTcpServer(void);

	bool RunServer(unsigned long ipaddress);
	bool ShutdownServer();

public:
	virtual void OnAccept(int nErrorCode) override;

private:
	void CloseVisitors();

private:
	int m_socket = 0;
	std::vector<CSakiTcpVisitor*> m_visitors;
	ISakiServerEngine* const m_pEngine;
};

