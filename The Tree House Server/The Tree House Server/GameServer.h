#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <concurrent_priority_queue.h>
#include <unordered_map>
#include "IOCPBase.h"
#include "ExpOver.h"


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

	void Accept(cExpOver* exp_over);
	void Send(cExpOver* exp_over);
	void Recv(cExpOver* exp_over, const unsigned int _user_id, const DWORD num_byte);
	void Disconnect(const unsigned int _user_id);

private:
	static cUserManager*  m_user_manager;

	cTimer* m_clock;
	std::vector <std::thread> m_worker_threads;
	thread           m_timer_thread;


};
