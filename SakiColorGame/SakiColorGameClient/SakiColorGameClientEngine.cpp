#include "stdafx.h"
#include "SakiColorGameTcpClient.h"
#include "SakiColorGameClientEngine.h"


void CSakiColorRed::Initialize()
{
	m_strName = _T("RED");
	m_bRequireRed = true;
	m_bRequireGreen = false;
	m_bRequireBlue = false;
	m_brush.CreateSolidBrush(RGB(255, 0, 0));
}


void CSakiColorGreen::Initialize()
{
	m_strName = _T("GRN");
	m_bRequireRed = false;
	m_bRequireGreen = true;
	m_bRequireBlue = false;
	m_brush.CreateSolidBrush(RGB(0, 255, 0));
}


void CSakiColorBlue::Initialize()
{
	m_strName = _T("BLU");
	m_bRequireRed = false;
	m_bRequireGreen = false;
	m_bRequireBlue = true;
	m_brush.CreateSolidBrush(RGB(0, 0, 255));
}


void CSakiColorRG::Initialize()
{
	m_strName = _T("R+G");
	m_bRequireRed = true;
	m_bRequireGreen = true;
	m_bRequireBlue = false;
	m_brush.CreateSolidBrush(RGB(255, 255, 0));
}


void CSakiColorRB::Initialize()
{
	m_strName = _T("R+B");
	m_bRequireRed = true;
	m_bRequireGreen = false;
	m_bRequireBlue = true;
	m_brush.CreateSolidBrush(RGB(255, 0, 255));
}


void CSakiColorGB::Initialize()
{
	m_strName = _T("G+B");
	m_bRequireRed = false;
	m_bRequireGreen = true;
	m_bRequireBlue = true;
	m_brush.CreateSolidBrush(RGB(0, 255, 255));
}


CSakiColorGameClientEngine::CSakiColorGameClientEngine(void)
	: m_bHasRed(false)
	, m_bHasGreen(false)
	, m_bHasBlue(false)
	, m_pClient(nullptr)
{
	srand(static_cast<unsigned int>(time(NULL)));

	//Generate number between 0 and 5 (0: Red, 1: Green, 2: Blue, 3: R+G, 4: R+B, 5: G+B)
	for (int i = 0; i < countCell; i++)
	{
		for (int j = 0; j < countCell; j++)
		{
			m_rows[i].cells[j].m_colorIndex = rand() % 6 + 0;
			m_rows[i].cells[j].m_colored = false;
		}
	}

	ASSERT(countColors == 6);
	m_brushes[0] = new CSakiColorRed();
	m_brushes[1] = new CSakiColorGreen();
	m_brushes[2] = new CSakiColorBlue();
	m_brushes[3] = new CSakiColorRG();
	m_brushes[4] = new CSakiColorRB();
	m_brushes[5] = new CSakiColorGB();
	for (auto x : m_brushes)
	{
		x->Initialize();
	}

	m_pClient = new CSakiColorGameTcpClient();
}


CSakiColorGameClientEngine::~CSakiColorGameClientEngine(void)
{
	for (auto x : m_brushes)
	{
		delete x;
	}

	m_pClient->DisconnectFromServer();
	delete m_pClient;
}

void CSakiColorGameClientEngine::UpdateState()
{
	if (m_pClient->IsConnected())
	{
		m_strLastStatus = _T("Connected");
	}
}

CString CSakiColorGameClientEngine::GetStatus()
{
	return m_strLastStatus;
}

CString CSakiColorGameClientEngine::GetMessage()
{
	return m_strLastMessage;
}


void CSakiColorGameClientEngine::StartGame()
{
	m_strLastStatus = _T("Connecting to server.");
	
	m_pClient->ConnectToServer();
}

void CSakiColorGameClientEngine::Draw(CDC* pDC)
{
	const int scaleFactor = 40;
	for (int i = 0; i < countCell; i++)
	{
		for (int j = 0; j < countCell; j++)
		{
			auto& cell = m_rows[i].cells[j];
			int colorIndex = cell.m_colorIndex;
			bool colored = cell.m_colored;
			if (colored)
			{
				pDC->SelectObject(m_brushes[colorIndex]->m_brush);
				pDC->Rectangle(j * scaleFactor, i * scaleFactor, (j + 1) * scaleFactor, (i + 1) * scaleFactor);
			}
			else
			{
				pDC->TextOut(j * scaleFactor, i * scaleFactor, m_brushes[colorIndex]->m_strName);
			}
		}
	}
}


