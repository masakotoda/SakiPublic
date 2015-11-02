#include "stdafx.h"
#include "SakiColorGameTcpClient.h"


CSakiColorGameTcpClient::CSakiColorGameTcpClient(void)
	: m_bConnected(false)
{
}


CSakiColorGameTcpClient::~CSakiColorGameTcpClient(void)
{
}


bool CSakiColorGameTcpClient::IsConnected()
{
	return m_bConnected;
}


bool CSakiColorGameTcpClient::ConnectToServer()
{
	int ret = Create();
	if (ret)
	{
		CString strIPAddress = _T("127.0.0.1");
		int nTCPPort = 1500;
		ret = Connect(strIPAddress, nTCPPort);
		int nErr = GetLastError();

		if (nErr == WSAEWOULDBLOCK)
		{
			//See http://msdn.microsoft.com/en-us/library/windows/desktop/ms740668(v=vs.85).aspx
			ret = TRUE;
		}
	}
	else
	{
		ASSERT(false);
		TRACE(_T("Did you call AfxSocketInit?\n"));
	}

	return ret ? true : false;
}


bool CSakiColorGameTcpClient::DisconnectFromServer()
{
	ShutDown(CAsyncSocket::both);
	Close();
	return true;
}

void CSakiColorGameTcpClient::OnConnect(int nErrorCode)
{
	if (nErrorCode == 0)
		m_bConnected = true;
	else
		m_bConnected = false;
	__super::OnConnect(nErrorCode);
}

void CSakiColorGameTcpClient::OnReceive(int nErrorCode)
{
	__super::OnReceive(nErrorCode);
}

void CSakiColorGameTcpClient::OnClose(int nErrorCode)
{
	__super::OnClose(nErrorCode);
}

