#include < bitset>
#include "stdafx.h"
#include "ServerNetworkSystem.h"
#include "ExpOver.h"
#include "UserManager.h"
#include "User.h"
#include "RoomManager.h"
#include "timer.h"
#include "Character.h"
#include "Room.h"

using namespace std;




void cMainServer::Init()
{
	m_room_manager = new cRoomManager;
	m_room_manager->Init();
	m_user_manager = new cUserManager;
	m_user_manager->Init();
	m_clock = new cTimer;
	m_clock->Init();
}

void cMainServer::Start()
{
	cIOCPServer::StartServer();

	for (int i = 0; i < 1; ++i)
		m_worker_threads.emplace_back(std::thread(&cMainServer::WorkerThread, this));
	//m_timer_thread = thread{ &ServerNetworkSystem::TimerThread,this };

	for (auto& th : m_worker_threads)
		th.join();
	m_timer_thread.join();

};

void cMainServer::TimerThread()
{
	//while(true)
	//{
	//	while (true)
	//	{
	//		if (m_timer_queue.empty() == true)
	//			continue;
	//		cTimerEvent timer_event;
	//		m_timer_queue.try_pop(timer_event);
	//		//if (ev.start_time <= chrono::system_clock::now())
	//	}

	//	m_clock->UpdateCurrentTime();
	//	this_thread::sleep_for(5ms);
	//}
}

void cMainServer::WorkerThread()
{
	while (true)
	{
		DWORD num_byte;
		LONG64 iocp_key;
		WSAOVERLAPPED* p_over;
		BOOL ret = GetQueuedCompletionStatus(cIOCPServer::m_h_IOCP, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
		//std::cout << "GQCS returned.\n";
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
			//cout << "recv called\n";
			Recv(exp_over, client_id, num_byte); 
			
			//SendGameState();

			break;
		}
		case OP_SEND: {
			//cout << "send called\n";
			if (num_byte != exp_over->m_wsa_buf.len) {
				cout << "send error" << endl;
				//DoDisconnect(client_id);
				delete exp_over;
				continue;
			}
			//DoSend(exp_over);
			delete exp_over;
			break;
		}
		case OP_ACCEPT: {
			//cout << "accpet called\n";
			Accept(exp_over);
			
		}
		}

	}

	while (true) { cout << "Thread Loop End" << endl; };
}

void cMainServer::Accept(CEXP_OVER* exp_over)
{
	unsigned short new_id = m_user_manager->GetNewID();

	if (new_id == MAX_USER) {/*server full*/}
	else
	{
		SOCKET c_socket = *(reinterpret_cast<SOCKET*>(exp_over->m_net_buf));
		m_user_manager->m_users[new_id]->SetSocket(c_socket);

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), cIOCPServer::m_h_IOCP, new_id, 0);
		m_user_manager->m_users[new_id]->Recv();

		ZeroMemory(&exp_over->m_wsa_over, sizeof(exp_over->m_wsa_over));
		c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
		*(reinterpret_cast<SOCKET*>(exp_over->m_net_buf)) = c_socket;
		AcceptEx(cIOCPServer::m_listen_socket, c_socket, exp_over->m_net_buf + 8, 0, sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16, NULL, &exp_over->m_wsa_over);
				
		sc_loginok_packet packet;
		packet.size = sizeof(sc_loginok_packet);
		packet.type = SC_PACKET::SC_LOGINOK;
		packet.id = new_id;
		m_user_manager->m_users[new_id]->Send(sizeof(packet), &packet);
		m_user_manager->m_users[new_id]->SetState(user_state::IN_ROBBY);
		cout << "New User [ " << new_id << " ] is accepte\n";

		if (new_id % 2 == 0)
		{
			char ret = m_room_manager->CreateRoom(new_id,m_user_manager->m_users[new_id]);
			if (ret != MAX_ROOM)
			{
				m_user_manager->m_users[new_id]->SetRoomID(ret);
				cout << "User [ " << new_id << " ] RoomID:  " << m_user_manager->m_users[new_id]->GetRoomID();

		
					}
		}
		else
		{
			bool ret = m_room_manager->JoinRoom(new_id, 0,  m_user_manager->m_users[new_id]);
			if (ret == true)
			{
				m_user_manager->m_users[new_id]->SetRoomID(0);
				cout << "User [ " << new_id << " ] RoomID:  " << m_user_manager->m_users[new_id]->GetRoomID();
	
			
			}
		}		
	}
}

void cMainServer::Send(CEXP_OVER* exp_over)
{

}

void cMainServer::Recv(CEXP_OVER* exp_over, const unsigned short _user_id, const DWORD num_byte)
{

	int remain_data = num_byte + m_user_manager->m_users[_user_id]->GetPrevSize();
	unsigned char* packet_start = exp_over->m_net_buf;
	int packet_size = packet_start[0];

	cout << "packet_size - :" << packet_size << "  //  remain_data - :" << remain_data << endl;
	while (packet_size <= remain_data) {
		ProcessPacket(_user_id, packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}

	if (0 < remain_data) {
		m_user_manager->m_users[_user_id]->SetPrevSize(remain_data);
		memcpy(&exp_over->m_net_buf, packet_start, remain_data);
	}
	m_user_manager->m_users[_user_id]->Recv();
}

void cMainServer::Disconnect()
{


}

void cMainServer::ProcessPacket(const unsigned short _user_id, unsigned char* p)
{	
	unsigned char packet_type = p[1];

	switch (packet_type) {

	case CS_PACKET::CS_PLAYER_DATA:
	{
		cs_player_data_packet* packet = reinterpret_cast<cs_player_data_packet*>(p);
		
		cout << "CS_PLAYER_DATA from User [ " << _user_id <<
			" ] position " <<packet->x << "," << packet->y << ", " << packet->z
			<< " // rotation " << packet->pitch << ", " << packet->yaw << "," << packet->roll << "\n";


		//캐릭터에 정보 저장 필요
		/*m_room_manager->m_rooms[m_user_manager->m_users[_user_id]->GetRoomID()].
			m_character.SetTransform(
			packet->x, packet->y, packet->z,
			 packet->pitch, packet->yaw, packet->roll);*/
				

		m_room_manager->m_rooms[m_user_manager->m_users[_user_id]->GetRoomID()]->SendPlayerTransform();

		break;
	}
	}
}


cMainServer::cMainServer() { };
cMainServer::~cMainServer() {};