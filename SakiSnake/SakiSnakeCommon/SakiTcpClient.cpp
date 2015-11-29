#include "stdafx.h"
#include "SakiTcpClient.h"
#include "ISakiClientEngine.h"
#include "SakiSnakeMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSakiTcpClient::CSakiTcpClient(ISakiClientEngine* pEngine)
	: m_bConnected(false)
	, m_pEngine(pEngine)
{
}


CSakiTcpClient::~CSakiTcpClient(void)
{
}


bool CSakiTcpClient::IsConnected()
{
	return m_bConnected;
}


bool CSakiTcpClient::ConnectToServer(unsigned long ipaddress)
{
	int ret = 0;

	if (m_socket == 0)
	{
		m_socket = Create();
	}

	if (m_socket)
	{
		CString strIPAddress;
		strIPAddress.Format(_T("%d.%d.%d.%d"),
			ipaddress >> 24, 
			(ipaddress >> 16) & 0xff, 
			(ipaddress >> 8) & 0xff, 
			ipaddress & 0xff);
		
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


bool CSakiTcpClient::DisconnectFromServer()
{
	ShutDown(CAsyncSocket::both);
	Close();
	return true;
}


void CSakiTcpClient::OnConnect(int nErrorCode)
{
	if (nErrorCode == 0)
		m_bConnected = true;
	else
		m_bConnected = false;
	__super::OnConnect(nErrorCode);
}


void CSakiTcpClient::OnReceive(int nErrorCode)
{
	static unsigned char buf[10 * 1024];

	__super::OnReceive(nErrorCode);
	int recv = Receive(buf, sizeof(buf));

	auto head = &buf[0];
	int handled = 0;
	while (recv > handled)
	{
		auto header = reinterpret_cast<SakiSnakeCommon::MsgHeader*>(head);

		auto length = header->_length + sizeof(*header);
		m_pEngine->OnReceive(head, length);

		head += length;
		handled += length;
	}
}

void CSakiTcpClient::OnClose(int nErrorCode)
{
	__super::OnClose(nErrorCode);

	m_pEngine->OnClose();
}

