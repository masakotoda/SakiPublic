#pragma once

class ISakiClientEngine;

class CSakiTcpClient : public CAsyncSocket
{
public:
	CSakiTcpClient(ISakiClientEngine* pEngine);
	virtual ~CSakiTcpClient(void);

	bool ConnectToServer(unsigned long ipaddrses);
	bool DisconnectFromServer();
	bool IsConnected();

public:
	virtual void OnConnect(int nErrorCode) override;
	virtual void OnReceive(int nErrorCode) override;
	virtual void OnClose(int nErrorCode) override;

private:
	int m_socket = 0;
	bool m_bConnected = false;
	ISakiClientEngine* const m_pEngine;
};

