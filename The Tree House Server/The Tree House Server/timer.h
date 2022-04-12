#pragma once
#include <iostream>
#include <chrono>

using namespace chrono;

enum EVENT_TYPE {};

class cTimerEvent
{
public:
	cTimerEvent();
	~cTimerEvent();

	void Init();

	constexpr bool operator < (const cTimerEvent& _Left) const;

private:
	unsigned int m_obj_id;
	high_resolution_clock::time_point m_start_time;
	EVENT_TYPE m_event_type;
};


/////////////////////////////////////

class cTimer
{
public:
	cTimer() ;
	~cTimer();
	
	void Init();
	void UpdateCurrentTime();
	high_resolution_clock::time_point GetTime();
	float GetDeltaTimeSecond();

private:
	high_resolution_clock::time_point m_started_time;
	high_resolution_clock::time_point m_current_time;
};