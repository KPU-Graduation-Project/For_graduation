#pragma once
#include "stdafx.h"
#include "UserManager.h"
#include <MSWSock.h>
#include "User.h"

 unordered_map <unsigned int, UserRef>cUserManager::m_users;


void cUserManager::Init()
{
	
	//m_users = new cUser[MAX_USER];
	//this->InitUsers();
};

void cUserManager::InitUsers()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		//m_users[i].Init(i);
	}
}

// return user's new id. If server is full, return MAX_USER
unsigned int cUserManager::GetNewID()
{
	static std::atomic<unsigned int>  last_id=0;

	if (last_id < MAX_USER)
	{
		unsigned short new_id = last_id;
		++last_id;

		m_users.emplace(new_id, new cUser(new_id));
		m_users[new_id]->SetState(user_state::ACCEPTED);
		return new_id;
	}
	else
	{
		return MAX_USER;
	}

}


