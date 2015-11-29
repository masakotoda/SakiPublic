#include "stdafx.h"
#include "SakiServerEngine.h"
#include "SakiSnakeMsg.h"


CSakiPlayer::CSakiPlayer(unsigned char player, CAsyncSocket* pSocket) : m_pSocket(pSocket)
{
	m_playerId = player;
	Initialize();
}


bool CSakiPlayer::IsMe(CAsyncSocket* pSocket) const
{
	return (m_pSocket == pSocket);
}


void CSakiPlayer::SetDirection(int direction)
{
	m_direction = direction;
}


void CSakiPlayer::SetReady(bool ready)
{
	m_ready = ready;
}


bool CSakiPlayer::IsReady() const
{
	return m_ready;
}


bool CSakiPlayer::Send(unsigned char* data, int length)
{
	auto sent = m_pSocket->Send(data, length);
	return (sent == length);
}


void CSakiPlayer::SetCurrent(int x, int y)
{ 
	m_x = x; 
	m_y = y; 
}


int CSakiPlayer::GetX() const 
{ 
	return m_x; 
}


int CSakiPlayer::GetY() const 
{ 
	return m_y; 
}


int CSakiPlayer::PlayerId() const 
{ 
	return m_playerId; 
}


D2D1::ColorF CSakiPlayer::Color() const 
{ 
	return m_color; 
}


void CSakiPlayer::Initialize()
{
	m_ready = false;

	switch (m_playerId)
	{
	case 1:
		m_direction = SakiSnakeCommon::directionEast;
		m_color = D2D1::ColorF(D2D1::ColorF::HotPink);
		break;
	case 2:
		m_direction = SakiSnakeCommon::directionWest;
		m_color = D2D1::ColorF(D2D1::ColorF::DarkSlateBlue);
		break;
	case 3:
		m_direction = SakiSnakeCommon::directionSouth;
		m_color = D2D1::ColorF(D2D1::ColorF::DarkOrange);
		break;
	case 4:
		m_direction = SakiSnakeCommon::directionNorth;
		m_color = D2D1::ColorF(D2D1::ColorF::ForestGreen);
		break;
	}
}


int CSakiPlayer::NextX() const
{
	int x = m_x;

	switch (m_direction)
	{
	case SakiSnakeCommon::directionEast:
		x++;
		break;
	case SakiSnakeCommon::directionWest:
		x--;
		break;
	}

	return x;
}


int CSakiPlayer::NextY() const
{
	int y = m_y;

	switch (m_direction)
	{
	case SakiSnakeCommon::directionNorth:
		y--;
		break;
	case SakiSnakeCommon::directionSouth:
		y++;
		break;
	}

	return y;
}


void CSakiPlayer::Move()
{
	switch (m_direction)
	{
	case SakiSnakeCommon::directionNorth:
		m_y--;
		break;
	case SakiSnakeCommon::directionSouth:
		m_y++;
		break;
	case SakiSnakeCommon::directionEast:
		m_x++;
		break;
	case SakiSnakeCommon::directionWest:
		m_x--;
		break;
	}
}


void CSakiPlayer::Remove()
{
	m_x = -1;
	m_y = -1;
}


void CSakiBoard::SetSize(int width, int height)
{
	m_width = width;
	m_height = height;
	m_cells.resize(m_width * m_height);
}


int CSakiBoard::Width() const 
{ 
	return m_width; 
}


int CSakiBoard::Height() const 
{ 
	return m_height; 
}


unsigned char CSakiBoard::GetCell(int index) const 
{
	return m_cells[index]; 
}


void CSakiBoard::SetCell(int index, unsigned char val) 
{ 
	m_cells[index] = val; 
}


size_t CSakiBoard::CellCount() const 
{
	return m_cells.size(); 
}


unsigned char* CSakiBoard::Cells() 
{ 
	return &m_cells[0]; 
}


int CSakiBoard::GetCellIndex(int x, int y)
{
	return y * m_width + x;
}


void CSakiBoard::Clear(unsigned char playerId)
{
	for (auto it = m_cells.begin(); it != m_cells.end(); it++)
	{
		if ((*it & 0xf) == playerId)
		{
			*it = 0;
		}
	}
}


void CSakiBoard::Clear()
{
	if (m_cells.size() > 0)
	{
		memset(&m_cells[0], 0, m_cells.size());
	}
}


bool CSakiBoard::InRange(int x, int y)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		return true;
	else
		return false;
}


bool CSakiBoard::IsOccupied(int x, int y)
{
	auto index = GetCellIndex(x, y);
	return (m_cells[index] != 0);
}


