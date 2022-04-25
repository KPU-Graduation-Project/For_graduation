
#include "stdafx.h"
#include "ServerNetworkSystem.h"
#include "ExpOver.h"
#include "IOCPServer.h"
#include "UserManager.h"
#include "RoomManager.h"

using namespace std;

void ServerNetworkSystem::Init()
{
		m_user_manager = new CUserManager;
	m_user_manager->Init();
	m_room_manager = new CRoomManager;

	
}

void ServerNetworkSystem::Open()
{
	m_IOCP_server = new CIOCPServer;
	m_IOCP_server->Open();
	
	for (int i = 0; i < 1; ++i)
		m_worker_threads.emplace_back(std::thread(&ServerNetworkSystem::WorkerThread, this));
	for (auto& th : m_worker_threads)
		th.join();

};


void ServerNetworkSystem::WorkerThread()
{
	for (;;) 
	{
		DWORD num_byte;
		LONG64 iocp_key;
		WSAOVERLAPPED* p_over;
		BOOL ret = GetQueuedCompletionStatus(m_IOCP_server->m_h_iocp, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
		std::cout << "GQCS returned.\n";
		int client_id = static_cast<int>(iocp_key);
		CEXP_OVER* exp_over = reinterpret_cast<CEXP_OVER*>(p_over);

		if (FALSE == ret) {
			int err_no = WSAGetLastError();
			cout << "GQCS Error : ";
			if (err_no == ERROR_INVALID_HANDLE)
			{
				cout << "handle_error" << endl;
			}
			//ErrorDisplay(err_no);
			cout << err_no << endl;
			WCHAR* lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, err_no,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf, 0, 0);
			std::wcout << lpMsgBuf << std::endl;
			while (true);
			LocalFree(lpMsgBuf);

			cout << endl;
			//DoDisconnect(client_id);
			if (exp_over->m_comp_op == OP_SEND)
				delete exp_over;
			continue;
		}
		switch (exp_over->m_comp_op) {
		case OP_RECV: {
			Recv(exp_over, client_id, num_byte);
			break;
		}
		case OP_SEND: {
			/*if (num_byte != exp_over->m_wsa_buf.len) {
				DoDisconnect(client_id);
				delete exp_over;
				continue;
			}
			DoSend(exp_over);*/
			delete exp_over;
			break;
		}
		case OP_ACCEPT: {
			Accept(exp_over);
			break;
		}
		}
	}

	while (true) {};
}

void ServerNetworkSystem::Accept(CEXP_OVER* exp_over)
{
	unsigned int new_id = m_user_manager->GetNewID();
	if (new_id == MAX_USER) {/*server full*/}
	else
	{
		cout << "Accept Completed.\n";
		SOCKET c_socket = *(reinterpret_cast<SOCKET*>(exp_over->m_net_buf));
		m_user_manager->AddNewUser(new_id, c_socket);

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), m_IOCP_server->m_h_iocp, new_id, 0);
		m_user_manager->UserRecv(new_id);

		ZeroMemory(&exp_over->m_wsa_over, sizeof(exp_over->m_wsa_over));
		c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
		*(reinterpret_cast<SOCKET*>(exp_over->m_net_buf)) = c_socket;
		AcceptEx(m_IOCP_server->m_listen_socket, c_socket, exp_over->m_net_buf + 8, 0, sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16, NULL, &exp_over->m_wsa_over);

		cout << "conneted " << endl;
	}
}

void ServerNetworkSystem::Send(CEXP_OVER* exp_over)
{

}

void ServerNetworkSystem::Recv(CEXP_OVER* exp_over, const unsigned int _user_id, const DWORD num_byte)
{
	int remain_data = num_byte + m_user_manager->GetPrevSize(_user_id);
	unsigned char* packet_start = exp_over->m_net_buf;
	int packet_size = packet_start[0];

	while (packet_size <= remain_data) {
		ProcessPacket(_user_id, packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}

	if (0 < remain_data) {
		m_user_manager->SetPrevSize(_user_id, remain_data);
		memcpy(&exp_over->m_net_buf, packet_start, remain_data);
	}
	m_user_manager->UserRecv(_user_id);
}

void ServerNetworkSystem::ProcessPacket(const unsigned int _user_id, unsigned char* p)
{
	unsigned char packet_type = p[1];
	
	CS_PLAYER_DATA* packet = reinterpret_cast<CS_PLAYER_DATA*>(p);
	cout << packet->player_position.x << ", " << packet->player_position.y << ", " <<
		packet->player_position.z << endl;

	cout << packet->player_rotation.x << ", " << packet->player_rotation.y << ", " <<
		packet->player_rotation.z << endl;

	switch (packet_type) {
		
	case 1:
	{

		break;
	}

	}

}

ServerNetworkSystem::ServerNetworkSystem() { };
ServerNetworkSystem::~ServerNetworkSystem() {};