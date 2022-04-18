#pragma once
#include "stdafx.h"
#include "UserManager.h"
#include <MSWSock.h>
#include "User.h"

 unordered_map <unsigned int, cUser*>cUserManager::m_users;

cUserManager::cUserManager() { m_last_id = 0; };
cUserManager::~cUserManager() {};

void cUserManager::Init()
{
	m_last_id = 0;
	InitializeCriticalSection(&m_last_id_cs);
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
	EnterCriticalSection(&m_last_id_cs);
	if (m_last_id < MAX_USER)
	{
		unsigned short new_id = m_last_id;
		++m_last_id;
		LeaveCriticalSection(&m_last_id_cs);

		m_users.emplace(new_id, new cUser(new_id));
		m_users[new_id]->SetState(user_state::ACCEPTED);
		return new_id;
	}
	else
	{
		LeaveCriticalSection(&m_last_id_cs);
		return MAX_USER;
	}

}


