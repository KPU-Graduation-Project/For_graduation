#pragma once
#include "stdafx.h"
#include "ServerNetworkSystem.h"
#include "IOCPServer.h"

#pragma comment (lib, "WS2_32.LIB")
#pragma comment (lib, "MSWSock.LIB")

ServerNetworkSystem* g_server = nullptr;

int main()
{
	g_server = new ServerNetworkSystem;
	g_server->Init();
	g_server->Open();
}