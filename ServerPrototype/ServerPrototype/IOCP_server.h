#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <thread>
#include <vector>
#include <array>
#include "protocol.h"
#include "exp_over.h"
#include "client.h"
#include "matching_room.h"

constexpr const int MAX_CLIENTS = 10;
constexpr const int MAX_MATCHINGROOM = MAX_CLIENTS / 2;
constexpr const int MAX_WORKER_THREADS = 6;

class CIOCPServer
{
public:
	void Init(const short server_port);
	void Release();
	void Close();

	void WorkerThread();
	void DoAccept(CEXP_OVER* exp_over);
	void DoSend(CEXP_OVER* exp_over);
	void DoRecv(CEXP_OVER* exp_over, const int client_id, const DWORD num_byte);
	void DoDisconnect(const int client_id);
	void ProcessPacket(const int client_id, unsigned char* packet_start);

	void ErrorDisplay(int err_no);
	int GetNewID();	

	explicit CIOCPServer();
	~CIOCPServer();

private:
	SOCKET m_listen_socket;
	HANDLE m_h_iocp;
		
	array <CClient, MAX_CLIENTS> m_clients;
	vector <thread> m_worker_threads;
};

