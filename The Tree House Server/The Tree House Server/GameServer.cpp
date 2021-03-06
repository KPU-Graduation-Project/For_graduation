#include < bitset>
#include "stdafx.h"
#include "GameServer.h"
#include "ExpOver.h"
#include "RoomManager.h"
#include "Room.h"
#include "UserManager.h"
#include "User.h"
#include "RoomManager.h"
#include "timer.h"
#include "Character.h"
#include <atomic>


using namespace std;

cUserManager* cGameServer::m_user_manager;

void cGameServer::Init()
{

	m_user_manager = new cUserManager;
	m_user_manager->Init();
	m_clock = new cTimer;
	m_clock->Init();
}

void cGameServer::Start()
{
	cIOCPBase::StartServer();

	for (int i = 0; i < 4; ++i)
		m_worker_threads.emplace_back(std::thread(&cGameServer::WorkerThread, this));
	m_timer_thread = thread{ &cGameServer::TimerThread,this };

	for (auto& th : m_worker_threads)
		th.join();
	m_timer_thread.join();

};

void cGameServer::TimerThread()
{
	//최초 1회
	cTimerEvent new_event;
	new_event.m_event_type = EVENT_TYPE::TICK_EVENT;
	new_event.m_excute_time = chrono::system_clock::now() + chrono::milliseconds(1000 / 60);
	g_timer_queue.push(new_event);

	while(true)
	{
		while (true)
		{
			cTimerEvent timer_event;
			if (g_timer_queue.try_pop(timer_event) == false)
				break;
				
			
			if (timer_event.m_excute_time <= chrono::system_clock::now())
			{
				switch (timer_event.m_event_type)
				{
				case EVENT_TYPE::TICK_EVENT:
				{
					//m_clock->UpdateCurrentTime();

					// 오브젝트풀로 수정 필요
					cExpOver* over = new cExpOver;
					over->m_comp_op = OP_TICK_EVENT;
					// 특정 유저에게 보내는 것이 아닌 경우 KEY 어떻게?
					PostQueuedCompletionStatus(m_h_IOCP, 1, NULL, &over->m_wsa_over);

					timer_event.m_excute_time += chrono::milliseconds(1000 / 60);
					g_timer_queue.push(timer_event);

					break;
				}
				case EVENT_TYPE::PROGRESS_BEHAVIOR_EVENT:
				{
					// 오브젝트풀로 수정 필요
					cExpOver* over = new cExpOver;
					over->m_comp_op = OP_PROGRESS_BEHAVIOR;
					PostQueuedCompletionStatus(m_h_IOCP,1, timer_event.m_obj_id, &over->m_wsa_over);

						break;
				}	
				}
			}
			else
			{
								g_timer_queue.push(timer_event);
				break;
			}
			
		}
		this_thread::sleep_for(5ms);
	}
}


void cGameServer::WorkerThread()
{
	while (true)
	{
		DWORD num_byte;
		LONG64 iocp_key;
		WSAOVERLAPPED* p_over;
		BOOL ret = GetQueuedCompletionStatus(cIOCPBase::m_h_IOCP, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
		//std::cout << "GQCS returned.\n";
		int client_id = static_cast<int>(iocp_key);
		cExpOver* exp_over = reinterpret_cast<cExpOver*>(p_over);

		if (FALSE == ret) {
			int err_no = WSAGetLastError();
			cout << "Client ID: " << client_id << " GQCS Error : ";
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

			LocalFree(lpMsgBuf);

			cout << endl;
			Disconnect(client_id);
			if (exp_over->m_comp_op == OP_SEND)
				delete exp_over;
			continue;
		}


		switch (exp_over->m_comp_op) {
		case OP_RECV: {
			//cout << "recv called: "<<num_byte<<"byte \n";
			Recv(exp_over, client_id, num_byte);

			break;
		}
		case OP_SEND: {
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
			break;
		}
		case OP_TICK_EVENT:
		{
			for (const auto& room : g_room_manager->m_rooms)
			{
				room.second->StateLock();
				if (room.second->m_state==ROOM_STATE::INGAME )
				{
					room.second->StateUnlock();
					room.second->Update(m_clock->GetDeltaTimeSecond());
					room.second->SendPlayerData();
					room.second->SendAllObjectData();
				}
				else 	room.second->StateUnlock();

			}
			delete exp_over;
			break;
		}
		case OP_PROGRESS_BEHAVIOR:
		{
			// 수정 필요 ( GQCS key를 세션레퍼런스로 변경 예정)
			for (const auto& room : g_room_manager->m_rooms)
			{
				room.second->StateLock();
				if (room.second->m_state == ROOM_STATE::INGAME)
				{
					room.second->StateUnlock();
					auto object = room.second->m_game_objects.find(client_id);
					if (object != room.second->m_game_objects.end())
					{
						switch (object->second->m_object_type)
						{
						case OBJECT_TYPE::PLAY_ZONE_TARGET:
						{
							reinterpret_cast<cPlayZoneTarget*>(object->second)->ProgressNextBehavior();
							break;
						}
						case OBJECT_TYPE::TARGET_DOLL:
						{
							//reinterpret_cast<cTargetDoll*>(object->second)->ProgressNextBehavior();
							break;
						}
						}
					}
				}
				else 	room.second->StateUnlock();
				delete exp_over;
				break;
			}
		}
		}
	}

	while (true) { cout << "Thread Loop End" << endl; };
}

void cGameServer::Accept(cExpOver* exp_over)
{

	static std::atomic<unsigned int> id_generator = 1;

	unsigned int new_id = id_generator++;

	if (new_id == MAX_USER) {/*server full*/}
	else
	{
		m_user_manager->m_users.emplace(new_id, new cUser(new_id));
		m_user_manager->m_users[new_id]->SetState(user_state::ACCEPTED);

		SOCKET c_socket = *(reinterpret_cast<SOCKET*>(exp_over->m_net_buf));
		m_user_manager->m_users[new_id]->SetSocket(c_socket);

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), cIOCPBase::m_h_IOCP, new_id, 0);
		m_user_manager->m_users[new_id]->Recv();

		ZeroMemory(&exp_over->m_wsa_over, sizeof(exp_over->m_wsa_over));
		c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
		*(reinterpret_cast<SOCKET*>(exp_over->m_net_buf)) = c_socket;
		AcceptEx(cIOCPBase::m_listen_socket, c_socket, exp_over->m_net_buf + 8, 0, sizeof(SOCKADDR_IN) + 16,
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
		{

			last_room_id = g_room_manager->CreateRoom(m_user_manager->m_users[new_id]);
			cout << "last room id:" << last_room_id << "\n";
		}
		else
		{
			bool ret = g_room_manager->JoinRoom(m_user_manager->m_users[new_id], last_room_id);
			cout << "last room id:" << last_room_id << "\n";
			if (ret == true)
				last_room_id = MAX_ROOM;
		}
	
	}
	
}

