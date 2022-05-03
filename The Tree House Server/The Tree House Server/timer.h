#pragma once
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

enum EVENT_TYPE { NONE = 0, EV_SEND_PLAYER_DATA };

class cTimerEvent
{
public:
	cTimerEvent() {};
	~cTimerEvent() {};

	void Init();

	constexpr bool operator < (const cTimerEvent& _Left) const
	{
		return (m_excute_time > _Left.m_excute_time);
	}

public:
	unsigned int m_room_id;
	unsigned int m_obj_id;
	high_resolution_clock::time_point m_excute_time;
	EVENT_TYPE m_event_type;
};


/////////////////////////////////////
/////////////////////////////////////

class cTimer
{
public:
	cTimer() ;
	~cTimer();
	
	void Init();
	void UpdateCurrentTime();
	chrono::high_resolution_clock::time_point GetTime();
	float GetDeltaTimeSecond();

private:
	high_resolution_clock::time_point m_started_time;
	high_resolution_clock::time_point m_current_time;
};