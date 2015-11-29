#pragma once

class ISakiClientEngine
{
public:
	virtual void OnReceive(unsigned char* data, int length) = 0;
	virtual void OnClose() = 0;
};
