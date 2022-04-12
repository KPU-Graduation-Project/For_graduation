#pragma once
#include "stdafx.h"
#include "User.h"

cUser::cUser()
{
	m_id = MAX_USER;
	m_room_id = MAX_ROOM;
	m_prev_size = 0;
	m_state = user_state::FREE;

	m_recv_over.m_comp_op = OP_RECV;
	m_recv_over.m_wsa_buf.buf = reinterpret_cast<char*>(m_recv_over.m_net_buf);
	m_recv_over.m_wsa_buf.len = sizeof(m_recv_over.m_net_buf);
	ZeroMemory(&m_recv_over.m_wsa_over, sizeof(m_recv_over.m_wsa_over));
};

cUser::cUser(unsigned short _id)
{
	m_id = _id;
	m_room_id = MAX_ROOM;
	m_prev_size = 0;
	m_state = user_state::FREE;

	m_recv_over.m_comp_op = OP_RECV;
	m_recv_over.m_wsa_buf.buf = reinterpret_cast<char*>(m_recv_over.m_net_buf);
	m_recv_over.m_wsa_buf.len = sizeof(m_recv_over.m_net_buf);
	ZeroMemory(&m_recv_over.m_wsa_over, sizeof(m_recv_over.m_wsa_over));
};

cUser::~cUser() { cout << "delete user" << endl; };


void cUser::Init(const unsigned short& _id)
{
	m_id = _id;
	m_room_id = MAX_ROOM;
	m_prev_size = 0;
	m_state = user_state::FREE;



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
void cUser::Send(int num_bytes, void* mess)
{
	CEXP_OVER* ex_over = new CEXP_OVER(OP_SEND, num_bytes, mess);
	int ret = WSASend(m_socket, &ex_over->m_wsa_buf, 1, 0, 0, &ex_over->m_wsa_over, NULL);

	/*
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		/if (ERROR_IO_PENDING != error_num)
			ErrorDisplay(error_num);
	}
	*/
}



