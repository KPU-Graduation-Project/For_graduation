#pragma once
#include "stdafx.h"
#include "UserManager.h"
#include <MSWSock.h>
#include "User.h"


void cUserManager::Init()
{
	m_connected_user_num = 0;
	m_users = new cUser[MAX_USER];
	this->InitUsers();
};

void cUserManager::InitUsers()
{

	for (int i = 0; i < MAX_USER; ++i)
	{
		m_users[i].Init(i);
	}
}

// return user's new id. If server is full, return MAX_USER
unsigned short cUserManager::GetNewID() 
{
	static unsigned short last_id = 0;

	// 현재 접속중인 인원이 MAX면 바로 리턴 필요
	/*
	if()
	{

	return MAX_USER
	}
	*/

	char cnt = 0;
	char current_id = last_id;
	while (cnt < MAX_USER)
	{
		// 스레드 동기화 필요
		if (m_users[current_id].GetState() == user_state::FREE)
		{
			m_users[current_id].SetState(user_state::IN_ROBBY);

			last_id = (current_id + 1 < MAX_USER) ? current_id + 1 : 0;
			return current_id;
		}
		if (++current_id == MAX_USER) current_id = 0;
		++cnt;
	}
	return MAX_USER;
}


cUserManager::cUserManager() { m_connected_user_num = 0; };
cUserManager::~cUserManager() {};