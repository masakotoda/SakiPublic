#include "stdafx.h"
#include "SakiColorGameTcpServer.h"


CSakiColorGameTcpVisitor::CSakiColorGameTcpVisitor(int nVisitorId)
	: m_hThread(NULL)
	, m_nStatus(statusNone)
	, m_nVisitorId(nVisitorId)
{
}


CSakiColorGameTcpVisitor::~CSakiColorGameTcpVisitor()
{
}


int CSakiColorGameTcpVisitor::ReceiveRequest(void* lpBuf, int nBufLen)
{
	return __super::Receive(lpBuf, nBufLen);
}

int CSakiColorGameTcpVisitor::SendResponse(void* lpBuf, int nBufLen)
{
	return __super::Send(lpBuf, nBufLen);
}

void CSakiColorGameTcpVisitor::OnReceive(int nErrorCode)
{
	__super::OnReceive(nErrorCode);
	
	char buffer[256] = { 0 };
	Receive(buffer, 256);
	TRACE(buffer);
}

void CSakiColorGameTcpVisitor::StartThread()
{
	unsigned long ulThreadID = 0;

	m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VisitorThread, (LPVOID)this, CREATE_SUSPENDED, &ulThreadID);

	if (m_hThread != NULL)
	{
		ResumeThread(m_hThread);
	}
}

void CSakiColorGameTcpVisitor::StopThread()
{
	m_nStatus = statusStopRequested;
}

DWORD WINAPI CSakiColorGameTcpVisitor::VisitorThread(LPVOID lpParam)
{
	auto pThis = reinterpret_cast<CSakiColorGameTcpVisitor*>(lpParam);
	pThis->m_nStatus = statusRunning;
	while (pThis->m_nStatus == statusRunning)
	{
		//TRACE(_T("a\n"));
		::Sleep(100);
	}
	return 0;
}

CSakiColorGameTcpServer::CSakiColorGameTcpServer(void)
{
}


CSakiColorGameTcpServer::~CSakiColorGameTcpServer(void)
{
}

bool CSakiColorGameTcpServer::RunServer()
{
	CString strIPAddress = _T("127.0.0.1");
	int nTCPPort = 1500;
	int nSocketType = SOCK_STREAM;
	long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE;

	auto ret = Create(nTCPPort, nSocketType, lEvent, strIPAddress);

	if (ret)
	{
		ret = Listen();
	}

	return ret ? true : false;
}

bool CSakiColorGameTcpServer::ShutdownServer()
{
	auto ret = ShutDown();

	Close();

	CloseVisitors();

	return ret  ? true : false;
}

void CSakiColorGameTcpServer::OnAccept(int nErrorCode)
{
	__super::OnAccept(nErrorCode);

	size_t nVisitorId = m_visitors.size() + 1;

	auto pVisitor = new CSakiColorGameTcpVisitor(nVisitorId);
	Accept(*pVisitor);

	DWORD dwTimeout = htonl(1000);
	pVisitor->SetSockOpt(SO_SNDTIMEO, (const char*)&dwTimeout, sizeof(dwTimeout));
	pVisitor->SetSockOpt(SO_RCVTIMEO, (const char*)&dwTimeout, sizeof(dwTimeout));

	pVisitor->StartThread();

	m_visitors.push_back(pVisitor);
}

void CSakiColorGameTcpServer::CloseVisitors()
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
