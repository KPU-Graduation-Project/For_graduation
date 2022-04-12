#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <unordered_map>
#include "IOCPServer.h"
#include "ExpOver.h"
#include "UserManager.h"


class GameProcessor;
class cUserManager;
class cRoomManager;
class cTimer;
class cTimerEvent;
class cUser;

class cMainServer:public cIOCPServer
{
public:

	cMainServer();
	~cMainServer();

	void Init();
	void Start();
	void WorkerThread();
	void TimerThread();
	void ProcessPacket(const unsigned short _user_id, unsigned char* p);

	void Accept(CEXP_OVER* exp_over);
	void Send(CEXP_OVER* exp_over);
	void Recv(CEXP_OVER* exp_over, const unsigned short _user_id, const DWORD num_byte);
	void Disconnect();

private:
	cUserManager*    m_user_manager;
	cRoomManager*    m_room_manager;

	GameProcessor*   m_game_processor;
	
	cTimer*          m_clock;
	std::vector <std::thread> m_worker_threads;
	thread           m_timer_thread;

	std::priority_queue<cTimerEvent> m_timer_queue;
};
