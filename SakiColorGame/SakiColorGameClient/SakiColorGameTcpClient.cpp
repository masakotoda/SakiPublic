#include "stdafx.h"
#include "SakiColorGameTcpClient.h"


CSakiColorGameTcpClient::CSakiColorGameTcpClient(void)
{
}


CSakiColorGameTcpClient::~CSakiColorGameTcpClient(void)
{
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

