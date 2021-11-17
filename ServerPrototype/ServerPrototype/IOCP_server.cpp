#pragma once
#include "IOCP_server.h"

using namespace std;

void CIOCPServer::Init(const short server_port)
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	m_listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(m_listen_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
};

void CIOCPServer::Release()
{
	listen(m_listen_socket, SOMAXCONN);
	m_h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_listen_socket), m_h_iocp, 0, 0);

	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	char	accept_buf[sizeof(SOCKADDR_IN) * 2 + 32 + 100];
	CEXP_OVER	accept_ex;
	*(reinterpret_cast<SOCKET*>(&accept_ex.m_net_buf)) = c_socket;
	ZeroMemory(&accept_ex.m_wsa_over, sizeof(accept_ex.m_wsa_over));
	accept_ex.m_comp_op = COMP_OP(OP_ACCEPT);

	AcceptEx(m_listen_socket, c_socket, accept_buf, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &accept_ex.m_wsa_over);
	cout << "Accept Called\n";

	for (int i = 0; i < MAX_CLIENTS; ++i)
		m_clients[i].SetID(i);

	cout << "Creating Worker Threads\n";

	
	for (int i = 0; i < MAX_WORKER_THREADS; ++i)
		m_worker_threads.emplace_back(thread(&CIOCPServer::WorkerThread,this));
	for (auto& th : m_worker_threads)
		th.join();

};


void CIOCPServer::Close()
{


};

void CIOCPServer::WorkerThread()
{
	for (;;) {
		DWORD num_byte;
		LONG64 iocp_key;
		WSAOVERLAPPED* p_over;
		BOOL ret = GetQueuedCompletionStatus(m_h_iocp, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
		cout << "GQCS returned.\n";
		int client_id = static_cast<int>(iocp_key);
		CEXP_OVER* exp_over = reinterpret_cast<CEXP_OVER*>(p_over);
		if (FALSE == ret) {
			int err_no = WSAGetLastError();
			cout << "GQCS Error : ";
			ErrorDisplay(err_no);
			cout << endl;
			DoDisconnect(client_id);
			if (exp_over->m_comp_op == OP_SEND)
				delete exp_over;
			continue;
		}
		switch (exp_over->m_comp_op) {
		case OP_RECV: {
			DoRecv(exp_over, client_id, num_byte);
			break;
		}
		case OP_SEND: {
			if (num_byte != exp_over->m_wsa_buf.len) {
				DoDisconnect(client_id);
				delete exp_over;
				continue;
			}
			DoSend(exp_over);
			delete exp_over;
			break;
		}
		case OP_ACCEPT: {
			DoAccept(exp_over);
			break;
		}
		}
	}

};


void CIOCPServer::DoAccept(CEXP_OVER* exp_over)
{
	cout << "Accept Completed.\n";
	SOCKET c_socket = *(reinterpret_cast<SOCKET*>(exp_over->m_net_buf));
	int new_id = GetNewID();
	CClient& cl = m_clients[new_id];
	cl.SetID(new_id);
	cl.SetPrevSize(0);
	cl.InitRecvOver();
	cl.SetSocket(c_socket);

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), m_h_iocp, new_id, 0);
	cl.DoRecv();

	ZeroMemory(&exp_over->m_wsa_over, sizeof(exp_over->m_wsa_over));
	c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	*(reinterpret_cast<SOCKET*>(exp_over->m_net_buf)) = c_socket;
	AcceptEx(m_listen_socket, c_socket, exp_over->m_net_buf + 8, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &exp_over->m_wsa_over);
};

void CIOCPServer::DoSend(CEXP_OVER* exp_over)
{

};

void CIOCPServer::DoRecv(CEXP_OVER* exp_over, const short client_id, const DWORD num_byte)
{
	CClient& cl = m_clients[client_id];
	int remain_data = num_byte + cl.GetPrevSize();
	unsigned char* packet_start = exp_over->m_net_buf;
	int packet_size = packet_start[0];

	while (packet_size <= remain_data) {
		ProcessPacket(client_id, packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}

	if (0 < remain_data) {
		cl.SetPrevSize(remain_data);
		memcpy(&exp_over->m_net_buf, packet_start, remain_data);
	}
	cl.DoRecv();
};


void  CIOCPServer::SendLoginOKPacket(const short id)
{
	sc_packet_login_ok packet;
	packet.info.size = sizeof(packet);
	packet.info.size = sc_packet_login_ok;
	clients[c_id].do_send(sizeof(packet), &packet);

	sc_packet_login_ok packet;
	packet.id = c_id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN_OK;
	packet.x = clients[c_id].x;
	packet.y = clients[c_id].y;
	clients[c_id].do_send(sizeof(packet), &packet);
}

void  CIOCPServer::ProcessPacket(const short client_id, unsigned char* p)
{
	unsigned char packet_type = p[1];
	CClient& cl = m_clients[client_id];

	//프로토콜별 처리
	switch (packet_type) {
	case CS_PACKET_LOGIN:
	{
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(p);
		//로그인 가능한지 체크
		cl.SetName(packet->name);
		//
		short room_num = 0;   // 매칭룸 생성 전까지 0으로 설정
		bool is_char1 = true; // 일단 1번 캐릭터로 설정
		cl.InitPlayer();
		SendLoginOKPacket(client_id);
		break;
	}
	case CS_PACKET_MOVE:
	{

		break;
	}

	}
};

void CIOCPServer::DoDisconnect(const short client_id)
{
	m_clients[client_id].Lock();
	m_clients[client_id].CloseSocket();
	m_clients[client_id].SetState(ST_FREE);
	m_clients[client_id].Unlock();

	for (auto& cl : m_clients) {
		cl.Lock();
		if (ST_INGAME != cl.GetState()) {
			cl.Unlock();
			continue;
		}
		cl.Unlock();
		//send_remove_object(cl._id, client_id);
	}
};

int CIOCPServer::GetNewID()
{
	static int g_id = 0;

	for (int i = 0; i < MAX_CLIENTS; ++i) {
		m_clients[i].Lock();
		if (ST_FREE == m_clients[i].GetState()) {
			m_clients[i].SetState(ST_ACCEPT);
			m_clients[i].Unlock();
			return i;
		}
		m_clients[i].Unlock();
	}
	cout << "Maximum Number of Clients Overflow!!\n";
	return -1;
}

void CIOCPServer::ErrorDisplay(int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	std:: wcout << lpMsgBuf << std::endl;
	while (true);
	LocalFree(lpMsgBuf);
};

explicit CIOCPServer::CIOCPServer() { };
CIOCPServer::~CIOCPServer() { };