CSakiServerEngine::CSakiServerEngine(int width, int height)
{
	m_board.SetSize(width, height);
}


CSakiServerEngine::~CSakiServerEngine()
{
}


void CSakiServerEngine::Tick()
{
	switch (m_state)
	{
	case stateNone:
		HandleStateNone();
		break;
	case stateAccepting:
		HandleStateAccepting();
		break;
	case stateCountDown:
		HandleStateCountDown();
		break;
	case stateRunning:
		HandleStateRunning();
		break;
	case stateGameOver:
		HandleStateGameOver();
		break;
	}
}


void CSakiServerEngine::HandleStateNone()
{
	if (m_players.size() != 0)
	{
		m_state = stateAccepting;
	}
}


void CSakiServerEngine::HandleStateAccepting()
{
	if (m_players.size() == SakiSnakeCommon::maxPlayers)
	{
		m_state = stateCountDown;
	}
	else if (m_players.size() >= SakiSnakeCommon::minPlayers)
	{
		size_t playersReady = 0;
		for (auto x : m_players)
		{
			if (x.second->IsReady())
			{
				playersReady++;
			}
		}
		if (playersReady == m_players.size())
		{
			m_state = stateCountDown;
		}
	}

	if (m_state == stateCountDown)
	{
		m_startCounting = ::GetTickCount64();
	}
}


void CSakiServerEngine::HandleStateCountDown()
{
	int second = -1;

	auto current = ::GetTickCount64();
	int delta = static_cast<int>(current - m_startCounting);
	if (delta >= 5000)
	{
		m_state = stateRunning;
		second = 0;
	}
	else
	{
		second = 1 + (5000 - delta) / 1000;
	}

	for (auto x : m_players)
	{
		SakiSnakeCommon::Msg<SakiSnakeCommon::msgCountDown, SakiSnakeCommon::CountDownMsg> msg;
		msg._data._second = second;
		x.second->Send(reinterpret_cast<unsigned char*>(&msg), sizeof(msg));
	}
}


void CSakiServerEngine::HandleStateRunning()
{
	std::map<int, unsigned char> nexts;
	for (auto player : m_players)
	{
		int x = player.second->NextX();
		int y = player.second->NextY();
		int cell = m_board.GetCellIndex(x, y);

		if (!m_board.InRange(x, y))
		{
			m_goners.insert(player.first);
		}
		else if (m_board.IsOccupied(x, y))
		{
			m_goners.insert(player.first);
		}
		else if (nexts.find(cell) != nexts.end())
		{
			m_goners.insert(nexts[cell]);
			m_goners.insert(player.first);
		}
		else
		{
			nexts[cell] = player.first;
		}
	}

	for (auto player : m_players)
	{
		if (m_goners.find(player.first) == m_goners.end())
		{
			player.second->Move();
			m_board.SetCell(m_board.GetCellIndex(player.second->GetX(), player.second->GetY()), player.first);
		}
		else
		{
			player.second->Remove();
			m_board.Clear(player.first);
		}
	}

	BroadcastBoard();

	int survivors = m_players.size() - m_goners.size();
	if (survivors <= 1)
	{
		m_state = stateGameOver;
	}
}


void CSakiServerEngine::HandleStateGameOver()
{
	if (m_startEnding == 0)
	{
		m_startEnding = ::GetTickCount64();

		// Who is the winner?
		unsigned char winner = 0;
		for (auto x : m_players)
		{
			if (m_goners.find(x.first) == m_goners.end())
			{
				winner = x.first;
				break;
			}
		}

		// Notify the winner to everybody
		for (auto x : m_players)
		{
			auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgGameOver, SakiSnakeCommon::GameOverMsg>();
			msg._data._winner = winner;
			x.second->Send((unsigned char*)&msg, sizeof(msg));
		}
	}
	else if (::GetTickCount64() - m_startEnding > 5000) // Reset game 5 seconds after the game over.
	{
		m_board.Clear();

		m_goners.clear();
		
		m_startEnding = 0;

		for (auto x : m_players)
		{
			InitializePlayer(x.second);
		}

		if (m_players.size() > 0 && m_players.size() < SakiSnakeCommon::maxPlayers)
		{
			m_state = stateAccepting;
		}
		else if (m_players.size() == SakiSnakeCommon::maxPlayers)
		{
			m_state = stateCountDown;
			m_startCounting = ::GetTickCount64();
		}
		else if (m_players.size() == 0)
		{
			m_state = stateNone;
		}

		BroadcastBoard();
	}
}


