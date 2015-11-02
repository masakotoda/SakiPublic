#pragma once

class CSakiColorGameTcpClient : public CAsyncSocket
{
public:
	CSakiColorGameTcpClient(void);
	virtual ~CSakiColorGameTcpClient(void);

	bool ConnectToServer();
	bool DisconnectFromServer();
	bool IsConnected();

public:
	virtual void OnConnect(int nErrorCode) override;
	virtual void OnReceive(int nErrorCode) override;
	virtual void OnClose(int nErrorCode) override;

private:
	bool m_bConnected;
};

