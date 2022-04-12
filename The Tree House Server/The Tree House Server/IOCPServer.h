#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "ExpOver.h"


class cIOCPServer
{
public:
	cIOCPServer();
	~cIOCPServer();

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
	CEXP_OVER m_accept_over;

};