#pragma once

#include "ISakiServerEngine.h"


namespace SakiSnakeCommon
{
	struct SetReadyMsg;
	struct SetDirectionMsg;
};


class CSakiPlayer
{
public:
	CSakiPlayer(unsigned char playerId, CAsyncSocket* pSocket);

	void Initialize();
	bool IsMe(CAsyncSocket* pSocket) const;
	void SetDirection(int direction);
	void SetReady(bool ready);
	bool IsReady() const;
	int PlayerId() const;
	D2D1::ColorF Color() const;

	void Move();
	void Remove();
	void SetCurrent(int x, int y);
	int GetX() const;
	int GetY() const;
	int NextX() const;
	int NextY() const;
	
	bool Send(unsigned char* data, int length);
	template <class T> bool Send(T* data)
	{
		return Send(reinterpret_cast<unsigned char*>(data), sizeof(T));
	}

private:
	int m_playerId = 0;
	int m_x = 0;
	int m_y = 0;
	bool m_ready = false;
	CAsyncSocket* const m_pSocket;
	int m_direction = 0;
	D2D1::ColorF m_color = D2D1::ColorF::AliceBlue;
};


class CSakiBoard
{
public:
	void SetSize(int width, int height);
	int GetCellIndex(int x, int y);
	void Clear(unsigned char playerId);
	void Clear();
	bool InRange(int x, int y);
	bool IsOccupied(int x, int y);
	int Width() const;
	int Height() const;
	unsigned char GetCell(int index) const;
	void SetCell(int index, unsigned char val);
	size_t CellCount() const;
	unsigned char* Cells();

private:
	std::vector<unsigned char> m_cells;
	int m_width;
	int m_height;
};


class CSakiServerEngine : public ISakiServerEngine
{
public:
	enum GameState
	{
		stateNone,
		stateAccepting,
		stateCountDown,
		stateRunning,
		stateGameOver,
	};

	CSakiServerEngine(int width, int height);
	virtual ~CSakiServerEngine();

	void Tick();

	virtual void OnClose(CAsyncSocket* pVisitor) override;
	virtual bool OnAccepted(CAsyncSocket* pVisitor) override;
	virtual void OnReceive(CAsyncSocket* pVisitor, unsigned char* data, int length) override;

private:
	void HandleStateNone();
	void HandleStateAccepting();
	void HandleStateCountDown();
	void HandleStateRunning();
	void HandleStateGameOver();

	void HandleMsg(CSakiPlayer* pPlayer, SakiSnakeCommon::SetReadyMsg& msg);
	void HandleMsg(CSakiPlayer* pPlayer, SakiSnakeCommon::SetDirectionMsg& msg);

	void BroadcastBoard();
	CSakiPlayer* JoinGame(CAsyncSocket* pVisitor);
	CSakiPlayer* GetPlayer(CAsyncSocket* pVisitor);
	
	void InitializePlayer(CSakiPlayer* player);

private:
	std::map<unsigned char, CSakiPlayer*> m_players;
	std::set<unsigned char> m_goners;
	unsigned __int64 m_startCounting = 0;
	unsigned __int64 m_startEnding = 0;
	CSakiBoard m_board;
	GameState m_state = stateNone;
};
