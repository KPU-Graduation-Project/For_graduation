#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "ExpOver.h"


class cIOCPBase
{
public:
	cIOCPBase();
	~cIOCPBase();

public:
	void StartServer();
	void CloseServer();
	

	void Disconnect();

private:
	void BindAndListen(USHORT server_port);

public:
	HANDLE m_h_IOCP;
	SOCKET m_listen_socket;
	SOCKET m_client_socket;
	cExpOver m_accept_over;

};