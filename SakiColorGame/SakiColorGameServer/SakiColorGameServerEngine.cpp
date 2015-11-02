#include "stdafx.h"
#include "SakiColorGameTcpServer.h"
#include "SakiColorGameServerEngine.h"


CSakiColorGameServerEngine::CSakiColorGameServerEngine(void)
	: m_nRedOwner(0)
	, m_nGreenOwner(0)
	, m_nBlueOwner(0)
{
	m_pServer = new CSakiColorGameTcpServer(this);
	::InitializeCriticalSection(&m_criticalSection);
}


CSakiColorGameServerEngine::~CSakiColorGameServerEngine(void)
{
	::DeleteCriticalSection(&m_criticalSection);
}


void CSakiColorGameServerEngine::StartGame()
{
	m_pServer->RunServer();
}


void CSakiColorGameServerEngine::EndGame()
{
	m_pServer->ShutdownServer();
	delete m_pServer;
}


void CSakiColorGameServerEngine::HandleMessage(CSakiColorGameTcpVisitor*pVisitor, const char* request, char* response, int size_response)
{
	::EnterCriticalSection(&m_criticalSection);

	strcpy_s(response, size_response, "OK");

	if (strcmp(request, "Request Red") == 0)
	{
		if (m_nRedOwner != 0)
		{
			strcpy_s(response, size_response, "Someone else is using it.");
		}
		else
		{
			m_nRedOwner = pVisitor->GetVisitorId();
		}
	}
	else if (strcmp(request, "Request Green") == 0)
	{
		if (m_nGreenOwner != 0)
		{
			strcpy_s(response, size_response, "Someone else is using it.");
		}
		else
		{
			m_nGreenOwner = pVisitor->GetVisitorId();
		}
	}
	else if (strcmp(request, "Request Blue") == 0)
	{
		if (m_nBlueOwner != 0)
		{
			strcpy_s(response, size_response, "Someone else is using it.");
		}
		else
		{
			m_nBlueOwner = pVisitor->GetVisitorId();
		}
	}
	else if (strcmp(request, "Return Red") == 0)
	{
		m_nRedOwner = 0;
	}
	else if (strcmp(request, "Return Green") == 0)
	{
		m_nGreenOwner = 0;
	}
	else if (strcmp(request, "Return Blue") == 0)
	{
		m_nBlueOwner = 0;
	}

	::LeaveCriticalSection(&m_criticalSection);
}

