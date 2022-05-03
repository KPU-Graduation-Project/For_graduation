#pragma once
#include "ExpOver.h"
#include "Character.h"
#include "Room.h"


namespace user_state
{
	enum eSTATE { FREE = 0, ACCEPTED = 1, IN_ROBBY = 2, IN_ROOM = 3,IN_LOADING=4, IN_GAME = 5 };
}

class cUser
{
public:
	cUser();
	cUser(unsigned int _id);
	~cUser();

	void Init();

	unsigned short GetID() { return m_id; }
	user_state::eSTATE GetState() { return m_state; };
	RoomRef GetRoom() { return m_room; };
	unsigned short GetPrevSize() { return m_prev_size; };
		
	void SetID(const unsigned int _id) { m_id = _id; };
	void SetState(const user_state::eSTATE _state) { m_state = _state; };
	void SetRoom(RoomRef _room) { m_room = _room; };
	void SetPrevSize(const unsigned short _prev_size) { m_prev_size = _prev_size; };
	void SetSocket(const SOCKET& _socket) { m_socket = _socket; };

	void StateLock() { EnterCriticalSection(&m_state_cs); }
	void StateUnlock() { LeaveCriticalSection(&m_state_cs); }

	void Send(int num_bytes, void* mess);
	void Recv();


private:
	unsigned int       m_id;
	user_state::eSTATE m_state;
	CRITICAL_SECTION   m_state_cs;
	//unsigned int       m_room_id;

public:
	// Use when the user in room
	bool               m_is_ready;
	char               m_selected_character;	
	cCharacter*        m_character;
	RoomRef             m_room;

private:
	SOCKET             m_socket;
	cExpOver           m_recv_over;
	unsigned short 	   m_prev_size;
};