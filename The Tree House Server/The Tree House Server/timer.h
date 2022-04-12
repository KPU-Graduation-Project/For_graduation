#pragma once
#include <iostream>
#include <chrono>

using namespace chrono;

enum EVENT_TYPE {};

class CTimerEvent
{
public:
	CTimerEvent();
	~CTimerEvent();

	void Init();

	constexpr bool operator < (const CTimerEvent& _Left) const;

private:
	unsigned int m_obj_id;
	high_resolution_clock::time_point m_start_time;
	EVENT_TYPE m_event;
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