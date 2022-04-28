#pragma once
#include "stdafx.h"
#include "RoomManager.h"
#include "Room.h"
#include "UserManager.h"
#include "User.h"

unordered_map<unsigned int, cRoom*> cRoomManager::m_rooms;
cObjectPool<cGameObject>       cRoomManager::m_object_pool(500);
cObjectPool<cCharacter>        cRoomManager::m_character_pool(MAX_USER/2); // 메모리풀 확장 확인을 위해 절반만
 cObjectPool<cBullet>           cRoomManager::m_bullet_pool(500);

void cRoomManager::Init()
{
	m_last_id = 0;
};

void cRoomManager::InitRooms()
{
	/*for (int i = 0; i < MAX_ROOM; ++i)
	{
		m_rooms[i].m_room_id = i;
		m_rooms[i].m_state = room_state::FREE;

	}*/

};

//success: return room_id / fail: return MAX_ROOM 
unsigned int cRoomManager::CreateRoom(const unsigned int _user_id)
{
	if (m_last_id < MAX_ROOM)
	{
		unsigned short new_id = m_last_id++;

		m_rooms.emplace(new_id,new cRoom( new_id, room_state::IN_ROBBY_CREATED, _user_id));

		cUserManager::m_users[_user_id]->SetRoomID(new_id);
		cUserManager::m_users[_user_id]->m_is_ready = false;
		cUserManager::m_users[_user_id]->m_selected_character = 0;

		cout << "Room [ " << new_id << " ] is created by User [ " << _user_id << " ]\n";
		return new_id;
	}

	cout << "Failed to create new room(MAX_ROOM), by user [ " << _user_id << " ]\n";
	return MAX_ROOM;
}

//success: return true / fail: return false
bool cRoomManager::JoinRoom(const unsigned int _user_id, const unsigned int _room_id)
{
	//data race 위험 (스테이트 락 직전에 방 파괴) -> 컨테이너 락 등 방법 강구해야함
	if (m_rooms.count(_room_id) != 0)
	{
		m_rooms[_room_id]->StateLock();
		if ((m_rooms[_room_id]->m_state == room_state::IN_ROBBY_CREATED))
		{
			m_rooms[_room_id]->m_state = room_state::IN_ROBBY_FULLED;
			m_rooms[_room_id]->StateUnlock();

			m_rooms[_room_id]->m_user_id[user_type::GUEST] = _user_id;
			cUserManager::m_users[_user_id]->m_is_ready = false;
			cUserManager::m_users[_user_id]->SetRoomID(_room_id);
			cUserManager::m_users[_user_id]->m_selected_character = 1;

			// 로비 생성 전까지는 READY_GAME 패킷 안받고 바로 시작
			m_rooms[_room_id]->StartGame();

			cout << "User [ " << _user_id << " ] join in Room [ " << (int)_room_id << " ] \n";

			return true;
		}
		cout << "User [ " << _user_id << " ] failed to join in Room [ " << (int)_room_id << " ] \n";
		return false;

	}
}

//UsersInRoom CRoomManager::GetPlayersID(const unsigned char _room_id) { /*return m_rooms[_room_id].m_users;*/ }



cRoomManager::cRoomManager() {};
cRoomManager::~cRoomManager() {};