void CSakiServerEngine::OnClose(CAsyncSocket* pVisitor)
{
	for (auto it = m_players.begin(); it != m_players.end(); it++)
	{
		if (it->second->IsMe(pVisitor))
		{
			delete it->second;
			m_players.erase(it);
			break;
		}
	}
}


bool CSakiServerEngine::OnAccepted(CAsyncSocket* pVisitor)
{
	auto player = JoinGame(pVisitor);

	if (player != nullptr)
	{
		// Send Hello message to new player
		{
			auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgHello, SakiSnakeCommon::HelloMsg>();
			msg._data._playerId = player->PlayerId();
			msg._data._height = m_board.Height();
			msg._data._width = m_board.Width();
			msg._data._r = player->Color().r;
			msg._data._g = player->Color().g;
			msg._data._b = player->Color().b;
			player->Send((unsigned char*)&msg, sizeof(msg));
		}

		// Send Joined message to everybody
		for (auto x : m_players)
		{
			for (auto y : m_players)
			{
				if (y.first != x.second->PlayerId())
				{
					auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgJoined, SakiSnakeCommon::JoinedMsg>();
					msg._data._playerId = y.second->PlayerId();
					msg._data._r = y.second->Color().r;
					msg._data._g = y.second->Color().g;
					msg._data._b = y.second->Color().b;
					x.second->Send((unsigned char*)&msg, sizeof(msg));
				}
			}
		}

		BroadcastBoard();
	}

	return (player != nullptr);
}


void CSakiServerEngine::OnReceive(CAsyncSocket* pVisitor, unsigned char* data, int length)
{
	auto player = GetPlayer(pVisitor);

	SakiSnakeCommon::MsgHeader* header = reinterpret_cast<SakiSnakeCommon::MsgHeader*>(data);
	switch (header->_id)
	{
		case SakiSnakeCommon::msgSetReady:
		{
			auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgSetReady, SakiSnakeCommon::SetReadyMsg>(data, length);
			HandleMsg(player, msg._data);
			break;
		}
		case SakiSnakeCommon::msgSetDirection:
		{
			auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgSetDirection, SakiSnakeCommon::SetDirectionMsg>(data, length);
			HandleMsg(player, msg._data);
			break;
		}
	}
}


void CSakiServerEngine::HandleMsg(CSakiPlayer* pPlayer, SakiSnakeCommon::SetReadyMsg& msg)
{
	pPlayer->SetReady(true);
}


void CSakiServerEngine::HandleMsg(CSakiPlayer* pPlayer, SakiSnakeCommon::SetDirectionMsg& msg)
{
	pPlayer->SetDirection(msg._direction);
}


CSakiPlayer* CSakiServerEngine::GetPlayer(CAsyncSocket* pVisitor)
{
	for (auto x : m_players)
	{
		if (x.second->IsMe(pVisitor))
		{
			return x.second;
		}
	}

	return nullptr;
}


CSakiPlayer* CSakiServerEngine::JoinGame(CAsyncSocket* pVisitor)
{
	CSakiPlayer* player = nullptr;
	unsigned char playerId = 0;

	for (int i = SakiSnakeCommon::player1; i <= SakiSnakeCommon::player4 && player == 0; i++)
	{
		if (m_players.find(i) == m_players.end())
		{
			playerId = i;
			player = new CSakiPlayer(playerId, pVisitor);
			m_players[playerId] = player;

			InitializePlayer(player);
		}
	}

	return player;
}


void CSakiServerEngine::InitializePlayer(CSakiPlayer* player)
{
	player->Initialize();

	switch (player->PlayerId())
	{
	case 1:
		player->SetCurrent(m_board.Width() / 4, m_board.Height() / 2);
		break;
	case 2:
		player->SetCurrent(m_board.Width() * 3 / 4, m_board.Height() / 2);
		break;
	case 3:
		player->SetCurrent(m_board.Width() / 2, m_board.Height() / 4);
		break;
	case 4:
		player->SetCurrent(m_board.Width() / 2, m_board.Height() * 3 / 4);
		break;
	}

	auto current = m_board.Width() * player->GetY() + player->GetX();
	m_board.SetCell(current, player->PlayerId());
}


void CSakiServerEngine::BroadcastBoard()
{
	for (auto x : m_players)
	{
		auto msg = SakiSnakeCommon::Msg<SakiSnakeCommon::msgUpdateBoard, SakiSnakeCommon::UpdateBoardMsg>();
		msg._header._length = static_cast<short>(m_board.CellCount());
		int size = sizeof(msg._header) + m_board.CellCount();
		x.second->Send((unsigned char*)&msg._header, sizeof(msg._header));
		x.second->Send(m_board.Cells(), m_board.CellCount());
	}
}
