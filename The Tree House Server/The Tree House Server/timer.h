#pragma once
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

enum EVENT_TYPE { NONE = 0,  TICK_EVENT,PROGRESS_BEHAVIOR_EVENT };

//class cTimerEvent
//{
//public:
//	cTimerEvent() {};
//	~cTimerEvent() {};
//
//	void Init();
//
//	constexpr bool operator < (const cTimerEvent& _Left) const
//	{
//		return (m_excute_time > _Left.m_excute_time);
//	}
//
//	void SetExcuteTimeMS(const unsigned int& _excute_time){		m_excute_time = system_clock::now() + milliseconds(_excute_time);		}
//
//public:
//	unsigned int m_obj_id;
//	system_clock::time_point m_excute_time;
//	EVENT_TYPE m_event_type;
//};
struct cTimerEvent
{
	constexpr bool operator < (const cTimerEvent& _Left) const
	{
		return (m_excute_time > _Left.m_excute_time);
	}

	unsigned int m_obj_id;
	system_clock::time_point m_excute_time;
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
	chrono::system_clock::time_point GetTime();
	float GetDeltaTimeSecond();

private:
	system_clock::time_point m_started_time;
	system_clock::time_point m_current_time;
};