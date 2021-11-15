#pragma once
#include "client.h"

void CClient::DoRecv()
{
	DWORD recv_flag = 0;
	ZeroMemory(&m_recv_over.m_wsa_over, sizeof(m_recv_over.m_wsa_over));
	m_recv_over.m_wsa_buf.buf = reinterpret_cast<char*>(m_recv_over.m_net_buf + m_prev_size);
	m_recv_over.m_wsa_buf.len = sizeof(m_recv_over.m_net_buf) - m_prev_size;
	int ret = WSARecv(m_socket, &m_recv_over.m_wsa_buf, 1, 0, &recv_flag, &m_recv_over.m_wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			ErrorDisplay(error_num);
	}
}

void CClient::DoSend(int num_bytes, void* mess)
{
	CEXP_OVER* ex_over = new CEXP_OVER(OP_SEND, num_bytes, mess);
	WSASend(m_socket, &ex_over->m_wsa_buf, 1, 0, 0, &ex_over->m_wsa_over, NULL);
}

void CClient::CloseSocket()
{
	closesocket(m_socket);
}

void CClient::SetState(STATE state)
{
	m_state = state;
};

void CClient::SetID(const int id) 
{
	m_id = id;
};

void CClient::SetRoomNum(const short room_num)
{
	m_room_num = room_num;
};

void CClient::SetChar(const short room_num)
{
	m_is_char1 = room_num;
};   

void CClient::SetPrevSize(const int prev_size)
{
	m_prev_size = prev_size;
};

void CClient::SetSocket(const SOCKET& socket)
{
	m_socket = socket;
}
void CClient::InitRecvOver()
{
	m_recv_over.m_comp_op = OP_RECV;
	m_recv_over.m_wsa_buf.buf = reinterpret_cast<char*>(m_recv_over.m_net_buf);
	m_recv_over.m_wsa_buf.len = sizeof(m_recv_over.m_net_buf);
	ZeroMemory(&m_recv_over.m_wsa_over, sizeof(m_recv_over.m_wsa_over));
}


short CClient::GetRoomNum()
{
	return m_room_num;
};

bool CClient::GetChar()
{
	return m_is_char1; // true: 1P  false: 2P
};             

STATE CClient::GetState() 
{
	return m_state;
};

int  CClient::GetID() 
{
	return m_id;
};

int CClient::GetPrevSize()
{
	return m_prev_size;
}

void CClient::Lock()
{
	m_state_lock.lock();
}

void CClient::Unlock()
{
	m_state_lock.unlock();
}


explicit CClient::CClient()
{
	m_state = ST_FREE;
	m_prev_size = 0;
};
explicit CClient::CClient(const char* name, int id)
{
	m_state = ST_FREE;
	m_prev_size = 0;
	strcpy(m_name, name);
	m_id = id;
}
CClient::~CClient()
{
	closesocket(m_socket);
};


void CClient::ErrorDisplay(int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	std::wcout << lpMsgBuf << std::endl;
	while (true);
	LocalFree(lpMsgBuf);
};
