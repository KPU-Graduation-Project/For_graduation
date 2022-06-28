#pragma once
#include "stdafx.h"
#include "User.h"

cUser::cUser()
{
	this->Init();
};

cUser::cUser(unsigned int _id)
{
	m_id = _id;	
	this->Init();
};

cUser::~cUser() { cout << "delete user" << endl; };


void cUser::Init()
{
	m_state = user_state::FREE;
	m_character = nullptr;
	m_room = nullptr;

	m_prev_size = 0;
	m_recv_over.m_comp_op = OP_RECV;
	m_recv_over.m_wsa_buf.buf = reinterpret_cast<char*>(m_recv_over.m_net_buf);
	m_recv_over.m_wsa_buf.len = sizeof(m_recv_over.m_net_buf);
	ZeroMemory(&m_recv_over.m_wsa_over, sizeof(m_recv_over.m_wsa_over));
}

void cUser::Recv()
{
	DWORD recv_flag = 0;

	ZeroMemory(&m_recv_over.m_wsa_over, sizeof(m_recv_over.m_wsa_over));
	m_recv_over.m_wsa_buf.buf = reinterpret_cast<char*>(m_recv_over.m_net_buf + m_prev_size);
	m_recv_over.m_wsa_buf.len = sizeof(m_recv_over.m_net_buf) -m_prev_size;
	int ret = WSARecv(m_socket, &m_recv_over.m_wsa_buf, 1, 0, &recv_flag, &m_recv_over.m_wsa_over, NULL);
	
	/*
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			ErrorDisplay(error_num);
	}
	*/
}
void cUser::Send(int _size, void* _mess)
{
	cExpOver* ex_over = new cExpOver(OP_SEND, _size, _mess);
	int ret = WSASend(m_socket, &ex_over->m_wsa_buf, 1, 0, 0, &ex_over->m_wsa_over, NULL);

	/*
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		/if (ERROR_IO_PENDING != error_num)
			ErrorDisplay(error_num);
	}
	*/
}

void cUser::Disconnect()
{
	/*reinterpret_cast<UserRef>(this)
	if (m_room != nullptr)
	{
		m_room->Disconnect(reinterpret_cast<UserRef>(this));

		
	}
	StateLock();
	m_state = user_state::FREE;
	StateUnlock();*/

}

