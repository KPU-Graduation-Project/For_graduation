#pragma once
#include "stdafx.h"
#include "RoomManager.h"
#include "Room.h"




void cRoomManager::Init()
{	
	m_last_id = 0;
	/*m_rooms=new cRoom[MAX_ROOM];
	this->InitRooms();*/
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
unsigned char cRoomManager::CreateRoom(const unsigned short _user_id, cUser* _p_user)
{
	m_last_id_lock.lock();
	if (m_last_id < MAX_ROOM)
	{
		unsigned short new_id = m_last_id++;
		m_last_id_lock.unlock();

		cRoom* new_room = new cRoom;
		new_room->m_room_id = new_id;
		new_room->m_state = room_state::ACCEPTED;
		new_room->m_users[user_type::HOST] = _p_user;

		m_rooms.emplace(m_last_id, new_room);
	
		cout << "Room [ " << new_id << " ] is created by User [ " << _user_id << " ]\n";
		return new_id;
	}
	m_last_id_lock.unlock();

	return MAX_ROOM;
}

//success: return true / fail: return false
bool cRoomManager::JoinRoom(const unsigned short _user_id, const unsigned short _room_id, cUser* _user_p)
{
	//data race 위험 (스테이트 락 직전에 방 파괴) -> 컨테이너 락 필요
	if (m_rooms.find(_room_id) != m_rooms.end())
	{
		m_rooms[_room_id]->StateLock();
		if ((m_rooms[_room_id]->m_state == room_state::ACCEPTED))
		{
			m_rooms[_room_id]->m_state = room_state::LOADING;
			m_rooms[_room_id]->StateUnlock();
			m_rooms[_room_id]->m_users[user_type::GUEST] = _user_p;		
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