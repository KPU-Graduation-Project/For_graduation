#pragma once
#include "stdafx.h"
#include "RoomManager.h"
#include "Room.h"




void cRoomManager::Init()
{	
	m_rooms=new cRoom[MAX_ROOM];
	this->InitRooms();
};

void cRoomManager::InitRooms()
{
	for (int i = 0; i < MAX_ROOM; ++i)
	{
		m_rooms[i].m_room_id = i;
		m_rooms[i].m_state = room_state::FREE;

	}
};

//success: return room_id / fail: return MAX_ROOM 
unsigned char cRoomManager::CreateRoom(const unsigned short _user_id, cUser* _p_user)
{
	//빈 방 탐색 알고리즘 추가 필요
	for (int i = 0; i < MAX_ROOM; ++i)
	{
		if (m_rooms[i].m_state == room_state::FREE);
		{
			cout << "Room [ "<<i<<" ] is created by User [ "<<_user_id<<" ]\n";
			m_rooms[i].m_state = room_state::IN_ROBBY;
			m_rooms[i].m_users[user_type::HOST] = _p_user;

			return i;
		}
	}

	return MAX_ROOM;
}

//success: return true / fail: return false
bool cRoomManager::JoinRoom(const unsigned short _user_id, const unsigned char _room_id, cUser* _user_p)
{
	if ((m_rooms[_room_id].m_state == room_state::IN_ROBBY))
	{
		cout << "User [ " << _user_id << " ] join in Room [ " << (int)_room_id << " ] \n";
		m_rooms[_room_id].m_users[user_type::GUEST] = _user_p;
		m_rooms[_room_id].m_state = room_state::IN_GAME;
		m_rooms[_room_id].StartGame();
		//m_rooms[_room_id].m_users.user1= _user_id;
		return true;
	}
	cout << "User [ " << _user_id << " ] failed to join in Room [ " << (int)_room_id << " ] \n";
	return false;
}

//UsersInRoom CRoomManager::GetPlayersID(const unsigned char _room_id) { /*return m_rooms[_room_id].m_users;*/ }


cRoomManager::cRoomManager() {};
cRoomManager::~cRoomManager() {};