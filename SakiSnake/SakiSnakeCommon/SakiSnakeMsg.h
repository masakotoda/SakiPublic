#pragma once


#pragma pack(1)

namespace SakiSnakeCommon
{
	const int maxPlayers = 4;
	const int minPlayers = 2;

	const int maxCols = 80;
	const int maxRows = 60;

	enum PlayerId
	{
		noPlayer,
		player1,
		player2,
		player3,
		player4,
	};

	enum Direction
	{
		directionNorth,
		directionSouth,
		directionEast,
		directionWest,
	};

	enum MsgId
	{
		// Messages from server to client
		msgHello,
		msgJoined,
		msgUpdateBoard,
		msgCountDown,
		msgGameOver,

		// Messages from client to server
		msgSetReady,
		msgSetDirection,
	};

	struct MsgHeader
	{
		unsigned char _id;
		short _length;
	};

	template <unsigned char id, class T> struct Msg
	{
		Msg()
		{
			_header._id = id;
			_header._length = sizeof(T);
		}

		Msg(unsigned char* data, int length)
		{
			if (length >= sizeof(_header))
			{
				_header = *(reinterpret_cast<MsgHeader*>(data));
				_data = T(data + sizeof(_header), length - sizeof(_header));
			}
		}

		MsgHeader _header;
		T _data;
	};

	struct HelloMsg
	{
		HelloMsg()
		{
			memset(this, 0, sizeof(*this));
		}

		HelloMsg(unsigned char* data, int length)
		{
			if (length == sizeof(*this))
			{
				*this = *(reinterpret_cast<HelloMsg*>(data));
			}
			else
			{
				ASSERT(false);
			}
		}

		unsigned char _playerId;
		int _width;
		int _height;
		float _r;
		float _g;
		float _b;
	};

	struct JoinedMsg
	{
		JoinedMsg()
		{
			memset(this, 0, sizeof(*this));
		}

		JoinedMsg(unsigned char* data, int length)
		{
			if (length == sizeof(*this))
			{
				*this = *(reinterpret_cast<JoinedMsg*>(data));
			}
			else
			{
				ASSERT(false);
			}
		}

		unsigned char _playerId;
		float _r;
		float _g;
		float _b;
	};

	struct UpdateBoardMsg
	{
		UpdateBoardMsg()
		{
		}

		UpdateBoardMsg(unsigned char* data, int length)
		{
			_cells.resize(length);
			if (length > 0)
			{
				memmove(&_cells[0], data, length);
			}
		}

		std::vector<unsigned char> _cells;
	};

	struct CountDownMsg
	{
		CountDownMsg()
		{
		}

		CountDownMsg(unsigned char* data, int length)
		{
			if (length == sizeof(*this))
			{
				*this = *(reinterpret_cast<CountDownMsg*>(data));
			}
			else
			{
				ASSERT(false);
			}
		}

		unsigned char _second;
	};

	struct GameOverMsg
	{
		GameOverMsg()
		{
		}

		GameOverMsg(unsigned char* data, int length)
		{
			if (length == sizeof(*this))
			{
				*this = *(reinterpret_cast<GameOverMsg*>(data));
			}
			else
			{
				ASSERT(false);
			}
		}

		unsigned char _winner;
	};

	struct SetReadyMsg
	{
		SetReadyMsg()
		{
		}

		SetReadyMsg(unsigned char* data, int length)
		{
			if (length == sizeof(*this))
			{
				*this = *(reinterpret_cast<SetReadyMsg*>(data));
			}
			else
			{
				ASSERT(false);
			}
		}

		unsigned char dummy;
	};

	struct SetDirectionMsg
	{
		SetDirectionMsg()
		{
		}

		SetDirectionMsg(unsigned char* data, int length)
		{
			if (length == sizeof(*this))
			{
				*this = *(reinterpret_cast<SetDirectionMsg*>(data));
			}
			else
			{
				ASSERT(false);
			}
		}

		unsigned char _direction;
	};
}

#pragma pack()