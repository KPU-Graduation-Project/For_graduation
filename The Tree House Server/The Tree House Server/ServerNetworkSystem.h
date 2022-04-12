#pragma once
#include <vector>
#include <thread>
#include <concurrent_priority_queue.h>
#include <unordered_map>
#include "IOCPServer.h"
#include "ExpOver.h"
#include "UserManager.h"


class GameProcessor;
class cUserManager;
class cRoomManager;
class cTimer;
class CTimerEvent;
class cUser;

class cMainServer:public cIOCPServer
{
public:

	cMainServer();
	~cMainServer();

	void Init();
	void Start();
	void WorkerThread();
	//void TimerThread();
	void ProcessPacket(const unsigned short _user_id, unsigned char* p);

	void Accept(CEXP_OVER* exp_over);
	void Send(CEXP_OVER* exp_over);
	void Recv(CEXP_OVER* exp_over, const unsigned short _user_id, const DWORD num_byte);
	void Disconnect();

	unsigned short GetNewID();

private:
	//unordered_map    <int, cUser> m_users;
	cUserManager*    m_user_manager;
	cRoomManager*    m_room_manager;

	GameProcessor*   m_game_processor;
	
	cTimer*          m_clock;
	std::vector <std::thread> m_worker_threads;
	thread           m_timer_thread;

	//concurrency::concurrent_priority_queue<CTimerEvent> m_timer_queue;
};
