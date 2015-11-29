#include "stdafx.h"
#include "SakiTcpServer.h"
#include "ISakiServerEngine.h"
#include "SakiSnakeMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSakiTcpVisitor::CSakiTcpVisitor(int nVisitorId, ISakiServerEngine* pEngine)
	: m_hThread(NULL)
	, m_nStatus(statusNone)
	, m_pEngine(pEngine)
{
}


CSakiTcpVisitor::~CSakiTcpVisitor()
{
}


int CSakiTcpVisitor::ReceiveRequest(void* lpBuf, int nBufLen)
{
	return __super::Receive(lpBuf, nBufLen);
}


int CSakiTcpVisitor::SendResponse(void* lpBuf, int nBufLen)
{
	return __super::Send(lpBuf, nBufLen);
}


void CSakiTcpVisitor::OnReceive(int nErrorCode)
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
		m_pEngine->OnReceive(this, head, length);

		head += length;
		handled += length;
	}
}


void CSakiTcpVisitor::OnClose(int nErrorCode)
{
	m_pEngine->OnClose(this);
}


void CSakiTcpVisitor::StartThread()
{
	unsigned long ulThreadID = 0;

	m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VisitorThread, (LPVOID)this, CREATE_SUSPENDED, &ulThreadID);

	if (m_hThread != NULL)
	{
		ResumeThread(m_hThread);
	}
}


void CSakiTcpVisitor::StopThread()
{
	m_nStatus = statusStopRequested;
}


DWORD WINAPI CSakiTcpVisitor::VisitorThread(LPVOID lpParam)
{
	auto pThis = reinterpret_cast<CSakiTcpVisitor*>(lpParam);
	pThis->m_nStatus = statusRunning;
	pThis->m_pEngine->OnAccepted(pThis);
	while (pThis->m_nStatus == statusRunning)
	{
		::Sleep(1000);
	}
	return 0;
}


CSakiTcpServer::CSakiTcpServer(ISakiServerEngine* pEngine)
	: m_pEngine(pEngine)
{
}


CSakiTcpServer::~CSakiTcpServer(void)
{
}


bool CSakiTcpServer::RunServer(unsigned long ipaddress)
{
	bool ret = false;

	if (m_socket == 0)
	{
		CString strIPAddress;
		strIPAddress.Format(_T("%d.%d.%d.%d"),
			ipaddress >> 24,
			(ipaddress >> 16) & 0xff,
			(ipaddress >> 8) & 0xff,
			ipaddress & 0xff);

		int nTCPPort = 1500;
		int nSocketType = SOCK_STREAM;
		long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE;

		m_socket = Create(nTCPPort, nSocketType, lEvent, strIPAddress);

		if (m_socket)
		{
			ret = Listen();
		}
	}

	return ret;
}


bool CSakiTcpServer::ShutdownServer()
{
	auto ret = ShutDown();

	Close();

	CloseVisitors();

	return ret ? true : false;
}


void CSakiTcpServer::OnAccept(int nErrorCode)
{
	__super::OnAccept(nErrorCode);

	size_t nVisitorId = m_visitors.size() + 1;

	auto pVisitor = new CSakiTcpVisitor(nVisitorId, m_pEngine);
	Accept(*pVisitor);

	DWORD dwTimeout = htonl(1000);
	pVisitor->SetSockOpt(SO_SNDTIMEO, (const char*)&dwTimeout, sizeof(dwTimeout));
	pVisitor->SetSockOpt(SO_RCVTIMEO, (const char*)&dwTimeout, sizeof(dwTimeout));

	pVisitor->StartThread();
	m_visitors.push_back(pVisitor);
}


void CSakiTcpServer::CloseVisitors()
{
	for (auto x : m_visitors)
	{
		x->StopThread();
		x->ShutDown();
		x->Close();
		delete x;
	}
	m_visitors.clear();
}

