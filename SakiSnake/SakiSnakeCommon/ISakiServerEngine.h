#pragma once

class ISakiServerEngine
{
public:
	virtual void OnClose(CAsyncSocket* pVisitor) = 0;
	virtual bool OnAccepted(CAsyncSocket* pVisitor) = 0;
	virtual void OnReceive(CAsyncSocket* pVisitor, unsigned char* data, int length) = 0;
};
