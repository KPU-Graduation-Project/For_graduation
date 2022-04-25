#include "stdafx.h"
#include "timer.h"

using namespace chrono;


void cTimerEvent::Init() {};




///////////////////////////////////////////////////

cTimer::cTimer() {};
cTimer::~cTimer() {};


void cTimer::Init()
{
	m_started_time =high_resolution_clock::now();
}

void cTimer::UpdateCurrentTime()
{
	m_current_time = high_resolution_clock::now();
}

high_resolution_clock::time_point cTimer::GetTime()
{
	return m_current_time;
}

float cTimer::GetDeltaTimeSecond()
{

	return duration_cast<seconds>(high_resolution_clock::now() - m_current_time).count();
}