#pragma once
#include "stdafx.h"
#include "GameServer.h"
#include "IOCPBase.h"

#pragma comment (lib, "WS2_32.LIB")
#pragma comment (lib, "MSWSock.LIB")

cGameServer* g_server = nullptr;

int main()
{
	g_server = new cGameServer;
	g_server->Init();
	g_server->Start();
}