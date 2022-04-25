#include < bitset>
#include "stdafx.h"
#include "ServerNetworkSystem.h"
#include "ExpOver.h"
#include "RoomManager.h"
#include "Room.h"
#include "UserManager.h"
#include "User.h"
#include "RoomManager.h"
#include "timer.h"
#include "Character.h"


using namespace std;

cUserManager* cMainServer::m_user_manager;
cRoomManager* cMainServer::m_room_manager;
GameProcessor* cMainServer::m_game_processor;

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
	while(true)
	{
		while (true)
		{
			if (g_timer_queue.empty() == true)
				continue;
			cTimerEvent timer_event;
			g_timer_queue.try_pop(timer_event);
			if (timer_event.m_excute_time <= chrono::high_resolution_clock::now())
			{
				switch (timer_event.m_event_type)
				{
				case EVENT_TYPE::EV_SEND_WORLD_DATA:
				{

										break;
				}

				}

			}
			else
			{
				g_timer_queue.push(timer_event);
			}
		}

		m_clock->UpdateCurrentTime();
		this_thread::sleep_for(5ms);
	}
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
		case OP_SEND_WORLD_DATA:
		{
			break;
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

		// 로비 제작 전까지 접속 순서대로 Room 배정
		static int last_room_id = MAX_ROOM;
		if (last_room_id == MAX_ROOM)
			last_room_id = m_room_manager->CreateRoom(new_id);
		else
		{
			bool ret = m_room_manager->JoinRoom(new_id, last_room_id);
			if (ret == true)
				last_room_id = MAX_ROOM;
		}
		cout << "last room id:" << last_room_id << "\n";
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

void cMainServer::ProcessPacket(const unsigned short _user_id, unsigned char* _p)
{
	unsigned char packet_type = _p[1];

	switch (packet_type) {
	case CS_PACKET::CS_CREATE_ROOM:
	{
		cs_create_room_packet* packet = reinterpret_cast<cs_create_room_packet*>(_p);
		
		unsigned int new_room_id = m_room_manager->CreateRoom(_user_id);
		if (new_room_id != MAX_USER)
		{
			sc_create_room_packet send_packet;

			send_packet.size = sizeof(sc_create_room_packet);
			send_packet.type = SC_PACKET::SC_CREATE_ROOM;
			send_packet.room_id = new_room_id;
			send_packet.is_ready= m_user_manager->m_users[_user_id]->m_is_ready;
			send_packet.selected_character = m_user_manager->m_users[_user_id]->m_selected_character;

			m_user_manager->m_users[_user_id]->Send(sizeof(sc_create_room_packet), &send_packet);
		}

		break;
	}
	case CS_PACKET::CS_JOIN_ROOM:
	{
		cs_join_room_packet* packet = reinterpret_cast<cs_join_room_packet*>(_p);

		bool ret = m_room_manager->JoinRoom(_user_id, packet->room_id);
		if (ret)
		{
			// for reduce cache miss after
		
			unsigned int host_user_id= m_room_manager->m_rooms[packet->room_id]->m_user_id[user_type::HOST];
	
			{
				sc_join_room_packet send_packet;
				send_packet.size = sizeof(sc_join_room_packet);
				send_packet.type = SC_PACKET::SC_JOIN_ROOM;
				send_packet.room_id = packet->room_id;
				send_packet.is_ready = m_user_manager->m_users[_user_id]->m_is_ready;
				send_packet.selected_character = m_user_manager->m_users[_user_id]->m_selected_character;


				m_user_manager->m_users[_user_id]->Send(sizeof(sc_join_room_packet), &send_packet);
			}
			// send my data to existed user
			{
				sc_user_join_room_packet send_packet;
				send_packet.size = sizeof(sc_user_join_room_packet);
				send_packet.type = SC_PACKET::SC_USER_JOIN_ROOM;
				send_packet.id = _user_id;
				send_packet.is_ready = m_user_manager->m_users[_user_id]->m_is_ready;
				send_packet.selected_character = m_user_manager->m_users[_user_id]->m_selected_character;;

				m_user_manager->m_users[host_user_id]->Send(sizeof(sc_user_join_room_packet), &send_packet);
			}
			// get existed user's data
			{
				sc_user_join_room_packet send_packet;
				send_packet.size = sizeof(sc_user_join_room_packet);
				send_packet.type = SC_PACKET::SC_USER_JOIN_ROOM;
				send_packet.id = host_user_id;	
				send_packet.is_ready = m_user_manager->m_users[host_user_id]->m_is_ready;
				send_packet.selected_character = m_user_manager->m_users[host_user_id]->m_selected_character;

				m_user_manager->m_users[_user_id]->Send(sizeof(sc_user_join_room_packet), &send_packet);
			}		
		}
		break;
	}
	case CS_PACKET::CS_JOIN_RANDOM_ROOM: //미완
	{
		cs_join_random_room_packet* packet = reinterpret_cast<cs_join_random_room_packet*>(_p);

		// 랜덤 방 입장 기능 추가 후 수정 필요
		//bool ret = m_room_manager->JoinRoom(_user_id, packet->room_id);
		//if (ret)
		//{
		//	// for reduce cache miss after
		//	unsigned int host_user_id = m_room_manager->m_rooms[packet->room_id]->m_user_id[user_type::HOST];
		//	char host_user_selected_character = m_room_manager->m_rooms[packet->room_id]->m_user_selected_character[user_type::HOST];
		//	char guest_user_selected_character = m_room_manager->m_rooms[packet->room_id]->m_user_selected_character[user_type::GUEST];

		//	{
		//		sc_join_room_packet send_packet;
		//		send_packet.size = sizeof(sc_join_room_packet);
		//		send_packet.type = SC_PACKET::SC_JOIN_ROOM;
		//		send_packet.room_id = packet->room_id;
		//		send_packet.selected_character = guest_user_selected_character;


		//		m_user_manager->m_users[_user_id]->Send(sizeof(sc_join_room_packet), &send_packet);
		//	}

		//	// get existed user's data
		//	{
		//		sc_user_join_room_packet send_packet;
		//		send_packet.size = sizeof(sc_user_join_room_packet);
		//		send_packet.type = SC_PACKET::SC_USER_JOIN_ROOM;
		//		send_packet.id = host_user_id;
		//		send_packet.selected_character = host_user_selected_character;

		//		m_user_manager->m_users[_user_id]->Send(sizeof(sc_user_join_room_packet), &send_packet);
		//	}

		//	// send my data to existed user
		//	{
		//		sc_user_join_room_packet send_packet;
		//		send_packet.size = sizeof(sc_user_join_room_packet);
		//		send_packet.type = SC_PACKET::SC_USER_JOIN_ROOM;
		//		send_packet.id = _user_id;
		//		send_packet.selected_character = guest_user_selected_character;

		//		m_user_manager->m_users[host_user_id]->Send(sizeof(sc_user_join_room_packet), &send_packet);
		//	}
		//}
		break;
	}
	case CS_PACKET::CS_READY_GAME:
	{
		cs_ready_game_packet* packet = reinterpret_cast<cs_ready_game_packet*>(_p);

		m_user_manager->m_users[_user_id]->m_is_ready = packet->is_ready;

		{
			sc_user_ready_game_packet send_packet;
			send_packet.size = sizeof(sc_user_ready_game_packet);
			send_packet.type = SC_PACKET::SC_USER_READY_GAME;
			send_packet.id = _user_id;
			send_packet.is_ready = packet->is_ready;


			m_room_manager->m_rooms[m_user_manager->m_users[_user_id]->GetRoomID()]
				->Broadcast(sizeof(sc_user_ready_game_packet), &send_packet);
		}
		{
			// 둘 다 준비 완료되면
			// 룸, 유저 상태 변환
		}
		break;
	}
	case CS_PACKET::CS_CHANGE_SELECTED_CHARACTER:
	{
		cs_change_selected_character* packet = reinterpret_cast<cs_change_selected_character*>(_p);

		m_user_manager->m_users[_user_id]->m_selected_character = packet->selected_character;
		{
			sc_user_change_selected_character send_packet;
			send_packet.size = sizeof(sc_user_change_selected_character);
			send_packet.type = SC_PACKET::SC_USER_READY_GAME;
			send_packet.id = _user_id;
			send_packet.selected_character = packet->selected_character;


			m_room_manager->m_rooms[m_user_manager->m_users[_user_id]->GetRoomID()]
				->Broadcast(sizeof(sc_user_change_selected_character), &send_packet);
		}
		break;
	}
	case CS_PACKET::CS_START_GAME: //보류
	{
		cs_start_game_packet* packet = reinterpret_cast<cs_start_game_packet*>(_p);

		
		break;
	}
	case CS_PACKET::CS_LOADING_COMPLETE:
	{
		cs_loading_complete_packet* packet = reinterpret_cast<cs_loading_complete_packet*>(_p);

		cout << "CS_LOADING_COMPLETE from User [ " << _user_id <<" ] \n";

		m_user_manager->m_users[_user_id]->StateLock();
		//m_user_manager->m_users[_user_id]->SetState(user_state::);
		m_user_manager->m_users[_user_id]->StateUnlock();

		break;
	}
	case CS_PACKET::CS_PLAYER_DATA:
	{
		cs_player_data_packet* packet = reinterpret_cast<cs_player_data_packet*>(_p);
		
		cout << "CS_PLAYER_DATA from User [ " << _user_id <<
			" ] position " << packet->x << "," << packet->y << ", " << packet->z
			<< " // rotation " << packet->pitch << ", " << packet->yaw << "," << packet->roll << "\n";

		m_user_manager->m_users[_user_id]->m_character->SetCharacterTransform
		({ packet->x, packet->y, packet->z }, { packet->pitch, packet->yaw, packet->roll },
			{ packet->head_x, packet->head_y, packet->head_z }, { packet->head_pitch, packet->head_yaw, packet->head_roll },
			{ packet->rh_x, packet->rh_y, packet->rh_z }, { packet->rh_pitch, packet->rh_yaw, packet->rh_roll },
			{ packet->lh_x, packet->lh_y, packet->lh_z }, { packet->lh_pitch, packet->lh_yaw, packet->lh_roll });

		m_room_manager->m_rooms[m_user_manager->m_users[_user_id]->GetRoomID()]->SendOtherPlayerTransform();
		m_room_manager->m_rooms[m_user_manager->m_users[_user_id]->GetRoomID()]->SendAllObjectData();
		break;
	}
	case CS_PACKET::CS_SHOOT_BULLET:
	{

		break;
	}
	case CS_PACKET::CS_BULLET_HIT:
	{

		break;
	}


	}
}


cMainServer::cMainServer() { };
cMainServer::~cMainServer() {};