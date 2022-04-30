#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <concurrent_priority_queue.h>
#include <unordered_map>
#include "IOCPBase.h"
#include "ExpOver.h"



class GameProcessor;
class cUserManager;
class cRoomManager;
class cTimer;
class cTimerEvent;


class cGameServer:public cIOCPBase
{
public:

	cGameServer();
	~cGameServer();

	void Init();
	void Start();
	void WorkerThread();
	void TimerThread();
	void ProcessPacket(const unsigned int _user_id, unsigned char* p);

	void Accept(CEXP_OVER* exp_over);
	void Send(CEXP_OVER* exp_over);
	void Recv(CEXP_OVER* exp_over, const unsigned int _user_id, const DWORD num_byte);
	void Disconnect();

private:
	static cUserManager*  m_user_manager;
	static cRoomManager*  m_room_manager;
	static GameProcessor* m_game_processor;

	cTimer* m_clock;
	std::vector <std::thread> m_worker_threads;
	thread           m_timer_thread;


};
