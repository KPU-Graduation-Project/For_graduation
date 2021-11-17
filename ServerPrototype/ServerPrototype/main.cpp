#pragma once
#include "client.h"
#include "exp_over.h"
#include "IOCP_server.h"
#include "matching_room.h"
#include "protocol.h"

#pragma comment (lib, "WS2_32.LIB")
#pragma comment (lib, "MSWSock.LIB")


int main()
{
	wcout.imbue(locale("korean"));
	CIOCPServer* IOCP_server=new CIOCPServer;

	IOCP_server->Init(SERVER_PORT);
	IOCP_server->Release();
}