void CSakiColorGameClientEngine::Click(CPoint pt)
{
	ColorCell* pCell = nullptr;
	const int scaleFactor = 40;
	for (int i = 0; i < countCell && pCell == nullptr; i++)
	{
		auto& row = m_rows[i];
		for (int j = 0; j < countCell && pCell == nullptr; j++)
		{
			auto& cell = row.cells[j];
			if (pt.x >= j * scaleFactor &&
				pt.x < (j + 1) * scaleFactor &&
				pt.y >= i * scaleFactor &&
				pt.y < (i + 1) * scaleFactor)
			{
				pCell = &cell;
			}
		}
	}

	if (pCell != nullptr)
	{
		if (HasColor(pCell->m_colorIndex))
		{
			pCell->m_colored = true;
		}
	}
}


bool CSakiColorGameClientEngine::WaitForResponse(char* response, int response_size)
{
	for (int i = 0; i < 10; i++)
	{
		int nRet = m_pClient->Receive(response, response_size);
		if (nRet > 0)
		{
			return true;
		}
		Sleep(100);
	}

	return false;
}


void CSakiColorGameClientEngine::ToggleRed()
{
	bool bOK = false;

	const int bufSize = 32;
	char request[bufSize] = { 0 };
	char response[bufSize] = { 0 };

	if (!m_bHasRed)
	{
		strcpy_s(request, "Request Red");
	}
	else
	{
		strcpy_s(request, "Return Red");
	}

	m_pClient->Send((BYTE*)request, sizeof(request));
	
	WaitForResponse(response, bufSize);
	
	if (strcmp(response, "OK") == 0)
	{
		m_bHasRed = m_bHasRed ? false : true;
	}
	else
	{
		m_strLastMessage = _T("Someone else is using Red.");
	}
}


void CSakiColorGameClientEngine::ToggleBlue()
{
	bool bOK = false;

	const int bufSize = 32;
	char request[bufSize] = { 0 };
	char response[bufSize] = { 0 };

	if (!m_bHasBlue)
	{
		strcpy_s(request, "Request Blue");
	}
	else
	{
		strcpy_s(request, "Return Blue");
	}

	m_pClient->Send((BYTE*)request, sizeof(request));
	
	WaitForResponse(response, bufSize);
	
	if (strcmp(response, "OK") == 0)
	{
		m_bHasBlue = m_bHasBlue ? false : true;
	}
	else
	{
		m_strLastMessage = _T("Someone else is using Blue.");
	}
}


void CSakiColorGameClientEngine::ToggleGreen()
{
	bool bOK = false;

	const int bufSize = 32;
	char request[bufSize] = { 0 };
	char response[bufSize] = { 0 };

	if (!m_bHasGreen)
	{
		strcpy_s(request, "Request Green");
	}
	else
	{
		strcpy_s(request, "Return Green");
	}

	m_pClient->Send((BYTE*)request, sizeof(request));
	
	WaitForResponse(response, bufSize);
	
	if (strcmp(response, "OK") == 0)
	{
		m_bHasGreen = m_bHasGreen ? false : true;
	}
	else
	{
		m_strLastMessage = _T("Someone else is using Green.");
	}
}

bool CSakiColorGameClientEngine::HasRed()
{
	return m_bHasRed;
}

bool CSakiColorGameClientEngine::HasBlue()
{
	return m_bHasBlue;
}

bool CSakiColorGameClientEngine::HasGreen()
{
	return m_bHasGreen;
}


bool CSakiColorGameClientEngine::HasColor(int index)
{
	bool hasColor = false;

	if (m_brushes[index]->m_bRequireRed == m_bHasRed &&
		m_brushes[index]->m_bRequireBlue == m_bHasBlue &&
		m_brushes[index]->m_bRequireGreen == m_bHasGreen)
	{
		hasColor = true;
	}

	return hasColor;
}
