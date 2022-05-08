#include "stdafx.h"
#include "Monster.h"




void cPlayZoneTarget::ChangeBehavior(unsigned char next_state)
{


};

void cPlayZoneTarget::ProgressNextBehavior()
{
	/*if (++m_behavior == BEHAVIOR::MAX)
		m_behavior = 1;*/
	std::cout << "progress\n";
	
		// use switch case if many events are existed that have to be tiggered when the behavior is changed
	switch (m_behavior)
	{
	case BEHAVIOR::NONE:
	{
		cout << "Object ID [ " << m_id << " ] have to change behavior first before progress next behavior\n";
		break;
	}

	case BEHAVIOR::BH1_NONE:
	{
		ClearDeltaTransform();
		SetDeltaPosition({ 0,-100000 / 2,0 });
		++m_behavior;	
		
		g_timer_queue.push(cTimerEvent{ m_id ,chrono::system_clock::now() + chrono::milliseconds(2000),EVENT_TYPE::PROGRESS_BEHAVIOR_EVENT });
		break;
	}
	// 타이머 이벤트 수정 필요 - 타이머 이벤트 풀 사용 및 함수화
	case BEHAVIOR::BH1_MOVE_DOWN1:
	{
		ClearDeltaTransform();
		++m_behavior			;

		g_timer_queue.push(cTimerEvent{ m_id ,chrono::system_clock::now() + chrono::milliseconds(2000),EVENT_TYPE::PROGRESS_BEHAVIOR_EVENT });

		break;
	}
	case BEHAVIOR::BH1_HOLD1:
	{
		SetDeltaPosition({ 0,100000 / 2,0 });
		++m_behavior;

		g_timer_queue.push(cTimerEvent{ m_id ,chrono::system_clock::now() + chrono::milliseconds(2000),EVENT_TYPE::PROGRESS_BEHAVIOR_EVENT });
		break;
	}
	case BEHAVIOR::BH1_MOVE_UP1:
	{
		ClearDeltaTransform();
		++m_behavior;

		g_timer_queue.push(cTimerEvent{ m_id ,chrono::system_clock::now() + chrono::milliseconds(5000),EVENT_TYPE::PROGRESS_BEHAVIOR_EVENT });
		break;
	}
	case BEHAVIOR::BH1_HOLD2:
	{
		SetDeltaPosition({ 0,-100000 / 2,0 });

		g_timer_queue.push(cTimerEvent{ m_id ,chrono::system_clock::now() + chrono::milliseconds(2000),EVENT_TYPE::PROGRESS_BEHAVIOR_EVENT });
		break;
	}


	case BEHAVIOR::BH2_NONE:
	{
		ClearDeltaTransform();
		SetDeltaRotation({ -24000/8,0,0 });
		++m_behavior;

		g_timer_queue.push(cTimerEvent{ m_id ,chrono::system_clock::now() + chrono::milliseconds(8000),EVENT_TYPE::PROGRESS_BEHAVIOR_EVENT });
		break;
	}
	case BEHAVIOR::BH2_ROTATE_PITCH_DOWN1:
	{
		//ClearDeltaTransform();
		SetDeltaRotation({ 24000 / 8,0,0 });
		++m_behavior;

		g_timer_queue.push(cTimerEvent{ m_id ,chrono::system_clock::now() + chrono::milliseconds(8000),EVENT_TYPE::PROGRESS_BEHAVIOR_EVENT });
		break;
	}
	case BEHAVIOR::BH2_ROTATE_PITCH_UP1:
	{
		//ClearDeltaTransform();
		SetDeltaRotation({ -24000 / 8,0,0 });
		//++m_behavior;
		m_behavior = BEHAVIOR::BH2_ROTATE_PITCH_DOWN1;

		g_timer_queue.push(cTimerEvent{ m_id ,chrono::system_clock::now() + chrono::milliseconds(8000),EVENT_TYPE::PROGRESS_BEHAVIOR_EVENT });
		break;
	}	
	}
};

void  cPlayZoneTarget::Update(float _delta_second)
{
	std::cout << "update\n";
	switch (m_behavior)
	{
	case BEHAVIOR::BH1_MOVE_DOWN1:
	{
		m_transform += m_delta_transform * _delta_second;
		break;
	}
	case BEHAVIOR::BH1_MOVE_UP1:
	{
		m_transform += m_delta_transform * _delta_second;
		break;
	}
	case BEHAVIOR::BH2_ROTATE_PITCH_DOWN1:
	{
		m_transform += m_delta_transform * _delta_second;
		++m_behavior;
		break;
	}
	case BEHAVIOR::BH2_ROTATE_PITCH_UP1:
	{
		m_transform += m_delta_transform * _delta_second;
		++m_behavior;
		break;
	}
	}
};