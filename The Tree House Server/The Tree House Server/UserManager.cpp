#pragma once
#include "stdafx.h"
#include "UserManager.h"
#include <MSWSock.h>
#include "User.h"

cUserManager::cUserManager() { m_last_id = 0; };
cUserManager::~cUserManager() {};

void cUserManager::Init()
{
	m_last_id = 0;
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
unsigned short cUserManager::GetNewID()
{
	m_last_id_lock.lock();
	if (m_last_id < MAX_USER)
	{
		unsigned short new_id = m_last_id;
		++m_last_id;
		m_last_id_lock.unlock();

		m_users.emplace(new_id, new cUser(new_id));
		m_users[new_id]->SetState(user_state::ACCEPTED);
		return new_id;
	}
	else
	{
		m_last_id_lock.unlock();
		return MAX_USER;
	}

}