void cGameServer::Disconnect(const unsigned int _user_id)
{
	// 방에 들어가 있으면 
	if (m_user_manager->m_users[_user_id]->m_room != nullptr)
	{
		m_user_manager->m_users[_user_id]->m_room;
	}
}

void cGameServer::Send(cExpOver* exp_over)
{

}

void cGameServer::Recv(cExpOver* exp_over, const unsigned int _user_id, const DWORD num_byte)
{

	unsigned int remain_data = num_byte + m_user_manager->m_users[_user_id]->GetPrevSize();
	unsigned char* packet_start = exp_over->m_net_buf;
	unsigned int packet_size = packet_start[0];

	//cout << "-------Recv-------\n";
	//cout << "packet_size - :" << packet_size << "  //  remain_data - :" << remain_data << endl;
	while (packet_size <= remain_data) {
		ProcessPacket(_user_id, packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		
		if (remain_data > 0)
		{
			packet_size = packet_start[0];			
		}
		else break;
	}

	if (0 < remain_data) {
		m_user_manager->m_users[_user_id]->SetPrevSize(remain_data);
		memcpy(&exp_over->m_net_buf, packet_start, remain_data);
	}
	m_user_manager->m_users[_user_id]->Recv();

	//cout << "----------------\n";
}

void cGameServer::ProcessPacket(const unsigned int _user_id, unsigned char* _recv_pkt)
{
	unsigned char packet_type = _recv_pkt[1];

	switch (packet_type) {
	case CS_PACKET::CS_CREATE_ROOM:
	{
		cs_create_room_packet* packet = reinterpret_cast<cs_create_room_packet*>(_recv_pkt);
		
		unsigned int new_room_id = g_room_manager->CreateRoom(m_user_manager->m_users[_user_id]);
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
		cs_join_room_packet* packet = reinterpret_cast<cs_join_room_packet*>(_recv_pkt);

		bool ret = g_room_manager->JoinRoom(m_user_manager->m_users[_user_id], packet->room_id);
		if (ret)
		{
			// for reduce cache miss after
		
			unsigned int host_user_id= g_room_manager->m_rooms[packet->room_id]->m_users[HOST]->GetID();
	
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
		cs_join_random_room_packet* packet = reinterpret_cast<cs_join_random_room_packet*>(_recv_pkt);

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
		cs_ready_game_packet* packet = reinterpret_cast<cs_ready_game_packet*>(_recv_pkt);

		m_user_manager->m_users[_user_id]->m_is_ready = packet->is_ready;

		{
			sc_user_ready_game_packet send_packet;
			send_packet.size = sizeof(sc_user_ready_game_packet);
			send_packet.type = SC_PACKET::SC_USER_READY_GAME;
			send_packet.id = _user_id;
			send_packet.is_ready = packet->is_ready;


			m_user_manager->m_users[_user_id]->GetRoom()
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
		cs_change_selected_character* packet = reinterpret_cast<cs_change_selected_character*>(_recv_pkt);

		m_user_manager->m_users[_user_id]->m_selected_character = packet->selected_character;
		{
			sc_user_change_selected_character send_packet;
			send_packet.size = sizeof(sc_user_change_selected_character);
			send_packet.type = SC_PACKET::SC_USER_READY_GAME;
			send_packet.id = _user_id;
			send_packet.selected_character = packet->selected_character;


			m_user_manager->m_users[_user_id]->GetRoom()
				->Broadcast(sizeof(sc_user_change_selected_character), &send_packet);
		}
		break;
	}
	case CS_PACKET::CS_START_GAME: //보류
	{
		cs_start_game_packet* packet = reinterpret_cast<cs_start_game_packet*>(_recv_pkt);

		
		break;
	}
	case CS_PACKET::CS_LOADING_COMPLETE:
	{
		cs_loading_complete_packet* packet = reinterpret_cast<cs_loading_complete_packet*>(_recv_pkt);

		cout << "CS_LOADING_COMPLETE from User [ " << _user_id <<" ] \n";

		m_user_manager->m_users[_user_id]->StateLock();
		//m_user_manager->m_users[_user_id]->SetState(user_state::);
		m_user_manager->m_users[_user_id]->StateUnlock();

		break;
	}
	case CS_PACKET::CS_PLAYER_DATA:
	{
		cs_player_data_packet* packet = reinterpret_cast<cs_player_data_packet*>(_recv_pkt);
		
		
		
		/*cout << "CS_PLAYER_DATA from User [ " << _user_id <<
			" ] position " << packet->x << "," << packet->y << ", " << packet->z
			<< " // rotation " << packet->pitch << ", " << packet->yaw << "," << packet->roll << "\n";*/

		m_user_manager->m_users[_user_id]->m_character->SetCharacterTransform
		({ packet->x, packet->y, packet->z }, { packet->pitch, packet->yaw, packet->roll },
			{ packet->head_x, packet->head_y, packet->head_z }, { packet->head_pitch, packet->head_yaw, packet->head_roll },
			{ packet->rh_x, packet->rh_y, packet->rh_z }, { packet->rh_pitch, packet->rh_yaw, packet->rh_roll },
			{ packet->lh_x, packet->lh_y, packet->lh_z }, { packet->lh_pitch, packet->lh_yaw, packet->lh_roll });

		//g_room_manager.m_rooms[m_user_manager->m_users[_user_id]->GetRoomID()]->SendOtherPlayerTransform(_user_id);
		//m_room_manager->m_rooms[m_user_manager->m_users[_user_id]->GetRoomID()]->SendAllObjectData();
		break;
	}
	case CS_PACKET::CS_SHOOT_BULLET:
	{
		cs_shoot_bullet_packet* packet = reinterpret_cast<cs_shoot_bullet_packet*>(_recv_pkt);
		
		cout << "CS_SHOOT_BULLET from User [ " << _user_id <<
			" ] position " << packet->x << "," << packet->y << ", " << packet->z
			<< " // rotation " << packet->pitch << ", " << packet->yaw << "," << packet->roll << "\n";

		m_user_manager->m_users[_user_id]->GetRoom()->ShootBullet(m_user_manager->m_users[_user_id], { packet->x, packet->y, packet->z }, { packet->pitch, packet->yaw, packet->roll });

		break;
	}
	case CS_PACKET::CS_BULLET_HIT:
	{
		cs_bullet_hit_packet* packet= reinterpret_cast<cs_bullet_hit_packet*>(_recv_pkt);

		
			
		cout << "CS_BULLET_HIT from User [ " << _user_id <<
			" ] bullet [ "<< packet->bullet_id <<" ] to [ "<< packet->object_id<<" ], position " << packet->x << "," << packet->y << ", " << packet->z
			<< " // rotation " << packet->pitch << ", " << packet->yaw << "," << packet->roll << "\n";

		m_user_manager->m_users[_user_id]->GetRoom()->BulletHit(packet->bullet_id, packet->object_id, { packet->x, packet->y, packet->z }, { packet->pitch, packet->yaw, packet->roll });
			break;
	}
	case CS_PACKET::CS_OBJECT_UPDATE:
	{
		cs_object_update_packet* packet= reinterpret_cast<cs_object_update_packet*>(_recv_pkt);

		cout << "CS_OBJECT_UPDATE from User [ " << _user_id << " ] object [ " << packet->object_id <<" , direction: "<<packet->direction << "\n";

		sc_object_update_packet send_packet;

		send_packet.size = sizeof(sc_object_update_packet);
		send_packet.type = SC_PACKET::SC_OBJECT_UPDATE;
		send_packet.object_id = packet->object_id;
		send_packet.direction = packet->direction;

		m_user_manager->m_users[_user_id]->GetRoom()->Broadcast(sizeof(send_packet), &send_packet);
		break;
	}


	}
}

cGameServer::cGameServer() { };
cGameServer::~cGameServer() {};