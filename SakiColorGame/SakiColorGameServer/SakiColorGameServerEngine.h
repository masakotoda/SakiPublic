#pragma once

class CSakiColorGameTcpServer;
class CSakiColorGameTcpVisitor;

class CSakiColorGameServerEngine
{
public:
	CSakiColorGameServerEngine(void);
	virtual ~CSakiColorGameServerEngine(void);

	void StartGame();
	void EndGame();
	void HandleMessage(CSakiColorGameTcpVisitor*pVisitor, const char* request, char* response, int size_response);

	int m_nRedOwner;
	int m_nGreenOwner;
	int m_nBlueOwner;

	CSakiColorGameTcpServer* m_pServer;
	CRITICAL_SECTION m_criticalSection;
};

