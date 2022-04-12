#pragma once
#include "ExpOver.h"
#include "UserManager.h"
#include "Character.h"



namespace user_state
{
	enum eSTATE { FREE = 0, ACCEPTED = 1, IN_ROBBY = 2, IN_MATCHING_ROOM = 3, IN_GAME = 4 };
}

class cUser
{
public:
	cUser();
	cUser(unsigned short _id);
	~cUser();

	void Init(const unsigned short& _id);

	unsigned short GetID() { return m_id; }
	user_state::eSTATE GetState() { return m_state; };
	unsigned short GetRoomID() { return m_room_id; };
	short GetPrevSize() { return m_prev_size; };
		
	void SetID(const unsigned short _id) { m_id = _id; };
	void SetState(const user_state::eSTATE _state) { m_state = _state; };
	void SetRoomID(const unsigned short _room_id) { m_room_id = _room_id; };
	void SetPrevSize(const short _prev_size) { m_prev_size = _prev_size; };
	void SetSocket(const SOCKET& _socket) { m_socket = _socket; };

	void StateLock() { m_state_lock.lock(); }
	void StateUnlock() { m_state_lock.unlock(); }

	void Send(int num_bytes, void* mess);
	void Recv();


private:
	unsigned short     m_id;
	user_state::eSTATE m_state;
	mutex              m_state_lock;
	unsigned short     m_room_id;

	//mutex              m_state_lock;
public:
	//cCharacter         m_character;

private:
	SOCKET             m_socket;
	CEXP_OVER          m_recv_over;
	short 		       m_prev_size;
};