#pragma once
#include "stdafx.h"
#include "UserManager.h"
#include <MSWSock.h>
#include "User.h"


void CUserManager::Init()
{
	m_users = new CUser[MAX_USER];
	m_connected_user_num = 0;
};

// return user's new id. If server is full, return MAX_USER
unsigned int CUserManager::GetNewID() 
{
	static unsigned int last_assigned_user_id = 0;	

	if (m_connected_user_num == MAX_USER)return MAX_USER;

	char cnt = 0;
	char i = last_assigned_user_id;
	do
	{
		if (m_users[i].m_state == E_USER_STATE::FREE)
		{
			m_users[i].m_state=E_USER_STATE::IN_ROBBY;
			m_connected_user_num++;
			std::cout << "new id assigned : " << (int)i << std::endl;
			return i;
		}

		if (i < MAX_USER - 1)++i;
		else i = 0;

	}while(cnt++<MAX_USER);
}

void CUserManager::AddNewUser(const unsigned int& _user_id, SOCKET& _socket)
{	
	this->UserInit(_user_id);
	m_users[_user_id].m_socket = _socket;
}

void CUserManager::UserInit(const unsigned int& _user_id )
{
	m_users[_user_id].m_id = _user_id;
	m_users[_user_id].m_prev_size = 0;

	CEXP_OVER& recv_over = m_users[_user_id].m_recv_over;
	recv_over.m_comp_op = OP_RECV;
	recv_over.m_wsa_buf.buf = reinterpret_cast<char*>(recv_over.m_net_buf);
	recv_over.m_wsa_buf.len = sizeof(recv_over.m_net_buf);
	ZeroMemory(&recv_over.m_wsa_over, sizeof(recv_over.m_wsa_over));
}

void CUserManager::UserSend(const unsigned int& _user_id, int num_bytes, void* mess)
{
	CEXP_OVER* ex_over = new CEXP_OVER(OP_SEND, num_bytes, mess);
	WSASend(m_users[_user_id].m_socket, &ex_over->m_wsa_buf, 1, 0, 0, &ex_over->m_wsa_over, NULL);
}

void CUserManager::UserRecv(const unsigned int& _user_id)
{
	CEXP_OVER& recv_over = m_users[_user_id].m_recv_over;
	DWORD recv_flag = 0;

	ZeroMemory(&recv_over.m_wsa_over, sizeof(recv_over.m_wsa_over));
	recv_over.m_wsa_buf.buf = reinterpret_cast<char*>(recv_over.m_net_buf + m_users[_user_id].m_prev_size);
	recv_over.m_wsa_buf.len = sizeof(recv_over.m_net_buf) - m_users[_user_id].m_prev_size;
	int ret = WSARecv(m_users[_user_id].m_socket, &recv_over.m_wsa_buf, 1, 0, &recv_flag, &recv_over.m_wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		/*if (ERROR_IO_PENDING != error_num)
			ErrorDisplay(error_num);*/
	}
};

int CUserManager::GetPrevSize(const unsigned int& _user_id)
{
	return m_users[_user_id].m_prev_size;
}

void CUserManager::SetPrevSize(const unsigned int& _user_id, int& _prev_size)
{
	m_users[_user_id].m_prev_size = _prev_size;
}

CUserManager::CUserManager() { };
CUserManager::~CUserManager() {};