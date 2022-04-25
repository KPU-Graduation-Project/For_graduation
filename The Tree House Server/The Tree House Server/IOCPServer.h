#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "ExpOver.h"


class CIOCPServer
{
public:
	CIOCPServer();
	~CIOCPServer();

public:
	void Open();
	void Close();
	

	void Disconnect();

private:
	void BindAndListen(USHORT server_port);

public:
	SOCKET m_listen_socket;
	HANDLE m_h_iocp;

};