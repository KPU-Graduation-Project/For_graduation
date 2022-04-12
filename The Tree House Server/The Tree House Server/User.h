#pragma once
#include "ExpOver.h"
#include "UserManager.h"
#include "Character.h"



namespace user_state
{
	enum eSTATE { FREE = 0, IN_ROBBY = 1, IN_MATCHING_ROOM = 2, IN_GAME = 3 };
}

class cUser
{
public:
	cUser();
	~cUser();

	void Init(const unsigned short& _id);

	unsigned short GetID();
	user_state::eSTATE GetState();
	unsigned short GetRoomID();
	short GetPrevSize();
		
	void SetID(const unsigned short _id);
	void SetState(const user_state::eSTATE _state);
	void SetRoomID(const unsigned short _room_id);
	void SetPrevSize(const short _prev_size);
	void SetSocket(const SOCKET& _socket);

	void Send(int num_bytes, void* mess);
	void Recv();


private:
	unsigned short     m_id;
	user_state::eSTATE m_state;
	unsigned short     m_room_id;

	//mutex              m_state_lock;
public:
	cCharacter         m_character;

private:
	SOCKET             m_socket;
	CEXP_OVER          m_recv_over;
	short 		       m_prev_size;
};