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

	// ���� �������� �ο��� MAX�� �ٷ� ���� �ʿ�
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
		// ������ ����ȭ �ʿ�
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