#pragma once
#include <vector>
#include <thread>
#include "ExpOver.h"


class CIOCPServer;
class GameProcessor;
class CUserManager;
class CRoomManager;

class ServerNetworkSystem
{
public:

	ServerNetworkSystem();
	~ServerNetworkSystem();

	void Init();
	void Open();
	void WorkerThread();
	void ProcessPacket(const unsigned int _user_id, unsigned char* p);

	void Accept(CEXP_OVER* exp_over);
	void Send(CEXP_OVER* exp_over);
	void Recv(CEXP_OVER* exp_over, const unsigned int _user_id, const DWORD num_byte);

private:

	CIOCPServer* m_IOCP_server;
	GameProcessor* m_game_processor;
	CUserManager* m_user_manager;
	CRoomManager* m_room_manager;
	std::vector <std::thread> m_worker_threads;

};
